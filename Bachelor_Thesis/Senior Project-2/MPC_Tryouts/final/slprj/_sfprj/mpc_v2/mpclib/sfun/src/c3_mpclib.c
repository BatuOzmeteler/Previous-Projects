/* Include files */

#include <stddef.h>
#include "blas.h"
#include "mpclib_sfun.h"
#include "c3_mpclib.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "mpclib_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)
#define c3_b_p                         (50.0)
#define c3_b_ny                        (1.0)
#define c3_b_nv                        (1.0)
#define c3_b_nx                        (2.0)
#define c3_b_nu                        (1.0)
#define c3_b_yoff                      (0.0)
#define c3_b_voff                      (0.0)
#define c3_b_no_md                     (1.0)
#define c3_b_no_ref                    (0.0)
#define c3_b_openloopflag              (false)
#define c3_b_Rscale                    (1.0)
#define c3_b_MDscale                   (0.0)
#define c3_b_uoff                      (0.0)
#define c3_b_no_mv                     (1.0)
#define c3_b_CustomEstimation          (0.0)
#define c3_b_myindex                   (1.0)
#define c3_b_myoff                     (0.0)
#define c3_b_Dv                        (0.0)
#define c3_b_no_uref                   (1.0)
#define c3_b_isQP                      (false)
#define c3_b_degrees                   (4.0)
#define c3_b_Mlim                      (0.0)
#define c3_b_Mu1                       (0.0)
#define c3_b_Mv                        (0.0)
#define c3_b_maxiter                   (120.0)
#define c3_b_nxQP                      (2.0)
#define c3_b_lims_inport               (0.0)
#define c3_b_no_umin                   (1.0)
#define c3_b_no_umax                   (1.0)
#define c3_b_no_ymin                   (1.0)
#define c3_b_no_ymax                   (1.0)
#define c3_b_switch_inport             (0.0)
#define c3_b_no_switch                 (1.0)
#define c3_b_enable_value              (0.0)
#define c3_b_return_cost               (0.0)
#define c3_b_return_sequence           (0.0)
#define c3_b_no_ywt                    (1.0)
#define c3_b_no_uwt                    (1.0)
#define c3_b_no_duwt                   (1.0)
#define c3_b_no_rhoeps                 (1.0)
#define c3_b_Wy                        (0.0)
#define c3_b_Wdu                       (0.0)
#define c3_b_Jm                        (0.0)
#define c3_b_SuJm                      (0.0)
#define c3_b_Su1                       (0.0)
#define c3_b_Sx                        (0.0)
#define c3_b_Hv                        (0.0)
#define c3_b_Wu                        (0.0)
#define c3_b_nCC                       (0.0)
#define c3_b_Ecc                       (0.0)
#define c3_b_Fcc                       (0.0)
#define c3_b_Scc                       (0.0)
#define c3_b_Gcc                       (0.0)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c3_debug_family_names[112] = { "isSimulation", "isAdaptive",
  "isDouble", "ZERO", "ONE", "rseq", "vseq", "v", "delmv", "ym_est", "y_innov",
  "utargetValue", "isQP", "nx", "nu", "ny", "degrees", "Hinv", "Kx", "Ku1",
  "Kut", "Kr", "Kv", "Mlim", "Mx", "Mu1", "Mv", "z_degrees", "utarget", "p",
  "uoff", "voff", "yoff", "maxiter", "nxQP", "openloopflag", "lims_inport",
  "no_umin", "no_umax", "no_ymin", "no_ymax", "switch_inport", "no_switch",
  "enable_value", "return_cost", "H", "return_sequence", "Linv", "Ac", "no_ywt",
  "no_uwt", "no_duwt", "no_rhoeps", "Wy", "Wdu", "Jm", "SuJm", "Su1", "Sx", "Hv",
  "Wu", "I1", "A", "Bu", "Bv", "C", "Dv", "Mrows", "nCC", "Ecc", "Fcc", "Scc",
  "Gcc", "nv", "no_md", "no_ref", "no_uref", "no_mv", "Rscale", "MDscale",
  "myindex", "myoff", "xoff", "CustomEstimation", "M", "L", "nargin", "nargout",
  "xk", "old_u", "ym", "ref", "md", "umin", "umax", "ymin", "ymax", "switch_in",
  "ext_mv", "MVtarget", "ywt", "uwt", "duwt", "rhoeps", "iA", "xk1", "u", "cost",
  "useq", "status", "xest", "iAout" };

/* Function Declarations */
static void initialize_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void initialize_params_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void enable_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void disable_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void c3_update_debugger_state_c3_mpclib(SFc3_mpclibInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c3_mpclib(SFc3_mpclibInstanceStruct
  *chartInstance);
static void set_sim_state_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_st);
static void finalize_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void sf_gateway_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void mdl_start_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void c3_chartstep_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void initSimStructsc3_mpclib(SFc3_mpclibInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c3_machineNumber, uint32_T
  c3_chartNumber, uint32_T c3_instanceNumber);
static const mxArray *c3_sf_marshallOut(void *chartInstanceVoid, void *c3_inData);
static void c3_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static const mxArray *c3_b_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static void c3_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance, const
  mxArray *c3_b_xest, const char_T *c3_identifier, real_T c3_y[2]);
static void c3_b_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[2]);
static void c3_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static const mxArray *c3_c_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static void c3_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static const mxArray *c3_d_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static void c3_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static const mxArray *c3_e_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_f_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_g_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_h_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_i_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_j_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_k_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_l_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_m_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static const mxArray *c3_n_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static void c3_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static boolean_T c3_c_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId);
static void c3_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_info_helper(const mxArray **c3_info);
static const mxArray *c3_emlrt_marshallOut(const char * c3_b_u);
static const mxArray *c3_b_emlrt_marshallOut(const uint32_T c3_b_u);
static real_T c3_eml_xdotu(SFc3_mpclibInstanceStruct *chartInstance, real_T
  c3_x[2], real_T c3_y[2]);
static void c3_eml_scalar_eg(SFc3_mpclibInstanceStruct *chartInstance);
static void c3_eml_xgemm(SFc3_mpclibInstanceStruct *chartInstance, real_T
  c3_c_A[4], real_T c3_B[2], real_T c3_c_C[2], real_T c3_d_C[2]);
static void c3_d_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_rseq, const char_T *c3_identifier, real_T c3_y[50]);
static void c3_e_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[50]);
static void c3_f_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_vseq, const char_T *c3_identifier, real_T c3_y[51]);
static void c3_g_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[51]);
static real_T c3_h_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_v, const char_T *c3_identifier);
static real_T c3_i_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId);
static void c3_j_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_iAout, const char_T *c3_identifier, boolean_T c3_y[5]);
static void c3_k_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, boolean_T c3_y[5]);
static const mxArray *c3_o_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData);
static int32_T c3_l_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId);
static void c3_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_m_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[16]);
static void c3_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_n_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[8]);
static void c3_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_o_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[4]);
static void c3_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_p_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[200]);
static void c3_k_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_q_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[204]);
static void c3_l_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_r_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[2]);
static void c3_m_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_s_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[4]);
static void c3_n_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static void c3_t_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[4]);
static void c3_o_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData);
static uint8_T c3_u_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_is_active_c3_mpclib, const char_T *c3_identifier);
static uint8_T c3_v_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId);
static void c3_b_eml_xgemm(SFc3_mpclibInstanceStruct *chartInstance, real_T
  c3_c_A[4], real_T c3_B[2], real_T c3_c_C[2]);
static void init_dsm_address_info(SFc3_mpclibInstanceStruct *chartInstance);
static void init_simulink_io_address(SFc3_mpclibInstanceStruct *chartInstance);

/* Function Definitions */
static void initialize_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  chartInstance->c3_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c3_is_active_c3_mpclib = 0U;
}

static void initialize_params_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  real_T c3_d0;
  real_T c3_d1;
  real_T c3_d2;
  real_T c3_d3;
  real_T c3_d4;
  real_T c3_dv0[16];
  int32_T c3_i0;
  real_T c3_dv1[8];
  int32_T c3_i1;
  real_T c3_dv2[4];
  int32_T c3_i2;
  real_T c3_dv3[200];
  int32_T c3_i3;
  real_T c3_dv4[200];
  int32_T c3_i4;
  real_T c3_dv5[204];
  int32_T c3_i5;
  real_T c3_d5;
  real_T c3_dv6[2];
  int32_T c3_i6;
  real_T c3_d6;
  real_T c3_d7;
  real_T c3_dv7[4];
  int32_T c3_i7;
  real_T c3_dv8[50];
  int32_T c3_i8;
  real_T c3_d8;
  real_T c3_d9;
  real_T c3_d10;
  real_T c3_d11;
  real_T c3_d12;
  real_T c3_d13;
  real_T c3_d14;
  real_T c3_d15;
  real_T c3_d16;
  real_T c3_d17;
  real_T c3_d18;
  real_T c3_d19;
  real_T c3_d20;
  real_T c3_d21;
  real_T c3_d22;
  real_T c3_d23;
  real_T c3_dv9[16];
  int32_T c3_i9;
  real_T c3_d24;
  real_T c3_dv10[16];
  int32_T c3_i10;
  real_T c3_dv11[4];
  int32_T c3_i11;
  real_T c3_d25;
  real_T c3_d26;
  real_T c3_d27;
  real_T c3_d28;
  real_T c3_d29;
  real_T c3_d30;
  real_T c3_d31;
  real_T c3_d32;
  real_T c3_d33;
  real_T c3_d34;
  real_T c3_d35;
  real_T c3_d36;
  real_T c3_dv12[50];
  int32_T c3_i12;
  real_T c3_dv13[4];
  int32_T c3_i13;
  real_T c3_dv14[2];
  int32_T c3_i14;
  real_T c3_dv15[2];
  int32_T c3_i15;
  real_T c3_dv16[2];
  int32_T c3_i16;
  real_T c3_d37;
  real_T c3_dv17[4];
  int32_T c3_i17;
  real_T c3_d38;
  real_T c3_d39;
  real_T c3_d40;
  real_T c3_d41;
  real_T c3_d42;
  real_T c3_d43;
  real_T c3_d44;
  real_T c3_d45;
  real_T c3_d46;
  real_T c3_d47;
  real_T c3_d48;
  real_T c3_d49;
  real_T c3_d50;
  real_T c3_d51;
  real_T c3_dv18[2];
  int32_T c3_i18;
  real_T c3_d52;
  real_T c3_dv19[2];
  int32_T c3_i19;
  real_T c3_dv20[2];
  int32_T c3_i20;
  sf_mex_import_named("isQP", sf_mex_get_sfun_param(chartInstance->S, 39, 0),
                      &c3_d0, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_isQP = (c3_d0 != 0.0);
  sf_mex_import_named("nx", sf_mex_get_sfun_param(chartInstance->S, 60, 0),
                      &c3_d1, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_nx = c3_d1;
  sf_mex_import_named("nu", sf_mex_get_sfun_param(chartInstance->S, 58, 0),
                      &c3_d2, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_nu = c3_d2;
  sf_mex_import_named("ny", sf_mex_get_sfun_param(chartInstance->S, 62, 0),
                      &c3_d3, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_ny = c3_d3;
  sf_mex_import_named("degrees", sf_mex_get_sfun_param(chartInstance->S, 37, 0),
                      &c3_d4, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_degrees = c3_d4;
  sf_mex_import_named("Hinv", sf_mex_get_sfun_param(chartInstance->S, 11, 0),
                      c3_dv0, 0, 0, 0U, 1, 0U, 2, 4, 4);
  for (c3_i0 = 0; c3_i0 < 16; c3_i0++) {
    chartInstance->c3_Hinv[c3_i0] = c3_dv0[c3_i0];
  }

  sf_mex_import_named("Kx", sf_mex_get_sfun_param(chartInstance->S, 19, 0),
                      c3_dv1, 0, 0, 0U, 1, 0U, 2, 2, 4);
  for (c3_i1 = 0; c3_i1 < 8; c3_i1++) {
    chartInstance->c3_Kx[c3_i1] = c3_dv1[c3_i1];
  }

  sf_mex_import_named("Ku1", sf_mex_get_sfun_param(chartInstance->S, 16, 0),
                      c3_dv2, 0, 0, 0U, 1, 0U, 2, 1, 4);
  for (c3_i2 = 0; c3_i2 < 4; c3_i2++) {
    chartInstance->c3_Ku1[c3_i2] = c3_dv2[c3_i2];
  }

  sf_mex_import_named("Kut", sf_mex_get_sfun_param(chartInstance->S, 17, 0),
                      c3_dv3, 0, 0, 0U, 1, 0U, 2, 50, 4);
  for (c3_i3 = 0; c3_i3 < 200; c3_i3++) {
    chartInstance->c3_Kut[c3_i3] = c3_dv3[c3_i3];
  }

  sf_mex_import_named("Kr", sf_mex_get_sfun_param(chartInstance->S, 15, 0),
                      c3_dv4, 0, 0, 0U, 1, 0U, 2, 50, 4);
  for (c3_i4 = 0; c3_i4 < 200; c3_i4++) {
    chartInstance->c3_Kr[c3_i4] = c3_dv4[c3_i4];
  }

  sf_mex_import_named("Kv", sf_mex_get_sfun_param(chartInstance->S, 18, 0),
                      c3_dv5, 0, 0, 0U, 1, 0U, 2, 51, 4);
  for (c3_i5 = 0; c3_i5 < 204; c3_i5++) {
    chartInstance->c3_Kv[c3_i5] = c3_dv5[c3_i5];
  }

  sf_mex_import_named("Mlim", sf_mex_get_sfun_param(chartInstance->S, 24, 0),
                      &c3_d5, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Mlim = c3_d5;
  sf_mex_import_named("Mx", sf_mex_get_sfun_param(chartInstance->S, 28, 0),
                      c3_dv6, 0, 0, 0U, 1, 0U, 2, 1, 2);
  for (c3_i6 = 0; c3_i6 < 2; c3_i6++) {
    chartInstance->c3_Mx[c3_i6] = c3_dv6[c3_i6];
  }

  sf_mex_import_named("Mu1", sf_mex_get_sfun_param(chartInstance->S, 26, 0),
                      &c3_d6, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Mu1 = c3_d6;
  sf_mex_import_named("Mv", sf_mex_get_sfun_param(chartInstance->S, 27, 0),
                      &c3_d7, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Mv = c3_d7;
  sf_mex_import_named("z_degrees", sf_mex_get_sfun_param(chartInstance->S, 73, 0),
                      c3_dv7, 0, 0, 0U, 1, 0U, 1, 4);
  for (c3_i7 = 0; c3_i7 < 4; c3_i7++) {
    chartInstance->c3_z_degrees[c3_i7] = c3_dv7[c3_i7];
  }

  sf_mex_import_named("utarget", sf_mex_get_sfun_param(chartInstance->S, 69, 0),
                      c3_dv8, 0, 0, 0U, 1, 0U, 1, 50);
  for (c3_i8 = 0; c3_i8 < 50; c3_i8++) {
    chartInstance->c3_utarget[c3_i8] = c3_dv8[c3_i8];
  }

  sf_mex_import_named("p", sf_mex_get_sfun_param(chartInstance->S, 64, 0),
                      &c3_d8, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_p = c3_d8;
  sf_mex_import_named("uoff", sf_mex_get_sfun_param(chartInstance->S, 68, 0),
                      &c3_d9, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_uoff = c3_d9;
  sf_mex_import_named("voff", sf_mex_get_sfun_param(chartInstance->S, 70, 0),
                      &c3_d10, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_voff = c3_d10;
  sf_mex_import_named("yoff", sf_mex_get_sfun_param(chartInstance->S, 72, 0),
                      &c3_d11, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_yoff = c3_d11;
  sf_mex_import_named("maxiter", sf_mex_get_sfun_param(chartInstance->S, 41, 0),
                      &c3_d12, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_maxiter = c3_d12;
  sf_mex_import_named("nxQP", sf_mex_get_sfun_param(chartInstance->S, 61, 0),
                      &c3_d13, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_nxQP = c3_d13;
  sf_mex_import_named("openloopflag", sf_mex_get_sfun_param(chartInstance->S, 63,
    0), &c3_d14, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_openloopflag = (c3_d14 != 0.0);
  sf_mex_import_named("lims_inport", sf_mex_get_sfun_param(chartInstance->S, 40,
    0), &c3_d15, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_lims_inport = c3_d15;
  sf_mex_import_named("no_umin", sf_mex_get_sfun_param(chartInstance->S, 52, 0),
                      &c3_d16, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_umin = c3_d16;
  sf_mex_import_named("no_umax", sf_mex_get_sfun_param(chartInstance->S, 51, 0),
                      &c3_d17, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_umax = c3_d17;
  sf_mex_import_named("no_ymin", sf_mex_get_sfun_param(chartInstance->S, 56, 0),
                      &c3_d18, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_ymin = c3_d18;
  sf_mex_import_named("no_ymax", sf_mex_get_sfun_param(chartInstance->S, 55, 0),
                      &c3_d19, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_ymax = c3_d19;
  sf_mex_import_named("switch_inport", sf_mex_get_sfun_param(chartInstance->S,
    67, 0), &c3_d20, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_switch_inport = c3_d20;
  sf_mex_import_named("no_switch", sf_mex_get_sfun_param(chartInstance->S, 50, 0),
                      &c3_d21, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_switch = c3_d21;
  sf_mex_import_named("enable_value", sf_mex_get_sfun_param(chartInstance->S, 38,
    0), &c3_d22, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_enable_value = c3_d22;
  sf_mex_import_named("return_cost", sf_mex_get_sfun_param(chartInstance->S, 65,
    0), &c3_d23, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_return_cost = c3_d23;
  sf_mex_import_named("H", sf_mex_get_sfun_param(chartInstance->S, 10, 0),
                      c3_dv9, 0, 0, 0U, 1, 0U, 2, 4, 4);
  for (c3_i9 = 0; c3_i9 < 16; c3_i9++) {
    chartInstance->c3_H[c3_i9] = c3_dv9[c3_i9];
  }

  sf_mex_import_named("return_sequence", sf_mex_get_sfun_param(chartInstance->S,
    66, 0), &c3_d24, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_return_sequence = c3_d24;
  sf_mex_import_named("Linv", sf_mex_get_sfun_param(chartInstance->S, 21, 0),
                      c3_dv10, 0, 0, 0U, 1, 0U, 2, 4, 4);
  for (c3_i10 = 0; c3_i10 < 16; c3_i10++) {
    chartInstance->c3_Linv[c3_i10] = c3_dv10[c3_i10];
  }

  sf_mex_import_named("Ac", sf_mex_get_sfun_param(chartInstance->S, 1, 0),
                      c3_dv11, 0, 0, 0U, 1, 0U, 2, 1, 4);
  for (c3_i11 = 0; c3_i11 < 4; c3_i11++) {
    chartInstance->c3_Ac[c3_i11] = c3_dv11[c3_i11];
  }

  sf_mex_import_named("no_ywt", sf_mex_get_sfun_param(chartInstance->S, 57, 0),
                      &c3_d25, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_ywt = c3_d25;
  sf_mex_import_named("no_uwt", sf_mex_get_sfun_param(chartInstance->S, 54, 0),
                      &c3_d26, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_uwt = c3_d26;
  sf_mex_import_named("no_duwt", sf_mex_get_sfun_param(chartInstance->S, 45, 0),
                      &c3_d27, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_duwt = c3_d27;
  sf_mex_import_named("no_rhoeps", sf_mex_get_sfun_param(chartInstance->S, 49, 0),
                      &c3_d28, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_rhoeps = c3_d28;
  sf_mex_import_named("Wy", sf_mex_get_sfun_param(chartInstance->S, 36, 0),
                      &c3_d29, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Wy = c3_d29;
  sf_mex_import_named("Wdu", sf_mex_get_sfun_param(chartInstance->S, 34, 0),
                      &c3_d30, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Wdu = c3_d30;
  sf_mex_import_named("Jm", sf_mex_get_sfun_param(chartInstance->S, 14, 0),
                      &c3_d31, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Jm = c3_d31;
  sf_mex_import_named("SuJm", sf_mex_get_sfun_param(chartInstance->S, 32, 0),
                      &c3_d32, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_SuJm = c3_d32;
  sf_mex_import_named("Su1", sf_mex_get_sfun_param(chartInstance->S, 31, 0),
                      &c3_d33, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Su1 = c3_d33;
  sf_mex_import_named("Sx", sf_mex_get_sfun_param(chartInstance->S, 33, 0),
                      &c3_d34, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Sx = c3_d34;
  sf_mex_import_named("Hv", sf_mex_get_sfun_param(chartInstance->S, 12, 0),
                      &c3_d35, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Hv = c3_d35;
  sf_mex_import_named("Wu", sf_mex_get_sfun_param(chartInstance->S, 35, 0),
                      &c3_d36, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Wu = c3_d36;
  sf_mex_import_named("I1", sf_mex_get_sfun_param(chartInstance->S, 13, 0),
                      c3_dv12, 0, 0, 0U, 1, 0U, 1, 50);
  for (c3_i12 = 0; c3_i12 < 50; c3_i12++) {
    chartInstance->c3_I1[c3_i12] = c3_dv12[c3_i12];
  }

  sf_mex_import_named("A", sf_mex_get_sfun_param(chartInstance->S, 0, 0),
                      c3_dv13, 0, 0, 0U, 1, 0U, 2, 2, 2);
  for (c3_i13 = 0; c3_i13 < 4; c3_i13++) {
    chartInstance->c3_A[c3_i13] = c3_dv13[c3_i13];
  }

  sf_mex_import_named("Bu", sf_mex_get_sfun_param(chartInstance->S, 2, 0),
                      c3_dv14, 0, 0, 0U, 1, 0U, 1, 2);
  for (c3_i14 = 0; c3_i14 < 2; c3_i14++) {
    chartInstance->c3_Bu[c3_i14] = c3_dv14[c3_i14];
  }

  sf_mex_import_named("Bv", sf_mex_get_sfun_param(chartInstance->S, 3, 0),
                      c3_dv15, 0, 0, 0U, 1, 0U, 1, 2);
  for (c3_i15 = 0; c3_i15 < 2; c3_i15++) {
    chartInstance->c3_Bv[c3_i15] = c3_dv15[c3_i15];
  }

  sf_mex_import_named("C", sf_mex_get_sfun_param(chartInstance->S, 4, 0),
                      c3_dv16, 0, 0, 0U, 1, 0U, 2, 1, 2);
  for (c3_i16 = 0; c3_i16 < 2; c3_i16++) {
    chartInstance->c3_C[c3_i16] = c3_dv16[c3_i16];
  }

  sf_mex_import_named("Dv", sf_mex_get_sfun_param(chartInstance->S, 6, 0),
                      &c3_d37, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Dv = c3_d37;
  sf_mex_import_named("Mrows", sf_mex_get_sfun_param(chartInstance->S, 25, 0),
                      c3_dv17, 0, 0, 0U, 1, 0U, 1, 4);
  for (c3_i17 = 0; c3_i17 < 4; c3_i17++) {
    chartInstance->c3_Mrows[c3_i17] = c3_dv17[c3_i17];
  }

  sf_mex_import_named("nCC", sf_mex_get_sfun_param(chartInstance->S, 44, 0),
                      &c3_d38, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_nCC = c3_d38;
  sf_mex_import_named("Ecc", sf_mex_get_sfun_param(chartInstance->S, 7, 0),
                      &c3_d39, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Ecc = c3_d39;
  sf_mex_import_named("Fcc", sf_mex_get_sfun_param(chartInstance->S, 8, 0),
                      &c3_d40, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Fcc = c3_d40;
  sf_mex_import_named("Scc", sf_mex_get_sfun_param(chartInstance->S, 30, 0),
                      &c3_d41, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Scc = c3_d41;
  sf_mex_import_named("Gcc", sf_mex_get_sfun_param(chartInstance->S, 9, 0),
                      &c3_d42, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Gcc = c3_d42;
  sf_mex_import_named("nv", sf_mex_get_sfun_param(chartInstance->S, 59, 0),
                      &c3_d43, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_nv = c3_d43;
  sf_mex_import_named("no_md", sf_mex_get_sfun_param(chartInstance->S, 46, 0),
                      &c3_d44, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_md = c3_d44;
  sf_mex_import_named("no_ref", sf_mex_get_sfun_param(chartInstance->S, 48, 0),
                      &c3_d45, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_ref = c3_d45;
  sf_mex_import_named("no_uref", sf_mex_get_sfun_param(chartInstance->S, 53, 0),
                      &c3_d46, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_uref = c3_d46;
  sf_mex_import_named("no_mv", sf_mex_get_sfun_param(chartInstance->S, 47, 0),
                      &c3_d47, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_no_mv = c3_d47;
  sf_mex_import_named("Rscale", sf_mex_get_sfun_param(chartInstance->S, 29, 0),
                      &c3_d48, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_Rscale = c3_d48;
  sf_mex_import_named("MDscale", sf_mex_get_sfun_param(chartInstance->S, 23, 0),
                      &c3_d49, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_MDscale = c3_d49;
  sf_mex_import_named("myindex", sf_mex_get_sfun_param(chartInstance->S, 42, 0),
                      &c3_d50, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_myindex = c3_d50;
  sf_mex_import_named("myoff", sf_mex_get_sfun_param(chartInstance->S, 43, 0),
                      &c3_d51, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_myoff = c3_d51;
  sf_mex_import_named("xoff", sf_mex_get_sfun_param(chartInstance->S, 71, 0),
                      c3_dv18, 0, 0, 0U, 1, 0U, 1, 2);
  for (c3_i18 = 0; c3_i18 < 2; c3_i18++) {
    chartInstance->c3_xoff[c3_i18] = c3_dv18[c3_i18];
  }

  sf_mex_import_named("CustomEstimation", sf_mex_get_sfun_param(chartInstance->S,
    5, 0), &c3_d52, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c3_CustomEstimation = c3_d52;
  sf_mex_import_named("M", sf_mex_get_sfun_param(chartInstance->S, 22, 0),
                      c3_dv19, 0, 0, 0U, 1, 0U, 1, 2);
  for (c3_i19 = 0; c3_i19 < 2; c3_i19++) {
    chartInstance->c3_M[c3_i19] = c3_dv19[c3_i19];
  }

  sf_mex_import_named("L", sf_mex_get_sfun_param(chartInstance->S, 20, 0),
                      c3_dv20, 0, 0, 0U, 1, 0U, 1, 2);
  for (c3_i20 = 0; c3_i20 < 2; c3_i20++) {
    chartInstance->c3_L[c3_i20] = c3_dv20[c3_i20];
  }
}

static void enable_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c3_update_debugger_state_c3_mpclib(SFc3_mpclibInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c3_mpclib(SFc3_mpclibInstanceStruct
  *chartInstance)
{
  const mxArray *c3_st;
  const mxArray *c3_y = NULL;
  real_T c3_hoistedGlobal;
  real_T c3_b_u;
  const mxArray *c3_b_y = NULL;
  int32_T c3_i21;
  boolean_T c3_c_u[5];
  const mxArray *c3_c_y = NULL;
  real_T c3_b_hoistedGlobal;
  real_T c3_d_u;
  const mxArray *c3_d_y = NULL;
  real_T c3_c_hoistedGlobal;
  real_T c3_e_u;
  const mxArray *c3_e_y = NULL;
  int32_T c3_i22;
  real_T c3_f_u[50];
  const mxArray *c3_f_y = NULL;
  int32_T c3_i23;
  real_T c3_g_u[2];
  const mxArray *c3_g_y = NULL;
  int32_T c3_i24;
  real_T c3_h_u[2];
  const mxArray *c3_h_y = NULL;
  uint8_T c3_d_hoistedGlobal;
  uint8_T c3_i_u;
  const mxArray *c3_i_y = NULL;
  c3_st = NULL;
  c3_st = NULL;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_createcellmatrix(8, 1), false);
  c3_hoistedGlobal = *chartInstance->c3_cost;
  c3_b_u = c3_hoistedGlobal;
  c3_b_y = NULL;
  sf_mex_assign(&c3_b_y, sf_mex_create("y", &c3_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c3_y, 0, c3_b_y);
  for (c3_i21 = 0; c3_i21 < 5; c3_i21++) {
    c3_c_u[c3_i21] = (*chartInstance->c3_iAout)[c3_i21];
  }

  c3_c_y = NULL;
  sf_mex_assign(&c3_c_y, sf_mex_create("y", c3_c_u, 11, 0U, 1U, 0U, 1, 5), false);
  sf_mex_setcell(c3_y, 1, c3_c_y);
  c3_b_hoistedGlobal = *chartInstance->c3_status;
  c3_d_u = c3_b_hoistedGlobal;
  c3_d_y = NULL;
  sf_mex_assign(&c3_d_y, sf_mex_create("y", &c3_d_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c3_y, 2, c3_d_y);
  c3_c_hoistedGlobal = *chartInstance->c3_u;
  c3_e_u = c3_c_hoistedGlobal;
  c3_e_y = NULL;
  sf_mex_assign(&c3_e_y, sf_mex_create("y", &c3_e_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c3_y, 3, c3_e_y);
  for (c3_i22 = 0; c3_i22 < 50; c3_i22++) {
    c3_f_u[c3_i22] = (*chartInstance->c3_useq)[c3_i22];
  }

  c3_f_y = NULL;
  sf_mex_assign(&c3_f_y, sf_mex_create("y", c3_f_u, 0, 0U, 1U, 0U, 1, 50), false);
  sf_mex_setcell(c3_y, 4, c3_f_y);
  for (c3_i23 = 0; c3_i23 < 2; c3_i23++) {
    c3_g_u[c3_i23] = (*chartInstance->c3_xest)[c3_i23];
  }

  c3_g_y = NULL;
  sf_mex_assign(&c3_g_y, sf_mex_create("y", c3_g_u, 0, 0U, 1U, 0U, 1, 2), false);
  sf_mex_setcell(c3_y, 5, c3_g_y);
  for (c3_i24 = 0; c3_i24 < 2; c3_i24++) {
    c3_h_u[c3_i24] = (*chartInstance->c3_xk1)[c3_i24];
  }

  c3_h_y = NULL;
  sf_mex_assign(&c3_h_y, sf_mex_create("y", c3_h_u, 0, 0U, 1U, 0U, 1, 2), false);
  sf_mex_setcell(c3_y, 6, c3_h_y);
  c3_d_hoistedGlobal = chartInstance->c3_is_active_c3_mpclib;
  c3_i_u = c3_d_hoistedGlobal;
  c3_i_y = NULL;
  sf_mex_assign(&c3_i_y, sf_mex_create("y", &c3_i_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c3_y, 7, c3_i_y);
  sf_mex_assign(&c3_st, c3_y, false);
  return c3_st;
}

static void set_sim_state_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_st)
{
  const mxArray *c3_b_u;
  boolean_T c3_bv0[5];
  int32_T c3_i25;
  real_T c3_dv21[50];
  int32_T c3_i26;
  real_T c3_dv22[2];
  int32_T c3_i27;
  real_T c3_dv23[2];
  int32_T c3_i28;
  chartInstance->c3_doneDoubleBufferReInit = true;
  c3_b_u = sf_mex_dup(c3_st);
  *chartInstance->c3_cost = c3_h_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c3_b_u, 0)), "cost");
  c3_j_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c3_b_u, 1)),
                        "iAout", c3_bv0);
  for (c3_i25 = 0; c3_i25 < 5; c3_i25++) {
    (*chartInstance->c3_iAout)[c3_i25] = c3_bv0[c3_i25];
  }

  *chartInstance->c3_status = c3_h_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c3_b_u, 2)), "status");
  *chartInstance->c3_u = c3_h_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c3_b_u, 3)), "u");
  c3_d_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c3_b_u, 4)),
                        "useq", c3_dv21);
  for (c3_i26 = 0; c3_i26 < 50; c3_i26++) {
    (*chartInstance->c3_useq)[c3_i26] = c3_dv21[c3_i26];
  }

  c3_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c3_b_u, 5)),
                      "xest", c3_dv22);
  for (c3_i27 = 0; c3_i27 < 2; c3_i27++) {
    (*chartInstance->c3_xest)[c3_i27] = c3_dv22[c3_i27];
  }

  c3_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c3_b_u, 6)),
                      "xk1", c3_dv23);
  for (c3_i28 = 0; c3_i28 < 2; c3_i28++) {
    (*chartInstance->c3_xk1)[c3_i28] = c3_dv23[c3_i28];
  }

  chartInstance->c3_is_active_c3_mpclib = c3_u_emlrt_marshallIn(chartInstance,
    sf_mex_dup(sf_mex_getcell(c3_b_u, 7)), "is_active_c3_mpclib");
  sf_mex_destroy(&c3_b_u);
  c3_update_debugger_state_c3_mpclib(chartInstance);
  sf_mex_destroy(&c3_st);
}

static void finalize_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  int32_T c3_i29;
  int32_T c3_i30;
  int32_T c3_i31;
  int32_T c3_i32;
  int32_T c3_i33;
  int32_T c3_i34;
  int32_T c3_i35;
  int32_T c3_i36;
  int32_T c3_i37;
  int32_T c3_i38;
  int32_T c3_i39;
  int32_T c3_i40;
  int32_T c3_i41;
  int32_T c3_i42;
  int32_T c3_i43;
  int32_T c3_i44;
  int32_T c3_i45;
  int32_T c3_i46;
  int32_T c3_i47;
  int32_T c3_i48;
  int32_T c3_i49;
  int32_T c3_i50;
  int32_T c3_i51;
  int32_T c3_i52;
  int32_T c3_i53;
  int32_T c3_i54;
  int32_T c3_i55;
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c3_sfEvent);
  chartInstance->c3_sfEvent = CALL_EVENT;
  c3_chartstep_c3_mpclib(chartInstance);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_mpclibMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  for (c3_i29 = 0; c3_i29 < 2; c3_i29++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c3_xk1)[c3_i29], 0U);
  }

  for (c3_i30 = 0; c3_i30 < 2; c3_i30++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c3_xk)[c3_i30], 1U);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_old_u, 2U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_ym, 3U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_ref, 4U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_md, 5U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_umin, 6U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_umax, 7U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_ymin, 8U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_ymax, 9U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_switch_in, 10U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_ext_mv, 11U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_MVtarget, 12U);
  _SFD_DATA_RANGE_CHECK((real_T)chartInstance->c3_isQP, 13U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_nx, 14U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_nu, 15U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_ny, 16U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_degrees, 17U);
  for (c3_i31 = 0; c3_i31 < 16; c3_i31++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Hinv[c3_i31], 18U);
  }

  for (c3_i32 = 0; c3_i32 < 8; c3_i32++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Kx[c3_i32], 19U);
  }

  for (c3_i33 = 0; c3_i33 < 4; c3_i33++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Ku1[c3_i33], 20U);
  }

  for (c3_i34 = 0; c3_i34 < 200; c3_i34++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Kut[c3_i34], 21U);
  }

  for (c3_i35 = 0; c3_i35 < 200; c3_i35++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Kr[c3_i35], 22U);
  }

  for (c3_i36 = 0; c3_i36 < 204; c3_i36++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Kv[c3_i36], 23U);
  }

  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Mlim, 24U);
  for (c3_i37 = 0; c3_i37 < 2; c3_i37++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Mx[c3_i37], 25U);
  }

  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Mu1, 26U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Mv, 27U);
  for (c3_i38 = 0; c3_i38 < 4; c3_i38++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_z_degrees[c3_i38], 28U);
  }

  for (c3_i39 = 0; c3_i39 < 50; c3_i39++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_utarget[c3_i39], 29U);
  }

  _SFD_DATA_RANGE_CHECK(chartInstance->c3_p, 30U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_uoff, 31U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_voff, 32U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_yoff, 33U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_maxiter, 34U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_nxQP, 35U);
  _SFD_DATA_RANGE_CHECK((real_T)chartInstance->c3_openloopflag, 36U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_lims_inport, 37U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_umin, 38U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_umax, 39U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_ymin, 40U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_ymax, 41U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_switch_inport, 42U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_switch, 43U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_enable_value, 44U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_return_cost, 45U);
  for (c3_i40 = 0; c3_i40 < 16; c3_i40++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_H[c3_i40], 46U);
  }

  _SFD_DATA_RANGE_CHECK(chartInstance->c3_return_sequence, 47U);
  for (c3_i41 = 0; c3_i41 < 16; c3_i41++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Linv[c3_i41], 48U);
  }

  for (c3_i42 = 0; c3_i42 < 4; c3_i42++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Ac[c3_i42], 49U);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_ywt, 50U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_uwt, 51U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_duwt, 52U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_rhoeps, 53U);
  for (c3_i43 = 0; c3_i43 < 5; c3_i43++) {
    _SFD_DATA_RANGE_CHECK((real_T)(*chartInstance->c3_iA)[c3_i43], 54U);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_u, 55U);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_cost, 56U);
  for (c3_i44 = 0; c3_i44 < 50; c3_i44++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c3_useq)[c3_i44], 57U);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c3_status, 58U);
  for (c3_i45 = 0; c3_i45 < 2; c3_i45++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c3_xest)[c3_i45], 59U);
  }

  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_ywt, 60U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_uwt, 61U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_duwt, 62U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_rhoeps, 63U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Wy, 64U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Wdu, 65U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Jm, 66U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_SuJm, 67U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Su1, 68U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Sx, 69U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Hv, 70U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Wu, 71U);
  for (c3_i46 = 0; c3_i46 < 50; c3_i46++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_I1[c3_i46], 72U);
  }

  for (c3_i47 = 0; c3_i47 < 5; c3_i47++) {
    _SFD_DATA_RANGE_CHECK((real_T)(*chartInstance->c3_iAout)[c3_i47], 73U);
  }

  for (c3_i48 = 0; c3_i48 < 4; c3_i48++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_A[c3_i48], 74U);
  }

  for (c3_i49 = 0; c3_i49 < 2; c3_i49++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Bu[c3_i49], 75U);
  }

  for (c3_i50 = 0; c3_i50 < 2; c3_i50++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Bv[c3_i50], 76U);
  }

  for (c3_i51 = 0; c3_i51 < 2; c3_i51++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_C[c3_i51], 77U);
  }

  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Dv, 78U);
  for (c3_i52 = 0; c3_i52 < 4; c3_i52++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_Mrows[c3_i52], 79U);
  }

  _SFD_DATA_RANGE_CHECK(chartInstance->c3_nCC, 80U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Ecc, 81U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Fcc, 82U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Scc, 83U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Gcc, 84U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_nv, 85U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_md, 86U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_ref, 87U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_uref, 88U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_no_mv, 89U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_Rscale, 90U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_MDscale, 91U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_myindex, 92U);
  _SFD_DATA_RANGE_CHECK(chartInstance->c3_myoff, 93U);
  for (c3_i53 = 0; c3_i53 < 2; c3_i53++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_xoff[c3_i53], 94U);
  }

  _SFD_DATA_RANGE_CHECK(chartInstance->c3_CustomEstimation, 95U);
  for (c3_i54 = 0; c3_i54 < 2; c3_i54++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_M[c3_i54], 96U);
  }

  for (c3_i55 = 0; c3_i55 < 2; c3_i55++) {
    _SFD_DATA_RANGE_CHECK(chartInstance->c3_L[c3_i55], 97U);
  }
}

