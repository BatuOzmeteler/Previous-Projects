#ifndef __SPI_MASTER_H__
#define __SPI_MASTER_H__

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <types.h>          /* Commonly used typedefs */

/*============================================================================*
 *  Local Header Files
 *============================================================================*/

#include "octet_queue.h"    /* Octet queue API */
#include "spi_cfg.h"        /* SPI configuration */

/*============================================================================*
 *  Public Data Types
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      SpiMasterTransactionCallbackType
 *
 *  DESCRIPTION
 *      Type defining the SPI Master data transfer call-back. This call-back
 *      is issued when all of the data within the Tx queue has been
 *      transferred to the remote SPI Slave. The application can add more data
 *      to the Tx queue from within this call-back to continue the transfer
 *      automatically. If no data is added within this call-back the transfer
 *      will halt (however SSEL will not be de-asserted unless the application
 *      calls #SpiMasterEndTransaction). If the application adds more data to
 *      the Tx queue after this call-back has returned, the application will
 *      need to explicitly initiate the transfer by calling
 *      #SpiMasterStartTransaction.
 *
 *      This call-back will also be issued when more data has been received
 *      than can be accommodated in the Rx queue. This is to give the
 *      application an opportunity to make room in the Rx queue. If the
 *      application doesn't remove data from Rx queue within the call-back,
 *      the data will not be transferred over from the Rx data area within the
 *      application shared memory; a future transfer can potentially over-run
 *      the Rx data area causing data corruption.
 *
 *      It is the application's responsibility to assert SSEL at the beginning
 *      of a transaction by calling #SpiMasterStartTransaction and to de-assert
 *      SSEL at the end of a transaction by calling #SpiMasterEndTransaction.
 *      Since this call-back is issued when the PIO controller has finished
 *      transferring all of the data from its Tx data area, it is advisable to
 *      de-assert the SSEL line within this call-back if the application intends
 *      to finish the transaction and relinquish the slave. This way SSEL
 *      de-assertion will be synchronised with the data transfer and any
 *      possibility of premature de-assertion can be prevented.
 *
 *  PARAMETERS
 *      rx_data [out]           Handle to the buffer containing data received
 *                              from the SPI Slave by the PIO controller. In
 *                              packed mode data is stored in little-endian
 *                              order, i.e. if 0x1A is received first and then
 *                              0x1B, the data stored will be 0x1B1A.
 *      tx_data [out]           Handle to the tx_data buffer
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
typedef void (*SpiMasterTransactionCallbackType)(OQ_HANDLE rx_data, 
                                                 OQ_HANDLE tx_data);

/*============================================================================*
 *  Public Function Prototypes
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
extern bool SpiMasterInit(
                   uint16                           *p_pio_controller_code_addr,
                   SpiMasterTransactionCallbackType  p_data_callback,
                   OQ_HANDLE                         tx_data_buffer,
                   OQ_HANDLE                         rx_data_buffer,
                   uint8                             tx_buffer_fill,
                   uint8                             inter_octet_delay);

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
extern void SpiMasterStart(void);

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
extern bool SpiMasterStartTransaction(void);

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
extern bool SpiMasterEndTransaction(void);

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
extern uint16 SpiMasterGetSharedRAMTxDataSize(void);

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
extern uint16 SpiMasterGetSharedRAMRxDataSize(void);

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
extern bool SpiMasterReset(uint8 tx_buffer_fill);

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
extern void SpiMasterDeinit(void);

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
extern void SpiMasterProcessPioControllerEvent(uint16 *p_data);

#endif /* __SPI_MASTER_H__ */
