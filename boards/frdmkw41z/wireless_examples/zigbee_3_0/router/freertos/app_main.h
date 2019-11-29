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

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitResources(void);
PUBLIC void APP_vMainLoop(void);
PUBLIC void APP_vSetUpHardware(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern PUBLIC uint8 u8TimerButtonScan;
extern PUBLIC uint8 u8TimerZCL;
#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
extern PUBLIC uint8 u8TimerNtag;
#endif

extern PUBLIC tszQueue zps_msgMlmeDcfmInd;
extern PUBLIC tszQueue zps_msgMcpsDcfmInd;
extern PUBLIC tszQueue zps_TimeEvents;

//extern PUBLIC tszQueue APP_msgZpsEvents;
extern PUBLIC tszQueue APP_msgBdbEvents;
extern PUBLIC tszQueue APP_msgAppEvents;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_MAIN_H */






