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
* @file   config.js
*
* @brief  MCAT tool main configuration file
*
******************************************************************************/

/******************************************************************************
* List of functions
******************************************************************************
* innnerHTMLonLoad() - initializes inner HTML page onLoad event
* selectMotor(motorNumber)  - select new active motor according to motorNumber
* getActiveMotor() - return active motor
* marChangedInput(varID) - change color of modified input form
* checkChangedVars(param) - find all inputs with change background color
* setActivePage() - set new active HTML module page
* build_multi_motor_selector (...) - creates multiply motor switch on the main page
* build_parameter_line(....) - add HTML code of new parameter input
* build_divider_line(title) - add a seperation line with optional title
* build_constant_line(...)  - add HTML code of new constant input
* build_control_structure_line(...) - add HTML code of new control line  in CtrlStructurePage.html
* checkIt(id) - check input characters put into Inputs
* checkItCS(id) - check input characters put into Inputs - Control structure
* calc_WE_max(state) - call or close Form of speed calcultor
* calc_ke(state) - call or close Form of BEMF constant calculator
* 
*******************************************************************************/

var tabHTMLFiles = [
        "", // Introduction
        "inner_MID.html", // Measurement
        "inner_Parameters.html", // Parameters
        "inner_CLoop.html", // Current Loop
        "inner_SLoop.html",  // Speed Loop
        "inner_PoSpeSensor.html", // PoSpes Sensors
        "inner_PoSpeBemfDQ.html", // Sensorless
        "inner_CtrlStructure.html",  // Cascade
        "inner_OutputHeader.html",   // Output File
        ""    // App Control
      ];
/***************************************************************************//*!
*
* @brief   The function initializes inner HTML page onLoad event
* @param   
* @return  None
* @remarks 
******************************************************************************/
function innerHTMLonload()
{
    var motorSelected = getActiveMotor();
    var checkBoxModeObj = document.getElementById("checkBoxMode");
}

/***************************************************************************//*!
*
* @brief   The function selects active motor
* @param   motorNumber - active motor number
* @return  None
* @remarks 
******************************************************************************/
function selectMotor(motorNumber)
{
  var activePage  = document.getElementById('mainFrame');
  
  var radioMotor1Obj = document.getElementById("radioMotor1");
  var radioMotor2Obj = document.getElementById("radioMotor2");
  var radioMotor3Obj = document.getElementById("radioMotor3");
  
  var tabMotor1Obj = document.getElementById("tabMotor1");
  var tabMotor2Obj = document.getElementById("tabMotor2");
  var tabMotor3Obj = document.getElementById("tabMotor3"); 
  
  var menuObj = document.getElementById("menu");
  var liObject = parent.document.getElementById('tabMotor'+[motorNumber]).getElementsByTagName('a');
  
  for (var i = 0; i < liObject.length; ++i)
  	{
 	    if (0 == i)   liObject[i].className = "active";
 	    else          liObject[i].className = "";
 	  }         
    
  var valDec = new Array(4);
   valDec[1] = 0;
   valDec[2] = 0;
   valDec[3] = 0;
    
   var liObject         = document.getElementById('tabMotor'+[motorNumber]).getElementsByTagName('a');
   valDec[motorNumber]  = parent.document.getElementById('Tab'+[motorNumber]+'manager').innerHTML ;
        
   for(var i=0;i<9;i++)
    {
      if(((valDec[motorNumber])>>>i)&1)  liObject[i].style.display = '';
      else                               liObject[i].style.display = 'none';
    }  

    //motor 1
    if(motorNumber==1)
    { 
      activePage.src = defaultPageMotor1;

      tabMotor1Obj.style.display='';
      tabMotor2Obj.style.display='none';  
      tabMotor3Obj.style.display='none';      
      menuObj.style.backgroundColor='rgb(53,58,63)';
      
      if(!radioMotor1Obj.checked)
      {  
        radioMotor1Obj.checked = true;
        radioMotor2Obj.checked = false; 
        radioMotor3Obj.checked = false;
      }
     }  
    
     // motor 2
    if(motorNumber==2)  
    {  
      activePage.src = defaultPageMotor2;
      
      tabMotor1Obj.style.display='none';
      tabMotor2Obj.style.display='';  
      tabMotor3Obj.style.display='none';  
      menuObj.style.backgroundColor='rgb(26,75,92)';
      
      if(!radioMotor2Obj.checked)
      {  
        radioMotor1Obj.checked = false;
        radioMotor2Obj.checked = true; 
        radioMotor3Obj.checked = false;
      }
     }
     
     // motor 3
    if(motorNumber==3)  
    {  
      activePage.src = defaultPageMotor3;
      
      tabMotor1Obj.style.display='none';
      tabMotor2Obj.style.display='none';  
      tabMotor3Obj.style.display='';  
      //menuObj.style.backgroundColor='rgb(79,120,24)';
      menuObj.style.backgroundColor='rgb(53,80,16)';
      
      if(!radioMotor3Obj.checked)
      {  
        radioMotor1Obj.checked = false;
        radioMotor2Obj.checked = false; 
        radioMotor3Obj.checked = true;
      }
     }
  // }

      liObject[9].style.backgroundColor = liObject[1].style.backgroundColor;
 } 
 
