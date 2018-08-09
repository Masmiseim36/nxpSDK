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
\file       ApplianceIdentification.c
\brief      Appliance Identification Cluster definition
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
#include "ApplianceIdentification.h"
#include "zcl_options.h"


#ifdef CLD_APPLIANCE_IDENTIFICATION

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
const tsZCL_AttributeDefinition asCLD_ApplianceIdentificationClusterAttributeDefinitions[] = {
#ifdef APPLIANCE_IDENTIFICATION_SERVER
        /* ZigBee Cluster Library Version */
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_BASIC_IDENTIFICATION,             E_ZCL_AF_RD,                E_ZCL_UINT56,    (uint32)(&((tsCLD_ApplianceIdentification*)(0))->u64BasicIdentification),        0},  /* Mandatory */

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_COMPANY_NAME
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_COMPANY_NAME ,                       E_ZCL_AF_RD,                E_ZCL_CSTRING,    (uint32)(&((tsCLD_ApplianceIdentification*)(0))->sCompanyName),                    0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_COMPANY_ID
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_COMPANY_ID,                       E_ZCL_AF_RD,                E_ZCL_UINT16,    (uint32)(&((tsCLD_ApplianceIdentification*)(0))->u16CompanyId),                    0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_BRAND_NAME
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_BRAND_NAME,                          E_ZCL_AF_RD,                E_ZCL_CSTRING,    (uint32)(&((tsCLD_ApplianceIdentification*)(0))->sBrandName),                    0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_BRAND_ID
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_BRAND_ID,                         E_ZCL_AF_RD,                E_ZCL_UINT16,  (uint32)(&((tsCLD_ApplianceIdentification*)(0))->u16BrandId),                        0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_MODEL
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_MODEL,                                E_ZCL_AF_RD,                E_ZCL_OSTRING,  (uint32)(&((tsCLD_ApplianceIdentification*)(0))->sModel),                            0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_PART_NUMBER
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_PART_NUMBER,                         E_ZCL_AF_RD,                E_ZCL_OSTRING,  (uint32)(&((tsCLD_ApplianceIdentification*)(0))->sPartNumber),                    0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_PRODUCT_REVISION
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_PRODUCT_REVISION,                  E_ZCL_AF_RD,                E_ZCL_OSTRING,  (uint32)(&((tsCLD_ApplianceIdentification*)(0))->sProductRevision),                0},
    #endif
        
    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_SOFTWARE_REVISION
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_SOFTWARE_REVISION,                  E_ZCL_AF_RD,                E_ZCL_OSTRING,  (uint32)(&((tsCLD_ApplianceIdentification*)(0))->sSoftwareRevision),                0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_PRODUCT_TYPE_NAME
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_PRODUCT_TYPE_NAME,                   E_ZCL_AF_RD,                E_ZCL_OSTRING,  (uint32)(&((tsCLD_ApplianceIdentification*)(0))->sProductTypeName),                0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_PRODUCT_TYPE_ID
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_PRODUCT_TYPE_ID,                    E_ZCL_AF_RD,                 E_ZCL_UINT16,   (uint32)(&((tsCLD_ApplianceIdentification*)(0))->u16ProductTypeId),                0},
    #endif

    #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_CECED_SPEC_VERSION
        {E_CLD_APPLIANCE_IDENTIFICATION_ATTR_ID_CECED_SPEC_VERSION,                 E_ZCL_AF_RD,                E_ZCL_UINT8,    (uint32)(&((tsCLD_ApplianceIdentification*)(0))->u8CECEDSpecificationVersion),    0},
    #endif
#endif
        {E_CLD_GLOBAL_ATTR_ID_CLUSTER_REVISION,                                     (E_ZCL_AF_RD|E_ZCL_AF_GA),   E_ZCL_UINT16,   (uint32)(&((tsCLD_ApplianceIdentification*)(0))->u16ClusterRevision),      0},   /* Mandatory  */    
};

