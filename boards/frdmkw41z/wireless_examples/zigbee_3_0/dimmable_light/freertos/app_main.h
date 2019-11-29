/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_MAIN_H
#define APP_MAIN_H

/*!
\file       app_main.h
\brief      app main header file.
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "ZQueue.h"
#ifdef CPU_MKW41Z512VHT4
#include "mac_sap.h"
#else
#include "tsv_pub.h"
#endif
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

extern PUBLIC uint8 u8TimerButtonScan;
extern PUBLIC uint8 u8TimerRadioRecal;
extern PUBLIC uint8 u8TimerTick;
extern PUBLIC uint8 u8TimerPowerOn;
#ifdef CLD_GREENPOWER
extern PUBLIC uint8 u8GPTimerTick;
#endif
#ifdef APP_NTAG
extern PUBLIC uint8 u8TimerNtag;
#endif


extern PUBLIC tszQueue zps_msgMlmeDcfmInd;
extern PUBLIC tszQueue zps_msgMcpsDcfmInd;
extern PUBLIC tszQueue zps_TimeEvents;

extern PUBLIC tszQueue APP_msgBdbEvents;
extern PUBLIC tszQueue APP_msgAppEvents;
#ifdef CLD_GREENPOWER
extern PUBLIC tszQueue APP_msgGPZCLTimerEvents;
extern uint8 au8GPZCLEvent[];
extern uint8 u8GPZCLTimerEvent;
#endif

#ifdef OTA_CLD_ATTR_REQUEST_DELAY
    extern PUBLIC uint8 u8TimerZclMsTick;
#endif

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_MAIN_H */






