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
* @file   calculations.js
*
* @brief  javascritp engine for MCAT Tool
*
******************************************************************************/

/******************************************************************************
* List of functions
******************************************************************************
*
*  getInnerHtmlValue(varID) - read value from a form cell
*  setInnerHtmlValue(varID,newValueFrac, newValueFloat) - write value to form cell
*  setInnerHtmlValueAsText(varID,typeAtr,newValue, newValueFloat) -
*  getParentHtmlValue(valueId) - 
*  storeParentHtmlValue(valueId) - 
*  setParentHtmlValue(valueId, newValue) -
*  copyParent2InnerValById(valueId) - 
*  loadXMLDoc(dname) -  
*  ReloadStoreButtonsOnOff(on_off_param) - 
*  UpdateFMVariable(xmlObject,varId) -
*  testVarValue(varId, value)
*  
*******************************************************************************/
/***************************************************************************//*!
*
* @brief   The function reads input form value
* @param   varID - variable ID
* @return  Actual value
* @remarks 
******************************************************************************/
function getInnerHtmlValue(varID)
{
    var object      = null;
    
    object = document.getElementById(varID);
    return (Number(object.value));  
}
 
 /***************************************************************************//*!
*
* @brief   The function writes input form value
* @param   varID - variable ID
*          newValue - value written to the cell
* @return  
* @remarks 
******************************************************************************/
function setInnerHtmlValue(varID,newValueFrac, newValueFloat)
{
    var valType     = parent.document.getElementById("Arithmetic").innerText;
    var object      = null;
    
    object = document.getElementById(varID);
    
    if(!getActiveMode())
      object.readOnly  = true;
    else
      object.readOnly  = false;  
  
    /* output number type is float */
    if(valType=="Float")
    {
      if(newValueFloat!='N/A')
      {
        object.value = newValueFloat;
        object.enabled = true;
      }
      else
      {
        object.disabled = true;
        object.value = 'N/A';
      }  
    }
    else
    {
      /* output number type is Frac16 or Frac32 */
      if(newValueFloat!='N/A')
      {
        object.value = newValueFrac;
      }
      else  
      {
        object.value = newValueFrac;
      }  
    }
}  

