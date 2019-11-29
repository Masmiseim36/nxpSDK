/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_SCENES_COMMANDS_H_
#define APP_SCENES_COMMANDS_H_



/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/



/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

#ifdef CLD_SCENES
PUBLIC void vAppRemoveSceneById(uint8 u8SceneId, uint16 u16GroupId);
PUBLIC void vAppRemoveAllScene(uint16 u16GroupId);
PUBLIC void vAppGetSceneMembership(uint16 u16GroupId);
PUBLIC void vAppRecallSceneById(uint8 u8SceneId, uint16 u16GroupId);
PUBLIC void vAppAddScene(uint8 u8SceneId, uint16 u16GroupId, uint16 u16TransitionTime);
PUBLIC void vAppEnhancedAddScene(uint8 u8Scene, uint16 u16GroupId);
PUBLIC void vAppViewScene(uint8 u8SceneId, uint16 u16GroupId);
PUBLIC void vAppEnhancedViewScene(uint8 u8SceneId, uint16 u16GroupId);
PUBLIC void vAppCopyScene(uint8 u8Mode, uint8 u8FromSceneId, uint16 u16FromGroupId, uint8 u8ToSceneId, uint16 u16ToGroupId);
PUBLIC void vAppStoreSceneById(uint8 u8SceneId, uint16 u16GroupId);
#endif


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/



/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_SCENES_COMMANDS_H_*/
