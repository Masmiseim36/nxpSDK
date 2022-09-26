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
#include <error.h>
#include "cci_metadata.h"
#include "codec_interface.h"
#ifdef OGG_OPUS_DEC
#include <opusfile.h>
#endif

////////////////////////////////////////////////////////////////////////////////
//! \brief Extract the meta data from raw opus file.
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
int32_t codec_extract_metadata_opus(file_meta_data_t *MetaData, CCI_Ctx *ctx)
{
    /* Hard-code the audio format details for streaming use case. */
    MetaData->sample_rate     = 48000;
    MetaData->num_channels    = 1;
    MetaData->bits_per_sample = 16;
    MetaData->stream_type     = STREAM_TYPE_OPUS;

    MetaData->valid_vbr_header = 0;
    MetaData->start_pos        = 0;
    /* Cannot calculate with audio stream.  Set to non-zero to avoid
     * streamer pipeline error about invalid duration. */
    MetaData->total_time = 1;

    return CODEC_SUCCESS;
}

#ifdef OGG_OPUS_DEC

////////////////////////////////////////////////////////////////////////////////
//! \brief Extract the meta data from the ogg opus file.
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
int32_t codec_extract_metadata_ogg_opus(file_meta_data_t *MetaData, CCI_Ctx *ctx)
{
    int ret = 0;
    uint8_t opus_buf[60];
    OpusHead head = {0};

    ctx->cci_dec_read(0, opus_buf, sizeof(opus_buf), 0, ctx->user_data);

    ret = op_test(&head, opus_buf, sizeof(opus_buf));
    if (ret < 0)
    {
        /* Cannot parse header */
        return CODEC_METADATA_NOT_FOUND;
    }

    /* Set the sample rate to 48kHz because the output data from the op_read() function is always at 48kHz. */
    MetaData->sample_rate = 48000U;

    MetaData->num_channels = head.channel_count;
    MetaData->stream_type  = STREAM_TYPE_OGG_OPUS;

    /* Hard-code the audio format details for streaming use case. */
    MetaData->bits_per_sample  = 16;
    MetaData->valid_vbr_header = 0;
    MetaData->start_pos        = 0;
    /* Cannot calculate with audio stream.  Set to non-zero to avoid
     * streamer pipeline error about invalid duration. */
    MetaData->total_time = 1;

    return CODEC_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}

#endif /* OGG_OPUS */
