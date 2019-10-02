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
            
  	        setParentHtmlValue((prefixM + "CLOOP_Ts"), FastLoopTs);
        }
        else
        {
        }
        
        copyParent2InnerValById("N_req");
        copyParent2InnerValById("pp");
        
        copyParent2InnerValById("Rs_calib");
        copyParent2InnerValById("I_calib");
        
        copyParent2InnerValById("Id_meas");
        copyParent2InnerValById("Id_ampl");
        
        copyParent2InnerValById("Freq_start");
        copyParent2InnerValById("Freq_min");
        copyParent2InnerValById("Ud_inc");
        copyParent2InnerValById("Freq_dec");
		
		copyParent2InnerValById("CLOOP_F0");
		copyParent2InnerValById("SLOOP_F0");
		copyParent2InnerValById("T_meas");
        
        Calibration = 0;
        
        //calculate constants
        clickCalculateMID();
        
        //Enable/Disable buttons
        MIDEnableButton("CalibrateButton");
        MIDEnableButton("MeasureButton");
        MIDEnableButton("MeasureMechButton");
        MIDEnableButton("GenCalButton");
        MIDEnableButton("PpButtonStart");
        MIDDisableButton("PpButtonStop");
        
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
		reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_J_meas"])[0];
        pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
		reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_B_meas"])[0];
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
		reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MechActive"])[0];
        pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
        
        reference_val = xmlDoc.getElementsByTagName([prefixM]+["state"])[0];
        pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
        reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_state"])[0];
        pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
        reference_val = xmlDoc.getElementsByTagName([prefixM]+["onoff"])[0];
        pcm.SubscribeVariable(reference_val.childNodes[0].nodeValue, 1000);
        reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_Fault"])[0];
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
    switchParam2BasicMode("Freq_start",1000);
    switchParam2BasicMode("Freq_min",200);
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
	//get active motor to selct proper prefix
    prefixM = getActiveMotor();
	
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
    var I_calib        = getParentHtmlValue("I_calib");
    var HWboardID      = getParentHtmlValue("HW_board");
    
    /* Set calibration range +-2A for FSL power stages */
    if(HWboardID < 1)
    {
        setParentHtmlValue((prefixM + "I_calib"), 2);
        copyParent2InnerValById("I_calib");
        I_calib = 2.0;
    }
    
    /* Ensure that Calibration range is inside +- Imax */
    if(I_calib > Imax)
    {
        setParentHtmlValue((prefixM + "I_calib"), Imax);
        copyParent2InnerValById("I_calib");
        I_calib = Imax;
        alert("Calibration range must be inside the I max.");
    }
    //Parameters limitations
    TestRangeTrim("Rs_calib", 0.3, 50, "\u03A9");
    TestRangeTrim("I_calib", 0.3, 8, "A");
    TestRangeTrim("Id_meas", 0.05, getParentHtmlValue("I_calib"), "A");
    TestRangeTrim("Id_ampl", 0.05, getParentHtmlValue("I_calib"), "A");
    TestRangeTrim("Freq_start", 100, (1/dTs/10), "Hz");
    TestRangeTrim("Freq_min", 100, (1/dTs/10), "Hz");
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
    //Enable/Disable buttons
    MIDDisableButton("CalibrateButton");
    MIDDisableButton("MeasureButton");
    MIDDisableButton("GenCalButton");
    MIDDisableButton("PpButtonStart");
    MIDDisableButton("PpButtonStop");
    
    var errorArray = [];
    var dTs        = getParentHtmlValue("CLOOP_Ts");
    
    CharDataStored    = 0;
    
    Rs_calib_MCAT   	= getParentHtmlValue("Rs_calib");
    I_calib         	= getParentHtmlValue("I_calib");
    MID_align_I_MCAT	= -getParentHtmlValue("Id_meas");
    MID_align_T_MCAT	= 1/dTs;
    var Id_increment	= I_calib*2/65;
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_calib', Rs_calib_MCAT, Rs_calib_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_I_calib', I_calib, I_calib));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Id_increment', Id_increment, Id_increment));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignCurrent', MID_align_I_MCAT, MID_align_I_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignDuration', MID_align_T_MCAT, MID_align_T_MCAT));
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MeasurementType', 0, 0));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
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
    var strDat = "", txt;
    var Uerr = [];
    var n, Uavg;
    
    pcm.ReadVariable(xmlDoc.getElementsByTagName([prefixM+"fm_NumOfPoints"])[0].childNodes[0].nodeValue);
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
    setParentHtmlValue([prefixM] + ["User_HW_CalibTable_range"], I_calib);
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
    var prefixM      = getActiveMotor();
    var errorArray   = [];
    var HWboardID    = getParentHtmlValue("HW_board");
    var NumPoint;
    var dUerr        = 0;
    var n            = 0;
    
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_DTcomp_LookUp"])[0].childNodes[0];
    reference=reference_val.nodeValue;
    
    if(HWboardID == 0)
    {
        CharArray = getParentHtmlValueStore("FSL_HW_CalibTable").split(" ");
        CharRange = getParentHtmlValue("FSL_HW_CalibTable_range");
        NumPoint  = getParentHtmlValue("FSL_HW_CalibTable_size");
    }
    
    if(HWboardID == 1)
    {
        CharArray = getParentHtmlValueStore("User_HW_CalibTable").split(" ");
        CharRange = getParentHtmlValue("User_HW_CalibTable_range");
        NumPoint  = getParentHtmlValue("User_HW_CalibTable_size");
    }
      
    //Update every single element in target
    for (var i=0;i<65;i++)
    {
        str = [reference] + ["["] + [i] + ["]"];
        errorArray.push(UpdateFMVariableDirect(str,Number(CharArray[i]),Number(CharArray[i])));
    }
    
    /* Calculate linear coefficient that applies out of calib range */
    I_range = getParentHtmlValue("User_HW_CalibTable_range");
    IStep = I_range*2/(NumPoint-1);
    
    for (var i=0;i<NumPoint;i++)
    {
        if((i<5) || ((i>=59) && (i<64))) 
        {
            //alert(CharArray[i] + "  " + i);
            dUerr = dUerr + (Math.abs(Number(CharArray[i]) - Number(CharArray[i+1])));
            n = n + 1;
        }
    }
      
    LinCoeff = dUerr / (IStep*n);
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_CharIRange', I_range, I_range));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_CharLinCoeff', LinCoeff, LinCoeff));
    
    // display error message
    UpdateError(errorArray);
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
    if(I_calibStr.indexOf('.') < 0)
        I_calibStr =  I_calibStr+".0F";
    else
        I_calibStr =  I_calibStr+"F";
    
    LinCoeffStr = (Math.round(LinCoeff*1e12)/1e12).toString();
    if(LinCoeffStr.indexOf('.') < 0) LinCoeffStr =  LinCoeffStr+".0F";
    else LinCoeffStr = LinCoeffStr+"F";
                 
    //var RelPath_DocLocation = getRelativePath(DocLocation);
    /*************** Headlines                              *********************/
    str = "/*";
    str = str + "----------------------------------------------------------------------\r\n";
    str = str + "Automatically generated file with power-stage characterization data\r\n";
    str = str + "\r\n";
    str = str + "File Name:\r\n{FM_project_loc}/" + DocLocation + "board/char_pwrstg.h \r\n";
    str = str + "\r\n";
    //str = str + "// Date:  " + today.getTime()+ today.getDate()+ ". " + mounth + ", " + today.getFullYear() + "\r\n";
    //document.write(HTML_write_head_line("Date:",""+mounth+ " "+ today.getDate() + ", " + today.getFullYear() + ", " + t+ ""));
    str = str + "Date:  " + mounth + " " + today.getDate() + ", " + today.getFullYear() + ", " + t + "\r\n";
    str = str + "Board:  " + strBoard + "\r\n";
    
    str = str + "----------------------------------------------------------------------";
    str = str + "*/\r\n";
     
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
    //Enable/Disable buttons
    MIDDisableButton("CalibrateButton");
    MIDDisableButton("MeasureButton");
    MIDDisableButton("MeasureMechButton");
    MIDDisableButton("GenCalButton");
    MIDDisableButton("PpButtonStart");
    MIDDisableButton("PpButtonStop");
    
    var prefixM    = getActiveMotor();
    var errorArray = [];
    var dTs        = getParentHtmlValue("CLOOP_Ts");
    
    clickCalculateMID();
    
    MeasurementCompleted = 0;
    MeasFaults = 0; 
    MeasWarnings = 0; 
    
    observer_done_MCAT  = 0;
    Imax                = getParentHtmlValue("I_max");
    I_calib             = getParentHtmlValue("I_calib");
    f_start_MCAT        = getParentHtmlValue("Freq_start");
    f_min_MCAT          = getParentHtmlValue("Freq_min");
    Ud_inc_MCAT         = getParentHtmlValue("Ud_inc");
    f_dec_MCAT          = getParentHtmlValue("Freq_dec");
    MID_align_I_MCAT    = getParentHtmlValue("Id_meas");
    MID_align_T_MCAT    = 1/dTs;
    Id_meas_MCAT        = getParentHtmlValue("Id_meas");
    Id_ampl_MCAT        = getParentHtmlValue("Id_ampl");
    N_req_ke_MCAT       = getParentHtmlValue("N_req")*getParentHtmlValue("pp")/60/10;
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Id_meas', Id_meas_MCAT, Id_meas_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Id_ampl', Id_ampl_MCAT, Id_ampl_MCAT));
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_FreqStart',f_start_MCAT,f_start_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_FreqMin',f_min_MCAT,f_min_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_FreqDec',f_dec_MCAT,f_dec_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_VoltInc',Ud_inc_MCAT,Ud_inc_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_IdReqOpenLoop',Id_meas_MCAT,Id_meas_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_FreqElReq',N_req_ke_MCAT,N_req_ke_MCAT));
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_meas', 0, 0));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ld_meas', 0, 0));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lq_meas', 0, 0));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ke_meas', 0, 0));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignCurrent', MID_align_I_MCAT, MID_align_I_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignDuration', MID_align_T_MCAT, MID_align_T_MCAT));
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MeasurementType', 2, 2));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
    errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 0, 0));
    errorArray.push(UpdateFMVariable(xmlDoc,'onoff', 1, 1));
    
    // display error message                           
    UpdateError(errorArray);          
}

