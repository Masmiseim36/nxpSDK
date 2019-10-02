/*******************************************************************************
*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause* 
*
****************************************************************************//*!
*
* @file   inner_Parameters.js
*
* @brief  Parameters tab engine
*
******************************************************************************/

/******************************************************************************
* List of functions
******************************************************************************
*  initLoadFormParamValues()
*  writeParametersHTMLOutput(prefix,xmlObject)
*  writeParametersHeaderOutput(str)
*  writeFMScalesHTMLOutput(prefix,xmlObject)
*  writeFMScalesHeaderOutput(str)
*  updateTab_Paramters() - callback function
*******************************************************************************/

/***************************************************************************//*!
/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID
* @param   
* @return 
* @remarks 
******************************************************************************/
 function initLoadFormParamValues()
 {
    // in basic mode, precalculate paramters
    if(getActiveMode()==0)
      basicModeCalcParam();
     
    // mandatory input parameters
    copyParent2InnerValById("Rs");
    copyParent2InnerValById("Ld");
    copyParent2InnerValById("Lq");
    copyParent2InnerValById("ke");         
    copyParent2InnerValById("pp");
    copyParent2InnerValById("J");
   
    copyParent2InnerValById("I_ph");
    copyParent2InnerValById("U_ph");
    copyParent2InnerValById("N_req");
    
    copyParent2InnerValById("I_max");
    copyParent2InnerValById("UDC_max");
    
    // precalculated or manualy added
    copyParent2InnerValById("UDC_trip");
    copyParent2InnerValById("UDC_under");
    copyParent2InnerValById("UDC_over");
    
    copyParent2InnerValById("N_over");
    
    copyParent2InnerValById("N_min");
    
    copyParent2InnerValById("N_max"); 
    //copyParent2InnerValById("E_max");
    copyParent2InnerValById("E_block");
    copyParent2InnerValById("E_block_per");
    //copyParent2InnerValById("kt");
    
    copyParent2InnerValById("ALIGN_U");
    copyParent2InnerValById("ALIGN_I");
    copyParent2InnerValById("ALIGN_T");
    
    
    if(testVarValue('Alignment','Voltage'))
    {
      document.getElementById("Volt_align").style.display = "";
      document.getElementById("Curr_align").style.display = "none";
    }
    else
    {
      document.getElementById("Volt_align").style.display = "none";
      document.getElementById("Curr_align").style.display = "";
    }
    // enable button enabling
    ReloadStoreButtonsOnOff(1);
    
    //calculate constants
    clickCalculateParam();
}

/***************************************************************************//*!
* @brief   Mark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function markUpdateParamField()
{
    /* DISABLED in current MCAT version */
    /*
    var obj = document.getElementById("Alignment").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';

    document.getElementById("ParamsUpdateFrm").title = "Red-legend constants will be updated on click";
    */
}

