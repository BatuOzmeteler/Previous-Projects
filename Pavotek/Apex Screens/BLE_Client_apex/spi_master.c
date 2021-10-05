/******************************************************************************
 *  Copyright Cambridge Silicon Radio Limited 2013-2015
 *  Part of CSR uEnergy SDK 2.4.4
 *  Application version 2.4.4.0
 *
 *  FILE
 *      spi_master.c
 *
 *  DESCRIPTION
 *      Library providing buffered asynchronous access to a SPI bus in the
 *      Master role. This library uses the PIO controller as a SPI driver.
 *
 ******************************************************************************/

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <types.h>          /* Commonly used typedefs */
#include <pio_ctrlr.h>      /* Access to the PIO controller */
#include <pio.h>            /* PIO configuration and control functions */
#include <mem.h>            /* Memory management routines */

/*============================================================================*
 *  Local Header Files
 *============================================================================*/

#include "spi_master.h"     /* Public interface to this file */

/*============================================================================*
 *  Private Definitions
 *============================================================================*/

/* PIO direction configured as output */
#define PIO_DIR_OUTPUT                      ( TRUE )
/* PIO direction configured as input */
#define PIO_DIR_INPUT                       ( FALSE )
/* The registers seen by PIO controller at addresses 0x41H and 0x40H */
#define PIO_CONTROLLER_REG_40H              ( PIO_CONTROLLER_RAM_START + 0x0020)
/* The registers seen by PIO controller at addresses 0x43H and 0x42H */
#define PIO_CONTROLLER_REG_42H              ( PIO_CONTROLLER_REG_40H + 1 )
/* The registers seen by PIO controller at addresses 0x45H and 0x44H */
#define PIO_CONTROLLER_REG_44H              ( PIO_CONTROLLER_REG_40H + 2 )
/* The registers seen by PIO controller at addresses 0x47H and 0x46H */
#define PIO_CONTROLLER_REG_46H              ( PIO_CONTROLLER_REG_40H + 3 )
/* The register where the PIO controller publishes the size of the data
 * it has transferred (in and out) prior to the SSEL de-assertion. */
#define PIO_CONTROLLER_REG_48H              ( PIO_CONTROLLER_REG_40H + 4 )

/* Offset (in words) from the start of the shared RAM, after which rest of
 * the area is used for SPI data exchange between XAP and PIO controller */
#define PIO_CONTROLLER_SPI_RAM_XAP_OFFSET   ( 0x0008 )
/* Address to the shared RAM used for exchanging SPI data between
 * PIO Controller and XAP */
#define PIO_CONTROLLER_SPI_RAM_XAP          ( PIO_CONTROLLER_REG_40H + \
                                             PIO_CONTROLLER_SPI_RAM_XAP_OFFSET )
/* Address to the shared RAM used for exchanging SPI data between
 * PIO Controller and XAP, as seen by PIO Controller */
#define PIO_CONTROLLER_SPI_RAM_PIO          \
                        ( 0x40 + ( PIO_CONTROLLER_SPI_RAM_XAP_OFFSET * 2U ) )
/* Size (in octets) of the shared RAM used for exchanging SPI data between
 * PIO Controller and XAP */
#define PIO_CONTROLLER_SPI_RAM_SIZE_OCTETS  \
                                    ( 0x80 - PIO_CONTROLLER_SPI_RAM_PIO )
/* Size (in words) of the shared RAM used for exchanging SPI data between
 * PIO Controller and XAP */
#define PIO_CONTROLLER_SPI_RAM_SIZE_WORDS   \
                                    ( PIO_CONTROLLER_SPI_RAM_SIZE_OCTETS / 2U )
/* HALF Size (in octets) of the total shared RAM used for exchanging SPI data
 * between PIO Controller and XAP */
#define PIO_CONTROLLER_SPI_RAM_HALF_OCTETS  PIO_CONTROLLER_SPI_RAM_SIZE_WORDS

/* Address to the section of shared RAM used to buffer incoming SPI data
 *  from SPI Master */
#define PIO_CONTROLLER_SPI_RAM_XAP_RX       PIO_CONTROLLER_SPI_RAM_XAP
/* Size (in octets) of the SPI RAM data area use for buffering incoming SPI
 * data from SPI Master */
#define PIO_CONTROLLER_SPI_RAM_RX_SIZE      PIO_CONTROLLER_SPI_RAM_HALF_OCTETS

/* Address to the section of shared RAM used to buffer SPI data for
 * onward transmission to SPI Master */
#define PIO_CONTROLLER_SPI_RAM_XAP_TX       \
    ( PIO_CONTROLLER_SPI_RAM_XAP_RX + ( PIO_CONTROLLER_SPI_RAM_RX_SIZE /2U ) )
/* Size (in octets) of the SPI RAM data area use to buffer SPI data for
 * onward transmission to SPI Master */
#define PIO_CONTROLLER_SPI_RAM_TX_SIZE      PIO_CONTROLLER_SPI_RAM_HALF_OCTETS

/* Convert a pio number to bit mask
 * @param[in] pio PIO number
 */
#define PIO_BIT_MASK(pio)                   (((uint32)1) << (pio))

