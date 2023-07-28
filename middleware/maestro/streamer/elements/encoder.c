/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    decoder.c
 * @brief   This file implements the Opus encoder stream element.
 *
 */

#include "streamer.h"
#include "streamer_element_properties.h"
#include "pipeline.h"
#include "pad.h"
#include "cei.h"
#include "encoder.h"

int32_t encoder_init_element(StreamElement *element)
{
    StreamReturnType ret   = STREAM_OK;
    ElementEncoder *encElm = (ElementEncoder *)element;

    STREAMER_FUNC_ENTER(DBG_ENCODER);

    CHK_ARGS(NULL == encElm, STREAM_ERR_INVALID_ARGS);

    /* Initialize the structure */
    encElm->chunk_size = 1920;

    encElm->bitstream_info_specified = false;
    encElm->bitstream_info           = (CeiBitstreamInfo){.sample_rate  = 0,
                                                .num_channels = 0,
                                                .endian       = AF_LITTLE_ENDIAN,
                                                .sign         = true,
                                                .sample_size  = 0,
                                                .interleaved  = true};

    encElm->initialized = false;
    encElm->eos_done    = false;
    encElm->offset      = 0;
    encElm->outOffset   = 0;

    encElm->enc_info = NULL;
    encElm->enc_type = CEIENC_LAST;

    // Get and set procedures
    encElm->get_property = &encoder_get_property;
    encElm->set_property = &encoder_set_property;

    /* Iterate over and initialize pads */
    uint32_t i = 0;

    /* ...source pads */
    for (i = 0; i < NUM_SRCS(TYPE_ELEMENT_ENCODER); i++)
    {
        PadSrc *pad = (PadSrc *)ELEMENT_SRC_PAD(encElm, i);

        pad->event_handler      = NULL;
        pad->activation_handler = encoder_src_pad_activation_handler;
        pad->pull_handler       = NULL;
        pad->query_handler      = NULL;
        pad->process_precheck   = NULL;
        pad->process_handler    = encoder_src_pad_process_handler;
    }

    /* ...sink pads */
    for (i = 0; i < NUM_SINKS(TYPE_ELEMENT_ENCODER); i++)
    {
        PadSink *pad = (PadSink *)ELEMENT_SINK_PAD(encElm, i);

        pad->query_handler      = NULL;
        pad->event_handler      = NULL;
        pad->activation_handler = NULL;
        pad->chain_handler      = NULL;
        pad->process_handler    = NULL;
        pad->process_precheck   = NULL;
    }

    STREAMER_FUNC_EXIT(DBG_ENCODER);
    return ret;
}

int32_t encoder_get_chunk_size(StreamElement *element, uint64_t *val_ptr)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    ElementEncoder *encElm = (ElementEncoder *)element;
    *val_ptr               = encElm->chunk_size;

    return STREAM_OK;
}

int32_t encoder_get_type(StreamElement *element, uint64_t *val_ptr)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    ElementEncoder *encElm = (ElementEncoder *)element;
    *val_ptr               = encElm->enc_type;

    return STREAM_OK;
}

int32_t encoder_get_config(StreamElement *element, uint64_t *val_ptr)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    ElementEncoder *encElm = (ElementEncoder *)element;

    if (encElm->enc_type == CEIENC_LAST)
        return STREAM_ERR_ELEMENT_BAD_STATUS;

    if (encElm->initialized == false && encElm->initial_config_ptr != NULL)
    {
        memcpy(val_ptr, encElm->initial_config_ptr, ceiEncConfigSizeTable[encElm->enc_type]);
        return STREAM_OK;
    }

    CeiEncoderFunctions fn = ceiEncTable[encElm->enc_type];
    if (fn.fnGetConfig == NULL)
        return STREAM_ERR_GENERAL;

    return fn.fnGetConfig(encElm->enc_info, val_ptr) == 0 ? STREAM_OK : STREAM_ERR_GENERAL;
}

int32_t encoder_get_bitstreaminfo(StreamElement *element, uint64_t *val_ptr)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    ElementEncoder *encElm = (ElementEncoder *)element;
    CeiBitstreamInfo *info = (CeiBitstreamInfo *)val_ptr;

    *info = encElm->bitstream_info;

    return STREAM_OK;
}

