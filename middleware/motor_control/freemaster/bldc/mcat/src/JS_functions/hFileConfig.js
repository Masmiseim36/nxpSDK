/*******************************************************************************
*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
****************************************************************************//*!
*
* @file   hFileConfig.js
*
* @brief  Form and generate output constant header file
*
******************************************************************************/

/******************************************************************************
* List of functions
********************************************************************************
*
* MounthConverter(in_getMounthParam)
* HTML_write_head_line(varName, varcomment)
* HTML_write_comment_line(varComment, varValue, valUnit)
* HTML_write_define_line_number(prefix,typeAtr,varName, xmlObject)
* HTML_write_comment_line_dash()
* HTML_write_blank_line()
* copyParent2HeaderCfgById(outputValueId,valueId, postfix, comment)
* copyParent2HeaderCfgByIdScaled(outputValueId,valueId, scaleId)
* calcFmScaleVariable(valueId, parentID)
* write_comment_line_star(string)
* write_comment_line_dash(string)
* write_blank_lines(string, No)
* testActiveTab(tabName)
* write_comment_text(string,text, varId)
* write_define_line_number(string,varId)
* HTML_write_output_blocks(blockName)
* Save_h_click()
*******************************************************************************/

function OutFileInit()
{
   // read the status which active motor tab is selected
   var prefixM      = getActiveMotor();

   // read the main background table and update the File Name status in the outFile tab
   document.getElementById(prefixM+"configFileName").innerHTML = parent.document.getElementById(prefixM+"configFileName").innerHTML;
   document.getElementById(prefixM+"fileName").value =  parent.document.getElementById(prefixM+"configFileName").innerHTML;
}

/***************************************************************************//*!
*
* @brief   The function converts 
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function MounthConverter(in_getMounthParam)
  {
  var mounthArray=new Array(12);
  mounthArray[0]="January";
  mounthArray[1]="February";
  mounthArray[2]="March";
  mounthArray[3]="April";
  mounthArray[4]="May";
  mounthArray[5]="June";
  mounthArray[6]="July";
  mounthArray[7]="August";
  mounthArray[8]="September";
  mounthArray[9]="October";
  mounthArray[10]="November";
  mounthArray[11]="December";

  return (mounthArray[in_getMounthParam]);
  }
 
/***************************************************************************//*!
*
* @brief   The function creates comment with a value
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function HTML_write_head_line(varName, varcomment)
  {
   var string = "<tr>";
   string = string + "<td align=\"left\" style=\" width:150px; font-weight:bold; font-size:13px; font-family:verdana;\">" + varName + "</td>";
   string = string + "<td align=\"left\" style=\" font-style:italic; font-size:13px; font-family:verdana;\" >" + varcomment + "</td>";    
   string = string + "</tr>";
   return string;
  }
  
 function HTML_write_head_line_input_file_string(varName, varcomment)
  {
   var prefixM = getActiveMotor();
   
   var string = "<tr>";
   string = string + "<td align=\"left\" style=\" width=150px; font-weight:bold; font-size:13px; font-family:'verdana'\">" + varName + "</td>";
   string = string + "<td align=\"left\" style=\" font-style:'italic'; font-size:13px; font-family:'verdana'\" >" + varcomment + "<b id=\""+prefixM+"configFileName\">PMSM_appconfig</b><b>.h</b></td>";    
   string = string + "</tr>";
   return string;
  }
  
    function HTML_head_line_input_file_field(varName)
  {
   var prefixM = getActiveMotor();
   var string = "<tr>";
   string = string + "<td align=\"left\" style=\" width=150px; font-weight:bold; font-size:13px; font-family:'verdana'\">" + varName + "</td>";
   string = string + "<td align=\"left\" ><input type=\"text\" style=\"text-align:right; font-size:15px; font-style:italic\" id = \""+prefixM+"fileName\" size=\"30\" onkeydown=\"return checkEnterPressFileName(event,id)\">";
   string = string + "<b style=\"font-weight:normal; font-style:italic; font-size:13px; font-family:'verdana'\">.h</b></td>";    
   string = string + "</tr>";
   return string;
  }
  
function checkEnterPressFileName(evt,id) {
    
    var prefixSingleMotor;
    var prefixM = getActiveMotor();
    /*************** Read application settings from parent document tables ******/   
    var MotorsNumber    = parent.document.getElementById("MotorsNo").innerText;

   // if single motor app then without prefix
   if (MotorsNumber == "Single") prefixSingleMotor = "";  
   else prefixSingleMotor = prefixM;
   
    // check enter pressing
    evt = (evt) ? evt : window.event
    var charCode = (evt.which) ? evt.which : evt.keyCode 
    if(charCode==13)
    {   
        object = document.getElementById(id);
        
        if(isNaN(object.value)) 
        {
          object.blur();
          // update the text field in the outheader tab / File name position
          document.getElementById(prefixM+"configFileName").innerHTML = prefixSingleMotor + object.value ;
          // text field right alignmenet in the outheader tab / File name position
          document.getElementById(prefixM+"configFileName").style.alignContent = "right";
          // update the background table
          parent.document.getElementById(prefixM+"configFileName").innerHTML = object.value;
        }
        else
        {
         document.getElementById(prefixM+"configFileName").innerHTML = prefixSingleMotor + 'PMSM_appconfig' ;
         document.getElementById(id).innerText = prefixSingleMotor+'PMSM_appconfig' ; 
          return true
        }
    } 
    }

