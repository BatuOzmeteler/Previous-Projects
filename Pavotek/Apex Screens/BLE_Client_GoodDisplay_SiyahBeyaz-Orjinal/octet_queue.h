/******************************************************************************
 *  Copyright Cambridge Silicon Radio Limited 2013-2015
 *  Part of CSR uEnergy SDK 2.4.4
 *  Application version 2.4.4.0
 *
 *  FILE
 *      octet_queue.h
 *
 *  DESCRIPTION
 *      Utility for implementing a queue on top of a buffer
 *
 ******************************************************************************/

#ifndef __OCTET_QUEUE_H__
#define __OCTET_QUEUE_H__

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <types.h>          /* Commonly used typedefs */

/*============================================================================*
 *  Local Header Files
 *============================================================================*/

/*============================================================================*
 *  Public Definitions
 *============================================================================*/

/* Maximum permitted buffer capacity */
/* The queue indices are of type uint16 and count octets, so this value is
 * limited by the maximum value a uint16 can hold, and needs to take into
 * account that a packed queue stores two octets per word. */
#define MAX_ALLOWED_BUFFER_CAPACITY     (0x7FFE)

/*============================================================================*
 *  Public Data Types
 *============================================================================*/

/* Type defining the queued data mode */
typedef enum
{
    /* Each word in the buffer contains a single octet of data */
    OQDataMode_unpacked = 0,

    /* Each word in the buffer contains two octets of data. For the XAP
     * each word accommodates 2 octets, first octet (based on the order they
     * were queued) will be stored in the least significant octet position
     * while a subsequent octet queued will be stored in the most significant
     * position. E.g. if the value stored as a word is 0x1B1A, it represents
     * the octets 0x1A (queued first) followed by 0x1B (queued next) */
    OQDataMode_packed,

    /* Defined to check for invalid values, so any new enumerations must
     * be defined above this */
    OQDataMode_INVALID
} OQDataMode;

/* Type defining the Octet queue handle data structure */
typedef struct _OQ_HANDLE_T
{
    /* Pointer to the backing buffer used by the octet queue to store the
     * queued data. This will be a pointer to uint16 for a packed queue, and
     * uint8 for an unpacked queue */
    void *p_buffer;

    /* Capacity of the backing buffer, in number of elements. For a packed
     * queue, this is the number of uint16's. For an unpacked queue this is the
     * number of uint8's.
     * This should always be less than MAX_ALLOWED_BUFFER_CAPACITY
     */
    uint16 capacity;
	
    /* Start octet index,
     * For packed mode:
     *          Element index: (value/2)
     *          octet index: value (even LSB octet, odd MSB octet)
     * For unpacked mode:
     *          Element index: value
     */
    uint16 start;
	
    /* Octet boundary
     * For all data modes:
     *          points to an index after the last element in the buffer. If
     *          buffer is full, its value matches start index. Same is true
     *          if buffer is empty, however empty condition is distinguished
     *          from full condition by the #empty flag
     */
    uint16 boundary;
    
    /* Buffer is empty
     * When start index and boundary coincide, this flag indicates whether the
     * buffer is empty or full.
     */
    bool empty;

    /* In order to fill empty areas of the buffer with a specific octet value,
     * define the macro OQ_ENABLE_BUFFER_FILL in the project properties
     */
#ifdef OQ_ENABLE_BUFFER_FILL
    /* When this feature is compiled in, filling can be enabled/disabled on a
     * per queue basis. Enabling filling slows down the speed at which the
     * queue can process removing data from buffer (as part of Pop and Transfer
     * operations)
     * If the most significant octet is not 0, buffer filling is enabled.
     * The least significant octet contains the octet to be used to fill
     * the empty areas of the buffer
     */
    uint16 fill;
#endif

    /* Data mode for the buffer */
    OQDataMode mode;
} OQ_HANDLE_T;

/* Type defining the Queue handle pointer */
typedef OQ_HANDLE_T *OQ_HANDLE;

/*============================================================================*
 *  Public Data
 *============================================================================*/

/*============================================================================*
 *  Public Function Prototypes
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQCreate
 *
 *  DESCRIPTION
 *      Create an octet queue on top of a given buffer. Automatically
 *      initialises the buffer with the fill character (since the queue is
 *      emptied upon creation), if buffer filling is enabled (#OQSetFill)
 *      prior to calling this function.
 *
 *  PARAMETERS
 *      p_buffer [in]           Buffer for backing the queue. The queue can only
 *                              function during the life span of the buffer. The
 *                              queue handle will become invalid once the buffer
 *                              is freed.
 *      buffer_capacity [in]    Total capacity of the buffer in number of
 *                              elements - i.e. number of uint8 for unpacked, or
 *                              number of uint16 if packed. Must be a positive
 *                              integer up to MAX_ALLOWED_BUFFER_CAPACITY.
 *      buffer_mode [in]        Mode in which the data is buffered
 *      handle [out]            Octet queue structure, into which the handle
 *                              will be created and copied.
 *
 *  RETURNS
 *      TRUE if success, FALSE if failed
 *----------------------------------------------------------------------------*/
