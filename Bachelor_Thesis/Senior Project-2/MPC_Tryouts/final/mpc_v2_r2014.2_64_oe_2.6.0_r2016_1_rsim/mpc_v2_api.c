/*
 *****************************************************************************
 *
 *  Project:        OpenECU
 *  File:           mpc_v2_api.c
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


/********* Includes *********/

#include "openecu.h"

#include "mpc_v2_api.h"

/********* These macros can be changed *********/

/* None */

/********* These macros must not be be changed *********/

/* This is the list of tasks available to the platform
 * and strategy tasks. The tasks must appear in priority
 * order, with the highest priority task first. The idle
 * task must always be the lowest task.
 */
enum
{
    PKNL_PSS_TASKTASK_IDX,
    PKNL_PAN_TASKTASK_IDX,
    PKNL_PFC_TASKTASK_IDX,
    PKNL_PSP_RECEIVETASK_IDX,
    PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_IDX,
    PKNL_PCX_CAN_CALLBACKTASK_IDX,
    PKNL_PJ1939_CLIENTTASK_IDX,
    PKNL_PFF_CLIENTTASK_IDX,
    PKNL_PFS_CLIENTTASK_IDX,
    PKNL_PISO_CLIENTTASK_IDX,
    PKNL_PDG_CLIENTTASK_IDX,
    PKNL_TASK_10MSTASK_IDX,
    PKNL_TASK_100MSTASK_IDX,
    PKNL_PDTC_CLIENTTASK_IDX,
    PKNL_PCX_QUEUEEMPTIERTASK_IDX,
    PKNL_PSP_PERIODICTASK_IDX,
    PKNL_PSC_WATCHDOGTASK_IDX,
    PKNL_PCP_CLIENTTASK_IDX,
    PKNL_PKN_IDLETASK_IDX,


    /* Must be last in enumeration */
    PKNL_NUM_TASKS
};

/* These are the periodic tasks. They may appear in any
 * order but the order of periodic task in pkn_periodic_task_table[]
 * must match the enumeration order.
 */
enum
{
    PKNL_PSS_TASKTASK_PIDX,
    PKNL_PFC_TASKTASK_PIDX,
    PKNL_PCX_CAN_CALLBACKTASK_PIDX,
    PKNL_PJ1939_CLIENTTASK_PIDX,
    PKNL_PFF_CLIENTTASK_PIDX,
    PKNL_PFS_CLIENTTASK_PIDX,
    PKNL_PISO_CLIENTTASK_PIDX,
    PKNL_PDG_CLIENTTASK_PIDX,
    PKNL_TASK_10MSTASK_PIDX,
    PKNL_TASK_100MSTASK_PIDX,
    PKNL_PDTC_CLIENTTASK_PIDX,
    PKNL_PCX_QUEUEEMPTIERTASK_PIDX,
    PKNL_PSP_PERIODICTASK_PIDX,
    PKNL_PSC_WATCHDOGTASK_PIDX,
    PKNL_PCP_CLIENTTASK_PIDX,


    /* Must be last in enumeration */
    PKNL_NUM_PERIODIC_TASKS
};

/* These are the priorities of the platform and strategy tasks
 * defined as bit patterns, and the respective ceiling priority of
 * each task. The highest task must start with the highest bit
 * set with subsequent tasks taking the next highest bit.
 */
