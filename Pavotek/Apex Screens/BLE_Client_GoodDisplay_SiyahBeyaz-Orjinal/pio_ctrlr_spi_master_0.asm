; Interface with PIO
.flag SCLK,         P0.5            ; Port connected to SPI clock line (PIO9)
.flag MOSI,         P0.7            ; Port connected to Master Out Slave In (PIO10)
.flag MISO,         P1.1            ; Port connected to Master In Slave Out (PIO11)

; CSR10xx specific SFRs to control port direction
; (not used directly but defined for reference)
.equ P0_DRIVE_EN,   0c0h            ; Port 0 bit direction
.equ P1_DRIVE_EN,   0c8h            ; Port 1 bit direction
.equ P2_DRIVE_EN,   0d8h            ; Port 2 bit direction
.equ P3_DRIVE_EN,   0e8h            ; Port 3 bit direction

; Special Function Register definitions
.equ WAKE_XAP,      9eh             ; CSR10xx specific SFR to interrupt XAP

; Interface with XAP (address locations 0x40 to 0x7f inclusive)

; Configuration registers
; These are aligned on 16-bit boundaries so the XAP can access them without
; having to pack octets or worry about masking. LSB is the "even" byte.
.equ XFER_SIZE,     40h             ; Number of bytes to transfer.
            ; An interrupt will be raised on the XAP when the transfer is
            ; complete. The XAP application is responsible for initialising this
            ; value before triggering PIO Controller External Interrupt 0
.equ INT_ACK,       46h             ; Interrupt acknowledgement flag
            ; The XAP interrupt is level triggered so several interrupts may be
            ; generated before the PIO Controller is able to clear the WAKE_XAP
            ; signal. To accommodate this, use a flag (INT_ACK) set by the PIO
            ; Controller and cleared by the XAP to handshake the interrupt.
.equ BYTE_DELAY,    48h             ; Count to affect inter octet delay
            ; Set a count to introduce additional delay between octets.
            ; Setting this count to 0x00 will introduce maximum delay (32.125us)
            ; while setting it to 0x01 will introduce minimum delay (250ns)

; Buffers used to exchange data with the XAP.
; Data read from the SPI slave is stored in the READ_BUFFER
; Data to be sent to the SPI slave is stored in the WRITE_BUFFER
; Buffer memory runs from 50h to 7fh inclusive.
; The two buffers must be the same size.
.equ READ_BUFFER,   50h             ; Start of read buffer
.equ WRITE_BUFFER,  68h             ; Start of write buffer
.equ BUFFER_SIZE,   18h             ; Size of each buffer in bytes

; ***********************
; Setup interrupt vectors
; ***********************

            ; This ISR is called when the PIO controller is powered up or reset.
            .org    0000h           ; Reset interrupt vector
            ljmp    MAIN            ; Jump to start of main routine
 
            ; This ISR is called when the XAP sends an interrupt.
            ; The XAP interrupt is only available on external interrupt 0.
            .org    0003h           ; External interrupt 0 vector
            sjmp    XFER_INIT       ; Commence next transfer

            ; No subsequent interrupt vectors in use so no need to pad out the
            ; rest of the interrupt vector table.

; *********************
; Main code begins here
; *********************

MAIN:       ; Set all outputs to open collector
            mov     P0, #11111111b  ; set to all inputs/high outputs
            mov     P1, #11111111b  ; set to all inputs/high outputs
            mov     P2, #11111111b  ; set to all inputs/high outputs
            mov     P3, #11111111b  ; set to all inputs/high outputs
            
            ; Configure MOSI and SCLK as outputs. This requires setting the
            ; appropriate bits in the drive enable registers. The drive enable
            ; registers are write only, so the entire register contents need to
            ; be prepared before writing.

            ; The drive enable register address is calculated from the port
            ; address. Ports 0 to 3 are at address 80, 90, a0 and b0
            ; respectively. The drive enable registers are at address c0, c8, d8
            ; and e8 respectively. To convert from the port address to the drive
            ; enable address, add 40h to the port address and subtract 8 if
            ; either of bits 4 or 5 are set.
            
            ; Each of the 8 bits in the drive enable register represents one of
            ; the I/O lines connected to that port. Which bit to set may be
            ; calculated from the port bit number.
            
            ; Check whether MOSI and SCLK are on the same port by comparing the
            ; top nibble.
            mov     A, #(MOSI & 0f0h)
            cjne    A, #(SCLK & 0f0h), DIFF_PORT

            ; MOSI and SCLK signals are on the same port, so only one drive
            ; enable register needs to be set.
