/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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