/***************************************************************************//*!
*
* @brief   The function creates comment without/ with  value [unit]
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function HTML_write_comment_line(varComment, varValue, valUnit)
{
 var string = "<tr>";
 string = string + "<td colspan = \"2\" align=\"left\" style=\" font-style:italic; font-size:12px; font-family:verdana; color:#006600;\" >// " + varComment + "</td>";
 
 if (varValue!="")
  string = string + "<td id=\"" +varValue+ "\" colspan = \"2\" align=\"left\" style=\" font-style:italic; font-size:12px; font-family:verdana; color:#006600\" width:200px;>" + varValue + " [" + valUnit + "]</td>";
  
 string = string + "</tr>";
 return string;
}

/***************************************************************************//*! 
*
* @brief   The function creates string: #define + Constant + value
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function HTML_write_define_line_number(prefix,typeAtr,varName, xmlObject)
{
  var varNameXml = varName;
  var xmlVariableNode = xmlObject.getElementsByTagName(varName)[0];
    
  // check if XLM variable name item is not empty
  if(xmlVariableNode.childNodes.length!=0)
  {
     if(xmlDoc.getElementsByTagName(varName))
       // get name of FM variable defined in XML param file
       varNameXml =  xmlVariableNode.childNodes[0].nodeValue;
  }

 varValue = 0;
 //valType = parent.document.getElementById("Arithmetic").innerHTML; 
   
 var string = "<tr>";
 string = string + "<td align=\"left\" style=\" font-size:14px; font-family:courier; color:blue\" width:60px;\">#define</td>";
 string = string + "<td align=\"left\" style=\" font-size:14px; font-family:courier;  width:450px;\">" + prefix + varNameXml + "</td>";
    
 string = string + "<td id=\""+varName+"\" name =\""+prefix + varNameXml+"\" style=\" font-size:14px; font-family:courier; text-align:left;  width:380px; \">" + varValue + "</td>";
 string = string + "<td style=\" font-size:13px; font-family:courier; width:0px;\"></td>";       

 string = string + "</tr>";
 
 switch (typeAtr)
    {
    case 0: // gain taken the default arithmetic type
      return string;    
      break;
    
    case 1: // scale taken the default arithmetic type, if Float do not show
      var valType     = parent.document.getElementById("Arithmetic").innerText;
      if (valType=='Float')
        string = '';  
      return string;
      break;

    case 2: // scale taken the default arithmetic type, if Float do not show
      var valType     = parent.document.getElementById("Arithmetic").innerText; 
      if (valType!='Float')
        string = '';  
      return string;
      break;
    }
}

/***************************************************************************//*! 
*
* @brief   The function creates string: #define + Constant + value
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function HTML_write_comment_line_dash()
{
 var string = "<tr>";
 
 string = string + "<td colspan = \"4\" align=\"left\" style=\" font-style:italic; font-size:12px; font-family:verdana; color:#006600;\" >";
 string = string + " //----------------------------------------------------------------------------------------------------------- </td>";
 string = string + "</tr>";
 
 return string;
}
  
/***************************************************************************//*! 
*
* @brief   The function creates string: #define + Constant + value
* @param   
* @return  None
* @remarks 
******************************************************************************/ 
function HTML_write_blank_line()
{
 var string = "<tr>";
 
 string = string + "<td style=\"font-size:6px;\"> &nbsp</td>";
 string = string + "</tr>";
 
 return string;
}
  
