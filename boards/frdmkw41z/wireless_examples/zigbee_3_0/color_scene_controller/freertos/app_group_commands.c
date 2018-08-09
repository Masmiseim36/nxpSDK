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
#include "app_group_commands.h"


#ifdef DEBUG_APP
    #define TRACE_APP   TRUE
#else
    #define TRACE_APP   FALSE
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
PUBLIC uint8 u8GroupId = 0;

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/



/****************************************************************************
 *
 * NAME: vAppAddGroup
 *
 * DESCRIPTION: send an add group to the currently selected light
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppAddGroup( uint16 u16GroupId, uint8 u8Index)
{

    tsCLD_Groups_AddGroupRequestPayload sPayload;
    uint8 u8Seq;
    tsZCL_Address sAddress;

    sAddress.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
    sAddress.uAddress.u16DestinationAddress = sEndpointTable.asEndpointRecords[u8Index].u16NwkAddr;


    sPayload.sGroupName.pu8Data = (uint8*)"";
    sPayload.sGroupName.u8Length = 0;
    sPayload.sGroupName.u8MaxLength = 0;
    sPayload.u16GroupId = u16GroupId;

    eCLD_GroupsCommandAddGroupRequestSend(
                            sDeviceTable.asDeviceRecords[0].u8Endpoint,
                            sEndpointTable.asEndpointRecords[u8Index].u8Endpoint   /* don't care group addr */,
                            &sAddress,
                            &u8Seq,
                            &sPayload);

}

/****************************************************************************
 *
 * NAME: vAppRemoveGroup
 *
 * DESCRIPTION:  send a remove group commanf to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppRemoveGroup( uint16 u16GroupId, bool_t bBroadcast)
{

    tsCLD_Groups_RemoveGroupRequestPayload sPayload;
    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, bBroadcast);

    sPayload.u16GroupId = u16GroupId;

    eCLD_GroupsCommandRemoveGroupRequestSend(
                            sDeviceTable.asDeviceRecords[0].u8Endpoint,
                            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint   /* don't care group addr */,
                            &sAddress,
                            &u8Seq,
                            &sPayload);

}

/****************************************************************************
 *
 * NAME: vAppRemoveAllGroups
 *
 * DESCRIPTION: send a remove all groups command to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppRemoveAllGroups(bool_t bBroadcast)
{

    uint8 u8Seq;
    tsZCL_Address sAddress;

    vSetAddress(&sAddress, bBroadcast);

    eCLD_GroupsCommandRemoveAllGroupsRequestSend(
                            sDeviceTable.asDeviceRecords[0].u8Endpoint,
                            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint   /* don't care group addr */,
                            &sAddress,
                            &u8Seq);

}

/****************************************************************************
 *
 * NAME: bAppSendGroupMembershipReq
 *
 * DESCRIPTION: send a get group membership to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void bAppSendGroupMembershipReq(uint8 u8GroupCount, bool_t bBroadcast)
{
    uint8 u8SeqNum;
    tsZCL_Address sAddress;
    tsCLD_Groups_GetGroupMembershipRequestPayload sGroupMembershipReqPayload;
    teZCL_Status eZCL_Status;

    zint16 au16GroupList[] = { 0x0000 };

    sGroupMembershipReqPayload.u8GroupCount = u8GroupCount;
    sGroupMembershipReqPayload.pi16GroupList = &au16GroupList[0];

    vSetAddress(&sAddress, bBroadcast);

    eZCL_Status = eCLD_GroupsCommandGetGroupMembershipRequestSend(
                            sDeviceTable.asDeviceRecords[0].u8Endpoint, /*SrcEP*/
                            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint, /*DstEP*/
                            &sAddress,
                            &u8SeqNum,
                            &sGroupMembershipReqPayload);

    DBG_vPrintf(TRACE_APP,"\neCLD_GroupsCommandGetGroupMembershipRequestSend  eZCL_Status = %d  \n",eZCL_Status);
}

/****************************************************************************
 *
 * NAME: bAppSendViewGroupReq
 *
 * DESCRIPTION: send a view group command to a light or lights
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void bAppSendViewGroupReq(uint16 u16GroupId, bool_t bBroadcast)
{
    uint8 u8SeqNum;
    tsZCL_Address sAddress;
    tsCLD_Groups_ViewGroupRequestPayload sViewGroupReqPayload;
    teZCL_Status eZCL_Status;

    sViewGroupReqPayload.u16GroupId = u16GroupId;

    vSetAddress(&sAddress, bBroadcast);

    eZCL_Status = eCLD_GroupsCommandViewGroupRequestSend(
                            sDeviceTable.asDeviceRecords[0].u8Endpoint,
                            sEndpointTable.asEndpointRecords[sEndpointTable.u8CurrentLight].u8Endpoint, /*DstEP*/
                            &sAddress,
                            &u8SeqNum,
                            &sViewGroupReqPayload);

    DBG_vPrintf(TRACE_APP,"\neCLD_GroupsCommandViewGroupRequestSend  eZCL_Status = %d  \n",eZCL_Status);
}


/****************************************************************************
 *
 * NAME: vAppSetGroupId
 *
 * DESCRIPTION:
 * sets the group id
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAppSetGroupId(uint8 u8GroupId_l)
{
    u8GroupId = u8GroupId_l;
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