/***************************************************************************//*!
*
* @brief  Button Measure mechanical
* @param   
* @return 
* @remarks 
******************************************************************************/
function clickMechMeasure()
{
    //Enable/Disable buttons
    MIDDisableButton("CalibrateButton");
    MIDDisableButton("MeasureButton");
    MIDDisableButton("MeasureMechButton");
    MIDDisableButton("GenCalButton");
    MIDDisableButton("PpButtonStart");
    MIDDisableButton("PpButtonStop");
    
    var prefixM             = getActiveMotor();
    var errorArray          = [];
    var dTs                 = getParentHtmlValue("CLOOP_Ts");
    var slowTs              = getParentHtmlValue("SLOOP_Ts");
    var TorqueReq           = getParentHtmlValue("T_meas");
    var Istartup            = getParentHtmlValue("Id_meas");
    var Motor_pp            = getParentHtmlValue("pp");
    var SpeedAngScale       = 60/(Motor_pp*2*Math.PI);
    var SpeedAccelThreshold = getParentHtmlValue("N_req") * 0.1;
    var SpeedIntegThreshold = getParentHtmlValue("N_req") * 0.15;
    var SpeedDecelThreshold = getParentHtmlValue("N_req") * 0.5;
    var StartupRamp         = getParentHtmlValue("N_req") / 5 / 10000;
    var Kt                  = getParentHtmlValue("ke") / 2 / Math.PI;
//    var Iaccelerate         = TorqueReq / Kt;
    var Iaccelerate         = TorqueReq / (3/2 * Kt * Motor_pp);
    
    SpeedAccelThreshold_fl = Math.round(SpeedAccelThreshold/SpeedAngScale*10)/10;
    SpeedIntegThreshold_fl = Math.round(SpeedIntegThreshold/SpeedAngScale*10)/10;
    SpeedDecelThreshold_fl = Math.round(SpeedDecelThreshold/SpeedAngScale*10)/10;
    StartupRamp_fl = Math.round((getParentHtmlValue("N_req")/SpeedAngScale / 5 / 10000)*100000)/100000;
    
    MID_align_I_MCAT    = getParentHtmlValue("Id_meas");
    MID_align_T_MCAT    = 1/dTs;
    
    calculateSensorless();
    updateSensorless();
    
    MeasurementCompleted = 0;
    MeasFaults = 0; 
    MeasWarnings = 0; 
    tuned_for_mechanical = 0; 
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechIqStartup', Istartup, Istartup));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechIqAccelerate', Iaccelerate, Iaccelerate));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechSpeedAccelThreshold', SpeedAccelThreshold_fl, SpeedAccelThreshold_fl));
	errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechSpeedAccelThreshold', SpeedIntegThreshold_fl, SpeedIntegThreshold_fl));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechSpeedDecelThreshold', SpeedDecelThreshold_fl, SpeedDecelThreshold_fl));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechStartupRampUp', StartupRamp_fl, StartupRamp_fl));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechStartupRampDown', StartupRamp_fl, StartupRamp_fl));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechTimeoutCount', 30000, 30000));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Mech_Kt', Kt, Kt));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_Mech_Pp', Motor_pp, Motor_pp));
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_J_meas', 0, 0));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_B_meas', 0, 0));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignCurrent', MID_align_I_MCAT, MID_align_I_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignDuration', MID_align_T_MCAT, MID_align_T_MCAT));
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MeasurementType', 3, 3));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
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
    //Enable/Disable buttons
    MIDDisableButton("CalibrateButton");
    MIDDisableButton("MeasureButton");
    MIDDisableButton("MeasureMechButton");
    MIDDisableButton("GenCalButton");
    MIDDisableButton("PpButtonStart");
    MIDEnableButton("PpButtonStop");
    
    var prefixM    = getActiveMotor();
    var errorArray = [];
    var dTs        = getParentHtmlValue("CLOOP_Ts");
    
    MID_align_I_MCAT    = getParentHtmlValue("Id_meas");
    MID_align_T_MCAT    = 1/dTs;
    Id_meas_MCAT        = getParentHtmlValue("Id_meas");
    Freq_req_pp_MCAT    = getParentHtmlValue("N_req")*getParentHtmlValue("pp")/60/20; // Pp measurement speed is Nnom/20
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_PpCurrent',Id_meas_MCAT,Id_meas_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_PpFreq',Freq_req_pp_MCAT,Freq_req_pp_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignCurrent', MID_align_I_MCAT, MID_align_I_MCAT));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_AlignDuration', MID_align_T_MCAT, MID_align_T_MCAT));
    
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_MeasurementType', 1, 1));
    errorArray.push(UpdateFMVariable(xmlDoc,'fm_EnableMeasurement', 1, 1));
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
    //Enable/Disable buttons
    MIDEnableButton("CalibrateButton");
    MIDEnableButton("MeasureButton");
    MIDEnableButton("MeasureMechButton");
    MIDEnableButton("GenCalButton");
    MIDEnableButton("PpButtonStart");
    MIDDisableButton("PpButtonStop");
    
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
    Ld = Math.round(Ld*10000000)/10000000;
    Lq = Math.round(Lq*10000000)/10000000;
    var Imax  = getParentHtmlValue("I_max");
    var UmaxCoeff = getParentHtmlValue("UmaxCoeff");
    var UDCbus_max = getParentHtmlValue("UDC_max");
    var Umax = Math.round(UDCbus_max/UmaxCoeff*10)/10;
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
  
    //********************** Related to Q axis **********************				
    Q_Kps = 2*Att*2*Math.PI*fo*Lq-Rs;
    Q_Kis = Math.pow((2*Math.PI*fo),2)*Lq;		
    Q_Kpz = Q_Kps;
    Q_Kiz = Q_Kis*Ts;
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
        errorArray.push(UpdateFMVariable(xmlDoc,'D_Kp_g',D_Kpz,D_Kpz));
        errorArray.push(UpdateFMVariable(xmlDoc,'D_Ki_g',D_Kiz,D_Kiz));
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_Kp_g',Q_Kpz,Q_Kpz));
        errorArray.push(UpdateFMVariable(xmlDoc,'Q_Ki_g',Q_Kiz,Q_Kiz));
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
    
    // BEMF DQ observer constants 
    I_gain_flt = Math.ceil((Ld/(Ld+Ts*Rs))*1e12)/1e12;
    U_gain_flt = Math.ceil(Ts/(Ld+Ts*Rs)*1e12)/1e12;                         
    E_gain_flt = Math.ceil(Ts/(Ld+Ts*Rs)*1e12)/1e12;
    WI_gain_flt = Math.ceil(Lq*Ts/(Ld+Ts*Rs)*1e12)/1e12;
    
    BemfDQ_Kps = (2*BEMF_DQ_att*2*Math.PI*BEMF_DQ_f0*Ld-Rs);
    BemfDQ_Kis = Ld*Math.pow(2*Math.PI*BEMF_DQ_f0,2);
    
    BemfDQ_Kpz = Math.ceil(BemfDQ_Kps*1e12)/1e12;
    BemfDQ_Kiz = Math.ceil(BemfDQ_Kis*Ts*1e12)/1e12;
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
    
    errorArray.push(UpdateFMVariable(xmlDoc,'E_scale',E_gain_flt,E_gain_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'I_scale',I_gain_flt,I_gain_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'U_scale',U_gain_flt,U_gain_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'WI_scale',WI_gain_flt,WI_gain_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'BEMF_DQ_Kp_g',BemfDQ_Kpz,BemfDQ_Kpz));
    errorArray.push(UpdateFMVariable(xmlDoc,'BEMF_DQ_Ki_g',BemfDQ_Kiz,BemfDQ_Kiz));
    
    // display error message
    UpdateError(errorArray);
}