SAME_PORT:  mov     (MOSI & 0f0h) + 40h - (((MOSI & 10h) >> 1) | ((MOSI & 20h) >> 2)), #((1 << (MOSI & 07h)) | (1 << (SCLK & 07h)))
            sjmp    INIT_BUFFER

            ; MOSI and SCLK signals are on different ports, so set each drive
            ; enable register separately.
DIFF_PORT:  mov     (MOSI & 0f0h) + 40h - (((MOSI & 10h) >> 1) | ((MOSI & 20h) >> 2)), #(1 << (MOSI & 07h))
            mov     (SCLK & 0f0h) + 40h - (((SCLK & 10h) >> 1) | ((SCLK & 20h) >> 2)), #(1 << (SCLK & 07h))

            ; Initialise buffer registers
INIT_BUFFER:mov     R0, #READ_BUFFER; R0 points to next address to read byte
                                    ; into
            mov     R1, #WRITE_BUFFER; R1 points to next address to write byte
                                    ; from
            mov     R2, #00h        ; Reset byte counter
            mov     R3, #BUFFER_SIZE; Reset buffer counter
            
            ; Clear interrupt acknowledgement flag
            mov     INT_ACK, #0
            mov     INT_ACK+1, #0   ; Clear MSB (so XAP can read as 16-bit)
            
            ; Initialise transfer length register
            mov     XFER_SIZE, #0
            mov     XFER_SIZE+1, #0 ; Clear MSB (so XAP can read as 16-bit)

            ; Set up the output lines for idle state (SPI mode 0)
            clr     SCLK            ; Clock signal idle

            ; Initialise interrupts
            setb    IT0             ; Make external interrupt 0 edge triggered
                                    ; i.e. interrupt is generated when the
                                    ; signal goes from high to low
            clr     IE0             ; Reset external interrupt 0 state
                                    ; (otherwise a spurious interrupt may be
                                    ; received)
            mov     IE, #81h        ; Enable external interrupt 0

            ; SPI master loop
IDLE:       sjmp    *               ; Busy loop. Wait for EX0 interrupt

            ; Begin next transfer
XFER_INIT:  mov     A, XFER_SIZE    ; Sanity check - if there are no bytes
            jz      XFER_EXIT       ; to transfer exit the loop immediately
            mov     R6, A           ; Initialise byte loop counter

            ; Transfer next byte
XFER_BYTE:  mov     R7, #08h        ; Initialise bit loop counter
            mov     A, @R1          ; Move next byte to send into shift
                                    ; register
            rlc     A               ; Load Carry flag with shift register msb
XFER_LOOP:  mov     MOSI, C         ; Put shift register msb onto MOSI line
            nop                     ; Delay by 1 clock cycle
            nop                     ; Delay by 1 clock cycle
            setb    SCLK            ; Assert SCLK
            mov     C, MISO         ; Copy MISO into Carry flag
            rlc     A               ; Shift the accumulator left copying the bit
                                    ; received from the slave into the lsb of
                                    ; the accumulator and the next bit to be
                                    ; sent into the Carry flag 
            nop                     ; Delay by 1 clock cycle
            nop                     ; Delay by 1 clock cycle
            clr     SCLK            ; De-assert SCLK
            djnz    R7, XFER_LOOP   ; Next iteration
            mov     @R0, A          ; Store the most recently received byte

            ; Perform inter-byte delay
            mov     R5, BYTE_DELAY  ; Load the desired delay
            djnz    R5, *           ; Wait for the desired delay

            ; Check whether buffers have wrapped and reset if so
            djnz    R3, SKIP_RESET  ; Check whether the buffer has wrapped
            mov     R0, #READ_BUFFER; Reset read buffer pointer
            mov     R1, #WRITE_BUFFER;Reset write buffer pointer
            mov     R3, #BUFFER_SIZE; Reset buffer counter
            sjmp    CHECK_DONE      ; Skip to transfer completion test
            
            ; Advance buffer pointers
SKIP_RESET: inc     R0              ; Point to next read buffer location
            inc     R1              ; Point to next write buffer location
            
            ; Check whether the required number of bytes have been transferred
CHECK_DONE: djnz    R6, XFER_BYTE   ; Next iteration

            ; SPI transfer complete. Trigger a XAP interrupt then wait for next
            ; transfer to begin
XFER_EXIT:  setb    MOSI            ; Reset MOSI to reduce current consumption
            mov     INT_ACK, #1     ; Set acknowledgement flag
            mov     WAKE_XAP, #1    ; Trigger XAP interrupt
            mov     WAKE_XAP, #0    ; Clear XAP interrupt
            clr     A               ; Wait for interrupt...
            cjne    A, INT_ACK, *   ; ...to be acknowledged
            reti                    ; Return from ISR
