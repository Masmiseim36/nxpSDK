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

#ifndef __SMAC_APPLICATION_CONF_H__
#define __SMAC_APPLICATION_CONF_H__

#include "fsl_common.h"
#include "MemManager.h"
#include "low_power_node_menus.h"      /*Defines the Application menus*/          
#include "SerialManager.h"
#include "LED.h"                    /*Include all LED functionality*/ 
#include "Keyboard.h"
#include "TimersManager.h"
#include "PhyInterface.h"
#include "SMAC_Interface.h"     
#include "FunctionLib.h"
#include "AspInterface.h"
#include "MKW41Z4.h"
#include "fsl_os_abstraction.h"
#include "board.h"

#include "fsl_clock.h"
#include "fsl_tpm.h"
#include "TMR_Adapter.h"
#include "fsl_trng.h"
#include "PWRLib.h"
#include "PWR_Platform.h"
/* BEGIN Configuration Parameters Definition */ 

#ifndef gMaxOutputPower_c
#define gMaxOutputPower_c	       ( 0x0F ) 
#endif

#ifndef gMinOutputPower_c
#define gMinOutputPower_c              ( 0x00 )
#endif

#ifndef gDefaultOutputPower_c
#define gDefaultOutputPower_c          ( (gMaxOutputPower_c + gMinOutputPower_c) >> 1 )
#endif

#ifdef gPHY_802_15_4g_d
#define gDefaultChannelNumber_c         gChannel0_c
#define gMaxChannel_c		        ((channels_t)(gTotalChannels - 1))
#define gMinChannel_c			(gChannel0_c)
#else
#define gDefaultChannelNumber_c		gChannel11_c
#define gMaxChannel_c		        ((channels_t)gTotalChannels)
#define gMinChannel_c			(gChannel11_c)
#endif

/* END Configuration Parameters Definition */

#define gLPTMR_MS_COUNT_c (5000)
typedef enum LPDStates_tag
{
  gLPDInitState_c,
  gLPDSelectOption_c,
  gLPDManual_c,
  gLPDLpps_c,
  gLPDMaxState_c
}LPDStates_t;

typedef enum LPDManualStates_tag
{
  gLPDMStateInit_c,
  gLPDMStateSelectMcuMode_c,
  gLPDMStateSelectXcvrMode_c,
  gLPDMStateSelectWakeUpSource_c,
  gLPDMStateEnterLowPower_c,
  gLPDMStateHandleExitLowPower_c
}LPDManualStates_t;

typedef enum LPDLppsStates_tag
{
  gLPDLppsInit_c,
  gLPDLppsConfigRole_c,
  gLPDLppsRX_c,
  gLPDLppsTX_c,
  gLPDLppsRXEnd_c,
  gLPDLppsTXEnd_c
}LPDLppsStates_t;

typedef pwrp_status_t (*ptr_set_mcu_mode)(void);
typedef pwrp_status_t (*ptr_set_mcu_mode_vlls)(uint8_t);

extern smacErrors_t smacToAppMlmeSap(smacToAppMlmeMessage_t* pMsg, instanceId_t instance);
extern smacErrors_t smacToAppMcpsSap(smacToAppDataMessage_t* pMsg, instanceId_t instance);
extern void InitApp();

#endif /* __SMAC_APP_CONFIG_H__ */

