/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <main.h>           /* Functions relating to powering up the device */
#include <types.h>          /* Commonly used type definitions */
#include <timer.h>          /* Chip timer functions */
#include <mem.h>            /* Memory library */
#include <config_store.h>   /* Interface to the Configuration Store */
#include <gatt.h>           /* GATT application interface */
#include <ls_app_if.h>      /* Link Supervisor application interface */
#include <gap_app_if.h>     /* GAP application interface */
#include <buf_utils.h>      /* Buffer functions */
#include <security.h>       /* Security Manager application interface */
#include <panic.h>          /* Support for applications to panic */
#include <nvm.h>            /* Access to Non-Volatile Memory */
#include <random.h>         /* Generators for pseudo-random data sequences */
#include <pio.h>            /* PIO configuration and control functions */
#include <pio_ctrlr.h>      /* Access to the PIO controller */
#include <aio.h>            /* Analogue I/O configuration and control */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===================================================================
=========*
 *  Local Header Files
 *============================================================================*/
#include <gatt_uuid.h>      /* Common Bluetooth UUIDs and macros */


#include "gatt_access.h"    /* GATT-related routines */
#include "nvm_access.h"     /* Non-volatile memory access */
#include "gatt_client.h"    /* Interface to this file */
#include "gap_access.h"     /* GAP service interface */
#include "debug_interface.h"/* Application debug routines */

//#include "battery_service_data.h"   /* Battery Service interface */

#include "dev_info_service_data.h"  /* Device Info Service interface */
#include "new_service_data.h"

#include "platform_config.h"


//ekran surme
//#include "Pervasive_Displays_small_EPD.h"
#include "spi_master.h"

/*============================================================================*
 *  Private Definitions
 *============================================================================*/

/* Maximum number of timers */
#ifdef PAIRING_SUPPORT
/* 1 - Discovery Procedure and connecting state expiry timer
     * 2 - Bonding timer
     */
#define MAX_APP_TIMERS                 (10)
#else
/* 1 - Discovery Procedure and connecting state expiry timer */
#define MAX_APP_TIMERS                 (15)
#endif /* PAIRING_SUPPORT */

/* Possible error code reported by SM_SIMPLE_PAIRING_COMPLETE_IND event.
 * See Vol 2, Part D, section 2.6 for more details.
 */
#define PIN_OR_KEY_MISSING             (6)

/* This macro defines which key types should be excluded from NVM store */
#define INVALID_KEYS                   (1 << SM_KEY_TYPE_NONE | \
1 << SM_KEY_TYPE_SIGN)

/* Magic value to check the sanity of Non-Volatile Memory (NVM) region used by
 * the application. This value is unique for each application.
 */
#define NVM_SANITY_MAGIC               (0xABAB)

/* NVM offset for NVM sanity word */
#define NVM_OFFSET_SANITY_WORD         (0)

/* Total size required in NVM for each bonded device */
#define NVM_OFFSET_SIZE_EACH_DEV       (sizeof(g_app_data.devices[0].bonded)\
+ sizeof(g_app_data.devices[0].keys))

/* Calculate NVM offset for each bonded device */
#define NVM_OFFSET_DEV_NUM(x)          ((x) * NVM_OFFSET_SIZE_EACH_DEV)

/* NVM offset for bonded flag */
#define NVM_OFFSET_BONDED_FLAG(x)      (NVM_OFFSET_SANITY_WORD + 1 + \
NVM_OFFSET_DEV_NUM(x))

/* NVM offset for SM Keys */
#define NVM_OFFSET_SM_KEYS(x)          (NVM_OFFSET_BONDED_FLAG(x) + \
sizeof(g_app_data.devices[(x)].bonded))

/* Timer value for starting the Discovery Procedure once the connection is 
 * established. During this time pairing is initiated and completed, if pairing
 * is supported by the application or initiated by the peer device.
 */
#define DISCOVERY_START_TIMER           (300 * MILLISECOND)

/* Timer value for starting the Pairing Procedure once the connection is 
 * established. This has been done to facilitate completion of any GATT
 * procedure requiring the devices to be paired.
 */
#define PAIRING_TIMER_VALUE             (150 * MILLISECOND)

/* Maximum expected time for a connection to be established. */
#define CONNECTING_STATE_EXPIRY_TIMER   (15 * SECOND)

/* functions */
void uartTxDataCallback(void);
void setButton(void);
void HandlePIOChangedEvent(pio_changed_data *pio_data);
//void checkNotification(void);
void batarya_oku(void);
void sendConnected(void);
void TakeSquare(void);
void SuccesImageAck(void);
void uartTimerHandler(void);
void sendMigros(void);

void sleepHibernateBARCODtoIMAGE(void);
void MessageNack(void);
void ekranaYaz(uint8 IDs[],uint8 size, uint8 startPoint);
void ID_yaz(void);
void ID_Kurulum(void);
void KonsKontrol(void);
void have5SecondsPassed(void);
void have7SecondsPassed(void);
void BarcodeMode(void);
void WarmReset(void);

uint16 uartRxDataCallback(void *p_rx_buffer, uint16  length, uint16 *p_additional_req_data_length);

/* variables */
uint32 time = 0;

uint16 messageLength = 0;
uint16 dataCounter=0;
uint16 counter;

timer_id systemCheckTimer;
timer_id resettimer;
timer_id timerID;
timer_id ConnectionTimer;

uint8 DeadTimeCount=0;
uint8 messageType = 0;
uint8 checksum = 0;
uint8 WRvalue[160];
uint8 connectingCounter=0;
uint8 sendBufferMigros[50];
uint8 requestCounter = 0;
uint8 KonsCount=0;



bool currentMode=FALSE;
bool button = TRUE, fiveSecondsPassed = FALSE,SevenSecondsPassed = FALSE;

extern bool KonsBulundu;
extern bool imagefor;

extern uint8 CheckCounter;
//extern uint8 final[];
extern uint8 Bitmap0[];
extern uint8 Bitmap1[];
extern uint8 Bitmap2[];
extern uint8 Bitmap3[];
extern uint8 Bitmap4[];
extern uint8 Bitmap5[];
extern uint8 Bitmap6[];
extern uint8 Bitmap7[];
extern uint8 Bitmap8[];
extern uint8 Bitmap9[];
extern uint8 BitmapYUZDE[];

tsClientData ClientData;
tsConfigInfo configInfo;

//OP_MODES ClientData.opMode = opIMAGE;

time48 sleepTime = {0x00,0x10,0x00}; 

enum {
    eSOH,
            MessageType,
            MsgMSB,
            MsgLSB,
            Data,
            Checksum,
            eEOT
        } uartReceiveStates = eSOH;


uint16 uartRxDataCallback(void *p_rx_buffer, uint16 length, uint16 *p_additional_req_data_length) {
    return 0;
}

void sleepHibernate(void)
{
    /*
      Ýþlemciniz minimum 0x100000 usec uyumak zorunda. Hemen hemen 1 saniyeye denk geliyor.
      Bunun altýnda bir deðer ile fonksiyonu kullandýðýnýzda yine min uyuyacaðý süre kadar uyuyor.
      */
    SleepRequest(sleep_state_hibernate, FALSE, sleepTime);
}

void sleepHibernateIMAGE(void) { /* buraya hangi fonksiyon ile geldik? */
    ClientData.opMode = opIMAGE;
    NvmWrite(&ClientData,100,6100);
    /* ClientData.opMode flasha opIMAGE þeklinde yazýlýyor. */
    
    SleepRequest(sleep_state_hibernate, FALSE, sleepTime);
}

void sleepHibernateBARCODtoIMAGE(void) { /* buraya hangi fonksiyon ile geldik? */
    ClientData.opMode = opBARCODtoIMAGE;
    NvmWrite(&ClientData,100,6100);
    /* ClientData.opMode flasha opIMAGE þeklinde yazýlýyor. */
    
    SleepRequest(sleep_state_hibernate, FALSE, sleepTime);
}
/*============================================================================*
 *  Private Data types
 *============================================================================*/

/* Application data structure */
typedef struct _APP_DATA_T
{
    TYPED_BD_ADDR_T           conn_address;
    
    /* GATT connection indentifier */
    uint16                    cid;
    
    /* HCI connection handle */
    hci_connection_handle_t   hciHandle;
    
    /* Stores if the device was bonded */
    bool                      bonded;
    
    /* Bonded device address */
    TYPED_BD_ADDR_T           bonded_address;
    
    /* Pairing Key information */
    SM_KEYSET_T               keys;
    
    /* New keys are requested. Set TRUE when the other device was previously
     * bonded, but has since changed its pairing data
     */
    bool                      requestNewKeys;
    
    /* Set TRUE to request re-pairing if the is bonded */
    bool                      encryptAgain;
    
    /* Application state for the connected device */
    app_state                 state;
    
    /* Application timer ID */
    timer_id                   app_timer;
    
    /* Application bonding ID */
    timer_id                   bonding_timer;
    
    /* Timer to control LEDs */
    timer_id                   led_timer_tid;
    
    /* Timer to track scan duration */
    timer_id                   scan_timer_tid;
    
    /* Variabe to store the attribute handle value which is being
     * written or read.
     */
    uint16                     temp_handle;
    
    /* Application  preferred parameters */
    ble_con_params             pref_conn_param;
    
    /* Boolean flag to indicate whether the discovered handles are present in
     * NVM or not
     */
    bool                       remote_gatt_handles_present;
    /* Connected devices */
    DEVICE_T                   devices[MAX_CONNECTED_DEVICES];
    
    /* Application timer ID */
    //    timer_id                   app_timer;
    
#ifdef PAIRING_SUPPORT
    /* Application bonding ID */
    timer_id                   bonding_timer;
#endif /* PAIRING_SUPPORT */
    
    /* Currently connected device */
    uint16                     dev_num;
    
    /* Offset to NVM data for the current device */
    uint16                     nvm_dev_num;
    
    /* Number of connected devices */
    uint16                     num_conn;
} APP_DATA_T;

/*============================================================================*
 *  Private Data
 *============================================================================*/

/* List of supported services' callback function tables */
static SERVICE_FUNC_POINTERS_T *g_supported_services[] = {
    //  &BatteryServiceFuncStore,   /* Battery Service */
    //&DeviceInfoServiceFuncStore, /* Device Information Service */
    &NewServiceFuncStore
        };

/* Declare space for application timers */
static uint16 app_timers[SIZEOF_APP_TIMER * MAX_APP_TIMERS];

/* Application data instance */
static APP_DATA_T g_app_data;


/*============================================================================*
 *  Private Function Prototypes
 *============================================================================*/

/* Initiate the scanning process for the connected device */
static void appStartScan(void);

/* Initialise application data structure */
static void appDataInit(void);

/* Check and read if the NVM data contains the specified device */
static void checkPersistentStore(uint16 *nvmDevNum, TYPED_BD_ADDR_T bdAddress);

/* Initialise and read NVM data for each device */
static void readPersistentStore(uint16 dev_num, uint16 nvm_dev_num);

/* Store the NVM data. readPersistent store must have been called at least once
 * before calling this function.
 */
static void storeNvmData(void);

/* Exit the initialisation state */
static void appInitExit(uint16 dev);

/* Start the Discovery Procedure timer */
static void appStartDiscoveryProcedure(uint16 dev);

/* Start the Discovery Procedure */
static void appStartDiscoveryTimerExpiry(timer_id tid);

/* Request connection parameter update */
static void requestConnParamUpdate(uint16 dev);

/* Handle connection timeout */
static void appConnectingStateTimerExpiry(timer_id tid);

/* Find the device from the given connection handle */
static uint16 findDeviceByHciHandle(hci_connection_handle_t handle);