/***************************************************************************//*!
*
* @brief   The function return number of active motor - disabled for
* @param   
* @return  motor number
* @remarks 
******************************************************************************/
function getActiveMotor()
{
    var object1     = null;
    var object2     = null;
    var object3     = null;
    
    object1 = parent.document.getElementById("radioMotor1");
    //object2 = parent.document.getElementById("radioMotor2");
    //object3 = parent.document.getElementById("radioMotor3");
    
    
    if(object1)
    {  
      if(object1.checked)
      {
        return 'M1_';
      }
      else if(object2.checked)
      {
        return 'M2_';
      }
      else if(object3.checked)
      {
        return 'M3_';
      }  
    }
    else
    {
      return '';
    }
}

/***************************************************************************//*!
*
* @brief   The function return number of active motor
* @param   
* @return  motor number
* @remarks 
******************************************************************************/
function getActiveMode()
{
    if(parent.document.getElementById("idTunningMode").value==1)
      return 1;
    else
      return 0;  
}

/***************************************************************************//*!
*
* @brief   The function return number of active motor
* @param   
* @return  motor number
* @remarks 
******************************************************************************/
function setActiveMode()
{
    var object = null;
    var activePage = parent.document.getElementById('mainFrame');
    var activeMode = parent.document.getElementById("idTunningMode").value;
    var motorTab = getActiveMotor();

    if (motorTab=="M1_")
        motorTab = "tabMotor1";         
    
    if (motorTab=="M2_")
        motorTab = "tabMotor2";
            
    if (motorTab=="M3_")
        motorTab = "tabMotor3";
    
    objectMode = parent.document.getElementById("Mode");

    if(objectMode)
    {
        if(activeMode==1)
            objectMode.innerHTML='Expert';
        else
            objectMode.innerHTML='Basic';

        // store Setting
        paramFileWriteData("Setting_");

        //  change active menu item
        var liObject = parent.document.getElementById(motorTab).getElementsByTagName('a');
        for (var i = 0; i < liObject.length; ++i)
        {
            if (liObject[i].className == "active")
            if (tabHTMLFiles[i]!="")
            {
                activePage.src = tabHTMLFiles[i];
            }
        }
    }

}
 
/***************************************************************************//*!
*
* @brief   The function mark background color of input elemements where value 
*          were changed
* @param   formName - name of Form section on active control page
*          varLabel - input box where changed was done
* @return  None
* @remarks 
******************************************************************************/
function markChangedInput(varID)
{
    var object     = null;
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();

    // add prefix to var ID
    if(prefixM!='')
      var paramTableID = 'paramTable'+prefixM;
    
    object      = document.getElementById(varID);
    
    // change color only of editable parmateters
    if(!(object.readOnly))
    { 
      if(document.getElementById("StoreData") != undefined)
      {
        document.getElementById("StoreData").disabled = false;
        document.getElementById("StoreData").className = "fButtonsEnabled";
        }
        
      if(document.getElementById("ReloadData") != undefined)
      {
        document.getElementById("ReloadData").disabled = false;  
        document.getElementById("ReloadData").className = "fButtonsEnabled";  
        }
        
      // store new value to main tab
      parent.document.getElementById(varID).innerHTML = object.value;
        
      // change input form bacground to see that value was changed
      object.style.background = 'rgb(250,183,153)';
      parent.document.getElementById(varID).style.color="red"; 
      // change color of paramter tab DIV to reflect modified input
      parent.document.getElementById(paramTableID).style.color="red";
    } 

    // update active tab constants
    updateTab();
} 

/***************************************************************************//*!
*
* @brief   The function set new control page in defined iFrame
* @param   pageName - name of required HTML to be shown
*          iFrame - iFrame name
* @return  None
* @remarks 
******************************************************************************/
function setActivePage(tabMotorId, pageName,liNumber) 
{
    var activePage = document.getElementById('mainFrame');
    var status = false;
    
    // set new page if it's allowed   
    activePage.src = pageName;
    
    //  change active menu item
    if(liNumber < 10){
      var liObject = parent.document.getElementById(tabMotorId).getElementsByTagName('a');
      for (var i = 0; i < liObject.length; ++i)
    	{
  	    if (liNumber === i)
  		     liObject[i].className = "active";
  	    else
  		     liObject[i].className = "";
  	  }  
    }
}   
 
