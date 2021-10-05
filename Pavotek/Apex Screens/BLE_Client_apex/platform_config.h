/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H


//#define EPD_BUSY_IN         (0U)    /* PIO0 */
//#define EPD_PWM_OUT         (4U)    /* PIO4 */
//#define EPD_BORDER_CTRL_OUT (10U)   /* PIO10 */
//#define N_EPD_RST_OUT       (1U)    /* PIO1 */
//#define EPD_DISCHARGE_OUT   (9U)    /* PIO9 */
//#define EPD_OFF_OUT         (3U)    /* PIO3 */
//#define CS_OUT              (6U)    /* PIO6 */


#define BUSY_PIN            (3U)    
#define SPI_MASTER_PIO_SCLK (5U)
#define CS_PIN              (6U)    
#define SPI_MASTER_PIO_MOSI (7U)
#define DC_PIN              (8U)    
#define RESET_PIN           (9U)   

// #define BS_PIN      (9U)    

#define DISLIKE_BUTTON      (10U) //DISLIKE

#define LIKE_BUTTON         (11U) //LIKE


/* Convert a PIO number into a bit mask */
#define PIO_BIT_MASK(pio)       (0x01UL << (pio))
/* Setup PIO 11 as Button PIO */
#define DISLIKE_BUTTON_MASK         (PIO_BIT_MASK(DISLIKE_BUTTON))
#define LIKE_BUTTON_MASK         (PIO_BIT_MASK(LIKE_BUTTON))

/* PIO direction */
#define PIO_DIRECTION_INPUT     (FALSE)
#define PIO_DIRECTION_OUTPUT    (TRUE)

#define HIGH    TRUE
#define LOW     FALSE
 
#define INPUT   FALSE
#define OUTPUT  TRUE


#ifndef ITOA_BUFFER_SIZE
#define ITOA_BUFFER_SIZE 7
#endif


#define ROW 	264
#define COLUMN  22
#define HEIGHT	10 // Barkodun yüksekliði 50 pixel olarak konfigüre edilmiþtir. Deðiþtirilebilir.
#define LONG	1
#define NORMAL	0
#define RIGHT	1
#define LEFT	0
#define BITMAP_SIZE ROW * COLUMN


extern uint8 final[];

#endif /* __PLATFORM_CONFIG_H */
