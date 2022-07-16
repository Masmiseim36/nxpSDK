/*******************************************************************************
*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause* 
*
****************************************************************************//*!
*
* @file   control_page_config.js
*
* @brief  Script file containing identification structures
*
******************************************************************************/
/******************************************************************************
| global variable
|----------------------------------------------------------------------------*/
/* application and board identification variables */
var boardID = new String();
var motorType = new String();
var appVersion;
var CpuFreq;
var SlowLoopFreq;
var MaxCycleNumber;

/* Default ranges and texts in case communication between MCU nad FM is not established */
var heel            = "NXP Semiconductors, Inc. 2019. Designed by Motor Control Team / Microcontroller Product Group";
var voltageSpan     = "DC-Bus Voltage [Volts]";
var CPUloadSpan     = "MC CPU Load [%]";
var currentLimSpan  = "Current Limitation [Amps]";
var sliderSpeedSpan = "Speed Required [rpm]";
var currentScale    = 8;
var currentInterMaj = 1;
var speedScale      = 4500/100;
var speedNominal    = 4000/100;
var speedMinimum    = 500/100;
var speedInterMaj   = 5;
var dcbVoltScale    = 36.3;
var dcbVoltInterMaj = 5;
var dcbVoltOver     = 30;
var dcbVoltUnder    = 10;
var appState        = 0;
var demoMode        = 0;

/* stuctures of texts selected based on connected MUC */
/************* PMSM Sensorless Control ****************/
/*************           KE15          ****************/
var pmsm_frdm_ke15z = {
    title   :   "PMSM Sensorless Control using FRDM-KE15Z",
}

/*************           KE18          ****************/
var pmsm_twr_ke18f = {
    title   :   "PMSM Sensorless Control using TWR-KE18F",
}

var pmsm_hvp_ke18f = {
    title   :   "PMSM Sensorless Control using HVP-KE18F",
}
/*************           KV10          ****************/
var pmsm_twr_kv10z = {
    title   :   "PMSM Sensorless Control using TWR-KV10Z32",
}

var pmsm_frdm_kv10z = {
    title   :   "PMSM Sensorless Control using FRDM-KV10Z32",
}

var pmsm_hvp_kv10z = {
    title   :   "PMSM Sensorless Control using HVP-KV10Z32",
}

/*************           KV11          ****************/
var pmsm_twr_kv11z = {
    title   :   "PMSM Sensorless Control using TWR-KV11Z75M",
}


/*************           KV31          ****************/
var pmsm_twr_kv31f = {
    title   :   "PMSM Sensorless Control using TWR-KV31F120M",
}

var pmsm_frdm_kv31f = {
    title   :   "PMSM Sensorless Control using FRDM-KV31F",
}

var pmsm_hvp_kv31f = {
    title   :   "PMSM Sensorless Control using HVP-KV31F120M",
}

/*************           KV46          ****************/
var pmsm_twr_kv46f = {
    title   :   "PMSM Sensorless Control using TWR-KV46F150",
}

var pmsm_hvp_kv46f = {
    title   :   "PMSM Sensorless Control using HVP-KV46F150",
}

/*************           KV58          ****************/
var pmsm_twr_kv58f = {
    title   :   "PMSM Sensorless Control using TWR-KV58F220",
}

var pmsm_hvp_kv58f = {
    title   :   "PMSM Sensorless Control using HVP-KV58F220",
}

var pmsm_evk_imxrt1050 = {
    title   :   "PMSM  Control using evk-imxrt1050",
}

var pmsm_evk_imxrt1060 = {
    title   :   "PMSM  Control using evk-imxrt1060",
}

var pmsm_evkb_imxrt1060 = {
    title   :   "PMSM  Control using evkb-imxrt1060",
}

var pmsm_evk_imxrt1064 = {
    title   :   "PMSM  Control using evk-imxrt1064",
}

var pmsm_evk_imxrt1024 = {
    title   :   "PMSM  Control using evk-imxrt1024",
}

var pmsm_evk_imxrt1020 = {
    title   :   "PMSM  Control using evk-imxrt1020",
}

var pmsm_evk_imxrt1010 = {
    title   :   "PMSM  Control using evk-imxrt1010",
}

var pmsm_evk_imxrt1170 = {
    title   :   "PMSM  Control using evk-imxrt1170",
}

var pmsm_evk_lpc55s69 = {
    title   :   "PMSM  Control using evk-lpc55s69",
}

var pmsm_frdm_k32l = {
    title   :   "PMSM  Control using FRDM-K32L4A",
}