#ifdef PAIRING_SUPPORT
/* Start the Pairing Procedure */
static void appPairingTimerHandlerExpiry(timer_id tid);
#endif /* PAIRING_SUPPORT */

/* LM_EV_CONNECTION_COMPLETE signal handler */
static void handleSignalLmEvConnectionComplete(
        HCI_EV_DATA_ULP_CONNECTION_COMPLETE_T *p_event_data);

/* GATT_CONNECT_CFM signal handler */
static void handleSignalGattConnectCfm(GATT_CONNECT_CFM_T* p_event_data);

/* SM_KEY_REQUEST_IND signal handler */
static void handleSignalSmKeyRequestInd(SM_KEY_REQUEST_IND_T * p_event_data);

/* SM_KEYS_IND signal handler */
static void handleSignalSmKeysInd(SM_KEYS_IND_T *p_event_data);

/* SM_SIMPLE_PAIRING_COMPLETE_IND signal handler */
static void handleSignalSmSimplePairingCompleteInd(
        SM_SIMPLE_PAIRING_COMPLETE_IND_T *p_event_data);

/* LM_EV_DISCONNECTION_COMPLETE signal handler */
static void handleSignalLmDisconnectComplete(
        HCI_EV_DATA_DISCONNECT_COMPLETE_T *p_event_data);

/* GATT_CANCEL_CONNECT_CFM signal handler */
static void handleSignalGattCancelConnectCfm(void);

/* LS_CONNECTION_PARAM_UPDATE_CFM signal handler */
static void handleSignalLsConnectionParamUpdateCfm(
        LS_CONNECTION_PARAM_UPDATE_CFM_T *p_event_data);

void setButton(void) { /* tuþa basýldýðýnda çaðýrýlan fonksiyon */
    PioSetMode(DISLIKE_BUTTON, pio_mode_user);
    PioSetDir(DISLIKE_BUTTON, PIO_DIRECTION_INPUT); /* button_pio 11. pinde analog pin */
    PioSetPullModes(DISLIKE_BUTTON_MASK, pio_mode_strong_pull_down); //pull_up ?? pull_down
    PioSetEventMask(DISLIKE_BUTTON_MASK, pio_event_mode_both  ); /* basýlýnca da cekilince de calýssýn tus*/
    
    PioSetMode(LIKE_BUTTON, pio_mode_user);
    PioSetDir(LIKE_BUTTON, PIO_DIRECTION_INPUT); /* button_pio 11. pinde analog pin */
    PioSetPullModes(LIKE_BUTTON_MASK, pio_mode_strong_pull_down); //pull_up ?? pull_down
    PioSetEventMask(LIKE_BUTTON_MASK, pio_event_mode_both  ); /* basýlýnca da cekilince de calýssýn tus*/
    
    
    //buton_kontrol
    //    PioSetMode(BUTTON_PIO, pio_mode_user);
    //  PioSetDir(BUTTON_PIO, PIO_DIRECTION_INPUT); /* button_pio 11. pinde analog pin */
    //PioSetPullModes(BUTTON_PIO_MASK, pio_mode_weak_pull_down);
    //PioSetEventMask(BUTTON_PIO_MASK, pio_event_mode_both); /* basýlýnca da cekilince de calýssýn tus*/
    
    
}

void TakeSquare(void){
    int sayi=2, us, i, sonuc = 1;
    us=configInfo.BitSirasi;
    for (i = 0; i < us; i++)
        sonuc = sonuc * sayi;
    ClientData.UsBitSirasi=sonuc;
}

void SuccesImageAck(void) {
    
    uint8 _checksum = 0;
    uint16 i = 0, k;
    
    //SOH
    sendBufferMigros[i++] = 0x01;
    
    //Message Type
    sendBufferMigros[i++] = 0xD5; _checksum += 0xD5;
    
    //Message Length
    sendBufferMigros[i++] = 0x00; _checksum += 0x00;
    sendBufferMigros[i++] = 0x09; _checksum += 0x09;
    
    //Data
    sendBufferMigros[i++] = configInfo.ReklamID;     _checksum += configInfo.ReklamID;
    sendBufferMigros[i++] = configInfo.PageID;        _checksum += configInfo.PageID;
    sendBufferMigros[i++] = configInfo.ByteSirasi;    _checksum += configInfo.ByteSirasi;
    sendBufferMigros[i++] = configInfo.BitSirasi;     _checksum += configInfo.BitSirasi;
    sendBufferMigros[i++] = (ClientData.NewDeadTime/360)*ClientData.DeadTimeEp;     _checksum += (ClientData.NewDeadTime/360)*ClientData.DeadTimeEp;
    sendBufferMigros[i++] = ClientData.SablonID;     _checksum += ClientData.SablonID;
    
    sendBufferMigros[i++] = ClientData.BatLevel;            _checksum += ClientData.BatLevel;
    sendBufferMigros[i++] = ClientData.OldLike ;    _checksum += ClientData.OldLike;
    sendBufferMigros[i++] = ClientData.OldDislike;     _checksum += ClientData.OldDislike;
    
    for(k=0;k<5;k++) {sendBufferMigros[i++] = ClientData.NewTrnx[k]; _checksum += ClientData.NewTrnx[k];}
    sendBufferMigros[i++] = ClientData.NewScanTime / 100;     _checksum += ClientData.NewDeadTime / 100;
    
    
    //Checksum & EOT
    sendBufferMigros[i++] = _checksum;
    sendBufferMigros[i++] = 0x04;
    
    
    // for(j = 0; j < i; j++) DebugWriteChar(sendBufferMigros[j]);
    
    memcpy(WRvalue,sendBufferMigros,0x15);
    MemSet(sendBufferMigros,0,sizeof(sendBufferMigros));
    if(imagefor==TRUE){
        ClientData.OldLike=0;
        ClientData.OldDislike=0;
        NvmWrite(&ClientData,100,6100);
        imagefor=FALSE;}
    
    dataCounter=0x15;
    sistemDurumu = WRITE;
    
    
}

void sendMigros(void) {
    
    uint8 _checksum = 0;
    uint16 i = 0;
    
    //SOH
    sendBufferMigros[i++] = 0x01;
    
    //Message Type
    sendBufferMigros[i++] = 0xD3; _checksum += 0xD3;
    
    //Message Length
    sendBufferMigros[i++] = 0x00; _checksum += 0x00;
    sendBufferMigros[i++] = 0x04; _checksum += 0x04;
    
    //Data
    sendBufferMigros[i++] = configInfo.ReklamID;     _checksum += configInfo.ReklamID;
    sendBufferMigros[i++] = configInfo.PageID;        _checksum += configInfo.PageID;
    sendBufferMigros[i++] = configInfo.ByteSirasi;    _checksum += configInfo.ByteSirasi;
    sendBufferMigros[i++] = configInfo.BitSirasi;     _checksum += configInfo.BitSirasi;
    
    //Checksum & EOT
    sendBufferMigros[i++] = _checksum;
    sendBufferMigros[i++] = 0x04;
    // for(j = 0; j < i; j++) DebugWriteChar(sendBufferMigros[j]);
    
    
    memcpy(WRvalue,sendBufferMigros,0x0a);
    dataCounter=0x0a;
    sistemDurumu = WRITE;
    
    
}


void sendSucInformation(void) {
    uint8 _checksum = 0;
    uint16 i = 0;
    
    //SOH
    sendBufferMigros[i++] = 0x01;
    
    //Message Type
    sendBufferMigros[i++] = 0xD9; _checksum += 0xD9;
    
    //Message Length
    sendBufferMigros[i++] = 0x00; _checksum += 0x00;
    sendBufferMigros[i++] = 0x04; _checksum += 0x04;
    
    //Data
    sendBufferMigros[i++] = configInfo.ReklamID;     _checksum += configInfo.ReklamID;
    sendBufferMigros[i++] = configInfo.PageID;        _checksum += configInfo.PageID;
    sendBufferMigros[i++] = configInfo.ByteSirasi;    _checksum += configInfo.ByteSirasi;
    sendBufferMigros[i++] = configInfo.BitSirasi;     _checksum += configInfo.BitSirasi;
    
    //Checksum & EOT
    sendBufferMigros[i++] = _checksum;
    sendBufferMigros[i++] = 0x04;
    
    memcpy(WRvalue,sendBufferMigros,0x0a);
    dataCounter=0x0a;
    sistemDurumu = WRITE;
    
    //for(j = 0; j < i; j++) DebugWriteChar(sendBuffer[j]);
}

void sendSucInformationSetup(void) {
    uint8 _checksum = 0;
    uint16 i = 0;
    
    //SOH
    sendBufferMigros[i++] = 0x01;
    
    //Message Type
    sendBufferMigros[i++] = 0xDA; _checksum += 0xDA;
    
    //Message Length
    sendBufferMigros[i++] = 0x00; _checksum += 0x00;
    sendBufferMigros[i++] = 0x04; _checksum += 0x04;
    
    //Data
    sendBufferMigros[i++] = configInfo.ReklamID;     _checksum += configInfo.ReklamID;
    sendBufferMigros[i++] = configInfo.PageID;        _checksum += configInfo.PageID;
    sendBufferMigros[i++] = configInfo.ByteSirasi;    _checksum += configInfo.ByteSirasi;
    sendBufferMigros[i++] = configInfo.BitSirasi;     _checksum += configInfo.BitSirasi;
    
    //Checksum & EOT
    sendBufferMigros[i++] = _checksum;
    sendBufferMigros[i++] = 0x04;
    
    memcpy(WRvalue,sendBufferMigros,0x0a);
    dataCounter=0x0a;
    sistemDurumu = WRITE;
    
    //for(j = 0; j < i; j++) DebugWriteChar(sendBuffer[j]);
}
void MessageNack(void){
    
    uint8 _checksum = 0;
    uint16 i = 0;
    
    //SOH
    sendBufferMigros[i++] = 0x01;
    
    //Message Type
    sendBufferMigros[i++] = 0xE1; _checksum += 0xE1;
    
    //Message Length
    sendBufferMigros[i++] = 0x00; _checksum += 0x00;
    sendBufferMigros[i++] = 0x04; _checksum += 0x04;
    
    //Data
    sendBufferMigros[i++] = configInfo.ReklamID;     _checksum += configInfo.ReklamID;
    sendBufferMigros[i++] = configInfo.PageID;        _checksum += configInfo.PageID;
    sendBufferMigros[i++] = configInfo.ByteSirasi;    _checksum += configInfo.ByteSirasi;
    sendBufferMigros[i++] = configInfo.BitSirasi;     _checksum += configInfo.BitSirasi;
    
    //Checksum & EOT
    sendBufferMigros[i++] = _checksum;
    sendBufferMigros[i++] = 0x04;
    
    memcpy(WRvalue,sendBufferMigros,0x0a);
    dataCounter=0x0a;
    sistemDurumu = WRITE;
    
    //for(j = 0; j < i; j++) DebugWriteChar(sendBuffer[j]);
    
}


void sendConnected(void) {
    if(ClientData.opMode==opKurulum)
        sendKurulum();
    else
        sendMigros();
    
}


/*============================================================================*
 *  Private Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      appStartScan
 *
 *  DESCRIPTION
 *      Initiates the scanning process for the connected device
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Nothing
 *---------------------------------------------------------------------------*/
