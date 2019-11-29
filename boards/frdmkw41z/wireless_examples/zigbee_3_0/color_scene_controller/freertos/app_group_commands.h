/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP__H_
#define aPP__NODEH_

#include "Groups.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/



/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vAppAddGroup(uint16 u16GroupId, uint8 u8Index);
PUBLIC void vAppRemoveGroup(uint16 u16GroupId, bool_t bBroadcast);
PUBLIC void vAppRemoveAllGroups(bool_t bBroadcast);
PUBLIC void bAppSendGroupMembershipReq(uint8 u8GroupCount, bool_t bBroadcast);
PUBLIC void bAppSendViewGroupReq(uint16 u16GroupId, bool_t bBroadcast);
PUBLIC void vAppSetGroupId(uint8 u8GroupId_l);




/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC uint8 u8GroupId;


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP___H_*/
