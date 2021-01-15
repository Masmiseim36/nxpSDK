copy ..\release\EAP_Simulator_W32_3_0_0_CS_DBE_EQNB_PREQND_LM_AVL_TE_VC_TG_PSA_LIMP_LIMR.exe ..\EAP_TestParam.exe
cd ..

call TalArgsTest\RunTest_Short.bat
call TalArgsTest\RunTest_Long.bat

pause