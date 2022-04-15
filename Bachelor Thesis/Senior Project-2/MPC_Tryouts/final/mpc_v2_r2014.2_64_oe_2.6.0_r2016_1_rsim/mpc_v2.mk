##############################################################################
##
##  Project:        OpenECU
##  Version:        $Id: mpc_v2.mk 46342 2015-05-06 19:29:09Z sborders $
##  File:           $URL: https://aetius.ps.local/svn/OpenECU/development/platform/tags/release-2.6.0-r2016-1/openecu/rtw/c/openecu_grt_rsim/mpc_v2.mk $
##  Description:    RTW template makefile for building an OpenECU model
##                  (specifically for R2014b).
##
##  Copyright (C) 2015 Pi Innovo
##  This document (program) contains proprietary information
##  which is the property of Pi Innovo
##  The contents of this document (program) must not be copied
##  or disclosed to a third party without the prior agreement
##  of an authorised officer of Pi Innovo
##
##  Notes:          This template makefile is automatically parsed by
##                  the RTW build procedure to create "<model>.mk"
##
##############################################################################

##============== RTW template portion ==========================================
##

#------------------------ Macros read by make_rtw ------------------------------
#
# The following macros are read by the Real-Time Workshop build procedure:
#
#  MAKECMD         - This is the command used to invoke the make utility
#  HOST            - What platform this template makefile is targeted for
#                    (i.e. PC or UNIX)
#  BUILD           - Invoke make from the Real-Time Workshop build procedure
#                    (yes/no)?
#  SYS_TARGET_FILE - Name of system target file.

MAKECMD =                       "%MATLAB%\bin\win64\gmake"
HOST =                          PC
BUILD =                         yes
SYS_TARGET_FILE =               openecu.tlc
COMPILER_TOOL_CHAIN =           default

#---------------------- Tokens expanded by make_rtw ----------------------------
#
# The following tokens, when wrapped with "|>" and "<|" are expanded by the
# Real-Time Workshop build procedure.
#
#  MODEL           - Name of the Simulink block diagram
#  MODULES         - Any additional generated source modules
#  MAKEFILE        - Name of makefile created from template makefile <model>.mk
#  MATLAB_ROOT     - Path to were MATLAB is installed.
#  MATLAB_BIN      - Path to MATLAB executable.
#  S_FUNCTIONS     - List of S-functions.
#  S_FUNCTIONS_LIB - List of S-functions libraries to link.
#  SOLVER          - Solver source file name
#  NUMST           - Number of sample times
#  TID01EQ         - yes (1) r no (0): Are sampling rates of continuous task
#                    (tid=0) and 1st discrete task equal.
#  NCSTATES        - Number of continuous states
#  BUILDARGS       - Options passed in at the command line.
#  MULTITASKING    - yes (1) or no (0): Is solver mode multitasking
#  EXT_MODE        - yes (1) or no (0): Build for external mode
#  ADD_INCLUDES    - series of include directories for s-functions etc.

MODEL                         = mpc_v2
MODULES                       = mpc_v2_data.c rtGetInf.c rtGetNaN.c rt_nonfinite.c rt_zcfcn.c 
MAKEFILE                      = mpc_v2.mk
MATLAB_ROOT                   = C:\Program Files\MATLAB\R2014b
MATLAB_BIN                    = C:\Program Files\MATLAB\R2014b\bin
S_FUNCTIONS                   = 
S_FUNCTIONS_LIB               = 
SOLVER                        = 
NUMST                         = 2
TID01EQ                       = 0
NCSTATES                      = 0
BUILDARGS                     =  RSIM_SOLVER_SELECTION=1 PCMATLABROOT="C:\\Program Files\\MATLAB\\R2014b" EXT_MODE=0 EXTMODE_STATIC_ALLOC=0 EXTMODE_STATIC_ALLOC_SIZE=1000000 EXTMODE_TRANSPORT=0 TMW_EXTMODE_TESTING=0 RSIM_PARAMETER_LOADING=1 MAX_DD_LENGTH=31 CONT_IF_VISION_STRATEGY_FAILS=0 MODELLIB=mpc_v2lib.a RELATIVE_PATH_TO_ANCHOR=.. MODELREF_TARGET_TYPE=NONE
MULTITASKING                  = 1
EXT_MODE                      = 0
GEN_IMAGE_SRECORD             = 1
GEN_IMAGE_HEX                 = 1
GEN_ASAP_GENERIC              = 1
GEN_ASAP_VISION               = 1
GEN_ASAP_INCA                 = 1
GEN_ASAP_CANAPE               = 1
GEN_DTC_DDES                  = 0
GEN_ASAP_OLD                  = 0
GEN_IMAGE_VISION_STRATEGY     = 1
GEN_ASAP_NEED_FLASH_VOLTS     = 0
ADD_INCLUDES                  =  E:\BATUOZ~1\SENIOR~1\MPC\v2\mpc_v2_r2014.2_64_oe_2.6.0_r2016_1_rsim  E:\BATUOZ~1\SENIOR~1\MPC\v2 

# Incomplete support for shared utilities

SHARED_SRC      = 
SHARED_SRC_DIR  = 
SHARED_BIN_DIR  = 
SHARED_LIB      = 

# Accept the alternative root and bin directories to resolve directory names with
# spaces.

ALT_MATLAB_ROOT               = C:\PROGRA~1\MATLAB\R2014b
ALT_MATLAB_BIN                = C:\PROGRA~1\MATLAB\R2014b\bin

#-- In the case when directory name contains space ---
ifneq ($(MATLAB_ROOT),$(ALT_MATLAB_ROOT))
MATLAB_ROOT := $(ALT_MATLAB_ROOT)
endif
ifneq ($(MATLAB_BIN),$(ALT_MATLAB_BIN))
MATLAB_BIN := $(ALT_MATLAB_BIN)
endif

##
##============== End of RTW template portion ===================================

GEN_RTW_LIBRARY = 0
PLAT_C_SRC      = rt_nonfinite.c
RTW_C_SRC       = rt_sim.c
MODEL_TUNE_SRC  = $(MODEL).c
USE_RTMODEL     = 0

include $(MODEL)_sup.mk
include $(PLATFORM_ROOT)\mk_model_$(COMPILER).mk

##==============================================================================
## EOF: mpc_v2.mk

