/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

var CMD_HANDLER_CFG_COMPATIBILITY = 0
var CMD_HANDLER_CFG_OBJ = 1
var CMD_HANDLER_SEND = 1;
var CMD_HANDLER_PROGRESS = 2;
var CMD_HANDLER_RECEIVED = 3;


//function coverts Array to SafeArray
function getSafeArray(jsArr) {
  var dict = new ActiveXObject("Scripting.Dictionary");
  for (var i = 0; i < jsArr.length; i++)
    dict.add(i, jsArr[i]);
  return dict.Items();
}

function BuildButtonContext(pCallbackFunct, args)
{
  //create context array 
  var finalArgs = new Array();
  var type = typeof(pCallbackFunct)
  if(type == 'function')
  {
    var name = pCallbackFunct
    var reg = /function ([^\(]*)/;
    var funct = reg.exec(name)[0];
    finalArgs[0] = funct
  }
  else if((CMD_HANDLER_CFG_OBJ)&& (type == 'object'))
  {
    finalArgs[0] = pCallbackFunct;
  }
  else
  {
    finalArgs[0] = null;
  }
  //convert argumnets to array
  for (var i = 0; i < args.length; i++)
  {
    finalArgs[i+Number(1)] = args[i];
  }
  return finalArgs;
}

function GetSizeFromType(varType)
{
  //check if parameter is valid number
  if(!isNaN(varType))
    return varType;
  //decode size from string
  if ((varType == "UINT8") || (varType == "SINT8"))
    return 1;
  else if ((varType == "UINT16") || (varType == "SINT16") || (varType == "UFRAC16") || (varType == "SFRAC16"))
    return 2;
  else if ((varType == "UINT32") || (varType == "SINT32") || (varType == "FLOAT") || (varType == "UFRAC32") || (varType == "SFRAC32"))
    return 4;
  else if ((varType == "UINT64") || (varType == "SINT64") || (varType == "DOUBLE"))
    return 8;
  else
    alert("GetSizeFromType(varType):Unsupported type of variable:"+varType);
};

function GetSignFromType(varType)
{
  //negative size is sign
  if(!isNaN(varType))
  {
    if(varType<0)
      return true;
    else
      return false;
  }
  //return tru when signed type
  if ((varType == "SINT8") || (varType == "SINT16") || (varType == "SFRAC16") || (varType == "SINT32") || (varType == "SFRAC32") || (varType == "SINT64") )
    return true;
  else 
    return false;
};

function IsIntXFromType(varType)
{
  //check if parameter is valid number
  if(!isNaN(varType))
    return false;
  //decode size from string
  if ((varType == "UINT8") || (varType == "SINT8") || (varType == "UINT16") || (varType == "SINT16") || (varType == "UINT32") || (varType == "SINT32") || (varType == "UINT64") || (varType == "SINT64"))
    return true;
  else
    return false;
};

function toFloat32(value) {
    var bytes = 0;
    var sign = 0;
    switch (value) {
        case Number.POSITIVE_INFINITY: bytes = 0x7F800000; break;
        case Number.NEGATIVE_INFINITY: bytes = 0xFF800000; break;
        case +0.0: bytes = 0x40000000; break;
        case -0.0: bytes = 0xC0000000; break;
        default:
            if (isNaN(value)) { bytes = 0x7FC00000; break; }

            if (value <= -0.0) {
                sign = 0x80000000;
                value = -value;
            }
            var exponent = Math.floor(Math.log(value) / Math.log(2));
            var significand = ((value / Math.pow(2, exponent)) * 0x00800000) | 0;

            exponent += 127;
            if (exponent >= 0xFF) {
                exponent = 0xFF;
                significand = 0;
            } else if (exponent < 0) exponent = 0;

            bytes = bytes | (exponent << 23);
            bytes = bytes | (significand & ~(-1 << 23));
            bytes = bytes | sign;
        break;
    }
    return bytes;
};

function toFloat64(value) {
    if ((byteOffset + 8) > this.byteLength) 
        throw "Invalid byteOffset: Cannot write beyond view boundaries.";

    var hiWord = 0, loWord = 0;
    switch (value) {
        case Number.POSITIVE_INFINITY: hiWord = 0x7FF00000; break;
        case Number.NEGATIVE_INFINITY: hiWord = 0xFFF00000; break;
        case +0.0: hiWord = 0x40000000; break;
        case -0.0: hiWord = 0xC0000000; break;
        default:
            if (Number.isNaN(value)) { hiWord = 0x7FF80000; break; }

            if (value <= -0.0) {
                hiWord = 0x80000000;
                value = -value;
            }

            var exponent = Math.floor(Math.log(value) / Math.log(2));
            var significand = Math.floor((value / Math.pow(2, exponent)) * Math.pow(2, 52));

            loWord = significand & 0xFFFFFFFF;
            significand /= Math.pow(2, 32);

            exponent += 1023;
            if (exponent >= 0x7FF) {
                exponent = 0x7FF;
                significand = 0;
            } else if (exponent < 0) exponent = 0;

            hiWord = hiWord | (exponent << 20);
            hiWord = hiWord | (significand & ~(-1 << 20));
        break;
    }

    return [hiWord, loWord];
};

function Bytes2Float32(bytes) {
    var sign = (bytes & 0x80000000) ? -1 : 1;
    var exponent = ((bytes >> 23) & 0xFF) - 127;
    var significand = (bytes & ~(-1 << 23));

    if (exponent == 128) 
        return sign * ((significand) ? Number.NaN : Number.POSITIVE_INFINITY);

    if (exponent == -127) {
        if (significand == 0) return sign * 0.0;
        exponent = -126;
        significand /= (1 << 22);
    } else significand = (significand | (1 << 23)) / (1 << 23);

    return sign * significand * Math.pow(2, exponent);
}

function VariableToRaw(varVal, length)
{
  var finalVal = varVal;
  if(isNaN(length))
  {
    var varType = length;

    length = GetSizeFromType(varType);
    if((varType == "UFRAC16") || (varType == "SFRAC16") || (varType == "UFRAC32") || (varType == "SFRAC32"))
    { 
      var finalValTmp = varVal;
      if(((varType == "SFRAC16") || (varType == "SFRAC32"))&&(varVal<0))
        finalValTmp=Number(2)+Number(varVal);
      if(finalValTmp<0)
        finalValTmp = 0;
      if(finalValTmp>=2)
        finalValTmp = 2;
      var mask = (Math.pow(2, (8*length))-1);
      var max =Math.pow(2, (8*length)-1);
      finalVal = Math.floor(finalValTmp*max);
//      alert(finalValTmp+"  "+finalVal+"  "+max+" "+mask)
    }
    if(varType == "FLOAT")
    {
      finalVal = toFloat32(varVal);
    }
/*    if((varType == "SINT8") || (varType == "SINT16") || (varType == "SINT32") || (varType == "SINT64"))
    {
      if(varVal<0)
      {
        finalVal = Math.pow(2, 8*length)-finalVal;
      }    
    }*/
  }
  else
  {

  }  
  return finalVal;
}

function VariableToArray(varVal, length)
{
  var varType = length;
  if(isNaN(length))
  {
    length = GetSizeFromType(varType);
  }

  var finalVal = VariableToRaw(varVal, varType);
  var arrayVar = [];
  arrayVar = longToByteArray(finalVal, length)
  return arrayVar;
}

function longToByteArray(/*long*/long, length) {
    // we want to represent the input as a 8-bytes array
    var i = length;
    if(isNaN(length))
      i = 8;
    var byteArray = [];
    while (i--) {
      byteArray[i] = 0;
    }
    for ( var index = 0; index < byteArray.length; index ++ ) {
        var byte = long & 0xff;
        byteArray [ index ] = byte;
        long = (long - byte) / 256 ;
    }
    return byteArray;
};

function byteArrayToLong(/*byte[]*/byteArray, /*length of buffer*/length) {
    var value = 0;
    for ( var i = length - 1; i >= 0; i--) {
        value = (value * 256) + byteArray[i];
    }
    return value;
};

function VariableToRawNvm(value, length, signed)
{
  var varType = length;
  if(isNaN(varType))
  {
    if((varType == "UFRAC16") || (varType == "SFRAC16") || (varType == "UFRAC32") || (varType == "SFRAC32"))
    {
      var tmp = VariableToRaw(value, varType);
      if(GetSignFromType(varType))
      {
        var sz = GetSizeFromType(varType);
        var msbMask = Math.pow(2,((8*sz)-1));
        var max = Math.pow(2, ((8*sz)));
        if(msbMask&tmp)
          return (tmp-max);
      }
      return tmp
    }
    else if(varType == "FLOAT")
    {
      return value;
    }
    if(value<0)
      return value;
    return RawToVariable(value, varType, signed);
  }
  return value;
}

function RawNvmToVariable(value, length, signed)
{
  var varType = length;
  if(isNaN(varType))
  {    
//    if((varType == "UFRAC16") || (varType == "SFRAC16") || (varType == "UFRAC32") || (varType == "SFRAC32"))
    {
//      return RawToVariable(value, varType, signed);
    }
//    else if(varType == "FLOAT")
    if(varType == "FLOAT")
    {
      return value;
    }
    return RawToVariable(value, varType, signed);
  }
  return value;
}


/*****************************************************************************/
/*! @addtogroup cmd_handler_api_html 
* @{*/
/*****************************************************************************
*//*!
* @brief            This function converts variable from RAW value to final formated variable. 
* @param[in]        value       Raw value of the variable.
* @param[in]        length      Length of the value (1-8) or one of .
* @param[in]        cmdParam4   cmdType specific agruments
*
* @return           This function returns formated value of the variable.

* @hideinitializer
* @internal
* @version          22-Sep-2014
******************************************************************************/
function RawToVariable(value, length, signed)
{
  var finalVal = value;
  var varType = length;
  if(isNaN(length))
  {
    length = GetSizeFromType(varType);
    signed = GetSignFromType(varType);
  }
  var convVal = Math.pow(2, (8*length));

  if(isNaN(varType))
  {
    if((varType == "UFRAC16") || (varType == "SFRAC16") || (varType == "UFRAC32") || (varType == "SFRAC32"))
    {
      var max = (Math.pow(2, (8*length)));
      if(value<0)
        value = Number(value)+max;

      finalVal = value/(Math.pow(2, (8*length)-1))
      var tmp = finalVal;
      if(tmp<0)
        finalVal = 0;
      if(tmp>=2)
        finalVal = 2;

      var tst=finalVal-Number(2);
      if((signed)&&(value>(Math.pow(2, (8*length)-1))))
        finalVal = tst;
    }
    else if(varType == "FLOAT")
    {
      finalVal = Bytes2Float32(value);
    }
    else
    {
      finalVal = value;
      if(signed&&(((Math.pow(2, (8*length)-1)))&finalVal)&&(finalVal>0))
      {
        finalVal = Number(value)-Number(convVal);
      }
    }    
  }
  else
  {
    finalVal = value;
    if(signed&&(((Math.pow(2, (8*length)-1)))&finalVal)&&(finalVal>0))
    {
      finalVal = Number(value)+Number(convVal);
    }
  }
  return finalVal
}
/*! @} End of cmd_handler_api_html */

function ArrayToVariable(array, length, signed)
{
  var value = array[0]&0xff;
  var finalVal;
  var varType = length;
  var sign = 0;
  if(isNaN(length))
  {
    length = GetSizeFromType(varType);
    sign = GetSizeFromType(varType);
  }

  value = byteArrayToLong(array, length);
/*  var signMask = Math.pow(2, ((8*length)-1));
  if(sign&&(signMask&value))
  {
    var max = Math.pow(2, ((8*length)));
    value = value-max;
  }
  */
  finalVal = RawToVariable(value, varType, signed)
  return finalVal;
}


function CmdHandler(fmstrObj, data, pStatDecoder)
{
  this.fmstrObj = fmstrObj;
  this.data = data;
  this.commError = false;
  this.buttonHandlerCnt = 0;
  this.statusTimeout = 0;
  this.twoVars = 0;
  this.StatDecoder = pStatDecoder
  this.buttonHandlerIdle = function()
  {
    return this.buttonHandlerCnt == 0;
  };
  this.SetStatusWithTimeout = function(statusId, statusIcon, iconLabel)
  {
    if(this.statusTimeout)
      clearTimeout(this.stausTimeout);
    var that = this;
    this.statusTimeout = setTimeout(function () {
      SetStatusLineRaw(statusId, statusIcon, iconLabel);
    }, 250);
  };
  this.ClearCommError = function()
  {
    //check if old version
    if((CMD_HANDLER_CFG_COMPATIBILITY)&&(this.twoVars==0))
      this.twoVars = this.fmstrObj.ReadMemory("wcttxDebugStatus", 2);
    this.commError = false;
  };
  this.WriteGuiBuffer = function(statusId, cmd, buffer, length)
  {
    if (this.commError)
      return true;

    if (!this.buttonHandlerIdle())
    {
      return true;
    }
    else
    {   
      var WriteLength = parseInt(length) + parseInt(2);
      var buff = buffer;
      // add cmd at the begin of the buffer
      if(this.twoVars)
      {
        WriteLength--;
        buff.unshift(0);
        buff.unshift(cmd);
      }
      else
      {
        buff.unshift(cmd);      
        buff.unshift(0);
      }
      //todo check the buffer size
      st = this.fmstrObj.WriteMemory(this.data, WriteLength, getSafeArray(buff));
      if(st)
      {
        return false;
      }
      else
      {
        SetStatusLineRaw(statusId, STATUS_ERROR, "Cannot write to \""+this.data+"\" variable. Please check FreeMASTER communication or FreeMASTER project.");
        this.commError = true;
        return true;
      }
    }
  };
  this.SendGuiCmd = function(statusId, comment, buttonContext)
  {
    if (this.commError)
      return true;
    if (this.buttonHandlerIdle())
    {
      var varName;
      var varLength = 0;
      if(this.twoVars)
      {
        varName = "wcttxDebugStatus";
        varLength = 2;
      }
      else
      {
        varName = this.data;
        varLength = 1;
      }
      var array = VariableToArray(CMD_HANDLER_SEND, varLength);
      if(this.fmstrObj.WriteMemory(varName, varLength, getSafeArray(array)))
      {
        SetStatusLineRaw(statusId, STATUS_CMD_SENDING, "Executing command:" + comment);
        this.buttonHandlerCnt = 20;   //2s timeout
        //is context valid?
        if(buttonContext)
        {
          this.ButtonHandler.apply(this,buttonContext);
        }
        else
        {
          //not valid, callback function will not be called, statusId is used for status image.
          var args = NewArrayOf(0, 2);
          args[1] = statusId;
          this.ButtonHandler.apply(this,args);
        }
      }
      else
      {
        SetStatusLineRaw(statusId, STATUS_ERROR, "Cannot write to \""+varName+"\" variable. Please check FreeMASTER communication or FreeMASTER project.");
        this.commError = true;
        return true;
      }
      return false;
    }
    else
    {
      SetStatusLineRaw(statusId, STATUS_WARNING, "Request to send is canceled, because another command is performing");
      return true;
    }
  };
  this.ButtonHandler = function ()
  {
    var pCallbackFunct = arguments[0];
    var objId = arguments[1];
    
    if(this.buttonHandlerCnt > 100)
      this.buttonHandlerCnt = 100;  //10s is maximum timeout
    
    if((this.buttonHandlerCnt > 1))
    {
      this.buttonHandlerCnt--;
      if(this.buttonHandlerCnt > 0)
      {
        //get status from board
        var packetStatus = 0;
        var packetState = 0;
        var varName;
        if(this.twoVars)
          varName = "wcttxDebugStatus";
        else
          varName = this.data;
        if(this.fmstrObj.ReadMemory(varName, 2))
        {
          var arr = this.fmstrObj.LastMemory_data;
          var temp = arr.toArray();
          packetStatus = temp[1];
          packetState = temp[0];
        }
        else
        {
          if(!this.commError)
            this.SetStatusWithTimeout(objId, STATUS_ERROR, "Cannot read from \""+ varName +"\" variable. Please check FreeMASTER communication or FreeMASTER project.");
          this.commError = true;
          return;
        }

        //wait for response from the board.
        if(packetState == CMD_HANDLER_RECEIVED)
        {
          if(packetStatus != 0)
          {
            var stMsg = "";
            var type = typeof(pStatDecoder)
            if ((pStatDecoder)&&(type == 'function'))
              stMsg = pStatDecoder(packetStatus);
            //error code from board
            if(!this.commError)
              this.SetStatusWithTimeout(objId, STATUS_ERROR, "Error Code from Board: " + stMsg);
          }
          else
          {
            //Read was succesfull
            if(!this.commError)
              this.SetStatusWithTimeout(objId, STATUS_OK, "Command was send and received response");
          }
          if(packetStatus == 0)
          {
            var pButtonHandlerFunct;
            var type = typeof(pCallbackFunct)
            var classObj=null;

          if(type == 'function')
          {
            pButtonHandlerFunct = GetFunct(pCallbackFunct);
          }
          else if((CMD_HANDLER_CFG_OBJ)&&(type == 'object')) 
          {
            pButtonHandlerFunct = pCallbackFunct.Callback;
            classObj = pCallbackFunct;
          }

          if(pButtonHandlerFunct)
          {
            var finalArgs = [];
            //remove pCallbackFunct from arguments
            for (var i = 0; i < (arguments.length-1); i++)
            {
              finalArgs[i] = arguments[i+Number(1)];
            }
            status = pButtonHandlerFunct.apply(classObj,finalArgs);
            var stIcon = Number(status.substr(0, 1)); // first character is number, which defines the status icon
            if(isNaN(stIcon))
            {
              stIcon = STATUS_ERROR;
            
            }
            else
            {
              status = status.substring(1)
            }
            if(status)
            {
              //error code from callback function
              if(!this.commError)
                this.SetStatusWithTimeout(objId, stIcon, status);
            }
          }
          }
          this.buttonHandlerCnt = 0;
        }
        else
        {
          if(packetState == CMD_HANDLER_PROGRESS)
          {
            SetStatusLineRaw(objId, STATUS_CMD_SENDING, "Sending command and receiving response");
          }
          //convert arguments to array
          var finalArgs = [];
          for (var i = 0; i < (arguments.length); i++)
          {
            finalArgs[i] = arguments[i];
          }

          var that = this;
          setTimeout(function () {
            that.ButtonHandler.apply(that,finalArgs );
          }, 100);
        }
      }
    }
    else
    {
      this.buttonHandlerCnt = 0;
      if(!this.commError)
        this.SetStatusWithTimeout(objId, STATUS_ERROR, "Board is not responsing, timeout error: 5s");
    }
  };
  this.ReadGuiBuffer = function(statusId, length)
  {
    if(this.commError)
      return{rxLength : 0, buffer : 0};;

    var buffer;
    if(this.fmstrObj.ReadMemory(this.data, parseInt(length) + parseInt(2)))
    {
      var arr = this.fmstrObj.LastMemory_data;
      buffer = arr.toArray();
      if(!this.twoVars)
      {
        //remove 2 bytes of status
        buffer.shift();
        buffer.shift();
      }
    }
    else
    {
      SetStatusLineRaw(statusId, STATUS_ERROR, "Cannot read from \""+this.data+"\" variable. Please check FreeMASTER communication or FreeMASTER project.");
      this.commError = true;
      return{rxLength : 0, buffer : 0};;
    }
    return {rxLength : length, buffer : buffer};
  };
  this.Fmstr_WriteVarRaw = function(objId, varName, varVal)
  {
    var st=0;
    //alert(varName+"   "+varVal); //todo remove
    if (pcm.WriteVariable(varName, varVal))
    {
      SetStatusLineRaw(objId, STATUS_OK, "Write of \n" + varName + " variable was succesfull");
      st=1;
    }
    else
    {
      SetStatusLineRaw(objId, STATUS_ERROR, "Cannot write value of "+ varName + " variable, Please check FreeMASTER communication and make sure, that in board is running FreeMASTER exerciser code");    
    }
    return st;
  };
  this.ReadMemArray = function(objId, varName, length)
  {
    var value = 0;
    SetStatusLineRaw(objId, STATUS_CMD_SENDING, "Reading from variable "+varName);

    if(this.fmstrObj.ReadMemory(varName, length))     //read immediately
    {
      var tmp = this.fmstrObj.LastMemory_data;
      value = tmp.toArray();
      this.SetStatusWithTimeout(objId, STATUS_OK, "Read of \n" + varName + " variable was succesfull");
    }
    else 
    {
      this.SetStatusWithTimeout(objId, STATUS_ERROR, "Cannot read value of \n"+ varName + " variable. \nPlease check FreeMASTER communication and make sure, that in board is running FreeMASTER exerciser code");
    }
    return value;
  };
  this.WriteMemArray = function(objId, varName, arrayVar, length)
  {
    var st = true;
    SetStatusLineRaw(objId, STATUS_CMD_SENDING, "Writing to variable "+varName);
    if (this.fmstrObj.WriteMemory(varName, length, getSafeArray(arrayVar)))
    {
        this.SetStatusWithTimeout(objId, STATUS_OK, "Write to \n" + varName + " variable was succesfull");
        st=false;
    }
    else
    {
        this.SetStatusWithTimeout(objId, STATUS_ERROR, "Cannot write value to "+ varName + " variable, Please check FreeMASTER communication and make sure, that in board is running FreeMASTER exerciser code");    
    }
    return st;
  };
}
