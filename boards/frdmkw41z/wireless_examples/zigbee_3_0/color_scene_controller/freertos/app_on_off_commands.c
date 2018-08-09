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
#include "app_on_off_commands.h"


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
 * NAME: vAppOnOff
 *
 * DESCRIPTION: sends an on off command to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppOnOff(teCLD_OnOff_Command eCmd)
{
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, FALSE);

    if ((eCmd == E_CLD_ONOFF_CMD_ON) ||
         (eCmd == E_CLD_ONOFF_CMD_OFF) ||
         (eCmd == E_CLD_ONOFF_CMD_TOGGLE) ||
         (eCmd == E_CLD_ONOFF_CMD_ON_RECALL_GLOBAL_SCENE))
    {
        eCLD_OnOffCommandSend( sDeviceTable.asDeviceRecords[0].u8Endpoint,
                               sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                               &sAddress,
                               &u8Seq,
                               eCmd);
    } else if (eCmd == E_CLD_ONOFF_CMD_OFF_EFFECT)
    {
        tsCLD_OnOff_OffWithEffectRequestPayload sPayload;
        sPayload.u8EffectId = 0;
        sPayload.u8EffectVariant = 0;

        eCLD_OnOffCommandOffWithEffectSend(
                sDeviceTable.asDeviceRecords[0].u8Endpoint,
                sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                &sAddress, &u8Seq, &sPayload);
    }
    else if (E_CLD_ONOFF_CMD_ON_TIMED_OFF)
    {
        tsCLD_OnOff_OnWithTimedOffRequestPayload sPayload;
        sPayload.u8OnOff = 0x00;  // 0x01 for accept if OnOff = 1
        sPayload.u16OnTime = 0;   // 1/10th of a second
        sPayload.u16OffTime = 20; // 1/10th of a second

        eCLD_OnOffCommandOnWithTimedOffSend(
                sDeviceTable.asDeviceRecords[0].u8Endpoint,
                sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                &sAddress, &u8Seq, &sPayload);
    }

}

/****************************************************************************
 *
 * NAME: vAppOffEffect
 *
 * DESCRIPTION: sends an off with effect command to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
#ifndef UNUSED_COMMANDS
PRIVATE void vAppOffEffect( uint8 u8Effect, uint8 u8Varient)
{
    uint8 u8Seq;
    tsZCL_Address sAddress;
    tsCLD_OnOff_OffWithEffectRequestPayload sPayload;

    sPayload.u8EffectId = u8Effect;
    sPayload.u8EffectVariant = u8Varient;


    sAddress.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
    sAddress.uAddress.u16DestinationAddress = sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u16NwkAddr;

    eCLD_OnOffCommandOffWithEffectSend(sDeviceTable.asDeviceRecords[0].u8Endpoint,
            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint,
                            &sAddress,
                            &u8Seq,
                            &sPayload );
}
#endif



/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