#define PKNL_PSS_TASKTASK_PRI  (1UL << (31 - PKNL_PSS_TASKTASK_IDX))
#define PKNL_PSS_TASKTASK_CEIL (PKNL_PSS_TASKTASK_PRI | (PKNL_PSS_TASKTASK_PRI-1))
#define PKNL_PAN_TASKTASK_PRI  (1UL << (31 - PKNL_PAN_TASKTASK_IDX))
#define PKNL_PAN_TASKTASK_CEIL (PKNL_PAN_TASKTASK_PRI | (PKNL_PAN_TASKTASK_PRI-1))
#define PKNL_PFC_TASKTASK_PRI  (1UL << (31 - PKNL_PFC_TASKTASK_IDX))
#define PKNL_PFC_TASKTASK_CEIL (PKNL_PFC_TASKTASK_PRI | (PKNL_PFC_TASKTASK_PRI-1))
#define PKNL_PSP_RECEIVETASK_PRI  (1UL << (31 - PKNL_PSP_RECEIVETASK_IDX))
#define PKNL_PSP_RECEIVETASK_CEIL (PKNL_PSP_RECEIVETASK_PRI | (PKNL_PSP_RECEIVETASK_PRI-1))
#define PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_PRI  (1UL << (31 - PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_IDX))
#define PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_CEIL (PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_PRI | (PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_PRI-1))
#define PKNL_PCX_CAN_CALLBACKTASK_PRI  (1UL << (31 - PKNL_PCX_CAN_CALLBACKTASK_IDX))
#define PKNL_PCX_CAN_CALLBACKTASK_CEIL (PKNL_PCX_CAN_CALLBACKTASK_PRI | (PKNL_PCX_CAN_CALLBACKTASK_PRI-1))
#define PKNL_PJ1939_CLIENTTASK_PRI  (1UL << (31 - PKNL_PJ1939_CLIENTTASK_IDX))
#define PKNL_PJ1939_CLIENTTASK_CEIL (PKNL_PJ1939_CLIENTTASK_PRI | (PKNL_PJ1939_CLIENTTASK_PRI-1))
#define PKNL_PFF_CLIENTTASK_PRI  (1UL << (31 - PKNL_PFF_CLIENTTASK_IDX))
#define PKNL_PFF_CLIENTTASK_CEIL (PKNL_PFF_CLIENTTASK_PRI | (PKNL_PFF_CLIENTTASK_PRI-1))
#define PKNL_PFS_CLIENTTASK_PRI  (1UL << (31 - PKNL_PFS_CLIENTTASK_IDX))
#define PKNL_PFS_CLIENTTASK_CEIL (PKNL_PFS_CLIENTTASK_PRI | (PKNL_PFS_CLIENTTASK_PRI-1))
#define PKNL_PISO_CLIENTTASK_PRI  (1UL << (31 - PKNL_PISO_CLIENTTASK_IDX))
#define PKNL_PISO_CLIENTTASK_CEIL (PKNL_PISO_CLIENTTASK_PRI | (PKNL_PISO_CLIENTTASK_PRI-1))
#define PKNL_PDG_CLIENTTASK_PRI  (1UL << (31 - PKNL_PDG_CLIENTTASK_IDX))
#define PKNL_PDG_CLIENTTASK_CEIL (PKNL_PDG_CLIENTTASK_PRI | (PKNL_PDG_CLIENTTASK_PRI-1))
#define PKNL_TASK_10MSTASK_PRI  (1UL << (31 - PKNL_TASK_10MSTASK_IDX))
#define PKNL_TASK_10MSTASK_CEIL (PKNL_TASK_10MSTASK_PRI | (PKNL_TASK_10MSTASK_PRI-1))
#define PKNL_TASK_100MSTASK_PRI  (1UL << (31 - PKNL_TASK_100MSTASK_IDX))
#define PKNL_TASK_100MSTASK_CEIL (PKNL_TASK_100MSTASK_PRI | (PKNL_TASK_100MSTASK_PRI-1))
#define PKNL_PDTC_CLIENTTASK_PRI  (1UL << (31 - PKNL_PDTC_CLIENTTASK_IDX))
#define PKNL_PDTC_CLIENTTASK_CEIL (PKNL_PDTC_CLIENTTASK_PRI | (PKNL_PDTC_CLIENTTASK_PRI-1))
#define PKNL_PCX_QUEUEEMPTIERTASK_PRI  (1UL << (31 - PKNL_PCX_QUEUEEMPTIERTASK_IDX))
#define PKNL_PCX_QUEUEEMPTIERTASK_CEIL (PKNL_PCX_QUEUEEMPTIERTASK_PRI | (PKNL_PCX_QUEUEEMPTIERTASK_PRI-1))
#define PKNL_PSP_PERIODICTASK_PRI  (1UL << (31 - PKNL_PSP_PERIODICTASK_IDX))
#define PKNL_PSP_PERIODICTASK_CEIL (PKNL_PSP_PERIODICTASK_PRI | (PKNL_PSP_PERIODICTASK_PRI-1))
#define PKNL_PSC_WATCHDOGTASK_PRI  (1UL << (31 - PKNL_PSC_WATCHDOGTASK_IDX))
#define PKNL_PSC_WATCHDOGTASK_CEIL (PKNL_PSC_WATCHDOGTASK_PRI | (PKNL_PSC_WATCHDOGTASK_PRI-1))
#define PKNL_PCP_CLIENTTASK_PRI  (1UL << (31 - PKNL_PCP_CLIENTTASK_IDX))
#define PKNL_PCP_CLIENTTASK_CEIL (PKNL_PCP_CLIENTTASK_PRI | (PKNL_PCP_CLIENTTASK_PRI-1))
#define PKNL_PKN_IDLETASK_PRI  (1UL << (31 - PKNL_PKN_IDLETASK_IDX))
#define PKNL_PKN_IDLETASK_CEIL (PKNL_PKN_IDLETASK_PRI | (PKNL_PKN_IDLETASK_PRI-1))

/* This is the list of resources available to the platform
 * and strategy code. The resources may appear in any order
 * but the order of resources in pkn_resource_table[] must
 * match the enumeration order.
 */
enum
{
    PKNL_SPI_QUEUE_RESOURCE,
    PKNL_CAN_QUEUE_RESOURCE,
    PKNL_J1939_BUF_RESOURCE,
    PKNL_PPID_BUF_RESOURCE,
    PKNL_PFS_STRUCTS_RESOURCE,
    PKNL_PFF_BUF_RESOURCE,


    /* Must be last in enumeration */
    PKNL_NUM_RESOURCES
};


/********* Static Structure Definitions *********/

/* None */

/********* Static Prototypes *********/

/* None */

/********* Static Variables *********/

/* None */

/* Routine storage */

/* None */

/********* Global Variable Instantiations *********/

/* These are the application version numbers.
 */
const U16 psc_app_major_ver_num     = 1;
const U16 psc_app_minor_ver_num     = 0;
const U16 psc_app_sub_minor_ver_num = 0;


/* These are the copyright text, app. description text, app. name and version string to be
 * applied to the application's binary image.
 */
const U8 psc_app_copyright[]      = "(none)";
const U8 psc_app_desc[]           = "(none)";
const U8 psc_app_name[]           = "MPC Design";
const U8 psc_app_ver[]            = "1.0.0";


#if !defined(__GCC__)
#pragma section CONST ".text" ".text"
#pragma section SCONST ".text" ".text"
const U16 psc_app_build_sec = 58;
const U16 psc_app_build_min = 33;
const U16 psc_app_build_hour = 18;
const U16 psc_app_build_day = 31;
const U16 psc_app_build_month = 5;
const U16 psc_app_build_year = 2020;
#pragma section CONST
#pragma section SCONST
#else
const U16 psc_app_build_sec  __attribute__ ((section(".text_data"))) = 58;
const U16 psc_app_build_min  __attribute__ ((section(".text_data"))) = 33;
const U16 psc_app_build_hour  __attribute__ ((section(".text_data"))) = 18;
const U16 psc_app_build_day  __attribute__ ((section(".text_data"))) = 31;
const U16 psc_app_build_month  __attribute__ ((section(".text_data"))) = 5;
const U16 psc_app_build_year  __attribute__ ((section(".text_data"))) = 2020;
#endif

const U8 psc_app_build_str[]         = "Sun, 31 May 2020, 18:33:58";



/* System stack definition. */
#if !defined(__GCC__)
#pragma section DATA ".bss_stack" ".bss_stack"
#pragma section SDATA ".bss_stack" ".bss_stack"
U8 psc_system_stack[8192];
#pragma section DATA
#pragma section SDATA
#else
U8 psc_system_stack[8192]  __attribute__ ((section(".bss_stack"))) ;
#endif


/* Specify memory configuration. */
const PIO_MEMORY_CONFIG_T pmem_memory_configuration_option = PIO_MEMORY_CONFIG_A;


