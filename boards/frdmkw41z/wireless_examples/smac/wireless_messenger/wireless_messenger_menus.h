/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
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
