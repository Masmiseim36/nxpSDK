/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the header file for the OTA Programming Support.
 *
 ** SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef _OTA_SUPPORT_H_
#define _OTA_SUPPORT_H_

#include "fwk_platform.h"
#include "fsl_component_generic_list.h"
#include "fwk_hal_macros.h"

/*!
 * @addtogroup OTA_module
 * The OTA_module
 *
 * OTA_module provides APIs a collection of over the air update features.
 * @{
 */
/*!
 * @addtogroup OTA_support
 * The OTA main module
 *
 * OTA_support provides APIs a collection of over the air update features.
 * @{
 */

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/*!< gOtaErasePolicyOnTheFly_c sector erasure interleaved with programming operations */
#define gOtaEraseOnTheFly_c 0

/*!< gOtaEraseBeforeImageBlockReq_c erase enough headroom for the BlockImage size requested */
#define gOtaEraseBeforeImageBlockReq_c 1

/*!< Select gOtaErasePolicy_c for efficiency */
#define gOtaErasePolicy_c gOtaEraseBeforeImageBlockReq_c

#ifndef gOtaVerifyWrite_d
#define gOtaVerifyWrite_d 1
#endif

/*!< The internal flash has a program page of 128 whereas the external flash has 256 byte pages
 * 256 is a good compromise.
 */
#define PROGRAM_PAGE_SZ 256U

/*!< Transaction size in posted_ops_storage must match sizeof(FLASH_TransactionOpNode_t)*/
#define gOtaTransactionSz_d (20U + PROGRAM_PAGE_SZ)

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

typedef enum
{
    gOtaSuccess_c = 0,
    gOtaNoImage_c,
    gOtaUpdated_c,
    gOtaError_c,
    gOtaCrcError_c,
    gOtaInvalidParam_c,
    gOtaInvalidOperation_c,
    gOtaExternalFlashError_c,
    gOtaInternalFlashError_c,
    gOtaImageTooLarge_c,
    gOtaImageInvalid_c,
} otaResult_t;

/*! Prototype of ota_completion callback */
typedef void (*ota_op_completion_cb_t)(uint32_t param);

typedef struct
{
    /*!< Only resume posted operations in Idle Task */
    bool PostedOpInIdleTask;
    /*!< Number of transactions processed when calling OTA_TransactionResume */
    int maxConsecutiveTransactions;
} ota_config_t;

/*
 The state transitions described below concern the
 The MCU bootloader writes fields in the active and candidate image trailers.
 The combination of the trailer states allows to determine the image state and passes
 information back and forth between the bootloader and the application to handle firmware
 update.


                    ----------------------------------------------------------------
                    |                                                               |
        /---------------------------\                                               |
       /    Bootloader image state   \___                                           |
       \          test               /  |         if                                |
        \ --------------------------/   | OtaImgState_CandidateRdy                  |
           if        |                  |                                           |
    OtaImgState None |                  |                                           |
     or              |      ---------------------------                             |
    Permanent        |      |        go to             |                            |
                     |      | OtaImgState_RunCandidate |                            |
                     |      ----------------------------                            |
                     |                   |                                          |
                     |                   -----------                                |
                     |                             |                                |
        -------------v------------     ------------v-------------                   |
        | OtaImgState_None       |     |    Run candidate       |                   | R
        --------------------------     --------------------------                   | E
                        |                               |                           | S
                        | OTA_StartImage    ------------v-------------              | E
                        v                   |        go to           |              | T
             ---------------------------    | OtaImgState_Permanent  |              |
             |  OtaImgState_Acquiring  |    --------------------------              |
             ---------------------------               |                            |
                        |                              ---------------------------->|
                        | OTA_CommitImage                                           |
                        v                                                           |
             ---------------------------                                            |
             | OtaImgState_CandidateRdy|                                            |
             ---------------------------                                            |
                        |                                                           |
                        -------------------------------------------------------------
*/

