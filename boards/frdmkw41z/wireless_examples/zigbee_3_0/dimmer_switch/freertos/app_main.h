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
#include "mac_sap.h"
#include "bdb_api.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/* Error codes */
enum {
  errorNoError,
  errorWrongConfirm,
  errorNotSuccessful,
  errorNoMessage,
  errorAllocFailed,
  errorInvalidParameter,
  errorNoScanResults
};


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitResources(void);
PUBLIC void APP_vSetUpHardware(void);
PUBLIC void APP_vInitLeds(void);
PUBLIC void APP_vSetLED(uint8 u8Led, bool_t bOn);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern PUBLIC uint8 u8TimerPoll;
extern PUBLIC uint8 u8TimerButtonScan;
extern PUBLIC uint8 u8TimerTick;
extern PUBLIC uint8 u8TimerJoin;
extern PUBLIC uint8 u8TimerButtonDelay;
extern PUBLIC uint8 u8TimerChangeMode;
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
extern PUBLIC uint8 u8TimerNtag;
#endif

extern PUBLIC tszQueue zps_msgMlmeDcfmInd;
extern PUBLIC tszQueue zps_msgMcpsDcfmInd;
extern PUBLIC tszQueue zps_TimeEvents;

extern PUBLIC tszQueue APP_msgAppEvents;
extern PUBLIC tszQueue APP_msgBdbEvents;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_MAIN_H */
