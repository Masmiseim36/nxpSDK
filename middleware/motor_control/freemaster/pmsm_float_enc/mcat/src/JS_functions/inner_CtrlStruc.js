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
* @file   inner_CtrlStruc.js
*
* @brief  Control structure tab engine
*
******************************************************************************/
/******************************************************************************
* Global variables
******************************************************************************/
var xmlDoc = loadXMLDoc("xml_files\\FM_params_list.xml");
var Timer_object;
var pospeArray  =   new Array(4); 
var MotorPrefix;
var controlMethodId;

var k_factor_max  = 175;
var k_factor_min  = 25;
var k_factor_basic = 140;
/***************************************************************************//*!
* @brief   The function check XML field ti not empty and try to read FM variable
* @param    
* @return  
* @remarks 
*******************************************************************************/   
function checkXMLvar()
{
    var tempXMLVar;
    // names of XML field used in CtrlScructure
    var xmlElements = new Array('ControlStructureMethod',
                                'onoff',
                                'SCALAR_VHZ_FACTOR_GAIN',
                                'Freq_req',
                                'Ud_req',
                                'Uq_req',
                                'Id_req',
                                'Iq_req',
                                'Speed_req',
                                'Pos_req');

    var errorFMread = '';
    var errorXMLfield = '';
  
    // check defined XML fields 
    for(var j=0;j<xmlElements.length;j++){
        tempXMLVar = xmlDoc.getElementsByTagName(MotorPrefix+ xmlElements[j])[0];
  
        if(tempXMLVar.childNodes.length!=0)
        {
            if(!(pcm.ReadVariable(tempXMLVar.childNodes[0].nodeValue)))
                errorFMread = errorFMread + '\n "' + tempXMLVar.childNodes[0].nodeValue + '"';
        }
        else // XML field empty
            errorXMLfield = errorXMLfield + '\n"' +MotorPrefix+ xmlElements[j] + '" ';
    }

    if(errorXMLfield!='')
        alert('Error: Empty XML field: '+ errorXMLfield);
     
    if(errorFMread!='')
        alert('Read error of FM variables: '+ errorFMread);
   
    if((errorXMLfield!='')||(errorFMread!=''))
        return (false);
    else
        return (true);
}

/***************************************************************************//*!
* @brief   Function performs page on load event
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function initLoadFormCascade()
{
    var objectCCS    = document.getElementById('ControlStructureTable').offsetHeight;
        
    document.getElementById('stateControlField').style.height = (objectCCS+10) + 'px';
    document.getElementById('ccsField').style.height = (objectCCS+10) + 'px';
    
    var obj = document.getElementById('V/Hz_factor');
    obj.title = obj.title + '\n'+ 'Max_factor = ' +k_factor_max.toString() + '% \n'+ 'Min_factor = ' +k_factor_min.toString()+'%';
    
    if(document.getElementById(MotorPrefix + 'scalar_ctrl_Input2_Um'))
        document.getElementById(MotorPrefix + 'scalar_ctrl_Input2_Um').disabled = true;
            
    if(!pcm.IsCommPortOpen())
    {
        alert("Communication is stopped.\nPress Ctrl+K to start the communication");
    }
    else
    {
        //get the prefix of active motor
        MotorPrefix = getActiveMotor();

        reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "onoff"])[0];
        reference_val_1 = reference_val.childNodes[0].nodeValue;
        pcm.SubscribeVariable(reference_val_1, 1000);

        reference_val_2 = xmlDoc.getElementsByTagName([MotorPrefix+ "states"])[0].getAttribute("FreemasterName");
        pcm.SubscribeVariable(reference_val_2, 500);
        
        reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "ControlStructureMethod"])[0];
        reference_val_3 = reference_val.childNodes[0].nodeValue;
        pcm.SubscribeVariable(reference_val_3, 1500);

        reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "ControlStructurePoSpe"])[0];
        reference_val_4 = reference_val.childNodes[0].nodeValue;
        pcm.SubscribeVariable(reference_val_4, 2000);  

        // basic mode, disable some inputs and pre-calculate parameters
        if(getActiveMode()===0)
        {
            //disable U/rad control
            disableInputParamBox(MotorPrefix+'scalar_ctrl_Input2');
            //disable d-axis voltage control
            disableInputParamBox(MotorPrefix+'volt_ctrl_Input1');
            //disable d-axis current control
            disableInputParamBox(MotorPrefix+'current_ctrl_Input1');
        }
      
        sensorTypeSelectInit();
        OnOff_Input(reference_val_1);
        CascadeStruc_Input(reference_val_3);
        StateMachine(reference_val_2);
    }
}

/***************************************************************************//*!
* @brief   The function OnVariableChanged event generated by FreeMASTER
* @param
* @return  None
* @remarks
******************************************************************************/
function Event1_OnVariableChanged(IDsubscribedVariable)
{
    switch(IDsubscribedVariable){
    case reference_val_1:
        // On/Off control
        OnOff_Input(IDsubscribedVariable);
        break;
    case reference_val_2:
        // Application state update
        StateMachine(IDsubscribedVariable);
        break;
    case reference_val_3:
        // Cascade control structure select
        CascadeStruc_Input(IDsubscribedVariable);
        break;
    case reference_val_4:
        // Cascade control structure select
        sensorTypeInput(IDsubscribedVariable);
        break;
    case reference_val_5:
        // Input text field update - first row
        updateCCStextField(controlMethodId,IDsubscribedVariable,'Input1');
        break;
    case reference_val_6:
        // Input text field update - second row
        updateCCStextField(controlMethodId,IDsubscribedVariable,'Input2');
        break;
    default:
    }
}

