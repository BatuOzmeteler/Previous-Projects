/*
 *****************************************************************************
 *
 *  Project:        OpenECU
 *  File:           mpc_v2_api.h
 *  Description:    (none)
 *
 *  (none)
 *
 *  Notes:          Automatically generated as part of the build process.
 *                  Do not modify.
 *                  Created on: Sun May 31 18:33:58 2020 
 *
 *****************************************************************************
 */

#ifndef MPC_V2_API_H
#define MPC_V2_API_H

/* These macros define the CCP setup. */
#define PCP_CCP_TX_ID         1784
#define PCP_CCP_TX_EXT_ID     0
#define PCP_CCP_RX_ID         1785
#define PCP_CCP_RX_EXT_ID     0
#define PCP_CCP_BUS_BAUD      1000
#define PCP_CCP_ENABLED       1
#define PCP_CCP_RESET_ENABLED 0
#define PCP_CCP_BUS_ID        0
#define PCP_CCP_STATION_ID    0


/* No ISO diagnostic PIDs */



/* No ISO diagnostic routines */


/* Freeze frame declarations
 */
extern U8 pff_data_holding[1];
extern const U16 pff_data_holding_size;
extern const U16 pff_max_nvm;
extern const U8 pff_app_max_num_files[];
extern const U8 pff_num_dm25_spns;
extern U8 pff_iso_ffno_fid_map[];
extern U8 pff_std_ffno_fid_map[];
extern U8 pff_exp_ffno_fid_map[];
extern U8 pff_uds_ffno_fid_map[];
extern const U32 * const pff_dm25_spn_set_ptr;


/* No diagnostic freeze frames */

/* ISO diagnostics declarations (if required) */


/* To hold DM24 const data used in message construction. */
extern const PJ1939_DM24_CONST_DATA_T pj1939_st_ds_spns_list[];
extern const U16 pj1939_num_st_ds_spns;

/* PPR data */


/* No Diagnostic Test Entities */


/* No Diagnostic Monitor group Entities */

/* Function prototypes for each task. */
extern void task_10ms(void);
extern void pkn_10ms_model_task(void);
extern void task_100ms(void);
extern void pkn_100ms_model_task(void);

/* Handles for tasks and resources. */
extern const PKN_TASK_HANDLE_T pkn_task_10ms_task_hdl;
extern const PKN_PERIODIC_TASK_HANDLE_T pkn_task_10ms_periodic_hdl;
extern const PKN_TASK_HANDLE_T pkn_task_100ms_task_hdl;
extern const PKN_PERIODIC_TASK_HANDLE_T pkn_task_100ms_periodic_hdl;

#endif
