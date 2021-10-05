#include <gatt.h>           /* GATT application interface */
#include <mem.h>            /* Memory library */
#include <gatt_uuid.h>      /* Common Bluetooth UUIDs and macros */
#include <persistent.h>
#include <sleep.h>
#include <nvm.h>
#include <reset.h>

#include "new_service_data.h" /* Interface to this file */
#include "gatt_access.h"    /* GATT-related routines */
#include "debug_interface.h"/* Application debug routines */
#include "gatt_client.h"    /* Interface to top level application functions */
#include "nvm_access.h"
//ekran_surme
//#include "Pervasive_Displays_small_EPD.h"

//extern OP_MODES ClientData.opMode;
extern uint8 final[];
extern uint8 DeadTimeCount;

extern void SuccesImageAck(void);
extern void MessageNack(void);
//extern void sendSucInformation();
//extern void sendSucInformationSetup();

extern timer_id ConnectionTimer;
extern timer_id resettimer;

uint8 BitmapOncesi[20];

uint16 imageCounter=0;

bool imagefor=FALSE;

uint16 BatteryReadVoltage(void);
void yazma(void);


/* Battery full level as a percentage */
#define BATTERY_LEVEL_FULL                            (100)
#define BATTERY_CRITICAL_LEVEL                        (10)
#define BATTERY_FULL_BATTERY_VOLTAGE                  (3000)          
#define BATTERY_FLAT_BATTERY_VOLTAGE                  (1500)         
#define BATTERY_SERVICE_NVM_MEMORY_WORDS              (1)
#define BATTERY_NVM_LEVEL_CLIENT_CONFIG_OFFSET        (0)
/* Number of characteristics present in this service, range [1, 15] */
#define MAXIMUM_NUMBER_OF_CHARACTERISTIC              (10)
/* New service structure used in discovery procedure */
typedef struct _NEW_SERVICE_DATA_T {
    /* Service attribute range */
    uint16 service_start_handle;
    uint16 service_end_handle;
    
    /* Connection handle */
    uint16 connect_handle;
    
    /* Characteristics */
    CHARACTERISTIC_T chars[MAXIMUM_NUMBER_OF_CHARACTERISTIC];
    
    /* Total number of supported characteristics found for this service in the
     * Server's GATT Database (number of entries in chars array). The optimal
     * value is MAXIMUM_NUMBER_OF_CHARACTERISTIC
     */
    uint16 total_char: MAXIMUM_NUMBER_OF_CHARACTERISTIC;
    
    /* Index into chars array of the current characteristic */
    uint16 curr_char:4;
    
    /* Index into chars array of characteristic currently being configured */
    uint16 curr_config_char:4;
    
    /* Flag set to 1 if configuration is ongoing */
    uint16 config_ongoing:1;
    
    /* Flag set to 1 if a write request initiated during configuration has
     * been confirmed
     */
    uint16 write_cfm:1;
    
    /* Flag set to 1 if a read request initiated during configuration has
     * been confirmed
     */
    uint16 read_cfm:1;
    
    /* Array of characteristic 'types' corresponding to the chars array. Only
     * used in read/write/notify procedures.
     */
    new_char_t type[MAXIMUM_NUMBER_OF_CHARACTERISTIC];
} NEW_SERVICE_DATA_T;

/*============================================================================*
 *  Private Data
 *============================================================================*/

/* New Service data.
 * A record is kept for each connected device supporting the service.
 */
static NEW_SERVICE_DATA_T g_new_serv_data[MAX_CONNECTED_DEVICES];

/*============================================================================*
 *  Public Data
 *============================================================================*/


SERVICE_FUNC_POINTERS_T NewServiceFuncStore = {
    .serviceUuid            = &NewServiceUuid,
            .isMandatory            = NULL,
            .serviceInit            = &NewServiceDataInit,
            .checkHandle            = &NewServiceCheckHandle,
            .getHandles             = &NewServiceGetHandles,
            .charDiscovered         = &NewServiceCharDiscovered,
            .descDiscovered         = &NewServiceCharDescDisc,
            .discoveryComplete      = &NewServiceDiscoveryComplete,
            .serviceIndNotifHandler = NULL,
            .configureServiceNotif  = NULL,
            .writeRequest           = &NewServiceWriteRequest,
            .writeConfirm           = &NewServiceWriteConfirm,
            .readRequest            = &NewServiceReadRequest,
            .readConfirm            = &NewServiceReadConfirm,
            .configureService       = NULL,
            .isServiceFound         = &NewServiceFound,
            .resetServiceData       = &NewServiceResetData
                                  };

