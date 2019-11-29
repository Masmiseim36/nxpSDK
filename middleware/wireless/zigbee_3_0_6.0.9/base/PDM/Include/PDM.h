/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef PDM_H_INCLUDED
#define PDM_H_INCLUDED

/*!
\file       PDM.h
\brief      Provide management of data which needs to persist over
            cold or warm start
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#if defined __cplusplus
extern "C" {
#endif
#include "dbg.h"

#ifndef PDM_NO_RTOS
#define PDM_NO_RTOS
#endif
    
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* JN514x always uses external flash */
#ifdef JENNIC_CHIP_FAMILY_JN514x
#define PDM_EXTERNAL_FLASH
#endif
#ifdef SE_HOST_COPROCESSOR
#define PDM_EXTERNAL_FLASH
#endif
#define PDM_NUM_BLOCKS     128
#ifdef PDM_EXTERNAL_FLASH
#define PDM_NAME_SIZE      16
#else
#define PDM_NAME_SIZE      7
#ifndef PDM_USER_SUPPLIED_ID
#define PDM_USER_SUPPLIED_ID /* Defaulting to having this enabled */
#endif
#endif
#define PDM_INVALID_ID     ((uint16)(-1))

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
    PDM_E_STATUS_OK,
    PDM_E_STATUS_INVLD_PARAM,
    // NVM based PDM codes
    PDM_E_STATUS_PDM_FULL,
    PDM_E_STATUS_NOT_SAVED,
    PDM_E_STATUS_RECOVERED,
    PDM_E_STATUS_PDM_RECOVERED_NOT_SAVED,
    PDM_E_STATUS_USER_BUFFER_SIZE,
    PDM_E_STATUS_BITMAP_SATURATED_NO_INCREMENT,
    PDM_E_STATUS_BITMAP_SATURATED_OK,
    PDM_E_STATUS_IMAGE_BITMAP_COMPLETE,
    PDM_E_STATUS_IMAGE_BITMAP_INCOMPLETE,
    PDM_E_STATUS_INTERNAL_ERROR
} PDM_teStatus;

typedef enum
{
    PDM_RECOVERY_STATE_NONE=0,
    PDM_RECOVERY_STATE_NEW, //1
    PDM_RECOVERY_STATE_RECOVERED, //2
    PDM_RECOVERY_STATE_RECOVERED_NOT_READ, //3
    PDM_RECOVERY_STATE_SAVED, //4
    PDM_RECOVERY_STATE_NOT_SAVED, //5
    PDM_RECOVERY_STATE_APPENDED, //6
    // do not move
    PDM_RECOVERY_STATE_NUMBER
} PDM_teRecoveryState;

typedef struct
{
    /* this function gets called after a cold or warm start */
    void (*prInitHwCb)(void);

    /* this function gets called to erase the given sector */
    void (*prEraseCb) (uint8 u8Sector);

    /* this function gets called to write data to an addresss
     * within a given sector. address zero is the start of the
     * given sector */
    void (*prWriteCb) (uint8 u8Sector,
#ifdef SE_HOST_COPROCESSOR
                       uint32 u32Addr,
                       uint32 u32Len,
#else
                       uint16 u16Addr,
                       uint16 u16Len,
#endif
                       uint8 *pu8Data);

    /* this function gets called to read data from an addresss
     * within a given sector. address zero is the start of the
     * given sector */
    void (*prReadCb)  (uint8 u8Sector,
#ifdef SE_HOST_COPROCESSOR
                       uint32 u32Addr,
                       uint32 u32Len,
#else
                       uint16 u16Addr,
                       uint16 u16Len,
#endif
                       uint8 *pu8Data);
} PDM_tsHwFncTable;