/***************************************************************************//*!
* @brief   On-Off toggle button routine - after load
* @param   None
* @return  None
* @remarks 
******************************************************************************/ 
function OnOff_Input(elementId)
{

    var object1     = document.getElementById('sw_app_onoff');
      
    if(pcm.ReadVariable(elementId)){
        var ValueState  = pcm.LastVariable_vValue;
        if(ValueState===1){
            object1.className = 'appOn';}
        else{
            object1.className = 'appOff';}
    }
} 
    
/***************************************************************************//*!
* @brief   On-Off toggle button routine - on click
* @param   None
* @return  None
* @remarks
******************************************************************************/
function OnOff_Click()
{
    var states_obj = xmlDoc.getElementsByTagName([MotorPrefix+ "states"])[0].getAttribute("FreemasterName");
    var ValueState;
    
    /* read application status variable */
    if(pcm.ReadVariable(states_obj))
    {
        ValueState  = pcm.LastVariable_vValue;
    }    
    
    var retMsg;
    var on_off = xmlDoc.getElementsByTagName([MotorPrefix]+"onoff");
    var object = document.getElementById('sw_app_onoff');

    if(pcm.ReadVariable(on_off[0].childNodes[0].nodeValue)) {
        var onoffvar = pcm.LastVariable_vValue;
        if(onoffvar === 0){
            onoffvar = 1;
            object.className    = "appOn";}
        else{
            onoffvar = 0;
            object.className    = "appOff"; }
    }

    /* for Fault state, set off state */
    if(ValueState === 0)
    {
        onoffvar = 0;
        object.className    = "appOff";
    }
    
    succ = pcm.WriteVariable(on_off[0].childNodes[0].nodeValue, onoffvar, retMsg);
}  

/***************************************************************************//*!
* @brief   update the text input fields in control page based on the Freemaster values
* @param   None
* @return  None
* @remarks 
******************************************************************************/ 
function updateCCStextField(methodId, elementId, inputNo)
{

    textControlType = conversionNo2Ctrl(methodId);
    var Motor_pp      = getParentHtmlValue("pp");
      
    if(pcm.ReadVariable(elementId)){
        switch (methodId){
        case 0:
            if(inputNo==='Input1')
                document.getElementById(MotorPrefix + 'scalar_ctrl_Input1').value = (pcm.LastVariable_vValue).toFixed(0);
            break;
        case 1:
            if(inputNo==='Input1')
                document.getElementById(MotorPrefix + 'volt_ctrl_Input1').value = pcm.LastVariable_vValue.toFixed(1); 
            else
                document.getElementById(MotorPrefix + 'volt_ctrl_Input2').value = pcm.LastVariable_vValue.toFixed(1); 
            break;
        case 2:
            if(inputNo==='Input1')
                document.getElementById(MotorPrefix + 'current_ctrl_Input1').value = pcm.LastVariable_vValue.toFixed(1);            
            else
                document.getElementById(MotorPrefix + 'current_ctrl_Input2').value = pcm.LastVariable_vValue.toFixed(1);            
            break;
        case 3:
            if(inputNo==='Input1')
                document.getElementById(MotorPrefix + 'speed_ctrl_Input1').value = pcm.LastVariable_vValue.toFixed(0);            
            break;
        case 4:
            if(inputNo==='Input1')
                document.getElementById(MotorPrefix + 'pos_ctrl_Input1').value = pcm.LastVariable_vValue.toFixed(0);            
            break;    
        }
    }
}