/***************************************************************************//*!
*
* @brief   The function creates multi-motor selector
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function build_multi_motor_selector(defaultChecking)
{
    
    var string = "";
    // motor 1
      string = string + "<div id =\"idMotor1Tab\" style=\"display:none; font-family: Arial; font-size: 15px; color:white; background-color: rgb(53,58,63); height: 30px; width: 250px; position: absolute; left: 0px; top:69px\">";
      string = string + "<font style=\"position: relative; left: 5%; top:7px\">App ID: </font>";
      string = string + "<font id =\"M1typeTabName\" style=\"position: relative; left: 10%; top:7px\"></font>";
      string = string + "</div>";
      // radio button to select actual motor setting
      string = string + "<div id =\"idMotor1Rad\" style=\"display:none; font-size: 15px; font-family: Arial; background-color: rgb(53,58,63); height: 30px; width: 40px; position: absolute; left: 250px; top: 69px\">";
      if(defaultChecking === 1)
        string = string + "<input type=\"radio\" id=\"radioMotor1\"  style=\"cursor:pointer; position: relative; left: 10px; top:6px;display:none;   \" onFocus=\"blur(selectMotor(1))\" value=\"1\" checked>";
      else
        string = string + "<input type=\"radio\" id=\"radioMotor1\"  style=\"cursor:pointer; position: relative; left: 20px; top:6px;display:none;  \" onFocus=\"blur(selectMotor(1))\" value=\"1\" >";
      string = string + "</div>";
    
    // motor 2 
      string = string + "<div id =\"idMotor2Tab\" style=\"display:none; font-size: 15px; font-family: Arial; color:white; background-color: rgb(26,75,92); height: 30px; width: 150px; position: absolute; left: 215px; top: 69px\">";    
      string = string + "<font style=\"position: relative; left: 15%; top:7px\">Motor 2:</font>";
      string = string + "<font id =\"M2typeTabName\" style=\"position: relative; left: 30%; top:7px\"></font>";                   
      string = string + "</div>";
      // radio button to select actual motor setting
      string = string + "<div  id =\"idMotor2Rad\" style=\"display:none; font-size: 15px; font-family: Arial; background-color: rgb(26,75,92); height: 30px; width: 50px; position: absolute; left: 365px; top: 69px\">";
      if(defaultChecking === 2)
        string = string + "<input type=\"radio\" id=\"radioMotor2\"   style=\"cursor:pointer; position: relative; left: 10px; top:6px \" onFocus=\"blur(selectMotor(2))\" value=\"2\" checked>";             
      else
        string = string + "<input type=\"radio\" id=\"radioMotor2\"   style=\"cursor:pointer; position: relative; left: 10px; top:6px \" onFocus=\"blur(selectMotor(2))\" value=\"2\" >";
      string = string + "</div>";
     
    // motor 3 
  
      string = string + "<div id =\"idMotor3Tab\" style=\"display:none; font-size: 15px; font-family: Arial; color:white; background-color: rgb(53,80,16); height: 30px; width: 150px; position: absolute; left: 430px; top: 69px\">";    
      string = string + "<font style=\"position: relative; left: 15%; top:7px\">Motor 3:</font>";
      string = string + "<font id =\"M3typeTabName\" style=\"position: relative; left: 30%;  top:7px\"></font>";                   
      string = string + "</div>";
      // radio button to select actual motor setting
      string = string + "<div id =\"idMotor3Rad\" style=\" display:none; font-size: 15px; font-family: Arial; background-color: rgb(53,80,16); height: 30px; width: 50px; position: absolute; left: 580px; top: 69px\">";
      if(defaultChecking === 3)
        string = string + "<input type=\"radio\" id=\"radioMotor3\"  style=\"cursor:pointer; position: relative; left: 10px; top:6px \" onFocus=\"blur(selectMotor(3))\" value=\"3\" checked>";             
      else
        string = string + "<input type=\"radio\" id=\"radioMotor3\"  style=\"cursor:pointer; position: relative; left: 10px; top:6px \" onFocus=\"blur(selectMotor(3))\" value=\"3\" >";
      string = string + "</div>";
      
      // tuning mode selector
      string = string + "<div id =\"idModeSelector\" style=\" display:none; font-size: 15px; font-family: Arial; color:black; background-color: rgb(195,199,204); height: 30px; width: 200px; position: absolute; left: 644px; top: 69px;\">";    
      string = string + "<font style=\" display:none; padding-left: 0.2cm; font-size: 15px; position: relative; left: 0px; top: 5px; \">Tuning Mode:</font>";
      string = string + "<select id=\"idTunningMode\"  onChange =\"setActiveMode()\"  size=\"1\" style=\" display:none; border:0px; width:85px; font-style: italic; font-size: 15px; position: relative; left: 10px; top: 6px;\">";  
      string = string + "<option value=\"0\">   Basic</option>"; 
      string = string + "<option value=\"1\">   Expert</option></select>";
      string = string + "</div>";
         
    return string;
}
  
/***************************************************************************//*!
* build_control_structure_line( varID, lineName,inputLabel1, unit1, inputComment1)
* @brief   The function creates code of input parameter
* @param   varLabel - visible label of pramater
*          varID    - HTML unique ID
*          varUnit  - input parameter Unit                                                                                                                        
*          valComment - help text which is active MouseOn event on varLabel
*          inObj - optional input object
*          inObjDescr - descritpion of optional input object
* @return  None
* @remarks 
* 
* string = string + "<td colspan=\"1\" width=\"40%\" style=\"padding-left:0.4cm\">";
    string = string + "<div class=\"fontControlLabelSet\">&nbsp" + title + " </div></td>";             
    string = string + "<td colspan=\"1\" align=\"left\" style=\"font-family: Arial; font-size: 12px;\">";
    string = string + "<div id=\""+varID+"\">&nbsp" + out_string + " </div>";
    string = string + "</td></tr>";
******************************************************************************/        
function build_parameter_line( varLabel, varID, varUnit, varComment, inObj, inObjDescr)
{
    var optionalObject =  inObj; 
  	// hidden parameter
    if(optionalObject == -1)
      var string = "<tr style=\"display:none;\">";
    else
      var string = "<tr>";  
	  var onchange = "";  
    var varIDprefix="";
    var varUnitField = "";
    
    if(varUnit!='')
      varUnitField = '[' + varUnit + ']';    
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    
    // add prefix to var ID
    if(prefixM!='')
      varIDprefix = prefixM + varID; 
  
    // parameter name + comment when mouse moves over
    string = string + "<td align=\"left\"  width=\"100px\" style=\"font-family:Arial; font-size:14px; padding-left:0.4cm\"> <label id = \"parText" + varLabel + "\" title=\"" + varComment + "\" >" + varLabel + "</label> </td>";
	 // next column
    string = string + "<td valign=\"top\" width=\"70px\">";
	 // input box with event "onchage"
    string = string + "<div style=\"float:left; width:10px; margin-bottom: 2px;\"><input type=\"text\" style=\"text-align:right; font-size: 12px; \" id=\"" + varIDprefix + "\" \n\
                        onkeydown=\"return checkEnterPress(event,id)\" onblur=\"return checkIt(id)\" onchange =\"markChangedInput('" + varIDprefix + "');  \"  name=\"" + varLabel + "\"size=\"6\" value=\"0\" ></div></td>";
	 // add param unit
    string = string + "<td valign=\"center\" width=\"40px\" style=\"font-family: Arial; font-size: 14px;\">" + varUnitField + "</td>";

    // optional input object
    // no object
    if(optionalObject == 0)
    {
      string = string + "</td><td valign=\"top\" width=\"30px\">&nbsp;</td><td>";
    }
    //checkbox
    if(optionalObject == 1)
    {
      string = string + "</td><td valign=\"top\" width=\"15%\" style=\"font-family:Arial; font-size: 12px;\"><input type=\"checkbox\" id=\"checkBox" + varIDprefix + "\" value=\"0\" >" + inObjDescr + "</td><td>";
  	}
    // button
    if(optionalObject == 2)
    {
      string = string + "</td><td valign=\"top\" width=\"15%\"><button id=\"button" + varID + " value=\"0\" onclick=\"calc_" + varID + "(1)\" >" + inObjDescr + "</td><td>";
  	}
    // radi button
    if(optionalObject == 3)
    {
      string = string + "</td><td valign=\"top\" width=\"15%\" style=\"font-family:Arial; font-size: 12px;\"><input type=\"radio\" id=\"radio" + varIDprefix + "\" value=\"1\" onclick=\"clickRadio('"+ varID + "')\" >" + inObjDescr + "</td><td>";
  	}
    // end table
    string = string + "</td></tr>";
  	return string;
}