int32_t encoder_get_property(StreamElement *element, uint16_t prop, uint64_t *val_ptr)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    switch (prop)
    {
        case PROP_ENCODER_CHUNK_SIZE:
            return encoder_get_chunk_size(element, val_ptr);

        case PROP_ENCODER_TYPE:
            return encoder_get_type(element, val_ptr);

        case PROP_ENCODER_CONFIG:
            return encoder_get_config(element, val_ptr);

        case PROP_ENCODER_BITSTREAMINFO:
            return encoder_get_bitstreaminfo(element, val_ptr);

        default:
            return STREAM_ERR_INVALID_ARGS;
    }
}

int32_t encoder_set_chunk_size(StreamElement *element, uintptr_t val)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    ElementEncoder *encElm = (ElementEncoder *)element;

    // Check that chunk size is bigger than 0
    if (val <= 0)
        return STREAM_ERR_INVALID_ARGS;

    // Chunk size change not allowed if already initialized
    if (encElm->initialized == true)
        return STREAM_ERR_ELEMENT_BAD_STATUS;

    encElm->chunk_size = val;
    return STREAM_OK;
}

int32_t encoder_set_type(StreamElement *element, uintptr_t val)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    if (val >= CEIENC_LAST)
        return STREAM_ERR_INVALID_ARGS;

    ElementEncoder *encElm = (ElementEncoder *)element;

    // Encoder type change not allowed if already initialized
    if (encElm->initialized == true)
        return STREAM_ERR_ELEMENT_BAD_STATUS;

    // Check if the encoder is implemented / enabled
    CeiEncoderFunctions fn = ceiEncTable[val];
    if (fn.fnGetMemorySize == NULL || fn.fnInit == NULL || fn.fnEncode == NULL)
        return STREAM_ERR_INVALID_ARGS;

    // Free the config structure if it has been ever allocated - most likely it
    // becomes invalid by this act
    if (encElm->initial_config_ptr != NULL)
    {
        OSA_MemoryFree(encElm->initial_config_ptr);
        encElm->initial_config_ptr = NULL;
    }

    encElm->enc_type = (CeiEncoderType)val;
    return STREAM_OK;
}

int32_t encoder_set_config(StreamElement *element, uintptr_t val)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    ElementEncoder *encElm = (ElementEncoder *)element;

    if (encElm->enc_type == CEIENC_LAST)
        return STREAM_ERR_ELEMENT_BAD_STATUS;

    CeiEncoderFunctions fn = ceiEncTable[encElm->enc_type];
    if (fn.fnSetConfig == NULL)
        return STREAM_ERR_GENERAL;

    if (encElm->initialized == false)
    {
        // Encoder element not initialized, save the config for later initial
        // configuration.

        // If it has been allocated before, destroy it, the encoder type may
        // have changed.
        if (encElm->initial_config_ptr != NULL)
        {
            OSA_MemoryFree(encElm->initial_config_ptr);
            encElm->initial_config_ptr = NULL;
        }

        // Allocate space for the config structure
        encElm->initial_config_ptr = OSA_MemoryAllocate(ceiEncConfigSizeTable[encElm->enc_type]);
        if (encElm->initial_config_ptr == NULL)
            return STREAM_ERR_NO_MEM;

        // Copy it and terminate
        memcpy(encElm->initial_config_ptr, (void *)val, ceiEncConfigSizeTable[encElm->enc_type]);
        return STREAM_OK;
    }
    else
    {
        // Encoder has been initialized, apply the configuration straight away
        return fn.fnSetConfig(encElm->enc_info, (void *)val) == 0 ? STREAM_OK : STREAM_ERR_GENERAL;
    }
}

int32_t encoder_set_bitstreaminfo(StreamElement *element, uintptr_t val)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    ElementEncoder *encElm = (ElementEncoder *)element;
    CeiBitstreamInfo *info = (CeiBitstreamInfo *)val;

    // Changing bitstream paramaters on the fly is not supported
    if (encElm->initialized == true)
        return STREAM_ERR_ELEMENT_BAD_STATUS;

    // As of now, only 16-bit samples are supported
    if (info->sample_size != 16 || info->interleaved == false)
        return STREAM_ERR_INVALID_ARGS;

    encElm->bitstream_info           = *info;
    encElm->bitstream_info_specified = true;

    return STREAM_OK;
}

