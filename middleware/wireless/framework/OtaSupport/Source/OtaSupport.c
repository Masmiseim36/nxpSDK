/*! *********************************************************************************
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * This source file contains the code that enables the OTA Programming protocol
 * to load an image received over the air into an external memory, using
 * the format that the Bootloader will understand
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#include <stddef.h>

#include "EmbeddedTypes.h"
#include "OtaSupport.h"
#include "fsl_component_messaging.h"
#include "FunctionLib.h"
#include "fwk_platform_ota.h"
#include "OtaPrivate.h"
#include "fsl_os_abstraction.h"

/******************************************************************************
*******************************************************************************
* Private Macros
*******************************************************************************
******************************************************************************/

#define gOtaVerifyWriteBufferSize_d (16) /* [bytes] */

#define RAISE_ERROR(x, val) \
    {                       \
        x = (val);          \
        break;              \
    }

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/

union ota_op_completion_cb
{
    /*! Prototype of ota_completion callback */
    ota_op_completion_cb_t func;
    uint32_t               pf;
};

/******************************************************************************
*******************************************************************************
* Private Prototypes
*******************************************************************************
******************************************************************************/

static void                   OTA_WritePendingData(void);
static int                    OTA_TransactionQueuePurge(void);
static void                   OTA_MsgQueue(FLASH_TransactionOp_t *pMsg);
static void                   OTA_MsgDequeue(void);
static otaResult_t            OTA_PostWriteToFlash(uint16_t NoOfBytes, uint32_t Addr, uint8_t *pData);
static bool                   OTA_UsePostedOperation(void);
static void                   OTA_FlashTransactionFree(FLASH_TransactionOp_t *pTr);
static FLASH_TransactionOp_t *OTA_FlashTransactionAlloc(void);
static otaResult_t            OTA_CheckVerifyFlash(uint8_t *pData, uint32_t flash_addr, uint16_t length);
static otaResult_t            OTA_WriteToFlash(uint16_t NoOfBytes, uint32_t Addr, uint8_t *outbuf);

static ota_flash_status_t OTA_TreatFlashOpWrite(FLASH_TransactionOp_t *pMsg);
static ota_flash_status_t OTA_TreatFlashOpEraseNextBlock(FLASH_TransactionOp_t *pMsg);
static ota_flash_status_t OTA_TreatFlashOpEraseNextBlockComplete(FLASH_TransactionOp_t *pMsg);
#if defined               DeprecatedOtaHasPostedEraseArea && (DeprecatedOtaHasPostedEraseArea > 0)
static ota_flash_status_t OTA_TreatFlashOpEraseArea(FLASH_TransactionOp_t *pMsg);
#endif
#if defined               DeprecatedOtaHasPostedEraseSector && (DeprecatedOtaHasPostedEraseSector > 0)
static ota_flash_status_t OTA_TreatFlashOpEraseSector(FLASH_TransactionOp_t *pMsg);
#endif
/******************************************************************************
*******************************************************************************
* Private Memory Declarations
*******************************************************************************
******************************************************************************/

static ota_config_t configuration = {
    .PostedOpInIdleTask         = false,
    .maxConsecutiveTransactions = 3,
};

OtaStateCtx_t mHdl = {
    .OtaImageTotalLength   = 0,
    .OtaImageCurrentLength = 0,
    .CurrentStorageAddress = 0,
    .ErasedUntilOffset     = 0,
    .FwUpdImageState       = OtaImgState_None,
    .FlashOps              = NULL,
    .ota_partition         = NULL,
    .ImageOffset           = 0,
    .MaxImageLength        = 0,

    .q_sz                  = 0,
    .q_max                 = 0,
    .StorageAddressWritten = 0,
    .OtaImageLengthWritten = 0,
    .PostedQ_storage       = NULL,
    .PostedQ_capacity      = 0,
    .PostedQInitialized    = false,
    .VerifyWrites          = true,
    .config                = &configuration,
};

