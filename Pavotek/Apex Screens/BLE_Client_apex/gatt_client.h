#ifndef __GATT_CLIENT_H__
#define __GATT_CLIENT_H__

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <types.h>          /* Commonly used type definitions */
#include <bluetooth.h>      /* Bluetooth specific type definitions */
#include <timer.h>          /* Chip timer functions */

/*============================================================================*
 *  Local Header Files
 *============================================================================*/

#include "gatt_access.h"    /* GATT-related routines */

/*============================================================================*
 *  Public Definitions
 *============================================================================*/

/* Maximum number of words in GAP central Identity Resolution Key (IRK) */
#define MAX_WORDS_IRK                       (8)

/*============================================================================*
 *  Public Function Prototypes
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      ReportPanic
 *
 *  DESCRIPTION
 *      This function calls firmware panic routine and gives a single point 
 *      of debugging any application level panics.
 *
 *  PARAMETERS
 *      panic_code [in]         Code to supply to firmware Panic function.
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void ReportPanic(app_panic_code panic_code);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DeviceFound
 *
 *  DESCRIPTION
 *      This function is called when a new device is discovered during the
 *      scan. It stores the information in the application data structure.
 *
 *  PARAMETERS
 *      disc_device [in]        Information for the discovered device
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void DeviceFound(DISCOVERED_DEVICE_T *disc_device);

/*----------------------------------------------------------------------------*
 *  NAME
 *      NotifyServiceFound
 *
 *  DESCRIPTION
 *      This function is used for notifying the application about the
 *      discovered service
 *
 *  PARAMETERS
 *      pService [in]           Discovered service callback function table
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void NotifyServiceFound(SERVICE_FUNC_POINTERS_T *pService);

/*----------------------------------------------------------------------------*
 *  NAME
 *      GetConnServices
 *
 *  DESCRIPTION
 *      This function returns a pointer to all the connected services
 *
 *  PARAMETERS
 *      dev [out]               Connected device number. Set to NULL if this
 *                              information is not required.
 *      totalServices [out]     Total number of connected services. Set to NULL
 *                              if this information is not required.
 *
 *  RETURNS
 *      Pointer to array of connected services' callback function tables
 *----------------------------------------------------------------------------*/
extern SERVICE_FUNC_POINTERS_T **GetConnServices(uint16 *dev,
                                                 uint16 *totalServices);

/*----------------------------------------------------------------------------*
 *  NAME
 *      SetState
 *
 *  DESCRIPTION
 *      This function is used to set the state of the application.
 *
 *  PARAMETERS
 *      dev [in]                Device number to change state for
 *      new_state [in]          New state to move to
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void SetState(uint16 dev, app_state new_state);

/*----------------------------------------------------------------------------*
 *  NAME
 *      GetState
 *
 *  DESCRIPTION
 *      This function returns the current state of the application for the 
 *      specified device. 
 *
 *  PARAMETERS
 *      dev [in]                Device number for which to return state
 *
 *  RETURNS
 *      Current state of the specified device
 *----------------------------------------------------------------------------*/
extern app_state GetState(uint16 dev);

/*----------------------------------------------------------------------------*
 *  NAME
 *      GetConnDevice
 *
 *  DESCRIPTION
 *      This function returns the connected device for which GATT procedure 
 *      is being performed. 
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Device number of currently connected device
 *----------------------------------------------------------------------------*/
extern uint16 GetConnDevice(void);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DisconnectDevice
 *
 *  DESCRIPTION
 *      Disconnect the specified device.
 *
 *  PARAMETERS
 *      dev [in]                Number of device to disconnect.
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void DisconnectDevice(uint16 dev);

/*----------------------------------------------------------------------------*
 *  NAME
 *      DeviceConfigured
 *
 *  DESCRIPTION
 *      Notifies the application that the specified device has been configured
 *      for all the connected services.
 *
 *  PARAMETERS
 *      dev [in]                Number of configured device
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void DeviceConfigured(uint16 dev);

#ifdef PAIRING_SUPPORT
/*----------------------------------------------------------------------------*
 *  NAME
 *      StartBonding
 *
 *  DESCRIPTION
 *      Start a timer that on expiry triggers the Pairing Procedure, if the
 *      peer device has a Resolvable Random Address and has not already
 *      initiated pairing by that time.
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void StartBonding(void);
#endif /* PAIRING_SUPPORT */

/*----------------------------------------------------------------------------*
 *  NAME
 *      NextReadWriteProcedure
 *
 *  DESCRIPTION
 *      This function initiates any read/write procedures. If argument 'next' is
 *      TRUE, it will initiate the next argument, otherwise it will initiate the 
 *      procedure for the current characteristic.
 *
 *  PARAMETERS
 *      next [in]               Whether to initiate the next argument
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
extern void NextReadWriteProcedure(bool next);


void timeWatch(void);
void sendKurulum(void);
void sleepHibernateIMAGE(void);
void sleepHibernate(void);
void sendSucInformation(void);
void sendSucInformationSetup(void);

uint8 readBatteryLevel(void);

#define DEAD_TIME ClientData.NewDeadTime * SECOND
#define SCAN_TIME ClientData.NewScanTime * MILLISECOND
#define BUTTON_TIMEOUT 1 * SECOND

timer_id tidPavo;
timer_id buttonPushTimer;
timer_id tidWatch;
timer_id kurulumTaramaTimeout;
timer_id kurulumHaberlesmeTimeout;
void scan(void);
void die(void);



typedef enum {
    WRITE=0,
    READ=1,
    ACK=2,
} tp_sistemDurumu;
tp_sistemDurumu sistemDurumu;

typedef struct 
{  char    b7:1;
   char    b6:1;
   char    b5:1;
   char    b4:1;
   char    b3:1;
   char    b2:1;
   char    b1:1;
   char    b0:1;
}tbitDf;


typedef union {
    tbitDf  bit;
    uint8   byte;
}tbyteDf;

extern tbyteDf ClientBit;

typedef enum {
    opBLE = 0,
    opIMAGE = 1,
    opBUTTON = 2,
    opBARCODE = 3,
    opKurulum = 4,
    opKurulumSuc = 5,
    opBARCODtoIMAGE = 6,
} OP_MODES;

typedef struct{
    uint8 firstTime;
    uint8 ReklamID;
    uint8 PageID;
    uint8 ByteSirasi;
    uint8 BitSirasi;
}tsConfigInfo;

typedef struct{
    uint8 ReklamID;
    uint8 PageID;
    uint8 ByteSirasi;
    uint8 BitSirasi;
    uint8 BatLevel;
    uint8 OldLike;
    uint8 OldDislike;
    uint8 NewTrnx[6];
    uint8 UsBitSirasi;
    uint16 NewDeadTime;
    uint8 NewScanTime;
    uint8 SablonID;
    uint16 StartPoint;
    uint16 EndPoint;
    uint8 KurulumState;
    uint8 DeadTimeEp;
    OP_MODES opMode;
    uint8 firsAdv;

}tsClientData;

extern tsClientData ClientData;
extern tsConfigInfo configInfo;




#endif /* __GATT_CLIENT_H__ */
