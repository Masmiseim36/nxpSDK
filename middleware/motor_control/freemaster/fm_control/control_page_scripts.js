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
* @file   control_page.js
*
* @brief  Script file containing  functions for gauges control
*
******************************************************************************/
/******************************************************************************
| global variable
|----------------------------------------------------------------------------*/
/* miscelleneous variables declaration and initialization */
var animation_dur   = 50;
var label_dist      = '37%';
var label_interval  = 5;
var vValue0, tValue0, retMsg;
var commError;

/******************************************************************************

| local function implementation
|----------------------------------------------------------------------------*/
/******************************************************************************
@brief   Function called during page loaded - currently disabled due to multiple
         generation of Non-connected board

@param   void

@return  none
******************************************************************************/
function initDemoPage(){

/* check enabled FM communication */
    if((!pcm.IsCommPortOpen())&&(commError!=true))
    {
        //alert("Communication is stopped. Press Ctrl+K to start the communication");
        commError = true;
    }
}

/******************************************************************************
@brief   Function called after page loaded

@param   void

@return  none
******************************************************************************/
$(document).ready(function(){

    /* Read identification variables and parse them accordingly */
    AppIdenInit();

    /* variable and gauge scales initialization */
    Init();

    /* info menu initialization */
    //InfoMenuInit();

    /* speed gauge initialization */
    SpeedGaugeInit();

    /* speed slider initialization */
    SpeedSliderInit();

    /* demo button initialization */
    DemoButtonInit();

    /* application switch button initialization */
    AppSwitchInit();

    /* application notification panel initialization */
    AppNotificationInit();

    /* dc-bus voltage gauge initialization */
    DCBusVoltGaugeInit();

    /* motor current gauge initialization */
    MotorCurrentGaugeInit();

    /* motor current limitation initialization */
    MotorCurrentLimitInit();
});

/******************************************************************************
@brief   Read FM variables and set gauge scales and page texts

@param   void

@return  none
******************************************************************************/
function Init()
{
    var currentLimHigh  = 0;
    var currentLimLow   = 0;
    /* Gauge, Buttons control - subscribed FreeMASTER variables for onChange detection */
    /* These variables are global to be used within entire code */
    appSwitchID     = pcm.SubscribeVariable("Application Switch", 1000);
    appStateID      = pcm.SubscribeVariable("Application State", 1000);
    actSpeedID      = pcm.SubscribeVariable("Speed Actual", 100);
    busVoltID       = pcm.SubscribeVariable("DCB Voltage Filtered", 450);
    motorCurrID     = pcm.SubscribeVariable("Iq", 100);
    faultID         = pcm.SubscribeVariable("Fault Pending", 500);
    demoID          = pcm.SubscribeVariable("Demo Mode",800);
    reqSpeedID      = pcm.SubscribeVariable("Speed Required",100);
    speedLimHighID  = pcm.SubscribeVariable("Speed Loop Limit High",1500);
    speedLimLowID   = pcm.SubscribeVariable("Speed Loop Limit Low",1500);

    /* read speed scales */
	succ = pcm.ReadVariable("FMSTR_M1_speedScale", vValue0, tValue0, retMsg)
	if (succ)
        speedScale = (pcm.LastVariable_vValue - (pcm.LastVariable_vValue%500) + 500)/100;
    succ = pcm.ReadVariable("Fault Threshold SpeedNom", vValue0, tValue0, retMsg)
	if (succ)
    	speedNominal = pcm.LastVariable_vValue/100;
    succ = pcm.ReadVariable("Fault Threshold SpeedMin", vValue0, tValue0, retMsg)
	if (succ)
    	speedMinimum = pcm.LastVariable_vValue/100;

    /* speed gauge major interval calculation */
    if(speedScale<=50)
        speedInterMaj = 5;
    if((speedScale>50)&&(speedScale<=100))
        speedInterMaj = 10;
    if(speedScale>100)
        speedInterMaj = 20;

    /* read dc-bus voltage scale and fault limits */
    succ = pcm.ReadVariable("FMSTR_M1_DCBvoltageScale", vValue0, tValue0, retMsg)
	if (succ)
    	dcbVoltScale = pcm.LastVariable_vValue;
    succ = pcm.ReadVariable("Fault Threshold DcBusOver", vValue0, tValue0, retMsg)
    if (succ)
    	dcbVoltOver = pcm.LastVariable_vValue;
    succ = pcm.ReadVariable("Fault Threshold DcBusUnder", vValue0, tValue0, retMsg)
	if (succ)
    	dcbVoltUnder = pcm.LastVariable_vValue;

    /* voltate gauge major interval calculation */
    if(dcbVoltScale<=50)
        dcbVoltInterMaj = 5;
    if((dcbVoltScale>50)&&(dcbVoltScale<=100))
        dcbVoltInterMaj = 10;
    if(dcbVoltScale>100)
        dcbVoltInterMaj = 50;

    /* read actual application state */
	succ = pcm.ReadVariable("Application State", vValue0, tValue0, retMsg)
	if (succ)
    	appState = pcm.LastVariable_vValue;

    /* read demo mode state */
    succ = pcm.ReadVariable("Demo Mode", vValue0, tValue0, retMsg)
	if (succ)
    	demoMode = pcm.LastVariable_vValue;

    /* motor current scale get from speed controller limits */
    succ = pcm.ReadVariable("Speed Loop Limit High", vValue0, tValue0, retMsg);
    if (succ)
       currentLimHigh = Math.abs(pcm.LastVariable_vValue);
    succ = pcm.ReadVariable("Speed Loop Limit Low", vValue0, tValue0, retMsg);
    if (succ)
       currentLimLow = Math.abs(pcm.LastVariable_vValue);
    currentScale = (currentLimHigh>=currentLimLow) ? currentLimHigh:currentLimLow;

    /* voltate gauge major interval calculation */
    if(currentScale<=5)
        currentInterMaj = 1;
    if((currentScale>5)&&(currentScale<=10))
        currentInterMaj = 2;
    if(currentScale>10)
        currentInterMaj = 5;

}

