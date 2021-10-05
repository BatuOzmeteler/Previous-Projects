/*
 * mpc_v2_private.h
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
#ifndef RTW_HEADER_mpc_v2_private_h_
#define RTW_HEADER_mpc_v2_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"

extern time_T rt_UpdateDiscreteEvents(SimStruct *S);
extern void rt_UpdateDiscreteTaskTime(SimStruct *S, int tid);
extern const char *rt_InitTimingEngine(SimStruct *S);
extern time_T rt_GetNextSampleHit();
extern void rt_UpdateDiscreteTaskSampleHits(SimStruct *S);

#if !defined(MULTITASKING) && !defined(NRT)
#  error Model (mpc_v2) was built \
in MultiTasking solver mode, however the MULTITASKING define \
is not present. Please verify that your template makefile is \
configured correctly.
#endif
#endif                                 /* RTW_HEADER_mpc_v2_private_h_ */
