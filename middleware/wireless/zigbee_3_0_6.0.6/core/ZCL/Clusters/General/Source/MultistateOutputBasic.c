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
\file       MultistateOutputBasic.c
\brief      Multistate Inpunt Basic cluster definition
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
#include "zcl_customcommand.h"
#include "MultistateOutputBasic.h"
#include "zcl_options.h"


#ifdef CLD_MULTISTATE_OUTPUT_BASIC

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#if defined(CLD_MULTISTATE_OUTPUT_BASIC) && !defined(MULTISTATE_OUTPUT_BASIC_SERVER) && !defined(MULTISTATE_OUTPUT_BASIC_CLIENT)
#error You Must Have either MULTISTATE_OUTPUT_BASIC_SERVER and/or MULTISTATE_OUTPUT_BASIC_CLIENT defined in zcl_options.h
#endif
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
tsZCL_AttributeDefinition asCLD_MultistateOutputBasicClusterAttributeDefinitions [] = {
#ifdef MULTISTATE_OUTPUT_BASIC_SERVER
        /* ZigBee Cluster Library Version */
    #ifdef CLD_MULTISTATE_OUTPUT_BASIC_ATTR_DESCRIPTION
        {E_CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ID_DESCRIPTION,            (E_ZCL_AF_RD|E_ZCL_AF_WR),               E_ZCL_CSTRING,      (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->sDescription),         0},  /* Optional */
    #endif

        {E_CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ID_NUMBER_OF_STATES,       (E_ZCL_AF_RD|E_ZCL_AF_WR),               E_ZCL_UINT16,       (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->u16NumberOfStates),    0}, /* Mandatory */   
    
        {E_CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ID_OUT_OF_SERVICE,         (E_ZCL_AF_RD|E_ZCL_AF_WR),               E_ZCL_BOOL,         (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->bOutOfService),        0},  /* Mandatory */

        {E_CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ID_PRESENT_VALUE,          (E_ZCL_AF_RD|E_ZCL_AF_WR|E_ZCL_AF_RP),   E_ZCL_UINT16,       (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->u16PresentValue),      0},  /* Mandatory */

    #ifdef CLD_MULTISTATE_OUTPUT_BASIC_ATTR_RELIABILITY
        {E_CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ID_RELIABILITY,            (E_ZCL_AF_RD|E_ZCL_AF_WR),               E_ZCL_ENUM8,        (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->u8Reliability),        0},  /* Optional */
    #endif        

    #ifdef CLD_MULTISTATE_OUTPUT_BASIC_ATTR_RELINQUISH_DEFAULT
        {E_CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ID_RELINQUISH_DEFAULT,     (E_ZCL_AF_RD|E_ZCL_AF_WR),               E_ZCL_UINT16,       (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->u16RelinquishDefault),   0},  /* Optional */
    #endif
    
        {E_CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ID_STATUS_FLAGS,           (E_ZCL_AF_RD|E_ZCL_AF_RP),               E_ZCL_BMAP8,        (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->u8StatusFlags),        0},  /* Mandatory */

    #ifdef CLD_MULTISTATE_OUTPUT_BASIC_ATTR_APPLICATION_TYPE
        {E_CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ID_APPLICATION_TYPE,        E_ZCL_AF_RD,                            E_ZCL_UINT32,       (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->u32ApplicationType),  0}, /* Optional */
    #endif    

#endif    
        {E_CLD_GLOBAL_ATTR_ID_CLUSTER_REVISION,                         (E_ZCL_AF_RD|E_ZCL_AF_GA),               E_ZCL_UINT16,       (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->u16ClusterRevision),     0},   /* Mandatory  */
        
    #if (defined MULTISTATE_OUTPUT_BASIC_SERVER) && (defined CLD_MULTISTATE_OUTPUT_BASIC_ATTR_ATTRIBUTE_REPORTING_STATUS)
        {E_CLD_GLOBAL_ATTR_ID_ATTRIBUTE_REPORTING_STATUS,               (E_ZCL_AF_RD|E_ZCL_AF_GA),            E_ZCL_ENUM8,        (uint32)(&((tsCLD_MultistateOutputBasic*)(0))->u8AttributeReportingStatus),  0},  /* Optional */
    #endif
};

