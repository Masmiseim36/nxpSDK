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