tsZCL_ClusterDefinition sCLD_ApplianceIdentification = {
        APPLIANCE_MANAGEMENT_CLUSTER_ID_APPLIANCE_IDENTIFICATION,
        FALSE,
        E_ZCL_SECURITY_NETWORK,
        (sizeof(asCLD_ApplianceIdentificationClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition)),
        (tsZCL_AttributeDefinition*)asCLD_ApplianceIdentificationClusterAttributeDefinitions,
        NULL
};
uint8 au8ApplianceIdentificationAttributeControlBits[(sizeof(asCLD_ApplianceIdentificationClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition))];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:       eCLD_ApplianceIdentificationCreateApplianceIdentification
 *
 * DESCRIPTION:
 * Creates a basic cluster
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
PUBLIC  teZCL_Status eCLD_ApplianceIdentificationCreateApplianceIdentification(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits)
{
    tsCLD_ApplianceIdentification *psCluster;
    
    #ifdef STRICT_PARAM_CHECK 
        /* Parameter check */
        if((psClusterInstance==NULL) ||
           (psClusterDefinition==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif

    /* Create an instance of a appliance identification cluster */
    vZCL_InitializeClusterInstance(
                           psClusterInstance, 
                           bIsServer,
                           psClusterDefinition,
                           pvEndPointSharedStructPtr,
                           pu8AttributeControlBits,
                           NULL,
                           NULL);
   
        /* Initialise attributes defaults */   
        if(pvEndPointSharedStructPtr != NULL)
        {
            psCluster = (tsCLD_ApplianceIdentification*)pvEndPointSharedStructPtr;
        #ifdef APPLIANCE_IDENTIFICATION_SERVER  
            
            psCluster->u64BasicIdentification = CLD_APPLIANCE_IDENTIFICATION_ATTR_BASIC_IDENTIFICATION;

            #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_COMPANY_NAME
                psCluster->sCompanyName.u8MaxLength = sizeof(psCluster->au8CompanyName);
                psCluster->sCompanyName.u8Length = 0;
                psCluster->sCompanyName.pu8Data = psCluster->au8CompanyName;
            #endif

            #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_BRAND_NAME
                psCluster->sBrandName.u8MaxLength = sizeof(psCluster->au8BrandName);
                psCluster->sBrandName.u8Length = 0;
                psCluster->sBrandName.pu8Data = psCluster->au8BrandName;
            #endif

            #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_MODEL
                psCluster->sModel.u8MaxLength = sizeof(psCluster->au8Model);
                psCluster->sModel.u8Length = 0;
                psCluster->sModel.pu8Data = psCluster->au8Model;
            #endif

            #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_PART_NUMBER
                psCluster->sPartNumber.u8MaxLength = sizeof(psCluster->au8PartNumber);
                psCluster->sPartNumber.u8Length = 0;
                psCluster->sPartNumber.pu8Data = psCluster->au8PartNumber;
            #endif

            #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_PRODUCT_REVISION
                psCluster->sProductRevision.u8MaxLength = sizeof(psCluster->au8ProductRevision);
                psCluster->sProductRevision.u8Length = 0;
                psCluster->sProductRevision.pu8Data = psCluster->au8ProductRevision;
            #endif

            #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_SOFTWARE_REVISION
                psCluster->sSoftwareRevision.u8MaxLength = sizeof(psCluster->au8SoftwareRevision);
                psCluster->sSoftwareRevision.u8Length = 0;
                psCluster->sSoftwareRevision.pu8Data = psCluster->au8SoftwareRevision;
            #endif

            #ifdef CLD_APPLIANCE_IDENTIFICATION_ATTR_PRODUCT_TYPE_NAME
                psCluster->sProductTypeName.u8MaxLength = sizeof(psCluster->au8ProductTypeName);
                psCluster->sProductTypeName.u8Length = 0;
                psCluster->sProductTypeName.pu8Data = psCluster->au8ProductTypeName;
            #endif
    #endif        
            psCluster->u16ClusterRevision = CLD_APPLIANCE_IDENTIFICATION_CLUSTER_REVISION;
        }
    return E_ZCL_SUCCESS;

}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

