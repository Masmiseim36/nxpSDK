/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* common stuf for page */
function NewArrayOf(val, length) {
  var a = []; 
  var i = length;
  while (i--) {
    a[i] = val;
  }
  return a;
}

function Get_pObjectFromID(objectID)
{
    var object = document.getElementById(objectID);
    if (object)
        return object;
    alert("Get_pObjectFromID(objectID): cannot find object:"+objectID);
    set_object_error(objectID,objectID,"");
    return 0;
}

//this function returns name of function to build rest of line
function GetFunct(cmdType)
{
    //Build function Name
    var strFun = cmdType;
 
    //Create the function
    var pBuildLineFunc = window[strFun];

    if(typeof (pBuildLineFunc) === 'function')
        return pBuildLineFunc;
    return null;
}

function CallFunctStr(functName, finalArgs)
{
    //Create the function
    var pFunc = window[functName];

    var val = "0";
    if(typeof (pFunc) === 'function')
    {
        val = pFunc.apply(this,finalArgs);
    }
    return val;
}

function IsValidDataPath(database, idpath)
{
    var database = GetDataObj(database, idpath);
    if(database == null)
    	return 0;
    return 1;
}

function GetDataObj(database, idpath)
{
    var path = idpath.split(".");
    var dataobj = database;
    for(key in path)
    {
        if(typeof dataobj  === 'object')
        {
        	dataobj = dataobj[path[key]];
        }
        else
        {
        	dataobj = 0;  
            break;
        }
    }
    return dataobj;
}

//this modification enables find object with key "aaa.bbb.ccc", the base function finds, that "aaa" not exists and exits.
function GetDataObj2(database, idpath)
{
    var path = idpath.split(".");
    var dataobj = database;
    var finalkey = "";
    for(key in path)
    {
        if(finalkey)
        	finalkey = finalkey+"."+path[key];
        else
        	finalkey = path[key];
    	if(typeof dataobj  === 'object')
        {
        	var dataobjnew = dataobj[finalkey];
        	if(typeof dataobjnew === 'object')
        	{
        		dataobj = dataobjnew;
        		finalkey = "";
        	}
        }
        else
        {
        	dataobj = 0;  
            break;
        }
    }
    return dataobj;
}

//function coverts Array to SafeArray
function getSafeArray(jsArr) {
  var dict = new ActiveXObject("Scripting.Dictionary");
  for (var i = 0; i < jsArr.length; i++)
    dict.add(i, jsArr[i]);
  return dict.Items();
}

function ConvertToHexRaw(numberValue)
{
    var decNumber = Number(numberValue);
    var hexNumber = decNumber.toString(16).toUpperCase();
    if(decNumber>15)
        return(hexNumber) ;
    else
        return("0" + hexNumber) ;
}

function ConvertToHex(numberValue)
{
    var hexNumber = ConvertToHexRaw(Number(numberValue))
    return("0x" + hexNumber) ;
}

//probably will be removed
function ConvertToDec(hexNumber)
{
    var decNumber = parseInt(hexNumber,16);
    return (decNumber);
}

function GetNumber(numstr)
{
	return format_variable_to_store(numstr)
}

function format_variable_to_store(number)
{
  //converts Hex to dec only when 0x/0X is presented in number
  var index = number.indexOf("0x");
  if (index >= 0){
    return (ConvertToDec(number));
  }
  index = number.indexOf("0X");
  if (index >= 0){
    return (ConvertToDec(number));
  }
  return Number(number);
}


function get_low_bit_mask()
{
//  if (LowBitMask)
//      return LowBitMask;
//  else
//        set_configuration_error(" Low Bit Mask is used, but not defined. Default Mask is 0x01 ");
    return 0x01; 
}

