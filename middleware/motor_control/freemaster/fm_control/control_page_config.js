/*******************************************************************************
*
* Copyright 2018 Freescale Semiconductor, Inc.

*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale License
* distributed with this Material.
* See the LICENSE file distributed for more details.
*
****************************************************************************//*!
*
* @file   control_page_config.js
*
* @brief  Script file containing identification sctructures
*
******************************************************************************/
/******************************************************************************
| global variable
|----------------------------------------------------------------------------*/
/* application and board identification variables */
var boardID = new String();
var motorType = new String();
var appVersion;

/* Default ranges and texts in case communication between MCU nad FM is not established */
var heel            = "NXP Semiconductors, Inc. 2016. Designed by Motor Control Team / Microcontroller Product Group";
var voltageSpan     = "DC-Bus Voltage";
var currentLimSpan  = "Current Limitation";
var sliderSpeedSpan = "Speed Required";
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
    title   : "PMSM Sensorless Control using FRDM-KE15Z",
}

/*************           KE18          ****************/
var pmsm_twr_ke18f = {
    title   : "PMSM Sensorless Control using TWR-KE18F",
}

var pmsm_hvp_ke18f = {
    title   : "PMSM Sensorless Control using HVP-KE18F",
}
/*************           KV10          ****************/
var pmsm_twr_kv10z = {
    title   : "PMSM Sensorless Control using TWR-KV10Z32",
}

var pmsm_frdm_kv10z = {
    title   : "PMSM Sensorless Control using FRDM-KV10Z32",
}

var pmsm_hvp_kv10z = {
    title   : "PMSM Sensorless Control using HVP-KV10Z32",
}

/*************           KV11          ****************/
var pmsm_twr_kv11z = {
    title   : "PMSM Sensorless Control using TWR-KV11Z75M",
}


/*************           KV31          ****************/
var pmsm_twr_kv31f = {
    title   : "PMSM Sensorless Control using TWR-KV31F120M",
}

var pmsm_frdm_kv31f = {
    title   : "PMSM Sensorless Control using FRDM-KV31F",
}

var pmsm_hvp_kv31f = {
    title   : "PMSM Sensorless Control using HVP-KV31F120M",
}

/*************           KV46          ****************/
var pmsm_twr_kv46f = {
    title   : "PMSM Sensorless Control using TWR-KV46F150",
}

var pmsm_hvp_kv46f = {
    title   : "PMSM Sensorless Control using HVP-KV46F150",
}

/*************           KV58          ****************/
var pmsm_twr_kv58f = {
    title   : "PMSM Sensorless Control using TWR-KV58F220",
}

var pmsm_hvp_kv58f = {
    title   : "PMSM Sensorless Control using HVP-KV58F220",
}

var pmsm_evk_imxrt1050 = {
    title   : "PMSM Sensorless Control using MIMXRT1050-EVK",
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

    /* put together motor type and board */
    appId = motorType + '_' + boardID;

    switch(appId){
    case "on Kinetis KV pmsm_frdm-ke15z":
        document.getElementById("title").innerHTML = pmsm_frdm_ke15z.title;
        break;
    case "on Kinetis KV pmsm_frdm-kv10z":
        document.getElementById("title").innerHTML = pmsm_frdm_kv10z.title;
        break;
    case "on Kinetis KV pmsm_twr-kv10z":
        document.getElementById("title").innerHTML = pmsm_twr_kv10z.title;
        break;
    case "on Kinetis KV pmsm_hvp-kv10z":
        document.getElementById("title").innerHTML = pmsm_hvp_kv10z.title;
        break;
    case "on Kinetis KV pmsm_twr-kv11z":
        document.getElementById("title").innerHTML = pmsm_twr_kv11z.title;
        break;
    case "on Kinetis KV pmsm_frdm-kv31f":
        document.getElementById("title").innerHTML = pmsm_frdm_kv31f.title;
        break;
    case "on Kinetis KV pmsm_twr-kv31f":
        document.getElementById("title").innerHTML = pmsm_twr_kv31f.title;
        break;
    case "on Kinetis KV pmsm_hvp-kv31f":
        document.getElementById("title").innerHTML = pmsm_hvp_kv31f.title;
        break;
    case "on Kinetis KV pmsm_twr-kv46f":
        document.getElementById("title").innerHTML = pmsm_twr_kv46f.title;
        break;
    case "on Kinetis KV pmsm_hvp-kv46f":
        document.getElementById("title").innerHTML = pmsm_hvp_kv46f.title;
        break;
    case "on Kinetis KV pmsm_twr-kv58f":
        document.getElementById("title").innerHTML = pmsm_twr_kv58f.title;
        break;
    case "on Kinetis KV pmsm_hvp-kv58f":
        document.getElementById("title").innerHTML = pmsm_hvp_kv58f.title;
        break;
    case "on Kinetis KV pmsm_twr-ke18f":
        document.getElementById("title").innerHTML = pmsm_twr_ke18f.title;
        break;
    case "on Kinetis KV pmsm_hvp-ke18f":
        document.getElementById("title").innerHTML = pmsm_hvp_ke18f.title;
        break;
    
    default:
        document.getElementById("title").innerHTML = "PM Synchronous Motor Control Demo ";
        break;
    }

    /* update gauge texts accroding to MCU */
    var titleSpan = document.getElementById("title");
    titleSpan.style.fontSize = "25px";	titleSpan.style.fontWeight = 'bold';

    document.getElementById("heel").innerHTML = heel;
    var heelSpan = document.getElementById("heel");
    heelSpan.style.fontSize = "15px";

    document.getElementById("DCVoltage").innerHTML = voltageSpan;
    var voltSpan = document.getElementById("DCVoltage");
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