static void mdl_start_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void c3_chartstep_c3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  real_T c3_hoistedGlobal;
  real_T c3_b_hoistedGlobal;
  real_T c3_c_hoistedGlobal;
  real_T c3_d_hoistedGlobal;
  real_T c3_e_hoistedGlobal;
  real_T c3_f_hoistedGlobal;
  real_T c3_g_hoistedGlobal;
  real_T c3_h_hoistedGlobal;
  real_T c3_i_hoistedGlobal;
  real_T c3_j_hoistedGlobal;
  real_T c3_k_hoistedGlobal;
  real_T c3_l_hoistedGlobal;
  real_T c3_m_hoistedGlobal;
  real_T c3_n_hoistedGlobal;
  real_T c3_o_hoistedGlobal;
  int32_T c3_i56;
  real_T c3_b_xk[2];
  real_T c3_b_old_u;
  real_T c3_b_ym;
  real_T c3_b_ref;
  real_T c3_b_md;
  real_T c3_b_umin;
  real_T c3_b_umax;
  real_T c3_b_ymin;
  real_T c3_b_ymax;
  real_T c3_b_switch_in;
  real_T c3_b_ext_mv;
  real_T c3_b_MVtarget;
  real_T c3_b_ywt;
  real_T c3_b_uwt;
  real_T c3_b_duwt;
  real_T c3_b_rhoeps;
  int32_T c3_i57;
  boolean_T c3_b_iA[5];
  uint32_T c3_debug_family_var_map[112];
  boolean_T c3_isSimulation;
  boolean_T c3_isAdaptive;
  boolean_T c3_isDouble;
  real_T c3_ZERO;
  real_T c3_ONE;
  real_T c3_rseq[50];
  real_T c3_vseq[51];
  real_T c3_v;
  real_T c3_delmv;
  real_T c3_ym_est;
  real_T c3_y_innov;
  real_T c3_utargetValue[50];
  boolean_T c3_c_isQP;
  real_T c3_c_nx;
  real_T c3_c_nu;
  real_T c3_c_ny;
  real_T c3_c_degrees;
  real_T c3_c_Hinv[16];
  real_T c3_c_Kx[8];
  real_T c3_c_Ku1[4];
  real_T c3_c_Kut[200];
  real_T c3_c_Kr[200];
  real_T c3_c_Kv[204];
  real_T c3_c_Mlim;
  real_T c3_c_Mx[2];
  real_T c3_c_Mu1;
  real_T c3_c_Mv;
  real_T c3_c_z_degrees[4];
  real_T c3_c_utarget[50];
  real_T c3_c_p;
  real_T c3_c_uoff;
  real_T c3_c_voff;
  real_T c3_c_yoff;
  real_T c3_c_maxiter;
  real_T c3_c_nxQP;
  boolean_T c3_c_openloopflag;
  real_T c3_c_lims_inport;
  real_T c3_c_no_umin;
  real_T c3_c_no_umax;
  real_T c3_c_no_ymin;
  real_T c3_c_no_ymax;
  real_T c3_c_switch_inport;
  real_T c3_c_no_switch;
  real_T c3_c_enable_value;
  real_T c3_c_return_cost;
  real_T c3_c_H[16];
  real_T c3_c_return_sequence;
  real_T c3_c_Linv[16];
  real_T c3_c_Ac[4];
  real_T c3_c_no_ywt;
  real_T c3_c_no_uwt;
  real_T c3_c_no_duwt;
  real_T c3_c_no_rhoeps;
  real_T c3_c_Wy;
  real_T c3_c_Wdu;
  real_T c3_c_Jm;
  real_T c3_c_SuJm;
  real_T c3_c_Su1;
  real_T c3_c_Sx;
  real_T c3_c_Hv;
  real_T c3_c_Wu;
  real_T c3_c_I1[50];
  real_T c3_c_A[4];
  real_T c3_b_Bu[2];
  real_T c3_b_Bv[2];
  real_T c3_c_C[2];
  real_T c3_c_Dv;
  real_T c3_b_Mrows[4];
  real_T c3_c_nCC;
  real_T c3_c_Ecc;
  real_T c3_c_Fcc;
  real_T c3_c_Scc;
  real_T c3_c_Gcc;
  real_T c3_c_nv;
  real_T c3_c_no_md;
  real_T c3_c_no_ref;
  real_T c3_c_no_uref;
  real_T c3_c_no_mv;
  real_T c3_c_Rscale;
  real_T c3_c_MDscale;
  real_T c3_c_myindex;
  real_T c3_c_myoff;
  real_T c3_b_xoff[2];
  real_T c3_c_CustomEstimation;
  real_T c3_b_M[2];
  real_T c3_b_L[2];
  real_T c3_nargin = 91.0;
  real_T c3_nargout = 7.0;
  real_T c3_b_xk1[2];
  real_T c3_b_u;
  real_T c3_b_cost;
  real_T c3_b_useq[50];
  real_T c3_b_status;
  real_T c3_b_xest[2];
  boolean_T c3_b_iAout[5];
  int32_T c3_i58;
  int32_T c3_i59;
  int32_T c3_i60;
  int32_T c3_i61;
  int32_T c3_i62;
  int32_T c3_i63;
  int32_T c3_i64;
  int32_T c3_i65;
  static real_T c3_d_A[4] = { 0.99999999964049646, 0.0, 0.0, 1.0 };

  int32_T c3_i66;
  int32_T c3_i67;
  int32_T c3_i68;
  static real_T c3_d_Linv[16] = { 0.16590488540551146, -9.41491001507046,
    20.805136803983817, 8.8043658990272267, 0.0, 9.70317046062206,
    -44.485582448082873, 5.0909420339612028, 0.0, 0.0, 23.770544618390307,
    -39.308982041433985, 0.0, 0.0, 0.0, 25.500521986274332 };

  int32_T c3_i69;
  static real_T c3_d_H[16] = { 36.331359581913212, 35.252032578117962,
    34.173735115495504, 33.097130427181519, 35.252032578117969,
    34.215391249398053, 33.178383611880363, 32.142405515550678,
    34.173735115495504, 33.178383611880371, 32.183215262119916,
    31.187680603576606, 33.097130427181526, 32.142405515550678,
    31.187680603576609, 30.233138845471828 };

  int32_T c3_i70;
  int32_T c3_i71;
  int32_T c3_i72;
  int32_T c3_i73;
  int32_T c3_i74;
  static real_T c3_d_Kr[200] = { -0.21496914319078766, -0.42993828630429315,
    -0.64490742934051648, -0.85987657229945758, -1.0748457151811166,
    -1.2898148579854936, -1.5047840007125883, -1.7197531433624009,
    -1.9347222859349311, -2.1496914284301796, -2.3646605708481454,
    -2.5796297131888291, -2.794598855452231, -3.0095679976383503,
    -3.2245371397471874, -3.4395062817787423, -3.6544754237330159,
    -3.8694445656100065, -4.0844137074097153, -4.299382849132142,
    -4.5143519907772856, -4.7293211323451478, -4.9442902738357279,
    -5.159259415249025, -5.3742285565850407, -5.5891976978437743,
    -5.8041668390252266, -6.0191359801293949, -6.2341051211562819,
    -6.4490742621058876, -6.66404340297821, -6.8790125437732508,
    -7.0939816844910082, -7.3089508251314843, -7.5239199656946782,
    -7.73888910618059, -7.9538582465892205, -8.1688273869205688,
    -8.3837965271746331, -8.5987656673514152, -8.813734807450917,
    -9.0287039474731365, -9.2436730874180721, -9.4586422272857273,
    -9.6736113670761, -9.88858050678919, -10.103549646424998,
    -10.318518785983523, -10.533487925464767, -10.748457064868729, -0.0,
    -0.21496914319078766, -0.42993828630429315, -0.64490742934051648,
    -0.85987657229945758, -1.0748457151811166, -1.2898148579854936,
    -1.5047840007125883, -1.7197531433624009, -1.9347222859349311,
    -2.1496914284301796, -2.3646605708481454, -2.5796297131888291,
    -2.794598855452231, -3.0095679976383503, -3.2245371397471874,
    -3.4395062817787423, -3.6544754237330159, -3.8694445656100065,
    -4.0844137074097153, -4.299382849132142, -4.5143519907772856,
    -4.7293211323451478, -4.9442902738357279, -5.159259415249025,
    -5.3742285565850407, -5.5891976978437743, -5.8041668390252266,
    -6.0191359801293949, -6.2341051211562819, -6.4490742621058876,
    -6.66404340297821, -6.8790125437732508, -7.0939816844910082,
    -7.3089508251314843, -7.5239199656946782, -7.73888910618059,
    -7.9538582465892205, -8.1688273869205688, -8.3837965271746331,
    -8.5987656673514152, -8.813734807450917, -9.0287039474731365,
    -9.2436730874180721, -9.4586422272857273, -9.6736113670761,
    -9.88858050678919, -10.103549646424998, -10.318518785983523,
    -10.533487925464767, -0.0, -0.0, -0.21496914319078766, -0.42993828630429315,
    -0.64490742934051648, -0.85987657229945758, -1.0748457151811166,
    -1.2898148579854936, -1.5047840007125883, -1.7197531433624009,
    -1.9347222859349311, -2.1496914284301796, -2.3646605708481454,
    -2.5796297131888291, -2.794598855452231, -3.0095679976383503,
    -3.2245371397471874, -3.4395062817787423, -3.6544754237330159,
    -3.8694445656100065, -4.0844137074097153, -4.299382849132142,
    -4.5143519907772856, -4.7293211323451478, -4.9442902738357279,
    -5.159259415249025, -5.3742285565850407, -5.5891976978437743,
    -5.8041668390252266, -6.0191359801293949, -6.2341051211562819,
    -6.4490742621058876, -6.66404340297821, -6.8790125437732508,
    -7.0939816844910082, -7.3089508251314843, -7.5239199656946782,
    -7.73888910618059, -7.9538582465892205, -8.1688273869205688,
    -8.3837965271746331, -8.5987656673514152, -8.813734807450917,
    -9.0287039474731365, -9.2436730874180721, -9.4586422272857273,
    -9.6736113670761, -9.88858050678919, -10.103549646424998,
    -10.318518785983523, -0.0, -0.0, -0.0, -0.21496914319078766,
    -0.42993828630429315, -0.64490742934051648, -0.85987657229945758,
    -1.0748457151811166, -1.2898148579854936, -1.5047840007125883,
    -1.7197531433624009, -1.9347222859349311, -2.1496914284301796,
    -2.3646605708481454, -2.5796297131888291, -2.794598855452231,
    -3.0095679976383503, -3.2245371397471874, -3.4395062817787423,
    -3.6544754237330159, -3.8694445656100065, -4.0844137074097153,
    -4.299382849132142, -4.5143519907772856, -4.7293211323451478,
    -4.9442902738357279, -5.159259415249025, -5.3742285565850407,
    -5.5891976978437743, -5.8041668390252266, -6.0191359801293949,
    -6.2341051211562819, -6.4490742621058876, -6.66404340297821,
    -6.8790125437732508, -7.0939816844910082, -7.3089508251314843,
    -7.5239199656946782, -7.73888910618059, -7.9538582465892205,
    -8.1688273869205688, -8.3837965271746331, -8.5987656673514152,
    -8.813734807450917, -9.0287039474731365, -9.2436730874180721,
    -9.4586422272857273, -9.6736113670761, -9.88858050678919,
    -10.103549646424998 };

  int32_T c3_i75;
  int32_T c3_i76;
  static real_T c3_d_Ku1[4] = { 36.331176427700711, 35.252032578117976,
    34.173735115495504, 33.097130427181511 };

  int32_T c3_i77;
  static real_T c3_d_Kx[8] = { 986.70834950945789, 274.08565595885312,
    948.01390443065566, 263.33719889398441, 910.09334825342933,
    252.80371096851962, 872.94668097777924, 242.48519218253611 };

  int32_T c3_i78;
  static real_T c3_d_Hinv[16] = { 599.038631339311, -972.06058894717955,
    148.45877167973663, 224.51592618334777, -972.06058894717955,
    2099.03625352622, -1257.5662714441969, 129.82167926787582,
    148.45877167973663, -1257.5662714441969, 2110.2348605886637,
    -1002.3995608056503, 224.51592618334777, 129.82167926787582,
    -1002.3995608056503, 650.27662157246061 };

  int32_T c3_i79;
  int32_T c3_i80;
  int32_T c3_i81;
  int32_T c3_i82;
  int32_T c3_i83;
  int32_T c3_i84;
  real_T c3_c_u;
  const mxArray *c3_y = NULL;
  real_T c3_d_u;
  const mxArray *c3_b_y = NULL;
  real_T c3_e_u;
  const mxArray *c3_c_y = NULL;
  real_T c3_f_u;
  const mxArray *c3_d_y = NULL;
  real_T c3_g_u;
  const mxArray *c3_e_y = NULL;
  real_T c3_h_u;
  const mxArray *c3_f_y = NULL;
  real_T c3_i_u;
  const mxArray *c3_g_y = NULL;
  real_T c3_j_u;
  const mxArray *c3_h_y = NULL;
  real_T c3_k_u;
  const mxArray *c3_i_y = NULL;
  boolean_T c3_l_u;
  const mxArray *c3_j_y = NULL;
  real_T c3_m_u;
  const mxArray *c3_k_y = NULL;
  real_T c3_n_u;
  const mxArray *c3_l_y = NULL;
  const mxArray *c3_b_v = NULL;
  const mxArray *c3_b_vseq = NULL;
  const mxArray *c3_b_rseq = NULL;
  real_T c3_dv24[50];
  int32_T c3_i85;
  real_T c3_dv25[51];
  int32_T c3_i86;
  int32_T c3_i87;
  int32_T c3_i88;
  int32_T c3_i89;
  real_T c3_b[2];
  int32_T c3_i90;
  real_T c3_dv26[2];
  int32_T c3_i91;
  real_T c3_b_b[2];
  real_T c3_m_y;
  real_T c3_c_b;
  int32_T c3_i92;
  real_T c3_n_y[2];
  int32_T c3_i93;
  int32_T c3_i94;
  int32_T c3_i95;
  real_T c3_o_u[50];
  const mxArray *c3_o_y = NULL;
  int32_T c3_i96;
  real_T c3_p_u[51];
  const mxArray *c3_p_y = NULL;
  real_T c3_q_u;
  const mxArray *c3_q_y = NULL;
  real_T c3_r_u;
  const mxArray *c3_r_y = NULL;
  real_T c3_s_u;
  const mxArray *c3_s_y = NULL;
  real_T c3_t_u;
  const mxArray *c3_t_y = NULL;
  real_T c3_u_u;
  const mxArray *c3_u_y = NULL;
  int32_T c3_i97;
  real_T c3_v_u[2];
  const mxArray *c3_v_y = NULL;
  real_T c3_w_u;
  const mxArray *c3_w_y = NULL;
  int32_T c3_i98;
  boolean_T c3_x_u[5];
  const mxArray *c3_x_y = NULL;
  boolean_T c3_y_u;
  const mxArray *c3_y_y = NULL;
  real_T c3_ab_u;
  const mxArray *c3_ab_y = NULL;
  real_T c3_bb_u;
  const mxArray *c3_bb_y = NULL;
  real_T c3_cb_u;
  const mxArray *c3_cb_y = NULL;
  int32_T c3_i99;
  real_T c3_db_u[16];
  const mxArray *c3_db_y = NULL;
  int32_T c3_i100;
  real_T c3_eb_u[8];
  const mxArray *c3_eb_y = NULL;
  int32_T c3_i101;
  real_T c3_fb_u[4];
  const mxArray *c3_fb_y = NULL;
  int32_T c3_i102;
  real_T c3_gb_u[200];
  const mxArray *c3_gb_y = NULL;
  int32_T c3_i103;
  real_T c3_hb_u[200];
  const mxArray *c3_hb_y = NULL;
  int32_T c3_i104;
  real_T c3_ib_u[204];
  const mxArray *c3_ib_y = NULL;
  real_T c3_jb_u;
  const mxArray *c3_jb_y = NULL;
  int32_T c3_i105;
  real_T c3_kb_u[2];
  const mxArray *c3_kb_y = NULL;
  real_T c3_lb_u;
  const mxArray *c3_lb_y = NULL;
  real_T c3_mb_u;
  const mxArray *c3_mb_y = NULL;
  int32_T c3_i106;
  real_T c3_nb_u[4];
  const mxArray *c3_nb_y = NULL;
  int32_T c3_i107;
  real_T c3_ob_u[50];
  const mxArray *c3_ob_y = NULL;
  real_T c3_pb_u;
  const mxArray *c3_pb_y = NULL;
  real_T c3_qb_u;
  const mxArray *c3_qb_y = NULL;
  real_T c3_rb_u;
  const mxArray *c3_rb_y = NULL;
  real_T c3_sb_u;
  const mxArray *c3_sb_y = NULL;
  real_T c3_tb_u;
  const mxArray *c3_tb_y = NULL;
  real_T c3_ub_u;
  const mxArray *c3_ub_y = NULL;
  boolean_T c3_vb_u;
  const mxArray *c3_vb_y = NULL;
  real_T c3_wb_u;
  const mxArray *c3_wb_y = NULL;
  real_T c3_xb_u;
  const mxArray *c3_xb_y = NULL;
  real_T c3_yb_u;
  const mxArray *c3_yb_y = NULL;
  real_T c3_ac_u;
  const mxArray *c3_ac_y = NULL;
  real_T c3_bc_u;
  const mxArray *c3_bc_y = NULL;
  real_T c3_cc_u;
  const mxArray *c3_cc_y = NULL;
  real_T c3_dc_u;
  const mxArray *c3_dc_y = NULL;
  real_T c3_ec_u;
  const mxArray *c3_ec_y = NULL;
  real_T c3_fc_u;
  const mxArray *c3_fc_y = NULL;
  int32_T c3_i108;
  real_T c3_gc_u[16];
  const mxArray *c3_gc_y = NULL;
  real_T c3_hc_u;
  const mxArray *c3_hc_y = NULL;
  int32_T c3_i109;
  real_T c3_ic_u[16];
  const mxArray *c3_ic_y = NULL;
  int32_T c3_i110;
  real_T c3_jc_u[4];
  const mxArray *c3_jc_y = NULL;
  real_T c3_kc_u;
  const mxArray *c3_kc_y = NULL;
  real_T c3_lc_u;
  const mxArray *c3_lc_y = NULL;
  real_T c3_mc_u;
  const mxArray *c3_mc_y = NULL;
  real_T c3_nc_u;
  const mxArray *c3_nc_y = NULL;
  real_T c3_oc_u;
  const mxArray *c3_oc_y = NULL;
  real_T c3_pc_u;
  const mxArray *c3_pc_y = NULL;
  real_T c3_qc_u;
  const mxArray *c3_qc_y = NULL;
  real_T c3_rc_u;
  const mxArray *c3_rc_y = NULL;
  real_T c3_sc_u;
  const mxArray *c3_sc_y = NULL;
  real_T c3_tc_u;
  const mxArray *c3_tc_y = NULL;
  real_T c3_uc_u;
  const mxArray *c3_uc_y = NULL;
  real_T c3_vc_u;
  const mxArray *c3_vc_y = NULL;
  real_T c3_wc_u;
  const mxArray *c3_wc_y = NULL;
  real_T c3_xc_u;
  const mxArray *c3_xc_y = NULL;
  real_T c3_yc_u;
  const mxArray *c3_yc_y = NULL;
  real_T c3_ad_u;
  const mxArray *c3_ad_y = NULL;
  int32_T c3_i111;
  real_T c3_bd_u[50];
  const mxArray *c3_bd_y = NULL;
  boolean_T c3_cd_u;
  const mxArray *c3_cd_y = NULL;
  int32_T c3_i112;
  real_T c3_dd_u[4];
  const mxArray *c3_dd_y = NULL;
  int32_T c3_i113;
  real_T c3_ed_u[2];
  const mxArray *c3_ed_y = NULL;
  int32_T c3_i114;
  real_T c3_fd_u[2];
  const mxArray *c3_fd_y = NULL;
  int32_T c3_i115;
  real_T c3_gd_u[2];
  const mxArray *c3_gd_y = NULL;
  real_T c3_hd_u;
  const mxArray *c3_hd_y = NULL;
  int32_T c3_i116;
  real_T c3_id_u[4];
  const mxArray *c3_id_y = NULL;
  real_T c3_jd_u;
  const mxArray *c3_jd_y = NULL;
  real_T c3_kd_u;
  const mxArray *c3_kd_y = NULL;
  real_T c3_ld_u;
  const mxArray *c3_ld_y = NULL;
  real_T c3_md_u;
  const mxArray *c3_md_y = NULL;
  real_T c3_nd_u;
  const mxArray *c3_nd_y = NULL;
  const mxArray *c3_c_iAout = NULL;
  const mxArray *c3_c_status = NULL;
  const mxArray *c3_c_useq = NULL;
  const mxArray *c3_c_cost = NULL;
  const mxArray *c3_od_u = NULL;
  real_T c3_dv27[50];
  int32_T c3_i117;
  boolean_T c3_bv1[5];
  int32_T c3_i118;
  int32_T c3_i119;
  int32_T c3_i120;
  int32_T c3_i121;
  real_T c3_e_A[4];
  int32_T c3_i122;
  real_T c3_d_b[2];
  real_T c3_e_b;
  int32_T c3_i123;
  real_T c3_f_b;
  int32_T c3_i124;
  real_T c3_od_y[2];
  real_T c3_g_b;
  int32_T c3_i125;
  real_T c3_pd_y[2];
  int32_T c3_i126;
  int32_T c3_i127;
  int32_T c3_i128;
  int32_T c3_i129;
  int32_T c3_i130;
  int32_T c3_i131;
  int32_T c3_i132;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c3_sfEvent);
  c3_hoistedGlobal = *chartInstance->c3_old_u;
  c3_b_hoistedGlobal = *chartInstance->c3_ym;
  c3_c_hoistedGlobal = *chartInstance->c3_ref;
  c3_d_hoistedGlobal = *chartInstance->c3_md;
  c3_e_hoistedGlobal = *chartInstance->c3_umin;
  c3_f_hoistedGlobal = *chartInstance->c3_umax;
  c3_g_hoistedGlobal = *chartInstance->c3_ymin;
  c3_h_hoistedGlobal = *chartInstance->c3_ymax;
  c3_i_hoistedGlobal = *chartInstance->c3_switch_in;
  c3_j_hoistedGlobal = *chartInstance->c3_ext_mv;
  c3_k_hoistedGlobal = *chartInstance->c3_MVtarget;
  c3_l_hoistedGlobal = *chartInstance->c3_ywt;
  c3_m_hoistedGlobal = *chartInstance->c3_uwt;
  c3_n_hoistedGlobal = *chartInstance->c3_duwt;
  c3_o_hoistedGlobal = *chartInstance->c3_rhoeps;
  for (c3_i56 = 0; c3_i56 < 2; c3_i56++) {
    c3_b_xk[c3_i56] = (*chartInstance->c3_xk)[c3_i56];
  }

  c3_b_old_u = c3_hoistedGlobal;
  c3_b_ym = c3_b_hoistedGlobal;
  c3_b_ref = c3_c_hoistedGlobal;
  c3_b_md = c3_d_hoistedGlobal;
  c3_b_umin = c3_e_hoistedGlobal;
  c3_b_umax = c3_f_hoistedGlobal;
  c3_b_ymin = c3_g_hoistedGlobal;
  c3_b_ymax = c3_h_hoistedGlobal;
  c3_b_switch_in = c3_i_hoistedGlobal;
  c3_b_ext_mv = c3_j_hoistedGlobal;
  c3_b_MVtarget = c3_k_hoistedGlobal;
  c3_b_ywt = c3_l_hoistedGlobal;
  c3_b_uwt = c3_m_hoistedGlobal;
  c3_b_duwt = c3_n_hoistedGlobal;
  c3_b_rhoeps = c3_o_hoistedGlobal;
  for (c3_i57 = 0; c3_i57 < 5; c3_i57++) {
    c3_b_iA[c3_i57] = (*chartInstance->c3_iA)[c3_i57];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 112U, 112U, c3_debug_family_names,
    c3_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_isSimulation, 0U, c3_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_isAdaptive, 1U, c3_j_sf_marshallOut,
    c3_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_isDouble, 2U, c3_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_ZERO, 3U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_ONE, 4U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c3_rseq, 5U, c3_d_sf_marshallOut,
    c3_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c3_vseq, 6U, c3_n_sf_marshallOut,
    c3_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_v, 7U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_delmv, 8U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_ym_est, 9U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_y_innov, 10U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c3_utargetValue, 11U, c3_d_sf_marshallOut,
    c3_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_isQP, 12U, c3_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_nx, 13U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_nu, 14U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_ny, 15U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_degrees, 16U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Hinv, 17U, c3_i_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Kx, 18U, c3_m_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Ku1, 19U, c3_h_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Kut, 20U, c3_l_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Kr, 21U, c3_l_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Kv, 22U, c3_k_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Mlim, 23U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Mx, 24U, c3_f_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Mu1, 25U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Mv, 26U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_z_degrees, 27U, c3_e_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_utarget, 28U, c3_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_p, 29U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_uoff, 30U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_voff, 31U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_yoff, 32U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_maxiter, 33U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_nxQP, 34U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_openloopflag, 35U, c3_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_lims_inport, 36U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_umin, 37U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_umax, 38U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_ymin, 39U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_ymax, 40U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_switch_inport, 41U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_switch, 42U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_enable_value, 43U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_return_cost, 44U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_H, 45U, c3_i_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_return_sequence, 46U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Linv, 47U, c3_i_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_Ac, 48U, c3_h_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_ywt, 49U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_uwt, 50U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_duwt, 51U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_rhoeps, 52U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Wy, 53U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Wdu, 54U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Jm, 55U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_SuJm, 56U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Su1, 57U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Sx, 58U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Hv, 59U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Wu, 60U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_I1, 61U, c3_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_A, 62U, c3_g_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_Bu, 63U, c3_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_Bv, 64U, c3_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_c_C, 65U, c3_f_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Dv, 66U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_Mrows, 67U, c3_e_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_nCC, 68U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Ecc, 69U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Fcc, 70U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Scc, 71U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Gcc, 72U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_nv, 73U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_md, 74U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_ref, 75U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_uref, 76U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_no_mv, 77U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_Rscale, 78U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_MDscale, 79U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_myindex, 80U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_myoff, 81U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_xoff, 82U, c3_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_c_CustomEstimation, 83U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_M, 84U, c3_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_L, 85U, c3_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_nargin, 86U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_nargout, 87U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_xk, 88U, c3_b_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_old_u, 89U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_ym, 90U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_ref, 91U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_md, 92U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_umin, 93U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_umax, 94U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_ymin, 95U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_ymax, 96U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_switch_in, 97U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_ext_mv, 98U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_MVtarget, 99U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_ywt, 100U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_uwt, 101U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_duwt, 102U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c3_b_rhoeps, 103U, c3_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c3_b_iA, 104U, c3_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c3_b_xk1, 105U, c3_b_sf_marshallOut,
    c3_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_b_u, 106U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_b_cost, 107U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c3_b_useq, 108U, c3_d_sf_marshallOut,
    c3_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c3_b_status, 109U, c3_c_sf_marshallOut,
    c3_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c3_b_xest, 110U, c3_b_sf_marshallOut,
    c3_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c3_b_iAout, 111U, c3_sf_marshallOut,
    c3_sf_marshallIn);
  for (c3_i58 = 0; c3_i58 < 2; c3_i58++) {
    c3_b_L[c3_i58] = 1.8194639560666E-5 + 0.15271476479067805 * (real_T)c3_i58;
  }

  for (c3_i59 = 0; c3_i59 < 2; c3_i59++) {
    c3_b_M[c3_i59] = 1.8194639567207035E-5 + 0.1527147647906715 * (real_T)c3_i59;
  }

  c3_c_CustomEstimation = c3_b_CustomEstimation;
  for (c3_i60 = 0; c3_i60 < 2; c3_i60++) {
    c3_b_xoff[c3_i60] = 0.0;
  }

  c3_c_myoff = c3_b_myoff;
  c3_c_myindex = c3_b_myindex;
  c3_c_MDscale = c3_b_MDscale;
  c3_c_Rscale = c3_b_Rscale;
  c3_c_no_mv = c3_b_no_mv;
  c3_c_no_uref = c3_b_no_uref;
  c3_c_no_ref = c3_b_no_ref;
  c3_c_no_md = c3_b_no_md;
  c3_c_nv = c3_b_nv;
  c3_c_Gcc = c3_b_Gcc;
  c3_c_Scc = c3_b_Scc;
  c3_c_Fcc = c3_b_Fcc;
  c3_c_Ecc = c3_b_Ecc;
  c3_c_nCC = c3_b_nCC;
  for (c3_i61 = 0; c3_i61 < 4; c3_i61++) {
    c3_b_Mrows[c3_i61] = 0.0;
  }

  c3_c_Dv = c3_b_Dv;
  for (c3_i62 = 0; c3_i62 < 2; c3_i62++) {
    c3_c_C[c3_i62] = 3.5999999999999992 + -2.5999999999999992 * (real_T)c3_i62;
  }

  for (c3_i63 = 0; c3_i63 < 2; c3_i63++) {
    c3_b_Bv[c3_i63] = 0.0;
  }

  for (c3_i64 = 0; c3_i64 < 2; c3_i64++) {
    c3_b_Bu[c3_i64] = 0.0010936806704141835 + -0.0010936806704141835 * (real_T)
      c3_i64;
  }

  for (c3_i65 = 0; c3_i65 < 4; c3_i65++) {
    c3_c_A[c3_i65] = c3_d_A[c3_i65];
  }

  for (c3_i66 = 0; c3_i66 < 50; c3_i66++) {
    c3_c_I1[c3_i66] = 1.0;
  }

  c3_c_Wu = c3_b_Wu;
  c3_c_Hv = c3_b_Hv;
  c3_c_Sx = c3_b_Sx;
  c3_c_Su1 = c3_b_Su1;
  c3_c_SuJm = c3_b_SuJm;
  c3_c_Jm = c3_b_Jm;
  c3_c_Wdu = c3_b_Wdu;
  c3_c_Wy = c3_b_Wy;
  c3_c_no_rhoeps = c3_b_no_rhoeps;
  c3_c_no_duwt = c3_b_no_duwt;
  c3_c_no_uwt = c3_b_no_uwt;
  c3_c_no_ywt = c3_b_no_ywt;
  for (c3_i67 = 0; c3_i67 < 4; c3_i67++) {
    c3_c_Ac[c3_i67] = 0.0;
  }

  for (c3_i68 = 0; c3_i68 < 16; c3_i68++) {
    c3_c_Linv[c3_i68] = c3_d_Linv[c3_i68];
  }

  c3_c_return_sequence = c3_b_return_sequence;
  for (c3_i69 = 0; c3_i69 < 16; c3_i69++) {
    c3_c_H[c3_i69] = c3_d_H[c3_i69];
  }

  c3_c_return_cost = c3_b_return_cost;
  c3_c_enable_value = c3_b_enable_value;
  c3_c_no_switch = c3_b_no_switch;
  c3_c_switch_inport = c3_b_switch_inport;
  c3_c_no_ymax = c3_b_no_ymax;
  c3_c_no_ymin = c3_b_no_ymin;
  c3_c_no_umax = c3_b_no_umax;
  c3_c_no_umin = c3_b_no_umin;
  c3_c_lims_inport = c3_b_lims_inport;
  c3_c_openloopflag = c3_b_openloopflag;
  c3_c_nxQP = c3_b_nxQP;
  c3_c_maxiter = c3_b_maxiter;
  c3_c_yoff = c3_b_yoff;
  c3_c_voff = c3_b_voff;
  c3_c_uoff = c3_b_uoff;
  c3_c_p = c3_b_p;
  for (c3_i70 = 0; c3_i70 < 50; c3_i70++) {
    c3_c_utarget[c3_i70] = 0.0;
  }

  for (c3_i71 = 0; c3_i71 < 4; c3_i71++) {
    c3_c_z_degrees[c3_i71] = 0.0;
  }

  c3_c_Mv = c3_b_Mv;
  c3_c_Mu1 = c3_b_Mu1;
  for (c3_i72 = 0; c3_i72 < 2; c3_i72++) {
    c3_c_Mx[c3_i72] = 0.0;
  }

  c3_c_Mlim = c3_b_Mlim;
  for (c3_i73 = 0; c3_i73 < 204; c3_i73++) {
    c3_c_Kv[c3_i73] = 0.0;
  }

  for (c3_i74 = 0; c3_i74 < 200; c3_i74++) {
    c3_c_Kr[c3_i74] = c3_d_Kr[c3_i74];
  }

  for (c3_i75 = 0; c3_i75 < 200; c3_i75++) {
    c3_c_Kut[c3_i75] = -0.0;
  }

  for (c3_i76 = 0; c3_i76 < 4; c3_i76++) {
    c3_c_Ku1[c3_i76] = c3_d_Ku1[c3_i76];
  }

  for (c3_i77 = 0; c3_i77 < 8; c3_i77++) {
    c3_c_Kx[c3_i77] = c3_d_Kx[c3_i77];
  }

  for (c3_i78 = 0; c3_i78 < 16; c3_i78++) {
    c3_c_Hinv[c3_i78] = c3_d_Hinv[c3_i78];
  }

  c3_c_degrees = c3_b_degrees;
  c3_c_ny = c3_b_ny;
  c3_c_nu = c3_b_nu;
  c3_c_nx = c3_b_nx;
  c3_c_isQP = c3_b_isQP;
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 13);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 14);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 15);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 16);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 19);
  c3_isSimulation = true;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 20);
  c3_isAdaptive = false;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 21);
  c3_isDouble = true;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 22);
  c3_ZERO = 0.0;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 23);
  c3_ONE = 1.0;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 28);
  CV_EML_IF(0, 1, 0, c3_isSimulation);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 29);
  for (c3_i79 = 0; c3_i79 < 50; c3_i79++) {
    c3_rseq[c3_i79] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 30);
  for (c3_i80 = 0; c3_i80 < 51; c3_i80++) {
    c3_vseq[c3_i80] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 31);
  c3_v = 0.0;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 32);
  for (c3_i81 = 0; c3_i81 < 2; c3_i81++) {
    c3_b_xk1[c3_i81] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 33);
  c3_b_u = 0.0;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 34);
  c3_b_cost = c3_ZERO;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 35);
  for (c3_i82 = 0; c3_i82 < 50; c3_i82++) {
    c3_b_useq[c3_i82] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 36);
  c3_b_status = c3_ONE;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 37);
  for (c3_i83 = 0; c3_i83 < 2; c3_i83++) {
    c3_b_xest[c3_i83] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 38);
  for (c3_i84 = 0; c3_i84 < 5; c3_i84++) {
    c3_b_iAout[c3_i84] = c3_b_iA[c3_i84];
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 42);
  CV_EML_IF(0, 1, 1, c3_isSimulation);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 44);
  CV_EML_IF(0, 1, 2, c3_isDouble);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 45);
  c3_c_u = c3_b_ref;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", &c3_c_u, 0, 0U, 0U, 0U, 0), false);
  c3_d_u = c3_b_md;
  c3_b_y = NULL;
  sf_mex_assign(&c3_b_y, sf_mex_create("y", &c3_d_u, 0, 0U, 0U, 0U, 0), false);
  c3_e_u = c3_b_nv;
  c3_c_y = NULL;
  sf_mex_assign(&c3_c_y, sf_mex_create("y", &c3_e_u, 0, 0U, 0U, 0U, 0), false);
  c3_f_u = c3_b_ny;
  c3_d_y = NULL;
  sf_mex_assign(&c3_d_y, sf_mex_create("y", &c3_f_u, 0, 0U, 0U, 0U, 0), false);
  c3_g_u = c3_b_p;
  c3_e_y = NULL;
  sf_mex_assign(&c3_e_y, sf_mex_create("y", &c3_g_u, 0, 0U, 0U, 0U, 0), false);
  c3_h_u = c3_b_yoff;
  c3_f_y = NULL;
  sf_mex_assign(&c3_f_y, sf_mex_create("y", &c3_h_u, 0, 0U, 0U, 0U, 0), false);
  c3_i_u = c3_b_voff;
  c3_g_y = NULL;
  sf_mex_assign(&c3_g_y, sf_mex_create("y", &c3_i_u, 0, 0U, 0U, 0U, 0), false);
  c3_j_u = c3_b_no_md;
  c3_h_y = NULL;
  sf_mex_assign(&c3_h_y, sf_mex_create("y", &c3_j_u, 0, 0U, 0U, 0U, 0), false);
  c3_k_u = c3_b_no_ref;
  c3_i_y = NULL;
  sf_mex_assign(&c3_i_y, sf_mex_create("y", &c3_k_u, 0, 0U, 0U, 0U, 0), false);
  c3_l_u = c3_b_openloopflag;
  c3_j_y = NULL;
  sf_mex_assign(&c3_j_y, sf_mex_create("y", &c3_l_u, 11, 0U, 0U, 0U, 0), false);
  c3_m_u = c3_b_Rscale;
  c3_k_y = NULL;
  sf_mex_assign(&c3_k_y, sf_mex_create("y", &c3_m_u, 0, 0U, 0U, 0U, 0), false);
  c3_n_u = c3_b_MDscale;
  c3_l_y = NULL;
  sf_mex_assign(&c3_l_y, sf_mex_create("y", &c3_n_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "mpcblock_refmd_double_mex", 3U,
                    12U, 14, c3_y, 14, c3_b_y, 14, c3_c_y, 14, c3_d_y, 14,
                    c3_e_y, 14, c3_f_y, 14, c3_g_y, 14, c3_h_y, 14, c3_i_y, 14,
                    c3_j_y, 14, c3_k_y, 14, c3_l_y, &c3_b_rseq, &c3_b_vseq,
                    &c3_b_v);
  c3_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_rseq), "rseq", c3_dv24);
  for (c3_i85 = 0; c3_i85 < 50; c3_i85++) {
    c3_rseq[c3_i85] = c3_dv24[c3_i85];
  }

  c3_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_vseq), "vseq", c3_dv25);
  for (c3_i86 = 0; c3_i86 < 51; c3_i86++) {
    c3_vseq[c3_i86] = c3_dv25[c3_i86];
  }

  c3_v = c3_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_v), "v");
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 56);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 57);
  CV_EML_IF(0, 1, 3, CV_RELATIONAL_EVAL(4U, 0U, 0, c3_b_no_mv, c3_ONE, -1, 0U,
             c3_b_no_mv == c3_ONE));
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 58);
  c3_delmv = 0.0;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 66);
  for (c3_i87 = 0; c3_i87 < 2; c3_i87++) {
    c3_b_xk[c3_i87];
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 67);
  CV_EML_IF(0, 1, 4, CV_RELATIONAL_EVAL(4U, 0U, 1, c3_b_CustomEstimation, c3_ONE,
             -1, 0U, c3_b_CustomEstimation == c3_ONE));
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 73);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 76);
  for (c3_i88 = 0; c3_i88 < 2; c3_i88++) {
    c3_b_xk[c3_i88];
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 78);
  for (c3_i89 = 0; c3_i89 < 2; c3_i89++) {
    c3_b[c3_i89] = c3_b_xk[c3_i89];
  }

  for (c3_i90 = 0; c3_i90 < 2; c3_i90++) {
    c3_dv26[c3_i90] = 3.5999999999999992 + -2.5999999999999992 * (real_T)c3_i90;
  }

  for (c3_i91 = 0; c3_i91 < 2; c3_i91++) {
    c3_b_b[c3_i91] = c3_b[c3_i91];
  }

  c3_m_y = c3_eml_xdotu(chartInstance, c3_dv26, c3_b_b);
  c3_ym_est = c3_m_y + c3_b_Dv * c3_v;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 79);
  c3_y_innov = c3_b_ym - c3_ym_est;
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 80);
  c3_c_b = c3_y_innov;
  for (c3_i92 = 0; c3_i92 < 2; c3_i92++) {
    c3_n_y[c3_i92] = (1.8194639567207035E-5 + 0.1527147647906715 * (real_T)
                      c3_i92) * c3_c_b;
  }

  for (c3_i93 = 0; c3_i93 < 2; c3_i93++) {
    c3_b_xest[c3_i93] = c3_b_xk[c3_i93] + c3_n_y[c3_i93];
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 85);
  CV_EML_IF(0, 1, 5, CV_RELATIONAL_EVAL(4U, 0U, 2, c3_b_no_uref, c3_ONE, -1, 0U,
             c3_b_no_uref == c3_ONE));
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 87);
  for (c3_i94 = 0; c3_i94 < 50; c3_i94++) {
    c3_utargetValue[c3_i94] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 95);
  CV_EML_IF(0, 1, 6, c3_isSimulation);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 97);
  CV_EML_IF(0, 1, 7, c3_isDouble);
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 98);
  for (c3_i95 = 0; c3_i95 < 50; c3_i95++) {
    c3_o_u[c3_i95] = c3_rseq[c3_i95];
  }

  c3_o_y = NULL;
  sf_mex_assign(&c3_o_y, sf_mex_create("y", c3_o_u, 0, 0U, 1U, 0U, 1, 50), false);
  for (c3_i96 = 0; c3_i96 < 51; c3_i96++) {
    c3_p_u[c3_i96] = c3_vseq[c3_i96];
  }

  c3_p_y = NULL;
  sf_mex_assign(&c3_p_y, sf_mex_create("y", c3_p_u, 0, 0U, 1U, 0U, 1, 51), false);
  c3_q_u = c3_b_umin;
  c3_q_y = NULL;
  sf_mex_assign(&c3_q_y, sf_mex_create("y", &c3_q_u, 0, 0U, 0U, 0U, 0), false);
  c3_r_u = c3_b_umax;
  c3_r_y = NULL;
  sf_mex_assign(&c3_r_y, sf_mex_create("y", &c3_r_u, 0, 0U, 0U, 0U, 0), false);
  c3_s_u = c3_b_ymin;
  c3_s_y = NULL;
  sf_mex_assign(&c3_s_y, sf_mex_create("y", &c3_s_u, 0, 0U, 0U, 0U, 0), false);
  c3_t_u = c3_b_ymax;
  c3_t_y = NULL;
  sf_mex_assign(&c3_t_y, sf_mex_create("y", &c3_t_u, 0, 0U, 0U, 0U, 0), false);
  c3_u_u = c3_b_switch_in;
  c3_u_y = NULL;
  sf_mex_assign(&c3_u_y, sf_mex_create("y", &c3_u_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i97 = 0; c3_i97 < 2; c3_i97++) {
    c3_v_u[c3_i97] = c3_b_xest[c3_i97];
  }

  c3_v_y = NULL;
  sf_mex_assign(&c3_v_y, sf_mex_create("y", c3_v_u, 0, 0U, 1U, 0U, 1, 2), false);
  c3_w_u = c3_b_old_u;
  c3_w_y = NULL;
  sf_mex_assign(&c3_w_y, sf_mex_create("y", &c3_w_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i98 = 0; c3_i98 < 5; c3_i98++) {
    c3_x_u[c3_i98] = c3_b_iA[c3_i98];
  }

  c3_x_y = NULL;
  sf_mex_assign(&c3_x_y, sf_mex_create("y", c3_x_u, 11, 0U, 1U, 0U, 1, 5), false);
  c3_y_u = c3_b_isQP;
  c3_y_y = NULL;
  sf_mex_assign(&c3_y_y, sf_mex_create("y", &c3_y_u, 11, 0U, 0U, 0U, 0), false);
  c3_ab_u = c3_b_nu;
  c3_ab_y = NULL;
  sf_mex_assign(&c3_ab_y, sf_mex_create("y", &c3_ab_u, 0, 0U, 0U, 0U, 0), false);
  c3_bb_u = c3_b_ny;
  c3_bb_y = NULL;
  sf_mex_assign(&c3_bb_y, sf_mex_create("y", &c3_bb_u, 0, 0U, 0U, 0U, 0), false);
  c3_cb_u = c3_b_degrees;
  c3_cb_y = NULL;
  sf_mex_assign(&c3_cb_y, sf_mex_create("y", &c3_cb_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i99 = 0; c3_i99 < 16; c3_i99++) {
    c3_db_u[c3_i99] = c3_d_Hinv[c3_i99];
  }

  c3_db_y = NULL;
  sf_mex_assign(&c3_db_y, sf_mex_create("y", c3_db_u, 0, 0U, 1U, 0U, 2, 4, 4),
                false);
  for (c3_i100 = 0; c3_i100 < 8; c3_i100++) {
    c3_eb_u[c3_i100] = c3_d_Kx[c3_i100];
  }

  c3_eb_y = NULL;
  sf_mex_assign(&c3_eb_y, sf_mex_create("y", c3_eb_u, 0, 0U, 1U, 0U, 2, 2, 4),
                false);
  for (c3_i101 = 0; c3_i101 < 4; c3_i101++) {
    c3_fb_u[c3_i101] = c3_d_Ku1[c3_i101];
  }

  c3_fb_y = NULL;
  sf_mex_assign(&c3_fb_y, sf_mex_create("y", c3_fb_u, 0, 0U, 1U, 0U, 2, 1, 4),
                false);
  for (c3_i102 = 0; c3_i102 < 200; c3_i102++) {
    c3_gb_u[c3_i102] = -0.0;
  }

  c3_gb_y = NULL;
  sf_mex_assign(&c3_gb_y, sf_mex_create("y", c3_gb_u, 0, 0U, 1U, 0U, 2, 50, 4),
                false);
  for (c3_i103 = 0; c3_i103 < 200; c3_i103++) {
    c3_hb_u[c3_i103] = c3_d_Kr[c3_i103];
  }

  c3_hb_y = NULL;
  sf_mex_assign(&c3_hb_y, sf_mex_create("y", c3_hb_u, 0, 0U, 1U, 0U, 2, 50, 4),
                false);
  for (c3_i104 = 0; c3_i104 < 204; c3_i104++) {
    c3_ib_u[c3_i104] = 0.0;
  }

  c3_ib_y = NULL;
  sf_mex_assign(&c3_ib_y, sf_mex_create("y", c3_ib_u, 0, 0U, 1U, 0U, 2, 51, 4),
                false);
  c3_jb_u = c3_b_Mlim;
  c3_jb_y = NULL;
  sf_mex_assign(&c3_jb_y, sf_mex_create("y", &c3_jb_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i105 = 0; c3_i105 < 2; c3_i105++) {
    c3_kb_u[c3_i105] = 0.0;
  }

  c3_kb_y = NULL;
  sf_mex_assign(&c3_kb_y, sf_mex_create("y", c3_kb_u, 0, 0U, 1U, 0U, 2, 1, 2),
                false);
  c3_lb_u = c3_b_Mu1;
  c3_lb_y = NULL;
  sf_mex_assign(&c3_lb_y, sf_mex_create("y", &c3_lb_u, 0, 0U, 0U, 0U, 0), false);
  c3_mb_u = c3_b_Mv;
  c3_mb_y = NULL;
  sf_mex_assign(&c3_mb_y, sf_mex_create("y", &c3_mb_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i106 = 0; c3_i106 < 4; c3_i106++) {
    c3_nb_u[c3_i106] = 0.0;
  }

  c3_nb_y = NULL;
  sf_mex_assign(&c3_nb_y, sf_mex_create("y", c3_nb_u, 0, 0U, 1U, 0U, 1, 4),
                false);
  for (c3_i107 = 0; c3_i107 < 50; c3_i107++) {
    c3_ob_u[c3_i107] = c3_utargetValue[c3_i107];
  }

  c3_ob_y = NULL;
  sf_mex_assign(&c3_ob_y, sf_mex_create("y", c3_ob_u, 0, 0U, 1U, 0U, 1, 50),
                false);
  c3_pb_u = c3_b_p;
  c3_pb_y = NULL;
  sf_mex_assign(&c3_pb_y, sf_mex_create("y", &c3_pb_u, 0, 0U, 0U, 0U, 0), false);
  c3_qb_u = c3_b_uoff;
  c3_qb_y = NULL;
  sf_mex_assign(&c3_qb_y, sf_mex_create("y", &c3_qb_u, 0, 0U, 0U, 0U, 0), false);
  c3_rb_u = c3_b_voff;
  c3_rb_y = NULL;
  sf_mex_assign(&c3_rb_y, sf_mex_create("y", &c3_rb_u, 0, 0U, 0U, 0U, 0), false);
  c3_sb_u = c3_b_yoff;
  c3_sb_y = NULL;
  sf_mex_assign(&c3_sb_y, sf_mex_create("y", &c3_sb_u, 0, 0U, 0U, 0U, 0), false);
  c3_tb_u = c3_b_maxiter;
  c3_tb_y = NULL;
  sf_mex_assign(&c3_tb_y, sf_mex_create("y", &c3_tb_u, 0, 0U, 0U, 0U, 0), false);
  c3_ub_u = c3_b_nxQP;
  c3_ub_y = NULL;
  sf_mex_assign(&c3_ub_y, sf_mex_create("y", &c3_ub_u, 0, 0U, 0U, 0U, 0), false);
  c3_vb_u = c3_b_openloopflag;
  c3_vb_y = NULL;
  sf_mex_assign(&c3_vb_y, sf_mex_create("y", &c3_vb_u, 11, 0U, 0U, 0U, 0), false);
  c3_wb_u = c3_b_lims_inport;
  c3_wb_y = NULL;
  sf_mex_assign(&c3_wb_y, sf_mex_create("y", &c3_wb_u, 0, 0U, 0U, 0U, 0), false);
  c3_xb_u = c3_b_no_umin;
  c3_xb_y = NULL;
  sf_mex_assign(&c3_xb_y, sf_mex_create("y", &c3_xb_u, 0, 0U, 0U, 0U, 0), false);
  c3_yb_u = c3_b_no_umax;
  c3_yb_y = NULL;
  sf_mex_assign(&c3_yb_y, sf_mex_create("y", &c3_yb_u, 0, 0U, 0U, 0U, 0), false);
  c3_ac_u = c3_b_no_ymin;
  c3_ac_y = NULL;
  sf_mex_assign(&c3_ac_y, sf_mex_create("y", &c3_ac_u, 0, 0U, 0U, 0U, 0), false);
  c3_bc_u = c3_b_no_ymax;
  c3_bc_y = NULL;
  sf_mex_assign(&c3_bc_y, sf_mex_create("y", &c3_bc_u, 0, 0U, 0U, 0U, 0), false);
  c3_cc_u = c3_b_switch_inport;
  c3_cc_y = NULL;
  sf_mex_assign(&c3_cc_y, sf_mex_create("y", &c3_cc_u, 0, 0U, 0U, 0U, 0), false);
  c3_dc_u = c3_b_no_switch;
  c3_dc_y = NULL;
  sf_mex_assign(&c3_dc_y, sf_mex_create("y", &c3_dc_u, 0, 0U, 0U, 0U, 0), false);
  c3_ec_u = c3_b_enable_value;
  c3_ec_y = NULL;
  sf_mex_assign(&c3_ec_y, sf_mex_create("y", &c3_ec_u, 0, 0U, 0U, 0U, 0), false);
  c3_fc_u = c3_b_return_cost;
  c3_fc_y = NULL;
  sf_mex_assign(&c3_fc_y, sf_mex_create("y", &c3_fc_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i108 = 0; c3_i108 < 16; c3_i108++) {
    c3_gc_u[c3_i108] = c3_d_H[c3_i108];
  }

  c3_gc_y = NULL;
  sf_mex_assign(&c3_gc_y, sf_mex_create("y", c3_gc_u, 0, 0U, 1U, 0U, 2, 4, 4),
                false);
  c3_hc_u = c3_b_return_sequence;
  c3_hc_y = NULL;
  sf_mex_assign(&c3_hc_y, sf_mex_create("y", &c3_hc_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i109 = 0; c3_i109 < 16; c3_i109++) {
    c3_ic_u[c3_i109] = c3_d_Linv[c3_i109];
  }

  c3_ic_y = NULL;
  sf_mex_assign(&c3_ic_y, sf_mex_create("y", c3_ic_u, 0, 0U, 1U, 0U, 2, 4, 4),
                false);
  for (c3_i110 = 0; c3_i110 < 4; c3_i110++) {
    c3_jc_u[c3_i110] = 0.0;
  }

  c3_jc_y = NULL;
  sf_mex_assign(&c3_jc_y, sf_mex_create("y", c3_jc_u, 0, 0U, 1U, 0U, 2, 1, 4),
                false);
  c3_kc_u = c3_b_ywt;
  c3_kc_y = NULL;
  sf_mex_assign(&c3_kc_y, sf_mex_create("y", &c3_kc_u, 0, 0U, 0U, 0U, 0), false);
  c3_lc_u = c3_b_uwt;
  c3_lc_y = NULL;
  sf_mex_assign(&c3_lc_y, sf_mex_create("y", &c3_lc_u, 0, 0U, 0U, 0U, 0), false);
  c3_mc_u = c3_b_duwt;
  c3_mc_y = NULL;
  sf_mex_assign(&c3_mc_y, sf_mex_create("y", &c3_mc_u, 0, 0U, 0U, 0U, 0), false);
  c3_nc_u = c3_b_rhoeps;
  c3_nc_y = NULL;
  sf_mex_assign(&c3_nc_y, sf_mex_create("y", &c3_nc_u, 0, 0U, 0U, 0U, 0), false);
  c3_oc_u = c3_b_no_ywt;
  c3_oc_y = NULL;
  sf_mex_assign(&c3_oc_y, sf_mex_create("y", &c3_oc_u, 0, 0U, 0U, 0U, 0), false);
  c3_pc_u = c3_b_no_uwt;
  c3_pc_y = NULL;
  sf_mex_assign(&c3_pc_y, sf_mex_create("y", &c3_pc_u, 0, 0U, 0U, 0U, 0), false);
  c3_qc_u = c3_b_no_duwt;
  c3_qc_y = NULL;
  sf_mex_assign(&c3_qc_y, sf_mex_create("y", &c3_qc_u, 0, 0U, 0U, 0U, 0), false);
  c3_rc_u = c3_b_no_rhoeps;
  c3_rc_y = NULL;
  sf_mex_assign(&c3_rc_y, sf_mex_create("y", &c3_rc_u, 0, 0U, 0U, 0U, 0), false);
  c3_sc_u = c3_b_Wy;
  c3_sc_y = NULL;
  sf_mex_assign(&c3_sc_y, sf_mex_create("y", &c3_sc_u, 0, 0U, 0U, 0U, 0), false);
  c3_tc_u = c3_b_Wdu;
  c3_tc_y = NULL;
  sf_mex_assign(&c3_tc_y, sf_mex_create("y", &c3_tc_u, 0, 0U, 0U, 0U, 0), false);
  c3_uc_u = c3_b_Jm;
  c3_uc_y = NULL;
  sf_mex_assign(&c3_uc_y, sf_mex_create("y", &c3_uc_u, 0, 0U, 0U, 0U, 0), false);
  c3_vc_u = c3_b_SuJm;
  c3_vc_y = NULL;
  sf_mex_assign(&c3_vc_y, sf_mex_create("y", &c3_vc_u, 0, 0U, 0U, 0U, 0), false);
  c3_wc_u = c3_b_Su1;
  c3_wc_y = NULL;
  sf_mex_assign(&c3_wc_y, sf_mex_create("y", &c3_wc_u, 0, 0U, 0U, 0U, 0), false);
  c3_xc_u = c3_b_Sx;
  c3_xc_y = NULL;
  sf_mex_assign(&c3_xc_y, sf_mex_create("y", &c3_xc_u, 0, 0U, 0U, 0U, 0), false);
  c3_yc_u = c3_b_Hv;
  c3_yc_y = NULL;
  sf_mex_assign(&c3_yc_y, sf_mex_create("y", &c3_yc_u, 0, 0U, 0U, 0U, 0), false);
  c3_ad_u = c3_b_Wu;
  c3_ad_y = NULL;
  sf_mex_assign(&c3_ad_y, sf_mex_create("y", &c3_ad_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i111 = 0; c3_i111 < 50; c3_i111++) {
    c3_bd_u[c3_i111] = 1.0;
  }

  c3_bd_y = NULL;
  sf_mex_assign(&c3_bd_y, sf_mex_create("y", c3_bd_u, 0, 0U, 1U, 0U, 1, 50),
                false);
  c3_cd_u = c3_isAdaptive;
  c3_cd_y = NULL;
  sf_mex_assign(&c3_cd_y, sf_mex_create("y", &c3_cd_u, 11, 0U, 0U, 0U, 0), false);
  for (c3_i112 = 0; c3_i112 < 4; c3_i112++) {
    c3_dd_u[c3_i112] = c3_d_A[c3_i112];
  }

  c3_dd_y = NULL;
  sf_mex_assign(&c3_dd_y, sf_mex_create("y", c3_dd_u, 0, 0U, 1U, 0U, 2, 2, 2),
                false);
  for (c3_i113 = 0; c3_i113 < 2; c3_i113++) {
    c3_ed_u[c3_i113] = 0.0010936806704141835 + -0.0010936806704141835 * (real_T)
      c3_i113;
  }

  c3_ed_y = NULL;
  sf_mex_assign(&c3_ed_y, sf_mex_create("y", c3_ed_u, 0, 0U, 1U, 0U, 1, 2),
                false);
  for (c3_i114 = 0; c3_i114 < 2; c3_i114++) {
    c3_fd_u[c3_i114] = 0.0;
  }

  c3_fd_y = NULL;
  sf_mex_assign(&c3_fd_y, sf_mex_create("y", c3_fd_u, 0, 0U, 1U, 0U, 1, 2),
                false);
  for (c3_i115 = 0; c3_i115 < 2; c3_i115++) {
    c3_gd_u[c3_i115] = 3.5999999999999992 + -2.5999999999999992 * (real_T)
      c3_i115;
  }

  c3_gd_y = NULL;
  sf_mex_assign(&c3_gd_y, sf_mex_create("y", c3_gd_u, 0, 0U, 1U, 0U, 2, 1, 2),
                false);
  c3_hd_u = c3_b_Dv;
  c3_hd_y = NULL;
  sf_mex_assign(&c3_hd_y, sf_mex_create("y", &c3_hd_u, 0, 0U, 0U, 0U, 0), false);
  for (c3_i116 = 0; c3_i116 < 4; c3_i116++) {
    c3_id_u[c3_i116] = 0.0;
  }

  c3_id_y = NULL;
  sf_mex_assign(&c3_id_y, sf_mex_create("y", c3_id_u, 0, 0U, 1U, 0U, 1, 4),
                false);
  c3_jd_u = c3_b_nCC;
  c3_jd_y = NULL;
  sf_mex_assign(&c3_jd_y, sf_mex_create("y", &c3_jd_u, 0, 0U, 0U, 0U, 0), false);
  c3_kd_u = c3_b_Ecc;
  c3_kd_y = NULL;
  sf_mex_assign(&c3_kd_y, sf_mex_create("y", &c3_kd_u, 0, 0U, 0U, 0U, 0), false);
  c3_ld_u = c3_b_Fcc;
  c3_ld_y = NULL;
  sf_mex_assign(&c3_ld_y, sf_mex_create("y", &c3_ld_u, 0, 0U, 0U, 0U, 0), false);
  c3_md_u = c3_b_Scc;
  c3_md_y = NULL;
  sf_mex_assign(&c3_md_y, sf_mex_create("y", &c3_md_u, 0, 0U, 0U, 0U, 0), false);
  c3_nd_u = c3_b_Gcc;
  c3_nd_y = NULL;
  sf_mex_assign(&c3_nd_y, sf_mex_create("y", &c3_nd_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "mpcblock_optimizer_double_mex",
                    5U, 75U, 14, c3_o_y, 14, c3_p_y, 14, c3_q_y, 14, c3_r_y, 14,
                    c3_s_y, 14, c3_t_y, 14, c3_u_y, 14, c3_v_y, 14, c3_w_y, 14,
                    c3_x_y, 14, c3_y_y, 14, c3_ab_y, 14, c3_bb_y, 14, c3_cb_y,
                    14, c3_db_y, 14, c3_eb_y, 14, c3_fb_y, 14, c3_gb_y, 14,
                    c3_hb_y, 14, c3_ib_y, 14, c3_jb_y, 14, c3_kb_y, 14, c3_lb_y,
                    14, c3_mb_y, 14, c3_nb_y, 14, c3_ob_y, 14, c3_pb_y, 14,
                    c3_qb_y, 14, c3_rb_y, 14, c3_sb_y, 14, c3_tb_y, 14, c3_ub_y,
                    14, c3_vb_y, 14, c3_wb_y, 14, c3_xb_y, 14, c3_yb_y, 14,
                    c3_ac_y, 14, c3_bc_y, 14, c3_cc_y, 14, c3_dc_y, 14, c3_ec_y,
                    14, c3_fc_y, 14, c3_gc_y, 14, c3_hc_y, 14, c3_ic_y, 14,
                    c3_jc_y, 14, c3_kc_y, 14, c3_lc_y, 14, c3_mc_y, 14, c3_nc_y,
                    14, c3_oc_y, 14, c3_pc_y, 14, c3_qc_y, 14, c3_rc_y, 14,
                    c3_sc_y, 14, c3_tc_y, 14, c3_uc_y, 14, c3_vc_y, 14, c3_wc_y,
                    14, c3_xc_y, 14, c3_yc_y, 14, c3_ad_y, 14, c3_bd_y, 14,
                    c3_cd_y, 14, c3_dd_y, 14, c3_ed_y, 14, c3_fd_y, 14, c3_gd_y,
                    14, c3_hd_y, 14, c3_id_y, 14, c3_jd_y, 14, c3_kd_y, 14,
                    c3_ld_y, 14, c3_md_y, 14, c3_nd_y, &c3_od_u, &c3_c_cost,
                    &c3_c_useq, &c3_c_status, &c3_c_iAout);
  c3_b_u = c3_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_od_u), "u");
  c3_b_cost = c3_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_cost), "cost");
  c3_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_useq), "useq", c3_dv27);
  for (c3_i117 = 0; c3_i117 < 50; c3_i117++) {
    c3_b_useq[c3_i117] = c3_dv27[c3_i117];
  }

  c3_b_status = c3_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_status),
    "status");
  c3_j_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_iAout), "iAout", c3_bv1);
  for (c3_i118 = 0; c3_i118 < 5; c3_i118++) {
    c3_b_iAout[c3_i118] = c3_bv1[c3_i118];
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 134U);
  CV_EML_IF(0, 1, 8, CV_RELATIONAL_EVAL(4U, 0U, 3, c3_b_CustomEstimation, c3_ONE,
             -1, 0U, c3_b_CustomEstimation == c3_ONE));
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 138U);
  for (c3_i119 = 0; c3_i119 < 2; c3_i119++) {
    c3_b[c3_i119] = c3_b_xk[c3_i119];
  }

  c3_eml_scalar_eg(chartInstance);
  c3_eml_scalar_eg(chartInstance);
  for (c3_i120 = 0; c3_i120 < 2; c3_i120++) {
    c3_n_y[c3_i120] = 0.0;
  }

  for (c3_i121 = 0; c3_i121 < 4; c3_i121++) {
    c3_e_A[c3_i121] = c3_d_A[c3_i121];
  }

  for (c3_i122 = 0; c3_i122 < 2; c3_i122++) {
    c3_d_b[c3_i122] = c3_b[c3_i122];
  }

  c3_b_eml_xgemm(chartInstance, c3_e_A, c3_d_b, c3_n_y);
  c3_e_b = c3_b_u;
  for (c3_i123 = 0; c3_i123 < 2; c3_i123++) {
    c3_b[c3_i123] = (0.0010936806704141835 + -0.0010936806704141835 * (real_T)
                     c3_i123) * c3_e_b;
  }

  c3_f_b = c3_v;
  for (c3_i124 = 0; c3_i124 < 2; c3_i124++) {
    c3_od_y[c3_i124] = 0.0 * c3_f_b;
  }

  c3_g_b = c3_y_innov;
  for (c3_i125 = 0; c3_i125 < 2; c3_i125++) {
    c3_pd_y[c3_i125] = (1.8194639560666E-5 + 0.15271476479067805 * (real_T)
                        c3_i125) * c3_g_b;
  }

  for (c3_i126 = 0; c3_i126 < 2; c3_i126++) {
    c3_b_xk1[c3_i126] = ((c3_n_y[c3_i126] + c3_b[c3_i126]) + c3_od_y[c3_i126]) +
      c3_pd_y[c3_i126];
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 140U);
  for (c3_i127 = 0; c3_i127 < 2; c3_i127++) {
    c3_b_xk1[c3_i127];
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 142U);
  CV_EML_IF(0, 1, 9, CV_RELATIONAL_EVAL(4U, 0U, 4, c3_b_CustomEstimation, c3_ONE,
             -1, 0U, c3_b_CustomEstimation == c3_ONE));
  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, 146U);
  for (c3_i128 = 0; c3_i128 < 2; c3_i128++) {
    c3_b_xest[c3_i128];
  }

  _SFD_EML_CALL(0U, chartInstance->c3_sfEvent, -146);
  _SFD_SYMBOL_SCOPE_POP();
  sf_mex_destroy(&c3_od_u);
  sf_mex_destroy(&c3_c_cost);
  sf_mex_destroy(&c3_c_useq);
  sf_mex_destroy(&c3_c_status);
  sf_mex_destroy(&c3_c_iAout);
  sf_mex_destroy(&c3_b_rseq);
  sf_mex_destroy(&c3_b_vseq);
  sf_mex_destroy(&c3_b_v);
  for (c3_i129 = 0; c3_i129 < 2; c3_i129++) {
    (*chartInstance->c3_xk1)[c3_i129] = c3_b_xk1[c3_i129];
  }

  *chartInstance->c3_u = c3_b_u;
  *chartInstance->c3_cost = c3_b_cost;
  for (c3_i130 = 0; c3_i130 < 50; c3_i130++) {
    (*chartInstance->c3_useq)[c3_i130] = c3_b_useq[c3_i130];
  }

  *chartInstance->c3_status = c3_b_status;
  for (c3_i131 = 0; c3_i131 < 2; c3_i131++) {
    (*chartInstance->c3_xest)[c3_i131] = c3_b_xest[c3_i131];
  }

  for (c3_i132 = 0; c3_i132 < 5; c3_i132++) {
    (*chartInstance->c3_iAout)[c3_i132] = c3_b_iAout[c3_i132];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c3_sfEvent);
}