function format_variable_to_show(number, size)
{
  var convertedVar = Number(number);
  var varType = size;
  if(varType == null)
    return convertedVar;

  var negative = (convertedVar<0);
  
  if(document.topform.hexdec[0].checked)
  {
    if(isNaN(varType))
    {
      size = GetSizeFromType(varType);
      var signType = GetSignFromType(varType);
      //do not convert to hex when frac/float
      if((varType == "UFRAC16") || (varType == "SFRAC16") || (varType == "UFRAC32") || (varType == "SFRAC32") || (varType == "FLOAT") || (varType == "DOUBLE"))
        return convertedVar;
    }
//    else if(size)
    {
      var temp=Number(number);
      if(negative&&signType)
      {
        var max = Math.pow(2, (8*size));
        convertedVar=(max+convertedVar);
      }
      convertedVar = ConvertToHex(convertedVar);
    }
  }
  return convertedVar;
}

function ConvertToArray(variable)
{
  var tmp=[];
  tmp[0] = variable;
  return tmp;
}

//function for the html
var ActivePage = 0;
function start_page()
{
    SetCurrenPageIndex(ActivePage);
}

function start_page2()
{
    SetCurrenPageIndex(ActivePage);
}

function SetCurrenPage(index)
{
	var ShowPage = document.all(pageIDs[index]);
    if(!ShowPage) 
        return;
    
    SetCurrenPageIndex(index);
    var lis = document.getElementById('nav').getElementsByTagName('li');
    for (var i = 0; i < pageIDs.length; ++i)
    {
        if (ActivePage == i)
            lis[i].className = lis[i].className = "selected";
        else
            lis[i].className = lis[i].className.replace(/\bselected\b/g, '');
        var view = document.all(pageIDs[i]);
        if(view) view.style.visibility = 'hidden';
    }
    if(pageSizes[ActivePage]>600)
        document.getElementById('pagebody').style.height = pageSizes[ActivePage];
    else
        document.getElementById('pagebody').style.height = 600;
    ShowPage.style.visibility = 'visible';
}

function SelectTabIndex(tabindex, action)
{
    var strid = "tab"+String(tabindex);
    var obj = document.getElementById(strid);
    var objli = document.getElementById(tab_li_ids[tabindex]);
    
    if(obj)
    {
      if(action)
        obj.style.display = "Block";
      else
        obj.style.display = "None";
    }
    if(objli)
    {
      if(action)
        objli.className ="active-link";
      else
        objli.className = "";
    }
}

function SetCurrenPageIndex(index)
{
    ActivePage = index;
}

function SetCurrenPage(index)
{
    if(ActivePage != index)
    {
       //disable all items
       for(var i=0; i<5; i++)
       {
         SelectTabIndex(i, 0);
       }
	   
    }
    SelectTabIndex(index, 1);
    SetCurrenPageIndex(index);
}

function UpdatePageSize(change)
{
  pageSizes[ActivePage]+=change;
  if (pageSizes[ActivePage]<350)
    pageSizes[ActivePage] = 350;
  if(pageSizes[ActivePage]>600)
    document.getElementById('pagebody').style.height = pageSizes[ActivePage];
  else
    document.getElementById('pagebody').style.height = 600;
}


//------------------------------------------------------------------------------------------------------

var STATUS_OK = 0x01;
var STATUS_CMD_SENDING = 0x02;
var STATUS_WARNING = 0x03;
var STATUS_ERROR = 0x04;
var STATUS_CHANGED = 0x05;

function SetStatusLine(statusId, statusIcon, iconLabel, cmdHandlerObj)
{
  if(cmdHandlerObj)
  {
//    alert(cmdHandlerObj.cmdHandler.commError); todo solve
    if (cmdHandlerObj.commError&& (Number(statusIcon) != STATUS_CHANGED))
      return;
  }
  SetStatusLineRaw(statusId, statusIcon, iconLabel);
}

function SetStatusLineRaw(statusId, statusIcon, iconLabel)
{
    var statusId = statusId + "StatusImg";
    var pImg = Get_pObjectFromID(statusId);
    var imgPath = "img/undetected.svg";
    var statusComments = "Status Error field does not exists"
    if (pImg)
    {
        switch(Number(statusIcon))
        {
            case STATUS_OK:
            {
                imgPath = "img/detected.svg";
                statusComments = iconLabel;
                break;
            }
            case STATUS_CMD_SENDING:
            {
                imgPath = "img/triangle.svg";
                statusComments = iconLabel;
                break;
            }
            case STATUS_WARNING:
            {
                imgPath = "img/triangle.svg";
                statusComments =iconLabel;
                break;
            }       
            case STATUS_ERROR:
            {
                imgPath = "img/undetected.svg";
                statusComments = iconLabel;
                break;
            }       
            case STATUS_CHANGED:
            {
                imgPath = "img/triangle.svgg";
                statusComments =iconLabel;
                break;
            }       
        }
        pImg.src = imgPath; 
        pImg.alt = statusComments;
        pImg.title = statusComments;
    }
}