/***************************************************************************//*!
* @brief   Control Structure select -  button click/update routine
* @param   None
* @return  None
* @remarks 
******************************************************************************/ 
function CascadeStruc_Input(elementId)
{
    var textControlType;

    for(var i=0;i<5;i++){
        // switch-off all switches EN/DIS in the Application Control Structure page
        CascadeMethods(i,'Disable');

        // disable all input text fields in the Application Control Structure page by default
        // and set all fields to zero
        textControlType = conversionNo2Ctrl(i);
        document.getElementById(MotorPrefix + textControlType +'_Input1').disabled = true;
        document.getElementById(MotorPrefix + textControlType +'_Input1').value = 0;
    
        if(document.getElementById(MotorPrefix + textControlType +'_Input2')){
            document.getElementById(MotorPrefix + textControlType +'_Input2').disabled = true;
        
        if(textControlType==='scalar_ctrl'){
            // basic mode, disable some inputs and pre-calculate paramters
            if(getActiveMode()===0)
            {
                // set default value of k_factor in case of basic mode
                k_factor = k_factor_basic;
                // write actual k_factor value
                document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value = Math.round(k_factor*100)/100;
            }
            else // expert mode
            {
                var k_factor = Number(parent.document.getElementById(MotorPrefix+'SCALAR_Factor').innerText);
                document.getElementById(MotorPrefix + textControlType +'_Input2').value = k_factor;
            }
        
            document.getElementById(MotorPrefix+'ScalarUp').disabled = false;
            document.getElementById(MotorPrefix+'ScalarDown').disabled = false;
            document.getElementById(MotorPrefix+'ScalarUp').className = 'UpDownButton_en';
            document.getElementById(MotorPrefix+'ScalarDown').className = 'UpDownButton_en';
        }
        else
            document.getElementById(MotorPrefix + textControlType +'_Input2').value = 0;
    
            document.getElementById(MotorPrefix+'ScalarUp').disabled = true;
            document.getElementById(MotorPrefix+'ScalarDown').disabled = true;
            document.getElementById(MotorPrefix+'ScalarUp').className = 'UpDownButton_dis';
            document.getElementById(MotorPrefix+'ScalarDown').className = 'UpDownButton_dis';
        }
    }

    //Enable Position/Speed button by default
    CascadeMethods(5,'Enable');
    
    if(pcm.ReadVariable(elementId)){
        // get control method ID raw value from freemaster
       controlMethodId = pcm.LastVariable_vValue;
        //Enable selected Control Method button
       CascadeMethods(controlMethodId,'Enable');
       
       // enable appropriate input text field according to selected control method
       textControlType = conversionNo2Ctrl(controlMethodId);                     
       document.getElementById(MotorPrefix + textControlType +'_Input1').disabled = false;    
       
       if(document.getElementById(MotorPrefix + textControlType +'_Input2'))
       document.getElementById(MotorPrefix + textControlType +'_Input2').disabled = false;          
       
        switch(controlMethodId){
        case 0:
            //Disable Position/Speed button in Scalar Mode
            CascadeMethods(5,'Disable');

            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Freq_req"])[0];
            reference_val_5 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_5, 1500);

            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "SCALAR_VHZ_FACTOR_GAIN"])[0];
            reference_val_6 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_6, 1500);    

            document.getElementById(MotorPrefix+'ScalarUp').disabled = false;                                            
            document.getElementById(MotorPrefix+'ScalarDown').disabled = false;            
            document.getElementById(MotorPrefix+'ScalarUp').className = 'UpDownButton_en';
            document.getElementById(MotorPrefix+'ScalarDown').className = 'UpDownButton_en'; 
            break;
        case 1:
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Ud_req"])[0];
            reference_val_5 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_5, 1500);
            
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Uq_req"])[0];
            reference_val_6 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_6, 1500);
            break;

        case 2: 
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Id_req"])[0];
            reference_val_5 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_5, 1500);
            
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Iq_req"])[0];
            reference_val_6 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_6, 1500);
            break;

        case 3: 
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Speed_req"])[0];
            reference_val_5 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_5, 1500);
            break;
            
        case 4: 
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Pos_req"])[0];
            reference_val_5 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_5, 1500);
            
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Speed_req"])[0];
            reference_val_6 = reference_val.childNodes[0].nodeValue;
            pcm.SubscribeVariable(reference_val_6, 1500);
            
            //Disable Position/Speed button in Position control
            CascadeMethods(5,'Disable');
           
            // Unsubscribe all second text fields for non-speed control
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "SCALAR_VHZ_FACTOR_GAIN"])[0].childNodes[0].nodeValue;
            pcm.UnSubscribeVariable(reference_val);
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Uq_req"])[0].childNodes[0].nodeValue;
            pcm.UnSubscribeVariable(reference_val);
            reference_val = xmlDoc.getElementsByTagName([MotorPrefix+ "Iq_req"])[0].childNodes[0].nodeValue;
            pcm.UnSubscribeVariable(reference_val);
            break;

        default:
        }

    }
}

/***************************************************************************//*!
* @brief   CascadeMethods
* @param   None
* @return  None
* @remarks
******************************************************************************/
function CascadeMethods(methodId, endisStatus)
{
    var object = new Array(6);
     
    object[0] = document.getElementById([MotorPrefix]+'scalar_ctrl');
    object[1] = document.getElementById([MotorPrefix]+'volt_ctrl');
    object[2] = document.getElementById([MotorPrefix]+'current_ctrl');      
    object[3] = document.getElementById([MotorPrefix]+'speed_ctrl');
    object[4] = document.getElementById([MotorPrefix]+'pos_ctrl');
    object[5] = document.getElementById([MotorPrefix]+'pospe_ctrl'); 
    
    // Enable/disable the En/Dis button in the CCS workspace
    if(endisStatus==='Enable'){
        object[methodId].className    = "switch_on";
        object[methodId].style.color  = "white";
        object[methodId].innerHTML    = "ENABLED";         
    }
    else{
        object[methodId].className    = "switch_off";
        object[methodId].style.color  = "gray";
        object[methodId].innerHTML    = "DISABLED";         
    }
    
    // Enable/disable the sensor type selector based on the control method
    if(object[5].innerHTML !== "ENABLED")  
        document.getElementById(MotorPrefix+'pospe_ctrl_Select').disabled = true;
    else    
        document.getElementById(MotorPrefix+'pospe_ctrl_Select').disabled = false;

}

/***************************************************************************//*!
*
* @brief   Sensor type selector init routine - according to parent doc. table
* @param   
* @return  None
* @remarks 
******************************************************************************/     
function sensorTypeSelectInit()
{
    var optionObject;
    var selectedSensor;
    var j = 0;
  
    for(var i=0;i<5;i++){ pospeArray[i] = i;}
    // read from main page table
    selectedSensor =  parent.document.getElementById(MotorPrefix+'PospeFbck').innerHTML;
    // read object by ID from CtrlStructurePage
    optionObject = document.getElementById(MotorPrefix + "pospe_ctrl_Select");
 
    for(var i=0;i<5;i++)
    {
        pospeArray[i] = j;
        if (((selectedSensor>>>[i])&1) !== 1){
            optionObject.remove(j);}
        else{
            j++;}
    }
}

