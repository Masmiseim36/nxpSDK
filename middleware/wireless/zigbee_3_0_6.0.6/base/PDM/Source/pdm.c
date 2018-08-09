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
\file       pdm.c
\brief      Shim layer for zps persistent data
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "EmbeddedTypes.h"
#include "NVM_Interface.h"
#include "MemManager.h"
#include "FunctionLib.h"
#include "PDM_IDs.h"
#include "PDM.h"
#include "Panic.h"

#include "app_zps_cfg.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "zps_apl_zdo.h"
#include "zps_apl_zdp.h"
#include "zps_apl_aib.h"
#include "zps_apl_aps.h"

extern void InitAplRecords(void);
extern void InitNwkRecords(void);
extern void ZPS_vApl_GetRecordInfo(uint8 u8RecordId, uint16 *pu16RecordSize, void **pvRecordAddr);
extern void ZPS_vNwk_GetRecordInfo(uint8 u8RecordId, uint16 *pu16RecordSize, void **pvRecordAddr);
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef PDM_APP_ID
#error "An application specific dataset id PDM_APP_ID should be defined" 
#endif
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

static uint16 u16AppPDMId;

/* Register application pdm/nvm dataset */

NVM_RegisterDataSet(&u16AppPDMId, 1, sizeof(uint16), PDM_APP_ID, gNVM_MirroredInRam_c);

/* This data set contains application variables to be preserved across resets */
NVM_RegisterDataSet(NULL, 1, (uint32)(&((ZPS_tsAplAib*)(0))->u8ApsNonMemberRadius), PDM_ID_ZPSAPL_AIB, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(ZPS_tsAplApsmeBindingTableEntry), PDM_ID_ZPSAPL_AIB_BTBL, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(ZPS_tsAPdmGroupTableEntry), PDM_ID_ZPSAPL_AIB_GTBL, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(ZPS_tsAplApsKeyDescriptorEntry), PDM_ID_ZPSAPL_AIB_LKTBL, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(ZPS_tsAplApsKeyDescriptorEntry), PDM_ID_ZPSAPL_TCDEVTBL , gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(ZPS_tsNWkNibPersist), PDM_ID_ZPSNWK_NIB, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(ZPS_tsNwkActvNtEntry), PDM_ID_ZPSNWK_NT, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(uint16), PDM_ID_ZPSNWK_ADMAP16, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(uint16), PDM_ID_ZPSNWK_ADMAPLKUP16, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(uint64), PDM_ID_ZPSNWK_ADMAP64, gNVM_MirroredInRam_c);

NVM_RegisterDataSet(NULL, 1, sizeof(ZPS_tsNwkSecMaterialSet), PDM_ID_ZPSNWK_SECMAT, gNVM_MirroredInRam_c);
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: PDM_eReadDataFromRecord
 *
 * DESCRIPTION:
 * Restore PDM data from nvm
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC PDM_teStatus PDM_eReadDataFromRecord(
        uint16                      u16IdValue,
        void                       *pvDataBuffer,
        uint16                      u16DataBufferLength,
        uint16                     *pu16DataBytesRead)
{
    NVM_Status_t nvmStatus;
    PDM_teStatus pdmStatus = PDM_E_STATUS_OK;
    (void)u16IdValue;
    (void)u16DataBufferLength;
    (void)pu16DataBytesRead;
    
    nvmStatus = NvRestoreDataSet(pvDataBuffer, TRUE);
    if(nvmStatus != gNVM_OK_c)
    {
       pdmStatus =  PDM_E_STATUS_NOT_SAVED;
    }
    
    return pdmStatus;
}
/****************************************************************************
 *
 * NAME: PDM_eSaveRecordData
 *
 * DESCRIPTION:
 * Save PDM dataset
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC PDM_teStatus PDM_eSaveRecordData(
        uint16                      u16IdValue,
        void                       *pvDataBuffer,
        uint16                      u16Datalength
#ifdef PDM_ENCRYPTION_ENABLED
        ,
        bool_t                      bSecure
#endif
    )
{
    (void)u16IdValue;
    (void)u16Datalength;
    #ifdef PDM_ENCRYPTION_ENABLED
    (void)bSecure;
    #endif
    NvSaveOnIdle(pvDataBuffer, TRUE);
    return 0;
}

/****************************************************************************
 *
 * NAME: PDM_vCompletePendingOperations
 *
 * DESCRIPTION:
 * The function attemps to complete all the NVM related pending
 * operations
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC void PDM_vCompletePendingOperations(void)
{
    NvCompletePendingOperations();
}
/****************************************************************************
 *
 * NAME: PDM_eInitialise
 *
 * DESCRIPTION:
 * Init PDM module 
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC PDM_teStatus PDM_eInitialise(
    uint16          u16StartSegment,
    uint8            u8NumberOfSegments
#if (!defined __JN518X__) && (!defined CPU_MKW41Z512VHT4)
#ifndef PDM_NO_RTOS
        ,
    OS_thMutex      hPdmMutex
#endif
#endif
#ifdef PDM_ENCRYPTION_ENABLED
        ,
    tsReg128       *psNVMdataEncryptionKey
#endif
    )
{
    
    NVM_Status_t nvmstatus;
    
    /* Not used */
    (void)u16StartSegment;
    (void)u8NumberOfSegments;
    
    InitAplRecords();
    InitNwkRecords();

    if(gNVM_OK_c != NvModuleInit())
    {
       panic(0,(uint32_t)NvModuleInit,0,0);
    }

    nvmstatus = NvRestoreDataSet(&u16AppPDMId, TRUE);

    /* check PDM App ID */
    if((u16AppPDMId != PDM_APP_ID) && (nvmstatus != gNVM_PageIsEmpty_c))
    {
        NvFormat();
    }
    
    if(u16AppPDMId != PDM_APP_ID)
    {
        u16AppPDMId = PDM_APP_ID;
        if(gNVM_OK_c != NvSyncSave(&u16AppPDMId, TRUE))
        {
            panic(0,(uint32_t)NvSyncSave,0,0);
        }
    }
    
    return 0;
}
/****************************************************************************
 *
 * NAME: PDM_vDeleteAllDataRecords
 *
 * DESCRIPTION:
 * Erase PDM
 *
 * RETURNS:
 * Never returns.
 *
 ****************************************************************************/