/* instantiate internal variable calibrations.
 */

OE_CAL F32 pioc_rate_pot_max_hz                     = 10000.0;
OE_CAL F32 pioc_rate_spot_max_hz                    = 10000.0;



/* Specify location of storage for non-volatile adaptive data. */
const U8 pnv_store = PNV_STORE_IN_FLASH;



/* These are mailboxes that are used to trigger measurement by the XETK */


/* TPU TCR1 scalar -- do not modify */
OE_CAL U16 mplc_tcr1_scalar = 0u;



/* This is the number of tasks available to the scheduler. It is
 * made constant for linker purposes.
 */
const U32 pkn_num_tasks = PKNL_NUM_TASKS;


/* This is the number of periodic tasks available to the scheduler.
 * It is made constant for linker purposes.
 */
const U32 pkn_num_periodic_tasks = PKNL_NUM_PERIODIC_TASKS;


/* This is the number of resources available to the scheduler. It is
 * made constant for linker purposes.
 */
const U32 pkn_num_resources = PKNL_NUM_RESOURCES;

/* This is an array of task information, indexed by the
 * task enumeration. Hence the order must match the enumeration
 * order.
 */
const PKN_TASK_T pkn_task_table[PKNL_NUM_TASKS] =
{
    /* Highest priority */
    { pss_task, PKNL_PSS_TASKTASK_PRI, PKNL_PSS_TASKTASK_CEIL },
    { pan_task, PKNL_PAN_TASKTASK_PRI, PKNL_PAN_TASKTASK_CEIL },
    { pfc_task, PKNL_PFC_TASKTASK_PRI, PKNL_PFC_TASKTASK_CEIL },
    { psp_receive_task, PKNL_PSP_RECEIVETASK_PRI, PKNL_PSP_RECEIVETASK_CEIL },
    { pcx_queue_emptier_task_sporadic, PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_PRI, PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_CEIL },
    { pcx_can_callback_task, PKNL_PCX_CAN_CALLBACKTASK_PRI, PKNL_PCX_CAN_CALLBACKTASK_CEIL },
    { pj1939_client_task, PKNL_PJ1939_CLIENTTASK_PRI, PKNL_PJ1939_CLIENTTASK_CEIL },
    { pff_client_task, PKNL_PFF_CLIENTTASK_PRI, PKNL_PFF_CLIENTTASK_CEIL },
    { pfs_client_task, PKNL_PFS_CLIENTTASK_PRI, PKNL_PFS_CLIENTTASK_CEIL },
    { piso_client_task, PKNL_PISO_CLIENTTASK_PRI, PKNL_PISO_CLIENTTASK_CEIL },
    { pdg_client_task, PKNL_PDG_CLIENTTASK_PRI, PKNL_PDG_CLIENTTASK_CEIL },
    { task_10ms, PKNL_TASK_10MSTASK_PRI, PKNL_TASK_10MSTASK_CEIL },
    { task_100ms, PKNL_TASK_100MSTASK_PRI, PKNL_TASK_100MSTASK_CEIL },
    { pdtc_client_task, PKNL_PDTC_CLIENTTASK_PRI, PKNL_PDTC_CLIENTTASK_CEIL },
    { pcx_queue_emptier_task, PKNL_PCX_QUEUEEMPTIERTASK_PRI, PKNL_PCX_QUEUEEMPTIERTASK_CEIL },
    { psp_periodic_task, PKNL_PSP_PERIODICTASK_PRI, PKNL_PSP_PERIODICTASK_CEIL },
    { psc_watchdog_task, PKNL_PSC_WATCHDOGTASK_PRI, PKNL_PSC_WATCHDOGTASK_CEIL },
    { pcp_client_task, PKNL_PCP_CLIENTTASK_PRI, PKNL_PCP_CLIENTTASK_CEIL },
    { psc_force_box_reset, PKNL_PKN_IDLETASK_PRI, PKNL_PKN_IDLETASK_CEIL },

    /* Lowest priority */
};

/* These are the task handles required to call the
 * scheduler API.
 */
