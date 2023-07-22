/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#if defined(AAC_DEC) && !defined(__ICCARM__)
#include <error.h>
#include "cci_metadata.h"
#include "codec_interface.h"
#include "fsl_os_abstraction.h"

////////////////////////////////////////////////////////////////////////////////
//! \brief Extract the meta data from the aac file.
//!
//!
//! Extract the meta data from the file.
//!
//! \param[in]  *MetaData    this structure will be filled in by the function
//! \param[in]  *ctx         CCI Context Structure
//! \retval SUCCESS
//! \retval CODEC_METADATA_NOT_FOUND -- metadata failed
//! \retval ERROR_ASI_DFFI_METADATA_NOT_FOUND - metadata was not found
//! \retval ERROR_OS_FILESYSTEM_FILE_OPEN -- the file failed to open
//! \retval CODEC_FSEEK_FAILED - fseek call failed
//!
////////////////////////////////////////////////////////////////////////////////
int32_t codec_extract_metadata_aac(file_meta_data_t *MetaData, CCI_Ctx *ctx)
{
    int32_t *memory[2]    = {NULL, NULL};
    int32_t *callbacks[3] = {NULL, NULL, NULL};
    int32_t size[2]       = {0, 0};
    int32_t ret           = CODEC_SUCCESS;

    if (ctx->user_data == NULL)
    {
        return CODEC_INIT_ERROR;
    }

    // Get size of read buffer
    AACDecoderGetIOFrameSize(&size[0], &size[1]);

    // Allocate memory for decoder to get metadata
    memory[0] = (int32_t *)OSA_MemoryAllocate(size[0]);
    if (memory[0] == NULL)
    {
        ret = CODEC_INIT_ERROR;
        goto Error;
    }
    memory[1] = (int32_t *)OSA_MemoryAllocate(sizeof(int32_t) * 2);
    if (memory[0] == NULL)
    {
        ret = CODEC_INIT_ERROR;
        goto Error;
    }

    // Set callback functions
    callbacks[0] = (int32_t *)ctx->cci_dec_read;
    callbacks[1] = (int32_t *)ctx->cci_dec_seek;
    callbacks[2] = (int32_t *)ctx->cci_dec_tell;

    // get metadata
    ret = AACDecodergetMetaData(memory, callbacks, (int32_t *)ctx->user_data);
    if (ret != CODEC_SUCCESS)
    {
        goto Error;
    }

    MetaData->sample_rate  = (uint32_t)(memory[1][0]);
    MetaData->num_channels = memory[1][1];

    /* Hard-code the audio format details for streaming use case. */
    MetaData->stream_type      = STREAM_TYPE_AAC;
    MetaData->bits_per_sample  = 16;
    MetaData->valid_vbr_header = 0;
    MetaData->start_pos        = 0;

    /* Cannot calculate with audio stream.  Set to non-zero to avoid
     * streamer pipeline error about invalid duration. */
    MetaData->total_time = 1;

Error:
    if (memory[0] != NULL)
    {
        OSA_MemoryFree(memory[0]);
        memory[0] = NULL;
    }
    if (memory[1] != NULL)
    {
        OSA_MemoryFree(memory[1]);
        memory[1] = NULL;
    }

    return ret;
}

#endif
///////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