/***************************************************************************//*!
* build_parameter_line_SL( varLabel, varID, varUnit, varComment)
* @brief   The function creates parameter line in Speed Loop tab
* @param   varLabel - visible label of pramater
*          varID    - HTML unique ID
*          varUnit  - input parameter Unit                                                                                                                        
*          valComment - help text which is active MouseOn event on varLabel
* @return  string
* @remarks 
******************************************************************************/        
function build_parameter_line_SL( varLabel, varID, varUnit, varComment)
{
    var string = "<tr>";  
	  var onchange = "";  
    var varIDprefix="";
    var varUnitField = "";
    
    if(varUnit!='')
      varUnitField = '[' + varUnit + ']';    
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    
    // add prefix to var ID
    if(prefixM!='')
      varIDprefix = prefixM + varID; 
	
	 // parameter name + comment when mouse moves over
    string = string + "<td align=\"left\" width=\"50%\" style=\"font-family:Arial; font-size:14px; padding-left:0.4cm; \"> <label id = \"parText" + varLabel + "\" title=\"" + varComment + "\" >" + varLabel + "</label> </td>";
	 // next column
    string = string + "<td valign=\"top\" width=\"30%\">";
	 // input box with event "onchage"
   string = string + "<div style=\"float:left; \"><input type=\"text\" style=\"text-align:right; font-size: 12px; \" id=\"" + varIDprefix + "\" onkeydown=\"return checkEnterPress(event,id)\" onblur=\"return checkIt(id)\" onchange =\"markChangedInput('" + varIDprefix + "');  \"  name=\"" + varLabel + "\"size=\"10\" value=\"0\" ></div></td>";
	 // add param unit
    string = string + "<td valign=\"center\" width=\"30%\" style=\"font-family: Arial; font-size: 12px;\">" + varUnitField + "</td>";
    // end table
    string = string + "</td></tr>";
  	return string;
}


/***************************************************************************//*!
*
* @brief   The function divides line with optional title
* @param   title - text shown in empty line (optional)
*          
* @return  None
* @remarks 
******************************************************************************/ 
function build_divider_line(title)
{
  	var string = "<tr>";
  	
  	string = string + "<td colspan=\"7\" align=\"center\" style=\"padding-left:0.3cm\">";
        string = string + "<div class=\"fontControlLabelSet\">&nbsp" + title + " </div>";             
        string = string + "</td></tr>";
  
  	return string;
}

