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
\file       OOSC.c
\brief      On/Off switch configuration cluster definition
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
#include "OOSC.h"
#include "zcl_options.h"
#include "OOSC.h"


#ifdef CLD_OOSC

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
const tsZCL_AttributeDefinition asCLD_OOSCClusterAttributeDefinitions[] = {
#ifdef OOSC_SERVER
        {E_CLD_OOSC_ATTR_ID_SWITCH_TYPE,    E_ZCL_AF_RD,                E_ZCL_ENUM8,    (uint32)(&((tsCLD_OOSC*)(0))->eSwitchType),0},    /* Mandatory */

        {E_CLD_OOSC_ATTR_ID_SWITCH_ACTIONS, (E_ZCL_AF_RD|E_ZCL_AF_WR),  E_ZCL_ENUM8,    (uint32)(&((tsCLD_OOSC*)(0))->eSwitchActions),0}, /* Mandatory */
#endif        
        {E_CLD_GLOBAL_ATTR_ID_CLUSTER_REVISION, (E_ZCL_AF_RD|E_ZCL_AF_GA),E_ZCL_UINT16,   (uint32)(&((tsCLD_OOSC*)(0))->u16ClusterRevision),0},   /* Mandatory  */

    };
tsZCL_ClusterDefinition sCLD_OOSC = {
        GENERAL_CLUSTER_ID_ONOFF_SWITCH_CONFIGURATION,
        FALSE,
        E_ZCL_SECURITY_NETWORK,
        (sizeof(asCLD_OOSCClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition)),
        (tsZCL_AttributeDefinition*)asCLD_OOSCClusterAttributeDefinitions,
        NULL
};
uint8 au8OOSCAttributeControlBits[(sizeof(asCLD_OOSCClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition))];
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       eCLD_OOSCCreateOnOffSwitchConfig
 **
 ** DESCRIPTION:
 ** Creates an on/off cluster
 **
 ** PARAMETERS:                 Name                        Usage
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC  teZCL_Status eCLD_OOSCCreateOnOffSwitchConfig(
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

        if(pvEndPointSharedStructPtr != NULL)
        {
#ifdef OOSC_SERVER            
            /* Set attribute defaults */
            ((tsCLD_OOSC*)psClusterInstance->pvEndPointSharedStructPtr)->eSwitchActions = 0x00;
#endif            
            ((tsCLD_OOSC*)psClusterInstance->pvEndPointSharedStructPtr)->u16ClusterRevision = CLD_OOSC_CLUSTER_REVISION;
        }

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

