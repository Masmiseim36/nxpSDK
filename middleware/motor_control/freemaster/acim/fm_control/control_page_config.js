/*******************************************************************************
*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
var heel            = "NXP Semiconductors, 2016. Designed by Motor Control Team / Microcontroller Product Group";
var voltageSpan     = "DC-bus Voltage [V]";
var currentLimSpan  = "Current Limitation";
var sliderSpeedSpan = "Speed Required";
var currentScale    = 8;
var currentInterMaj = 1;
var speedScale      = 3500/100;
var speedNominal    = 3000/100;
var speedMinimum    = 300/100;
var speedInterMaj   = 5;
var dcbVoltScale    = 36.3;
var dcbVoltInterMaj = 5;
var dcbVoltOver     = 30;
var dcbVoltUnder    = 10;
var appState        = 0;
var demoMode        = 0;

/* stuctures of texts selected based on connected MUC */
/************* ACIM Sensorless Control ****************/
/*************           KV31          ****************/
var acim_hvp_kv31f = {
    title   : "ACIM Sensorless Control using HVP-KV31F120M"
};

/*************           KV46          ****************/
var acim_hvp_kv46f = {
    title   : "ACIM Sensorless Control using HVP-KV46F150M"
};


/*************           KV58          ****************/
var acim_hvp_kv58f = {
    title   : "ACIM Sensorless Control using HVP-KV58F220M"
};





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
    case "acim_hvp-kv31f":
        document.getElementById("title").innerHTML = acim_hvp_kv31f.title;
        break;
    case "acim_hvp-kv46f":
        document.getElementById("title").innerHTML = acim_hvp_kv46f.title;
        break;
    case "acim_hvp-kv58f":
        document.getElementById("title").innerHTML = acim_hvp_kv58f.title;
        break;
    default:
        document.getElementById("title").innerHTML = "Motor Control on Kinetis KV";
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