static void appStartScan(void)
{
    /* Number of services listed in g_supported_services */
    const uint16 size = sizeof(g_supported_services) /
                        sizeof(SERVICE_FUNC_POINTERS_T *);
    
    /* Start scanning for Servers advertising any supported service */
#ifdef FILTER_DEVICE_BY_SERVICE
    GattStartScan(size, g_supported_services, TRUE);
#else
    GattStartScan(size, g_supported_services, FALSE);
#endif /* FILTER_DEVICE_BY_SERVICE */
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      appDataInit
 *
 *  DESCRIPTION
 *      This function is called to initialise the application data structure
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Nothing
 *---------------------------------------------------------------------------*/
static void appDataInit(void)
{
    /* Initialise general application timer */
    if (g_app_data.app_timer != TIMER_INVALID)
    {
        TimerDelete(g_app_data.app_timer);
        g_app_data.app_timer = TIMER_INVALID;
    }
    
#ifdef PAIRING_SUPPORT
    /* Initialise bonding timer */
    if (g_app_data.bonding_timer != TIMER_INVALID)
    {
        TimerDelete(g_app_data.bonding_timer);
        g_app_data.bonding_timer = TIMER_INVALID;
    }
#endif /* PAIRING_SUPPORT */
    
    /* Initialise the application GATT data. */
    InitGattData();
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      checkPersistentStore
 *
 *  DESCRIPTION
 *      Check and read if the NVM data contains the specified device.
 *
 *  PARAMETERS
 *      nvmDevNum [out]         Device number
 *      bdAddress [in]          Bluetooth address of device to find in NVM
 *
 *  RETURNS
 *      Nothing
 *---------------------------------------------------------------------------*/
static void checkPersistentStore(uint16 *nvmDevNum, TYPED_BD_ADDR_T bdAddress) {
    uint16 dev = MAX_BONDED_DEVICES;    /* Found device number */
    uint16 nvm_sanity = 0xffff;         /* NVM sanity magic number */
    
    /* Check whether the NVM has been initialised yet by looking for the magic
     * number assigned to this application.
     */
    Nvm_Read(&nvm_sanity, sizeof(nvm_sanity), NVM_OFFSET_SANITY_WORD);
    
    if (nvm_sanity == NVM_SANITY_MAGIC)
    {
        /* Search the list of bonded devices stored in NVM for the supplied
         * Bluetooth address
         */
        for (dev = 0; dev < MAX_BONDED_DEVICES; dev++)
        {
            SM_KEYSET_T keys;
            TYPED_BD_ADDR_T addr;
            
            Nvm_Read((uint16*) &keys, sizeof(SM_KEYSET_T), NVM_OFFSET_SM_KEYS(dev));
            
            addr = keys.id_addr;
            
            if(!MemCmp(&addr, &bdAddress, sizeof(TYPED_BD_ADDR_T))) {
                /* Device matched */
                break;
            }
        }
    }
    
    *nvmDevNum = dev;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      readPersistentStore
 *
 *  DESCRIPTION
 *      Initialise and read NVM data.
 *
 *  PARAMETERS
 *      dev [in]                Device number
 *      nvm_dev_num [in]        Index to NVM data for specified device
 *
 *  RETURNS
 *      Nothing
 *---------------------------------------------------------------------------*/
static void readPersistentStore(uint16 dev, uint16 nvm_dev_num)
{
    uint16 nvm_sanity = 0xffff;         /* NVM sanity magic number */
    
    /* Read persistent storage to know if the device was last bonded
     * to another device
     */
    
    Nvm_Read(&nvm_sanity, sizeof(nvm_sanity), NVM_OFFSET_SANITY_WORD);
    
    if(nvm_sanity == NVM_SANITY_MAGIC) {
        if(dev == MAX_CONNECTED_DEVICES || nvm_dev_num == MAX_BONDED_DEVICES) {
            /* The NVM was initialised in its previous run and the application
             * is coming up again after a reset cycle. Do not store the bonding
             * information at this time
             */
            return;
        }
        
        /* Read Bonded Flag from NVM */
        Nvm_Read((uint16*)&g_app_data.devices[dev].bonded, sizeof(g_app_data.devices[dev].bonded),
                 NVM_OFFSET_BONDED_FLAG(nvm_dev_num));
        
        if(g_app_data.devices[dev].bonded)
        {
            /* Bonded Host Typed Bluetooth Address will only be stored if bonded
             * flag is set to TRUE. Read last bonded device address.
             */
            
            /* Link keys will only be stored only be stored if bonded flag
             * is set to TRUE. Read the link keys from the persistence store.
             */
            Nvm_Read((uint16*)&g_app_data.devices[dev].keys, sizeof(g_app_data.devices[dev].keys),
                     NVM_OFFSET_SM_KEYS(nvm_dev_num));
            
            /* Read last bonded device address */
            MemCopy(&g_app_data.devices[dev].address, &g_app_data.devices[dev].keys.id_addr,
                    sizeof(TYPED_BD_ADDR_T));
        }
        else /* Case when we have only written NVM_SANITY_MAGIC to NVM but
              * didn't get bonded to any host in the last powered session
              */
        {
            /* Any initialisation can be done here for non-bonded devices */
        }
    }
    else /* NVM Sanity check failed means either the device is being brought up
          * for the first time or NVM has been corrupted in which case discard
          * the data and start fresh.
          */
    {
        nvm_sanity = NVM_SANITY_MAGIC;
        
        /* Write NVM Sanity word to the NVM */
        Nvm_Write(&nvm_sanity,
                  sizeof(nvm_sanity),
                  NVM_OFFSET_SANITY_WORD);
        
        if(dev == MAX_CONNECTED_DEVICES &&
           nvm_dev_num == MAX_BONDED_DEVICES)
        {
            for(dev = 0; dev < MAX_CONNECTED_DEVICES; dev++)
            {
                /* Initialise bonded device flag */
                g_app_data.devices[dev].bonded = FALSE;
                
                /* Store bonded device flag in NVM */
                Nvm_Write((uint16 *)&g_app_data.devices[dev].bonded, sizeof(g_app_data.devices[dev].bonded),
                          NVM_OFFSET_BONDED_FLAG(dev));
            }
            
            for (; dev < MAX_BONDED_DEVICES; dev++) {
                /* If NVM has support for more than MAX_CONNECTED_DEVICES
                 * devices, initialise bonded device flag in NVM using the
                 * first device as a template.
                 */
                Nvm_Write((uint16 *)&g_app_data.devices[0].bonded, sizeof(g_app_data.devices[0].bonded),
                          NVM_OFFSET_BONDED_FLAG(dev));
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      storeNvmData
 *
 *  DESCRIPTION
 *      Store the NVM data. Should only be called after readPersistentStore has
 *      been called at least once.
 *
 *  PARAMETERS
 *      None
 *
 *  RETURNS
 *      Nothing
 *---------------------------------------------------------------------------*/
static void storeNvmData(void)
{
    uint16 nvm_sanity = 0xffff;         /* NVM sanity magic number */
    uint16 dev = 0;                     /* Device number */
    bool bondedFlag = FALSE;            /* Whether device is bonded */
    
    /* Check NVM to see whether the currently connected device has already
     * bonded with the Client. If it has then we assume the same keys are
     * used again.
     */
    checkPersistentStore(&dev, g_app_data.devices[(g_app_data.dev_num)].address);
    
    if(dev != MAX_BONDED_DEVICES)
    {
        /* Device data already exists in NVM. Implies device is already
         * paired.
         */
        if(dev != g_app_data.nvm_dev_num)
        {
            /* Update the offset to the device's bonding data in NVM */
            g_app_data.nvm_dev_num = dev;
        }
        
        /* Do not store the data again */
        return;
    }
    
    Nvm_Read(&nvm_sanity, sizeof(nvm_sanity), NVM_OFFSET_SANITY_WORD);
    
    if(nvm_sanity == NVM_SANITY_MAGIC)
    {
        /* If pairing data for the current device is not already stored in NVM,
         * then look for the first free slot in NVM to store the data in
         */
        for(dev = 0; dev < MAX_BONDED_DEVICES; dev++)
        {
            Nvm_Read((uint16*)&bondedFlag, sizeof(bondedFlag), NVM_OFFSET_BONDED_FLAG(dev));
            if(!bondedFlag) {
                g_app_data.nvm_dev_num = dev;
                break;
            }
        }
        
        if(dev == MAX_BONDED_DEVICES)
        {
            /* If the NVM has no room to store new bonded devices, overwrite
             * the last entry in the list.
             *
             * It may be preferrable to reject the pairing request if the list
             * is full instead.
             */
            g_app_data.nvm_dev_num = MAX_BONDED_DEVICES - 1;
        }
        
        /* Store the bonded flag */
        Nvm_Write((uint16*)&g_app_data.devices[(g_app_data.dev_num)].bonded,
                  sizeof(g_app_data.devices[(g_app_data.dev_num)].bonded),
                  NVM_OFFSET_BONDED_FLAG(g_app_data.nvm_dev_num));
        
        /* Store the Link keys */
        Nvm_Write((uint16*)&g_app_data.devices[(g_app_data.dev_num)].keys,
                  sizeof(g_app_data.devices[(g_app_data.dev_num)].keys),
                  NVM_OFFSET_SM_KEYS(g_app_data.nvm_dev_num));
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      appInitExit
 *
 *  DESCRIPTION
 *      This function is called upon exiting from app_state_init state. The
 *      application starts advertising after exiting this state.
 *
 *  PARAMETERS
 *      dev [in]                Device number
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void appInitExit(uint16 dev)
{
    /* Initialise data stored for this device */
    MemSet(&g_app_data.devices[dev], 0x0, sizeof(DEVICE_T));
    
    g_app_data.devices[dev].connectHandle = GATT_INVALID_UCID;
    g_app_data.devices[dev].hciHandle     = GATT_INVALID_UCID;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      appStartDiscoveryProcedure
 *
 *  DESCRIPTION
 *      Start the Discovery Procedure.
 *
 *  PARAMETERS
 *      dev [in]                Device number
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void appStartDiscoveryProcedure(uint16 dev)
{
    /* Enter the discovering state */
    SetState(dev, app_state_discovering);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      appStartDiscoveryTimerExpiry
 *
 *  DESCRIPTION
 *      This function is used to handle Discovery timer expiry.
 *
 *  PARAMETERS
 *      tid [in]                ID of timer that has expired
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void appStartDiscoveryTimerExpiry(timer_id tid)
{
    /* Device Number */
    const uint16 dev = g_app_data.dev_num;
    /* Connection handle */
    const uint16 connectHandle = g_app_data.devices[dev].connectHandle;
    
    if(tid == g_app_data.app_timer)
    {
        /* Timer has just expired, so mark it as invalid */
        g_app_data.app_timer = TIMER_INVALID;
        
        /* Start discovering the connected device's GATT database */
        if(!GattDiscoverRemoteDatabase(connectHandle))
        {
            /* No supported services found or Discovery Procedure failed */
            
            /* Disconnect the device */
            SetState(dev, app_state_disconnecting);
        }
    }
    /* Else it may be because of some race condition. Ignore it */
}

/*-----------------------------------------------------------------------------*
 *  NAME
 *      requestConnParamUpdate
 *
 *  DESCRIPTION
 *      This function is used to update the connection parameters to reduce
 *      current consumption after the Discovery Procedure completes.
 *
 *  PARAMETERS
 *      dev [in]                Number of device to request connection parameter
 *                              update from
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void requestConnParamUpdate(uint16 dev)
{
    /* New connection parameters */
    ble_con_params app_pref_conn_param =
    {
        PREFERRED_MIN_CON_INTERVAL,
                PREFERRED_MAX_CON_INTERVAL,
                PREFERRED_RUNNING_SLAVE_LATENCY,
                PREFERRED_SUPERVISION_TIMEOUT
            };
    
    /* Send a connection parameter update */
    if(LsConnectionParamUpdateReq(&g_app_data.devices[dev].address,
                                  &app_pref_conn_param))
    {
        ReportPanic(app_panic_con_param_update);
    }
    
    /* When the connection parameters have been updated the firmware will issue
     * a LS_CONNECTION_PARAM_UPDATE_CFM event, which causes this application to
     * move to the app_state_configured state.
     */
}

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

/*----------------------------------------------------------------------------*
 *  NAME
 *      appConnectingStateTimerExpiry
 *
 *  DESCRIPTION
 *      This function issues the GATT cancel connect request if the device is in
 *      this state for a long time
 *
 *  PARAMETERS
 *      tid [in]                ID of timer that has expired
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void appConnectingStateTimerExpiry(timer_id tid)
{
    /* Device Number */
    const uint16 dev = g_app_data.dev_num;
    if(tid == g_app_data.app_timer)
    {
        /* Timer has just expired, so mark it as invalid */
        g_app_data.app_timer = TIMER_INVALID;
        
        /* If we're still in the connecting state, cancel the connection */
        if(g_app_data.devices[dev].state == app_state_connecting)
        {
            GattCancelConnectReq();
        }
    }
    /* Else it may be because of some race condition. Ignore it */
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      findDeviceByHciHandle
 *
 *  DESCRIPTION
 *      This function finds the device in g_app_data by the given connection
 *      handle.
 *
 *  PARAMETERS
 *      handle [in]             Connection handle of device to find
 *
 *  RETURNS
 *      Corresponding device number.
 *----------------------------------------------------------------------------*/
static uint16 findDeviceByHciHandle(hci_connection_handle_t handle)
{
    uint16 dev_num;             /* Number of connected device */
    
    /* Browse through all devices */
    for(dev_num = 0; dev_num < MAX_CONNECTED_DEVICES; dev_num++)
    {
        if(g_app_data.devices[dev_num].hciHandle == handle)
            break;
    }
    
    return dev_num;
}

#ifdef PAIRING_SUPPORT
/*----------------------------------------------------------------------------*
 *  NAME
 *      appPairingTimerHandlerExpiry
 *
 *  DESCRIPTION
 *      This function handles the expiry of the pairing timer.
 *
 *  PARAMETERS
 *      tid [in]                ID of timer that has expired
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/

static void appPairingTimerHandlerExpiry(timer_id tid)
{
    /* Device Number */
    const uint16 dev = g_app_data.dev_num;
    
    if(g_app_data.bonding_timer == tid)
    {
        /* Timer has just expired, so mark it as invalid */
        g_app_data.bonding_timer = TIMER_INVALID;
        
        /* The bonding chance timer has expired. This means the remote has not
         * encrypted the link using old keys or pairing was not initiated.
         * Try initiating pairing request.
         */
        
        /* Handling signal as per current state */
        switch(g_app_data.devices[dev].state)
        {
        case app_state_connected:
        case app_state_discovering:
        case app_state_configured:
            {
                /* Initiate pairing */
                if(!GattIsAddressResolvableRandom(&g_app_data.devices[dev].address))
                {
                    SMRequestSecurityLevel(&g_app_data.devices[dev].address);
                }
            }
            break;
            
            default:
            /* Ignore timer in any other state */
            break;
        }
    }/* Else it may be due to some race condition. Ignore it. */
}
#endif /* PAIRING_SUPPORT */

/*---------------------------------------------------------------------------
 *  NAME
 *      handleSignalLmEvConnectionComplete
 *
 *  DESCRIPTION
 *      This function handles the signal LM_EV_CONNECTION_COMPLETE.
 *
 *  PARAMETERS
 *      p_event_data [in]       Data supplied by LM_EV_CONNECTION_COMPLETE
 *                              signal
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void handleSignalLmEvConnectionComplete(HCI_EV_DATA_ULP_CONNECTION_COMPLETE_T *p_event_data) {
    /* Device Number */
    const uint16 dev = g_app_data.dev_num;
    
    if(p_event_data->status == HCI_SUCCESS)
    {
        /* Connection complete */
        
        /* Connected device address is known, compare it to confirm */
        if(MemCmp(&g_app_data.devices[dev].address.addr, &p_event_data->peer_address, sizeof(BD_ADDR_T)) ||
           MemCmp(&g_app_data.devices[dev].address.type, &p_event_data->peer_address_type, sizeof(p_event_data->peer_address_type)) ||
           (p_event_data -> role == 0x1))             /* Master role */
        {
            /* Device address does not match, continue scanning */
            
            g_app_data.devices[dev].connected = FALSE;
            
            /* Restart the scanning */
            SetState(dev, app_state_scanning);
            
            return;
        }
        
        /* Increase the number of connections */
        g_app_data.num_conn++;
        
        /* Store the device details */
        g_app_data.devices[dev].connected = TRUE;
        
        g_app_data.devices[dev].hciHandle = p_event_data->connection_handle;
        
        TimerDelete(tidPavo);
        
        /* DebugIfWriteString("\r\n*** Connected to ");
//        DebugIfWriteBdAddress(&g_app_data.devices[dev].address);
//        DebugIfWriteString(" conn params (");
//        DebugIfWriteUint16(p_event_data->conn_interval);
//        DebugIfWriteString(" ");
//        DebugIfWriteUint16(p_event_data->conn_latency);
//        DebugIfWriteString(" ");
//        DebugIfWriteUint16(p_event_data->supervision_timeout);
//        DebugIfWriteString(")\r\n");
        */
        
        /* Nothing else to do till GATT_CONNECT_CFM event is received */
    }
    else
    {
        g_app_data.devices[dev].connected = FALSE;
        
        /*  DebugIfWriteString("\r\n*** Failed to connect to ");
//        DebugIfWriteBdAddress(&g_app_data.devices[dev].address);
//        DebugIfWriteString(" (HCI error code: 0x");
//        DebugIfWriteUint16(p_event_data->status);
//        DebugIfWriteString(")\r\n");*/
            
        /* Restart the scanning */
        SetState(dev, app_state_scanning);
    }
}

/*---------------------------------------------------------------------------
 *  NAME
 *      handleSignalGattConnectCfm
 *
 *  DESCRIPTION
 *      This function handles the signal GATT_CONNECT_CFM.
 *
 *  PARAMETERS
 *      p_event_data [in]       Data supplied by GATT_CONNECT_CFM signal
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void handleSignalGattConnectCfm(GATT_CONNECT_CFM_T *p_event_data)
{
    /* Device Number */
    const uint16 dev = g_app_data.dev_num;
    if(p_event_data->result == sys_status_success && /* Connection successful */
       /* Compare the address and its type */
       !(MemCmp(&g_app_data.devices[dev].address,
                &p_event_data->bd_addr,
                sizeof(TYPED_BD_ADDR_T))) &&
       /* Check if the device is connected */
       (g_app_data.devices[dev].connected))
    {
        
        /* Store connection handle */
        g_app_data.devices[dev].connectHandle = p_event_data->cid;
        //        DebugIfWriteString("Connected to ");
        //       DebugIfWriteBdAddress(&p_event_data->bd_addr);
        
        checkPersistentStore(&g_app_data.nvm_dev_num, p_event_data->bd_addr);
        
        if(g_app_data.nvm_dev_num < MAX_BONDED_DEVICES)
        {
            /* Read Persistent Store data and store it*/
            readPersistentStore(g_app_data.dev_num,
                                g_app_data.nvm_dev_num);
        }
        
        SetState(dev, app_state_connected);
    }
    else
    {
        /* Connection failed - remove the device from the list */
        g_app_data.devices[dev].connected = FALSE;
        
        /*        DebugIfWriteString("Failed to connect to ");
//        DebugIfWriteBdAddress(&p_event_data->bd_addr);
//        DebugIfWriteString("\r\n");*/
                  
        /* Restart scanning */
        SetState(dev, app_state_scanning);
    }
}

/*---------------------------------------------------------------------------
 *  NAME
 *      handleSignalSmKeyRequestInd
 *
 *  DESCRIPTION
 *      This function handles the signal SM_KEY_REQUEST_IND and passes the
 *      keys to the Security Manager, if previously paired and new keys have
 *      not been requested.
 *
 *  PARAMETERS
 *      p_event_data [in]       Data supplied by SM_KEY_REQUEST_IND signal
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void handleSignalSmKeyRequestInd(SM_KEY_REQUEST_IND_T *p_event_data)
{
    /* Keys stored for the peer device, if previously paired */
    SM_KEYSET_T *keys = NULL;
    /* Device Number */
    const uint16 dev = g_app_data.dev_num;
    
    if(g_app_data.devices[dev].bonded &&
       !g_app_data.devices[dev].requestNewKeys)
    {
        /* If the device is bonded, and new keys have not been requested, use
         * the valid keys fetched from NVM
         */
        keys = &g_app_data.devices[dev].keys;
    }
    
    /* Pass the keys to the SM */
    SMKeyRequestResponse(&g_app_data.devices[dev].address, keys);
}

/*---------------------------------------------------------------------------
 *  NAME
 *      handleSignalSmKeysInd
 *
 *  DESCRIPTION
 *      This function handles the signal SM_KEYS_IND.
 *
 *  PARAMETERS
 *      p_event_data [in]       Data supplied by SM_KEYS_IND signal
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void handleSignalSmKeysInd(SM_KEYS_IND_T *p_event_data)
{
    /* Device Number */
    const uint16 dev = g_app_data.dev_num;
    
    if(p_event_data->keys != NULL && /* Valid pointer */
       !(p_event_data->keys->keys_present & (uint16)INVALID_KEYS) &&
       (p_event_data->keys->keys_present & (uint16)(1 << SM_BD_ADDR)) &&
       ((g_app_data.devices[dev].bonded != TRUE) ||
        (g_app_data.devices[dev].requestNewKeys == TRUE)))
    {
        /* Store the new keys in application data structure */
        MemCopy(&g_app_data.devices[dev].keys,
                p_event_data->keys,
                sizeof(SM_KEYSET_T));
        
        if(g_app_data.devices[dev].requestNewKeys == TRUE)
        {
            /* Store the new keys in NVM */
            Nvm_Write((uint16*)&g_app_data.devices[(dev)].keys,
                      sizeof(g_app_data.devices[(dev)].keys),
                      NVM_OFFSET_SM_KEYS(g_app_data.nvm_dev_num));
        }
    }
}

/*---------------------------------------------------------------------------
 *  NAME
 *      handleSignalSmSimplePairingCompleteInd
 *
 *  DESCRIPTION
 *      This function handles the signal SM_SIMPLE_PAIRING_COMPLETE_IND.
 *
 *  PARAMETERS
 *      p_event_data [in]       Data supplied by SM_SIMPLE_PAIRING_COMPLETE_IND
 *                              signal
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void handleSignalSmSimplePairingCompleteInd(
        SM_SIMPLE_PAIRING_COMPLETE_IND_T *p_event_data)
{
    /* Device Number - to improve readability */
    const uint16 dev = g_app_data.dev_num;
    /* Handling signal as per current state */
    switch(g_app_data.devices[dev].state)
    {
    case app_state_discovering:
    case app_state_connected:
    case app_state_configured:
        {
            if(p_event_data->status == sys_status_success)
            {
                if(p_event_data->security_level == gap_mode_security_unauthenticate)
                {
#ifdef PAIRING_SUPPORT
                    /* Make sure pairing is not requested again by deleting the
                     * timer
                     */
                    if (g_app_data.bonding_timer != TIMER_INVALID)
                    {
                        TimerDelete(g_app_data.bonding_timer);
                        g_app_data.bonding_timer = TIMER_INVALID;
                    }
#endif /* PAIRING_SUPPORT */
                    
                    //                DebugIfWriteString("\r\n*** Pairing Completed ");
                    //                DebugIfWriteBdAddress(&p_event_data->bd_addr);
                    //                DebugIfWriteString("\r\n");
                    
                    if(GattServiceIncomplete() &&
                       ((!g_app_data.devices[dev].bonded) ||
                        g_app_data.devices[(dev)].encryptAgain))
                        /* Make sure that the device was initially not bonded
                         * and device is not being re-paired
                         */
                    {
                        /* Implies the Discovery Procedure is still
                         * incomplete
                         */
                        GattInitServiceCompletion(dev,
                                                  g_app_data.devices[dev].connectHandle);
                    }
                    else if(GattPairingInitiated())
                    {
                        /* Pairing was initiated due to insufficient
                         * authentication/authorisation. So continue the
                         * Discovery Procedure from where it left off
                         */
                        GattInitiateProcedureAgain(dev);
                    }
                    
                    g_app_data.devices[dev].requestNewKeys = FALSE;
                    
                    g_app_data.devices[dev].encryptAgain = FALSE;
                    
                    /* If the keys supplied during pairing are valid, then
                     * update the keys in NVM and record that the device is
                     * bonded.
                     */
                    if (!MemCmp(&p_event_data->bd_addr,
                                &g_app_data.devices[dev].keys.id_addr,
                                sizeof(TYPED_BD_ADDR_T)))
                    {
                        /* Store bonded host information in NVM. This includes
                         * application and service specific information
                         */
                        g_app_data.devices[dev].bonded = TRUE;
                        storeNvmData();
                    }
                }
            }
            else if(p_event_data->status == PIN_OR_KEY_MISSING)
                /* One of the error codes reported. Refer to Vol 2, Part D,
             * section 2.6 for more details
             */
            {
                /* Bonded flag will be updated again in DeviceFound */
                
                if(g_app_data.devices[(dev)].bonded) /* Device already bonded */
                {
                    g_app_data.devices[(dev)].requestNewKeys = TRUE;
                    
                    g_app_data.devices[(dev)].encryptAgain = TRUE;
                    
#ifdef PAIRING_SUPPORT
                    
                    /* Initiate pairing */
                    StartBonding();
                    
                    /*  DebugIfWriteString("\r\n*** Request pairing again ");
//                    DebugIfWriteBdAddress(&p_event_data->bd_addr);
//                    DebugIfWriteString("\r\n");*/
                        
#else /* !PAIRING_SUPPORT */
                        
                    /* Disconnect the link */
                    SetState(dev, app_state_disconnecting);
                    /*
//                    DebugIfWriteString("\r\n*** Disconnect - PIN/KEY missing");
//                    DebugIfWriteString(" found and pairing not supported");
                    */
                    /* Print the Bluetooth address */
                    /*  DebugIfWriteString("\r\n*** BD Address - ");
//                    DebugIfWriteBdAddress(&p_event_data->bd_addr);
                        
//                    DebugIfWriteString("\r\n");*/
                        
#endif /* PAIRING_SUPPORT */
                }
                else
                {
                    /* Bonded flag is false - update the NVM */
                    Nvm_Write((uint16*)&g_app_data.devices[dev].bonded,
                              sizeof(g_app_data.devices[dev].bonded),
                              NVM_OFFSET_BONDED_FLAG(g_app_data.nvm_dev_num));
                              
                    /* Disconnect the device */
                    SetState(dev, app_state_disconnecting);
                    
                    /* DebugIfWriteString("\r\n*** Disconnecting the ");
//                    DebugIfWriteBdAddress(&p_event_data->bd_addr);
//                    DebugIfWriteString("\r\n");*/
                }
            }
            else if (p_event_data->status == sm_status_pairing_not_supported)
            {
                /* DebugIfWriteString("\r\n*** Device ");
//                DebugIfWriteBdAddress(&p_event_data->bd_addr);
//                DebugIfWriteString(" Already bonded.");
//                DebugIfWriteString("\r\n*** Remove pairing to proceed.");
//                DebugIfWriteString("\r\n");*/
            }
        }
        break;
        
        default:
        break;
    }
}

/*---------------------------------------------------------------------------
 *  NAME
 *      handleSignalLmDisconnectComplete
 *
 *  DESCRIPTION
 *      This function handles LM_EV_DISCONNECTION_COMPLETE event which is
 *      received at the completion of disconnect procedure triggered either by
 *      the device or remote host or because of link loss
 *
 *  PARAMETERS
 *      p_event_data [in]       Data supplied by LM_EV_DISCONNECTION_COMPLETE
 *                              signal
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void handleSignalLmDisconnectComplete(
        HCI_EV_DATA_DISCONNECT_COMPLETE_T *p_event_data)
{
    uint16 dev_discon;              /* Number of disconnected device */
    uint16 dev;                     /* Loop counter */
    bool initiateScanning;          /* Whether to start scanning */
    
    /* Find device number of disconnected device */
    dev_discon = findDeviceByHciHandle(p_event_data->handle);
    
    if(ClientData.opMode!=opKurulumSuc && currentMode==TRUE){
        //#ifdef DEBUG_OUTPUT_ENABLED
        //        DebugIfWriteString("\r\ndisconnecting...\r\n");
        //#endif
        /*TimerDelete(resettimer);
        resettimer=TimerCreate(100*MILLISECOND,TRUE,sleepHibernate);
        if(resettimer == TIMER_INVALID){WarmReset();}
        currentMode=FALSE;*/
        sleepHibernate();
    }
    else if(ClientData.opMode==opKurulumSuc) sleepHibernate();
    
    connectingCounter++;
    
    //DebugIfWriteInt(connectingCounter);
    if(connectingCounter>100) {
        //DebugIfWriteString("\r\n*\r\n");
        connectingCounter=0;
        TimerDelete(tidPavo);
        tidPavo = TimerCreate(10*MILLISECOND, TRUE, (timer_callback_arg) WarmReset);
        if(tidPavo == TIMER_INVALID){WarmReset();}
    }
    
    if(dev_discon < MAX_CONNECTED_DEVICES)
    {
        /*  DebugIfWriteString("\r\n*** Disconnected from ");
//                DebugIfWriteBdAddress(&g_app_data.devices[dev_discon].address);*/
            
        /* Reset all the service data, connected/discovered for this device */
        GattResetAllServices(dev_discon);
        
        /* Reset the data in the device record */
        MemSet(&g_app_data.devices[dev_discon], 0x0, sizeof(DEVICE_T));
        
        g_app_data.devices[dev_discon].connectHandle = GATT_INVALID_UCID;
        g_app_data.devices[dev_discon].hciHandle = GATT_INVALID_UCID;
        
        /* Decrease the number of connected peripheral devices */
        if(g_app_data.num_conn)
            g_app_data.num_conn--;
            
        /* Check the state of all the other devices to see whether scanning
         * should be initiated for the disconnected device number
         */
        for (dev = 0, initiateScanning = TRUE; (dev < MAX_CONNECTED_DEVICES) && initiateScanning;
        dev++)
        {
            if(dev != dev_discon)
            {
                switch(g_app_data.devices[dev].state)
                {
                case app_state_connecting:
                case app_state_scanning:
                case app_state_connected:
                case app_state_discovering:
                    {
                        /* As one of the devices is already in one of the above
                         * states, we cannot initialise scanning for the
                         * disconnected device number.
                         */
                        initiateScanning = FALSE;
                    }
                    break;
                    
                case app_state_init:
                case app_state_configured:
                    {
                        /* Safe to start scanning for the disconnected device
                         * number.
                         */
                    }
                    break;
                    
                    default:
                    {
                        /* Control should never come here */
                        ReportPanic(app_panic_invalid_state);
                    }
                    break;
                }
            }
        }
        
        if(initiateScanning)
        {
            /* No other device is being configured, so initiate scanning on this
             * device number.
             */
            SetState(dev_discon, app_state_scanning);
        }
        else
        {
            /* Move to the init state on this device number, as one of the
             * other devices is still being configured.
             */
            SetState(dev_discon, app_state_init);
        }
    }
    
}
/*---------------------------------------------------------------------------
 *  NAME
 *      handleSignalGattCancelConnectCfm
 *
 *  DESCRIPTION
 *      This function handles the signal GATT_CANCEL_CONNECT_CFM.
 *
 *  PARAMETERS
 *      p_event_data [in]       Data supplied by GATT_CANCEL_CONNECT_CFM signal
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void handleSignalGattCancelConnectCfm(void)
{
    /* This event was received after the application was in app_state_connecting
     * for too long, so move back to the app_state_scanning state.
     */
    SetState(g_app_data.dev_num, app_state_scanning);
}

/*---------------------------------------------------------------------------
 *  NAME
 *      handleSignalLsConnectionParamUpdateCfm
 *
 *  DESCRIPTION
 *      This function handles the signal LS_CONNECTION_PARAM_UPDATE_CFM
 *      received when the connection parameter update requested by the Master
 *      completes.
 *
 *  PARAMETERS
 *      p_event_data [in]       Data supplied by LS_CONNECTION_PARAM_UPDATE_CFM
 *                              signal
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
static void handleSignalLsConnectionParamUpdateCfm(
        LS_CONNECTION_PARAM_UPDATE_CFM_T *p_event_data)
{
    /* The connection parameters are updated when the Discovery Procedure and
     * service configuration are complete. If the update request was successful
     * then the connection parameters resulting in lower current consumption are
     * in effect. If the request failed then more current is being consumed than
     * is necessary, but the device is still connected and working properly, so
     * just report a warning but otherwise continue to the app_state_configured
     * state.
     */
    if (p_event_data->status != sys_status_success)
    {
        //#ifdef DEBUG_OUTPUT_ENABLED
        //        DebugIfWriteString("\r\nConnection parameter update request failed on " "device ");
        //        DebugIfWriteBdAddress(&g_app_data.devices[g_app_data.dev_num].address);
        //#endif
    }
    
    SetState(g_app_data.dev_num, app_state_configured);
}

/*============================================================================*
 *  Public Function Implementations
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
void ReportPanic(app_panic_code panic_code)
{
    /* Raise panic */
    Panic(panic_code);
}