/***************************************************************************//*!
*
* @brief   The function writes input form value
* @param   varID - variable ID
*          newValue - value written to the cell
* @return  
* @remarks 
******************************************************************************/
function setInnerHtmlValueAsText(varID,typeAtr,newValue, newValueFloat)
{
    var object      = null;
    var valType     = parent.document.getElementById("Arithmetic").innerText;
    var x = newValue;
    
    /* transformation form 0.000 to 0 format */
    if(newValue==0)
      newValue = 0;
    if(newValueFloat==0)
      newValueFloat = 0; 

    /* transformation form 1.000 to 1 format */    
    if(newValue==1)
      newValue = 1;

    /* transformation form -1.000 to -1 format */    
    if(newValue==-1)
      newValue = -1;

    
    switch (typeAtr)
    {
    case 0: // gain taken the default arithmetic type
      object = document.getElementById(varID);
      
      if (valType=='Float')  
        if(newValueFloat%1==0)                                                        
          object.innerHTML = "("+newValueFloat+".0F)";
         else 
          object.innerHTML = "("+newValueFloat+"F)";
      if (valType=='Frac16')  
          if(newValue%1==0)
            object.innerHTML = "FRAC16("+newValue+".0)";
           else
            object.innerHTML = "FRAC16("+newValue+")";
      
      if (valType=='Frac32')  
        if(newValue%1==0)
          object.innerHTML = "FRAC32("+newValue+".0)";
        else
          object.innerHTML = "FRAC32("+newValue+")";    
      break;
    
    case 1: // scale taken the default arithmetic type, if Float do not show
      object = document.getElementById(varID);
      if (object)  
        object.innerHTML = "("+newValue+")";
      break;  
    
    case 2: // value is always integer
      object = document.getElementById(varID);
      if (object)  
        object.innerHTML = "("+newValue+")";
      break;
    
    case 3: // value is FRAC32 in case of FLOAT is selected
      object = document.getElementById(varID);
      if (object)
        if((valType=='Float')||(valType=='Frac32')) 
          if(newValue%1==0)
            object.innerHTML = "FRAC32("+newValue+".0)";
          else
            object.innerHTML = "FRAC32("+newValue+")";
        else
          if(newValue%1==0)
            object.innerHTML = "FRAC16("+newValue+".0)";
          else
            object.innerHTML = "FRAC16("+newValue+")";               
        break;    
    
    case 4: // value is FRAC16 
      object = document.getElementById(varID);
      if (object)
          if(newValue%1==0)
            object.innerHTML = "FRAC16("+newValue+".0)";
          else
            object.innerHTML = "FRAC16("+newValue+")";               
        break;    
        
    case 5: // value is integer with .0 format or float 
    object = document.getElementById(varID);
    if (object)
      if (valType=='Float')  
      {
         if(newValueFloat%1==0)                                                        
          object.innerHTML = "("+newValueFloat+".0F)";
         else 
          object.innerHTML = "("+newValueFloat+"F)";
      }
      else
      {
        if(newValueFloat%1==0)                                                        
          object.innerHTML = "("+newValue+".0)";
         else 
          object.innerHTML = "("+newValue +")";      
      }                  
      break;   
    case 6: // value is integer with .0 format or float 
      object = document.getElementById(varID);
      if (object)
       if (valType=='Float')  
       {
         if(newValueFloat%1==0)                                                        
          object.innerHTML = "("+newValueFloat+".0F)";
         else 
          object.innerHTML = "("+newValueFloat+"F)";
       } 
      
       if ((valType=='Frac16')||(valType=='Frac32'))  
         if(newValue%1==0)
           object.innerHTML = "FRAC32("+newValue+".0)";
         else
           object.innerHTML = "FRAC32("+newValue+")";  
       break; 
      
     
     case 7: // value is FRAC32 
      object = document.getElementById(varID);
      if (object)
          if(newValue%1==0)
            object.innerHTML = "FRAC32("+newValue+".0)";
          else
            object.innerHTML = "FRAC32("+newValue+")";               
        break;   
     
     case 8: // value is always FLOAT 
      object = document.getElementById(varID);
      if (object)
          if(newValue%1==0)
          {
            newValueFloat = Number(newValueFloat).toFixed(0);
            object.innerHTML = "("+newValueFloat+".0F)";
          }
          else
            object.innerHTML = "("+newValueFloat+"F)";               
        break;   
     
     case 9: // value is ACC16 
      object = document.getElementById(varID);
      if (object)
          if(newValue%1==0)
            object.innerHTML = "ACC16("+newValue+".0)";
          else
            object.innerHTML = "ACC16("+newValue+")";               
        break;   
     
     case 10: // value is ACC32 
      object = document.getElementById(varID);
      if (object)
          if(newValue%1==0)
            object.innerHTML = "ACC32("+newValue+".0)";
          else
            object.innerHTML = "ACC32("+newValue+")";               
        break;   
    }
}    

/***************************************************************************//*!
*
* @brief   The function reads input form value ID and converts it to a number form
* @param   valueId - name of ID where the value is read
* @return  Actual value
* @remarks 
******************************************************************************/
function getParentHtmlValue(valueId)
{
    var object      = null;
    
    if((document.getElementById("settingTab") != undefined)||(document.getElementById("mainPageTab") != undefined))
    {
        // add prefix to var ID
        object = parent.document.getElementById(valueId).innerHTML;
    }
    else
    {
      //get active motor to selct proper prefix
      var prefixM = getActiveMotor();
      // add prefix to var ID
      try{
        object = Number(parent.document.getElementById(prefixM + valueId).innerHTML);
      
      }
      catch(err){
        window.alert(("getParentHtmlValue"+prefixM+valueId).toString());
      };
    }
     
    return (object);  
} 