int32_t encoder_set_property(StreamElement *element, uint16_t prop, uintptr_t val)
{
    if (element == NULL)
        return STREAM_ERR_INVALID_ARGS;

    switch (prop)
    {
        case PROP_ENCODER_CHUNK_SIZE:
            return encoder_set_chunk_size(element, val);

        case PROP_ENCODER_TYPE:
            return encoder_set_type(element, val);

        case PROP_ENCODER_CONFIG:
            return encoder_set_config(element, val);

        case PROP_ENCODER_BITSTREAMINFO:
            return encoder_set_bitstreaminfo(element, val);

        default:
            return STREAM_ERR_INVALID_ARGS;
    }
}

uint8_t encoder_src_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret             = true;
    ElementEncoder *element = (ElementEncoder *)PAD_PARENT(pad);

    STREAMER_FUNC_ENTER(DBG_ENCODER);

    // Check encoder type
    if (element->enc_type == CEIENC_LAST)
        return false;

    /* activate the pad */
    if ((uint8_t) true == active)
    {
        // Allocate buffers, encoder initialization is not possible as we don't
        // know the input bitstream parameters

        // Buffer sizes - they contain a header

        // ASSUMPTION: AudioPacketHeader is the biggest packet header type.
        size_t inBufSize = element->chunk_size + sizeof(AudioPacketHeader);

        // We're going to produce packets with the RawPacketHeader
        size_t outBufSize = element->chunk_size + sizeof(RawPacketHeader);

        // Construct the input buffer
        BufferInfo inBuf = {.ptr = OSA_MemoryAllocate(inBufSize), .size = inBufSize};
        if (inBuf.ptr == NULL)
        {
            return false;
        }

        // Construct the output buffer
        BufferInfo outBuf = {.ptr = OSA_MemoryAllocate(outBufSize), .size = outBufSize};
        if (outBuf.ptr == NULL)
        {
            OSA_MemoryFree(inBuf.ptr);
            inBuf.ptr  = NULL;
            inBuf.size = 0;
            return false;
        }

        // Save the buffers
        element->inBuf  = inBuf;
        element->outBuf = outBuf;
    }
    else
    {
        // Pad destroy requested, free all buffers, free the encoder state,
        // reset and unset the initialized flag
        if (element->inBuf.ptr != NULL)
        {
            OSA_MemoryFree(element->inBuf.ptr);
            element->inBuf = (BufferInfo){.ptr = NULL, .size = 0};
        }

        if (element->outBuf.ptr != NULL)
        {
            OSA_MemoryFree(element->outBuf.ptr);
            element->outBuf = (BufferInfo){.ptr = NULL, .size = 0};
        }

        if (element->enc_info != NULL)
        {
            OSA_MemoryFree(element->enc_info);
            element->enc_info = NULL;
        }

        if (element->initial_config_ptr != NULL)
        {
            OSA_MemoryFree(element->initial_config_ptr);
            element->initial_config_ptr = NULL;
        }

        element->initialized = false;
    }

    // Activate peer pad
    ret = pad_activate_pull(pad->peer, active);

    STREAMER_FUNC_EXIT(DBG_CCID);
    return ret;
}