function BuilStatusIcon(id)
{
  return "ds<IMG src=\"img/Blank.png\" id = \"" + id + "StatusImg\"alt = \"status\">sd";
}

function StatusChanged(objId, cmdHandlerObj)
{
  var alertStr="Value was changed but not uploaded to board";
  var statcode = STATUS_CHANGED;
  SetStatusLine(objId, statcode, alertStr, cmdHandlerObj);
}

function GetRandObjId()
{
  var rnd = Math.random()*(0x100000000);
  return Math.round(rnd);//ConvertToHex(rnd);
}

/******************************************************************************
 * function to create checboxes and connection between checboxes and final value
 *****************************************************************************/

//expected callbacks StatusChanged(objId, cmdHandlerObj)
//expects that objId is id of text, which is connected to checkboxes 
//use UpdateCheckboxesFromVariableRun(divID+ cmdCode, cmdParam1, cmdBytes, 0); on any change of text with id=objId
//expects format_variable_to_store() which convers value in text to real number
//expects format_variable_to_show()  which converts value for text.
//expects Get_pObjectFromID, which returns pointer to object defined by ID
//expects UpdatePageSize(change), which update size of current page
function BuildCheckboxes(objId, bitEnable, bitGroups, bitLabels, objName)
{
  var cmdBytes;
  var string = "<div ><table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">";
  if(typeof(bitGroups) == "string")
    return "";

  //convert to array    
  if(isNaN(bitGroups.length))
  {
    cmdBytes = 1;
    bitGroups = ConvertToArray(bitGroups);
    bitEnable = ConvertToArray(bitEnable);
    bitLabels = ConvertToArray(bitLabels);
  }
  else
  {
    cmdBytes = bitGroups.length;
  }
  
  var i;
  for(i=cmdBytes; i>0; i--)
  {
    var index = i-Number(1);
    var update_event = "onclick=\"ShowBitComments('" + objId + "', '" + i + "')\"";
    var bitsLabel = "Mid";
    if(i==cmdBytes)
    {
      if(cmdBytes == 1)
        bitsLabel = "Bits:";
      else
        bitsLabel = "High:";
    }
    if((i==1) && (cmdBytes!=1))
    {
      bitsLabel = "Low:";
    }
    var bitCount = GetBitFieldCount(bitGroups[index]);
    string = string + "<tr><td>";
    if((bitCount.usedBitsCount > 0) && (bitCount.usedBitsCount < 9))
    {
      string = string + "<div style=\"float:left;\"><input type=\"checkbox\" style=\" width:24px; \" id=\"" + objId + "_" + i + "showBits\" title=\" Shows register bits\" " + update_event + " ></div>";
      string = string + "<div style=\"float:left; width:40px;\">" + bitsLabel + "</div><div style=\"float:left;\">";
        
      string = string + BuildCheckboxLine(objId, i, bitEnable[index], bitGroups[index], bitLabels[index], cmdBytes, objName) + "</div>";  //parameter cmdBytes means to controls second byte in two bytes variables
    }
    string = string + "</td></tr>";
  }
  return string+"</table></div>";
}

function BuildCheckboxesGetOnChangeVal(objId, bitGroups, objName)
{
  if(typeof(bitGroups) == "string")
    return "";

  //convert to array    
  if(isNaN(bitGroups.length))
  {
    bitGroups = ConvertToArray(bitGroups);
  }
  
  var bitCount = GetBitFieldCount(bitGroups[0]);
  var onChange = "";
  if((bitCount.usedBitsCount > 0) && (bitCount.usedBitsCount < 9))
  {
    onChange = "onChange=\"UpdateCheckboxesFromVariableRun('" + objId + "', '" + bitGroups + "', '" + objName + "')\""; //parameter cmdBytes means to controls second byte in two bytes variables
    onChange = onChange + "onKeyPress=\"setTimeout('UpdateCheckboxesFromVariableRun(\\\'" + objId + "\\\', \\\'" + bitGroups + "\\\', \\\'" + objName + "\\\')',1);\"";
  }
  return onChange;
}