/***************************************************************************//*!
*
* @brief   This function is a replacement for getParentHtmlValue(valueId) in paramFileWriteData(tableType)
* @param   valueId - name of ID where the value is read
* @return  Actual value
* @remarks 
******************************************************************************/
function getParentHtmlValueStore(valueId)
{
    var object      = null;
    
    if((document.getElementById("settingTab") != undefined)||(document.getElementById("mainPageTab") != undefined))
    {
        // add prefix to var ID
        object = parent.document.getElementById(valueId).innerHTML;
    }
    else
    {
      //get active motor to selct proper prefix
      var prefixM = getActiveMotor();
      // add prefix to var ID
        object = parent.document.getElementById(prefixM + valueId).innerHTML;
     }
     
    return (object);  

} 

/***************************************************************************//*!
*
* @brief   The function write input form value ID and keeps it as a string form
* @param   valueId - name of ID where the value is read
*          value - param value to be written
* @return  
* @remarks 
******************************************************************************/
function storeParentHtmlValue(valueId, value)
{
    
    if((document.getElementById("settingTab") != undefined)||(document.getElementById("mainPageTab") != undefined))
    {
        // add prefix to var ID
        parent.document.getElementById(valueId).innerHTML = value;
    }
    else
    {
      //get active motor to selct proper prefix
      var prefixM = getActiveMotor();
      // add prefix to var ID
      parent.document.getElementById(prefixM + valueId).innerHTML = value;
     }
}

/***************************************************************************//*!
*
* @brief   The function set read only attribute on input box and change background color
* @param   valueId - name of ID where the value is read
* @return  
* @remarks 
******************************************************************************/
function disableInputParamBox(valueId)
{
    
    if(document.getElementById("ControlStructure") != undefined)
    {
      // set read only attributte
      document.getElementById(valueId).readOnly  = true;
      // change background color
      document.getElementById(valueId).style.backgroundColor ='#C3C7CC';   //rgb(195,199,204)    
    }
    else
    {
      //get active motor to selct proper prefix
      var prefixM = getActiveMotor();
      // set read only attributte
      document.getElementById(prefixM + valueId).readOnly  = true;
      // change background color
      document.getElementById(prefixM + valueId).style.backgroundColor ='#C3C7CC';   //rgb(195,199,204)
     
      // clear red text color of ID in main inner table 
      parent.document.getElementById(prefixM + valueId).style.color="black";
     }
     
     
 }
 
 /***************************************************************************//*!
*
* @brief   The function clear read only attribute on input box and change background color
* @param   valueId - name of ID where the value is read
* @return  
* @remarks 
******************************************************************************/
function enableInputParamBox(valueId)
{
    
    if(document.getElementById("ControlStructure") != undefined)
    {
      // set read only attributte
      document.getElementById(valueId).readOnly  = false;
      // change background color
      document.getElementById(valueId).style.backgroundColor ='white';      
    }
    else
    {
      //get active motor to selct proper prefix
      var prefixM = getActiveMotor();
      // set read only attributte
      document.getElementById(prefixM + valueId).readOnly  = false;
      // change background color
      document.getElementById(prefixM + valueId).style.backgroundColor ='white';  
     
      // clear red text color of ID in main inner table 
      parent.document.getElementById(prefixM + valueId).style.color="black";
     }
     
     
 }  

/***************************************************************************//*!
*
* @brief   The function calle storing and disabling functions
* @param   valueId - name of ID where the value is read
*          value - param value to be written
* @return  
* @remarks 
******************************************************************************/
function switchParam2BasicMode(valueId,value)
{
    storeParentHtmlValue(valueId,value);

    disableInputParamBox(valueId);
    
}
  