/* Write to LSB Octet in shared RAM */
#define WRITE_OCTET_TO_LSB(X)               ( (X) & 0xFF )

/* Read from LSB Octet in shared RAM */
#define READ_OCTET_FROM_LSB(X)              ( (X) & 0xFF )

/* Set number of octets of RX data to be received by PIO Controller prior to
 * issuing an interrupt, to XAP */
#define SET_SPI_TRANSFER_SIZE_REG(X)        \
        (*((volatile uint16 *)PIO_CONTROLLER_REG_40H) = WRITE_OCTET_TO_LSB(X))

/* Get the current threshold for PIO controller RX data before issuing an
 * interrupt to XAP */
#define GET_SPI_TRANSFER_SIZE_REG           \
       (READ_OCTET_FROM_LSB(*((const volatile uint16 *)PIO_CONTROLLER_REG_40H)))

/* The XAP interrupt is level triggered so several interrupts may be generated
 * before the PIO Controller is able to clear the WAKE_XAP signal. To avoid
 * duplicate interrupts, a flag (INT_ACK) is set by the PIO Controller and
 * cleared by the XAP application to handshake the interrupt. */

/* Read the PIO controller interrupt acknowledgement flag */
#define GET_SPI_INT_ACK                     \
       (READ_OCTET_FROM_LSB(*((const volatile uint16 *)PIO_CONTROLLER_REG_46H)))

/* Write to the PIO Controller interrupt acknowledgement flag */
#define SET_SPI_INT_ACK(X)                  \
        (*((volatile uint16 *)PIO_CONTROLLER_REG_46H) = WRITE_OCTET_TO_LSB(X))

/* Read the inter-octet delay currently set */
#define GET_INTER_OCTET_DELAY               \
       (READ_OCTET_FROM_LSB(*((const volatile uint16 *)PIO_CONTROLLER_REG_48H)))

/* Set the inter-octet delay for the PIO controller. Setting this to 1 would
 * introduce no additional delay. Setting it to 0 would introduce maximum
 * delay (510 cycles).
 */
#define SET_INTER_BYTE_DELAY(X)             \
        (*((volatile uint16 *)PIO_CONTROLLER_REG_48H) = WRITE_OCTET_TO_LSB(X))

/*============================================================================*
 *  Private Data Types
 *============================================================================*/

/*
 * Type defining various states of the SPI master
 */
typedef enum
{
    /* SPI master has been initialised, but not ready yet */
    TransactionState_Initialised,
    /* SPI master is currently idle and SSEL is currently de-asserted */
    TransactionState_PIOControllerReady,
    /* SPI master has asserted the SSEL and a transaction is in progress */
    TransactionState_TransactionStarted,
    /* SPI master is processing the PIO controller interrupt and hence
     * PIO controller is currently paused */
    TransactionState_PioControllerOnHold,
    /* SPI master is waiting for PIO Controller interrupt */
    TransactionState_WaitingForPioController,
    /* SPI master is waiting for PIO Controller interrupt, from which it
     * intends to end the transaction by de-asserting the slave select */
    TransactionState_WaitingForPioControllerToDeassert,

    /* Invalid state */
    TransactionState_Invalid
} TransactionState;

/*
 * Type defining various events posted to the SPI master state machine
 */
typedef enum
{
    /* Event posted to indicate that SPI master is ready and PIO controller
     * is running */
    TransactionEvent_PIOControllerStart,
    /* Event posted to start a transaction by asserting the slave select */
    TransactionEvent_AssertSSEL,
    /* Event posted to stop a transaction by de-asserting the slave select */
    TransactionEvent_DeassertSSEL,
    /* Event posted to notify that data has been transferred over to PIO
     * controller shared RAM and PIO controller has been resumed */
    TransactionEvent_PIOControllerUnblockedWithDataToSend,
    /* Event posted to notify that PIO controller has been resumed but no
     * data has been transferred over to PIO controller shared RAM */
    TransactionEvent_PIOControllerUnblockedButNoData,
    /* Event posted to signal that PIO controller has finished a transfer */
    TransactionEvent_PIOControllerEvent,
    /* Event posted to reset the SPI Master */
    TransactionEvent_Reset
} TransactionEvent;

/*
 * Type defining the SPI Master's state
 */
typedef struct _SPI_MASTER_STATE_T
{
    /* Current transaction state of SPIMaster */
    TransactionState state;
    /* Handle to tx buffer with data to be sent */
    OQ_HANDLE p_tx_data;
    /* Handle to rx buffer with recieved data */
    OQ_HANDLE p_rx_data;
    /* Rx data pointer to the data area shared with PIO Controller */
    OQ_HANDLE_T pio_rx_data;
    /* Tx data pointer to the data area shared with PIO Controller */
    OQ_HANDLE_T pio_tx_data;
    /* Data transfer callback */
    SpiMasterTransactionCallbackType p_data_cb_func;
} SPI_MASTER_STATE_T;

/*============================================================================*
 *  Private Data
 *============================================================================*/

/* SPI Master's state variable */
static SPI_MASTER_STATE_T state;

