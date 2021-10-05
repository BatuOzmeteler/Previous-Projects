/*
 * mpc_v2.h
 *
 * Code generation for model "mpc_v2".
 *
 * Model version              : 1.138
 * Simulink Coder version : 8.7 (R2014b) 08-Sep-2014
 * C source code generated on : Sun May 31 18:32:42 2020
 *
 * Target selection: openecu.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Freescale->32-bit PowerPC
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#ifndef RTW_HEADER_mpc_v2_h_
#define RTW_HEADER_mpc_v2_h_
#include <stddef.h>
#include <math.h>
#include <string.h>
#ifndef mpc_v2_COMMON_INCLUDES_
# define mpc_v2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif                                 /* mpc_v2_COMMON_INCLUDES_ */

#include "mpc_v2_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rt_zcfcn.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"

/* Include all OpenECU platform headers. */
#include "openecu.h"

/* Include all auto-generated platform headers. */
#include "mpc_v2_api.h"
#define MODEL_NAME                     mpc_v2
#define NSAMPLE_TIMES                  (2)                       /* Number of sample times */
#define NINPUTS                        (0)                       /* Number of model inputs */
#define NOUTPUTS                       (0)                       /* Number of model outputs */
#define NBLOCKIO                       (9)                       /* Number of data output port signals */
#define NUM_ZC_EVENTS                  (1)                       /* Number of zero-crossing events */
#ifndef NCSTATES
# define NCSTATES                      (0)                       /* Number of continuous states */
#elif NCSTATES != 0
# error Invalid specification of NCSTATES defined in compiler command
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T xk1[2];                       /* '<S7>/optimizer' */
  real32_T RateTransition;             /* '<Root>/Rate Transition' */
  real32_T DataTypeConversion1;        /* '<Root>/Data Type Conversion1' */
  real32_T DataTypeConversion;         /* '<Root>/Data Type Conversion' */
  boolean_T pcx_CANReceiveMessage_o1;  /* '<Root>/pcx_CANReceiveMessage' */
  boolean_T pcx_CANReceiveMessage_o2;  /* '<Root>/pcx_CANReceiveMessage' */
  boolean_T pcx_CANReceiveMessage_o3;  /* '<Root>/pcx_CANReceiveMessage' */
  boolean_T pcx_CANTransmitMessage;    /* '<Root>/pcx_CANTransmitMessage' */
  boolean_T iAout[5];                  /* '<S7>/optimizer' */
} B_mpc_v2_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay_DSTATE;             /* '<Root>/Unit Delay' */
  real_T last_mv_DSTATE;               /* '<S7>/last_mv' */
  real_T last_x_PreviousInput[2];      /* '<S7>/last_x' */
  real_T Memory_PreviousInput;         /* '<Root>/Memory' */
  struct {
    void *PrevTimePtr;
  } FromFile_PWORK;                    /* '<S5>/From File' */

  struct {
    void *PrevTimePtr;
  } FromFile1_PWORK;                   /* '<S5>/From File1' */

  struct {
    void *PrevTimePtr;
  } FromFile2_PWORK;                   /* '<S5>/From File2' */

  struct {
    void *PrevTimePtr;
  } FromFile3_PWORK;                   /* '<S5>/From File3' */

  int32_T clockTickCounter;            /* '<Root>/Pulse Generator' */
  boolean_T Memory_PreviousInput_k[5]; /* '<S7>/Memory' */
} DW_mpc_v2_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState data_out_Trig_ZCE;        /* '<Root>/data_out' */
} PrevZCX_mpc_v2_T;

/* Constant parameters (auto storage) */
typedef struct {
  /* Computed Parameter: OFF0P1R2N3D4_Value
   * Referenced by: '<Root>/OFF = 0, P = 1 R = 2, N = 3, D = 4'
   */
  real32_T OFF0P1R2N3D4_Value;

  /* Computed Parameter: FbrakeN_Value
   * Referenced by: '<Root>/Fbrake (N)'
   */
  real32_T FbrakeN_Value;
} ConstP_mpc_v2_T;