/***************************************************************************//*!
*
* @brief   The function reads input form value
* @param   valueId - name of ID where the value is written to
*          newValue - value to write
* @return  
* @remarks 
******************************************************************************/ 
function setParentHtmlValue(valueId, newValue)
{   
    parent.document.getElementById(valueId).innerHTML = newValue; 
} 
  
 /***************************************************************************//*!
*
* @brief   The function writes input form value
* @param   valueId - name of ID where the value is written to
* 
* @return  
* @remarks 
******************************************************************************/
function copyParent2InnerValById(valueId)
{
    var object      = null;
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();

    // add prefix to var ID
    if(prefixM!='')
      object = document.getElementById(prefixM + valueId);
      
     try{
    if(object)  
      object.value = parent.document.getElementById(prefixM+valueId).innerHTML;
  }
     catch(err){
         window.alert(("copyParent2InnerValById-"+prefixM+valueId).toString());
         window.alert(object.toString());
         };
      
      if(parent.document.getElementById(prefixM+valueId).style.color=="red")
          object.style.background = 'rgb(250,183,153)';            
} 
 
/***************************************************************************//*!
*
* @brief   Check if param tab consists of unsaved data. If some, enable Store and 
*          Unload Data buttons
* @param   0 - disable buttons
*          1  - enable buttons if condition is TRUE
* @return  None
* @remarks 
******************************************************************************/ 
 function ReloadStoreButtonsOnOff(on_off_param)
 {
    var prefixM = getActiveMotor();

    // add prefix to var ID
    if(prefixM!='')
      var paramTableID = 'paramTable' + prefixM;
      
    //disable buttons
    if(on_off_param==0)
    {
      if(document.getElementById("StoreData") != undefined){
            document.getElementById("StoreData").disabled = true;
            document.getElementById("StoreData").className = "fButtonsDisabled";
        }
      
      if(document.getElementById("ReloadData") != undefined){
        document.getElementById("ReloadData").disabled = true;
        document.getElementById("ReloadData").className = "fButtonsDisabled";
        }
      
      if(document.getElementById("Calculate") != undefined)
        document.getElementById("Calculate").disabled = true;
    
    }
    
     //alert(parent.document.getElementById(paramTableID).style.color);
    
    //enable buttons if condition is TRUE
    if((on_off_param==1)&&(parent.document.getElementById(paramTableID).style.color=="red"))
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
    
    if(document.getElementById("Calculate") != undefined)
        document.getElementById("Calculate").disabled = false;
    }
 
 }  

/***************************************************************************//*!
*
* @brief   Create new XML object 
* @param   
* @return  None
* @remarks 
******************************************************************************/
function loadXMLDoc(dname) 
{
    try //Internet Explorer
    {
      xmlDoc=new ActiveXObject("Msxml2.DOMDocument.3.0");
    }
    catch(e)
    {
      try //Firefox, Mozilla, Opera, etc.
    {
      xmlDoc=document.implementation.createDocument("","",null);
    }
    catch(e) {alert(e.message)}
    }
    try 
    {
    xmlDoc.async=false;
    xmlDoc.load(dname);
    return(xmlDoc);
    }
    catch(e) {alert(e.message)}
    return(null);
}

/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function UpdateFMVariable(xmlObject,varId,valueFrac,valueFloat){
  
    var retMsg;
    var prefixM = getActiveMotor();
    var valType     = parent.document.getElementById("Arithmetic").innerText;
    var valUpdate;      
    
    /* output number type is float */
    if(valType=="Float")
    {
      if(valueFloat!='')
        valUpdate = valueFloat; 
      else
        valUpdate = 0;
    }
    else
    {
      /* output number type is Frac16 or Frac32 */
      if(valueFrac!='')
        valUpdate = valueFrac; 
      else
        valUpdate = 0;
    }
    
    // add prefix to var ID
    if(prefixM!='')
        var xmlVarName = prefixM + varId;
    
    var xmlVariableNode = xmlObject.getElementsByTagName(xmlVarName)[0];
   // check if XLM variable name item is not empty
   if(xmlVariableNode.childNodes.length!=0)
   {
      if(xmlObject.getElementsByTagName(xmlVarName))
        // get name of FM variable defined in XML param file
       var xmlVariable =  xmlVariableNode.childNodes[0].nodeValue;
     
      // update FM variable
      succ = pcm.WriteVariable(xmlVariable,valUpdate, retMsg);
      // varible not updated properly in FM
      if(!succ)
        return(varId +'=NA');
      else
        return('');  
    }
    else
    {
       // variable name not filled in xml file
       return(varId + '=EMPTY');
    }

}