static void initSimStructsc3_mpclib(SFc3_mpclibInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void init_script_number_translation(uint32_T c3_machineNumber, uint32_T
  c3_chartNumber, uint32_T c3_instanceNumber)
{
  (void)c3_machineNumber;
  (void)c3_chartNumber;
  (void)c3_instanceNumber;
}

static const mxArray *c3_sf_marshallOut(void *chartInstanceVoid, void *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i133;
  boolean_T c3_b_inData[5];
  int32_T c3_i134;
  boolean_T c3_b_u[5];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  for (c3_i133 = 0; c3_i133 < 5; c3_i133++) {
    c3_b_inData[c3_i133] = (*(boolean_T (*)[5])c3_inData)[c3_i133];
  }

  for (c3_i134 = 0; c3_i134 < 5; c3_i134++) {
    c3_b_u[c3_i134] = c3_b_inData[c3_i134];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 11, 0U, 1U, 0U, 1, 5), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static void c3_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_b_iAout;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  boolean_T c3_y[5];
  int32_T c3_i135;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_b_iAout = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_k_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_iAout), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_b_iAout);
  for (c3_i135 = 0; c3_i135 < 5; c3_i135++) {
    (*(boolean_T (*)[5])c3_outData)[c3_i135] = c3_y[c3_i135];
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static const mxArray *c3_b_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i136;
  real_T c3_b_inData[2];
  int32_T c3_i137;
  real_T c3_b_u[2];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  for (c3_i136 = 0; c3_i136 < 2; c3_i136++) {
    c3_b_inData[c3_i136] = (*(real_T (*)[2])c3_inData)[c3_i136];
  }

  for (c3_i137 = 0; c3_i137 < 2; c3_i137++) {
    c3_b_u[c3_i137] = c3_b_inData[c3_i137];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 1, 2), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static void c3_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance, const
  mxArray *c3_b_xest, const char_T *c3_identifier, real_T c3_y[2])
{
  emlrtMsgIdentifier c3_thisId;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_xest), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_b_xest);
}

