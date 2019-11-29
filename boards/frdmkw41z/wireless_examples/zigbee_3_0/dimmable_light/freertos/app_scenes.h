/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_SCENES_H_
#define APP_SCENES_H_

/*!
\file       app_scenes.h
\brief      Application Scenes save and load functionality -Interface
*/
     
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "zcl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#if (defined CLD_SCENES) && (defined SCENES_SERVER)
typedef struct
{
    bool_t  bIsSceneValid;
    uint16  u16GroupId;
    uint8   u8SceneId;
    uint16  u16TransitionTime;
    uint16  u16SceneDataLength;
    uint8   au8SceneData[CLD_SCENES_MAX_SCENE_STORAGE_BYTES];
    uint8 u8TransitionTime100ms;
} tsAPP_ScenesCustomTableEntry;

/* Scenes data structure for PDM saving */
typedef struct
{
    tsAPP_ScenesCustomTableEntry  asScenesCustomTableEntry[CLD_SCENES_MAX_NUMBER_OF_SCENES];
} tsAPP_ScenesCustomData;
#endif

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
#if (defined CLD_SCENES) && (defined SCENES_SERVER)
PUBLIC void vLoadScenesNVM(void);
PUBLIC void vSaveScenesNVM(void);
#endif

#ifdef CLD_GROUPS
PUBLIC void APP_vRemoveAllGroupsAndScenes(void);
#endif

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif //APP_SCENES_H_