/*============================================================================*
 *  Private Function Prototypes
 *============================================================================*/

/* Setup the PIO Controller shared RAM */
static bool setupSharedRAM(uint8 tx_buffer_fill);

/* Set SSEL line */
static void setSSEL(bool assert);

/* Given an event check and validate whether the event can be processed in the
 * the current state */
static TransactionState processEvent(TransactionEvent event);

/*============================================================================*
 *  Private Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      setupSharedRAM
 *
 *  DESCRIPTION
 *      Setup the PIO Controller shared RAM. Should only be called when PIO
 *      controller is not started
 *
 *  PARAMETERS
 *      tx_buffer_fill [in]     Fill character to be used to fill the Tx and Rx
 *                              data areas of the application shared memory
 *
 *  RETURNS
 *      TRUE if success, FALSE if failed.
 *----------------------------------------------------------------------------*/
static bool setupSharedRAM(uint8 tx_buffer_fill)
{
    bool ret_val = FALSE;
    
    /* Set the fill octet for the PIO controller tx buffer in the shared RAM */
    OQSetFill(&(state.pio_tx_data), TRUE, tx_buffer_fill);
    if (
            
            OQCreate(PIO_CONTROLLER_SPI_RAM_XAP_TX,  (PIO_CONTROLLER_SPI_RAM_TX_SIZE / 2U), OQDataMode_packed,  &(state.pio_tx_data))
            
            
            )
    /* If queue has been setup for the shared tx data area successfully */
    {
        /* Set the fill octet for the PIO controller rx buffer in the shared
         * RAM, just so we clear any existing data */
        OQSetFill(&(state.pio_rx_data), TRUE, tx_buffer_fill);
        if (OQCreate(PIO_CONTROLLER_SPI_RAM_XAP_RX,
                (PIO_CONTROLLER_SPI_RAM_RX_SIZE / 2U), OQDataMode_packed,
                &(state.pio_rx_data)))
        /* If queue has been setup for the shared rx data area successfully */
        {
            /* Fill the header area of the shared RAM */
            MemSet( PIO_CONTROLLER_REG_40H, 0U,
                                    PIO_CONTROLLER_SPI_RAM_XAP_OFFSET
                                        * sizeof(uint16) / sizeof(uint8));
            ret_val = TRUE;
        }
        /* No need to have the empty areas of the rx buffer in shared
         * RAM filled anymore */
        OQSetFill(&(state.pio_rx_data), FALSE, 0U);
    }

    if (!ret_val)
    {
        OQDestroy(&(state.pio_tx_data));
        OQDestroy(&(state.pio_rx_data));
    }

    return ret_val;
} /* setupSharedRAM */

/*----------------------------------------------------------------------------*
 *  NAME
 *      setSSEL
 *
 *  DESCRIPTION
 *      Set SSEL line
 *
 *  PARAMETERS
 *      assert [in]             Assert SSEL if TRUE, de-assert otherwise
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void setSSEL(bool assert)
{
    /*
     *   ACTIVE_LOW      ASSERT     PIO_STATE
     *   ====================================
     *      TRUE          TRUE        LOW   (FALSE)
     *      TRUE          FALSE       HIGH  (TRUE)
     *      FALSE         TRUE        HIGH  (TRUE)
     *      FALSE         FALSE       LOW   (FALSE)
     */
    PioSet(SPI_MASTER_PIO_SSEL, (SPI_MASTER_SSEL_ACTIVE_LOW != assert));
} /* setSSEL */

/** @page SPI_MASTER_STATE_TMachine SPI Master Statemachine
 *
 * SPI Master State machine implementation
 *
 * @dot
 * digraph finite_state_machine {
 *      rankdir=TD;
 *      node [shape = point ]; Invalid;
 *      node [shape = ellipse];
 *
 *      Invalid                 ->              Initialised;
 *      Initialised             ->              PIOControllerReady
 *              [label = "PIOControllerStart"];
 *      PIOControllerReady      ->              TransactionStarted
 *              [label = "AssertSSEL"];
 *      PIOControllerReady      ->              Initialised
 *              [label = "Reset"];
 *      TransactionStarted      ->              PIOControllerReady
 *              [label = "DeassertSSEL"];
 *      TransactionStarted      ->              WaitingForPioController
 *              [label = "PIOControllerUnblockedWithDataToSend"];
 *      TransactionStarted      ->              Initialised
 *              [label = "Reset"];
 *      WaitingForPioController ->              WaitingForPioControllerToDeassert
 *              [label = "DeassertSSEL"];
 *      WaitingForPioController ->              PioControllerOnHold
 *              [label = "PIOControllerEvent"];
 *      WaitingForPioController ->              Initialised
 *              [label = "Reset"];
 *      PioControllerOnHold     ->              PIOControllerReady
 *              [label = "DeassertSSEL"];
 *      PioControllerOnHold     ->              WaitingForPioController
 *              [label = "PIOControllerUnblockedWithDataToSend"];
 *      PioControllerOnHold     ->              TransactionStarted
 *              [label = "PIOControllerUnblockedButNoData"];
 *      PioControllerOnHold     ->              Initialised
 *              [label = "Reset"];
 *      WaitingForPioControllerToDeassert
 *                              ->              PIOControllerReady
 *              [label = "PIOControllerEvent"];
 *      WaitingForPioControllerToDeassert
 *                              ->              Initialised
 *              [label = "Reset"];
 *      }
 * @enddot
 */