static void c3_b_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[2])
{
  real_T c3_dv28[2];
  int32_T c3_i138;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv28, 1, 0, 0U, 1, 0U, 1, 2);
  for (c3_i138 = 0; c3_i138 < 2; c3_i138++) {
    c3_y[c3_i138] = c3_dv28[c3_i138];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_b_xest;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[2];
  int32_T c3_i139;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_b_xest = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_xest), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_b_xest);
  for (c3_i139 = 0; c3_i139 < 2; c3_i139++) {
    (*(real_T (*)[2])c3_outData)[c3_i139] = c3_y[c3_i139];
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static const mxArray *c3_c_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  real_T c3_b_u;
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_b_u = *(real_T *)c3_inData;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", &c3_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static void c3_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_v;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_v = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_v), &c3_thisId);
  sf_mex_destroy(&c3_v);
  *(real_T *)c3_outData = c3_y;
  sf_mex_destroy(&c3_mxArrayInData);
}

static const mxArray *c3_d_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i140;
  real_T c3_b_inData[50];
  int32_T c3_i141;
  real_T c3_b_u[50];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  for (c3_i140 = 0; c3_i140 < 50; c3_i140++) {
    c3_b_inData[c3_i140] = (*(real_T (*)[50])c3_inData)[c3_i140];
  }

  for (c3_i141 = 0; c3_i141 < 50; c3_i141++) {
    c3_b_u[c3_i141] = c3_b_inData[c3_i141];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 1, 50), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static void c3_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_rseq;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[50];
  int32_T c3_i142;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_rseq = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_rseq), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_rseq);
  for (c3_i142 = 0; c3_i142 < 50; c3_i142++) {
    (*(real_T (*)[50])c3_outData)[c3_i142] = c3_y[c3_i142];
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static const mxArray *c3_e_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i143;
  real_T c3_b_inData[4];
  int32_T c3_i144;
  real_T c3_b_u[4];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  for (c3_i143 = 0; c3_i143 < 4; c3_i143++) {
    c3_b_inData[c3_i143] = (*(real_T (*)[4])c3_inData)[c3_i143];
  }

  for (c3_i144 = 0; c3_i144 < 4; c3_i144++) {
    c3_b_u[c3_i144] = c3_b_inData[c3_i144];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 1, 4), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_f_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i145;
  real_T c3_b_inData[2];
  int32_T c3_i146;
  real_T c3_b_u[2];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  for (c3_i145 = 0; c3_i145 < 2; c3_i145++) {
    c3_b_inData[c3_i145] = (*(real_T (*)[2])c3_inData)[c3_i145];
  }

  for (c3_i146 = 0; c3_i146 < 2; c3_i146++) {
    c3_b_u[c3_i146] = c3_b_inData[c3_i146];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 2, 1, 2), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_g_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i147;
  int32_T c3_i148;
  int32_T c3_i149;
  real_T c3_b_inData[4];
  int32_T c3_i150;
  int32_T c3_i151;
  int32_T c3_i152;
  real_T c3_b_u[4];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_i147 = 0;
  for (c3_i148 = 0; c3_i148 < 2; c3_i148++) {
    for (c3_i149 = 0; c3_i149 < 2; c3_i149++) {
      c3_b_inData[c3_i149 + c3_i147] = (*(real_T (*)[4])c3_inData)[c3_i149 +
        c3_i147];
    }

    c3_i147 += 2;
  }

  c3_i150 = 0;
  for (c3_i151 = 0; c3_i151 < 2; c3_i151++) {
    for (c3_i152 = 0; c3_i152 < 2; c3_i152++) {
      c3_b_u[c3_i152 + c3_i150] = c3_b_inData[c3_i152 + c3_i150];
    }

    c3_i150 += 2;
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 2, 2, 2), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_h_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i153;
  real_T c3_b_inData[4];
  int32_T c3_i154;
  real_T c3_b_u[4];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  for (c3_i153 = 0; c3_i153 < 4; c3_i153++) {
    c3_b_inData[c3_i153] = (*(real_T (*)[4])c3_inData)[c3_i153];
  }

  for (c3_i154 = 0; c3_i154 < 4; c3_i154++) {
    c3_b_u[c3_i154] = c3_b_inData[c3_i154];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 2, 1, 4), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_i_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i155;
  int32_T c3_i156;
  int32_T c3_i157;
  real_T c3_b_inData[16];
  int32_T c3_i158;
  int32_T c3_i159;
  int32_T c3_i160;
  real_T c3_b_u[16];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_i155 = 0;
  for (c3_i156 = 0; c3_i156 < 4; c3_i156++) {
    for (c3_i157 = 0; c3_i157 < 4; c3_i157++) {
      c3_b_inData[c3_i157 + c3_i155] = (*(real_T (*)[16])c3_inData)[c3_i157 +
        c3_i155];
    }

    c3_i155 += 4;
  }

  c3_i158 = 0;
  for (c3_i159 = 0; c3_i159 < 4; c3_i159++) {
    for (c3_i160 = 0; c3_i160 < 4; c3_i160++) {
      c3_b_u[c3_i160 + c3_i158] = c3_b_inData[c3_i160 + c3_i158];
    }

    c3_i158 += 4;
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 2, 4, 4), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_j_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  boolean_T c3_b_u;
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_b_u = *(boolean_T *)c3_inData;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", &c3_b_u, 11, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_k_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i161;
  int32_T c3_i162;
  int32_T c3_i163;
  real_T c3_b_inData[204];
  int32_T c3_i164;
  int32_T c3_i165;
  int32_T c3_i166;
  real_T c3_b_u[204];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_i161 = 0;
  for (c3_i162 = 0; c3_i162 < 4; c3_i162++) {
    for (c3_i163 = 0; c3_i163 < 51; c3_i163++) {
      c3_b_inData[c3_i163 + c3_i161] = (*(real_T (*)[204])c3_inData)[c3_i163 +
        c3_i161];
    }

    c3_i161 += 51;
  }

  c3_i164 = 0;
  for (c3_i165 = 0; c3_i165 < 4; c3_i165++) {
    for (c3_i166 = 0; c3_i166 < 51; c3_i166++) {
      c3_b_u[c3_i166 + c3_i164] = c3_b_inData[c3_i166 + c3_i164];
    }

    c3_i164 += 51;
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 2, 51, 4),
                false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_l_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i167;
  int32_T c3_i168;
  int32_T c3_i169;
  real_T c3_b_inData[200];
  int32_T c3_i170;
  int32_T c3_i171;
  int32_T c3_i172;
  real_T c3_b_u[200];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_i167 = 0;
  for (c3_i168 = 0; c3_i168 < 4; c3_i168++) {
    for (c3_i169 = 0; c3_i169 < 50; c3_i169++) {
      c3_b_inData[c3_i169 + c3_i167] = (*(real_T (*)[200])c3_inData)[c3_i169 +
        c3_i167];
    }

    c3_i167 += 50;
  }

  c3_i170 = 0;
  for (c3_i171 = 0; c3_i171 < 4; c3_i171++) {
    for (c3_i172 = 0; c3_i172 < 50; c3_i172++) {
      c3_b_u[c3_i172 + c3_i170] = c3_b_inData[c3_i172 + c3_i170];
    }

    c3_i170 += 50;
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 2, 50, 4),
                false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_m_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i173;
  int32_T c3_i174;
  int32_T c3_i175;
  real_T c3_b_inData[8];
  int32_T c3_i176;
  int32_T c3_i177;
  int32_T c3_i178;
  real_T c3_b_u[8];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_i173 = 0;
  for (c3_i174 = 0; c3_i174 < 4; c3_i174++) {
    for (c3_i175 = 0; c3_i175 < 2; c3_i175++) {
      c3_b_inData[c3_i175 + c3_i173] = (*(real_T (*)[8])c3_inData)[c3_i175 +
        c3_i173];
    }

    c3_i173 += 2;
  }

  c3_i176 = 0;
  for (c3_i177 = 0; c3_i177 < 4; c3_i177++) {
    for (c3_i178 = 0; c3_i178 < 2; c3_i178++) {
      c3_b_u[c3_i178 + c3_i176] = c3_b_inData[c3_i178 + c3_i176];
    }

    c3_i176 += 2;
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 2, 2, 4), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static const mxArray *c3_n_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_i179;
  real_T c3_b_inData[51];
  int32_T c3_i180;
  real_T c3_b_u[51];
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  for (c3_i179 = 0; c3_i179 < 51; c3_i179++) {
    c3_b_inData[c3_i179] = (*(real_T (*)[51])c3_inData)[c3_i179];
  }

  for (c3_i180 = 0; c3_i180 < 51; c3_i180++) {
    c3_b_u[c3_i180] = c3_b_inData[c3_i180];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 0, 0U, 1U, 0U, 1, 51), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static void c3_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_vseq;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[51];
  int32_T c3_i181;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_vseq = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_vseq), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_vseq);
  for (c3_i181 = 0; c3_i181 < 51; c3_i181++) {
    (*(real_T (*)[51])c3_outData)[c3_i181] = c3_y[c3_i181];
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static boolean_T c3_c_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId)
{
  boolean_T c3_y;
  boolean_T c3_b0;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), &c3_b0, 1, 11, 0U, 0, 0U, 0);
  c3_y = c3_b0;
  sf_mex_destroy(&c3_b_u);
  return c3_y;
}

