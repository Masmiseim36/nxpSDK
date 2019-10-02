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
* @file   inner_PoSpeSensor.js
*
* @brief  Position and Speed engine
* 
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormPoSpe() - init Position and Speed page paramters and constants  
* clickCalculatePoSpe() - calculates control constants based on input parameters
* clickUpdatePoSpeFM() - update selected variables in FreeMASTER application
* writePoSpeHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writePoSpeHeaderOutput(str) - write selected constants to output header file
*******************************************************************************/
/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID 
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormPoSpe()
{
   var ControllerType = null;
   
   //get active motor to selct proper prefix
    var prefixM = getActiveMotor();

    // add prefix to var ID
    if(prefixM!='')
    {
       var PositionType = parent.document.getElementById(prefixM + 'PospeFbck').innerText;
    }
   
    // in basic mode, precalculate paramters
    if(getActiveMode()==0)
      basicModeCalcPoSpe();
   
   copyParent2InnerValById("ENC_P");
   copyParent2InnerValById("RES_PP");

   copyParent2InnerValById("POSPE_Ts");
   copyParent2InnerValById("ATO_F0");   
   copyParent2InnerValById("ATO_Att");
   
   // display only required type of Position Sensor
   document.getElementById("Encoder").style.display = "none";
   document.getElementById("Resolver").style.display = "none";
    
    //encoder
    if((PositionType>>>0)&1)
        document.getElementById("Encoder").style.display = "block";
    // resolver
    if((PositionType>>>1)&1)
        document.getElementById("Resolver").style.display = "block";
           
   // display only required type of PI controller
    if(testVarValue('TOcontroller','Parallel'))
    {
        document.getElementById("PoSpe_PIparal").style.display = "block";
        document.getElementById("PoSpe_PIrecur").style.display = "none";
        ControllerType = 'Parallel';       
    }
    else
    {
        document.getElementById("PoSpe_PIparal").style.display = "none";
        document.getElementById("PoSpe_PIrecur").style.display = "block";
        ControllerType = 'Reccurent';
    }        
   
   //document.getElementById("Arithmetic").innerText = parent.document.getElementById("Arithmetic").innerText;
   document.getElementById("TOcontroller").innerText = ControllerType;  
   
   // enable button enabling
   ReloadStoreButtonsOnOff(1);
   
   //calculate constants
   clickCalculatePoSpe();
}

/***************************************************************************//*!
*
* @brief   Mark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function markUpdatePospeField()
{
    /* DISABLED in current MCAT version */
    /*
    var obj = document.getElementById("PoSpe_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    var obj = document.getElementById("PoSpe_PIrecur").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';
    var obj = document.getElementById("PoSpe_Integ").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabelred';      
    
    document.getElementById("PoSpeUpdateFrm").title = "Red-legend constants will be updated on click"
    */
}

/***************************************************************************//*!
* @brief   UnMark the Labels in the Fieldsets that's going to be updated
* @param   
* @return  None
* @remarks 
******************************************************************************/
function unmarkUpdatePospeField()
{
    var obj = document.getElementById("PoSpe_PIparal").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
    var obj = document.getElementById("PoSpe_PIrecur").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel';
    var obj = document.getElementById("PoSpe_Integ").getElementsByTagName('legend')[0];
    obj.className = 'fontControlLabel'    
}