/******************************************************************************
@brief   Info menu initialization

@param   void

@return  none
******************************************************************************/
function InfoMenuInit()
{
    var InfoMenuSource =
    [{
        label: "Info",
        items:[{
            label:"MCU",
            subMenuWidth:'625px',
            items:
            [{
                html: '<div align="center"><a href = "http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=KV3x" target="_blank"> <img src="KV3X_BD.jpg" /></a><br><a href ="http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=KV3x&tab=Documentation_Tab&pspll=1&SelectedAsset=Documentation&ProdMetaId=PID/DC/KV3x&fromPSP=true&assetLockedForNavigation=true&componentId=2&leftNavCode=1&pageSize=25&Documentation=Documentation/00610Ksd1nd%60%60Data%20Sheets&fpsp=1&linkline=Data%20Sheets" target="_blank"; style = "text-decoration:underline; text-align:center"  ><span style ="text-align:center" >Datasheets & Reference Manuals</span></a></div>',
            }]
        },
        {
            label:"Freedom Board",
            subMenuWidth:'425px',
            items:
            [{ html:'<div align="center"><a href = "http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KV31F" target="_blank"><img src="FRDM-KV31F.jpg" /></a> <p> Arduino R3 Pin Compatible</p><p>OpenSDA</p></div>'}]
        },
        {
            label:"BLDC & Power Stage",
            subMenuWidth:'455px',
            items:
            [{
                html:'<a href = "http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-MC-LVBLDC" target="_blank"> <img src="FRDM-MC-LVBLDC.jpg" /></a>\
                <p align = "center">Motor: LINIX -> 24V DC -> 40W -> 4000 r/min</p>\
                <p align = "left"><b>FRDM-MC-LVBLDC Powerstage Features:</b></p>\
                \
                <p>•Power Supply Input voltage DC: 10-15VDC</p>\
                <p>•Output current up to 5 amps RMS</p>\
                <p>•Power supply reverse polarity protection circuitry</p>\
                <p>•3-phase bridge inverter (6-MOSFET’s)</p>\
                <p>•3-phase MOSFET gate driver with over current and under voltage protection</p>\
                <p>•Analog sensing (DC bus voltage, DC bus current, 3-phase back-EMF voltage)</p>\
                <p>•5.5 VDC auxiliary power supply providing FRDM MCU board supplying</p>\
                <p>•Motor speed/position sensors interface (Encoder, Hall)</p>\
                <p>•Freedom motor control headers compatible with Arduino ™ R3 pin layout</p>\
                \
                ',
            }]
        },
        {
            label:"App Block Diagram",
            subMenuWidth:'400px',
            items:
            [{
                html:'<p>• • • TBD • • •</p>',
            }]
        },
        ]

    }];

    $("#jqxInfoMenu").jqxMenu(
    {
        source:InfoMenuSource,
        width: 70,
        height: 32,
        popupZIndex:99999,
        mode: "vertical",
        minimizeWidth:"null",
        theme: 'energyblue'
    });
}

