/******************************************************************************
 *  Copyright Cambridge Silicon Radio Limited 2013-2014
 *  Part of CSR uEnergy SDK 2.3.0
 *  Application version 2.3.0.0
 *
 *  FILE
 *      debug_interface.c
 *
 *  DESCRIPTION
 *      This file contains implementation of the debug functions
 *
 *****************************************************************************/

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <debug.h>          /* Simple host interface to the UART driver */
#include <types.h>          /* Commonly used type definitions */
#include <gatt_uuid.h>      /* Common Bluetooth UUIDs and macros */
#include <mem.h>            /* Memory library */

/*============================================================================*
 *  Local Header File
 *============================================================================*/

#include "debug_interface.h"/* Interface to this file */
#include "user_config.h"    /* User configuration */

#ifdef DEBUG_OUTPUT_ENABLED

/*============================================================================*
 *  Private Definitions
 *===========================================================================*/

/* Smallest buffer size required to hold longest 16-bit integer value */
#define ITOA_BUFFER_SIZE 7

/*============================================================================*
 *  Private Function Prototypes
 *============================================================================*/

/* Convert decimal integer value to ASCII string */
static uint8 itoa(int16 value, char *str);

/*============================================================================*
 *  Private Function Implementations
 *============================================================================*/
extern uint16 uartRxDataCallback(void *p_rx_buffer, uint16  length, uint16 *p_additional_req_data_length);
/*----------------------------------------------------------------------------*
 *  NAME
 *      itoa
 *
 *  DESCRIPTION
 *      Convert a 16-bit signed integer decimal value into a string.
 *
 *  PARAMETERS
 *      value [in]              Decimal value to convert
 *      str [in]                Buffer into which to place string (must have
 *                              room for at least ITOA_BUFFER_SIZE chars)
 *
 *  RETURNS
 *      Length of the string.
 *---------------------------------------------------------------------------*/
static uint8 itoa(int16 value, char *str)
{
    uint8  i = ITOA_BUFFER_SIZE;    /* Index into buffer */
    uint16 remainder;               /* Remaining value to convert */
    char   buffer[ITOA_BUFFER_SIZE];/* Buffer for ASCII string */
    
    /* Initialise remainder. If value is negative convert remainder from 16-bit
     * 2's complement negative value to 16-bit absolute value
     */
    remainder = (value < 0) ? ~value + 1 : value;

    /* Ensure the string is correctly terminated */    
    buffer[--i] = '\0';

    /* Loop at least once and until the whole value has been converted */
    do
    {
        /* Convert the unit value into ASCII and store in the buffer */
        buffer[--i] = (remainder % 10) + '0';
        
        /* Shift the value right one decimal */
        remainder /= 10;
    } while (remainder > 0);
    
    /* Prepend '-' if the value is negative */
    if (value < 0)
    {
        buffer[--i] = '-';
    }
    
    /* Copy the buffer into the return string, including NUL terminator */
    MemCopy(str, buffer + i, ITOA_BUFFER_SIZE - i);

    /* Return length of str, excluding NUL terminator (as per strlen) */
    return (ITOA_BUFFER_SIZE - 1) - i;
} /* itoa */

/*============================================================================*
 *  Public Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      DebugIfInit
 *
 *  DESCRIPTION
 *      Initialise debug output.
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void DebugIfInit(void)
{
    /* Initialise debug output */
    DebugInit(1, uartRxDataCallback, NULL);
} /* DebugIfInit */

/*----------------------------------------------------------------------------*
 *  NAME
 *      DebugIfWriteBdAddress
 *
 *  DESCRIPTION
 *      Print out a Bluetooth address.
 *
 *  PARAMETERS
 *      address [in]            Bluetooth address to write to UART
 *
 *  RETURNS
 *      Nothing
 *---------------------------------------------------------------------------*/
void DebugIfWriteBdAddress(const TYPED_BD_ADDR_T *address)
{
     DebugWriteUint8(WORD_MSB(address->addr.lap));
     DebugWriteString(":");
     DebugWriteUint8(WORD_LSB(address->addr.lap));
     
     DebugWriteString(":");
     
         DebugWriteUint8(address->addr.uap);
         DebugWriteString(":");
         DebugWriteUint8(WORD_LSB(address->addr.lap >> 16));
         DebugWriteString(":");
    
    DebugWriteUint8(WORD_MSB(address->addr.nap));
    DebugWriteString(":");
    DebugWriteUint8(WORD_LSB(address->addr.nap));
    
   
    
} /* DebugIfWriteBdAddress */

/*----------------------------------------------------------------------------*
 *  NAME
 *      DebugIfWriteInt
 *
 *  DESCRIPTION
 *      Print out an integer value
 *
 *  PARAMETERS
 *      value [in]              Integer value to write to UART
 *
 *  RETURNS
 *      Nothing
 *---------------------------------------------------------------------------*/
void DebugIfWriteInt(int16 value)
{
    char valueString[ITOA_BUFFER_SIZE];

    itoa(value, valueString);
    
    DebugWriteString(valueString);
} /* DebugIfWriteInt */

/*----------------------------------------------------------------------------*
 *  NAME
 *      DebugIfWriteUuid128
 *
 *  DESCRIPTION
 *      Print out a 128-bit UUID
 *
 *  PARAMETERS
 *      uuid [in]               Buffer containing 128-bit UUID to write to UART
 *
 *  RETURNS
 *      Nothing
 *---------------------------------------------------------------------------*/
void DebugIfWriteUuid128(const uint8 *uuid)
{
    uint8 i;
    
    for(i = 0; i < 16; i++)
    {
        DebugWriteUint8(uuid[i]);
    }
} /* DebugIfWriteUuid128 */

#endif /* DEBUG_OUTPUT_ENABLED */
