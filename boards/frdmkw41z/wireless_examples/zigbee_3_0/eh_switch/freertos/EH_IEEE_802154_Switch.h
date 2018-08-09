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


#ifndef EH_IEEE_802154_SWITCH_H_
#define EH_IEEE_802154_SWITCH_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"

#include "string.h"
#include <dbg.h>
#include "EH_IEEE_802154_Switch.h"
#include "EH_Switch_Configurations.h"


/****************************************************************************/
/***        Macro Definitions                                          ***/
/****************************************************************************/
#ifndef DBG_DEVICE_SWITCH
#define DBG_DEVICE_SWITCH FALSE
#endif

/* Customer MAC address at page 5, word 7 (16-byte words, 16 words/page) */
#define MAC_ADDR1_BASE 0x01001570
/* Default MAC address at page 5, word 8 (16-byte words, 16 words/page) */
#define MAC_ADDR0_BASE 0x01001580

#define GP_ON_OFF_SWITCH                            0x02
#define GP_LEVEL_CONTROL_SWITCH                     0x03
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef uint8     bmap8;

/****************************************************************************/
/***        Exported Variables                                           ***/
/****************************************************************************/
extern volatile bool_t bRxTimerExpired;
extern volatile bool_t bEnableSleep;

/****************************************************************************/
/***        Exported Functions                                           ***/
/****************************************************************************/


PUBLIC void vSendMACFrame( bool bUpdatePersistentData);
void vEnableRxAndReceive(void);
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* EH_IEEE_802154_SWITCH_H_ */