/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcPoSpe()
{
    var Ts            = getParentHtmlValue("POSPE_Ts");
    
    // replace and disable params
    switchParam2BasicMode("POSPE_Ts",Ts);
    switchParam2BasicMode("ATO_F0",150);
    switchParam2BasicMode("ATO_Att",1);

}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickCalculatePoSpe()
{
    var POSPE_Ts    = getParentHtmlValue("POSPE_Ts");
    var ATO_F0      = getParentHtmlValue("ATO_F0");
    var ATO_Att     = getParentHtmlValue("ATO_Att");       
    var RES_PP      = getParentHtmlValue("RES_PP");
    var ENC_P      = getParentHtmlValue("ENC_P");
    var pp          = getParentHtmlValue("pp");
    var Nmax        = getParentHtmlValue("N_max");
    var Wmax        = 2*Math.PI*pp*Nmax/60;
    
    
    //////////////////////// Related to POSITION OBSERVER ///////////////////			
	/* check preset discretization method */
    var DiscMethod = parent.document.getElementById("DiscMethod").innerText;
    /* set default value for backward Euler due to library function incompatibility */
    var DiscMethodFactor = 1;
    
    /* original version */
    //ATO_Kps = Math.round(2*ATO_Att*2*Math.PI*ATO_F0/RES_PP*1000000000000)/1000000000000;	
    //ATO_Kis = Math.round(Math.pow((2*Math.PI*ATO_F0),2)/RES_PP*1000000000000)/1000000000000;	
    /* IMM MC version  */
    ATO_Kps = Math.round(2*ATO_Att*2*Math.PI*ATO_F0*1000000000000)/1000000000000;	
    ATO_Kis = Math.round(Math.pow((2*Math.PI*ATO_F0),2)*1000000000000)/1000000000000;

	  ATO_Kps_frac = ATO_Kps*Math.PI/Wmax;	
    ATO_Kis_frac = ATO_Kis*POSPE_Ts*Math.PI/Wmax/DiscMethodFactor;
		
  	if(ATO_Kps_frac>1){
	    ATO_Kps_sc = Math.ceil(Math.log(Math.abs(ATO_Kps_frac))/Math.log(2));
    }
    else{
       ATO_Kps_sc = Math.ceil(Math.log(Math.abs(1/ATO_Kps_frac))/Math.log(2)-1);
    }
    
	if(ATO_Kis_frac>1){
	   ATO_Kis_sc = Math.ceil(Math.log(Math.abs(ATO_Kis_frac))/Math.log(2));
    }
    else{
       ATO_Kis_sc = -Math.ceil(Math.log(Math.abs(1/ATO_Kis_frac))/Math.log(2)-1);
    } 			
    
    ATO_CC1s =  ATO_Kps + ATO_Kis*POSPE_Ts/DiscMethodFactor;
    ATO_CC2s = -ATO_Kps + ATO_Kis*POSPE_Ts/DiscMethodFactor;
   
    ATO_CC1f = ATO_CC1s*Math.PI/Wmax;
    ATO_CC2f = ATO_CC2s*Math.PI/Wmax;
    
    if ((Math.abs(ATO_CC1f)<1) && (Math.abs(ATO_CC2f)<1)){
      ATO_Nshift = 0;
    }
    else{
      if (Math.abs(ATO_CC1f) > Math.abs(ATO_CC2f)){
      ATO_Nshift = Math.ceil(Math.log(Math.abs(ATO_CC1f))/Math.log(2));
      }
      else{
      ATO_Nshift = Math.ceil(Math.log(Math.abs(ATO_CC2f))/Math.log(2));        
      }
    }

    RES_PPsc = Math.ceil(Math.log(Math.abs(RES_PP))/Math.log(2)); 
    RES_PPfr = Math.round(RES_PP/Math.pow(2,RES_PPsc)*1000000000000)/1000000000000;
          
	  ATO_Kp_gain  = Math.round(ATO_Kps_frac/Math.pow(2,ATO_Kps_sc)*1000000000000)/1000000000000;
    ATO_Ki_gain  = Math.round(ATO_Kis_frac/Math.pow(2,ATO_Kis_sc)*1000000000000)/1000000000000;
    testFracValRange("ATO_Kp_gain",ATO_Kp_gain);
    testFracValRange("ATO_Ki_gain",ATO_Ki_gain);

	  //	ATO_Nsh   = ATO_Nshift;
    ATO_CC1_calc   = Math.round(ATO_CC1f/Math.pow(2,ATO_Nshift)*1000000000000)/1000000000000;
		ATO_CC2_calc   = Math.round(ATO_CC2f/Math.pow(2,ATO_Nshift)*1000000000000)/1000000000000;
    testFracValRange("ATO_CC1_calc",ATO_CC1_calc);
    testFracValRange("ATO_CC2_calc",ATO_CC2_calc,1);

    Kint = Wmax*POSPE_Ts/Math.PI/DiscMethodFactor;
            
    if (Kint>=(1-1/Math.pow(2,31)))
    {
	   Kint_sc = Math.ceil(Math.log(Math.abs(Kint))/Math.log(2));
    }
    else
    {
       Kint_sc = 0;
    }
    
		Int_Nsh  = Kint_sc;
    Int_C1   = Math.round(Kint/Math.pow(2,Kint_sc)*1000000000000)/1000000000000;
    testFracValRange("Int_C1",Int_C1);
    
    // Encoder scale constant calculation
    Enc_edge_per_half_el_rev_f = 32768/((ENC_P*4/pp)/2);
    Enc_edge_per_half_mech_rev_f = 32768/((ENC_P*4)/2); 
    
    Enc_el_rev_sc = Math.ceil(Math.log(Math.abs(Enc_edge_per_half_el_rev_f))/Math.log(2));
    Enc_mech_rev_sc = Math.ceil(Math.log(Math.abs(Enc_edge_per_half_mech_rev_f))/Math.log(2));
    
    Enc_el_rev_gain  = Math.round(Enc_edge_per_half_el_rev_f/Math.pow(2,Enc_el_rev_sc)*1000000000000)/1000000000000;
    Enc_mech_rev_gain  = Math.round(Enc_edge_per_half_mech_rev_f/Math.pow(2,Enc_mech_rev_sc)*1000000000000)/1000000000000;
    
    
    // If POSPE tab is active ******************************************
    if(document.getElementById("PoSpe") != undefined)
    {
      // write values to forms in current Html page
      setInnerHtmlValue("ATO_Kp_g",ATO_Kps,ATO_Kp_gain);
      setInnerHtmlValue("ATO_Ki_g",ATO_Kis,ATO_Ki_gain);
      
      //setInnerHtmlValue("ATO_Nsh", ATO_Nshift);
      setInnerHtmlValue("ATO_CC1", ATO_CC1s,ATO_CC1s);
      setInnerHtmlValue("ATO_CC2", ATO_CC1s,ATO_CC2s);
             
      setInnerHtmlValue("Integ_g", Int_C1,Int_C1);
   	}
   
    // If HEADER FILE tab is active ********************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {	
      var PositionType = parent.document.getElementById(prefixM + 'PospeFbck').innerText;
      
      if(testVarValue('TOcontroller','Parallel')) // parallel type of PI controller
      {
        setInnerHtmlValueAsText("POSPE_KP_GAIN",0,ATO_Kp_gain,ATO_Kps_frac); 
        setInnerHtmlValueAsText("POSPE_KI_GAIN",0,ATO_Ki_gain,ATO_Kis_frac);
        setInnerHtmlValueAsText("POSPE_KP_SC",1,ATO_Kps_sc,'N/A');
        setInnerHtmlValueAsText("POSPE_KI_SC",1,ATO_Kis_sc,'N/A');
      }
      else
      {
        setInnerHtmlValueAsText("POSPE_NSHIFT",2,ATO_Nshift,'N/A');
        setInnerHtmlValueAsText("POSPE_CC1",0,ATO_CC1_calc,ATO_CC1s);
        setInnerHtmlValueAsText("POSPE_CC2",0,ATO_CC2_calc,ATO_CC2s);      
      }
      
      setInnerHtmlValueAsText("POSPE_INTEG_GAIN",0,Int_C1,Kint);
      setInnerHtmlValueAsText("POSPE_INTEG_SC",1,Int_Nsh);
      
      // Encoder
      if((PositionType>>>0)&1)
      {
          setInnerHtmlValueAsText("POSPE_ENC_PULSES",1,ENC_P,'N/A');
          setInnerHtmlValueAsText("POSPE_EL_POS_GAIN",3,Enc_el_rev_gain); 
          setInnerHtmlValueAsText("POSPE_MECH_POS_GAIN",3,Enc_mech_rev_gain);
          setInnerHtmlValueAsText("POSPE_EL_POS_SC",2,Enc_el_rev_sc);
          setInnerHtmlValueAsText("POSPE_MECH_POS_SC",2,Enc_mech_rev_sc);
      }
      
      // Resolver
      if((PositionType>>>1)&1)
      {       
          setInnerHtmlValueAsText("POSPE_RES_PP_GAIN",0,RES_PPfr,RES_PP);
          setInnerHtmlValueAsText("POSPE_RES_PP_SC",2,RES_PPsc,'N/A');            
      }
    }
  } 

