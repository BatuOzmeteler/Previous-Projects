function runtlccmd
% RUNTLCCMD - run tlc command (regenerate C code from .rtw file) for model mpc_v2
% This function will run the tlc command stored in the variable 
% "tlccmd" in tlccmd.mat, whose contents is as follows:
% 
% 	tlc
% 	-r
% 	E:\BATU OZMETELER\Senior Project\MPC\v2\mpc_v2_r2014.2_64_oe_2.6.0_r2016_1_rsim\mpc_v2.rtw
% 	c:\openecu\platform\2_6_0_r2016-1\openecu\rtw\c\openecu_grt_rsim\openecu.tlc
% 	-OE:\BATU OZMETELER\Senior Project\MPC\v2\mpc_v2_r2014.2_64_oe_2.6.0_r2016_1_rsim
% 	-Ic:\openecu\platform\2_6_0_r2016-1\openecu\rtw\c\openecu_grt_rsim
% 	-Ic:\openecu\platform\2_6_0_r2016-1\openecu\mex_r2014b_64
% 	-IE:\BATU OZMETELER\Senior Project\MPC\v2\mpc_v2_r2014.2_64_oe_2.6.0_r2016_1_rsim\tlc
% 	-IC:\Program Files\MATLAB\R2014b\rtw\c\tlc\mw
% 	-IC:\Program Files\MATLAB\R2014b\rtw\c\tlc\lib
% 	-IC:\Program Files\MATLAB\R2014b\rtw\c\tlc\blocks
% 	-IC:\Program Files\MATLAB\R2014b\rtw\c\tlc\fixpt
% 	-IC:\Program Files\MATLAB\R2014b\stateflow\c\tlc
% 	-aFoldNonRolledExpr=0
% 	-aInlineInvariantSignals=1
% 	-aInlineParameters=1
% 	-aLocalBlockOutputs=1
% 	-aRollThreshold=5
% 	-aForceBlockIOInitOptimize=0
% 	-aGenerateReport=0
% 	-aGenCodeOnly=0
% 	-aRTWVerbose=1
% 	-aIncludeHyperlinkInReport=0
% 	-aLaunchReport=0
% 	-aGenerateTraceInfo=0
% 	-aForceParamTrailComments=0
% 	-aGenerateComments=1
% 	-aIgnoreCustomStorageClasses=1
% 	-aIncHierarchyInIds=0
% 	-aMaxRTWIdLen=31
% 	-aShowEliminatedStatements=1
% 	-aIncDataTypeInIds=0
% 	-aInsertBlockDesc=0
% 	-aIgnoreTestpoints=0
% 	-aSimulinkBlockComments=1
% 	-aInlinedPrmAccess="Literals"
% 	-aTargetFcnLib="ansi_tfl_table_tmw.mat"
% 	-aLogVarNameModifier="rt_"
% 	-aGenerateFullHeader=1
% 	-aRSIM_SOLVER_SELECTION=1
% 	-aExtMode=0
% 	-aExtModeStaticAlloc=0
% 	-aExtModeStaticAllocSize=1000000
% 	-aExtModeTransport=0
% 	-aExtModeTesting=0
% 	-aRSIM_PARAMETER_LOADING=1
% 	-aRTWCAPISignals=0
% 	-aRTWCAPIParams=0
% 	-aAngularFunctionality=0
% 	-aMaxDDLength=31
% 	-aRereadBuildlist=1
% 	-aStackSize=8192
% 	-aGenImageSrecord=1
% 	-aGenImageHex=1
% 	-aGenImageVisionStrategy=1
% 	-aContIfVisionStrategyFails=0
% 	-aAsap2Naming="prefix_name"
% 	-aGenPlatformASAP=1
% 	-aGenGenericASAP=1
% 	-aGenVisionASAP=1
% 	-aGenASAPNeedFlashVolts=0
% 	-aGenIncaASAP=1
% 	-aGenCanapeASAP=1
% 	-aGenDtcDdes=0
% 	-aWorkspaceMapSizes=0
% 	-aGenOldFormatMaps=0
% 	-aCompilerName="GCC-4.7.3"
% 	-aecu_type="M250"
% 	-apart_number="01T-068276-000"
% 	-aissue_number="3"
% 	-ause_oe_data_objects=0
% 	-adisable_naming=0
% 	-aGenerateTraceInfo=0
% 	-aIgnoreTestpoints=0
% 	-aProtectedModelReferenceTarget=0
% 	-p10000

   disp('This function will be obsoleted in a future release.') 
   mdl = 'mpc_v2';

   sysopen = ~isempty(strmatch(mdl, find_system('type', 'block_diagram'), 'exact'));

   if ~sysopen

      disp([mfilename ': Error: model ' mdl ' is not open. Please open model ' mdl ' and then run ' mfilename ' again.']);

   else

      rtwprivate('rtwattic', 'setBuildDir', 'E:\BATU OZMETELER\Senior Project\MPC\v2\mpc_v2_r2014.2_64_oe_2.6.0_r2016_1_rsim');
      rtwprivate('ec_set_replacement_flag', 'mpc_v2');
      set_param('mpc_v2', 'RTWGenSharedUtilitiesWithoutAssert', 0);
      load tlccmd.mat;
      savedpwd = pwd;
      cd ..;
      coder.internal.ModelCodegenMgr.setInstance(modelCodegenMgr);
      tlccmd{end+1} = '-aSLCGUseRTWContext=0';
      feval(tlccmd{:});
      coder.internal.ModelCodegenMgr.setInstance([]);
      
      rtwprivate rtwattic clean;
      set_param('mpc_v2', 'RTWGenSharedUtilitiesWithoutAssert', -1);
      cd(savedpwd);

   end