/*============================================================================*
 *  Private Function Prototypes
 *===========================================================================*/
/* Initialise New Service data */
static void newDataInit(uint16 dev); 

/* Check if the characteristic supports the specified ATT permissions */
static bool newCheckATTPermission(uint16 dev, new_char_t type, uint16 permission, uint16 *count);

/*============================================================================*
 *  Private Function Implementations
 *===========================================================================*/

/*---------------------------------------------------------------------------
 *  PARAMETERS
 *      dev [in]                Device to initialise service data for
 *----------------------------------------------------------------------------*/
static void newDataInit(uint16 dev) {
    uint16 char_num;            /* Loop counter */
    /* Service data for the specified device */
    NEW_SERVICE_DATA_T *data = &g_new_serv_data[dev];
    
    /* Reset the data */
    data->connect_handle       = GATT_INVALID_UCID;
    data->service_start_handle = INVALID_ATT_HANDLE;
    data->service_end_handle   = INVALID_ATT_HANDLE;
    data->total_char           = 0; 
    data->curr_char            = 0;
    
    /* Reset characteristics array */
    MemSet(data->chars, 0x0, sizeof(data->chars));
    
    /* Initialise characteristic array values */
    for(char_num = 0; char_num < MAXIMUM_NUMBER_OF_CHARACTERISTIC; char_num ++) {
        data->type[char_num]                        = new_type_invalid; 
        data->chars[char_num].valHandle             = INVALID_ATT_HANDLE;
        data->chars[char_num].descriptors[0].handle = INVALID_ATT_HANDLE;
        data->chars[char_num].descriptors[1].handle = INVALID_ATT_HANDLE;
    }
}

/*---------------------------------------------------------------------------
 *  DESCRIPTION
 *      Check whether a characteristic supports ATT permissions.
 *
 *  PARAMETERS
 *      dev [in]                Device to check
 *      type [in]               Characteristic to check
 *      permission [in]         ATT permission(s) to check
 *      count [out]             Index into service data of characteristic
 *
 *  RETURNS
 *      TRUE if the characteristic supports the specified ATT permissions on
 *      this device
 *      FALSE otherwise, or on error
 *----------------------------------------------------------------------------*/
static bool newCheckATTPermission(uint16 dev, new_char_t type, uint16 permission, uint16 *count) {
    /* Initialise return value */
    *count = 0;
    
    /* Check the requested characteristic is supported */
    if(type >= new_type_invalid) return FALSE;
    
    /* Search for the specified characteristic in the chars array */
    while(g_new_serv_data[dev].type[*count] != type) {
        (*count)++;
        
        if(*count == MAXIMUM_NUMBER_OF_CHARACTERISTIC) {
            /* Characteristic not found */
            return FALSE;
        }
    }
    
    /* Check that the characteristic supports all the permissions requested */
    if((g_new_serv_data[dev].chars[*count].properties & permission) != permission) return FALSE;
    
    return TRUE;
}

