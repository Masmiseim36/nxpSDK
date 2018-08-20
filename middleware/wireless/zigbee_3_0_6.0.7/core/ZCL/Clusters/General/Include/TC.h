/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef TC_H
#define TC_H

/*!
\file       TC.h
\brief      Header for Test Cluster
*/
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/     
#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Cluster ID's */
#define GENERAL_CLUSTER_ID_TC   0xffff


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* test cluster attribute ID's */
typedef enum 
{
#ifdef CLD_TC_UINTS
    E_CLD_TC_ATTR_ID_UINT8          = 0x0000,
    E_CLD_TC_ATTR_ID_UINT16,
    E_CLD_TC_ATTR_ID_UINT24,
    E_CLD_TC_ATTR_ID_UINT32,
    E_CLD_TC_ATTR_ID_UINT40,
    E_CLD_TC_ATTR_ID_UINT48,
    E_CLD_TC_ATTR_ID_UINT56,
    E_CLD_TC_ATTR_ID_UINT64,
#endif

#ifdef CLD_TC_INTS
    E_CLD_TC_ATTR_ID_INT8,
    E_CLD_TC_ATTR_ID_INT16,
    E_CLD_TC_ATTR_ID_INT24,
    E_CLD_TC_ATTR_ID_INT32,
    E_CLD_TC_ATTR_ID_INT40,
    E_CLD_TC_ATTR_ID_INT48,
    E_CLD_TC_ATTR_ID_INT56,
    E_CLD_TC_ATTR_ID_INT64,
#endif
    E_CLD_TC_ATTR_ID_NONE,
} teCLD_TC_ClusterID;


/* Test Cluster */
typedef struct
{
#ifdef CLD_TC_UINTS
    zuint8                  u8;
    zuint16                 u16;
    zuint24                 u24;
    zuint32                 u32;
    zuint40                 u40;
    zuint48                 u48;
    zuint56                 u56;
    zuint64                 u64;
#endif

#ifdef CLD_TC_INTS
    zint8                   i8;
    zint16                  i16;
    zint24                  i24;
    zint32                  i32;
    zint40                  i40;
    zint48                  i48;
    zint56                  i56;
    zint64                  i64;
#endif
    bool                    bDummy;
} tsCLD_TC;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_TCCreateTestCluster(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                                        *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern const tsZCL_AttributeDefinition asCLD_TCClusterAttributeDefinitions[];
extern tsZCL_ClusterDefinition sCLD_TC;

extern uint8 au8TCServerAttributeControlBits[];

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* TC_H */
