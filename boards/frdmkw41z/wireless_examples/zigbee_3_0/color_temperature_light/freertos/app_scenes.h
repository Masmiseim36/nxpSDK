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
