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
* @file   inner_SLoop.js
*
* @brief  Cpeed control loop engine
*
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormSloop() - init Speed Loop control page paramters and constants  
* clickCalculateSloop() - calculates control constants based on input parameters
* clickUpdateSloopFM() - update selected variables in FreeMASTER application
* writeSLoopHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writeSLoopHeaderOutput(str,prefix) - write selected constants to output header file
*******************************************************************************/
/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID 
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormSloop()
{
    prefixM = getActiveMotor();
    var ControllerType = null;
    var SlowLoopTs;
    
    /* get actual board Id if known board connected */
    var appId = parent.document.getElementById('M1typeTabName').innerHTML;
    
    // in basic mode, precalculate paramters
    if(getActiveMode()==0)
      basicModeCalcSLoop();
    
    /* If connected to target (sample time was read from target), disable param box */
    if(appId!="offline")
        disableInputParamBox("SLOOP_Ts");
    
    // Loop Sample Time
    copyParent2InnerValById("SLOOP_Ts");
    
    copyParent2InnerValById("SLOOP_F0");   
    copyParent2InnerValById("SLOOP_Att");
    copyParent2InnerValById("SLOOP_Ts");

    copyParent2InnerValById("RAMP_UP");   
    copyParent2InnerValById("RAMP_DOWN");
    copyParent2InnerValById("SPEED_MAF"); 
    copyParent2InnerValById("SPEED_IIR_FREQ");
    
    copyParent2InnerValById("SL_HIGH_LIM");   
    copyParent2InnerValById("SL_LOW_LIM");
    
    copyParent2InnerValById("PLOOP_KP_MAN");
   
    // display only required type of Speed filter
    if(testVarValue('WFilt','MA Filter'))
    {
      document.getElementById("SpeedFilterIIR").style.display = "none";
      document.getElementById("Speed_IIR_filt").style.display = "none";
      document.getElementById("SpeedFilterMAF").style.display = ""; 
    }
    else
    {
      document.getElementById("SpeedFilterIIR").style.display = "";
      document.getElementById("Speed_IIR_filt").style.display = "";
      document.getElementById("SpeedFilterMAF").style.display = "none";
    }
  
    // display only required type of PI controller
    if(testVarValue('Scontroller','Parallel'))
    {
        document.getElementById("Speed_PIparal").style.display = "";
        document.getElementById("Speed_PIrecur").style.display = "none";   
        ControllerType = 'Parallel';    
    }
    else
    {
        document.getElementById("Speed_PIparal").style.display = "none";
        document.getElementById("Speed_PIrecur").style.display = "";
        ControllerType = 'Reccurent';
    }
    
    // display Zero Cancelation filter constants
    if(testVarValue('FFw_SL','Zero Cancellation'))
    {
        document.getElementById("Speed_ZC").style.display = '';
    }
    else
    {
        document.getElementById("Speed_ZC").style.display = 'none';
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

    //document.getElementById("Arithmetic").innerText = parent.document.getElementById("Arithmetic").innerText;
    document.getElementById("Scontroller").innerText = ControllerType;    
    
    // enable button enabling
    ReloadStoreButtonsOnOff(1);
   
    // check manual tuning mode
    //get active motor to select proper prefix
    var prefixM = getActiveMotor();
    if(parent.document.getElementById(prefixM + 'SLOOP_PI_MAN_EN').innerHTML==1)
      document.getElementById(prefixM+'SL_PImanualTuning').checked=true;
    else
      document.getElementById(prefixM+'SL_PImanualTuning').checked=false;  
    
    //calculate constants
    clickCalculateSloop();
    
    // manual tunning function
    SL_PImanualTuning();  
    
}

/***************************************************************************//*!
* @brief   Mark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function markUpdateSLField()
{
    /* DISABLED in current MCAT version */
    /*
    var obj = document.getElementById("Speed_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    var obj = document.getElementById("Speed_IIR_filt").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';    

    document.getElementById("SLoopUpdateFrm").title = "Red-legend constants will be updated on click"
    */
}