/******************************************************************************
@brief   Read application identification paramters and parse them

@param   void

@return  none
******************************************************************************/
function AppIdenInit()
{

    /* read identification values of MCU, board , motor type nad sw version */
    succ = pcm.ReadVariable("Board ID", vValue0, tValue0, retMsg)
    if (succ)
        boardID = pcm.LastVariable_tValue;

    succ = pcm.ReadVariable("Motor Type", vValue0, tValue0, retMsg)
    if (succ)
        motorType = pcm.LastVariable_tValue;

    succ = pcm.ReadVariable("App Version", vValue0, tValue0, retMsg)
    if (succ)
        appVersion = Math.round(pcm.LastVariable_vValue*100)/100;
	
	succ = pcm.ReadVariable("M1 CPU Frequency", vValue0, tValue0, retMsg)
    if (succ)
        CpuFreq = Math.round(pcm.LastVariable_vValue*100)/100;
	else
		CpuFreq = 600000000;
	
	succ = pcm.ReadVariable("M1 Fast Control Loop Frequency", vValue0, tValue0, retMsg)
    if (succ)
        SlowLoopFreq = Math.round(pcm.LastVariable_vValue*100)/100;
	else
		SlowLoopFreq = 10000;
		
	MaxCycleNumber = (10000*CpuFreq/SlowLoopFreq);
	
    /* put together motor type and board */
    appId = motorType + '_' + boardID;

    switch(appId){
    case "pmsm_frdm-ke15z":
        document.getElementById("title").innerHTML = pmsm_frdm_ke15z.title;
        break;
    case "pmsm_frdm-kv10z":
        document.getElementById("title").innerHTML = pmsm_frdm_kv10z.title;
        break;
    case "pmsm_twr-kv10z":
        document.getElementById("title").innerHTML = pmsm_twr_kv10z.title;
        break;
    case "pmsm_hvp-kv10z":
        document.getElementById("title").innerHTML = pmsm_hvp_kv10z.title;
        break;
    case "pmsm_twr-kv11z":
        document.getElementById("title").innerHTML = pmsm_twr_kv11z.title;
        break;
    case "pmsm_frdm-kv31":
        document.getElementById("title").innerHTML = pmsm_frdm_kv31f.title;		
        break;
    case "pmsm_twr-kv31":
        document.getElementById("title").innerHTML = pmsm_twr_kv31f.title;
        break;
    case "pmsm_hvp-kv31":
        document.getElementById("title").innerHTML = pmsm_hvp_kv31f.title;
        break;
    case "pmsm_twr-kv46":
        document.getElementById("title").innerHTML = pmsm_twr_kv46f.title;
        break;
    case "pmsm_hvp-kv46":
        document.getElementById("title").innerHTML = pmsm_hvp_kv46f.title;
        break;
    case "pmsm_twr-kv58":
        document.getElementById("title").innerHTML = pmsm_twr_kv58f.title;
        break;
    case "pmsm_hvp-kv58":
        document.getElementById("title").innerHTML = pmsm_hvp_kv58f.title;
        break;
    case "pmsm_twr-ke18":
        document.getElementById("title").innerHTML = pmsm_twr_ke18f.title;
        break;
    case "pmsm_hvp-ke18":
        document.getElementById("title").innerHTML = pmsm_hvp_ke18f.title;
        break;
    case "pmsm_evk-imxrt1050":
        document.getElementById("title").innerHTML = pmsm_evk_imxrt1050.title;
        break;
	case "pmsm_evk-imxrt1024":
        document.getElementById("title").innerHTML = pmsm_evk_imxrt1024.title;
        break;
	case "pmsm_evk-imxrt1020":
        document.getElementById("title").innerHTML = pmsm_evk_imxrt1020.title;
        break;
	case "pmsm_evk-imxrt1010":
        document.getElementById("title").innerHTML = pmsm_evk_imxrt1010.title;
        break;
	case "pmsm_evk-imxrt1170":
        document.getElementById("title").innerHTML = pmsm_evk_imxrt1170.title;
        break;
	case "pmsm_evk-imxrt1060":
        document.getElementById("title").innerHTML = pmsm_evk_imxrt1060.title;
        break;
	case "pmsm_evkb-imxrt1060":
        document.getElementById("title").innerHTML = pmsm_evkb_imxrt1060.title;
        break;
	case "pmsm_evk-imxrt1064":
        document.getElementById("title").innerHTML = pmsm_evk_imxrt1064.title;
        break;
	case "pmsm_evk-lpc55s69":
        document.getElementById("title").innerHTML = pmsm_evk_lpc55s69.title;
        break;	
	case "pmsm_frdm-k32l":
        document.getElementById("title").innerHTML = pmsm_frdm_k32l.title;
        break;	
    default:
        document.getElementById("title").innerHTML = "PMSM Motor Control";
        break;
    }

    /* update gauge texts according to MCU */
    var titleSpan = document.getElementById("title");
    titleSpan.style.fontSize = "25px";	titleSpan.style.fontWeight = 'bold';

    document.getElementById("heel").innerHTML = heel;
    var heelSpan = document.getElementById("heel");
    heelSpan.style.fontSize = "15px";

    document.getElementById("DCVoltage").innerHTML = voltageSpan;
    var voltSpan = document.getElementById("DCVoltage");
    voltSpan.style.fontSize = "15px";voltSpan.style.fontWeight = 'bold';
	
	document.getElementById("CPULoad").innerHTML = CPUloadSpan;
    var voltSpan = document.getElementById("CPULoad");
    voltSpan.style.fontSize = "15px";voltSpan.style.fontWeight = 'bold';

    document.getElementById("CurrentLimit").innerHTML = currentLimSpan;
    var currLimSpan = document.getElementById("CurrentLimit");
    currLimSpan.style.fontSize = "15px";currLimSpan.style.fontWeight = 'bold';

    document.getElementById("SpeedSlider").innerHTML = sliderSpeedSpan;
    var speedSpan = document.getElementById("SpeedSlider");
    speedSpan.style.fontSize = "15px";speedSpan.style.fontWeight = 'bold';

}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