/*----------------------------------------------------------------------------*
 *  NAME
 *      processEvent
 *
 *  DESCRIPTION
 *      Process a transaction event according to the current state
 *
 *  PARAMETERS
 *      event [in]              Transaction event to process
 *
 *  RETURNS
 *      Invalid state if event cannot be processed in the current state,
 *      a valid state if the event is allowed
 *----------------------------------------------------------------------------*/
static TransactionState processEvent(TransactionEvent event)
{
    /* Post-processing state */
    TransactionState ret_val = TransactionState_Invalid;

    switch (state.state)
    {
        case TransactionState_Initialised:
            /* SPI master has been initialised, but not ready yet */
            if (event == TransactionEvent_PIOControllerStart)
                /* If PIO Controller has been started */
            {
                /* Since PIO controller has started, SPI Master is ready */
                ret_val = TransactionState_PIOControllerReady;
            }
            break;
        case TransactionState_PIOControllerReady:
            /* SPI master is currently idle and SSEL is currently de-asserted */
            if (event == TransactionEvent_AssertSSEL)
                /* SSEL is being asserted */
            {
                /* SSEL can be asserted */
                ret_val = TransactionState_TransactionStarted;
            }
            else if (event == TransactionEvent_Reset)
                /* PIO controller is to be reset */
            {
                /* Go back to initialised state */
                ret_val = TransactionState_Initialised;
            }
            break;
        case TransactionState_TransactionStarted:
            /* SPI master has asserted the SSEL but no data transfer is
             * currently in progress */
            if (event == TransactionEvent_DeassertSSEL)
                /* SSEL is being de-asserted */
            {
                ret_val = TransactionState_PIOControllerReady;
            }
            else if (event ==
                          TransactionEvent_PIOControllerUnblockedWithDataToSend)
                /* If more data has been transferred to PIO controller and it is
                 * unblocked */
            {
                /* More data can be transferred to PIO controller */
                ret_val = TransactionState_WaitingForPioController;
            }
            else if (event == TransactionEvent_Reset)
                /* PIO controller is to be reset */
            {
                /* Go back to initialised state */
                ret_val = TransactionState_Initialised;
            }
            break;
        case TransactionState_PioControllerOnHold:
            /* If a transfer has just been completed PIO controller is currently
             * in hold */
            if (event == TransactionEvent_DeassertSSEL)
                /* SSEL is being de-asserted */
            {
                ret_val = TransactionState_PIOControllerReady;
            }
            else if (event ==
                          TransactionEvent_PIOControllerUnblockedWithDataToSend)
                /* If more data has been transferred to PIO controller and it is
                 * unblocked */
            {
                /* More data can be transferred to PIO controller */
                ret_val = TransactionState_WaitingForPioController;
            }
            else if (event == TransactionEvent_PIOControllerUnblockedButNoData)
                /* If no data has been transferred to PIO controller and it is
                 * unblocked */
            {
                /* We have nothing to wait on PIO controller */
                ret_val = TransactionState_TransactionStarted;
            }
            else if (event == TransactionEvent_Reset)
                /* PIO controller is to be reset */
            {
                /* Go back to initialised state */
                ret_val = TransactionState_Initialised;
            }
            break;
        case TransactionState_WaitingForPioController:
            /* If PIO controller is currently doing a transfer */
            if (event == TransactionEvent_DeassertSSEL)
                /* SSEL is being de-asserted */
            {
                /* SSEL de-assertion has to be postponed */
                ret_val = TransactionState_WaitingForPioControllerToDeassert;
            }
            else if (event == TransactionEvent_PIOControllerEvent)
                /* PIO Controller has finished a data transfer and is currently
                 * blocked */
            {
                /* PIO controller is on hold */
                ret_val = TransactionState_PioControllerOnHold;
            }
            else if (event == TransactionEvent_Reset)
                /* PIO controller is to be reset */
            {
                /* Go back to initialised state */
                ret_val = TransactionState_Initialised;
            }
            break;
        case TransactionState_WaitingForPioControllerToDeassert:
            /* If waiting for PIO controller to finish a data transfer to
             * de-assert the SSEL */
            if (event == TransactionEvent_PIOControllerEvent)
            {
                /* SSEL can be de-asserted */
                ret_val = TransactionState_PIOControllerReady;
            }
            else if (event == TransactionEvent_Reset)
                /* PIO controller is to be reset */
            {
                /* Go back to initialised state */
                ret_val = TransactionState_Initialised;
            }
            break;
        case TransactionState_Invalid:
            /* Nothing to do here */
            break;
        /* NOTE default case deliberately left out so as to catch any states
         * left un-handled */
    }
    
    return ret_val;
} /* processEvent */

