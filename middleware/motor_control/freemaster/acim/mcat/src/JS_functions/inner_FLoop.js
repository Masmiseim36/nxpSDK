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
* @file   inner_FLoop.js
*
* @brief  Flux control loop engine
*
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormFloop() - init Speed Loop control page paramters and constants  
* clickCalculateFloop() - calculates control constants based on input parameters
* clickUpdateFloopFM() - update selected variables in FreeMASTER application
* writeFLoopHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writeFLoopHeaderOutput(str,prefix) - write selected constants to output header file
*******************************************************************************/
/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID 
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormFloop()
{  
    // in basic mode, precalculate parameters
    if(getActiveMode()==0)
      basicModeCalcFLoop();
    
   
    copyParent2InnerValById("FL_MTPA_MIN");
    copyParent2InnerValById("FL_MTPA_MAX");
    copyParent2InnerValById("FL_FW_MIN");
    copyParent2InnerValById("FL_FW_F0");
    copyParent2InnerValById("FL_MTPA_FILT_FRQ");
    copyParent2InnerValById("FL_STARTUP");
    copyParent2InnerValById("FL_FW_IQERR_FILT_FRQ");
 
    // enable button enabling
    ReloadStoreButtonsOnOff(1);

    //calculate constants
    clickCalculateFloop();  
}

/***************************************************************************//*!
* @brief   Mark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function markUpdateFLField()
{
    /* DISABLED in current MCAT version */
    /*
    var obj = document.getElementById("Speed_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    var obj = document.getElementById("Speed_IIR_filt").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';    

    document.getElementById("FLoopUpdateFrm").title = "Red-legend constants will be updated on click"
    */
}

