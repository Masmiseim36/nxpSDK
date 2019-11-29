/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