typedef enum
{
    OtaImgState_None,      /*!< Default value when there is no upgradable image and current image was made permanent */
    OtaImgState_Acquiring, /*!< Acquisition of new firmware image is ongoing */
    OtaImgState_CandidateRdy, /*!< Acquisition complete. It denotes that a candidate image is fully loaded.
                               * The application has to switch to this state when finishing the update operation.
                               * This state is transient and the bootloader is expected to change it to
                               * OtaImgState_RunCandidate if the image verification criteria pass.
                               */
    OtaImgState_RunCandidate, /*!< The bootloader needs to switch to this state before allowing the the test image
                               * to run. From this state, the application may decide to revert from the candidate image
                               * to the original one, or make it permanent.
                               */
    OtaImgState_Permanent,    /*!< The application needs to switch to this state when the self-test is okay.
                               * This state can only exist if the OTA system provides a revert/confirmation mechanism
                               * such as that of MCUBOOT. When conplete, the image state become OtaImgState_None again
                               * (allowing a    new OTA).
                               */
    OtaImgState_Fail,
    OtaImgState_Max,
} OtaImgState_t;

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
/*! *********************************************************************************
 * \brief  Discover state of running image.
 *
 * \return
 *  - gOtaSuccess_c if procedure Ok. gOtaError_c otherwise
 *
 * Note: Applications running in conjunction with MCUBOOT perform this as part of Self_test
 *
 ********************************************************************************** */
otaResult_t OTA_Initialize(void);

/*! *********************************************************************************
 * \brief  Starts the process of writing a new image to the selected OTA storage.
 *
 * \param[in] length: the length of the image to be written.
 *
 * \return
 *  - gOtaInvalidParam_c: the intended length is bigger than the FLASH capacity
 *  - gOtaInvalidOperation_c: the process is already started (can be cancelled)
 *  - gOtaEepromError_c: can not detect external EEPROM
 *
 ********************************************************************************** */
otaResult_t OTA_StartImage(uint32_t length);

/*! *********************************************************************************
 * \brief  Places the next image chunk into the selected OTA storage. The CRC will not be computed.
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
otaResult_t OTA_PushImageChunk(uint8_t *pData, uint16_t length, uint32_t *pImageLength, uint32_t *pImageOffset);

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
otaResult_t OTA_PullImageChunk(uint8_t *pData, uint16_t length, uint32_t *pImageOffset);

/*! *********************************************************************************
 * \brief  Finishes the writing of a new image to the permanent storage.
 *         It will write the image header (signature and length) and footer (sector copy bitmap).
 *
 * \param[in] pBitmap  pointer to a byte array indicating the sector erase pattern for the
 *                     internal FLASH before the image update.
 *
 * \return
 *  - gOtaInvalidOperation_c: the process is not started,
 *  - gOtaEepromError_c: error while trying to write the EEPROM
 *
 ********************************************************************************** */
otaResult_t OTA_CommitImage(uint8_t *pBitmap);

/*! *********************************************************************************
 * \brief  Cancels the process of writing a new image to the selected OTA storage.
 *
 ********************************************************************************** */
void OTA_CancelImage(void);

/*! *********************************************************************************
 * \brief  Set the boot flags, to trigger the Bootloader at the next CPU reset.
 *
 ********************************************************************************** */
void OTA_SetNewImageFlag(void);

/*! *********************************************************************************
 * \brief  Set the boot flags, to trigger the Bootloader at the next CPU reset.
 *
 * \param[in] offset specify an offset to determine image address
 *
 ********************************************************************************** */
void OTA_SetNewImageFlagWithOffset(uint32_t offset);

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
otaResult_t OTA_ReadStorageMemory(uint8_t *pData, uint16_t length, uint32_t address);

/*! *********************************************************************************
 * \brief  Write into the image storage (external memory or internal flash)
 *
 * \param[in] pData    pointer to the data chunk
 * \param[in] length   the length of the data chunk
 * \param[in] address  image storage address
 *
 * \return  error code.
 *
 ********************************************************************************** */
otaResult_t OTA_WriteStorageMemory(uint8_t *pData, uint16_t length, uint32_t address);

/*! *********************************************************************************
 * \brief  Compute CRC over a data chunk.
 *
 * \param[in] pData        pointer to the data chunk
 * \param[in] lenData      the length of the data chunk
 * \param[in] crcValueOld  current CRC value
 *
 * \return  computed CRC.
 *
 ********************************************************************************** */
uint16_t OTA_CrcCompute(uint8_t *pData, uint16_t lenData, uint16_t crcValueOld);

/*! *********************************************************************************
 * \brief  This function is called in order to erase the entire image storage
 *         (external memory or internal flash)
 *
 * \return  computed CRC.
 *
 ********************************************************************************** */
otaResult_t OTA_EraseStorageMemory(void);

/*! *********************************************************************************
 * \brief  Selects Internal Storage for OTA
 *
 *  Offset and size of the partition must be set via PLATFORM_OtaGetOtaPartitionConfig.
 * \return  error code.
 *
 ********************************************************************************** */
