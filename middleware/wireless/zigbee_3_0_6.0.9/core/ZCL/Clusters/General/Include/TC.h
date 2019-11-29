/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
