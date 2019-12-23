/*******************************************************************************
*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
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
    
    // Hide Application scales in float
    document.getElementById("ApplicationScales").style.display = 'none';
    
    copyParent2InnerValById("RsIM");
    copyParent2InnerValById("RrIM");
    copyParent2InnerValById("LsIM");
    copyParent2InnerValById("LrIM");
    copyParent2InnerValById("LmIM");
    copyParent2InnerValById("pp");
    copyParent2InnerValById("JIM");
    copyParent2InnerValById("TmIM");

    copyParent2InnerValById("I_nom");
    copyParent2InnerValById("U_nom");
    copyParent2InnerValById("f_nom");
    
    
    copyParent2InnerValById("I_max");
    copyParent2InnerValById("UDC_max");
    
    // precalculated or manualy added
    copyParent2InnerValById("UDC_trip");
    copyParent2InnerValById("UDC_under");
    copyParent2InnerValById("UDC_over");
    copyParent2InnerValById("N_over");
    
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
    var obj = document.getElementById("FaultLimits").getElementsByTagName('legend')[0];
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
    var obj = document.getElementById("FaultLimits").getElementsByTagName('legend')[0];
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
    var N_over        = getParentHtmlValue("N_over");
    var I_nom         = getParentHtmlValue("I_nom");
    var Imax          = getParentHtmlValue("I_max");
    var pp            = getParentHtmlValue("pp");
    var J             = getParentHtmlValue("JIM");
    
    // calculated input parameters
    UDCB_trip = Math.round(UDCB_max*0.8*10)/10;
    UDCB_under = Math.round(UDCB_max*0.4*10)/10;
    UDCB_over = Math.round(UDCB_max*0.85*10)/10;
    
    // replace and disable params
    switchParam2BasicMode("UDC_trip",UDCB_trip);
    switchParam2BasicMode("UDC_under",UDCB_under);
    switchParam2BasicMode("UDC_over",UDCB_over);
    switchParam2BasicMode("N_over",N_over);

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
    var Rs      = getParentHtmlValue("RsIM");
    var Rr      = getParentHtmlValue("RrIM");
    var Ls      = getParentHtmlValue("LsIM");
    var Lr      = getParentHtmlValue("LrIM");
    var Lm      = getParentHtmlValue("LmIM");
    var Ts      = getParentHtmlValue("CLOOP_Ts");   
    var CLOOP_Ts      = getParentHtmlValue("CLOOP_Ts");
    var SLOOP_Ts      = getParentHtmlValue("SLOOP_Ts");
    var Calib_dur     = getParentHtmlValue("CALIB_T");
    var Fault_dur     = getParentHtmlValue("FAULT_T");
    var Freewheel_dur = getParentHtmlValue("FREEWHEEL_T");
    var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
    var Motor_pp      = getParentHtmlValue("pp");
    var IIRxCoefsScaleType = parent.document.getElementById("IIRxCoefsScale").innerText;

    U_nom       = getParentHtmlValue("U_nom");
    I_nom       = getParentHtmlValue("I_nom");
    f_nom       = getParentHtmlValue("f_nom"); 

    Imax        = getParentHtmlValue("I_max");
    UDCmax      = getParentHtmlValue("UDC_max");
    Umax        = Math.round(UDCmax/UmaxCoeff*10)/10;
    
    
    UDCunder    = getParentHtmlValue("UDC_under");
    UDCover     = getParentHtmlValue("UDC_over");
    UDCtrip     = getParentHtmlValue("UDC_trip"); 
    Nover       = getParentHtmlValue("N_over");

    Calib_dur_sc =  Math.round(Calib_dur/SLOOP_Ts);
    Fault_dur_sc =  Math.round(Fault_dur/SLOOP_Ts);
    Freewheel_dur_sc =  Math.round(Freewheel_dur/SLOOP_Ts);
        
    UDC_trip_sc = Math.round(UDCtrip/UDCmax*1000000000000)/1000000000000;
    UDC_under_sc = Math.round(UDCunder/UDCmax*1000000000000)/1000000000000;
    UDC_over_sc = Math.round(UDCover/UDCmax*1000000000000)/1000000000000;
    N_over_sc = 0; // to be added in case frac is needed    
      
    I_nom_sc = Math.round(I_nom/Imax*1000000000000)/1000000000000;

    
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
    
    TestRangeTrim("UDC_trip", 0, UDCmax-1, "V");
    TestRangeTrim("UDC_under", 0, UDCmax-1, "V");
    TestRangeTrim("UDC_over", 0, UDCmax-1, "V");
     
    
    // If PARAMETERS tab is active ******************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {
      setInnerHtmlValueAsText("SQRT2",0, Math.sqrt(2.0) , Math.round(Math.sqrt(2.0)*1e12)/1e12);
      setInnerHtmlValueAsText("MOTOR_PP",2,Motor_pp, Math.round(Motor_pp*1e12)/1e12);
      setInnerHtmlValueAsText("U_NOM",0, U_nom, Math.round(U_nom*1e12)/1e12);
      setInnerHtmlValueAsText("I_PH_NOM",0, I_nom, Math.round(I_nom*1e12)/1e12);
      setInnerHtmlValueAsText("F_NOM",0, f_nom, Math.round(f_nom*1e12)/1e12);

      setInnerHtmlValueAsText("R_S",0, Rs, Math.round(Rs*1e12)/1e12);
      setInnerHtmlValueAsText("R_R",0, Rr, Math.round(Rr*1e12)/1e12);
      setInnerHtmlValueAsText("L_S",0, Ls, Math.round(Ls*1e12)/1e12);
      setInnerHtmlValueAsText("L_R",0, Lr, Math.round(Lr*1e12)/1e12);
      setInnerHtmlValueAsText("L_M",0, Lm, Math.round(Lm*1e12)/1e12);
      
            
      // write maximal scales to forms in Output File HTML page
      setInnerHtmlValueAsText("I_MAX",8,Imax, Math.round(Imax*1e12)/1e12);
      setInnerHtmlValueAsText("U_DCB_MAX",8,UDCmax, Math.round(UDCmax*1e12)/1e12);
      setInnerHtmlValueAsText("U_MAX",8,Umax, Math.round(Umax*1e12)/1e12);
      
      setInnerHtmlValueAsText("U_DCB_TRIP",0,UDC_trip_sc, Math.round(UDCtrip*1e12)/1e12);
      setInnerHtmlValueAsText("U_DCB_UNDERVOLTAGE",0,UDC_under_sc, Math.round(UDCunder*1e12)/1e12);
      setInnerHtmlValueAsText("U_DCB_OVERVOLTAGE",0,UDC_over_sc, Math.round(UDCover*1e12)/1e12);
      setInnerHtmlValueAsText("N_OVERSPEED",0,N_over_sc, Math.round(Nover*1e12)/1e12);     
      
      // DCB voltage IIR filter
      setInnerHtmlValueAsText("UDCB_IIR_B0",0,UDCB_IIR_B0_out,Math.round(UDCB_IIR_B0_fl*1e12)/1e12);
      setInnerHtmlValueAsText("UDCB_IIR_B1",0,UDCB_IIR_B1_out,Math.round(UDCB_IIR_B1_fl*1e12)/1e12);
      setInnerHtmlValueAsText("UDCB_IIR_A1",0,UDCB_IIR_A1_out,Math.round(UDCB_IIR_A1_fl*1e12)/1e12);
      
      // write values to forms in Output File Html page
      setInnerHtmlValueAsText("CALIB_DURATION",2,Calib_dur_sc);
      setInnerHtmlValueAsText("FAULT_DURATION",2,Fault_dur_sc);
      setInnerHtmlValueAsText("FREEWHEEL_DURATION",2,Freewheel_dur_sc);  
      
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
        
        // Scales
        errorArray.push(UpdateFMVariable(xmlDoc,'I_max',Imax,Imax));
        errorArray.push(UpdateFMVariable(xmlDoc,'DCB_max',UDCmax,UDCmax));
        errorArray.push(UpdateFMVariable(xmlDoc,'U_max',Umax,Umax));
        
        // Fault thresholds
        TestRangeTrim("UDC_trip", 0, UDCmax-1, "V");
        TestRangeTrim("UDC_under", 0, UDCmax-1, "V");
        TestRangeTrim("UDC_over", 0, UDCmax-1, "V");
        errorArray.push(UpdateFMVariable(xmlDoc,'N_over',getParentHtmlValue("N_over"),getParentHtmlValue("N_over")));
        errorArray.push(UpdateFMVariable(xmlDoc,'DCB_under',getParentHtmlValue("UDC_under"),getParentHtmlValue("UDC_under")));
        errorArray.push(UpdateFMVariable(xmlDoc,'DCB_over',getParentHtmlValue("UDC_over"),getParentHtmlValue("UDC_over")));
        errorArray.push(UpdateFMVariable(xmlDoc,'DCB_trip',getParentHtmlValue("UDC_trip"),getParentHtmlValue("UDC_trip")));
        
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
    document.write(HTML_write_define_line_number(prefix,0,"SQRT2",xmlObject));
    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("/* motor & application parameters","",""));
    document.write(HTML_write_comment_line_dash());
    document.write(HTML_write_comment_line("pole-pair numbers","pp",""));
    document.write(HTML_write_comment_line("stator resistance","Rs",""));
    document.write(HTML_write_comment_line("rotor resistance","Rr",""));
    document.write(HTML_write_comment_line("stator inductance","Ls",""));
    document.write(HTML_write_comment_line("rotor inductance","Lr",""));
    document.write(HTML_write_comment_line("mutual inductance","Lm",""));
    document.write(HTML_write_comment_line("moment of inertia","J",""));
    document.write(HTML_write_comment_line("mechanical time constant","Tm",""));
    document.write(HTML_write_comment_line_dash("*/"));
    document.write(HTML_write_define_line_number(prefix,0,"MOTOR_PP",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_NOM",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"I_PH_NOM",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"F_NOM",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"R_S",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"R_R",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"L_S",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"L_R",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"L_M",xmlObject));
    
    document.write(HTML_write_comment_line("/* application scales */","",""));
    document.write(HTML_write_define_line_number(prefix,0,"I_MAX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_MAX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_MAX",xmlObject));
    
    document.write(HTML_write_comment_line("/* fault thresholds */","",""));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_TRIP",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_UNDERVOLTAGE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_OVERVOLTAGE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"N_OVERSPEED",xmlObject));
    
    /* DCB voltage filter */
    //speed IIR filter
    document.write(HTML_write_comment_line("/* DC-bus voltage filter parameters */","",""));
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_B0",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_B1",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_A1",xmlObject));  
    
    // Application counters  
    document.write(HTML_write_comment_line("/* application counters */","",""));
    document.write(HTML_write_define_line_number(prefix,0,"CALIB_DURATION",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FAULT_DURATION",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FREEWHEEL_DURATION",xmlObject));

    
    // motor parameters commented
    copyParent2HeaderCfgById('pp','pp',' [-]',true);
    copyParent2HeaderCfgById('Rs','RsIM',' [Ohms]',true);
    copyParent2HeaderCfgById('Rr','RrIM',' [Ohms]',true);
    copyParent2HeaderCfgById('Ls','LsIM',' [H]',true);
    copyParent2HeaderCfgById('Lr','LrIM',' [H]',true);
    copyParent2HeaderCfgById('Lm','LmIM',' [H]',true);
    copyParent2HeaderCfgById('J','JIM',' [kg.m2]',true);
    copyParent2HeaderCfgById('Tm','TmIM',' [s]',true);

    
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
     str = write_define_line_number(prefix, str,'SQRT2');
     str = write_blank_lines(str,1);     
     str = write_comment_text(str,'/* motor & application parameters','');
     str = write_comment_line_dash(str);
     str = write_comment_text(str,'pole-pair number','pp');
     str = write_comment_text(str,'stator resistance','Rs');
     str = write_comment_text(str,'rotor resistance','Rr');
     str = write_comment_text(str,'stator inductance','Ls');
     str = write_comment_text(str,'rotor inductance','Lr');
     str = write_comment_text(str,'mutual inductance','Lm');
     str = write_comment_text(str,'moment of inertia','J');
     str = write_comment_text(str,'mechanical time constant','Tm');
     str = write_comment_line_dash(str,"*/");  
     
     str = write_define_line_number(prefix, str,'MOTOR_PP');
     str = write_define_line_number(prefix, str,'U_NOM');
     str = write_define_line_number(prefix, str,'I_PH_NOM');
     str = write_define_line_number(prefix, str,'F_NOM');
     str = write_define_line_number(prefix, str,'R_S');
     str = write_define_line_number(prefix, str,'R_R');
     str = write_define_line_number(prefix, str,'L_S');
     str = write_define_line_number(prefix, str,'L_R');
     str = write_define_line_number(prefix, str,'L_M');
     str = write_comment_text(str,'/* application scales */','');   
     str = write_define_line_number(prefix, str,'I_MAX');
     str = write_define_line_number(prefix, str,'U_MAX');
     str = write_define_line_number(prefix, str,'U_DCB_MAX');
                               
     str = write_comment_text(str,'/* fault thresholds */','');   
     str = write_define_line_number(prefix, str,'U_DCB_TRIP');
     str = write_define_line_number(prefix, str,'U_DCB_UNDERVOLTAGE');
     str = write_define_line_number(prefix, str,'U_DCB_OVERVOLTAGE');
     str = write_define_line_number(prefix, str,'N_OVERSPEED');
   
     
     str = write_comment_text(str,'/* DC-bus voltage filter */',''); 
     str = write_define_line_number(prefix, str,'UDCB_IIR_B0');
     str = write_define_line_number(prefix, str,'UDCB_IIR_B1');
     str = write_define_line_number(prefix, str,'UDCB_IIR_A1');
    
     str = write_comment_text(str,'/* application counters */',''); 
     str = write_define_line_number(prefix, str,'CALIB_DURATION');
     str = write_define_line_number(prefix, str,'FAULT_DURATION');
     str = write_define_line_number(prefix, str,'FREEWHEEL_DURATION');
     
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