/* Constant parameters (auto storage) */
extern const ConstP_mpc_v2_T mpc_v2_rtConstP;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real_T v_ref;                   /* '<S5>/From File3' */
extern real_T Grade;                   /* '<S5>/MATLAB Function' */
extern real_T trq_req;                 /* '<Root>/Torque Regulation' */
extern real_T v_current;               /* '<Root>/Speed Regulation' */
extern real_T trq_out;                 /* '<S7>/optimizer' */
extern real32_T v_raw;                 /* '<Root>/pcx_CANReceiveMessage' */
extern boolean_T time_tick_ecu;        /* '<S1>/Compare' */
extern int8_T pcx_CANReceiveMessage_msg_handle;
extern int8_T pcx_CANTransmitMessage_msg_handle;

/* Simulation Structure */
extern SimStruct *const rtS;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S8>/Matrix Dimension Check' : Unused code path elimination
 * Block '<S9>/Matrix Dimension Check' : Unused code path elimination
 * Block '<S10>/Vector Dimension Check' : Unused code path elimination
 * Block '<S11>/Vector Dimension Check' : Unused code path elimination
 * Block '<S12>/Vector Dimension Check' : Unused code path elimination
 * Block '<S13>/Vector Dimension Check' : Unused code path elimination
 * Block '<S14>/Vector Dimension Check' : Unused code path elimination
 * Block '<S15>/Vector Dimension Check' : Unused code path elimination
 * Block '<S16>/Vector Dimension Check' : Unused code path elimination
 * Block '<S17>/Vector Dimension Check' : Unused code path elimination
 * Block '<S18>/Vector Dimension Check' : Unused code path elimination
 * Block '<S19>/Vector Dimension Check' : Unused code path elimination
 * Block '<S20>/Vector Dimension Check' : Unused code path elimination
 * Block '<S21>/Vector Dimension Check' : Unused code path elimination
 * Block '<S7>/constant' : Unused code path elimination
 * Block '<S7>/umin_scale2' : Unused code path elimination
 * Block '<S7>/umin_scale3' : Unused code path elimination
 * Block '<S7>/ym_zero' : Unused code path elimination
 * Block '<S7>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion10' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion11' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion12' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion13' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion4' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion5' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion6' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion7' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion8' : Eliminate redundant data type conversion
 * Block '<S7>/Data Type Conversion9' : Eliminate redundant data type conversion
 * Block '<S7>/ext.mv_scale' : Eliminated nontunable gain of 1
 * Block '<S7>/ext.mv_scale1' : Eliminated nontunable gain of 1
 * Block '<S7>/mo or x Conversion' : Eliminate redundant data type conversion
 * Block '<S7>/umax_scale' : Eliminated nontunable gain of 1
 * Block '<S7>/umin_scale' : Eliminated nontunable gain of 1
 * Block '<S7>/umin_scale1' : Eliminated nontunable gain of 1
 * Block '<S7>/ymax_scale' : Eliminated nontunable gain of 1
 * Block '<S7>/ymin_scale' : Eliminated nontunable gain of 1
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'mpc_v2'
 * '<S1>'   : 'mpc_v2/Compare To Constant1'
 * '<S2>'   : 'mpc_v2/MPC Controller'
 * '<S3>'   : 'mpc_v2/Speed Regulation'
 * '<S4>'   : 'mpc_v2/Torque Regulation'
 * '<S5>'   : 'mpc_v2/data_out'
 * '<S6>'   : 'mpc_v2/pcp_CCPConfigur  ation'
 * '<S7>'   : 'mpc_v2/MPC Controller/MPC'
 * '<S8>'   : 'mpc_v2/MPC Controller/MPC/MPC Preview Signal Check'
 * '<S9>'   : 'mpc_v2/MPC Controller/MPC/MPC Preview Signal Check1'
 * '<S10>'  : 'mpc_v2/MPC Controller/MPC/MPC Scalar Signal Check'
 * '<S11>'  : 'mpc_v2/MPC Controller/MPC/MPC Scalar Signal Check1'
 * '<S12>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check'
 * '<S13>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check11'
 * '<S14>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check2'
 * '<S15>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check3'
 * '<S16>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check4'
 * '<S17>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check5'
 * '<S18>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check6'
 * '<S19>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check7'
 * '<S20>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check8'
 * '<S21>'  : 'mpc_v2/MPC Controller/MPC/MPC Vector Signal Check9'
 * '<S22>'  : 'mpc_v2/MPC Controller/MPC/optimizer'
 * '<S23>'  : 'mpc_v2/data_out/MATLAB Function'
 */
#endif                                 /* RTW_HEADER_mpc_v2_h_ */
