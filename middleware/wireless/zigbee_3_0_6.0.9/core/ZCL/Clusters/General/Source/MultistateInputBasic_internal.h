/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  MULTISTATE_INPUT_BASIC_INTERNAL_H_INCLUDED
#define  MULTISTATE_INPUT_BASIC_INTERNAL_H_INCLUDED

/*!
\file       MultistateInputBasic_internal.h
\brief      The internal API for the Multistate Input Basic Cluster
*/
     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "MultistateInputBasic.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

#ifdef MULTISTATE_INPUT_BASIC_SERVER                
PUBLIC teZCL_Status eCLD_MultistateInputBasicHandler(
                uint8                               u8SourceEndPointId);
#endif  

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* MULTISTATE_INPUT_BASIC_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