/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER directly without XML reference
* @param   
* @return  None
* @remarks 
******************************************************************************/
function UpdateFMVariableDirect(varId,valueFrac,valueFloat){
  
    var retMsg;
    var prefixM = getActiveMotor();
    var valType     = parent.document.getElementById("Arithmetic").innerText;
    var valUpdate;      
    
    /* output number type is float */
    if(valType=="Float")
    {
      if(valueFloat!='')
        valUpdate = valueFloat; 
      else
        valUpdate = 0;
    }
    else
    {
      /* output number type is Frac16 or Frac32 */
      if(valueFrac!='')
        valUpdate = valueFrac; 
      else
        valUpdate = 0;
    }
     
    // update FM variable
    succ = pcm.WriteVariable(varId,valUpdate, retMsg);
    // varible not updated properly in FM
    if(!succ)
      return(varId +'=NA');
    else
      return('');  
}

/***************************************************************************//*!
*
* @brief   update variables in FreeMASTER application
* @param   
* @return  None
* @remarks 
******************************************************************************/
function Update2FMVariables(xmlObject,var1Id,value1,var2Id,value2){
  
    var retMsg;
    var prefixM     = getActiveMotor();
    var valType     = parent.document.getElementById("Arithmetic").innerText;
  //  var valUpdate;      
    
    /* output number type is float *
    if(valType=="Float")
    {
      if(valueFloat!='')
        valUpdate = valueFloat; 
      else
        valUpdate = 0;
    }
    else
    {
      /* output number type is Frac16 or Frac32 *
      if(valueFrac!='')
        valUpdate = valueFrac; 
      else
        valUpdate = 0;
    }  */
    
    // add prefix to var ID
    if(prefixM!='')
    {
        var xmlVar1Name = prefixM + var1Id;
        var xmlVar2Name = prefixM + var2Id;        
     }
    
    var xmlVariable1Node = xmlObject.getElementsByTagName(xmlVar1Name)[0];
    var xmlVariable2Node = xmlObject.getElementsByTagName(xmlVar2Name)[0];
        
   // check if XLM variable name item is not empty
   if(xmlVariable1Node.childNodes.length!=0 && xmlVariable2Node.childNodes.length!=0)
   {
    //  if(xmlObject.getElementsByTagName(xmlVar1Name))
        // get name of FM variable defined in XML param file
       var xmlVariable1 =  xmlVariable1Node.childNodes[0].nodeValue;
       var xmlVariable2 =  xmlVariable2Node.childNodes[0].nodeValue;       
     
      // update FM variable
      succ = pcm.WriteVariable(xmlVariable1,value1, retMsg);
      succ = pcm.WriteVariable(xmlVariable2,value2, retMsg);
      
      
      // varible not updated properly in FM
      
      
      if(!succ)
        return(var1Id +'=NA');
      else
        return('');  
    }
    else
    {
       // variable name not filled in xml file
       return(var1Id + '=EMPTY');
    }

}

/***************************************************************************//*!
* @brief:   Test Id value based on active Motor     
* @param:   tabName
* @return:  true or false 
* @remarks:  
******************************************************************************/
 function testVarValue(varId, value)
 {
    var object      = null;
  
    //get active motor to selct proper prefix
    var prefixM = getActiveMotor();
                                                             
    object = parent.document.getElementById(prefixM + varId); 

    if(object.innerHTML === value)
       return true;
    else
        return false;  
}

