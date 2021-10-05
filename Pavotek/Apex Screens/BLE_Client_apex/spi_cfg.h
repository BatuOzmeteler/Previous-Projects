#ifndef __SPI_CFG_H__
#define __SPI_CFG_H__

#define SPI_MASTER_PIO_SSEL                         (6U)
#define SPI_MASTER_PIO_SCLK                         (5U)
#define SPI_MASTER_PIO_MOSI                         (7U)
#define SPI_MASTER_PIO_MISO                         (8U)

#define SPI_MASTER_SSEL_ACTIVE_LOW                  (TRUE)
extern void pio_ctrlr_spi_master_0(void);

#endif /* __SPI_CFG_H__ */