/***************************************************************************//*!
*
* @brief   The function write input form value
* @param   valueId - name of ID where the value is written to
* @return  
* @remarks 
******************************************************************************/
function copyParent2HeaderCfgById(outputValueId,valueId, postfix, comment)
{
    var object      = null;
  
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();

    object = document.getElementById(outputValueId);
    var mainValue = parent.document.getElementById(prefixM + valueId).innerHTML;
      
    if(object)  
      if(comment)
        object.innerHTML = "= " + mainValue + postfix;
      else  
        {
          if((mainValue%1)==0)
            object.innerHTML = "(" + mainValue + postfix + ")";
          else
            object.innerHTML = "(" + mainValue + ")";  
 }      }

 /***************************************************************************//*!
*
* @brief   The function write input form value scaled to scaleId variable
* @param   valueId - name of ID where the value is written to
*          scaleId - name of ID where the value is scaled to
* @return  
* @remarks 
******************************************************************************/
function copyParent2HeaderCfgByIdScaled(outputValueId,valueId, scaleId)
{
    var object      = null;
    var valType     = parent.document.getElementById("Arithmetic").innerText;
  
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();

    object = document.getElementById(outputValueId);
      
    if(object)  
    {
      var mainValue = Math.round(((parent.document.getElementById(prefixM + valueId).innerHTML)/(parent.document.getElementById(prefixM + scaleId).innerHTML))*1000000000000)/1000000000000; 

      if (valType=='Float')  
      object.innerHTML = "("+mainValue+"F)";
    
      if (valType=='Frac16')  
        object.innerHTML = "FRAC16("+mainValue+")";
      
      if (valType=='Frac32')  
        object.innerHTML = "FRAC32("+mainValue+")";
        
      // for number higher than +-1 round to whole number
      if(Math.abs(mainValue)>1)
        object.innerHTML = "(" + Math.ceil(mainValue) + ")";
    
    }
 } 
   
 /***************************************************************************//*!
*
* @brief   The function write input form value
* @param   valueId - name of ID where the value is written to
* @return  
* @remarks 
******************************************************************************/
function calcFmScaleVariable(valueId, parentID,value)
{
    var object      = null;
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();

    object = document.getElementById(valueId);
    if(parentID!="")
      var FMscale     = parent.document.getElementById(prefixM + parentID).innerHTML;    
    else
      var FMscale     = value;
    
    if(object)
    {
    if(FMscale%1==0)                                                        
      object.innerHTML = "("+ Math.ceil(FMscale)+".0F)";
    else 
      object.innerHTML = "("+FMscale+"F)";
    }  
} 
       
/***************************************************************************//*!
* @brief:   generates a string of star symbols as a comemnt line    
* @param:   string - input string    
* @return:  string - output string = input string + comment line 
* @remarks: the width of the line is 72 characters 
******************************************************************************/
 function write_comment_line_star(string)
 {
  string = string + "/"
  for(var i=0;i<70;i++)
  {
  string = string + "*"
  }
  string = string + "/\r\n"   
 
  return(string);
 }

/***************************************************************************//*!
* @brief:   generates a string of dash symbols as a comemnt line    
* @param:   string - input string    
* @return:  string - output string = input string + comment line 
* @remarks: the width of the line is 72 characters 
******************************************************************************/
 function write_comment_line_dash(string)
 {
  string = string + "//"
  for(var i=0;i<70;i++)
  {
  string = string + "-"
  }
  string = string + "\r\n"   
 
  return(string);
 }
 
 /***************************************************************************//*!
* @brief:   generates an empty line     
* @param:   string - input string
*           No - number of required empty lines   
* @return:  string - output string = input string + new line command 
* @remarks:  
******************************************************************************/
 function write_blank_lines(string, No)
 {
  for(var i=0;i<No;i++)
  {
  string = string + "\r\n"
  }
  return(string);
 } 
 