/*----------------------------------------------------------------------------*
 *  NAME
ID YAZ
 *----------------------------------------------------------------------------*/
void ekranaYaz(uint8 IDs[],uint8 size, uint8 startPoint){
    //    int max=0;
    int say=0;
    uint8 u;
    uint8 gecici;
    uint8 z=0;
    
    uint8 ustsinir = startPoint; //soldan sýnýrlama, çünkü aynalama var
    uint8 ustsinir_ = ustsinir;
    
    bool onKont = FALSE;
    for(say=size - 1 ;say>=0;){
        /*
        DebugIfWriteString("IDs: ");
        DebugIfWriteUint8(IDs[say]);
        DebugIfWriteString("\r\n");
       */ 
        uint8 Num = 0;
        
        if  (IDs[say] > 9 && IDs[say] != 0xff &&  IDs[say] != 0xfe && IDs[say] <= 99){
            
            if(onKont == FALSE){
                Num = IDs[say] % 10;
                onKont = TRUE;
            }else{
                Num = IDs[say] / 10;
                say--;
                onKont = FALSE;
            }
            
        }else if( IDs[say] > 99 &&  IDs[say] != 0xff &&  IDs[say] != 0xfe && IDs[say] <= 255 ){
            /*dec olarak iki basamak üstünü henüz desteklememekte */
            
            uint8 tempNum =      IDs[say];
            uint8 basamakSayisi = 3;
            uint8 kalanDeger = 0;
            uint8 NumThree[]={0,0,0};
            
            while ( tempNum > 0 ){
                basamakSayisi--;
                kalanDeger = tempNum % 10;
                tempNum /= 10;    
                NumThree[basamakSayisi] = kalanDeger;
            }
            ekranaYaz(NumThree +  basamakSayisi ,3 - basamakSayisi,startPoint);   
            
            say--;
            
            continue;
            
        }else{
            Num = IDs[say];
            say--;
        }
        /*   DebugIfWriteString("Num: ");
//                DebugIfWriteUint8(Num);
//                DebugIfWriteString("\r\n");
                */
        
        
        for(u=0;u<39;u++){
            z++;
            if(u%3==0) {z=0; ustsinir=ustsinir+22;}
            
            switch(Num){
            case 0:
                gecici = Bitmap0[u];
                break;
            case 1:
                gecici = Bitmap1[u];
                break;
            case 2:
                gecici = Bitmap2[u];
                break;
            case 3:
                gecici = Bitmap3[u];
                break;
            case 4:
                gecici = Bitmap4[u];
                break;
            case 5:
                gecici = Bitmap5[u];
                break;
            case 6:
                gecici = Bitmap6[u];
                break;
            case 7:
                gecici = Bitmap7[u];
                break;
            case 8:
                gecici = Bitmap8[u];
                break;
            case 9:
                gecici = Bitmap9[u];
                break;
                
            case 0xfe:
                gecici = BitmapYUZDE[u];
                break;
                
            case 0xff:
                break;
                
                default:
                break;
            }
            final[ustsinir+z] = gecici;
        }
        ustsinir_+=330;
        ustsinir=ustsinir_;
        // altsinir=altsinir+3;
    }
    
}

