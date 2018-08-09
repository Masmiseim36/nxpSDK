/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* Global variables*/
/* If you want to change text of headers or scales of gauges do it here*/
/* If you want to change the placement of elements do it in .html document */
    var title = "PMSM Sensorless Control on Kinetis KV";
	var heel = "Freescale Semiconductor, Inc. 2015. Designed by Motor Control Team / Microcontroller Product Group";
	var voltageSpan = "DC Bus Voltage";
	var currentSpan = "DC Bus Current";
	var currentLimSpan = "Current Limitation";
	var sliderSpeed = "Speed Required";
	var current_scale = 8.25 , speed_scale = 3000 , dcbusvoltage_scale =15;
	var reset_switch, AppState, speed, dcbvoltage, dcbcurrent, fault,demo=0,demospeed;
	var vValue0, tValue0, retMsg;
	var demovar=0;
	var set_speed,set_current_limit;
    var gauge_width = '30%' <!-- 400-->
    var current_gauge_width = '20%' <!-- 150-->
	var animation_dur = 50;
    var max_speed = 50;
	var label_dist = '37%';
	var label_interval = 5;//500 
	var togg=0;
	var current_limit_default = 3;
        
		/* Headers style properties*/
		$(document).ready(function(){
			 document.getElementById("title").innerHTML = title;
			 var titleSpan = document.getElementById("title");
	         titleSpan.style.fontSize = "30px";	titleSpan.style.fontWeight = 'bold';
			 			
			 document.getElementById("heel").innerHTML = heel;
			 var heelSpan = document.getElementById("heel");
	         heelSpan.style.fontSize = "18px";
			 heelSpan.style.fontWeight = 'bold';
			 
			 document.getElementById("DCVoltage").innerHTML = voltageSpan;
			 var voltSpan = document.getElementById("DCVoltage");
	         voltSpan.style.fontSize = "18px";voltSpan.style.fontWeight = 'bold';
			 
			 document.getElementById("Current").innerHTML = currentSpan;
			 var currSpan = document.getElementById("Current");
	         currSpan.style.fontSize = "18px";currSpan.style.fontWeight = 'bold';
			 
			 document.getElementById("CurrentLimit").innerHTML = currentLimSpan;
			 var currLimSpan = document.getElementById("CurrentLimit");
	         currLimSpan.style.fontSize = "18px";currLimSpan.style.fontWeight = 'bold';
			 
			 document.getElementById("SpeedSlider").innerHTML = sliderSpeed;
			 var speedSpan = document.getElementById("SpeedSlider");
	         speedSpan.style.fontSize = "18px";speedSpan.style.fontWeight = 'bold';
			 
		});
		
		// <!-- Widgets placement, properties and attributes-->
		// <!-- Speed Gauge, scaled, and divided by 100 due to x100 rpm caption -->
			// Two speed gauges are used, it is due to possibility to have two pointers. For actual and for required speed.
            // Gauage for required speed is filled with none color. Tricky!!!			
		$(document).ready(function () {
			// <!-- var newSize = $(document).width()*0.3; -->
			max_speed = speed_scale/100; // divided by 100 due to speed gauge caption
            label_interval = max_speed*0.1;			
			//required speed gauge
			$('#gaugeContainerSpeedRequired').jqxGauge({
				ranges: [{ startValue: -max_speed, endValue: -max_speed*0.8, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 },
						{ startValue: -max_speed*0.8, endValue: -max_speed*0.1, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 },
						{ startValue: -max_speed*0.1, endValue: max_speed*0.1, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 },
						{ startValue: max_speed*0.1, endValue: max_speed*0.8, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 },
                        { startValue: max_speed*0.8, endValue: max_speed, style: { fill: 'none', stroke: 'none' }, endWidth: 6, startWidth: 6 }],
                ticksMinor: { interval: max_speed*0.1, size: '5%',visible:false },
                ticksMajor: { interval: max_speed*0.2, size: '9%',visible:false },
				caption: {
                         value: 'x100 rpm',
                         position: 'bottom',
                         offset: [0, 40],
                         visible: false
     },
				max: max_speed,
				min: -max_speed,
			    labels: { distance: label_dist,  interval: label_interval,visible:false},
				startAngle: 67,
				endAngle: 382,
				border: {  style:{fill:'none', stroke:'none'}, showGradient: false },
			    style:{  fill:'none', stroke:'none'},
				value: 0,
                width: 360, //  <!-- '30%' -->
                height: 360,  //<!-- '60%' -->
                cap: {
					size: '4%',
                      style: {
						      
                              fill: 'black',
                              stroke: 'black'
					         },
							 visible:true
                     },
                pointer:{style: {
                                 fill: '#e02629',
                                 stroke: '#e02629'
                                },
							   length:'50%',width:'3%'},
				animationDuration: animation_dur
            });
			
			
            $('#gaugeContainerSpeedM1').jqxGauge({
                ranges: [{ startValue: -max_speed, endValue: -max_speed*0.8, style: { fill: '#e02629', stroke: '#e02629' }, endWidth: 6, startWidth: 6 },
						{ startValue: -max_speed*0.8, endValue: -max_speed*0.05, style: { fill: '#4bb648', stroke: '#4bb648' }, endWidth: 6, startWidth: 6 },
						{ startValue: -max_speed*0.05, endValue: max_speed*0.05, style: { fill: '#e02629', stroke: '#e02629' }, endWidth: 6, startWidth: 6 },
						{ startValue: max_speed*0.05, endValue: max_speed*0.8, style: { fill: '#4bb648', stroke: '#4bb648' }, endWidth: 6, startWidth: 6 },
                        { startValue: max_speed*0.8, endValue: max_speed, style: { fill: '#e02629', stroke: '#e02629' }, endWidth: 6, startWidth: 6 }],
                ticksMinor: { interval: max_speed*0.1, size: '5%' },
                ticksMajor: { interval: max_speed*0.2, size: '9%' },
				caption: {
                          value: 'x100 rpm',
                          position: 'bottom',
                          offset: [0, 40],
                          visible: true
     },
				max: max_speed,
				min: -max_speed,
			    labels: { distance: label_dist,  interval: label_interval},
				startAngle: 67,
				endAngle: 382,
				border: {  size: '5%'},
                value: 0,
				cap: {
                      style: {
						  
                              fill: '#4bb648',
                              stroke: '#4bb648'
					         },
							// visible:true
                     },
			    pointer:{
					     style: {
                                 fill: 'black',
                                 stroke: 'black'
						        }
                        },
                width: 360, //  <!-- '30%' -->
                height: 360,  //<!-- '60%' -->
                animationDuration: animation_dur
            });
		
            $('#gaugeContainerSpeedM1').jqxGauge({
            cap: {
            size: '5%',
            style: {
                    fill: 'black',
                    stroke: 'black'
                   },
            visible: true
            }
           });

		 });
		
        // <!-- Current Linear Gauge -->
        		
		$(document).ready(function () {
            $('#gaugeLinearCurrentM1').jqxLinearGauge({
                orientation: 'horizontal',
                width: 300,
                height: 60,
                ticksMajor: { size: '18%', interval: current_scale/4 },
                ticksMinor: { size: '9%', interval: current_scale/16, style: { 'stroke-width': 1, stroke: '#aaaaaa'} },
                max: current_scale,
				min: 0,
                pointer: { size: '20%' },
                colorScheme: 'scheme05',
                labels: { interval: 2, position: 'near' },
				value: 0,
                ranges: [
                { startValue: 0, endValue: current_limit_default, style: { fill: '#4bb648', stroke: '#4bb648'} },
                { startValue: current_limit_default, endValue: current_scale*0.84, style: { fill: '#FF4800', stroke: '#FF4800'} },
                { startValue: current_scale*0.84, endValue: current_scale, style: { fill: '#e02629', stroke: '#e02629'}}],
                animationDuration: animation_dur
            });
        });
		
		// <!-- DC Bus Linear Gauge -->
		
        $(document).ready(function () {
            $('#gaugeLinearDCBusVoltageM1').jqxLinearGauge({
                orientation: 'horizontal',
                width: 300,
                height: 60,
                ticksMajor: { size: '18%', interval: dcbusvoltage_scale/3 },
                ticksMinor: { size: '9%', interval: dcbusvoltage_scale/6, style: { 'stroke-width': 1, stroke: '#aaaaaa'} },
                max: dcbusvoltage_scale*1.25,
				min: 0,
                pointer: { size: '20%' },
                colorScheme: 'scheme05',
                labels: { interval: 5, position: 'near' },
				value: 0,
                ranges: [
                { startValue: 0, endValue: dcbusvoltage_scale*0.53, style: { fill: '#FF4800', stroke: '#FF4800'} },
                { startValue: dcbusvoltage_scale*0.53, endValue: dcbusvoltage_scale*0.96, style: { fill: '#4bb648', stroke: '#4bb648'} },
                { startValue: dcbusvoltage_scale*0.96, endValue: dcbusvoltage_scale*1.25, style: { fill: '#FF4800', stroke: '#FF4800'}}],
                animationDuration: animation_dur
            });
        });
		
		 // <!-- Set Speed Slider -->
		
		$(document).ready(function () {
            $('#jqxSliderSetSpeedM1').jqxSlider({ 
				tooltip: true, 
				theme:'energyblue',
				mode: 'default',
				showButtons: true,  
				height: 30, //<!-- 30 -->
				width: 300, //<!-- 300-->
				min: -speed_scale, max: speed_scale,
				step: 500, 
				ticksFrequency: 500, 
				values: [-speed_scale, speed_scale]
			});
		});
			
		// <!-- Set Current Limit Slider -->
		
		$(document).ready(function () {
            $('#jqxSliderSetCurrentLimitM1').jqxSlider({ 
				mode: 'default',
				orientation: 'horizontal',
				tooltip: true, 
				theme:'energyblue',
				showButtons: true,  
				height: 30, //<!--150 --> 
				width: 300,  // <!--20 -->
				min: 0, max: current_scale,
				step: 0.2, 
				ticksFrequency: 0.5, 
				values: [0, current_scale],
				value: 3
			});	
        });
		
		// <!-- STOP BUTTON button -->
		
		 $(document).ready(function () {
               // Create a jqxToggleButton widget.
			 $("#jqxButton").jqxNotification({ 
				width: 250,
				height: 50,
				position: "top-centre",
				//appendContainer:"#container",
				opacity: 1,
				autoOpen:  false,
				autoClose: false,
				showCloseButton: false,
				template: "error"
			});
         });
		
		 // State Run Notification  
		
		$(document).ready(function () {
			$("#jqxAppStateRun").jqxNotification({ 
				width: 250,
				height: 50,
				position: "top-centre",
				opacity: 1,
				//appendContainer:"#containerState",
				autoOpen:  false,
				autoClose: false,
				showCloseButton: false,
				template: "success"
			});
		});
		
		 // Demo mode Notification  
		 
		  // $(document).ready(function(){
		  // $('#jqxSwitchButton').jqxSwitchButton({
            // height:50,
            // width: 250,
            // theme: 'energy blue',
            // onLabel:'DEMO ON',
            // offLabel:'DEMO OFF'
           // });
           // });
		
		$(document).ready(function () {
			$("#DemoButton").jqxButton({ 
				width: 242,
				height: 40,
				disabled: true,
				template: "success"
				});
		});
				
		 // State Stop Notification  -->
		
		$(document).ready(function () {
			$("#jqxAppStateStop").jqxNotification({ 
				width: 250,
				height: 50,
				position: "top-centre",
				opacity: 1,
				//appendContainer:"#containerState",
				autoOpen:  false,
				autoClose: false,
				showCloseButton: false,
				template: "error"
			});
		});
		
		// State Fault Over Voltage Notification  
		
		$(document).ready(function () {
			$("#jqxAppStateFaultOV").jqxNotification({ 
				width: 250,
				height: 50,
				position: "top-centre",
				opacity: 1,
				//appendContainer:"#container",
				autoOpen:  false,
				autoClose: false,
				showCloseButton: false,
				template: "error"
			});
		});
		
		//State Fault Under Voltage Notification  
		
		$(document).ready(function () {
			$("#jqxAppStateFaultUV").jqxNotification({ 
				width: 250,
				height: 50,
				position: "top-centre",
				opacity: 1,
				//appendContainer:"#container",
				autoOpen:  false,
				autoClose: false,
				showCloseButton: false,
				template: "error"
			});
		});
		
		// State Fault Over Current Notification  
		
		$(document).ready(function () {
			$("#jqxAppStateFaultOC").jqxNotification({ 
				width: 250,
				height: 50,
				position: "top-centre",
				//appendContainer:"#container",
				opacity: 1,
				autoOpen:  false,
				autoClose: false,
				showCloseButton: false,
				template: "error"
			});
		});
		
		// Current Limitation Notification  
		
		$(document).ready(function () {
			$("#jqxAppStateCurrentLimitation").jqxNotification({ 
				width: 250,
				height: 50,
				position: "top-centre",
				//appendContainer:"#container",
				opacity: 1,
				autoOpen:  false,
				autoClose: false,
				showCloseButton: false,
				template: "error"
			});
		});
		
	
    // Linear DC Bus Voltage tooltip 
	
     $(document).ready(function(){	
	 $("#gaugeLinearDCBusVoltageM1").jqxTooltip({
       position: 'bottom',
       theme: 'energyblue',
       content: 'Fault conditions still remaining!',
       width: 'auto',
       height: 'auto',
	   trigger: 'none',
	   absolutePositionX : 1,
	   absolutePositionY : 1,
	   autoHide :true,
	   closeOnClick : true,
	   disabled: false
   });
   });
   
  		//Info Menu
		
		$(document).ready(function(){
			Init();
		  var source = [{
		  label: "Info",
		  items:[{
			      label:"MCU",
		          subMenuWidth:'625px',
		          items:
		          [{
					 // html :"http://www.nhl.com",
		             html: '<div align="center"><a href = "http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=KV3x" target="_blank"> <img src="KV3X_BD.jpg" /></a><br><a href ="http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=KV3x&tab=Documentation_Tab&pspll=1&SelectedAsset=Documentation&ProdMetaId=PID/DC/KV3x&fromPSP=true&assetLockedForNavigation=true&componentId=2&leftNavCode=1&pageSize=25&Documentation=Documentation/00610Ksd1nd%60%60Data%20Sheets&fpsp=1&linkline=Data%20Sheets" target="_blank"; style = "text-decoration:underline; text-align:center"  ><span style ="text-align:center" >Datasheets & Reference Manuals</span></a></div>', 	  
		             //value:'',
					 
		           }]  
		         },
				 {
				label:"Freedom Board",
		        subMenuWidth:'425px',
		        items:
		              [{
		                //label:"",
						html:'<div align="center"><a href = "http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KV31F" target="_blank"><img src="FRDM-KV31F.jpg" /></a> <p> Arduino R3 Pin Compatible</p><p>OpenSDA</p></div>' 
		              }]	 
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
		  
	  }
	  ];
	  
	  $("#jqxmenu").jqxMenu({
	  source:source,
      width: 70,
      height: 32,
	  popupZIndex:99999,
	  mode: "vertical",
	  minimizeWidth:"null",
      theme: 'energyblue'
  });
		});
		
		
		// Finally some actions
		// Routines for clicks , hovers....
		
		$(document).ready(function(){
			
		$('#jqxSliderSetSpeedM1').on('change', function (event) {
			if(demo == 0)
			{
			set_speed= $('#jqxSliderSetSpeedM1').jqxSlider('getValue');
			succ = pcm.WriteVariable("Required Speed", set_speed, retMsg);
			$('#gaugeContainerSpeedRequired').jqxGauge({
			value: set_speed/100, });
			
			if(set_speed != 0)
			{
				succ = pcm.WriteVariable("Application Switch", 1, retMsg);
			if (fault ==0){
			$("#jqxButton").jqxNotification('open');
			$("#jqxAppStateRun").jqxNotification('open');
			}
			}	
			}
		});
						
		$('#jqxSliderSetCurrentLimitM1').on('change', function (event) {
			set_current_limit= $('#jqxSliderSetCurrentLimitM1').jqxSlider('getValue');
			succ = pcm.WriteVariable("DC Bus Current limit", set_current_limit, retMsg);
		
			$('#gaugeLinearCurrentM1').jqxLinearGauge({
                ranges: [
                { startValue: 0, endValue: set_current_limit, style: { fill: '#4bb648', stroke: '#4bb648'} },
                { startValue: set_current_limit, endValue: current_scale*0.84, style: { fill: '#FF4800', stroke: '#FF4800'} },
                { startValue: current_scale*0.84, endValue: current_scale, style: { fill: '#e02629', stroke: '#e02629'}}],
            });
		});
			
						
		$("#jqxButton").on('click', function () {
            succ = pcm.WriteVariable("Application Switch", 0, retMsg);
			$('#gaugeContainerSpeedRequired').jqxGauge({
			value: 0, });
			$('#gaugeContainerSpeedM1').jqxGauge({
			value: 0, });
			$("#jqxButton").jqxNotification('closeAll'); 
			
        });
		
		$("#jqxAppStateFaultOV").on('click', function () {
				if(dcbvoltage < dcbusvoltage_scale*0.99)
				{
				succ = pcm.WriteVariable("Fault", 0, retMsg);
				$("#DemoButton").jqxButton({template:'inverse'});
				//$("#jqxSwitchButton").val(false);
				succ = pcm.WriteVariable("bDemoMode", 0, retMsg);
				$("#jqxAppStateFaultOV").jqxNotification("close");
				$("#gaugeLinearDCBusVoltageM1").jqxTooltip('close');
					
				succ = pcm.WriteVariable("Application Switch", 1, retMsg);
				succ = pcm.WriteVariable("Application State", 1, retMsg); // init
				$('#gaugeContainerSpeedRequired').jqxGauge({
                value: 0, });
				$('#jqxSliderSetSpeedM1').jqxGauge({
                value: 0, });
			
				
				}
				else
				{	
                    $("#jqxAppStateFaultOV").jqxNotification("open");	
                    $("#gaugeLinearDCBusVoltageM1").jqxTooltip('open');
					$("#jqxAppStateRun").jqxNotification("closeAll");
                    $("#jqxAppStateStop").jqxNotification("closeAll");					
					$("#jqxButton").hide();
					$('#gaugeContainerSpeedRequired').jqxGauge({
                    value: 0, });
				}
				
				
		});
		
		$("#jqxAppStateFaultUV").on('click', function () {
				if(dcbvoltage > dcbusvoltage_scale*0.53)
				{
				succ = pcm.WriteVariable("Fault",0, retMsg);
				$("#DemoButton").jqxButton({template:'inverse'});
				//$("#jqxSwitchButton").val(false);
				succ = pcm.WriteVariable("bDemoMode", 0, retMsg);
				$("#jqxAppStateFaultUV").jqxNotification("close");
				$("#gaugeLinearDCBusVoltageM1").jqxTooltip('close');
				
				succ = pcm.WriteVariable("Application Switch",1, retMsg);
				succ = pcm.WriteVariable("Application State",1, retMsg); // init
								
				$('#gaugeContainerSpeedRequired').jqxGauge({
                value: 0, });
				$('#jqxSliderSetSpeedM1').jqxGauge({
                value: 0, });
			
				
				}
				else
				{	
                    $("#jqxAppStateFaultUV").jqxNotification("open");	
                    $("#gaugeLinearDCBusVoltageM1").jqxTooltip('open');
					$("#jqxAppStateRun").jqxNotification("closeAll");
                    $("#jqxAppStateStop").jqxNotification("closeAll");					
					$("#jqxButton").hide();
					$('#gaugeContainerSpeedRequired').jqxGauge({
                    value: 0, });
				}
				
		});
		
		$("#jqxAppStateFaultOC").on('click', function () {
				succ = pcm.WriteVariable("Fault", 0, retMsg)
				$("#jqxAppStateFaultOC").jqxNotification("close");
				succ = pcm.WriteVariable("Application Switch", 1, retMsg);
				succ = pcm.WriteVariable("Application State", 1, retMsg); // init
				$("#jqxButton").jqxNotification('closeAll');
				$("#DemoButton").jqxButton({template:'inverse'});
		});
		
		$("#DemoButtonDiv").on('click',function(){
			 if(demovar)
			 {
			  $("#DemoButton").jqxButton({disabled:true});
			  succ = pcm.WriteVariable("bDemoMode",0,retMsg);
              demovar=0;
			 
			 }
			 else
			 {
			  succ = pcm.WriteVariable("bDemoMode",1,retMsg);
              demovar=1;
			  $("#DemoButton").jqxButton({disabled:false});
			 }
		})
		
		// $("#DemoButton").on('click',function(){
			 // if(demovar)
			 // {
			  // succ = pcm.WriteVariable("bDemoMode",0,retMsg);
              // demovar=0;
			  // $("#DemoButton").jqxButton({template:'inverse'});
			 // }
			 // else
			 // {
			  // succ = pcm.WriteVariable("bDemoMode",1,retMsg);
              // demovar=1;
			  // $("#DemoButton").jqxButton({template:'success'});
			 // }
		// })
		
		$('#jqxSwitchButton').bind('change', function (event){
			 if(demovar)
			 {
			  succ = pcm.WriteVariable("bDemoMode",0,retMsg);
              demovar=0;
			 }
			 else
			 {
			  succ = pcm.WriteVariable("bDemoMode",1,retMsg);
              demovar=1;
			 }
		 });
		
	  		
		});

/* ---- Initialization ------------------------*/
function Init() {
    
    // Gauge, Buttons control - subscribed FreeMASTER variables for onChange detection
   appSwitchID = pcm.SubscribeVariable("Application Switch", 1000);
    
   appStateID  = pcm.SubscribeVariable("Application State", 1000);
    
   actSpeedID  = pcm.SubscribeVariable("Actual Speed", 100);
    
   busVoltID   = pcm.SubscribeVariable("DC Bus Voltage", 450);  
    
   busCurrID   = pcm.SubscribeVariable("DC Bus Current", 150);      
    
   faultID     = pcm.SubscribeVariable("Fault", 1000);      
   
   demoID   = pcm.SubscribeVariable("bDemoMode",800); 
   
   demospeedID = pcm.SubscribeVariable("Required Speed",1100); 
    
    $('#gaugeLinearDCBusVoltageM1').jqxLinearGauge({
                value: 0,  });
			
				
	$('#gaugeLinearCurrentM1').jqxLinearGauge({
                value: 3, });						
				
				
	$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
				
				set_current_limit=3;
				
	//READ Speed, Current, DCbus Scales
	succ = pcm.ReadVariable("FM_speed_scale", vValue0, tValue0, retMsg)
	if (succ)
	{
		speed_scale = pcm.LastVariable_vValue;
        speed_scale = speed_scale/1000;		
	}

    succ = pcm.ReadVariable("FM_current_scale", vValue0, tValue0, retMsg)
	if (succ)
	{
		current_scale = pcm.LastVariable_vValue;
        current_scale = current_scale/1000;		
	}	
	
	succ = pcm.ReadVariable("FM_voltage_scale", vValue0, tValue0, retMsg)
	if (succ)
	{
		dcbusvoltage_scale = pcm.LastVariable_vValue;
        dcbusvoltage_scale = dcbusvoltage_scale/1000;		
	}
	
	succ = pcm.ReadVariable("Application State", vValue0, tValue0, retMsg)
		if (succ)
		{
			AppState = pcm.LastVariable_vValue;		
		}	
		
		if (AppState == 2)
		{
			$("#jqxAppStateStop").jqxNotification("open");			
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
				
				succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
		}
		if (AppState == 3)
		{
			$("#jqxAppStateRun").jqxNotification("open");
		}
        
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg);
			succ = pcm.WriteVariable("Required Speed", 0, retMsg);
			
			succ = pcm.ReadVariable("Fault", vValue0, tValue0, retMsg)
		if (succ)
		{
			fault = pcm.LastVariable_vValue;		
		}	
		
		if (fault == 1)
		{
			$("#jqxAppStateFaultUV").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
				$("#jqxAppStateFaultUV").jqxTooltip('open');
		}
	
		if (fault == 2)
		{
			$("#jqxAppStateFaultOV").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
		}
		
		if (fault == 4)
		{
			$("#jqxAppStateFaultOC").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
		}
		
		
			succ = pcm.WriteVariable("bDemoMode", 0, retMsg);
			
			 
		
 }

//OnVariableChanged event generated by FreeMASTER
function Event_OnVariableChanged(a,IDsubscribedVariable)
{
    switch(IDsubscribedVariable){
        case appSwitchID:
            // sensor type changed in FreeMASTER
            ApplicationSwitch();
            break;
        case appStateID:
            ApplicationState();
            break;
        case actSpeedID:
            // Actual Speed
            ActualSpeed();
            break;
        case busVoltID:
            // Speed feedback sensing
            DCBusVoltage();
            break;
        case busCurrID:
            // event on speed reference change 
            DCBusCurrent();
            break;           
        case faultID:
            // Application state update
            Fault();
            break; 
	    case demoID:
            // Demo mode
        Demo();
		break;
		case demospeedID:
            // Demo mode speed
        Demospeed();
		break; 
        default:
    }
}

/* ------------------ On-Off toggle button routine - ------------------------- */         
    function ApplicationSwitch(){
        succ = pcm.ReadVariable("Application Switch", vValue0, tValue0, retMsg)
		if (succ)
		{
			reset_switch = pcm.LastVariable_vValue;		
		}	
        if (reset_switch == 0)
		{
			$('#jqxSliderSetSpeedM1').jqxSlider({
			value: 0});
			
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
			
				
			$('#jqxSliderSetCurrentLimitM1').jqxSlider({
			value: 3});
					
		}
		else
		{
		
		if (fault == 1)
		{
			$("#jqxAppStateFaultUV").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
		}
	
		if (fault == 2)
		{
			$("#jqxAppStateFaultOV").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
		}
		
		if (fault == 4)
		{
			$("#jqxAppStateFaultOC").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
		}
		}
		
      }
    
    
   function ApplicationState(){
        succ = pcm.ReadVariable("Application State", vValue0, tValue0, retMsg)
		if (succ)
		{
			AppState = pcm.LastVariable_vValue;		
		}	
		
		if (AppState == 2)
		{
			$("#jqxAppStateStop").jqxNotification("open");			
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
				
				succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
		}
		if (AppState == 3)
		{
			$("#jqxAppStateRun").jqxNotification("open");
		}	
    }     
/* ---------------- EOF On-Off toggle button routine ------------------------- */  

function ActualSpeed(){
	
		succ = pcm.ReadVariable("Actual Speed", vValue0, tValue0, retMsg)
		if (succ)
		{
			speed = pcm.LastVariable_vValue;
		}	
			
		$('#gaugeContainerSpeedM1').jqxGauge({
			value: speed/100, });
}

function DCBusVoltage(){

		succ = pcm.ReadVariable("DC Bus Voltage", vValue0, tValue0, retMsg)
		if (succ)
		{
			dcbvoltage = pcm.LastVariable_vValue;	
		}	
		
		$('#gaugeLinearDCBusVoltageM1').jqxLinearGauge({
		value: dcbvoltage,  });
}
		
function DCBusCurrent(){
	
		succ = pcm.ReadVariable("DC Bus Current", vValue0, tValue0, retMsg)
		if (succ)
		{
			dcbcurrent = pcm.LastVariable_vValue;
		}	
			
		$('#gaugeLinearCurrentM1').jqxLinearGauge({
			value: dcbcurrent, });
			
		if(set_current_limit < dcbcurrent)
		{
				$("#jqxAppStateCurrentLimitation").jqxNotification("open");
		}
		
		if(set_current_limit> dcbcurrent)
		{
				$("#jqxAppStateCurrentLimitation").jqxNotification("closeAll");
		}
}		

function Fault() {

		succ = pcm.ReadVariable("Fault", vValue0, tValue0, retMsg)
		if (succ)
		{
			fault = pcm.LastVariable_vValue;		
		}	
		
		if (fault == 1)
		{
			$("#jqxAppStateFaultUV").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
				$("#jqxAppStateFaultUV").jqxTooltip('open');
		}
	
		if (fault == 2)
		{
			$("#jqxAppStateFaultOV").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
		}
		
		if (fault == 4)
		{
			$("#jqxAppStateFaultOC").jqxNotification("open");
			$("#jqxButton").jqxNotification("closeAll");
			succ = pcm.WriteVariable("Actual Speed", 0, retMsg)
			$('#gaugeContainerSpeedM1').jqxGauge({
                value: 0, });
		}		
}

function Demo() {

		succ = pcm.ReadVariable("bDemoMode", vValue0, tValue0, retMsg);
		if (succ)
		{
			demo = pcm.LastVariable_vValue;		
		}	
		
		if (demo)
		{
		//	$("#jqxDemo").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
			$("#jqxAppStateFaultUV").jqxNotification("closeAll");
			$("#jqxAppStateStop").jqxNotification("closeAll");
			$("#jqxAppStateRun").jqxNotification("closeAll");
			$("#jqxButton").jqxNotification("closeAll");
		}
		else
		{
			succ = pcm.WriteVariable("Required Speed",0, retMsg);
			//$("#jqxDemo").jqxNotification("closeAll");
			$('#gaugeContainerSpeedM1').jqxGauge({
			value: 0, });
			$('#gaugeContainerSpeedRequired').jqxGauge({
			value: 0, });
			$('#jqxSliderSetSpeedM1').jqxSlider({
			value: 0, });
			
			if (fault == 1)
		{
			$("#jqxAppStateFaultUV").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
		}
	
		if (fault == 2)
		{
			$("#jqxAppStateFaultOV").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
		}
		
		if (fault == 4)
		{
			$("#jqxAppStateFaultOC").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
		}
		
		if (AppState == 2)
		{
			$("#jqxAppStateStop").jqxNotification("open");			
		}
		if (AppState == 3)
		{
			$("#jqxAppStateRun").jqxNotification("open");
		}
		
		}
	
				
}

function Demospeed() {

		succ = pcm.ReadVariable("Required Speed", vValue0, tValue0, retMsg)
		if (succ)
		{
			demospeed = pcm.LastVariable_vValue;		
		}	
		
		if (demo)
		{
			//$("#jqxDemo").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
			
			$('#gaugeContainerSpeedRequired').jqxGauge({
			value: demospeed/100, });
			if (fault == 1)
		{
			$("#jqxAppStateFaultUV").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
		}
			//$("#jqxAppStateFaultUV").jqxNotification("closeAll");
					
		}
		else
		{
		
		if (fault == 1)
		{
			$("#jqxAppStateFaultUV").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
		}
	
		if (fault == 2)
		{
			$("#jqxAppStateFaultOV").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
		}
		
		if (fault == 4)
		{
			$("#jqxAppStateFaultOC").jqxNotification("open");
			// $("#jqxFaultButton").jqxRepeatButton({disabled: false});
		}
		}
}
