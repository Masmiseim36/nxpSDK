/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "ZQueue.h"
//#include "tsv_pub.h"

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

extern PUBLIC uint8 u8TimerLedBlink;
extern PUBLIC uint8 u8TimerId;
extern PUBLIC uint8 u8TimerAddGroup;
extern PUBLIC uint8 u8TimerPoll;
extern PUBLIC uint8 u8TimerZCL;

extern PUBLIC tszQueue zps_msgMlmeDcfmInd;
extern PUBLIC tszQueue zps_msgMcpsDcfmInd;
extern PUBLIC tszQueue zps_TimeEvents;

extern PUBLIC tszQueue APP_msgZclEvents;
extern PUBLIC tszQueue APP_msgAppEvents;
extern PUBLIC tszQueue APP_msgBdbEvents;

#ifdef DR1199
extern PUBLIC tszQueue APP_msgSerialTx;
extern PUBLIC tszQueue APP_msgSerialRx;
#endif
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_MAIN_H */