/***************************************************************************//*!
*
* @brief   Sensor type selector init routine - according to parent doc. table
* @param
* @return  None
* @remarks
******************************************************************************/
function sensorTypeInput(elementId)
{

    if(pcm.ReadVariable(elementId)){
        document.getElementById(MotorPrefix +'pospe_ctrl_Select').selectedIndex = pcm.LastVariable_vValue;
    }
}

/***************************************************************************//*!
* @brief   The function converts the Number to String form, according to selected control method
* @param   inPointer - number representing the selected ctrl method 
* @return  string of selected control method
* @remarks 
*******************************************************************************/   
function conversionNo2Ctrl(inPointer)
{
    var arithmeticArray=new Array(5);
    arithmeticArray[0] = "scalar_ctrl";
    arithmeticArray[1] = "volt_ctrl";
    arithmeticArray[2] = "current_ctrl";
    arithmeticArray[3] = "speed_ctrl";    
    arithmeticArray[4] = "pos_ctrl";

    return arithmeticArray[inPointer];     
} 

/***************************************************************************//*!
* @brief   The function converts the Number to String form, according to selected reference signals
* @param   inPointer - number representing the selected ctrl method 
* @return  string of selected reference signals
* @remarks 
*******************************************************************************/
function conversionXml2Variables(inPointer)
{
    var referencesArray=new Array(8);
    referencesArray[0] = "SCALAR_VHZ_FACTOR_GAIN";
    referencesArray[1] = "Freq_req";
    referencesArray[2] = "Ud_req";
    referencesArray[3] = "Uq_req";    
    referencesArray[4] = "Id_req";
    referencesArray[5] = "Iq_req";    
    referencesArray[6] = "Speed_req";
    referencesArray[7] = "Pos_req";
    
    return referencesArray[inPointer];     
}

/***************************************************************************//*!
*
* @brief   Change of the cascade control structure method - according to click
* @param   optionNo - number of selected method
* @return  None
* @remarks 
******************************************************************************/ 
function sensorTypeChange(optionNo)
{
    var retMsg;                                                                               
    var prefixM = getActiveMotor();  
    var PospeRegisterVal = xmlDoc.getElementsByTagName([prefixM]+"ControlStructurePoSpe")[0];

    succ = pcm.WriteVariable(PospeRegisterVal.childNodes[0].nodeValue, Number(optionNo), retMsg);    
}

/***************************************************************************//*!
* @brief   Change of the Id control method - according to selector
* @param   optionNo - number of selected method
* @return  None
* @remarks 
******************************************************************************/ 
function IdMethodChange(optionNo)
{
    var retMsg;
    var prefixM = getActiveMotor();  
    var CtrlRegisterVal = xmlDoc.getElementsByTagName([prefixM]+"ControlStructureMethod")[0];
    
    succ = pcm.WriteVariable(CtrlRegisterVal.childNodes[0].nodeValue, Number(optionNo)+3, retMsg);
}