/***************************************************************************//*!
* @brief   UnMark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function unmarkUpdateParamField()
{
    var obj = document.getElementById("Alignment").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
}

/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcParam()
{
    var UDCB_max      = getParentHtmlValue("UDC_max");
    var ke            = getParentHtmlValue("ke");
    var N_req_max     = getParentHtmlValue("N_req");
    var I_nom         = getParentHtmlValue("I_ph");
    var U_nom         = getParentHtmlValue("U_ph");
    var Imax          = getParentHtmlValue("I_max");
    var pp            = getParentHtmlValue("pp");
    var J             = getParentHtmlValue("J");
    var Rs            = getParentHtmlValue("Rs");
    
    // calculated input parameters
    UDCB_trip = Math.round(UDCB_max*0.8*10)/10;
    UDCB_under = Math.round(UDCB_max*0.3*10)/10;
    UDCB_over = Math.round(UDCB_max*0.8*10)/10;
    N_max =   Math.round(1.1*N_req_max*10)/10;
    //E_scale = Math.round(1.1*ke*N_max*pp*2*Math.PI/60*100)/100;
    //Kt = Math.round(ke*Math.sqrt(2/3)*100000)/100000;
    //Kt = (0.5).toFixed(1);
    U_align = Math.round(U_nom*0.1*10)/10;
    I_align = Math.round(I_nom*0.1*10)/10;
    //T_align = Math.round(J*Rs/ke/Kt/2*100000)/100000;
    T_align = Math.round(0.8*100000)/100000;
    N_max_over = Math.round(N_max*0.95*10)/10;
    N_min_under = Math.round(N_req_max*0.05*10)/10;
    
    // replace and disable params
    switchParam2BasicMode("UDC_trip",UDCB_trip);
    switchParam2BasicMode("UDC_under",UDCB_under);
    switchParam2BasicMode("UDC_over",UDCB_over);
    switchParam2BasicMode("N_over",N_max_over);
    switchParam2BasicMode("N_min",N_min_under);
    
    switchParam2BasicMode("N_max",N_max);
    //switchParam2BasicMode("E_max",E_scale);
    //switchParam2BasicMode("kt",Kt);
    switchParam2BasicMode("ALIGN_U",U_align);
    switchParam2BasicMode("ALIGN_I",I_align);
    switchParam2BasicMode("ALIGN_T",T_align);
    
    // disable button FRM Update
//    document.getElementById("ParamsUpdateFrm").disabled = true;
//    document.getElementById("ParamsUpdateFrm").className = "fButtonsDisabled";

}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickCalculateParam()
{
    Align_volt        = getParentHtmlValue("ALIGN_U"); //need to be global var
    Align_cur         = getParentHtmlValue("ALIGN_I"); //need to be global var
    
    var CLOOP_Ts      = getParentHtmlValue("CLOOP_Ts");
    var SLOOP_Ts      = getParentHtmlValue("SLOOP_Ts");
    var Align_dur     = getParentHtmlValue("ALIGN_T");
    var Calib_dur     = getParentHtmlValue("CALIB_T");
    var Fault_dur     = getParentHtmlValue("FAULT_T");
    var Freewheel_dur = getParentHtmlValue("FREEWHEEL_T");      
    var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
    var UDCtrip       = getParentHtmlValue("UDC_trip"); 
    var Iphnom        = getParentHtmlValue("I_ph");
    var Motor_pp      = getParentHtmlValue("pp");
    var IIRxCoefsScaleType     = parent.document.getElementById("IIRxCoefsScale").innerText;
    var E_block   = getParentHtmlValue("E_block");

    SpeedAngScale = 60/(Motor_pp*2*Math.PI);

    Imax          = getParentHtmlValue("I_max");
    UDCmax        = getParentHtmlValue("UDC_max");
    Nmax          = getParentHtmlValue("N_max");
    Umax          = Math.round(UDCmax/UmaxCoeff*10)/10;
    Emax          = getParentHtmlValue("E_max");
    
    UDCunder      = getParentHtmlValue("UDC_under");
    UDCover       = getParentHtmlValue("UDC_over");
    Nover         = getParentHtmlValue("N_over");
    Nmin          = getParentHtmlValue("N_min");
    Nreqmax       = getParentHtmlValue("N_req");
      
    FM_Umax = Umax;

    Align_volt_sc = Math.round(Align_volt/Umax*1000000000000)/1000000000000;
    testFracValRange("Align_volt_sc",Align_volt_sc); 
    Align_cur_sc = Math.round(Align_cur/Imax*1000000000000)/1000000000000;
    testFracValRange("Align_cur_sc",Align_cur_sc);
    Align_dur_sc =  Math.round(Align_dur/CLOOP_Ts);
    Calib_dur_sc =  Math.round(Calib_dur/SLOOP_Ts);
    Fault_dur_sc =  Math.round(Fault_dur/SLOOP_Ts);
    Freewheel_dur_sc =  Math.round(Freewheel_dur/SLOOP_Ts);

    /* Blocked rotor fault treshold */
    E_block_sc = Math.round(E_block/Emax*1000000000000)/1000000000000;
    
    UDC_trip_sc = Math.round(UDCtrip/UDCmax*1000000000000)/1000000000000;
    UDC_under_sc = Math.round(UDCunder/UDCmax*1000000000000)/1000000000000;
    UDC_over_sc = Math.round(UDCover/UDCmax*1000000000000)/1000000000000;

    N_over_fl = Math.round(Nover/SpeedAngScale*10)/10;
    N_min_fl = Math.round(Nmin/SpeedAngScale*10)/10;
	N_max_fl = Math.round(Nmax/SpeedAngScale*10)/10;
	N_reqmax_fl = Math.round(Nreqmax/SpeedAngScale*10)/10;

    N_over_sc = Math.round(Nover/Nmax*1000000000000)/1000000000000;
    N_min_sc = Math.round(Nmin/Nmax*1000000000000)/1000000000000;
    testValRange("N_over",Nover,Nmin,Nmax);
    testValRange("N_under",N_min_sc,0,Nreqmax);
    
    N_req_max_sc = Math.round(Nreqmax/Nmax*1000000000000)/1000000000000;
    I_ph_nom_sc = Math.round(Iphnom/Imax*1000000000000)/1000000000000;
    FREQmax = Math.round(Nmax/60*Motor_pp);
    
    // UDC voltage IIR filter
    // Cut off frequency  = 100Hz
    UDCB_IIR_cutoff_freq = 100; 
    UDCB_IIR_B0_fl = Math.round((2*Math.PI*UDCB_IIR_cutoff_freq*CLOOP_Ts)/(2+(2*Math.PI*UDCB_IIR_cutoff_freq*CLOOP_Ts))*1000000000000)/1000000000000; 
    UDCB_IIR_B1_fl = Math.round((2*Math.PI*UDCB_IIR_cutoff_freq*CLOOP_Ts)/(2+(2*Math.PI*UDCB_IIR_cutoff_freq*CLOOP_Ts))*1000000000000)/1000000000000;
    UDCB_IIR_A1_fl = -Math.round((2*Math.PI*UDCB_IIR_cutoff_freq*CLOOP_Ts-2)/(2+(2*Math.PI*UDCB_IIR_cutoff_freq*CLOOP_Ts))*1000000000000)/1000000000000;
    UDCB_IIR_B0_out =  Math.round(UDCB_IIR_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    UDCB_IIR_B1_out =  Math.round(UDCB_IIR_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    UDCB_IIR_A1_out =  Math.round(UDCB_IIR_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    testFracValRange("UDCB_IIR_B0_out",UDCB_IIR_B0_out,1);
    testFracValRange("UDCB_IIR_B1_out",UDCB_IIR_B1_out,1);
    testFracValRange("UDCB_IIR_A1_out",UDCB_IIR_A1_out,1);   
    
     
    Motor_ppsc = Math.ceil(Math.log(Math.abs(Motor_pp+0.00))/Math.log(2)); 
    Motor_ppfr = Math.round(Motor_pp/Math.pow(2,Motor_ppsc)*1000000000000)/1000000000000; 
    
    TestRangeTrim("UDC_trip", 0, UDCmax-1, "V");
    TestRangeTrim("UDC_under", 0, UDCmax-1, "V");
    TestRangeTrim("UDC_over", 0, UDCmax-1, "V");

    // If PARAMETERS tab is active ******************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {
    
      setInnerHtmlValueAsText("MOTOR_PP",2,Motor_pp, Math.round(Motor_pp*1e12)/1e12);
          
      // write maximal scales to forms in Output File HTML page
      setInnerHtmlValueAsText("I_MAX",8,Imax, Imax);
      setInnerHtmlValueAsText("U_DCB_MAX",8,UDCmax, UDCmax);
      setInnerHtmlValueAsText("U_MAX",8,Umax, Umax);
      setInnerHtmlValueAsText("N_MAX",8,Nmax, N_max_fl);
      setInnerHtmlValueAsText("E_MAX",8,Emax, Emax);
      setInnerHtmlValueAsText("FREQ_MAX",8,FREQmax, FREQmax);
      
      setInnerHtmlValueAsText("U_DCB_TRIP",0,UDC_trip_sc, UDCtrip);
      setInnerHtmlValueAsText("U_DCB_UNDERVOLTAGE",0,UDC_under_sc, UDCunder);
      setInnerHtmlValueAsText("U_DCB_OVERVOLTAGE",0,UDC_over_sc, UDCover);
      setInnerHtmlValueAsText("N_OVERSPEED",0,N_over_sc, N_over_fl);
      setInnerHtmlValueAsText("N_MIN",0,N_min_sc, N_min_fl);
      
      setInnerHtmlValueAsText("N_NOM",0, N_req_max_sc, N_reqmax_fl);
      setInnerHtmlValueAsText("I_PH_NOM",0,I_ph_nom_sc,Iphnom);
      
      setInnerHtmlValueAsText("FM_FREQ_SCALE",8,FREQmax, FREQmax);
    
      // write values to forms in Output File Html page
      if(testVarValue('Alignment','Voltage'))
        setInnerHtmlValueAsText("ALIGN_VOLTAGE",0,Align_volt_sc, Align_volt);
      else
        setInnerHtmlValueAsText("ALIGN_CURRENT",0,Align_cur_sc, Align_cur);  
      
      setInnerHtmlValueAsText("ALIGN_DURATION",2,Align_dur_sc);
      setInnerHtmlValueAsText("CALIB_DURATION",2,Calib_dur_sc);
      setInnerHtmlValueAsText("FAULT_DURATION",2,Fault_dur_sc);
      setInnerHtmlValueAsText("FREEWHEEL_DURATION",2,Freewheel_dur_sc);  
      setInnerHtmlValueAsText("E_BLOCK_TRH",0,E_block_sc, E_block);
      setInnerHtmlValueAsText("E_BLOCK_PER",2,getParentHtmlValue("E_block_per"));
      
      // DCB voltage IIR filter
      setInnerHtmlValueAsText("UDCB_IIR_B0",0,UDCB_IIR_B0_out,UDCB_IIR_B0_fl);
      setInnerHtmlValueAsText("UDCB_IIR_B1",0,UDCB_IIR_B1_out,UDCB_IIR_B1_fl);
      setInnerHtmlValueAsText("UDCB_IIR_A1",0,UDCB_IIR_A1_out,UDCB_IIR_A1_fl);
      
    }  
}
/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickUpdateParamFM()
{
    if(!pcm.IsCommPortOpen())
    {
      alert("Communication is stopped.\nPress Ctrl+K to start the communication");
    }
    else
    {
    var errorArray = [];
   
    xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml"); 
    
    //calculate constants
    clickCalculateParam();
    
    if(testVarValue('Alignment','Voltage'))
      errorArray.push(UpdateFMVariable(xmlDoc,'Align_Voltage',Align_volt,Align_volt));
    else
      errorArray.push(UpdateFMVariable(xmlDoc,'Align_Current',Align_cur,Align_cur_sc));
   
    // alignment duration
    errorArray.push(UpdateFMVariable(xmlDoc,'Align_Duration',Align_dur_sc,Align_dur_sc));
    
    // Scales
    errorArray.push(UpdateFMVariable(xmlDoc,'FREQ_max',FREQmax,FREQmax));
    errorArray.push(UpdateFMVariable(xmlDoc,'N_max',Nmax,N_max_fl));
	errorArray.push(UpdateFMVariable(xmlDoc,'N_AngularScale',SpeedAngScale,SpeedAngScale));
    errorArray.push(UpdateFMVariable(xmlDoc,'I_max',Imax,Imax));
    errorArray.push(UpdateFMVariable(xmlDoc,'DCB_max',UDCmax,UDCmax));
    errorArray.push(UpdateFMVariable(xmlDoc,'U_max',Umax,Umax));
    //errorArray.push(UpdateFMVariable(xmlDoc,'E_max',Emax,Emax));
    
    // Fault thresholds
    TestRangeTrim("N_req", 0, Nmax-1, "rpm");
    TestRangeTrim("N_min", 0, Nmax-1, "rpm");
    TestRangeTrim("N_over", 0, Nmax-1, "rpm");
    TestRangeTrim("UDC_under", 0, UDCmax-1, "V");
    TestRangeTrim("UDC_over", 0, UDCmax-1, "V");
    TestRangeTrim("E_block", 0, Emax, "V");
    errorArray.push(UpdateFMVariable(xmlDoc,'N_nom',getParentHtmlValue("N_req"),N_reqmax_fl));
    errorArray.push(UpdateFMVariable(xmlDoc,'N_min',getParentHtmlValue("N_min"),N_min_fl));
    errorArray.push(UpdateFMVariable(xmlDoc,'N_over',getParentHtmlValue("N_over"),N_over_fl));
    errorArray.push(UpdateFMVariable(xmlDoc,'DCB_under',getParentHtmlValue("UDC_under"),getParentHtmlValue("UDC_under")));
    errorArray.push(UpdateFMVariable(xmlDoc,'DCB_over',getParentHtmlValue("UDC_over"),getParentHtmlValue("UDC_over")));
    errorArray.push(UpdateFMVariable(xmlDoc,'DCB_trip',getParentHtmlValue("UDC_trip"),getParentHtmlValue("UDC_trip")));
    errorArray.push(UpdateFMVariable(xmlDoc,'E_block',getParentHtmlValue("E_block"),getParentHtmlValue("E_block")));
    
    // display error message if any error detected
    UpdateError(errorArray);       
    }
}

/***************************************************************************//*!
*
* @brief  The function reads values from input forms, scales them and write 
*         to output HTML form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeParametersHTMLOutput(prefix,xmlObject)
{
    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Motor Parameters","",""));
    document.write(HTML_write_comment_line_dash());
    document.write(HTML_write_comment_line("Stator resistance","Rs",""));
    document.write(HTML_write_comment_line("Pole-pair numbers","pp",""));
    document.write(HTML_write_comment_line("Direct axis inductance","Ld",""));
    document.write(HTML_write_comment_line("Quadrature axis inductance","Lq",""));
    document.write(HTML_write_comment_line("Back-EMF constant","ke",""));
    document.write(HTML_write_comment_line("Drive inertia","J",""));
    document.write(HTML_write_comment_line("Nominal current","Iph",""));
    document.write(HTML_write_blank_line());
    document.write(HTML_write_define_line_number(prefix,0,"MOTOR_PP",xmlObject));
    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Application Scales","",""));
    document.write(HTML_write_comment_line_dash());
    document.write(HTML_write_define_line_number(prefix,0,"I_MAX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_MAX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_MAX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"N_MAX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FREQ_MAX",xmlObject));
    //if(testActiveTab('Sensorless'))
    //  document.write(HTML_write_define_line_number(prefix,0,"E_MAX",xmlObject));
    document.write(HTML_write_blank_line());
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_TRIP",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_UNDERVOLTAGE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_OVERVOLTAGE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"N_OVERSPEED",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"N_MIN",xmlObject));
    
    document.write(HTML_write_blank_line());
    document.write(HTML_write_define_line_number(prefix,0,"N_NOM",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"I_PH_NOM",xmlObject));

    /* DCB voltage filter */
    //speed IIR filter
    document.write(HTML_write_blank_line());
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_B0",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_B1",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_A1",xmlObject));  
    
    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Mechanical alignment","",""));
    document.write(HTML_write_comment_line_dash());
    
    // Alignment control              
    document.write(HTML_write_define_line_number(prefix,0,"ALIGN_DURATION",xmlObject));
    if(testVarValue('Alignment','Voltage'))
      document.write(HTML_write_define_line_number(prefix,0,"ALIGN_VOLTAGE",xmlObject));
    else  
      document.write(HTML_write_define_line_number(prefix,0,"ALIGN_CURRENT",xmlObject));
    
    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Application counters","",""));
    document.write(HTML_write_comment_line_dash());
    
    // Application counters            
    document.write(HTML_write_define_line_number(prefix,0,"CALIB_DURATION",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FAULT_DURATION",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FREEWHEEL_DURATION",xmlObject));

    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Miscellaneous","",""));
    document.write(HTML_write_comment_line_dash());
    document.write(HTML_write_define_line_number(prefix,0,"E_BLOCK_TRH",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"E_BLOCK_PER",xmlObject));

    
    // motor parameters commented
    copyParent2HeaderCfgById('pp','pp',' [-]',true);
    copyParent2HeaderCfgById('Rs','Rs',' [Ohms]',true);
    copyParent2HeaderCfgById('Ld','Ld',' [H]',true);
    copyParent2HeaderCfgById('Lq','Lq',' [H]',true);
    copyParent2HeaderCfgById('ke','ke',' [V.sec/rad]',true);
    copyParent2HeaderCfgById('J','J',' [kg.m2]',true);
    copyParent2HeaderCfgById('Iph','I_ph',' [A]',true);
    
    //calculate constants
    clickCalculateParam();
}

