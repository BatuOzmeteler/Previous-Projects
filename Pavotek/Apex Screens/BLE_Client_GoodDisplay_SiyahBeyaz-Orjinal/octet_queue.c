/******************************************************************************
 *  Copyright Cambridge Silicon Radio Limited 2013-2015
 *  Part of CSR uEnergy SDK 2.4.4
 *  Application version 2.4.4.0
 *
 *  FILE
 *      octet_queue.c
 *
 *  DESCRIPTION
 *      Circular buffer implementation using packed and unpacked arrays
 *
 ******************************************************************************/

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <mem.h>            /* Memory library */

/*============================================================================*
 *  Local Header Files
 *============================================================================*/

#include "octet_queue.h"    /* Interface to this source file */

/*============================================================================*
 *  Private Definitions
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      FILL_BUFFER
 *
 *  DESCRIPTION
 *      Fill a buffer with a fill octet.
 *
 *  PARAMETERS
 *      buffer [in]             Buffer to fill
 *      octet [in]              Octet to fill buffer with. The octet in the LSB
 *                              of the 16-bit word will be used
 *      size [in]               Number of elements to fill. For a packed buffer
 *                              this is the number of uint16's, for an unpacked
 *                              unpacked buffer this is the number of uint8's
 *
 *  RETURNS
 *      Original pointer to the destination buffer
 *----------------------------------------------------------------------------*/
#define FILL_BUFFER(buffer, octet, size) \
    MemSet(buffer, ((uint16)(octet) << 8) | ((octet) & 0x00ff), size)

/*============================================================================*
 *  Private Function Prototypes
 *============================================================================*/

/* Store data in the queue */
static uint16 queueData(OQ_HANDLE   handle,
                        const void *p_data,
                        uint16      data_size_in_octets,
                        bool        src_starts_at_odd_octet,
                        OQDataMode  src_data_mode);

/* Extract data from the queue */
static uint16 popData(OQ_HANDLE  handle,
                      void      *p_data,
                      uint16     data_size_in_octets);

/*============================================================================*
 *  Private Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      queueData
 *
 *  DESCRIPTION
 *      Queue data into octet queue.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue into which data is to
 *                              be queued
 *      p_data [in]             Pointer to the source buffer holding data to be
 *                              queued. Size of each element of the buffer
 *                              depends on the source buffer data mode.
 *      data_size_in_octets [in]
 *                              The number of octets to be queued.
 *      src_starts_at_odd_octet [in] 
 *                              Start from odd octet when copying from the
 *                              source buffer. This parameter is only applicable
 *                              in packed mode.
 *      src_data_mode [in]      Data mode that the source buffer uses.
 *
 *  RETURNS
 *      Number of octets of data actually buffered
 *----------------------------------------------------------------------------*/