/***************************************************************************//*!
* @brief:   Test if tabName is inclluded for active Motor      
* @param:   tabName
* @return:  true or false 
* @remarks:  
******************************************************************************/
 function testActiveTab(tabName)
 {
    var object      = null;
  
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    
    if(prefixM == 'M1_')
      object = parent.document.getElementById('Tab'+[1]+'manager'); 
    
    if(prefixM == 'M2_')
      object = parent.document.getElementById('Tab'+[2]+'manager');
    
    if(prefixM == 'M3_')
      object = parent.document.getElementById('Tab'+[3]+'manager');
    
    var tabNames = [ 
        "Introduction",
        "Parameters",
        "Control Loop",
        "Speed Loop",
        "Position Speed",
        "Sensorless",
        "Output File",
        "Control Struc",
        "Application Control",
      ];
      
   for (var i=0; i<9; i++)
   {
      if(tabName == tabNames[i])
      {
          if((object.innerHTML>>>[i])&1)
            return true;
          else
            return false;
      }
   }   
 }
 
 /***************************************************************************//*!
* @brief:   generates a string of commented text + value + unit    
* @param:   string - input string
*           Prefix - Mx_ ; x - number of selected motor 
*           Text - string that has to be displayed
*           Value - a value of displaied variable (if needed)
*           Unit - an unit of value (if needed)     
* @return:  string - output string = input string + commented text line 
* @remarks:  
*           If Prefix = "Mx_" then length between text and Value = 38
*           If Prefix = "" then length between text and Value = 41           
******************************************************************************/
 function write_comment_text(string,text, varId)
 {
  var object = document.getElementById(varId);
  var line_length;
  var Text = text;
  var Value = "";
  var a=text.length;

  if(object)
    Value = object.innerHTML;
    
  line_length = 38-a;
  
  for (var i = 0;i<line_length;i++)
  {
      Text = Text + " ";
  } 
    
  string = string + '//' + Text+ Value + '\r\n';
  return (string);
 } 
 
 
/***************************************************************************//*!
* @brief:   generates a string: #define + Prefix_Variable + (Value)     
* @param:   string - input string
*           Prefix - Mx_ ; x - number of selected motor 
*           Variable - name of the variable that has to be displayed
*           Value - a value of displaied variable 
*           Type - data type representation     
* @return:  string - output string = input string + generated string 
* @remarks:  
*           if Type = "FRAC16 or FRAC32" then: string is displayed
*           if Type = "FLOAT" then: string is NOT displayed
*           if Type = " " then: string is displayed (e.g. Application scales)  
*                              
******************************************************************************/
 function write_define_line_number(prefix, string,varId)
 {
  var object = document.getElementById(varId);  

  if(object)
  {
    var Variable = document.getElementById(varId).id;
    var Value = document.getElementById(varId).innerHTML; 
    var a = Variable.length;
  
    for (var i = 0;i<(32-a);i++)
    {
      Variable = Variable + ' ';
    } 
   
      string = string + '#define ' + prefix + Variable + Value + '\r\n';
  }
  else
  {
    string=string;
  }
  
  return(string);
 } 

/***************************************************************************//*! 
*
* @brief   The function creates string: #define + Constant + value
* @param   
* @return  None
* @remarks 
******************************************************************************/
function HTML_write_output_blocks(blockName)
{
    var MotorsNumber    = parent.document.getElementById("MotorsNo").innerText;
    
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    var prefix = prefixM;
    
    //get object of XML constant name list
    xmlDoc=loadXMLDoc("xml_files\\Header_file_constant_list.xml");

   // if single motor app then without prefix
   //if (MotorsNumber == "Single") prefix = "";
    
   // write HTML output preview of header file based on defined blocks 
   if(blockName == 'Parameters')
       writeParametersHTMLOutput(prefix,xmlDoc);    
    
   if(blockName == 'Control Loop')
      writeCtrlLoopHTMLOutput(prefix,xmlDoc);       
   
   if(blockName == 'Speed Loop')
      writeSLoopHTMLOutput(prefix,xmlDoc);  
      
   if(blockName == 'Sensorless')
      writeSensorlessHTMLOutput(prefix,xmlDoc);   
      
   if(blockName == 'Control Struc')
      writeCascadeHTMLOutput(prefix,xmlDoc);
}

