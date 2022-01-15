#ifndef __CEI_ENCTYPES_H__
#define __CEI_ENCTYPES_H__

typedef enum {
    CEIENC_OPUS,
    CEIENC_LAST
} CeiEncoderType;

typedef struct {
    uint32_t         sample_rate;
    uint8_t          num_channels;
    uint8_t          endian;
    uint8_t          sign;
    uint8_t          sample_size;
    uint8_t          interleaved;
} CeiBitstreamInfo;

typedef struct {
    int32_t complexity;
    int32_t bitrate;
    int32_t vbr;
    int32_t vbrConstraint;
    int32_t forceChannels;
    int32_t maxBandwidth;
    int32_t bandwidth;
    int32_t application;
    int32_t inbandFec;
    int32_t packetLossPerc;
    int32_t dtx;
    int32_t lsbDepth;
    int32_t expertFrameDuration;
    int32_t predictionDisabled;
} CeiOpusConfig;

#endif