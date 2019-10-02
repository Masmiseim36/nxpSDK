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
* @file   inner_Sensorless.js
*
* @brief  Sensorless control setting for BLDC control
*
******************************************************************************/
/******************************************b************************************
* List of functions
******************************************************************************
* initLoadFormSensorless() - init BLDC Sensorless page paramters and constants  
* clickCalculateSensorless() - calculates control constants based on input parameters
* clickUpdateSensorlessFM() - update selected variables in FreeMASTER application
* writeSensorlessHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writeSensorlessHeaderOutput(str) - write selected constants to output header file
*******************************************************************************/
/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID 
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormSensorless()
{
    var ControllerType = null;
    var succ, vValue0, tValue0, retMsg;
    var Timer_freq_MCU;

    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();

    /* get actual board Id if known board connected */
    var appId = parent.document.getElementById('M1typeTabName').innerHTML;
      
    // in basic mode, precalculate paramters
    if(getActiveMode()==0)
        basicModeCalcSensorless();
   
    /* for known boards read defined sample time from target MCU */
    if(appId!="offline")
    {
        // replace and disable params
        succ = pcm.ReadVariable("Cmt Timer Freq", vValue0, tValue0, retMsg)
        if (succ)
            Timer_freq_MCU = pcm.LastVariable_tValue;

        switchParam2BasicMode("TIMER_FREQ",Timer_freq_MCU);
        copyParent2InnerValById("TIMER_FREQ");
    }
    else
    {
        /* default value */
        copyParent2InnerValById("TIMER_FREQ");
    }

    copyParent2InnerValById("TIMER_FREQ");
    copyParent2InnerValById("SPEED_MIN");
    copyParent2InnerValById("FREEWHEEL_TIME");

    copyParent2InnerValById("STARTUP_SPEED");
    copyParent2InnerValById("STARTUP_CMT_COUNT");
    copyParent2InnerValById("STARTUP_CMT_PER");

    copyParent2InnerValById("T_OFF_CMT");
    copyParent2InnerValById("INTEG_TRH_CORR");


    // enable button enabling
    ReloadStoreButtonsOnOff(1);

    //calculate constants
    clickCalculateSensorless();
}

