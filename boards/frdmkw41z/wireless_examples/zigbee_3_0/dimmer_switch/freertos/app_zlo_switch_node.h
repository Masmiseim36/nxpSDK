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
