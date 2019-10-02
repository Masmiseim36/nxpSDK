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
* @file   inner_PoSpeBemfDQ.js
*
* @brief  BEMF Observer engine
*
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormBemfDQ() - init Sensorless page paramters and constants  
* clickCalculateBemfDQ() - calculates control constants based on input parameters
* clickUpdateBemfDQFM() - update selected variables in FreeMASTER application
* writeBemfDQHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writeBemfDQHeaderOutput(str) - write selected constants to output header file
*******************************************************************************/

/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID   
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormBemfDqObsvr()
{
   // in basic mode, precalculate paramters
    if(getActiveMode()==0)
      basicModeCalcPoSpeBEMF();
   
   copyParent2InnerValById("BEMF_DQ_F0");   
   copyParent2InnerValById("BEMF_DQ_Att");

   copyParent2InnerValById("TO_F0");   
   copyParent2InnerValById("TO_Att");
   
   copyParent2InnerValById("STARTUP_RAMP");   
   copyParent2InnerValById("STARTUP_CURRENT");
   copyParent2InnerValById("STARTUP_MERG_SPEED");   
   copyParent2InnerValById("STARTUP_MERG_COEFF");
   
   var typeArithmetic =  parent.document.getElementById("Arithmetic").innerText;
      
   // enable button enabling
   ReloadStoreButtonsOnOff(1);
   
   //calculate constants
   clickCalculateBemfDqObsrv();
}

/***************************************************************************//*!
*
* @brief   Mark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function markUpdateSLSField()
{
    /* DISABLED in current MCAT version */
    /*
    var obj = document.getElementById("DQbemfObs").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    var obj = document.getElementById("BemfPIpar").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    var obj = document.getElementById("BemfPIrec").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';      
    var obj = document.getElementById("ToPIparam").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    
    document.getElementById("BemfObsrvUpdateFrm").title = "Red-legend constants will be updated on click"
    */
}

