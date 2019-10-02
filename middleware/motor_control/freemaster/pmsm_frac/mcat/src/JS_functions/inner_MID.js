/*******************************************************************************
*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause* 
*
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormMID() - init motor resistance and inductance measurement page paramters and constants  
* clickCalculateMID() - calculates control constants based on input parameters
* clickUpdateScales() - update selected variables in FreeMASTER application
* writeMIDHTMLOutput(prefix,xmlObject) - write selected constants to output preview page 
* writeMIDHeaderOutput(str) - write selected constants to output header file
*******************************************************************************/

//Global for Warnings storage
var MeasWarnings;
var xmlDoc     = loadXMLDoc("xml_files\\FM_params_list.xml");

/***************************************************************************//*!
*
* @brief  The function loads values from inner storage table to forms based
*         on parameter ID 
* @param   
* @return 
* @remarks 
******************************************************************************/
function initLoadFormMID()
{
  if(!pcm.IsCommPortOpen())
  {
    alert("Communication is stopped. Press Ctrl+K to start the communication");
    Status.innerHTML = "Communication is stopped. Press Ctrl+K to start the communication";
    document.getElementById("Status").style.color = "red";
  }

  else
  {
    var ControllerType = null;
    var FastLoopTs;
   
    //get active motor to selct proper prefix
    prefixM = getActiveMotor();
    
    /* Select HW board */
    var HWboardID    = getParentHtmlValue("HW_board");
    HWboardSelector(HWboardID);
    
    /* get actual board Id if known board connected */
    var appId = parent.document.getElementById('M1typeTabName').innerHTML;
    
    // in basic mode 
    if(getActiveMode()==0)
        basicModeCalcMID();
    else
        document.getElementById("Params_expert").style.display = "";
    
    /* for known boards read defined sample time from target MCU */
    if(appId!="offline")
    {
        // replace and disable params
        reference_val = xmlDoc.getElementsByTagName([prefixM]+["Fast_Loop_Freq"])[0];
        if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue));
            FastLoopTs = Math.round(1/pcm.LastVariable_tValue*10000)/10000;
        
        switchParam2BasicMode("CLOOP_Ts",FastLoopTs);
        copyParent2InnerValById("CLOOP_Ts");
    }
    else
    {
        // Loop Sample Time
        copyParent2InnerValById("CLOOP_Ts");
    }
    
    copyParent2InnerValById("I_max");
    copyParent2InnerValById("UDC_max");
    copyParent2InnerValById("CLOOP_Ts");
    copyParent2InnerValById("N_req");
    copyParent2InnerValById("pp");
    
    copyParent2InnerValById("Rs_calib");
    copyParent2InnerValById("Id_calib");

    copyParent2InnerValById("Id_meas");
    copyParent2InnerValById("Id_ampl");

    copyParent2InnerValById("Freq_start");
    copyParent2InnerValById("Freq_min");
    copyParent2InnerValById("Ud_inc");
    copyParent2InnerValById("Freq_dec");
    
    Calibration = 0;
    
    //calculate constants
    clickCalculateMID();
    
    //Update FreeMASTER
    clickUpdateScales();
    
    //Disable buttons
    if(document.getElementById("CalibrateButton") != undefined)
    document.getElementById("CalibrateButton").disabled = false;
    if(document.getElementById("MeasureButton") != undefined)
    document.getElementById("MeasureButton").disabled = false;
    if(document.getElementById("PpButtonStart") != undefined)
    document.getElementById("PpButtonStart").disabled = false;
    if(document.getElementById("PpButtonStop") != undefined)
    document.getElementById("PpButtonStop").disabled = true;
    
    //Update status
    updateStatus();
    
    //Subscribe variables for watching
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Rs_meas"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Ld_meas"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Lq_meas"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Ke_meas"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_AlignActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_CharActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 100);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_RsActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_LsActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_KeActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_PpActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["state"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_state"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["onoff"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Fault"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_KRS_RS_MAX_SHIFT"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_ZD_MAX_SHIFT"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_ZQ_MAX_SHIFT"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_LD_MAX_SHIFT"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_LQ_MAX_SHIFT"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_KE_MAX_SHIFT"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
  }    
}

/***************************************************************************//*!
*
* @brief   Callback function that updates most of the paramerameters during
*          and after measurement 
* @param   
* @return  None
* @remarks 
******************************************************************************/
function Event_OnVariableChanged(varName)
{
    //If fault occured and stopped the application
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["state"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue) == "FAULT")
    {
      handleFault();
    }
  
    updateParams();
    calculateScalesMID();
    clickUpdateScales();
}

/***************************************************************************//*!
*
* @brief   Callback function that checks open/close communication and updates it
*          in MCAT 
* @param   
* @return  None
* @remarks 
******************************************************************************/
function Event_OnCommPortStateChanged(vPortOpen)
{
    updateStatus();
}