/*============================================================================*
 *  Public Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterInit
 *
 *  DESCRIPTION
 *      Configure the SPI master. Once the SPI master implementation on
 *      the PIO controller is configured, it needs to be explicitly
 *      started by calling #SpiMasterStart.
 *
 *      The default PIO controller code is available via the symbols
 *      defined in #spi_cfg.h. However, if application wants to use its
 *      own PIO controller code then it should specify the appropriate
 *      symbol name, e.g. if the PIO controller assembly is added to the
 *      project in a file named xyz.asm, a symbol xyz would have been
 *      defined, which points to the address of the PIO controller code.
 *      This symbol should be passed in while initialising the SPI master
 *      driver. Note that a custom implementation of the PIO controller
 *      code, must maintain the interface between the XAP processor and
 *      the PIO Controller.
 *
 *      In order for the PIO Controller to run with fastest clock (to be
 *      able to support maximum SPI bus speed), it may be necessary to
 *      ensure that the XAP processor doesn't go to sleep, prior to calling
 *      this function.
 *
 *      This function can be called multiple times and each time it
 *      reinitialises the PIO Controller and starts afresh. However,
 *      calling it while a transfer is in progress might lead to data
 *      corruption. In order to simply reset the PIO controller use
 *      #SpiMasterReset instead.
 *
 *      NOTE: See the note regarding the maintenance of rx_data_buffer
 *      under #SpiMasterTransactionCallbackType
 *
 *  PARAMETERS
 *      p_pio_controller_code_addr [in]
 *                              Memory address containing SPI Master code to be
 *                              loaded into the PIO Controller
 *      p_data_callback [in]    Pointer to a function to be called upon data
 *                              being transferred to/received from the PIO
 *                              controller.
 *      tx_data_buffer [in]     Handle to a buffer to be used by the SPI Master
 *                              to read the tx data from, in order to be able to
 *                              transfer to PIO controller for onward
 *                              transmission to SPI Slave. It can contain data
 *                              to be transferred, which would then be
 *                              transferred to shared RAM upon starting the PIO
 *                              controller ready to be sent out when
 *                              transactions are started.
 *      rx_data_buffer [in]     Handle to a buffer to be used by the SPI Master
 *                              to queue any data received from the PIO
 *                              controller over SPI.
 *      tx_buffer_fill [in]     If buffer filling feature is compiled into the
 *                              octet_queue, the empty areas of the buffer
 *                              containing the outgoing data in the shared RAM
 *                              will be filled with this octet. Useful for
 *                              debugging the transfers.
 *      inter_octet_delay [in]  Set this to a count that will affect the inter-
 *                              octet delay. Setting it to 0 will cause no
 *                              additional delay. Each increment by 1 will
 *                              introduce 2 clock cycles (at 16MHz this is
 *                              0.125us). This is in addition to the existing
 *                              minimum inter-octet delay, see assembly source
 *                              for exact value of the minimum delay.
 *
 *  RETURNS
 *      TRUE if success, FALSE if failed.
 *----------------------------------------------------------------------------*/
bool SpiMasterInit(uint16                           *p_pio_controller_code_addr,
                   SpiMasterTransactionCallbackType  p_data_callback,
                   OQ_HANDLE                         p_tx_data_buffer,
                   OQ_HANDLE                         p_rx_data_buffer,
                   uint8                             tx_buffer_fill,
                   uint8                             inter_octet_delay)
{
    /* Check if valid tx and rx queue handles have been supplied */
    bool ret_val = (p_tx_data_buffer != NULL) &&
                   (p_rx_data_buffer != NULL) &&
                   (p_pio_controller_code_addr != NULL);

    if (ret_val)
    {
        /* Stop the PIO controller, in case it was already running */
        PioCtrlrStop();

        /* Set up the application buffers */
        state.p_tx_data = p_tx_data_buffer;
        state.p_rx_data = p_rx_data_buffer;

        /* Setup the data callback function */
        state.p_data_cb_func = p_data_callback;
        
        /* Configure the SSEL to be controlled independently */
        PioSetMode(SPI_MASTER_PIO_SSEL, pio_mode_user);
        
        /* Configure button to be input */
        PioSetDir(SPI_MASTER_PIO_SSEL, PIO_DIR_OUTPUT);
        
        /* De-assert the SPI Slave select */
        setSSEL(FALSE);
        
        /* Give the PIO controller access to the SPI Slave PIOs */
        PioSetModes(PIO_BIT_MASK(SPI_MASTER_PIO_SCLK) |
                    PIO_BIT_MASK(SPI_MASTER_PIO_MOSI) |
                    PIO_BIT_MASK(SPI_MASTER_PIO_MISO),
                    pio_mode_pio_controller);
        
        /* Set the PIO Controller clock to fastest available, so that it can run
         * off 16MHz clock when not sleeping */
        PioCtrlrClock(TRUE);
        
        /* Now download code onto PIO controller */
        PioCtrlrInit(p_pio_controller_code_addr);
        
        /* Setup the PIO controller shared RAM buffer */
        ret_val = setupSharedRAM(tx_buffer_fill);
        
        /* Setup the interrupt threshold */
        SET_SPI_TRANSFER_SIZE_REG(0U);
        
        /* Setup the inter-octet delay */
        SET_INTER_BYTE_DELAY(inter_octet_delay + 1U);
        
        /* Initialise the SPI Master state - no transaction is in progress */
        state.state = TransactionState_Initialised;
    }
    
    return ret_val;
} /* SpiMasterInit */

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterStart
 *
 *  DESCRIPTION
 *      Start the SPI Master. It must have been successfully configured
 *      first by calling #SpiMasterInit.
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void SpiMasterStart(void)
{
    /* Check if the SPI master has already been configured */
    const TransactionState newState =
                              processEvent(TransactionEvent_PIOControllerStart);
    
    if (newState == TransactionState_PIOControllerReady)
    {
        PioCtrlrStart();
        state.state = newState;
    }
} /* SpiMasterStart */

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterStartTransaction
 *
 *  DESCRIPTION
 *      Transfer data from the Tx queue and issue the data transfer call-back
 *      when all of the data within the Tx queue has been transferred over to
 *      the remote SPI slave. If SSEL is currently asserted calling this
 *      function would simply continue sending more data without
 *      de-asserting/re-asserting the SSEL line, allowing the application to
 *      continue an existing transaction.
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      TRUE if a new transaction is started or a transfer is already in
 *          progress.
 *      FALSE if no new transaction can be started at this time
 *          (most probably because current transaction is scheduled to be
 *          ended)
 *----------------------------------------------------------------------------*/