const PKN_TASK_HANDLE_T pkn_pss_task_task_hdl = &pkn_task_table[PKNL_PSS_TASKTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pan_task_task_hdl = &pkn_task_table[PKNL_PAN_TASKTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pfc_task_task_hdl = &pkn_task_table[PKNL_PFC_TASKTASK_IDX];
const PKN_TASK_HANDLE_T pkn_psp_receive_task_hdl = &pkn_task_table[PKNL_PSP_RECEIVETASK_IDX];
const PKN_TASK_HANDLE_T pkn_pcx_queue_emptier_sporadic_task_hdl = &pkn_task_table[PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pcx_can_callback_task_hdl = &pkn_task_table[PKNL_PCX_CAN_CALLBACKTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pj1939_client_task_hdl = &pkn_task_table[PKNL_PJ1939_CLIENTTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pff_client_task_hdl = &pkn_task_table[PKNL_PFF_CLIENTTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pfs_client_task_hdl = &pkn_task_table[PKNL_PFS_CLIENTTASK_IDX];
const PKN_TASK_HANDLE_T pkn_piso_client_task_task_hdl = &pkn_task_table[PKNL_PISO_CLIENTTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pdg_client_task_task_hdl = &pkn_task_table[PKNL_PDG_CLIENTTASK_IDX];
const PKN_TASK_HANDLE_T pkn_task_10ms_task_hdl = &pkn_task_table[PKNL_TASK_10MSTASK_IDX];
const PKN_TASK_HANDLE_T pkn_task_100ms_task_hdl = &pkn_task_table[PKNL_TASK_100MSTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pdtc_client_task_task_hdl = &pkn_task_table[PKNL_PDTC_CLIENTTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pcx_queue_emptier_task_hdl = &pkn_task_table[PKNL_PCX_QUEUEEMPTIERTASK_IDX];
const PKN_TASK_HANDLE_T pkn_psp_periodic_task_hdl = &pkn_task_table[PKNL_PSP_PERIODICTASK_IDX];
const PKN_TASK_HANDLE_T pkn_psc_watchdog_task_hdl = &pkn_task_table[PKNL_PSC_WATCHDOGTASK_IDX];
const PKN_TASK_HANDLE_T pkn_pcp_client_task_hdl = &pkn_task_table[PKNL_PCP_CLIENTTASK_IDX];
const PKN_TASK_HANDLE_T pkn_angular_model_task_hdl = NULL;

/* This is an array of periodic timers, one for each periodic
 * task. The array is indexed by the periodic task enumeration.
 * Range: [0, 2^31) ms
 */
U32 pkn_periodic_records[PKNL_NUM_PERIODIC_TASKS];


/* This is an array of number of times the periodic task has overrun.
 * The array is indexed by the periodic task enumeration.
 * Range: [0, 255] counts
 */
U8 pkn_periodic_task_overrun_count[PKNL_NUM_PERIODIC_TASKS];


/* This is an array of periodic task information, indexed
 * by the periodic task enumeration. Hence the order must
 * match the enumeration order.
 */
const PKN_PERIODIC_TASK_T pkn_periodic_task_table[PKNL_NUM_PERIODIC_TASKS] =
{
    {
        &pkn_periodic_records[PKNL_PSS_TASKTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PSS_TASKTASK_PIDX],
        TRUE,
        4,
        10,
        &pkn_task_table[PKNL_PSS_TASKTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PFC_TASKTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PFC_TASKTASK_PIDX],
        TRUE,
        7,
        100,
        &pkn_task_table[PKNL_PFC_TASKTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PCX_CAN_CALLBACKTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PCX_CAN_CALLBACKTASK_PIDX],
        FALSE,
        8,
        2,
        &pkn_task_table[PKNL_PCX_CAN_CALLBACKTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PJ1939_CLIENTTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PJ1939_CLIENTTASK_PIDX],
        TRUE,
        2,
        5,
        &pkn_task_table[PKNL_PJ1939_CLIENTTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PFF_CLIENTTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PFF_CLIENTTASK_PIDX],
        TRUE,
        3,
        10,
        &pkn_task_table[PKNL_PFF_CLIENTTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PFS_CLIENTTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PFS_CLIENTTASK_PIDX],
        TRUE,
        3,
        10,
        &pkn_task_table[PKNL_PFS_CLIENTTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PISO_CLIENTTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PISO_CLIENTTASK_PIDX],
        TRUE,
        1,
        3,
        &pkn_task_table[PKNL_PISO_CLIENTTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PDG_CLIENTTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PDG_CLIENTTASK_PIDX],
        TRUE,
        2,
        10,
        &pkn_task_table[PKNL_PDG_CLIENTTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_TASK_10MSTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_TASK_10MSTASK_PIDX],
        TRUE,
        0,
        10,
        &pkn_task_table[PKNL_TASK_10MSTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_TASK_100MSTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_TASK_100MSTASK_PIDX],
        TRUE,
        0,
        100,
        &pkn_task_table[PKNL_TASK_100MSTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PDTC_CLIENTTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PDTC_CLIENTTASK_PIDX],
        TRUE,
        3,
        100,
        &pkn_task_table[PKNL_PDTC_CLIENTTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PCX_QUEUEEMPTIERTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PCX_QUEUEEMPTIERTASK_PIDX],
        FALSE,
        3,
        2,
        &pkn_task_table[PKNL_PCX_QUEUEEMPTIERTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PSP_PERIODICTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PSP_PERIODICTASK_PIDX],
        TRUE,
        0,
        1,
        &pkn_task_table[PKNL_PSP_PERIODICTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PSC_WATCHDOGTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PSC_WATCHDOGTASK_PIDX],
        TRUE,
        0,
        200,
        &pkn_task_table[PKNL_PSC_WATCHDOGTASK_IDX]
    },
    {
        &pkn_periodic_records[PKNL_PCP_CLIENTTASK_PIDX],
        &pkn_periodic_task_overrun_count[PKNL_PCP_CLIENTTASK_PIDX],
        TRUE,
        6,
        5,
        &pkn_task_table[PKNL_PCP_CLIENTTASK_IDX]
    },
};

/* These are the periodic task handles required to call the
 * scheduler API.
 */
const PKN_PERIODIC_TASK_HANDLE_T pkn_pss_task_periodic_hdl = &pkn_periodic_task_table[PKNL_PSS_TASKTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pfc_task_periodic_hdl = &pkn_periodic_task_table[PKNL_PFC_TASKTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pcx_can_callback_periodic_hdl = &pkn_periodic_task_table[PKNL_PCX_CAN_CALLBACKTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pj1939_client_periodic_hdl = &pkn_periodic_task_table[PKNL_PJ1939_CLIENTTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pff_client_periodic_hdl = &pkn_periodic_task_table[PKNL_PFF_CLIENTTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pfs_client_periodic_hdl = &pkn_periodic_task_table[PKNL_PFS_CLIENTTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_piso_client_task_periodic_hdl = &pkn_periodic_task_table[PKNL_PISO_CLIENTTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pdg_client_task_periodic_hdl = &pkn_periodic_task_table[PKNL_PDG_CLIENTTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_task_10ms_periodic_hdl = &pkn_periodic_task_table[PKNL_TASK_10MSTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_task_100ms_periodic_hdl = &pkn_periodic_task_table[PKNL_TASK_100MSTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pdtc_client_task_periodic_hdl = &pkn_periodic_task_table[PKNL_PDTC_CLIENTTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pcx_queue_emptier_periodic_hdl = &pkn_periodic_task_table[PKNL_PCX_QUEUEEMPTIERTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_psp_periodic_periodic_hdl = &pkn_periodic_task_table[PKNL_PSP_PERIODICTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_psc_watchdog_periodic_hdl = &pkn_periodic_task_table[PKNL_PSC_WATCHDOGTASK_PIDX];
const PKN_PERIODIC_TASK_HANDLE_T pkn_pcp_client_periodic_hdl = &pkn_periodic_task_table[PKNL_PCP_CLIENTTASK_PIDX];


/* This is an array of resource ceiling records, indexed by
 * the resource enumeration. Each entry contains the priority
 * ceiling of the task which acquired the resource so that
 * when the task releases the resource, the task's previous
 * priority ceiling can be restored.
 */
PKN_TASKSET_T pkn_ceiling_records[PKNL_NUM_RESOURCES];


/* This is an array of resource priority ceiling information,
 * indexed by the resource enumeration. Hence the order must
 * match the enumeration order.
 */
const PKN_RESOURCE_T pkn_resource_table[PKNL_NUM_RESOURCES] =
{
    {
        &pkn_ceiling_records[PKNL_SPI_QUEUE_RESOURCE],
        PKNL_PSP_RECEIVETASK_PRI | PKNL_PSP_PERIODICTASK_PRI | PKNL_TASK_10MSTASK_PRI | PKNL_TASK_100MSTASK_PRI | PKNL_PKN_IDLETASK_PRI | PKNL_PAN_TASKTASK_PRI | PKNL_PCX_QUEUEEMPTIERTASK_PRI | PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_PRI,
        PKNL_PSP_RECEIVETASK_CEIL | PKNL_PSP_PERIODICTASK_CEIL | PKNL_TASK_10MSTASK_CEIL | PKNL_TASK_100MSTASK_CEIL | PKNL_PKN_IDLETASK_CEIL | PKNL_PAN_TASKTASK_CEIL | PKNL_PCX_QUEUEEMPTIERTASK_CEIL | PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_CEIL,
    },
    {
        &pkn_ceiling_records[PKNL_CAN_QUEUE_RESOURCE],
        PKNL_PCX_QUEUEEMPTIERTASK_PRI | PKNL_PCX_CAN_CALLBACKTASK_PRI | PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_PRI | PKNL_TASK_10MSTASK_PRI | PKNL_TASK_100MSTASK_PRI | PKNL_PKN_IDLETASK_PRI | PKNL_PCP_CLIENTTASK_PRI | PKNL_PJ1939_CLIENTTASK_PRI,
        PKNL_PCX_QUEUEEMPTIERTASK_CEIL | PKNL_PCX_CAN_CALLBACKTASK_CEIL | PKNL_PCX_QUEUE_EMPTIER_SPORADICTASK_CEIL | PKNL_TASK_10MSTASK_CEIL | PKNL_TASK_100MSTASK_CEIL | PKNL_PKN_IDLETASK_CEIL | PKNL_PCP_CLIENTTASK_CEIL | PKNL_PJ1939_CLIENTTASK_CEIL,
    },
    {
        &pkn_ceiling_records[PKNL_J1939_BUF_RESOURCE],
        PKNL_PJ1939_CLIENTTASK_PRI | PKNL_TASK_10MSTASK_PRI | PKNL_TASK_100MSTASK_PRI | PKNL_PKN_IDLETASK_PRI,
        PKNL_PJ1939_CLIENTTASK_CEIL | PKNL_TASK_10MSTASK_CEIL | PKNL_TASK_100MSTASK_CEIL | PKNL_PKN_IDLETASK_CEIL,
    },
    {
        &pkn_ceiling_records[PKNL_PPID_BUF_RESOURCE],
        PKNL_PDG_CLIENTTASK_PRI | PKNL_PISO_CLIENTTASK_PRI | PKNL_TASK_10MSTASK_PRI | PKNL_TASK_100MSTASK_PRI | PKNL_PKN_IDLETASK_PRI,
        PKNL_PDG_CLIENTTASK_CEIL | PKNL_PISO_CLIENTTASK_CEIL | PKNL_TASK_10MSTASK_CEIL | PKNL_TASK_100MSTASK_CEIL | PKNL_PKN_IDLETASK_CEIL,
    },
    {
        &pkn_ceiling_records[PKNL_PFS_STRUCTS_RESOURCE],
        PKNL_PFS_CLIENTTASK_PRI | PKNL_TASK_10MSTASK_PRI | PKNL_TASK_100MSTASK_PRI | PKNL_PKN_IDLETASK_PRI | PKNL_PFF_CLIENTTASK_PRI | PKNL_PDG_CLIENTTASK_PRI | PKNL_PDTC_CLIENTTASK_PRI | PKNL_PCP_CLIENTTASK_PRI,
        PKNL_PFS_CLIENTTASK_CEIL | PKNL_TASK_10MSTASK_CEIL | PKNL_TASK_100MSTASK_CEIL | PKNL_PKN_IDLETASK_CEIL | PKNL_PFF_CLIENTTASK_CEIL | PKNL_PDG_CLIENTTASK_CEIL | PKNL_PDTC_CLIENTTASK_CEIL | PKNL_PCP_CLIENTTASK_CEIL,
    },
    {
        &pkn_ceiling_records[PKNL_PFF_BUF_RESOURCE],
        PKNL_PFF_CLIENTTASK_PRI | PKNL_TASK_10MSTASK_PRI | PKNL_TASK_100MSTASK_PRI | PKNL_PKN_IDLETASK_PRI,
        PKNL_PFF_CLIENTTASK_CEIL | PKNL_TASK_10MSTASK_CEIL | PKNL_TASK_100MSTASK_CEIL | PKNL_PKN_IDLETASK_CEIL,
    },

};



/* These are the resource handles required to call the scheduler API.
*/
const PKN_RESOURCE_HANDLE_T pkn_psp_logical_queues_r_hdl = &pkn_resource_table[PKNL_SPI_QUEUE_RESOURCE];
const PKN_RESOURCE_HANDLE_T pkn_can_queues_r_hdl = &pkn_resource_table[PKNL_CAN_QUEUE_RESOURCE];
const PKN_RESOURCE_HANDLE_T pkn_j1939_buf_r_hdl = &pkn_resource_table[PKNL_J1939_BUF_RESOURCE];
const PKN_RESOURCE_HANDLE_T pkn_ppid_buf_r_hdl = &pkn_resource_table[PKNL_PPID_BUF_RESOURCE];
const PKN_RESOURCE_HANDLE_T pkn_pfs_structs_r_hdl = &pkn_resource_table[PKNL_PFS_STRUCTS_RESOURCE];
const PKN_RESOURCE_HANDLE_T pkn_pff_buff_r_hdl = &pkn_resource_table[PKNL_PFF_BUF_RESOURCE];


/* This is an array, indexed by task priority, that contains
 * the start time of the task or the restart time of the
 * task if it was interrupted.
 */
volatile U32 pkn_task_start_time[PKNL_NUM_TASKS];


/* This is an array, indexed by task priority, that contains
 * the partially accumulated run time of the task.
 */
volatile U32 pkn_task_accum_time[PKNL_NUM_TASKS];




/* This is an array, indexed by task priority, that contains
 * the uninterrupted run time of the task the last time it ran.
 */
volatile U32 pkn_task_time[PKNL_NUM_TASKS];

/* This is an array, indexed by task priority, that contains
 * the maximum uninterrupted run time of the task for every
 * time it has run.
 */
volatile U32 pkn_max_task_time[PKNL_NUM_TASKS];



/* Create the calibration header (which is altered later on in the build process).
 */
#if !defined(__GCC__)
#pragma section CONST ".cal_header_sec" ".cal_header_sec"
#pragma section SCONST ".cal_header_sec" ".cal_header_sec"
const PHDR_HEADER_T psc_calibration_header;
#pragma section CONST
#pragma section SCONST
#else
const PHDR_HEADER_T psc_calibration_header  __attribute__ ((section(".cal_header_sec"))) ;
#endif


const PCX_HANDLE_T pcx_tot_allow_msg_tx = 2;
const PCX_HANDLE_T pcx_tot_allow_msg_rx = 2;

PCX_RX_MSG_T pcx_rx_msg[2];
PCX_HANDLE_T pcx_sorted_rx_msg[2];

PCX_TX_MSG_T pcx_tx_msg[2];
PCX_HANDLE_T pcx_queued_tx_list[3][PCX_NUM_CAN_DEVICES];



/* J1939 Configuration */


const U8 pj1939_enabled = 0;



const U8 name[] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

OE_CAL U8 pj1939c_node_addr_0 = 0;

U8 node_addr[2] =
{
    0,  /* Set to pj1939c_node_addr_0 during initialisation */
    255 /* The global address is always required to be last */
};

const U8 pj1939_num_node_addr = 2;

const PGN_T _pgn_table[] = { 0 };
const U32 pj1939_num_pgns = 0;

const PGN_T pj1939_pgn_requested_table[] = { 0 };
U8 pj1939_pgn_requested_src_addr[1];
U8 pj1939_pgn_requested_dest_addr[1];
U32 pj1939_pgn_requested_timestamp[1];
U8 pj1939_pgn_requested_bitmap[1];
const U32 pj1939_num_requested_pgns =  0;


U16 pj1939_rx_buf_data_size[1] = { 0 };
U8 *const pj1939_rx_buf_data[] = { 0 };
PJ1939_RX_BUF_T *const pj1939_rx_buf[1] = { NULL };



const U8 pj1939_can_link = 0;
const U8 pj1939_num_ttx = 1;
const U8 pj1939_num_trx = 1;
const U16 pj1939_size_j1939_rx_buf = 1785;
const U8 pj1939_num_rx_tx_bufs = 1;

const U8 pj1939_dm7_request_buf_size = 1;

const BOOL pj1939_use_common_mf_priority = FALSE;

const U8 pj1939_common_multiframe_priority = 7;

PJ1939_DM7_BUFFER_T  pj1939_req_test_list[] =
{
{0,0,0,0,0}
};

J1939_TX_MESSAGE_T pj1939_ttx_buf[1];
U8 pj1939_ttx_buf_data[1785 * 1];
U8 *pj1939_ttx_error_ptr[1];

U8 pj1939_msg_buffer[1785 * 1];
J1939_TRANSPORT_TX_INFO tx_state[1];
J1939_MESSAGE rx_message[1];
CAN_PACKET_T in_queue[1];
CAN_PACKET_T out_queue[1];

const U8 pj1939_dm1_source_addr_num = 0;
const U8 pj1939_dm1_source_addr[1] = { 0 };


U8 *const pj1939_dm1_rx_buf_data[1] = { NULL };

PJ1939_RX_BUF_T *const pj1939_dm1_rx_buf[1] = { NULL };

U32 pj1939_dm1_rx_counters[1] = { 0 };

const U8 pj1939_dm2_source_addr_num = 0;
const U8 pj1939_dm2_source_addr[1] = { 0 };


U8 *const pj1939_dm2_rx_buf_data[1] = { NULL };

PJ1939_RX_BUF_T *const pj1939_dm2_rx_buf[1] = { NULL };

U32 pj1939_dm2_rx_counters[1] = { 0 };


PJ1939_AECD_T * const pj1939_aecd_table[1] = {NULL};
const U8 pj1939_num_aecd = 0;

OE_CAL U32 svcc_ecu_config_seedkey_cal = 0x10ACED08;
OE_CAL U32 svcc_ecu_reprog_seedkey_cal = 0x10DEED82;
volatile const U32* pj1939_mem_sec_config_cal = &svcc_ecu_config_seedkey_cal;
volatile const U32* pj1939_mem_sec_reprog_cal = &svcc_ecu_reprog_seedkey_cal;

/* ISO diagnostic parameters */

/*  (not enabled)  */
const BOOL pdg_enabled = 0;

OE_CAL U32                pdgc_can_tx_id                  = 0x0;
OE_CAL BOOL               pdgc_can_tx_id_extd             = 0;
OE_CAL U32                pdgc_can_rx_id                  = 0x0;
OE_CAL BOOL               pdgc_can_rx_id_extd             = 0;
OE_CAL PCX_LCHAN_T        pdgc_can_bus_id                 = 0;

OE_CAL PIO_EMISSION_SEV_TYPE_T pdgc_emissions_report_min_sev  = PIO_ESEV_C;

/* ISO 15765-2 message storage (minimal size if not used) */

U8 pdg_iso_tx_buffer[1];
const S16 pdg_iso_tx_buffer_size           = 1;

const PDG_GENERAL_CALLBACK_FN_T pdg_general_callback_ptr = NULL;
const PDG_ID_REQUEST_CALLBACK_FN_T pdg_id_request_callback_ptr = NULL;
const PDG_SECURITY_CALLBACK_FN_T pdg_security_callback_ptr = NULL;
const PDG_SECURITY_END_FN_T pdg_security_end_ptr = NULL;
const PDG_SECURITY_OPTION_T pdg_security_flash_allowed_option = PDG_SECURITY_NEVER_ALLOWED;
const PDG_SECURITY_OPTION_T pdg_security_mem_read_allowed_option = PDG_SECURITY_NEVER_ALLOWED;
const U8 pdg_num_flash_security_levels = 0;
const U8 pdg_specific_flash_security_level[1] = {0}; /* Dummy entry */
const U8 pdg_num_mem_read_security_levels = 0;
const U8 pdg_specific_mem_read_security_level[1] = {0}; /* Dummy entry */
const BOOL pdg_mem_read_ok_def_session  = 0;
const BOOL pdg_mem_read_ok_extd_session = 0;
const BOOL pdg_mem_read_ok_prog_session = 0;
const BOOL pdg_mem_read_needs_security  = 0;
const BOOL pdg_active_session_has_id16 = FALSE;
const U16 pdg_active_session_iso_16bit_id = 0x0000; /* unused */
const BOOL pdg_active_session_has_id8 = FALSE;
const U8 pdg_active_session_kwp_8bit_id = 0x00; /* unused */
U8 pdg_iso_rx_buffer[1];
const S16 pdg_iso_rx_buffer_size           = 1;

/* Definitions for functional addressing */
OE_CAL U32 pdgc_func_can_rx_id  = 0x0;

OE_CAL PIO_SERVICE_03_OVERRIDE_OPTIONS_T pdgc_override_service_03  = PIO_SERVICE_03_NO_OVERRIDE;
OE_CAL PIO_SERVICE_07_OVERRIDE_OPTIONS_T pdgc_override_service_07  = PIO_SERVICE_07_NO_OVERRIDE;
OE_CAL PIO_SERVICE_0A_OVERRIDE_OPTIONS_T pdgc_override_service_0a  = PIO_SERVICE_0A_NO_OVERRIDE;

const U8 pdg_extd_data_rec_num[PDG_EXTD_NUM_RECORDS] = {0, 0, 0, 0, 0, 0, 0, 0};

const U16 pdg_pdid_base_period = 1000;
const U16 pdg_ddid_pool_num_bytes = 0;
U8 pdg_ddid_pool_data_array[1];
const U16 pdg_pdid_pool_num_bytes = 0;
U8 pdg_pdid_pool_data_array[1];

/* PID data table */

const struct PPID_PID_CONST_T ppid_table[1] = {0, 0, 0, NULL, NULL, NULL};

const U16 ppid_num_pids = 0;

const struct PPID_8BIT_ID_LOOKUP_T ppid_j1979_8bit_map[] =
{
    {0, 0}
};

const U16 ppid_num_j1979_ids = 0;

const struct PPID_8BIT_ID_LOOKUP_T ppid_kwp_8bit_map[] =
{
    {0, 0}
};

const U16 ppid_num_kwp_ids = 0;

const struct PPID_J1939_SPN_LOOKUP_T ppid_j1939_spn_map[] =
{
    {0, 0}
};

const U16 ppid_num_j1939_spns = 0;


/* Non-volatile PID definitions */
const PPID_NV_PID_T ppid_app_allowed_nv_pids[1] =
{ /* len, min, max, 16-bit,  KWP,                                                 flags,                                 SPN */
    {   0,      0,                            0,                                    0,      0}    /* none defined */
};

const U8 ppid_num_app_nv_pids = 0;
U8 ppid_nv_buffer[1];
const U16 ppid_nv_buffer_size = 1;
const BOOL ppid_allow_only_app_nv = TRUE;

/* Routine data table */

struct PDG_ROUTINE_T pdg_routine_table[1] = {0, 0, 0, 0, NULL, NULL, NULL, FALSE, FALSE, FALSE, PDG_ROUTINE_CTRL_NO_RQST, PDG_ROUTINE_CTRL_NO_RQST, PDG_ROUTINE_INACTIVE, FALSE};

const U16 pdg_num_routines = 0;


/* Allocate RAM for the freeze frame buffer (aka holding array).
 */
U8 pff_data_holding[1];
const U16 pff_data_holding_size = 0;

/* Set NVM maximum limit for freeze frame storage.
 */
const U16 pff_max_nvm = 0;

/* Set up the FFNO->FID (file ID) mapping array for j1979 freeze frames.
 */
U8 pff_iso_ffno_fid_map[1] = {255};
/* Set up the FFNO->FID (file ID) mapping array for j1939 DM4 freeze frames.
 */
U8 pff_std_ffno_fid_map[1] = {255};
/* Set up the FFNO->FID (file ID) mapping array for j1939 DM25 freeze frames.
 */
U8 pff_exp_ffno_fid_map[1] = {255};
/* Set up the FFNO->FID (file ID) mapping array for UDS Snapshot freeze frames.
 */
U8 pff_uds_ffno_fid_map[1] = {255};
const U8 pff_app_max_num_files[4] = { 0,  0,  0,  0 };

/* Set the number of SPNs in the J1939 DM25 expanded freeze frame.
 */
const U8 pff_num_dm25_spns = 0;
const U32 * const pff_dm25_spn_set_ptr = NULL;

/* Freeze frame data
 */
const BOOL pff_dtc_sev_overrides_ff_age  = FALSE;   /* In case of lack of storage space, latest DTC will be discarded */
PFF_DTC_SEV_FF_INDEX_T pff_dtc_sev_and_ff_idx_list[1] = {{PIO_ESEV_NONE,0}, };    /* Not used here */

/* PPR data */
const U8 ppr_store = 1;

/* DTE data table */

const PPR_DTE_CONST_T ppr_dte_table[1] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL};

const U16 ppr_num_dte = 0;

/* The test mapping for DM10 message for supported tests */
const U8 ppr_j1939_test_mapping_list[64] = {
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                    0x00,
                                  };


/* DME data table */

const PPR_DME_CONST_T ppr_dme_table[1] = {0, 0, 0, (PIO_MONITOR_GROUP_T)0, 0, 0, NULL};

const U16 ppr_num_dme = 0;

/* PFS data */

/* The size constants should not be altered using external calibration tool at runtime
 * They should only be specified in the capi configuration file
 */
const U16 pfs_write_queue_size = 17;
const U16 pfs_max_num_user_files = 0;
const U16 pfs_max_num_platform_files = 20;

volatile PFS_FILE_WRITE_ENTRY_T pfs_write_queue[17];
volatile PFS_FILE_DIR_ENTRY_T   pfs_directory[20];


/* Whether CCP is enabled or not */
const U8 pcp_ccpenabled = PCP_CCP_ENABLED;

/* CCP DAQ and ODT declarations */
const U8                ccp_daq_size[4] = { 15, 15, 15, 15,  };
      PCCP_DAQ_LIST_T   ccp_daq_list[4];
      PCCP_ODT_ENTRY_T  ccp_odt_list[60][7];
const U8                ccp_first_odt[4] = { 0, 15, 30, 45,  };
const U8                ccp_num_daqs = 4;
const U8                ccp_num_odts = 60;

/* CCP security data */

const PCP_SEED_KEY_CONFIG_T pcp_seed_key_config[1] = { {PCP_PL_ALL, FALSE, NULL, NULL, NULL, NULL} };

const U8 pcp_num_seed_key_configs = 1;

const U8 pcp_num_seed_key_files = 1;
const BOOL pcp_security_dev_mode = FALSE;


/* Whether watchdog task is enabled or not */
const U8 psc_watchdog_task_enabled = TRUE;


/* Run-time memory check settings */
const BOOL psc_mem_runtime_checks_enabled = TRUE;


/* Define the location of the non-volatile DTC store.
 */
const U8 pdtc_store = PDTC_STORE_IN_FLASH;

/* Select the priority scheme to calculate overall lamp states for multiple DTCs.
 * TRUE = fast flash - slow flash - on - off
 * FALSE = on - fast flash - slow flash - off
 */
const BOOL pdtc_lamp_flashing_is_priority = FALSE;

/* Select DTC state transitions allowed from previously active state.
 * TRUE  = If DTC is in previously active state and fault recurs, DTC goes
 *         to pending state.
 *         If fault recurs on next drive cycle, DTC goes to active state.
 *         If fault does not recur on next drive cycle, DTC returns to
 *         previously active state.
 * FALSE = If DTC is previously active and fault recurs, DTC goes directly
 *         to active state.
 */
const BOOL pdtc_transition_prev_act_to_pend = TRUE;

/* Define the non-volatile variable data for each DTC.
 */

/* Records of first/most recent DTC to fail/be confirmed for certain UDS $19 subfunctions
 * and non-volatile counters for EuroVI statistics, etc.
 */

/* Records of first/most recent DTC to fail/be confirmed for certain UDS $19 subfunctions
 * and non-volatile counters for EuroVI statistics, etc
 */
#if !defined(__GCC__)
#pragma section DATA ".dtcs" ".dtcs"
#pragma section SDATA ".dtcs" ".dtcs"
PDTC_DTC_ID_T      pdtc_first_last_record[PDTC_NUM_FIRST_LAST];
PDTC_NV_COUNTERS_T pdtc_nv_counters;
PDTC_DTC_ID_T pdtc_first_last_record[PDTC_NUM_FIRST_LAST];
PDTC_NV_COUNTERS_T pdtc_nv_counters;
#pragma section DATA
#pragma section SDATA
#else
PDTC_DTC_ID_T      pdtc_first_last_record[PDTC_NUM_FIRST_LAST]  __attribute__ ((section(".dtcs"))) ;
PDTC_NV_COUNTERS_T pdtc_nv_counters  __attribute__ ((section(".dtcs"))) ;
PDTC_DTC_ID_T pdtc_first_last_record[PDTC_NUM_FIRST_LAST]  __attribute__ ((section(".dtcs"))) ;
PDTC_NV_COUNTERS_T pdtc_nv_counters  __attribute__ ((section(".dtcs"))) ;
#endif

/* Define the volatile variable data for each DTC.
 */

/* Define the constant data for each DTC.
 */

/* Define the DTC pointer lists for each DTC table.
 */
const PDTC_DTC_T *const pdtc_dtcs_for_table_all[] =
{
    NULL
};

const PDTC_TABLE_T pdtc_table_all =
{
    0,
    0, /* Should never be used for J1939 send/receive */
    &(pdtc_dtcs_for_table_all[0]),
    NULL
};

/* List of DTC tables (not including table of all DTCs) */
const U16 pdtc_num_dtc_tables = 0;
const PDTC_TABLE_T * const pdtc_table_list[1] =
{
    NULL
};

/* Define state history for DTCs for the J1939 feature.
 */
U8 pj1939_state[1];
U8 pj1939_sent[1];
U8 pj1939_transient[1];

/* Ordered list of SPNs and flag info identifying
 * DM24 data: Scaled Test Results and Data Stream.
 */

const PJ1939_DM24_CONST_DATA_T pj1939_st_ds_spns_list[1] = {255, 0x00};

const U16 pj1939_num_st_ds_spns = 0;


/* Pi internal CAPI items */
const BOOL ptpu_override_sdm_allocation = FALSE;


/*
 *****************************************************************************
 *  Purpose:        Complete a periodic rate iteration of the application,
 *                  pass all data consistently and trigger any SPI
 *                  I/O for this rate.
 *  Returns:        void
 *  Notes:          None.
 *****************************************************************************
 */
void task_10ms(void)
{
    pkn_10ms_model_task();
    psp_spi_trigger();
}



/*
 *****************************************************************************
 *  Purpose:        Complete a periodic rate iteration of the application,
 *                  pass all data consistently and trigger any SPI
 *                  I/O for this rate.
 *  Returns:        void
 *  Notes:          None.
 *****************************************************************************
 */
void task_100ms(void)
{
    pkn_100ms_model_task();
    psp_spi_trigger();
}