int32_t encoder_src_pad_process_handler(StreamPad *pad)
{
    FlowReturn ret          = FLOW_OK;
    ElementEncoder *element = NULL;
    int32_t retCode         = 0;

    STREAMER_FUNC_ENTER(DBG_CCID);
    CHK_ARGS(NULL == pad, false);

    // Acquire the element structure from a pad
    element = PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    // Check encoder type - CEIENC_LAST signalizes that an encoder type has not
    // been set
    if (element->enc_type == CEIENC_LAST)
        return false;

    CeiEncoderFunctions fn = ceiEncTable[element->enc_type];

    if (false == (uint8_t)element->eos_done)
    {
        // pad_pull_range retrieves data
        // pad_push pushes data out

        // Construct (get) an input buffer and fill it with data read from
        // source pad's peer (read)
        StreamBuffer inBuf = {.buffer = element->inBuf.ptr, .size = element->inBuf.size, .offset = 0, .time = 0};
        ret                = pad_pull_range(ELEMENT_SINK_PAD(element, 0), element->offset, element->chunk_size, &inBuf);

        // This is supposed to trigger on FLOW_EOS, but netbuf_src and
        // mem_src return this code instead even for an EOS condition.
        if (ret == FLOW_EOS)
        {
            // EOS condition reached on source pad read (pull) - send an event,
            // set a flag and terminate
            ret = FLOW_OK;
            STREAMER_LOG_DEBUG(DBG_CCID, "[ENC] EOS\n");

            element->eos_done = true;

            StreamEvent event;
            event_create_eos(&event);
            pad_push_event(pad, &event);

            goto Error;
        }
        else if (ret != FLOW_OK)
        {
            // Read failed
            ret = FLOW_ERROR;
            goto Error;
        }

        // Extract packet header
        PacketHeader *hdr = (PacketHeader *)element->inBuf.ptr;

        // Audio packet header, this means that bitstream parameters are available
        if (hdr->id == AUDIO_DATA)
        {
            AudioPacketHeader *aHdr = (AudioPacketHeader *)hdr;
            CeiBitstreamInfo *info  = &element->bitstream_info;

            // Encoder has not been created yet and we know the bitstream parameters,
            // so save them for later use
            if (element->initialized == false)
            {
                aHdr->sample_rate     = info->sample_rate;
                aHdr->num_channels    = info->num_channels;
                aHdr->endian          = info->endian;
                aHdr->sign            = info->sign;
                aHdr->bits_per_sample = info->sample_size;

                // As of now, only 16-bit samples are supported
                if (aHdr->bits_per_sample != 16)
                {
                    ret = FLOW_ERROR;
                    goto Error;
                }

                element->bitstream_info_specified = true;
            }
            else if (info->sample_rate != aHdr->sample_rate || info->num_channels != aHdr->num_channels ||
                     info->endian != aHdr->endian || info->sign != aHdr->sign ||
                     info->sample_size != aHdr->bits_per_sample)
            {
                // Bitstream parameters changed when encoder already initialized,
                // this situation is not supported.

                ret = FLOW_ERROR;
                goto Error;
            }
        }
        else if (element->bitstream_info_specified == false)
        {
            // Bitstream parameters were not specified by the user nor by the
            // preceeding element, cannot proceed

            ret = FLOW_ERROR;
            goto Error;
        }

        // Encoder not yet initialized, initialize
        if (element->initialized == false)
        {
            element->enc_info    = NULL;
            element->initialized = false;

            CeiEncoderFunctions fn = ceiEncTable[element->enc_type];

            // Allocate space for the encoder structure
            int size          = fn.fnGetMemorySize(element->bitstream_info.num_channels);
            element->enc_info = OSA_MemoryAllocate(size);
            if (element->enc_info == NULL)
            {
                ret = FLOW_ERROR;
                goto Error;
            }

            // Initialize the encoder
            if (fn.fnInit(element->enc_info, element->bitstream_info.sample_rate,
                          element->bitstream_info.num_channels) != 0)
            {
                OSA_MemoryFree(element->enc_info);
                element->enc_info = NULL;

                ret = FLOW_ERROR;
                goto Error;
            }

            // Encoder initialized
            element->initialized = true;

            // If initial config specified and the encoder supports configuration, set it
            if (element->initial_config_ptr != NULL && fn.fnSetConfig != NULL)
            {
                if (encoder_set_config((StreamElement *)element, (uintptr_t)element->initial_config_ptr) != STREAM_OK)
                {
                    ret = FLOW_ERROR;
                    goto Error;
                }
            }
        }

        // Pulled data contains a header, so we need to construct an offset
        // buffer just containing its payload (raw samples)

        // Determine the header size
        size_t hdrSize = 0;
        switch (hdr->id)
        {
            case RAW_DATA:
                hdrSize = sizeof(RawPacketHeader);
                break;
            case AUDIO_DATA:
                hdrSize = sizeof(AudioPacketHeader);
                break;
        }

        // Actual size of the payload (pulled data)
        size_t payloadSize = inBuf.size - hdrSize;

        // Construct the offset buffer
        BufferInfo rawInBuf = (BufferInfo){.ptr = (uint8_t *)inBuf.buffer + hdrSize, .size = element->chunk_size};

        // The output buffer, too, should contain a header, so for purposes of
        // holding the encoded data, we need an offset buffer as well
        BufferInfo rawOutBuf = {.ptr  = (uint8_t *)element->outBuf.ptr + sizeof(RawPacketHeader),
                                .size = element->outBuf.size - sizeof(RawPacketHeader)};

        // Fill the output buffer header
        RawPacketHeader *header = element->outBuf.ptr;
        header->id              = RAW_DATA;

        // Sample format conversion needed if the input is big-endian. Only
        // applicable to 16-bit samples.
        //
        // ASSUMPTION: ARM is always little-endian.
        if (element->bitstream_info.sample_size == 16 && element->bitstream_info.endian != AF_LITTLE_ENDIAN)
        {
            uint16_t *ptr = (uint16_t *)rawInBuf.ptr;
            for (size_t i = 0; i < payloadSize / sizeof(uint16_t); i++)
            {
                // Converting endianness is just a matter of swapping the bytes
                // around.
                ptr[i] = (ptr[i] >> 8) | (ptr[i] << 8);
            }
        }

        // Conversion from offset binary ("unsigned") to two's complement (signed)
        // sample format. Only applicable to 16-bit samples.
        if (element->bitstream_info.sample_size == 16 && element->bitstream_info.sign == false)
        {
            for (size_t i = 0; i < payloadSize / sizeof(uint16_t); i++)
            {
                // Converting from offset binary to two's complement is just a
                // matter of XORing the MSB(it) with 1.
                ((uint16_t *)rawInBuf.ptr)[i] ^= 0x8000;
            }
        }

        // Check if the byte count is equal to the requested one
        if (payloadSize != element->chunk_size)
        {
            // It is not, fill the rest of the packet with 0s
            uint8_t *bufPtr = (uint8_t *)rawInBuf.ptr;
            for (size_t i = payloadSize; i < element->chunk_size; i++)
            {
                bufPtr[i] = 0;
            }
        }

        // Invoke the encoder and advance the current encoder location (to facilitate
        // offset when pulling data from the preceeding pad).
        retCode = fn.fnEncode(element->enc_info, &rawInBuf, &rawOutBuf);
        element->offset += rawInBuf.size;

        if (retCode == 0)
        {
            // EOS condition reached on encode - send an event, set a flag
            // and terminate
            ret = FLOW_OK;
            STREAMER_LOG_DEBUG(DBG_CCID, "[encoder] EOS\n");

            element->eos_done = true;

            StreamEvent event;
            event_create_eos(&event);
            pad_push_event(pad, &event);

            goto Error;
        }
        else if (retCode == CEI_ENCODER_ENCODEERR)
        {
            // Encoder failure condition reached
            ret = FLOW_ERROR;
            STREAMER_LOG_ERR(DBG_CCID, ERRCODE_INTERNAL, "[encoder] Encoder error\n");

            goto Error;
        }

        // Encode succeeded, advance the output buffer location (otherwise we
        // would overwrite already written data)
        element->outOffset += retCode;

        // Construct a full output buffer with limited to the size of actually
        // encoded data
        // FIXME: Buffer time?
        StreamBuffer resultBuf = {
            .buffer = (int8_t *)element->outBuf.ptr, .size = retCode + sizeof(RawPacketHeader), .offset = 0, .time = 0};

        // Write (push) the data out
        ret = pad_push((StreamPad *)ELEMENT_SRC_PAD(element, 0), &resultBuf);
        if (ret != FLOW_OK)
        {
            goto Error;
        }
    }
    else
    {
        /* After "EOS" is hit, continuously executing streamer task is delayed
        to make sure that the player task gets some time slice to handle its
        pending messages */
        OSA_TimeDelay(10);
    }

Error:
    STREAMER_FUNC_EXIT(DBG_CCID);
    return (int32_t)ret;
}