function GetBitFieldCount(bitGroups)
{
  var bitFieldCount = 0; // number of bitfields
  var usedBitsCount = 0; // number of bits
  //get number of checkboxes/text_fields idefined in array and count number of controled bits
  for(i=0; i<=7; i++)
  {
    var BitField = (bitGroups>>(i*4))&0xf; 
    if (BitField == 0)
      break;
    bitFieldCount++;
    usedBitsCount += BitField;
  }
  return {bitFieldCount : bitFieldCount, usedBitsCount : usedBitsCount};
}

function ShowBitComments(objId, byteToControl)
{
  var objectID = objId+"_" + byteToControl + "showBits";
  var object = Get_pObjectFromID(objectID);
  if (object)
  {
    var cellID = objId + byteToControl +"bitsCell";
    var x=Get_pObjectFromID(cellID)
        
    var change = 0;
    if (x)
    {
      if(object.checked)
      {
        change = 178; 
        x.style.display = '';
      }
      else
      {
        change = -178;
        x.style.display = 'none';
      }
    }
    UpdatePageSize(change);
  }
}

function BuildCheckboxLine(objId, byteIndex, bitEnable, bitGroups, bitLabels, varSize, objName)
{
  //get number of checkboxes/text_fields idefined in bitGroups and count number of controled bits
  var bitCount = GetBitFieldCount(bitGroups); 

  if((bitCount.usedBitsCount < 1) || (bitCount.usedBitsCount > 8))
  {
    var text = "Ilegal parameter array:" + ConvertToHex(bitCount.usedBitsCount) + " in object:" + fObjectID;
    set_configuration_error(text);
    return "";
  }
  
  //put checkboxes into new table
  var string = "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" align=\"left\"><tr>";
  string = string + "<td valign=\"top\" style=\"padding-left:" + (24*(8-bitCount.usedBitsCount)) + "px;\" >";
  var bitIndex = bitCount.usedBitsCount-1;
  var bitMask = 1<<(bitCount.bitFieldCount-1);
  for (i=0; i<bitCount.bitFieldCount;i++)
  {
    var disabled = "DISABLED";
    if (bitEnable&bitMask)
        disabled = " ";
    var type;
    var title;
    var update_event;
        
    //get amount of bit to control (from number in format 0x11131)
    var actBitCount =((bitGroups>>((bitCount.bitFieldCount-i-Number(1))*4))&0xf);
    //alert("bitIndex " + bitIndex + " bitMask " + bitMask + " actBitCount " + actBitCount);
    
    if ( actBitCount == 1)
    {
      //when one bit, put checbox
      type = "type=\"checkbox\" style=\" width:24px;\"";      
      //title = "bit" + bitIndex;
      title = "bit" + bitIndex + " - " + bitLabels[bitIndex];
      update_event = "onclick=\"UpdateVariableFromChecboxes('" + objId + "', '" + bitGroups + "', '" + byteIndex + "', '" + varSize + "', '" + objName + "')\""; //byteIndex means to controls second byte in two bytes variables
    }
    else
    {
      //when more bits, put text
      type = "type=\"text\" style=\" width:" + actBitCount*23 + "px;\" "
      title = "bit" + bitIndex + "-" + (bitIndex-actBitCount+Number(1));
      var update_event = "onChange=\"UpdateVariableFromChecboxes('" + objId + "', '" + bitGroups + "', '" + byteIndex + "', '" + varSize + "', '" + objName + "')\""; 
      update_event = update_event + "onKeyPress=\"setTimeout('UpdateVariableFromChecboxes(\\\'" + objId + "\\\', \\\'" + bitGroups + "\\\', \\\'" + byteIndex + "\\\', \\\'" + varSize + "\\\', \\\'" + objName + "\\\')',1);\"";
    }
    string = string + "<input " + type + " id=\"" + objId + byteIndex + "_b" + bitIndex + "_l" + actBitCount +"\" title=\"" + title + "\" " + update_event + " " + disabled +" >";
    bitIndex -= actBitCount;
    bitMask = bitMask>>1;
  }
  string = string + "</td><td></td></tr>";
  string = string + "<tr id=\"" + objId + byteIndex + "bitsCell\" style=\"display: none;\">";
  //string = string + "<tr>";
  string = string + BuildCheckboxLabels(bitLabels) + "</tr>";
    
  return string + "</table>";
}