void ID_yaz(void){
    
    uint8 IDs1[] = { configInfo.ReklamID    };
    uint8 IDs2[] = { configInfo.PageID      };
    uint8 IDs3[] = { configInfo.ByteSirasi  };
    uint8 IDs4[] = { configInfo.BitSirasi   };
    
    ekranaYaz(IDs1 , sizeof(IDs1), 4862 );
    ekranaYaz(IDs2 , sizeof(IDs2), 4334 - 176);
    ekranaYaz(IDs3 , sizeof(IDs3), 3630 - (2* 176));
    ekranaYaz(IDs4 , sizeof(IDs4), 2926 - (3*176));
}

void ID_Kurulum(void){
    uint8 IDs1[] = { 0xfe };
    
    ekranaYaz(IDs1 , sizeof(IDs1), 5038 );
    ekranaYaz(IDs1 , sizeof(IDs1), 4334 - 176);
    ekranaYaz(IDs1 , sizeof(IDs1), 3630 - (2* 176));
    ekranaYaz(IDs1 , sizeof(IDs1), 2926 - (3*176));  
}

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
void DeviceFound(DISCOVERED_DEVICE_T *disc_device) {
    uint16 dev;                     /* Loop counter */
    /* Add device to the list of connections */
    for(dev = 0; dev < MAX_CONNECTED_DEVICES; dev++)
    {
        /* Find next unoccupied slot in the device list */
        if(!(g_app_data.devices[dev].connected) &&
           g_app_data.devices[dev].state == app_state_scanning)
        {
            break;
        }
    }
    
    /* No free slots found */
    if(dev == MAX_CONNECTED_DEVICES)
    {
        return;
    }
    
    /* Store the connected device number */
    g_app_data.dev_num = dev;
    
    /* Store the device details */
    MemCopy(&g_app_data.devices[dev].address,
            &disc_device->address,
            sizeof(TYPED_BD_ADDR_T));
    
    /* Stop scanning for advertisements */
    LsStartStopScan(FALSE,
                    whitelist_disabled,
                    ls_addr_type_public);
    
    /* Start the connection to the device */
    
    /* One can choose to pass the connection parameters requested by the slave
     * (if bonded)
     */
    
    /* In the example application the connection parameters (400ms, 400ms, 1,
     * 1000) will not be stored.
     */
    GapSetDefaultConnParams(NULL);
    
    /* Move to the connecting state */
    SetState(dev, app_state_connecting);
    
    /* Create a timer to cancel the connection request if it takes too long.
     * If the application does not leave the connecting state within
     * CONNECTING_STATE_EXPIRY_TIMER microseconds call
     * appConnectingStateTimerExpiry to issue a GATT cancel connect request.
     */
    if (g_app_data.app_timer != TIMER_INVALID)
    {
        TimerDelete(g_app_data.app_timer);
    }
    g_app_data.app_timer = TimerCreate(CONNECTING_STATE_EXPIRY_TIMER, TRUE,
                                       appConnectingStateTimerExpiry);
    
    /* Send the connection request */
    GattConnectReq(&g_app_data.devices[dev].address, L2CAP_CONNECTION_MASTER_DIRECTED | L2CAP_PEER_ADDR_TYPE_PUBLIC);
}

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
void NotifyServiceFound(SERVICE_FUNC_POINTERS_T *pService)
{
    /* Device Number */
    const uint16 dev = g_app_data.dev_num;
    /* Total number of connected services */
    uint16 *totalServices = &g_app_data.devices[dev].totalConnectedServices;
    
    if(*totalServices < MAX_SUPPORTED_SERV_PER_DEVICE && pService != NULL)
    {
        /* Populate the service database for the connected device */
        g_app_data.devices[g_app_data.dev_num].
                connected_services[*totalServices] = pService;
        
        /* Increment total number of connected services */
        (*totalServices)++;
    }
}

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
SERVICE_FUNC_POINTERS_T **GetConnServices(uint16 *dev, uint16 *totalServices)
{
    /* Device Number */
    const uint16 dev_conn = g_app_data.dev_num;
    
    if(dev != NULL)
    {
        *dev = dev_conn; /* Return the connected device number */
    }
    
    if(totalServices != NULL)
    {
        /* Return the total number of services */
        *totalServices = g_app_data.devices[dev_conn].totalConnectedServices;
    }
    
    return g_app_data.devices[dev_conn].connected_services;
}

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
void SetState(uint16 dev, app_state new_state)
{
    /* Current state */
    const app_state old_state = g_app_data.devices[dev].state;
    
    /* Check if the new state to be set is not the same as the present state */
    if (old_state != new_state)
    {
        /* Handle exiting old state */
        switch (old_state)
        {
        case app_state_init:
            {
                appInitExit(dev);
            }
            break;
            
        case app_state_disconnecting:
            {
                
                /* Common things to do whenever the application exits the
                 * disconnecting state.
                 */
                
                /* This may involve freeing resources allocated to the device
                 * and reseting application data.
                 */
            }
            break;
            
        case app_state_connected:
            {
                /* The application may need to maintain the values of some
                 * profile specific data across connections and power cycles.
                 * These values may have changed in 'connected' state, so the
                 * application may update data stored in the NVM here.
                 */
            }
            break;
            
        case app_state_discovering:
            {
                /* Nothing to do */
            }
            break;
            
            default:
            {
                /* Nothing to do */
            }
            break;
        }
        
        /* Set new state */
        g_app_data.devices[dev].state = new_state;
        
        /* Handle entering new state */
        switch (new_state)
        {
        case app_state_scanning:
            {
                /* Start scanning for advertising devices */
                //DebugIfWriteString("\r\nScanning for devices...\r\n");
                /* Reset application data */
                appDataInit();
                
                /* Start scanning */
                appStartScan();
            }
            break;
            
        case app_state_connecting:
            {
                /* This state has been introduced between app_state_connected
                 * and app_state_scanning so that if the connection does not
                 * succeed we can revert to app_state_scanning.
                 */
            }
            break;
            
        case app_state_connected:
            {
                /* Common things to do whenever the application enters
                 * app_state_connected state.
                 */
#ifdef PAIRING_SUPPORT                
                /* Start a timer to trigger the Pairing Procedure after
                 * PAIRING_TIMER_VALUE ms if the remote device is using a
                 * resolvable random address and has not initiated pairing.
                 */
                StartBonding();
#endif /* PAIRING_SUPPORT */                             
                /* Move to the app_state_discovering state */
                appStartDiscoveryProcedure(dev);
            }
            break;
            
        case app_state_discovering:
            {
                /* Application enters this state from app_state_connected. In
                 * this state the application initiates the Discover Procedure.
                 */
                
                if (g_app_data.app_timer != TIMER_INVALID)
                {
                    TimerDelete(g_app_data.app_timer);
                }
                /* Discovery will start in DISCOVERY_START_TIMER ms */
                g_app_data.app_timer = TimerCreate(DISCOVERY_START_TIMER, TRUE,
                                                   appStartDiscoveryTimerExpiry);
                
            }
            break;
            
        case app_state_configured:
            {
                /* Peer device has been configured for all the supported
                 * services */
                //#ifdef DEBUG_OUTPUT_ENABLED
                //            DebugIfWriteString("\r\nPeer device is Configured...\r\n");
                //            DebugIfWriteInt(connectingCounter);
                //#endif
                sendConnected();
                TimerDelete(tidWatch); /* debug olunca ekrana saniye saniye sayýyor  */
                LsStartStopScan(FALSE, whitelist_disabled, ls_addr_type_public); /* baðlandýðý için reklamýný kapattý. */
                time = 0;
                TimerDelete(tidPavo);
                NvmWrite(&ClientData,100,6100); /*baglanti koparsa deadtime guncel yedeklensin diye*/
                
                TimerDelete(ConnectionTimer);
                
                if(ClientData.opMode!=opKurulum)ConnectionTimer=TimerCreate(45*SECOND, TRUE, (timer_callback_arg) sleepHibernate);
                else ConnectionTimer=TimerCreate(10*SECOND, TRUE, (timer_callback_arg) sleepHibernate);
                
                if(ConnectionTimer == TIMER_INVALID){WarmReset();}
                
                currentMode = TRUE;
                sistemDurumu = WRITE;
                NextReadWriteProcedure(TRUE);
            }
            break;
            
        case app_state_disconnecting:
            {
                /* Disconnect the link */
                TimerDelete(ConnectionTimer);
                GattDisconnectReq(g_app_data.devices[dev].connectHandle);
            }
            break;
            
            default:
            {
                /* Unhandled state */
                ReportPanic(app_panic_invalid_state);
            }
            break;
        }
    }
}

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
app_state GetState(uint16 dev)
{
    return g_app_data.devices[dev].state;
}


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
uint16 GetConnDevice(void)
{
    return g_app_data.dev_num;
}