/******************************************************************************
*******************************************************************************
* Public Functions
*******************************************************************************
******************************************************************************/
otaResult_t OTA_Initialize(void)
{
    otaResult_t status = gOtaSuccess_c;

    do
    {
        OtaImgState_t img_state;

        if (mHdl.FwUpdImageState != OtaImgState_None)
        {
            OTA_CancelImage();
            break;
        }
        img_state = OTA_GetImgState();
        if (img_state == OtaImgState_Fail)
        {
            RAISE_ERROR(status, gOtaError_c);
        }

        if ((img_state == OtaImgState_Permanent) || (img_state == OtaImgState_RunCandidate))
        {
            status = OTA_UpdateImgState(img_state);
        }
        else if (img_state == OtaImgState_None)
        {
            /* case where we just downloaded via debugger */
            status = OTA_UpdateImgState(OtaImgState_Permanent);
        }
        else
        {
            status = gOtaSuccess_c; /* no state transition : leave as is without error */
        }
    } while (false);

    return status;
}
otaResult_t OTA_ServiceInit(void *posted_ops_storage, size_t posted_ops_sz)
{
    otaResult_t st = gOtaSuccess_c;
    do
    {
        if (posted_ops_storage == NULL)
        {
            if (posted_ops_sz == 0u)
            {
                /* The implementer has opted for direct operation (no posted transactions) */
                /* No other initialization is required */
                if (mHdl.PostedQInitialized)
                {
                    /* Should have called OTA_ServiceDeInit beforehand */
                    RAISE_ERROR(st, gOtaInvalidOperation_c);
                }
                if ((mHdl.FwUpdImageState == OtaImgState_Acquiring) ||
                    (mHdl.FwUpdImageState == OtaImgState_CandidateRdy))
                {
                    /* Should have cancelled the OTA beforehand */
                    RAISE_ERROR(st, gOtaInvalidOperation_c);
                }
                mHdl.FwUpdImageState = OtaImgState_None;
                st                   = OTA_Initialize();
                break;
            }
            else
            {
                RAISE_ERROR(st, gOtaInvalidParam_c);
            }
        }
        /* If we have not exit before , the posted operations structure needs to be set */
        list_status_t         status;
        list_element_handle_t list_handle;

        /* FLASH_TransactionOpNode_t size shall be multiple of 4 bytes. The reason is to avoid
         *  doing unaligned access when going through the transaction operation queue, this could lead to
         *  crash on some toolchain (gcc) when using some instructions not supporting unaligned access*/
        assert((sizeof(FLASH_TransactionOpNode_t) & 0x3U) == 0U);
        assert(gOtaTransactionSz_d == sizeof(FLASH_TransactionOpNode_t));

        uint8_t                    nbTransactions = (uint8_t)(posted_ops_sz / sizeof(FLASH_TransactionOpNode_t));
        FLASH_TransactionOpNode_t *pOpNode        = (FLASH_TransactionOpNode_t *)posted_ops_storage;
        uint32_t                   posted_ops_storage_32bits;

        FLib_MemCpyWord(&posted_ops_storage_32bits, &posted_ops_storage);
        /* Check arguments */

        if ((posted_ops_storage_32bits & 0x3U) != 0U)
        {
            /* Avoid unaligned access on operation storage buffer, posted_ops_storage shall be word aligned */
            RAISE_ERROR(st, gOtaInvalidParam_c);
        }
        if ((posted_ops_sz % sizeof(FLASH_TransactionOpNode_t)) != 0U)
        {
            /* ops buffer size must be a multiple of transaction node */
            RAISE_ERROR(st, gOtaInvalidParam_c);
        }
        if (nbTransactions < 2U)
        {
            /* at least 2 transactions are needed to make use of posted ops */
            RAISE_ERROR(st, gOtaInvalidParam_c);
        }

        /* Check state */
        if (mHdl.PostedQ_nb_in_queue != 0u)
        {
            RAISE_ERROR(st, gOtaInvalidOperation_c);
        }

        if (mHdl.PostedQInitialized)
        {
            /* Covers the cases of pending transations in queue,
             * that could not be pending unless initialized  */
            RAISE_ERROR(st, gOtaInvalidOperation_c);
        }
        if ((mHdl.FwUpdImageState == OtaImgState_Acquiring) || (mHdl.FwUpdImageState == OtaImgState_CandidateRdy))
        {
            /* Should have cancelled the OTA beforehand */
            RAISE_ERROR(st, gOtaInvalidOperation_c);
        }

        /* Prevent creating mutex multiple times */
        mHdl.PostedQ_storage = posted_ops_storage;

        LIST_Init(&mHdl.transaction_free_list, 0);

        for (uint8_t i = 0U; i < nbTransactions; i++)
        {
            void *ptr;
            ptr         = &pOpNode[i];
            list_handle = (list_element_handle_t)ptr;
            status      = LIST_AddTail(&mHdl.transaction_free_list, list_handle);
            assert(status == kLIST_Ok);
            (void)status;
        }
        mHdl.PostedQ_capacity = nbTransactions;
        if (OSA_MutexCreate((osa_mutex_handle_t)mHdl.msgQueueMutex) != KOSA_StatusSuccess)
        {
            RAISE_ERROR(st, gOtaError_c);
        }
        mHdl.PostedQInitialized = true;

        mHdl.FwUpdImageState = OtaImgState_None;
        st                   = OTA_Initialize();

    } while (false);

    return st;
}

otaResult_t OTA_ServiceDeInit(void)
{
    otaResult_t st = gOtaSuccess_c;
    do
    {
        mHdl.PostedQ_capacity    = 0;
        mHdl.PostedQ_nb_in_queue = 0;
        if (!mHdl.PostedQInitialized)
        {
            break;
        }
        if (mHdl.FwUpdImageState == OtaImgState_Acquiring)
        {
            RAISE_ERROR(st, gOtaInvalidOperation_c);
        }

        if (OSA_MutexDestroy((osa_mutex_handle_t)mHdl.msgQueueMutex) != KOSA_StatusSuccess)
        {
            RAISE_ERROR(st, gOtaError_c);
        }
        mHdl.PostedQInitialized = false;
    } while (false);

    return st;
}

void OTA_GetDefaultConfig(ota_config_t *userConfig)
{
    assert(userConfig != NULL);
    (void)memcpy(userConfig, mHdl.config, sizeof(ota_config_t));
}

void OTA_SetConfig(ota_config_t *userConfig)
{
    assert(userConfig != NULL);
    (void)memcpy(mHdl.config, userConfig, sizeof(ota_config_t));
}

/*! *********************************************************************************
 * \brief  Starts the process of writing a new image to the OTA storage.
 *
 * \param[in] length: the length of the image to be written in the OTA storage
 *
 * \return
 *  - gOtaInvalidParam_c: the intended length is bigger than the FLASH capacity
 *  - gOtaInvalidOperation_c: the process is already started (can be cancelled)
 *  - gOtaEepromError_c: can not detect external OTA storage
 *
 ********************************************************************************** */
otaResult_t OTA_StartImage(uint32_t length)
{
    otaResult_t status = gOtaSuccess_c;
    do
    {
        if (NULL == mHdl.FlashOps)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        /* Check if we already have an operation of writing an OTA image in the OTA Storage
        in progress and if yes, deny the current request */
        /* A new image cannot be started if :
         *   - a previous image is being acquired (OtaImgState_Acquiring)
         *   - a candidate image was acquired (OtaImgState_CandidateRdy) and reset is awaiting to have it
         *     launched by bootloader, self-test it.
         *   - or if the */

        if (mHdl.FwUpdImageState != OtaImgState_Permanent)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }

        /* Check if the internal FLASH and the OTA storage have enough room to store
           the image */
        if (length > mHdl.MaxImageLength)
        {
            RAISE_ERROR(status, gOtaImageTooLarge_c);
        }

        /* Mark that we have started loading an OTA image in OTA Storage */
        if (OTA_UpdateImgState(OtaImgState_Acquiring) != gOtaSuccess_c)
        {
            /* the transition is only valid if we are in the right state */
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        /* Save the total length of the OTA image */
        mHdl.OtaImageTotalLength = length;
        /* Init the length of the OTA image currently written */
        mHdl.OtaImageCurrentLength = 0;
        /* Init the current OTA Storage write address */
        mHdl.CurrentStorageAddress = mHdl.ImageOffset;
        mHdl.OtaImageLengthWritten = 0;
        mHdl.StorageAddressWritten = mHdl.ImageOffset;

    } while (false);
    return status;
}

/*! *********************************************************************************
 * \brief  Places the next image chunk into the external FLASH. The CRC will not be computed.
 *
 * \param[in] pData          pointer to the data chunk
 * \param[in] length         the length of the data chunk
 * \param[in] pImageLength   if it is not null and the function call is successful,
 *                           it will be filled with the current length of the image
 * \param[in] pImageOffset   if it is not null contains the current offset of the image
 *
 * \return
 *  - gOtaInvalidParam_c: pData is NULL or the resulting image would be bigger than the
 *       final image length specified with OTA_StartImage()
 *  - gOtaInvalidOperation_c: the process is not started
 *
 ********************************************************************************** */
