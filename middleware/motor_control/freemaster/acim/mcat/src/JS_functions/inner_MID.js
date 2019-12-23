/******************************************************************************
*
 * Copyright 2012-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
*
****************************************************************************//*!
*
* @file   inner_MID.js
*
* @brief  javascript engine for MCAT
*
* @version 1.0.1.0
* 
* @date Dec-28-2012
* 
******************************************************************************/
/******************************************************************************
* List of functions
******************************************************************************
* initLoadFormMID() - init motor resistance and inductance measurement page paramters and constants  
* clickCalculateMID() - calculates control constants based on input parameters
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
    
    MIDDisableButton("CalibrateButton");
    MIDDisableButton("MeasureButton");
    MIDDisableButton("GenCalButton");
  }
  else
  {
    var ControllerType = null;
    
    //get active motor to selct proper prefix
    prefixM = getActiveMotor();
    
    /* Select HW board */
    var HWboardID = getParentHtmlValue("HW_board");
    HWboardSelector(HWboardID);
    
    // in basic mode 
    if(getActiveMode()==0)
    {
        basicModeCalcMID();
    }
    
    //copyParent2InnerValById("I_max");
    //copyParent2InnerValById("UDC_max");
   
    
    copyParent2InnerValById("Rs_calib");
    copyParent2InnerValById("I_calib");

    copyParent2InnerValById("P_nom");
    copyParent2InnerValById("I_nom");
    copyParent2InnerValById("U_nom");
    copyParent2InnerValById("f_nom");
    copyParent2InnerValById("pp");
    
    copyParent2InnerValById("CLOOP_F0");
    copyParent2InnerValById("SLOOP_F0");
    copyParent2InnerValById("T_meas");
    
    Calibration = 0;
    
    //calculate constants
    clickCalculateMID();
    
    //Update status
    updateStatus();
    
    //Subscribe variables for watching
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Rs"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Rr"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Ls"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Lr"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Lm"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_J"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Tm"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_CharActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 100);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_RsActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_NoLoadActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_BlockedActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MechActive"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    
    reference_val = xmlDoc.getElementsByTagName([prefixM+ "states"])[0].getAttribute("FreemasterName");
    pcm.SubscribeVariable(reference_val, 1000);
    pcm.ReadVariable(reference_val);   
    if(pcm.LastVariable_vValue > 0)
    {
        //Disable buttons
        MIDEnableButton("CalibrateButton");
        MIDEnableButton("MeasureButton");
    }  
    MIDEnableButton("GenCalButton");
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_state"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["onoff"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Fault"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_success"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_restore"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_update"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_CalcElParState"])[0];
    pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_CalcMeParState"])[0];
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
    var state;
   
    if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_MID_success"])[0].childNodes[0].nodeValue))
    MIDSuccess = pcm.LastVariable_vValue;
   
    //If main application fault occured and stopped the application
    reference_val = xmlDoc.getElementsByTagName([prefixM+ "states"])[0].getAttribute("FreemasterName");
    pcm.ReadVariable(reference_val); 
    state = pcm.LastVariable_vValue;
    if(state == 0)
    {
        handleFault();
        MIDDisableButton("CalibrateButton");
        MIDDisableButton("MeasureButton");
    }
    else if((MeasurementCompleted > 0) && (CharDataStored > 0))
    {
        MIDEnableButton("CalibrateButton");
        MIDEnableButton("MeasureButton");
    }
    
    if(MIDSuccess)
    {
        MIDEnableButton("StoreData");
    }
    else MIDDisableButton("StoreData");
    
    if(MIDSuccess && (state == 2))
    {
        MIDEnableButton("btnMIDUpdate");
        MIDEnableButton("btnMIDRestore");
    }
    else 
    {
        MIDDisableButton("btnMIDUpdate");
        MIDDisableButton("btnMIDRestore");
    }
    
    
    updateParams();
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
    Rs_calib_MCAT         = getParentHtmlValue("Rs_calib");
    
    //Global for checking whether controllers and observer have been calculated or not
    CharDataStored        = 1;
    MeasurementCompleted  = 1;
  
    //local
    var P_nom          = getParentHtmlValue("P_nom");
    var pp             = getParentHtmlValue("pp");
    var I_calib        = getParentHtmlValue("I_calib");
    var T_meas         = getParentHtmlValue("T_meas"); 
    var HWboardID      = getParentHtmlValue("HW_board");
    var f0_Cloop       = getParentHtmlValue("CLOOP_F0");
    var f0_Sloop       = getParentHtmlValue("SLOOP_F0");
    
    /* read I_max, U_max and Ts */
    var xmlDoc     = loadXMLDoc("xml_files\\FM_params_list.xml");
    pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"U_max"])[0].childNodes[0].nodeValue); 
    var Umax = pcm.LastVariable_vValue;
    pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"I_max"])[0].childNodes[0].nodeValue); 
    var Imax = pcm.LastVariable_vValue; 
    pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"TsFastLoop"])[0].childNodes[0].nodeValue); 
    var dTs = pcm.LastVariable_vValue;

    /* Set calibration range +-2A for FSL power stages */
    if(HWboardID < 1)
    {
      setParentHtmlValue((prefixM + "I_calib"), 2);
      copyParent2InnerValById("I_calib");
      I_calib = 2.0;
    }
 
    
    /* Ensure that Calibration range is inside +- Imax */
    if(I_calib > Imax*0.95)
    {
      setParentHtmlValue((prefixM + "I_calib"), Imax*0.95);
      copyParent2InnerValById("I_calib");
      I_calib = Imax*0.9;
      alert("Calibration range must be lower than 95% of Imax current scale.");
    }
    
    //Generated #defines
    TIME_1S_def = Math.round(1.0/dTs);
    
    //Parameters limitations
    if(document.getElementById("MID") != undefined)
    {
        // user HW - do not limit
        TestRangeTrimInf("I_nom", 0.0, Imax*0.7071068, "Arms", "Value is outside of current HW scale and measurement will be taken with ");
        TestRangeTrimInf("U_nom", 0.0, Umax*0.7071068, "Vrms", "Value is outside of voltage HW scale and measurement will be taken with ");
    }
    
    
    // If HEADER FILE tab is active ********************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {
      setInnerHtmlValueAsText("P_NOM",0,P_nom, P_nom);
      setInnerHtmlValueAsText("TIME_1S",1,TIME_1S_def, TIME_1S_def);
      setInnerHtmlValueAsText("TRQ_MEAS",0,T_meas, Math.round(T_meas*1e12)/1e12);
      setInnerHtmlValueAsText("FRQCLOOP",0,f0_Cloop, Math.round(f0_Cloop*1e12)/1e12);
      setInnerHtmlValueAsText("FRQSLOOP",0,f0_Sloop, Math.round(f0_Sloop*1e12)/1e12);
     
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
function writeMIDHTMLOutput(prefix,xmlObject)
{   
    // Motor identification module
    document.write(HTML_write_blank_line());     
    document.write(HTML_write_comment_line("/* motor parameter identification","",""));
    document.write(HTML_write_comment_line_dash("*/")); 
    
    document.write(HTML_write_define_line_number(prefix,0,"P_NOM",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TIME_1S",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"TRQ_MEAS",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FRQCLOOP",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FRQSLOOP",xmlObject));
    
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
    str = write_comment_text(str,'/* motor parameter identification','');
    str = write_comment_line_dash(str,"*/");
    
    str = write_define_line_number(prefix, str,'P_NOM');
    str = write_define_line_number(prefix, str,'TIME_1S');
    str = write_define_line_number(prefix, str,'TRQ_MEAS');
    str = write_define_line_number(prefix, str,'FRQCLOOP');
    str = write_define_line_number(prefix, str,'FRQSLOOP');
   
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
  MIDDisableButton("CalibrateButton");
  MIDDisableButton("MeasureButton");
  
  var errorArray = [];
  /* read number of points in table */
  pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"fm_pwrstg_numofpnts"])[0].childNodes[0].nodeValue);
  var NumOfPoints = pcm.LastVariable_vValue;
  
  CharDataStored    = 0;
  
  Rs_calib_MCAT     = getParentHtmlValue("Rs_calib");
  I_calib           = getParentHtmlValue("I_calib");
  var I_increment  = I_calib*2.0/NumOfPoints;
  
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
  
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_calib', Rs_calib_MCAT, Rs_calib_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_I_calib', I_calib, I_calib));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_I_increment', I_increment, I_increment));
  

  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Calibration', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
  
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
  var strDat = "", txt;
  var Uerr = [];
  var n, Uavg;

  pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"fm_pwrstg_numofpnts"])[0].childNodes[0].nodeValue);
  var NumOfPoints = pcm.LastVariable_vValue;
  
  

  txt = xmlDoc.getElementsByTagName("M1_fm_PWRSTG_LookUp")[0].childNodes[0].nodeValue;
  
  for (var i=0;i<NumOfPoints;i++)
  {
    Uerr[i] = 0;
    if(pcm.ReadVariable([txt]+["["]+[i.toString()]+["]"]))
    {
      Uerr[i] = Math.round(pcm.LastVariable_vValue*1e12)/1e12;
    }
  }
  /* average mesured values */
  n = (NumOfPoints-1)/2;
  Uerr[n] = 0.0;
  for (var i=0;i<=n;i++)
  {
      Uavg = (Uerr[n+i] - Uerr[n-i]) * 0.5;
      Uavg = Math.round(Uavg*1e12)/1e12;
      Uerr[n+i] = Uavg;
      Uerr[n-i] = -Uavg;
  }
  
  for (var i=0;i<NumOfPoints;i++)
      strDat = [strDat]+[Uerr[i].toString()]+[" "];
  setParentHtmlValue([prefixM] + ["User_HW_CalibTable"], strDat);
  setParentHtmlValue([prefixM] + ["User_HW_CalibTable_size"], NumOfPoints);
  paramFileWriteData(prefixM);

}

