/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef  PDMIDS_H_INCLUDED
#define  PDMIDS_H_INCLUDED


/*!
\file       PDM_IDs.h
\brief      Persistent Data Manager ID definitions
*/

#if defined __cplusplus
extern "C" {
#endif


/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
    
/*  application ids start from 0Xa100 */
#define PDM_ID_APP_TL_ROUTER                 0xA100
#define PDM_ID_APP_SCENES_DATA               0xA101
#define PDM_ID_OTA_DATA                      0xA102
#define PDM_ID_APP_CLD_GP_TRANS_TABLE        0xA103
#define PDM_ID_APP_CLD_GP_SINK_PROXY_TABLE   0xA104
#define PDM_ID_APP_REPORTS                   0xA105
#define PDM_ID_POWER_ON_COUNTER              0xA106
#define PDM_ID_ZCL_ATTRIB                    0xA107
#define PDM_ID_APP_NTAG_NWK                  0xA108

#define PDM_ID_APP_COORD                     0xA109
#define PDM_ID_APP_ROUTER                    0xA10A
#define PDM_ID_APP_END_DEVICE                0xA10B
    
#define PDM_ID_APP_NFC_NWK_NCI               0xA10C
#define PDM_ID_APP_NFC_ICODE                 0xA10D

#define PDM_ID_APP_ZLL_CMSSION               0xA10E
#define PDM_ID_APP_END_P_TABLE               0xA10F
#define PDM_ID_APP_GROUP_TABLE               0xA110    

#define PDM_APP_FSCIBB_ID                    0xA120
#define PDM_APP_BDB_ZR_ID                    0xA121    
#define PDM_APP_LIGHT_ID                     0xA122
#define PDM_APP_BDB_ZED_ID                   0xA123    
#define PDM_APP_BDB_ZC_ID                    0xA124
#define PDM_APP_BLE_CONTROLBRIDGE_ID         0xA125
#define PDM_APP_CONTROLBRIDGE_ID             0xA126
#define PDM_APP_AT_ID                        0xA127
#define PDM_APP_AT_BDB_ZC_ID                 0xA128
#define PDM_APP_AT_BDB_ZED_ID                0xA129
#define PDM_APP_AT_BDB_ZR_ID                 0xA12A
#define PDM_APP_AT_ZTT_ID                    0xA12B
#define PDM_APP_AT_ZLO_ZCR_ID                0xA12C
#define PDM_APP_AT_ZLO_ZED_ID                0xA12D
#define PDM_ID_APP_ZLO_SWITCH                0xA12E    

#ifndef PDM_ID_ZPSAPL_BASE
#define PDM_ID_ZPSAPL_BASE                   0xf000
#endif
#ifndef PDM_ID_ZPSNWK_BASE
#define PDM_ID_ZPSNWK_BASE                   0xf100 
#endif

#define PDM_ID_ZPSAPL_AIB                    0xf000
#define PDM_ID_ZPSAPL_AIB_BTBL               0xf001
#define PDM_ID_ZPSAPL_AIB_GTBL               0xf002
#define PDM_ID_ZPSAPL_AIB_LKTBL              0xf003
#define PDM_ID_ZPSAPL_TCDEVTBL               0xf004
#define PDM_ID_ZPSAPL_RECORD_COUNT           0xf005


#define PDM_ID_ZPSNWK_NIB                    0xf100
#define PDM_ID_ZPSNWK_NT                     0xf101
#define PDM_ID_ZPSNWK_ADMAP16                0xf102
#define PDM_ID_ZPSNWK_ADMAP64                0xf103
#define PDM_ID_ZPSNWK_ADMAPLKUP16            0xf104
#define PDM_ID_ZPSNWK_SECMAT                 0xf105
#define PDM_ID_ZPSNWK_RECORD_COUNT           0xf106

#define PDM_ID_APP_ZLO_CONTROLLER            0xf201
#define PDM_ID_APP_ZR                        0xf204
#define PDM_ID_APP_ZED                       0xf205
#define PDM_ID_APP_SENSOR                    0xf206
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /* PDMIDS_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
