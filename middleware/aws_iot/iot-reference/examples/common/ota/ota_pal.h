/*
 * FreeRTOS version 202107.00-LTS
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */


/**
 * @brief Header file containing platform abstraction layer APIS for OTA update.
 */

#ifndef OTA_PAL_H
#define OTA_PAL_H

#include "ota.h"

/**
 * @brief Retrieve the current firmware image state from flash.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @return Appropriate state of the firmware image.
 */
OtaPalImageState_t xOtaPalGetPlatformImageState( OtaFileContext_t * const pFileContext );

/**
 * @brief Set the current firmware image state in flash.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @param[in] eState State to be set for the firmware image.
 * @return OtaPalSuccess if successful, else OTA error code along with detailed PAL error code.
 */
OtaPalStatus_t xOtaPalSetPlatformImageState( OtaFileContext_t * const pFileContext,
                                             OtaImageState_t eState );

/**
 * @brief Resets the device with the current firmware image.
 * The API can be invoked when aborting the current OTA update to fall back to old image or while activating
 * new image after a successful update.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @return OtaPalSuccess if successful, else OTA error code along with detailed PAL error code.
 */
OtaPalStatus_t xOtaPalResetDevice( OtaFileContext_t * const pFileContext );

/**
 * @brief Activates the device with the new firmware image.
 * The API should prepare the new image to be booted up and reset the device to boot up with the new image.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @return OtaPalSuccess if successful, else OTA error code along with detailed PAL error code.
 */
OtaPalStatus_t xOtaPalActivateNewImage( OtaFileContext_t * const pFileContext );

/**
 * @brief Writes a block of the firmware image to flash.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @param[in] offset Offset in bytes of the block with in the whole firmware image
 * @param[in] pData Pointer to buffer containing the block.
 * @param[in] blockSize Size of the block
 * @return Number of bytes of block written or < 0 if there is an error.
 */
int16_t xOtaPalWriteBlock( OtaFileContext_t * const pFileContext,
                           uint32_t offset,
                           uint8_t * const pData,
                           uint32_t blockSize );

/**
 * @brief Closes the firmware image after reading or writing.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @return OtaPalSuccess if successful, else OTA error code along with detailed PAL error code.
 */
OtaPalStatus_t xOtaPalCloseFile( OtaFileContext_t * const pFileContext );

/**
 * @brief Prepares a new firmware image to be written.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @return OtaPalSuccess if successful, else OTA error code along with detailed PAL error code.
 */
OtaPalStatus_t xOtaPalCreateFileForRx( OtaFileContext_t * const pFileContext );

/**
 * @brief Aborts the current firmware image being written.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @return OtaPalSuccess if successful, else OTA error code along with detailed PAL error code.
 */
OtaPalStatus_t xOtaPalAbort( OtaFileContext_t * const pFileContext );

/**
 * @brief Opens the firmware image for reading
 * The API is used to verify the image signature after it has been written.
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @return OtaPalSuccess if successful, else OTA error code along with detailed PAL error code.
 */
OtaPalStatus_t xOtaPalOpenFileForRead( OtaFileContext_t * const pContext );

/**
 * @brief Reads a block of firmware image from flash
 *
 * @param[in] pFileContext Pointer to a context containing firmware image details.
 * @param[in] offset Offset in bytes of the block with in the whole firmware image
 * @param[in] pData Pointer to buffer where block is read to.
 * @param[in] blockSize Size of the buffer
 * @return Number of bytes of block read or < 0 if there is an error.
 */
int32_t xOtaPalReadBlock( OtaFileContext_t * const pContext,
                          uint32_t offset,
                          uint8_t * pData,
                          uint16_t blockSize );

OtaPalStatus_t xFlashPalValidateSignature( uint8_t * pMappedAddress,
                                           size_t mappedLength,
                                           char * pCertificatePath,
                                           uint8_t * pSignature,
                                           size_t signatureLength );

#endif /* OTA_PAL_H */