/***************************************************************************//*!
*
* @brief   Parameter Calculation in BASIC mode
* @param   
* @return  None
* @remarks 
******************************************************************************/
function basicModeCalcMID()
{  
    // replace and disable params
    switchParam2BasicMode("Freq_start",999);
    switchParam2BasicMode("Freq_min",400);
    switchParam2BasicMode("Freq_dec",100);
}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickCalculateMID()
{   
    //Global for FMSTR update
    Rs_calib_MCAT      = getParentHtmlValue("Rs_calib");
    Id_meas_MCAT       = getParentHtmlValue("Id_meas");
    Id_ampl_MCAT       = getParentHtmlValue("Id_ampl");
    f_start_MCAT       = getParentHtmlValue("Freq_start");
    f_min_MCAT         = getParentHtmlValue("Freq_min");
    
    //Global for checking whether controllers and observer have been calculated or not
    observer_done_MCAT    = 0;
    CharDataStored        = 1;
    MeasurementCompleted  = 1;
    
    //local
    var Imax           = getParentHtmlValue("I_max");
    var dTs            = getParentHtmlValue("CLOOP_Ts");
    var UmaxCoeff      = getParentHtmlValue("UmaxCoeff");
    var UDCbus_max     = getParentHtmlValue("UDC_max");
    var Umax           = Math.round(UDCbus_max/UmaxCoeff*10)/10;
    var Nmax           = getParentHtmlValue("N_req")*1.1;
    var pp             = getParentHtmlValue("pp");
    var Id_calib       = getParentHtmlValue("Id_calib");
    var HWboardID      = getParentHtmlValue("HW_board");
    
    /* Set calibration range +-2A for FSL power stages */
    if(HWboardID < 1)
    {
      setParentHtmlValue((prefixM + "Id_calib"), 2);
      copyParent2InnerValById("Id_calib");
      Id_calib = 2.0;
    }
    
    /* Ensure that Calibration range is inside +- Imax */
    if(Id_calib > Imax)
    {
      setParentHtmlValue((prefixM + "Id_calib"), Imax);
      copyParent2InnerValById("Id_calib");
      Id_calib = Imax;
      alert("Calibration range must be inside the I max.");
    }
    
    calculateScalesMID();
  
    /* intergration constant for position calculation from speed*/
    /* check preset discretization method */
    var DiscMethod = parent.document.getElementById("DiscMethod").innerText;
    var DiscMethodFactor = 1;
    if(DiscMethod=="Trapezoidal")
      DiscMethodFactor = 2;
    
    Kint = 2*Math.PI*pp*Nmax/60*dTs/DiscMethodFactor/Math.PI; 
    if (Kint>=(1-1/Math.pow(2,31)))
      Kint_sc = Math.ceil(Math.log(Math.abs(Kint))/Math.log(2));
    else
      Kint_sc = 0;
    
    Int_Nsh  = Kint_sc;
    Int_C1   = Math.round(Kint/Math.pow(2,Kint_sc)*1000000000000)/1000000000000;
    testFracValRange("Int_C1",Int_C1);
      
    //Generated #defines
    CHAR_NUMBER_OF_POINTS_BASE_def = 6;
    CHAR_CURRENT_POINT_NUMBERS_def = 65;
    CHAR_NUMBER_OF_POINTS_HALF_def = 32;
    TIME_50MS_def = Math.round(0.05/dTs);
    TIME_100MS_def = Math.round(0.1/dTs);
    TIME_300MS_def = Math.round(0.3/dTs);
    TIME_600MS_def = Math.round(0.6/dTs);
    TIME_1200MS_def = Math.round(1.2/dTs);
    TIME_2400MS_def = Math.round(2.4/dTs);
    
    //To be encapsulated into FRAC16()
    K_RESCALE_DCB_TO_PHASE_HALF_def = UDCbus_max/Umax/2;
    K_ANGLE_INCREMENT_def = 2*F_MAX*dTs;
    INV_MOD_INDEX_def = Math.sqrt(3)/2*Umax/UDCbus_max;
    K_I_50MA_def = 0.05/Imax;
    
    //Parameters limitations
    
    TestRangeTrim("Rs_calib", 0.3, 50, "\u03A9");
    TestRangeTrim("Id_meas", 0.05, Id_calib, "A");
    TestRangeTrim("Id_ampl", 0.05, Id_calib, "A");
    TestRangeTrim("Freq_start", 100, F_MAX-1, "Hz");
    TestRangeTrim("Freq_min", 100, F_MAX-1, "Hz");
    TestRangeTrim("Ud_inc", 0.1, 30, "V");
    TestRangeTrim("Freq_dec", 20, 200, "Hz");
    
    //Check if f_min is lower than f_start
    if(Number(getParentHtmlValue("Freq_min")) > Number(getParentHtmlValue("Freq_start")))
    {
      setParentHtmlValue((prefixM + "Freq_min"), Number(getParentHtmlValue("Freq_start")));
      copyParent2InnerValById("Freq_min");
      alert("Freq_min can't be lower than Freq_start");
    }  
    
    // If MID tab is active ******************************************
    if(document.getElementById("MID") != undefined)
    {
      
    }
    
    // If HEADER FILE tab is active ********************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {
      setInnerHtmlValueAsText("CHAR_NUMBER_OF_POINTS_BASE",1,CHAR_NUMBER_OF_POINTS_BASE_def, CHAR_NUMBER_OF_POINTS_BASE_def);
      setInnerHtmlValueAsText("CHAR_CURRENT_POINT_NUMBERS",1,CHAR_CURRENT_POINT_NUMBERS_def, CHAR_CURRENT_POINT_NUMBERS_def);
      setInnerHtmlValueAsText("CHAR_NUMBER_OF_POINTS_HALF",1,CHAR_NUMBER_OF_POINTS_HALF_def, CHAR_NUMBER_OF_POINTS_HALF_def);
      setInnerHtmlValueAsText("TIME_50MS",1,TIME_50MS_def, TIME_50MS_def);
      setInnerHtmlValueAsText("TIME_100MS",1,TIME_100MS_def, TIME_100MS_def);
      setInnerHtmlValueAsText("TIME_300MS",1,TIME_300MS_def, TIME_300MS_def);
      setInnerHtmlValueAsText("TIME_600MS",1,TIME_600MS_def, TIME_600MS_def);
      setInnerHtmlValueAsText("TIME_1200MS",1,TIME_1200MS_def, TIME_1200MS_def);
      setInnerHtmlValueAsText("TIME_2400MS",1,TIME_2400MS_def, TIME_2400MS_def);

      setInnerHtmlValueAsText("K_RESCALE_DCB_TO_PHASE_HALF",0,K_RESCALE_DCB_TO_PHASE_HALF_def, K_RESCALE_DCB_TO_PHASE_HALF_def);
      setInnerHtmlValueAsText("K_ANGLE_INCREMENT",0,K_ANGLE_INCREMENT_def, K_ANGLE_INCREMENT_def);
      setInnerHtmlValueAsText("INV_MOD_INDEX",0,INV_MOD_INDEX_def, INV_MOD_INDEX_def);
      setInnerHtmlValueAsText("K_I_50MA",0,K_I_50MA_def, K_I_50MA_def);
    }
}

/***************************************************************************//*!
*
* @brief  The function calculates ouput constans based on input parameters   
* @param   
* @return  None
* @remarks 
******************************************************************************/
function calculateScalesMID()
{   
    //local
    var Imax           = getParentHtmlValue("I_max");
    var dTs            = getParentHtmlValue("CLOOP_Ts");
    var UmaxCoeff      = getParentHtmlValue("UmaxCoeff");
    var UDCbus_max     = getParentHtmlValue("UDC_max");
    var Umax           = Math.round(UDCbus_max/UmaxCoeff*10)/10;
    var Nmax           = getParentHtmlValue("N_req")*1.1;
    var pp             = getParentHtmlValue("pp");
    var prefixM        = getActiveMotor();
    var xmlDoc         = loadXMLDoc("xml_files\\FM_params_list.xml");
    
    //Measurement scales calculation
    //R scale
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_KRS_RS_MAX_SHIFT"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    var R_SHIFT  = pcm.LastVariable_vValue;
    R_MAX    = (Umax/Imax)*Math.pow(2,R_SHIFT);
    
    //Zd scale
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_ZD_MAX_SHIFT"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    var ZD_SHIFT  = pcm.LastVariable_vValue;
    ZD_MAX    = (Umax/Imax)*Math.pow(2,ZD_SHIFT);
    
    //Zq scale
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_ZQ_MAX_SHIFT"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    var ZQ_SHIFT  = pcm.LastVariable_vValue;
    ZQ_MAX    = (Umax/Imax)*Math.pow(2,ZQ_SHIFT);
    
    //F scale
    F_MAX    = 1/dTs/10;
    
    //Ld scale
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_LD_MAX_SHIFT"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    var LD_SHIFT  = pcm.LastVariable_vValue;
    LD_MAX    = (ZD_MAX/(2*Math.PI*F_MAX))*Math.pow(2,LD_SHIFT);
    
    //Lq scale
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_LQ_MAX_SHIFT"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    var LQ_SHIFT  = pcm.LastVariable_vValue;
    LQ_MAX    = (ZQ_MAX/(2*Math.PI*F_MAX))*Math.pow(2,LQ_SHIFT);
    
    //KE scale
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_K_KE_MAX_SHIFT"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    var KE_SHIFT  = pcm.LastVariable_vValue;
    KE_MAX   = (Umax*60/(Nmax*pp*2*Math.PI))*Math.pow(2,KE_SHIFT);
}