/***************************************************************************//*!
*
* @brief   Calculate all sensorless parameters
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function calculateSensorless()
{
    var prefixM        = getActiveMotor();
    var BEMF_DQ_f0     = 300;
    var BEMF_DQ_att    = 1;
    var TO_f0          = 20;
    var TO_att         = 1;
    var Ts             = getParentHtmlValue("CLOOP_Ts");
    var SLOOP_Ts       = getParentHtmlValue("SLOOP_Ts");
    var Rs             = getParentHtmlValue("Rs");
    var Ld             = getParentHtmlValue("Ld");
    var Lq             = getParentHtmlValue("Lq");
    var ke             = getParentHtmlValue("ke");
    var UDCmax         = getParentHtmlValue("UDC_max");
    var Imax           = getParentHtmlValue("I_max");
    var UmaxCoeff      = getParentHtmlValue("UmaxCoeff");
	var Nnom           = getParentHtmlValue("N_req");
	var pp             = getParentHtmlValue("pp");
	var IstartUp       = getParentHtmlValue("I_ph");
    var Nmax           = Nnom * 1.1;
    var Nmin           = Nnom * 0.1; 
    var Emax           = ke * (Nmax * pp / 60);
    var RampStartUp    = 1000;
    var Nmerging       = Nnom * 0.15;
    var mergingCoeff   = 100;
    var IIRxCoefsScale = 2;	
    var Umax           = Math.round((UDCmax/UmaxCoeff)*10)/10;
    
    var ThetaMax       = Math.PI;
    var ERRmax         = 1;
    var Wmax           = 2*Math.PI*pp*Nmax/60;
    var SpeedAngScale  = 60/(pp*2*Math.PI);
    
    I_gain_flt = Math.ceil((Ld/(Ld+Ts*Rs))*1e12)/1e12;
    U_gain_flt = Math.ceil(Ts/(Ld+Ts*Rs)*1e12)/1e12;
    E_gain_flt = Math.ceil(Ts/(Ld+Ts*Rs)*1e12)/1e12;
    WI_gain_flt = Math.ceil(Lq*Ts/(Ld+Ts*Rs)*1e12)/1e12;
    
    /* BEMF DQ observer constants */
    BemfDQ_Kps = (2*BEMF_DQ_att*2*Math.PI*BEMF_DQ_f0*Ld-Rs);
    BemfDQ_Kis = Ld*Math.pow(2*Math.PI*BEMF_DQ_f0,2);

    BemfDQ_Kpz = Math.ceil(BemfDQ_Kps*1e12)/1e12;
    BemfDQ_Kiz = Math.ceil(BemfDQ_Kis*Ts*1e12)/1e12;
    
    /* Tracking Observer observer constants */
    TO_Kps = 2*TO_att*2*Math.PI*TO_f0;
    TO_Kis = Math.pow(2*Math.PI*TO_f0,2);
    
    TO_Kpz = Math.ceil(TO_Kps*1e12)/1e12;
    TO_Kiz = Math.ceil(TO_Kis*Ts*1e12)/1e12;
	TO_Theta_flt = Math.ceil(Ts/Math.PI*1e12)/1e12;
    
    TO_Kpz_f = TO_Kpz*(ERRmax/Wmax);
    TO_Kiz_f = TO_Kiz*(ERRmax/Wmax);
	TO_Theta_f = Math.round(Ts*Wmax/ThetaMax*1e12)/1e12;
    
    //Start-up calcs
    rampStartUpInc_flt = Math.round(RampStartUp * Ts/SpeedAngScale*1000000000000)/1000000000000;
    currentStartUp     =  IstartUp;
    speedMerging       =   Math.round(Nmerging/SpeedAngScale*1000000000000)/1000000000000;
    merginCoeff_f      = Math.round((mergingCoeff/100)*(60/(pp*Nmerging))/Ts/2/32768*1000000000000)/1000000000000;
    testValRange("Merging Coeff",merginCoeff_f,1/32767,1); 
    
    // Observer speed output IIR filter
    // Cut off frequency is 80% of half of SPEED LOOP frequency
    TO_W_IIR_cutoff_freq = 1/(2*SLOOP_Ts)*0.8; 
    TO_W_IIR_B0_fl = Math.round((2*Math.PI*TO_W_IIR_cutoff_freq*Ts)/(2+(2*Math.PI*TO_W_IIR_cutoff_freq*Ts))*1000000000000)/1000000000000; 
    TO_W_IIR_B1_fl = Math.round((2*Math.PI*TO_W_IIR_cutoff_freq*Ts)/(2+(2*Math.PI*TO_W_IIR_cutoff_freq*Ts))*1000000000000)/1000000000000;
    TO_W_IIR_A1_fl = -Math.round((2*Math.PI*TO_W_IIR_cutoff_freq*Ts-2)/(2+(2*Math.PI*TO_W_IIR_cutoff_freq*Ts))*1000000000000)/1000000000000;
    TO_W_IIR_B0_out =  Math.round(TO_W_IIR_B0_fl/IIRxCoefsScale*1000000000000)/1000000000000;
    TO_W_IIR_B1_out =  Math.round(TO_W_IIR_B1_fl/IIRxCoefsScale*1000000000000)/1000000000000;
    TO_W_IIR_A1_out =  Math.round(TO_W_IIR_A1_fl/IIRxCoefsScale*1000000000000)/1000000000000;
    testFracValRange("TO_W_IIR_B0_out",TO_W_IIR_B0_out,1);
    testFracValRange("TO_W_IIR_B1_out",TO_W_IIR_B1_out,1);
    testFracValRange("TO_W_IIR_A1_out",TO_W_IIR_A1_out,1);
   
    // If BEMF OBSRV LOOP tab is active ******************************************
    if(document.getElementById("BemfDqObsrv") != undefined)
    {
        // write values to forms
        setInnerHtmlValue("I_scale", I_gain_flt, I_gain_flt);
        setInnerHtmlValue("U_scale", U_gain_flt, U_gain_flt);
        setInnerHtmlValue("E_scale", E_gain_flt, E_gain_flt);  
        setInnerHtmlValue("WI_scale", WI_gain_flt, WI_gain_flt);
       
        setInnerHtmlValue("BEMF_DQ_Kp_g",BemfDQ_Kpz,BemfDQ_Kpz);
        setInnerHtmlValue("BEMF_DQ_Ki_g",BemfDQ_Kiz,BemfDQ_Kiz);
       
        setInnerHtmlValue("TO_Kp_g",TO_Kpz_f,TO_Kpz);
        setInnerHtmlValue("TO_Ki_g",TO_Kpz_f,TO_Kiz);
        setInnerHtmlValue("Theta_g",TO_Theta_f, TO_Theta_flt);
        
        setInnerHtmlValue("Startup_Ramp_inc",rampStartUpInc_flt,rampStartUpInc_flt);
        setInnerHtmlValue("Startup_I_scaled",currentStartUp, currentStartUp);
        setInnerHtmlValue("Startup_N_merging",speedMerging,speedMerging);
        setInnerHtmlValue("Startup_coeff_merging",merginCoeff_f,merginCoeff_f);
    }
	
    // If HEADER FILE tab is active ********************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {	
    }   
}