/******************************************************************************
@brief   Speed Gauge, scaled, and divided by 100 due to x100 rpm caption
         Two speed gauges are used, it is due to possibility to have two
          pointers. For actual and for required speed. Gauage for required
          speed is filled with none color.

@param   void

@return  none
******************************************************************************/
function SpeedGaugeInit()
{
    /* required speed gauge */
    $('#gaugeContainerSpeedRequired').jqxGauge({
        ranges: [{ startValue: -speedScale, endValue: -speedNominal, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 },
        { startValue: -speedNominal, endValue: -speedMinimum, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 },
        { startValue: -speedMinimum, endValue: speedMinimum, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 },
        { startValue: speedMinimum, endValue: speedNominal, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 },
        { startValue: speedNominal, endValue: speedScale, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 }],
        ticksMinor: { interval: speedInterMaj/2, size: '5%',visible:false },
        ticksMajor: { interval: speedInterMaj, size: '9%',visible:false },
        caption: {value: 'x100 rpm', position: 'bottom', offset: [0, 40], visible: false},
        max: speedScale,
        min: -speedScale,
        labels: { distance: label_dist,  interval: label_interval,visible:false},
        startAngle: 67,
        endAngle: 382,
        border: {  style:{fill:'none', stroke:'none'}, showGradient: false },
        style:{  fill:'none', stroke:'none'},
        value: 0,
        width: 300,
        height: 300,
        cap: {size: '4%', style: {fill: 'black', stroke: 'black'},
        visible:true},
        pointer:{style: {fill: '#e02629', stroke: '#e02629'},
        length:'50%',width:'3%'},
        animationDuration: animation_dur
    });

    /* actual speed gauge */
    $('#gaugeContainerSpeedActual').jqxGauge({
        ranges: [{ startValue: -speedScale, endValue: -speedNominal, style: { fill: '#e02629', stroke: '#e02629' }, endWidth: 6, startWidth: 6 },
        { startValue: -speedNominal, endValue: -speedMinimum, style: { fill: '#0e29f4', stroke: '#0e29f4' }, endWidth: 6, startWidth: 6 },
        { startValue: -speedMinimum, endValue: speedMinimum, style: { fill: '#e02629', stroke: '#e02629' }, endWidth: 6, startWidth: 6 },
        { startValue: speedMinimum, endValue: speedNominal, style: { fill: '#0e29f4', stroke: '#0e29f4' }, endWidth: 6, startWidth: 6 },
        { startValue: speedNominal, endValue: speedScale, style: { fill: '#e02629', stroke: '#e02629' }, endWidth: 6, startWidth: 6 }],
        ticksMinor: { interval: speedInterMaj/2, size: '5%' },
        ticksMajor: { interval: speedInterMaj, size: '9%' },
        caption: {value: 'x100 rpm', position: 'bottom', offset: [0, 40], visible: true},
        max: speedScale,
        min: -speedScale,
        labels: { distance: label_dist,  interval: label_interval},
        startAngle: 67,
        endAngle: 382,
        border: {  size: '5%'},
        value: 0,
        cap: {style: {fill: '#4bb648', stroke: '#4bb648'}},
        pointer:{style: {fill: 'black',stroke: 'black'}},
        width: 300,
        height: 300,
        animationDuration: animation_dur
    });

    /* update init values */
    RequiredSpeedUpdate();
    ActualSpeedUpdate();
};

/******************************************************************************
@brief   Speed slider setting required speed

@param   void

@return  none
******************************************************************************/
function SpeedSliderInit()
{
    $('#jqxSliderSetSpeed').jqxSlider(
    {
        tooltip: true,
        theme:'energyblue',
        mode: 'default',
        showButtons: true,
        height: 30,
        width: 300,
        min: -speedNominal*100, max: speedNominal*100,
        step: 500,
        ticksFrequency: 500,
        values: [-speedNominal*100, speedNominal*100]
    });

    /* set init value */
    RequiredSpeedUpdate();
};

