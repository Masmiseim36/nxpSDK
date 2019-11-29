/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_MAIN_H_
#define APP_MAIN_H_

/*!
\file       app_main.h
\brief      ZLO Main Handler Interface
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "ZQueue.h"
#include "bdb_api.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitResources(void);
PUBLIC void APP_vSetUpHardware(void);
PUBLIC void APP_vMainLoop(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern PUBLIC uint8 u8TimerPoll;
extern PUBLIC uint8 u8TimerPowerOnCount;
extern PUBLIC uint8 u8TimerLightTempSensorSample;
extern PUBLIC uint8 u8TimerTick;
extern PUBLIC uint8 u8TimerBlink;
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
extern PUBLIC uint8 u8TimerNtag;
#endif

extern PUBLIC tszQueue zps_msgMlmeDcfmInd;
extern PUBLIC tszQueue zps_msgMcpsDcfmInd;
extern PUBLIC tszQueue zps_TimeEvents;

extern PUBLIC tszQueue APP_msgZpsEvents;
extern PUBLIC tszQueue APP_msgZclEvents;
extern PUBLIC tszQueue APP_msgAppEvents;
extern PUBLIC tszQueue APP_msgBdbEvents;

extern PUBLIC bool_t APP_bPersistantPolling;
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_MAIN_H_ */