/***************************************************************************//*!
*
* @brief   The function divides line with optional title
* @param   title - text shown in empty line (optional)
*          
* @return  None
* @remarks 
******************************************************************************/ 
function build_informative_line(varID, title, out_string)
{
    var string = "<tr>";
  	
    string = string + "<td colspan=\"1\" width=\"50%\" style=\"padding-left:0.4cm;\">";
    string = string + "<div class=\"fontControlLabelSet\">" + title + " </div></td>";             
    string = string + "<td colspan=\"1\" align=\"left\" style=\"font-family: Arial; font-size: 12px;\">";
    string = string + "<div id=\""+varID+"\">" + out_string + " </div>";
    string = string + "</td></tr>";
  
    return string;
}

/***************************************************************************//*!
*
* @brief   The function creates code of for output constant
* @param   varLabel - visible label of pramater
*          varID -  constant unique ID
*          valComment - help text which is active MouseOn event on varLabel
* @return  None
* @remarks 
******************************************************************************/ 
function build_constant_line( varLabel, varID, valComment)
{
	 var string = "<tr>";
	  	 
  	//string = string + "<td align=\"left\" valign=\"top\" width=1> </td>";
  	string = string + "<td align=\"left\" width=\"50%\" style=\"font-family: Arial; font-size: 14px; padding-left:0.4cm\"> <label id = \"constText" + varLabel + "\" title=\"" + valComment + "\" >" + varLabel + "</label> </td>";
      	string = string + "<td valign=\"top\" width=\"50%\">";
  	//string = string + "<div\"><input disabled type=\"text\" style=\"text-align:right; font-family: Arial; background-color:#DDDDDD; color:black; font-size: 12px;\" id=\"" + varID + "\" name=\"" + varLabel + "\"size=\"10\" value=\"0\"></div>";
        string = string + "<input disabled type=\"text\" style=\"text-align:right; font-family: Arial; background-color:#DDDDDD; color:black; font-size: 12px;\" id=\"" + varID + "\" name=\"" + varLabel + "\"size=\"10\" value=\"0\">";
  	string = string + "</td></tr>";
  	return string;
}

/***************************************************************************//*!
*
* @brief   The function creates code of for output constant with unit
* @param   varLabel - visible label of pramater
*          varID -  constant unique ID
*          valComment - help text which is active MouseOn event on varLabel
*          varUnit - variable unit
* @return  None
* @remarks 
******************************************************************************/ 
function build_constant_line_unit( varLabel, varID, valComment, varUnit)
{
    var string = "<tr>";
    var varUnitField = "";
    
    if(varUnit!='')
      varUnitField = '[' + varUnit + ']';
    //string = string + "<td align=\"left\" valign=\"top\" width=1> </td>";
    string = string + "<td align=\"left\" width=\"25px\" style=\"font-family: Arial; font-size: 14px; padding-left:0.4cm\"> <label id = \"constText" + varLabel + "\" title=\"" + valComment + "\" >" + varLabel + "</label> </td>";
    string = string + "<td valign=\"top\" width=\"70px\">";
    //string = string + "<div\"><input disabled type=\"text\" style=\"text-align:right; font-family: Arial; background-color:#DDDDDD; color:black; font-size: 12px;\" id=\"" + varID + "\" name=\"" + varLabel + "\"size=\"10\" value=\"0\"></div>";
    string = string + "<input disabled type=\"text\" style=\"text-align:right; font-family: Arial; background-color:#DDDDDD; color:black; font-size: 12px;\" id=\"" + varID + "\" name=\"" + varLabel + "\"size=\"6\" value=\"0\">";
    string = string + "<td valign=\"center\" width=\"25px\" style=\"font-family: Arial; font-size: 12px;\">" + varUnitField + "</td>";
    string = string + "</td></tr>";
    return string;
}


