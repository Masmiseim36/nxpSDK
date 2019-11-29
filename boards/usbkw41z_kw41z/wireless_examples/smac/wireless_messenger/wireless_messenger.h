/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef __SMAC_APPLICATION_CONF_H__
#define __SMAC_APPLICATION_CONF_H__

#include "MemManager.h"
#include "wireless_messenger_menus.h"      /*Defines the Application menus*/          
#include "SerialManager.h"
#include "LED.h"                    /*Include all LED functionality*/ 
#include "TimersManager.h"
#include "PhyInterface.h"
#include "SMAC_Interface.h"     
#include "FunctionLib.h"
#include "AspInterface.h"

#ifdef gPHY_802_15_4g_d
#include "PhyTime.h"
#endif

#include "fsl_os_abstraction.h"
#include "board.h"

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

/*  gRFMode1_c is Mode1      */
/*  gRFMode2_c is Mode2      */
/*  gRFMode3_c is Mode3      */
/*  gRFMode4_c is Mode4      */
/*  gRFMode5_c is ARIB mode 1*/
/*  gRFMode6_c is ARIB mode 2*/

#ifdef gPHY_802_15_4g_d
#define gDefaultMode1_c                 gRFMode1_c
#define gDefaultMode2_c                 gRFMode2_c
#endif

/* END Configuration Parameters Definition */


extern smacErrors_t smacToAppMlmeSap(smacToAppMlmeMessage_t* pMsg, instanceId_t instance);
extern smacErrors_t smacToAppMcpsSap(smacToAppDataMessage_t* pMsg, instanceId_t instance);
extern void InitApp();

#endif /* __SMAC_APP_CONFIG_H__ */

