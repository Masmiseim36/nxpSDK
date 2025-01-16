
/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2021-2024 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the header file for the OTA Internal Flash adaption.
 * It is to remain a private header - not a public interface.
 *
 ** SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#ifndef __OTA_PRIVATE_H__
#define __OTA_PRIVATE_H__

#include "fsl_component_messaging.h"
#include "fsl_os_abstraction.h"
#include "OtaSupport.h"
#include "fwk_platform_ota.h"

/*! @brief Hal flash status. */
typedef enum _ota_flash_status
{
    kStatus_OTA_Flash_Success,         /*!< flash operation is successful*/
    kStatus_OTA_Flash_Fail,            /*!< flash operation is not successful*/
    kStatus_OTA_Flash_InvalidArgument, /*!< Invalid argument */
    kStatus_OTA_Flash_AlignmentError,  /*!< Alignment Error */
    kStatus_OTA_Flash_EccError,        /*!< ECC error detected */
    kStatus_OTA_Flash_Error,           /*!< Illegal command */
    kStatus_OTA_Flash_NotSupport,      /*!< Not support */
} ota_flash_status_t;

typedef struct
{
    ota_flash_status_t (*init)(void);
    ota_flash_status_t (*format_storage)(void);
    ota_flash_status_t (*writeData)(uint32_t NoOfBytes, uint32_t Addr, uint8_t *Outbuf);
    ota_flash_status_t (*readData)(uint16_t NoOfBytes, uint32_t Addr, uint8_t *inbuf);
    uint8_t (*isBusy)(void);
    ota_flash_status_t (*eraseArea)(uint32_t *pAddr, int32_t *pSize, bool non_blocking);
    ota_flash_status_t (*flushWriteBuf)(void);

} OtaFlashOps_t;

/* When performing Erase / Program operations, a Flash device cannot be read.
 * During the duration such operation, instructions canot be fetched from device.
 * Buffered posted operation can be used in order to postpone the actions until a
 * propitious time, i.e. from the Idle loop.
 * The erase time is more or less proportional to the number of sectors to erase,
 * nonetheless erasing by block provides a slight benefit : ~10% faster than sector
 * by sector approach.
 */
typedef enum
{
    FLASH_OP_WRITE,            /*< Posted Write (Program) operation */
    FLASH_OP_ERASE_THEN_WRITE, /*< Posted Erase then Write operation */
    FLASH_OP_READ,             /*< Queued Read operation */
    FLASH_OP_ERASE_SECTOR,     /*< Posted Erase sector (minimal erase size) */
    FLASH_OP_ERASE_BLOCK,      /*< Posted Erase Block (32kB or 64kB) optional implementation */
    FLASH_OP_ERASE_AREA,
    FLASH_OP_ERASE_NEXT_BLOCK,
    FLASH_OP_ERASE_NEXT_BLOCK_COMPLETE,
    FLASH_OP_CONSUMED = 0xff
} FLASH_op_type;

typedef struct
{
    FLASH_op_type op_type;
    uint32_t      flash_addr;
    uint32_t      sz;
    uint8_t       buf[PROGRAM_PAGE_SZ];
} FLASH_TransactionOp_t;

typedef struct
{
    list_element_t        list_node;
    FLASH_TransactionOp_t flash_transac;
} FLASH_TransactionOpNode_t;

typedef struct
{
    uint32_t OtaImageTotalLength;   /*!< Total length of the OTA image that is currently being written in OTA storage */

    uint32_t OtaImageCurrentLength; /*!< The length of the OTA image that has being written in OTA storage so far */

    uint32_t CurrentStorageAddress; /*!< Current write address in the OTA storage */
    uint32_t ErasedUntilOffset;     /*!< Limit up to which erase was already completed */

    OtaImgState_t FwUpdImageState;  /*!< State of OTA evolving throughout firmware update.
                                     * When a new image is ready the flash flags will be written in idle task */

    const OtaFlashOps_t *FlashOps;  /*! pointer on flash operation depending on flash where selected partition dwells
                                       internal or external */

    const OtaPartition_t *ota_partition;
    uint32_t ImageOffset;    /*!< Select offset at which the image is expected to start - leave space for a possible
                              * BootInfo section remains 0 for most devices (not the offset of the partition itself)*/

    uint32_t MaxImageLength; /*!< Define the maximum image size, normally the partition size minus the image offset if
                                any  */

    messaging_t op_queue;    /*!< Queue of flash operations */

    FLASH_TransactionOp_t *cur_transaction; /*!< Element in which accumulation of PROGRAM_PAGE_SZ bytes is performed  */

    uint32_t StorageAddressWritten; /*!< Address of actual EEPROM address, must remain less than CurrentEepromAddress */

    uint32_t
        OtaImageLengthWritten; /*! Size actually written into OTA storage must be less than OtaImageCurrentLength */

    int          cnt_idle_op;  /*! Number of polling: OTA_TransactionResume calls  */
    int          max_cnt_idle;
    int          q_sz;         /*!< Number of pending operations in op_queue transaction queue */
    int          q_max;        /*!< Maximum value reached by q_sz */
    void        *PostedQ_storage;
    uint8_t      PostedQ_capacity;
    uint8_t      PostedQ_nb_in_queue;
    list_label_t transaction_free_list;
    bool         Initialized;
    bool         VerifyWrites; /*!< Defines requested behaviour regarding read back of programmed buffer */

    ota_config_t *config;
    /* Mutex used for locking OTA Transactions */
    OSA_MUTEX_HANDLE_DEFINE(msgQueueMutex);

} OtaStateCtx_t;

extern OtaStateCtx_t mOtaHdl;

#define DBG_LEVEL_NONE    0
#define DBG_LEVEL_WARNING 1
#define DBG_LEVEL_INFO    2
#define DBG_LEVEL_DEBUG   3
#define DBG_LEVEL_VERBOSE 4

#define OTA_DBG_LVL DBG_LEVEL_NONE

#if (OTA_DBG_LVL > DBG_LEVEL_NONE)
#include "fsl_debug_console.h"
#endif

#if (OTA_DBG_LVL >= DBG_LEVEL_VERBOSE)
#define OTA_VERBOSE_TRACE PRINTF
#else
#define OTA_VERBOSE_TRACE(...)
#endif
#if (OTA_DBG_LVL >= DBG_LEVEL_DEBUG)
#define OTA_DEBUG_TRACE PRINTF
#else
#define OTA_DEBUG_TRACE(...)
#endif

#if (OTA_DBG_LVL >= DBG_LEVEL_INFO)
#define OTA_INFO_TRACE PRINTF
#else
#define OTA_INFO_TRACE(...)
#endif

#if (OTA_DBG_LVL >= DBG_LEVEL_WARNING)
#define OTA_WARNING_TRACE PRINTF
#else
#define OTA_WARNING_TRACE(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif /* __OTA_PRIVATE_H__ */