bool SpiMasterStartTransaction(void)
{
    bool ret_val = FALSE;
    
    TransactionState newState = processEvent(TransactionEvent_AssertSSEL);
    
    if (newState == TransactionState_TransactionStarted)
        /* If SSEL assertion is permitted */
    {
        /* Setup the state such that we will assert the SPI Slave select
         * just before starting a transfer */
        state.state = newState;
    }

    newState =
            processEvent(TransactionEvent_PIOControllerUnblockedWithDataToSend);

    if (newState == TransactionState_WaitingForPioController)
        /* If data transfer to PIO Controller shared RAM is permitted */
    {
        /* Transfer as much tx data over to PIO controller as possible */
        const uint16 transfer_size = OQTransferData(state.p_tx_data,
                                                     &(state.pio_tx_data),
                                                     OQSize(state.p_tx_data));
        
        /* Set the transfer size */
        SET_SPI_TRANSFER_SIZE_REG(transfer_size);

        if (state.state == TransactionState_TransactionStarted)
            /* If SSEL is yet to be asserted */
        {
            /* Assert the SPI Slave select */
            setSSEL(TRUE);
        }
        
        /* Issue interrupt to PIO controller to start the transfer */
        PioCtrlrInterrupt();
        
        /* Transaction is in progress */
        state.state = newState;
        ret_val = TRUE;
    }
    else if (state.state == TransactionState_WaitingForPioController)
        /* If a transfer is already in progress */
    {
        /* The data from tx data buffer will be transferred over to shared
         * RAM when the current transfer is completed */
        ret_val = TRUE;
    }
    
    return ret_val;
} /* SpiMasterStartTransaction */

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterEndTransaction
 *
 *  DESCRIPTION
 *      End an existing transaction by de-asserting the SSEL. If SSEL line
 *      is currently not asserted, calling this function will have no
 *      effect. If SSEL could not be asserted at this time usually because
 *      PIO controller is currently carrying out a data transfer this
 *      function schedules it such that SSEL gets de-asserted after the
 *      transfer is complete. Once SSEL is de-asserted the data transfer
 *      call-back will be issued. If this function is called while there
 *      is data existing in the Tx queue that has not yet been transferred
 *      over to the Tx data area, then that data will not be transferred
 *      over. As soon as the PIO controller finishes transferring
 *      existing data in the Tx data area, SPI master driver de-asserts
 *      the SSEL. Application should however be able to start a new
 *      transaction which will send the pending data existing in the
 *      Tx queue.
 *
 *      NOTE1: If FALSE is returned, a data transfer callback will be issued
 *      after the transfer is completed and SSEL has been de-asserted
 *
 *      NOTE2: Any data currently in the tx data buffer, pending to be
 *      delivered will not be transferred until the next transaction is
 *      started
 *
 *      NOTE3: If SSEL is to be de-asserted when the current data transfer
 *      has been completed, no new transfers can be started until the
 *      de-assertion happens
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      TRUE If SSEL has been de-asserted and transaction has ended,
 *      FALSE if a transaction is currently in progress and SSEL will be
 *          asserted when the data transfer has been completed.
 *----------------------------------------------------------------------------*/
