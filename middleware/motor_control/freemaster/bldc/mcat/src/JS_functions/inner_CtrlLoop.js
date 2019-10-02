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
* @file   inner_CtrlLoop.js
*
* @brief  Control loop engine
*
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormCtrlloop() - init Speed Loop control page paramters and constants  
* clickCalculateCtrlloop() - calculates control constants based on input parameters
* clickUpdateCtrlloopFM() - update selected variables in FreeMASTER application
* writeCtrlLoopHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writeCtrlLoopHeaderOutput(str,prefix) - write selected constants to output header file
*******************************************************************************/
/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID 
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormCtrlLoop()
{
    var ControllerType = null;
    var succ, vValue0, tValue0, retMsg;
    var Ctrl_Ts_MCU;
    var Pwm_freq_MCU;

    /* get actual board Id if known board connected */
    var appId = parent.document.getElementById('M1typeTabName').innerHTML;

    /* in basic mode, precalculate paramters */
    if(getActiveMode()==0)
      basicModeCalcCtrlLoop();
    
    /* for known boards read defined sample time from target MCU */
    if(appId!="offline")
    {
        // replace and disable params
        succ = pcm.ReadVariable("Ctrl loop freq", vValue0, tValue0, retMsg)
        if (succ)
            Ctrl_Ts_MCU = Math.round(1/pcm.LastVariable_tValue*10000)/10000;

        // replace and disable params
        succ = pcm.ReadVariable("Pwm Freq", vValue0, tValue0, retMsg)
        if (succ)
            Pwm_freq_MCU = Math.round(pcm.LastVariable_tValue);

        switchParam2BasicMode("CtrlLOOP_Ts",Ctrl_Ts_MCU);
        copyParent2InnerValById("CtrlLOOP_Ts");
        switchParam2BasicMode("PWM_freq",Pwm_freq_MCU);
        copyParent2InnerValById("PWM_freq");
    }
    else
    {
        // Loop Sample Time
        copyParent2InnerValById("CtrlLOOP_Ts");
        copyParent2InnerValById("PWM_freq");
    }

    // Control loop limits
    copyParent2InnerValById("CtrlLOOP_LIM_HIGH");
    copyParent2InnerValById("CtrlLOOP_LIM_LOW");

    // Torque controller
    // disable constant displaying of PI constants
    document.getElementById('TL_PI_const').style.display = "none";
    
    if(testVarValue('Ccontroller','Parallel'))
     {
       document.getElementById("Tloop_PIparal").style.display = "";
       document.getElementById("Tloop_PIrecur").style.display = "none";   
       ControllerType = 'Parallel';
       copyParent2InnerValById("TLOOP_KP_MAN");
       copyParent2InnerValById("TLOOP_KI_MAN");
     }
     else
     {
       document.getElementById("Tloop_PIparal").style.display = "none";
       document.getElementById("Tloop_PIrecur").style.display = "";
       ControllerType = 'Reccurent';    
     }   

    // Speed controller
    copyParent2InnerValById("RAMP_UP");   
    copyParent2InnerValById("RAMP_DOWN");

    // diable constant displaying of PI constants
    document.getElementById('SL_PI_const').style.display = "none";

    // display only required type of PI controller
    if(testVarValue('Scontroller','Parallel'))
    {
        document.getElementById("Speed_PIparal").style.display = "";
        document.getElementById("Speed_PIrecur").style.display = "none";   
        ControllerType = 'Parallel'; 
        copyParent2InnerValById("SLOOP_KP_MAN");
        copyParent2InnerValById("SLOOP_KI_MAN");   
    }
    else
    {
        document.getElementById("Speed_PIparal").style.display = "none";
        document.getElementById("Speed_PIrecur").style.display = "";
        ControllerType = 'Reccurent';
    }
    
    // display Speed ramp constants
    if(testVarValue('FFw_SL','Incremental Ramp'))
    { 
        document.getElementById("SpeedRamp").style.display = "";
    }    
    else
    {
        document.getElementById("SpeedRamp").style.display = "none";
    }   

    document.getElementById("Loop_controllers").innerText = ControllerType;    
    
    // enable button enabling
    ReloadStoreButtonsOnOff(1);
   
    //calculate constants
    clickCalculateCtrlLoop();  
}