/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function clickUpdateScales()
{ 
    var errorArray = [];

    errorArray.push(UpdateFMVariable(xmlDoc,'fm_R_MAX_scale',R_MAX,R_MAX));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_F_MAX_scale',F_MAX,F_MAX));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_LD_MAX_scale',LD_MAX,LD_MAX));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_LQ_MAX_scale',LQ_MAX,LQ_MAX));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_KE_MAX_scale',KE_MAX,KE_MAX));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_K_SpeedIntegGain',Kint,Kint));//Int_C1,Int_C1
    //errorArray.push(UpdateFMVariable(xmlDoc,'fm_K_SpeedIntegShift',Int_Nsh,Int_Nsh));

    // error message display is commented due to periodic call of this function 
    // which would cause annoying periodic pop-up when FMSTR communication is stopped
    //UpdateError(errorArray);
}    

/***************************************************************************//*!
*
* @brief  The function reads values from input forms, scales them and write 
*         to output HTML form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeMIDHTMLOutput(prefix,xmlObject)
{   
    // Motor identification module
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("Motor Identification Module","",""));
    document.write(HTML_write_comment_line_dash()); 
    
    document.write(HTML_write_define_line_number(prefix,1,"CHAR_NUMBER_OF_POINTS_BASE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"CHAR_CURRENT_POINT_NUMBERS",xmlObject)); 
    document.write(HTML_write_define_line_number(prefix,1,"CHAR_NUMBER_OF_POINTS_HALF",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"TIME_50MS",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"TIME_100MS",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"TIME_300MS",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"TIME_600MS",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"TIME_1200MS",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"TIME_2400MS",xmlObject));
    
    document.write(HTML_write_define_line_number(prefix,0,"K_RESCALE_DCB_TO_PHASE_HALF",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"K_ANGLE_INCREMENT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"INV_MOD_INDEX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"K_I_50MA",xmlObject));
    
    clickCalculateMID();
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeMIDHeaderOutput(prefix, str)
{
    str = write_blank_lines(str,1);     
    str = write_comment_text(str,'Motor Identification Module','');
    str = write_comment_line_dash(str);
 
    str = write_define_line_number(prefix, str,'CHAR_NUMBER_OF_POINTS_BASE'); 
    str = write_define_line_number(prefix, str,'CHAR_CURRENT_POINT_NUMBERS');
    str = write_define_line_number(prefix, str,'CHAR_NUMBER_OF_POINTS_HALF');
    str = write_define_line_number(prefix, str,'TIME_50MS');
    str = write_define_line_number(prefix, str,'TIME_100MS');
    str = write_define_line_number(prefix, str,'TIME_300MS');
    str = write_define_line_number(prefix, str,'TIME_600MS');
    str = write_define_line_number(prefix, str,'TIME_1200MS');
    str = write_define_line_number(prefix, str,'TIME_2400MS');

    str = write_define_line_number(prefix, str,'K_RESCALE_DCB_TO_PHASE_HALF');
    str = write_define_line_number(prefix, str,'K_ANGLE_INCREMENT');
    str = write_define_line_number(prefix, str,'INV_MOD_INDEX');
    str = write_define_line_number(prefix, str,'K_I_50MA');
   
    return str;
}

/***************************************************************************//*!
*
* @brief  Button Calibrate
* @param   
* @return 
* @remarks 
******************************************************************************/
function clickCalibrate()
{
  //Disable buttons
  if(document.getElementById("CalibrateButton") != undefined)
  document.getElementById("CalibrateButton").disabled = true;
  if(document.getElementById("MeasureButton") != undefined)
  document.getElementById("MeasureButton").disabled = true;
  if(document.getElementById("PpButtonStart") != undefined)
  document.getElementById("PpButtonStart").disabled = true;
  if(document.getElementById("PpButtonStop") != undefined)
  document.getElementById("PpButtonStop").disabled = true;
  
  var errorArray = [];
  var dTs        = getParentHtmlValue("CLOOP_Ts");
  
  var Imax          = getParentHtmlValue("I_max");
  var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
  var UDCbus_max    = getParentHtmlValue("UDC_max");
  var Umax           = Math.round(UDCbus_max/UmaxCoeff*10)/10;
  
  CharDataStored    = 0;
  
  Rs_calib_MCAT       = getParentHtmlValue("Rs_calib");
  Id_calib            = getParentHtmlValue("Id_calib");
  MID_align_I_MCAT    = -getParentHtmlValue("Id_meas");
  MID_align_T_MCAT    = 1/dTs;
  
  var R_SHIFT       = Math.ceil(Math.log(Rs_calib_MCAT/(Umax/Imax))/Math.log(2));
  R_MAX             = (Umax/Imax)*Math.pow(2,R_SHIFT);
  var Id_increment  = Id_calib*2/65;
  
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_R_MAX_scale',R_MAX,R_MAX));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_KCHAR_RS_MAX_SHIFT',R_SHIFT,R_SHIFT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_calib', Rs_calib_MCAT, Rs_calib_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Id_calib', Id_calib, Id_calib));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Id_increment', Id_increment, Id_increment));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignCurrent', MID_align_I_MCAT, MID_align_I_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignDuration', MID_align_T_MCAT, MID_align_T_MCAT));
  
  //errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_state', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Calibration', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_ElectricalParameters', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_PolePairs', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 1, 1));
  
  // display error message                           
  UpdateError(errorArray); 
}

/***************************************************************************//*!
*
* @brief  Button Store Data
* @param   
* @return 
* @remarks 
******************************************************************************/
function StoreCalibTable()
{ 
  var prefixM = getActiveMotor();   
  var str = "";

  reference_val = xmlDoc.getElementsByTagName("M1_fm_PWRSTG_LookUp")[0].childNodes[0];
  txt=reference_val.nodeValue;
  
  for (var i=0;i<65;i++)
  {
    if(pcm.ReadVariable([txt]+["["]+[i.toString()]+["]"]))
    {
      Ud = Math.round(pcm.LastVariable_vValue*1000)/1000;
      str = [str]+[Ud.toString()]+[" "];
    }
  }
  setParentHtmlValue([prefixM] + ["User_HW_CalibTable"],str);
  paramFileWriteData(prefixM);
}

