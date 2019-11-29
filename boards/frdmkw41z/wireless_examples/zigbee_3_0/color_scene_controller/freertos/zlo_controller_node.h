/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef ZLO_CONTROLLER_NODEH_
#define ZLO_CONTROLLER_NODEH_

#include "zps_nwk_sap.h"
#include "zps_nwk_nib.h"
#include "zps_nwk_pub.h"
#include "zps_apl.h"
#include "zps_apl_zdo.h"

#include "app_common.h"

#include "zcl_customcommand.h"
#include "zll_utility.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define KEEP_ALIVE_FACTORY_NEW          (45)
#define KEEP_ALIVETIME                  (20)
#define FIND_AND_BIND_IME               (182)
#define DEEP_SLEEPTIME                  (20)
#define SLEEP_DURATION_MS               (800)
#define SLEEP_TIMER_TICKS_PER_MS        (32)
#define ZCL_TICK_TIME           ZTIMER_TIME_MSEC(100)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum {
    E_REMOTE_STARTUP,
    E_REMOTE_NFN_START,
    E_REMOTE_RUNNING
} teRemoteNode_States;

typedef enum {
        FACTORY_NEW_REMOTE = 0,
        NOT_FACTORY_NEW_REMOTE = 0xff
} teRemoteState;



typedef struct {
    teRemoteState eState;
    teRemoteNode_States eNodeState;
    uint8 u8MyChannel;
    uint16 u16MyAddr;
    uint16 u16FreeAddrLow;
    uint16 u16FreeAddrHigh;
    uint16 u16FreeGroupLow;
    uint16 u16FreeGroupHigh;
}tsZllRemoteState;

/*
 * Set TRUE to allow lights that fail to respond to read attribute requests
 * to be aged out of the lighting database
 */
#define LIGHT_DEVICE_AGING          TRUE
#define LIGHT_DEVICE_AGE_LIMIT      (8)
#define LIGHT_DEVICE_NO_ROUTE       (4)

/* set TRUE to produce debug output to show the generated network key
 */


/* set TRUE to fix the channel the network will operate on
 * set FALSE for release builds
 */
#define FIX_CHANNEL         FALSE


#define aplcMaxParentLossRetryAttempts     10
#define ZLL_MAX_REJOIN_ATTEMPTS            aplcMaxParentLossRetryAttempts


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitialiseNode(void);
PUBLIC void APP_taskController(void);
PUBLIC void APP_cbTimerAddGroup(void *pvParam);
PUBLIC void APP_cbTimerPoll(void *pvParam);
PUBLIC void APP_cbTimerId(void *pvParam);

PUBLIC void vInitCommission();
PUBLIC void vSetGroupAddress(uint16 u16GroupStart, uint8 u8NumGroups);
PUBLIC void APP_vFactoryResetRecords( void);
PUBLIC void vStartPolling(void);
PUBLIC void APP_vStartUpHW(void);
PUBLIC void vSelectLight(void);
PUBLIC void vHandleIdentifyRequest(uint16 u16Duration);


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC tsZllRemoteState sZllState;

extern PUBLIC bool_t bTLinkInProgress;
extern uint16 au16AttribList[];
extern PUBLIC tsZllEndpointInfoTable sEndpointTable;
extern PUBLIC tsZllGroupInfoTable sGroupTable;
extern PUBLIC bool_t bAddrMode;
extern PUBLIC uint8 u8KeepAliveTime;


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_SENSOR_NDOE_H_*/