function UpdateCheckboxesFromVariableRun(objId, bitGroups, objName)
{
  // convert bitGroups parameter to array
  var bitArray = [];
  bitArray = bitGroups.split(",");
  var cmdBytes;
  if(isNaN(Number(bitArray[0])+Number(1)))
    return;
  if(isNaN(bitArray.length))
  {
    cmdBytes = 1;
  }
  else
  {
    cmdBytes = bitArray.length;
  }
  var i;
  for(i=0; i<cmdBytes; i++)
  {
    UpdateCheckboxesFromVariable(objId, Number(bitArray[i]), i+Number(1));
  }

  var tmpObj = null;
  if(objName)
  {
    tmpObj = window[objName];
    if(typeof(tmpObj) != "object")
      tmpObj = null;
    else
      tmpObj = tmpObj.cmdHandlerObj
  }
  StatusChanged(objId, tmpObj);
}

function UpdateCheckboxesFromVariable(objId, bitGroups, byteIndex)
{
  var object = Get_pObjectFromID(objId);
  var number = 0; 
  var bitsToControl = byteIndex - Number(1);//converts from 1-4 to 0-3
  //get number of checkboxes/text_fields idefined in bitGroups and count number of controled bits
  var bitCount = GetBitFieldCount(bitGroups);

  //alert("ping from control_checbox: " +  baseName + ", " +  objId + ", " + bitGroups + ", " +  cmdBytes)

  if((bitCount.usedBitsCount < 1) || (bitCount.usedBitsCount > 8))
  {
    var text = "Ilegal parameter bitGroups:" + ConvertToHex(bitGroups) + " in object:" + objId;
    set_configuration_error(text);
    return;
  }
    
  //converts number of bits to bit index 
  var usedBitsCntMax = bitCount.usedBitsCount - 1;
  var bitIndex = usedBitsCntMax;
  if (object) 
  {
    number = format_variable_to_store(object.value);
    //use the right byte from the number
    if(bitsToControl>0)
      number = number>>(8*bitsToControl);
            

    //update all checkboxes/text_fields defined in bitGroups 
    for(i=0;i<bitCount.bitFieldCount;i++)
    {
      var actBitCount =((bitGroups>>((bitCount.bitFieldCount-i-Number(1))*4))&0xf);
      var text = objId.toString() + byteIndex + "_b" + bitIndex + "_l" + actBitCount;
      var value;
            
      //update checbox
      if( actBitCount == 1)
      {
        var mask;
        //big endian/ little endian compensation
        if( get_low_bit_mask() != 0x80)
          mask = 1<<(bitIndex&0x7);
        else
          mask = 1<<(usedBitsCntMax-(bitIndex&0x7));
        
        //get the value for the checkbox
        if ( number & mask )
        {
          value = true;
        }
        else
        {
          value = false;
        }
        
        //update checkbox        
        var object = Get_pObjectFromID(text);
        if (object) object.checked = value;
        else set_object_checkbox_error (text, objId, "");
      }
      else //or update text_field
      {
        var base_mask = (1<<(actBitCount))-1;  //4 actBitCount converts to 0b1111
        //big endian/ little endian compensation
        if ( get_low_bit_mask() != 0x80)
        {
          value = number>>(bitIndex-actBitCount+Number(1));
          value &= base_mask;
        }
        else
        {
          var val = number>>(usedBitsCntMax-(bitIndex&0x7));
          //value = bit_reversal(val, actBitCount);
          value = bit_reversal(val)>>(8-actBitCount);
        }
        var object = Get_pObjectFromID(text);
        if (object) object.value = format_variable_to_show(value, 1);
        else set_object_checkbox_error (text, objId, "");
      }
      bitIndex -= actBitCount;
    }
  }
  else
  {
    set_object_error (objId, objId,"");
  }
}