otaResult_t OTA_PushImageChunk(uint8_t *pData, uint16_t length, uint32_t *pImageLength, uint32_t *pImageOffset)
{
    otaResult_t status = gOtaSuccess_c;
    do
    {
        bool posted_pos = OTA_UsePostedOperation();
        if (mHdl.FlashOps == NULL)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        /* Cannot add a chunk without a prior call to OTA_StartImage() */
        if (mHdl.FwUpdImageState != OtaImgState_Acquiring)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        /* Validate parameters */
        if ((length == 0U) || (pData == NULL))
        {
            RAISE_ERROR(status, gOtaInvalidParam_c);
        }
        /* Check if the chunk does not extend over the boundaries of the image */
        if (mHdl.OtaImageCurrentLength + length > mHdl.OtaImageTotalLength)
        {
            RAISE_ERROR(status, gOtaInvalidParam_c);
        }

        /* Received a chunk with offset */
        if (NULL != pImageOffset)
        {
            mHdl.CurrentStorageAddress = mHdl.ImageOffset + *pImageOffset;
        }
        if (posted_pos)
        {
            OTA_DEBUG_TRACE("storage addr=%x length=%d\r\n", mHdl.CurrentStorageAddress, length);
            status = OTA_PostWriteToFlash(length, mHdl.CurrentStorageAddress, pData);
        }
        else
        {
            /* Try to write the data chunk into the image storage */
            status = OTA_WriteToFlash(length, mHdl.CurrentStorageAddress, pData);
        }

        if (status != gOtaSuccess_c)
        {
            break;
        }
        /* Data chunk successfully written into OTA Storage
        Update operation parameters */
        mHdl.CurrentStorageAddress += length;
        mHdl.OtaImageCurrentLength += length;

        /* Return the currently written length of the OTA image to the caller */
        if (pImageLength != NULL)
        {
            *pImageLength = mHdl.OtaImageCurrentLength;
        }
    } while (false);
    return status;
}

/*! *********************************************************************************
 * \brief  Read and copy from previous pushed chunks (Flash or RAM) to RAM pointed by pData
 *
 * \param[in] pData          pointer to the data chunk, to be allocated by caller
 * \param[in] length         the length of the data chunk
 * \param[in] pImageOffset   if it is not null contains the current offset of the image
 *
 * \return
 *  - gOtaInvalidParam_c: pData is NULL or the resulting image would be bigger than the
 *       final image length specified with OTA_StartImage()
 *  - gOtaInvalidOperation_c: the process is not started
 *
 ********************************************************************************** */
otaResult_t OTA_PullImageChunk(uint8_t *pData, uint16_t length, uint32_t *pImageOffset)
{
    otaResult_t        status = gOtaSuccess_c;
    ota_flash_status_t st;

    do
    {
        bool     posted_ops;
        uint32_t mAbsoluteOffset;
        /* Validate parameters */
        if ((length == 0U) || (pData == NULL) || (pImageOffset == NULL))
        {
            RAISE_ERROR(status, gOtaInvalidParam_c);
        }

        if (mHdl.FlashOps == NULL)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        posted_ops      = OTA_UsePostedOperation();
        mAbsoluteOffset = mHdl.ImageOffset + *pImageOffset;
        if (posted_ops)
        {
            /* When posted operations are used, a requested chunk may be partially written to flash
             * and the remainder staged in RAM buffer.
             */
            uint32_t end_of_queried_data;
            end_of_queried_data = mAbsoluteOffset + length;
            if (mAbsoluteOffset > mHdl.StorageAddressWritten)
            {
                if (mAbsoluteOffset <= (mHdl.StorageAddressWritten + PROGRAM_PAGE_SZ) &&
                    (end_of_queried_data <= (mHdl.StorageAddressWritten + PROGRAM_PAGE_SZ + 1U)))
                {
                    /* The asked buffer is still in RAM */
                    FLib_MemCpy(pData, mHdl.cur_transaction->buf + (mAbsoluteOffset - PROGRAM_PAGE_SZ), length);
                    status = gOtaSuccess_c;
                    break;
                }
            }
            else
            {
                /* so (mAbsoluteOffset <= mHdl.StorageAddressWritten)
                 * end_of_queried_data should be positioned inside the selected image partition
                 * taking into consideration the offset at which the image starts in flash */
                if ((end_of_queried_data > (mHdl.OtaImageLengthWritten + mHdl.ImageOffset)) &&
                    (end_of_queried_data < (mHdl.StorageAddressWritten + PROGRAM_PAGE_SZ)))
                {
                    uint16_t lenInFlash = (length - (uint16_t)mHdl.OtaImageLengthWritten);
                    uint16_t lenInRam   = (length - lenInFlash);
                    /* The asked buffer is in Flash and in RAM */
                    st = mHdl.FlashOps->readData(lenInFlash, mAbsoluteOffset, pData);
                    if (st != kStatus_OTA_Flash_Success)
                    {
                        RAISE_ERROR(status, gOtaExternalFlashError_c);
                    }
                    pData += lenInFlash;
                    FLib_MemCpy(pData, mHdl.cur_transaction->buf, lenInRam);
                    status = gOtaSuccess_c;
                    break;
                }
            }
        }
        /* The asked buffer is in Flash */
        st = mHdl.FlashOps->readData(length, mAbsoluteOffset, pData);
        if (st != kStatus_OTA_Flash_Success)
        {
            RAISE_ERROR(status, gOtaExternalFlashError_c);
        }
        status = gOtaSuccess_c;
    } while (false);
    return status;
}

/*! *********************************************************************************
 * \brief  Finishes the writing of a new image to the permanent storage.
 *         It will write the image header (signature and length) and footer (sector copy bitmap).
 *
 * \param[in] bitmap   pointer to a  byte array indicating the sector erase pattern for the
 *                     internal FLASH before the image update.
 *
 * \return
 *  - gOtaInvalidOperation_c: the process is not started,
 *  - gOtaEepromError_c: error while trying to write the OTA Storage
 *
 ********************************************************************************** */