static void c3_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_isAdaptive;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  boolean_T c3_y;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_isAdaptive = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_isAdaptive),
    &c3_thisId);
  sf_mex_destroy(&c3_isAdaptive);
  *(boolean_T *)c3_outData = c3_y;
  sf_mex_destroy(&c3_mxArrayInData);
}

const mxArray *sf_c3_mpclib_get_eml_resolved_functions_info(void)
{
  const mxArray *c3_nameCaptureInfo = NULL;
  c3_nameCaptureInfo = NULL;
  sf_mex_assign(&c3_nameCaptureInfo, sf_mex_createstruct("structure", 2, 33, 1),
                false);
  c3_info_helper(&c3_nameCaptureInfo);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c3_nameCaptureInfo);
  return c3_nameCaptureInfo;
}

static void c3_info_helper(const mxArray **c3_info)
{
  const mxArray *c3_rhs0 = NULL;
  const mxArray *c3_lhs0 = NULL;
  const mxArray *c3_rhs1 = NULL;
  const mxArray *c3_lhs1 = NULL;
  const mxArray *c3_rhs2 = NULL;
  const mxArray *c3_lhs2 = NULL;
  const mxArray *c3_rhs3 = NULL;
  const mxArray *c3_lhs3 = NULL;
  const mxArray *c3_rhs4 = NULL;
  const mxArray *c3_lhs4 = NULL;
  const mxArray *c3_rhs5 = NULL;
  const mxArray *c3_lhs5 = NULL;
  const mxArray *c3_rhs6 = NULL;
  const mxArray *c3_lhs6 = NULL;
  const mxArray *c3_rhs7 = NULL;
  const mxArray *c3_lhs7 = NULL;
  const mxArray *c3_rhs8 = NULL;
  const mxArray *c3_lhs8 = NULL;
  const mxArray *c3_rhs9 = NULL;
  const mxArray *c3_lhs9 = NULL;
  const mxArray *c3_rhs10 = NULL;
  const mxArray *c3_lhs10 = NULL;
  const mxArray *c3_rhs11 = NULL;
  const mxArray *c3_lhs11 = NULL;
  const mxArray *c3_rhs12 = NULL;
  const mxArray *c3_lhs12 = NULL;
  const mxArray *c3_rhs13 = NULL;
  const mxArray *c3_lhs13 = NULL;
  const mxArray *c3_rhs14 = NULL;
  const mxArray *c3_lhs14 = NULL;
  const mxArray *c3_rhs15 = NULL;
  const mxArray *c3_lhs15 = NULL;
  const mxArray *c3_rhs16 = NULL;
  const mxArray *c3_lhs16 = NULL;
  const mxArray *c3_rhs17 = NULL;
  const mxArray *c3_lhs17 = NULL;
  const mxArray *c3_rhs18 = NULL;
  const mxArray *c3_lhs18 = NULL;
  const mxArray *c3_rhs19 = NULL;
  const mxArray *c3_lhs19 = NULL;
  const mxArray *c3_rhs20 = NULL;
  const mxArray *c3_lhs20 = NULL;
  const mxArray *c3_rhs21 = NULL;
  const mxArray *c3_lhs21 = NULL;
  const mxArray *c3_rhs22 = NULL;
  const mxArray *c3_lhs22 = NULL;
  const mxArray *c3_rhs23 = NULL;
  const mxArray *c3_lhs23 = NULL;
  const mxArray *c3_rhs24 = NULL;
  const mxArray *c3_lhs24 = NULL;
  const mxArray *c3_rhs25 = NULL;
  const mxArray *c3_lhs25 = NULL;
  const mxArray *c3_rhs26 = NULL;
  const mxArray *c3_lhs26 = NULL;
  const mxArray *c3_rhs27 = NULL;
  const mxArray *c3_lhs27 = NULL;
  const mxArray *c3_rhs28 = NULL;
  const mxArray *c3_lhs28 = NULL;
  const mxArray *c3_rhs29 = NULL;
  const mxArray *c3_lhs29 = NULL;
  const mxArray *c3_rhs30 = NULL;
  const mxArray *c3_lhs30 = NULL;
  const mxArray *c3_rhs31 = NULL;
  const mxArray *c3_lhs31 = NULL;
  const mxArray *c3_rhs32 = NULL;
  const mxArray *c3_lhs32 = NULL;
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "context", "context", 0);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("eml_mtimes_helper"), "name",
                  "name", 0);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 0);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m"),
                  "resolved", "resolved", 0);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1383873694U), "fileTimeLo",
                  "fileTimeLo", 0);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 0);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 0);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 0);
  sf_mex_assign(&c3_rhs0, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs0, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs0), "rhs", "rhs", 0);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs0), "lhs", "lhs", 0);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m!common_checks"),
                  "context", "context", 1);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "coder.internal.isBuiltInNumeric"), "name", "name", 1);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 1);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/isBuiltInNumeric.m"),
                  "resolved", "resolved", 1);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1395928256U), "fileTimeLo",
                  "fileTimeLo", 1);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 1);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 1);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 1);
  sf_mex_assign(&c3_rhs1, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs1, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs1), "rhs", "rhs", 1);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs1), "lhs", "lhs", 1);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m"),
                  "context", "context", 2);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("eml_index_class"), "name",
                  "name", 2);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 2);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m"),
                  "resolved", "resolved", 2);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1323166978U), "fileTimeLo",
                  "fileTimeLo", 2);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 2);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 2);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 2);
  sf_mex_assign(&c3_rhs2, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs2, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs2), "rhs", "rhs", 2);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs2), "lhs", "lhs", 2);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m"),
                  "context", "context", 3);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("eml_scalar_eg"), "name",
                  "name", 3);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 3);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "resolved",
                  "resolved", 3);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1375977088U), "fileTimeLo",
                  "fileTimeLo", 3);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 3);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 3);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 3);
  sf_mex_assign(&c3_rhs3, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs3, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs3), "rhs", "rhs", 3);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs3), "lhs", "lhs", 3);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m"), "context",
                  "context", 4);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.scalarEg"),
                  "name", "name", 4);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 4);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/scalarEg.p"),
                  "resolved", "resolved", 4);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804170U), "fileTimeLo",
                  "fileTimeLo", 4);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 4);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 4);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 4);
  sf_mex_assign(&c3_rhs4, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs4, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs4), "rhs", "rhs", 4);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs4), "lhs", "lhs", 4);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m"),
                  "context", "context", 5);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "coder.internal.blas.use_refblas"), "name", "name", 5);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 5);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/use_refblas.p"),
                  "resolved", "resolved", 5);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804170U), "fileTimeLo",
                  "fileTimeLo", 5);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 5);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 5);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 5);
  sf_mex_assign(&c3_rhs5, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs5, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs5), "rhs", "rhs", 5);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs5), "lhs", "lhs", 5);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m"),
                  "context", "context", 6);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("eml_xdotu"), "name", "name", 6);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 6);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xdotu.m"),
                  "resolved", "resolved", 6);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1375977090U), "fileTimeLo",
                  "fileTimeLo", 6);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 6);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 6);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 6);
  sf_mex_assign(&c3_rhs6, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs6, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs6), "rhs", "rhs", 6);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs6), "lhs", "lhs", 6);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xdotu.m"), "context",
                  "context", 7);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.blas.inline"),
                  "name", "name", 7);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 7);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/inline.p"),
                  "resolved", "resolved", 7);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804172U), "fileTimeLo",
                  "fileTimeLo", 7);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 7);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 7);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 7);
  sf_mex_assign(&c3_rhs7, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs7, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs7), "rhs", "rhs", 7);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs7), "lhs", "lhs", 7);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xdotu.m"), "context",
                  "context", 8);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.blas.xdotu"),
                  "name", "name", 8);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 8);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xdotu.p"),
                  "resolved", "resolved", 8);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804170U), "fileTimeLo",
                  "fileTimeLo", 8);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 8);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 8);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 8);
  sf_mex_assign(&c3_rhs8, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs8, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs8), "rhs", "rhs", 8);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs8), "lhs", "lhs", 8);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xdotu.p"),
                  "context", "context", 9);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.blas.xdot"),
                  "name", "name", 9);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 9);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xdot.p"),
                  "resolved", "resolved", 9);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804172U), "fileTimeLo",
                  "fileTimeLo", 9);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 9);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 9);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 9);
  sf_mex_assign(&c3_rhs9, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs9, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs9), "rhs", "rhs", 9);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs9), "lhs", "lhs", 9);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xdot.p"),
                  "context", "context", 10);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "coder.internal.blas.use_refblas"), "name", "name", 10);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 10);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/use_refblas.p"),
                  "resolved", "resolved", 10);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804170U), "fileTimeLo",
                  "fileTimeLo", 10);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 10);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 10);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 10);
  sf_mex_assign(&c3_rhs10, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs10, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs10), "rhs", "rhs",
                  10);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs10), "lhs", "lhs",
                  10);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xdot.p!below_threshold"),
                  "context", "context", 11);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.blas.threshold"),
                  "name", "name", 11);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 11);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/threshold.p"),
                  "resolved", "resolved", 11);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804172U), "fileTimeLo",
                  "fileTimeLo", 11);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 11);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 11);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 11);
  sf_mex_assign(&c3_rhs11, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs11, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs11), "rhs", "rhs",
                  11);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs11), "lhs", "lhs",
                  11);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/threshold.p"),
                  "context", "context", 12);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("eml_switch_helper"), "name",
                  "name", 12);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 12);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_switch_helper.m"),
                  "resolved", "resolved", 12);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1393327258U), "fileTimeLo",
                  "fileTimeLo", 12);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 12);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 12);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 12);
  sf_mex_assign(&c3_rhs12, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs12, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs12), "rhs", "rhs",
                  12);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs12), "lhs", "lhs",
                  12);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xdot.p"),
                  "context", "context", 13);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.refblas.xdot"),
                  "name", "name", 13);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 13);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xdot.p"),
                  "resolved", "resolved", 13);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804172U), "fileTimeLo",
                  "fileTimeLo", 13);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 13);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 13);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 13);
  sf_mex_assign(&c3_rhs13, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs13, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs13), "rhs", "rhs",
                  13);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs13), "lhs", "lhs",
                  13);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xdot.p"),
                  "context", "context", 14);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.refblas.xdotx"),
                  "name", "name", 14);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 14);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xdotx.p"),
                  "resolved", "resolved", 14);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804172U), "fileTimeLo",
                  "fileTimeLo", 14);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 14);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 14);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 14);
  sf_mex_assign(&c3_rhs14, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs14, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs14), "rhs", "rhs",
                  14);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs14), "lhs", "lhs",
                  14);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xdotx.p"),
                  "context", "context", 15);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.scalarEg"),
                  "name", "name", 15);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 15);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/scalarEg.p"),
                  "resolved", "resolved", 15);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804170U), "fileTimeLo",
                  "fileTimeLo", 15);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 15);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 15);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 15);
  sf_mex_assign(&c3_rhs15, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs15, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs15), "rhs", "rhs",
                  15);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs15), "lhs", "lhs",
                  15);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xdotx.p"),
                  "context", "context", 16);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.indexMinus"),
                  "name", "name", 16);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 16);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexMinus.m"),
                  "resolved", "resolved", 16);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1372579560U), "fileTimeLo",
                  "fileTimeLo", 16);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 16);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 16);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 16);
  sf_mex_assign(&c3_rhs16, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs16, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs16), "rhs", "rhs",
                  16);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs16), "lhs", "lhs",
                  16);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xdotx.p"),
                  "context", "context", 17);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.indexTimes"),
                  "name", "name", 17);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.indexInt"),
                  "dominantType", "dominantType", 17);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexTimes.m"),
                  "resolved", "resolved", 17);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1372579560U), "fileTimeLo",
                  "fileTimeLo", 17);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 17);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 17);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 17);
  sf_mex_assign(&c3_rhs17, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs17, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs17), "rhs", "rhs",
                  17);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs17), "lhs", "lhs",
                  17);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xdotx.p"),
                  "context", "context", 18);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.indexPlus"),
                  "name", "name", 18);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.indexInt"),
                  "dominantType", "dominantType", 18);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexPlus.m"),
                  "resolved", "resolved", 18);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1372579560U), "fileTimeLo",
                  "fileTimeLo", 18);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 18);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 18);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 18);
  sf_mex_assign(&c3_rhs18, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs18, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs18), "rhs", "rhs",
                  18);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs18), "lhs", "lhs",
                  18);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xdotx.p"),
                  "context", "context", 19);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "eml_int_forloop_overflow_check"), "name", "name", 19);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 19);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m"),
                  "resolved", "resolved", 19);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1397253822U), "fileTimeLo",
                  "fileTimeLo", 19);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 19);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 19);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 19);
  sf_mex_assign(&c3_rhs19, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs19, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs19), "rhs", "rhs",
                  19);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs19), "lhs", "lhs",
                  19);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper"),
                  "context", "context", 20);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("isfi"), "name", "name", 20);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.indexInt"),
                  "dominantType", "dominantType", 20);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/fixedpoint/isfi.m"), "resolved",
                  "resolved", 20);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1346506758U), "fileTimeLo",
                  "fileTimeLo", 20);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 20);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 20);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 20);
  sf_mex_assign(&c3_rhs20, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs20, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs20), "rhs", "rhs",
                  20);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs20), "lhs", "lhs",
                  20);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/fixedpoint/isfi.m"), "context",
                  "context", 21);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("isnumerictype"), "name",
                  "name", 21);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 21);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/fixedpoint/isnumerictype.m"), "resolved",
                  "resolved", 21);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1398871998U), "fileTimeLo",
                  "fileTimeLo", 21);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 21);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 21);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 21);
  sf_mex_assign(&c3_rhs21, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs21, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs21), "rhs", "rhs",
                  21);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs21), "lhs", "lhs",
                  21);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper"),
                  "context", "context", 22);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("intmax"), "name", "name", 22);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 22);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m"), "resolved",
                  "resolved", 22);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1362258282U), "fileTimeLo",
                  "fileTimeLo", 22);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 22);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 22);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 22);
  sf_mex_assign(&c3_rhs22, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs22, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs22), "rhs", "rhs",
                  22);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs22), "lhs", "lhs",
                  22);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m"), "context",
                  "context", 23);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("eml_switch_helper"), "name",
                  "name", 23);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 23);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_switch_helper.m"),
                  "resolved", "resolved", 23);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1393327258U), "fileTimeLo",
                  "fileTimeLo", 23);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 23);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 23);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 23);
  sf_mex_assign(&c3_rhs23, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs23, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs23), "rhs", "rhs",
                  23);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs23), "lhs", "lhs",
                  23);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper"),
                  "context", "context", 24);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("intmin"), "name", "name", 24);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 24);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmin.m"), "resolved",
                  "resolved", 24);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1362258282U), "fileTimeLo",
                  "fileTimeLo", 24);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 24);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 24);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 24);
  sf_mex_assign(&c3_rhs24, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs24, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs24), "rhs", "rhs",
                  24);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs24), "lhs", "lhs",
                  24);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmin.m"), "context",
                  "context", 25);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("eml_switch_helper"), "name",
                  "name", 25);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 25);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_switch_helper.m"),
                  "resolved", "resolved", 25);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1393327258U), "fileTimeLo",
                  "fileTimeLo", 25);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 25);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 25);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 25);
  sf_mex_assign(&c3_rhs25, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs25, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs25), "rhs", "rhs",
                  25);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs25), "lhs", "lhs",
                  25);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/eml_mtimes_helper.m"),
                  "context", "context", 26);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("eml_xgemm"), "name", "name",
                  26);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 26);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m"),
                  "resolved", "resolved", 26);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1375977090U), "fileTimeLo",
                  "fileTimeLo", 26);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 26);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 26);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 26);
  sf_mex_assign(&c3_rhs26, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs26, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs26), "rhs", "rhs",
                  26);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs26), "lhs", "lhs",
                  26);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m"), "context",
                  "context", 27);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.blas.inline"),
                  "name", "name", 27);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 27);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/inline.p"),
                  "resolved", "resolved", 27);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804172U), "fileTimeLo",
                  "fileTimeLo", 27);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 27);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 27);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 27);
  sf_mex_assign(&c3_rhs27, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs27, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs27), "rhs", "rhs",
                  27);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs27), "lhs", "lhs",
                  27);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgemm.m"), "context",
                  "context", 28);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.blas.xgemm"),
                  "name", "name", 28);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 28);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xgemm.p"),
                  "resolved", "resolved", 28);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804170U), "fileTimeLo",
                  "fileTimeLo", 28);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 28);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 28);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 28);
  sf_mex_assign(&c3_rhs28, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs28, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs28), "rhs", "rhs",
                  28);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs28), "lhs", "lhs",
                  28);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xgemm.p"),
                  "context", "context", 29);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "coder.internal.blas.use_refblas"), "name", "name", 29);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(""), "dominantType",
                  "dominantType", 29);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/use_refblas.p"),
                  "resolved", "resolved", 29);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804170U), "fileTimeLo",
                  "fileTimeLo", 29);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 29);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 29);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 29);
  sf_mex_assign(&c3_rhs29, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs29, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs29), "rhs", "rhs",
                  29);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs29), "lhs", "lhs",
                  29);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xgemm.p!below_threshold"),
                  "context", "context", 30);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.blas.threshold"),
                  "name", "name", 30);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 30);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/threshold.p"),
                  "resolved", "resolved", 30);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804172U), "fileTimeLo",
                  "fileTimeLo", 30);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 30);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 30);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 30);
  sf_mex_assign(&c3_rhs30, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs30, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs30), "rhs", "rhs",
                  30);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs30), "lhs", "lhs",
                  30);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xgemm.p"),
                  "context", "context", 31);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.scalarEg"),
                  "name", "name", 31);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("double"), "dominantType",
                  "dominantType", 31);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/scalarEg.p"),
                  "resolved", "resolved", 31);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804170U), "fileTimeLo",
                  "fileTimeLo", 31);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 31);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 31);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 31);
  sf_mex_assign(&c3_rhs31, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs31, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs31), "rhs", "rhs",
                  31);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs31), "lhs", "lhs",
                  31);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+blas/xgemm.p"),
                  "context", "context", 32);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("coder.internal.refblas.xgemm"),
                  "name", "name", 32);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut("char"), "dominantType",
                  "dominantType", 32);
  sf_mex_addfield(*c3_info, c3_emlrt_marshallOut(
    "[ILXE]$matlabroot$/toolbox/coder/coder/+coder/+internal/+refblas/xgemm.p"),
                  "resolved", "resolved", 32);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(1410804172U), "fileTimeLo",
                  "fileTimeLo", 32);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "fileTimeHi",
                  "fileTimeHi", 32);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeLo",
                  "mFileTimeLo", 32);
  sf_mex_addfield(*c3_info, c3_b_emlrt_marshallOut(0U), "mFileTimeHi",
                  "mFileTimeHi", 32);
  sf_mex_assign(&c3_rhs32, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_assign(&c3_lhs32, sf_mex_createcellmatrix(0, 1), false);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_rhs32), "rhs", "rhs",
                  32);
  sf_mex_addfield(*c3_info, sf_mex_duplicatearraysafe(&c3_lhs32), "lhs", "lhs",
                  32);
  sf_mex_destroy(&c3_rhs0);
  sf_mex_destroy(&c3_lhs0);
  sf_mex_destroy(&c3_rhs1);
  sf_mex_destroy(&c3_lhs1);
  sf_mex_destroy(&c3_rhs2);
  sf_mex_destroy(&c3_lhs2);
  sf_mex_destroy(&c3_rhs3);
  sf_mex_destroy(&c3_lhs3);
  sf_mex_destroy(&c3_rhs4);
  sf_mex_destroy(&c3_lhs4);
  sf_mex_destroy(&c3_rhs5);
  sf_mex_destroy(&c3_lhs5);
  sf_mex_destroy(&c3_rhs6);
  sf_mex_destroy(&c3_lhs6);
  sf_mex_destroy(&c3_rhs7);
  sf_mex_destroy(&c3_lhs7);
  sf_mex_destroy(&c3_rhs8);
  sf_mex_destroy(&c3_lhs8);
  sf_mex_destroy(&c3_rhs9);
  sf_mex_destroy(&c3_lhs9);
  sf_mex_destroy(&c3_rhs10);
  sf_mex_destroy(&c3_lhs10);
  sf_mex_destroy(&c3_rhs11);
  sf_mex_destroy(&c3_lhs11);
  sf_mex_destroy(&c3_rhs12);
  sf_mex_destroy(&c3_lhs12);
  sf_mex_destroy(&c3_rhs13);
  sf_mex_destroy(&c3_lhs13);
  sf_mex_destroy(&c3_rhs14);
  sf_mex_destroy(&c3_lhs14);
  sf_mex_destroy(&c3_rhs15);
  sf_mex_destroy(&c3_lhs15);
  sf_mex_destroy(&c3_rhs16);
  sf_mex_destroy(&c3_lhs16);
  sf_mex_destroy(&c3_rhs17);
  sf_mex_destroy(&c3_lhs17);
  sf_mex_destroy(&c3_rhs18);
  sf_mex_destroy(&c3_lhs18);
  sf_mex_destroy(&c3_rhs19);
  sf_mex_destroy(&c3_lhs19);
  sf_mex_destroy(&c3_rhs20);
  sf_mex_destroy(&c3_lhs20);
  sf_mex_destroy(&c3_rhs21);
  sf_mex_destroy(&c3_lhs21);
  sf_mex_destroy(&c3_rhs22);
  sf_mex_destroy(&c3_lhs22);
  sf_mex_destroy(&c3_rhs23);
  sf_mex_destroy(&c3_lhs23);
  sf_mex_destroy(&c3_rhs24);
  sf_mex_destroy(&c3_lhs24);
  sf_mex_destroy(&c3_rhs25);
  sf_mex_destroy(&c3_lhs25);
  sf_mex_destroy(&c3_rhs26);
  sf_mex_destroy(&c3_lhs26);
  sf_mex_destroy(&c3_rhs27);
  sf_mex_destroy(&c3_lhs27);
  sf_mex_destroy(&c3_rhs28);
  sf_mex_destroy(&c3_lhs28);
  sf_mex_destroy(&c3_rhs29);
  sf_mex_destroy(&c3_lhs29);
  sf_mex_destroy(&c3_rhs30);
  sf_mex_destroy(&c3_lhs30);
  sf_mex_destroy(&c3_rhs31);
  sf_mex_destroy(&c3_lhs31);
  sf_mex_destroy(&c3_rhs32);
  sf_mex_destroy(&c3_lhs32);
}