/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcCtrlLoop()
{
    var CtrlLOOP_Ts      = getParentHtmlValue("CtrlLOOP_Ts");
    var SLOOP_Kp_man     = getParentHtmlValue("SLOOP_KP_MAN");
    var SLOOP_Ki_man     = getParentHtmlValue("SLOOP_KI_MAN");
    var TLOOP_Kp_man     = getParentHtmlValue("TLOOP_KP_MAN");
    var TLOOP_Ki_man     = getParentHtmlValue("TLOOP_KI_MAN");
    var PWM_freq         = getParentHtmlValue("PWM_freq");
    
    // replace and disable params
    switchParam2BasicMode("CtrlLOOP_Ts",CtrlLOOP_Ts);
    switchParam2BasicMode("PWM_freq",PWM_freq);
    
    // torque loop
    switchParam2BasicMode("CtrlLOOP_LIM_HIGH",90);
    switchParam2BasicMode("CtrlLOOP_LIM_LOW",0);
    switchParam2BasicMode("TLOOP_KP_MAN",TLOOP_Kp_man);
    switchParam2BasicMode("TLOOP_KI_MAN",TLOOP_Ki_man);

    // Speed loop
    switchParam2BasicMode("SLOOP_KP_MAN",SLOOP_Kp_man);
    switchParam2BasicMode("SLOOP_KI_MAN",SLOOP_Ki_man);   
}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickCalculateCtrlLoop()
{
    // need to be global vars due FM update
    var CtrlLOOP_Ts   = getParentHtmlValue("CtrlLOOP_Ts");
    var RAMP_Inc      = getParentHtmlValue("RAMP_UP");
    var RAMP_Dec      = getParentHtmlValue("RAMP_DOWN");
    var Imax          = getParentHtmlValue("I_max");
    var Umax          = getParentHtmlValue("UDC_max");
    var Nmax          = getParentHtmlValue("N_max");
    var PP            = getParentHtmlValue("pp");   
    var Wmax          = 2*Math.PI*PP*Nmax/60;
    var W_KP_MAN      = getParentHtmlValue("SLOOP_KP_MAN"); 
    var W_KI_MAN      = getParentHtmlValue("SLOOP_KI_MAN");
    var PWM_freq      = getParentHtmlValue("PWM_freq");
    var TL_KP_MAN     = getParentHtmlValue("TLOOP_KP_MAN");
    var TL_KI_MAN     = getParentHtmlValue("TLOOP_KI_MAN");
    CtrlLoop_LIM_high = getParentHtmlValue("CtrlLOOP_LIM_HIGH");
    CtrlLoop_LIM_low  = getParentHtmlValue("CtrlLOOP_LIM_LOW");

    //////////////////////// Related to TORQUE LOOP ///////////////////
    // torque controller limit in percentage of DC_BUS voltage actual
    CtrlLoop_LIM_high_sc = Math.round(CtrlLoop_LIM_high/100*1000000000000)/1000000000000;
    testValRange("CtrlLoop_LIM_high",CtrlLoop_LIM_high,0,100);
    CtrlLoop_LIM_low_sc = Math.round(CtrlLoop_LIM_low/100*1000000000000)/1000000000000;
    testValRange("CtrlLoop_LIM_low",CtrlLoop_LIM_low,0,CtrlLoop_LIM_high);  
  
    TL_Kps = TL_KP_MAN;
    TL_Kis = TL_KI_MAN;

  	TL_Kpz = TL_Kps;
    TL_Kiz = TL_Kis*CtrlLOOP_Ts;
    
    ////// PARALLEL PI CONTROLLER TYPE ///////
    TL_Kpz_f = TL_Kpz*Imax/Umax; 
    TL_Kiz_f = TL_Kiz*Imax/Umax;
    

    ///// ************** RECCURENT PI CONTROLLER TYPE ***************** ///////
    // scaling to scale and scale shift components
    TL_CC1s = Math.round((TL_Kps + TL_Kis*CtrlLOOP_Ts/2)*1000000000000)/1000000000000;
  	TL_CC2s = Math.round((-TL_Kps + TL_Kis*CtrlLOOP_Ts/2)*1000000000000)/1000000000000;
    	
    // scale constants
    TL_CC1f = TL_CC1s*Imax/Umax;
    TL_CC2f = TL_CC2s*Imax/Umax;
  
    // scale shift
    if ((Math.abs(TL_CC1f)<1) && (Math.abs(TL_CC2f)<1))   TL_Nshift = 0;
    else
    {
      if (Math.abs(TL_CC1f) > Math.abs(TL_CC2f))
          TL_Nshift = Math.ceil(Math.log(Math.abs(TL_CC1f))/Math.log(2));
      else
          TL_Nshift = Math.ceil(Math.log(Math.abs(TL_CC2f))/Math.log(2));        
    }
    
    TL_CC1_out = Math.round(TL_CC1f/Math.pow(2,TL_Nshift)*1000000000000)/1000000000000;
    TL_CC2_out = Math.round(TL_CC2f/Math.pow(2,TL_Nshift)*1000000000000)/1000000000000;
    testFracValRange("TL_CC1_out",TL_CC1_out);
    testFracValRange("TL_CC2_out",TL_CC2_out,1);
    
    
    //////////////////////// Related to SPEED LOOP ///////////////////				
    W_Kps = W_KP_MAN;
    W_Kis = W_KI_MAN;
    
    W_Kpz = Math.round(W_Kps*1000000000000)/1000000000000;
	W_Kiz = Math.round(W_Kis*CtrlLOOP_Ts*1000000000000)/1000000000000;

    W_Kpz_f = W_Kpz*Wmax/Imax;
	W_Kiz_f = W_Kiz*Wmax/Imax;
    

    
     ////// RECCURENT PI CONTROLLER TYPE ///////
    // scaling to scale and scale shift components
    W_CC1s = Math.round((W_Kps + W_Kis*CtrlLOOP_Ts/2)*1000000000000)/1000000000000;
  	W_CC2s = Math.round((-W_Kps + W_Kis*CtrlLOOP_Ts/2)*1000000000000)/1000000000000;
    
    W_CC1f = W_CC1s*Wmax/Imax;
    W_CC2f = W_CC2s*Wmax/Imax;
  
    // scale shift
    if ((Math.abs(W_CC1f)<1) && (Math.abs(W_CC2f)<1))  W_Nshift = 0;
    else
    {
      if (Math.abs(W_CC1f) > Math.abs(W_CC2f))
          W_Nshift = Math.ceil(Math.log(Math.abs(W_CC1f))/Math.log(2));
      else
          W_Nshift = Math.ceil(Math.log(Math.abs(W_CC2f))/Math.log(2));        
    }
    
    W_CC1_out = Math.round(W_CC1f/Math.pow(2,W_Nshift)*1000000000000)/1000000000000;
    W_CC2_out = Math.round(W_CC2f/Math.pow(2,W_Nshift)*1000000000000)/1000000000000;
    testFracValRange("W_CC1_out",W_CC1_out);
    testFracValRange("W_CC2_out",W_CC2_out,1);
        
     
    // speed ramp increments
    rampInc_float = RAMP_Inc*CtrlLOOP_Ts;
    rampDec_float = RAMP_Dec*CtrlLOOP_Ts;
    rampInc = Math.round(RAMP_Inc/60*PP*2*Math.PI/Wmax*CtrlLOOP_Ts*1000000000000)/1000000000000;
    rampDec = Math.round(RAMP_Dec/60*PP*2*Math.PI/Wmax*CtrlLOOP_Ts*1000000000000)/1000000000000;
    testFracValRange("rampInc",rampInc,1);
    testFracValRange("rampInc",rampDec,1);

    // speed counter
    speedCounter =  Math.round(CtrlLOOP_Ts * PWM_freq);
    
    // If SPEED LOOP tab is active ******************************************
    if(document.getElementById("CtrlLoop") != undefined)
    {
      // write values to forms in current Html page
      setInnerHtmlValue("SL_Kp_g",W_Kps,W_Kpz);
      setInnerHtmlValue("SL_Ki_g",W_Kis,W_Kiz);
      
      // write values to forms in current Html page
      setInnerHtmlValue("SL_CC1",W_CC1s,W_CC1s);
      setInnerHtmlValue("SL_CC2",W_CC2s,W_CC2s);
        
      // write values to forms in current Html page
      setInnerHtmlValue("TL_CC1",TL_CC1s,TL_CC1s);
      setInnerHtmlValue("TL_CC2",TL_CC2s,TL_CC2s);
      
      // write values to forms in current Html page
      setInnerHtmlValue("TL_Kp_g",TL_Kps,TL_Kpz);
      setInnerHtmlValue("TL_Ki_g",TL_Kis,TL_Kiz);        
 
    }
      
     // If HEADER FILE tab is active ********************************************
     if(document.getElementById("HeaderFileTab") != undefined)
     {	
        if(testVarValue('Scontroller','Parallel')) // parallel type of PI controller
        {
          // write values to forms in current Html page
          setInnerHtmlValueAsText("SPEED_LOOP_KP_GAIN",10,W_Kpz_f, W_Kpz);
          setInnerHtmlValueAsText("SPEED_LOOP_KI_GAIN",10,W_Kiz_f, W_Kiz);
        }
        else // reccurent type of PI controller 
        {
          // write values to forms in current Html page
          setInnerHtmlValueAsText("SPEED_LOOP_NSHIFT",1,W_Nshift);
          setInnerHtmlValueAsText("SPEED_LOOP_CC1SC",0,W_CC1f,W_CC1s);
          setInnerHtmlValueAsText("SPEED_LOOP_CC2SC",0,W_CC2f,W_CC2s);
        }
        
        if(testVarValue('FFw_SL','Incremental Ramp'))
        {
          // ramp increment
          setInnerHtmlValueAsText("SPEED_LOOP_RAMP_UP",6,rampInc,rampInc);
          setInnerHtmlValueAsText("SPEED_LOOP_RAMP_DOWN",6,rampDec,rampDec);
        }
              
        // speed counter
        speedCounter =  Math.round(CtrlLOOP_Ts / CtrlLOOP_Ts);
        setInnerHtmlValueAsText("SPEED_LOOP_CNTR",2,speedCounter);
        
        setInnerHtmlValueAsText("CTRL_LOOP_LIM_HIGH",0,CtrlLoop_LIM_high_sc, CtrlLoop_LIM_high);
        setInnerHtmlValueAsText("CTRL_LOOP_LIM_LOW",0,CtrlLoop_LIM_low_sc, CtrlLoop_LIM_low);

        if(testVarValue('Ccontroller','Parallel')) // parallel type of PI controller
        {
          ////// PARALLEL PI CONTROLLER TYPE ///////
          setInnerHtmlValueAsText("TORQUE_LOOP_KP_GAIN",10,TL_Kpz_f,TL_Kpz);
          setInnerHtmlValueAsText("TORQUE_LOOP_KI_GAIN",10,TL_Kiz_f,TL_Kiz);

        }
    }   
 } 