/***************************************************************************//*!
*
* @brief   The function creates code of control structure line
* @param   varID - ID of inut form
*          varLabel - visible label of pramater
*          inputLabel1  - label above first input form
*          inputLabel2  - label above second input form
*          valComment1 - help text which is active Mouse over event on input 1
*          valComment1 - help text which is active Mouse over event on input 2
* @return  None
* @remarks 
******************************************************************************/ 
function build_control_structure_line( varID, lineName,inputLabel1, unit1, inputLabel2, unit2, inputComment1,inputComment2)
{
    var string = "<tr>";
    var varIDprefix="";    
    
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    // add prefix to var ID
    if(prefixM!='')
      varIDprefix = prefixM + varID;    
	 
	  string = string + "<td  valign=\"center\"><div class=\"fontControlLabelS1\" style=\"padding-left:0.0cm\">" + lineName + "</div></td>";             
    string = string + "<td rowspan =\"2\" id=\"" + varIDprefix + "\" onClick=\"Ctrl_Structure_click(id)\" class=\"switch_off\">DISABLED</td>";
      
    if(inputLabel2!="")                                                                           
    {
      string = string + "<td valign=\"center\"><div class=\"fontControlLabelS\" style=\"padding-left:0.2cm\" title=\"" + inputComment1 + "\">" + inputLabel1 + "</div></td>"; 
      string = string + "<td valign=\"center\"> <input  type = \"text\", style=\" font-size:14px; text-align:right;\" maxlength=\"6\" size=\"6\" id = \"" + varIDprefix + "_Input1\" \n\
                        onkeydown=\"return checkEnterPressCtrlStruc(event,id)\" onblur=\"return checkItCS(id)\" name=\"" + inputLabel1 + "\" > </td>";
      string = string + "<td  valign=\"center\"><div class=\"fontControlLabelS\">" + unit1 + "</div></td>";
    }
    else
    {
      string = string + "<td rowspan =\"2\" valign=\"center\"><div class=\"fontControlLabelS\" style=\"padding-left:0.2cm\" title=\"" + inputComment1 + "\">" + inputLabel1 + "</div></td>"; 
      string = string + "<td rowspan =\"2\"  valign=\"center\"> <input  type = \"text\", style=\" font-size:14px; text-align:right;\" maxlength=\"6\" size=\"6\" id = \"" + varIDprefix + "_Input1\" \n\
                        onkeydown=\"return checkEnterPressCtrlStruc(event,id)\" onblur=\"return checkItCS(id)\" name=\"" + inputLabel1 + "\"  > </td>";
      string = string + "<td rowspan =\"2\"  valign=\"center\"><div class=\"fontControlLabelS\">" + unit1 + "</div></td>";
    }
     
     string = string + "</tr>";
     string = string + "<tr>";
                              
    string = string + "<td  valign=\"center\" style=\"text-align: center;\"><input type=\"button\" id=\"" + varIDprefix + "\" value=\"view\" onclick=\"clickCntStrImage(id)\" class=\"ctrlStrucView\"></td>";
     if(inputLabel2!="")
     {
        string = string + "<td  w valign=\"center\"><div class=\"fontControlLabelS\" style=\"padding-left:0.2cm\" title=\"" + inputComment2 + "\">" + inputLabel2 + "</div></td>";
        string = string + "<td  valign=\"center\"> <input  type = \"text\" style=\" font-size:14px; text-align:right;\" maxlength=\"6\" size=\"6\" id = \"" + varIDprefix + "_Input2\"  \n\
                           onkeydown=\"return checkEnterPressCtrlStruc(event,id)\" onblur=\"return checkItCS(id)\" name=\"" + inputLabel2 + "\" > </td>";
        string = string + "<td  valign=\"center\"><div class=\"fontControlLabelS\">" + unit2 + "</div></td>";
    }
     
     

    string = string + "</tr>"; 
 
  	return string;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
function build_VHz_control_structure_line( varID, lineName,inputLabel1, unit1, inputLabel2, unit2, inputComment1,inputComment2)
{
    var string = "<tr>";
    var varIDprefix="";
                  
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    // add prefix to var ID
    if(prefixM!='')
      varIDprefix = prefixM + varID; 

    var string = string + "<td rowspan =\"2\" valign=\"center\"  ><div class=\"fontControlLabelS1\" style=\"padding-left:0.0cm;\">" + lineName + "</div></td>"; 
    var string = string + "<td rowspan =\"3\" id=\"" + varIDprefix + "\" onClick=\"Ctrl_Structure_click(id)\" class=\"switch_off\">DISABLED</td>";
    var string = string + "<td  valign=\"center\"><div class=\"fontControlLabelS\" style=\"padding-left:0.2cm\" id=\"" + inputLabel1 + "\" title=\"" + inputComment1 + "\">" + inputLabel1 + "</div></td>";
    var string = string + "<td valign=\"center\"  style=\"padding: 0px; height:30px; width:100px;\"><div style=\"padding: 0px; margin:0px; display: inline;\">\n\
                          <input type=\"text\", onkeydown=\"return checkEnterPressCtrlStruc(event,id)\" onblur=\"return checkItCS(id)\" style=\"font-size:14px; text-align:right;  \n\
                           height:18px;\" maxlength=\"4\" size=\"2\" id = \"" + varIDprefix + "_Input2\" ></div>";
    var string = string + "<input type=\"button\" id=\""+prefixM+"ScalarUp\"; value=\"&#x2191\"  onclick=\"clickScalarUpDown(id)\" disabled=\"disabled\"; class=\"UpDownButton_dis\" style=\"width:12px; height:70%;\">";
    var string = string + "<input type=\"button\" id=\""+prefixM+"ScalarDown\"; value=\"&#x2193\"  onclick=\"clickScalarUpDown(id)\" disabled=\"disabled\"; class=\"UpDownButton_dis\" style=\"width:12px; height:70%;\">";
    var string = string + "</td><td><div class=\"fontControlLabelS\">" + unit1 + "</div></td>";
    var string = string + "</tr>";

    // Uq_required line
    var string = string + "<tr>"; 
    var string = string + "<td><div class=\"fontControlLabelS\" title=\"Resulting supply voltage\" style=\"padding-left:0.2cm;\">Uq_req</div></td>";
    var string = string + "<td><input disabled type=\"text\" value=\"0\" style=\"font-size:14px; text-align:right;\" maxlength=\"6\" size=\"6\" id = \"" + varIDprefix + "_Input2_Um\"></td>";
    var string = string + "<td><div class=\"fontControlLabelS\" >[V]</div></td>";
    var string = string + "</tr>"; 
                                
    var string = string + "<tr>";  
    
    var string = string + "<td valign=\"center\" style=\"text-align: center;\"><input type=\"button\" id=\"" + varIDprefix + "\" value=\"view\" onclick=\"clickCntStrImage(id)\" class=\"ctrlStrucView\"></td>";
    var string = string + "<td valign=\"center\"><div class=\"fontControlLabelS\" style=\"padding-left:0.2cm\" title=\"" + inputComment2 + "\">" + inputLabel2 + "</div></td>";    
    var string = string + "<td valign=\"center\"> <input type = \"text\" style=\"font-size:14px; text-align:right;\" maxlength=\"6\" size=\"6\" id = \"" + varIDprefix + "_Input1\"  \n\
                            onkeydown=\"return checkEnterPressCtrlStruc(event,id)\" onblur=\"return checkItCS(id)\" name=\"" + inputLabel2 + "\" ></td>";
    var string = string + "<td><div class=\"fontControlLabelS\" >" + unit2 + "</div></td>";
    var string = string + "</tr>";           
 
  	return string; 
}
/***************************************************************************//*!
*
* @brief   The function creates code of for output constant
* @param   varLabel - visible label of pramater
*          varID -  constant unique ID
*          valComment - help text which is active MouseOn event on varLabel
* @return  None
* @remarks 
******************************************************************************/ 
function build_control_structure_line_inbox_selector( varID, lineName,inputLabel1, unit1, unit2, inputComment1,inputComment2)
{
    var string = "<tr>";
    
    string = string + "<td width=\"100\" valign=\"center\"><div class=\"fontControlLabelS1\" style=\"padding-left:0.1cm;\" >" + lineName + "</div></td>";             
    string = string + "<td  rowspan =\"2\" id=\"" + varID + "\" onClick=\"Ctrl_Structure_click(id)\" class=\"switch_off\">DISABLED</td>";
  
    string = string + "<td width=\"20%\" valign=\"center\"><div class=\"fontControlLabelS\" style=\"padding-left:0.2cm\">" + inputLabel1 + "</div></td>"; 
    string = string + "<td width=\"10%\" valign=\"center\"> <input  type = \"text\", maxlength=\"6\" size=\"6\" id = \"" + varID + "_Input1\" onkeydown=\"return checkItCS(event,id)\" title=\"" + inputComment1 + "\"> </td>";
    string = string + "<td width=\"5%\" valign=\"center\"><div class=\"fontControlLabelS\">" + unit1 + "</div></td>";
    string = string + "</tr>";

    string = string + "<tr>";
    string = string + "<td width=\"10%\" valign=\"center\" style=\"text-align: center;\"><input type=\"button\" value=\"view\" onclick=\"clickCntStrImage("+varID+")\" class=\"fontControlLabelS\" style=\"padding-left:0.3cm;\"></td>";
    string = string + "<td width=\"10%\" valign=\"center\"><select id=\"" + varID + "_Select\" onChange =\"IdMethodChange(this.value)\" size=\"1\">"; 
    string = string + "<option value=\"0\">Id = manual</option>";
    string = string + "<option value=\"1\">Id = MTPA</option>";
    string = string + "<option value=\"2\">Id = FW</option></select> </td>";
    
    string = string + "<td  valign=\"center\"> <input  type = \"text\" maxlength=\"6\" size=\"6\" value = \"0\" id = \"" + varID + "_Input2\"  onkeydown=\"return checkItCS(event,id)\" title=\"" + inputComment2 + "\"> </td>";
    string = string + "<td width=\"10\" valign=\"center\"><div class=\"fontControlLabelS\" >" + unit2 + "</div></td>";
    string = string + "</tr>"; 
 
  	return string;
}

/***************************************************************************//*!
*
* @brief   The function creates code of for output constant
* @param   varLabel - visible label of pramater
*          varID -  constant unique ID
*          valComment - help text which is active MouseOn event on varLabel
* @return  None                                                                 
* @remarks 
******************************************************************************/ 
function build_control_structure_line_with_selector( varID, lineName,inputLabel1, unit1, inputComment1)
{
    var string = "<tr style=\"display:;\">";
    
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    // add prefix to var ID
    if(prefixM!='')
      varIDprefix = prefixM + varID;     
	 
	  string = string + "<td width=\"100\"  valign=\"center\"><div class=\"fontControlLabelS1\" style=\"padding-left:0.1cm\">" + lineName + "</div></td>";             
    string = string + "<td id=\"" + varIDprefix + "\" class=\"switch_off\" >DISABLED</td>";
  
    string = string + "<td valign=\"center\"><div class=\"fontControlLabelS\" style=\"padding-left:0.2cm\" title=\"" + inputComment1 + "\">" + inputLabel1 + "</div></td>"; 
    string = string + "<td valign=\"center\" ><select  style=\"font-size:14px\" id=\"" + varIDprefix + "_Select\" onChange =\"sensorTypeChange(this.value)\" size=\"1\">"; 
    string = string + "<option value=\"0\">sensorless</option>";
    string = string + "<option value=\"1\">encoder</option>";
    //string = string + "<option value=\"2\">resolver</option>";
    //string = string + "<option value=\"3\">hall</option>";
    //string = string + "<option value=\"4\">sincos</option></select></td>";
    string = string + "<td valign=\"center\"><div class=\"fontControlLabelS\">" + unit1 + "</div></td>";

    string = string + "</tr>"; 
 
  	return string;
} 

/***************************************************************************//*!
*
* @brief   The function creates code of for output constant
* @param   varLabel - visible label of pramater
*          varID -  constant unique ID
*          valComment - help text which is active MouseOn event on varLabel
* @return  None                                                                 
* @remarks 
******************************************************************************/ 
function build_line_with_selector(varLabel, varID,varComment)
{
    var string = "<tr>";
    
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    // add prefix to var ID
    if(prefixM!='')
      varIDprefix = prefixM + varID;     
	 
    // parameter name + comment when mouse moves over
    string = string + "<td align=\"left\" valign=\"top\" style=\"font-family:Arial; font-size:14px; padding-left:0.4cm\"> <label id = \"parText" + varLabel + "\" title=\"" + varComment + "\" >" + varLabel + "</label> </td>";
 	  // next column
    //string = string + "<td valign=\"top\" width=25%>";
    // input box with event "onchage"
    
    string = string + "<td  width=\"58%\"  valign=\"top\" style=\"overflow: visible; position: relative;\"><div style=\"margin-right:-100px; width:40px; overflow: visible; position: relative;\"><select style=\"width: 140px; margin: 0px;\" id=\"" + varIDprefix + "\" onChange =\"HWboardSelector(this.value);\" size=\"1\">"; 
    if(getBoardType() == "HVP")
    {
      string = string + "<option value=\"0\">NXP HVP-MC3PH</option>";
      string = string + "<option value=\"1\">User HW</option></select></div></td><td colspan=\"3\"></td>";
    }
    else if(getBoardType() == "TWR")
    {
      string = string + "<option value=\"0\">NXP TWR-LV3PH</option>";
      string = string + "<option value=\"1\">User HW</option></select></div></td><td colspan=\"3\"></td>";
    }
    else if(getBoardType() == "FRDM")
    {
      string = string + "<option value=\"0\">NXP FRDM-LV-MCPMSM</option>"; 
      string = string + "<option value=\"1\">User HW</option></select></div></td><td colspan=\"3\"></td>";
    }
    else if(getBoardType() == "EVK")
    {
      string = string + "<option value=\"0\">NXP FRDM-LV-MCPMSM</option>"; 
      string = string + "<option value=\"1\">User HW</option></select></div></td><td colspan=\"3\"></td>";
    }
    else
      string = string + "<option value=\"0\">User HW</option></select></div></td><td colspan=\"3\"></td>";
    
        
    // end table
    string = string + "</tr>";
  	return string;
    
}  

/***************************************************************************//*!
*
* @brief   The function checks input characters to forms. Enables only numbers
* @param   Event
* @return  true - if number was put
*          false - different character was put
* @remarks 
******************************************************************************/
function checkIt(id) {
    object = document.getElementById(id);
  
    if(isNaN(object.value) || object.value==='') 
    {
        alert("The field '" + object.name + "' accepts numbers only!");
        status = "This field accepts numbers only.";
        //object.style.background = "red";
        object.value = '';
        object.focus();
        return false  
    }
    else
    {
        status = "";
        return true
    }
}       

/***************************************************************************//*!
*
* @brief   The function checks input characters to forms. Enables only numbers
* @param   Event
* @return  true - if number was put
*          false - different character was put
* @remarks 
******************************************************************************/
function checkItCS(id) {      
    object = document.getElementById(id);
           
    if(isNaN(object.value) || object.value==='') 
    {
        alert("The field '" + object.name + "' accepts numbers only!");
        status = "This field accepts numbers only.";
        object.value = '';
        object.focus();
        return false  
    }
    else
    {
        status = "";
        object.blur();
        return true
    }
}

/***************************************************************************//*!
*
* @brief   The function checks pressing of ENTER
* @param   Event
* @return  true - enter was pressed
*          false - different key was pressed
* @remarks 
******************************************************************************/
function checkEnterPress(evt,id) {
    // check enter pressing
    evt = (evt) ? evt : window.event
    var charCode = (evt.which) ? evt.which : evt.keyCode 
    if(charCode==13)
    {   
        object = document.getElementById(id);
        if(isNaN(object.value)) 
        {
          object.blur();
        }
        else
        {
          markChangedInput(id);
          return true
        }
    }
}    

function checkEnterPressCtrlStruc(evt,id) {
    // check enter pressing
    evt = (evt) ? evt : window.event
    var charCode = (evt.which) ? evt.which : evt.keyCode 
    if(charCode==13)
    {   
        object = document.getElementById(id);
        if(isNaN(object.value)) 
        {
          object.blur();          
        }
        else
        {
          object.blur();
          clickUpdateCtrlStruc();
          return true
        }
    }
}
/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/