otaResult_t OTA_SelectInternalStoragePartition(void);

/*! *********************************************************************************
 * \brief  Selects External for OTA
 *
 *  Offset and size of the partition must be set via PLATFORM_OtaGetOtaPartitionConfig.
 *
 * \return  error code.
 *
 ********************************************************************************** */
otaResult_t OTA_SelectExternalStoragePartition(void);

/*! *********************************************************************************
 * \brief  Attempt to resume OTA from some polling task (Idle Task preferably)
 *
 * \return  Number of transactions treated.
 *
 ********************************************************************************** */
int OTA_TransactionResume(void);

/*! *********************************************************************************
 * \brief  Start OTA service - Discover state of running image.
 *
 * \param[in] posted_ops_storage   pointer to the buffer used for transaction storage.
 *            Ideally application allocates a dynamic buffer to allow
 *            allocation of transactions. Between 4 and 8 transaction
 *            buffers are required to prevent stalling.
 *    @warning posted_ops_storage shall be 4 bytes aligned
 * \param[in] posted_ops_sz         size in bytes of the transaction buffer.
 *
 * If posted_ops_storage is NULL and posted_ops_sz is 0, the direct operation mode is opted
 * (no deferred flash transactions)
 *
 * \return
 *  - gOtaSuccess_c if procedure Ok. gOtaError_c otherwise
 *
 *  Note: Transaction size should be set to gOtaTransactionSz_d (sizeof(FLASH_TransactionOpNode_t)).
 *
 ********************************************************************************** */
otaResult_t OTA_ServiceInit(void *posted_ops_storage, size_t posted_ops_sz);

/*! *********************************************************************************
 * \brief  Suppress all transactions pending in storage
 *
 * \return  error code 0.
 *
 ********************************************************************************** */
otaResult_t OTA_ServiceDeInit(void);

/*! *********************************************************************************
* \brief  Get the default configuration of OTA setting

* \param[in] userConfig             pointer to the configuration structure
*
********************************************************************************** */
void OTA_GetDefaultConfig(ota_config_t *userConfig);

/*! *********************************************************************************
* \brief  Set the configuration of OTA setting

* \param[in] userConfig             pointer to the configuration structure
*
********************************************************************************** */
void OTA_SetConfig(ota_config_t *userConfig);

/*! *********************************************************************************
* \brief  Erase in advance enough space to receive an Image Block

* \param [in] size  block size to prepare
* \param [in] cb    callback function to call on completion of erase operation
* \param [in] param callback parameter (not really used)
*
* \return  error code 0.
*
********************************************************************************** */
otaResult_t OTA_MakeHeadRoomForNextBlock(uint32_t size, ota_op_completion_cb_t cb, uint32_t param);

/*! *********************************************************************************
 * \brief  Return available size of current OTA storage
 *
 * \return  0 if not initialized, total size otherwise.
 *
 ********************************************************************************** */
uint32_t OTA_GetSelectedFlashAvailableSpace(void);

/*! *********************************************************************************
 * \brief  Return if there is a pending
 *
 * \return  bool pending transaction or not.
 *
 ********************************************************************************** */
bool OTA_IsTransactionPending(void);

/*! *********************************************************************************
 * \brief  Determine Image state in order to determine whether it requires to be made
 *         permanent or reverted. Only works in conjunction MCUBOOT or devices having a
 *         hardware remap capability.
 *
 * \return  gOtaSuccess_c if operation correctly executed. Denotes error in all other cases.
 *          OtaImgState_None is image is permanent
 *          OtaImgState_Permanent is normally only transient in MCUBOOT
 *
 ********************************************************************************** */
OtaImgState_t OTA_GetImgState(void);

/*! *********************************************************************************
 * \brief  After acquiring an firmware update image, update image image state to OtaImgState_CandidateRdy
 *          before reset and handing it to MCUBOOT to apply the image.
 * \param [in] new_state  state to which we intend to go to.
 *
 * \return  gOtaSuccess_c if successful, all other statuses denote errors.
 *
 ********************************************************************************** */
otaResult_t OTA_UpdateImgState(OtaImgState_t new_state);

#ifdef __cplusplus
}
#endif

/*!
 * @}  end of OTA_support addtogroup
 */
/*!
 * @}  end of OTA_module addtogroup
 */

#endif /* _OTA_SUPPORT_H_ */
