/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
/*!=============================================================================
\file       on_off_light.c
\brief      ZigBee Light Link profile functions
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zps_apl.h"
#include "zcl_heap.h"
#include "on_off_light.h"
#ifdef CLD_OTA
#include "OTA.h"
#endif

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
 * NAME: eZLO_RegisterOnOffLightEndPoint
 *
 * DESCRIPTION:
 * Registers a on off device with the ZCL layer
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

PUBLIC teZCL_Status eZLO_RegisterOnOffLightEndPoint(uint8 u8EndPointIdentifier,
                                              tfpZCL_ZCLCallBackFunction cbCallBack,
                                              tsZLO_OnOffLightDevice *psDeviceInfo)
{

    /* Fill in end point details */
    psDeviceInfo->sEndPoint.u8EndPointNumber = u8EndPointIdentifier;
    psDeviceInfo->sEndPoint.u16ManufacturerCode = ZCL_MANUFACTURER_CODE;
    psDeviceInfo->sEndPoint.u16ProfileEnum = HA_PROFILE_ID;
    psDeviceInfo->sEndPoint.bIsManufacturerSpecificProfile = FALSE;
    psDeviceInfo->sEndPoint.u16NumberOfClusters = sizeof(tsZLO_OnOffLightDeviceClusterInstances) / sizeof(tsZCL_ClusterInstance);
    psDeviceInfo->sEndPoint.psClusterInstance = (tsZCL_ClusterInstance*)&psDeviceInfo->sClusterInstance;
    psDeviceInfo->sEndPoint.bDisableDefaultResponse = ZCL_DISABLE_DEFAULT_RESPONSES;
    psDeviceInfo->sEndPoint.pCallBackFunctions = cbCallBack;

    #if (defined CLD_BASIC) && (defined BASIC_SERVER)
        /* Create an instance of a basic cluster as a server */
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

    #if (defined CLD_ONOFF) && (defined ONOFF_SERVER)
        /* Create an instance of a On/Off cluster as a server */
        if(eCLD_OnOffCreateOnOff(&psDeviceInfo->sClusterInstance.sOnOffServer,
                              TRUE,
                              &sCLD_OnOff,
                              &psDeviceInfo->sOnOffServerCluster,
                              &au8OnOffAttributeControlBits[0],
                              &psDeviceInfo->sOnOffServerCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }  
    #endif

    #if (defined CLD_SCENES) && (defined SCENES_SERVER)
        /* Create an instance of a Scenes cluster as a server */
        if(eCLD_ScenesCreateScenes(&psDeviceInfo->sClusterInstance.sScenesServer,
                              TRUE,
                              &sCLD_Scenes,
                              &psDeviceInfo->sScenesServerCluster,
                              &au8ScenesAttributeControlBits[0],
                              &psDeviceInfo->sScenesServerCustomDataStructure,
                              &psDeviceInfo->sEndPoint) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        } 
    #endif

    #if (defined CLD_GROUPS) && (defined GROUPS_SERVER)
        /* Create an instance of a Groups cluster as a server */
        if(eCLD_GroupsCreateGroups(&psDeviceInfo->sClusterInstance.sGroupsServer,
                              TRUE,
                              &sCLD_Groups,
                              &psDeviceInfo->sGroupsServerCluster,
                              &au8GroupsAttributeControlBits[0],
                              &psDeviceInfo->sGroupsServerCustomDataStructure,
                              &psDeviceInfo->sEndPoint) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        } 
    #endif

    #if (defined CLD_LEVEL_CONTROL) && (defined LEVEL_CONTROL_SERVER)
        /* Create an instance of a Level Control cluster as a server */
        if(eCLD_LevelControlCreateLevelControl(&psDeviceInfo->sClusterInstance.sLevelControlServer,
                              TRUE,
                              &sCLD_LevelControl,
                              &psDeviceInfo->sLevelControlServerCluster,
                              &au8LevelControlAttributeControlBits[0],
                              &psDeviceInfo->sLevelControlServerCustomDataStructure) != E_ZCL_SUCCESS)
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
    
    #if (defined CLD_OCCUPANCY_SENSING) && (defined OCCUPANCY_SENSING_CLIENT)
        /* Create an instance of an Occupancy Sensing cluster as a client */
        if(eCLD_OccupancySensingCreateOccupancySensing(
                              &psDeviceInfo->sClusterInstance.sOccupancySensingClient,
                              FALSE,
                              &sCLD_OccupancySensing,
                              &psDeviceInfo->sOccupancySensingClientCluster,
                              &au8OccupancySensingAttributeControlBits[0])!= E_ZCL_SUCCESS)
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

