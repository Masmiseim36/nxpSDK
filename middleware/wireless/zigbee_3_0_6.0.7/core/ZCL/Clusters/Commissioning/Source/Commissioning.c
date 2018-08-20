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
\file       Commissioning.c
\brief      Commissioning cluster definition
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "FunctionLib.h"
#include <jendefs.h>
#include "zcl.h"
#include "Commissioning.h"
#include "Commissioning_internal.h"
#include "zcl_options.h"
#include <string.h>

#ifdef CLD_COMMISSIONING

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
#ifdef ZCL_COMMAND_DISCOVERY_SUPPORTED
    const tsZCL_CommandDefinition asCLD_CommissioningClusterCommandDefinitions[] = {
        {E_CLD_COMMISSIONING_CMD_RESTART_DEVICE,             E_ZCL_CF_RX|E_ZCL_CF_TX},     /* Mandatory */
    #ifdef CLD_COMMISSIONING_CMD_SAVE_STARTUP_PARAMS    
        {E_CLD_COMMISSIONING_CMD_SAVE_STARTUP_PARAMS,        E_ZCL_CF_RX|E_ZCL_CF_TX},     /* Optional */ 
    #endif
    #ifdef CLD_COMMISSIONING_CMD_RESTORE_STARTUP_PARAMS    
        {E_CLD_COMMISSIONING_CMD_RESTORE_STARTUP_PARAMS,     E_ZCL_CF_RX|E_ZCL_CF_TX},     /* Optional */
    #endif
        {E_CLD_COMMISSIONING_CMD_RESET_STARTUP_PARAMS,       E_ZCL_CF_RX|E_ZCL_CF_TX},     /* Mandatory */
    };
#endif

