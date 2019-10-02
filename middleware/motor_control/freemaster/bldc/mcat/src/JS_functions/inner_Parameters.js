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

    
    // mandatory input parameters
    copyParent2InnerValById("pp");
   
    copyParent2InnerValById("I_ph_nom");
    copyParent2InnerValById("U_ph_nom");
    copyParent2InnerValById("N_req");
    
    copyParent2InnerValById("I_max");
    copyParent2InnerValById("UDC_max");
    
    // fault limits
    copyParent2InnerValById("IDC_over");
    copyParent2InnerValById("UDC_under");
    copyParent2InnerValById("UDC_over");
    copyParent2InnerValById("N_over");
    copyParent2InnerValById("N_min");

    
    // precalculated or manualy added
    copyParent2InnerValById("UDC_trip");
    copyParent2InnerValById("IDC_limit");
    copyParent2InnerValById("N_max"); 
    copyParent2InnerValById("ke");         
    copyParent2InnerValById("PWM_freq");
    
    copyParent2InnerValById("ALIGN_U");
    copyParent2InnerValById("ALIGN_I");
    copyParent2InnerValById("ALIGN_T");
    
    
    if(testVarValue('Alignment','Voltage'))
    {
      document.getElementById("Volt_align").style.display = "";
      document.getElementById("Curr_align").style.display = "none";
    }
    else
    {
      document.getElementById("Volt_align").style.display = "none";
      document.getElementById("Curr_align").style.display = "";
    }
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
    var obj = document.getElementById("Alignment").getElementsByTagName('legend')[0];
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
    var obj = document.getElementById("Alignment").getElementsByTagName('legend')[0];
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
    var N_req_max     = getParentHtmlValue("N_req");
    var I_nom         = getParentHtmlValue("I_ph_nom");
    var U_nom         = getParentHtmlValue("U_ph_nom");
    var Imax          = getParentHtmlValue("I_max");
    var pp            = getParentHtmlValue("pp");

    // calculated input parameters
    UDCB_under  = Math.round(UDCB_max*0.4*10)/10;
    UDCB_over   = Math.round(UDCB_max*0.8*10)/10;
    UDCB_trip   = Math.round(UDCB_max*0.8*10)/10;
    N_max       = Math.round(1.1*N_req_max*10)/10;
    N_max_over  = Math.round(N_max*0.95*10)/10;
    N_min_under = Math.round(N_req_max*0.05*10)/10;
    ke          = Math.round(U_nom/(2*Math.PI*pp*N_req_max/60)*100000)/100000;
    U_align     = Math.round(U_nom*0.1*10)/10;
    I_align     = Math.round(I_nom*0.8*100)/100;
    // default alignment time 1sec
    T_align = 1;
    
    
    // replace and disable params
    switchParam2BasicMode("UDC_under",UDCB_under);
    switchParam2BasicMode("UDC_over",UDCB_over);
    switchParam2BasicMode("UDC_trip",UDCB_trip);
    switchParam2BasicMode("N_over",N_max_over);
    switchParam2BasicMode("N_min",N_min_under);
    switchParam2BasicMode("N_max",N_max);
    switchParam2BasicMode("ke",ke);
    switchParam2BasicMode("ALIGN_U",U_align);
    switchParam2BasicMode("ALIGN_I",I_align);
    switchParam2BasicMode("ALIGN_T",T_align);
    
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
    Align_volt        = getParentHtmlValue("ALIGN_U"); //need to be global var
    Align_cur         = getParentHtmlValue("ALIGN_I"); //need to be global var
    
    var CtrlLOOP_Ts   = getParentHtmlValue("CtrlLOOP_Ts");
    var Align_dur     = getParentHtmlValue("ALIGN_T");
    var Imax          = getParentHtmlValue("I_max");
    var UDCmax        = getParentHtmlValue("UDC_max");
    var Nmax          = getParentHtmlValue("N_max");
    var UDCtrip       = getParentHtmlValue("UDC_trip"); 
    var IDClimit      = getParentHtmlValue("IDC_limit");
    var UDCunder      = getParentHtmlValue("UDC_under");
    var UDCover       = getParentHtmlValue("UDC_over");
    var IDCover       = getParentHtmlValue("IDC_over");
    var Nreqmax       = getParentHtmlValue("N_req");
    var IphLim        = getParentHtmlValue("I_ph_nom");
    var Uphnom        = getParentHtmlValue("U_ph_nom");
    var Calib_dur     = getParentHtmlValue("CALIB_T");
    var Fault_dur     = getParentHtmlValue("FAULT_T");
    var UdcIIRCutOff  = getParentHtmlValue("UDC_FILT_FREQ");
    var IdcIIRCutOff  = getParentHtmlValue("IDC_FILT_FREQ");
    var PwmFreq       = getParentHtmlValue("PWM_freq");
    
    var IIRxCoefsScaleType     = parent.document.getElementById("IIRxCoefsScale").innerText;
    
    Align_volt_sc   = Math.round(Align_volt/UDCmax*1000000000000)/1000000000000;
    testValRange("Align voltage",Align_volt,0,UDCmax);
    Align_cur_sc    = Math.round(Align_cur/Imax*1000000000000)/1000000000000;
    testValRange("Align_current",Align_cur,0,Imax);
    Align_dur_sc    = Math.round(Align_dur/0.001*1000000000000)/1000000000000;
    testValRange("Align_duration",Align_dur,0.1,30);
    Calib_dur_sc =  Math.round(Calib_dur/CtrlLOOP_Ts);
    testValRange("Calib_duration",Calib_dur,0.001,10);
    Fault_dur_sc =  Math.round(Fault_dur/CtrlLOOP_Ts);
    testValRange("Fault_duration",Fault_dur,0.1,15);
    
    IDC_over_sc     = Math.round(IDCover/Imax*1000000000000)/1000000000000;
    testValRange("IDCB_over",IDCover,0,Imax);
    IDC_limit_sc    = Math.round(IDClimit/Imax*1000000000000)/1000000000000;
    testValRange("IDCB_limit",IDClimit,0,Imax);
    UDC_trip_sc     = Math.round(UDCtrip/UDCmax*1000000000000)/1000000000000;
    testValRange("UDCB_trip",UDCtrip,0,UDCmax);
    UDC_under_sc    = Math.round(UDCunder/UDCmax*1000000000000)/1000000000000;
    testValRange("UDCB_under",UDCunder,0,UDCmax);
    UDC_over_sc     = Math.round(UDCover/UDCmax*1000000000000)/1000000000000;
    testValRange("UDCB_over",UDCover,0,UDCmax);
    N_req_max_sc    = Math.round(Nreqmax/Nmax*1000000000000)/1000000000000;
    testValRange("N_req",Nreqmax,0,Nmax);
    I_ph_lim_sc     = Math.round(IphLim/Imax*1000000000000)/1000000000000;
    testValRange("Iph_nom",IphLim,0,Imax);
    U_ph_nom_sc     = Math.round(Uphnom/UDCmax*1000000000000)/1000000000000;
    testValRange("Uph_nom",Uphnom,0,UDCmax);

    /*  Actual DCBus voltage IIR filter constant calculation */
    UDCB_IIR_B0_fl = Math.round((2*Math.PI*UdcIIRCutOff/PwmFreq)/(2+(2*Math.PI*UdcIIRCutOff/PwmFreq))*1000000000000)/1000000000000;
    UDCB_IIR_B1_fl = Math.round((2*Math.PI*UdcIIRCutOff/PwmFreq)/(2+(2*Math.PI*UdcIIRCutOff/PwmFreq))*1000000000000)/1000000000000;
    UDCB_IIR_A1_fl = Math.round((2*Math.PI*UdcIIRCutOff/PwmFreq-2)/(2+(2*Math.PI*UdcIIRCutOff/PwmFreq))*1000000000000)/1000000000000;
    UDCB_IIR_B0_out =  Math.round(4.0*UDCB_IIR_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    UDCB_IIR_B1_out =  Math.round(4.0*UDCB_IIR_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    UDCB_IIR_A1_out =  Math.round(-4.0*UDCB_IIR_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    testFracValRange("UDCB_IIR_B0_out",UDCB_IIR_B0_out,1);
    testFracValRange("UDCB_IIR_B1_out",UDCB_IIR_B1_out,1);
    testFracValRange("UDCB_IIR_A1_out",UDCB_IIR_A1_out,1);

    /*  Actual DCBus current IIR filter constant calculation */
    IDCB_IIR_B0_fl = Math.round((2*Math.PI*IdcIIRCutOff/PwmFreq)/(2+(2*Math.PI*IdcIIRCutOff/PwmFreq))*1000000000000)/1000000000000;
    IDCB_IIR_B1_fl = Math.round((2*Math.PI*IdcIIRCutOff/PwmFreq)/(2+(2*Math.PI*IdcIIRCutOff/PwmFreq))*1000000000000)/1000000000000;
    IDCB_IIR_A1_fl = Math.round((2*Math.PI*IdcIIRCutOff/PwmFreq-2)/(2+(2*Math.PI*IdcIIRCutOff/PwmFreq))*1000000000000)/1000000000000;
    IDCB_IIR_B0_out =  Math.round(4.0*IDCB_IIR_B0_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    IDCB_IIR_B1_out =  Math.round(4.0*IDCB_IIR_B1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    IDCB_IIR_A1_out =  Math.round(-4.0*IDCB_IIR_A1_fl/IIRxCoefsScaleType*1000000000000)/1000000000000;
    testFracValRange("IDCB_IIR_B0_out",IDCB_IIR_B0_out,1);
    testFracValRange("IDCB_IIR_B1_out",IDCB_IIR_B1_out,1);
    testFracValRange("IDCB_IIR_A1_out",IDCB_IIR_A1_out,1);
      
    
    // If PARAMETERS tab is active ******************************************
    if(document.getElementById("HeaderFileTab") != undefined)
    {
      // write maximal scales to forms in Output File HTML page
      setInnerHtmlValueAsText("I_MAX",5,Imax, Imax);
      setInnerHtmlValueAsText("U_DCB_MAX",5,UDCmax, UDCmax);
      setInnerHtmlValueAsText("N_MAX",5,Nmax, Nmax);
      
      setInnerHtmlValueAsText("I_DCB_OVERCURRENT",0,IDC_over_sc, IDCover);
      setInnerHtmlValueAsText("U_DCB_UNDERVOLTAGE",0,UDC_under_sc, UDCunder);
      setInnerHtmlValueAsText("U_DCB_OVERVOLTAGE",0,UDC_over_sc, UDCover);
      
      setInnerHtmlValueAsText("I_DCB_LIMIT",0,IDC_limit_sc, IDClimit);
      setInnerHtmlValueAsText("U_DCB_TRIP",0,UDC_trip_sc, UDCtrip);
      setInnerHtmlValueAsText("N_NOM",0, N_req_max_sc, Nreqmax);
      setInnerHtmlValueAsText("I_PH_NOM",0,I_ph_lim_sc,IphLim);
      setInnerHtmlValueAsText("U_PH_NOM",0,U_ph_nom_sc,Uphnom);
    
      // write values to forms in Output File Html page
      if(testVarValue('Alignment','Voltage'))
        setInnerHtmlValueAsText("ALIGN_VOLTAGE",0,Align_volt_sc, Align_volt);
      else
        setInnerHtmlValueAsText("ALIGN_CURRENT",0,Align_cur_sc, Align_cur);  
      
      setInnerHtmlValueAsText("ALIGN_DURATION",2,Align_dur_sc);
      setInnerHtmlValueAsText("CALIB_DURATION",2,Calib_dur_sc);
      setInnerHtmlValueAsText("FAULT_DURATION",2,Fault_dur_sc);

        /* Actual DCBus voltage IIR filter */
        setInnerHtmlValueAsText("UDCB_IIR_B0",6,UDCB_IIR_B0_out,UDCB_IIR_B0_out);
        setInnerHtmlValueAsText("UDCB_IIR_B1",6,UDCB_IIR_B1_out,UDCB_IIR_B1_out);
        setInnerHtmlValueAsText("UDCB_IIR_A1",6,UDCB_IIR_A1_out,UDCB_IIR_A1_out);

        /* Actual ICBus voltage IIR filter */
        setInnerHtmlValueAsText("IDCB_IIR_B0",6,IDCB_IIR_B0_out,IDCB_IIR_B0_out);
        setInnerHtmlValueAsText("IDCB_IIR_B1",6,IDCB_IIR_B1_out,IDCB_IIR_B1_out);
        setInnerHtmlValueAsText("IDCB_IIR_A1",6,IDCB_IIR_A1_out,IDCB_IIR_A1_out);
      
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

        if(testVarValue('Alignment','Voltage'))
          errorArray.push(UpdateFMVariable(xmlDoc,'Align_Voltage',Align_volt,Align_volt));
        else
          errorArray.push(UpdateFMVariable(xmlDoc,'Align_Current',Align_cur,Align_cur_sc));

        // alignment duration
        errorArray.push(UpdateFMVariable(xmlDoc,'Align_Duration',Align_dur_sc,Align_dur_sc));

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
    document.write(HTML_write_comment_line("Motor Parameters","",""));
    document.write(HTML_write_comment_line_dash());
    document.write(HTML_write_comment_line("Pole-pair numbers","pp",""));
    document.write(HTML_write_comment_line("Back-EMF constant","ke",""));
    document.write(HTML_write_comment_line("Phase current nominal","I_nom",""));
    document.write(HTML_write_comment_line("Phase voltage nominal","U_nom",""));
    document.write(HTML_write_comment_line("Speed motor nominal","N_mech_nom",""));
    document.write(HTML_write_comment_line_dash());
    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Application Scales","",""));
    document.write(HTML_write_comment_line_dash());
    document.write(HTML_write_define_line_number(prefix,0,"I_MAX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_MAX",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"N_MAX",xmlObject));
    document.write(HTML_write_blank_line());
    document.write(HTML_write_define_line_number(prefix,0,"I_DCB_OVERCURRENT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_UNDERVOLTAGE",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_OVERVOLTAGE",xmlObject));
    document.write(HTML_write_blank_line());
    document.write(HTML_write_define_line_number(prefix,0,"I_DCB_LIMIT",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_DCB_TRIP",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"N_NOM",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"I_PH_NOM",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"U_PH_NOM",xmlObject));

    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Mechanical alignment","",""));
    document.write(HTML_write_comment_line_dash());
    
    // Alignment control              
    document.write(HTML_write_define_line_number(prefix,0,"ALIGN_DURATION",xmlObject));
    if(testVarValue('Alignment','Voltage'))
      document.write(HTML_write_define_line_number(prefix,0,"ALIGN_VOLTAGE",xmlObject));
    else  
      document.write(HTML_write_define_line_number(prefix,0,"ALIGN_CURRENT",xmlObject));

    // Application counters
    document.write(HTML_write_define_line_number(prefix,0,"CALIB_DURATION",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"FAULT_DURATION",xmlObject));

    /* Udcb and Idcb IIR filter constats */
    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Udcb and Idcb IIR Filters","",""));
    document.write(HTML_write_comment_line_dash());
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_B0",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_B1",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"UDCB_IIR_A1",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"IDCB_IIR_B0",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"IDCB_IIR_B1",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"IDCB_IIR_A1",xmlObject));

    // motor parameters commented
    document.write(HTML_write_blank_line());
    copyParent2HeaderCfgById('pp','pp',' [-]',true);
    copyParent2HeaderCfgById('ke','ke',' [V.sec/rad]',true);
    copyParent2HeaderCfgById('I_nom','I_ph_nom',' [A]',true);
    copyParent2HeaderCfgById('U_nom','U_ph_nom',' [V]',true);
    copyParent2HeaderCfgById('N_mech_nom','N_req',' [rpm]',true);

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
    str = write_comment_text(str,'Motor Parameters','');
    str = write_comment_line_dash(str);
    str = write_comment_text(str,'Pole-pair number','pp');
    str = write_comment_text(str,'Back-EMF constant','ke');
    str = write_comment_text(str,'Phase current nominal','I_nom');
    str = write_comment_text(str,'Phase voltage nominal','U_nom');
    str = write_comment_line_dash(str);


    str = write_blank_lines(str,1);
    str = write_comment_text(str,'Application scales','');
    str = write_define_line_number(prefix,str,'I_MAX');
    str = write_define_line_number(prefix,str,'U_DCB_MAX');
    str = write_define_line_number(prefix,str,'N_MAX');

    str = write_define_line_number(prefix,str,'I_DCB_OVERCURRENT');
    str = write_define_line_number(prefix,str,'U_DCB_UNDERVOLTAGE');
    str = write_define_line_number(prefix,str,'U_DCB_OVERVOLTAGE');
    str = write_define_line_number(prefix,str,'I_DCB_LIMIT');
    str = write_define_line_number(prefix,str,'U_DCB_TRIP');
    str = write_define_line_number(prefix,str,'N_NOM');
    str = write_define_line_number(prefix,str,'I_PH_NOM');
    str = write_define_line_number(prefix,str,'U_PH_NOM');

    str = write_comment_text(str,'Mechanical Alignemnt','');
    str = write_define_line_number(prefix,str,'ALIGN_CURRENT');
    str = write_define_line_number(prefix,str,'ALIGN_VOLTAGE');
    str = write_define_line_number(prefix,str,'ALIGN_DURATION');

    str = write_comment_text(str,'Application counters','');
    str = write_define_line_number(prefix, str,'CALIB_DURATION');
    str = write_define_line_number(prefix, str,'FAULT_DURATION');

    str = write_comment_text(str,'Udcb and Idcb Filters','');
    str = write_define_line_number(prefix,str,'UDCB_IIR_B0');
    str = write_define_line_number(prefix,str,'UDCB_IIR_B1');
    str = write_define_line_number(prefix,str,'UDCB_IIR_A1');
    str = write_define_line_number(prefix,str,'IDCB_IIR_B0');
    str = write_define_line_number(prefix,str,'IDCB_IIR_B1');
    str = write_define_line_number(prefix,str,'IDCB_IIR_A1');
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