/***************************************************************************//*!
* @brief   UnMark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function unmarkUpdateSLField()
{
    var obj = document.getElementById("Speed_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';

    var obj = document.getElementById("Speed_IIR_filt").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';       

}
/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcSLoop()
{
    var Rs            = getParentHtmlValue("Rs");
    var Lq            = getParentHtmlValue("Lq");
    var I_ph          = getParentHtmlValue("I_ph");
    var SLOOP_Ts      = getParentHtmlValue("SLOOP_Ts");
    var CLOOP_fo      = getParentHtmlValue("CLOOP_F0");
    
    // calculated input parameters
    SLoop_bandwidth = Math.round(CLOOP_fo/10);
    
    // replace and disable params
    switchParam2BasicMode("SLOOP_Ts",SLOOP_Ts);
    switchParam2BasicMode("SLOOP_F0",SLoop_bandwidth);
    switchParam2BasicMode("SPEED_MAF",2); 
    switchParam2BasicMode("SPEED_IIR_FREQ",100);
    switchParam2BasicMode("SL_HIGH_LIM",Math.round(I_ph*100)/100.0);
    switchParam2BasicMode("SL_LOW_LIM",Math.round(-I_ph*100)/100.0);
    
    if(testVarValue('FFw_SL','Zero Cancellation'))
      switchParam2BasicMode("SLOOP_Att",0.707);
    else
      switchParam2BasicMode("SLOOP_Att",1);                                                

}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickCalculateSloop()
{
    // need to be global vars due FM update
    SLOOP_Up_lim  = getParentHtmlValue("SL_HIGH_LIM");   
    SLOOP_Low_lim = getParentHtmlValue("SL_LOW_LIM");
    SPEED_MAF_filt= getParentHtmlValue("SPEED_MAF");
    
    var SLOOP_Ts      	= getParentHtmlValue("SLOOP_Ts");
    var SLOOP_F0      	= getParentHtmlValue("SLOOP_F0");
    var SLOOP_att     	= getParentHtmlValue("SLOOP_Att");       
    var RAMP_Up       	= getParentHtmlValue("RAMP_UP");
    var RAMP_Down     	= getParentHtmlValue("RAMP_DOWN");
    var SPEED_IIR_filt	= getParentHtmlValue("SPEED_IIR_FREQ");
    var Imax          	= getParentHtmlValue("I_max");
    var Nmax          	= getParentHtmlValue("N_max");
    var ke            	= getParentHtmlValue("ke");
    var J             	= getParentHtmlValue("J");  
    var CLOOP_Ts      	= getParentHtmlValue("CLOOP_Ts"); 
    var PP            	= getParentHtmlValue("pp");   
    var Wmax          	= 2*Math.PI*PP*Nmax/60;
	
    var W_KP_MAN      	= getParentHtmlValue("SLOOP_KP_MAN"); 
    var W_KI_MAN      	= getParentHtmlValue("SLOOP_KI_MAN");
    var SL_MAN_TUN    	= getParentHtmlValue("SLOOP_PI_MAN_EN");
    
    var P_KP_MAN        = getParentHtmlValue("PLOOP_KP_MAN");
    var IIRxCoefsScaleType     = parent.document.getElementById("IIRxCoefsScale").innerText;
	
    var Kt   = 3 * ke / (Math.sqrt(3));
	
    var Tmax = Imax*Kt; 

    var SpeedAngScale = 60/(PP*2*Math.PI);
    
    //////////////////////// Related to SPEED LOOP ///////////////////				
    if (SL_MAN_TUN)
    {
        W_Kps = W_KP_MAN;
        W_Kis = W_KI_MAN;
    }
    else
    {	
		
	W_Kps = 2*Math.PI/60 * (4*SLOOP_att*Math.PI*SLOOP_F0)*J/Kt;	
	W_Kis = 2*Math.PI/60 * ((2*Math.PI*SLOOP_F0)*(2*Math.PI*SLOOP_F0)*J)/(Kt*10);	
		
    }
    
    W_Kpz = Math.ceil(W_Kps*1000000000000)/1000000000000;	
    W_Kiz = Math.ceil(W_Kis*SLOOP_Ts*1000000000000)/1000000000000;	
    
    W_Kpz_f = W_Kpz*Wmax/Imax;	
    W_Kiz_f = W_Kiz*Wmax/Imax;
    
     ////// RECCURENT PI CONTROLLER TYPE ///////
    // scaling to scale and scale shift components
    W_CC1s = Math.round((W_Kps + W_Kis*SLOOP_Ts/2)*1000000000000)/1000000000000;	
    W_CC2s = Math.round((-W_Kps + W_Kis*SLOOP_Ts/2)*1000000000000)/1000000000000;
    
    W_CC1f = W_CC1s*Wmax/Tmax;
    W_CC2f = W_CC2s*Wmax/Tmax;
    
   // alert(W_CC1f+'\n'+W_CC2f); 
  
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
        
    // Zero-cross cancelation filter constants
    W_ZC_B0_fl = Math.round(W_Kis*SLOOP_Ts/(W_Kps+W_Kis*SLOOP_Ts)/2*1000000000000)/1000000000000;
    W_ZC_B1_fl = 0.0;
    W_ZC_A1_fl = W_ZC_B0_fl/IIRxCoefsScaleType;
    W_ZC_B0_out = Math.round(W_Kis*SLOOP_Ts/(W_Kps+W_Kis*SLOOP_Ts)/2*1000000000000)/1000000000000;
    W_ZC_B1_out = W_ZC_B1_fl/IIRxCoefsScaleType;
    W_ZC_A1_out = W_ZC_A1_fl/IIRxCoefsScaleType;
    testFracValRange("W_ZC_B0_out",W_ZC_B0_out,1);
    testFracValRange("W_ZC_B1_out",W_ZC_B1_out,1);
    testFracValRange("W_ZC_A1_out",W_ZC_A1_out,1);
    
    // speed ramp increments
    rampIncUp_float = Math.round(RAMP_Up*SLOOP_Ts/SpeedAngScale*1000000000000)/1000000000000;
    rampIncDown_float = Math.round(RAMP_Down*SLOOP_Ts/SpeedAngScale*1000000000000)/1000000000000;
    rampIncUp =  Math.round(RAMP_Up/60*PP*2*Math.PI/Wmax*SLOOP_Ts*1000000000000)/1000000000000;
    rampIncDown =  Math.round(RAMP_Down/60*PP*2*Math.PI/Wmax*SLOOP_Ts*1000000000000)/1000000000000;
    testFracValRange("rampIncUp",rampIncUp,1);
    testFracValRange("rampIncDown",rampIncDown,1);
    
    // scalar speed ramp increments
    ScalarRampIncUp_float = Math.round(RAMP_Up*CLOOP_Ts/60*PP*1000000000000)/1000000000000;
    ScalarRampIncDown_float = Math.round(RAMP_Down*CLOOP_Ts/60*PP*1000000000000)/1000000000000;
    ScalarRampIncUp =  Math.round(RAMP_Up/60*PP*2*Math.PI/Wmax*CLOOP_Ts*1000000000000)/1000000000000;
    ScalarRampIncDown =  Math.round(RAMP_Down/60*PP*2*Math.PI/Wmax*CLOOP_Ts*1000000000000)/1000000000000;
    testFracValRange("ScalarRampIncUp",ScalarRampIncUp,1);
    testFracValRange("ScalarRampIncDown",ScalarRampIncDown,1);
    
    // PI controller limits scaled
    Upper_limit = Math.round((SLOOP_Up_lim/Imax)*1000000000000)/1000000000000;
    Lower_limit = Math.round((SLOOP_Low_lim/Imax)*1000000000000)/1000000000000;
    testFracValRange("Upper_limit",Upper_limit,1);
    testFracValRange("Lower_limit",Lower_limit,1);
    
    // actual speed IIR filter
    W_IIR_B0_fl =  Math.round((2*Math.PI*SPEED_IIR_filt*CLOOP_Ts)/(2+(2*Math.PI*SPEED_IIR_filt*CLOOP_Ts))*1000000000000)/1000000000000; 
    W_IIR_B1_fl =  Math.round((2*Math.PI*SPEED_IIR_filt*CLOOP_Ts)/(2+(2*Math.PI*SPEED_IIR_filt*CLOOP_Ts))*1000000000000)/1000000000000;
    W_IIR_A1_fl =  -Math.round((2*Math.PI*SPEED_IIR_filt*CLOOP_Ts-2)/(2+(2*Math.PI*SPEED_IIR_filt*CLOOP_Ts))*1000000000000)/1000000000000;
    W_IIR_B0_out = Math.round(W_IIR_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    W_IIR_B1_out = Math.round(W_IIR_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    W_IIR_A1_out = Math.round(W_IIR_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    testFracValRange("W_IIR_B0_out",W_IIR_B0_out,1);
    testFracValRange("W_IIR_B1_out",W_IIR_B1_out,1);
    testFracValRange("W_IIR_A1_out",W_IIR_A1_out,1);
    // speed counter
    speedCounter =  Math.round(SLOOP_Ts / CLOOP_Ts);
    
    // Position controller
    P_Kpz_f = Math.round(P_KP_MAN*1000000000000)/1000000000000;
    testFracValRange("P_Kpz_f",P_Kpz_f,1);
    
    // If SPEED LOOP tab is active ******************************************
    if(document.getElementById("SLoop") != undefined)
    {
      // write values to forms in current Html page
      setInnerHtmlValue("SL_Kp_g",W_Kpz_f,W_Kps);
      setInnerHtmlValue("SL_Ki_g",W_Kiz_f,W_Kis);
      
      // write values to forms in current Html page
      setInnerHtmlValue("SL_CC1",W_CC1s,W_CC1s);
      setInnerHtmlValue("SL_CC2",W_CC2s,W_CC2s);
        
      // Zero cancelation
      setInnerHtmlValue("SL_ZC_B0",W_ZC_B0_out,W_ZC_B0_fl);
      setInnerHtmlValue("SL_ZC_B1",W_ZC_B1_out,W_ZC_B1_fl);
      setInnerHtmlValue("SL_ZC_A1",W_ZC_A1_out,W_ZC_A1_fl);
      
      // Zero cancelation
      setInnerHtmlValue("SL_IIR_B0",W_IIR_B0_out,W_IIR_B0_fl);
      setInnerHtmlValue("SL_IIR_B1",W_IIR_B1_out,W_IIR_B1_fl);
      setInnerHtmlValue("SL_IIR_A1",W_IIR_A1_out,W_IIR_A1_fl);
    }
      
     // If HEADER FILE tab is active ********************************************
     if(document.getElementById("HeaderFileTab") != undefined)
     {	
        if(testVarValue('Scontroller','Parallel')) // parallel type of PI controller
        {
            // write values to forms in current Html page
            // write values to forms in current Html page
            setInnerHtmlValueAsText("SPEED_PI_PROP_GAIN",0,W_Kpz_f, W_Kpz);
            setInnerHtmlValueAsText("SPEED_PI_INTEG_GAIN",0,W_Kiz_f, W_Kiz);
        }
        else // reccurent type of PI controller 
        {
          // write values to forms in current Html page
          setInnerHtmlValueAsText("SPEED_NSHIFT",1,W_Nshift);
          setInnerHtmlValueAsText("SPEED_CC1SC",0,W_CC1_out,W_CC1s);
          setInnerHtmlValueAsText("SPEED_CC2SC",0,W_CC2_out,W_CC2s);
        }
        
        if(testVarValue('FFw_SL','Zero Cancellation'))
        {
          // Zero cancelation
          setInnerHtmlValueAsText("SPEED_ZC_B0",0,W_ZC_B0_out,W_ZC_B0_fl);
          setInnerHtmlValueAsText("SPEED_ZC_B1",0,W_ZC_B1_out,W_ZC_B1_fl);
          setInnerHtmlValueAsText("SPEED_ZC_A1",0,W_ZC_A1_out,W_ZC_A1_fl);
        } 

      
        if(testVarValue('FFw_SL','Incremental Ramp'))
        {
          // ramp increment
          setInnerHtmlValueAsText("SPEED_RAMP_UP",0,rampIncUp,rampIncUp_float);
          setInnerHtmlValueAsText("SPEED_RAMP_DOWN",0,rampIncDown,rampIncDown_float);
        }
                
        // Speed controller limits
        setInnerHtmlValueAsText("SPEED_LOOP_HIGH_LIMIT",0,Upper_limit,SLOOP_Up_lim);
        setInnerHtmlValueAsText("SPEED_LOOP_LOW_LIMIT",0,Lower_limit,SLOOP_Low_lim);
        
        if(testVarValue('WFilt','MA Filter'))
        {
          //speed MA filter
          setInnerHtmlValueAsText("SPEED_FILTER_MA",2,SPEED_MAF_filt);
        }
        
        if(testVarValue('WFilt','IIR Filter'))
        {
          //speed IIR filter
          setInnerHtmlValueAsText("SPEED_IIR_B0",0,W_IIR_B0_out,W_IIR_B0_fl);
          setInnerHtmlValueAsText("SPEED_IIR_B1",0,W_IIR_B1_out,W_IIR_B1_fl);
          setInnerHtmlValueAsText("SPEED_IIR_A1",0,W_IIR_A1_out,W_IIR_A1_fl);
        }
        
        // position proportional gain
        //setInnerHtmlValueAsText("POS_P_PROP_GAIN",4,P_Kpz_f, P_Kpz_f);
         
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
function clickUpdateSloopFM(){
    
  if(!pcm.IsCommPortOpen())
  {
      alert("Communication is stopped.\nPress Ctrl+K to start the communication");
  }
  else
  {
    xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml"); 
     var MotorPrefix = getActiveMotor();
    var errorArray = [];
    
    // calculate actual constant values
    clickCalculateSloop();
    
    var selectedSensor = xmlDoc.getElementsByTagName([MotorPrefix+ "ControlStructurePoSpe"])[0];
    if(pcm.ReadVariable(selectedSensor.childNodes[0].nodeValue))
	var sensorType = pcm.LastVariable_vValue;
    
    var aritType     = parent.document.getElementById("Arithmetic").innerText;
        
    if(testVarValue('Scontroller','Parallel')) // parallel type of PI controller
     {
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_Kp_g',W_Kpz_f, W_Kpz));
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_Ki_g',W_Kiz_f, W_Kiz));
     }
     else // reccurent type of PI controller
     {
        if (aritType!=='Float')
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_Nsh',W_Nshift));
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_CC1SC',W_CC1f,W_CC1s));
        errorArray.push(UpdateFMVariable(xmlDoc,'SL_CC2SC',W_CC2f,W_CC2s));
     }
  
    // PI controller limits
    errorArray.push(UpdateFMVariable(xmlDoc,'SL_UP_LIM',SLOOP_Up_lim,SLOOP_Up_lim));
    errorArray.push(UpdateFMVariable(xmlDoc,'SL_LOW_LIM',SLOOP_Low_lim,SLOOP_Low_lim));
    
    // speed ramp
    if(testVarValue('FFw_SL','Incremental Ramp'))
    {
        errorArray.push(UpdateFMVariable(xmlDoc,'RAMP_UP_g',rampIncUp,rampIncUp_float));
        errorArray.push(UpdateFMVariable(xmlDoc,'RAMP_DOWN_g',rampIncDown,rampIncDown_float));
        /* scalar control V/Hz ramp */
        errorArray.push(UpdateFMVariable(xmlDoc,'Scalar_ramp_up',ScalarRampIncUp,ScalarRampIncUp_float));
        errorArray.push(UpdateFMVariable(xmlDoc,'Scalar_ramp_down',ScalarRampIncDown,ScalarRampIncDown_float));
    } 
    
    if(testVarValue('WFilt','MA Filter'))
    {
      //speed MA filter
      errorArray.push(UpdateFMVariable(xmlDoc,'SPEED_MAF_sc',SPEED_MAF_filt,SPEED_MAF_filt));
    }
    
    if(testVarValue('WFilt','IIR Filter'))
    {
       //speed IIR filter
       errorArray.push(UpdateFMVariable(xmlDoc,'SPEED_IIR_B0_g',W_IIR_B0_out,W_IIR_B0_fl));
       errorArray.push(UpdateFMVariable(xmlDoc,'SPEED_IIR_B1_g',W_IIR_B1_out,W_IIR_B1_fl));
       errorArray.push(UpdateFMVariable(xmlDoc,'SPEED_IIR_A1_g',W_IIR_A1_out,W_IIR_A1_fl));
    }
    
    // position controller 
    //errorArray.push(UpdateFMVariable(xmlDoc,'PL_Kp_g',P_Kpz_f,P_Kpz_f));
    
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
function writeSLoopHTMLOutput(prefix,xmlObject)
{
    
    // Speed Loop Control
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("Speed Loop Control","",""));
    document.write(HTML_write_comment_line_dash()); 
    document.write(HTML_write_comment_line("Loop bandwidth","SLOOP_F0",""));
    document.write(HTML_write_comment_line("Loop attenuation","SLOOP_Att",""));
    document.write(HTML_write_comment_line("Loop sample time","SLOOP_Ts",""));
    
    if(testVarValue('Scontroller','Parallel'))
    {
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_PI_PROP_GAIN",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_PI_INTEG_GAIN",xmlObject));
    }
    else
    {
      document.write(HTML_write_define_line_number(prefix,1,"SPEED_NSHIFT",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_CC1SC",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_CC2SC",xmlObject));              
    } 
    
    document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_HIGH_LIMIT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"SPEED_LOOP_LOW_LIMIT",xmlObject));
    
    if(testVarValue('FFw_SL','Zero Cancellation'))
    {              
      document.write(HTML_write_blank_line());
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_ZC_B0",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_ZC_B1",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_ZC_A1",xmlObject));
    }     
    
    if(testVarValue('FFw_SL','Incremental Ramp'))
    {  
      document.write(HTML_write_blank_line());
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_RAMP_UP",xmlObject));
      document.write(HTML_write_define_line_number(prefix,0,"SPEED_RAMP_DOWN",xmlObject));
    }
    
    document.write(HTML_write_blank_line());
    /* Actual Speed MA filter */
    if(testVarValue('WFilt','MA Filter'))
    {
       document.write(HTML_write_define_line_number(prefix,0,"SPEED_FILTER_MA",xmlObject));
    }   

    // Actual Speed IIR filter 
    if(testVarValue('WFilt','IIR Filter'))
    {
       document.write(HTML_write_define_line_number(prefix,0,"SPEED_IIR_B0",xmlObject));
       document.write(HTML_write_define_line_number(prefix,0,"SPEED_IIR_B1",xmlObject));
       document.write(HTML_write_define_line_number(prefix,0,"SPEED_IIR_A1",xmlObject));
    } 
    
    document.write(HTML_write_blank_line());
    // position controller
    //document.write(HTML_write_define_line_number(prefix,0,"POS_P_PROP_GAIN",xmlObject));
    
    copyParent2HeaderCfgById('SLOOP_F0','SLOOP_F0',' [Hz]',true);
    copyParent2HeaderCfgById('SLOOP_Att','SLOOP_Att',' [-]',true);
    copyParent2HeaderCfgById('SLOOP_Ts','SLOOP_Ts',' [sec]',true);
    clickCalculateSloop();

}    

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeSLoopHeaderOutput(prefix, str)
{
  
   str = write_blank_lines(str,1);     
   str = write_comment_text(str,'Speed Loop Control','');
   str = write_comment_line_dash(str);
   str = write_comment_text(str,'Loop bandwidth','SLOOP_F0');    
   str = write_comment_text(str,'Loop attenuation','SLOOP_Att');
   str = write_comment_text(str,'Loop sample time','SLOOP_Ts');
   str = write_comment_line_dash(str);
 
   //Speed controller
   if(testVarValue('Scontroller','Parallel'))
   {
      str = write_comment_text(str,'Speed Controller - Parallel type','','');   
      str = write_define_line_number(prefix, str,'SPEED_PI_PROP_GAIN'); 
      str = write_define_line_number(prefix, str,'SPEED_PI_INTEG_GAIN');
    }
    else
    {
      str = write_comment_text(str,'Speed Controller - Recurrent type','','');    
      str = write_define_line_number(prefix, str,'SPEED_NSHIFT');
      str = write_define_line_number(prefix, str,'SPEED_CC1SC');
      str = write_define_line_number(prefix, str,'SPEED_CC2SC');
    }
    
    str = write_define_line_number(prefix, str,'SPEED_LOOP_HIGH_LIMIT');
    str = write_define_line_number(prefix, str,'SPEED_LOOP_LOW_LIMIT');
    
    if(testVarValue('FFw_SL','Zero Cancelation'))
    {              
      str = write_blank_lines(str,1);
      str = write_define_line_number(prefix, str,'SPEED_ZC_B0');
      str = write_define_line_number(prefix, str,'SPEED_ZC_B1');
      str = write_define_line_number(prefix, str,'SPEED_ZC_A1');
    }
    if(testVarValue('FFw_SL','Incremental Ramp'))
    {
      str = write_blank_lines(str,1);
      str = write_define_line_number(prefix, str,'SPEED_RAMP_UP');
      str = write_define_line_number(prefix, str,'SPEED_RAMP_DOWN');
    } 

    str = write_blank_lines(str,1);
    if(testVarValue('WFilt','MA Filter'))
    {
      str = write_define_line_number(prefix, str,'SPEED_FILTER_MA');
    }  
    // Actual Speed MA filter 
    if(testVarValue('WFilt','IIR Filter'))
    {
      str = write_define_line_number(prefix, str,'SPEED_IIR_B0');
      str = write_define_line_number(prefix, str,'SPEED_IIR_B1');
      str = write_define_line_number(prefix, str,'SPEED_IIR_A1');
    }
    // position controller
    //str = write_blank_lines(str,1);
    //str = write_define_line_number(prefix, str,'POS_P_PROP_GAIN');
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
    
    // set manual tuning enablin to parameter
    parent.document.getElementById(prefixM + 'SLOOP_PI_MAN_EN').innerHTML = 0;
    // update items
    clickCalculateSloop();
    
    /* get actual board Id if known board connected */
    var appId = parent.document.getElementById('M1typeTabName').innerHTML;
    var SlowLoopTs;
    
    /* for known boards read defined sample time from target MCU */
    if(appId!="offline")
    {
        // replace and disable params
        reference_val = xmlDoc.getElementsByTagName([prefixM]+["Slow_Loop_Freq"])[0];
        if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue));
            SlowLoopTs = Math.round(1/pcm.LastVariable_tValue*10000)/10000;
        
        switchParam2BasicMode("SLOOP_Ts",SlowLoopTs);
        copyParent2InnerValById("SLOOP_Ts");
    }
    else
    {
        // Loop Sample Time
        copyParent2InnerValById("SLOOP_Ts");
    }
  }  
 // in basic mode, precalculate paramters
    if(getActiveMode()==0)
      basicModeCalcSLoop();
      
    
}

/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_SLoop()
{
   // update constants
   clickCalculateSloop();
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
