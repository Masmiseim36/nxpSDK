/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sbloader/sbloader.h"
#include "bootloader/bl_context.h"
#include "bootloader/bootloader.h"
#include "bootloader/bl_shutdown_cleanup.h"
#if defined(BL_FEATURE_ENCRYPTION) && BL_FEATURE_ENCRYPTION
#include "aes_security.h"
#include "cbc_mac.h"
#endif // BL_FEATURE_ENCRYPTION
#include "utilities/fsl_assert.h"
#include "utilities/fsl_rtos_abstraction.h"

#ifdef DEBUG
#include "property/property.h"
#endif

#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
#include "qspi.h"
#endif
#if defined(BL_FEATURE_OTFAD_MODULE) && BL_FEATURE_OTFAD_MODULE
#include "fsl_otfad_driver.h"
#endif

#if defined(BL_FEATURE_OCOTP_MODULE) && BL_FEATURE_OCOTP_MODULE
#include "ocotp/fsl_ocotp.h"
#endif

//! @addtogroup sbloader
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define SB_KEY_RDONCE_INDEX 0x30u

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
typedef struct _ldr_buf ldr_buf_t;

struct _ldr_buf
{
    chunk_t data;
    uint32_t fillPosition;
};

//! Loader utilities.
status_t ldr_GoToNextSection(ldr_Context_t *context);
status_t sbloader_handle_chunk(void);

//! Loader action (context.Action) functions for header processing.
status_t ldr_DoHeader(ldr_Context_t *context);
status_t ldr_DoHeader1(ldr_Context_t *context);
status_t ldr_DoHeader2(ldr_Context_t *context);
status_t ldr_DoHeaderMac(ldr_Context_t *context);
status_t ldr_DoKeyTest(ldr_Context_t *context);
status_t ldr_DoGetDek(ldr_Context_t *context);

//! Loader action (context.Action) functions for command processing.
status_t ldr_DoCommand(ldr_Context_t *context);
status_t ldr_DoLoadBytes(ldr_Context_t *context);
status_t ldr_DoLoadChunks(ldr_Context_t *context);
status_t ldr_DoLoadCmd(ldr_Context_t *context);
status_t ldr_DoFillCmd(ldr_Context_t *context);
status_t ldr_DoJumpCmd(ldr_Context_t *context);
status_t ldr_DoCallCmd(ldr_Context_t *context);
status_t ldr_DoTagCmd(ldr_Context_t *context);
status_t ldr_DoEraseCmd(ldr_Context_t *context);
status_t ldr_DoResetCmd(ldr_Context_t *context);
status_t ldr_DoMemEnableCmd(ldr_Context_t *context);
status_t ldr_DoProgramCmd(ldr_Context_t *context);

////////////////////////////////////////////////////////////////////////////////
// Data
////////////////////////////////////////////////////////////////////////////////

//! static loader context data.
static ldr_Context_t s_loaderContext;

//! static buffer queueing up a chunk worth of data
static ldr_buf_t s_loaderBuf;

#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
//! Pointer for the aes which will for now be a null pointer unless in debug mode
//! we will try to read from the config area
static uint32_t s_aesKey[AES_128_KEY_SIZE_WORDS];
#endif