/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcSensorless()
{
    var Timer_freq    = getParentHtmlValue("TIMER_FREQ");
    var Nmin          = getParentHtmlValue("SPEED_MIN");
    var FW_time       = getParentHtmlValue("FREEWHEEL_TIME");
    var NstartTrh     = getParentHtmlValue("STARTUP_SPEED");
    var Cmt_t_off     = getParentHtmlValue("T_OFF_CMT");
    var Cmt_per       = getParentHtmlValue("STARTUP_CMT_PER");
    var Cmt_count     = getParentHtmlValue("STARTUP_CMT_COUNT");
    
    
    // replace and disable params
    switchParam2BasicMode("TIMER_FREQ",Timer_freq);
    switchParam2BasicMode("SPEED_MIN",Nmin);
    switchParam2BasicMode("FREEWHEEL_TIME",FW_time);
    switchParam2BasicMode("STARTUP_SPEED",NstartTrh);
    switchParam2BasicMode("STARTUP_CMT_COUNT",Cmt_count);
    switchParam2BasicMode("STARTUP_CMT_PER",Cmt_per);
    switchParam2BasicMode("T_OFF_CMT",Cmt_t_off);
}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None   
* @remarks 
******************************************************************************/
function clickCalculateSensorless()
{
    var Timer_freq    = getParentHtmlValue("TIMER_FREQ");
        Nmin          = getParentHtmlValue("SPEED_MIN");
    var Nmax          = getParentHtmlValue("N_max");
    var NstartTrh     = getParentHtmlValue("STARTUP_SPEED");
    var CtrlLOOP_Ts   = getParentHtmlValue("CtrlLOOP_Ts");
    var PP            = getParentHtmlValue("pp");
    var Wmax          = 2*Math.PI*PP*Nmax/60;
        Cmt_count     = getParentHtmlValue("STARTUP_CMT_COUNT");
        Cmt_per       = getParentHtmlValue("STARTUP_CMT_PER");
        Cmt_t_off     = getParentHtmlValue("T_OFF_CMT");
    var FW_time       = getParentHtmlValue("FREEWHEEL_TIME");
    var Ke            = getParentHtmlValue("ke");
    var Nnom          = getParentHtmlValue("N_req");
    var Umax          = getParentHtmlValue("UDC_max");
    var Unom          = getParentHtmlValue("U_ph_nom");
    var Int_trh_corr  = getParentHtmlValue("INTEG_TRH_CORR");
    var PWM_freq      = getParentHtmlValue("PWM_freq");      
              
    //freewheel time from minimal and any speed
    Freewheel_time_long = Math.round(FW_time/0.001);
    Freewheel_time_short = Math.round(FW_time/2/0.001);
    testValRange("Freewheel time",FW_time,0,30);
    
    // speed where control switchs to close loop
    NstartTrh_sc = Math.round(NstartTrh/Nmax*1000000000000)/1000000000000;
    testValRange("Open Loop Speed limit",NstartTrh,(60/(PP*6*Cmt_per)),Nmax);
    
    // minimal speed for sensorless control
    Nmin_sc = Math.round(Nmin/Nmax*1000000000000)/1000000000000;
    testValRange("Open Loop Speed limit",Nmin,(60/(PP*6*Cmt_per)),Nmax);
    
    //start-up commutation period
    Cmt_period_start_sc = Math.round(Cmt_per*Timer_freq);
    testValRange("First Commutation Period",Cmt_per,0,32767/Timer_freq);
    
    // commutation time off
    Cmt_t_off_sc = Math.round(Cmt_t_off/100*1000000000000)/1000000000000;
    testValRange("Commutation Time Off",Cmt_t_off,0,100);
    
    // speed scale constant
    Speed_scale_constant = Math.round(Timer_freq*60/(Nmax*PP));
    
    // minimal commutation period
    Cmt_period_min_sc = Math.round(Timer_freq/(Nmax*PP/10));
   
    // start-up acceleration constant
    Startup_acceleration = Math.round(Math.pow((60/(NstartTrh*PP*6)/Cmt_per),(1/(Cmt_count-1)))*1000000000000)/1000000000000;
    Startup_acceleration_show =   Math.round(Startup_acceleration*100000000)/100000000;
    testFracValRange("START_CMT_ACCELER",Startup_acceleration,1);
    
    // Back-EMF treshold constant
    temp1 = 1/4*Ke*(2*Math.PI*PP*Nnom/60);
    temp2 = 1/2/(Nnom*PP*6/60)*PWM_freq*temp1;
    Integ_trh = Math.round(temp2/Umax*32768*Int_trh_corr/100);
   //alert(temp1, temp2, Integ_trh); 
    
    // If BLDC Sensorless tab is active ******************************************
    if(document.getElementById("SensorlessModule") != undefined)
    {
      // write values to forms in current Html page
      setInnerHtmlValue("CMT_PER_MIN",Cmt_period_min_sc,Cmt_period_min_sc);
      setInnerHtmlValue("CMT_PER_START",Cmt_period_start_sc,Cmt_period_start_sc);
      setInnerHtmlValue("SPEED_SC_CONST",Speed_scale_constant,Speed_scale_constant);
      setInnerHtmlValue("START_CMT_ACCELER",Startup_acceleration_show,Startup_acceleration_show);
      setInnerHtmlValue("INTEG_TRH",Integ_trh,Integ_trh);
   	}
   
    // If HEADER FILE tab is active ********************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {	
        setInnerHtmlValueAsText("FREEWHEEL_T_LONG",2,Freewheel_time_long);
        setInnerHtmlValueAsText("FREEWHEEL_T_SHORT",2,Freewheel_time_short);
        setInnerHtmlValueAsText("N_MIN",0,Nmin_sc,Nmin); 
        setInnerHtmlValueAsText("N_START_TRH",0,NstartTrh_sc,NstartTrh);
        setInnerHtmlValueAsText("STARTUP_CMT_CNT",2,Cmt_count);
        setInnerHtmlValueAsText("STARTUP_CMT_PER",2,Cmt_period_start_sc);
        setInnerHtmlValueAsText("CMT_T_OFF",0,Cmt_t_off_sc,Cmt_t_off);
        setInnerHtmlValueAsText("SPEED_SCALE_CONST",2,Speed_scale_constant);
        setInnerHtmlValueAsText("CMT_PER_MIN",2,Cmt_period_min_sc);
        setInnerHtmlValueAsText("START_CMT_ACCELER",0,Startup_acceleration,Startup_acceleration);
        setInnerHtmlValueAsText("INTEG_TRH",2,Integ_trh);
    }
  } 