/***************************************************************************//*!
*
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeParametersHeaderOutput(prefix, str)
{
     str = write_blank_lines(str,1);     
     str = write_comment_text(str,'Motor Parameters','');
     str = write_comment_line_dash(str);
     str = write_comment_text(str,'Pole-pair number','pp');
     str = write_comment_text(str,'Stator resistance','Rs');    
     str = write_comment_text(str,'Direct axis inductance','Ld');
     str = write_comment_text(str,'Quadrature axis inductance','Lq');
     str = write_comment_text(str,'Back-EMF constant','ke');
     str = write_comment_text(str,'Drive inertia','J');
     str = write_comment_text(str,'Nominal current','Iph');
     
     str = write_blank_lines(str,1);     
     str = write_define_line_number(prefix, str,'MOTOR_PP');
     
     str = write_comment_line_dash(str);
     
     str = write_blank_lines(str,1);
     str = write_comment_text(str,'Application scales','');   
     str = write_comment_line_dash(str);
     str = write_define_line_number(prefix, str,'I_MAX');
     str = write_define_line_number(prefix, str,'U_DCB_MAX');
     str = write_define_line_number(prefix, str,'U_MAX');
     str = write_define_line_number(prefix, str,'N_MAX');
     str = write_define_line_number(prefix, str,'FREQ_MAX');
     //if(testActiveTab('Sensorless'))
     //   str = write_define_line_number(prefix, str,'E_MAX','');
                                            
     str = write_define_line_number(prefix, str,'U_DCB_TRIP');
     str = write_define_line_number(prefix, str,'U_DCB_UNDERVOLTAGE');
     str = write_define_line_number(prefix, str,'U_DCB_OVERVOLTAGE');
     str = write_define_line_number(prefix, str,'N_OVERSPEED');
     str = write_define_line_number(prefix, str,'N_MIN');
     str = write_define_line_number(prefix, str,'N_NOM');
     str = write_define_line_number(prefix, str,'I_PH_NOM');
     
     str = write_comment_text(str,'DCB Voltage Filter',''); 
     str = write_define_line_number(prefix, str,'UDCB_IIR_B0');
     str = write_define_line_number(prefix, str,'UDCB_IIR_B1');
     str = write_define_line_number(prefix, str,'UDCB_IIR_A1');
    
     str = write_comment_text(str,'Mechanical Alignment',''); 
     str = write_define_line_number(prefix, str,'ALIGN_CURRENT');
     str = write_define_line_number(prefix, str,'ALIGN_VOLTAGE');
     str = write_define_line_number(prefix, str,'ALIGN_DURATION');
    
     str = write_comment_text(str,'Application counters',''); 
     str = write_define_line_number(prefix, str,'CALIB_DURATION');
     str = write_define_line_number(prefix, str,'FAULT_DURATION');
     str = write_define_line_number(prefix, str,'FREEWHEEL_DURATION');

     str = write_comment_text(str,'Miscellaneous','');
     str = write_define_line_number(prefix, str,'E_BLOCK_TRH');
     str = write_define_line_number(prefix, str,'E_BLOCK_PER');
     
     return str;
}

/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_Parameters()
{
   // update constants
   initLoadFormParamValues();
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
