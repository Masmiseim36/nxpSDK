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
\file       colour_controller.c
\brief      ZigBee Smart Lighting profile functions
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zps_apl.h"
#include "zcl_heap.h"
#include "colour_controller.h"

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

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: eZLO_RegisterColourControllerEndPoint
 *
 * DESCRIPTION:
 * Registers a colour remote device with the ZCL layer
 *
 * PARAMETERS:  Name                            Usage
 *              u8EndPointIdentifier            Endpoint being registered
 *              cbCallBack                      Pointer to endpoint callback
 *              psDeviceInfo                    Pointer to struct containing
 *                                              data for endpoint
 *
 * RETURNS:
 * teZCL_Status
 *
 ****************************************************************************/

PUBLIC teZCL_Status eZLO_RegisterColourControllerEndPoint(uint8 u8EndPointIdentifier,
                                              tfpZCL_ZCLCallBackFunction cbCallBack,
                                              tsZLO_ColourControllerDevice *psDeviceInfo )
{

    /* Fill in end point details */
    psDeviceInfo->sEndPoint.u8EndPointNumber = u8EndPointIdentifier;
    psDeviceInfo->sEndPoint.u16ManufacturerCode = ZCL_MANUFACTURER_CODE;
    psDeviceInfo->sEndPoint.u16ProfileEnum = HA_PROFILE_ID;
    psDeviceInfo->sEndPoint.bIsManufacturerSpecificProfile = FALSE;
    psDeviceInfo->sEndPoint.u16NumberOfClusters = sizeof(tsZLO_ColourControllerDeviceClusterInstances) / sizeof(tsZCL_ClusterInstance);
    psDeviceInfo->sEndPoint.psClusterInstance = (tsZCL_ClusterInstance*)&psDeviceInfo->sClusterInstance;
    psDeviceInfo->sEndPoint.bDisableDefaultResponse = ZCL_DISABLE_DEFAULT_RESPONSES;
    psDeviceInfo->sEndPoint.pCallBackFunctions = cbCallBack;

    #if (defined CLD_BASIC) && (defined BASIC_SERVER)
        /* Create an instance of a Basic cluster as a server */
        if(eCLD_BasicCreateBasic(&psDeviceInfo->sClusterInstance.sBasicServer,
                              TRUE,
                              &sCLD_Basic,
                              &psDeviceInfo->sBasicServerCluster,
                              &au8BasicClusterAttributeControlBits[0]) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }    
    #endif

    #if (defined CLD_IDENTIFY) && (defined IDENTIFY_SERVER)
        /* Create an instance of an Identify cluster as a server */
        if(eCLD_IdentifyCreateIdentify(&psDeviceInfo->sClusterInstance.sIdentifyServer,
                              TRUE,
                              &sCLD_Identify,
                              &psDeviceInfo->sIdentifyServerCluster,
                              &au8IdentifyAttributeControlBits[0],
                              &psDeviceInfo->sIdentifyServerCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }     
    #endif

    #if (defined CLD_ZLL_COMMISSION) && (defined ZLL_COMMISSION_SERVER)
        /* Create an instance of a basic cluster as a server */
        if(eCLD_ZllCommissionCreateCommission(&psDeviceInfo->sClusterInstance.sZllCommissionServer,
                              TRUE,
                              &sCLD_ZllCommission,
                              &psDeviceInfo->sZllCommissionServerCluster,
                              &au8ZllCommissionAttributeControlBits[0],
                              &psDeviceInfo->sZllCommissionServerCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }
    #endif

    /* Mandatory client clusters */
    #if (defined CLD_IDENTIFY) && (defined IDENTIFY_CLIENT)
        /* Create an instance of an Identify cluster as a client */
        if(eCLD_IdentifyCreateIdentify(&psDeviceInfo->sClusterInstance.sIdentifyClient,
                              FALSE,
                              &sCLD_Identify,
                              &psDeviceInfo->sIdentifyClientCluster,
                              &au8IdentifyAttributeControlBits[0],
                              &psDeviceInfo->sIdentifyClientCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }    
    #endif
    
    #if (defined CLD_BASIC) && (defined BASIC_CLIENT)
        /* Create an instance of a Basic cluster as a client */
        if(eCLD_BasicCreateBasic(&psDeviceInfo->sClusterInstance.sBasicClient,
                              FALSE,
                              &sCLD_Basic,
                              &psDeviceInfo->sBasicClientCluster,
                              &au8BasicClusterAttributeControlBits[0]) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }  
    #endif
    
    #if (defined CLD_ONOFF) && (defined ONOFF_CLIENT)
        /* Create an instance of an On/Off cluster as a client */
        if(eCLD_OnOffCreateOnOff(&psDeviceInfo->sClusterInstance.sOnOffClient,
                              FALSE,
                              &sCLD_OnOff,
                              &psDeviceInfo->sOnOffClientCluster,
                              &au8OnOffAttributeControlBits[0],
                              NULL  /* no cust data struct for client */) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }                                
    #endif

    #if (defined CLD_LEVEL_CONTROL) && (defined LEVEL_CONTROL_CLIENT)
        /* Create an instance of a Level Control cluster as a client */
        if(eCLD_LevelControlCreateLevelControl(&psDeviceInfo->sClusterInstance.sLevelControlClient,
                              FALSE,
                              &sCLD_LevelControl,
                              &psDeviceInfo->sLevelControlClientCluster,
                              &au8LevelControlAttributeControlBits[0],
                              &psDeviceInfo->sLevelControlClientCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        } 
    #endif

    #if (defined CLD_COLOUR_CONTROL) && (defined COLOUR_CONTROL_CLIENT)
        /* Create an instance of a Colour Control cluster as a client */
        if(eCLD_ColourControlCreateColourControl(
                              &psDeviceInfo->sClusterInstance.sColourControlClient,
                              FALSE,
                              &sCLD_ColourControl,
                              &psDeviceInfo->sColourControlClientCluster,
                              &au8ColourControlAttributeControlBits[0],
                              &psDeviceInfo->sColourControlClientCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }  
    #endif

    /* Recommended Optional client cluster */
    #if (defined CLD_GROUPS) && (defined GROUPS_CLIENT)
        /* Create an instance of a Groups cluster as a client */
        if(eCLD_GroupsCreateGroups(&psDeviceInfo->sClusterInstance.sGroupsClient,
                              FALSE,
                              &sCLD_Groups,
                              &psDeviceInfo->sGroupsClientCluster,
                              &au8GroupsAttributeControlBits[0],
                              &psDeviceInfo->sGroupsClientCustomDataStructure,
                              &psDeviceInfo->sEndPoint) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        } 
    #endif

    #if (defined CLD_OTA) && (defined OTA_CLIENT)
        /* Create an instance of an OTA cluster as a client */
       if(eOTA_Create(&psDeviceInfo->sClusterInstance.sOTAClient,
                      FALSE,  /* client */
                      &sCLD_OTA,
                      &psDeviceInfo->sCLD_OTA,  /* cluster definition */
                      u8EndPointIdentifier,
                      au8OTAAttributeControlBits, 
                      &psDeviceInfo->sCLD_OTA_CustomDataStruct)!= E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }
    #endif
    
    #if (defined CLD_ZLL_COMMISSION) && (defined ZLL_COMMISSION_CLIENT)
        /* Create an instance of a basic cluster as a server */
        if(eCLD_ZllCommissionCreateCommission(&psDeviceInfo->sClusterInstance.sZllCommissionClient,
                              FALSE,
                              &sCLD_ZllCommission,
                              &psDeviceInfo->sZllCommissionClientCluster,
                              &au8ZllCommissionAttributeControlBits[0],
                              &psDeviceInfo->sZllCommissionClientCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }
    #endif

    return eZCL_Register(&psDeviceInfo->sEndPoint);
}


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