void DisconnectDevice(uint16 dev)
{
    SetState(dev, app_state_disconnecting);
}

void DeviceConfigured(uint16 dev)
{
    /* Update the connection parameters to reduce current
     * consumption.
     */
    requestConnParamUpdate(dev);
}

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
void StartBonding(void)
{
    if (g_app_data.bonding_timer != TIMER_INVALID)
    {
        TimerDelete(g_app_data.bonding_timer);
    }
    g_app_data.bonding_timer = TimerCreate(PAIRING_TIMER_VALUE,
                                           TRUE, appPairingTimerHandlerExpiry);
}
#endif /* PAIRING_SUPPORT */


void NextReadWriteProcedure(bool next) {
    
    SERVICE_FUNC_POINTERS_T *pService;
    const uint16 uuid = UUID_NEW_SERVICE;
    uint16 char_type = new_level;
    pService = GattFindServiceByUuid(GATT_UUID16, &uuid);
    uint16 i=0;
    uint16 advSize = 20;
    // NvmWrite(&tem,2,NVM_OFFSET_SANITY_PAVO);
    
    switch(sistemDurumu){
    case WRITE:
        while(i < dataCounter){
            uint8 temp[20];
            uint16 u=0;
            MemSet(temp,0,20);
            for(u=0;u<20;u++ ){
                if(u+i >= dataCounter) break;
                temp[u]=WRvalue[u+i];
                //  DebugWriteChar(WRvalue[u+i]);
                if(u+i >= 160) break;
            }
            if(u<20) advSize = u;
            GattWriteRequest(g_app_data.dev_num, pService, char_type, temp, advSize);
            i=u+i;
            if(i>=160) break;
            // delay_ms(5);
            //  DebugIfWriteString("1");
            
        }
        dataCounter=0;
        MemSet(WRvalue,0,sizeof(WRvalue));
        sistemDurumu = READ;
        TimerDelete(systemCheckTimer);
        
        break;
    case READ:
        GattReadRequest(g_app_data.dev_num, pService, char_type);
        break;
        default:
        //#ifdef DEBUG_OUTPUT_ENABLED
        //        DebugIfWriteString("bilinmeyen durum");
        //#endif
        break;
    }
    
}