otaResult_t OTA_CommitImage(uint8_t *pBitmap)
{
    NOT_USED(pBitmap);
    otaResult_t status = gOtaSuccess_c;
    do
    {
        OtaLoaderInfo_t ota_load_info;

        /* Cannot commit a image without a prior call to OTA_StartImage() */
        if (mHdl.FwUpdImageState != OtaImgState_Acquiring)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        /* Cannot commit if the image hasn't been completely received */
        if (mHdl.OtaImageCurrentLength != mHdl.OtaImageTotalLength)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        /* Writes the pending data to flash */
        OTA_WritePendingData();
        /* After flushing the remainder the written length must match the queued length */
        if (mHdl.OtaImageLengthWritten != mHdl.OtaImageTotalLength)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        ota_load_info.image_sz   = mHdl.OtaImageTotalLength;
        ota_load_info.image_addr = mHdl.ota_partition->start_offset + mHdl.ImageOffset;

        ota_load_info.pBitMap = pBitmap;

        if (0 != PLATFORM_OtaBootDataUpdateOnCommit(&ota_load_info))
        {
            RAISE_ERROR(status, gOtaImageInvalid_c);
        }
        /* Flash flags will be written in next idle task execution */
        status = OTA_UpdateImgState(OtaImgState_CandidateRdy);

        /* End the load of OTA image in OTA storage process */
    } while (false);
    return status;
}

/*! *********************************************************************************
 * \brief  Set the boot flags, to trigger the Bootloader at the next CPU reset.
 * Must be invoked after the completion of the image download (after OTA_CommitImage).
 * and after the connection with the OTA server has been closed, if required. Specify
 * offset to be used to determine the exact image location in case it's not located
 * at the start of OTA partition.
 *
 * \param[in] offset specify an offset to determine image address
 *
 ********************************************************************************** */
void OTA_SetNewImageFlagWithOffset(uint32_t offset)
{
    /* OTA image successfully written into the non-volatile storage.
       Set the boot flag to trigger the Bootloader at the next CPU Reset. */

    int st;

    if (mHdl.FwUpdImageState == OtaImgState_CandidateRdy)
    {
        if (offset < mHdl.OtaImageTotalLength)
        {
            OtaLoaderInfo_t loader_info;
            loader_info.image_addr     = mHdl.ota_partition->start_offset + mHdl.ImageOffset + offset;
            loader_info.image_sz       = mHdl.OtaImageTotalLength - offset;
            loader_info.pBitMap        = NULL;
            loader_info.partition_desc = mHdl.ota_partition;

            st = PLATFORM_OtaNotifyNewImageReady(&loader_info);
            if (st != 0)
            {
                mHdl.FwUpdImageState = OtaImgState_Fail;
            }
        }
    }
}

/*! *********************************************************************************
 * \brief  Set the boot flags, to trigger the Bootloader at the next CPU reset.
 * Must be invoked after the completion of the image download (after OTA_CommitImage).
 * and after the connection with the OTA server has been closed, if required.
 *
 ********************************************************************************** */
void OTA_SetNewImageFlag(void)
{
    OTA_SetNewImageFlagWithOffset(0U);
}

OtaImgState_t OTA_GetImgState(void)
{
    OtaImgState_t ret       = OtaImgState_Fail;
    uint8_t       img_state = (uint8_t)mHdl.FwUpdImageState;
    int           val       = -1;
    val                     = PLATFORM_OtaGetImageState(&img_state);
    if (val == 0)
    {
        /* The actual Ota state is retrived from the PLATFORM dependent function  */
        mHdl.FwUpdImageState = (OtaImgState_t)img_state;
    }
    /* if 1 was returned the PLATFORM_OtaGetImageState does not know */
    if (val >= 0)
    {
        ret = mHdl.FwUpdImageState;
    }
    return ret;
}

static int OtaGoToNoneState(void)
{
    int st = -1;
    switch (mHdl.FwUpdImageState)
    {
        /* Full re-initialization : forget previously received OTA image */
        case OtaImgState_Acquiring:
        case OtaImgState_CandidateRdy:
        case OtaImgState_RunCandidate:
        case OtaImgState_Fail:
        {
            if (mHdl.OtaImageTotalLength != 0u)
            {
                OTA_CancelImage();
            }
            st = 0;
        }
        break;
        case OtaImgState_Permanent:
        case OtaImgState_None:
            st = 0;
            break;
        /* Once we have determined we are in RunCandidate state, should go to Permanent or fail and reboot  */
        default:; /* Nothing to do */
            break;
    }
    return st;
}

static int OtaGoToPermanentState(void)
{
    int st = -1;
    switch (mHdl.FwUpdImageState)
    {
        case OtaImgState_None: /* not initialized yet */
        case OtaImgState_Fail: /* forget previous error */
            st = 0;
            break;
        case OtaImgState_Permanent:
            st = 1;
            break;
        case OtaImgState_RunCandidate:
            /* go to permanent */
            st = PLATFORM_OtaUpdateImageState((uint8_t)OtaImgState_Permanent);
            break;
        case OtaImgState_Acquiring:
        case OtaImgState_CandidateRdy:
        {
            if (mHdl.OtaImageTotalLength != 0u)
            {
                OTA_CancelImage();
            }
            st = 0;
        }
        break;
        default:; /* Nothing to do */
            break;
    }

    return st;
}

static int OtaGoToCandidateRdyState(void)
{
    int st = -1;
    switch (mHdl.FwUpdImageState)
    {
        case OtaImgState_Acquiring:
        {
            if (mHdl.OtaImageTotalLength == mHdl.OtaImageCurrentLength)
            {
                OtaLoaderInfo_t loader_info;
                loader_info.image_addr     = mHdl.ota_partition->start_offset + mHdl.ImageOffset;
                loader_info.image_sz       = mHdl.OtaImageTotalLength;
                loader_info.pBitMap        = NULL;
                loader_info.partition_desc = mHdl.ota_partition;

                st = PLATFORM_OtaNotifyNewImageReady(&loader_info);
            }
            else
            {
                st = -1;
            }
        }
        break;
        case OtaImgState_CandidateRdy:
            st = 1; /* do nothing */
            break;
        case OtaImgState_Permanent:
        case OtaImgState_RunCandidate:
        case OtaImgState_None:
        case OtaImgState_Fail:
        default:; /* Nothing to do */
            break;
    }

    return st;
}