static uint16 queueData(OQ_HANDLE   handle,
                        const void *p_data,
                        uint16      data_size_in_octets,
                        bool        src_starts_at_odd_octet,
                        OQDataMode  src_data_mode)
{
    /* Available space in the queue, in octets */
    /* (This will be 0 if handle is invalid) */
    const uint16 space = OQSpace(handle);
    
    /* Number of octets of data that can be queued */
    const uint16 octets_queued = MIN(space, data_size_in_octets);
    
    /* Number of octets of data remaining to be queued */
    uint16 data_to_be_queued = octets_queued;
    
    /* If there is no space to queue more data return now */
    if (data_to_be_queued == 0)
        return 0;
    
    if ((handle->mode == OQDataMode_packed) &&
        (src_data_mode == OQDataMode_packed))
    {
        /* Pointer into source buffer */
        const uint16 *in = (const uint16 *)p_data;
        /* Pointer into destination buffer */
        uint16 *out = (uint16 *)handle->p_buffer + handle->boundary/2;
        /* Flag to indicate whether source octet is from MSB or LSB */
        bool in_msb = src_starts_at_odd_octet;
        /* Flag to indicate whether destination octet is from MSB or LSB */
        bool out_msb = handle->boundary & 0x1;
    
        /* If the source and destination buffers are not similarly aligned */
        if (in_msb != out_msb)
        {
            /* Copy each octet from source to destination one by one */
            do
            {
                uint16 src_octet;       /* Octet to copy */
            
                /* Read octet from source */
                if (in_msb)
                {
                    src_octet = (*in & 0xff00) >> 8;
                    in++;   /* Move the input pointer to the next word */
                }
                else
                {
                    src_octet = *in & 0x00ff;
                }
            
                /* Write octet to destination */
                if (out_msb)
                {
                    *out = (*out & 0x00ff) | (src_octet << 8);
                    out++;  /* Move the output pointer to the next word */
                }
                else
                {
                    *out = (*out & 0xff00) | src_octet;
                }

                /* Toggle MSB flags */
                in_msb = !in_msb;
                out_msb = !out_msb;
                    
                /* The above could be optimised given that if in_msb is true,
                 * then out_msb must be false, and vice versa. In fact we do
                 * not need out_msb at all.
                 * It has been left un-optimised to improve readability.
                 */
                    
                /* Decrement number of octets to be queued */
                data_to_be_queued--;

                /* Advance queue boundary and wrap round if end of buffer
                 * reached.
                 */
                if (++handle->boundary == OQCapacity(handle))
                {
                    out = (uint16 *)handle->p_buffer;
                    handle->boundary = 0;
                }
            } while (data_to_be_queued > 0);
        }
        else /* in_msb == out_msb */
        {
            /* How many octets will fit in the remaining buffer */
            uint16 remaining;

            /* If the buffers are aligned on the MSB copy the first octet
             * separately (because MemCopy only works on whole words).
             */
            if (in_msb)
            {
                /* Copy the octet from the source to the destination */
                *out = (*out & 0x00ff) | (*in & 0xff00);
            
                /* Decrement number of octets to be queued */
                data_to_be_queued--;
            
                /* Advance buffer pointers */
                in++;
                out++;
            
                /* Advance queue boundary */
                handle->boundary++;
            }
                
            /* Check whether the buffer will wrap round */
            remaining = OQCapacity(handle) - handle->boundary;
            if (data_to_be_queued >= remaining)
            {
                /* Copy the data from source to destination */
                MemCopy(out, in, remaining/2);
                    
                /* Decrement the number of octets remaining to be copied */
                data_to_be_queued -= remaining;
                    
                /* Advance the input pointer */
                in += remaining/2;
            
                /* Reset the output pointer */
                out = (uint16 *)handle->p_buffer;
            
                /* Reset the queue boundary */
                handle->boundary = 0;
            }
        
            /* Copy from source to destination */
            MemCopy(out, in, data_to_be_queued/2);
        
            /* If there is an odd octet left over */
            if (data_to_be_queued & 0x1)
            {
                /* Advance buffer pointers */
                in += data_to_be_queued/2;
                out += data_to_be_queued/2;
            
                /* Copy the octet into the LSB leaving the MSB unaltered */
                *out = (*out & 0xff00) | (*in & 0x00ff);
            }
                
            /* Advance queue boundary */
            handle->boundary += data_to_be_queued;
        }
    }
    else if ((handle->mode == OQDataMode_unpacked) &&
             (src_data_mode == OQDataMode_unpacked))
    {
        /* Pointer into source buffer */
        const uint8 *in = (const uint8 *)p_data;
        /* Pointer into destination buffer */
        uint8 *out = (uint8 *)handle->p_buffer + handle->boundary;
        /* How many octets will fit in the remaining buffer */
        const uint16 remaining = OQCapacity(handle) - handle->boundary;

        /* Check whether the buffer will wrap round */
        if (data_to_be_queued >= remaining)
        {
            /* Copy the data from source to destination */
            MemCopy(out, in, remaining);

            /* Decrement the number of octets remaining to be copied */
            data_to_be_queued -= remaining;
        
            /* Advance the input pointer */
            in += remaining;

            /* Reset the output pointer */
            out = (uint8 *)handle->p_buffer;
        
            /* Reset the queue boundary */
            handle->boundary = 0;
        }

        /* Copy from source to destination */
        MemCopy(out, in, data_to_be_queued);

        /* Advance queue boundary */
        handle->boundary += data_to_be_queued;
    }
    else if ((handle->mode == OQDataMode_packed) &&
             (src_data_mode == OQDataMode_unpacked))
    {
        /* Pointer into source buffer */
        const uint8 *in = (const uint8 *)p_data;
        /* Pointer into destination buffer */
        uint16 *out = (uint16 *)handle->p_buffer + handle->boundary/2;
        /* How many octets will fit in the remaining buffer */
        uint16 remaining;
    
        /* If the first octet to write is in the MSB */
        if (handle->boundary & 0x1)
        {
            /* Copy the first octet separately (MemCopyPack only copies into
             * whole words)
             */
            *out = (*out & 0x00ff) | ((*in & 0x00ff) << 8);
        
            /* Decrement number of octets remaining to be copied */
            data_to_be_queued--;
        
            /* Advance pointers */
            out++;
            in++;
        
            /* Advance queue boundary */
            handle->boundary++;
        }

        /* Check whether the buffer will wrap round */
        remaining = OQCapacity(handle) - handle->boundary;
        if (data_to_be_queued >= remaining)
        {
            /* Copy the data from source to destination */
            MemCopyPack(out, in, remaining);

            /* Decrement the number of octets remaining to be copied */
            data_to_be_queued -= remaining;

            /* Advance the input pointer */
            in += remaining;
        
            /* Reset the output pointer */
            out = (uint16 *)handle->p_buffer;
        
            /* Reset the queue boundary */
            handle->boundary = 0;
        }

        /* Copy whole words from source to destination. If there are an odd
         * number of octets to copy then the last octet is copied into the
         * LSB of the last destination word, and the MSB is not modified.
         */
        MemCopyPack(out, in, data_to_be_queued);

        /* Advance queue boundary */
        handle->boundary += data_to_be_queued;
    }
    else /* ((handle->mode == OQDataMode_unpacked) &&
             (src_data_mode == OQDataMode_packed)) */
    {
        /* Pointer into source buffer */
        const uint16 *in = (const uint16 *)p_data;
        /* Pointer into destination buffer */
        uint8 *out = (uint8 *)handle->p_buffer + handle->boundary;
        /* How many octets will fit in the remaining buffer */
        uint16 remaining;

        /* If the first octet to read is in the MSB */
        if (src_starts_at_odd_octet)
        {
            /* Copy the first octet separately (MemCopyUnPack only copies
             * from whole words)
             */
            *out = (*in & 0xff00) >> 8;
        
            /* Decrement number of octets remaining to be copied */
            data_to_be_queued--;
        
            /* Advance buffer pointers */
            out++;
            in++;
        
            /* Advance queue boundary */
            handle->boundary++;
        }

        /* Check whether the buffer will wrap round */    
        remaining = OQCapacity(handle) - handle->boundary;
        if (data_to_be_queued >= remaining)
        {
            /* Copy the data from source to destination */
            MemCopyUnPack(out, in, remaining);

            /* Decrement the number of octets remaining to be copied */
            data_to_be_queued -= remaining;
        
            /* Advance the input pointer */
            in += remaining/2;

            /* Reset the output pointer */
            out = (uint8 *)handle->p_buffer;
        
            /* Reset the queue boundary */
            handle->boundary = 0;
                
            /* If the next octet to read is in the MSB */
            if ((data_to_be_queued > 0) && (remaining & 0x1))
            {
                /* Copy the next octet separately (MemCopyUnPack only copies
                 * from whole words)
                 */
                *out = (*in & 0xff00) >> 8;
        
                /* Decrement number of octets remaining to be copied */
                data_to_be_queued--;
        
                /* Advance buffer pointers */
                out++;
                in++;
        
                /* Advance queue boundary */
                handle->boundary++;                    
            }
        }

        /* Copy from source to destination */
        MemCopyUnPack(out, in, data_to_be_queued);

        /* Advance queue boundary */
        handle->boundary += data_to_be_queued;
    }

    /* Buffer is not empty as we have just queued data */
    handle->empty = FALSE;

    return octets_queued;
} /* queueData */