/***************************************************************************//*!
*
* @brief  Button Load User Data
* @param   
* @return 
* @remarks 
******************************************************************************/
function LoadCalibTable()
{
  var reference;
  var prefixM = getActiveMotor();
  var errorArray = [];
  var HWboardID    = getParentHtmlValue("HW_board");
  
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Rs_Meas_LookUp"])[0].childNodes[0];
  reference=reference_val.nodeValue;
  
  if(HWboardID == 0)
    CharArray = getParentHtmlValueStore("FSL_HW_CalibTable").split(" ");
  
  if(HWboardID == 1)
    CharArray = getParentHtmlValueStore("User_HW_CalibTable").split(" ");
  
  for (var i=0;i<65;i++)
  {
    str = [reference] + ["["] + [i] + ["]"];
    errorArray.push(UpdateFMVariableDirect(str,Number(CharArray[i]),Number(CharArray[i])));
  }   
  // display error message                           
  UpdateError(errorArray);         
}

/***************************************************************************//*!
*
* @brief  Button Measure
* @param   
* @return 
* @remarks 
******************************************************************************/
function clickMeasure()
{
  //Disable buttons 
  if(document.getElementById("CalibrateButton") != undefined)
    document.getElementById("CalibrateButton").disabled = true;
  if(document.getElementById("MeasureButton") != undefined)
  document.getElementById("MeasureButton").disabled = true;
  if(document.getElementById("PpButtonStart") != undefined)
  document.getElementById("PpButtonStart").disabled = true;
  if(document.getElementById("PpButtonStop") != undefined)
  document.getElementById("PpButtonStop").disabled = true;

  var prefixM    = getActiveMotor();
  var errorArray = [];
  var dTs        = getParentHtmlValue("CLOOP_Ts");
  
  clickCalculateMID();
  clickUpdateScales();
  
  MeasurementCompleted = 0;
  MeasWarnings = 0; 
  
  observer_done_MCAT  = 0;
  Imax                = getParentHtmlValue("I_max");
  Id_calib            = getParentHtmlValue("Id_calib");
  f_start_MCAT        = getParentHtmlValue("Freq_start");
  f_min_MCAT          = getParentHtmlValue("Freq_min");
  Ud_inc_MCAT         = getParentHtmlValue("Ud_inc");
  f_dec_MCAT          = getParentHtmlValue("Freq_dec");
  MID_align_I_MCAT    = getParentHtmlValue("Id_meas");
  MID_align_T_MCAT    = 1/dTs;
  Id_meas_MCAT        = getParentHtmlValue("Id_meas");
  Id_ampl_MCAT        = getParentHtmlValue("Id_ampl");
  N_req_ke_MCAT       = getParentHtmlValue("N_req")/10;
  
  /* Calculation of rescaling constants for LUT */
  Rescale_Id_LUT_real = Imax*0.5/Id_calib;
  if(Rescale_Id_LUT_real<1) 
    Rescale_LUT_shift_MCAT = -Math.ceil(Math.log(Math.abs(1/Rescale_Id_LUT_real))/Math.log(2)-1);
  else          
    Rescale_LUT_shift_MCAT = Math.ceil(Math.log(Math.abs(Rescale_Id_LUT_real))/Math.log(2));
  
  Rescale_LUT_gain_MCAT = Math.round(Rescale_Id_LUT_real*Math.pow(2,-Rescale_LUT_shift_MCAT)*1000000000000)/1000000000000;
  if(Rescale_LUT_gain_MCAT == 1)
    Rescale_LUT_gain_MCAT = 0.99996;
    
  testFracValRange("Rescale_LUT_gain_MCAT",Rescale_LUT_gain_MCAT);
  
  /* update calib table in target */
  LoadCalibTable();
  
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Id_meas', Id_meas_MCAT, Id_meas_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Id_ampl', Id_ampl_MCAT, Id_ampl_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rescale_LUT_gain', Rescale_LUT_gain_MCAT, Rescale_LUT_gain_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rescale_LUT_shift', Rescale_LUT_shift_MCAT, Rescale_LUT_shift_MCAT));
  
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_FreqStart',f_start_MCAT,f_start_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_FreqMin',f_min_MCAT,f_min_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_FreqDec',f_dec_MCAT,f_dec_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_VoltInc',Ud_inc_MCAT,Ud_inc_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_IdReqOpenLoop',Id_meas_MCAT,Id_meas_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_SpeedElReq',N_req_ke_MCAT,N_req_ke_MCAT));
  
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_meas', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ld_meas', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lq_meas', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ke_meas', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignCurrent', MID_align_I_MCAT, MID_align_I_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignDuration', MID_align_T_MCAT, MID_align_T_MCAT));
  
  //errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_state', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Calibration', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_ElectricalParameters', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_PolePairs', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 1, 1));

  // display error message                           
  UpdateError(errorArray);          
}

/***************************************************************************//*!
*
* @brief  Button Click Pp start
* @param   
* @return 
* @remarks 
******************************************************************************/
function clickPpStart()
{
  //Disable/enable buttons
  if(document.getElementById("CalibrateButton") != undefined)
    document.getElementById("CalibrateButton").disabled = true;
  if(document.getElementById("MeasureButton") != undefined)
  document.getElementById("MeasureButton").disabled = true;
  if(document.getElementById("PpButtonStart") != undefined)
  document.getElementById("PpButtonStart").disabled = true;
  if(document.getElementById("PpButtonStop") != undefined)
  document.getElementById("PpButtonStop").disabled = false;

  var prefixM    = getActiveMotor();
  var errorArray = [];
  var dTs        = getParentHtmlValue("CLOOP_Ts");

  MID_align_I_MCAT    = getParentHtmlValue("Id_meas");
  MID_align_T_MCAT    = 1/dTs;
  Id_meas_MCAT        = getParentHtmlValue("Id_meas");
  N_req_pp_MCAT       = getParentHtmlValue("N_req")/20;
    
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_PpCurrent',Id_meas_MCAT,Id_meas_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_PpSpeed',N_req_pp_MCAT,N_req_pp_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignCurrent', MID_align_I_MCAT, MID_align_I_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignDuration', MID_align_T_MCAT, MID_align_T_MCAT));
  
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Calibration', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_ElectricalParameters', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_PolePairs', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 1, 1));

  // display error message
  UpdateError(errorArray);
}

/***************************************************************************//*!
*
* @brief  Button Click Pp stop
* @param   
* @return 
* @remarks 
******************************************************************************/
function clickPpStop()
{
  //Disable/enable buttons
  if(document.getElementById("CalibrateButton") != undefined)
    document.getElementById("CalibrateButton").disabled = false;
  if(document.getElementById("MeasureButton") != undefined)
  document.getElementById("MeasureButton").disabled = false;
  if(document.getElementById("PpButtonStart") != undefined)
  document.getElementById("PpButtonStart").disabled = false;
  if(document.getElementById("PpButtonStop") != undefined)
  document.getElementById("PpButtonStop").disabled = true;

  var prefixM    = getActiveMotor();
  var errorArray = [];
    
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_PpDetermined', 1, 1));

  // display error message
  UpdateError(errorArray);
}