/******************************************************************************
@brief   Demo button initialization

@param   void

@return  none
******************************************************************************/
function DemoButtonInit()
{
    $("#jqxDemoButton").jqxButton(
    {
        width: 200,
        height: 40,
        disabled: false,
        template: "default",
    });

    /* set init value */
    DemoModeUpdate();
};

/******************************************************************************
@brief   Application switch button initialization

@param   void

@return  none
******************************************************************************/
function AppSwitchInit()
{
    $("#jqxAppSwitchButton").jqxButton(
    {
        width: 200,
        height: 40,
        disabled: false,
        template: "success",
    });

    /* set init value */
    ApplicationSwitchUpdate();
};

/******************************************************************************
@brief   Application state notification initialization

@param   void

@return  none
******************************************************************************/
function AppNotificationInit()
{
    $("#jqxAppState").jqxNotification(
    {
        width: 280,
        height: 50,
        position: "top-centre",
        opacity: 1,
        autoOpen:  false,
        autoClose: false,
        showCloseButton: false,
        template: "info"
    });

    /* set init value */
    ApplicationStateUpdate();
};

/******************************************************************************
@brief   DC-Bus voltage gauge initialization

@param   void

@return  none
******************************************************************************/
function DCBusVoltGaugeInit()
{
    $('#gaugeLinearDCBusVoltage').jqxLinearGauge(
    {
        orientation: 'horizontal',
        width: 280,
        height: 50,
        ticksMajor: { size: '18%', interval: dcbVoltInterMaj },
        ticksMinor: { size: '9%', interval: dcbVoltInterMaj, style: { 'stroke-width': 1, stroke: '#aaaaaa'} },
        max: dcbVoltScale,
        min: 0,
        pointer: { size: '20%' },
        colorScheme: 'scheme05',
        labels: { interval: dcbVoltInterMaj, position: 'near', offset: 4 },
        value: 0,
        ranges: [
            { startValue: 0, endValue: dcbVoltUnder, style: { fill: '#FF4800', stroke: '#FF4800'} },
            { startValue: dcbVoltUnder, endValue: dcbVoltOver, style: { fill: '#4bb648', stroke: '#4bb648'} },
            { startValue: dcbVoltOver, endValue: dcbVoltScale, style: { fill: '#FF4800', stroke: '#FF4800'}}],
        animationDuration: animation_dur
    });

    /* update init values */
    DCBusVoltageUpdate();
};

/******************************************************************************
@brief   Motor current gauge initialization

@param   void

@return  none
******************************************************************************/
function MotorCurrentGaugeInit()
{
    var currentScaleMax = Math.ceil(currentScale)+0.5;

    $('#gaugeLinearCurrent').jqxLinearGauge(
    {
        orientation: 'horizontal',
        width: 280,
        height: 50,
        ticksMajor: { size: '18%', interval: currentInterMaj },
        ticksMinor: { size: '9%', interval: currentInterMaj/2, style: { 'stroke-width': 1, stroke: '#aaaaaa'} },
        max: currentScaleMax,
        min: 0,
        pointer: { size: '20%' },
        colorScheme: 'scheme05',
        labels: { interval: currentInterMaj, position: 'near',offset: 4 },
        value: 0,
        ranges: [
            { startValue: 0, endValue: currentScale, style: { fill: '#4bb648', stroke: '#4bb648'} },
            { startValue: currentScale, endValue: currentScaleMax, style: { fill: '#FF4800', stroke: '#FF4800'}}],
        animationDuration: animation_dur
    });

    /* set init value */
    MotorCurrentUpdate();
};

/******************************************************************************
@brief   Motor current limiter initialization

@param   void

@return  none
******************************************************************************/
function MotorCurrentLimitInit()
{
    $('#jqxSliderSetCurrentLimit').jqxSlider(
    {
        mode: 'default',
        orientation: 'horizontal',
        tooltip: true,
        theme:'energyblue',
        showButtons: true,
        height: 30,
        width: 280,
        min: 0, max: currentScale,
        step: 0.2,
        ticksFrequency: 0.5,
        values: [0, currentScale],
        value: 3
    });

    /* set init value */
    MotorCurrentUpdate();
};

