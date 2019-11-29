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
#include "dbg.h"
#include "app_zcl_cfg.h"
#include "app_pdum_cfg.h"
#include "app_common.h"
#include "zlo_controller_node.h"
#include "app_menu_handler.h"
#include "app_general_commands.h"
#include "app_identify_commands.h"

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
 * NAME: vAppIdentifyEffect
 *
 * DESCRIPTION: sends the specified identify effect to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppIdentifyEffect( teCLD_Identify_EffectId eEffect)
{
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress,FALSE);

    eCLD_IdentifyCommandTriggerEffectSend(sDeviceTable.asDeviceRecords[0].u8Endpoint,
            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                            &sAddress,
                            &u8Seq,
                            eEffect,
                            0 /* Effect varient */);
}

/****************************************************************************
 *
 * NAME: vAppIdentifyQuery
 *
 * DESCRIPTION: sends an identify query command to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppIdentifyQuery( void)
{
    uint8 u8Seq;
    tsZCL_Address sAddress;


    sAddress.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
    sAddress.uAddress.u16DestinationAddress = sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u16NwkAddr;

    eCLD_IdentifyCommandIdentifyQueryRequestSend(sDeviceTable.asDeviceRecords[0].u8Endpoint,
            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                            &sAddress,
                            &u8Seq);
}

/****************************************************************************
 *
 * NAME: vAppIdentify
 *
 * DESCRIPTION: sends an identify command to a light
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppIdentify( uint16 u16Time)
{
    uint8 u8Seq;
    tsZCL_Address sAddress;
    tsCLD_Identify_IdentifyRequestPayload sPayload;

    sPayload.u16IdentifyTime = u16Time;


    sAddress.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
    sAddress.uAddress.u16DestinationAddress = sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u16NwkAddr;

    eCLD_IdentifyCommandIdentifyRequestSend(sDeviceTable.asDeviceRecords[0].u8Endpoint,
            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                            &sAddress,
                            &u8Seq,
                            &sPayload);
}



/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