/***************************************************************************//*!
*
* @brief  Button Load User Data
* @param   
* @return 
* @remarks 
******************************************************************************/
function clickGenCalibFile()
{

  var prefixM = getActiveMotor();
  var HWboardID = getParentHtmlValue("HW_board");
  /* get actual date and time of file storing */
  var str='', strDat='', CharArray;
  var today       = new Date();                         
  var mounth      = MounthConverter(today.getMonth());
  var t, I_calib, LinCoeff, n = 0, dUerr = 0, IStep;
  var I_calibStr, LinCoeffStr, strBoard;
  var DocLocation = parent.document.getElementById(prefixM + "ProjectPath").innerHTML;
    
  t = today.getHours() + ":";
  t += today.getMinutes() + ":";
  t += today.getSeconds(); 
  
  if(HWboardID == 0)
  {
     strDat = getParentHtmlValueStore("FSL_HW_CalibTable");
     strBoard = "HVP-MC3PH";
     var NumPoint = getParentHtmlValue("FSL_HW_CalibTable_size");
  }
   
  if(HWboardID == 1)
  {
     strDat =getParentHtmlValueStore("User_HW_CalibTable");
     strBoard = "user hardware";
     var NumPoint = getParentHtmlValue("User_HW_CalibTable_size");
 }
  
  I_calib = parent.document.getElementById(prefixM + "I_calib").innerHTML;
  IStep = I_calib*2/(NumPoint-1);
  
  
  CharArray = strDat.split(" ");
  strDat = "";
  for (var i=0;i<NumPoint;i++)
  {
    if (i > 0) strDat += ",\\";
    if(CharArray[i].indexOf('.') < 0) 
        strDat +=  "\r\n    " + CharArray[i]+".0F";
    else
        strDat +=  "\r\n    " + CharArray[i]+"F";
    if((i<5) || ((i>=59) && (i<64))) 
    {
        dUerr = dUerr + (Math.abs(CharArray[i] - CharArray[i+1]));
        n = n + 1;
    }
  }  

  LinCoeff = dUerr / (IStep*n);
  
  
  I_calibStr = (Math.round(I_calib*1e12)/1e12).toString();
  if(I_calibStr.indexOf('.') < 0) I_calibStr =  I_calibStr+".0F";
  else I_calibStr =  I_calibStr+"F";
  
  LinCoeffStr = (Math.round(LinCoeff*1e12)/1e12).toString();
  if(LinCoeffStr.indexOf('.') < 0) LinCoeffStr =  LinCoeffStr+".0F";
  else LinCoeffStr = LinCoeffStr+"F";
               
  //var RelPath_DocLocation = getRelativePath(DocLocation);
  /*************** Headlines                              *********************/
   str = "/*";
   str = write_comment_line_dash(str);
   str = str + "Automatically generated file with power-stage characterization data\r\n";
   str = str + "\r\n";
   str = str + "File Name:\r\n{FM_project_loc}/" + DocLocation + "board/char_pwrstg.h \r\n";
   str = str + "\r\n";
   //str = str + "// Date:  " + today.getTime()+ today.getDate()+ ". " + mounth + ", " + today.getFullYear() + "\r\n";
   //document.write(HTML_write_head_line("Date:",""+mounth+ " "+ today.getDate() + ", " + today.getFullYear() + ", " + t+ ""));
   str = str + "Date:  " + mounth + " " + today.getDate() + ", " + today.getFullYear() + ", " + t + "\r\n";
   str = str + "Board:  " + strBoard + "\r\n";
   
   str = write_comment_line_dash(str,"*/");
   
  /****************************************************************************/
   str = str + "#ifndef _"+prefixM+"CHAR_PWRSTG_H_\r\n";
   str = str + "#define _"+prefixM+"CHAR_PWRSTG_H_\r\n";  
   
   str = str + "\r\n#define DTCOMP_TABLE_SIZE    ("+ NumPoint +")"; 
   str = str + "\r\n#define DTCOMP_LINCOEFF      ("+ LinCoeffStr +")"; 
   str = str + "\r\n#define DTCOMP_I_RANGE       ("+ I_calibStr +")"; 
   str = str + "\r\n#define DTCOMP_TABLE_DATA    \\\r\n{\\"+ strDat +" \\\r\n}\r\n"; 
  
      
  /*************** End of page                            *********************/ 
   str = write_blank_lines(str,1);    
   str = str + '#endif /* _'+prefixM+'CHAR_PWRSTG_H_ */\r\n';
   str = str + "/*\r\n*######################################################################\r\n";
   str = str + '*                           End of File\r\n';
   str = str + "*######################################################################\r\n*/\r\n";

  /*************** Writing to file process                *********************/   
  // open file for writting
   var paramFile = pcm.LocalFileOpen(DocLocation + "board/char_pwrstg.h","w");

  // write string to output file
   charNumber = pcm.LocalFileWriteString(paramFile,str); 
  // close param file
   fileStatus = pcm.LocalFileClose(paramFile);
   
   if(fileStatus = false)
      alert('File closing error');
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
  var HWboardID = getParentHtmlValue("HW_board");  
    
  //Disable buttons 
  MIDDisableButton("CalibrateButton");
  MIDDisableButton("MeasureButton");

  var prefixM    = getActiveMotor();
  var errorArray = [];
  var pp         = getParentHtmlValue("pp");
  var Inom       = getParentHtmlValue("I_nom"); 
  var Unom       = getParentHtmlValue("U_nom");
  var Pnom       = getParentHtmlValue("P_nom");
  var Pnom        = getParentHtmlValue("P_nom");
  var fCloop      = getParentHtmlValue("CLOOP_F0");
  var fSloop      = getParentHtmlValue("SLOOP_F0");
  pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"I_max"])[0].childNodes[0].nodeValue); 
  var Imax = pcm.LastVariable_vValue;
  pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"U_max"])[0].childNodes[0].nodeValue); 
  var Umax = pcm.LastVariable_vValue;
  //pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"TsFastLoop"])[0].childNodes[0].nodeValue); 
  //var dTs = pcm.LastVariable_vValue;

  
  clickCalculateMID();
  
  MeasurementCompleted = 0;
  MeasWarnings = 0; 
  CalcElPar = 0;
  CalcMechPar = 0;
  
  I_calib            = getParentHtmlValue("I_calib");
  I_meas_MCAT        = Inom; // blocked rotor test - in rms
  U_meas_MCAT        = Unom*1.41421356; // no-load test - in amplitude value
  f_meas_MCAT        = getParentHtmlValue("f_nom");
  T_meas_MCAT        = getParentHtmlValue("T_meas"); 
  
  if(I_meas_MCAT > Imax*0.707107*0.9) I_meas_MCAT = Imax*0.707107*0.9;
  else if(I_meas_MCAT < 0) I_meas_MCAT = 0; 
  
  if(U_meas_MCAT > Umax) U_meas_MCAT = Umax;
  else if(U_meas_MCAT < 0) U_meas_MCAT = 0; 


  I_DCmeas_MCAT = I_meas_MCAT*1.414213562;
  if(I_DCmeas_MCAT > I_calib) I_DCmeas_MCAT = I_calib; 
  

  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Par_Pnom', Pnom, Pnom));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Par_Unom', Unom, Unom));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Par_Inom', Inom, Inom));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Par_fnom', f_meas_MCAT, f_meas_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Par_pp', pp, pp));
  
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_NoLoad_UMeas', U_meas_MCAT, U_meas_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Blocked_IMeas', I_meas_MCAT, I_meas_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Mech_TrqAccl', T_meas_MCAT, T_meas_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Id_meas', I_DCmeas_MCAT, I_DCmeas_MCAT));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_calib', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rr', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ls', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lr', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lm', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_J', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Tm', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_CloopFreq', fCloop, fCloop));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_SloopFreq', fSloop, fSloop)); 
  
                                      
  //errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_state', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Calibration', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Blocked_RotorBlocked', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_CalcElParState', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_CalcMeParState', 0, 0));
  
  if(confirm("STEP 1/3 - Stator resistance measurement and no-load test\r\n\r\n\u2022Please detach load and click OK\r\n\u2022WARNING: Motor will spin at nominal speed!"))
  {
      errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
      errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 1, 1));
  }
  else handleFault();

  
  


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
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_Rs"])[0].childNodes[0].nodeValue))
  {
    Rs_rounded = Math.round(pcm.LastVariable_vValue*1000)/1000;
    setInnerHtmlValue("Rs_result",Rs_rounded,Rs_rounded);
  }
  
  //Read Rr and write it to Rr_result
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_Rr"])[0].childNodes[0].nodeValue))
  {
    Rr_rounded = Math.round(pcm.LastVariable_vValue*1000)/1000;
    setInnerHtmlValue("Rr_result",Rr_rounded,Rr_rounded);
  }
  
  //Read Ls and write it to Ls_result
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_Ls"])[0].childNodes[0].nodeValue))
  {
    Ls_rounded = Math.round(pcm.LastVariable_vValue*1000)/1000;
    setInnerHtmlValue("Ls_result",Ls_rounded,Ls_rounded);
  }
  
  //Read Lr and write it to Lr_result
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_Lr"])[0].childNodes[0].nodeValue))
  {
    Lr_rounded = Math.round(pcm.LastVariable_vValue*1000)/1000;
    setInnerHtmlValue("Lr_result",Lr_rounded,Lr_rounded);
  }
  
  //Read Lm and write it to Lm_result
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_Lm"])[0].childNodes[0].nodeValue))
  {
    Lm_rounded = Math.round(pcm.LastVariable_vValue*1000)/1000;
    setInnerHtmlValue("Lm_result",Lm_rounded,Lm_rounded);
  }
  
  //Read J and write it to J_result
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_J"])[0].childNodes[0].nodeValue))
  {
    J_rounded = Math.round(pcm.LastVariable_vValue*1000000)/1000;
    setInnerHtmlValue("J_result",J_rounded,J_rounded);
  }
  
    //Read Tm and write it to Tm_result
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_Tm"])[0].childNodes[0].nodeValue))
  {
    Tm_rounded = Math.round(pcm.LastVariable_vValue*1000)/1000;
    setInnerHtmlValue("Tm_result",Tm_rounded,Tm_rounded);
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
  var errorArray = [];
  
  if(!pcm.IsCommPortOpen())
  {
    Status.innerHTML = "Communication is stopped\n\r\n\r\u2022Press Ctrl+K to start the communication";
    document.getElementById("Status").style.color = "red";
    MIDDisableButton("CalibrateButton");
    MIDDisableButton("MeasureButton");
    MIDDisableButton("GenCalButton");
    return;
  }
  
  updateResults();
  
  //Read Transfer Char status
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_CharActive"])[0].childNodes[0].nodeValue))
    CharActive = pcm.LastVariable_vValue;
  //Read Rs status
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_RsActive"])[0].childNodes[0].nodeValue))
    RsActive = pcm.LastVariable_vValue;
  //Read NoLoad status
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_NoLoadActive"])[0].childNodes[0].nodeValue))
    NoLoadActive = pcm.LastVariable_vValue;
  //Read Blocked status
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_BlockedActive"])[0].childNodes[0].nodeValue))
    BlockedActive = pcm.LastVariable_vValue;
   //Read Mech status
  if(pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["fm_MechActive"])[0].childNodes[0].nodeValue))
    MechActive = pcm.LastVariable_vValue;

  //Write status
  if(CharActive != 0)
  {
    Status.innerHTML = "Characterisation...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if(RsActive != 0)
  {
    Status.innerHTML = "Measuring stator resistance...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if(NoLoadActive != 0)
  {
    Status.innerHTML = "Performing No Load test...";
    document.getElementById("Status").style.color = "blue";
  }
  
  if(BlockedActive != 0)
  {
    Status.innerHTML = "Performing Blocked Rotor test...";
    document.getElementById("Status").style.color = "blue";
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Blocked_RotorBlocked"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
      BlockedRotor = pcm.LastVariable_vValue;
      
    if(BlockedRotor == 0)
    {
      if(confirm("STEP 2/3 - Blocked rotor test\r\n\r\n\u2022Please lock rotor and click OK"))
      {
          errorArray.push(UpdateFMVariable(xmlDoc,'fm_Blocked_RotorBlocked', 1, 1));
      }
      else handleFault();
      UpdateError(errorArray);
      return;
    }
  }
  
  
  if(MechActive != 0)
  {
    Status.innerHTML = "Measuring Mechanical Parameters...";
    document.getElementById("Status").style.color = "blue";
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Blocked_RotorBlocked"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
      BlockedRotor = pcm.LastVariable_vValue;
      
    if(BlockedRotor == 1)
    {
      if(confirm("STEP 3/3 - Mechanical parameters measurement\r\n\r\n\u2022Please attach load to unlocked rotor and click OK\r\n\u2022WARNING: Motor will spin!"))
      {
          errorArray.push(UpdateFMVariable(xmlDoc,'fm_Blocked_RotorBlocked', 0, 0));
      }
      else handleFault();      
      UpdateError(errorArray);
    }
  }

  if((CharActive == 0) && (RsActive == 0) && (NoLoadActive == 0) && (BlockedActive == 0) && (HWboardID == 1))
  {
    Status.innerHTML = "Ready for measurement or calibration ...";
    document.getElementById("Status").style.color = "black";
  }
  
  if((CharActive == 0) && (RsActive == 0) && (NoLoadActive == 0) && (BlockedActive == 0) && (HWboardID != 1))
  {
    Status.innerHTML = "Ready for measurement ...";
    document.getElementById("Status").style.color = "black";
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
  var errorArray = [];
  
 
  //Stop Application
  errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
  
  //Clear all uw16Active
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_CharActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_RsActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_NoLoadActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_BlockedActive', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechActive', 0, 0));
  
  //Clear results
  /*errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rr', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ls', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lr', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lm', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_J', 0, 0));
  errorArray.push(UpdateFMVariable(xmlDoc,'fm_Tm', 0, 0));*/

  //Enable buttons
  MIDEnableButton("CalibrateButton");
  MIDEnableButton("MeasureButton");
  
  UpdateError(errorArray);
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
  
  //updateResults();
  updateStatus();

  //Read faults
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MIDFault"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
  {
    MIDFaults = pcm.LastVariable_vValue;
    
    //Check and assign faults
    if((MIDFaults != 0))
    {
      //Faults nr. 0x01, 0x02, 0x08, 0x10, 0x20, 0x80 (0x00bb) kill the measurement immediatelly
      if((MIDFaults & 0x00bb) > 0)
      {
        //Stop Application
        errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
        //Clear all uw16Active
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_CharActive', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_RsActive', 0, 0));
        //Clear results 
       /* errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rr', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ls', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lr', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lm', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_J', 0, 0));
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_Tm', 0, 0));*/
        
        //Add "Faults:"
        aux_str = "FAILURE\n\r\n\r";
        //Add fault descriptions
        if(MIDFaults & 0x0001)
            aux_str += "\n\rERROR: Measurement aborted by user";
        if(MIDFaults & 0x0002)
            aux_str += "\n\rERROR: Motor not connected";
        if(MIDFaults & 0x0004)
            aux_str += "\n\rERROR: Stator resistance too high for calibration";
        if(MIDFaults & 0x0008)
            aux_str += "\n\rERROR: Stator resistance out of range";
        if(MIDFaults & 0x00016)
            aux_str += "\n\rERROR: Mechanical parameters measurement timeout";
        
        aux_str += "\n\r\u2022See AN5051 for more details";
        
        //Alert faults
        alert(aux_str);
        
        //Enable buttons
        MIDEnableButton("CalibrateButton");
        MIDEnableButton("MeasureButton");
      }
            
      //Clear faults
      errorArray.push(UpdateFMVariable(xmlDoc,'fm_MIDFault', 0, 0));
      UpdateError(errorArray);
    }
  }
  
  //Read MID state
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_state"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    MID_state = pcm.LastVariable_vValue;

  //Read MID warnings
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MIDWarn"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    MeasWarnings = pcm.LastVariable_vValue;
  
  //Read status of calibration (if calibration or measurement or pp is done)
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Calibration"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    Calibration = pcm.LastVariable_vValue;
   
  //Read status of Application switch (after measurement the App switch is turned off)
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["onoff"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    OnOffSwitch = pcm.LastVariable_vValue;
  
  //If calibration is done and data has not been stored
  if((CharDataStored == 0) && (Calibration == 1) && (CharActive == 0) && (OnOffSwitch == 0))
  {
    //Enable buttons
    MIDEnableButton("CalibrateButton");
    MIDEnableButton("MeasureButton");
  
    // save measured data to mcat
    StoreCalibTable();
    // generate file with data
    //clickGenCalibFile();
    CharDataStored = 1;
    alert("SUCCESS\n\rCalibration data were succesfully obtained and stored in MCAT\n\r\n\r\u2022You can click [Generate Calibration Data File] button and then recompile and load project to target MCU to apply chages");
  }
  
  reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Lm"])[0];
  if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
  {
    Lm_rounded = Math.round(pcm.LastVariable_vValue*100)/100;
  }
  
  //If measurement is completed and measured params has not been sent to Parameters tab
  if((MeasurementCompleted == 0) && (Calibration == 0) && (CharActive == 0) && (OnOffSwitch == 0) && (Lm_rounded != 0))
  {
    //Enable buttons
    MIDEnableButton("CalibrateButton");
    MIDEnableButton("MeasureButton");
  
    //Alert completed and avoid next alert
    MeasurementCompleted = 1;
    
    //If there were no warnings during measurement
    if(MeasWarnings == 0)
    {
        
      aux_str = "SUCCESS\n\rMeasurement was completed without any warnings\n\r\n\r";
      
    }
    //If there were warnings during measuremento
    else
    {
      aux_str = "SUCCESS\n\rMeasurement succesfully completed\n\r";
      
      if(MeasWarnings & 0x0001)
        aux_str += "\n\rWARNING: Required stator resistance measurement current was not reached";
        
      if(MeasWarnings & 0x0002)
        aux_str += "\n\rWARNING: Required nominal stator voltage not reached during no-load test";
        
      if(MeasWarnings & 0x0004)
        aux_str += "\n\rWARNING: Required nominal stator current not reached during locked-rotor test";
    
      if(MeasWarnings & 0x0008)
        aux_str += "\n\rWARNING: Precision of stator resistance measurement is too low";
            
      //Clear MeasWarning for the next measurement
      MeasWarnings = 0;
    }
    aux_str += "\n\r\n\r\u2022You can click [Apply Parameters] button to apply changes on MCU. Changes may be reverted by clicking [Restore Parameters] button or restarting MCU\n\r\u2022You can click [Save Parameters in MCAT] to save new parameters and then generate new configuration file in [Output File] tab";
    //Alert warnings
    alert(aux_str);
    
    
    updateResults();
  }
}