/***************************************************************************//*!
* @brief   UnMark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function unmarkUpdateSLSField()
{
    var obj = document.getElementById("DQbemfObs").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
    var obj = document.getElementById("BemfPIpar").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
    var obj = document.getElementById("BemfPIrec").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';    
    var obj = document.getElementById("ToPIparam").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
    
}
/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcPoSpeBEMF()
{
    var Nreq          = getParentHtmlValue("N_req");
    
    speedMerging = Math.ceil(Nreq*0.1);
    
    // replace and disable params
    switchParam2BasicMode("BEMF_DQ_F0",250);
    switchParam2BasicMode("BEMF_DQ_Att",1);
    
    switchParam2BasicMode("TO_F0",15);
    switchParam2BasicMode("TO_Att",1);
    
    switchParam2BasicMode("STARTUP_MERG_SPEED",speedMerging);
    switchParam2BasicMode("STARTUP_MERG_COEFF",100);

}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/

/* --------------- Spee parameters routine ---------------- */
function clickCalculateBemfDqObsrv()
{
    var retMsg;
    var prefixM       = getActiveMotor();
    var BEMF_DQ_f0    = getParentHtmlValue("BEMF_DQ_F0");
    var BEMF_DQ_att   = getParentHtmlValue("BEMF_DQ_Att");
    var TO_f0         = getParentHtmlValue("TO_F0");
    var TO_att        = getParentHtmlValue("TO_Att");
    var Ts            = getParentHtmlValue("CLOOP_Ts");
    var SLOOP_Ts      = getParentHtmlValue("SLOOP_Ts");
    var Rs            = getParentHtmlValue("Rs");
    var Ld            = getParentHtmlValue("Ld");
    var Lq            = getParentHtmlValue("Lq");
    var ke            = getParentHtmlValue("ke");      
    var UDCmax        = getParentHtmlValue("UDC_max");
    var Imax          = getParentHtmlValue("I_max");
    var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
    var Nmax          = getParentHtmlValue("N_max");
    var Nmin          = getParentHtmlValue("N_min"); 
    var Emax          = getParentHtmlValue("E_max");
    var pp            = getParentHtmlValue("pp");
    var RampStartUp   = getParentHtmlValue("STARTUP_RAMP");
    var IstartUp      = getParentHtmlValue("STARTUP_CURRENT");
    var Nmerging      = getParentHtmlValue("STARTUP_MERG_SPEED");
    var mergingCoeff  = getParentHtmlValue("STARTUP_MERG_COEFF");
    var IIRxCoefsScaleType     = parent.document.getElementById("IIRxCoefsScale").innerText;
    var Umax          = Math.round((UDCmax/UmaxCoeff)*10)/10;
    
    //var Emax          = ke*Wmax; 
    var ThetaMax      = Math.PI;  
    var ERRmax        = 1;
    var Wmax        = 2*Math.PI*pp*Nmax/60;

    /* Ensure that merging speed is higher than minimal speed */
    if(Nmerging < Nmin)
    {
      setParentHtmlValue((prefixM + "STARTUP_MERG_SPEED"), Nmin);
      copyParent2InnerValById("STARTUP_MERG_SPEED");
      Nmerging = Nmin;
      alert("Merging speed must be greater or equal to minimal speed.");
    }
    
    I_scaled = Math.ceil((Ld/(Ld+Ts*Rs))*1000000000000)/1000000000000;
    U_scaled = Math.ceil(Ts/(Ld+Ts*Rs)*(Umax/Imax)*1000000000000)/1000000000000;
    E_scaled = Math.ceil(Ts/(Ld+Ts*Rs)*(Emax/Imax)*1000000000000)/1000000000000;
    WI_scaled = Math.ceil(Lq*Ts/(Ld+Ts*Rs)*Wmax*1000000000000)/1000000000000;
    
    /* BEMF DQ observer constants */
    BemfDQ_Kps = 2*BEMF_DQ_att*2*Math.PI*BEMF_DQ_f0*Ld-Rs;
    BemfDQ_Kis = Ld*Math.pow(2*Math.PI*BEMF_DQ_f0,2);
    
    BemfDQ_Kpz = BemfDQ_Kps;
    BemfDQ_Kiz = BemfDQ_Kis*Ts;
    
    BemfDQ_Kpz_f = BemfDQ_Kpz*(Imax/Emax);
    BemfDQ_Kiz_f = BemfDQ_Kiz*(Imax/Emax);
    
    /* Tracking Observer observer constants */
    TO_Kps = 2*TO_att*2*Math.PI*TO_f0;
    TO_Kis = Math.pow(2*Math.PI*TO_f0,2);
    
    TO_Kpz = TO_Kps;
    TO_Kiz = TO_Kis*Ts;
    
    TO_Kpz_f = TO_Kpz*(ERRmax/Wmax);
    TO_Kiz_f = TO_Kiz*(ERRmax/Wmax);
   
    /* TO scales */
    // proportional
    if(TO_Kpz_f<1) TO_Kp_shift = -Math.ceil(Math.log(Math.abs(1/TO_Kpz_f))/Math.log(2)-1);
    else          TO_Kp_shift = Math.ceil(Math.log(Math.abs(TO_Kpz_f))/Math.log(2));
    
    TO_Kp_gain = Math.round(TO_Kpz_f*Math.pow(2,-TO_Kp_shift)*1000000000000)/1000000000000;
    testFracValRange("TO_Kp_gain",TO_Kp_gain); 
   
    // integral
    if(TO_Kiz_f<1) TO_Ki_shift = -Math.ceil(Math.log(Math.abs(1/TO_Kiz_f))/Math.log(2)-1);
    else          TO_Ki_shift = Math.ceil(Math.log(Math.abs(TO_Kiz_f))/Math.log(2));
    
    TO_Ki_gain = Math.round(TO_Kiz_f*Math.pow(2,-TO_Ki_shift)*1000000000000)/1000000000000;
    testFracValRange("TO_Ki_gain",TO_Ki_gain); 
    
    // theta
    TO_Theta_f = Ts*Wmax/ThetaMax;
    
    if(TO_Theta_f<1) TO_Theta_shift = -Math.ceil(Math.log(Math.abs(1/TO_Theta_f))/Math.log(2)-1);
    else          TO_Theta_shift = Math.ceil(Math.log(Math.abs(TO_Theta_f))/Math.log(2));
    
    TO_Theta_gain = Math.round(TO_Theta_f*Math.pow(2,-TO_Theta_shift)*1000000000000)/1000000000000;
    testFracValRange("TO_Theta_gain",TO_Theta_gain); 
    
    //Start-up calcs
    rampStartUpInc    =  Math.round(RampStartUp/60*pp*2*Math.PI/Wmax*Ts*1000000000000)/1000000000000;
    currentStartUp    =  IstartUp;
    currentStartUp_f  =  Math.round(IstartUp/Imax*1000000000000)/1000000000000;
    speedMerging_f    =  Math.round(Nmerging/Nmax*1000000000000)/1000000000000;
    speedMerging    = Nmerging;
    merginCoeff_f     = Math.round((mergingCoeff/100)*(60/(pp*Nmerging))/Ts/2/32768*1000000000000)/1000000000000;
    testFracValRange("currentStartUp_f",currentStartUp_f); 
    testFracValRange("speedMerging_f",speedMerging_f);
    testValRange("Merging Coeff",merginCoeff_f,1/32767,1); 
    
    // Observer speed output IIR filter
    // Cut off frequency is 80% of half of SPEED LOOP frequency
    TO_W_IIR_cutoff_freq = 1/(2*SLOOP_Ts)*0.8; 
    TO_W_IIR_B0_fl = Math.round((2*Math.PI*TO_W_IIR_cutoff_freq*Ts)/(2+(2*Math.PI*TO_W_IIR_cutoff_freq*Ts))*1000000000000)/1000000000000; 
    TO_W_IIR_B1_fl = Math.round((2*Math.PI*TO_W_IIR_cutoff_freq*Ts)/(2+(2*Math.PI*TO_W_IIR_cutoff_freq*Ts))*1000000000000)/1000000000000;
    TO_W_IIR_A1_fl = Math.round((2*Math.PI*TO_W_IIR_cutoff_freq*Ts-2)/(2+(2*Math.PI*TO_W_IIR_cutoff_freq*Ts))*1000000000000)/1000000000000;
    TO_W_IIR_B0_out =  Math.round(4.0*TO_W_IIR_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    TO_W_IIR_B1_out =  Math.round(4.0*TO_W_IIR_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    TO_W_IIR_A1_out =  Math.round(-4.0*TO_W_IIR_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    testFracValRange("TO_W_IIR_B0_out",TO_W_IIR_B0_out,1);
    testFracValRange("TO_W_IIR_B1_out",TO_W_IIR_B1_out,1);
    testFracValRange("TO_W_IIR_A1_out",TO_W_IIR_A1_out,1);
   
    // If BEMF OBSRV LOOP tab is active ******************************************
    if(document.getElementById("BemfDqObsrv") != undefined)
    {
        // write values to forms
        setInnerHtmlValue("I_scale",I_scaled,I_scaled);
        setInnerHtmlValue("U_scale",U_scaled,U_scaled);
        setInnerHtmlValue("E_scale",E_scaled,E_scaled);  
        setInnerHtmlValue("WI_scale",WI_scaled,WI_scaled);
       
        setInnerHtmlValue("BEMF_DQ_Kp_g",BemfDQ_Kps,BemfDQ_Kps);
        setInnerHtmlValue("BEMF_DQ_Ki_g",BemfDQ_Kis,BemfDQ_Kis);
       
        setInnerHtmlValue("TO_Kp_g",TO_Kps,TO_Kp_gain);
        setInnerHtmlValue("TO_Ki_g",TO_Kis,TO_Ki_gain);
        setInnerHtmlValue("Theta_g",TO_Theta_gain, TO_Theta_gain);
        
        setInnerHtmlValue("Startup_Ramp_inc",rampStartUpInc,rampStartUpInc);
        setInnerHtmlValue("Startup_I_scaled",currentStartUp_f,currentStartUp_f);
        setInnerHtmlValue("Startup_N_merging",speedMerging_f,speedMerging_f);
        setInnerHtmlValue("Startup_coeff_merging",merginCoeff_f,merginCoeff_f);
        
        setInnerHtmlValue("Startup_I_scaled_FM",IstartUp,IstartUp);
    }
    
    // If HEADER FILE tab is active ********************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {	
      // write values to forms in current Html page
      setInnerHtmlValueAsText("I_SCALE",10,I_scaled);
      setInnerHtmlValueAsText("U_SCALE",10,U_scaled);
      setInnerHtmlValueAsText("E_SCALE",10,E_scaled);
      setInnerHtmlValueAsText("WI_SCALE",10,WI_scaled);
      
      setInnerHtmlValueAsText("BEMF_DQ_KP_GAIN",10,BemfDQ_Kpz_f);
      setInnerHtmlValueAsText("BEMF_DQ_KI_GAIN",10,BemfDQ_Kiz_f);
      setInnerHtmlValueAsText("TO_KP_GAIN",4,TO_Kp_gain);
      setInnerHtmlValueAsText("TO_KP_SHIFT",2,TO_Kp_shift);
      setInnerHtmlValueAsText("TO_KI_GAIN",4,TO_Ki_gain);
      setInnerHtmlValueAsText("TO_KI_SHIFT",2,TO_Ki_shift);
      setInnerHtmlValueAsText("TO_THETA_GAIN",4,TO_Theta_gain);
      setInnerHtmlValueAsText("TO_THETA_SHIFT",2,TO_Theta_shift);
      setInnerHtmlValueAsText("TO_SPEED_IIR_B0",7,TO_W_IIR_B0_out,TO_W_IIR_B0_out);
      setInnerHtmlValueAsText("TO_SPEED_IIR_B1",7,TO_W_IIR_B1_out,TO_W_IIR_B1_out);
      setInnerHtmlValueAsText("TO_SPEED_IIR_A1",7,TO_W_IIR_A1_out,TO_W_IIR_A1_out); 
      setInnerHtmlValueAsText("OL_START_RAMP_INC",7,rampStartUpInc);
      setInnerHtmlValueAsText("OL_START_I",3,currentStartUp_f);
      setInnerHtmlValueAsText("MERG_SPEED_TRH",3,speedMerging_f);
      setInnerHtmlValueAsText("MERG_COEFF",3,merginCoeff_f); 
    }   
 }
 
/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickUpdatePoSpeBemfDQFM()
{
      if(!pcm.IsCommPortOpen())
  {
      alert("Communication is stopped.\nPress Ctrl+K to start the communication");
  }
  else
  {
    xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml"); 
     var errorArray = [];
     
    // // calculate actual constant values
    clickCalculateBemfDqObsrv(); 
     
    errorArray.push(UpdateFMVariable(xmlDoc,'E_scale',E_scaled));
    errorArray.push(UpdateFMVariable(xmlDoc,'I_scale',I_scaled));
    errorArray.push(UpdateFMVariable(xmlDoc,'U_scale',U_scaled));
    errorArray.push(UpdateFMVariable(xmlDoc,'WI_scale',WI_scaled));
    errorArray.push(UpdateFMVariable(xmlDoc,'BEMF_DQ_Kp_g',BemfDQ_Kpz_f));
    errorArray.push(UpdateFMVariable(xmlDoc,'BEMF_DQ_Ki_g',BemfDQ_Kiz_f));
    errorArray.push(UpdateFMVariable(xmlDoc,'TO_Kp_g',TO_Kp_gain));
    errorArray.push(UpdateFMVariable(xmlDoc,'TO_Ki_g',TO_Ki_gain));
    errorArray.push(UpdateFMVariable(xmlDoc,'TO_Kp_sc',TO_Kp_shift));
    errorArray.push(UpdateFMVariable(xmlDoc,'TO_Ki_sc',TO_Ki_shift));
    errorArray.push(UpdateFMVariable(xmlDoc,'Theta_g',TO_Theta_gain));
    errorArray.push(UpdateFMVariable(xmlDoc,'Theta_sc',TO_Theta_shift));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_Ramp_dec',rampStartUpInc));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_Ramp_inc',rampStartUpInc));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_I_scaled_FM',currentStartUp));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_N_merging',speedMerging));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_coeff_merging',merginCoeff_f));        

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
function writePoSpeBemfDQHTMLOutput(prefix,xmlObject)
{ 
  // Speed Loop Control
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("Sensorless BEMF DQ Observer","",""));
    document.write(HTML_write_comment_line_dash()); 
    document.write(HTML_write_comment_line("Loop bandwidth","BEMF_DQ_F0",""));
    document.write(HTML_write_comment_line("Loop attenuation","BEMF_DQ_Att",""));
    document.write(HTML_write_comment_line("Loop sample time","BEMF_DQ_Ts",""));
    
    document.write(HTML_write_define_line_number(prefix,0,"I_SCALE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_SCALE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"E_SCALE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"WI_SCALE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"BEMF_DQ_KP_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"BEMF_DQ_KI_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_KP_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_KP_SHIFT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_KI_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_KI_SHIFT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_THETA_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_THETA_SHIFT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_SPEED_IIR_B0",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_SPEED_IIR_B1",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TO_SPEED_IIR_A1",xmlObject));
    
    // open loop start-up
    document.write(HTML_write_define_line_number(prefix,0,"OL_START_RAMP_INC",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"OL_START_I",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"MERG_SPEED_TRH",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"MERG_COEFF",xmlObject));
    
    
    copyParent2HeaderCfgById('BEMF_DQ_F0','BEMF_DQ_F0',' [Hz]',true);
    copyParent2HeaderCfgById('BEMF_DQ_Att','BEMF_DQ_Att',' [-]',true);
    copyParent2HeaderCfgById('BEMF_DQ_Ts','CLOOP_Ts',' [sec]',true);
    clickCalculateBemfDqObsrv();
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writePoSpeBemfDHeaderOutput(prefix, str)
{
   str = write_blank_lines(str,1);     
   str = write_comment_text(str,'Sensorless BEMF DQ and Tracking Observer','');
   str = write_comment_line_dash(str);
   str = write_comment_text(str,'Loop bandwidth','BEMF_DQ_F0');    
   str = write_comment_text(str,'Loop attenuation','BEMF_DQ_Att');
   str = write_comment_text(str,'Loop sample time','BEMF_DQ_Ts');
   str = write_comment_line_dash(str);
 
   //Bemf DQ Observer
   str = write_comment_text(str,'Bemf DQ Observer','','');
   str = write_define_line_number(prefix, str,'I_SCALE');
   str = write_define_line_number(prefix, str,'U_SCALE');
   str = write_define_line_number(prefix, str,'E_SCALE');
   str = write_define_line_number(prefix, str,'WI_SCALE');
   
   str = write_define_line_number(prefix, str,'BEMF_DQ_KP_GAIN');
   str = write_define_line_number(prefix, str,'BEMF_DQ_KI_GAIN');
   str = write_blank_lines(str,1);
   //Tracking Observer
   str = write_comment_text(str,'Bemf DQ Observer','','');
   str = write_define_line_number(prefix, str,'TO_KP_GAIN');
   str = write_define_line_number(prefix, str,'TO_KP_SHIFT');
   str = write_define_line_number(prefix, str,'TO_KI_GAIN');
   str = write_define_line_number(prefix, str,'TO_KI_SHIFT');
   str = write_define_line_number(prefix, str,'TO_THETA_GAIN');
   str = write_define_line_number(prefix, str,'TO_THETA_SHIFT');
   
   //Open loop start up
   str = write_comment_text(str,'Observer speed output filter','','');
   str = write_define_line_number(prefix, str,'TO_SPEED_IIR_B0');
   str = write_define_line_number(prefix, str,'TO_SPEED_IIR_B1');
   str = write_define_line_number(prefix, str,'TO_SPEED_IIR_A1');
   
   //Open loop start up
   str = write_comment_text(str,'Open loop start-up','','');
   str = write_define_line_number(prefix, str,'OL_START_RAMP_INC');
   str = write_define_line_number(prefix, str,'OL_START_I');
   str = write_define_line_number(prefix, str,'MERG_SPEED_TRH');
   str = write_define_line_number(prefix, str,'MERG_COEFF');
    
    return str;
}

/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_BemfDqObsrv()
{
   // update constants
   clickCalculateBemfDqObsrv();
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