static int OtaGoToAcquiringState(void)
{
    int st = -1;
    switch (mHdl.FwUpdImageState)
    {
        case OtaImgState_Acquiring:
        case OtaImgState_CandidateRdy:
        {
            if (mHdl.OtaImageTotalLength != 0u)
            {
                OTA_CancelImage();
            }
            st = 0;
        }
        break;
        case OtaImgState_Permanent:
            st = 0;
            break;
        /* We can go from RunCandidate state to Permanent but not acquire a new FW directly  */
        case OtaImgState_RunCandidate:
        case OtaImgState_Fail:
        case OtaImgState_None:
        default:; /* Nothing to do */
            break;
    }

    return st;
}

otaResult_t OTA_UpdateImgState(OtaImgState_t new_state)
{
    int         st     = -1;
    otaResult_t status = gOtaError_c;

    switch (new_state)
    {
        case OtaImgState_None:
            st = OtaGoToNoneState();
            break;
        case OtaImgState_Permanent:
            st = OtaGoToPermanentState();
            break;

        case OtaImgState_CandidateRdy:
            st = OtaGoToCandidateRdyState();
            break;

        case OtaImgState_Acquiring:
            st = OtaGoToAcquiringState();
            break;
        case OtaImgState_RunCandidate:
            assert(new_state != OtaImgState_RunCandidate);
            st = -1; /* transition not allowed */
            break;

        default:; /* Nothing to do */
            break;
    }

    if (st >= 0)
    {
        mHdl.FwUpdImageState = new_state;
        status               = gOtaSuccess_c;
    }
    else
    {
        mHdl.FwUpdImageState = OtaImgState_Fail;
        status               = gOtaError_c;
    }

    return status;
}

/*! *********************************************************************************
 * \brief  Cancels the process of writing a new image to the OTA storage.
 *
 ********************************************************************************** */
void OTA_CancelImage(void)
{
    if ((mHdl.FwUpdImageState == OtaImgState_Acquiring) || (mHdl.FwUpdImageState == OtaImgState_CandidateRdy) ||
        (mHdl.FwUpdImageState == OtaImgState_Fail))
    {
        if (OTA_UsePostedOperation())
        {
            (void)OTA_TransactionQueuePurge();
        }
    }
    mHdl.FwUpdImageState = OtaImgState_Permanent;
}

/*! *********************************************************************************
 * \brief  Compute CRC over a data chunk.
 * This CRC computation is the CCITT CRC16 (polynomial X^16 + X^12+ X^5 + 1).
 *
 * \param[in] pData        pointer to the data chunk
 * \param[in] length       the length of the data chunk
 * \param[in] crcValueOld  current CRC value
 *
 * \return  computed CRC.
 *
 ********************************************************************************** */
uint16_t OTA_CrcCompute(uint8_t *pData, uint16_t lenData, uint16_t crcValueOld)
{
    uint8_t i;

    while (0U != (lenData--))
    {
        crcValueOld ^= (uint16_t)((uint16_t)*pData++ << 8);
        for (i = 0; i < 8U; ++i)
        {
            if (0U != (crcValueOld & 0x8000U))
            {
                crcValueOld = (crcValueOld << 1) ^ 0x1021U;
            }
            else
            {
                crcValueOld = crcValueOld << 1;
            }
        }
    }
    return crcValueOld;
}

/*! *********************************************************************************
 * \brief  This function is called in order to erase the entire image storage
 *         (external memory or internal flash)
 *
 * \return  error code.
 *
 ********************************************************************************** */
otaResult_t OTA_EraseStorageMemory(void)
{
    otaResult_t status;
    do
    {
        ota_flash_status_t st;

        if (NULL == mHdl.FlashOps)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        st = mHdl.FlashOps->format_storage();
        if (st != kStatus_OTA_Flash_Success)
        {
            RAISE_ERROR(status, gOtaExternalFlashError_c);
        }
        status = gOtaSuccess_c;
    } while (false);
    return status;
}

/*! *********************************************************************************
 * \brief  Read from the image storage (external memory or internal flash)
 *
 * \param[in] pData    pointer to the data chunk
 * \param[in] length   the length of the data chunk
 * \param[in] address  image storage address
 *
 * \return  error code.
 *
 ********************************************************************************** */
otaResult_t OTA_ReadStorageMemory(uint8_t *pData, uint16_t length, uint32_t address)
{
    otaResult_t status;
    do
    {
        ota_flash_status_t st;
        if (NULL == mHdl.FlashOps)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }

        st = mHdl.FlashOps->readData(length, address, pData);
        if (st != kStatus_OTA_Flash_Success)
        {
            RAISE_ERROR(status, gOtaExternalFlashError_c);
        }

        status = gOtaSuccess_c;
    } while (false);

    return status;
}

/*! *********************************************************************************
 * \brief  Write into the image storage (external memory or internal flash)
 *
 * \param[in] pData    pointer to the data chunk
 * \param[in] length   the length of the data chunk
 * \param[in] address  image storage offset relative to OTA partition start
 *
 * \return  error code.
 *
 ********************************************************************************** */
otaResult_t OTA_WriteStorageMemory(uint8_t *pData, uint16_t length, uint32_t address)
{
    otaResult_t status;
    do
    {
        if (NULL == mHdl.FlashOps)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }
        status = OTA_WriteToFlash(length, address, pData);

    } while (false);
    return status;
}

/*! *********************************************************************************
 * \brief  Called in background to poll whether current flash transactions completed
 *         and process the next one from the queue.
 *
 * \return  number of transactions treated.
 *
 ********************************************************************************** */