/*----------------------------------------------------------------------------*
 *  NAME
 *      popData
 *
 *  DESCRIPTION
 *      Pop octets from the start of the queue into a buffer
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue from which data is to
 *                              be read
 *      p_data [in]             Pointer to a buffer where data from the queue is
 *                              to be copied. The size of each element in the
 *                              buffer depends on the data mode of the queue.
 *      data_size_in_octets [in]
 *                              The number of octets to be extracted.
 *
 *  RETURNS
 *      Number of octets actually extracted from the queue
 *----------------------------------------------------------------------------*/
static uint16 popData(OQ_HANDLE  handle,
                      void      *p_data,
                      uint16     data_size_in_octets)
{
    /* Total available data in the queue, in octets */
    /* (This will be 0 if handle is invalid) */
    const uint16 avlbl_data = OQSize(handle);
    
    /* Number of octets of data that are to be fetched */
    const uint16 octets_fetched = MIN(avlbl_data, data_size_in_octets);

    /* Number of octets remaining to be fetched */
    uint16 data_to_be_fetched = octets_fetched;

    /* If there is no data available return now */
    if (data_to_be_fetched == 0)
        return 0;

    if (handle->mode == OQDataMode_packed)
    {
        /* Octet queue is packed */

        /* Pointer into source buffer */
        const uint16 *in = (const uint16 *)handle->p_buffer + handle->start/2;
        /* Pointer into destination buffer */
        uint16 *out = (uint16 *)p_data;
        /* Flag to indicate whether source octet is from MSB or LSB */
        bool in_msb = handle->start & 0x1;

        /* If the source and destination buffers are not similarly aligned
         * (first destination octet is always in the LSB)
         */
        if (in_msb)
        {
            /* Pointer to first element beyond the buffer */
            const uint16 *const buffer_end = (const uint16 *)handle->p_buffer +
                                                              handle->capacity;
            /* Flag to indicate whether destination octet is from MSB or LSB */
            bool out_msb = FALSE;   /* Always LSB to start with */

            /* Copy each octet from source to destination one by one */
            do
            {
                uint16 src_octet;       /* Octet to copy */
            
                /* Read octet from source */
                if (in_msb)
                {
                    src_octet = (*in & 0xff00) >> 8;
                    in++;   /* Move the input pointer to the next word */
                }
                else
                {
                    src_octet = *in & 0x00ff;
                }
            
                /* Write octet to destination */
                if (out_msb)
                {
                    *out = (*out & 0x00ff) | (src_octet << 8);
                    out++;  /* Move the output pointer to the next word */
                }
                else
                {
                    *out = (*out & 0xff00) | src_octet;
                }

                /* Toggle MSB flags */
                in_msb = !in_msb;
                out_msb = !out_msb;
                    
                /* The above could be optimised given that if in_msb is true,
                 * then out_msb must be false, and vice versa. In fact we do
                 * not need out_msb at all.
                 * It has been left un-optimised to improve readability.
                 */
                    
                /* Decrement number of octets to be fetched */
                data_to_be_fetched--;

                /* Reset the input pointer if end of buffer reached. */
                if (in == buffer_end)
                {
                    in = (const uint16 *)handle->p_buffer;
                }
            } while (data_to_be_fetched > 0);
        }
        else /* !in_msb */
        {
            /* How many octets are available before the queue wraps round */
            const uint16 remaining = OQCapacity(handle) - handle->start;
            
            /* Check whether the queue will wrap round */
            if (data_to_be_fetched >= remaining)
            {
                /* Copy the data from source to destination */
                MemCopy(out, in, remaining/2);
                
                /* Decrement the number of octets waiting to be copied */
                data_to_be_fetched -= remaining;
                
                /* Reset the input pointer */
                in = (const uint16 *)handle->p_buffer;
                
                /* Advance the output pointer */
                out += remaining/2;
            }

            /* Copy from source to destination */
            MemCopy(out, in, data_to_be_fetched/2);

            /* If there is an odd octet remaining to be copied */
            if (data_to_be_fetched & 0x1)
            {
                /* Advance buffer pointers */
                in += data_to_be_fetched/2;
                out += data_to_be_fetched/2;
            
                /* Copy the octet into the LSB leaving the MSB unaltered */
                *out = (*out & 0xff00) | (*in & 0x00ff);
            }
        }
    }
    else /* handle->mode == OQDataMode_unpacked */
    {
        /* Octet queue is unpacked */

        /* Pointer into source buffer */
        const uint8 *in = (const uint8 *)handle->p_buffer + handle->start;
        /* Pointer into destination buffer */
        uint8 *out = (uint8 *)p_data;
        /* How many octets are available before the queue wraps round */
        const uint16 remaining = OQCapacity(handle) - handle->start;
            
        /* Check whether the queue will wrap round */
        if (data_to_be_fetched >= remaining)
        {
            /* Copy the data from source to destination */
            MemCopy(out, in, remaining);
                
            /* Decrement the number of octets waiting to be copied */
            data_to_be_fetched -= remaining;
                
            /* Reset the input pointer */
            in = (const uint8 *)handle->p_buffer;
                
            /* Advance the output pointer */
            out += remaining;
        }

        /* Copy from source to destination */
        MemCopy(out, in, data_to_be_fetched);
    }
    
    /* Advance queue indices (and optionally fill empty spaces with fill
     * octet)
     */
    OQDecrementSize(handle, octets_fetched);

    return octets_fetched;
} /* popData */

