/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "connectivity_test_platform.h"

/************************************************************************************
*************************************************************************************
* Macros
*************************************************************************************
************************************************************************************/
#define gCTSelf_EVENT_c (1<<7)
#define SelfNotificationEvent() (OSA_EventSet(&gTaskEvent, gCTSelf_EVENT_c));

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
const registerLimits_t registerIntervals[] = 
{
  {.regStart=0x00 , .regEnd=0x00, FALSE}
};

uint8_t u8Prbs9Buffer[] =
{
  0x42,
  0xff,0xc1,0xfb,0xe8,0x4c,0x90,0x72,0x8b,0xe7,0xb3,0x51,0x89,0x63,0xab,0x23,0x23,  
  0x02,0x84,0x18,0x72,0xaa,0x61,0x2f,0x3b,0x51,0xa8,0xe5,0x37,0x49,0xfb,0xc9,0xca,
  0x0c,0x18,0x53,0x2c,0xfd,0x45,0xe3,0x9a,0xe6,0xf1,0x5d,0xb0,0xb6,0x1b,0xb4,0xbe,
  0x2a,0x50,0xea,0xe9,0x0e,0x9c,0x4b,0x5e,0x57,0x24,0xcc,0xa1,0xb7,0x59,0xb8,0x87
};
/************************************************************************************
*************************************************************************************
* Private function prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*
* InitApp_custom
*
************************************************************************************/
void InitApp_custom()
{
}

/************************************************************************************
*
* InitProject_custom
*
************************************************************************************/
void InitProject_custom()
{
#if CT_Feature_Xtal_Trim
  AppToAspMessage_t msg;
  
  msg.msgType = aspMsgTypeGetXtalTrimReq_c;
  xtalTrimValue = APP_ASP_SapHandler(&msg, 0);
#endif
}

/************************************************************************************
*
* PrintTestParameters
*
************************************************************************************/
void PrintTestParameters(bool_t bEraseLine)
{
  uint8_t u8lineLen = 80;
  uint8_t u8Index;
  
  if(bEraseLine)
  {
    Serial_Print(mAppSer, "\r", gAllowToBlock_d);
    for(u8Index = 0;u8Index<u8lineLen;u8Index++)
    {
      Serial_Print(mAppSer, " ", gAllowToBlock_d);
    }
    Serial_Print(mAppSer ,"\r", gAllowToBlock_d);
  }
  
  Serial_Print(mAppSer, "Mode ", gAllowToBlock_d);
  if(mTxOperation_c == testOpMode)
  {
    Serial_Print(mAppSer, "Tx", gAllowToBlock_d);
  }
  else
  {
    Serial_Print(mAppSer, "Rx", gAllowToBlock_d);
  }
  Serial_Print(mAppSer, ", Channel ", gAllowToBlock_d);
  Serial_PrintDec(mAppSer, (uint32_t)testChannel);
  Serial_Print(mAppSer,", Power ", gAllowToBlock_d);
  Serial_PrintDec(mAppSer,(uint32_t)testPower);
  Serial_Print(mAppSer,", Payload ", gAllowToBlock_d);
  Serial_PrintDec(mAppSer, (uint32_t)testPayloadLen);
  Serial_Print(mAppSer,", CCA Thresh ", gAllowToBlock_d);
  if(ccaThresh != 0)
  {
    Serial_Print(mAppSer,"-",gAllowToBlock_d);
  }
  Serial_PrintDec(mAppSer, (uint32_t)ccaThresh);
  Serial_Print(mAppSer,"dBm",gAllowToBlock_d);
  
#if CT_Feature_Xtal_Trim
  Serial_Print(mAppSer,", XtalTrim ", gAllowToBlock_d);
  Serial_PrintDec(mAppSer, (uint32_t)xtalTrimValue);
#endif
  
  Serial_Print(mAppSer," >", gAllowToBlock_d);
}

/************************************************************************************
*
* ShortCutsParser
*
* Performs changes in different menus whenever shortcuts are allowed
*
************************************************************************************/
void ShortCutsParser(uint8_t u8UartData)
{
  evTestParameters = TRUE;
  evDataFromUART = FALSE;
  switch(u8UartData){
  case 't':
    testOpMode = mTxOperation_c;
    break;
  case 'r':
    testOpMode = mRxOperation_c;
    break;
  case 'q': 
    if(testChannel == (channels_t)gTotalChannels)
    {
      testChannel = gChannel11_c;
    }
    else
    {
      testChannel++;
    }
    break;
  case 'w':
    if(testChannel == gChannel11_c)
    {
      testChannel = (channels_t)gTotalChannels;
    }
    else
    {
      testChannel--;
    }
    break;
  case 'a':
    testPower++;
    if(gMaxOutputPower_c < testPower)
    {
      testPower = gMinOutputPower_c;
    }
    break;
  case 's':
    if(testPower == gMinOutputPower_c)
    {
      testPower = gMaxOutputPower_c;
    }
    else
    {
      testPower--;	
    }
    break;
  case 'n':
    testPayloadLen++;
    if(gMaxSmacSDULength_c < testPayloadLen)
    {
      testPayloadLen = 17;
    }    
    break;
  case 'm':
    testPayloadLen--;
    if(17 > testPayloadLen)
    {
      testPayloadLen = gMaxSmacSDULength_c;
    }    
    break;
  case 'k':
    ccaThresh++;
    if(ccaThresh > gMaxCCAThreshold_c)
    {
      ccaThresh = gMinCCAThreshold_c;
    }
    break;
  case 'l':
    ccaThresh--;
    if(ccaThresh > gMaxCCAThreshold_c)
    {
      ccaThresh = gMaxCCAThreshold_c;
    }
    break;
#if CT_Feature_Xtal_Trim
  case 'd':
    xtalTrimValue++;
    if(xtalTrimValue > gMaxTrimValue_c)
    {
      xtalTrimValue = gMinTrimValue_c;
    }
    break;
  case 'f':
    if(xtalTrimValue == gMinTrimValue_c)
    {
      xtalTrimValue = gMaxTrimValue_c;
    }
    else
    {
      xtalTrimValue--;
    }
#endif
    break;
  default:
    evDataFromUART = TRUE;
    evTestParameters = FALSE;
    break;
  }
}