static const mxArray *c3_emlrt_marshallOut(const char * c3_b_u)
{
  const mxArray *c3_y = NULL;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", c3_b_u, 15, 0U, 0U, 0U, 2, 1, strlen
    (c3_b_u)), false);
  return c3_y;
}

static const mxArray *c3_b_emlrt_marshallOut(const uint32_T c3_b_u)
{
  const mxArray *c3_y = NULL;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", &c3_b_u, 7, 0U, 0U, 0U, 0), false);
  return c3_y;
}

static real_T c3_eml_xdotu(SFc3_mpclibInstanceStruct *chartInstance, real_T
  c3_x[2], real_T c3_y[2])
{
  real_T c3_d;
  int32_T c3_k;
  int32_T c3_b_k;
  (void)chartInstance;
  c3_d = 0.0;
  for (c3_k = 1; c3_k < 3; c3_k++) {
    c3_b_k = c3_k;
    c3_d += c3_x[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
      (real_T)c3_b_k), 1, 2, 1, 0) - 1] * c3_y[_SFD_EML_ARRAY_BOUNDS_CHECK("",
      (int32_T)_SFD_INTEGER_CHECK("", (real_T)c3_b_k), 1, 2, 1, 0) - 1];
  }

  return c3_d;
}

static void c3_eml_scalar_eg(SFc3_mpclibInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void c3_eml_xgemm(SFc3_mpclibInstanceStruct *chartInstance, real_T
  c3_c_A[4], real_T c3_B[2], real_T c3_c_C[2], real_T c3_d_C[2])
{
  int32_T c3_i182;
  int32_T c3_i183;
  real_T c3_d_A[4];
  int32_T c3_i184;
  real_T c3_b_B[2];
  for (c3_i182 = 0; c3_i182 < 2; c3_i182++) {
    c3_d_C[c3_i182] = c3_c_C[c3_i182];
  }

  for (c3_i183 = 0; c3_i183 < 4; c3_i183++) {
    c3_d_A[c3_i183] = c3_c_A[c3_i183];
  }

  for (c3_i184 = 0; c3_i184 < 2; c3_i184++) {
    c3_b_B[c3_i184] = c3_B[c3_i184];
  }

  c3_b_eml_xgemm(chartInstance, c3_d_A, c3_b_B, c3_d_C);
}

static void c3_d_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_rseq, const char_T *c3_identifier, real_T c3_y[50])
{
  emlrtMsgIdentifier c3_thisId;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_rseq), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_rseq);
}

static void c3_e_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[50])
{
  real_T c3_dv29[50];
  int32_T c3_i185;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv29, 1, 0, 0U, 1, 0U, 1, 50);
  for (c3_i185 = 0; c3_i185 < 50; c3_i185++) {
    c3_y[c3_i185] = c3_dv29[c3_i185];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_f_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_vseq, const char_T *c3_identifier, real_T c3_y[51])
{
  emlrtMsgIdentifier c3_thisId;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_vseq), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_vseq);
}

static void c3_g_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[51])
{
  real_T c3_dv30[51];
  int32_T c3_i186;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv30, 1, 0, 0U, 1, 0U, 1, 51);
  for (c3_i186 = 0; c3_i186 < 51; c3_i186++) {
    c3_y[c3_i186] = c3_dv30[c3_i186];
  }

  sf_mex_destroy(&c3_b_u);
}

static real_T c3_h_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_v, const char_T *c3_identifier)
{
  real_T c3_y;
  emlrtMsgIdentifier c3_thisId;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_v), &c3_thisId);
  sf_mex_destroy(&c3_v);
  return c3_y;
}

static real_T c3_i_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId)
{
  real_T c3_y;
  real_T c3_d53;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), &c3_d53, 1, 0, 0U, 0, 0U, 0);
  c3_y = c3_d53;
  sf_mex_destroy(&c3_b_u);
  return c3_y;
}

static void c3_j_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_iAout, const char_T *c3_identifier, boolean_T c3_y[5])
{
  emlrtMsgIdentifier c3_thisId;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_k_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_iAout), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_b_iAout);
}

static void c3_k_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, boolean_T c3_y[5])
{
  boolean_T c3_bv2[5];
  int32_T c3_i187;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_bv2, 1, 11, 0U, 1, 0U, 1, 5);
  for (c3_i187 = 0; c3_i187 < 5; c3_i187++) {
    c3_y[c3_i187] = c3_bv2[c3_i187];
  }

  sf_mex_destroy(&c3_b_u);
}

static const mxArray *c3_o_sf_marshallOut(void *chartInstanceVoid, void
  *c3_inData)
{
  const mxArray *c3_mxArrayOutData = NULL;
  int32_T c3_b_u;
  const mxArray *c3_y = NULL;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_mxArrayOutData = NULL;
  c3_b_u = *(int32_T *)c3_inData;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", &c3_b_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c3_mxArrayOutData, c3_y, false);
  return c3_mxArrayOutData;
}

static int32_T c3_l_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId)
{
  int32_T c3_y;
  int32_T c3_i188;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), &c3_i188, 1, 6, 0U, 0, 0U, 0);
  c3_y = c3_i188;
  sf_mex_destroy(&c3_b_u);
  return c3_y;
}

static void c3_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_b_sfEvent;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  int32_T c3_y;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_b_sfEvent = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_l_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_b_sfEvent),
    &c3_thisId);
  sf_mex_destroy(&c3_b_sfEvent);
  *(int32_T *)c3_outData = c3_y;
  sf_mex_destroy(&c3_mxArrayInData);
}

static void c3_m_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[16])
{
  real_T c3_dv31[16];
  int32_T c3_i189;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv31, 1, 0, 0U, 1, 0U, 2, 4,
                4);
  for (c3_i189 = 0; c3_i189 < 16; c3_i189++) {
    c3_y[c3_i189] = c3_dv31[c3_i189];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_c_Hinv;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[16];
  int32_T c3_i190;
  int32_T c3_i191;
  int32_T c3_i192;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_c_Hinv = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_m_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_Hinv), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_c_Hinv);
  c3_i190 = 0;
  for (c3_i191 = 0; c3_i191 < 4; c3_i191++) {
    for (c3_i192 = 0; c3_i192 < 4; c3_i192++) {
      (*(real_T (*)[16])c3_outData)[c3_i192 + c3_i190] = c3_y[c3_i192 + c3_i190];
    }

    c3_i190 += 4;
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static void c3_n_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[8])
{
  real_T c3_dv32[8];
  int32_T c3_i193;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv32, 1, 0, 0U, 1, 0U, 2, 2,
                4);
  for (c3_i193 = 0; c3_i193 < 8; c3_i193++) {
    c3_y[c3_i193] = c3_dv32[c3_i193];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_c_Kx;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[8];
  int32_T c3_i194;
  int32_T c3_i195;
  int32_T c3_i196;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_c_Kx = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_n_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_Kx), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_c_Kx);
  c3_i194 = 0;
  for (c3_i195 = 0; c3_i195 < 4; c3_i195++) {
    for (c3_i196 = 0; c3_i196 < 2; c3_i196++) {
      (*(real_T (*)[8])c3_outData)[c3_i196 + c3_i194] = c3_y[c3_i196 + c3_i194];
    }

    c3_i194 += 2;
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static void c3_o_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[4])
{
  real_T c3_dv33[4];
  int32_T c3_i197;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv33, 1, 0, 0U, 1, 0U, 2, 1,
                4);
  for (c3_i197 = 0; c3_i197 < 4; c3_i197++) {
    c3_y[c3_i197] = c3_dv33[c3_i197];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_c_Ku1;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[4];
  int32_T c3_i198;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_c_Ku1 = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_o_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_Ku1), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_c_Ku1);
  for (c3_i198 = 0; c3_i198 < 4; c3_i198++) {
    (*(real_T (*)[4])c3_outData)[c3_i198] = c3_y[c3_i198];
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static void c3_p_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[200])
{
  real_T c3_dv34[200];
  int32_T c3_i199;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv34, 1, 0, 0U, 1, 0U, 2, 50,
                4);
  for (c3_i199 = 0; c3_i199 < 200; c3_i199++) {
    c3_y[c3_i199] = c3_dv34[c3_i199];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_k_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_c_Kut;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[200];
  int32_T c3_i200;
  int32_T c3_i201;
  int32_T c3_i202;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_c_Kut = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_p_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_Kut), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_c_Kut);
  c3_i200 = 0;
  for (c3_i201 = 0; c3_i201 < 4; c3_i201++) {
    for (c3_i202 = 0; c3_i202 < 50; c3_i202++) {
      (*(real_T (*)[200])c3_outData)[c3_i202 + c3_i200] = c3_y[c3_i202 + c3_i200];
    }

    c3_i200 += 50;
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static void c3_q_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[204])
{
  real_T c3_dv35[204];
  int32_T c3_i203;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv35, 1, 0, 0U, 1, 0U, 2, 51,
                4);
  for (c3_i203 = 0; c3_i203 < 204; c3_i203++) {
    c3_y[c3_i203] = c3_dv35[c3_i203];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_l_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_c_Kv;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[204];
  int32_T c3_i204;
  int32_T c3_i205;
  int32_T c3_i206;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_c_Kv = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_q_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_Kv), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_c_Kv);
  c3_i204 = 0;
  for (c3_i205 = 0; c3_i205 < 4; c3_i205++) {
    for (c3_i206 = 0; c3_i206 < 51; c3_i206++) {
      (*(real_T (*)[204])c3_outData)[c3_i206 + c3_i204] = c3_y[c3_i206 + c3_i204];
    }

    c3_i204 += 51;
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static void c3_r_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[2])
{
  real_T c3_dv36[2];
  int32_T c3_i207;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv36, 1, 0, 0U, 1, 0U, 2, 1,
                2);
  for (c3_i207 = 0; c3_i207 < 2; c3_i207++) {
    c3_y[c3_i207] = c3_dv36[c3_i207];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_m_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_c_Mx;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[2];
  int32_T c3_i208;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_c_Mx = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_r_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_Mx), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_c_Mx);
  for (c3_i208 = 0; c3_i208 < 2; c3_i208++) {
    (*(real_T (*)[2])c3_outData)[c3_i208] = c3_y[c3_i208];
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static void c3_s_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[4])
{
  real_T c3_dv37[4];
  int32_T c3_i209;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv37, 1, 0, 0U, 1, 0U, 1, 4);
  for (c3_i209 = 0; c3_i209 < 4; c3_i209++) {
    c3_y[c3_i209] = c3_dv37[c3_i209];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_n_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_c_z_degrees;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[4];
  int32_T c3_i210;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_c_z_degrees = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_s_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_z_degrees), &c3_thisId,
                        c3_y);
  sf_mex_destroy(&c3_c_z_degrees);
  for (c3_i210 = 0; c3_i210 < 4; c3_i210++) {
    (*(real_T (*)[4])c3_outData)[c3_i210] = c3_y[c3_i210];
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static void c3_t_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId, real_T c3_y[4])
{
  real_T c3_dv38[4];
  int32_T c3_i211;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), c3_dv38, 1, 0, 0U, 1, 0U, 2, 2,
                2);
  for (c3_i211 = 0; c3_i211 < 4; c3_i211++) {
    c3_y[c3_i211] = c3_dv38[c3_i211];
  }

  sf_mex_destroy(&c3_b_u);
}

static void c3_o_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c3_mxArrayInData, const char_T *c3_varName, void *c3_outData)
{
  const mxArray *c3_c_A;
  const char_T *c3_identifier;
  emlrtMsgIdentifier c3_thisId;
  real_T c3_y[4];
  int32_T c3_i212;
  int32_T c3_i213;
  int32_T c3_i214;
  SFc3_mpclibInstanceStruct *chartInstance;
  chartInstance = (SFc3_mpclibInstanceStruct *)chartInstanceVoid;
  c3_c_A = sf_mex_dup(c3_mxArrayInData);
  c3_identifier = c3_varName;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_t_emlrt_marshallIn(chartInstance, sf_mex_dup(c3_c_A), &c3_thisId, c3_y);
  sf_mex_destroy(&c3_c_A);
  c3_i212 = 0;
  for (c3_i213 = 0; c3_i213 < 2; c3_i213++) {
    for (c3_i214 = 0; c3_i214 < 2; c3_i214++) {
      (*(real_T (*)[4])c3_outData)[c3_i214 + c3_i212] = c3_y[c3_i214 + c3_i212];
    }

    c3_i212 += 2;
  }

  sf_mex_destroy(&c3_mxArrayInData);
}

static uint8_T c3_u_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_is_active_c3_mpclib, const char_T *c3_identifier)
{
  uint8_T c3_y;
  emlrtMsgIdentifier c3_thisId;
  c3_thisId.fIdentifier = c3_identifier;
  c3_thisId.fParent = NULL;
  c3_y = c3_v_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c3_b_is_active_c3_mpclib), &c3_thisId);
  sf_mex_destroy(&c3_b_is_active_c3_mpclib);
  return c3_y;
}

static uint8_T c3_v_emlrt_marshallIn(SFc3_mpclibInstanceStruct *chartInstance,
  const mxArray *c3_b_u, const emlrtMsgIdentifier *c3_parentId)
{
  uint8_T c3_y;
  uint8_T c3_u0;
  (void)chartInstance;
  sf_mex_import(c3_parentId, sf_mex_dup(c3_b_u), &c3_u0, 1, 3, 0U, 0, 0U, 0);
  c3_y = c3_u0;
  sf_mex_destroy(&c3_b_u);
  return c3_y;
}