/***************************************************************************//*! 
*
* @brief   The function creates string: #define + Constant + value
* @param   
* @return  None
* @remarks 
******************************************************************************/
   
/* ----------- Generate Header File - onclick event -------------- */
function Save_h_click()
{
    var str='';
    /* get actual date and time of file storing */
    var today       = new Date();
    var mounth      = MounthConverter(today.getMonth());
    var t;
    var c = ":";
    var hours       = today.getHours();
    var minutes     = today.getMinutes();
    var seconds     = today.getSeconds();

    time_out = hours + c;
    (minutes<10) ? (time_out += '0' + minutes + c) : (time_out += minutes + c);
    (seconds<10) ? (time_out += '0' + seconds) : (time_out += seconds);

    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
    var prefix = prefixM;

    if(!pcm.IsCommPortOpen())
    {
        alert('Communication error, output file will be generated to the same folder as FreeMASTER project file!');
        //return;
    }
  
    /*************** Read application settings from parent document tables ******/
    var MotorsNumber    = parent.document.getElementById("MotorsNo").innerText;

    // if single motor app then without prefix
    //if (MotorsNumber == "Single") prefix = "";

    var prefixM = getActiveMotor();
    var DocLocation =  parent.document.getElementById(prefixM + "ProjectPath").innerHTML;
    //var RelPath_DocLocation = getRelativePath(DocLocation);
    /*************** Headlines                              *********************/
   
    str = write_comment_line_star(str);
    str = str + "// File Name: {FM_project_loc}/" + DocLocation + "bldc_appconfig.h \r\n";
    str = str + "//\r\n";
    //str = str + "// Date:  " + today.getTime()+ today.getDate()+ ". " + mounth + ", " + today.getFullYear() + "\r\n";
    //document.write(HTML_write_head_line("Date:",""+mounth+ " "+ today.getDate() + ", " + today.getFullYear() + ", " + t+ ""));
    str = str + "// Date:  " + mounth + " " + today.getDate() + ", " + today.getFullYear() + ", " + time_out + "\r\n";
    str = str + "//\r\n";
    str = str + "// Automatically generated file for static configuration of the BLDC application\r\n";
  
    str = write_comment_line_star(str);
   
    /****************************************************************************/
    str = write_blank_lines(str,1);
    str = str + "#ifndef __"+prefixM+"BLDC_APPCONFIG_H\r\n";
    str = str + "#define __"+prefixM+"BLDC_APPCONFIG_H\r\n";
   
    /*************** Application scales                     *********************/
    str = write_blank_lines(str,1);
   
    // input Parameters
    if(testActiveTab('Parameters'))
        str = writeParametersHeaderOutput(prefix, str);
  

    // Torque Loop
    if(testActiveTab('Control Loop'))
        str = writeCtrlLoopHeaderOutput(str);

    // Sensorless
    if(testActiveTab('Sensorless'))
        str = writeSensorlessHeaderOutput(str);


    /*************** End of page                            *********************/
    str = write_blank_lines(str,1);
    str = str + '#endif';
    str = write_blank_lines(str,2);
    str = write_comment_text(str,'End of generated file','','');
    str = write_comment_line_star(str);


    /*************** Writing to file process                *********************/
    // open file for writting
    var paramFile = pcm.LocalFileOpen(DocLocation + prefix.toLowerCase() + "bldc_appconfig.h","w");
    // write string to output file
    charNumber = pcm.LocalFileWriteString(paramFile,str);
    // close param file
    fileStatus = pcm.LocalFileClose(paramFile);

    if(fileStatus = false)
        alert('File closing error');
      
}
  
/* ----------- End of Generate Header File - onclick event onload="hFileHead()" -------------- */   