/***************************************************************************//*!
* @brief   Cascade control structure selector
* @param   varID - ID of selected control structure
* @return  None
* @remarks 
******************************************************************************/ 
function Ctrl_Structure_click(varID)
{
    var retMsg;
    var ctrlRegister  = null;
   
    var Nmax          = getParentHtmlValue("N_max");
    var UDCmax        = getParentHtmlValue("UDC_max");
    var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
    var Uphnom        = getParentHtmlValue("U_ph");
    var N_nom         = getParentHtmlValue("N_req");
    var Motor_pp      = getParentHtmlValue("pp");
    var Umax          = Math.round(UDCmax/UmaxCoeff*10)/10;
    
    var aritType      = parent.document.getElementById("Arithmetic").innerText;
                                        
    var CtrlRegisterVal  = xmlDoc.getElementsByTagName([MotorPrefix]+"ControlStructureMethod")[0];
    var on_off           = xmlDoc.getElementsByTagName([MotorPrefix]+"onoff")[0];

    if(!pcm.IsCommPortOpen()){return;}
    
    // read the state of ON/OFF switch    
    if(pcm.ReadVariable(on_off.childNodes[0].nodeValue));
    {
        var onoffvar = pcm.LastVariable_vValue;
      
        // If ON state, the change of the control structure is not allowed
	if(onoffvar === 1)
        {
            alert("The control structure cannot be changed during the RUN state!"+ '\n' + "Switch off the application.");        
        }
        else // If OFF state is active, the change of the control structure is allowed
        {
            if(pcm.ReadVariable(CtrlRegisterVal.childNodes[0].nodeValue))
            {
                // read currently active control mode
                var CCSmethodVal = pcm.LastVariable_vValue;
                
                // Disable the current active mode - button
                CascadeMethods(CCSmethodVal, 'Disable');

                //Disable Pos/Speed signal button
                CascadeMethods(5, 'Disable');

                // Disable the current active mode - input fields disabled
                textControlType = conversionNo2Ctrl(CCSmethodVal);                     
                document.getElementById(MotorPrefix + textControlType +'_Input1').disabled = true;    

                if(document.getElementById(MotorPrefix + textControlType +'_Input2'))
                document.getElementById(MotorPrefix + textControlType +'_Input2').disabled = true;    
            }
            
            // based on the control mode selection generate ctrlRegister value - send to FRM
            if (varID === MotorPrefix+'scalar_ctrl')    
            {
                ctrlRegister = 0;
                
                /* check for enabled Basic mode */
                if(getActiveMode()==0)
                {
                    // set default value of k_factor in case of basic mode
                    k_factor = k_factor_basic;

                    /* write actual k_factor value */
                    document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value = Math.round(k_factor*100)/100;
                }
                else /* expert mode */
                { 
                    /*  read actual k_factor value */
                    k_factor = Number(parent.document.getElementById(MotorPrefix+'SCALAR_Factor').innerHTML);

                    document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value = parent.document.getElementById(MotorPrefix+'SCALAR_Factor').innerHTML;
                }

                document.getElementById(MotorPrefix+'ScalarUp').disabled = false;
                document.getElementById(MotorPrefix+'ScalarDown').disabled = false;
                
                document.getElementById(MotorPrefix+'ScalarUp').className = 'UpDownButton_en';
                document.getElementById(MotorPrefix+'ScalarDown').className = 'UpDownButton_en';                
            }                              

            if (varID === MotorPrefix+'volt_ctrl')      
            {
                ctrlRegister = 1;
                document.getElementById(MotorPrefix+'volt_ctrl_Input1').value = 0;
                document.getElementById(MotorPrefix+'volt_ctrl_Input2').value = 0;
                document.getElementById(MotorPrefix+'ScalarUp').disabled = true;                                            
                document.getElementById(MotorPrefix+'ScalarDown').disabled = true;
                document.getElementById(MotorPrefix+'ScalarUp').className = 'UpDownButton_dis';
                document.getElementById(MotorPrefix+'ScalarDown').className = 'UpDownButton_dis';
            }                              

            if (varID === MotorPrefix+'current_ctrl')   
            {
                ctrlRegister = 2;
                document.getElementById(MotorPrefix+'current_ctrl_Input1').value = 0;
                document.getElementById(MotorPrefix+'current_ctrl_Input2').value = 0;
                document.getElementById(MotorPrefix+'ScalarUp').disabled = true;                                            
                document.getElementById(MotorPrefix+'ScalarDown').disabled = true;
                
                document.getElementById(MotorPrefix+'ScalarUp').className = 'UpDownButton_dis';
                document.getElementById(MotorPrefix+'ScalarDown').className = 'UpDownButton_dis';              
            }

            if (varID === MotorPrefix+'speed_ctrl')     
            {
                ctrlRegister = 3;
                document.getElementById(MotorPrefix+'ScalarUp').disabled = true;                                            
                document.getElementById(MotorPrefix+'ScalarDown').disabled = true;
                
                document.getElementById(MotorPrefix+'ScalarUp').className = 'UpDownButton_dis';
                document.getElementById(MotorPrefix+'ScalarDown').className = 'UpDownButton_dis';             
            }

            if (varID === MotorPrefix+'pos_ctrl')     
            {
                var activeSensor = document.getElementById(MotorPrefix +'pospe_ctrl_Select').selectedIndex;
                
                if(activeSensor ===1)
                {
                    ctrlRegister = 4;
                    document.getElementById(MotorPrefix+'ScalarUp').disabled = true;                                            
                    document.getElementById(MotorPrefix+'ScalarDown').disabled = true;
                
                    document.getElementById(MotorPrefix+'ScalarUp').className = 'UpDownButton_dis';
                    document.getElementById(MotorPrefix+'ScalarDown').className = 'UpDownButton_dis';
                }
                else
                {
                    alert("The position control requires enabled encoder!"+ '\n' + "Choose correct position sensor below."); 
                    ctrlRegister = 3;
                }
            }            

            // write to FRM   
            CascadeMethods(ctrlRegister, 'Enable');
            if((ctrlRegister!==0)&(ctrlRegister!==4)) 
                CascadeMethods(5, 'Enable');
            
            textControlType = conversionNo2Ctrl(ctrlRegister);                     
            document.getElementById(MotorPrefix + textControlType +'_Input1').disabled = false;    
            if(document.getElementById(MotorPrefix + textControlType +'_Input2'))
                document.getElementById(MotorPrefix + textControlType +'_Input2').disabled = false;

            succ = pcm.WriteVariable(CtrlRegisterVal.childNodes[0].nodeValue, ctrlRegister, retMsg);
       }
    }
}

/***************************************************************************//*!
*
* @brief   State machine engine
* @param   None
* @return  None
* @remarks 
******************************************************************************/ 
function StateMachine(elementId)
{
    if(pcm.ReadVariable(elementId))
    {
        var states      = xmlDoc.getElementsByTagName(MotorPrefix+"state"); 
        var ValueState  = pcm.LastVariable_vValue;
        var x           = document.getElementById('textApplicationState');
        
        x.value =  states[ValueState].childNodes[0].nodeValue;  

        if(x.value === 'FAULT' || x.value === 'Fault') // If Fault state
        {  
            var object = document.getElementById('textApplicationState');
            var object2 = document.getElementById('appStateLabel');

            object.className = 'appStateBox_Button';
            object.disabled  = false;
          
            object2.className = 'appStateLabel_red';
            object2.innerHTML = 'Clear FAULT';
        }
        else
        {
            var object = document.getElementById('textApplicationState');
            var object2 = document.getElementById('appStateLabel');
          
            object.className = 'appStateBox';
            object.disabled  = true;
          
            object2.className = 'appStateLabel';
            object2.innerHTML = 'Application State';
        }
    }
}