/* ----------- End of PI controller parameters routine -------------- */  

/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickUpdateCtrlLoopFM(){
    
    xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml"); 
    var errorArray = [];
    
    // calculate actual constant values
    clickCalculateCtrlLoop();
    
    
    if(testVarValue('Scontroller','Parallel')) // parallel type of PI controller
    {
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_Kp_g',W_Kpz_f, W_Kpz));
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_Ki_g',W_Kiz_f, W_Kiz));
     }
    else // reccurent type of PI controller
     {
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_Nsh',W_Nshift));
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_CC1SC',W_CC1f,W_CC1s));
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_CC2SC',W_CC2f,W_CC2s));
     }

    errorArray.push(UpdateFMVariable(xmlDoc,'SL_UP_LIM',CtrlLoop_LIM_high_sc, CtrlLoop_LIM_high));
    errorArray.push(UpdateFMVariable(xmlDoc,'SL_LOW_LIM',CtrlLoop_LIM_low_sc, CtrlLoop_LIM_low));

    // speed ramp
    if(testVarValue('FFw_SL','Incremental Ramp'))
    {
        errorArray.push(UpdateFMVariable(xmlDoc,'RAMP_UP_g',rampInc,rampInc_float));
        errorArray.push(UpdateFMVariable(xmlDoc,'RAMP_DOWN_g',rampDec,rampDec_float));
    } 

    if(testVarValue('Ccontroller','Parallel')) // parallel type of PI controller
    {
        errorArray.push(UpdateFMVariable(xmlDoc,'TL_Kp_g',TL_Kpz_f,TL_Kpz));
        errorArray.push(UpdateFMVariable(xmlDoc,'TL_Ki_g',TL_Kiz_f,TL_Kiz));
    }
    
    errorArray.push(UpdateFMVariable(xmlDoc,'TL_UP_LIM',CtrlLoop_LIM_high_sc, CtrlLoop_LIM_high));
    errorArray.push(UpdateFMVariable(xmlDoc,'TL_LOW_LIM',CtrlLoop_LIM_low_sc, CtrlLoop_LIM_low));

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
function writeCtrlLoopHTMLOutput(prefix,xmlObject)
{
    
    // Speed Loop Control
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("BLDC Control Loop","",""));
    document.write(HTML_write_comment_line_dash()); 
    document.write(HTML_write_comment_line("Loop sample time","CtrlLOOP_Ts",""));
    
    document.write(HTML_write_blank_line());
    document.write(HTML_write_define_line_number(prefix,0,"CTRL_LOOP_LIM_HIGH",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"CTRL_LOOP_LIM_LOW",xmlObject));
    document.write(HTML_write_blank_line());
    
    if(testVarValue('Scontroller','Parallel'))
    {
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_KP_GAIN",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_KI_GAIN",xmlObject));
    }
    else
    {
      document.write(HTML_write_define_line_number(prefix,1,"SPEED_LOOP_NSHIFT",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_CC1SC",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_CC2SC",xmlObject));              
    } 
    
    if(testVarValue('FFw_SL','Incremental Ramp'))
    {  
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_RAMP_UP",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_RAMP_DOWN",xmlObject));
    }
    
    //document.write(HTML_write_blank_line());
    //document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_CNTR",xmlObject));
    
    document.write(HTML_write_blank_line());
    // torque loop
    if(testVarValue('Ccontroller','Parallel'))
    {
      document.write(HTML_write_define_line_number(prefix,0,"TORQUE_LOOP_KP_GAIN",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"TORQUE_LOOP_KI_GAIN",xmlObject));
    }
    else
    {
      document.write(HTML_write_define_line_number(prefix,1,"TORQUE_LOOP_NSHIFT",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"TORQUE_LOOP_CC1SC",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"TORQUE_LOOP_CC2SC",xmlObject));              
    } 
    

    copyParent2HeaderCfgById('CtrlLOOP_Ts','CtrlLOOP_Ts',' [sec]',true);
    clickCalculateCtrlLoop();

}    

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeCtrlLoopHeaderOutput(str)
{
  
   str = write_blank_lines(str,1);     
   str = write_comment_text(str,'BLDC Control Loop','');
   str = write_comment_line_dash(str);
   str = write_comment_text(str,'Loop sample time','CtrlLOOP_TS');
   str = write_comment_line_dash(str);
 
   str = write_comment_text(str,'Control loop limits','','');
   str = write_define_line_number(prefix,str,'CTRL_LOOP_LIM_HIGH');
   str = write_define_line_number(prefix,str,'CTRL_LOOP_LIM_LOW');
   str = write_blank_lines(str,1);
   
   //Speed controller
   str = write_comment_text(str,'Speed Controller - Parallel type','','');
   if(testVarValue('Scontroller','Parallel'))
   {
      str = write_define_line_number(prefix,str,'SPEED_LOOP_KP_GAIN');
      str = write_define_line_number(prefix,str,'SPEED_LOOP_KI_GAIN');
    }
    else
    {
      str = write_define_line_number(prefix,str,'SPEED_LOOP_NSHIFT');
      str = write_define_line_number(prefix,str,'SPEED_LOOP_CC1SC');
      str = write_define_line_number(prefix,str,'SPEED_LOOP_CC2SC');
    }

    if(testVarValue('FFw_SL','Incremental Ramp'))
    {
      str = write_blank_lines(str,1);
      str = write_comment_text(str,'Speed ramp increments','','');
      str = write_define_line_number(prefix,str,'SPEED_LOOP_RAMP_UP');
      str = write_define_line_number(prefix,str,'SPEED_LOOP_RAMP_DOWN');
    } 

    str = write_blank_lines(str,1);

    // torque loop 
    str = write_comment_text(str,'Torque Controller - Parallel type','','');
    if(testVarValue('Ccontroller','Parallel'))
    {
      str = write_define_line_number(prefix,str,'TORQUE_LOOP_KP_GAIN');
      str = write_define_line_number(prefix,str,'TORQUE_LOOP_KI_GAIN');
    }
    else
    {
      str = write_define_line_number(prefix,str,'TORQUE_LOOP_NSHIFT');
      str = write_define_line_number(prefix,str,'TORQUE_LOOP_CC1SC');
      str = write_define_line_number(prefix,str,'TORQUE_LOOP_CC2SC');
    }
    

    return str;
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function SL_PImanualTuning()
{
  var parameterIdArray=new Array(3);
  parameterIdArray[0] = "SLOOP_Ts";
  parameterIdArray[1] = "SLOOP_F0";
  parameterIdArray[2] = "SLOOP_Att";
  
  //get active motor to select proper prefix
  var prefixM = getActiveMotor();
  
  // enable manunal tuning of SL PI controller constants
  if(document.getElementById(prefixM+'SL_PImanualTuning').checked)
  {
    for(i=0;i<3;i++){
      // set read only attributte
      document.getElementById(prefixM +  parameterIdArray[i]).readOnly  = true;
      // change background color
      document.getElementById(prefixM +  parameterIdArray[i]).style.backgroundColor ='#C3C7CC';   //rgb(195,199,204)
      // clear red text color of ID in main inner table 
      parent.document.getElementById(prefixM +  parameterIdArray[i]).style.color="black";
    }
   
    // swap constant / parametr displaying of PI constants
    document.getElementById('SL_PI_const').style.display = "none";
    document.getElementById('SL_PI_const_param').style.display = "";
    
    // enable Kp and Ki for writting
    document.getElementById('SL_Kp_g').style.backgroundColor ='';
    document.getElementById('SL_Ki_g').style.backgroundColor ='';
    
    // set manual tuning enabling to parameter
    parent.document.getElementById(prefixM + 'SLOOP_PI_MAN_EN').innerHTML = 1;
    
    // preset values for manual constant tuning
    if((getParentHtmlValue('SLOOP_KP_MAN'))==0)
      document.getElementById(prefixM + 'SLOOP_KP_MAN').value = document.getElementById('SL_Kp_g').value;
    else
      copyParent2InnerValById("SLOOP_KP_MAN");
   
    if((getParentHtmlValue('SLOOP_KI_MAN'))==0)
      document.getElementById(prefixM + 'SLOOP_KI_MAN').value = document.getElementById('SL_Ki_g').value;
    else
      copyParent2InnerValById("SLOOP_KI_MAN");
  }
  else
  {
    for(i=0;i<3;i++){
      // set read only attributte
      document.getElementById(prefixM +  parameterIdArray[i]).readOnly  = false;
      // change background color
      document.getElementById(prefixM +  parameterIdArray[i]).style.backgroundColor ='';   //rgb(195,199,204)
      // clear red text color of ID in main inner table 
      parent.document.getElementById(prefixM +  parameterIdArray[i]).style.color="";
    }
    // swap constant / parametr displaying of PI constants
    document.getElementById('SL_PI_const').style.display = "";
    document.getElementById('SL_PI_const_param').style.display = "none";
    
    // enable Kp and Ki for writting
    document.getElementById('SL_Kp_g').style.backgroundColor ='#C3C7CC';
    document.getElementById('SL_Ki_g').style.backgroundColor ='#C3C7CC';
    
    // set manual tuning enablin to parameter
    parent.document.getElementById(prefixM + 'SLOOP_PI_MAN_EN').innerHTML = 0;
    
    // update items
    clickCalculateSloop();
  }  
 // in basic mode, precalculate paramters
    if(getActiveMode()==0)
      basicModeCalcCtrlLoop();
}

/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_CtrlLoop()
{
   // update constants
   clickCalculateCtrlLoop();
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