/*============================================================================*
 *  Public Function Implementations
 *===========================================================================*/

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceUuid
 *
 *  DESCRIPTION
 *      Return the Service UUID and type (16- or 128-bit)
 *
 *  PARAMETERS
 *      type [out]              UUID type
 *      uuid [out]              Service UUID
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void NewServiceUuid(GATT_UUID_T *type, uint16 *uuid) {
    *type = GATT_UUID16;
    uuid[0] = UUID_NEW_SERVICE;
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceDataInit
 *
 *  DESCRIPTION
 *      Initialise service data during the Discovery Procedure when the service
 *      has been discovered in the Server's GATT Database.
 *
 *  PARAMETERS
 *      dev [in]                Device to initialise service data for
 *      p_event_data [in]       Primary service discovery event data
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void NewServiceDataInit(uint16 dev, GATT_DISC_PRIM_SERV_BY_UUID_IND_T *p_event_data) {
    
    newDataInit(dev);
    
    g_new_serv_data[dev].service_start_handle = p_event_data->strt_handle;
    g_new_serv_data[dev].service_end_handle   = p_event_data->end_handle;
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceCheckHandle
 *
 *  DESCRIPTION
 *      Check if the specified handle belongs to the service.
 *
 *  PARAMETERS
 *      dev [in]                Device to check handle for
 *      handle [in]             Handle to check
 *
 *  RETURNS
 *      TRUE if the supplied handle belongs to this service
 *      FALSE otherwise
 *----------------------------------------------------------------------------*/
bool NewServiceCheckHandle(uint16 dev, uint16 handle) {
    return (((handle >= g_new_serv_data[dev].service_start_handle) && (handle <= g_new_serv_data[dev].service_end_handle)) ? TRUE : FALSE);
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceGetHandles
 *
 *  DESCRIPTION
 *      This function is called during the Discovery Procedure. Its behaviour
 *      depends on the value of 'type':
 *
 *      service_type:        Return the full range of characteristic handles
 *                           supported by this service
 *      characteristic_type: Return the full range of descriptor handles
 *                           supported by this characteristic
 *
 *  PARAMETERS
 *      dev [in]                Device to return handle range for
 *      start_hndl [out]        Start of handle range, or INVALID_ATT_HANDLE
 *      end_hndl [out]          End of handle range, or INVALID_ATT_HANDLE
 *      type [in]               Type of handles to return
 *
 *  RETURNS
 *      TRUE if type is service_type
 *      TRUE if type is characteristic type and there are more characteristics
 *      to be discovered.
 *      FALSE otherwise
 *----------------------------------------------------------------------------*/
bool NewServiceGetHandles(uint16 dev, uint16 *start_hndl, uint16 *end_hndl, gatt_profile_hierarchy_t type) {
    switch(type) {
    case service_type:
        {
            *start_hndl = g_new_serv_data[dev].service_start_handle;
            *end_hndl   = g_new_serv_data[dev].service_end_handle;
        }
        break;
        
    case characteristic_type:
        {
            const uint16 curr_char = g_new_serv_data[dev].curr_char++;
            
            /* Start handle will be the 'value handle' */
            *start_hndl = g_new_serv_data[dev].chars[curr_char].valHandle;
            
            if(curr_char + 1 == g_new_serv_data[dev].total_char) {
                /* If this is the last service characteristic the end handle is
                 * the service end handle.
                 */
                *end_hndl = g_new_serv_data[dev].service_end_handle;
            }
            else if(curr_char + 1 > g_new_serv_data[dev].total_char) {
                /* If there are no more characteristics populate start and end
                 * handles with INVALID_ATT_HANDLE and return FALSE 
                 */
                *start_hndl = INVALID_ATT_HANDLE;
                *end_hndl   = INVALID_ATT_HANDLE;
                
                return FALSE;
            } else {
                /* Otherwise the end handle is the start handle of the next
                 * characteristic less 2.
                 */
                *end_hndl = g_new_serv_data[dev].chars[curr_char + 1].valHandle - 2;
            }
        }
        break;
        
        default:
        {
            *start_hndl = INVALID_ATT_HANDLE;
            *end_hndl   = INVALID_ATT_HANDLE;
            
            /* Unsupported type passed from the application */
            return FALSE;
        }
    }
    
    /* More characteristics are available - return TRUE */
    return TRUE;
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceCharDiscovered
 *
 *  DESCRIPTION
 *      This function is called during the Discovery Procedure after a service
 *      characteristic has been discovered.
 *
 *  PARAMETERS
 *      dev [in]                Device on which characteristic has been
 *                              discovered
 *      p_event_data [in]       Characteristic discovery event data
 *
 *  RETURNS
 *      TRUE if the discovered characteristic is supported by this service
 *      FALSE otherwise
 *----------------------------------------------------------------------------*/
bool NewServiceCharDiscovered(uint16 dev, GATT_CHAR_DECL_INFO_IND_T *p_event_data) {
    /* Handle of discovered characteristic */
    const uint16 handle = p_event_data->val_handle;
    
    /* Total number of supported characteristics discovered on this Server */
    const uint16 total_char = g_new_serv_data[dev].total_char;
    
    /* Check if the discovered characteristic belongs to this service */
    if(!NewServiceCheckHandle(dev, handle)) {
        /* Discovered characteristic does not belong this service */
        return FALSE;
    }
    
    /* Check if the discovered characteristic is supported by this service */
    switch(p_event_data->uuid[0]) {
    case UUID_NEW_LEVEL:
        {
            /* Store related values */
            g_new_serv_data[dev].type[total_char] = new_level;
        }
        break;
        
        default:
        {
            /* Discovered characteristic is not supported by this service */
            return FALSE;
        }
    }
    
    /* Store discovered characteristic data */ 
    /* (Example application only supports 16-bit characteristic UUIDs) */
    g_new_serv_data[dev].chars[total_char].uuid = p_event_data->uuid[0];
    g_new_serv_data[dev].chars[total_char].valHandle = handle;
    g_new_serv_data[dev].chars[total_char].properties = p_event_data->prop;
    g_new_serv_data[dev].chars[total_char].nDescriptors = 0;
    
    /* Increment total number of supported characteristics discovered on this
     * this Server
     */
    g_new_serv_data[dev].total_char++;
    
    return TRUE;
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceCharDescDisc
 *
 *  DESCRIPTION
 *      This function is called during the Discovery Procedure after a
 *      characteristic descriptor has been discovered.
 *
 *  PARAMETERS
 *      dev [in]                Device on which descriptor has been discovered
 *      p_event_data [in]       Descriptor discovery event data
 *
 *  RETURNS
 *      TRUE if the discovered characteristic descriptor is supported by this
 *      service
 *      FALSE otherwise
 *----------------------------------------------------------------------------*/
void NewServiceCharDescDisc(uint16 dev, GATT_CHAR_DESC_INFO_IND_T *p_event_data) {
    /* Current characteristic */
    const uint16 curr_char = g_new_serv_data[dev].curr_char - 1;
    /* Number of descriptors discovered for the current characteristic */
    uint8 *numDesc = &g_new_serv_data[dev].chars[curr_char].nDescriptors;
    /* Properties of the current characteristic */
    const uint8 prop = g_new_serv_data[dev].chars[curr_char].properties;
    
    /* Only the Client Characteristic Configuration Descriptor is supported by
     * this service
     */
    if(((prop & ATT_PERM_NOTIFY) || (prop & ATT_PERM_INDICATE)) && p_event_data->uuid[0] == UUID_CLIENT_CHAR_CFG) {
        /* Add the Client Characteristic Configuration Descriptor 16-bit UUID
         * only. This may be expanded to include support for 128-bit UUIDs.
         */
        g_new_serv_data[dev].chars[curr_char].descriptors[*numDesc].uuid = p_event_data->uuid[0];
        g_new_serv_data[dev].chars[curr_char].descriptors[*numDesc].handle = p_event_data->desc_handle;
        
        /* Increment the number of descriptors discovered for the current
         * characteristic
         */
        (*numDesc)++;
    }
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceDiscoveryComplete
 *
 *  DESCRIPTION
 *      This function called when the discovery of this service is complete.
 *      Although GATT write/read requests are supported, it is highly
 *      recommended that the full Discovery Procedure be completed before
 *      GATT read/write procedures are initiated.
 *
 *  PARAMETERS
 *      dev [in]                Device on which service discovery has completed
 *      connect_handle [in]     Handle of connection with the device
 *
 *  RETURNS
 *      TRUE if a GATT read/write request is initated by this function
 *      FALSE otherwise
 *----------------------------------------------------------------------------*/
bool NewServiceDiscoveryComplete(uint16 dev, uint16 connect_handle) {
    /* Reset the current characteristic index */
    g_new_serv_data[dev].curr_char = 0;
    
    /* Store the connection handle */
    g_new_serv_data[dev].connect_handle = connect_handle;
    
    return FALSE;
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceHandlerNotifInd
 *
 *  DESCRIPTION
 *      Handle GATT_IND_CHAR_VAL_IND and GATT_NOT_CHAR_VAL_IND events for this
 *      service.
 *
 *  PARAMETERS
 *      dev [in]                Device on which notification/indication has
 *                              arisen
 *      handle [in]             Characteristic affected
 *      size [in]               Characteristic value size
 *      value [in]              New characteristic value
 *
 *  RETURNS
 *      TRUE on success, FALSE otherwise
 *----------------------------------------------------------------------------*/
bool NewServiceHandlerNotifInd(uint16 dev, uint16 handle, uint16 size, uint8  *value) {
    /* Index into chars array of the supplied characteristic */
    uint16 count = 0;
    
    /* Check the supplied characterstic is belongs to this service */
    if(!NewServiceCheckHandle(dev, handle)) {
        /* Characteristic does not belong to this service */
        return FALSE;
    }
    
    /* Check whether the supplied characteristic is supported by this service */
    while(g_new_serv_data[dev].chars[count].valHandle != handle) {
        count ++;
        
        if(count == MAXIMUM_NUMBER_OF_CHARACTERISTIC) {
            /* Characteristic is not supported by this service */
            return FALSE;
        }
    }
    
    /* Act on the notification/indication */
    switch (g_new_serv_data[dev].type[count]) {
    case new_level:
        {
//            uint16 size_print;      /* Loop counter */

//            for(size_print = 0; size_print < size; size_print ++) {
//                DebugIfWriteUint8(value[size - size_print - 1]);
//            }
        }
        break;
        
        default:
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

/*---------------------------------------------------------------------------
 *  NAME
 *     NewServiceConfigNotif
 *
 *  DESCRIPTION
 *      Update the value of the specified descriptor for the specified
 *      characteristic of this service according to the value of 'Enable'.
 *
 *  PARAMETERS
 *      dev [in]                Device on which characteristic descriptor
 *                              should be updated
 *      Type [in]               Characteristic the descriptor belongs to
 *      SubType [in]            Characteristic descriptor to update
 *      Enable [in]             New characteristic descriptor value
 *
 *  RETURNS
 *      TRUE on success, FALSE otherwise
 *----------------------------------------------------------------------------*/
bool NewServiceConfigNotif(uint16 dev, uint16 Type, uint8 SubType, bool Enable) {
    /* Type is promoted to the service characteristics enumerated type */
    uint16 client_cfg;          /* Descriptor handle corresponding to SubType */
    uint16 count = 0;           /* Index into chars array of characteristic
                                 * corresponding to Type
                                 */
    uint8 notification[2];      /* New descriptor value corresponding to
                                 * Enabled
                                 */
    
    /* Check that the specified characteristic supports notification on this
     * device. If so, obtain the index into chars of this characteristic.
     */
    /* This example application only supports notifications. Support for
     * indcations may be added by specifying ATT_PERM_INDICATE
     */
    if(!newCheckATTPermission(dev, Type, ATT_PERM_NOTIFY, &count)) return FALSE;
    
    /* Check if the specified descriptor is supported for this characteristic */
    if(SubType > g_new_serv_data[dev].chars[count].nDescriptors || !SubType) return FALSE;
    
    
    /* Obtain the descriptor handle */
    client_cfg = g_new_serv_data[dev].chars[count].descriptors[SubType - 1].handle;
    if (client_cfg == INVALID_ATT_HANDLE) return FALSE;
    
    /* Prepare the new descriptor value based on 'Enable' */
    if(Enable) {
        notification[0] = WORD_LSB(gatt_client_config_notification);
        notification[1] = WORD_MSB(gatt_client_config_notification);
    } else {
        notification[0] = WORD_LSB(gatt_client_config_none);
        notification[1] = WORD_MSB(gatt_client_config_none);
    }
    
    /* Request that the descriptor be modified */
    GattWriteCharValueReq(g_new_serv_data[dev].connect_handle, GATT_WRITE_REQUEST, client_cfg, sizeof(notification), notification);
    
    return TRUE;
}

/*---------------------------------------------------------------------------
 *  NAME
 *     NewServiceReadRequest
 *
 *  DESCRIPTION
 *      Initiate a GATT read request for the specified characteristic value.
 *
 *  PARAMETERS
 *      dev [in]                Device on which to read characteristic value
 *      type [in]               Characteristic to read
 *
 *  RETURNS
 *      TRUE on success, FALSE otherwise
 *----------------------------------------------------------------------------*/
bool NewServiceReadRequest(uint16 dev, uint16 type) {
    /* Type is promoted to the service characteristics enumerated type */
    uint16 value_handle;        /* Handle of characteristic value */
    uint16 count;               /* Index into chars array of characteristic
                                 * corresponding to Type
                                 */
    uint16 status;              /* Function status */
    
    /* Check if read access is permitted to the specified characteristic on
     * this device
     */
    if(!newCheckATTPermission(dev, type, ATT_PERM_READ, &count)) return FALSE;
    
    /* Check that the characteristic is supported by this service */
    value_handle = g_new_serv_data[dev].chars[count].valHandle;
    if (value_handle == INVALID_ATT_HANDLE) return FALSE;
    
    /* Update the current characteristic index */
    g_new_serv_data[dev].curr_char = count;
    
    /* Send the read request to the Server */
    
    /* Note that the maximum amount of data that may be transmitted in one PDU
     * is limited to 22 octets (DEFAULT_ATT_MTU - 1) so any values longer than
     * this will be truncated. See GattReadLongCharValue() in the Firmware API
     * for a method to read longer values using multiple PDUs.
     */
    status = GattReadCharValue(g_new_serv_data[dev].connect_handle, value_handle);
    
    return (status == sys_status_success) ? TRUE : FALSE;
}
uint8 SatirSayisi;
uint8 SatirFark;
uint8 SatirSirasi;

uint8 readBatteryLevel(void)
{
    uint32 bat_voltage;                 /* Battery voltage in mV */
    uint32 bat_level;                   /* Battery level in percent */
    
    /* Read battery voltage and level it with minimum voltage */
    bat_voltage = BatteryReadVoltage();
    
    /* Level the read battery voltage to the minimum value */
    if(bat_voltage < BATTERY_FLAT_BATTERY_VOLTAGE)
    {
        bat_voltage = BATTERY_FLAT_BATTERY_VOLTAGE;
    }
    
    bat_voltage -= BATTERY_FLAT_BATTERY_VOLTAGE;
    
    /* Get battery level in percent */
    bat_level = (bat_voltage * 100) / (BATTERY_FULL_BATTERY_VOLTAGE -
                                       BATTERY_FLAT_BATTERY_VOLTAGE);
    
    /* Check the precision errors */
    if(bat_level > 100)
    {
        bat_level = 100;
    }
    /* Return the battery level (as a percentage of full) */
    return (uint8)bat_level;
}

void yazma(void){
    uint16 sayici=0;
    imageCounter=0;
    if(final[0]==0xD2){
        
        ClientData.opMode = opKurulumSuc;
        configInfo.firstTime=1;
        configInfo.ReklamID=final[1];
        configInfo.PageID=final[2];
        configInfo.ByteSirasi=final[3];
        configInfo.BitSirasi=final[4];
        NvmWrite(&ClientData,100,6100);
        delay_ms(50);
        NvmWrite(&configInfo,10,6201);
        sendSucInformation();
    }
    else if(final[0]==0xD8){

        if(configInfo.ReklamID==final[1] 
           && configInfo.PageID==final[2] 
                                && configInfo.ByteSirasi==final[3]
                                                         && configInfo.BitSirasi==final[4])
        {
            ClientData.opMode = opKurulum;
            NvmWrite(&ClientData,100,6100);
            sendSucInformationSetup();
            
        }
        else{ 
            ClientData.opMode = opBLE;
            MessageNack();
            NvmWrite(&ClientData,100,6100);

        }
        
    }
    else if(final[0]==0xD6){ 
        /* SORGU CEVABI*/
        
        if(configInfo.ReklamID==final[1] 
           && configInfo.PageID==final[2] 
                                && configInfo.ByteSirasi==final[3]
                                                         && configInfo.BitSirasi==final[4])
        {
            ClientData.opMode = opIMAGE;      

            if(final[5]<1) {ClientData.NewDeadTime=360;DeadTimeCount=1;}
            else if(final[5]>=1 && final[5]<=5) {ClientData.NewDeadTime=final[5]*360;  DeadTimeCount=1;}
            else {DeadTimeCount=(final[5]*6)/30; ClientData.NewDeadTime=1800;}
            
            ClientData.DeadTimeEp=DeadTimeCount;
            
            NvmWrite(&ClientData,100,6100);
            SuccesImageAck();
            
        }
        else{ 
            ClientData.opMode = opBLE;
            NvmWrite(&ClientData,100,6100);
            MessageNack();
        }
    }
    else if(imagefor==TRUE ){
        
        if(configInfo.ReklamID==BitmapOncesi[1] 
           && configInfo.PageID==BitmapOncesi[2] 
                                && configInfo.ByteSirasi==BitmapOncesi[3]
                                                         && configInfo.BitSirasi==BitmapOncesi[4])
        { 
            ClientData.opMode = opIMAGE;
            
            if(BitmapOncesi[5]<1) { ClientData.NewDeadTime=360;  DeadTimeCount=1;}
            else if(BitmapOncesi[5]>=1 && BitmapOncesi[5]<=5) {ClientData.NewDeadTime=BitmapOncesi[5]*360; DeadTimeCount=1;}    
            else {DeadTimeCount=(BitmapOncesi[5]*6)/30; ClientData.NewDeadTime=1800;}
            
            ClientData.DeadTimeEp=DeadTimeCount;

            ClientData.SablonID=BitmapOncesi[6];
            ClientData.StartPoint=BitmapOncesi[7]*0x100+BitmapOncesi[8];
            ClientData.EndPoint=BitmapOncesi[9]*0x100+BitmapOncesi[10];
            for(sayici=0;sayici<5;sayici++) ClientData.NewTrnx[sayici]=BitmapOncesi[sayici+11];
            ClientData.NewScanTime=BitmapOncesi[16]*100;
            if(BitmapOncesi[16]<5 || BitmapOncesi[16]>20) ClientData.NewScanTime=500;

            NvmWrite(final,5808,0);
            NvmWrite(&ClientData,100,6100);
            SuccesImageAck();

        }
        else{
            ClientData.opMode = opBLE;
            NvmWrite(&ClientData,100,6100);
            MessageNack();
        }
    }
}
bool valF = FALSE;
bool val0 = FALSE;

void NewServiceReadConfirm(uint16 dev, uint16 size, uint8 *value) { /*BARCODE GELÝNCE BURADA FLASHA KAYDEDÝLÝYOR*/
    static uint16 counter=1, i; 
    if(g_new_serv_data[dev].config_ongoing) {
        g_new_serv_data[dev].read_cfm = TRUE;
    } else {
        switch(g_new_serv_data[dev].type[g_new_serv_data[dev].curr_char]) {
        case new_level: 
            
            /*if(value[0]==0x04 && value[1]==0x00){for(i = 2; i < 22; i++) DebugWriteChar(value[i]);}
            else if(value[0]==0x05){for(i = 2; i < value[1]+2; i++) DebugWriteChar(value[i]);}*/
            
            if(value[0]==0x04 && value[1]==0x00){             
                for(i = 2; i < 22; i++) {
                    if(value[2]==0xD4){
                        BitmapOncesi[(i-2)]=value[i]; 
                        imagefor=TRUE;
                        // DebugWriteChar(BitmapOncesi[i-2]);
                        ClientData.StartPoint=BitmapOncesi[7]*0x100+BitmapOncesi[8];
                        ClientData.EndPoint=BitmapOncesi[9]*0x100+BitmapOncesi[10];
                        imageCounter=ClientData.StartPoint;
                    }
                    else if(imagefor) {
                        //FF ve 00 sýkýstýrmasini cozmek icin
                        if(value[i] == 0xFF && valF == FALSE && val0==FALSE){
                            valF= TRUE; 
                            val0 = FALSE;
                            continue;
                        }else if(value[i] == 0x00 && val0 == FALSE && valF == FALSE){
                            val0 = TRUE;
                            valF= FALSE;
                            continue;
                        }
                        
                        if (valF){
                            uint8 t=0;
                            for(t=0;t<value[i];t++){    
                                final[imageCounter++]=0xFF;  
                            }
                            
                            valF=FALSE;
                            val0=FALSE;
                        }else if(val0){
                            uint8 t=0;
                            
                            for(t=0;t<value[i];t++){   
                                final[imageCounter++]=0x00;  
                            }
                            valF= FALSE;
                            val0=FALSE;
                        }else{
                            final[imageCounter++]=value[i]; 
                            valF= FALSE;
                            val0=FALSE;
                        }
                    }else{
                        final[imageCounter++]=value[i];   
                    }
                    // DebugWriteChar(final[20*(counter - 1) + (i-2)]);
                }
                if(value[2]!=0xD4)counter++;
            }
            else if(value[0]==0x05){
                for(i = 2; i < value[1]+2; i++){
                    if(imagefor){
                        if(value[i] == 0xFF && valF == FALSE && val0==FALSE){
                            valF= TRUE; 
                            val0 = FALSE;
                            continue;
                        }else if(value[i] == 0x00 && val0 == FALSE && valF == FALSE){
                            val0 = TRUE;
                            valF= FALSE;
                            continue;
                        }
                        
                        if (valF){
                            uint8 t=0;
                            for(t=0;t<value[i];t++){     
                                final[imageCounter++]=0xFF;  
                            }
                            
                            valF=FALSE;
                            val0=FALSE;
                        }else if(val0){
                            uint8 t=0;
                            
                            for(t=0;t<value[i];t++){     
                                final[imageCounter++]=0x00;  
                            }
                            valF= FALSE;
                            val0=FALSE;
                        }else{  
                            final[imageCounter++]=value[i]; 
                            valF= FALSE;
                            val0=FALSE;
                        }
                    }else{
                        final[imageCounter++]=value[i]; 
                    }
                }   
                yazma();
            }
            
            break;
            
            default:
            break;
        }
    }
}



/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceWriteConfirm
 *
 *  DESCRIPTION
 *      Called when a write request is successful.
 *
 *  PARAMETERS
 *      dev [in]                Device on characteristic value was read
 *      connect_handle [in]     Device connection handle
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void NewServiceWriteConfirm(uint16 dev, uint16 connect_handle) { 
    if(g_new_serv_data[dev].config_ongoing) {
        
        g_new_serv_data[dev].write_cfm = TRUE;
    } else {
        
        switch(g_new_serv_data[dev].type[g_new_serv_data[dev].curr_char]) {
            case new_level:  /* NewService'nin write'i da rasgele birþey yazýp gönderiyor gönderdikten sonra disconnect diyor */
            {
                
            }
            break;
            default:
            
            break;
        }
    }
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceConfigure
 *
 *  DESCRIPTION
 *      Configure the Server's GATT database for this service.
 *
 *  PARAMETERS
 *      dev [in]                Device to configure
 *
 *  RETURNS
 *      TRUE if notification is initiated for the current characteristic
 *      FALSE when configuration is complete
 *----------------------------------------------------------------------------*/
bool NewServiceConfigure(uint16 dev) {
    /* Current characteristic type */
    new_char_t type = g_new_serv_data[dev].type[g_new_serv_data[dev].curr_config_char];
    
    if(g_new_serv_data[dev].write_cfm) {
        /* If the previous write request was successful advance the index to the
         * next characteristic to configure
         */
        g_new_serv_data[dev].curr_config_char ++;
        
        /* Reset the confirmation flag */
        g_new_serv_data[dev].write_cfm = FALSE;
    }
    
    if(type == new_level) {
        /* Configure the Client Configuration Characteristic Descriptor for the
         * current characteristic
         */
        if(!NewServiceConfigNotif(dev, type, 0x1, TRUE)) ReportPanic(app_panic_config_fail);
        
        g_new_serv_data[dev].config_ongoing = TRUE;
        
        return TRUE;
    }
    
    if(type == new_list_of_IDs) {
        /* Configure the Client Configuration Characteristic Descriptor for the
         * current characteristic
         */
        if(!NewServiceConfigNotif(dev, type, 0x1, TRUE)) ReportPanic(app_panic_config_fail);
        
        g_new_serv_data[dev].config_ongoing = TRUE;
        
        return TRUE;
    }
    
    /* Do not reset curr_char_config, as the configuration is done only once
     * per connection 
     */
    g_new_serv_data[dev].config_ongoing = FALSE;
    
    return FALSE;
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceFound
 *
 *  DESCRIPTION
 *      Check if this service is supported on the specified device.
 *
 *  PARAMETERS
 *      dev [in]                Device to check
 *
 *  RETURNS
 *      TRUE if this service is supported on the specified device.
 *      FALSE otherwise
 *----------------------------------------------------------------------------*/
bool NewServiceFound(uint16 dev) {
    if((g_new_serv_data[dev].service_start_handle != INVALID_ATT_HANDLE) && (g_new_serv_data[dev].service_end_handle != INVALID_ATT_HANDLE)) {
        /* Service is supported for the specified device */
        return TRUE;
    }
    
    /* Service has yet to be discovered or is not supported */
    return FALSE;
}

/*---------------------------------------------------------------------------
 *  NAME
 *      NewServiceResetData
 *
 *  DESCRIPTION
 *      Reset the service data for the specified device.
 *
 *  PARAMETERS
 *      dev [in]                Device to reset data for
 *
 *  RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void NewServiceResetData(uint16 dev) {
    newDataInit(dev);
}


bool NewServiceWriteRequest(uint16 dev, uint16 type, uint8* data, uint16 length) {
    uint16 value_handle, count, status;
    if(!newCheckATTPermission(dev, type, ATT_PERM_WRITE_CMD, &count)) return FALSE;
    value_handle = g_new_serv_data[dev].chars[count].valHandle;
    if (value_handle == INVALID_ATT_HANDLE) return FALSE;
    
    g_new_serv_data[dev].curr_char = count;
    status = GattWriteCharValueReq(g_new_serv_data[dev].connect_handle, GATT_WRITE_COMMAND, value_handle, length, data);
    return (status == sys_status_success) ? TRUE : FALSE;
    
}