/***************************************************************************//*!
* @brief   Store measured parameters to file
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function clickMIDStoreData()
{
    var tmp;
    
    /* check whether the measurement was succesfull */
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_success"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        MIDSuccess = pcm.LastVariable_vValue;
    
    if(MIDSuccess)
    {
        updateResults();
        setParentHtmlValue((prefixM + "RsIM"),Rs_rounded);
        setParentHtmlValue((prefixM + "RrIM"),Rr_rounded);
        setParentHtmlValue((prefixM + "LsIM"),Ls_rounded);
        setParentHtmlValue((prefixM + "LrIM"),Lr_rounded);
        setParentHtmlValue((prefixM + "LmIM"),Lm_rounded);
        setParentHtmlValue((prefixM + "JIM"), Math.round(J_rounded*1e7)/1e10);
        setParentHtmlValue((prefixM + "TmIM"),Tm_rounded);
        
        /* store other options into mcat  */
        
        /* mxaimal stator current amplitude */
        pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["MID_SL_LIM"])[0].childNodes[0].nodeValue);
        tmp = Math.round(pcm.LastVariable_vValue*100)/100;
        setParentHtmlValue((prefixM + "SL_HIGH_LIM"), tmp);
        setParentHtmlValue((prefixM + "SL_LOW_LIM"), -tmp);
        /* minimal d-axis current */
        pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["MID_FL_MTPA_MIN"])[0].childNodes[0].nodeValue);
        tmp = Math.round(pcm.LastVariable_vValue*100)/100;
        setParentHtmlValue((prefixM + "FL_MTPA_MIN"), tmp);
        setParentHtmlValue((prefixM + "FL_FW_MIN"), tmp);
        /*maximal d-axis current*/
        pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["MID_FL_MTPA_MAX"])[0].childNodes[0].nodeValue);
        tmp = Math.round(pcm.LastVariable_vValue*100)/100;
        setParentHtmlValue((prefixM + "FL_MTPA_MAX"), tmp);
        /*startup current*/
        pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["MID_FL_STARTUP"])[0].childNodes[0].nodeValue);
        tmp = Math.round(pcm.LastVariable_vValue*100)/100;
        setParentHtmlValue((prefixM + "FL_STARTUP"), tmp);
        /*minimal required speed*/
        pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["MID_SL_SPDREQMIN"])[0].childNodes[0].nodeValue);
        tmp = Math.round(pcm.LastVariable_vValue*100)/100;
        setParentHtmlValue((prefixM + "N_min"), tmp);
        /*maximal required speed*/
        pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM]+["MID_SL_SPDREQMAX"])[0].childNodes[0].nodeValue);
        tmp = Math.round(pcm.LastVariable_vValue*100)/100;
        setParentHtmlValue((prefixM + "N_max"), tmp);
        
        /* set defaukt value for field weakening time constant */
        //setParentHtmlValue((prefixM + "FL_FW_TC"), 0.5);
        
    }
    
    clickStoreData();
}



