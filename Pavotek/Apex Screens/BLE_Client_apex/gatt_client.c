/******************************************************************************
 *  Copyright Cambridge Silicon Radio Limited 2012-2014
 *  Part of CSR uEnergy SDK 2.3.0
 *  Application version 2.3.0.0
 *
 *  FILE
 *      main.c
 *
 *  DESCRIPTION
 *      Simple example to show a gpio usage. This application also
 *      demonstrates how to configure and set the state of a gpio.
 *
 ******************************************************************************/

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/
#include <main.h>           /* Functions relating to powering up the device */
#include <ls_app_if.h>      /* Link Supervisor application interface */
#include <panic.h>          /* Support for applications to panic */
#include <pio.h>
#include <platform_config.h>
#include <timer.h>  
/*============================================================================*
 *  Private Definitions
 *============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bitmapExamples.h>
/*============================================================================*
 *  Private Data
 *============================================================================*/
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
//extern uint8 final[];
#define PIO_DIR_INPUT   FALSE   
/*============================================================================*
 *  Private Function Prototypes
 *============================================================================*/
void ekranaYaz(uint8 IDs[],uint8 size, uint8 startPoint);
void HandlePIOChangedEvent(pio_changed_data *pio_data);
void writeBarcodeID();
void ID_yaz(void);
/*============================================================================*
 *  Private Function Implementations
 *============================================================================*/
time48 sleepTime = {0x00,0x10,0x00}; 
void sleepHibernate(void)
{
    /*
      Y?lemciniz minimum 0x100000 usec uyumak zorunda. Hemen hemen 1 saniyeye denk geliyor.
      Bunun altynda bir de?er ile fonksiyonu kullandy?ynyzda yine min uyuyaca?y süre kadar uyuyor.
      */
    SleepRequest(sleep_state_hibernate, FALSE, sleepTime);
}
/*============================================================================*
 *  Public Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      AppPowerOnReset
 *
 *  DESCRIPTION
 *      This user application function is called just after a power-on reset
 *      (including after a firmware panic), or after a wakeup from Hibernate or
 *      Dormant sleep states.
 *
 *      At the time this function is called, the last sleep state is not yet
 *      known.
 *
 *      NOTE: this function should only contain code to be executed after a
 *      power-on reset or panic. Code that should also be executed after an
 *      HCI_RESET should instead be placed in the AppInit() function.
 *
 * PARAMETERS
 *      None
 *
 * RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void AppPowerOnReset(void)
{
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      AppInit
 *
 *  DESCRIPTION
 *      This user application function is called after a power-on reset
 *      (including after a firmware panic), after a wakeup from Hibernate or
 *      Dormant sleep states, or after an HCI Reset has been requested.
 *
 *      NOTE: In the case of a power-on reset, this function is called
 *      after app_power_on_reset().
 *
 * PARAMETERS
 *      last_sleep_state [in]   Last sleep state
 *
 * RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void AppInit(sleep_state last_sleep_state)
{   
    /*
    PioSetDir( RESET_PIN, OUTPUT );  //pull_up direnc var
    PioSet( RESET_PIN, LOW );
    delay_ms(100);*/
    
    /* Configure button to be controlled directly */
    PioSetMode(LIKE_BUTTON, pio_mode_user);
    PioSetMode(DISLIKE_BUTTON, pio_mode_user);
    /* Configure button to be input */
    PioSetDir(LIKE_BUTTON, PIO_DIR_INPUT);
    PioSetDir(DISLIKE_BUTTON, PIO_DIR_INPUT);
    /* Set weak pull up on button PIO, in order not to draw too much current
     * while button is pressed
     */
    PioSetPullModes((1UL << LIKE_BUTTON), pio_mode_weak_pull_up);
    PioSetPullModes((1UL << DISLIKE_BUTTON), pio_mode_weak_pull_up);
    /* Set the button to generate sys_event_pio_changed when pressed as well
     * as released
     */
    PioSetEventMask((1UL << LIKE_BUTTON), pio_event_mode_both);
    PioSetEventMask((1UL << DISLIKE_BUTTON), pio_event_mode_both);
    
}

/*-----------------------------------------------------------------------------*
 *  NAME
 *      AppProcesSystemEvent
 *
 *  DESCRIPTION
 *      This user application function is called whenever a system event, such
 *      as a battery low notification, is received by the system.
 *
 * PARAMETERS
 *      id   [in]   System event ID
 *      data [in]   Event data
 *
 * RETURNS
 *      Nothing
 *----------------------------------------------------------------------------*/
void AppProcessSystemEvent(sys_event_id id, void *data)
{
    switch (id) {
    case sys_event_pio_changed:
        HandlePIOChangedEvent((pio_changed_data*)data); /* Button handler , butona basyldy?ynda */
        break;
        default:
        /* Ignore all other events */
        break;
    }
}
/*----------------------------------------------------------------------------*
 *  NAME
 *      AppProcessLmEvent
 *
 *  DESCRIPTION
 *      This user application function is called whenever a LM-specific event
 *      is received by the system.
 *
 * PARAMETERSF
 *      event_code [in]   LM event ID
 *      event_data [in]   LM event data
 *
 * RETURNS
 *      TRUE if the app has finished with the event data; the control layer
 *      will free the buffer.
 *----------------------------------------------------------------------------*/
bool AppProcessLmEvent(lm_event_code event_code, LM_EVENT_T *event_data)
{
    return TRUE;
}

void HandlePIOChangedEvent(pio_changed_data *pio_data) {  /* tu?a basyldy?yn da bu interupta geliyor*/

    if( PioGet(DISLIKE_BUTTON) ) {
        
       memcpy(final, gImage_a, sizeof(gImage_a));
        Display();
        TimerCreate(100 * MILLISECOND, TRUE, (timer_callback_arg) sleepHibernate);   
    }
    
    else if( PioGet(LIKE_BUTTON) ){
        
        memcpy(final, gImage_a, sizeof(gImage_a));
        Display();
        TimerCreate(100 * MILLISECOND, TRUE, (timer_callback_arg) sleepHibernate); 
        
    }
}
   