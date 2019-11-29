/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  ANALOG_INPUT_BASIC_INTERNAL_H_INCLUDED
#define  ANALOG_INPUT_BASIC_INTERNAL_H_INCLUDED

/*!
\file       AnalogInputBasic_internal.h
\brief      The internal API for the Analog Input Basic Cluster
*/     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "AnalogInputBasic.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

#ifdef ANALOG_INPUT_BASIC_SERVER
PUBLIC teZCL_Status eCLD_AnalogInputBasicHandler(
                uint8                               u8SourceEndPointId);
#endif  

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* ANALOG_INPUT_BASIC_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
