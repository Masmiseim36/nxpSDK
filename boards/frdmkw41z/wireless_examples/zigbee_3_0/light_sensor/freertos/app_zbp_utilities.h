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