/******************************************************************************
@brief   Speed slider on change event

@param   void

@return  none
******************************************************************************/
$(document).ready(function(){
    $('#jqxSliderSetSpeed').on('change', function (event)
    {
        if(demoMode == 0)
        {
            speedReqNew= $('#jqxSliderSetSpeed').jqxSlider('getValue');
            succ = pcm.WriteVariable("Speed Required", speedReqNew, retMsg);
            $('#gaugeContainerSpeedRequired').jqxGauge({value: speedReqNew/100, });
            if(speedReqNew != 0)
            {
                succ = pcm.WriteVariable("Application Switch", 1, retMsg);
            }
        }
    });
});

/******************************************************************************
@brief   Current limitation slider on change event

@param   void

@return  none
******************************************************************************/
$(document).ready(function(){
    $('#jqxSliderSetCurrentLimit').on('change', function (event)
    {
        var currentLimNew= $('#jqxSliderSetCurrentLimit').jqxSlider('getValue');
        /* set speed controller limits */
        succ = pcm.WriteVariable("Speed Loop Limit High", currentLimNew, retMsg);
        succ = pcm.WriteVariable("Speed Loop Limit Low", -currentLimNew, retMsg);
    });
});

/******************************************************************************
@brief   Demo button on change event

@param   void

@return  none
******************************************************************************/
$(document).ready(function(){
$("#jqxDemoButton").on('click',function()
    {
        succ = pcm.ReadVariable("Demo Mode", vValue0, tValue0, retMsg);
        if(pcm.LastVariable_vValue==1)
        {
            succ = pcm.WriteVariable("Demo Mode",0,retMsg);
        }
        else
        {
            succ = pcm.WriteVariable("Demo Mode",1,retMsg);
        }
    });
});


/******************************************************************************
@brief   Application switch button on change event

@param   void

@return  none
******************************************************************************/
$(document).ready(function(){
    $("#jqxAppSwitchButton").on('click', function ()
    {
        var appState  = 0;
        var appSwitch = 0;

        /* read State variable from FreeMASTER */
        succ = pcm.ReadVariable("Application State", vValue0, tValue0, retMsg)
        if(succ)
            appState = pcm.LastVariable_vValue;

        /* for fault state clear fault */
        if(appState==0)
        {
            succ = pcm.WriteVariable("Fault Clear", 1, retMsg);
        }
        else
        {
            succ = pcm.ReadVariable("Application Switch", vValue0, tValue0, retMsg)
            if(succ)
                appSwitch = pcm.LastVariable_vValue;

            if(appSwitch==1)
            {
                succ = pcm.WriteVariable("Application Switch", 0, retMsg);
                succ = pcm.WriteVariable("Demo Mode",0,retMsg);
            }
            else
            {
                succ = pcm.WriteVariable("Application Switch", 1, retMsg);
            }
        }
    });
});

/******************************************************************************
@brief   Application state boox

@param   void

@return  none
******************************************************************************/
$(document).ready(function(){
$("#jqxAppState").on('click',function()
    {
        /* refresh application state box, otherwise is disappeares */
        ApplicationStateUpdate();
    });
});

/******************************************************************************
@brief   FreeMASTER on variable change event

@param   void

@return  none
******************************************************************************/
function Event_OnVariableChanged(a,IDsubscribedVariable)
{
    switch(IDsubscribedVariable){
    case appSwitchID:
        // sensor type changed in FreeMASTER
        ApplicationSwitchUpdate();
        break;
    case appStateID:
        // application state update
        ApplicationStateUpdate();
        break;
    case actSpeedID:
        // Actual Speed
        ActualSpeedUpdate();
        break;
    case busVoltID:
        // Speed feedback sensing
        DCBusVoltageUpdate();
        break;
    case motorCurrID:
        // Actual Iq current
        MotorCurrentUpdate();
        break;
    case faultID:
        // Application state update
        SetFaultSource();
        break;
    case demoID:
        // Demo mode
        DemoModeUpdate();
        break;
    case reqSpeedID:
        // Demo mode speed
        RequiredSpeedUpdate();
        break;
    case speedLimHighID:
    case speedLimLowID:
        // Current limitation of speed PI controller
        CurrentLimitUpdate();
        break;
    default:
    }
}