function UpdateVariableFromChecboxes(objId, bitGroups, byteIndex, varSize, objName)
{
  //get number of checkboxes/text_fields idefined in bitGroups and count number of controled bits
  var bitCount = GetBitFieldCount(bitGroups); 
  //check if configuration is valid
  if((bitCount.usedBitsCount < 1) || (bitCount.usedBitsCount > 8))
  {
    var text = "Ilegal parameter bitGroups:" + ConvertToHex(bitGroups) + " in object:" + objId;
    set_configuration_error(text);
    return;
  }

  //converts number of bits to bit index
  var bitIndexMax = bitCount.usedBitsCount - Number(1);
  var bitIndex = bitIndexMax;
  var finalValue = 0;
  
  //read and convert all checkboxes/text_fields defined in bitGroups to finalValue
  for (i=0;i<bitCount.bitFieldCount;i++)
  {
    var bitCnt =((bitGroups>>((bitCount.bitFieldCount-i-1)*4))&0xf); 
    var mask;
    // prepare mask for final register
    if ( bitCnt == 1)
    {
      if ( get_low_bit_mask() != 0x80)
        mask = 1<<(bitIndex&0x7);
      else
        mask = 1<<(bitIndexMax-(bitIndex&0x7));
    }
    else
    {
      var base_mask = (1<<(bitCnt))-1;  //4 bitCnt converts to 0b1111
      if ( get_low_bit_mask() != 0x80)
        mask = base_mask<<(bitIndex-bitCnt+1);
      else
        mask = base_mask<<(bitIndexMax-(bitIndex&0x7));
    }

    //get state of the checbox/text_field
    var text = objId.toString() + byteIndex + "_b" + bitIndex + "_l" + bitCnt;
    var obj = Get_pObjectFromID(text); 
    //if object is valid, read value and save it to variable
    if (obj)
    {
      //clear the bits before update
      finalValue &= (~mask);
      //when 1 bit-> take data from checbox
      if(bitCnt == 1)
      {
        if (obj.checked)
        {
          finalValue |= mask;
        }
      }
      else
      {
        //data are in text
        if ( get_low_bit_mask() != 0x80)
        {
          finalValue |= ((format_variable_to_store(obj.value)<<(bitIndex-bitCnt+Number(1)))&mask);        
        }
        else
        {
          //endianes conversion
          var val = format_variable_to_store(obj.value);
          var reverse = bit_reversal(val)>>(8-bitCnt);
          finalValue |=  (((reverse)<<(bitIndexMax - (bitIndex&0x7)))&mask);
        }
      }
    }
    else
    {
      set_object_checkbox_error (text, objId, ""); //todo define default debug function
      break;
    }
    bitIndex-=bitCnt;
  }

  //save final value
  var object = Get_pObjectFromID(objId);
  if (object)
  {
    var tmpObj = null;
    if(objName)
    {
      tmpObj = window[objName];
      if(typeof(tmpObj) != "object")
        tmpObj = null;
      else
        tmpObj = tmpObj.cmdHandlerObj
    }
    StatusChanged(objId, tmpObj);
    
    byteIndex--;
    var actVal = object.value&(0xffffffff^(0xff<<(8*byteIndex)));
    actVal |= finalValue<<(8*byteIndex);
    object.value  = format_variable_to_show(actVal, varSize);
  }
  else 
  {
    //todo solve the error report
    set_object_error (objId, FormID, "");  
  }
}

/******************************************************************************
 * function to create content of the page
 *****************************************************************************/
function build_gui_line_header(cmdLabel)
{
    var string = "<tr>";
    var onchange = ""; 
    string = string + "<td align=\"left\" valign=\"top\" width=35>" + " </td>";
    string = string + "<td align=\"left\" valign=\"top\" width=150 style=\"font-weight: bold\"> " + cmdLabel + " </td>";
    string = string + "<td valign=\"top\" width=120>";
    
    string = string + "</td><td valign=\"top\"></td><td>";
    
    string = string + "</td></tr>";
    return string;
}

