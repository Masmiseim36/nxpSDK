/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifdef OPUS_ENCODE

#include "cei.h"
#include "cei_enctypes.h"

#include "opusenc_cei.h"

#include "opus.h"
#include "opus_types.h"

// (u)int32_t little-endian to big-endian conversion macro
#define LEBE32(x) (((x >> 24) & 0xff) | ((x << 8) & 0xff0000) | ((x >> 8) & 0xff00) | ((x << 24) & 0xff000000))

// This described a header of each Opus packet
typedef struct
{
    int32_t len;
    uint32_t entropy;
} OpusPacketHeader;

// This holds additional information about the encoder
// (members of the OpusEncoder struct are not accessible, as it is not defined
// as a complete type in the headers)
//
// Beware, it must be aligned, otherwise accesses to the encoder state will be
// unaligned.
typedef struct
{
    uint32_t channels;
} OpusEncoderInfo;

int OPUSEncoderGetMemorySize(uint8_t channels)
{
    // Allocating OpusEncoderInfo and the OpusEncoder struct
    return sizeof(OpusEncoderInfo) + opus_encoder_get_size(channels);
}

int OPUSEncoderInit(void *memory, uint32_t sampleRate, uint8_t channels)
{
    // Return value
    int ret;

    // Cast the memory and initialize the additional info struct
    OpusEncoderInfo *encInfo = (OpusEncoderInfo *)memory;
    encInfo->channels        = (uint32_t)channels;

    // Cast the memory and initialize the encoder state, its struct follows the previous one
    OpusEncoder *enc = (OpusEncoder *)((uint8_t *)memory + sizeof(OpusEncoderInfo));

    // Initialize the decoder state
    ret = opus_encoder_init(enc, sampleRate, channels, OPUS_APPLICATION_AUDIO);
    if (ret != OPUS_OK)
        goto end;

end:
    return ret;
}

int OPUSEncoderGetConfig(void *memory, void *config)
{
    CeiOpusConfig cfg = {0};
    int ret;

    if (memory == NULL)
    {
        cfg = (CeiOpusConfig){.complexity          = 0,
                              .bitrate             = OPUS_AUTO,
                              .vbr                 = 1,
                              .vbrConstraint       = 1,
                              .forceChannels       = OPUS_AUTO,
                              .maxBandwidth        = OPUS_BANDWIDTH_FULLBAND,
                              .bandwidth           = OPUS_BANDWIDTH_FULLBAND,
                              .application         = OPUS_APPLICATION_AUDIO,
                              .inbandFec           = 0,
                              .packetLossPerc      = 0,
                              .dtx                 = 0,
                              .lsbDepth            = 24,
                              .expertFrameDuration = OPUS_FRAMESIZE_ARG,
                              .predictionDisabled  = 0};
    }
    else
    {
        // Cast memory
        OpusEncoder *enc = (OpusEncoder *)((uint8_t *)memory + sizeof(OpusEncoderInfo));

        ret = opus_encoder_ctl(enc, OPUS_GET_COMPLEXITY(&cfg.complexity));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_BITRATE(&cfg.bitrate));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_VBR(&cfg.vbr));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_VBR_CONSTRAINT(&cfg.vbrConstraint));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_FORCE_CHANNELS(&cfg.forceChannels));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_MAX_BANDWIDTH(&cfg.maxBandwidth));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_BANDWIDTH(&cfg.bandwidth));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_APPLICATION(&cfg.application));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_INBAND_FEC(&cfg.inbandFec));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_PACKET_LOSS_PERC(&cfg.packetLossPerc));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_DTX(&cfg.dtx));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_LSB_DEPTH(&cfg.lsbDepth));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_EXPERT_FRAME_DURATION(&cfg.expertFrameDuration));
        if (ret != OPUS_OK)
            goto err;

        ret = opus_encoder_ctl(enc, OPUS_GET_PREDICTION_DISABLED(&cfg.predictionDisabled));
        if (ret != OPUS_OK)
            goto err;
    }

    *(CeiOpusConfig *)config = cfg;
    return 0;

err:
    return -1;
}

int OPUSEncoderSetConfig(void *memory, void *config)
{
    int ret;

    CeiOpusConfig *cfg = (CeiOpusConfig *)config;
    OpusEncoder *enc   = (OpusEncoder *)((uint8_t *)memory + sizeof(OpusEncoderInfo));

    ret = opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(cfg->complexity));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_BITRATE(cfg->bitrate));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_VBR(cfg->vbr));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_VBR_CONSTRAINT(cfg->vbrConstraint));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_FORCE_CHANNELS(cfg->forceChannels));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_MAX_BANDWIDTH(cfg->maxBandwidth));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_BANDWIDTH(cfg->bandwidth));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_APPLICATION(cfg->application));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_INBAND_FEC(cfg->inbandFec));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_PACKET_LOSS_PERC(cfg->packetLossPerc));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_DTX(cfg->dtx));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_LSB_DEPTH(cfg->lsbDepth));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_EXPERT_FRAME_DURATION(cfg->expertFrameDuration));
    if (ret != OPUS_OK)
        goto err;

    ret = opus_encoder_ctl(enc, OPUS_SET_PREDICTION_DISABLED(cfg->predictionDisabled));
    if (ret != OPUS_OK)
        goto err;

    return 0;

err:
    return -1;
}

int OPUSEncoderEncode(void *memory, BufferInfo *source, BufferInfo *dest)
{
    // Cast memory
    OpusEncoderInfo *encInfo = (OpusEncoderInfo *)memory;
    OpusEncoder *enc         = (OpusEncoder *)((uint8_t *)memory + sizeof(OpusEncoderInfo));

    // Encode
    int ret =
        opus_encode(enc, source->ptr,
                    source->size / encInfo->channels / sizeof(opus_int16), // Number of samples in the source buffer
                    (uint8_t *)dest->ptr + sizeof(OpusPacketHeader), // Payload is offset because of the packet header
                    dest->size - sizeof(OpusPacketHeader) // Maximum payload size does not contain the packet header
        );

    // Lower than zero, that designates an error
    if (ret < 0)
        return CEI_ENCODER_ENCODEERR;

    // No one documented this and previous iterations were just copypasta, so
    // let's document the following PROPERLY.
    //
    // In order to stay compatible with the reference decoder
    // (https://github.com/xiph/opus/blob/master/src/opus_demo.c), every encoded
    // packet needs to have a header, which contains the payload length and
    // encoder's entropy.
    //
    // The header is 8 bytes in length and its layout is as follows:
    //
    // |----------------|----------------|
    // | LEN (int32 BE) | ENT (int32 BE) |
    // |----------------|----------------|
    // |     4 bytes    |     4 bytes    |
    // |----------------|----------------|

    // Header is at the beginning of the packet
    OpusPacketHeader *hdr = (OpusPacketHeader *)dest->ptr;
    hdr->len              = LEBE32(ret); // Output is big endian

    // Get the entropy from the encoder and save it
    uint32_t hdrEntropy = 0;
    opus_encoder_ctl(enc, OPUS_GET_FINAL_RANGE(&hdrEntropy));
    hdr->entropy = LEBE32(hdrEntropy); // Output is big endian

    // We produced (header + payload length) bytes
    return sizeof(OpusPacketHeader) + ret;
}

#endif /* OPUS_ENCODE */
