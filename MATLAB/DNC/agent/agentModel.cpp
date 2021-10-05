/*
 * agentModel.cpp
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "agentModel".
 *
 * Model version              : 3.13
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C++ source code generated on : Tue Jun 22 11:03:49 2021
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "agentModel.h"
#include "agentModel_private.h"

/* Block states (default storage) */
DW_agentModel_T agentModel_DW;

/* External inputs (root inport signals with default storage) */
ExtU_agentModel_T agentModel_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_agentModel_T agentModel_Y;

/* Real-time model */
RT_MODEL_agentModel_T agentModel_M_ = RT_MODEL_agentModel_T();
RT_MODEL_agentModel_T *const agentModel_M = &agentModel_M_;

/* Model step function for TID0 */
void agentModel_step0(void)            /* Sample time: [0.0001s, 0.0s] */
{
  /* Matfile logging */
  rt_UpdateTXYLogVars(agentModel_M->rtwLogInfo, (&agentModel_M->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.0001s, 0.0s] */
    if ((rtmGetTFinal(agentModel_M)!=-1) &&
        !((rtmGetTFinal(agentModel_M)-agentModel_M->Timing.taskTime0) >
          agentModel_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(agentModel_M, "Simulation finished");
    }
  }

  /* Update absolute time */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++agentModel_M->Timing.clockTick0)) {
    ++agentModel_M->Timing.clockTickH0;
  }

  agentModel_M->Timing.taskTime0 = agentModel_M->Timing.clockTick0 *
    agentModel_M->Timing.stepSize0 + agentModel_M->Timing.clockTickH0 *
    agentModel_M->Timing.stepSize0 * 4294967296.0;
}

/* Model step function for TID1 */
void agentModel_step1(void)            /* Sample time: [0.02s, 0.0s] */
{
  real_T tmp;
  real_T tmp_0;

  /* Outport: '<Root>/Output' incorporates:
   *  Delay: '<Root>/Delay'
   *  Gain: '<Root>/C'
   */
  agentModel_Y.Output = agentModel_P.Cd[0] * agentModel_DW.Delay_DSTATE[0] +
    agentModel_P.Cd[1] * agentModel_DW.Delay_DSTATE[1];

  /* Sum: '<Root>/Sum1' incorporates:
   *  Delay: '<Root>/Delay'
   *  Gain: '<Root>/A'
   *  Gain: '<Root>/B'
   *  Inport: '<Root>/Input'
   */
  tmp_0 = (agentModel_P.Ad[0] * agentModel_DW.Delay_DSTATE[0] + agentModel_P.Ad
           [2] * agentModel_DW.Delay_DSTATE[1]) + agentModel_P.Bd[0] *
    agentModel_U.Input;
  tmp = agentModel_P.Ad[1] * agentModel_DW.Delay_DSTATE[0] + agentModel_P.Ad[3] *
    agentModel_DW.Delay_DSTATE[1];

  /* Update for Delay: '<Root>/Delay' incorporates:
   *  Gain: '<Root>/A'
   *  Gain: '<Root>/B'
   *  Inport: '<Root>/Input'
   *  Sum: '<Root>/Sum1'
   */
  agentModel_DW.Delay_DSTATE[0] = tmp_0;
  agentModel_DW.Delay_DSTATE[1] = agentModel_P.Bd[1] * agentModel_U.Input + tmp;
}

/* Model step wrapper function for compatibility with a static main program */
void agentModel_step(int_T tid)
{
  switch (tid) {
   case 0 :
    agentModel_step0();
    break;

   case 1 :
    agentModel_step1();
    break;

   default :
    break;
  }
}

/* Model initialize function */
void agentModel_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));
  (agentModel_M)->Timing.TaskCounters.cLimit[0] = 1;
  (agentModel_M)->Timing.TaskCounters.cLimit[1] = 200;
  rtmSetTFinal(agentModel_M, 130.0);
  agentModel_M->Timing.stepSize0 = 0.0001;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    agentModel_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(agentModel_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(agentModel_M->rtwLogInfo, (NULL));
    rtliSetLogT(agentModel_M->rtwLogInfo, "tout");
    rtliSetLogX(agentModel_M->rtwLogInfo, "");
    rtliSetLogXFinal(agentModel_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(agentModel_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(agentModel_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(agentModel_M->rtwLogInfo, 0);
    rtliSetLogDecimation(agentModel_M->rtwLogInfo, 1);
    rtliSetLogY(agentModel_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(agentModel_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(agentModel_M->rtwLogInfo, (NULL));
  }

  /* states (dwork) */
  (void) memset(static_cast<void *>(&agentModel_DW), 0,
                sizeof(DW_agentModel_T));

  /* external inputs */
  agentModel_U.Input = 0.0;

  /* external outputs */
  agentModel_Y.Output = 0.0;

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(agentModel_M->rtwLogInfo, 0.0, rtmGetTFinal
    (agentModel_M), agentModel_M->Timing.stepSize0, (&rtmGetErrorStatus
    (agentModel_M)));

  /* InitializeConditions for Delay: '<Root>/Delay' */
  agentModel_DW.Delay_DSTATE[0] = agentModel_P.Delay_InitialCondition[0];
  agentModel_DW.Delay_DSTATE[1] = agentModel_P.Delay_InitialCondition[1];
}

/* Model terminate function */
void agentModel_terminate(void)
{
  /* (no terminate code required) */
}