/***************************************************************************//*!
* @brief:   Test fractional value if is in required range <-1;1)     
* @param:   Frac value
* @return:  true or false 
* @remarks:  
******************************************************************************/
 function testFracValRange(varName,value, negative)
 {
    var valType     = parent.document.getElementById("Arithmetic").innerText;
        
    /* output number type is float */
    if(valType!="Float")
    {    
      if(negative==1)
      {  
        if((value<-1)||(value>=1))
          alert('Variable "' + varName + '= ' + value +'" is out of range <-1;1)!');      
      }
      else
      {
        if((value<0)||(value>1))
          alert('Variable "' + varName + '= ' + value +'" is out of range <0;1>!');  
       }
    } 
 }     
/***************************************************************************//*!
* @brief:   Test fractional value if is in required range <minVal;maxVal)     
* @param:   Frac value
* @return:  true or false 
* @remarks:  
******************************************************************************/
function testValRange(varName,value,minVal, maxVal)
{
  var LimitHigh;
  var LimitLow;
  
  LimitHigh = Math.round(maxVal*10000)/10000;
  LimitLow = Math.round(minVal*10000)/10000;
  
  if((value<minVal)||(value>maxVal))
    alert('Variable "' + varName + '= ' + value +'" is out of range <' + LimitLow + ';'+ LimitHigh +'>');
}
/***************************************************************************//*!
* @brief:   Return active Tab body ID and call dedicated update tab constants     
* @param:   
* @return:   
* @remarks:  
******************************************************************************/
function updateTab()
{
 var bodyId = document.getElementsByTagName("body")[0].id;

 // call dedicated unified function for each tab
 newUpdateTab = new Function ("updateTab_" + bodyId + "()");
 newUpdateTab();

}

/***************************************************************************//*!
* @brief:   Check wheter variable update to FM was correct or not     
* @param:   
* @return:   
* @remarks:  
******************************************************************************/
function UpdateError(errorArray)
{
  var notUpdated = '';
  var empty ='';
  var onlyError = [];
  
  for (i=0;i<errorArray.length;i++)
  { 
    if(errorArray[i]!='') 
    {
      onlyError = errorArray[i].split("=");
      if(onlyError[1]=='NA')
        notUpdated = notUpdated + onlyError[0] + '\n';  
      if(onlyError[1]=='EMPTY')
        empty = empty + onlyError[0] + '\n'; 
    }     
  }
  
  // display error message of empty XML constant items      
  if(empty!='')
    alert("Error - empty XML items of constants:\n\n" + empty + "\nDouble-check the FM_params_list.xml file\nand fill the missing parameters!");
  
  // display error message of non updated constants
  if(notUpdated!='')
    alert("Error - following constants cannot be updated in FM:\n\n" +notUpdated + "\nName of entered values in XML file does not correspond with a symbol name in the current FreeMASTER project.\nDouble-check the FM_params_list.xml file!");

}
/***************************************************************************//*!
* @brief:   Test variable range and trim to max or min value     
* @param:   
* @return:   
* @remarks:  
******************************************************************************/
function TestRangeTrim(valueId,minVal, maxVal, unit)
{
  var prefixM = getActiveMotor();
  
  if(Number(getParentHtmlValue(valueId)) > maxVal)      
  {   
      maxVal = Math.round(maxVal*1000)/1000;
      setParentHtmlValue((prefixM + valueId), maxVal);
      copyParent2InnerValById(valueId);
      alert("Maximal " + valueId + " is " + maxVal + unit); 
  }
  if(Number(getParentHtmlValue(valueId)) < minVal)
  {
      minVal = Math.round(minVal*1000)/1000;
      setParentHtmlValue((prefixM + valueId), minVal);
      copyParent2InnerValById(valueId);
      alert("Minimal " + valueId + " is " + minVal + unit);
  }
}
/***************************************************************************//*!
* 
******************************************************************************
* End of code
******************************************************************************/