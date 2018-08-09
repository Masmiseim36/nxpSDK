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

/*!=============================================================================
\file       TC.c
\brief      Test cluster definition
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
#include "OOSC.h"
#include "zcl_options.h"
#include "TC.h"

#ifdef CLD_TC

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

const tsZCL_AttributeDefinition asCLD_TCClusterAttributeDefinitions[] = {

#ifdef CLD_TC_UINTS
    /* Unsigned ints */
    {E_CLD_TC_ATTR_ID_UINT8,    (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_UINT8,    (uint32)(&((tsCLD_TC*)(0))->u8),0},
    {E_CLD_TC_ATTR_ID_UINT16,   (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_UINT16,   (uint32)(&((tsCLD_TC*)(0))->u16),0},
    {E_CLD_TC_ATTR_ID_UINT24,   (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_UINT24,   (uint32)(&((tsCLD_TC*)(0))->u24),0},
    {E_CLD_TC_ATTR_ID_UINT32,   (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_UINT32,   (uint32)(&((tsCLD_TC*)(0))->u32),0},
    {E_CLD_TC_ATTR_ID_UINT40,   (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_UINT40,   (uint32)(&((tsCLD_TC*)(0))->u40),0},
    {E_CLD_TC_ATTR_ID_UINT48,   (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_UINT48,   (uint32)(&((tsCLD_TC*)(0))->u48),0},
    {E_CLD_TC_ATTR_ID_UINT56,   (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_UINT56,   (uint32)(&((tsCLD_TC*)(0))->u56),0},
    {E_CLD_TC_ATTR_ID_UINT64,   (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_UINT64,   (uint32)(&((tsCLD_TC*)(0))->u64),0},
#endif

#ifdef CLD_TC_INTS
    /* Signed ints */
    {E_CLD_TC_ATTR_ID_INT8,     (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_INT8,     (uint32)(&((tsCLD_TC*)(0))->i8),0},
    {E_CLD_TC_ATTR_ID_INT16,    (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_INT16,    (uint32)(&((tsCLD_TC*)(0))->i16),0},
    {E_CLD_TC_ATTR_ID_INT24,    (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_INT24,    (uint32)(&((tsCLD_TC*)(0))->i24),0},
    {E_CLD_TC_ATTR_ID_INT32,    (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_INT32,    (uint32)(&((tsCLD_TC*)(0))->i32),0},
    {E_CLD_TC_ATTR_ID_INT40,    (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_INT40,    (uint32)(&((tsCLD_TC*)(0))->i40),0},
    {E_CLD_TC_ATTR_ID_INT48,    (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_INT48,    (uint32)(&((tsCLD_TC*)(0))->i48),0},
    {E_CLD_TC_ATTR_ID_INT56,    (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_INT56,    (uint32)(&((tsCLD_TC*)(0))->i56),0},
    {E_CLD_TC_ATTR_ID_INT64,    (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_SE),  E_ZCL_INT64,    (uint32)(&((tsCLD_TC*)(0))->i64),0},
#endif
};


tsZCL_ClusterDefinition sCLD_TC = {
        GENERAL_CLUSTER_ID_TC,
        FALSE,
        E_ZCL_SECURITY_NETWORK,
        (sizeof(asCLD_TCClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition)),
        (tsZCL_AttributeDefinition*)asCLD_TCClusterAttributeDefinitions,
        NULL
};

uint8 au8TCServerAttributeControlBits[(sizeof(asCLD_TCClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition))];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       eCLD_TCCreateTestCluster
 **
 ** DESCRIPTION:
 ** Creates a test cluster
 **
 ** PARAMETERS:                 Name                        Usage
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC teZCL_Status eCLD_TCCreateTestCluster(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits)
{

    #ifdef STRICT_PARAM_CHECK 
        /* Parameter check */
        if(psClusterInstance==NULL)
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif
    
    // cluster data
    
     vZCL_InitializeClusterInstance(
                                   psClusterInstance, 
                                   bIsServer,
                                   psClusterDefinition,
                                   pvEndPointSharedStructPtr,
                                   pu8AttributeControlBits,
                                   NULL,
                                   NULL);   
    //  register timer
    return E_ZCL_SUCCESS;

}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