/***************************************************************************//*!
*
* @brief   Calculate current controllers
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function calculateControllers()
{
  //Current controller variables
  var fo    = 300;
  var Att   = 1;
  var Ts    = getParentHtmlValue("CLOOP_Ts");
  var Rs    = getInnerHtmlValue("Rs_result");
  var Ld    = getInnerHtmlValue("Ld_result");
  var Lq    = getInnerHtmlValue("Lq_result");
  //mH -> H, rounding to uH
  Ld = Math.round(Ld*10000000)/10000000;
  Lq = Math.round(Lq*10000000)/10000000;
  var Imax  = getParentHtmlValue("I_max");
  var UmaxCoeff = getParentHtmlValue("UmaxCoeff");
  var UDCbus_max = getParentHtmlValue("UDC_max");
  var Umax           = Math.round(UDCbus_max/UmaxCoeff*10)/10;
  var Ulim  = getParentHtmlValue("CLOOP_LIM");
  var aritType = parent.document.getElementById("Arithmetic").innerText;
  var IIRxCoefsScaleType     = parent.document.getElementById("IIRxCoefsScale").innerText;
  
  /////////////////// Calculate Controllers /////////////////////////
  Ulim_out = Math.round(Ulim/100*100)/100;
  
  //********************** Related to D axis **********************				
  D_Kps = 2*Att*2*Math.PI*fo*Ld-Rs;
  D_Kis = Math.pow((2*Math.PI*fo),2)*Ld;
  D_Kpz = D_Kps;
  D_Kiz = D_Kis*Ts;
  
  //PARALLEL PI CONTROLLER TYPE
  D_Kpz_f = D_Kpz*Imax/Umax; 
  D_Kiz_f = D_Kiz*Imax/Umax;
  
  // scaling to scale and scale shift components
  // proportional               D_Nshift = Math.ceil(Math.log(Math.abs(D_CC1f))/Math.log(2));
  if(D_Kpz_f<1) D_Kp_shift = -Math.ceil(Math.log(Math.abs(1/D_Kpz_f))/Math.log(2)-1);
  else          D_Kp_shift = Math.ceil(Math.log(Math.abs(D_Kpz_f))/Math.log(2));
  
  D_Kp_gain = Math.round(D_Kpz_f*Math.pow(2,-D_Kp_shift)*1000000000000)/1000000000000;
  testFracValRange("D_Kp_gain",D_Kp_gain);
  
  // integral
  if(D_Kiz_f<1) D_Ki_shift = -Math.ceil(Math.log(Math.abs(1/D_Kiz_f))/Math.log(2)-1);
  else          D_Ki_shift = Math.ceil(Math.log(Math.abs(D_Kiz_f))/Math.log(2));
  
  D_Ki_gain = Math.round(D_Kiz_f*Math.pow(2,-D_Ki_shift)*1000000000000)/1000000000000; 
  testFracValRange("D_Ki_gain",D_Ki_gain);
  
  //RECCURENT PI CONTROLLER TYPE
  // scaling to scale and scale shift components
  D_CC1s =  Math.round((D_Kps + D_Kis*Ts/2)*1000000000000)/1000000000000;	
  D_CC2s =  Math.round((-D_Kps + D_Kis*Ts/2)*1000000000000)/1000000000000;
  D_CC1f = D_CC1s*Imax/Umax;
  D_CC2f = D_CC2s*Imax/Umax;
  
  // scale shift
  if ((Math.abs(D_CC1f)<1) && (Math.abs(D_CC2f)<1))   D_Nshift = 0;
  else
  {
    if (Math.abs(D_CC1f) > Math.abs(D_CC2f))
        D_Nshift = Math.ceil(Math.log(Math.abs(D_CC1f))/Math.log(2));
    else
        D_Nshift = Math.ceil(Math.log(Math.abs(D_CC2f))/Math.log(2));        
  }
  
  D_CC1_out = Math.round(D_CC1f/Math.pow(2,D_Nshift)*1000000000000)/1000000000000;
  D_CC2_out = Math.round(D_CC2f/Math.pow(2,D_Nshift)*1000000000000)/1000000000000;
  testFracValRange("D_CC1_out",D_CC1_out);
  testFracValRange("D_CC2_out",D_CC2_out,1);
  
  // Zero-cross cancelation filter constants
  D_ZC_B0_fl = D_Kis*Ts/(D_Kps+D_Kis*Ts);
  D_ZC_B1_fl = 0;
  D_ZC_A1_fl = -D_Kps/(D_Kps+D_Kis*Ts);
  D_ZC_B0_fl_out = Math.round(D_ZC_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
  D_ZC_B1_fl_out = Math.round(D_ZC_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
  D_ZC_A1_fl_out = Math.round(D_ZC_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
  D_ZC_B0_f_out  = Math.round(D_ZC_B0_fl_out*Imax/Umax*1000000000000)/1000000000000;
  D_ZC_B1_f_out  = Math.round(D_ZC_B1_fl_out*Imax/Umax*1000000000000)/1000000000000;
  D_ZC_A1_f_out  = Math.round(D_ZC_A1_fl_out*Imax/Umax*1000000000000)/1000000000000;
  testFracValRange("D_ZC_B0_f_out",D_ZC_B0_f_out,1);
  testFracValRange("D_ZC_B1_f_out",D_ZC_B1_f_out,1);
  testFracValRange("D_ZC_A1_f_out",D_ZC_A1_f_out,1);
  
  //********************** Related to Q axis **********************				
  Q_Kps = 2*Att*2*Math.PI*fo*Lq-Rs;
  Q_Kis = Math.pow((2*Math.PI*fo),2)*Lq;		
  Q_Kpz = Q_Kps;
  Q_Kiz = Q_Kis*Ts;
  
  //PARALLEL PI CONTROLLER TYPE
  Q_Kpz_f = Q_Kpz*Imax/Umax; 
  Q_Kiz_f = Q_Kiz*Imax/Umax;
  
  // scaling to scale and scale shift components
  // proportional
  if(Q_Kpz_f<1) Q_Kp_shift = -Math.ceil(Math.log(Math.abs(1/Q_Kpz_f))/Math.log(2)-1);
  else          Q_Kp_shift = Math.ceil(Math.log(Math.abs(Q_Kpz_f))/Math.log(2));
  
  Q_Kp_gain = Math.round(Q_Kpz_f*Math.pow(2,-Q_Kp_shift)*1000000000000)/1000000000000;
  testFracValRange("Q_Kp_gain",Q_Kp_gain);
                
  // integral
  if(Q_Kiz_f<1) Q_Ki_shift = -Math.ceil(Math.log(Math.abs(1/Q_Kiz_f))/Math.log(2)-1);
  else          Q_Ki_shift = Math.ceil(Math.log(Math.abs(Q_Kiz_f))/Math.log(2));
  
  Q_Ki_gain = Math.round(Q_Kiz_f*Math.pow(2,-Q_Ki_shift)*1000000000000)/1000000000000;
  testFracValRange("Q_Ki_gain",Q_Ki_gain);  
  
  //RECCURENT PI CONTROLLER TYPE
  // scaling to scale and scale shift components
  Q_CC1s = Math.round((Q_Kps + Q_Kis*Ts/2)*1000000000000)/1000000000000;	
  Q_CC2s = Math.round((-Q_Kps + Q_Kis*Ts/2)*1000000000000)/1000000000000;
  Q_CC1f = Q_CC1s*Imax/Umax;
  Q_CC2f = Q_CC2s*Imax/Umax;
  
  // scale shift
  if ((Math.abs(Q_CC1f)<1) && (Math.abs(Q_CC2f)<1))  Q_Nshift = 0;
  else
  {
    if (Math.abs(Q_CC1f) > Math.abs(Q_CC2f))
        Q_Nshift = Math.ceil(Math.log(Math.abs(Q_CC1f))/Math.log(2));
    else
        Q_Nshift = Math.ceil(Math.log(Math.abs(Q_CC2f))/Math.log(2));        
  }
  
  Q_CC1_out = Math.round(Q_CC1f/Math.pow(2,Q_Nshift)*1000000000000)/1000000000000;
  Q_CC2_out = Math.round(Q_CC2f/Math.pow(2,Q_Nshift)*1000000000000)/1000000000000;
  testFracValRange("D_CC1_out",D_CC1_out);
  testFracValRange("D_CC2_out",D_CC2_out,1);
  
  // Zero-cross cancelation filter constants
  Q_ZC_B0_fl = Q_Kis*Ts/(Q_Kps+Q_Kis*Ts);
  Q_ZC_B1_fl = 0;
  Q_ZC_A1_fl = -Q_Kps/(Q_Kps+Q_Kis*Ts);
  Q_ZC_B0_fl_out = Math.round(Q_ZC_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
  Q_ZC_B1_fl_out = Math.round(Q_ZC_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
  Q_ZC_A1_fl_out = Math.round(Q_ZC_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
  Q_ZC_B0_f_out  = Math.round(Q_ZC_B0_fl_out*Imax/Umax*1000000000000)/1000000000000;
  Q_ZC_B1_f_out  = Math.round(Q_ZC_B1_fl_out*Imax/Umax*1000000000000)/1000000000000;
  Q_ZC_A1_f_out  = Math.round(Q_ZC_A1_fl_out*Imax/Umax*1000000000000)/1000000000000;
  testFracValRange("Q_ZC_B0_f_out",Q_ZC_B0_f_out,1);
  testFracValRange("Q_ZC_B1_f_out",Q_ZC_B1_f_out,1);                                    
  testFracValRange("Q_ZC_A1_f_out",Q_ZC_A1_f_out,1);
}

/***************************************************************************//*!
*
* @brief   Update current controllers
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function updateControllers()
{
  var errorArray = [];
  var aritType     = parent.document.getElementById("Arithmetic").innerText;
  
  errorArray.push(UpdateFMVariable(xmlDoc,'CL_LIMIT',Ulim_out,Ulim_out));
  if(testVarValue('Ccontroller','Parallel')) // parallel type of PI controller
  {
   
    errorArray.push(UpdateFMVariable(xmlDoc,'D_Kp_g',D_Kpz_f,D_Kpz_f));//D_Kp_gain,D_Kpz
    errorArray.push(UpdateFMVariable(xmlDoc,'D_Ki_g',D_Kiz_f,D_Kiz_f));//D_Ki_gain,D_Kiz
    
    
   }
 
  
  // display error message
  UpdateError(errorArray);
}

/***************************************************************************//*!
*
* @brief   Calculate Bemf observer
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function calculateObserver()
{
  var BEMF_DQ_f0  = 280;
  var BEMF_DQ_att = 1;
  var pp          = 2;
  var Imax        = getParentHtmlValue("I_max");
  var UmaxCoeff   = getParentHtmlValue("UmaxCoeff");
  var UDCbus_max  = getParentHtmlValue("UDC_max");
  var Umax        = Math.round(UDCbus_max/UmaxCoeff*10)/10;
  var Emax        = Umax;
  var Nmax        = getParentHtmlValue("N_req")*1.1;
  var Wmax        = 2*Math.PI*pp*Nmax/60;
  var Ts    = getParentHtmlValue("CLOOP_Ts");
  var Rs    = getInnerHtmlValue("Rs_result");
  var Ld    = getInnerHtmlValue("Ld_result");
  var Lq    = getInnerHtmlValue("Lq_result");
  
  /* BEMF-DQ observer calculation scales */
  U_scaled_tmp  = Ts/(Ld+Ts*Rs)*(Umax/Imax);
  E_scaled_tmp  = Ts/(Ld+Ts*Rs)*(Emax/Imax);
  WI_scaled_tmp = Lq*Ts/(Ld+Ts*Rs)*Wmax;
    
  I_scaled = Math.ceil((Ld/(Ld+Ts*Rs))*1000000000000)/1000000000000;
  /* Saturate U_scale in case U_scale >1 */
  if(U_scaled_tmp>1)
  {
      U_scaled = Math.ceil(32767/32768*1000000000000)/1000000000000;
  }
  else
  {
      U_scaled = Math.ceil(Ts/(Ld+Ts*Rs)*(Umax/Imax)*1000000000000)/1000000000000;
  }
  
  /* Saturate E_scale in case E_scale >1 */
  if(E_scaled_tmp>1)
  {
      E_scaled = Math.ceil(32767/32768*1000000000000)/1000000000000;
  }
  else
  {
      E_scaled = Math.ceil(Ts/(Ld+Ts*Rs)*(Emax/Imax)*1000000000000)/1000000000000;
  }
  
  WI_scaled = Math.ceil(Lq*Ts/(Ld+Ts*Rs)*Wmax*1000000000000)/1000000000000;
  testFracValRange("I_scaled",I_scaled);
  testFracValRange("U_scaled",U_scaled);
  testFracValRange("E_scaled",E_scaled);
  testFracValRange("WI_scaled",WI_scaled);
  
  /* BEMF DQ observer constants */
  BemfDQ_Kps = 2*BEMF_DQ_att*2*Math.PI*BEMF_DQ_f0*Ld-Rs;
  BemfDQ_Kis = Ld*Math.pow(2*Math.PI*BEMF_DQ_f0,2);
  
  BemfDQ_Kpz = BemfDQ_Kps;
  BemfDQ_Kiz = BemfDQ_Kis*Ts;
  
  BemfDQ_Kpz_f = BemfDQ_Kpz*(Imax/Emax);
  BemfDQ_Kiz_f = BemfDQ_Kiz*(Imax/Emax);
  
  
  
  /* BEMF DQ scales */
  // proportional
  BemfDQ_Kp_gain = BemfDQ_Kpz_f;
  BemfDQ_Ki_gain = BemfDQ_Kiz_f;
}