void timeWatch(void) {
#ifdef DEBUG_OUTPUT_ENABLED    
    DebugIfWriteUint32(time);
#endif
    time++;
    TimerDelete(tidWatch);
    tidWatch = TimerCreate(SECOND, TRUE, (timer_callback_arg) timeWatch);
    if(tidWatch == TIMER_INVALID){WarmReset();}
    
    
}

void KonsKontrol(void){
#ifdef DEBUG_OUTPUT_ENABLED
    DebugIfWriteString("KonsKontrol\r\n");
#endif
    if(KonsBulundu==TRUE && KonsCount<5 ) scan();
    else die();
}
/*
  Scan iþlemini bitir.
  */
void die(void) {
    
#ifdef DEBUG_OUTPUT_ENABLED
    DebugIfWriteString("die \r\n");
#endif
    
    PioSetDir( RESET_PIN, INPUT );  //pull_up direnc var
    
    
    Nvm_Disable();
    
    LsStartStopScan(FALSE, whitelist_disabled, ls_addr_type_public); /* scanning'i kapat */
    CheckCounter=0;
    KonsCount=0;
    KonsBulundu=FALSE;
    
    if(DeadTimeCount>1){
        DeadTimeCount--;
        TimerDelete(tidPavo);
        tidPavo = TimerCreate(DEAD_TIME, TRUE, (timer_callback_arg) die);
        if(tidPavo == TIMER_INVALID){WarmReset();}
    }
    else{
        DeadTimeCount=ClientData.DeadTimeEp;
        TimerDelete(tidPavo);
        tidPavo = TimerCreate(DEAD_TIME, TRUE, (timer_callback_arg) scan); /* DEAD_TIME boyunca ölü kal , hiçbir þey yapma3 ardýndan scan fonksiyonuna geç */
        if(tidPavo == TIMER_INVALID){WarmReset();}
        
    }
#ifdef DEBUG_OUTPUT_ENABLED
    timeWatch();
#endif
    
}

/*
  Scan iþlemini baþlat
  */
void scan(void) {
    LsStartStopScan(TRUE, whitelist_disabled, ls_addr_type_public); /* scanning moduna geç */
    TimerDelete(tidPavo);
    KonsCount++;
    if( ClientData.opMode == opKurulum) tidPavo = TimerCreate(SCAN_TIME, TRUE, (timer_callback_arg) die);
    else tidPavo = TimerCreate(SCAN_TIME, TRUE, (timer_callback_arg) KonsKontrol);
    if(tidPavo == TIMER_INVALID){WarmReset();}
    /* #ifdef DEBUG_OUTPUT_ENABLED
//            DebugIfWriteString("+\r\n");
            TimerDelete(tidWatch);
            time = 0;
#endif
            */
}

void have5SecondsPassed(void) {
    //        DebugIfWriteString("BARCODEEE\r\n");
    ClientData.opMode = opBARCODE;
    NvmWrite(&ClientData,100,6100);
    TimerCreate(10 * MILLISECOND, TRUE, (timer_callback_arg) sleepHibernate);  /* 10ms sonra barcode modunda diye reset atsýn*/
}

void have7SecondsPassed(void) {
    //        DebugIfWriteString("Kurulum MOdu Aktif\r\n");
    ClientData.opMode = opKurulum;
    NvmWrite(&ClientData,100,6100);
    TimerCreate(10 * MILLISECOND, TRUE, (timer_callback_arg) sleepHibernate);  /* 10ms sonra barcode modunda diye reset atsýn*/
}

/*
  Buton interrupt handler
  Butonlardan herhangi birine basýldýysa çaðýrýlýyor.
  Hangisine basýldýðýný analog pinleri okuyarak test edebilirsiniz.
  */
void HandlePIOChangedEvent(pio_changed_data *pio_data) {  /* tuþa basýldýðýn da bu interupta geliyor*/
    if(button) { /* burasý bir kere butona bastýktan sonra eðer butona basýlýrsa tekrar tekrar butona ilk defa basýlmýþ gibi davranýlmasýn diye */
        button = FALSE; /* Butondan gelen interrupt larý ignore et */
        
        TimerDelete(buttonPushTimer);
        
        if(ClientData.opMode == opBARCODE) {
            
            if( PioGet(DISLIKE_BUTTON) ) {
                
                ClientData.opMode = opBUTTON;
                NvmWrite(&ClientData,100,6100);
                TimerDelete(tidPavo); /*Tuþa basýlýrsa tidPavo'ý sil*/
                TimerCreate(100 * MILLISECOND, TRUE, (timer_callback_arg) sleepHibernate);   
            }
            
            else if( PioGet(LIKE_BUTTON) ){
                SevenSecondsPassed= FALSE;
                buttonPushTimer = TimerCreate(1 * SECOND, TRUE, (timer_callback_arg) have7SecondsPassed);
            }
            
            else if( !PioGet(DISLIKE_BUTTON) && !SevenSecondsPassed && !PioGet(LIKE_BUTTON) ){
                TimerDelete(buttonPushTimer);
                
                //#ifdef DEBUG_OUTPUT_ENABLED
                //                    DebugIfWriteString("\r\n");
                //                    DebugIfWriteString("NO 7 seconds\r\n");
                //#endif
            }
        }
        else if(  !PioGet(DISLIKE_BUTTON) && fiveSecondsPassed){ /* 7saniye basýlý tuttu ve býraktýysa */
            //#ifdef DEBUG_OUTPUT_ENABLED
            //                DebugIfWriteString("7 seconds\r\n");
            //#endif                
            have7SecondsPassed();
        }
        else {
            if( PioGet(DISLIKE_BUTTON) ) {
                fiveSecondsPassed = FALSE;
                buttonPushTimer = TimerCreate(1 * SECOND, TRUE, (timer_callback_arg) have5SecondsPassed);
                ClientData.OldDislike =  ClientData.OldDislike + 1;
                //#ifdef DEBUG_OUTPUT_ENABLED
                //                    DebugIfWriteString("\r\n");
                //                    DebugIfWriteInt(ClientData.OldDislike);
                //                    DebugIfWriteString("  ClientData.OldDislike \r\n");
                //#endif
                NvmWrite(&ClientData,100,6100);
                delay_ms(1500);
            }
            else if( PioGet(LIKE_BUTTON) ){
                
                ClientData.OldLike = ClientData.OldLike + 1;
                
                //#ifdef DEBUG_OUTPUT_ENABLED
                //                    DebugIfWriteString("\r\n");
                //                    DebugIfWriteInt(ClientData.OldLike);
                //                    DebugIfWriteString("   ClientData.OldLike \r\n");
                //#endif
                NvmWrite(&ClientData,100,6100);
                delay_ms(1500);
                
                
            }
            else if( !PioGet(DISLIKE_BUTTON) && !fiveSecondsPassed && !PioGet(LIKE_BUTTON) ){
                TimerDelete(buttonPushTimer);
                //#ifdef DEBUG_OUTPUT_ENABLED
                //                    DebugIfWriteString("\r\n");
                //                    DebugIfWriteString("NO 5 seconds\r\n");
                //#endif
            }
            
            else if( !PioGet(DISLIKE_BUTTON)){
                //#ifdef DEBUG_OUTPUT_ENABLED
                //                    DebugIfWriteString("5 seconds\r\n");
                //#endif                
                have5SecondsPassed();
            }
        }
    }
    
    button = TRUE;
}



void writeBattery(){
    uint8 anlikbat=0, j = 0;
    
    for(j=0;j<6;j++){
        anlikbat += readBatteryLevel();
    }
    anlikbat = anlikbat/6;  
    
    uint8  gecici = anlikbat, basamak=3, kalan;
    uint8 _Bat[]={0,0,0};
    
    while ( gecici > 0 ){
        basamak--;
        kalan = gecici % 10;
        gecici /= 10;    
        _Bat[basamak] = kalan;
    }
    
    ekranaYaz(_Bat +  basamak ,3 - basamak,0);   
}


void writeBarcodeID(){
    
    char _barcode[14] ;
    
    memset(_barcode,'0',14);
    
    uint8 IDs[10];
    TYPED_BD_ADDR_T thisDeviceAddress;
    thisDeviceAddress.type = 0;
    CSReadBdaddr(&(thisDeviceAddress.addr)); 
    
    _barcode[0] =  ((uint8) (thisDeviceAddress.addr.lap & 0x0f000) >> 12) ;
    _barcode[1] =  ((uint8) (thisDeviceAddress.addr.lap & 0x00f00) >> 8) ;
    _barcode[2] =  ((uint8) (thisDeviceAddress.addr.lap & 0x000f0) >> 4) ;
    _barcode[3] =  ((uint8)  thisDeviceAddress.addr.lap & 0x0000f) ;
    
    _barcode[4] =  ((uint8) (thisDeviceAddress.addr.uap & 0x000f0) >> 4);
    _barcode[5] =  ((uint8) thisDeviceAddress.addr.uap & 0x0000f);
    _barcode[6] =  ((uint8) ((thisDeviceAddress.addr.lap >> 16) & 0x000f0) >> 4);
    _barcode[7] =  ((uint8) (thisDeviceAddress.addr.lap >> 16) & 0x0000f);
    _barcode[8] =  ((uint8) (thisDeviceAddress.addr.nap & 0x0f000 ) >> 12);
    _barcode[9] =  ((uint8) (thisDeviceAddress.addr.nap & 0x00f00 ) >> 8 );
    _barcode[10] = ((uint8) (thisDeviceAddress.addr.nap & 0x000f0 ) >> 4 );
    _barcode[11] = ((uint8) (thisDeviceAddress.addr.nap & 0x0000f ));
    
    ekranaYaz(_barcode      ,4,4396);      
    ekranaYaz(_barcode + 4  ,4,2878);      
    ekranaYaz(_barcode + 8  ,4,1360);      
    // +22 sola kaydýrýyor
    // -22 saða kaydýrýyor
    
    
    
    uint8 barcCount = 0;
    
    for( barcCount=0;barcCount < 12; barcCount++)
        _barcode[barcCount] = _barcode[barcCount] + 0x30;
    
    
    
    calculateChecksum(_barcode);
    bitmapBarcode(_barcode, 10, 202);
}        



void AppPowerOnReset(void) {}

