/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_SENSOR_NODE_H_
#define APP_SENSOR_NODE_H_

 /*!
\file       app_zlo_switch_node.h
\brief      ZLO Controller & Switch Demo : Stack <-> Remote Control App Interaction
            (Interface)
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "OnOff.h"
#include "LevelControl.h"
//#include "MiniMac_Pib.h"

#include "zps_nwk_sap.h"
#include "zps_nwk_nib.h"
#include "zps_nwk_pub.h"
#include "zps_apl.h"
#include "zps_apl_zdo.h"
#include "app_common.h"
#include "app_events.h"
#include "zcl_customcommand.h"
#ifdef PDM_EEPROM
 #include "app_pdm.h"
#endif
#ifdef CLD_OTA
#include "OTA.h"
#endif
#include "bdb_api.h"
#include "bdb_fb_api.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEEP_SLEEP_ENABLE
#ifdef CLD_OTA
    #define DEEP_SLEEP_TIME         10 /* drop in the deep sleep after 10*12 secs + last iteration = 2 minutes to check for ota server */
#else
    #define DEEP_SLEEP_TIME         5 /* drop in the deep sleep after 5*12 secs + last iteration = 1 minutes */
#endif
#endif

#define KEEP_ALIVETIME (3)

#define APP_IDENTIFY_TIME_IN_SECS   5
#define NUMBER_DEVICE_TO_BE_DISCOVERED 8 // Should be same as Binding table size
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct{
    uint16 u16Address;
    uint8  u8Ep;
}tsLightInfo;

typedef struct{
    uint8       u8Discovered;
    uint8       u8Index;
    tsLightInfo sLightInfo[NUMBER_DEVICE_TO_BE_DISCOVERED];
}tsDeviceInfo;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitialiseNode(void);
PUBLIC void APP_taskSwitch(void);
PUBLIC bool bLightsDiscovered(void);
PUBLIC void vStartFastPolling(uint8 u8Seconds);

PUBLIC void vAppOnOff(teCLD_OnOff_Command eCmd);
PUBLIC void vAppLevelStepMove(teCLD_LevelControl_MoveMode eMode);
PUBLIC void vAppLevelMove(teCLD_LevelControl_MoveMode eMode, uint8 u8Rate, bool_t bWithOnOff);
PUBLIC void vAppLevelStop(void);

PUBLIC void vAppIdentify(uint16 u16Time);

PUBLIC void vAppAddGroup(uint16 u16GroupId, bool_t bBroadcast);
PUBLIC void vAppRemoveGroup(uint16 u16GroupId, bool_t bBroadcast);
PUBLIC void vAppRemoveAllGroups(bool_t bBroadcast);

#ifdef SLEEP_ENABLE
    PUBLIC void vReloadSleepTimers(void);
    PUBLIC void vUpdateKeepAliveTimer(void);
    PUBLIC bool bWaitingToSleep(void);
    PUBLIC void vWakeCallBack(void);
    #ifdef DEEP_SLEEP_ENABLE
        PUBLIC void vLoadDeepSleepTimer(uint8 u8SleepTime);
        PUBLIC bool bGoingDeepSleep(void);
    #endif
#endif

#ifdef CLD_SCENES
PUBLIC void vAppRecallSceneById(uint8 u8SceneId, uint16 u16GroupId);
PUBLIC void vAppEnhancedAddScene(uint8 u8Scene, uint16 u16GroupId);
PUBLIC void vAppEnhancedViewScene(uint8 u8SceneId, uint16 u16GroupId);
PUBLIC void vAppStoreSceneById(uint8 u8SceneId, uint16 u16GroupId);
#endif

PUBLIC void vSetIndividualLightInformation(void);
PUBLIC void vSelectLight(void);

#ifdef CLD_OTA
    PUBLIC teNODE_STATES eGetNodeState(void);
    PUBLIC tsOTA_PersistedData sGetOTACallBackPersistdata(void);
#endif

PUBLIC void APP_cbTimerPoll(void *pvParam);
PUBLIC void APP_vFactoryResetRecords( void);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsDeviceDesc sDeviceDesc;
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_SENSOR_NDOE_H_*/