/***************************************************************************//*!
* @brief   Clear fault
* @param   None
* @return  None
* @remarks
******************************************************************************/
function ClearFault()
{
    var retMsg;

    var fltClr = xmlDoc.getElementsByTagName([MotorPrefix]+"FltClr")[0];
    
    if(pcm.ReadVariable(fltClr.childNodes[0].nodeValue))
    {
        succ = pcm.WriteVariable(fltClr.childNodes[0].nodeValue, 1, retMsg);
    }

}
     
/***************************************************************************//*!
* @brief   chose the image according to selected method
* @param   None
* @return  None
* @remarks               document.getElementById('scalar_ctrl_Input1').value
******************************************************************************/ 
function clickCntStrImage(varID){
    switch(varID)
    {
    case (MotorPrefix+"scalar_ctrl"):
        setImageDimensions('images/SCcontrol_n.png',640,300);
        break;
    case (MotorPrefix+"volt_ctrl"):
        setImageDimensions('images/Vcontrol.png',430,295);
        break;
    case (MotorPrefix+"current_ctrl"):
        setImageDimensions('images/Ccontrol.png',590,400);
        break;
    case (MotorPrefix+"speed_ctrl"):
        setImageDimensions('images/Wcontrol.png',765,400);
        break;
    case (MotorPrefix+"position_ctrl"):
        setImageDimensions('images/Pcontrol.png',765,400);
        break;    
    default:
    }
}  

/***************************************************************************//*!
* @brief   Sets the size of iframe window according to displayed image
* @param   None
* @return  None
* @remarks 
******************************************************************************/ 
function setImageDimensions(imPath, inW, inH)
{
    var object  = document.getElementById('cntrStrucFrameImage');
    var object2 = document.getElementById('cntrStrucFrameButton');
      
    var buttonUP   = document.getElementById(MotorPrefix+'ScalarUp');
    var buttonDOWN = document.getElementById(MotorPrefix+'ScalarDown');
      
    object.src            = imPath;
      
    object.style.width    = inW.toString()+"px";
    object.style.height   = inH.toString()+"px";
    
    object.style.top      = "40px";
    object.style.left     = ((800-inW)/2).toString()+"px";
      
    object2.style.width   = "120px";
    object2.style.height  = "25px";
            
    object2.style.top     = (40 + inH - 30).toString()+"px";
    object2.style.left    = ((800-120)/2).toString()+"px";
      
    object.style.display = "block";
    object2.style.display = "block";
    
    buttonUP.style.visibility = "hidden";
    buttonDOWN.style.visibility = "hidden";    

    object2.src = 'form_buttonCloseWindow.html';      
}   

