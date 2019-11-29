/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "app_zcl_cfg.h"
#include "App_ColorSceneController.h"
#include "bdb_options.h"
#include <string.h>
#include "zlo_device_id.h"
#include "FunctionLib.h"
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
tsZLO_ColourSceneControllerDevice sRemote;
tsCLD_ZllDeviceTable sDeviceTable = { ZLO_NUMBER_DEVICES,
                                       {
                                          { 0,
                                            HA_PROFILE_ID,
                                            ZLO_COLOUR_SCENE_CONTROLLER_DEVICE_ID,
                                            COLORSCENECONTROLLER_REMOTE_ENDPOINT,
                                            APPLICATION_DEVICE_VERSION,
                                            GROUPS_REQUIRED,
                                            0
                                          }
                                       }
};

#ifdef BDB_APPLICATION_DEFINED_TL_MASTER_KEY
PUBLIC tsReg128 sTLMasterKey = {0x11223344, 0x55667788, 0x99aabbcc, 0xddeeff00 };
#endif

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PRIVATE bool bAllowInterPanEp(uint8 u8Ep, uint16 u16ProfileId);

/****************************************************************************
 *
 * NAME: eApp_ZLO_RegisterEndpoint
 *
 * DESCRIPTION:
 * Register ZLO endpoints
 *
 * PARAMETER
 * Type                                Name                    Descirption
 * tfpZCL_ZCLCallBackFunction          fptr                    Pointer to ZCL Callback function
 *
 * RETURNS:
 * teZCL_Status
 *
 ****************************************************************************/
teZCL_Status eApp_ZLO_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr)
{


    ZPS_vAplZdoRegisterInterPanFilter( bAllowInterPanEp);
    zps_vSetIgnoreProfileCheck();

     return eZLO_RegisterColourSceneControllerEndPoint( COLORSCENECONTROLLER_REMOTE_ENDPOINT,
                                                  fptr,
                                                  &sRemote);
}

/****************************************************************************
 *
 * NAME: vAPP_ZCL_DeviceSpecific_Init
 *
 * DESCRIPTION:
 * ZLO Device Specific initialization
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
void vAPP_ZCL_DeviceSpecific_Init()
{
    /* Initialise the strings in Basic */
    uint8 au8PCode[CLD_BAS_PCODE_SIZE] = { 1, 2, 3, 4};
    FLib_MemCpy(sRemote.sBasicServerCluster.au8ManufacturerName, "NXP", CLD_BAS_MANUF_NAME_SIZE);
    FLib_MemCpy(sRemote.sBasicServerCluster.au8ModelIdentifier, "ZLO-ColorSceneController", CLD_BAS_MODEL_ID_SIZE);
    FLib_MemCpy(sRemote.sBasicServerCluster.au8DateCode, "20160301", CLD_BAS_DATE_SIZE);
    FLib_MemCpy(sRemote.sBasicServerCluster.au8SWBuildID, "2000-0001", CLD_BAS_SW_BUILD_SIZE);
    FLib_MemCpy(sRemote.sBasicServerCluster.au8ProductURL, "www.nxp.com", CLD_BAS_URL_SIZE);
    FLib_MemCpy(sRemote.sBasicServerCluster.au8ProductCode, au8PCode, CLD_BAS_PCODE_SIZE);
    #ifdef CLD_BAS_ATTR_MANUFACTURER_VERSION_DETAILS
    FLib_MemCpy(sRemote.sBasicServerCluster.au8ManufacturerVersionDetails, "Zigbee_Version_3.0", CLD_BAS_MANUFACTURER_VERSION_SIZE);
    #endif
    #ifdef CLD_BAS_ATTR_SERIAL_NUMBER
    FLib_MemCpy(sRemote.sBasicServerCluster.au8SerialNumber, "1234", CLD_BAS_SERIAL_NUMBER_SIZE);
    #endif
    #ifdef CLD_BAS_ATTR_PRODUCT_LABEL
    FLib_MemCpy(sRemote.sBasicServerCluster.au8ProductLabel, "Kinetis_KW41Z", CLD_BAS_PRODUCT_LABEL_SIZE);
    #endif

}

/****************************************************************************
 *
 * NAME: vApp_ZCL_ResetDeviceStructure
 *
 * DESCRIPTION:
 * Resets the device structure
 *
 * PARAMETER: void
 *
 * RETURNS: void
 *
 ****************************************************************************/
PUBLIC void vApp_ZCL_ResetDeviceStructure(void)
{
    FLib_MemSet(&sRemote,0,sizeof(tsZLO_ColourSceneControllerDevice));
}

/****************************************************************************
*
* NAME: bAllowInterPanEp
*
* DESCRIPTION: Allows the application to decide which end point receive
* inter pan messages
*
*
* PARAMETER: the end point receiving the inter pan
*
* RETURNS: True to allow reception, False otherwise
*
****************************************************************************/
PRIVATE bool bAllowInterPanEp(uint8 u8Ep, uint16 u16ProfileId) {

    if ((u8Ep == COLORSCENECONTROLLER_REMOTE_ENDPOINT) &&
            (u16ProfileId == ZLL_PROFILE_ID))
    {
        return TRUE;
    }
    return FALSE;
}

/****************************************************************************
 *
 * NAME: app_u8GetDeviceEndpoint
 *
 * DESCRIPTION:
 * Return the application endpoint
 *
 * PARAMETER: void
 *
 * RETURNS: uint8
 *
 ****************************************************************************/
PUBLIC uint8 app_u8GetDeviceEndpoint( void)
{
	return COLORSCENECONTROLLER_REMOTE_ENDPOINT;
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