/***************************************************************************//*!
* @brief   Update parameters in target button click
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function fcnClickMIDUpdate()
{
    var errorArray = [];
    var MIDSuccess = 0;
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_success"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        MIDSuccess = pcm.LastVariable_vValue;
    
    
    if(MIDSuccess)
    {
        /* update parameters in target */
        /* set update flag */
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_update', 1, 1));
        if(!UpdateError(errorArray))
            window.alert("New parameters were applied\n\r\n\r\u2022Click [Restore Parameters] button or restart MCU to revert changes");
        
    }
}

/***************************************************************************//*!
* @brief   Restore parameters in target
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function fcnClickMIDRestore()
{
    var errorArray = [];
    var MIDSuccess = 0;
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_success"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        MIDSuccess = pcm.LastVariable_vValue;
    
    if(MIDSuccess)
    {
        /* set update flag */
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_restore', 1, 1));
        if(!UpdateError(errorArray))
            window.alert("Parameters were restored to state prior the last measurement");
    }
}


/***************************************************************************//*!
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
      if(getBoardType() == "HVP");
      {
        //switchParam2BasicMode("I_max",8);
        //switchParam2BasicMode("UDC_max",433);
        //switchParam2BasicMode("CLOOP_Ts",0.0001);
        document.getElementById("PWRSTG_charact").style.display = "none";
      }
   }
   
   /* User's board */
   if(boardID == 1)
   {
      //clickReloadData();
      //enableInputParamBox("I_max");   
      //enableInputParamBox("UDC_max");
      //enableInputParamBox("CLOOP_Ts");
      document.getElementById("PWRSTG_charact").style.display = "";
      /* read user HW data from param file */
      clickReloadData();
   }

   /* update params */
   //copyParent2InnerValById("I_max");
   //copyParent2InnerValById("UDC_max");
   copyParent2InnerValById("CLOOP_Ts");
   copyParent2InnerValById("U_nom");
   copyParent2InnerValById("I_nom");
   copyParent2InnerValById("f_nom");
   copyParent2InnerValById("pp");
   
   /* update HW boar in parent table */
   setParentHtmlValue((prefixM) + "HW_board", boardID);
   
   clickCalculateMID();
   
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
* @brief  Function for periodical reading of FM variables
* @param   
* @return 
* @remarks 
******************************************************************************/
function MIDEnableButton(ButtonID)
{
    if(document.getElementById(ButtonID) != undefined)
    {
        document.getElementById(ButtonID).disabled = false;
        document.getElementById(ButtonID).className = 'fButtonsEnabled';
    } 
}

