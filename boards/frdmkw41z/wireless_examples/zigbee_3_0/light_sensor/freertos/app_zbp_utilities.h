/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_ZBP_UTILITIES_H_
#define APP_ZBP_UTILITIES_H_

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "zps_apl_af.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void vClearDiscNT(void);
PUBLIC void vRemoveCoordParents(void);
PUBLIC void vDisplayDiscNT(void);
PUBLIC bool bInRoutingTable(uint16 u16ShortAddress);
PUBLIC void vDisplayRoutingTable(void);
PUBLIC void vDisplayTableSizes(void);
PUBLIC void vDisplayAddressMapTable(void);
PUBLIC void vDisplayNT(void);
PUBLIC void vClearNTEntry( uint64 u64AddressToRemove );
PUBLIC void vClearRoutingTable(void);
PUBLIC void vDisplayRouteRecordTable(void);
PUBLIC void vClearRouteRecordTable(void);
PUBLIC void vDisplayAPSTable(void);
PUBLIC void vDisplayNWKTransmitTable(void);
PUBLIC void vDisplayStackEvent( ZPS_tsAfEvent sStackEvent );
PUBLIC void vStartStopTimer(uint8 u8Timer, uint32 u32Ticks,uint8* pu8State, uint8 eNextState);
PUBLIC void vEnablePermitJoin(uint8 u8Time);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        In line  Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_ZBP_UTILITIES_H_*/
