/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_CFG_H_
#define _APP_CFG_H_

/*!
\file       app_cfg.h
\brief      This file is a for application configuration of demo applications
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Events that are passed to the application task. 
   Are defined as byte masks to make possible 
   send multiple events to the task */
#define gPhyEvtMessageFromPD_c         (1 << 0)
#define gPhyEvtMessageFromPLME_c       (1 << 1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/


#endif /* _APP_CFG_H_ */