typedef enum
{
    E_PDM_SYSTEM_EVENT_WEAR_COUNT_TRIGGER_VALUE_REACHED=0,
    E_PDM_SYSTEM_EVENT_WEAR_COUNT_MAXIMUM_REACHED,
    E_PDM_SYSTEM_EVENT_SAVE_FAILED,
    E_PDM_SYSTEM_EVENT_NOT_ENOUGH_SPACE,
    E_PDM_SYSTEM_EVENT_LARGEST_RECORD_FULL_SAVE_NO_LONGER_POSSIBLE,
    E_PDM_SYSTEM_EVENT_SEGMENT_DATA_CHECKSUM_FAIL,
    // nerdy event codes
    E_PDM_SYSTEM_EVENT_NVM_SEGMENT_HEADER_REPAIRED,
    E_PDM_SYSTEM_EVENT_SYSTEM_INTERNAL_BUFFER_WEAR_COUNT_SWAP,
    E_PDM_SYSTEM_EVENT_SYSTEM_DUPLICATE_FILE_SEGMENT_DETECTED,
    E_PDM_SYSTEM_EVENT_SYSTEM_ERROR,
    // used in test harness
    E_PDM_SYSTEM_EVENT_SEGMENT_PREWRITE,
    E_PDM_SYSTEM_EVENT_SEGMENT_POSTWRITE,
    E_PDM_SYSTEM_EVENT_SEQUENCE_DUPLICATE_DETECTED,
    E_PDM_SYSTEM_EVENT_SEQUENCE_VERIFY_FAIL
} PDM_eSystemEventCode;

typedef struct
{
    uint32                          u32eventNumber;
    PDM_eSystemEventCode            eSystemEventCode;
} PDM_tfpSystemEventCallback;

typedef void (*PDM_tpfvSystemEventCallback) (
    uint32                          u32eventNumber,
    PDM_eSystemEventCode            eSystemEventCode);

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/* initialise the module - call this EVERY cold start and EVERY warm start */

PUBLIC void PDM_vInit(
        uint8                       u8StartSector,
        uint8                       u8NumSectors,
        uint32                      u32SectorSize,
#if (!defined __JN518X__)
#ifndef PDM_NO_RTOS
        OS_thMutex                  hPdmMutex,
        OS_thMutex                  hPdmMediaMutex,
#endif
#else
        void*                       hPdmMutex,
        void*                       hPdmMediaMutex,
#endif
        PDM_tsHwFncTable           *psHwFuncTable
#ifdef PDM_ENCRYPTION_ENABLED
        ,
        const tsReg128             *psKey
#endif
        );

#if defined(PDM_BITMAP_FUNCTIONALITY_ENABLED)
PUBLIC PDM_teStatus PDM_eCreateBitmap(
        uint16                      u16IdValue,
        uint32                      u32InitialValue
        );

PUBLIC PDM_teStatus PDM_eDeleteBitmap(
        uint16                      u16IdValue
        );

PUBLIC PDM_teStatus PDM_eGetBitmap(
        uint16                      u16IdValue,
        uint32                     *pu32InitialValue,
        uint32                     *pu32BitmapValue
        );

PUBLIC PDM_teStatus PDM_eIncrementBitmap(
        uint16                      u16IdValue
        );

PUBLIC PDM_teStatus PDM_eCreateImageBitmap(
        uint16                      u16IdValue,
        uint8                      *pu8RAMImageBitmap,
        uint16                      u16ImageBitmapSize);

PUBLIC PDM_teStatus PDM_eGetImageBitmap(
        uint16                      u16IdValue,
        uint8                      *pu8RAMImageBitmap,
        uint16                      u16ImageBitmapSize);

PUBLIC PDM_teStatus PDM_eSetImageBitmapBits(
        uint16                      u16IdValue,
        uint16                      u16FirstBit,
        uint16                      u16NumberOfBits,
        uint8                      *pu8RAMImageBitmap,
        uint16                      u16ImageBitmapSize);

PUBLIC PDM_teStatus PDM_eSaveImageBitmap(
        uint16                      u16IdValue,
        uint8                      *pu8RAMImageBitmap,
        uint16                      u16ImageBitmapSize,
        bool_t                      bSaveCompleteBytesOnly);

PUBLIC PDM_teStatus PDM_eIsImageBitmapComplete(
        uint16                      u16IdValue,
        uint8                      *pu8RAMImageBitmap,
        uint16                      u16ImageBitmapSize,
        bool_t                      bRAMImageBitmap);

PUBLIC PDM_teStatus PDM_eGetFirstUnusedSlotInBitmap(
        uint16                      u16IdValue,
        uint8                      *pu8RAMImageBitmap,
        uint16                      u16ImageBitmapSize,
        bool_t                      bRAMImageBitmap,
        uint16                     *pu16FirstUnusedSlotInBitmap);