extern bool OQCreate(void *p_buffer, uint16 buffer_capacity,
                      OQDataMode buffer_mode, OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQDestroy
 *
 *  DESCRIPTION
 *      Destroy the queue. The handle will no longer be valid.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the queue to destroy
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void OQDestroy(OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQIsValid
 *
 *  DESCRIPTION
 *      Check that the supplied handle has not been destroyed
 *
 *  PARAMETERS
 *      handle [in]             Handle to the queue to validate
 *
 *  RETURNS
 *      TRUE if the queue is valid, FALSE otherwise
 *----------------------------------------------------------------------------*/
extern bool OQIsValid(const OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQQueueData
 *
 *  DESCRIPTION
 *      Queue data into octet queue
 *
 *  PARAMETERS
 *      handle [in]             Handle to the queue
 *      p_data [in]             Buffer holding data to be queued. The buffer
 *                              must be packed in the same way as the queue.
 *                              If the buffer is packed, then the first octet
 *                              will be taken from the LSB of the first word in
 *                              the buffer.
 *      octet_count [in]        The number of octets of data to be queued.
 *                              If there is not enough space in the queue then
 *                              as many octets as will fit will be queued.
 *                              An odd number of octets is acceptable whichever
 *                              packing mode is used.
 *
 *  RETURNS
 *      Number of octets of data actually queued
 *----------------------------------------------------------------------------*/
extern uint16 OQQueueData(OQ_HANDLE handle, const void *p_data,
                           uint16 octet_count);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQPopData
 *
 *  DESCRIPTION
 *      Extract octets from the head of the queue into a buffer
 *
 *  PARAMETERS
 *      handle [in]             Handle to the queue
 *      p_data [out]            Buffer to which data from the queue is to be
 *                              extracted. The buffer must be packed in the same
 *                              way as the queue. If the buffer is packed, then
 *                              the first octet will be stored in the LSB of the
 *                              first word in the buffer.
 *      octet_count [in]        The number of octets of data to be extracted.
 *                              If there is not enough data in the queue then
 *                              as many octets as are available will be
 *                              extracted. An odd number of octets is acceptable
 *                              whichever packing mode is used.
 *
 *  RETURNS
 *      Number of octets actually extracted from the queue.
 *----------------------------------------------------------------------------*/
extern uint16 OQPopData(OQ_HANDLE handle, void *p_data, uint16 octet_count);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQTransferData
 *
 *  DESCRIPTION
 *      Extract data from one octet queue and insert it into another. Queues
 *      need not have identical data packing modes.
 *      Effectively the same as calling OQPopData followed by OQQueueData
 *      without having to create a buffer to hold the data inbetween.
 *
 *  PARAMETERS
 *      from [in]               Handle to the octet queue from which data is to
 *                              be read
 *      to [out]                Handle to the octet queue into which data is to
 *                              be queued
 *      octet_count [in]        The number of octets of data to be transferred.
 *                              If there is not enough data in the source queue
 *                              then only as much data as is available will be
 *                              transferred.
 *                              If there is not enough space in the destination
 *                              queue then only as much data as there is space
 *                              for will be transferred.
 *                              An odd number of octets is acceptable whichever
 *                              packing mode is used.
 *
 *  RETURNS
 *      Number of octets actually transferred.
 *----------------------------------------------------------------------------*/
extern uint16 OQTransferData(OQ_HANDLE from, OQ_HANDLE to, uint16 octet_count);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQSize
 *
 *  DESCRIPTION
 *      Get the size of the data in an octet queue.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      Number of octets queued
 *----------------------------------------------------------------------------*/
extern uint16 OQSize(const OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQIsEmpty
 *
 *  DESCRIPTION
 *      Check if the given queue is empty
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      TRUE if empty, FALSE otherwise
 *----------------------------------------------------------------------------*/
extern bool OQIsEmpty(const OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQIsFull
 *
 *  DESCRIPTION
 *      Check if the given queue is full
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      TRUE if full, FALSE otherwise
 *----------------------------------------------------------------------------*/
extern bool OQIsFull(const OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQCapacity
 *
 *  DESCRIPTION
 *      Get the total capacity of the octet queue
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      Total capacity of the queue in octets
 *----------------------------------------------------------------------------*/
extern uint16 OQCapacity(const OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQSpace
 *
 *  DESCRIPTION
 *      Available space in an octet queue
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      Space available in the queue in octets
 *----------------------------------------------------------------------------*/
extern uint16 OQSpace(const OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQClear
 *
 *  DESCRIPTION
 *      Clear the data in an octet queue. If buffer filling is enabled for
 *      this octet queue, it fills the empty areas of the buffer with the
 *      fill character configured using #OQSetFill. This actually removes
 *      the data by moving the buffer start index down to coincide with
 *      the buffer end index.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void OQClear(OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQSetFill
 *
 *  DESCRIPTION
 *      Enable/Disable buffer filling and set the fill octet for a given octet
 *      queue. Buffer filling enables automatically setting empty areas of the
 *      queue to a specified fill octet.
 *
 *      The queue need not have been created yet. This function is typically
 *      called prior to OQCreate so that when the queue is created it is
 *      automatically filled with the fill octet.
 *
 *      If the buffer filling feature is not compiled in (by not defining macro
 *      OQ_ENABLE_BUFFER_FILL), calling this function has no effect.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *      enable [in]             Set to TRUE to enable buffer filling for this
 *                              octet queue, or FALSE to disable it
 *      fill_octet [in]         Octet to be used to fill the empty space in the
 *                              buffer. Only effective when the parameter
 *                              #enable is set to TRUE.
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void OQSetFill(OQ_HANDLE handle, bool enable, uint8 fill_octet);

/******************************************************************************
 * Interfaces useful for using the queue on a shared memory, for example
 * reading a shared memory area already being filled by external means.
 *****************************************************************************/

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQRollbackClear
 *
 *  DESCRIPTION
 *      Same as OQClear, except that it removes the data by moving the buffer
 *      end index up to coincide with the buffer start index. Useful when
 *      the octet queue is used to access a shared buffer, when required to
 *      overwrite any existing data.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void OQRollbackClear(OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQSetSize
 *
 *  DESCRIPTION
 *      Adjust the buffer indexes as per the octet_count supplied. The buffer
 *      size can be increased or decreased, there by affecting the boundary or
 *      start index respectively. Useful when octet queue is used to access
 *      a shared buffer, to account for data added/extracted externally.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *      octet_count [in]        New queue size, in octets
 *
 *  RETURNS
 *      TRUE:  Success. The queue size is set to the specified value. Any
 *             surplus data is removed (if the fill character is set, removed
 *             data is replaced by that character). If the queue size has
 *             increased the end index is simply increased to account for the
 *             new size.
 *
 *      FALSE: Failed.
 *----------------------------------------------------------------------------*/
extern bool OQSetSize(OQ_HANDLE handle, uint16 octet_count);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQIncrementSize
 *
 *  DESCRIPTION
 *      Adjust the buffer end index to add a given number of octets, without
 *      actually adding those octets. Useful when the octet queue is used
 *      to access a shared buffer, to adjust for the data added externally
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *      octet_count [in]        Number of octets to add
 *
 *  RETURNS
 *      Number of octets actually added to the queue
 *----------------------------------------------------------------------------*/
extern uint16 OQIncrementSize(OQ_HANDLE handle, uint16 octet_count);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQDecrementSize
 *
 *  DESCRIPTION
 *      Adjust the buffer start index to remove a given number of octets,
 *      without actually extracting those octets. If buffer filling is
 *      enabled, this also fills the emptied area with the chosen fill
 *      character. Useful when the octet queue is used to access
 *      a shared buffer, to adjust for the data extracted externally.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *      octet_count [in]        The number of octets to remove
 *
 *  RETURNS
 *      Number of octets actually removed from the queue
 *----------------------------------------------------------------------------*/
extern uint16 OQDecrementSize(OQ_HANDLE handle, uint16 octet_count);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQGetStartOffset
 *
 *  DESCRIPTION
 *      Get the offset to the head of the queue in octets.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      Index of the first octet in the buffer. For an empty queue this is
 *      the index where the first octet will be queued.
 *----------------------------------------------------------------------------*/
extern uint16 OQGetStartOffset(const OQ_HANDLE handle);

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQGetEndOffset
 *
 *  DESCRIPTION
 *      Get the offset of the tail of the queue in octets.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *
 *  RETURNS
 *      Index of the last octet in the buffer. For an empty queue this is
 *      invalid and does not point to any valid data
 *----------------------------------------------------------------------------*/
extern uint16 OQGetEndOffset(const OQ_HANDLE handle);

#endif /* __OCTET_QUEUE_H__ */