/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickUpdatePoSpeFM()
{
  if(!pcm.IsCommPortOpen())
  {
      alert("Communication is stopped.\nPress Ctrl+K to start the communication");
  }
  else
  {
    xmlDoc=loadXMLDoc("xml_files\\FM_params_list.xml"); 
    var errorArray = [];
    var MotorPrefix = getActiveMotor();
    
    // calculate actual constant values 
    clickCalculatePoSpe();
    var aritType     = parent.document.getElementById("Arithmetic").innerText; 
    
    var selectedSensor = xmlDoc.getElementsByTagName([MotorPrefix+ "ControlStructurePoSpe"])[0];
    
    if(pcm.ReadVariable(selectedSensor.childNodes[0].nodeValue))
	var sensorType = pcm.LastVariable_vValue;

    if(testVarValue('TOcontroller','Parallel')) // parallel type of PI controller
    {    
      errorArray.push(UpdateFMVariable(xmlDoc,'ATO_Kp_g',ATO_Kp_gain,ATO_Kps_frac));
      errorArray.push(UpdateFMVariable(xmlDoc,'ATO_Ki_g',ATO_Ki_gain,ATO_Kis_frac));
    }
    else
    {
      errorArray.push(UpdateFMVariable(xmlDoc,'ATO_CC1',ATO_CC1_calc,ATO_CC1f));
      errorArray.push(UpdateFMVariable(xmlDoc,'ATO_CC2',ATO_CC2_calc,ATO_CC2f));
    }
    
    errorArray.push(UpdateFMVariable(xmlDoc,'Integ_g',Int_C1,Int_C1));

    
    if (aritType!=='Float')
    {
      if(testVarValue('TOcontroller','Parallel')) // parallel type of PI controller
      {
      errorArray.push(UpdateFMVariable(xmlDoc,'ATO_Kp_sc',ATO_Kps_sc));    
      errorArray.push(UpdateFMVariable(xmlDoc,'ATO_Ki_sc',ATO_Kis_sc));
      }
      else
      errorArray.push(UpdateFMVariable(xmlDoc,'ATO_Nsh',ATO_Nshift));
      
      errorArray.push(UpdateFMVariable(xmlDoc,'Integ_sc',Int_Nsh));
    }            
    
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
function writePoSpeSensorHTMLOutput(prefix,xmlObject)
{ 
    var PositionType = parent.document.getElementById(prefixM + 'PospeFbck').innerText;
    
    
    // Position & Speed module
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("Position & Speed Sensors Module","",""));
    document.write(HTML_write_comment_line_dash()); 
    document.write(HTML_write_comment_line("Loop bandwidth","ATO_F0",""));
    document.write(HTML_write_comment_line("Loop attenuation","ATO_Att",""));
    document.write(HTML_write_comment_line("Loop sample time","ATO_Ts",""));
    
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("ATO input parameters","",""));
    
    if(testVarValue('TOcontroller','Parallel')) // parallel type of PI controller
    {
        document.write(HTML_write_define_line_number(prefix,0,"POSPE_KP_GAIN",xmlObject));
        document.write(HTML_write_define_line_number(prefix,1,"POSPE_KP_SC",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"POSPE_KI_GAIN",xmlObject));
        document.write(HTML_write_define_line_number(prefix,1,"POSPE_KI_SC",xmlObject));
    }
    else
    {
        document.write(HTML_write_define_line_number(prefix,0,"POSPE_CC1",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"POSPE_CC2",xmlObject));
        document.write(HTML_write_define_line_number(prefix,1,"POSPE_NSHIFT",xmlObject));        
    }
    
    document.write(HTML_write_define_line_number(prefix,0,"POSPE_INTEG_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"POSPE_INTEG_SC",xmlObject));
    
    if((PositionType>>>0)&1) // Encoder
    {
        document.write(HTML_write_define_line_number(prefix,1,"POSPE_ENC_PULSES",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"POSPE_EL_POS_GAIN",xmlObject));
        document.write(HTML_write_define_line_number(prefix,1,"POSPE_EL_POS_SC",xmlObject));
        document.write(HTML_write_define_line_number(prefix,0,"POSPE_MECH_POS_GAIN",xmlObject));
        document.write(HTML_write_define_line_number(prefix,1,"POSPE_MECH_POS_SC",xmlObject));
    }
    
      // Resolver
    if((PositionType>>>1)&1)
      {       
         document.write(HTML_write_define_line_number(prefix,0,"POSPE_RES_PP_GAIN",xmlObject));
         document.write(HTML_write_define_line_number(prefix,1,"POSPE_RES_PP_SC",xmlObject));           
      }
    
    copyParent2HeaderCfgById('ATO_F0','ATO_F0',' [Hz]',true);
    copyParent2HeaderCfgById('ATO_Att','ATO_Att',' [-]',true);
    copyParent2HeaderCfgById('ATO_Ts','CLOOP_Ts',' [sec]',true);
    clickCalculatePoSpe();
    
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writePoSpeSensorHeaderOutput(prefix, str)
{
   var PositionType = parent.document.getElementById(prefixM + 'PospeFbck').innerText;
  
   str = write_blank_lines(str,1);     
   str = write_comment_text(str,'Position & Speed Sensors Module','');
   str = write_comment_line_dash(str);
   str = write_comment_text(str,'Loop Bandwidth','ATO_F0');    
   str = write_comment_text(str,'Loop Attenuation','ATO_Att');
   str = write_comment_text(str,'Loop sample time','ATO_Ts');
   str = write_comment_line_dash(str);
 
   // Position & Speed module
   if(testVarValue('TOcontroller','Parallel')) // parallel type of PI controller
   {
      str = write_define_line_number(prefix, str,'POSPE_KP_GAIN'); 
      str = write_define_line_number(prefix, str,'POSPE_KP_SC');
      str = write_define_line_number(prefix, str,'POSPE_KI_GAIN');
      str = write_define_line_number(prefix, str,'POSPE_KI_SC');
    }
    else
    {
      str = write_define_line_number(prefix, str,'POSPE_NSHIFT');
      str = write_define_line_number(prefix, str,'POSPE_CC1');
      str = write_define_line_number(prefix, str,'POSPE_CC2');
    }
    
    str = write_define_line_number(prefix, str,'POSPE_INTEG_GAIN');
    str = write_define_line_number(prefix, str,'POSPE_INTEG_SC');
    
    if((PositionType>>>0)&1) // Encoder
    {
        str = write_define_line_number(prefix, str,'POSPE_ENC_PULSES');
        str = write_define_line_number(prefix, str,'POSPE_EL_POS_GAIN');
        str = write_define_line_number(prefix, str,'POSPE_EL_POS_SC');
        str = write_define_line_number(prefix, str,'POSPE_MECH_POS_GAIN');
        str = write_define_line_number(prefix, str,'POSPE_MECH_POS_SC');
    }
      // Resolver
    if((PositionType>>>1)&1)
      {       
        str = write_define_line_number(prefix, str,'POSPE_RES_PP_GAIN');
        str = write_define_line_number(prefix, str,'POSPE_RES_PP_SC');         
      }    
        
    return str;
}

/***************************************************************************//*!
*
* @brief  Unified function updating constants on active tab
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_PoSpe()
{
   // update constants
   clickCalculatePoSpe();
}
/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
    