int OTA_TransactionResume(void)
{
    int                nb_treated = 0;
    ota_flash_status_t st         = kStatus_OTA_Flash_Success;

    if (mHdl.PostedQInitialized)
    {
        /* Mutex to lock transaction processing */
        osa_status_t status = OSA_MutexLock(mHdl.msgQueueMutex, osaWaitForever_c);
        assert(status == KOSA_StatusSuccess);

        while (OTA_UsePostedOperation() &&
               (kStatus_OTA_Flash_Success == st) /* Stop as soon as there is an error */
               /* && mHdl.LoadOtaImageInProgress */
               && OTA_IsTransactionPending() /* There are queued flash operations pending in queue */
               && (nb_treated <
                   mHdl.config->maxConsecutiveTransactions)) /* ... but do not schedule too many in a same pass */
        {
            if (mHdl.FlashOps->isBusy() != 0U)
            {
                /* There were transactions pending but we consumed none */
                mHdl.cnt_idle_op++;
                if (mHdl.cnt_idle_op > mHdl.max_cnt_idle)
                {
                    mHdl.max_cnt_idle = mHdl.cnt_idle_op;
                }
                break;
            }
            nb_treated++;
            /* Use MSG_GetHead so as to leave Msg in queue so that op_type or sz can be transformed when operation
             * completes (in particular for block erasure) */
            FLASH_TransactionOp_t *pMsg = MSG_QueueGetHead(&mHdl.op_queue);
            if (pMsg == NULL)
            {
                break;
            }
            switch (pMsg->op_type)
            {
                case FLASH_OP_WRITE:
                {
                    st = OTA_TreatFlashOpWrite(pMsg);
                }
                break;
#if defined DeprecatedOtaHasPostedEraseArea && (DeprecatedOtaHasPostedEraseArea > 0)
                case FLASH_OP_ERASE_AREA:
                {
                    st = OTA_TreatFlashOpEraseArea(pMsg);
                }
                break;
#endif
                case FLASH_OP_ERASE_NEXT_BLOCK:
                {
                    st = OTA_TreatFlashOpEraseNextBlock(pMsg);
                }
                break;
                case FLASH_OP_ERASE_NEXT_BLOCK_COMPLETE:
                {
                    st = OTA_TreatFlashOpEraseNextBlockComplete(pMsg);
                }
                break;
#if defined DeprecatedOtaHasPostedEraseSector && (DeprecatedOtaHasPostedEraseSector > 0)
                case FLASH_OP_ERASE_BLOCK:
                case FLASH_OP_ERASE_SECTOR:
                {
                    st = OTA_TreatFlashOpEraseSector(pMsg);
                }
                break;
#endif
                default:
                {
                    /*MISRA rule 16.4*/
                    assert(0);
                    break;
                }
            };
        } /* while */
        /* There were transactions pending but we consumed some */
        mHdl.cnt_idle_op = 0;
        if (st != kStatus_OTA_Flash_Success)
        {
            OTA_CancelImage();
        }
        /* Unlock Mutex to be accessed by other tasks */
        status = OSA_MutexUnlock(mHdl.msgQueueMutex);
        assert(status == KOSA_StatusSuccess);

        /* Fix MISRA in release mode when assert() is stubbed*/
        NOT_USED(status);
    }
    return nb_treated;
}

/*****************************************************************************
 *   OTA_MakeHeadRoomForNextBlock
 *
 *  This function is called in order to erase enough blocks to receive next OTA window
 *
 *****************************************************************************/
otaResult_t OTA_MakeHeadRoomForNextBlock(uint32_t size, ota_op_completion_cb_t cb, uint32_t param)
{
    otaResult_t                status = gOtaSuccess_c;
    union ota_op_completion_cb callback;
    callback.func = cb;

    FLASH_TransactionOp_t *pMsg;

    do
    {
        if (NULL == mHdl.FlashOps)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }

        if (size == 0U)
        {
            RAISE_ERROR(status, gOtaInvalidParam_c);
        }
        if (OTA_UsePostedOperation())
        {
            pMsg = OTA_FlashTransactionAlloc();
            if (pMsg == NULL)
            {
                assert(pMsg == NULL);
                RAISE_ERROR(status, gOtaError_c);
            }

            pMsg->flash_addr = mHdl.ErasedUntilOffset;
            pMsg->sz         = (int32_t)size;
            pMsg->op_type    = FLASH_OP_ERASE_NEXT_BLOCK;

            FLib_MemCpyWord(&pMsg->buf[0], &callback.pf);
            FLib_MemCpyWord(&pMsg->buf[4], &param);

            OTA_MsgQueue(pMsg);

            if (!mHdl.config->PostedOpInIdleTask)
            {
                /* Always take head of queue */
                (void)OTA_TransactionResume();
            }
        }
        else
        {
            /* Make Headroom for the synchronous execution case */
            ota_flash_status_t st;
            uint32_t *         p_erase_addr = &mHdl.ErasedUntilOffset;
            int32_t            remain_sz    = (int32_t)size;
            st                              = mHdl.FlashOps->eraseArea(p_erase_addr, &remain_sz, false);
            if (kStatus_OTA_Flash_Success == st)
            {
                if (callback.func != NULL)
                {
                    callback.func(param);
                }
            }
            else
            {
                mHdl.ErasedUntilOffset = 0;
                status                 = gOtaError_c;
            }
        }
    } while (false);

    return status;
}

/*****************************************************************************
 *  OTA_GetSelectedFlashAvailableSpace
 *
 *  return ota_partition->size if selected 0 otherwise.
 *
 *****************************************************************************/
uint32_t OTA_GetSelectedFlashAvailableSpace(void)
{
    uint32_t sz = 0;
    if (mHdl.ota_partition != NULL)
    {
        sz = mHdl.ota_partition->size;
    }
    return sz;
}

