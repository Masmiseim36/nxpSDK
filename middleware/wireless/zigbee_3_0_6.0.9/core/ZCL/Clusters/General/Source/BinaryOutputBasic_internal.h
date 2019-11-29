/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  BINARY_OUTPUT_BASIC_INTERNAL_H_INCLUDED
#define  BINARY_OUTPUT_BASIC_INTERNAL_H_INCLUDED

/*!
\file       BinaryInputBasic_internal.h
\brief      The internal API for the Binary Input Basic Cluster
*/
     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "BinaryOutputBasic.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

#ifdef BINARY_OUTPUT_BASIC_SERVER                
PUBLIC teZCL_Status eCLD_BinaryOutputBasicHandler(
                uint8                               u8SourceEndPointId);
#endif  

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* BINARY_OUTPUT_BASIC_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