bool SpiMasterEndTransaction(void)
{
    bool ret_val = FALSE;
    const TransactionState newState =
                                    processEvent(TransactionEvent_DeassertSSEL);
    
    if (newState == TransactionState_PIOControllerReady)
        /* If ending the transaction is permitted */
    {
        /* De-assert the SPI Slave select */
        setSSEL(FALSE);
        
        /* No transaction is in progress */
        state.state = newState;
        
        /* SSEL has been de-asserted */
        ret_val = TRUE;
    }
    else if (newState == TransactionState_WaitingForPioControllerToDeassert)
        /* If SSEL de-assertion is to be scheduled since a transfer is
         * currently in progress */
    {
        /* Schedule the SSEL to be de-asserted when PIO controller has finished
         * the transaction */
        state.state = newState;
    }
    else if (state.state == TransactionState_PIOControllerReady)
        /* If SSEL has already been de-asserted */
    {
        ret_val = TRUE;
    }
    
    return ret_val;
} /* SpiMasterEndTransaction */

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterGetSharedRAMTxDataSize
 *
 *  DESCRIPTION
 *      Get the size of data in the Tx data area (see 2.1.2.2) within the
 *      application shared memory. This value may not reflect the true size
 *      of the outstanding data that has not yet been sent by the SPI master
 *      on the PIO controller. This value is only updated when an interrupt
 *      is received from the PIO controller.
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Number of octets of data in the Tx data area within the application
 *      shared memory
 *----------------------------------------------------------------------------*/
uint16 SpiMasterGetSharedRAMTxDataSize(void)
{
    return OQSize(&(state.pio_tx_data));
} /* SpiMasterGetSharedRAMTxDataSize */

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterGetSharedRAMRxDataSize
 *
 *  DESCRIPTION
 *      Get the size of data in the Rx data area within the application
 *      shared memory. This value may not reflect the true size of the
 *      outstanding data that has been received by the SPI master on
 *      the PIO controller. This value is only updated when an interrupt
 *      is received from the PIO controller.
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Number of octets of data in the Rx data area within the application
 *      shared memory
 *----------------------------------------------------------------------------*/
uint16 SpiMasterGetSharedRAMRxDataSize(void)
{
    return OQSize(&(state.pio_rx_data));
} /* SpiMasterGetSharedRAMRxDataSize */

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterReset
 *
 *  DESCRIPTION
 *      Restarts the PIO controller and clears the application shared memory
 *      including the Tx/Rx data areas. Data in the Tx and Rx queues will not
 *      be cleared. This is useful when the application needs to reset and
 *      start over. Subsequent Tx data will be queued from the start of the Tx
 *      data area within the application shared memory.
 *
 *      NOTE: If failed, PIO controller would be stopped. The failure is
 *      usually attributed to the configuration of the octet queue.
 *
 *  PARAMETERS
 *      tx_buffer_fill [in]     If buffer filling feature is compiled into the
 *                              octet_queue, the empty areas of the buffer
 *                              containing the outgoing data in the shared RAM
 *                              will be filled with this octet
 *
 *  RETURNS
 *      TRUE if success, FALSE otherwise
 *----------------------------------------------------------------------------*/