/***************************************************************************//*!
*
* @brief   Update Bemf observer
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function updateObserver()
{
  var errorArray = [];
  var prefixM    = getActiveMotor();
  
  errorArray.push(UpdateFMVariable(xmlDoc,'E_scale',E_scaled));
  errorArray.push(UpdateFMVariable(xmlDoc,'I_scale',I_scaled));
  errorArray.push(UpdateFMVariable(xmlDoc,'U_scale',U_scaled));
  errorArray.push(UpdateFMVariable(xmlDoc,'WI_scale',WI_scaled));
  errorArray.push(UpdateFMVariable(xmlDoc,'BEMF_DQ_Kp_g',BemfDQ_Kp_gain));
  errorArray.push(UpdateFMVariable(xmlDoc,'BEMF_DQ_Ki_g',BemfDQ_Ki_gain));
  
  
  // display error message
  UpdateError(errorArray);
}

/***************************************************************************//*!
*
* @brief   Update measured electrical params in MCAT
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function updateResults()
{ 
  //Read Rs and write it to Rs_result
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Rs_meas"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
  {
    Rs_rounded = Math.round(pcm.LastVariable_vValue*100)/100;
    setInnerHtmlValue("Rs_result",Rs_rounded,Rs_rounded);
  }
  //Read Ld,Lq and write it to Ld_result, Lq_result  
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Ld_meas"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
  {
    Ld_rounded = Math.round(pcm.LastVariable_vValue*1000000)/1000000;
    setInnerHtmlValue("Ld_result",Ld_rounded,Ld_rounded);
  }
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Lq_meas"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
  {
    Lq_rounded = Math.round(pcm.LastVariable_vValue*1000000)/1000000;
    setInnerHtmlValue("Lq_result",Lq_rounded,Lq_rounded);
  }
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Ke_meas"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
  {
    Ke_rounded = Math.round(pcm.LastVariable_vValue*10000)/10000;
    setInnerHtmlValue("Ke_result",Ke_rounded,Ke_rounded);
  }
}

/***************************************************************************//*!
*
* @brief   Status reading + update
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function updateStatus()
{
  HWboardID  = getParentHtmlValue("HW_board");
  
  //Read Alignment status
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_AlignActive"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    AlignActive = pcm.LastVariable_vValue;
  //Read Transfer Char status
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_CharActive"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    CharActive = pcm.LastVariable_vValue;
  //Read Rs status
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_RsActive"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    RsActive = pcm.LastVariable_vValue;
  //Read Ls status
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_LsActive"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    LsActive = pcm.LastVariable_vValue;
  //Read Ke status
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_KeActive"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    KeActive = pcm.LastVariable_vValue;
  //Read Pp status
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_PpActive"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    PpActive = pcm.LastVariable_vValue;
  
  //Write status
  if(AlignActive != 0)
  {
    Status.innerHTML = "Aligning ...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if(CharActive != 0)
  {
    Status.innerHTML = "Characterisation ...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if(RsActive != 0)
  {
    Status.innerHTML = "Measuring Rs ...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if(LsActive != 0)
  {
    Status.innerHTML = "Measuring Ls ...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if(KeActive != 0)
  {
    Status.innerHTML = "Measuring Ke ...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if(PpActive != 0)
  {
    Status.innerHTML = "Measuring Pp ...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if((AlignActive == 0) && (CharActive == 0) && (RsActive == 0) && (LsActive == 0) && (KeActive == 0) && (PpActive == 0) && (HWboardID == 2))
  {
    Status.innerHTML = "Ready for measurement or calibration ...";
    document.getElementById("Status").style.color = "black";
  }
  
  if((AlignActive == 0) && (CharActive == 0) && (RsActive == 0) && (LsActive == 0) && (KeActive == 0) && (PpActive == 0) && (HWboardID != 2))
  {
    Status.innerHTML = "Ready for measurement ...";
    document.getElementById("Status").style.color = "black";
  }
  
  if(!pcm.IsCommPortOpen())
  {
    Status.innerHTML = "Communication is stopped. Press Ctrl+K to start the communication";
    document.getElementById("Status").style.color = "red";
  }
}

/***************************************************************************//*!
*
* @brief   Handle general FAULT (stop application, clear all Active,
*          clear results, Enable buttons)
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function handleFault()
{ 
  alert("fault");
  //Stop Application
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
  
  //Clear all uw16Active
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_AlignActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_CharActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_RsActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_LsActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_KeActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_PpActive', 0, 0));
  
  //Clear results
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_meas', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ld_meas', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lq_meas', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ke_meas', 0, 0));

  //Enable buttons
  if(document.getElementById("CalibrateButton") != undefined)
  document.getElementById("CalibrateButton").disabled = false;
  if(document.getElementById("MeasureButton") != undefined)
  document.getElementById("MeasureButton").disabled = false;
  if(document.getElementById("PpButtonStart") != undefined)
  document.getElementById("PpButtonStart").disabled = false;
  if(document.getElementById("PpButtonStop") != undefined)
  document.getElementById("PpButtonStop").disabled = true;
}

/***************************************************************************//*!
*
* @brief   Measured parameters reading
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function updateParams()
{
  var errorArray = [];
  var prefixM    = getActiveMotor();
  var dTs        = getParentHtmlValue("CLOOP_Ts");
  var aux_str = '';
  var MID_state;
  
  updateResults();
  updateStatus();

  //Read faults
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MeasFault"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
  {
    MeasFaults = pcm.LastVariable_vValue;
    
    //Check and assign faults
    if((MeasFaults != 0))
    {
      //Faults nr. 0x01, 0x02, 0x08, 0x10, 0x20, 0x80 (0x00bb) kill the measurement immediatelly
      if((MeasFaults & 0x00bb) > 0)
      {
        //Stop Application
        errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
        //Clear all uw16Active
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_AlignActive', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_CharActive', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_RsActive', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_LsActive', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_KeActive', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_PpActive', 0, 0));
        //Clear results
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_meas', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ld_meas', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lq_meas', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ke_meas', 0, 0));
        
        //Add "Faults:"
        aux_str = "Faults: \n\r";
        //Add fault descriptions
        if(MeasFaults & 0x0001)
          aux_str = [aux_str] + ["\n\r -motor not connected"];
        else
        {
          if(MeasFaults & 0x0002)
            aux_str = [aux_str] + ["\n\r -too high Rs for calibration"];
          if(MeasFaults & 0x0008)
            aux_str = [aux_str] + ["\n\r -wrong characteristic data"];
          if(MeasFaults & 0x0010)
            aux_str = [aux_str] + ["\n\r -insufficient scale for Ls measurement (L_MAX < Ls)"];
          if(MeasFaults & 0x0020)
            aux_str = [aux_str] + ["\n\r -insufficient scale for Z measurement (Z_MAX < Z)"];  
          if(MeasFaults & 0x0080)
            aux_str = [aux_str] + ["\n\r -insufficient scale for Ke measurement (KE_MAX < Ke)"];
        }
        
        //Alert faults
        alert(aux_str);
        
        //Enable buttons
        if(document.getElementById("CalibrateButton") != undefined)
        document.getElementById("CalibrateButton").disabled = false;
        if(document.getElementById("MeasureButton") != undefined)
        document.getElementById("MeasureButton").disabled = false;
        if(document.getElementById("PpButtonStart") != undefined)
        document.getElementById("PpButtonStart").disabled = false;
        if(document.getElementById("PpButtonStop") != undefined)
        document.getElementById("PpButtonStop").disabled = true;
      }
      
      //Faults nr. 0x04, 0x40 (0x0044)cause warnings
      if((MeasFaults & 0x0144) > 0)
      {
        MeasWarnings = MeasFaults;
      }
      
      //Clear faults
      errorArray.push(UpdateFMVariable(xmlDoc,'fm_MeasFault', 0, 0));
      UpdateError(errorArray);
    }
  }

  //Read MID state
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_state"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    MID_state = pcm.LastVariable_vValue;
  
  //Read status of calibration (if calibration or measurement or pp is done)
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Calibration"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    Calibration = pcm.LastVariable_vValue;
  
  //Read status of measurement (if calibration or measurement or pp is done)
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_ElectricalParameters"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    ElectricalParameters = pcm.LastVariable_vValue;
  
  //Read status of Application switch (after measurement the App switch is turned off)
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["onoff"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    OnOffSwitch = pcm.LastVariable_vValue;
  
  //If calibration is done and data has not been stored
  if((CharDataStored == 0) && (Calibration == 1) && (CharActive == 0) && (OnOffSwitch == 0))
  {
    //Enable buttons
    if(document.getElementById("CalibrateButton") != undefined)
    document.getElementById("CalibrateButton").disabled = false;
    if(document.getElementById("MeasureButton") != undefined)
    document.getElementById("MeasureButton").disabled = false;
    if(document.getElementById("PpButtonStart") != undefined)
    document.getElementById("PpButtonStart").disabled = false;
    if(document.getElementById("PpButtonStop") != undefined)
    document.getElementById("PpButtonStop").disabled = true;
  
    StoreCalibTable();
    CharDataStored = 1;
    alert("Calibration data stored.");
  }
  
  //If measurement is completed and measured params has not been sent to Parameters tab
  if((MeasurementCompleted == 0) && (ElectricalParameters == 1) && (CharActive == 0) && (OnOffSwitch == 0) && (Ke_rounded != 0))
  {
    //Enable buttons
    if(document.getElementById("CalibrateButton") != undefined)
    document.getElementById("CalibrateButton").disabled = false;
    if(document.getElementById("MeasureButton") != undefined)
    document.getElementById("MeasureButton").disabled = false;
    if(document.getElementById("PpButtonStart") != undefined)
    document.getElementById("PpButtonStart").disabled = false;
    if(document.getElementById("PpButtonStop") != undefined)
    document.getElementById("PpButtonStop").disabled = true;
  
    //Alert completed and avoid next alert
    MeasurementCompleted = 1;
    
    //If there were no warnings during measurement
    if(MeasWarnings == 0)
    {
      alert("Measurement completed.");
    }
    //If there were warnings during measurement
    else
    {
      aux_str = "Measurement completed with warnings: \n\r";
      
      if(MeasWarnings & 0x0004)
        aux_str = [aux_str] + ["\n\r -measuring current Id_meas not reached"];
      if(MeasWarnings & 0x0040)
        aux_str = [aux_str] + ["\n\r -measuring current amplitude Id_ampl not reached"];
      if(MeasWarnings & 0x0100)
        aux_str = [aux_str] + ["\n\r -motor locked"];
      
      //Alert warnings
      alert(aux_str);
      
      //Clear MeasWarning for the next measurement
      MeasWarnings = 0;
    }
    
    //sent params to Parameters tab
    setParentHtmlValue([prefixM] + ["Rs"],Rs_rounded);
    setParentHtmlValue([prefixM] + ["Ld"],Ld_rounded);
    setParentHtmlValue([prefixM] + ["Lq"],Lq_rounded);
    setParentHtmlValue([prefixM] + ["ke"],Ke_rounded);
    
    clickStoreData();
  }
  
  // if the calculation has not been done yet
  // and MID state machine is in Pp measurement state
  // and Rs, Ld, Lq results has been read from FMSTR to MCAT
  /////////////////// Calculate Controllers and Bemf Observer /////////////////////////
  if((observer_done_MCAT == 0) && (MID_state == 6) && (getInnerHtmlValue("Rs_result") > 0) && (getInnerHtmlValue("Ld_result") > 0) && (getInnerHtmlValue("Lq_result") > 0))
  {
    calculateControllers();
    updateControllers();
    calculateObserver();
    updateObserver();
    
    observer_done_MCAT = 1;
    // acknowledge that calculation is done
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_MCATObserverDone',1));
            
    // display error message
    UpdateError(errorArray)
  }
}

/***************************************************************************//*!
*
* @brief   HW board selector
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function HWboardSelector(boardID)
{
   var prefixM    = getActiveMotor();
   
   document.getElementById(prefixM + "Board_selector").value = boardID;
   
   /* FSL board */
   if(boardID == 0)
   {
      if(getBoardType() == "HVP")
      {
        switchParam2BasicMode("I_max",8);
        switchParam2BasicMode("UDC_max",433);
        document.getElementById("PWRSTG_charact").style.display = "none";
      }

      if(getBoardType() == "TWR")
      {
        switchParam2BasicMode("I_max",8);
        switchParam2BasicMode("UDC_max",36.3);
        document.getElementById("PWRSTG_charact").style.display = "none";
      }
      
      if(getBoardType() == "FRDM")
      {
        switchParam2BasicMode("I_max",8.25);
        switchParam2BasicMode("UDC_max",60.8);
        document.getElementById("PWRSTG_charact").style.display = "none";
      }
   }
   
   /* User's board */
   if(boardID == 1)
   {
      //clickReloadData();
      enableInputParamBox("I_max");   
      enableInputParamBox("UDC_max");
      document.getElementById("PWRSTG_charact").style.display = "";
      /* read user HW data from param file */
      clickReloadData();
   }

   /* update params */
   copyParent2InnerValById("I_max");
   copyParent2InnerValById("UDC_max");
   copyParent2InnerValById("Ud_inc");
   
   /* update HW boar in parent table */
   setParentHtmlValue((prefixM) + "HW_board", boardID);
   
   // write data to param file
   //paramFileWriteData(prefixM);
  
}

/***************************************************************************//*!
*
* @brief  Function for periodical reading of FM variables
* @param   
* @return 
* @remarks 
******************************************************************************/
function updateTab_MID()
{
   // update constants
   clickCalculateMID();
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
    
