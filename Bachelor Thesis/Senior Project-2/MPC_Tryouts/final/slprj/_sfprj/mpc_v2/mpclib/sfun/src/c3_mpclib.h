#ifndef __c3_mpclib_h__
#define __c3_mpclib_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc3_mpclibInstanceStruct
#define typedef_SFc3_mpclibInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c3_sfEvent;
  boolean_T c3_isStable;
  boolean_T c3_doneDoubleBufferReInit;
  uint8_T c3_is_active_c3_mpclib;
  boolean_T c3_isQP;
  real_T c3_nx;
  real_T c3_nu;
  real_T c3_ny;
  real_T c3_degrees;
  real_T c3_Hinv[16];
  real_T c3_Kx[8];
  real_T c3_Ku1[4];
  real_T c3_Kut[200];
  real_T c3_Kr[200];
  real_T c3_Kv[204];
  real_T c3_Mlim;
  real_T c3_Mx[2];
  real_T c3_Mu1;
  real_T c3_Mv;
  real_T c3_z_degrees[4];
  real_T c3_utarget[50];
  real_T c3_p;
  real_T c3_uoff;
  real_T c3_voff;
  real_T c3_yoff;
  real_T c3_maxiter;
  real_T c3_nxQP;
  boolean_T c3_openloopflag;
  real_T c3_lims_inport;
  real_T c3_no_umin;
  real_T c3_no_umax;
  real_T c3_no_ymin;
  real_T c3_no_ymax;
  real_T c3_switch_inport;
  real_T c3_no_switch;
  real_T c3_enable_value;
  real_T c3_return_cost;
  real_T c3_H[16];
  real_T c3_return_sequence;
  real_T c3_Linv[16];
  real_T c3_Ac[4];
  real_T c3_no_ywt;
  real_T c3_no_uwt;
  real_T c3_no_duwt;
  real_T c3_no_rhoeps;
  real_T c3_Wy;
  real_T c3_Wdu;
  real_T c3_Jm;
  real_T c3_SuJm;
  real_T c3_Su1;
  real_T c3_Sx;
  real_T c3_Hv;
  real_T c3_Wu;
  real_T c3_I1[50];
  real_T c3_A[4];
  real_T c3_Bu[2];
  real_T c3_Bv[2];
  real_T c3_C[2];
  real_T c3_Dv;
  real_T c3_Mrows[4];
  real_T c3_nCC;
  real_T c3_Ecc;
  real_T c3_Fcc;
  real_T c3_Scc;
  real_T c3_Gcc;
  real_T c3_nv;
  real_T c3_no_md;
  real_T c3_no_ref;
  real_T c3_no_uref;
  real_T c3_no_mv;
  real_T c3_Rscale;
  real_T c3_MDscale;
  real_T c3_myindex;
  real_T c3_myoff;
  real_T c3_xoff[2];
  real_T c3_CustomEstimation;
  real_T c3_M[2];
  real_T c3_L[2];
  real_T (*c3_xk1)[2];
  real_T (*c3_xk)[2];
  real_T *c3_old_u;
  real_T *c3_ym;
  real_T *c3_ref;
  real_T *c3_md;
  real_T *c3_umin;
  real_T *c3_umax;
  real_T *c3_ymin;
  real_T *c3_ymax;
  real_T *c3_switch_in;
  real_T *c3_ext_mv;
  real_T *c3_MVtarget;
  real_T *c3_ywt;
  real_T *c3_uwt;
  real_T *c3_duwt;
  real_T *c3_rhoeps;
  boolean_T (*c3_iA)[5];
  real_T *c3_u;
  real_T *c3_cost;
  real_T (*c3_useq)[50];
  real_T *c3_status;
  real_T (*c3_xest)[2];
  boolean_T (*c3_iAout)[5];
} SFc3_mpclibInstanceStruct;

#endif                                 /*typedef_SFc3_mpclibInstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c3_mpclib_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c3_mpclib_get_check_sum(mxArray *plhs[]);
extern void c3_mpclib_method_dispatcher(SimStruct *S, int_T method, void *data);

#endif