bool SpiMasterReset(uint8 tx_buffer_fill)
{
    bool ret_val = FALSE;
    const TransactionState newState = processEvent(TransactionEvent_Reset);
    
    if (newState == TransactionState_Initialised)
        /* If a reset can be done */
    {
        /* Stop the PIO controller */
        PioCtrlrStop();
        
        /* Setup the shared RAM buffers */
        if (setupSharedRAM(tx_buffer_fill))
        {
            /* Restart PIO Controller */
            PioCtrlrStart();

            /* De-assert slave select */
            setSSEL(FALSE);

            /* Reset the state */
            state.state = TransactionState_PIOControllerReady;

            /* return success */
            ret_val = TRUE;
        }
    }
    else if (state.state == TransactionState_Initialised)
        /* If already reset */
    {
        /* return success */
        ret_val = TRUE;
    }
    
    return ret_val;
} /* SpiMasterReset */

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterDeinit
 *
 *  DESCRIPTION
 *      Stops the SPI master and frees the PIO Controller. The Tx and Rx queues
 *      allocated to the SPI master are now free for the application to use for
 *      any other purposes. In order to use the SPI master, it must be
 *      configured again by calling SpiMasterInit.
 *
 *      The data already existing in the Tx and Rx queues will not be
 *      affected.
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void SpiMasterDeinit(void)
{
    /* Stop the SPI Slave */
    PioCtrlrStop();

    /* Release the application buffers */
    state.p_tx_data = NULL;
    state.p_rx_data = NULL;

    /* Release the pio data buffer */
    OQDestroy(&(state.pio_tx_data));
    /* Release the pio data buffer */
    OQDestroy(&(state.pio_rx_data));
    /* Clear the data callback pointer */
    state.p_data_cb_func = NULL;

    /* De-assert slave select */
    setSSEL(FALSE);

    /* Reset the state */
    state.state = TransactionState_Invalid;

    /* We can't release the PIOs as we won't know what to set them to */
} /* SpiMasterDeinit */

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterProcessPioControllerEvent
 *
 *  DESCRIPTION
 *      Processes an interrupt from the PIO controller. When the PIO controller
 *      issues an interrupt the system event "sys_event_pio_ctrlr" is received
 *      via the system event handler, AppProcessSystemEvent.
 *
 *      SpiMasterProcessPioControllerEvent must be called as soon as possible
 *      upon receiving the system event.
 *
 * PARAMETERS
 *      p_data [in]             Data associated with the PIO controller event
 *
 * RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void SpiMasterProcessPioControllerEvent(uint16 *p_data)
{
    /* Check this is not a duplicate interrupt */
    const uint16 valid_interrupt = GET_SPI_INT_ACK;
    
    if (valid_interrupt)
    {
        TransactionState newState =
                              processEvent(TransactionEvent_PIOControllerEvent);
        
        if (newState != TransactionState_Invalid)
            /* If PIO Controller event is expected */
        {
            /* Get the previous transfer size */
            uint16 transfer_size = GET_SPI_TRANSFER_SIZE_REG;
            /* Previous data size in the PIO shared tx data buffer */
            uint16 pio_tx_data_size;
            /* Data size in the PIO shared rx data buffer */
            uint16 pio_rx_data_size;

            /* Adjust the rx data buffer to include the octets that have been
             * received */
            OQIncrementSize(&(state.pio_rx_data), transfer_size);

            /* Transfer the received data from the shared RAM into the rx data
             * buffer */
            OQTransferData(&(state.pio_rx_data), state.p_rx_data,
                                                PIO_CONTROLLER_SPI_RAM_RX_SIZE);

            /* Rx data buffer should now be empty, unless Rx queue is full */
            pio_rx_data_size = OQSize(&(state.pio_rx_data));

            /* Get the transfer size in the PIO controller */
            pio_tx_data_size = OQSize(&(state.pio_tx_data));

            if (pio_tx_data_size < transfer_size)
                /* If there was buffer under-run in the tx data area of the
                 * shared RAM. This should never happen since we would never
                 * set the transfer size to be more than the data we have
                 * actually copied over to the PIO controller shared RAM */
            {
                /* Adjust the PIO shared tx data buffer size to account for the
                 * data under-run in the tx buffer */
                OQIncrementSize(&(state.pio_tx_data),
                                              transfer_size - pio_tx_data_size);
            }

            /* Adjust the PIO shared tx data buffer size to account for the
             * data that has been sent. By this point the tx data in the PIO
             * controller shared RAM must have been empty */
            OQDecrementSize(&(state.pio_tx_data), transfer_size);

            if (state.state ==
                             TransactionState_WaitingForPioControllerToDeassert)
                /* If no more data can be transferred and SSEL needs to be
                 * de-asserted */
            {
                /* Set the transfer size */
                SET_SPI_TRANSFER_SIZE_REG(0U);
                
                /* De-assert the SPI Slave select */
                setSSEL(FALSE);
                
                /* No transaction is in progress */
                state.state = newState;
                
                /* Issue the callback notifying the current status of the
                 * transfer */
                if (state.p_data_cb_func != NULL)
                {
                    (*(state.p_data_cb_func))(state.p_rx_data, state.p_tx_data);
                }
            }
            else if (state.state == TransactionState_WaitingForPioController)
                /* If data transfer has to be continued */
            {
                /* PIO controller on hold */
                state.state = newState;

                if (((OQSize(state.p_tx_data) == 0) ||
                    /* If no more data is left in the tx data buffer to be
                     * transferred over to the PIO Controller shared RAM,
                     * it means the data transfer has been completed */
                    (pio_rx_data_size > 0)) &&
                    /* If there is no room in the Rx queue to transfer all
                     * of the data from the rx data area */
                    (state.p_data_cb_func != NULL))
                {
                    (*(state.p_data_cb_func))(state.p_rx_data, state.p_tx_data);
                }

                if (pio_rx_data_size > 0)
                    /* There was no room in the Rx queue to transfer all
                     * of the data from the rx data buffer, see if applications
                     * made more room available from the call-back */
                {
                    /* Transfer the received data from the shared RAM into the
                     * rx data buffer */
                    OQTransferData(&(state.pio_rx_data), state.p_rx_data,
                                                PIO_CONTROLLER_SPI_RAM_RX_SIZE);
                }

                if (state.state == newState)
                    /* If the callback did not change the state, which would
                     * be the case if the transaction has ended and/or new
                     * transaction has started */
                {
                    if (OQSize(state.p_tx_data) > 0)
                        /* If there is more data to be transferred */
                    {
                        newState = processEvent(
                         TransactionEvent_PIOControllerUnblockedWithDataToSend);
                    }
                    else
                        /* If there is no more data to be transferred */
                    {
                        newState = processEvent(
                              TransactionEvent_PIOControllerUnblockedButNoData);
                    }

                    if (newState == TransactionState_WaitingForPioController)
                        /* If more data is to be transferred over to shared RAM */
                    {
                        /* Transfer as much tx data over to PIO controller as
                         * possible */
                        transfer_size = OQTransferData(state.p_tx_data,
                                                      &(state.pio_tx_data),
                                                      OQSize(state.p_tx_data));
                        
                        /* Set the transfer size */
                        SET_SPI_TRANSFER_SIZE_REG(transfer_size);
                        
                        /* Issue interrupt to PIO controller to resume the
                         * transfer */
                        PioCtrlrInterrupt();
                    }
                    
                    /* Transaction is in progress */
                    state.state = newState;
                }
            }
        }
        
        /* Acknowledge the interrupt */
        SET_SPI_INT_ACK(0U);
    }
} /* SpiMasterProcessPioControllerEvent */