void AppInit(sleep_state last_sleep_state) {
    
    uint16 dev/*, i*/;
    uint8 tempBat;
    SleepModeChange(sleep_mode_deep);
    NvmConfigureI2cEeprom();
    TimerInit(MAX_APP_TIMERS, (void*)app_timers);
    g_app_data.app_timer = TIMER_INVALID;
    
    TYPED_BD_ADDR_T thisDeviceAddress;
    thisDeviceAddress.type = 0;
    
#ifdef PAIRING_SUPPORT
    g_app_data.bonding_timer = TIMER_INVALID;
#endif
    
    for(dev = 0; dev < MAX_CONNECTED_DEVICES; dev++) {
        g_app_data.devices[dev].state = app_state_init;
    }
#ifdef DEBUG_OUTPUT_ENABLED      
    DebugIfInit();    
#endif
    g_app_data.dev_num = MAX_CONNECTED_DEVICES;
    g_app_data.nvm_dev_num = MAX_BONDED_DEVICES;
    GattInit();
    GattInstallClientRole();
    readPersistentStore(g_app_data.dev_num, g_app_data.nvm_dev_num);
    SMInit(0);
    g_app_data.num_conn = 0;
    
    tempBat=readBatteryLevel();
    
    uint8 kurulumRepeatCount = 0;
    
    NvmRead(&configInfo,10,6201);
    delay_ms(100);
    
    if ( configInfo.firstTime != 1){
        memset(&configInfo,0x00,sizeof(configInfo));
        delay_ms(100);
    }
    
    
    NvmRead(&ClientData,100,6100);
    delay_ms(100);
    
    if( ClientData.NewDeadTime == 0xFFFF)
    {
        /* MemSet(&ClientData,0x00,sizeof(ClientData));*/
        ClientData.NewDeadTime =1800;
        ClientData.NewScanTime =500;
        ClientData.firsAdv=0;
        ClientData.DeadTimeEp=24;
        DeadTimeCount=ClientData.DeadTimeEp;
        
    }else if (ClientData.NewDeadTime > 1800){
        ClientData.NewDeadTime =1800;
        ClientData.NewScanTime =500;
        ClientData.firsAdv=1;
        ClientData.DeadTimeEp=1;
        DeadTimeCount=ClientData.DeadTimeEp;
    }
    
    
    TakeSquare();
    sistemDurumu = READ;
    ClientData.BatLevel=readBatteryLevel();
    
    
    
    //        DebugIfWriteUint8(ClientData.opMode);
    
    if(ClientData.opMode == opIMAGE) {
        
        ClientData.opMode = opBLE;
        ClientData.firsAdv=0;
        NvmWrite(&ClientData,100,6100);
        NvmRead(final, 5808, 0);
        delay_ms(500);
        
        Display();
        
        TimerCreate(100 * MILLISECOND, TRUE, (timer_callback_arg) sleepHibernate);
    }
    else if(ClientData.opMode == opBARCODtoIMAGE) {
        ClientData.opMode = opBLE;
        ClientData.firsAdv=1;
        NvmWrite(&ClientData,100,6100);
        NvmRead(final, 5808, 0);
        delay_ms(500);
        
        Display();
        
        TimerCreate(100 * MILLISECOND, TRUE, (timer_callback_arg) sleepHibernate);
    }
    else if (ClientData.opMode == opBARCODE) {
        
            uint8 vers[] = { 0x01   };
    
    ekranaYaz(vers , sizeof(vers), 1000 );
    
        writeBarcodeID();
        writeBattery();
        ID_yaz();
        Display();
        
        setButton();
        time = 0;
        tidPavo = TimerCreate(30 * SECOND, TRUE, (timer_callback_arg) sleepHibernateBARCODtoIMAGE);
        
#ifdef DEBUG_OUTPUT_ENABLED
        //    DebugIfInit();
        DebugIfWriteString("\r\nBARCODE\r\n");
        //                timeWatch();
#endif              
    }
    
    else if(ClientData.opMode==opKurulum){
        
        MemSet(&ClientData,0x00,sizeof(ClientData));
        memset(final, 0, BITMAP_SIZE);
        ClientData.opMode = opKurulum;
        ClientData.firsAdv=1;
        ClientData.KurulumState=0;
        
        NvmWrite(&ClientData,100,6100);
        
        writeBarcodeID();
        ID_Kurulum();
        Display();
        
        
#ifdef DEBUG_OUTPUT_ENABLED
        //                DebugIfInit();
        DebugIfWriteString("\r\nKURULUM\r\n");
        //     timeWatch();
#endif  
        
        NvmRead(&kurulumRepeatCount,1,6250);
        if ( kurulumRepeatCount == 0xFF ) kurulumRepeatCount =0;
        kurulumRepeatCount++;
        

        
        if ( kurulumRepeatCount > 0x09 ){
            kurulumRepeatCount = 0;
            NvmWrite(&kurulumRepeatCount,1,6250);
            setButton();
            kurulumTaramaTimeout = TimerCreate(2*SECOND, TRUE, (timer_callback_arg) sleepHibernateIMAGE);
            
        }else{
            NvmWrite(&kurulumRepeatCount,1,6250);
            setButton();
            kurulumTaramaTimeout = TimerCreate(120*SECOND, TRUE, (timer_callback_arg) sleepHibernateIMAGE);
            SetState(0, app_state_scanning);
        }
        
        
    }
    else if(ClientData.opMode==opKurulumSuc){
        memset(final, 0, BITMAP_SIZE);
        /*  BarcodeMode();
                ID_yaz();
                */
        writeBarcodeID();
        ID_yaz();
        //ekran_surme
        Display();
        
        uint8 kurulumRepeatCount = 0;
        NvmWrite(&kurulumRepeatCount,1,6250);
        
        ClientData.NewDeadTime =1800;
        ClientData.firsAdv=0;
        ClientData.KurulumState=1;
        ClientData.DeadTimeEp=1;
        ClientData.NewScanTime=500;
        DeadTimeCount=ClientData.DeadTimeEp;
        
#ifdef DEBUG_OUTPUT_ENABLED
        //            DebugIfInit();
        DebugIfWriteString("\r\nKurulum Basarili\r\n");
#endif 
        setButton();
        ClientData.opMode = opBLE;
        NvmWrite(&ClientData,100,6100);
        delay_ms(100);
        tidPavo = TimerCreate(100 * MILLISECOND, TRUE, (timer_callback_arg) sleepHibernate);
    }
    else{
        
        /*if(ClientData.opMode == opBLE || ClientData.opMode == opBUTTON)*/
        /*    
        NvmRead(final,5808,0);
        delay_ms(750);
        */
        DeadTimeCount=ClientData.DeadTimeEp;
        setButton();
#ifdef DEBUG_OUTPUT_ENABLED
        
        DebugIfWriteString("opBLE \r\n");
#endif
        if( configInfo.firstTime != 1 ) {
            ClientData.NewDeadTime=1800; 
            ClientData.NewScanTime=200;
        }
        
        PinModeDir();
        
        EPD_W27_Init(); 
        
        SPI4W_WRITECOM(0x06);         //boostÉè¶¨
        SPI4W_WRITEDATA (0x07);
        SPI4W_WRITEDATA (0x07);
        SPI4W_WRITEDATA (0x04);       //
        
        SPI4W_WRITECOM(0xF8);         //boostÉè¶¨
        SPI4W_WRITEDATA (0x60);
        SPI4W_WRITEDATA (0xA5);
        
        SPI4W_WRITECOM(0xF8);         //boostÉè¶¨
        SPI4W_WRITEDATA (0x89);
        SPI4W_WRITEDATA (0xA5);
        
        SPI4W_WRITECOM(0xF8);         //boostÉè¶¨
        SPI4W_WRITEDATA (0x90);
        SPI4W_WRITEDATA (0x00);
        
        SPI4W_WRITECOM(0xF8);         //boostÉè¶¨
        SPI4W_WRITEDATA (0x93);
        SPI4W_WRITEDATA (0x2A);
        
        SPI4W_WRITECOM(0x04);
        READBUSY();
        
        SPI4W_WRITECOM(0X02);   //power off
        SPI4W_WRITECOM(0X07);   //deep sleep
        SPI4W_WRITEDATA(0xa5);        
        
        PioSet( SPI_MASTER_PIO_MOSI, LOW );
        PioSet( SPI_MASTER_PIO_SCLK, LOW );
        
        SetState(0, app_state_scanning);
    }
}

void AppProcessSystemEvent(sys_event_id id, void *data)
{
    switch (id) {
    case sys_event_pio_ctrlr:
        /* Process the interrupt from the PIO controller */
        SpiMasterProcessPioControllerEvent((uint16 *)data);
        break;
    case sys_event_pio_changed:
        HandlePIOChangedEvent((pio_changed_data*)data); /* Button handler , butona basýldýðýnda */
        break;
        default:
        /* Ignore all other events */
        break;
    }
}

bool AppProcessLmEvent(lm_event_code event_code, LM_EVENT_T *p_event_data)
{
    switch (event_code)
    {
        
        /* Handle events received from Firmware */
        
    case LM_EV_CONNECTION_COMPLETE:
        {
            /* Handle the LM connection complete event. */
            handleSignalLmEvConnectionComplete(
                    &((LM_EV_CONNECTION_COMPLETE_T*)p_event_data)->data);
        }
        break;
        
    case GATT_CONNECT_CFM:
        {
            
            /* Confirmation for the completion of GattConnectReq() procedure */
            handleSignalGattConnectCfm((GATT_CONNECT_CFM_T*)p_event_data);
            
            
        }
        break;
        
    case SM_KEY_REQUEST_IND:
        {
            /* Indicates that the Security Manager cannot find
             * security keys for the host in its persistent store.
             * Application responds with either a SM_KEYSET_T or
             * NULL pointer in SMKeyRequestResponse()
             */
            handleSignalSmKeyRequestInd((SM_KEY_REQUEST_IND_T *)p_event_data);
            
        }
        break;
        
    case SM_KEYS_IND:
        {
            /* Indication for the keys and associated security information
             * on a connection that has completed Short Term Key Generation
             * or Transport Specific Key Distribution
             */
            handleSignalSmKeysInd((SM_KEYS_IND_T *)p_event_data);
        }
        break;
        
    case SM_SIMPLE_PAIRING_COMPLETE_IND:
        {
            /* Indication for completion of Pairing procedure */
            handleSignalSmSimplePairingCompleteInd(
                    (SM_SIMPLE_PAIRING_COMPLETE_IND_T *)p_event_data);
            
        }
        break;
        
    case LM_EV_ENCRYPTION_CHANGE:
        {
            /* Indication for encryption change event */
            
            /* Nothing to do */
        }
        break;
        
    case GATT_DISCONNECT_IND:
        {
            /* Disconnect procedure triggered by remote host or due to
             * link loss is considered complete on reception of
             * LM_EV_DISCONNECT_COMPLETE event. So, it gets handled on
             * reception of LM_EV_DISCONNECT_COMPLETE event.
             */
            
        }
        break;
        
    case GATT_DISCONNECT_CFM:
        {
            
            /* Confirmation for the completion of GattDisconnectReq()
             * procedure is ignored as the procedure is considered complete
             * on reception of LM_EV_DISCONNECT_COMPLETE event. So, it gets
             * handled on reception of LM_EV_DISCONNECT_COMPLETE event.
             */
        }
        break;
        
    case LM_EV_DISCONNECT_COMPLETE:
        {
            
            
            /* Disconnect procedures either triggered by application or remote
             * host or link loss case are considered completed on reception
             * of LM_EV_DISCONNECT_COMPLETE event
             */
            
            handleSignalLmDisconnectComplete(
                    &((LM_EV_DISCONNECT_COMPLETE_T *)p_event_data)->data);
            
        }
        break;
        
    case GATT_CANCEL_CONNECT_CFM:
        {
            /* Confirmation for the completion of GattCancelConnectReq()
             * procedure
             */
            
            handleSignalGattCancelConnectCfm();
            
        }
        
        break;
        
    case LS_CONNECTION_UPDATE_SIGNALLING_IND:
        {
            /* This event is raised on a master after a slave initiates a
             * Connection Parameter Update procedure. It is handled by a call
             * to LsConnectionUpdateSignalingRsp() accepting or rejecting
             * the proposed connection parameters
             */
            
            /* Vendor may choose to accept/reject the connection paramters
             * received in this call, if the connection parameters are not
             * required to be stored in the application and the application
             * always accepts the params, then this call is not required
             */
        }
        break;
        
    case LS_CONNECTION_PARAM_UPDATE_CFM:
        {
            /* This event is raised after a Connection Parameter Update
             * procedure initiated by the master has completed.
             */
            handleSignalLsConnectionParamUpdateCfm(
                    (LS_CONNECTION_PARAM_UPDATE_CFM_T *)p_event_data);
            
        }
        break;
        
        default:
        {
            /* All the Discovery Procedure events are handled here */
            GattDiscoveryEvent(event_code, p_event_data);
        }
        break;
        
    }
    
    return TRUE;
}
