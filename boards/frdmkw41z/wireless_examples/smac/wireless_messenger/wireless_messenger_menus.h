/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
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

#include "EmbeddedTypes.h"          /*Include special data types*/             
#include "SerialManager.h"
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern char * const cu8Logo[];
extern char * const cu8MainMenu[]; 
extern char * const cu8ShortCutsBar[];
extern char * const cu8ConfigMenu[];
/************************************************************************************
*************************************************************************************
* Module macros
*************************************************************************************
************************************************************************************/

#define isAsciiHex(Data) (((Data >= '0') && (Data <= '9')) \
                       || ((Data >= 'A') && (Data <= 'F')) \
                       || ((Data >= 'a') && (Data <= 'f'))) 
                       
/************************************************************************************
*************************************************************************************
* Module type definitions
*************************************************************************************
************************************************************************************/
typedef enum WMStates_tag
{
  gWMInitState_c,
#if gSmacUseSecurity_c
  gWMSec_c,
#endif
  gWMIdleState_c,
  gWMSelectOption_c,
  gWMConfiguration_c,
  gWMConsole_c,
  gWMMaxState_c
}WMStates_t;

#if gSmacUseSecurity_c
typedef enum SecurityMenuStates_tag
{
  gSecurityMenuInit_c,
  gSecurityMenuInputKEY_c,
  gSecurityMenuInputIV_c,
  gSecurityMenuMaxStates_c
}SecurityMenuStates_t;
#endif
/************************************************************************************/ 
typedef enum ConfigMenuStates_tag
{
  gConfigMenuInit_c,
  gConfigMenuSelectOption_c,
  gConfigMenuInputPan_c,
  gConfigMenuInputSrc_c,
  gConfigMenuInputDst_c,
  gConfigMenuInputValue_c,
  gConfigMenuEnd_c,
  gConfigMenuMaxState_c
}ConfigMenuStates_t;

/************************************************************************************/
typedef enum ConsoleMenuStates_tag
{
  gConsoleMenuInit_c,
  gConsoleMenuWaitEv_c,
  gConsoleMenuPrepare_c,
  gConsoleMenuSendMessage_c,
  gConsoleMenuPrintMessage_c,
  gConsoleMenuWaitingConfirmation_c,
  gConsoleMenuEnd_c,
  gConsoleMaxState_c
}ConsoleMenuStates_t;

/************************************************************************************
*************************************************************************************
* Interface functions prototypes
*************************************************************************************
************************************************************************************/
void PrintMenu(char * const pu8Menu[], uint8_t port);