/***************************************************************************//*!
* @brief   Onclick button event - Update FRM values
* @param   None
* @return  None
* @remarks 
******************************************************************************/
function clickUpdateCtrlStruc()
{
    var retMsg;
    var writeFRM  = new Array();
    var writeFRMoffset = 0;
    
    MotorPrefix = getActiveMotor();
    var MethodCtrlVal = xmlDoc.getElementsByTagName([MotorPrefix]+"ControlStructureMethod")[0];
     
    
    if(pcm.ReadVariable(MethodCtrlVal.childNodes[0].nodeValue))
    {
        var in_MethodCtrlVal = pcm.LastVariable_vValue;

        if (in_MethodCtrlVal === 0)
        {
            var Nmax          = getParentHtmlValue("N_max");
            var UDCmax        = getParentHtmlValue("UDC_max");
            var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
            var Uphnom        = getParentHtmlValue("U_ph");
            var N_nom         = getParentHtmlValue("N_req");
            var Motor_pp      = getParentHtmlValue("pp");
            var Umax          = Math.round(UDCmax/UmaxCoeff*10)/10;
            var Wmax          = 2*Math.PI*Motor_pp*Nmax/60;
            
            var aritType      = parent.document.getElementById("Arithmetic").innerText;

            /* select k_factor value according to control mode */
            if(getActiveMode()===0)
                var k_factor = 100; // basic mode
            else
                var k_factor = (document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value); // expert mode

            /* limit k_factor to range */
            if(k_factor>k_factor_max)
            {
                k_factor=k_factor_max;
                document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value = k_factor_max;
            }
            if(k_factor<k_factor_min)
            {
                k_factor=k_factor_min;
                document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value = k_factor_min;
            }

            /* write k_factor value to internal table */
            parent.document.getElementById(MotorPrefix+'SCALAR_Factor').innerHTML = k_factor;

            //Float value
            k_rate_float    = Uphnom*k_factor/100/(N_nom*Motor_pp/60);

            // Scaled value
            k_rate_gain    = Uphnom*k_factor/100/(N_nom*Motor_pp*2*Math.PI/60);
            k_rate_sc      = k_rate_gain*Wmax/Umax;

            //Shift factor
            if(k_rate_sc >1)
                k_rate_shift    = Math.ceil(Math.log(Math.abs(k_rate_sc))/Math.log(2));
            else
                k_rate_shift    = 0;
            
            // Resulting fractional value
            k_rate_frac       = k_rate_sc*Math.pow(2,-k_rate_shift);
                   
            if(aritType!=='Float') 
            {
                writeFRM[0] = k_rate_frac;
                writeFRM[1] = k_rate_shift;
            }
            else // float
            {
                writeFRM[0] = k_rate_float;
            }    
            var MCATspeedReq = document.getElementById(MotorPrefix+'scalar_ctrl_Input1').value;
            
            
            if(aritType!=='Float') // fractional arithmetic
            { 
                MCATspeedReq = MCATspeedReq*2*Math.PI*Motor_pp/60/Wmax;                
                var MCATvoltReq = Umax*MCATspeedReq*k_rate_frac*Math.pow(2,k_rate_shift);
                writeFRM[2] = Number((document.getElementById(MotorPrefix+'scalar_ctrl_Input1').value));    
            }
            else // float
            {
                var MCATfreqReq = MCATspeedReq;                
                var MCATvoltReq = MCATfreqReq*k_rate_float;
                writeFRM[2] = Number(MCATfreqReq);    
            }

            document.getElementById(MotorPrefix+'scalar_ctrl_Input2_Um').value = MCATvoltReq.toFixed(3);

            var writeFRMregisterVal0 = xmlDoc.getElementsByTagName([MotorPrefix]+"SCALAR_VHZ_FACTOR_GAIN");
            var writeFRMregisterVal1 = xmlDoc.getElementsByTagName([MotorPrefix]+"SCALAR_VHZ_FACTOR_SHIFT");
            var writeFRMregisterVal2 = xmlDoc.getElementsByTagName([MotorPrefix]+"Freq_req"); 
        }
        else
         document.getElementById(MotorPrefix+'scalar_ctrl_Input2_Um').value = 0;
        
        if (in_MethodCtrlVal === 1){
            writeFRM[0] = document.getElementById(MotorPrefix+'volt_ctrl_Input1').value;
            writeFRM[1] = document.getElementById(MotorPrefix+'volt_ctrl_Input2').value;
             
            var writeFRMregisterVal0 = xmlDoc.getElementsByTagName([MotorPrefix]+"Ud_req");
            var writeFRMregisterVal1 = xmlDoc.getElementsByTagName([MotorPrefix]+"Uq_req");
        }
        
        if (in_MethodCtrlVal === 2){
            writeFRM[0] = document.getElementById(MotorPrefix+'current_ctrl_Input1').value;
            writeFRM[1] = document.getElementById(MotorPrefix+'current_ctrl_Input2').value;
             
            var writeFRMregisterVal0 = xmlDoc.getElementsByTagName([MotorPrefix]+"Id_req");
            var writeFRMregisterVal1 = xmlDoc.getElementsByTagName([MotorPrefix]+"Iq_req");
        }
        
        if (in_MethodCtrlVal === 3){
            writeFRM[0] = document.getElementById(MotorPrefix+'speed_ctrl_Input1').value;
            var writeFRMregisterVal0 = xmlDoc.getElementsByTagName([MotorPrefix]+"Speed_req");
        }
        
        if (in_MethodCtrlVal === 4){
            writeFRM[0] = document.getElementById(MotorPrefix+'pos_ctrl_Input1').value;
            var writeFRMregisterVal0 = xmlDoc.getElementsByTagName([MotorPrefix]+"Pos_req");
        }
        
        if (in_MethodCtrlVal<3)
        {
            succ = pcm.WriteVariable(writeFRMregisterVal0[0].childNodes[0].nodeValue, Number(writeFRM[0]), retMsg); 
            if(aritType!=='Float')
                succ = pcm.WriteVariable(writeFRMregisterVal1[0].childNodes[0].nodeValue, Number(writeFRM[1]), retMsg);
            if(in_MethodCtrlVal=== 0)
            {   
                succ = pcm.WriteVariable(writeFRMregisterVal2[0].childNodes[0].nodeValue, Number(writeFRM[2]), retMsg);
            }
        }
        else if (in_MethodCtrlVal==3)
        {
            succ = pcm.WriteVariable(writeFRMregisterVal0[0].childNodes[0].nodeValue, Number(writeFRM[0]), retMsg);
        }   
        else
            succ = pcm.WriteVariable(writeFRMregisterVal0[0].childNodes[0].nodeValue, Number(writeFRM[0]), retMsg);
             
      }  
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write
*         to output HTML form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeCascadeHTMLOutput(prefix,xmlObject)
{ 
    var Nmax          = getParentHtmlValue("N_max");
    var UDCmax        = getParentHtmlValue("UDC_max");
    var UmaxCoeff     = getParentHtmlValue("UmaxCoeff");
    var Uphnom        = getParentHtmlValue("U_ph");
    var N_nom         = getParentHtmlValue("N_req");
    var Motor_pp      = getParentHtmlValue("pp");
    var CLOOP_Ts      = getParentHtmlValue("CLOOP_Ts");
    var SLOOP_Ts      = getParentHtmlValue("SLOOP_Ts");
    var RAMP_Up       = getParentHtmlValue("RAMP_UP");
    var RAMP_Down     = getParentHtmlValue("RAMP_DOWN");
    var PP            = getParentHtmlValue("pp");
    var Wmax          = 2*Math.PI*PP*Nmax/60;
    var Umax          = Math.round(UDCmax/UmaxCoeff*10)/10;

    MotorPrefix = getActiveMotor();
  
    // Control Structure module - scalar control part
    document.write(HTML_write_blank_line());
    document.write(HTML_write_comment_line("Control Structure Module - Scalar Control","",""));
    document.write(HTML_write_comment_line_dash());

    k_factor          = (parent.document.getElementById(MotorPrefix+'SCALAR_Factor').innerHTML);

    //Float value
    k_rate_float    = Uphnom*k_factor/100/(N_nom*Motor_pp/60);

    // Scaled value
    k_rate_gain    = Uphnom*k_factor/100/(N_nom*Motor_pp*2*Math.PI/60);
    k_rate_sc      = k_rate_gain*Wmax/Umax;

    if(k_rate_sc ===1)  k_rate_sc = k_rate_sc + 0.0000001;

    //Shift factor
    if(k_rate_sc >1)
        k_rate_shift    = Math.ceil(Math.log(Math.abs(k_rate_sc))/Math.log(2));
    else
        k_rate_shift    = 0;

    // Resulting fractional value
    k_rate_frac       = k_rate_sc*Math.pow(2,-k_rate_shift);

    /* intergration constant for scalar position calculation */
    Kint_f = Math.round(2*Math.PI*Motor_pp*Nmax/60*CLOOP_Ts/Math.PI*1000000000000)/1000000000000;

    // scalar speed ramp increments
    ScalarRampIncUp_float = Math.round(RAMP_Up*CLOOP_Ts/60*PP*1000000000000)/1000000000000;;
    ScalarRampIncDown_float = Math.round(RAMP_Down*CLOOP_Ts/60*PP*1000000000000)/1000000000000;;
    ScalarRampIncUp =  Math.round(RAMP_Up/60*PP*2*Math.PI/Wmax*CLOOP_Ts*1000000000000)/1000000000000;
    ScalarRampIncDown =  Math.round(RAMP_Down/60*PP*2*Math.PI/Wmax*CLOOP_Ts*1000000000000)/1000000000000;
    testFracValRange("ScalarRampIncUp",ScalarRampIncUp,1);
    testFracValRange("ScalarRampIncDown",ScalarRampIncDown,1);

    document.write(HTML_write_define_line_number(prefix,0,"SCALAR_VHZ_FACTOR_GAIN",xmlObject));
    document.write(HTML_write_define_line_number(prefix,1,"SCALAR_VHZ_FACTOR_SHIFT",xmlObject));
    setInnerHtmlValueAsText("SCALAR_VHZ_FACTOR_GAIN",0,k_rate_frac,k_rate_float);
    setInnerHtmlValueAsText("SCALAR_VHZ_FACTOR_SHIFT",1,k_rate_shift,'N/A');

    document.write(HTML_write_define_line_number(prefix,0,"SCALAR_INTEG_GAIN",xmlObject));
    setInnerHtmlValueAsText("SCALAR_INTEG_GAIN",10,Kint_f,Kint_f);

    /* scalar ramp increments */
    document.write(HTML_write_define_line_number(prefix,0,"SCALAR_RAMP_UP",xmlObject));
    document.write(HTML_write_define_line_number(prefix,0,"SCALAR_RAMP_DOWN",xmlObject));
    setInnerHtmlValueAsText("SCALAR_RAMP_UP",0,ScalarRampIncUp,ScalarRampIncUp_float);
    setInnerHtmlValueAsText("SCALAR_RAMP_DOWN",0,ScalarRampIncDown,ScalarRampIncDown_float);
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function writeCascadeHeaderOutput(prefix, str)
{
   str = write_blank_lines(str,1);     
   str = write_comment_text(str,'Control Structure Module - Scalar Control','');
   str = write_comment_line_dash(str);
 
   var aritType     = parent.document.getElementById("Arithmetic").innerText;
   
   // Cascade module
   str = write_define_line_number(prefix, str,'SCALAR_VHZ_FACTOR_GAIN');
   str = write_define_line_number(prefix, str,'SCALAR_VHZ_FACTOR_SHIFT');
   str = write_define_line_number(prefix, str,'SCALAR_INTEG_GAIN');
   str = write_define_line_number(prefix, str,'SCALAR_RAMP_UP');
   str = write_define_line_number(prefix, str,'SCALAR_RAMP_DOWN');
        
   return str;
}

/***************************************************************************//*!
* @brief  The function reads values from input forms, scales them and write 
*         to output file form
* @param   
* @return 
* @remarks 
******************************************************************************/
function clickScalarUpDown(varID)
{
    var IncDec        = 1;
    var k_factor      = Number(document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value);
                
    // Up/Down buttons enabled only in expert mode
    if(getActiveMode()!==0)            
    {
        switch(varID)
        {
         case MotorPrefix+"ScalarUp":
          if (k_factor<k_factor_max) {
            k_factor =  k_factor + IncDec;
            parent.document.getElementById(MotorPrefix+'SCALAR_Factor').innerHTML = k_factor;
            document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value = k_factor;

           }
          break;       
         case MotorPrefix+"ScalarDown":
         if(k_factor>k_factor_min){
            k_factor =  k_factor - IncDec;
            parent.document.getElementById(MotorPrefix+'SCALAR_Factor').innerHTML = k_factor;
            document.getElementById(MotorPrefix+'scalar_ctrl_Input2').value = k_factor;
            }
          break;  
         default: 
        }
    
       clickUpdateCtrlStruc();
    }
}

/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
    