/***************************************************************************//*!
*
* @brief  Function for periodical reading of FM variables
* @param   
* @return 
* @remarks 
******************************************************************************/
function MIDDisableButton(ButtonID)
{
    if(document.getElementById(ButtonID) != undefined)
    {
        document.getElementById(ButtonID).disabled = true;
        document.getElementById(ButtonID).className = 'fButtonsDisabled';
    } 
}

/***************************************************************************//*!
* @brief:   Test variable range and trim to max or min value     
* @param:   
* @return:   
* @remarks:  
******************************************************************************/
function TestRangeTrimInf(valueId, minVal, maxVal, varUnit, varMessage)
{
  var prefixM = getActiveMotor();
  
 try{
  if(Number(getParentHtmlValue(valueId)) > maxVal)      
  {       
      document.getElementById("img_inf_"+valueId).style.visibility = 'visible';
      document.getElementById("img_inf_"+valueId).title = varMessage+maxVal+" "+varUnit+".";
  } else if(Number(getParentHtmlValue(valueId)) < minVal)      
  {       
      document.getElementById("img_inf_"+valueId).style.visibility = 'visible';
      document.getElementById("img_inf_"+valueId).title = varMessage+minVal+" "+varUnit+".";
  }
  else document.getElementById("img_inf_"+valueId).style.visibility = 'hidden';
 
 }
 catch(err){window.alert("Internal error: unknown ID "+valueId.toString());}
    
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
    