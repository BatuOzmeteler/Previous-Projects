#ifndef __c2_check2_h__
#define __c2_check2_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc2_check2InstanceStruct
#define typedef_SFc2_check2InstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c2_sfEvent;
  boolean_T c2_isStable;
  boolean_T c2_doneDoubleBufferReInit;
  uint8_T c2_is_active_c2_check2;
  real_T *c2_trq_req;
  real_T *c2_prev_speed;
  real_T *c2_next_speed;
  real_T (*c2_grade)[2946];
  real_T *c2_step;
} SFc2_check2InstanceStruct;

#endif                                 /*typedef_SFc2_check2InstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c2_check2_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c2_check2_get_check_sum(mxArray *plhs[]);
extern void c2_check2_method_dispatcher(SimStruct *S, int_T method, void *data);

#endif
