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