static void c3_b_eml_xgemm(SFc3_mpclibInstanceStruct *chartInstance, real_T
  c3_c_A[4], real_T c3_B[2], real_T c3_c_C[2])
{
  int32_T c3_i215;
  int32_T c3_i216;
  int32_T c3_i217;
  (void)chartInstance;
  for (c3_i215 = 0; c3_i215 < 2; c3_i215++) {
    c3_c_C[c3_i215] = 0.0;
    c3_i216 = 0;
    for (c3_i217 = 0; c3_i217 < 2; c3_i217++) {
      c3_c_C[c3_i215] += c3_c_A[c3_i216 + c3_i215] * c3_B[c3_i217];
      c3_i216 += 2;
    }
  }
}

static void init_dsm_address_info(SFc3_mpclibInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc3_mpclibInstanceStruct *chartInstance)
{
  chartInstance->c3_xk1 = (real_T (*)[2])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c3_xk = (real_T (*)[2])ssGetInputPortSignal_wrapper
    (chartInstance->S, 0);
  chartInstance->c3_old_u = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c3_ym = (real_T *)ssGetInputPortSignal_wrapper(chartInstance->S,
    2);
  chartInstance->c3_ref = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 3);
  chartInstance->c3_md = (real_T *)ssGetInputPortSignal_wrapper(chartInstance->S,
    4);
  chartInstance->c3_umin = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 5);
  chartInstance->c3_umax = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 6);
  chartInstance->c3_ymin = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 7);
  chartInstance->c3_ymax = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 8);
  chartInstance->c3_switch_in = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 9);
  chartInstance->c3_ext_mv = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 10);
  chartInstance->c3_MVtarget = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 11);
  chartInstance->c3_ywt = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 12);
  chartInstance->c3_uwt = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 13);
  chartInstance->c3_duwt = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 14);
  chartInstance->c3_rhoeps = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 15);
  chartInstance->c3_iA = (boolean_T (*)[5])ssGetInputPortSignal_wrapper
    (chartInstance->S, 16);
  chartInstance->c3_u = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    2);
  chartInstance->c3_cost = (real_T *)ssGetOutputPortSignal_wrapper
    (chartInstance->S, 3);
  chartInstance->c3_useq = (real_T (*)[50])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 4);
  chartInstance->c3_status = (real_T *)ssGetOutputPortSignal_wrapper
    (chartInstance->S, 5);
  chartInstance->c3_xest = (real_T (*)[2])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 6);
  chartInstance->c3_iAout = (boolean_T (*)[5])ssGetOutputPortSignal_wrapper
    (chartInstance->S, 7);
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c3_mpclib_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3730021664U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1136285071U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3931342645U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1984404751U);
}

mxArray* sf_c3_mpclib_get_post_codegen_info(void);
mxArray *sf_c3_mpclib_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals", "postCodegenInfo" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1, 1, sizeof
    (autoinheritanceFields)/sizeof(autoinheritanceFields[0]),
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("pcP5nOlS3PyES7ogb6fmWC");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,17,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,6,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,6,"type",mxType);
    }

    mxSetField(mxData,6,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,7,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,7,"type",mxType);
    }

    mxSetField(mxData,7,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,8,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,8,"type",mxType);
    }

    mxSetField(mxData,8,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,9,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,9,"type",mxType);
    }

    mxSetField(mxData,9,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,10,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,10,"type",mxType);
    }

    mxSetField(mxData,10,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,11,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,11,"type",mxType);
    }

    mxSetField(mxData,11,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,12,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,12,"type",mxType);
    }

    mxSetField(mxData,12,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,13,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,13,"type",mxType);
    }

    mxSetField(mxData,13,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,14,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,14,"type",mxType);
    }

    mxSetField(mxData,14,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,15,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,15,"type",mxType);
    }

    mxSetField(mxData,15,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(5);
      pr[1] = (double)(1);
      mxSetField(mxData,16,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(1));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,16,"type",mxType);
    }

    mxSetField(mxData,16,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,74,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(2);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(4);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(2);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,6,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,6,"type",mxType);
    }

    mxSetField(mxData,6,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,7,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,7,"type",mxType);
    }

    mxSetField(mxData,7,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,8,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,8,"type",mxType);
    }

    mxSetField(mxData,8,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,9,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,9,"type",mxType);
    }

    mxSetField(mxData,9,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(4);
      pr[1] = (double)(4);
      mxSetField(mxData,10,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,10,"type",mxType);
    }

    mxSetField(mxData,10,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(4);
      pr[1] = (double)(4);
      mxSetField(mxData,11,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,11,"type",mxType);
    }

    mxSetField(mxData,11,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,12,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,12,"type",mxType);
    }

    mxSetField(mxData,12,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(50);
      pr[1] = (double)(1);
      mxSetField(mxData,13,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,13,"type",mxType);
    }

    mxSetField(mxData,13,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,14,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,14,"type",mxType);
    }

    mxSetField(mxData,14,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(50);
      pr[1] = (double)(4);
      mxSetField(mxData,15,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,15,"type",mxType);
    }

    mxSetField(mxData,15,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(4);
      mxSetField(mxData,16,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,16,"type",mxType);
    }

    mxSetField(mxData,16,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(50);
      pr[1] = (double)(4);
      mxSetField(mxData,17,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,17,"type",mxType);
    }

    mxSetField(mxData,17,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(51);
      pr[1] = (double)(4);
      mxSetField(mxData,18,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,18,"type",mxType);
    }

    mxSetField(mxData,18,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(4);
      mxSetField(mxData,19,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,19,"type",mxType);
    }

    mxSetField(mxData,19,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(1);
      mxSetField(mxData,20,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,20,"type",mxType);
    }

    mxSetField(mxData,20,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(4);
      pr[1] = (double)(4);
      mxSetField(mxData,21,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,21,"type",mxType);
    }

    mxSetField(mxData,21,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(1);
      mxSetField(mxData,22,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,22,"type",mxType);
    }

    mxSetField(mxData,22,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,23,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,23,"type",mxType);
    }

    mxSetField(mxData,23,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,24,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,24,"type",mxType);
    }

    mxSetField(mxData,24,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(4);
      pr[1] = (double)(1);
      mxSetField(mxData,25,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,25,"type",mxType);
    }

    mxSetField(mxData,25,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,26,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,26,"type",mxType);
    }

    mxSetField(mxData,26,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,27,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,27,"type",mxType);
    }

    mxSetField(mxData,27,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(2);
      mxSetField(mxData,28,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,28,"type",mxType);
    }

    mxSetField(mxData,28,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,29,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,29,"type",mxType);
    }

    mxSetField(mxData,29,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,30,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,30,"type",mxType);
    }

    mxSetField(mxData,30,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,31,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,31,"type",mxType);
    }

    mxSetField(mxData,31,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,32,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,32,"type",mxType);
    }

    mxSetField(mxData,32,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,33,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,33,"type",mxType);
    }

    mxSetField(mxData,33,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,34,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,34,"type",mxType);
    }

    mxSetField(mxData,34,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,35,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,35,"type",mxType);
    }

    mxSetField(mxData,35,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,36,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,36,"type",mxType);
    }

    mxSetField(mxData,36,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,37,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,37,"type",mxType);
    }

    mxSetField(mxData,37,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,38,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,38,"type",mxType);
    }

    mxSetField(mxData,38,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,39,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(1));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,39,"type",mxType);
    }

    mxSetField(mxData,39,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,40,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,40,"type",mxType);
    }

    mxSetField(mxData,40,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,41,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,41,"type",mxType);
    }

    mxSetField(mxData,41,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,42,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,42,"type",mxType);
    }

    mxSetField(mxData,42,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,43,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,43,"type",mxType);
    }

    mxSetField(mxData,43,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,44,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,44,"type",mxType);
    }

    mxSetField(mxData,44,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,45,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,45,"type",mxType);
    }

    mxSetField(mxData,45,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,46,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,46,"type",mxType);
    }

    mxSetField(mxData,46,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,47,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,47,"type",mxType);
    }

    mxSetField(mxData,47,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,48,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,48,"type",mxType);
    }

    mxSetField(mxData,48,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,49,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,49,"type",mxType);
    }

    mxSetField(mxData,49,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,50,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,50,"type",mxType);
    }

    mxSetField(mxData,50,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,51,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,51,"type",mxType);
    }

    mxSetField(mxData,51,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,52,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,52,"type",mxType);
    }

    mxSetField(mxData,52,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,53,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,53,"type",mxType);
    }

    mxSetField(mxData,53,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,54,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,54,"type",mxType);
    }

    mxSetField(mxData,54,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,55,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,55,"type",mxType);
    }

    mxSetField(mxData,55,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,56,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,56,"type",mxType);
    }

    mxSetField(mxData,56,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,57,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,57,"type",mxType);
    }

    mxSetField(mxData,57,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,58,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,58,"type",mxType);
    }

    mxSetField(mxData,58,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,59,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,59,"type",mxType);
    }

    mxSetField(mxData,59,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,60,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,60,"type",mxType);
    }

    mxSetField(mxData,60,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,61,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,61,"type",mxType);
    }

    mxSetField(mxData,61,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,62,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,62,"type",mxType);
    }

    mxSetField(mxData,62,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,63,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(1));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,63,"type",mxType);
    }

    mxSetField(mxData,63,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,64,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,64,"type",mxType);
    }

    mxSetField(mxData,64,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,65,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,65,"type",mxType);
    }

    mxSetField(mxData,65,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,66,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,66,"type",mxType);
    }

    mxSetField(mxData,66,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,67,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,67,"type",mxType);
    }

    mxSetField(mxData,67,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,68,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,68,"type",mxType);
    }

    mxSetField(mxData,68,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(50);
      pr[1] = (double)(1);
      mxSetField(mxData,69,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,69,"type",mxType);
    }

    mxSetField(mxData,69,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,70,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,70,"type",mxType);
    }

    mxSetField(mxData,70,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(1);
      mxSetField(mxData,71,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,71,"type",mxType);
    }

    mxSetField(mxData,71,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,72,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,72,"type",mxType);
    }

    mxSetField(mxData,72,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(4);
      pr[1] = (double)(1);
      mxSetField(mxData,73,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,73,"type",mxType);
    }

    mxSetField(mxData,73,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxData);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,7,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(50);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(2);
      pr[1] = (double)(1);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(5);
      pr[1] = (double)(1);
      mxSetField(mxData,6,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(1));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,6,"type",mxType);
    }

    mxSetField(mxData,6,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  {
    mxArray* mxPostCodegenInfo = sf_c3_mpclib_get_post_codegen_info();
    mxSetField(mxAutoinheritanceInfo,0,"postCodegenInfo",mxPostCodegenInfo);
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c3_mpclib_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c3_mpclib_jit_fallback_info(void)
{
  const char *infoFields[] = { "fallbackType", "fallbackReason",
    "incompatibleSymbol", };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 3, infoFields);
  mxArray *fallbackReason = mxCreateString("feature_off");
  mxArray *incompatibleSymbol = mxCreateString("");
  mxArray *fallbackType = mxCreateString("early");
  mxSetField(mxInfo, 0, infoFields[0], fallbackType);
  mxSetField(mxInfo, 0, infoFields[1], fallbackReason);
  mxSetField(mxInfo, 0, infoFields[2], incompatibleSymbol);
  return mxInfo;
}

mxArray *sf_c3_mpclib_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

mxArray* sf_c3_mpclib_get_post_codegen_info(void)
{
  const char* fieldNames[] = { "exportedFunctionsUsedByThisChart",
    "exportedFunctionsChecksum" };

  mwSize dims[2] = { 1, 1 };

  mxArray* mxPostCodegenInfo = mxCreateStructArray(2, dims, sizeof(fieldNames)/
    sizeof(fieldNames[0]), fieldNames);

  {
    mxArray* mxExportedFunctionsChecksum = mxCreateString("");
    mwSize exp_dims[2] = { 0, 1 };

    mxArray* mxExportedFunctionsUsedByThisChart = mxCreateCellArray(2, exp_dims);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsUsedByThisChart",
               mxExportedFunctionsUsedByThisChart);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsChecksum",
               mxExportedFunctionsChecksum);
  }

  return mxPostCodegenInfo;
}