const tsZCL_AttributeDefinition asCLD_CommissioningClusterAttributeDefinitions[] = {
#ifdef COMMISSIONING_SERVER
            /* Commissioning Startup attribute set (13.2.2.2.1) */
    #ifdef     CLD_COMMISSIONING_ATTR_SHORT_ADDRESS
        {E_CLD_COMMISSIONING_ATTR_ID_SHORT_ADDRESS,        (E_ZCL_AF_RD),               E_ZCL_UINT16,        (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u16ShortAddress),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_EXTENED_PAN_ID
        {E_CLD_COMMISSIONING_ATTR_ID_EXT_PANID,            (E_ZCL_AF_RD),                E_ZCL_IEEE_ADDR,     (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u64ExtPanId),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_PAN_ID
        {E_CLD_COMMISSIONING_ATTR_ID_PANID,                (E_ZCL_AF_RD),               E_ZCL_UINT16,        (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u16PANId),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_CHANNEL_MASK
        {E_CLD_COMMISSIONING_ATTR_ID_CHANNEL_MASK,         (E_ZCL_AF_RD),               E_ZCL_BMAP32,        (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u32ChannelMask),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_PROTOCOL_VERSION
        {E_CLD_COMMISSIONING_ATTR_ID_PROTOCOL_VERSION,     (E_ZCL_AF_RD),               E_ZCL_UINT8,        (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u8ProtocolVersion),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_STACK_PROFILE
        {E_CLD_COMMISSIONING_ATTR_ID_STACK_PROFILE,        (E_ZCL_AF_RD),               E_ZCL_UINT8,        (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u8StackProfile),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_START_UP_CONTROL
        {E_CLD_COMMISSIONING_ATTR_ID_STARTUP_CONTROl,      (E_ZCL_AF_RD),               E_ZCL_ENUM8,         (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.e8StartUpControl),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_TC_ADDR
        {E_CLD_COMMISSIONING_ATTR_ID_TC_ADDR,              (E_ZCL_AF_RD),               E_ZCL_IEEE_ADDR,     (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u64TcAddr),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_TC_MASTER_KEY
        {E_CLD_COMMISSIONING_ATTR_ID_TC_MASTER_KEY,        (E_ZCL_AF_RD),               E_ZCL_KEY_128,       (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.sTcMasterKey),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_NWK_KEY
        {E_CLD_COMMISSIONING_ATTR_ID_NETWORK_KEY,          (E_ZCL_AF_RD),               E_ZCL_KEY_128,       (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.sNwkKey),0},
    #endif


    #ifdef     CLD_COMMISSIONING_ATTR_USE_INSECURE_JOIN
        {E_CLD_COMMISSIONING_ATTR_ID_USE_INSECURE_JOIN,    (E_ZCL_AF_RD),               E_ZCL_BOOL,          (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.bUseInsecureJoin),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_PRE_CONFIG_LINK_KEY
        {E_CLD_COMMISSIONING_ATTR_ID_PRECONFIG_LINK_KEY,   (E_ZCL_AF_RD),               E_ZCL_KEY_128,       (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.sPreConfigLinkKey),0},
    #endif


    #ifdef     CLD_COMMISSIONING_ATTR_NWK_KEY_SEQ_NO
        {E_CLD_COMMISSIONING_ATTR_ID_NWK_KEY_SEQ_NO,       (E_ZCL_AF_RD),               E_ZCL_UINT8,         (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u8NwkKeySeqNo),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_NWK_KEY_TYPE
        {E_CLD_COMMISSIONING_ATTR_ID_NWK_KEY_TYPE,         (E_ZCL_AF_RD),               E_ZCL_ENUM8,         (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.e8NwkKeyType),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_NWK_MANAGER_ADDR
        {E_CLD_COMMISSIONING_ATTR_ID_NWK_MANAGER_ADDR,     (E_ZCL_AF_RD),               E_ZCL_UINT16,        (uint32)(&((tsCLD_Commissioning*)(0))->sStartupParameters.u16NwkManagerAddr),0},
    #endif
        /* Join Parameters attribute set (13.2.2.2.2)*/
    #ifdef     CLD_COMMISSIONING_ATTR_SCAN_ATTEMPTS
        {E_CLD_COMMISSIONING_ATTR_ID_SCAN_ATTEMPTS,        (E_ZCL_AF_RD),               E_ZCL_UINT8,         (uint32)(&((tsCLD_Commissioning*)(0))->sJoinParameters.u8ScanAttempts),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_TIME_BW_SCANS
        {E_CLD_COMMISSIONING_ATTR_ID_TIME_BW_SCANS,        (E_ZCL_AF_RD),               E_ZCL_UINT16,        (uint32)(&((tsCLD_Commissioning*)(0))->sJoinParameters.u16TimeBwScans),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_REJOIN_INTERVAL
        {E_CLD_COMMISSIONING_ATTR_ID_REJOIN_INTERVAL,      (E_ZCL_AF_RD),               E_ZCL_UINT16,        (uint32)(&((tsCLD_Commissioning*)(0))->sJoinParameters.u16RejoinInterval),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_MAX_REJOIN_INTERVAL
        {E_CLD_COMMISSIONING_ATTR_ID_MAX_REJOIN_INTERVAL,   (E_ZCL_AF_RD),              E_ZCL_UINT16,       (uint32)(&((tsCLD_Commissioning*)(0))->sJoinParameters.u16MaxRejoinInterval),0},
    #endif
        /* End Device Parameters attribute set (13.2.2.2.3)*/
    #ifdef     CLD_COMMISSIONING_ATTR_INDIRECT_POLL_RATE
        {E_CLD_COMMISSIONING_ATTR_ID_INDIRECT_POLL_RATE,    (E_ZCL_AF_RD),              E_ZCL_UINT16,       (uint32)(&((tsCLD_Commissioning*)(0))->sEndDeviceParameters.u16IndirectPollRate),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_PARENT_RETRY_THRSHLD
        {E_CLD_COMMISSIONING_ATTR_ID_PARENT_RETRY_THRSHOLD,  (E_ZCL_AF_RD),             E_ZCL_UINT8,        (uint32)(&((tsCLD_Commissioning*)(0))->sEndDeviceParameters.u8ParentRetryThreshold),0},
    #endif
        /* Concentrator Parameters attribute set (3.15.2.2.4)*/
    #ifdef     CLD_COMMISSIONING_ATTR_CONCENTRATOR_FLAG
        {E_CLD_COMMISSIONING_ATTR_ID_CONCENTRATOR_FLAG,     (E_ZCL_AF_RD),              E_ZCL_BOOL,         (uint32)(&((tsCLD_Commissioning*)(0))->sConcentratorParameters.bConcentratorFlag),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_CONCENTRATOR_RADIUS
        {E_CLD_COMMISSIONING_ATTR_ID_CONCENTRATOR_RADIUS,   (E_ZCL_AF_RD),              E_ZCL_UINT8,        (uint32)(&((tsCLD_Commissioning*)(0))->sConcentratorParameters.u8ConcentratorRadius),0},
    #endif

    #ifdef     CLD_COMMISSIONING_ATTR_CONCENTRATOR_DISCVRY_TIME
        {E_CLD_COMMISSIONING_ATTR_ID_CONCENTRATOR_DISCVRY_TIME,(E_ZCL_AF_RD),           E_ZCL_UINT8,        (uint32)(&((tsCLD_Commissioning*)(0))->sConcentratorParameters.u8ConcentratorDiscoveryTime),0},
    #endif
#endif
        {E_CLD_GLOBAL_ATTR_ID_CLUSTER_REVISION,             (E_ZCL_AF_RD|E_ZCL_AF_GA),  E_ZCL_UINT16,       (uint32)(&((tsCLD_Commissioning*)(0))->u16ClusterRevision),0},   /* Mandatory  */
    };

tsZCL_ClusterDefinition sCLD_Commissioning = {
        COMMISSIONING_CLUSTER_ID_COMMISSIONING,
        FALSE,
        E_ZCL_SECURITY_APPLINK,
        (sizeof(asCLD_CommissioningClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition)),
        (tsZCL_AttributeDefinition*)asCLD_CommissioningClusterAttributeDefinitions,
        NULL
#ifdef ZCL_COMMAND_DISCOVERY_SUPPORTED        
        ,
        (sizeof(asCLD_CommissioningClusterCommandDefinitions) / sizeof(tsZCL_CommandDefinition)),
        (tsZCL_CommandDefinition*)asCLD_CommissioningClusterCommandDefinitions          
#endif  
};
uint8 au8CommissioningClusterAttributeControlBits[(sizeof(asCLD_CommissioningClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition))];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 **
 ** NAME:       eCLD_CommissioningClusterCreateCommissioning
 **
 ** DESCRIPTION:
 ** Creates a time cluster
 **
 ** PARAMETERS:                 Name                        Usage
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_CommissioningClusterCreateCommissioning(
                tsZCL_ClusterInstance                   *psClusterInstance,
                bool_t                                  bIsServer,
                tsZCL_ClusterDefinition                 *psClusterDefinition,
                uint8                                   u8ClusterSecurity,
                void                                    *pvEndPointSharedStructPtr,
                uint8                                   *pu8AttributeControlBits,
                tsCLD_CommissioningCustomDataStructure  *psCustomDataStructure)
{

    tsCLD_Commissioning *psCluster;

    #ifdef STRICT_PARAM_CHECK 
        /* Parameter check */
        if((psClusterInstance==NULL) ||
           (psClusterDefinition==NULL) ||
            (psCustomDataStructure==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif

    /* Create an instance of a commissioning cluster */
    vZCL_InitializeClusterInstance(
                                   psClusterInstance, 
                                   bIsServer,
                                   psClusterDefinition,
                                   pvEndPointSharedStructPtr,
                                   pu8AttributeControlBits,
                                   psCustomDataStructure,
                                   eCLD_CommissioningCommandHandler);

    psCustomDataStructure->sCustomCallBackEvent.eEventType = E_ZCL_CBET_CLUSTER_CUSTOM;
    psCustomDataStructure->sCustomCallBackEvent.uMessage.sClusterCustomMessage.u16ClusterId = psClusterDefinition->u16ClusterEnum;
    psCustomDataStructure->sCustomCallBackEvent.uMessage.sClusterCustomMessage.pvCustomData = (void *)&psCustomDataStructure->sCallBackMessage;
    psCustomDataStructure->sCustomCallBackEvent.psClusterInstance = psClusterInstance; 
    
    /* Update cluster security */
    psClusterInstance->psClusterDefinition->u8ClusterControlFlags |= u8ClusterSecurity;

    //  attributes set default values
    psCluster = (tsCLD_Commissioning*)pvEndPointSharedStructPtr;

    psCluster->u16ClusterRevision = CLD_COMMISSIONING_CLUSTER_REVISION;
    
    return E_ZCL_SUCCESS;

}

#ifdef COMMISSIONING_SERVER
/****************************************************************************
 **
 ** NAME:       eCLD_CommissioningSetAttribute
 **
 ** DESCRIPTION:
 ** Used by cluster to set its attributes
 **
 ** PARAMETERS:                         Name                        Usage
 ** uint8                               u8SourceEndPointId          EndPoint Id
 ** teCLD_Commissioning_AttributeSet    eAttributeSet               Attrbute Set
 ** void*                               pvAttributeSetStructure    Pointer to attribute set structure
 **
 ** RETURN:
 ** teZCL_Status
 ** 
 ****************************************************************************/

PUBLIC  teZCL_Status eCLD_CommissioningSetAttribute(
                                            uint8                                   u8SourceEndPointId,
                                            teCLD_Commissioning_AttributeSet        eAttributeSet,
                                            void                                    *pvAttributeSetStructure)  
{
    teZCL_Status eStatus;
    tsZCL_EndPointDefinition *psEndPointDefinition;
    tsZCL_ClusterInstance *psClusterInstance;
    tsCLD_CommissioningCustomDataStructure *psCustomDataStructure;

    if(pvAttributeSetStructure == NULL)
    {
        return E_ZCL_ERR_PARAMETER_NULL;
    }    
    
    eStatus = eZCL_FindCluster(COMMISSIONING_CLUSTER_ID_COMMISSIONING, u8SourceEndPointId, TRUE, &psEndPointDefinition, &psClusterInstance, (void*)&psCustomDataStructure);
    if(eStatus != E_ZCL_SUCCESS)
    {
        return eStatus;
    }
        
    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif
    
    switch(eAttributeSet)
    {
        case E_CLD_COMMISSIONING_ATTR_SET_STARTUP_PARAMS:
        {
            tsCLD_StartupParameters *psStartupParameters = (tsCLD_StartupParameters *)&(((tsCLD_Commissioning *)(psClusterInstance->pvEndPointSharedStructPtr))->sStartupParameters);
            FLib_MemCpy(psStartupParameters,(tsCLD_StartupParameters *)pvAttributeSetStructure,sizeof(tsCLD_StartupParameters));
            break;
        }
        case E_CLD_COMMISSIONING_ATTR_SET_JOIN_PARAMS:
        {
            tsCLD_JoinParameters *psJoinParameters = (tsCLD_JoinParameters *)&(((tsCLD_Commissioning *)(psClusterInstance->pvEndPointSharedStructPtr))->sJoinParameters);
            FLib_MemCpy(psJoinParameters,(tsCLD_JoinParameters *)pvAttributeSetStructure,sizeof(tsCLD_JoinParameters));
            break;
        }
        case E_CLD_COMMISSIONING_ATTR_SET_ENDDEVICE_PARAMS:
        {
            tsCLD_EndDeviceParameters *psEndDeviceParameters = (tsCLD_EndDeviceParameters *)&(((tsCLD_Commissioning *)(psClusterInstance->pvEndPointSharedStructPtr))->sEndDeviceParameters);
            FLib_MemCpy(psEndDeviceParameters,(tsCLD_EndDeviceParameters *)pvAttributeSetStructure,sizeof(tsCLD_EndDeviceParameters));
            break;
        }
        case E_CLD_COMMISSIONING_ATTR_SET_CONCENTRATOR_PARAMS:
        {
            tsCLD_ConcentratorParameters *psConcentratorParameters = (tsCLD_ConcentratorParameters *)&(((tsCLD_Commissioning *)(psClusterInstance->pvEndPointSharedStructPtr))->sConcentratorParameters);
            FLib_MemCpy(psConcentratorParameters,(tsCLD_ConcentratorParameters *)pvAttributeSetStructure,sizeof(tsCLD_ConcentratorParameters));
            break;
        }
        
        default:
            return E_ZCL_ERR_ATTRIBUTE_TYPE_UNSUPPORTED;
            break;
    }
    
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif
    
    return eStatus;
}

#endif
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