PUBLIC PDM_teStatus PDM_eDeleteImageBitmap(
        uint16                      u16IdValue);

PUBLIC PDM_teStatus ePDM_ReadPartialDataFromTable(
        uint16                      u16IdValue,
        uint16                      u16TableOffset,
        void                       *pvDataBuffer,
        uint16                      u16DataBufferLength,
        uint16                     *pu16DataBytesRead
        );
#endif

PUBLIC PDM_teStatus PDM_eInitialise(
    uint16          u16StartSegment,
    uint8           u8NumberOfSegments
#if (!defined __JN518X__)
#ifndef PDM_NO_RTOS
        ,
    OS_thMutex      hPdmMutex
#endif
#endif
#ifdef PDM_ENCRYPTION_ENABLED
        ,
    tsReg128       *psNVMdataEncryptionKey
#endif
    );

PUBLIC PDM_teStatus PDM_eSaveRecordData(
        uint16                      u16IdValue,
        void                       *pvDataBuffer,
        uint16                      u16Datalength
#ifdef PDM_ENCRYPTION_ENABLED
        ,
        bool_t                      bSecure
#endif
    );

PUBLIC PDM_teStatus PDM_eAppendDataToExistingRecord(
        uint16                      u16IdValue,
        void                       *pvData,
        uint16                      u16DataSize);

PUBLIC PDM_teStatus PDM_eReadPartialDataFromExistingRecord(
        uint16                      u16IdValue,
        uint16                      u16TableOffset,
        void                       *pvDataBuffer,
        uint16                      u16DataBufferLength,
        uint16                     *pu16DataBytesRead);

PUBLIC PDM_teStatus PDM_eOverWriteExistingDataInRecord(
        uint16                      u16IdValue,
        uint16                      u16TableOffset,
        void                       *pvDataBuffer,
        uint16                      u16DataBufferLength);

PUBLIC PDM_teStatus PDM_eReadDataFromRecord(
        uint16                      u16IdValue,
        void                       *pvDataBuffer,
        uint16                      u16DataBufferLength,
        uint16                     *pu16DataBytesRead);

PUBLIC void PDM_vDeleteDataRecord(
        uint16                      u16IdValue);

PUBLIC void PDM_vDeleteAllDataRecords(void);

PUBLIC void PDM_vCompletePendingOperations(void);

PUBLIC bool_t PDM_bDoesDataExist(
        uint16                      u16IdValue,
        uint16                     *pu16DataLength);

PUBLIC uint8 PDM_u8GetSegmentCapacity(void);

PUBLIC uint8 PDM_u8GetSegmentOccupancy(void);

PUBLIC void PDM_vRegisterSystemCallback(
        PDM_tpfvSystemEventCallback fpvPDM_SystemEventCallback);

PUBLIC PDM_tpfvSystemEventCallback  PDM_pfGetSystemCallback(void);

PUBLIC void PDM_vSetWearCountTriggerLevel(
        uint32                      u32WearCountTriggerLevel);

PUBLIC PDM_teStatus PDM_eGetSegmentWearCount(
        uint8                       u8SegmentIndex,
        uint32                     *pu32WearCount);

PUBLIC PDM_teStatus PDM_eGetDeviceWearCountProfile(
        uint32      au32WearCount[],
        uint8       u8NumberOfSegments);

#if defined UART_DEBUG
PUBLIC void vPDM_DisplayDataInFileSystem(void);

PUBLIC int iPDM_DisplayDataWithIdInFileSystem(
        uint16                      u16IdValue);

PUBLIC void vPDM_DisplayDataInNVM(void);

PUBLIC int iPDM_DisplayNVMsegmentData(
        uint8                       u8SegmentIndex);

PUBLIC char * psPDM_PrintEventID(
        PDM_eSystemEventCode        eSystemEventCode);

PUBLIC int iPDM_ReadRawNVMsegmentDataToBuffer(
        uint8                       u8SegmentIndex,
        uint8                      *pu8SegmentDataBuffer,
        uint16                     *pu16SegmentDataSize);

#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

extern PUBLIC const uint32 PDM_g_u32Version;

#if defined __cplusplus
};
#endif

#endif /*PDM_H_INCLUDED*/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
