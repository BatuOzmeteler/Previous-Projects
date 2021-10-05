CC=gcc
CFLAGS= -I.

#_OBJ = main.o agentModel.o agentModel_data.o rt_logging.o  rt_nonfinite.o rtGetInf.cpp rtGetNaN.o
#ODIR=obj
#OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#DEPS = agentModel.h agentModel_private.h agentModel_types.h bultin_typeid_types.h defines.txt ext_work.h multiword_types.h rt_mxclassid.h rtmodel.h rtw_continuous.h rtw_extmode.h rtw_matlogging.h rtw_solver.h rtwtypes.h simstruct_types.h sl_sample_time_defs.h sl_types_def.h sysran_types.h tmwtypes.h agentModel_data.h rt_logging.h rt_nonfinite.h rtGetInf.h rtGetNaN.h
	
hellomake: main.cpp agentModel.cpp agentModel_data.cpp rt_logging.c rt_nonfinite.cpp rtGetInf.cpp rtGetNaN.cpp
	$(CC) -I/home/pi/agent -o agent main.cpp agentModel.cpp agentModel_data.cpp rt_logging.c rt_nonfinite.cpp rtGetInf.cpp rtGetNaN.cpp -lstdc++ -lm -lpthread -std=c++17
	
clean :
	rm agent OutputAgent.txt