/******************************************************************************
@brief   Application switch value update

@param   void

@return  none
******************************************************************************/
function ApplicationSwitchUpdate()
{
    var appState    = 0;
    var appSwitch   = 0;

    /* read State variable from FreeMASTER */
    succ = pcm.ReadVariable("Application State", vValue0, tValue0, retMsg)
    if(succ)
    {
        appState = pcm.LastVariable_vValue;

        /* for fault state clear fault */
        if(appState==0)
        {
            $("#jqxAppSwitchButton").jqxButton('val', "CLEAR FAULT");
            $("#jqxAppSwitchButton").jqxButton({template: 'warning'});
        }
        else
        {
            succ = pcm.ReadVariable("Application Switch", vValue0, tValue0, retMsg)
            if (succ)
                appSwitch = pcm.LastVariable_vValue;

            if (appSwitch == 0)
            {
                $('#jqxSliderSetSpeed').jqxSlider({value: 0});
                $('#gaugeContainerSpeedActual').jqxGauge({value: 0, });
                $("#jqxAppSwitchButton").jqxButton('val', "RUN");
                $("#jqxAppSwitchButton").jqxButton({template: 'success'});
                $("#jqxDemoButton").jqxButton({disabled:true});
            }
            else
            {
                 $("#jqxAppSwitchButton").jqxButton('val', "STOP");
                 $("#jqxAppSwitchButton").jqxButton({template: 'danger'});
                 $("#jqxDemoButton").jqxButton({disabled:false});
            }
        }
    }
    else
    {
        $("#jqxAppSwitchButton").jqxButton('val', "COMMUNICATION ERROR");
        $("#jqxAppSwitchButton").jqxButton({template: 'danger'});
    }
}

/******************************************************************************
@brief   Application state update on notice panel

@param   void

@return  none
******************************************************************************/
function ApplicationStateUpdate()
{
    var appState = 0;
    /* read State variable from FreeMASTER */
    succ = pcm.ReadVariable("Application State", vValue0, tValue0, retMsg)
    if(succ)
        appState = pcm.LastVariable_vValue;
    else
        /* error state */
        appState = 99;

    /* close all previous notifications */
    $("#jqxAppState").jqxNotification("closeAll");
    /* select particular Application State */
    switch(appState){
    case 0: //FAULT
        $("#jqxAppStateText").text("Application State: FAULT");
        $("#jqxAppState").jqxNotification("open");
        break;
    case 1: //INIT
        $("#jqxAppStateText").text("Application State: INIT");
        $("#jqxAppState").jqxNotification("open");
        break;
    case 2: //STOP
        $("#jqxAppStateText").text("Application State: STOP");
        $("#jqxAppState").jqxNotification("open");
        break;
    case 3: //RUN
        succ = pcm.ReadVariable("Demo Mode", vValue0, tValue0, retMsg);
        if (succ)
        	demoMode = pcm.LastVariable_vValue;
        /* if demo mode is active, update App state notification */
        if (demoMode)
        {
            $("#jqxAppStateText").text("Application State: DEMO RUN");
            $("#jqxAppState").jqxNotification("open");
        }
        else
        {
            $("#jqxAppStateText").text("Application State: RUN");
            $("#jqxAppState").jqxNotification("open");
        }
        break;
    case 99:
    default:
        $("#jqxAppStateText").text("FAULT: UNDEFINED STATE");
        $("#jqxAppState").jqxNotification("open");

    }

    /* update Applicationa switch button accroding to actual state  */
    ApplicationSwitchUpdate();
}

/******************************************************************************
@brief   Required speed value update

@param   void

@return  none
******************************************************************************/
function RequiredSpeedUpdate()
{
    var speedRequiredAct = 0;
    succ = pcm.ReadVariable("Speed Required", vValue0, tValue0, retMsg);
    if (succ)
       speedRequiredAct = pcm.LastVariable_vValue;

    $('#gaugeContainerSpeedRequired').jqxGauge({value: speedRequiredAct/100, });
    $('#jqxSliderSetSpeed').jqxSlider({value: speedRequiredAct});
}

/******************************************************************************
@brief   Actual speed value update

@param   void

@return  none
******************************************************************************/
function ActualSpeedUpdate()
{
    var speedAct = 0;
    succ = pcm.ReadVariable("Speed Actual", vValue0, tValue0, retMsg)
    if (succ)
        speedAct = pcm.LastVariable_vValue;
    $('#gaugeContainerSpeedActual').jqxGauge({value: speedAct/100, });
}

