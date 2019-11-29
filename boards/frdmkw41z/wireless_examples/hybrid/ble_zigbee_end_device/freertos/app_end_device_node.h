/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_END_DEVICE_NODE_H_
#define APP_END_DEVICE_NODE_H_

/*!
\file       app_end_device_node.h
\brief      Base Device Demo: End Device Application
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define NUMBER_DEVICE_TO_BE_DISCOVERED 8 // Should be same as Binding table size
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitialiseNode(void);
PUBLIC void APP_vFactoryResetRecords(void);
PUBLIC void APP_cbTimerPoll(void *pvParam);
PUBLIC void APP_vStartUpHW(void);
PUBLIC void APP_vOobcSetRunning(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC bool_t bDeepSleep;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_END_DEVICE_NODE_H_*/