static const mxArray *sf_get_sim_state_info_c3_mpclib(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x8'type','srcId','name','auxInfo'{{M[1],M[20],T\"cost\",},{M[1],M[145],T\"iAout\",},{M[1],M[126],T\"status\",},{M[1],M[19],T\"u\",},{M[1],M[21],T\"useq\",},{M[1],M[177],T\"xest\",},{M[1],M[174],T\"xk1\",},{M[8],M[0],T\"is_active_c3_mpclib\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 8, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c3_mpclib_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc3_mpclibInstanceStruct *chartInstance;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    chartInstance = (SFc3_mpclibInstanceStruct *) chartInfo->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _mpclibMachineNumber_,
           3,
           1,
           1,
           0,
           98,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           (void *)S);

        /* Each instance must initialize its own list of scripts */
        init_script_number_translation(_mpclibMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_mpclibMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _mpclibMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,2,0,1,"xk1");
          _SFD_SET_DATA_PROPS(1,1,1,0,"xk");
          _SFD_SET_DATA_PROPS(2,1,1,0,"old_u");
          _SFD_SET_DATA_PROPS(3,1,1,0,"ym");
          _SFD_SET_DATA_PROPS(4,1,1,0,"ref");
          _SFD_SET_DATA_PROPS(5,1,1,0,"md");
          _SFD_SET_DATA_PROPS(6,1,1,0,"umin");
          _SFD_SET_DATA_PROPS(7,1,1,0,"umax");
          _SFD_SET_DATA_PROPS(8,1,1,0,"ymin");
          _SFD_SET_DATA_PROPS(9,1,1,0,"ymax");
          _SFD_SET_DATA_PROPS(10,1,1,0,"switch_in");
          _SFD_SET_DATA_PROPS(11,1,1,0,"ext_mv");
          _SFD_SET_DATA_PROPS(12,1,1,0,"MVtarget");
          _SFD_SET_DATA_PROPS(13,10,0,0,"isQP");
          _SFD_SET_DATA_PROPS(14,10,0,0,"nx");
          _SFD_SET_DATA_PROPS(15,10,0,0,"nu");
          _SFD_SET_DATA_PROPS(16,10,0,0,"ny");
          _SFD_SET_DATA_PROPS(17,10,0,0,"degrees");
          _SFD_SET_DATA_PROPS(18,10,0,0,"Hinv");
          _SFD_SET_DATA_PROPS(19,10,0,0,"Kx");
          _SFD_SET_DATA_PROPS(20,10,0,0,"Ku1");
          _SFD_SET_DATA_PROPS(21,10,0,0,"Kut");
          _SFD_SET_DATA_PROPS(22,10,0,0,"Kr");
          _SFD_SET_DATA_PROPS(23,10,0,0,"Kv");
          _SFD_SET_DATA_PROPS(24,10,0,0,"Mlim");
          _SFD_SET_DATA_PROPS(25,10,0,0,"Mx");
          _SFD_SET_DATA_PROPS(26,10,0,0,"Mu1");
          _SFD_SET_DATA_PROPS(27,10,0,0,"Mv");
          _SFD_SET_DATA_PROPS(28,10,0,0,"z_degrees");
          _SFD_SET_DATA_PROPS(29,10,0,0,"utarget");
          _SFD_SET_DATA_PROPS(30,10,0,0,"p");
          _SFD_SET_DATA_PROPS(31,10,0,0,"uoff");
          _SFD_SET_DATA_PROPS(32,10,0,0,"voff");
          _SFD_SET_DATA_PROPS(33,10,0,0,"yoff");
          _SFD_SET_DATA_PROPS(34,10,0,0,"maxiter");
          _SFD_SET_DATA_PROPS(35,10,0,0,"nxQP");
          _SFD_SET_DATA_PROPS(36,10,0,0,"openloopflag");
          _SFD_SET_DATA_PROPS(37,10,0,0,"lims_inport");
          _SFD_SET_DATA_PROPS(38,10,0,0,"no_umin");
          _SFD_SET_DATA_PROPS(39,10,0,0,"no_umax");
          _SFD_SET_DATA_PROPS(40,10,0,0,"no_ymin");
          _SFD_SET_DATA_PROPS(41,10,0,0,"no_ymax");
          _SFD_SET_DATA_PROPS(42,10,0,0,"switch_inport");
          _SFD_SET_DATA_PROPS(43,10,0,0,"no_switch");
          _SFD_SET_DATA_PROPS(44,10,0,0,"enable_value");
          _SFD_SET_DATA_PROPS(45,10,0,0,"return_cost");
          _SFD_SET_DATA_PROPS(46,10,0,0,"H");
          _SFD_SET_DATA_PROPS(47,10,0,0,"return_sequence");
          _SFD_SET_DATA_PROPS(48,10,0,0,"Linv");
          _SFD_SET_DATA_PROPS(49,10,0,0,"Ac");
          _SFD_SET_DATA_PROPS(50,1,1,0,"ywt");
          _SFD_SET_DATA_PROPS(51,1,1,0,"uwt");
          _SFD_SET_DATA_PROPS(52,1,1,0,"duwt");
          _SFD_SET_DATA_PROPS(53,1,1,0,"rhoeps");
          _SFD_SET_DATA_PROPS(54,1,1,0,"iA");
          _SFD_SET_DATA_PROPS(55,2,0,1,"u");
          _SFD_SET_DATA_PROPS(56,2,0,1,"cost");
          _SFD_SET_DATA_PROPS(57,2,0,1,"useq");
          _SFD_SET_DATA_PROPS(58,2,0,1,"status");
          _SFD_SET_DATA_PROPS(59,2,0,1,"xest");
          _SFD_SET_DATA_PROPS(60,10,0,0,"no_ywt");
          _SFD_SET_DATA_PROPS(61,10,0,0,"no_uwt");
          _SFD_SET_DATA_PROPS(62,10,0,0,"no_duwt");
          _SFD_SET_DATA_PROPS(63,10,0,0,"no_rhoeps");
          _SFD_SET_DATA_PROPS(64,10,0,0,"Wy");
          _SFD_SET_DATA_PROPS(65,10,0,0,"Wdu");
          _SFD_SET_DATA_PROPS(66,10,0,0,"Jm");
          _SFD_SET_DATA_PROPS(67,10,0,0,"SuJm");
          _SFD_SET_DATA_PROPS(68,10,0,0,"Su1");
          _SFD_SET_DATA_PROPS(69,10,0,0,"Sx");
          _SFD_SET_DATA_PROPS(70,10,0,0,"Hv");
          _SFD_SET_DATA_PROPS(71,10,0,0,"Wu");
          _SFD_SET_DATA_PROPS(72,10,0,0,"I1");
          _SFD_SET_DATA_PROPS(73,2,0,1,"iAout");
          _SFD_SET_DATA_PROPS(74,10,0,0,"A");
          _SFD_SET_DATA_PROPS(75,10,0,0,"Bu");
          _SFD_SET_DATA_PROPS(76,10,0,0,"Bv");
          _SFD_SET_DATA_PROPS(77,10,0,0,"C");
          _SFD_SET_DATA_PROPS(78,10,0,0,"Dv");
          _SFD_SET_DATA_PROPS(79,10,0,0,"Mrows");
          _SFD_SET_DATA_PROPS(80,10,0,0,"nCC");
          _SFD_SET_DATA_PROPS(81,10,0,0,"Ecc");
          _SFD_SET_DATA_PROPS(82,10,0,0,"Fcc");
          _SFD_SET_DATA_PROPS(83,10,0,0,"Scc");
          _SFD_SET_DATA_PROPS(84,10,0,0,"Gcc");
          _SFD_SET_DATA_PROPS(85,10,0,0,"nv");
          _SFD_SET_DATA_PROPS(86,10,0,0,"no_md");
          _SFD_SET_DATA_PROPS(87,10,0,0,"no_ref");
          _SFD_SET_DATA_PROPS(88,10,0,0,"no_uref");
          _SFD_SET_DATA_PROPS(89,10,0,0,"no_mv");
          _SFD_SET_DATA_PROPS(90,10,0,0,"Rscale");
          _SFD_SET_DATA_PROPS(91,10,0,0,"MDscale");
          _SFD_SET_DATA_PROPS(92,10,0,0,"myindex");
          _SFD_SET_DATA_PROPS(93,10,0,0,"myoff");
          _SFD_SET_DATA_PROPS(94,10,0,0,"xoff");
          _SFD_SET_DATA_PROPS(95,10,0,0,"CustomEstimation");
          _SFD_SET_DATA_PROPS(96,10,0,0,"M");
          _SFD_SET_DATA_PROPS(97,10,0,0,"L");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,10,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,6128);
        _SFD_CV_INIT_EML_IF(0,1,0,1329,1344,-1,1652);
        _SFD_CV_INIT_EML_IF(0,1,1,1714,1729,2095,2264);
        _SFD_CV_INIT_EML_IF(0,1,2,1814,1825,1952,2094);
        _SFD_CV_INIT_EML_IF(0,1,3,2390,2403,2440,2546);
        _SFD_CV_INIT_EML_IF(0,1,4,2596,2620,2667,3068);
        _SFD_CV_INIT_EML_IF(0,1,5,3149,3164,3219,3437);
        _SFD_CV_INIT_EML_IF(0,1,6,3439,3454,5012,5785);
        _SFD_CV_INIT_EML_IF(0,1,7,3539,3550,4277,5011);
        _SFD_CV_INIT_EML_IF(0,1,8,5787,5811,5826,5954);
        _SFD_CV_INIT_EML_IF(0,1,9,6012,6036,6095,6127);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,0,2393,2403,-1,0);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,1,2599,2620,-1,0);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,2,3152,3164,-1,0);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,3,5790,5811,-1,0);
        _SFD_CV_INIT_EML_RELATIONAL(0,1,4,6015,6036,-1,0);

        {
          unsigned int dimVector[1];
          dimVector[0]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)
            c3_b_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(11,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(12,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(13,SF_UINT8,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_j_sf_marshallOut,(MexInFcnForType)c3_f_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(14,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(15,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(16,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(17,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[2];
          dimVector[0]= 4;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(18,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_i_sf_marshallOut,(MexInFcnForType)
            c3_h_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 2;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(19,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_m_sf_marshallOut,(MexInFcnForType)
            c3_i_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(20,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_h_sf_marshallOut,(MexInFcnForType)
            c3_j_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 50;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(21,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_l_sf_marshallOut,(MexInFcnForType)
            c3_k_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 50;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(22,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_l_sf_marshallOut,(MexInFcnForType)
            c3_k_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 51;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(23,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_k_sf_marshallOut,(MexInFcnForType)
            c3_l_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(24,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(25,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_f_sf_marshallOut,(MexInFcnForType)
            c3_m_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(26,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(27,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(28,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_e_sf_marshallOut,(MexInFcnForType)
            c3_n_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 50;
          _SFD_SET_DATA_COMPILED_PROPS(29,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_d_sf_marshallOut,(MexInFcnForType)
            c3_d_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(30,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(31,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(32,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(33,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(34,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(35,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(36,SF_UINT8,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_j_sf_marshallOut,(MexInFcnForType)c3_f_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(37,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(38,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(39,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(40,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(41,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(42,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(43,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(44,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(45,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[2];
          dimVector[0]= 4;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(46,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_i_sf_marshallOut,(MexInFcnForType)
            c3_h_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(47,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[2];
          dimVector[0]= 4;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(48,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_i_sf_marshallOut,(MexInFcnForType)
            c3_h_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(49,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_h_sf_marshallOut,(MexInFcnForType)
            c3_j_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(50,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(51,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(52,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(53,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 5;
          _SFD_SET_DATA_COMPILED_PROPS(54,SF_UINT8,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(55,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(56,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 50;
          _SFD_SET_DATA_COMPILED_PROPS(57,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_d_sf_marshallOut,(MexInFcnForType)
            c3_d_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(58,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(59,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)
            c3_b_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(60,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(61,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(62,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(63,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(64,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(65,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(66,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(67,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(68,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(69,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(70,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(71,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 50;
          _SFD_SET_DATA_COMPILED_PROPS(72,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_d_sf_marshallOut,(MexInFcnForType)
            c3_d_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 5;
          _SFD_SET_DATA_COMPILED_PROPS(73,SF_UINT8,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_sf_marshallOut,(MexInFcnForType)
            c3_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 2;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(74,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_g_sf_marshallOut,(MexInFcnForType)
            c3_o_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(75,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)
            c3_b_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(76,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)
            c3_b_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(77,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_f_sf_marshallOut,(MexInFcnForType)
            c3_m_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(78,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 4;
          _SFD_SET_DATA_COMPILED_PROPS(79,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_e_sf_marshallOut,(MexInFcnForType)
            c3_n_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(80,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(81,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(82,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(83,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(84,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(85,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(86,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(87,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(88,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(89,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(90,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(91,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(92,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(93,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(94,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)
            c3_b_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(95,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c3_c_sf_marshallOut,(MexInFcnForType)c3_c_sf_marshallIn);

        {
          unsigned int dimVector[1];
          dimVector[0]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(96,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)
            c3_b_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 2;
          _SFD_SET_DATA_COMPILED_PROPS(97,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c3_b_sf_marshallOut,(MexInFcnForType)
            c3_b_sf_marshallIn);
        }

        _SFD_SET_DATA_VALUE_PTR(0U, *chartInstance->c3_xk1);
        _SFD_SET_DATA_VALUE_PTR(1U, *chartInstance->c3_xk);
        _SFD_SET_DATA_VALUE_PTR(2U, chartInstance->c3_old_u);
        _SFD_SET_DATA_VALUE_PTR(3U, chartInstance->c3_ym);
        _SFD_SET_DATA_VALUE_PTR(4U, chartInstance->c3_ref);
        _SFD_SET_DATA_VALUE_PTR(5U, chartInstance->c3_md);
        _SFD_SET_DATA_VALUE_PTR(6U, chartInstance->c3_umin);
        _SFD_SET_DATA_VALUE_PTR(7U, chartInstance->c3_umax);
        _SFD_SET_DATA_VALUE_PTR(8U, chartInstance->c3_ymin);
        _SFD_SET_DATA_VALUE_PTR(9U, chartInstance->c3_ymax);
        _SFD_SET_DATA_VALUE_PTR(10U, chartInstance->c3_switch_in);
        _SFD_SET_DATA_VALUE_PTR(11U, chartInstance->c3_ext_mv);
        _SFD_SET_DATA_VALUE_PTR(12U, chartInstance->c3_MVtarget);
        _SFD_SET_DATA_VALUE_PTR(13U, &chartInstance->c3_isQP);
        _SFD_SET_DATA_VALUE_PTR(14U, &chartInstance->c3_nx);
        _SFD_SET_DATA_VALUE_PTR(15U, &chartInstance->c3_nu);
        _SFD_SET_DATA_VALUE_PTR(16U, &chartInstance->c3_ny);
        _SFD_SET_DATA_VALUE_PTR(17U, &chartInstance->c3_degrees);
        _SFD_SET_DATA_VALUE_PTR(18U, chartInstance->c3_Hinv);
        _SFD_SET_DATA_VALUE_PTR(19U, chartInstance->c3_Kx);
        _SFD_SET_DATA_VALUE_PTR(20U, chartInstance->c3_Ku1);
        _SFD_SET_DATA_VALUE_PTR(21U, chartInstance->c3_Kut);
        _SFD_SET_DATA_VALUE_PTR(22U, chartInstance->c3_Kr);
        _SFD_SET_DATA_VALUE_PTR(23U, chartInstance->c3_Kv);
        _SFD_SET_DATA_VALUE_PTR(24U, &chartInstance->c3_Mlim);
        _SFD_SET_DATA_VALUE_PTR(25U, chartInstance->c3_Mx);
        _SFD_SET_DATA_VALUE_PTR(26U, &chartInstance->c3_Mu1);
        _SFD_SET_DATA_VALUE_PTR(27U, &chartInstance->c3_Mv);
        _SFD_SET_DATA_VALUE_PTR(28U, chartInstance->c3_z_degrees);
        _SFD_SET_DATA_VALUE_PTR(29U, chartInstance->c3_utarget);
        _SFD_SET_DATA_VALUE_PTR(30U, &chartInstance->c3_p);
        _SFD_SET_DATA_VALUE_PTR(31U, &chartInstance->c3_uoff);
        _SFD_SET_DATA_VALUE_PTR(32U, &chartInstance->c3_voff);
        _SFD_SET_DATA_VALUE_PTR(33U, &chartInstance->c3_yoff);
        _SFD_SET_DATA_VALUE_PTR(34U, &chartInstance->c3_maxiter);
        _SFD_SET_DATA_VALUE_PTR(35U, &chartInstance->c3_nxQP);
        _SFD_SET_DATA_VALUE_PTR(36U, &chartInstance->c3_openloopflag);
        _SFD_SET_DATA_VALUE_PTR(37U, &chartInstance->c3_lims_inport);
        _SFD_SET_DATA_VALUE_PTR(38U, &chartInstance->c3_no_umin);
        _SFD_SET_DATA_VALUE_PTR(39U, &chartInstance->c3_no_umax);
        _SFD_SET_DATA_VALUE_PTR(40U, &chartInstance->c3_no_ymin);
        _SFD_SET_DATA_VALUE_PTR(41U, &chartInstance->c3_no_ymax);
        _SFD_SET_DATA_VALUE_PTR(42U, &chartInstance->c3_switch_inport);
        _SFD_SET_DATA_VALUE_PTR(43U, &chartInstance->c3_no_switch);
        _SFD_SET_DATA_VALUE_PTR(44U, &chartInstance->c3_enable_value);
        _SFD_SET_DATA_VALUE_PTR(45U, &chartInstance->c3_return_cost);
        _SFD_SET_DATA_VALUE_PTR(46U, chartInstance->c3_H);
        _SFD_SET_DATA_VALUE_PTR(47U, &chartInstance->c3_return_sequence);
        _SFD_SET_DATA_VALUE_PTR(48U, chartInstance->c3_Linv);
        _SFD_SET_DATA_VALUE_PTR(49U, chartInstance->c3_Ac);
        _SFD_SET_DATA_VALUE_PTR(50U, chartInstance->c3_ywt);
        _SFD_SET_DATA_VALUE_PTR(51U, chartInstance->c3_uwt);
        _SFD_SET_DATA_VALUE_PTR(52U, chartInstance->c3_duwt);
        _SFD_SET_DATA_VALUE_PTR(53U, chartInstance->c3_rhoeps);
        _SFD_SET_DATA_VALUE_PTR(54U, *chartInstance->c3_iA);
        _SFD_SET_DATA_VALUE_PTR(55U, chartInstance->c3_u);
        _SFD_SET_DATA_VALUE_PTR(56U, chartInstance->c3_cost);
        _SFD_SET_DATA_VALUE_PTR(57U, *chartInstance->c3_useq);
        _SFD_SET_DATA_VALUE_PTR(58U, chartInstance->c3_status);
        _SFD_SET_DATA_VALUE_PTR(59U, *chartInstance->c3_xest);
        _SFD_SET_DATA_VALUE_PTR(60U, &chartInstance->c3_no_ywt);
        _SFD_SET_DATA_VALUE_PTR(61U, &chartInstance->c3_no_uwt);
        _SFD_SET_DATA_VALUE_PTR(62U, &chartInstance->c3_no_duwt);
        _SFD_SET_DATA_VALUE_PTR(63U, &chartInstance->c3_no_rhoeps);
        _SFD_SET_DATA_VALUE_PTR(64U, &chartInstance->c3_Wy);
        _SFD_SET_DATA_VALUE_PTR(65U, &chartInstance->c3_Wdu);
        _SFD_SET_DATA_VALUE_PTR(66U, &chartInstance->c3_Jm);
        _SFD_SET_DATA_VALUE_PTR(67U, &chartInstance->c3_SuJm);
        _SFD_SET_DATA_VALUE_PTR(68U, &chartInstance->c3_Su1);
        _SFD_SET_DATA_VALUE_PTR(69U, &chartInstance->c3_Sx);
        _SFD_SET_DATA_VALUE_PTR(70U, &chartInstance->c3_Hv);
        _SFD_SET_DATA_VALUE_PTR(71U, &chartInstance->c3_Wu);
        _SFD_SET_DATA_VALUE_PTR(72U, chartInstance->c3_I1);
        _SFD_SET_DATA_VALUE_PTR(73U, *chartInstance->c3_iAout);
        _SFD_SET_DATA_VALUE_PTR(74U, chartInstance->c3_A);
        _SFD_SET_DATA_VALUE_PTR(75U, chartInstance->c3_Bu);
        _SFD_SET_DATA_VALUE_PTR(76U, chartInstance->c3_Bv);
        _SFD_SET_DATA_VALUE_PTR(77U, chartInstance->c3_C);
        _SFD_SET_DATA_VALUE_PTR(78U, &chartInstance->c3_Dv);
        _SFD_SET_DATA_VALUE_PTR(79U, chartInstance->c3_Mrows);
        _SFD_SET_DATA_VALUE_PTR(80U, &chartInstance->c3_nCC);
        _SFD_SET_DATA_VALUE_PTR(81U, &chartInstance->c3_Ecc);
        _SFD_SET_DATA_VALUE_PTR(82U, &chartInstance->c3_Fcc);
        _SFD_SET_DATA_VALUE_PTR(83U, &chartInstance->c3_Scc);
        _SFD_SET_DATA_VALUE_PTR(84U, &chartInstance->c3_Gcc);
        _SFD_SET_DATA_VALUE_PTR(85U, &chartInstance->c3_nv);
        _SFD_SET_DATA_VALUE_PTR(86U, &chartInstance->c3_no_md);
        _SFD_SET_DATA_VALUE_PTR(87U, &chartInstance->c3_no_ref);
        _SFD_SET_DATA_VALUE_PTR(88U, &chartInstance->c3_no_uref);
        _SFD_SET_DATA_VALUE_PTR(89U, &chartInstance->c3_no_mv);
        _SFD_SET_DATA_VALUE_PTR(90U, &chartInstance->c3_Rscale);
        _SFD_SET_DATA_VALUE_PTR(91U, &chartInstance->c3_MDscale);
        _SFD_SET_DATA_VALUE_PTR(92U, &chartInstance->c3_myindex);
        _SFD_SET_DATA_VALUE_PTR(93U, &chartInstance->c3_myoff);
        _SFD_SET_DATA_VALUE_PTR(94U, chartInstance->c3_xoff);
        _SFD_SET_DATA_VALUE_PTR(95U, &chartInstance->c3_CustomEstimation);
        _SFD_SET_DATA_VALUE_PTR(96U, chartInstance->c3_M);
        _SFD_SET_DATA_VALUE_PTR(97U, chartInstance->c3_L);
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _mpclibMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "s3HhfPOqr7Dw0SxT1pN0pD";
}

static void sf_opaque_initialize_c3_mpclib(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc3_mpclibInstanceStruct*) chartInstanceVar)->S,
    0);
  initialize_params_c3_mpclib((SFc3_mpclibInstanceStruct*) chartInstanceVar);
  initialize_c3_mpclib((SFc3_mpclibInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c3_mpclib(void *chartInstanceVar)
{
  enable_c3_mpclib((SFc3_mpclibInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c3_mpclib(void *chartInstanceVar)
{
  disable_c3_mpclib((SFc3_mpclibInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c3_mpclib(void *chartInstanceVar)
{
  sf_gateway_c3_mpclib((SFc3_mpclibInstanceStruct*) chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c3_mpclib(SimStruct* S)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  return get_sim_state_c3_mpclib((SFc3_mpclibInstanceStruct*)
    chartInfo->chartInstance);         /* raw sim ctx */
}

static void sf_opaque_set_sim_state_c3_mpclib(SimStruct* S, const mxArray *st)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  set_sim_state_c3_mpclib((SFc3_mpclibInstanceStruct*)chartInfo->chartInstance,
    st);
}

static void sf_opaque_terminate_c3_mpclib(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc3_mpclibInstanceStruct*) chartInstanceVar)->S;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_mpclib_optimization_info();
    }

    finalize_c3_mpclib((SFc3_mpclibInstanceStruct*) chartInstanceVar);
    utFree(chartInstanceVar);
    if (crtInfo != NULL) {
      utFree(crtInfo);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc3_mpclib((SFc3_mpclibInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c3_mpclib(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    initialize_params_c3_mpclib((SFc3_mpclibInstanceStruct*)
      (chartInfo->chartInstance));
  }
}

static void mdlSetWorkWidths_c3_mpclib(SimStruct *S)
{
  /* Actual parameters from chart:
     A Ac Bu Bv C CustomEstimation Dv Ecc Fcc Gcc H Hinv Hv I1 Jm Kr Ku1 Kut Kv Kx L Linv M MDscale Mlim Mrows Mu1 Mv Mx Rscale Scc Su1 SuJm Sx Wdu Wu Wy degrees enable_value isQP lims_inport maxiter myindex myoff nCC no_duwt no_md no_mv no_ref no_rhoeps no_switch no_umax no_umin no_uref no_uwt no_ymax no_ymin no_ywt nu nv nx nxQP ny openloopflag p return_cost return_sequence switch_inport uoff utarget voff xoff yoff z_degrees
   */
  const char_T *rtParamNames[] = { "A", "Ac", "Bu", "Bv", "C",
    "CustomEstimation", "Dv", "Ecc", "Fcc", "Gcc", "H", "Hinv", "Hv", "I1", "Jm",
    "Kr", "Ku1", "Kut", "Kv", "Kx", "L", "Linv", "M", "MDscale", "Mlim", "Mrows",
    "Mu1", "Mv", "Mx", "Rscale", "Scc", "Su1", "SuJm", "Sx", "Wdu", "Wu", "Wy",
    "degrees", "enable_value", "isQP", "lims_inport", "maxiter", "myindex",
    "myoff", "nCC", "no_duwt", "no_md", "no_mv", "no_ref", "no_rhoeps",
    "no_switch", "no_umax", "no_umin", "no_uref", "no_uwt", "no_ymax", "no_ymin",
    "no_ywt", "nu", "nv", "nx", "nxQP", "ny", "openloopflag", "p", "return_cost",
    "return_sequence", "switch_inport", "uoff", "utarget", "voff", "xoff",
    "yoff", "z_degrees" };

  ssSetNumRunTimeParams(S,ssGetSFcnParamsCount(S));

  /* registration for A*/
  ssRegDlgParamAsRunTimeParam(S, 0, 0, rtParamNames[0], SS_DOUBLE);

  /* registration for Ac*/
  ssRegDlgParamAsRunTimeParam(S, 1, 1, rtParamNames[1], SS_DOUBLE);

  /* registration for Bu*/
  ssRegDlgParamAsRunTimeParam(S, 2, 2, rtParamNames[2], SS_DOUBLE);

  /* registration for Bv*/
  ssRegDlgParamAsRunTimeParam(S, 3, 3, rtParamNames[3], SS_DOUBLE);

  /* registration for C*/
  ssRegDlgParamAsRunTimeParam(S, 4, 4, rtParamNames[4], SS_DOUBLE);

  /* registration for CustomEstimation*/
  ssRegDlgParamAsRunTimeParam(S, 5, 5, rtParamNames[5], SS_DOUBLE);

  /* registration for Dv*/
  ssRegDlgParamAsRunTimeParam(S, 6, 6, rtParamNames[6], SS_DOUBLE);

  /* registration for Ecc*/
  ssRegDlgParamAsRunTimeParam(S, 7, 7, rtParamNames[7], SS_DOUBLE);

  /* registration for Fcc*/
  ssRegDlgParamAsRunTimeParam(S, 8, 8, rtParamNames[8], SS_DOUBLE);

  /* registration for Gcc*/
  ssRegDlgParamAsRunTimeParam(S, 9, 9, rtParamNames[9], SS_DOUBLE);

  /* registration for H*/
  ssRegDlgParamAsRunTimeParam(S, 10, 10, rtParamNames[10], SS_DOUBLE);

  /* registration for Hinv*/
  ssRegDlgParamAsRunTimeParam(S, 11, 11, rtParamNames[11], SS_DOUBLE);

  /* registration for Hv*/
  ssRegDlgParamAsRunTimeParam(S, 12, 12, rtParamNames[12], SS_DOUBLE);

  /* registration for I1*/
  ssRegDlgParamAsRunTimeParam(S, 13, 13, rtParamNames[13], SS_DOUBLE);

  /* registration for Jm*/
  ssRegDlgParamAsRunTimeParam(S, 14, 14, rtParamNames[14], SS_DOUBLE);

  /* registration for Kr*/
  ssRegDlgParamAsRunTimeParam(S, 15, 15, rtParamNames[15], SS_DOUBLE);

  /* registration for Ku1*/
  ssRegDlgParamAsRunTimeParam(S, 16, 16, rtParamNames[16], SS_DOUBLE);

  /* registration for Kut*/
  ssRegDlgParamAsRunTimeParam(S, 17, 17, rtParamNames[17], SS_DOUBLE);

  /* registration for Kv*/
  ssRegDlgParamAsRunTimeParam(S, 18, 18, rtParamNames[18], SS_DOUBLE);

  /* registration for Kx*/
  ssRegDlgParamAsRunTimeParam(S, 19, 19, rtParamNames[19], SS_DOUBLE);

  /* registration for L*/
  ssRegDlgParamAsRunTimeParam(S, 20, 20, rtParamNames[20], SS_DOUBLE);

  /* registration for Linv*/
  ssRegDlgParamAsRunTimeParam(S, 21, 21, rtParamNames[21], SS_DOUBLE);

  /* registration for M*/
  ssRegDlgParamAsRunTimeParam(S, 22, 22, rtParamNames[22], SS_DOUBLE);

  /* registration for MDscale*/
  ssRegDlgParamAsRunTimeParam(S, 23, 23, rtParamNames[23], SS_DOUBLE);

  /* registration for Mlim*/
  ssRegDlgParamAsRunTimeParam(S, 24, 24, rtParamNames[24], SS_DOUBLE);

  /* registration for Mrows*/
  ssRegDlgParamAsRunTimeParam(S, 25, 25, rtParamNames[25], SS_DOUBLE);

  /* registration for Mu1*/
  ssRegDlgParamAsRunTimeParam(S, 26, 26, rtParamNames[26], SS_DOUBLE);

  /* registration for Mv*/
  ssRegDlgParamAsRunTimeParam(S, 27, 27, rtParamNames[27], SS_DOUBLE);

  /* registration for Mx*/
  ssRegDlgParamAsRunTimeParam(S, 28, 28, rtParamNames[28], SS_DOUBLE);

  /* registration for Rscale*/
  ssRegDlgParamAsRunTimeParam(S, 29, 29, rtParamNames[29], SS_DOUBLE);

  /* registration for Scc*/
  ssRegDlgParamAsRunTimeParam(S, 30, 30, rtParamNames[30], SS_DOUBLE);

  /* registration for Su1*/
  ssRegDlgParamAsRunTimeParam(S, 31, 31, rtParamNames[31], SS_DOUBLE);

  /* registration for SuJm*/
  ssRegDlgParamAsRunTimeParam(S, 32, 32, rtParamNames[32], SS_DOUBLE);

  /* registration for Sx*/
  ssRegDlgParamAsRunTimeParam(S, 33, 33, rtParamNames[33], SS_DOUBLE);

  /* registration for Wdu*/
  ssRegDlgParamAsRunTimeParam(S, 34, 34, rtParamNames[34], SS_DOUBLE);

  /* registration for Wu*/
  ssRegDlgParamAsRunTimeParam(S, 35, 35, rtParamNames[35], SS_DOUBLE);

  /* registration for Wy*/
  ssRegDlgParamAsRunTimeParam(S, 36, 36, rtParamNames[36], SS_DOUBLE);

  /* registration for degrees*/
  ssRegDlgParamAsRunTimeParam(S, 37, 37, rtParamNames[37], SS_DOUBLE);

  /* registration for enable_value*/
  ssRegDlgParamAsRunTimeParam(S, 38, 38, rtParamNames[38], SS_DOUBLE);

  /* registration for isQP*/
  ssRegDlgParamAsRunTimeParam(S, 39, 39, rtParamNames[39], SS_BOOLEAN);

  /* registration for lims_inport*/
  ssRegDlgParamAsRunTimeParam(S, 40, 40, rtParamNames[40], SS_DOUBLE);

  /* registration for maxiter*/
  ssRegDlgParamAsRunTimeParam(S, 41, 41, rtParamNames[41], SS_DOUBLE);

  /* registration for myindex*/
  ssRegDlgParamAsRunTimeParam(S, 42, 42, rtParamNames[42], SS_DOUBLE);

  /* registration for myoff*/
  ssRegDlgParamAsRunTimeParam(S, 43, 43, rtParamNames[43], SS_DOUBLE);

  /* registration for nCC*/
  ssRegDlgParamAsRunTimeParam(S, 44, 44, rtParamNames[44], SS_DOUBLE);

  /* registration for no_duwt*/
  ssRegDlgParamAsRunTimeParam(S, 45, 45, rtParamNames[45], SS_DOUBLE);

  /* registration for no_md*/
  ssRegDlgParamAsRunTimeParam(S, 46, 46, rtParamNames[46], SS_DOUBLE);

  /* registration for no_mv*/
  ssRegDlgParamAsRunTimeParam(S, 47, 47, rtParamNames[47], SS_DOUBLE);

  /* registration for no_ref*/
  ssRegDlgParamAsRunTimeParam(S, 48, 48, rtParamNames[48], SS_DOUBLE);

  /* registration for no_rhoeps*/
  ssRegDlgParamAsRunTimeParam(S, 49, 49, rtParamNames[49], SS_DOUBLE);

  /* registration for no_switch*/
  ssRegDlgParamAsRunTimeParam(S, 50, 50, rtParamNames[50], SS_DOUBLE);

  /* registration for no_umax*/
  ssRegDlgParamAsRunTimeParam(S, 51, 51, rtParamNames[51], SS_DOUBLE);

  /* registration for no_umin*/
  ssRegDlgParamAsRunTimeParam(S, 52, 52, rtParamNames[52], SS_DOUBLE);

  /* registration for no_uref*/
  ssRegDlgParamAsRunTimeParam(S, 53, 53, rtParamNames[53], SS_DOUBLE);

  /* registration for no_uwt*/
  ssRegDlgParamAsRunTimeParam(S, 54, 54, rtParamNames[54], SS_DOUBLE);

  /* registration for no_ymax*/
  ssRegDlgParamAsRunTimeParam(S, 55, 55, rtParamNames[55], SS_DOUBLE);

  /* registration for no_ymin*/
  ssRegDlgParamAsRunTimeParam(S, 56, 56, rtParamNames[56], SS_DOUBLE);

  /* registration for no_ywt*/
  ssRegDlgParamAsRunTimeParam(S, 57, 57, rtParamNames[57], SS_DOUBLE);

  /* registration for nu*/
  ssRegDlgParamAsRunTimeParam(S, 58, 58, rtParamNames[58], SS_DOUBLE);

  /* registration for nv*/
  ssRegDlgParamAsRunTimeParam(S, 59, 59, rtParamNames[59], SS_DOUBLE);

  /* registration for nx*/
  ssRegDlgParamAsRunTimeParam(S, 60, 60, rtParamNames[60], SS_DOUBLE);

  /* registration for nxQP*/
  ssRegDlgParamAsRunTimeParam(S, 61, 61, rtParamNames[61], SS_DOUBLE);

  /* registration for ny*/
  ssRegDlgParamAsRunTimeParam(S, 62, 62, rtParamNames[62], SS_DOUBLE);

  /* registration for openloopflag*/
  ssRegDlgParamAsRunTimeParam(S, 63, 63, rtParamNames[63], SS_BOOLEAN);

  /* registration for p*/
  ssRegDlgParamAsRunTimeParam(S, 64, 64, rtParamNames[64], SS_DOUBLE);

  /* registration for return_cost*/
  ssRegDlgParamAsRunTimeParam(S, 65, 65, rtParamNames[65], SS_DOUBLE);

  /* registration for return_sequence*/
  ssRegDlgParamAsRunTimeParam(S, 66, 66, rtParamNames[66], SS_DOUBLE);

  /* registration for switch_inport*/
  ssRegDlgParamAsRunTimeParam(S, 67, 67, rtParamNames[67], SS_DOUBLE);

  /* registration for uoff*/
  ssRegDlgParamAsRunTimeParam(S, 68, 68, rtParamNames[68], SS_DOUBLE);

  /* registration for utarget*/
  ssRegDlgParamAsRunTimeParam(S, 69, 69, rtParamNames[69], SS_DOUBLE);

  /* registration for voff*/
  ssRegDlgParamAsRunTimeParam(S, 70, 70, rtParamNames[70], SS_DOUBLE);

  /* registration for xoff*/
  ssRegDlgParamAsRunTimeParam(S, 71, 71, rtParamNames[71], SS_DOUBLE);

  /* registration for yoff*/
  ssRegDlgParamAsRunTimeParam(S, 72, 72, rtParamNames[72], SS_DOUBLE);

  /* registration for z_degrees*/
  ssRegDlgParamAsRunTimeParam(S, 73, 73, rtParamNames[73], SS_DOUBLE);
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_mpclib_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,3);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(sf_get_instance_specialization(),
                infoStruct,3,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,3,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(sf_get_instance_specialization(),infoStruct,3);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 5, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 6, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 7, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 8, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 9, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 10, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 11, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 12, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 13, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 14, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 15, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 16, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,3,17);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,3,7);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=7; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 17; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,3);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(412001795U));
  ssSetChecksum1(S,(3018233730U));
  ssSetChecksum2(S,(1699523276U));
  ssSetChecksum3(S,(2672769794U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c3_mpclib(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c3_mpclib(SimStruct *S)
{
  SFc3_mpclibInstanceStruct *chartInstance;
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)utMalloc(sizeof
    (ChartRunTimeInfo));
  chartInstance = (SFc3_mpclibInstanceStruct *)utMalloc(sizeof
    (SFc3_mpclibInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc3_mpclibInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c3_mpclib;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c3_mpclib;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c3_mpclib;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c3_mpclib;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c3_mpclib;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c3_mpclib;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c3_mpclib;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c3_mpclib;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c3_mpclib;
  chartInstance->chartInfo.mdlStart = mdlStart_c3_mpclib;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c3_mpclib;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.callAtomicSubchartUserFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartAutoFcn = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  crtInfo->checksum = SF_RUNTIME_INFO_CHECKSUM;
  crtInfo->instanceInfo = (&(chartInstance->chartInfo));
  crtInfo->isJITEnabled = false;
  crtInfo->compiledInfo = NULL;
  ssSetUserData(S,(void *)(crtInfo));  /* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  init_simulink_io_address(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  sf_opaque_init_subchart_simstructs(chartInstance->chartInfo.chartInstance);
  chart_debug_initialization(S,1);
}

void c3_mpclib_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c3_mpclib(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c3_mpclib(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c3_mpclib(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c3_mpclib_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