/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickUpdateSensorlessFM()
{
    xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml"); 
    var errorArray = [];
    
    // calculate actual constant values 
    clickCalculateSensorless();
 
    errorArray.push(UpdateFMVariable(xmlDoc,'Speed_min_sc',Nmin,Nmin));
	errorArray.push(UpdateFMVariable(xmlDoc,'Freewheel_long',Freewheel_time_long, Freewheel_time_long));
	errorArray.push(UpdateFMVariable(xmlDoc,'Freewheel_short',Freewheel_time_short, Freewheel_time_short));
    errorArray.push(UpdateFMVariable(xmlDoc,'Start_cmt_accel',Startup_acceleration, Startup_acceleration));
    errorArray.push(UpdateFMVariable(xmlDoc,'Start_cmt_cnt',Cmt_count, Cmt_count));
    errorArray.push(UpdateFMVariable(xmlDoc,'Start_cmt_per',Cmt_period_start_sc, Cmt_period_start_sc));
    errorArray.push(UpdateFMVariable(xmlDoc,'Cmt_time_off',Cmt_t_off_sc,Cmt_t_off));
    errorArray.push(UpdateFMVariable(xmlDoc,'Flux_trh',Integ_trh, Integ_trh));
    // display error message                           
    UpdateError(errorArray);
              
}     

/***************************************************************************//*!
*
* @brief  The function reads values from input forms, scales them and write 
*         to output HTML form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeSensorlessHTMLOutput(prefix,xmlObject)
{ 
    // BLDC Sensorless Module
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("BLDC Sensorless Module","",""));
    document.write(HTML_write_comment_line_dash()); 
    document.write(HTML_write_comment_line("Timer frequency","TIMER_FREQ",""));
    document.write(HTML_write_blank_line());
    
    document.write(HTML_write_define_line_number(prefix,0,"N_MIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"N_START_TRH",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"STARTUP_CMT_CNT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"STARTUP_CMT_PER",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"CMT_T_OFF",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FREEWHEEL_T_LONG",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FREEWHEEL_T_SHORT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"SPEED_SCALE_CONST",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"CMT_PER_MIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"START_CMT_ACCELER",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"INTEG_TRH",xmlObject));
    
    copyParent2HeaderCfgById('TIMER_FREQ','TIMER_FREQ',' [Hz]',true);
    
    clickCalculateSensorless();
    
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeSensorlessHeaderOutput(str)
{
  
   str = write_blank_lines(str,1);     
   str = write_comment_text(str,'Sensoroless Control Module','');
   str = write_comment_line_dash(str);
   str = write_comment_text(str,'Timer Frequency','TIMER_FREQ');    
   str = write_comment_line_dash(str);
  
   str = write_define_line_number(prefix,str,'N_MIN');
   str = write_define_line_number(prefix,str,'N_START_TRH');
   str = write_define_line_number(prefix,str,'STARTUP_CMT_CNT');
   str = write_define_line_number(prefix,str,'STARTUP_CMT_PER');
   str = write_define_line_number(prefix,str,'CMT_T_OFF');
   str = write_define_line_number(prefix,str,'FREEWHEEL_T_LONG');
   str = write_define_line_number(prefix,str,'FREEWHEEL_T_SHORT');
   str = write_define_line_number(prefix,str,'SPEED_SCALE_CONST');
   str = write_define_line_number(prefix,str,'CMT_PER_MIN');
   str = write_define_line_number(prefix,str,'START_CMT_ACCELER');
   str = write_define_line_number(prefix,str,'INTEG_TRH');
        
    return str;
}

/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_SensorlessModule()
{
   // update constants
   clickCalculateSensorless();
}


/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
    