/***************************************************************************//*!
* @brief   UnMark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function unmarkUpdateFLField()
{
 
}
/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcFLoop()
{
    var Inom        = getParentHtmlValue("I_nom");


    switchParam2BasicMode("FL_MTPA_MIN", Math.round(Inom*0.333*100)/100.0);
    switchParam2BasicMode("FL_MTPA_MAX", Inom);
    switchParam2BasicMode("FL_FW_MIN", Math.round(Inom*0.333*100)/100.0);
    switchParam2BasicMode("FL_FW_F0", 0.25);
    switchParam2BasicMode("FL_MTPA_FILT_FRQ", 50);
    switchParam2BasicMode("FL_FW_IQERR_FILT_FRQ", 50);
    switchParam2BasicMode("FL_STARTUP", Inom);
}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constants based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickCalculateFloop()
{
    var SLOOP_Ts      = getParentHtmlValue("SLOOP_Ts");
    var Imax          = getParentHtmlValue("I_max");
    var Inom          = getParentHtmlValue("I_nom");
    var UDCmax        = getParentHtmlValue("UDC_max");
    var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
    var Umax          = Math.round(UDCmax/UmaxCoeff*10)/10;  
    var Nmax          = getParentHtmlValue("N_max");
    var Nmin          = getParentHtmlValue("N_min");
    var Wmax          = 2*Math.PI*PP*Nmax/60;
    var IIRxCoefsScaleType = parent.document.getElementById("IIRxCoefsScale").innerText;
    var Lr            = getParentHtmlValue("LrIM");
    var Ls            = getParentHtmlValue("LrIM");
    var Rr            = getParentHtmlValue("RrIM");
    var Lm            = getParentHtmlValue("LmIM");
    var PP            = getParentHtmlValue("pp"); 
    mtpa_min          = getParentHtmlValue("FL_MTPA_MIN");
    mtpa_max          = getParentHtmlValue("FL_MTPA_MAX");
    var mtpa_filt_frq = getParentHtmlValue("FL_MTPA_FILT_FRQ");
    startup_min       = getParentHtmlValue("FL_STARTUP");
    startup_min_pct   = getParentHtmlValue("FL_STARTUP_MIN_PCT")/100.0;
    fw_min            = getParentHtmlValue("FL_FW_MIN");
    var fw_f0         = getParentHtmlValue("FL_FW_F0");
    var fw_filt_frq   = getParentHtmlValue("FL_FW_IQERR_FILT_FRQ");


     if(TestRangeTrim("FL_MTPA_MIN", 0,  mtpa_max, ""))    
        mtpa_min = getParentHtmlValue("FL_MTPA_MIN"); 
    if(TestRangeTrim("FL_FW_MIN", 0,  mtpa_max, ""))    
        fw_min = getParentHtmlValue("FL_FW_MIN");



    // MTPA filtartion coefficient
    mtpa_filt_coef = 1.0-Math.exp(-2.0 * Math.PI * mtpa_filt_frq * SLOOP_Ts);
    
    // FW controller gains
    fw_kp = 2.0 * Math.PI * fw_f0 * Lr / (Lm * Rr);
    fw_ki = 2.0 * Math.PI * fw_f0 * SLOOP_Ts * Rr / (Lm * Rr);
    fw_max = mtpa_max; 
    
    fw_filt_b0_fl  =  Math.round((2.0 * Math.PI * fw_filt_frq * SLOOP_Ts)       / (2.0 + (2.0 * Math.PI * fw_filt_frq * SLOOP_Ts))*1e12)/1e12; 
    fw_filt_b1_fl  =  Math.round((2.0 * Math.PI * fw_filt_frq * SLOOP_Ts)       / (2.0 + (2.0 * Math.PI * fw_filt_frq * SLOOP_Ts))*1e12)/1e12;
    fw_filt_a1_fl  = -Math.round((2.0 * Math.PI * fw_filt_frq * SLOOP_Ts - 2.0) / (2.0 + (2.0 * Math.PI * fw_filt_frq * SLOOP_Ts))*1e12)/1e12;
    fw_filt_b0_out = Math.round(fw_filt_b0_fl/IIRxCoefsScaleType*1e12)/1e12;
    fw_filt_b1_out = Math.round(fw_filt_b1_fl/IIRxCoefsScaleType*1e12)/1e12;
    fw_filt_a1_out = Math.round(fw_filt_a1_fl/IIRxCoefsScaleType*1e12)/1e12;
    testFracValRange("W_IIR_B0_out", fw_filt_b0_out, 1);
    testFracValRange("W_IIR_B1_out", fw_filt_b1_out, 1);
    testFracValRange("W_IIR_A1_out", fw_filt_a1_out, 1);
    
    
    // If FLUX LOOP tab is active **********************************************
    if(document.getElementById("FLoop") != undefined)
    {
      // write values to forms in current Html page 
    }
      
    // If HEADER FILE tab is active ********************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {	
        setInnerHtmlValueAsText("FLUX_ID_MIN", 0, mtpa_min, Math.round(mtpa_min*1e12)/1e12);
        setInnerHtmlValueAsText("FLUX_ID_MAX", 0, mtpa_max, Math.round(mtpa_max*1e12)/1e12);
        setInnerHtmlValueAsText("FLUX_MTPA_FILT_COEFF", 0, mtpa_filt_coef, Math.round(mtpa_filt_coef*1e12)/1e12);
        
        setInnerHtmlValueAsText("FLUX_ID_START_MIN", 0, startup_min, Math.round(startup_min*1e12)/1e12);
        setInnerHtmlValueAsText("FLUX_ID_START_MIN_PCT", 0, startup_min_pct, Math.round(startup_min_pct*1e12)/1e12);
        
        setInnerHtmlValueAsText("FLUX_FW_ID_MIN", 0, fw_min, Math.round(fw_min*1e12)/1e12);
        setInnerHtmlValueAsText("FLUX_FW_KP_GAIN", 0, fw_kp, Math.round(fw_kp*1e12)/1e12);
        setInnerHtmlValueAsText("FLUX_FW_KI_GAIN", 0, fw_ki, Math.round(fw_ki*1e12)/1e12);
        
        setInnerHtmlValueAsText("FLUX_IIR_B0", 0, fw_filt_b0_out, Math.round(fw_filt_b0_fl*1e12)/1e12);
        setInnerHtmlValueAsText("FLUX_IIR_B1", 0, fw_filt_b1_out, Math.round(fw_filt_b1_fl*1e12)/1e12);
        setInnerHtmlValueAsText("FLUX_IIR_A1", 0, fw_filt_a1_out, Math.round(fw_filt_a1_fl*1e12)/1e12);
    }   
} 

/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickUpdateFloopFM(){
    
    if(!pcm.IsCommPortOpen())
    {
      alert("Communication is stopped.\nPress Ctrl+K to start the communication");
    }
    else
    {
      xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml"); 
  
      var errorArray = [];
            
      // calculate actual constant values
      clickCalculateFloop();
 
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_ID_MIN', mtpa_min, mtpa_min));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_ID_MAX', mtpa_max, mtpa_max));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_MTPA_FILT_COEFF', mtpa_filt_coef, mtpa_filt_coef));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_ID_START_MIN', startup_min, startup_min));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_ID_START_MIN_PCT', startup_min_pct, startup_min_pct));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_FW_ID_MIN', fw_min, fw_min));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_FW_ID_MAX', fw_max, fw_max));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_FW_KP_GAIN', fw_kp, fw_kp));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_FW_KI_GAIN', fw_ki, fw_ki));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_IIR_B0', fw_filt_b0_fl, fw_filt_b0_fl));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_IIR_B1', fw_filt_b1_fl, fw_filt_b1_fl));
      errorArray.push(UpdateFMVariable(xmlDoc, 'FLUX_IIR_A1', fw_filt_a1_fl, fw_filt_a1_fl));  
      
      // display error message                           
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
function writeFLoopHTMLOutput(prefix,xmlObject)
{
    
    // Flux Loop Control
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("/* flux loop control", "", ""));
    document.write(HTML_write_comment_line_dash()); 
    document.write(HTML_write_comment_line("MTPA minimal d-axis current", "FL_MTPA_MIN", ""));
    document.write(HTML_write_comment_line("MTPA maximal d-axis current", "FL_MTPA_MAX", ""));
    document.write(HTML_write_comment_line("MTPA filter bandwidth", "FL_MTPA_FILT_FRQ", ""));
    document.write(HTML_write_comment_line("FW control bandwidth", "FL_FW_F0", ""));
    document.write(HTML_write_comment_line("FW filter bandwidth", "FL_FW_IQERR_FILT_FRQ", ""));
    document.write(HTML_write_comment_line_dash("*/"));
    
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_ID_MIN", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_ID_MAX", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_MTPA_FILT_COEFF", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_ID_START_MIN", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_ID_START_MIN_PCT", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_FW_ID_MIN", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_FW_KP_GAIN", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_FW_KI_GAIN", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_IIR_B0", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_IIR_B1", xmlObject));
    document.write(HTML_write_define_line_number(prefix, 0, "FLUX_IIR_A1", xmlObject));
   
    copyParent2HeaderCfgById('FL_MTPA_MIN', 'FL_MTPA_MIN', ' [A]', true);
    copyParent2HeaderCfgById('FL_MTPA_MAX', 'FL_MTPA_MAX', ' [A]', true);
    copyParent2HeaderCfgById('FL_MTPA_FILT_FRQ', 'FL_MTPA_FILT_FRQ', ' [Hz]', true);
    copyParent2HeaderCfgById('FL_FW_F0', 'FL_FW_F0', ' [Hz]', true);
    copyParent2HeaderCfgById('FL_FW_IQERR_FILT_FRQ', 'FL_FW_IQERR_FILT_FRQ', ' [Hz]', true);
  

    clickCalculateFloop();
}    

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeFLoopHeaderOutput(prefix, str)
{
    str = write_blank_lines(str,1);     
    str = write_comment_text(str,'/* flux loop control','');
    str = write_comment_line_dash(str);
    str = write_comment_text(str,'maximal d-axis current','ID_REQ');  
    str = write_comment_text(str,'MTPA minimal d-axis current','FL_MTPA_MIN');
    str = write_comment_text(str,'MTPA maximal d-axis current','FL_MTPA_MAX');
    str = write_comment_text(str,'MTPA filter bandwidth','FL_MTPA_FILT_FRQ');
    str = write_comment_text(str,'FW control bandwidth','FL_FW_F0');
    str = write_comment_text(str,'FW filter bandwidth','FL_FW_IQERR_FILT_FRQ');
    str = write_comment_line_dash(str,"*/");
     
    str = write_define_line_number(prefix, str, 'FLUX_ID_MIN');
    str = write_define_line_number(prefix, str, 'FLUX_ID_MAX');
    str = write_define_line_number(prefix, str, 'FLUX_MTPA_FILT_COEFF');
    str = write_define_line_number(prefix, str, 'FLUX_ID_START_MIN');
    str = write_define_line_number(prefix, str, 'FLUX_ID_START_MIN_PCT');
    str = write_define_line_number(prefix, str, 'FLUX_FW_ID_MIN');
    str = write_define_line_number(prefix, str, 'FLUX_FW_KP_GAIN');
    str = write_define_line_number(prefix, str, 'FLUX_FW_KI_GAIN');
    str = write_define_line_number(prefix, str, 'FLUX_IIR_B0');
    str = write_define_line_number(prefix, str, 'FLUX_IIR_B1');
    str = write_define_line_number(prefix, str, 'FLUX_IIR_A1');
    
    return str;
}

/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_FLoop()
{
   // update constants
   clickCalculateFloop();
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