PUBLIC void PDM_vDeleteAllDataRecords(void)
{
    NvFormat();
}

/****************************************************************************
 *
 * NAME: PDM_GetNVMTableEntry
 *
 * DESCRIPTION:
 * Search nvm table and return nvm entry
 *
 * RETURNS:
 * 
 *
 ****************************************************************************/
void PDM_GetNVMTableEntry(uint16 u16RecordId, NVM_DataEntry_t** ppNvmDataEntry)
{
    uint8 i = 0;
    while (gNVM_TABLE_entries_c != i)
    {
        if(pNVM_DataTable[i].DataEntryID == u16RecordId)
        {
          *ppNvmDataEntry = &pNVM_DataTable[i];
          break;
        }
        i++;
    }
}

/****************************************************************************
 *
 * NAME: InitAplRecords
 *
 * DESCRIPTION:
 * Init APL Records
 *
 * RETURNS:
 * 
 *
 ****************************************************************************/
void InitAplRecords(void)
{
    
    NVM_DataEntry_t* pNvmDataEntry;
    
    /* setup AIB entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSAPL_AIB, &pNvmDataEntry);
    ZPS_vApl_GetRecordInfo(PDM_ID_ZPSAPL_AIB-PDM_ID_ZPSAPL_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /* Setup Binding Table entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSAPL_AIB_BTBL, &pNvmDataEntry);
    ZPS_vApl_GetRecordInfo(PDM_ID_ZPSAPL_AIB_BTBL-PDM_ID_ZPSAPL_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /*setup group table entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSAPL_AIB_GTBL, &pNvmDataEntry);
    ZPS_vApl_GetRecordInfo(PDM_ID_ZPSAPL_AIB_GTBL-PDM_ID_ZPSAPL_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /* setup link table entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSAPL_AIB_LKTBL, &pNvmDataEntry);
    ZPS_vApl_GetRecordInfo(PDM_ID_ZPSAPL_AIB_LKTBL-PDM_ID_ZPSAPL_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /* setup TC table entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSAPL_TCDEVTBL, &pNvmDataEntry);
    ZPS_vApl_GetRecordInfo(PDM_ID_ZPSAPL_TCDEVTBL-PDM_ID_ZPSAPL_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);
    
}
/****************************************************************************
 *
 * NAME: InitNwkRecords
 *
 * DESCRIPTION:
 * Init NWK records
 *
 * RETURNS:
 * 
 *
 ****************************************************************************/
void InitNwkRecords(void)
{
    NVM_DataEntry_t* pNvmDataEntry;
    /* setup NIB entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSNWK_NIB, &pNvmDataEntry);
    ZPS_vNwk_GetRecordInfo(PDM_ID_ZPSNWK_NIB-PDM_ID_ZPSNWK_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /* setup NT entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSNWK_NT, &pNvmDataEntry);
    ZPS_vNwk_GetRecordInfo(PDM_ID_ZPSNWK_NT-PDM_ID_ZPSNWK_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /* setup ADMAP16 entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSNWK_ADMAP16, &pNvmDataEntry);
    ZPS_vNwk_GetRecordInfo(PDM_ID_ZPSNWK_ADMAP16-PDM_ID_ZPSNWK_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /* setup ADMAPLKUP16 entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSNWK_ADMAP64, &pNvmDataEntry);
    ZPS_vNwk_GetRecordInfo(PDM_ID_ZPSNWK_ADMAP64-PDM_ID_ZPSNWK_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /* setup ADMAP64 entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSNWK_ADMAPLKUP16, &pNvmDataEntry);
    ZPS_vNwk_GetRecordInfo(PDM_ID_ZPSNWK_ADMAPLKUP16-PDM_ID_ZPSNWK_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);

    /* setup SECMAT entry */
    PDM_GetNVMTableEntry(PDM_ID_ZPSNWK_SECMAT, &pNvmDataEntry);
    ZPS_vNwk_GetRecordInfo(PDM_ID_ZPSNWK_SECMAT-PDM_ID_ZPSNWK_BASE, &pNvmDataEntry->ElementSize, &pNvmDataEntry->pData);
    
}