/***************************************************************************//*!
*
* @brief   Update Sensorless parameters
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function updateSensorless()
{
    var errorArray = [];
    var prefixM    = getActiveMotor();
    
    errorArray.push(UpdateFMVariable(xmlDoc,'E_scale',E_gain_flt, E_gain_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'I_scale',I_gain_flt, I_gain_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'U_scale',U_gain_flt, U_gain_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'WI_scale',WI_gain_flt, WI_gain_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'BEMF_DQ_Kp_g',BemfDQ_Kpz, BemfDQ_Kpz));
    errorArray.push(UpdateFMVariable(xmlDoc,'BEMF_DQ_Ki_g',BemfDQ_Kiz, BemfDQ_Kiz));
    errorArray.push(UpdateFMVariable(xmlDoc,'TO_Kp_g',TO_Kpz_f, TO_Kpz));
    errorArray.push(UpdateFMVariable(xmlDoc,'TO_Ki_g',TO_Kiz_f, TO_Kiz));
    errorArray.push(UpdateFMVariable(xmlDoc,'Theta_g',TO_Theta_f, TO_Theta_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_Ramp_dec',rampStartUpInc_flt, rampStartUpInc_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_Ramp_inc',rampStartUpInc_flt, rampStartUpInc_flt));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_I_scaled_FM',currentStartUp, currentStartUp));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_N_merging',speedMerging, speedMerging));
    errorArray.push(UpdateFMVariable(xmlDoc,'Startup_coeff_merging',merginCoeff_f, merginCoeff_f));  
    
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
	reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_J_meas"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    {
        J_rounded = Math.round(pcm.LastVariable_vValue*1000000)/1000000;
        setInnerHtmlValue("J_result",J_rounded,J_rounded);
    }
	reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_B_meas"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    {
        B_rounded = Math.round(pcm.LastVariable_vValue*10000)/10000;
        setInnerHtmlValue("B_result",B_rounded,B_rounded);
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
    //Read Mech status
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MechActive"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        MechActive = pcm.LastVariable_vValue;
    
    //Write status
    if(AlignActive != 0)
    {
        Status.innerHTML = "Aligning ...";
        document.getElementById("Status").style.color = "blue";
    }
    
    if(CharActive != 0)
    {
        Status.innerHTML = "Characterization ...";
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
	
	if(MechActive != 0)
    {
        Status.innerHTML = "Measuring Mechanical params ...";
        document.getElementById("Status").style.color = "blue";
    }
    
    if((AlignActive == 0) && (CharActive == 0) && (RsActive == 0) && (LsActive == 0) && (KeActive == 0) && (PpActive == 0) && (MechActive == 0) && (HWboardID == 2))
    {
        Status.innerHTML = "Ready for measurement or calibration ...";
        document.getElementById("Status").style.color = "black";
    }
    
    if((AlignActive == 0) && (CharActive == 0) && (RsActive == 0) && (LsActive == 0) && (KeActive == 0) && (PpActive == 0) && (MechActive == 0) && (HWboardID != 2))
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
	errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechActive', 0, 0));
    
    //Enable buttons
    MIDEnableButton("CalibrateButton");
    MIDEnableButton("MeasureButton");
    MIDEnableButton("MeasureMechButton");
    MIDEnableButton("GenCalButton");
    MIDEnableButton("PpButtonStart");
    MIDDisableButton("PpButtonStop");
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
    var HWboardID = getParentHtmlValue("HW_board");
    
    updateResults();
    updateStatus();
    
	//Read MID state
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MID_state"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        MID_state = pcm.LastVariable_vValue;
    
    //Read Measurement Type (if calibration or measurement or pp is done)
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MeasurementType"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        MeasurementType = pcm.LastVariable_vValue;
    
    //Read status of Application switch (after measurement the App switch is turned off)
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["onoff"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        OnOffSwitch = pcm.LastVariable_vValue;
	
    //Read faults
    reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MeasFault"])[0];
    if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
    {
        MeasFaults = pcm.LastVariable_vValue;
        
        //Check and assign faults
        if((MeasFaults != 0))
        {
            //Any fault kills the measurement immediatelly
            if(MeasFaults > 0)
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
                errorArray.push(UpdateFMVariable(xmlDoc,'fm_MechActive', 0, 0));
                
				//Clear electrical or mechanical results
				if(MeasurementType == 2)
				{
					errorArray.push(UpdateFMVariable(xmlDoc,'fm_Rs_meas', 0, 0));
					errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ld_meas', 0, 0));
					errorArray.push(UpdateFMVariable(xmlDoc,'fm_Lq_meas', 0, 0));
					errorArray.push(UpdateFMVariable(xmlDoc,'fm_Ke_meas', 0, 0));
				}
				if(MeasurementType == 3)
				{
					errorArray.push(UpdateFMVariable(xmlDoc,'fm_J_meas', 0, 0));
					errorArray.push(UpdateFMVariable(xmlDoc,'fm_B_meas', 0, 0));
				}
                
                //Add "Faults:"
                aux_str = "Faults: \n\r";
                //Add fault descriptions
                if(MeasFaults & 0x02)
                  aux_str = [aux_str] + ["\n\r -motor not connected"];
                if(MeasFaults & 0x04)
                  aux_str = [aux_str] + ["\n\r -too high Rs for calibration"];
                if(MeasFaults & 0x08)
                  aux_str = [aux_str] + ["\n\r -mechanical measurement timeout"];
                
                //Alert faults
                //alert(aux_str);
                
                //Enable/Disable buttons
                MIDEnableButton("CalibrateButton");
                MIDEnableButton("MeasureButton");
                MIDEnableButton("MeasureMechButton");
                MIDDisableButton("GenCalButton");
                MIDEnableButton("PpButtonStart");
                MIDDisableButton("PpButtonStop");
            }
            
            //Clear faults
            errorArray.push(UpdateFMVariable(xmlDoc,'fm_MeasFault', 0, 0));
            UpdateError(errorArray);
        }
    }
    
    //If calibration is done and data has not been stored
    if((CharDataStored == 0) && (MeasurementType == 0) && (CharActive == 0) && (OnOffSwitch == 0))
    {
        //Enable/Disable buttons
        MIDEnableButton("CalibrateButton");
        MIDEnableButton("MeasureButton");
        MIDEnableButton("MeasureMechButton");
        MIDEnableButton("GenCalButton");
        MIDEnableButton("PpButtonStart");
        MIDDisableButton("PpButtonStop");
        
        StoreCalibTable();
        CharDataStored = 1;
        alert("Calibration data stored.");
    }
  
    //If electrical measurement is completed and measured params has not been sent to Parameters tab
    if((MeasurementCompleted == 0) && (MeasurementType == 2) && (CharActive == 0) && (OnOffSwitch == 0) && (Ke_rounded != 0))
    {
        //Enable/Disable buttons
        MIDEnableButton("CalibrateButton");
        MIDEnableButton("MeasureButton");
        MIDEnableButton("MeasureMechButton");
        MIDEnableButton("GenCalButton");
        MIDEnableButton("PpButtonStart");
        MIDDisableButton("PpButtonStop");
        
          //Alert completed and avoid next alert
          MeasurementCompleted = 1;
          
        //Read faults
        reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MeasWarn"])[0];
        if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        {
            MeasWarnings = pcm.LastVariable_vValue;
            
            //If there were no warnings during measurement
            if(MeasWarnings == 0)
            {
                 alert("Electrical measurement completed.");
            }
            //If there were warnings during measurement
            else
            {
                aux_str = "Measurement completed with warnings: \n\r";
                
                if(MeasWarnings & 0x01)
                    aux_str = [aux_str] + ["\n\r -measuring current Is_DC not reached."];
                if(MeasWarnings & 0x02)
                    aux_str = [aux_str] + ["\n\r -measuring current amplitude Is_AC not reached."];
                if(MeasWarnings & 0x04)
                    aux_str = [aux_str] + ["\n\r -measured value of Rs is probably out of range."];
                if(MeasWarnings & 0x08)
                    aux_str = [aux_str] + ["\n\r -measured value of Ls is probably out of range."];
                if(MeasWarnings & 0x10)
                    aux_str = [aux_str] + ["\n\r -measured value of Ke is probably out of range."];
                
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
    }
	
	//If mechanical measurement is completed and measured params has not been sent to Parameters tab
    if((MeasurementCompleted == 0) && (MeasurementType == 3) && (OnOffSwitch == 0) && (J_rounded != 0))
    {
        //Enable/Disable buttons
        MIDEnableButton("CalibrateButton");
        MIDEnableButton("MeasureButton");
        MIDEnableButton("MeasureMechButton");
        MIDEnableButton("GenCalButton");
        MIDEnableButton("PpButtonStart");
        MIDDisableButton("PpButtonStop");
        
        //Alert completed and avoid next alert
        MeasurementCompleted = 1;
          
        //Read faults
        reference_val = xmlDoc.getElementsByTagName([prefixM]+["fm_MeasWarn"])[0];
        if(pcm.ReadVariable(reference_val.childNodes[0].nodeValue))
        {
            MeasWarnings = pcm.LastVariable_vValue;
            
            //If there were no warnings during measurement
            if(MeasWarnings == 0)
            {
                 alert("Mechanical measurement completed.");
            }
            //If there were warnings during measurement
            else
            {
                aux_str = "Measurement completed with warnings: \n\r";
                
                if(MeasWarnings & 0x01)
                    aux_str = [aux_str] + ["\n\r -measuring current Is_DC not reached."];
                if(MeasWarnings & 0x02)
                    aux_str = [aux_str] + ["\n\r -measuring current amplitude Is_AC not reached."];
                if(MeasWarnings & 0x04)
                    aux_str = [aux_str] + ["\n\r -measured value of Rs is probably out of range."];
                if(MeasWarnings & 0x08)
                    aux_str = [aux_str] + ["\n\r -measured value of Ls is probably out of range."];
                if(MeasWarnings & 0x10)
                    aux_str = [aux_str] + ["\n\r -measured value of Ke is probably out of range."];
                
                //Alert warnings
                alert(aux_str);
                
                //Clear MeasWarning for the next measurement
                MeasWarnings = 0;
            }
            
            //sent params to Parameters tab
			setParentHtmlValue([prefixM] + ["J"],J_rounded);
            
            clickStoreData();
        }
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
        errorArray.push(UpdateFMVariable(xmlDoc,'fm_MID_MCATObserverDone', 1, 1));
        
        // display error message
        UpdateError(errorArray); 
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
            document.getElementById("PWRSTG_charact").style.display = "none";
        }
        
        if(getBoardType() == "TWR")
        {
            document.getElementById("PWRSTG_charact").style.display = "none";
        }
        
        if(getBoardType() == "FRDM")
        {
            document.getElementById("PWRSTG_charact").style.display = "none";
        }
        
        if(getBoardType() == "EVK")
        {
            document.getElementById("PWRSTG_charact").style.display = "none";
        }
    }
    
    /* User's board */
    if(boardID == 1)
    {
        document.getElementById("PWRSTG_charact").style.display = "";
        /* read user HW data from param file */
        clickReloadData();
    }
    
    /* update HW board in parent table */
    setParentHtmlValue((prefixM) + "HW_board", boardID);
    
    /* Reload dead time compensation lookup table */
    LoadCalibTable();
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
  
    try
    {
        if(Number(getParentHtmlValue(valueId)) > maxVal)      
        {       
            document.getElementById("img_inf_"+valueId).style.visibility = 'visible';
            document.getElementById("img_inf_"+valueId).title = varMessage+maxVal+" "+varUnit+".";
        }
        else if(Number(getParentHtmlValue(valueId)) < minVal)      
        {       
            document.getElementById("img_inf_"+valueId).style.visibility = 'visible';
            document.getElementById("img_inf_"+valueId).title = varMessage+minVal+" "+varUnit+".";
        }
        else
            document.getElementById("img_inf_"+valueId).style.visibility = 'hidden';
    }
    catch(err)
    {
        window.alert("Internal error: unknown ID "+valueId.toString());
    }
    
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/