bool OTA_IsTransactionPending(void)
{
    /* When the op_queue size is 0 the list of pending operations is empty*/
    return LIST_GetSize(&mHdl.op_queue) != 0U ? true : false;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*****************************************************************************
 *  OTA_WritePendingData
 *
 *  Writes pending data buffer into OTA storage
 *
 *****************************************************************************/
static void OTA_WritePendingData(void)
{
    ota_flash_status_t status;
    if (OTA_UsePostedOperation())
    {
        FLASH_TransactionOp_t *pMsg = mHdl.cur_transaction;
        do
        {
            if ((pMsg != NULL) && (pMsg->sz != 0))
            {
                mHdl.cur_transaction = NULL;
                /* Submit transaction */
                OTA_MsgQueue(pMsg);
            }

            while (mHdl.FlashOps->isBusy() != 0U)
            {
            }

            /* Make sure to flush the entire posted ops queue */
            while (OTA_IsTransactionPending())
            {
                (void)OTA_TransactionResume();
                while (mHdl.FlashOps->isBusy() != 0U)
                {
                }
            }

        } while (false);
    }
    else
    {
        status = mHdl.FlashOps->flushWriteBuf();
        assert(status == kStatus_OTA_Flash_Success);
        (void)status;
    }

    mHdl.OtaImageLengthWritten = mHdl.OtaImageCurrentLength;
}

/*****************************************************************************
 *  OTA_UsePostedOperation
 *
 *  Tell if erase and writes to flash are blocking.
 *
 *****************************************************************************/
static bool OTA_UsePostedOperation(void)
{
    return (mHdl.PostedQ_capacity != 0U);
}

static otaResult_t OTA_PostWriteToFlash(uint16_t NoOfBytes, uint32_t Addr, uint8_t *pData)
{
    otaResult_t            status = gOtaSuccess_c;
    FLASH_TransactionOp_t *pMsg;
    uint8_t *              Outbuf;
    Outbuf = pData;
    do
    {
        if (mHdl.OtaImageLengthWritten > mHdl.OtaImageCurrentLength)
        {
            RAISE_ERROR(status, gOtaInvalidParam_c);
        }

        while (NoOfBytes > 0U)
        {
            uint8_t *p; /* write pointer to buffer */
            size_t   remaining_space;
            size_t   nb_bytes_copy;

            if (mHdl.cur_transaction != NULL)
            {
                pMsg = mHdl.cur_transaction;
                /* Current transaction was ongoing : continue filling it */
                remaining_space = PROGRAM_PAGE_SZ - (uint32_t)pMsg->sz;
                Addr += remaining_space;
            }
            else
            {
                pMsg = OTA_FlashTransactionAlloc();
                if (pMsg == NULL)
                {
                    assert(pMsg != NULL);
                    RAISE_ERROR(status, gOtaError_c);
                }
                pMsg->flash_addr = Addr;
                pMsg->op_type    = FLASH_OP_WRITE;
                pMsg->sz         = 0;
                remaining_space  = PROGRAM_PAGE_SZ;
            }
            p             = &pMsg->buf[pMsg->sz];
            nb_bytes_copy = MIN(remaining_space, NoOfBytes);
            FLib_MemCpy(p, Outbuf, nb_bytes_copy);
            Outbuf += nb_bytes_copy;
            pMsg->sz += (int16_t)nb_bytes_copy;
            if (pMsg->sz == (int16_t)PROGRAM_PAGE_SZ)
            {
                assert((pMsg->flash_addr % PROGRAM_PAGE_SZ) == 0);
                /* Submit transaction */
                OTA_MsgQueue(pMsg);
                if (mHdl.cur_transaction != NULL)
                {
                    mHdl.cur_transaction = NULL;
                }
                else
                {
                    Addr += PROGRAM_PAGE_SZ;
                }
            }
            else
            {
                mHdl.cur_transaction = pMsg;
            }
            NoOfBytes -= (uint16_t)nb_bytes_copy;
        }

        if ((!mHdl.config->PostedOpInIdleTask) && (OTA_IsTransactionPending()))
        {
            /* Always take head of queue */
            (void)OTA_TransactionResume();
        }
    } while (false);
    return status;
}

static FLASH_TransactionOp_t *OTA_FlashTransactionAlloc(void)
{
    FLASH_TransactionOp_t *    pTr = NULL;
    FLASH_TransactionOpNode_t *flash_transaction;
    list_element_handle_t      list_handle;
    void *                     ptr;
    OSA_DisableIRQGlobal();

    list_handle       = LIST_RemoveHead(&mHdl.transaction_free_list);
    ptr               = list_handle;
    flash_transaction = (FLASH_TransactionOpNode_t *)ptr;

    if (flash_transaction != NULL)
    {
        pTr = &flash_transaction->flash_transac;
        mHdl.PostedQ_nb_in_queue++;
    }
    OSA_EnableIRQGlobal();

    return pTr;
}

static void OTA_FlashTransactionFree(FLASH_TransactionOp_t *pTr)
{
    list_status_t         status;
    uint8_t *             flash_transaction;
    list_element_handle_t list_handle;
    OSA_DisableIRQGlobal();
    flash_transaction = ((uint8_t *)pTr - offsetof(FLASH_TransactionOpNode_t, flash_transac));
    mHdl.PostedQ_nb_in_queue--;
    list_handle = (list_element_handle_t)((uint32_t)flash_transaction);
    status      = LIST_AddTail(&mHdl.transaction_free_list, list_handle);
    assert(status == kLIST_Ok);
    (void)status;
    OSA_EnableIRQGlobal();
}

static void OTA_MsgQueue(FLASH_TransactionOp_t *pMsg)
{
    OSA_DisableIRQGlobal();
    (void)MSG_QueueAddTail(&mHdl.op_queue, pMsg);
    mHdl.q_sz++;
    if (mHdl.q_sz > mHdl.q_max)
    {
        mHdl.q_max = mHdl.q_sz;
    }
    OSA_EnableIRQGlobal();
}

static void OTA_MsgDequeue(void)
{
    OSA_DisableIRQGlobal();
    (void)MSG_QueueRemoveHead(&mHdl.op_queue);
    mHdl.q_sz--;
    OSA_EnableIRQGlobal();
}

/*****************************************************************************
 *  OTA_TransactionQueuePurge
 *
 *  Purge queue and abandon current posted operations
 *
 *****************************************************************************/
static int OTA_TransactionQueuePurge(void)
{
    int nb_purged = 0;
    while (OTA_IsTransactionPending())
    {
        FLASH_TransactionOp_t *pMsg = MSG_QueueGetHead(&mHdl.op_queue);
        if (pMsg == NULL)
        {
            break;
        }
        OTA_MsgDequeue();
        OTA_FlashTransactionFree(pMsg);
        nb_purged++;
    }

    if (mHdl.cur_transaction != NULL)
    {
        OTA_FlashTransactionFree(mHdl.cur_transaction);
        mHdl.cur_transaction = NULL;
    }

    return nb_purged;
}

/*****************************************************************************
 *  OTA_CheckVerifyFlash
 *
 *  Compare if flash contents matches that of RAM programmed buffer.
 *  Maybe be called when data are still held in accumulation write buffer.
 *
 *****************************************************************************/
static otaResult_t OTA_CheckVerifyFlash(uint8_t *pData, uint32_t flash_addr, uint16_t length)
{
    otaResult_t status                                = gOtaSuccess_c;
    uint8_t     readData[gOtaVerifyWriteBufferSize_d] = {0};
    uint16_t    readLen;
    uint16_t    i = 0;
    /* Not very easy to use when writes are partial,
    the actual size written differs : works only for posted operations */
    /* We iterate so as to keep the readData buffer reasonable in size */
    while (i < length)
    {
        ota_flash_status_t st;

        readLen = length - i;

        if (readLen > sizeof(readData))
        {
            readLen = (uint16_t)sizeof(readData);
        }
        st = mHdl.FlashOps->readData(readLen, flash_addr + i, readData);
        if (st != kStatus_OTA_Flash_Success)
        {
            RAISE_ERROR(status, gOtaExternalFlashError_c);
        }

        if (!FLib_MemCmp(&pData[i], readData, readLen))
        {
            RAISE_ERROR(status, gOtaExternalFlashError_c);
        }

        i += readLen;
    }
    assert(status == gOtaSuccess_c);
    return status;
}

static otaResult_t OTA_WriteToFlash(uint16_t NoOfBytes, uint32_t Addr, uint8_t *outbuf)
{
    otaResult_t status = gOtaSuccess_c;
    do
    {
        /* Try to write the data chunk into the image storage */
        if (mHdl.FlashOps->writeData(NoOfBytes, Addr, outbuf) != kStatus_OTA_Flash_Success)
        {
            RAISE_ERROR(status, gOtaExternalFlashError_c);
        }
        /* If Flash programming operation requires verification do it now
         */
        if (mHdl.VerifyWrites == true)
        {
            status = OTA_CheckVerifyFlash(outbuf, Addr, NoOfBytes);
        }
    } while (false);
    return status;
}

static ota_flash_status_t OTA_TreatFlashOpWrite(FLASH_TransactionOp_t *pMsg)
{
    ota_flash_status_t st;
    if (pMsg->sz < (int32_t)PROGRAM_PAGE_SZ) /* Should only happen at last chunk */
    {
        FLib_MemSet(&pMsg->buf[pMsg->sz], 0, PROGRAM_PAGE_SZ - ((uint32_t)pMsg->sz));
        /* Message buffer completed with 0 from pMsg-sz index to PROGRAM_PAGE_SZ
        new size is PROGRAM_PAGE_SZ */
        pMsg->sz = (int32_t)PROGRAM_PAGE_SZ;
    }
    if (OTA_WriteStorageMemory(&pMsg->buf[0], (uint16_t)pMsg->sz, pMsg->flash_addr) == gOtaSuccess_c)
    {
        mHdl.OtaImageLengthWritten += ((uint32_t)pMsg->sz);
        assert(mHdl.StorageAddressWritten == pMsg->flash_addr);
        mHdl.StorageAddressWritten += ((uint32_t)pMsg->sz);
        st = kStatus_OTA_Flash_Success;
    }
    else
    {
        OTA_WARNING_TRACE("Failed FlashOp %x @%08x sz=%d\r\n", pMsg->op_type, pMsg->flash_addr, pMsg->sz);
        st = kStatus_OTA_Flash_Error;
        assert(st == kStatus_OTA_Flash_Success);
    }
    OTA_MsgDequeue();
    OTA_FlashTransactionFree(pMsg);
    return st;
}

#if defined               DeprecatedOtaHasPostedEraseArea && (DeprecatedOtaHasPostedEraseArea > 0)
static ota_flash_status_t OTA_TreatFlashOpEraseArea(FLASH_TransactionOp_t *pMsg)
{
    ota_flash_status_t st;
    int32_t            remain_sz  = (int32_t)pMsg->sz;
    uint32_t           erase_addr = pMsg->flash_addr;

    st = mHdl.FlashOps->eraseArea(&erase_addr, &remain_sz, true);
    if (kStatus_OTA_Flash_Success == st)
    {
        if (remain_sz <= 0)
        {
            OTA_MsgDequeue();
            OTA_FlashTransactionFree(pMsg);
        }
        else
        {
            /* Leave head request in queue */
            pMsg->flash_addr = erase_addr;
            pMsg->sz         = (int32_t)remain_sz;
        }
    }
    else
    {
        OTA_WARNING_TRACE("Failed FlashOp %x @%08x sz=%d\r\n", pMsg->op_type, pMsg->flash_addr, pMsg->sz);
        assert(st == kStatus_OTA_Flash_Success);
    }
    return st;
}
#endif

static ota_flash_status_t OTA_TreatFlashOpEraseNextBlock(FLASH_TransactionOp_t *pMsg)
{
    ota_flash_status_t st;
    int32_t            remain_sz = (int32_t)pMsg->sz;
    st                           = mHdl.FlashOps->eraseArea(&pMsg->flash_addr, &remain_sz, false);
    if (kStatus_OTA_Flash_Success == st)
    {
        pMsg->op_type          = FLASH_OP_ERASE_NEXT_BLOCK_COMPLETE;
        mHdl.ErasedUntilOffset = pMsg->flash_addr;
    }
    else
    {
        OTA_WARNING_TRACE("Failed FlashOp %x @%08x sz=%d\r\n", pMsg->op_type, pMsg->flash_addr, pMsg->sz);
        assert(st == kStatus_OTA_Flash_Success);
    }
    return st;
}

static ota_flash_status_t OTA_TreatFlashOpEraseNextBlockComplete(FLASH_TransactionOp_t *pMsg)
{
    union ota_op_completion_cb cb;
    cb.func        = NULL;
    uint32_t param = 0U;

    FLib_MemCpyWord(&cb.pf, &(pMsg->buf[0]));

    if (cb.func != NULL)
    {
        FLib_MemCpyWord(&param, &(pMsg->buf[4]));
        cb.func(param);
    }
    OTA_MsgDequeue();
    OTA_FlashTransactionFree(pMsg);
    return kStatus_OTA_Flash_Success;
}

#if defined               DeprecatedOtaHasPostedEraseSector && (DeprecatedOtaHasPostedEraseSector > 0)
static ota_flash_status_t OTA_TreatFlashOpEraseSector(FLASH_TransactionOp_t *pMsg)
{
    ota_flash_status_t st;
    OTA_DEBUG_TRACE("Erase block @%08x sz=%d\r\n", pMsg->flash_addr, pMsg->sz);

    int32_t remain_sz = (int32_t)pMsg->sz;
    st                = mHdl.FlashOps->eraseArea(&pMsg->flash_addr, &remain_sz, true);
    if (kStatus_OTA_Flash_Success == st)
    {
        OTA_MsgDequeue();
        OTA_FlashTransactionFree(pMsg);
    }
    else
    {
        OTA_WARNING_TRACE("Failed FlashOp %x @%08x sz=%d\r\n", pMsg->op_type, pMsg->flash_addr, pMsg->sz);
        assert(st == kStatus_OTA_Flash_Success);
    }
    return st;
}
#endif