/******************************************************************************
@brief   DC-Bus voltage value update

@param   void

@return  none
******************************************************************************/
function DCBusVoltageUpdate()
{
    var dcbVoltAct = 0;
    succ = pcm.ReadVariable("DCB Voltage Filtered", vValue0, tValue0, retMsg)
    if (succ)
        dcbVoltAct = pcm.LastVariable_vValue;

    $('#gaugeLinearDCBusVoltage').jqxLinearGauge({value: dcbVoltAct,  });
}

/******************************************************************************
@brief   Demo mode update

@param   void

@return  none
******************************************************************************/
function DemoModeUpdate()
{
    var demoMode = 0;

    succ = pcm.ReadVariable("Demo Mode", vValue0, tValue0, retMsg);

    if (succ)
    	demoMode = pcm.LastVariable_vValue;

    if (demoMode)
        $("#jqxDemoButton").jqxButton({disabled:true});
    else
        succ = pcm.WriteVariable("Speed Required",0, retMsg);

    /* call application state update function */
    ApplicationStateUpdate();
}

/******************************************************************************
@brief   Motor current limit value update

@param   void

@return  none
******************************************************************************/
function CurrentLimitUpdate()
{
    var currentScaleMax = Math.ceil(currentScale)+0.5;
    var currentLimHigh  = 0;
    var currentLimLow   = 0;
    var currentScaleNew = 0;

    succ = pcm.ReadVariable("Speed Loop Limit High", vValue0, tValue0, retMsg);
    if (succ)
       currentLimHigh = Math.abs(pcm.LastVariable_vValue);

    succ = pcm.ReadVariable("Speed Loop Limit Low", vValue0, tValue0, retMsg);
    if (succ)
       currentLimLow = Math.abs(pcm.LastVariable_vValue);

    /* update current limitation gauge */
    if(currentLimHigh > currentLimLow)
        currentScaleNew = currentLimHigh;
    else
        currentScaleNew = currentLimLow;

    /* update ranges - green and red areas */
    var ranges = [{
            startValue: currentScaleNew,
            endValue: currentScaleMax,
            style: { fill: '#FF4800', stroke: '#FF4800'}
        },
        {   startValue: 0,
            endValue: currentScaleNew,
            style: { fill: '#4bb648', stroke: '#4bb648'}
        }]

    $('#gaugeLinearCurrent').jqxLinearGauge({ ranges: ranges });
    $('#jqxSliderSetCurrentLimit').jqxSlider({value: currentScaleNew});

}

/******************************************************************************
@brief   Actual motor current value update

@param   void

@return  none
******************************************************************************/
function MotorCurrentUpdate()
{
    var motorCurrent = 0;

    succ = pcm.ReadVariable("Iq", vValue0, tValue0, retMsg);
    if (succ)
       motorCurrent = Math.abs(pcm.LastVariable_vValue);

    $('#gaugeLinearCurrent').jqxLinearGauge({value: motorCurrent,  });
}

/******************************************************************************
@brief   Update fault source on state notice board

@param   void

@return  none
******************************************************************************/
function SetFaultSource()
{
    var faultState = 0;

    /* read Fault varialbe from FreeMASTER */
    succ = pcm.ReadVariable("Fault Pending", vValue0, tValue0, retMsg);
    if(succ)
        faultState = pcm.LastVariable_vValue;

    /* select particular Fault State Message*/
    switch(faultState){
    case (faultState&0x1): //Over Current
        $("#jqxAppStateText").text("FAULT: Over Current");
        $("#jqxAppState").jqxNotification("open");
        break;
    case (faultState&0x2): //Under Voltage
        $("#jqxAppStateText").text("FAULT: Under Voltage");
        $("#jqxAppState").jqxNotification("open");
        break;
    case (faultState&0x4): //Over Voltage
        $("#jqxAppStateText").text("FAULT: Over Voltage");
        $("#jqxAppState").jqxNotification("open");
        break;
    case (faultState&0x8): //Overload
        $("#jqxAppStateText").text("FAULT: Overload");
        $("#jqxAppState").jqxNotification("open");
        break;
    case (faultState&0x10): //Overspeed
        $("#jqxAppStateText").text("FAULT: Overspeed");
        $("#jqxAppState").jqxNotification("open");
        break;
    case (faultState&0x20): //Blocked Rotor
        $("#jqxAppStateText").text("FAULT: Blocked Rotor");
        $("#jqxAppState").jqxNotification("open");
        break;
    default:
        break;
    }
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