tsZCL_ClusterDefinition sCLD_MultistateOutputBasic = {
        GENERAL_CLUSTER_ID_MULTISTATE_OUTPUT_BASIC,
        FALSE,
        E_ZCL_SECURITY_NETWORK,
        (sizeof(asCLD_MultistateOutputBasicClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition)),
        (tsZCL_AttributeDefinition*)asCLD_MultistateOutputBasicClusterAttributeDefinitions,
        NULL
};
uint8 au8MultistateOutputBasicAttributeControlBits[(sizeof(asCLD_MultistateOutputBasicClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition))];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:       eCLD_MultistateOutputBasicCreateMultistateOutputBasic
 *
 * DESCRIPTION:
 * Creates a multistate output basic cluster
 *
 * PARAMETERS:  Name                         Usage
 *              psClusterInstance            Cluster structure
 *              bIsServer                    Server/Client Flag
 *              psClusterDefinition          Cluster Definitation
 *              pvEndPointSharedStructPtr    EndPoint Shared Structure Pointer
 *              pu8AttributeControlBits      Attribute Control Bits
 * RETURN:
 * teZCL_Status
 *
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_MultistateOutputBasicCreateMultistateOutputBasic(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits)
{


    #ifdef STRICT_PARAM_CHECK 
        /* Parameter check */
        if((psClusterInstance==NULL) ||
           (psClusterDefinition==NULL)  )
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif
    

    /* Create an instance of a multistate output basic cluster */
    vZCL_InitializeClusterInstance(
       psClusterInstance, 
       bIsServer,
       psClusterDefinition,
       pvEndPointSharedStructPtr,
       pu8AttributeControlBits,
       NULL,
       NULL);
       

    /* Initialise attributes defaults */
    if(psClusterInstance->pvEndPointSharedStructPtr != NULL)
    {
#ifdef MULTISTATE_OUTPUT_BASIC_SERVER
    #ifdef CLD_MULTISTATE_OUTPUT_BASIC_ATTR_DESCRIPTION
            ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->sDescription.u8MaxLength = sizeof(((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->au8Description);
            ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->sDescription.u8Length = 0;
            ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->sDescription.pu8Data = ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->au8Description;
    #endif

            ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->u16NumberOfStates = CLD_MULTISTATE_OUTPUT_BASIC_ATTR_NUMBER_OF_STATES;
            
            ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->bOutOfService = FALSE;
        
    #ifdef CLD_MULTISTATE_OUTPUT_BASIC_ATTR_RELIABILITY
            ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->u8Reliability = 0; //E_CLD_MULTISTATE_OUTPUT_BASIC_RELIABILITY_NO_FAULT_DETECTED
    #endif

            ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->u8StatusFlags = 0;
#endif            
        ((tsCLD_MultistateOutputBasic*)pvEndPointSharedStructPtr)->u16ClusterRevision = CLD_MULTISTATE_OUTPUT_BASIC_CLUSTER_REVISION;
    }

    /* As this cluster has reportable attributes enable default reporting */
    vZCL_SetDefaultReporting(psClusterInstance);
    
    return E_ZCL_SUCCESS;

}

#ifdef MULTISTATE_OUTPUT_BASIC_SERVER

/****************************************************************************
 **
 **  NAME:       eCLD_MultistateOutputBasicHandler
 **
 ** DESCRIPTION:
 ** Handles the dependency between Basic Multistate Output attributes
 **
 ** PARAMETERS          
 ** Type                    Name                         Usage
 ** uint8                   u8SourceEndPointId           Source Endpoint ID
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
 
PUBLIC  teZCL_Status eCLD_MultistateOutputBasicHandler(
                                    uint8                       u8SourceEndPointId)
{

    teZCL_Status eStatus;
    tsZCL_EndPointDefinition *psEndPointDefinition;
    tsZCL_ClusterInstance *psClusterInstance;
    tsCLD_MultistateOutputBasic *psSharedStruct;
    void *psCustomDataStructure;
    
    /* Find pointers to cluster */
    eStatus = eZCL_FindCluster(GENERAL_CLUSTER_ID_MULTISTATE_OUTPUT_BASIC, u8SourceEndPointId, TRUE, &psEndPointDefinition, &psClusterInstance, (void*)&psCustomDataStructure);
    if(eStatus != E_ZCL_SUCCESS)
    {
        return eStatus;
    }

    /* Point to shared struct */
    psSharedStruct = (tsCLD_MultistateOutputBasic *)psClusterInstance->pvEndPointSharedStructPtr;
    
    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    if(psSharedStruct->bOutOfService)
    {
        psSharedStruct->u8StatusFlags |= MULTISTATE_OUTPUT_BASIC_STATUS_FLAGS_OUT_OF_SERVICE_MASK;
    }else{
        psSharedStruct->u8StatusFlags  &=  ~(MULTISTATE_OUTPUT_BASIC_STATUS_FLAGS_OUT_OF_SERVICE_MASK);
    }
#ifdef CLD_MULTISTATE_OUTPUT_BASIC_ATTR_RELIABILITY
    if(psSharedStruct->u8Reliability != E_CLD_MULTISTATE_OUTPUT_BASIC_RELIABILITY_NO_FAULT_DETECTED)
        psSharedStruct->u8StatusFlags  |= MULTISTATE_OUTPUT_BASIC_STATUS_FLAGS_FAULT_MASK;
    else
        psSharedStruct->u8StatusFlags  &= ~(MULTISTATE_OUTPUT_BASIC_STATUS_FLAGS_FAULT_MASK);
#endif    

    // release EP
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    
    return E_ZCL_SUCCESS;
}
#endif

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