/*============================================================================*
 *  Public Function Implementations
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
bool OQCreate(void       *p_buffer,
               uint16      buffer_capacity,
               OQDataMode  buffer_mode,
               OQ_HANDLE   handle)
{
    /* If  valid buffer capacity is specified, and data mode is valid */
    const bool ret_val = (handle != NULL) && (p_buffer != NULL) && 
                         (buffer_capacity <= MAX_ALLOWED_BUFFER_CAPACITY) &&
                         (buffer_capacity > 0) &&
                         (buffer_mode < OQDataMode_INVALID);

    if (ret_val)
    {
        /* Initialise the octet queue control structure */
        handle->p_buffer = p_buffer;
        handle->capacity = buffer_capacity;
        handle->mode = buffer_mode;
        handle->start = 0;
        handle->boundary = 0;
        handle->empty = TRUE;
#ifdef OQ_ENABLE_BUFFER_FILL
        /* Check whether filling is enabled for this queue */
        if (handle->fill & 0xff00)
        {
            /* Fill the buffer with fill octets */
            FILL_BUFFER(handle->p_buffer, handle->fill, handle->capacity);
        }
#endif
    }
    
    return ret_val;
} /* OQCreate */

/*----------------------------------------------------------------------------*
 *  NAME
 *      OQDestroy
 *
 *  DESCRIPTION
 *      Destroy the queue. The handle will no longer be valid.
 *
 *  PARAMETERS
 *      handle [in/out]         Handle to the queue to destroy
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void OQDestroy(OQ_HANDLE handle)
{
    if (handle != NULL)
    {
        handle->p_buffer = NULL;
        handle->capacity = 0;
        handle->empty = TRUE;
    }
} /* OQDestroy */

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
bool OQIsValid(const OQ_HANDLE handle)
{
    return ((handle != NULL) &&
            (handle->p_buffer != NULL) &&
            (handle->capacity > 0));
} /* OQIsValid */

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
uint16 OQQueueData(OQ_HANDLE handle, const void *p_data, uint16 octet_count)
{
    return queueData(handle, p_data, octet_count, FALSE, handle->mode);
} /* OQQueueData */

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
uint16 OQPopData(OQ_HANDLE handle, void *p_data, uint16 octet_count)
{
    return popData(handle, p_data, octet_count);
} /* OQPopData */

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
uint16 OQTransferData(OQ_HANDLE from, OQ_HANDLE to, uint16 octet_count)
{
    /* Available space in the destination queue, in octets */
    /* (This will be 0 if handle is invalid) */
    const uint16 avlbl_space = OQSpace(to);
    
    /* Total available data in the source queue, in octets */
    /* (This will be 0 if handle is invalid) */
    const uint16 avlbl_data = OQSize(from);

    /* Maximum number of octets of data that can be transferred */
    const uint16 octets_moved = MIN(MIN(avlbl_data, avlbl_space), octet_count);

    /* Number of octets remaining to be transferred */
    uint16 data_to_be_moved = octets_moved;
    
    /* How many octets are available before the source queue wraps round */
    const uint16 remaining = OQCapacity(from) - from->start;
	
    /* Indicate whether first octet is from MSB (ignored in unpacked mode) */
    bool first_msb = from->start & 0x1;

    /* Pointer into the source buffer */
    const void *src;
	
    /* If there is no data to transfer return now */
    if (data_to_be_moved == 0)
        return 0;
    
    /* Set up the source pointer based on the queue data mode */
    if (from->mode == OQDataMode_packed)
    {
        src = (const uint16 *)from->p_buffer + from->start/2;
    }
    else
    {
        src = (const uint8 *)from->p_buffer + from->start;
    }
    
    /* Check whether the source buffer will wrap round */
    if (data_to_be_moved > remaining)
    {
        /* Copy data from source to destination */
        queueData(to, src, remaining, first_msb, from->mode);

        /* Decrement the number of octets waiting to be moved */
        data_to_be_moved -= remaining;
        
        /* Reset the input pointer */
        src = from->p_buffer;
	    
        /* Update MSB flag */
        first_msb = FALSE;
    }

    /* Copy data from source to destination */
    queueData(to, src, data_to_be_moved, first_msb, from->mode);

    /* Decrement the size of the source to account for data that has
     * been transferred */
    OQDecrementSize(from, octets_moved);

    return octets_moved;
} /* OQTransferData */

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
uint16 OQSize(const OQ_HANDLE handle)
{
    uint16 data_size = 0;               /* Size of queued data in octets */
    
    /* If the buffer is not empty */
    if ((handle != NULL) && !handle->empty)
    {
        /* If the boundary index has rolled over */
        if (handle->start >= handle->boundary)
        {
            data_size = OQCapacity(handle) - handle->start + handle->boundary;
        }
        else
        {
            data_size = handle->boundary - handle->start;
        }
    }
    
    return data_size;
} /* OQSize */

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
bool OQIsEmpty(const OQ_HANDLE handle)
{
    return ((handle != NULL) && handle->empty);
} /* OQIsEmpty */

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
bool OQIsFull(const OQ_HANDLE handle)
{
    /* if buffer rolled over to start and is not empty, it is full */
    return ((handle != NULL) &&
            (handle->start == handle->boundary) &&
            !handle->empty);
} /* OQIsFull */

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
uint16 OQCapacity(const OQ_HANDLE handle)
{
    uint16 octet_count = 0;     /* Number of octets the queue can hold */

    if (handle != NULL)
    {
        /* Number of elements in the queue */
        octet_count = handle->capacity;
        
        /* For packed mode, we can store 2 octets in each element */
        if (handle->mode == OQDataMode_packed)
        {
            octet_count *= 2;
        }
    }
    
    return octet_count;
} /* OQCapacity */

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
uint16 OQSpace(const OQ_HANDLE handle)
{
    return OQCapacity(handle) - OQSize(handle);
} /* OQSpace */

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
void OQClear(OQ_HANDLE handle)
{
    if ((handle != NULL) && !handle->empty)
    {
        /* Reset the start index of the buffer */
        handle->start = handle->boundary;
        
        /* Indicate that the buffer is empty */
        handle->empty = TRUE;

#ifdef OQ_ENABLE_BUFFER_FILL
        /* Check filling is enabled for this queue */
        if (handle->fill & 0xFF00)
        {
            /* Fill the buffer with fill octets */
            FILL_BUFFER(handle->p_buffer, handle->fill, handle->capacity);
        }
#endif
    }
} /* OQClear */

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
 *      If the buffer filling feature is disabled (by not defining macro
 *      OQ_ENABLE_BUFFER_FILL) calling this function has no effect.
 *
 *  PARAMETERS
 *      handle [in]             Handle to the octet queue
 *      enable [in]             Set to TRUE to enable buffer filling for this
 *                              octet queue, or FALSE to disable it
 *      fill_octet [in]         Octet to be used to fill the empty space in the
 *                              buffer. Only effective when the parameter
 *                              "enable" is set to TRUE.
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void OQSetFill(OQ_HANDLE handle, bool enable, uint8 fill_octet)
{
#ifdef OQ_ENABLE_BUFFER_FILL
    if (handle != NULL)
    {
        handle->fill = (((uint16)enable) << 8) | (fill_octet & 0xFF);
    }
#endif
} /* OQSetFill */

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
void OQRollbackClear(OQ_HANDLE handle)
{
    if ((handle != NULL) && !handle->empty)
    {
        /* Reset the boundary index of the buffer */
        handle->boundary = handle->start;
        
        /* Indicate that the buffer is empty */
        handle->empty = TRUE;
        
#ifdef OQ_ENABLE_BUFFER_FILL
        /* Check filling is enabled for this queue */
        if (handle->fill & 0xFF00)
        {
            /* Fill the buffer with fill octets */
            FILL_BUFFER(handle->p_buffer, handle->fill, handle->capacity);
        }
#endif
    }
} /* OQRollbackClear */

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
bool OQSetSize(OQ_HANDLE handle, uint16 octet_count)
{
    /* Function status */
    bool ret_val = FALSE;
    
    /* Check the new size does not exceed the maximum queue capacity */
    /* (OQCapacity returns 0 if the queue is invalid) */
    if (octet_count <= OQCapacity(handle))
    {
        const uint16 current_size = OQSize(handle);
        
        if (current_size > octet_count)
        {
            OQDecrementSize(handle, current_size - octet_count);
        }
        else if (current_size < octet_count)
        {
            OQIncrementSize(handle, octet_count - current_size);
        }
        
        ret_val = TRUE;
    }

    return ret_val;
} /* OQSetSize */

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
uint16 OQIncrementSize(OQ_HANDLE handle, uint16 octet_count)
{
    /* Available space in the queue in octets, 0 if the queue is invalid */
    const uint16 space = OQSpace(handle);
    
    /* Check there's enough space left in the queue */
    if (octet_count > space)
        octet_count = space;
    
    if (octet_count > 0)
    {
        /* Amount of space left before the buffer wraps round */
        const uint16 remaining = OQCapacity(handle) - handle->boundary;
        
        /* Advance the boundary to account for the octets added */
        if (octet_count >= remaining)
            handle->boundary = octet_count - remaining;
        else
            handle->boundary += octet_count;
        
        /* Buffer isn't empty as we have just moved the boundary */
        handle->empty = FALSE;
    }

    return octet_count;
} /* OQIncrementSize */

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
uint16 OQDecrementSize(OQ_HANDLE handle, uint16 octet_count)
{
    /* Total available data in the queue, in octets */
    /* (This will be 0 if handle is invalid) */
    const uint16 avlbl_data = OQSize(handle);
    
    /* Number of octets of data that are to be removed */
    const uint16 octets_removed = MIN(avlbl_data, octet_count);
    
    /* Number of octets between queue head and end of buffer */
    uint16 wrap_size;

    /* If there is no data to be removed return now */
    if (octets_removed == 0)
        return 0;

    /* Safe to initialise wrap_size now */
    wrap_size = OQCapacity(handle) - handle->start;

#ifdef OQ_ENABLE_BUFFER_FILL
    /* If buffer filling has not been disabled */

    /* Check whether buffer filling is enabled for this queue */
    if (handle->fill & 0xff00)
    {
        /* Fill octet */
        const uint16 fill = handle->fill & 0x00ff;
        
        /* Number of octets remaining to be filled */
        uint16 octets_to_be_filled = octets_removed;
    
        /* Check whether the queue is packed or unpacked */
        if (handle->mode == OQDataMode_packed)
        {
            /* Pointer into queue buffer */
            uint16 *ptr = (uint16 *)handle->p_buffer + handle->start/2;

            /* How many octets remain before the queue wraps round */
            uint16 remaining = wrap_size;

            /* If the first byte is from the MSB, fill that byte */
            if (handle->start & 0x1)
            {
                /* Insert the fill octet into the MSB */
                *ptr = (*ptr & 0x00ff) | (fill << 8);
            
                /* Advance the buffer pointer */
                ptr++;
            
                /* Decrement number of octets remaining before queue wraps */
                remaining--;
            
                /* Decrement number of octets to be filled */
                octets_to_be_filled--;
            }

            /* Check whether the queue will wrap round */
            if (octets_to_be_filled >= remaining)
            {
                /* Set the remaining octets in the buffer */
                FILL_BUFFER(ptr, fill, remaining/2);
                
                /* Decrement the number of octets waiting to be filled */
                octets_to_be_filled -= remaining;

                /* Reset the buffer pointer */
                ptr = handle->p_buffer;
            }

            /* Fill the buffer with the fill octet */
            FILL_BUFFER(ptr, fill, octets_to_be_filled/2);

            /* If there is an odd octet remaining to be filled */
            if (octets_to_be_filled & 0x1)
            {
                /* Advance buffer pointer */
                ptr += octets_to_be_filled/2;
            
                /* Fill the octet in the LSB leaving the MSB unaltered */
                *ptr = (*ptr & 0xff00) | fill;
            }
        }
        else /* handle->mode == OQDataMode_unpacked */
        {
            /* Octet queue is unpacked */

            /* Pointer into queue buffer */
            uint8 *ptr = (uint8 *)handle->p_buffer + handle->start;
            
            /* Check whether the queue will wrap round */
            if (octets_to_be_filled >= wrap_size)
            {
                /* Fill the rest of the buffer */
                FILL_BUFFER(ptr, fill, wrap_size);

                /* Decrement the number of octets waiting to be filled */
                octets_to_be_filled -= wrap_size;
                  
                /* Reset the buffer pointer */
                ptr = handle->p_buffer;
            }

            /* Fill the queue buffer */
            FILL_BUFFER(ptr, fill, octets_to_be_filled);
        }
    }
#endif /* OQ_ENABLE_BUFFER_FILL */

    /* Advance queue start index, wrapping when end of buffer is reached */
    if (octets_removed >= wrap_size)
        handle->start = octets_removed - wrap_size;
    else
        handle->start += octets_removed;

    /* Indicate whether the queue is empty */
    if (handle->start == handle->boundary)
        handle->empty = TRUE;
    
    /* Return the number of octets the queue size has been decremented by */
    return octets_removed;
} /* OQDecrementSize */

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
uint16 OQGetStartOffset(const OQ_HANDLE handle)
{
    return (handle != NULL) ? handle->start : 0;
} /* OQGetStartOffset */

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
uint16 OQGetEndOffset(const OQ_HANDLE handle)
{
    /* Index of last octet in the queue */
    uint16 end_offset = 0;
    
    /* Check the queue is valid */
    if (handle != NULL)
    {
        /* Extract the offset to the end of the queue */
        end_offset = handle->boundary;
        
        /* The boundary index actually points to the next octet to be filled at
         * the end of the queue. Decrement the index by one to find the most
         * recently queued item */
        if (end_offset > 0)
        {
            /* If the index is not at 0 it can simply be decremented */
            end_offset--;
        }
        else
        {
            /* Otherwise it needs to wrap round to the maximum index */
            end_offset = OQCapacity(handle) - 1;
        }
    }

    return end_offset;
} /* OQGetEndOffset */
