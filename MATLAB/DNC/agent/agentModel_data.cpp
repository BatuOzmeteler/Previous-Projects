/*
 * agentModel_data.cpp
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

/* Block parameters (default storage) */
P_agentModel_T agentModel_P = {
  /* Variable: Ad
   * Referenced by: '<Root>/A'
   */
  { 0.99980000666657776, -0.01999866669333308, 0.01999866669333308,
    0.99980000666657776 },

  /* Variable: Bd
   * Referenced by: '<Root>/B'
   */
  { 0.00019999333342222162, 0.019998666693333084 },

  /* Variable: Cd
   * Referenced by: '<Root>/C'
   */
  { 0.0, 1.0 },

  /* Expression: [1 2]
   * Referenced by: '<Root>/Delay'
   */
  {2.0 , 1.0 }
};