////////////////////////////////////////////////////////////////////////////////
//! \brief  Skip ahead to the next "tag" command
//!
//! \This function will set up the skipCount to skip over the remainder of any section
//! \that you are currently in. If encryption is enabled it will also re-init the encryption
//! \process to begin on the new section it arrives at.
//!
//! \retval kStatus_Success
//! \retval kStatusRomLdrEOFReached if the end of the image file is reached.
////////////////////////////////////////////////////////////////////////////////
status_t ldr_GoToNextSection(ldr_Context_t *context)
{
    status_t status = (int32_t)kStatus_Success;

    // If we are not in the last section of the image file, setup to get a
    // "tag" command then skip the remaining chunks in the section. Otherwise,
    // return an error.
    if (context->fileChunks > context->sectChunks)
    {
        // Skip over any remaining chunks in section, making sure to adjust
        // the file chunk count.
        context->fileChunks -= context->sectChunks;
        context->skipCount = context->sectChunks;

        // Setup the loader context to get the next command
        context->Action = ldr_DoCommand;

// Reinit the encryption for the start of a new section
#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
        if (context->keyCount)
        {
            cbc_mac_init(context->dek, context->initVector);
        }
#endif

        // Assume for now the new section has at least one chunk
        context->sectChunks = 1;

        status = (int32_t)kStatus_Success;
    }
    else
    {
        // No where to skip to, so return an error
        status = (int32_t)kStatusRomLdrEOFReached;
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader action function processes the first header chunk
//!
//! Processes the first header chunk of the boot image file, which
//! saves the SHA-1 to the context initVector field, the CBC-MAC process is
//! started here to handle the DEK if one is found
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->src points at the current header chunk.
//!
//! \post   context->Action is set to process the next header chunk.
//!
//! \retval SUCCESS
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoHeader(ldr_Context_t *context)
{
#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
    // Start the cbc-mac decode process which we may not need but won't know until we get
    // to the keyCount variable in the header
    cbc_mac_init((uint8_t *)&s_aesKey, 0);

    // The contents of header chunk[0] are used as the initialization vector
    // for the CBC decryption process. Save this value in the loader context.
    (void)memcpy(context->initVector, context->src, sizeof(chunk_t));

    // Feed the cbc mac the received data, and store the data in the DEK
    cbc_mac_encrypt(context->src, sizeof(chunk_t), context->dek);
#endif

    // Setup to process the next header chunk
    context->Action = ldr_DoHeader1;

    return (int32_t)kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader action function processes header chunk 1
//!
//! Processes the second header chunk of the boot image file, which contains
//! the file signature, version, flags and chunk count.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->src points at the current header chunk.
//!
//! \post   context->Action is set to process the next header chunk.
//! \post   context->fileChunks and fileFlags are set to the header values.
//!
//! \retval kStatus_Success
//! \retval kStatusRomLdrSignature if the signature or version are incorrect.
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoHeader1(ldr_Context_t *context)
{
    union
    {
        uint8_t *pSrc;
        boot_hdr1_t *pHdr1;
    } header;

    header.pSrc = context->src;

    status_t status = (int32_t)kStatus_Success;

    // Copy the file flags and chunk count
    context->fileFlags = header.pHdr1->fileFlags;
    context->fileChunks = header.pHdr1->fileChunks - 2u;

    // Check the file signature and version
    if ((header.pHdr1->signature != (uint32_t)BOOT_SIGNATURE) || (header.pHdr1->major > (uint8_t)SB_FILE_MAJOR_VERSION))
    {
        status = (int32_t)kStatusRomLdrSignature;
    }
    else
    {
#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
        // Feed the cbc mac the received data
        cbc_mac_encrypt(context->src, sizeof(chunk_t), context->dek);
#endif

        // Setup to process the next header chunk
        context->Action = ldr_DoHeader2;

        status = (int32_t)kStatus_Success;
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader action function processes header chunk 2
//!
//! Processes the third header chunk of the boot image file, which contains
//! parameters for the first boot section and the key dictionary.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->src points at the current header chunk.
//!
//! \post   context->Action is set to process the next header chunk.
//! \post   context->sectChunks, objectID and keyCount are set to the header values.
//!
//! \retval kStatus_Success
//! \retval kStatusRomLdrSectionLength if the bootOffset is out of range.
//! \retval kStatusRomLdrUnencryptedOnly if the unencrypted image is disabled.
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoHeader2(ldr_Context_t *context)
{
    union
    {
        uint8_t *pSrc;
        boot_hdr2_t *pHdr2;
    } header;

    header.pSrc = context->src;

    status_t status = (int32_t)kStatus_Success;

    // Save first boot section parameters
    context->objectID = header.pHdr2->bootSectID;
    // Set up our first sectChunks value since we can determine it now
    context->sectChunks = header.pHdr2->bootOffset - 3u;

    // Sanity check the section chunk count
    if (context->sectChunks >= context->fileChunks)
    {
        status = (int32_t)kStatusRomLdrSectionLength;
    }
    else
    {
        // Check whether the image is encrypted
        if (header.pHdr2->keyCount != 0u)
        {
#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
            // Save the key count for the dictionary search
            context->keyCount = header.pHdr2->keyCount;

            // Use bootCmd.count as temporary storage to hold the number of chunks
            // remaining in the plaintext header
            context->bootCmd.count = header.pHdr2->keyOffset - 3u;

            // Set action to finish calculation of CBC MAC over the header
            context->Action = ldr_DoHeaderMac;

            // Feed the cbc mac the received data
            cbc_mac_encrypt(context->src, sizeof(chunk_t), context->dek);

            status = (int32_t)kStatus_Success;
#else
        debug_printf("Error: image is encrypted\r\n");
            status = (int32_t)kStatusRomLdrUnencryptedOnly;
#endif
        }
        else
        {
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
#if !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
            ftfx_security_state_t securityState = kFTFx_SecurityStateNotSecure;
            // If the image is not encrypted and we have flash security enabled
            // abort the transfer
            // Note: Both Main and Secondary flash share the same security state
            //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
            (void)g_bootloaderContext.flashDriverInterface->flash_get_security_state(
                g_bootloaderContext.allFlashState, &securityState);

            if (securityState != kFTFx_SecurityStateNotSecure)
            {
                status = (int32_t)kStatusRomLdrSecureOnly;
            }
            else
#endif // !BL_DEVICE_IS_LPC_SERIES
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
            // Skip the rest of the header
            {
                status = ldr_GoToNextSection(context);
            }
        }
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader action function finishes CBC-MAC over final header chunks
//!
//! Completes the CBC-MAC calculation over the remaining plaintext header.
//! This function will only get called if CBC_MAC is supported
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->src points at the current header chunk.
//! \pre    context->bootCmd.count = number of chunks left in plaintext header.
//!
//! \post   context->scratchPad contains the computed CBC-MAC.
//! \post   context->bootCmd.count is updated.
//!
//! \retval SUCCESS
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoHeaderMac(ldr_Context_t *context)
{
    // Continue to compute the CBC-MAC over the remaining plaintext header
    // one chunk at a time.
    --context->sectChunks;

#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
    // Feed the cbc mac the received data
    cbc_mac_encrypt(context->src, sizeof(chunk_t), context->dek);
#endif

    if (--(context->bootCmd.count) == 0u)
    {
        context->Action = ldr_DoKeyTest;
    }

    return (int32_t)kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader action function processes a key dictionary match
//!
//! This loader action function is one of three functions used to parse the
//! key dictionary. It is called after a match of the CBC-MAC is found and the
//! session key (DEK) is decrypted. It saves the DEK in the boot context and
//! then skips to the first boot command in the image.
//! This function will only get called if CBC_MAC is supported
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->src points to the the encrypted session key.
//! \pre    context->initVector contains the SHA-1 chunk from the header
//!
//! \post   *context is setup to decrypt the first boot command.
//! \post   The session key is decrypted and stored in the context dek field
//!
//! \return Return value from ldr_GoToNextSection().
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoGetDek(ldr_Context_t *context)
{
    --context->sectChunks;

#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
    // Use the OTP key and the init vector from the header
    cbc_mac_init((uint8_t *)&s_aesKey, context->initVector);

    // decrypt the dek
    cbc_mac_decrypt(context->src, sizeof(chunk_t), context->dek);
#endif

    // Skip ahead to the next section
    return ldr_GoToNextSection(context);
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader action function tests for a key dictionary match
//!
//! This loader action function is one of three functions used to parse the
//! key dictionary. It is called after a CBC-MAC entry from the dictionary
//! has been read. If the value read is equal to the computed CBC-MAC, then
//! the next encrypted chunk holds the session key (DEK), and the next action
//! function will decrypt the key. Otherwise if there are remaining entries
//! in the dictionary, the next action will get the next CBC-MAC to test.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->dek holds the CBC-MAC computed over the header.
//! \pre    context->src holds a CBC-MAC value from the dictionary.
//!
//! \post   if CBC-MAC matches we transition to decrypt the DEK
//! \post   context->keyCount is updated to the remaining number of keys to test.
//!
//! \retval SUCCESS
//! \retval ERROR_ROM_LDR_KEY_NOT_FOUND if there are no more entries in the
//!             dictionary and a CBC-MAC match was not found.
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoKeyTest(ldr_Context_t *context)
{
    status_t rc = (int32_t)kStatus_Success;

    --context->sectChunks;

    // See if the computed CBC-MAC and the dictionary entry match
    if (memcmp(context->dek, context->src, sizeof(chunk_t)) == 0)
    {
        // Yes, the next chunk holds the session key. The next action will
        // load the key and terminate the key search.
        context->Action = ldr_DoGetDek;
    }
    else if (--(context->keyCount) > 0u)
    {
        // No, but there are more entries in the dictionary, so go get the
        // next one to test. Skip over the next chunk which is the unmatching DEK
        --context->sectChunks;
        context->skipCount = 1u;
    }
    else
    {
        // No, and there are no more entries in the dictionary.
        rc = (int32_t)kStatusRomLdrKeyNotFound;
    }

    return rc;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader action function loads the trailing "ragged edge bytes"
//!
//! Implements the second of two action functions for the boot "load" command.
//! The last move sequence for the load command moves the final
//! payload chunk. This function checks the payload
//! CRC, then copies the final load bytes to the load
//! destination.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//! \pre    context->src contains the final decrypted payload chunk.
//! \pre    context->crc32 holds the CRC calculated over the entire payload.
//!
//! \post   *context is setup to get the next boot command.
//!
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoLoadBytes(ldr_Context_t *context)
{
    uint32_t crc32Result;

// Copy the trailing edge payload bytes to the load destination.
#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
    if (context->keyCount)
    {
        // decrypt the block in place
        cbc_mac_decrypt(context->src, sizeof(chunk_t), context->src);
    }
#endif
    uint32_t memoryId = MAKE_MEMORYID((context->bootCmd.flags & (uint32_t)ROM_MEM_GROUP_ID_MASK) >> (uint32_t)ROM_MEM_GROUP_ID_SHIFT,
                                      (context->bootCmd.flags & (uint32_t)ROM_MEM_DEVICE_ID_MASK) >> (uint32_t)ROM_MEM_DEVICE_ID_SHIFT);
    status_t status = g_bootloaderContext.memoryInterface->write(context->bootCmd.address, context->bootCmd.count,
                                                                 context->src, memoryId);

    if (status == (int32_t)kStatus_Success)
    {
        // Force to write buffered data to target memory if it is last data
        // chunks without reset/call/jump command at the end
        assert(g_bootloaderContext.memoryInterface->flush);
        status = g_bootloaderContext.memoryInterface->flush();
        if (status == (int32_t)kStatus_Success)
        {
            // update the crc running value then finalize
            crc32_update(&context->crc32, context->src, sizeof(chunk_t));

            crc32_finalize(&context->crc32, &crc32Result);

            if (crc32Result != context->bootCmd.data)
            {
                status = (int32_t)kStatusRomLdrCrc32Error;
            }
            else
            {
                // Setup to get the next boot command.
                context->Action = ldr_DoCommand;
            }
        }
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader action function loads data chunks
//!
//! Implements the first of two action functions for the boot "load" command.
//! Loads one or more complete "chunk size" blocks of data.
//!
//! If the amount of data remaining to load is one chunk or less, it sets the
//! state sequence to move the next chunk. The
//! action function is changed to \ref ldr_DoLoadBytes, which will check the
//! payload CRC and copy to the correct location.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \post   context->bootCmd is adjusted to reflect the previous move.
//! \post   context->Action is set to move the next received payload.
//!
//! \retval kStatus_Success
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoLoadChunks(ldr_Context_t *context)
{
    uint32_t crc32Result;

#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
    if (context->keyCount)
    {
        // decrypt the block in place
        cbc_mac_decrypt(context->src, sizeof(chunk_t), context->src);
    }
#endif

    // update the crc running value
    crc32_update(&context->crc32, context->src, sizeof(chunk_t));

    uint32_t memoryId = MAKE_MEMORYID((context->bootCmd.flags & (uint32_t)ROM_MEM_GROUP_ID_MASK) >> (uint32_t)ROM_MEM_GROUP_ID_SHIFT,
                                      (context->bootCmd.flags & (uint32_t)ROM_MEM_DEVICE_ID_MASK) >> (uint32_t)ROM_MEM_DEVICE_ID_SHIFT);

    status_t status =
        g_bootloaderContext.memoryInterface->write(context->bootCmd.address, sizeof(chunk_t), context->src, memoryId);
    if (status == (int32_t)kStatus_Success)
    {
        // Adjust the boot command parameters to reflect the last move
        context->bootCmd.address += sizeof(chunk_t);
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH
        const memory_map_entry_t *mapEntry; 
        status = find_map_entry((context->bootCmd.address - sizeof(chunk_t)), sizeof(chunk_t), &mapEntry);
        if (status == (int32_t)kStatus_Success)
        {
#if defined(BL_FEATURE_SUPPORT_PFLASH_DFLASH_CROSS_WRITE) && BL_FEATURE_SUPPORT_PFLASH_DFLASH_CROSS_WRITE
            if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
            {      
                if ((mapEntry->memoryProperty == ((uint32_t)kMemoryIsExecutable | (uint32_t)kMemoryType_FLASH)) && (context->bootCmd.address  > mapEntry->endAddress))
                { 
                    assert(g_bootloaderContext.memoryInterface->flush);
                    (void)g_bootloaderContext.memoryInterface->flush(); 
                    context->bootCmd.address = (uint32_t)FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS;
                }
            }
#endif  // BL_FEATURE_SUPPORT_PFLASH_DFLASH_CROSS_WRITE         
        }
#endif // BL_FEATURE_SUPPORT_DFLASH              
    
        context->bootCmd.count -= sizeof(chunk_t);
        if (context->bootCmd.count > sizeof(chunk_t))
        {
            context->Action = ldr_DoLoadChunks;
        }
        else if (context->bootCmd.count != 0u)
        {
            context->Action = ldr_DoLoadBytes;
        }
        else
        {
            // Force to write buffered data to target memory
            assert(g_bootloaderContext.memoryInterface->flush);
            status = g_bootloaderContext.memoryInterface->flush();
            if (status == (int32_t)kStatus_Success)
            {
                // The last amount of data we received was a full chunk so get our crc result
                crc32_finalize(&context->crc32, &crc32Result);

                if (crc32Result != context->bootCmd.data)
                {
                    status = (int32_t)kStatusRomLdrCrc32Error;
                }
                else
                {
                    context->Action = ldr_DoCommand;
                }
            }
        }
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "load" command handler
//!
//! Implements the first command handler called for the boot "load" command.
//! The load action is split into two functions:
//! - \ref ldr_DoLoadChunks, loads chunks
//! - \ref ldr_DoLoadBytes, loads the trailing "ragged edge bytes"
//! This function initializes the context before calling load action.
//!
//! \param[in]  context    Pointer to a loader context structure.
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoLoadCmd(ldr_Context_t *context)
{
    // initialize crc32
    crc32_init(&context->crc32);

    // Next chunk will be the first chunk to load, if its a full chunk do load chunks
    if (context->bootCmd.count > sizeof(chunk_t))
    {
        context->Action = ldr_DoLoadChunks;
    }
    else
    {
        context->Action = ldr_DoLoadBytes;
    }

    return (int32_t)kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "fill" command handler
//!
//! Implements the action function for the boot "fill" command.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \post   *context is left setup to get the next boot command.
//!
//! \retval kStatus_Success
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoFillCmd(ldr_Context_t *context)
{
    status_t status = g_bootloaderContext.memoryInterface->fill(context->bootCmd.address, context->bootCmd.count,
                                                                context->bootCmd.data);
    // Context is already setup to get the next boot command
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "program" command handler
//!
//! Implements the action function for the boot "program" command.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \post   *context is left setup to get the next boot command.
//!
//! \retval kStatus_Success
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoProgramCmd(ldr_Context_t *context)
{
    status_t status = (int32_t)kStatus_Success;
    uint32_t memoryId = MAKE_MEMORYID((context->bootCmd.flags & (uint32_t)ROM_MEM_GROUP_ID_MASK) >> (uint32_t)ROM_MEM_GROUP_ID_SHIFT,
                                      (context->bootCmd.flags & (uint32_t)ROM_MEM_DEVICE_ID_MASK) >> (uint32_t)ROM_MEM_DEVICE_ID_SHIFT);
    bool isEightByte = (context->bootCmd.flags & (uint16_t)ROM_PROG_8BYTE_MASK) != 0u;
    uint32_t byteCount = (isEightByte ? 8u : 4u);
    uint32_t index = context->bootCmd.address;
    uint32_t *data =
        &context->bootCmd.count; // First 4 bytes are in count field, second 4 bytes are in following data field

#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH) && \
    !(defined(BL_DEVICE_IS_LPC_SERIES) && BL_DEVICE_IS_LPC_SERIES)
    if (memoryId != (uint32_t)kMemoryIFR0_Fuse)
    {
        status = (int32_t)kStatus_InvalidArgument;
    }
    else
    {
        lock_acquire();
        // Note: Both Main and Secondary flash share the same IFR Memory
        //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
        status = g_bootloaderContext.flashDriverInterface->flash_program_once(
            &g_bootloaderContext.allFlashState->ftfxConfig[kFlashIndex_Main], index, (uint8_t *)data, byteCount);
        lock_release();
        // Context is already setup to get the next boot command
    }
    
    return status;
#elif defined(BL_FEATURE_OCOTP_MODULE)
    if (memoryId != kMemoryIFR0_Fuse)
    {
        return kStatus_InvalidArgument;
    }
    return ocotp_program_once(OCOTP, index, data, byteCount);
#else

    return kStatusRomLdrUnknownCommand;
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "jump" command handler
//!
//! Implements the action function for the boot "jump" command.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \retval kStatusRomLdrJumpReturned if the plugin returns
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoJumpCmd(ldr_Context_t *context)
{
    // Actual jump is implemented in sbloader_finalize().
    return (int32_t)kStatus_AbortDataPhase;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "call" command handler
//!
//! Implements the action function for the boot "call" command.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \post   *context is setup to get the next boot command.
//! \post   context->objectID is set to the next boot section or image ID.
//!
//! \return Return code from plugin or ldr_GoToNextSection.
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoCallCmd(ldr_Context_t *context)
{
    union
    {
        uint32_t address;
        pCallFnc_t entry_fun;
    } func_ptr;

    func_ptr.address = context->bootCmd.address;
    status_t rc = (int32_t)kStatusRomLdrCallFailed;
    status_t status = (int32_t)kStatus_Success;

    // todo: need common impl with bootloader call cmd

    // Call the plugin entry point with the specified parameter. The plugin
    // can start a new section or image by returning the appropriate code and
    // updating the object id pointed to by the second parameter.
    if (is_valid_application_location(func_ptr.address))
    {
        rc = func_ptr.entry_fun(context->bootCmd.data, &context->objectID);
    }

    if (rc == (int32_t)ROM_BOOT_SECTION_ID)
    {
        // The plugin returned a section ID, skip ahead to the next section
        status = ldr_GoToNextSection(context);
    }
    else if (rc == (int32_t)ROM_BOOT_IMAGE_ID)
    {
        // The plugin returned an image ID, restart the loader state machine
        (void)sbloader_init();
    }
    else
    {
        // Otherwise, just pass on the plugin return code. Context is already
        // setup to get the next boot command.
        status = rc;
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "tag" command handler
//!
//! Implements the action function for the boot "tag" command.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \post   *context is setup to get the next boot command.
//! \post   context->sectChunks is updated to the next section.
//!
//! \retval kStatusRomLdrSectionLength if new section count is out of range.
//! \retval kStatusRomLdrIdNotFound if no match and no more sections.
//! \retval Return code from ldr_GoToNextSection.
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoTagCmd(ldr_Context_t *context)
{
    status_t status = (int32_t)kStatus_Success;

    // Get the section chunk count from the boot command
    context->sectChunks = context->bootCmd.count;

    // Sanity check the new section count
    if (context->sectChunks > context->fileChunks)
    {
        status = (int32_t)kStatusRomLdrSectionLength;
    }
    else
    {
        if ((context->bootCmd.data & SFLG_SECTION_BOOTABLE) != 0u && 
            (context->bootCmd.address == context->objectID) &&
            (context->bootCmd.count != 0u))
        {
            // This section is bootable and matches the ID we are looking for, so
            // just continue getting commands from this point
            // Reinitialize the decryption for the rest of the boot commands and the section

            context->bootSectChunks = context->bootCmd.count;

#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
            if (context->keyCount)
            {
                cbc_mac_init(context->dek, context->initVector);
            }
#endif
        }
        else if ((context->bootCmd.flags & CFLG_LAST_TAG) != 0u)
        {
            // This isn't the right boot section and it's the last one, so return
            // an error
            status = (int32_t)kStatusRomLdrIdNotFound;
        }
        else
        {
            // This isn't the boot section we are looking for and there are more, so
            // skip to the next one
            status = ldr_GoToNextSection(context);
        }
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "erase" command handler
//!
//! Implements the action function for the boot "erase" command.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \post   *context is setup to get the next boot command.
//! \post   context->sectChunks is updated to the next section.
//!
//! \retval kStatus_Success
//! \retval kStatus_InvalidArgument
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoEraseCmd(ldr_Context_t *context)
{
    status_t status = (int32_t)kStatus_Success;

    if ((context->bootCmd.flags & ROM_ERASE_ALL_UNSECURE_MASK) != 0u)
    {
#if defined(BL_FEATURE_ERASEALL_UNSECURE) && BL_FEATURE_ERASEALL_UNSECURE
        status = flash_mem_erase_all_unsecure();
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH 
        if (g_bootloaderContext.dflashDriverInterface != NULL)
        {
            status += flexNVM_mem_erase_all_unsecure();   
        }
#endif // BL_FEATURE_SUPPORT_DFLASH         
#else
        status = (int32_t)kStatus_InvalidArgument;
#endif // BL_FEATURE_ERASEALL_UNSECURE
    }
    else if ((context->bootCmd.flags & ROM_ERASE_ALL_MASK) != 0u)
    {
        uint32_t memoryId = MAKE_MEMORYID((context->bootCmd.flags & (uint32_t)ROM_MEM_GROUP_ID_MASK) >> (uint32_t)ROM_MEM_GROUP_ID_SHIFT,
                                          (context->bootCmd.flags & (uint32_t)ROM_MEM_DEVICE_ID_MASK) >> (uint32_t)ROM_MEM_DEVICE_ID_SHIFT);
        switch (memoryId)
        {
#if !(defined(BL_FEATURE_HAS_NO_INTERNAL_FLASH) && BL_FEATURE_HAS_NO_INTERNAL_FLASH)
            case (uint32_t)kMemoryInternal:
#if defined(BL_FEATURE_FAC_ERASE) && BL_FEATURE_FAC_ERASE
                status = flash_mem_erase_all(kFlashEraseAllOption_Blocks);
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH 
                if (g_bootloaderContext.dflashDriverInterface != NULL)
                {
                    status += flexNVM_mem_erase_all();  
                }
#endif // BL_FEATURE_SUPPORT_DFLASH                 
#else
                status = flash_mem_erase_all();  
#if defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH 
                if (g_bootloaderContext.dflashDriverInterface != (void *)0u)
                {
                    status += flexNVM_mem_erase_all(); 
                }
#endif // BL_FEATURE_SUPPORT_DFLASH                 
#endif // BL_FEATURE_FAC_ERASE
                break;
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
            case (uint32_t)kMemoryQuadSpi0:
                status = qspi_mem_erase_all();
                break;
#endif
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE) && BL_FEATURE_FLEXSPI_NOR_MODULE
            case (uint32_t)kMemoryFlexSpiNor:
                status = flexspi_nor_mem_erase_all();
                break;
#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE
#if defined(BL_FEATURE_SPINAND_MODULE) && BL_FEATURE_SPINAND_MODULE
            case (uint32_t)kMemorySpiNand:
                status = spinand_mem_erase_all();
                break;
#endif
#if defined(BL_FEATURE_SEMC_NOR_MODULE) && BL_FEATURE_SEMC_NOR_MODULE
            case (uint32_t)kMemorySemcNor:
                status = semc_nor_mem_erase_all();
                break;
#endif // #if BL_FEATURE_SEMC_NOR_MODULE
#if defined(BL_FEATURE_SEMC_NAND_MODULE) && BL_FEATURE_SEMC_NAND_MODULE
            case (uint32_t)kMemorySemcNand:
                status = semc_nand_mem_erase_all();
                break;
#endif
            default:
                status = (int32_t)kStatus_InvalidArgument;
                break;
        }
    }
    else
    {
        uint32_t memoryId = MAKE_MEMORYID((context->bootCmd.flags & (uint32_t)ROM_MEM_GROUP_ID_MASK) >> (uint32_t)ROM_MEM_GROUP_ID_SHIFT,
                                          (context->bootCmd.flags & (uint32_t)ROM_MEM_DEVICE_ID_MASK) >> (uint32_t)ROM_MEM_DEVICE_ID_SHIFT);
        status = g_bootloaderContext.memoryInterface->erase(context->bootCmd.address, context->bootCmd.count, memoryId);
    }

    return status;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "reset" command handler
//!
//! Implements the action function for the boot "reset" command.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \post   *context is setup to get the next boot command.
//! \post   context->sectChunks is updated to the next section.
//!
//! \retval kStatus_Success
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoResetCmd(ldr_Context_t *context)
{
    // Actual reset is implemented in sbloader_finalize().
    return (int32_t)kStatus_AbortDataPhase;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief  Loader "memory enable" command handler
//!
//! Implements the action function for the boot "memory enable" command.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \post   *context is setup to get the next boot command.
//! \post   context->sectChunks is updated to the next section.
//!
//! \retval kStatus_Success
//! \retval kStatus_InvalidArgument
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoMemEnableCmd(ldr_Context_t *context)
{
    status_t status = (int32_t)kStatus_Success;
#if defined(BL_FEATURE_SPIFI_NOR_MODULE) && BL_FEATURE_SPIFI_NOR_MODULE || \
    defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE || \
    defined(BL_FEATURE_FLEXSPI_NOR_MODULE) && BL_FEATURE_FLEXSPI_NOR_MODULE || \
    defined(BL_FEATURE_SEMC_NOR_MODULE) && BL_FEATURE_SEMC_NOR_MODULE || \
    defined(BL_FEATURE_EXPAND_MEMORY) && BL_FEATURE_EXPAND_MEMORY
    uint32_t memoryId = MAKE_MEMORYID((context->bootCmd.flags & ROM_MEM_GROUP_ID_MASK) >> ROM_MEM_GROUP_ID_SHIFT,
                                      (context->bootCmd.flags & ROM_MEM_DEVICE_ID_MASK) >> ROM_MEM_DEVICE_ID_SHIFT);
#endif

#if defined(BL_FEATURE_SPIFI_NOR_MODULE) && BL_FEATURE_SPIFI_NOR_MODULE
    if (memoryId == kMemorySpifiNor)
    {
        status = spifi_nor_mem_config((uint32_t*)context->bootCmd.address);
    }
    else
#endif //BL_FEATURE_SPIFI_NOR_MODULE
#if defined(BL_FEATURE_QSPI_MODULE) && BL_FEATURE_QSPI_MODULE
    if (memoryId == kMemoryQuadSpi0)
    {
        status = configure_qspi(context->bootCmd.address);
    }
    else
#endif // #if BL_FEATURE_QSPI_MODULE
#if defined(BL_FEATURE_FLEXSPI_NOR_MODULE) && BL_FEATURE_FLEXSPI_NOR_MODULE
        if (memoryId == kMemoryFlexSpiNor)
    {
        status = flexspi_nor_mem_config((uint32_t *)context->bootCmd.address);
    }
    else
#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE
#if defined(BL_FEATURE_SEMC_NOR_MODULE) && BL_FEATURE_SEMC_NOR_MODULE
    if (memoryId == kMemorySemcNor)
    {
        status = semc_nor_mem_config((uint32_t *)context->bootCmd.address);
    }
    else
#endif // #if BL_FEATURE_SEMC_NOR_MODULE
#if defined(BL_FEATURE_EXPAND_MEMORY) && BL_FEATURE_EXPAND_MEMORY
        if (GROUPID(memoryId) == kGroup_External)
    {
        uint32_t index;
        status = find_external_map_index(memoryId, &index);
        if (status == (int32_t)kStatus_Success)
        {
            external_memory_map_entry_t *map =
                (external_memory_map_entry_t *)&g_bootloaderContext.externalMemoryMap[index];
            status = map->memoryInterface->config((uint32_t *)context->bootCmd.address);
        }
    }
    else
#endif // #if BL_FEATURE_EXPAND_MEMORY
    {
        status = (int32_t)kStatus_InvalidArgument;
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief  Loader boot command parsing function
//!
//! Implements the action function for parsing a boot command. Performs generic
//! integrity tests and flag handling, then calls the appropriate command
//! handler function based on the command tag.
//!
//! \param[in]  context    Pointer to a loader context structure.
//!
//! \pre    context->bootCmd contains the boot command.
//!
//! \retval kStatusRomLdrChecksum if command checksum failed.
//! \retval kStatusRomLdrUnknownCommand if command tag is not recognized.
//! \retval Return code from the boot command handler function.
////////////////////////////////////////////////////////////////////////////////
status_t ldr_DoCommand(ldr_Context_t *context)
{
    static boot_cmd_t *pCmd;
    pCmd = &context->bootCmd;
    uint32_t i, sum = 0x5a;
    status_t status = (int32_t)kStatus_Success;

#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
    // Decrypt this boot command block if enabled
    if (context->keyCount)
    {
        cbc_mac_decrypt(context->src, sizeof(chunk_t), context->src);
    }
#endif

    union
    {
        uint8_t *src;
        boot_cmd_t *cmd;
    } boot_cmd;

    boot_cmd.src = context->src;
    // Save the boot commmand.
    context->bootCmd = *boot_cmd.cmd;

    // Compute and test the boot command checksum
    for (i = 1u; i < sizeof(boot_cmd_t); i++)
    {
        sum += ((uint8_t *)pCmd)[i];
    }
    if (((uint8_t *)pCmd)[0] != (sum & 0xFFu))
    {
        debug_printf("Error: invalid boot command checksum\r\n");
        status = (int32_t)kStatusRomLdrChecksum;
    }
    else
    {
        // Switch to the appropriate command handler function
        switch (pCmd->tag)
        {
            case (uint8_t)ROM_NOP_CMD:
                status = (int32_t)kStatus_Success;
                break;
            case (uint8_t)ROM_TAG_CMD:
                status = ldr_DoTagCmd(context);
                break;
            case (uint8_t)ROM_LOAD_CMD:
                status = ldr_DoLoadCmd(context);
                break;
            case (uint8_t)ROM_FILL_CMD:
                status = ldr_DoFillCmd(context);
                break;
            case (uint8_t)ROM_JUMP_CMD:
                status = ldr_DoJumpCmd(context);
                break;
            case (uint8_t)ROM_CALL_CMD:
                status = ldr_DoCallCmd(context);
                break;
            case (uint8_t)ROM_ERASE_CMD:
                status = ldr_DoEraseCmd(context);
                break;
            case (uint8_t)ROM_MODE_CMD:
                status = (int32_t)kStatus_Success; // ignored for Kinetis
                break;
            case (uint8_t)ROM_RESET_CMD:
                status = ldr_DoResetCmd(context);
                break;
            case (uint8_t)ROM_MEM_ENABLE_CMD:
                status = ldr_DoMemEnableCmd(context);
                break;
            case (uint8_t)ROM_PROG_CMD:
                status = ldr_DoProgramCmd(context);
                break;
            default:
                status = (int32_t)kStatusRomLdrUnknownCommand;
              break;
        }
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! @brief Initialize the loader state machine.
////////////////////////////////////////////////////////////////////////////////
status_t sbloader_init(void)
{
    // Initialize the context
    (void)memset(&s_loaderContext, 0, sizeof(ldr_Context_t));

    // Process the first chunk of the image header
    s_loaderContext.Action = ldr_DoHeader;

#if defined(AES_SECURITY_SUPPORTED) && AES_SECURITY_SUPPORTED
#ifndef BL_FEATURE_ENCRYPTION_KEY_ADDRESS
    // Ensure the process of read key cannot be interrupted by other IRQs.
    lock_acquire();
    // Note: Both Main and Secondary flash share the same IFR Memory
    //  So it doesn't matter what index of allFlashState[] we use for this FLASH API.
    g_bootloaderContext.flashDriverInterface->flash_read_once(g_bootloaderContext.allFlashState->ftfxConfig,
                                                              SB_KEY_RDONCE_INDEX + 0, (uint8_t *)&s_aesKey[0],
                                                              sizeof(s_aesKey[0]));
    g_bootloaderContext.flashDriverInterface->flash_read_once(g_bootloaderContext.allFlashState->ftfxConfig,
                                                              SB_KEY_RDONCE_INDEX + 1, (uint8_t *)&s_aesKey[1],
                                                              sizeof(s_aesKey[1]));
    g_bootloaderContext.flashDriverInterface->flash_read_once(g_bootloaderContext.allFlashState->ftfxConfig,
                                                              SB_KEY_RDONCE_INDEX + 2, (uint8_t *)&s_aesKey[2],
                                                              sizeof(s_aesKey[2]));
    g_bootloaderContext.flashDriverInterface->flash_read_once(g_bootloaderContext.allFlashState->ftfxConfig,
                                                              SB_KEY_RDONCE_INDEX + 3, (uint8_t *)&s_aesKey[3],
                                                              sizeof(s_aesKey[3]));
    lock_release();
#else
    const uint32_t *kekTable = (const uint32_t *)BL_FEATURE_ENCRYPTION_KEY_ADDRESS;
    s_aesKey[0] = kekTable[0];
    s_aesKey[1] = kekTable[1];
    s_aesKey[2] = kekTable[2];
    s_aesKey[3] = kekTable[3];
#endif

    uint32_t i;
    // Test to see if the key is non blank
    for (i = 0u; i < (uint32_t)AES_128_KEY_SIZE_WORDS; i++)
    {
        if (s_aesKey[i] != ~0u)
        {
            break;
        }
    }

    // If i went all the way through the loop the key is blank so default to a blank key
    if (i == AES_128_KEY_SIZE_WORDS)
    {
        (void)memset(s_aesKey, 0, sizeof(s_aesKey));
    }
#endif

    return (int32_t)kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
//! @brief Pump the loader state machine.
////////////////////////////////////////////////////////////////////////////////
status_t sbloader_pump(uint8_t *data, uint32_t length)
{
    status_t status = (int32_t)kStatus_Success;
    uint32_t required;
    uint32_t available;
    uint32_t readPosition = 0u;

    while (readPosition != length)
    {
        required = sizeof(s_loaderBuf.data) - s_loaderBuf.fillPosition;
        available = length - readPosition;

        // copy what we need to complete a full chunk into the chunk buffer
        while ((required > 0u) && (available > 0u))
        {
            s_loaderBuf.data[s_loaderBuf.fillPosition++] = data[readPosition++];
            --required;
            --available;
        }

        if (required == 0u)
        {
            // a full chunk was filled to process it
            s_loaderBuf.fillPosition = 0u;

            status = sbloader_handle_chunk();

            if (status != (int32_t)kStatus_Success)
            {
                break;
            }
        }
        else if (available == 0u)
        {
            // otherwise we are just going to wait for more data
            status = (int32_t)kStatusRomLdrDataUnderrun;
            break;
        }
        else
        {
            // doing nothing
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! @brief Handles the chunk in the s_loaderBuf
////////////////////////////////////////////////////////////////////////////////
status_t sbloader_handle_chunk(void)
{
    status_t status = (int32_t)kStatus_Success;

    s_loaderContext.src = s_loaderBuf.data;

    // If we have found the boot section and are currently working on its commands
    // Once we have received the number of chunks in the section we will be done
    if (s_loaderContext.bootSectChunks != 0u)
    {
        if (s_loaderContext.receivedChunks != s_loaderContext.bootSectChunks)
        {
            s_loaderContext.receivedChunks++;
            status = s_loaderContext.Action(&s_loaderContext);
        }
    }
    else
    {
        // Otherwise this is header info or a non bootable section that might be skipped through
        if (s_loaderContext.skipCount == 0u)
        {
            status = s_loaderContext.Action(&s_loaderContext);
        }
        else
        {
            --s_loaderContext.skipCount;
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
//! @brief Implement jump or reset command
////////////////////////////////////////////////////////////////////////////////
status_t sbloader_finalize(void)
{
    // Force to write buffered data to target memory if it is last data
    // chunks with reset/call/jump command at the end
    assert(g_bootloaderContext.memoryInterface->flush);
    status_t status = g_bootloaderContext.memoryInterface->flush();
    if (status == (int32_t)kStatus_Success)
    {
        if (s_loaderContext.bootCmd.tag == ROM_JUMP_CMD)
        {
            union
            {
                uint32_t address;
                pJumpFnc_t entry_fun;
            } func_ptr;

            func_ptr.address = s_loaderContext.bootCmd.address;

            // Jump to the entry point with the specified parameter
            bool isValid = is_valid_application_location(func_ptr.address);
#if defined(BL_FEATURE_OTFAD_MODULE) && BL_FEATURE_OTFAD_MODULE
            if (isValid && is_qspi_present())
            {
                quadspi_cache_clear();
                status_t status = otfad_init_as_needed();
                if (status != (int32_t)kStatus_Success)
                {
                    isValid = false;
                }
                update_qspi_otfad_init_status(status);
            }
            if (!isValid)
            {
                status = (int32_t)kStatus_OtfadInvalidKeyBlob;
            }
#endif
            if (isValid)
            {
                // Clean up prior to calling user code.
                shutdown_cleanup(kShutdownType_Shutdown);

                // Set initial SP if requested in by flag
                if ((s_loaderContext.bootCmd.flags & (uint16_t)ROM_JUMP_SP_MASK) != 0u)
                {
                    static uint32_t s_addr = 0u;
                    s_addr = s_loaderContext.bootCmd.count;

                    // Get RAM address ranges
                    const memory_map_entry_t *map = &g_bootloaderContext.memoryMap[(uint32_t)kIndexSRAM];

                    // Validate stack pointer address. It must either be 0 or within the RAM range.
                    if (!((s_addr == 0u) || ((s_addr >= map->startAddress) && (s_addr <= map->endAddress + 1u))))
                    {
                        // Invalid stack pointer value, respond with kStatus_InvalidArgument.
                        status = (int32_t)kStatus_InvalidArgument;
                    }
                    else if (s_addr != 0u)
                    {
                        // Set main stack pointer and process stack pointer
                        __set_MSP(s_addr);
                        __set_PSP(s_addr);
                    }
                    else
                    {
                        // doing nothing.
                    }
                }

                if (status == (int32_t)kStatus_Success)
                {
                    (void)func_ptr.entry_fun(s_loaderContext.bootCmd.data);
                }
            }
            else
            {
#if defined(BL_FEATURE_OTFAD_MODULE) && BL_FEATURE_OTFAD_MODULE
                if (status != (int32_t)kStatus_OtfadInvalidKeyBlob)
#endif
                {
                    // We should never get here, so return an error if we do
                    status = (int32_t)kStatusRomLdrJumpReturned;
                }
            }
        }
        else if (s_loaderContext.bootCmd.tag == ROM_RESET_CMD)
        {
            // Prepare for shutdown.
            shutdown_cleanup(kShutdownType_Reset);

            NVIC_SystemReset();
            // Does not get here.
            assert(0);

            // We should never get here, so return an error if we do
            status = (int32_t)kStatusRomLdrResetReturned;
        }
        else
        {
            status = (int32_t)kStatusRomLdrUnknownCommand;
        }
    }
    // We should never get here, so return an error if we do
    return status;
}

//! @}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
