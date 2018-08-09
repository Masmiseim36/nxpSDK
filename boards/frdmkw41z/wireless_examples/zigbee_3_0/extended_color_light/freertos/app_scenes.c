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

/*!=============================================================================
\file       app_scenes.c
\brief      Application Scenes save and load functionality
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <string.h>
#include "app_pdum_cfg.h"
#include "PDM.h"
#include "dbg.h"
#include "app_zcl_cfg.h"
#include "PDM_IDs.h"
#include "NVM_Interface.h"
#include "zcl_options.h"
#include "zps_apl_af.h"

#include "app_common.h"

#include "Scenes.h"
#include "app_scenes.h"
#ifdef CLD_GROUPS
#include "Groups_internal.h"
#endif
#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define SCENES_SEARCH_GROUP_ID      (1 << 0)
#define SCENES_SEARCH_SCENE_ID      (1 << 1)


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct
{
    uint8   u8SearchOptions;
    uint16  u16GroupId;
    uint8   u8SceneId;
} tsSearchParameter;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
#if (defined CLD_SCENES) && (defined SCENES_SERVER)
PRIVATE tsAPP_ScenesCustomData sScenesCustomData;
NVM_RegisterDataSet(&sScenesCustomData, 1, sizeof(tsAPP_ScenesCustomData), PDM_ID_APP_SCENES_DATA, gNVM_MirroredInRam_c);
#endif


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

#if (defined CLD_SCENES) && (defined SCENES_SERVER)
/****************************************************************************
*
* NAME: vSaveScenesNVM
*
* DESCRIPTION:
* To save scenes data to EEPROM
*
* RETURNS:
* void
*
****************************************************************************/
PUBLIC void vSaveScenesNVM(void)
{
    uint8 i=0, j=0;
    tsCLD_ScenesTableEntry *psTableEntry;

    /* Before saving data again clear off the earlier entries and recopy all the scenes */
    FLib_MemSet(&sScenesCustomData,0,sizeof(tsAPP_ScenesCustomData));
    psTableEntry = (tsCLD_ScenesTableEntry*)psDLISTgetHead(&sLight.sScenesServerCustomDataStructure.lScenesAllocList);
    for(i = 0; i < iDLISTnumberOfNodes(&sLight.sScenesServerCustomDataStructure.lScenesAllocList); i++)
    {
        if(psTableEntry != NULL)
        {
             sScenesCustomData.asScenesCustomTableEntry[j].bIsSceneValid = TRUE;
              sScenesCustomData.asScenesCustomTableEntry[j].u16GroupId = psTableEntry->u16GroupId;
              sScenesCustomData.asScenesCustomTableEntry[j].u8SceneId = psTableEntry->u8SceneId;
             sScenesCustomData.asScenesCustomTableEntry[j].u16TransitionTime = psTableEntry->u16TransitionTime;
             sScenesCustomData.asScenesCustomTableEntry[j].u16SceneDataLength = psTableEntry->u16SceneDataLength;
             FLib_MemCpy(sScenesCustomData.asScenesCustomTableEntry[j].au8SceneData,psTableEntry->au8SceneData,psTableEntry->u16SceneDataLength);
              #ifdef CLD_SCENES_TABLE_SUPPORT_TRANSITION_TIME_IN_MS
                     sScenesCustomData.asScenesCustomTableEntry[j].u8TransitionTime100ms = psTableEntry->u8TransitionTime100ms;
              #endif
              j++;
        }
        psTableEntry = (tsCLD_ScenesTableEntry*)psDLISTgetNext((DNODE*)psTableEntry);
    }

    PDM_eSaveRecordData(PDM_ID_APP_SCENES_DATA,&sScenesCustomData,sizeof(tsAPP_ScenesCustomData));
}

/****************************************************************************
*
* NAME: vLoadScenesNVM
*
* DESCRIPTION:
* To load scenes data from EEPROM
*
* RETURNS:
* void
*
****************************************************************************/
PUBLIC void vLoadScenesNVM(void)
{
    uint8 i=0;
    uint16 u16ByteRead;

    PDM_eReadDataFromRecord(PDM_ID_APP_SCENES_DATA,
                            &sScenesCustomData,
                            sizeof(tsAPP_ScenesCustomData), &u16ByteRead);

    /* initialise lists */
    vDLISTinitialise(&sLight.sScenesServerCustomDataStructure.lScenesAllocList);
    vDLISTinitialise(&sLight.sScenesServerCustomDataStructure.lScenesDeAllocList);

    for(i=0; i<CLD_SCENES_MAX_NUMBER_OF_SCENES; i++)
    {
        /* Rebuild the scene list to avoid scene loss after re-flashing */
        if(sScenesCustomData.asScenesCustomTableEntry[i].bIsSceneValid == TRUE)
        {
             vDLISTaddToTail(&sLight.sScenesServerCustomDataStructure.lScenesAllocList,
                            (DNODE *)&sLight.sScenesServerCustomDataStructure.asScenesTableEntry[i]);
        }
        else
        {
             vDLISTaddToTail(&sLight.sScenesServerCustomDataStructure.lScenesDeAllocList,
                            (DNODE *)&sLight.sScenesServerCustomDataStructure.asScenesTableEntry[i]);
        }

        sLight.sScenesServerCustomDataStructure.asScenesTableEntry[i].u16GroupId = sScenesCustomData.asScenesCustomTableEntry[i].u16GroupId;
        sLight.sScenesServerCustomDataStructure.asScenesTableEntry[i].u8SceneId = sScenesCustomData.asScenesCustomTableEntry[i].u8SceneId;
        sLight.sScenesServerCustomDataStructure.asScenesTableEntry[i].u16TransitionTime = sScenesCustomData.asScenesCustomTableEntry[i].u16TransitionTime;
        sLight.sScenesServerCustomDataStructure.asScenesTableEntry[i].u16SceneDataLength = sScenesCustomData.asScenesCustomTableEntry[i].u16SceneDataLength;
        FLib_MemCpy(sLight.sScenesServerCustomDataStructure.asScenesTableEntry[i].au8SceneData, sScenesCustomData.asScenesCustomTableEntry[i].au8SceneData,sScenesCustomData.asScenesCustomTableEntry[i].u16SceneDataLength);
#ifdef CLD_SCENES_TABLE_SUPPORT_TRANSITION_TIME_IN_MS
        sLight.sScenesServerCustomDataStructure.asScenesTableEntry[i].u8TransitionTime100ms = sScenesCustomData.asScenesCustomTableEntry[i].u8TransitionTime100ms;
#endif
    }
}
#endif


#ifdef CLD_GROUPS
/****************************************************************************
 *
 * NAME: APP_vRemoveAllGroupsAndScenes
 *
 * DESCRIPTION:
 * to remove all scenes and groups after a leave or factory reset
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vRemoveAllGroupsAndScenes(void)
{
    eCLD_GroupsRemoveAllGroups(&sLight.sEndPoint,
                               &sLight.sClusterInstance.sGroupsServer,
                               (uint64)0xffffffffffffffffLL);

}
#endif

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