function build_gui_line_common_obj(objLabel, buttonConf, buttonLabel, objType)
{
  var string = "<tr>";
  var objId = GetRandObjId();
  string = string + "<td align=\"left\" valign=\"top\" width=10>" + " </td>";
  string = string + "<td align=\"left\" valign=\"top\" width=150> " + objLabel + " </td>";
  string = string + "<td valign=\"top\" width=110>";

  var args = [];
  args[0] = objId;
  var i;
  var parameters="'"+objId+"'";
  for(i=1;i<(arguments.length-3); i++)
  {
    args[i] = arguments[i+Number(3)];
    parameters+=",'" + arguments[i+Number(3)]+ "'";
  }
  var objTypeName = "";
/*  if (typeof objType != "object" || objType === null) return string+" invalid parameter objType" + "</td></tr>";
  else objTypeName = /(\w+)\(/.exec(objType.constructor.toString())[1];*/
  
  objTypeName = objType.objName;
  //put buttons
  if(buttonConf&0x1)
  {
    string = string + "<input type=\"button\" style=\"width:50px;\" value=\"" + buttonLabel[0] + "\" onclick=\""+objTypeName+".Button0("+parameters+")\">";
  }
        
  if(buttonConf&0x2)
  {
    if(buttonConf&0x1)
      string = string + " <input type=\"button\" style=\"width:50px;\" value=\"" + buttonLabel[1] + "\" onclick=\""+objTypeName+".Button1("+parameters+")\">";
    else
      string = string + " <input type=\"button\" style=\"width:50px;\" value=\"" + buttonLabel[0] + "\" onclick=\""+objTypeName+".Button1("+parameters+")\">";
  }
  string = string + "</td><td valign=\"top\">"+BuilStatusIcon(objId)+"</td><td>";

  string=string+objType.BuildLine.apply(null,args );
  string = string + "</td></tr>";
  return string;
}

function build_gui_line_common_objnn(objType, objId, objLabel, buttonConf, buttonLabel)
{
  var string = "<tr>";
  string = string + "<td align=\"left\" valign=\"top\" width=10>" + " </td>";
  string = string + "<td align=\"left\" valign=\"top\" width=150> " + objLabel + " </td>";
  string = string + "<td valign=\"top\" width=110>";

  var args = [];
  args[0] = objId;
  var i;
  var parameters="'"+objId+"'";
  for(i=1;i<(arguments.length-4); i++)
  {
    args[i] = arguments[i+Number(4)];
    parameters+=",'" + arguments[i+Number(4)]+ "'";
  }
  var objTypeName = "";
  
  objTypeName = objType.objName;
  //put buttons
  if(buttonConf&0x1)
  {
    string = string + "<input type=\"button\" style=\"width:50px;\" value=\"" + buttonLabel[0] + "\" onclick=\""+objTypeName+".Button0("+parameters+")\">";
  }
        
  if(buttonConf&0x2)
  {
    if(buttonConf&0x1)
      string = string + " <input type=\"button\" style=\"width:50px;\" value=\"" + buttonLabel[1] + "\" onclick=\""+objTypeName+".Button1("+parameters+")\">";
    else
      string = string + " <input type=\"button\" style=\"width:50px;\" value=\"" + buttonLabel[0] + "\" onclick=\""+objTypeName+".Button1("+parameters+")\">";
  }
  string = string + "</td><td valign=\"top\">"+BuilStatusIcon(objId)+"</td><td>";

  string=string+objType.BuildLine.apply(objType,args );
  string = string + "</td></tr>";
  return string;
}

function executeButton0Handler(pObj)
{
    var args = [];
    for(i=0;i<(arguments.length); i++)
    {
      args[i] = arguments[i];
    }
    alert(pObj.Init());
     pObj = args.shift();

    pObj.Button0.apply(pObj,args);

}

function executeButton1Handler()
{
    var args = [];
    for(i=0;i<(arguments.length); i++)
    {
      args[i] = arguments[i];
    }
    var pObj = args.shift();
    alert(pObj.Button1);

    pObj.Button1.apply(pObj,args);
}
