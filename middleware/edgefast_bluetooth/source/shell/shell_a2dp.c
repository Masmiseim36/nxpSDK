/** @file
 * @brief Bluetooth A2DP shell module
 *
 * Provide some Bluetooth shell commands that can be useful to applications.
 */

/*
 * Copyright (c) 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno/errno.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/byteorder.h>
#include <porting.h>


#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp_codec_sbc.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/sdp.h>

#include "fsl_shell.h"

#include "shell_bt.h"

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
#if (defined(CONFIG_BT_A2DP) && ((CONFIG_BT_A2DP) > 0U))

struct bt_a2dp_ep_test {
    struct bt_a2dp_endpoint *ep;
    struct bt_delayed_work consume_stream_work;
    uint8_t media_started;
    uint8_t fisrt_media;
    uint8_t delay_ms;
};

#define MAX_REGISTERED_EP (10)
struct bt_a2dp *default_a2dp;
static uint8_t a2dp_sink_sdp_registered;
static uint8_t a2dp_source_sdp_registered;
static uint8_t a2dp_initied;
static struct bt_a2dp_ep_test *default_ep;
static uint8_t registered_index;
static struct bt_a2dp_ep_test registered_eps[MAX_REGISTERED_EP];
static uint8_t media_data[] = {
0x0,  0x0,  0x0,  0x0, 0xb5, 0x10, 0xb5, 0x10, 0x21, 0x21, 0x21, 0x21, 0xfb, 0x30, 0xfb, 0x30,
0xff, 0x3f, 0xff, 0x3f, 0xeb, 0x4d, 0xeb, 0x4d, 0x82, 0x5a, 0x82, 0x5a, 0x8c, 0x65, 0x8c, 0x65,
0xd9, 0x6e, 0xd9, 0x6e, 0x41, 0x76, 0x41, 0x76, 0xa2, 0x7b, 0xa2, 0x7b, 0xe7, 0x7e, 0xe7, 0x7e,
0xff, 0x7f, 0xff, 0x7f, 0xe7, 0x7e, 0xe7, 0x7e, 0xa2, 0x7b, 0xa2, 0x7b, 0x41, 0x76, 0x41, 0x76,
0xd9, 0x6e, 0xd9, 0x6e, 0x8c, 0x65, 0x8c, 0x65, 0x82, 0x5a, 0x82, 0x5a, 0xeb, 0x4d, 0xeb, 0x4d,
0xff, 0x3f, 0xff, 0x3f, 0xfb, 0x30, 0xfb, 0x30, 0x21, 0x21, 0x21, 0x21, 0xb5, 0x10, 0xb5, 0x10,
0x0,  0x0,  0x0,  0x0, 0x4b, 0xef, 0x4b, 0xef, 0xdf, 0xde, 0xdf, 0xde,  0x5, 0xcf,  0x5, 0xcf,
0x0, 0xc0,  0x0, 0xc0, 0x15, 0xb2, 0x15, 0xb2, 0x7e, 0xa5, 0x7e, 0xa5, 0x74, 0x9a, 0x74, 0x9a,
0x27, 0x91, 0x27, 0x91, 0xbf, 0x89, 0xbf, 0x89, 0x5e, 0x84, 0x5e, 0x84, 0x19, 0x81, 0x19, 0x81,
0x1, 0x80,  0x1, 0x80, 0x19, 0x81, 0x19, 0x81, 0x5e, 0x84, 0x5e, 0x84, 0xbf, 0x89, 0xbf, 0x89,
0x27, 0x91, 0x27, 0x91, 0x74, 0x9a, 0x74, 0x9a, 0x7e, 0xa5, 0x7e, 0xa5, 0x15, 0xb2, 0x15, 0xb2,
0x0, 0xc0,  0x0, 0xc0,  0x5, 0xcf,  0x5, 0xcf, 0xdf, 0xde, 0xdf, 0xde, 0x4b, 0xef, 0x4b, 0xef,
};

static struct bt_sdp_attribute a2dp_sink_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_AUDIO_SINK_SVCLASS) //11 0B
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),//35 10
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL) // 00 19
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL) // 00 19
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0X0100u) //AVDTP version: 01 00
            },
            )
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_ADVANCED_AUDIO_SVCLASS) //11 0d
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0103U) //01 03
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("A2DPSink"),
    BT_SDP_SUPPORTED_FEATURES(0x0001U),
};

static struct bt_sdp_record a2dp_sink_rec = BT_SDP_RECORD(a2dp_sink_attrs);

static struct bt_sdp_attribute a2dp_source_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
            BT_SDP_ARRAY_16(BT_SDP_AUDIO_SOURCE_SVCLASS)
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL)
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(0X0100u)
            },
            )
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_SDP_ADVANCED_AUDIO_SVCLASS)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(0x0103U)
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("A2DPSink"),
    BT_SDP_SUPPORTED_FEATURES(0x0001U),
};

static struct bt_sdp_record a2dp_source_rec = BT_SDP_RECORD(a2dp_source_attrs);

#define MPEG_1_2_MPF_1                          0x00U
#define MPEG_1_2_MPF_2                          0x40U

#define MPEG_1_2_VBR_ON                         0x80U
#define MPEG_1_2_VBR_OFF                        0x00U

#define MPEG_1_2_SAMPLING_FREQUENCY_16000       0x20U
#define MPEG_1_2_SAMPLING_FREQUENCY_22050       0x10U
#define MPEG_1_2_SAMPLING_FREQUENCY_24000       0x08U
#define MPEG_1_2_SAMPLING_FREQUENCY_32000       0x04U
#define MPEG_1_2_SAMPLING_FREQUENCY_44100       0x02U
#define MPEG_1_2_SAMPLING_FREQUENCY_48000       0x01U

#define MPEG_1_2_CHANNEL_MONO                   0x08U
#define MPEG_1_2_CHANNEL_DUAL                   0x04U
#define MPEG_1_2_CHANNEL_STEREO                 0x02U
#define MPEG_1_2_CHANNEL_JOINT_STEREO           0x01U

#define MPEG_1_2_LAYER_I                        0x80U
#define MPEG_1_2_LAYER_II                       0x40U
#define MPEG_1_2_LAYER_III                      0x20U

#define MPEG_1_2_CRC_OFF                         0x00U
#define MPEG_1_2_CRC_ON                          0x10U

#define MPEG_1_2_BITRATE_INDEX_0                0x0001U
#define MPEG_1_2_BITRATE_INDEX_1                0x0002U
#define MPEG_1_2_BITRATE_INDEX_2                0x0004U
#define MPEG_1_2_BITRATE_INDEX_3                0x0008U
#define MPEG_1_2_BITRATE_INDEX_4                0x0010U
#define MPEG_1_2_BITRATE_INDEX_5                0x0020U
#define MPEG_1_2_BITRATE_INDEX_6                0x0040U
#define MPEG_1_2_BITRATE_INDEX_7                0x0080U
#define MPEG_1_2_BITRATE_INDEX_8                0x0100U
#define MPEG_1_2_BITRATE_INDEX_9                0x0200U
#define MPEG_1_2_BITRATE_INDEX_10               0x0400U
#define MPEG_1_2_BITRATE_INDEX_11               0x0800U
#define MPEG_1_2_BITRATE_INDEX_12               0x1000U
#define MPEG_1_2_BITRATE_INDEX_13               0x2000U
#define MPEG_1_2_BITRATE_INDEX_14               0x4000U

static uint8_t mpeg12_endpoint_cap[5] =
{
    4U,                           
    /* Layer, CRC, Channel Mode */
    (MPEG_1_2_LAYER_I             |
    MPEG_1_2_LAYER_II            |
    MPEG_1_2_LAYER_III           |
    MPEG_1_2_CRC_ON              |
    MPEG_1_2_CHANNEL_MONO        |
    MPEG_1_2_CHANNEL_DUAL        |
    MPEG_1_2_CHANNEL_STEREO      |
    MPEG_1_2_CHANNEL_JOINT_STEREO),
    /* Media Payload Format(MPF), Sampling Frequency */
    (MPEG_1_2_MPF_1                    |
    MPEG_1_2_SAMPLING_FREQUENCY_16000 |
    MPEG_1_2_SAMPLING_FREQUENCY_22050 |
    MPEG_1_2_SAMPLING_FREQUENCY_24000 |
    MPEG_1_2_SAMPLING_FREQUENCY_32000 |
    MPEG_1_2_SAMPLING_FREQUENCY_44100 |
    MPEG_1_2_SAMPLING_FREQUENCY_48000),
    /* Form Supported Bit Rate indexes */
    ((MPEG_1_2_BITRATE_INDEX_0  |
    MPEG_1_2_BITRATE_INDEX_1  |
    MPEG_1_2_BITRATE_INDEX_2  |
    MPEG_1_2_BITRATE_INDEX_3  |
    MPEG_1_2_BITRATE_INDEX_4  |
    MPEG_1_2_BITRATE_INDEX_5  |
    MPEG_1_2_BITRATE_INDEX_6  |
    MPEG_1_2_BITRATE_INDEX_7  |
    MPEG_1_2_BITRATE_INDEX_8  |
    MPEG_1_2_BITRATE_INDEX_9  |
    MPEG_1_2_BITRATE_INDEX_10 |
    MPEG_1_2_BITRATE_INDEX_11 |
    MPEG_1_2_BITRATE_INDEX_12 |
    MPEG_1_2_BITRATE_INDEX_13 |
    MPEG_1_2_BITRATE_INDEX_14) >> 8U) | MPEG_1_2_VBR_ON,
    (uint8_t)(MPEG_1_2_BITRATE_INDEX_0  |
    MPEG_1_2_BITRATE_INDEX_1  |
    MPEG_1_2_BITRATE_INDEX_2  |
    MPEG_1_2_BITRATE_INDEX_3  |
    MPEG_1_2_BITRATE_INDEX_4  |
    MPEG_1_2_BITRATE_INDEX_5  |
    MPEG_1_2_BITRATE_INDEX_6  |
    MPEG_1_2_BITRATE_INDEX_7  |
    MPEG_1_2_BITRATE_INDEX_8  |
    MPEG_1_2_BITRATE_INDEX_9  |
    MPEG_1_2_BITRATE_INDEX_10 |
    MPEG_1_2_BITRATE_INDEX_11 |
    MPEG_1_2_BITRATE_INDEX_12 |
    MPEG_1_2_BITRATE_INDEX_13 |
    MPEG_1_2_BITRATE_INDEX_14)
};

static uint8_t mpeg12_endpoint_config[5] =
{
    4U,                           
    /* Layer, CRC, Channel Mode */
    (MPEG_1_2_LAYER_I             |
    MPEG_1_2_CRC_ON              |
    MPEG_1_2_CHANNEL_JOINT_STEREO),
    /* Media Payload Format(MPF), Sampling Frequency */
    (MPEG_1_2_MPF_1                    |
    MPEG_1_2_SAMPLING_FREQUENCY_44100),
    /* Form Supported Bit Rate indexes */
    ((MPEG_1_2_BITRATE_INDEX_0) >> 8U) | MPEG_1_2_VBR_ON,
    (MPEG_1_2_BITRATE_INDEX_0)
};

#define MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC          0x80U
#define MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LC          0x40U
#define MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LTP         0x20U
#define MPEG_2_4_OBJ_TYPE_MPEG4_AAC_SCALABLE    0x10U

#define MPEG_2_4_SAMPLING_FREQUENCY_8000        0x8000U
#define MPEG_2_4_SAMPLING_FREQUENCY_11025       0x4000U
#define MPEG_2_4_SAMPLING_FREQUENCY_12000       0x2000U
#define MPEG_2_4_SAMPLING_FREQUENCY_16000       0x1000U
#define MPEG_2_4_SAMPLING_FREQUENCY_22050       0x0800U
#define MPEG_2_4_SAMPLING_FREQUENCY_24000       0x0400U
#define MPEG_2_4_SAMPLING_FREQUENCY_32000       0x0200U
#define MPEG_2_4_SAMPLING_FREQUENCY_44100       0x0100U
#define MPEG_2_4_SAMPLING_FREQUENCY_48000       0x0080U
#define MPEG_2_4_SAMPLING_FREQUENCY_64000       0x0040U
#define MPEG_2_4_SAMPLING_FREQUENCY_88200       0x0020U
#define MPEG_2_4_SAMPLING_FREQUENCY_96000       0x0010U

#define MPEG_2_4_CHANNELS_1                     0x08U
#define MPEG_2_4_CHANNELS_2                     0x04U

#define MPEG_2_4_BITRATE_UNKNOWN               0x0000U
#define MPEG_2_4_BITRATE_CONFIG                320000U

#define MPEG_2_4_VBR_ON                         0x80U
#define MPEG_2_4_VBR_OFF                        0x00U

static uint8_t mpeg24_endpoint_cap[7] =
{
    6,
    /* MPEG_2_4 Object Type  */
    MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC,
    (MPEG_2_4_SAMPLING_FREQUENCY_44100 |
     MPEG_2_4_SAMPLING_FREQUENCY_48000 |
     MPEG_2_4_CHANNELS_1  |
     MPEG_2_4_CHANNELS_2) >> 8U,
    (uint8_t)(MPEG_2_4_SAMPLING_FREQUENCY_44100 |
     MPEG_2_4_SAMPLING_FREQUENCY_48000 |
     MPEG_2_4_CHANNELS_1  |
     MPEG_2_4_CHANNELS_2),
     (MPEG_2_4_BITRATE_UNKNOWN >> 16U | MPEG_2_4_VBR_ON),
     (MPEG_2_4_BITRATE_UNKNOWN >> 8U),
     MPEG_2_4_BITRATE_UNKNOWN
};

static uint8_t mpeg24_endpoint_config[7] =
{
    6,
    /* MPEG_2_4 Object Type  */
    MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC,
    (MPEG_2_4_SAMPLING_FREQUENCY_44100 |
     MPEG_2_4_CHANNELS_2) >> 8U,
    (uint8_t)(MPEG_2_4_SAMPLING_FREQUENCY_44100 |
     MPEG_2_4_CHANNELS_2),
     (MPEG_2_4_BITRATE_CONFIG >> 16U | MPEG_2_4_VBR_ON),
     (uint8_t)(MPEG_2_4_BITRATE_CONFIG >> 8U),
     (uint8_t)MPEG_2_4_BITRATE_CONFIG
};

#define VENDOR_NXP 0x00001fc9
#define VENDOR_CODEC 0x0001
#define VENDOR_SPECIFIC_VAL 0x01
static uint8_t vendor_endpoint_cap[8] =
{
    7,
    /* MPEG_2_4 Object Type  */
    (uint8_t)VENDOR_NXP,
    VENDOR_NXP >> 8U,
    VENDOR_NXP >> 16U,
    VENDOR_NXP >> 24U,
    VENDOR_CODEC,
    VENDOR_CODEC >> 8U,
    VENDOR_SPECIFIC_VAL
};

static uint8_t vendor_endpoint_config[8] =
{
    7,
    /* MPEG_2_4 Object Type  */
    (uint8_t)VENDOR_NXP,
    VENDOR_NXP >> 8U,
    VENDOR_NXP >> 16U,
    VENDOR_NXP >> 24U,
    VENDOR_CODEC,
    VENDOR_CODEC >> 8U,
    VENDOR_SPECIFIC_VAL
};

BT_A2DP_SBC_SINK_ENDPOINT(sink_sbc_endpoint);
BT_A2DP_SBC_SOURCE_ENDPOINT(source_sbc_endpoint, A2DP_SBC_SAMP_FREQ_44100);
struct bt_a2dp_endpoint sink_mpeg12_endpoint =
    BT_A2DP_SINK_ENDPOINT_INIT(BT_A2DP_MPEG1, &mpeg12_endpoint_cap[0], NULL, NULL);
struct bt_a2dp_endpoint source_mpeg12_endpoint =
    BT_A2DP_SOURCE_ENDPOINT_INIT(BT_A2DP_MPEG1, &mpeg12_endpoint_cap[0],
                             &mpeg12_endpoint_config[0], NULL, NULL);
struct bt_a2dp_endpoint sink_mpeg24_endpoint =
    BT_A2DP_SINK_ENDPOINT_INIT(BT_A2DP_MPEG2, &mpeg24_endpoint_cap[0], NULL, NULL);
struct bt_a2dp_endpoint source_mpeg24_endpoint =
BT_A2DP_SOURCE_ENDPOINT_INIT(BT_A2DP_MPEG2, &mpeg24_endpoint_cap[0],
                             &mpeg24_endpoint_config[0], NULL, NULL);
struct bt_a2dp_endpoint sink_vendor_endpoint =
BT_A2DP_SINK_ENDPOINT_INIT(BT_A2DP_VENDOR, &vendor_endpoint_cap[0], NULL, NULL);
struct bt_a2dp_endpoint source_vendor_endpoint =
BT_A2DP_SOURCE_ENDPOINT_INIT(BT_A2DP_VENDOR, &vendor_endpoint_cap[0],
                             &vendor_endpoint_config[0], NULL, NULL);

static void shell_a2dp_print_capabilities(struct bt_a2dp_endpoint *endpoint)
{
    uint8_t codec_type;
    uint8_t *codec_ie;
    uint16_t codec_ie_len;

    codec_type = endpoint->codec_id;
    codec_ie = endpoint->capabilities->codec_ie;
    codec_ie_len = endpoint->capabilities->len;
    if (BT_A2DP_SBC == codec_type)
    {
        shell_print(ctx_shell, "SBC%s%s",
            (endpoint->info.sep.tsep == BT_A2DP_SINK) ? "(sink)" : "(source)",
            (endpoint->info.sep.inuse) ? "(in use)" : "(idle)");

        if (BT_A2DP_SBC_IE_LENGTH != codec_ie_len)
        {
            shell_error(ctx_shell, "  wrong sbc codec ie");
            return;
        }

        shell_print(ctx_shell, "  sample frequency:");
        if (0U != (codec_ie[0U] & A2DP_SBC_SAMP_FREQ_16000))
        {
            shell_print(ctx_shell, "    16000 ");
        }
        if (0U != (codec_ie[0U] & A2DP_SBC_SAMP_FREQ_32000))
        {
            shell_print(ctx_shell, "    32000 ");
        }
        if (0U != (codec_ie[0U] & A2DP_SBC_SAMP_FREQ_44100))
        {
            shell_print(ctx_shell, "    44100 ");
        }
        if (0U != (codec_ie[0U] & A2DP_SBC_SAMP_FREQ_48000))
        {
            shell_print(ctx_shell, "    48000");
        }

        shell_print(ctx_shell, "  channel mode:");
        if (0U != (codec_ie[0U] & A2DP_SBC_CH_MODE_MONO))
        {
            shell_print(ctx_shell, "    Mono ");
        }
        if (0U != (codec_ie[0U] & A2DP_SBC_CH_MODE_DUAL))
        {
            shell_print(ctx_shell, "    Dual ");
        }
        if (0U != (codec_ie[0U] & A2DP_SBC_CH_MODE_STREO))
        {
            shell_print(ctx_shell, "    Stereo ");
        }
        if (0U != (codec_ie[0U] & A2DP_SBC_CH_MODE_JOINT))
        {
            shell_print(ctx_shell, "    Joint-Stereo");
        }

         /* Decode Support for Block Length */
        shell_print(ctx_shell, "  Block Length:");
        if (0U != (codec_ie[1U] & A2DP_SBC_BLK_LEN_4))
        {
            shell_print(ctx_shell, "    4 ");
        }
        if (0U != (codec_ie[1U] & A2DP_SBC_BLK_LEN_8))
        {
            shell_print(ctx_shell, "    8 ");
        }
        if (0U != (codec_ie[1U] & A2DP_SBC_BLK_LEN_12))
        {
            shell_print(ctx_shell, "    12 ");
        }
        if (0U != (codec_ie[1U] & A2DP_SBC_BLK_LEN_16))
        {
            shell_print(ctx_shell, "    16");
        }

        /* Decode Support for Subbands */
        shell_print(ctx_shell, "  Subbands:");
        if (0U != (codec_ie[1U] & A2DP_SBC_SUBBAND_4))
        {
            shell_print(ctx_shell, "    4 ");
        }
        if (0U != (codec_ie[1U] & A2DP_SBC_SUBBAND_8))
        {
            shell_print(ctx_shell, "    8");
        }

        /* Decode Support for Allocation Method */
        shell_print(ctx_shell, "  Allocation Method:");
        if (0U != (codec_ie[1U] & A2DP_SBC_ALLOC_MTHD_SNR))
        {
            shell_print(ctx_shell, "    SNR ");
        }
        if (0U != (codec_ie[1U] & A2DP_SBC_ALLOC_MTHD_LOUDNESS))
        {
            shell_print(ctx_shell, "    Loudness");
        }

        shell_print(ctx_shell, "  Bitpool Range: %d - %d",
                    codec_ie[2U], codec_ie[3U]);

#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
		if ((endpoint->cp_config != NULL) && (endpoint->cp_ie_count > 0u)) {
			shell_print(ctx_shell, "  content protection service");
		}
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
		if (endpoint->recovery_ie != NULL) {
			shell_print(ctx_shell, "  recovery service");
		}
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
		if (endpoint->reporting_service_enable) {
			shell_print(ctx_shell, "  reporting service");
		}
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
		if (endpoint->delay_reporting_service_enable) {
			shell_print(ctx_shell, "  delay reporting service");
		}
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
		if (endpoint->header_compression_cap) {
			shell_print(ctx_shell, "  header compression service");
		}
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
		if (endpoint->multiplexing_service_enable) {
			shell_print(ctx_shell, "  multiplexing service");
		}
#endif
    }
    else if (BT_A2DP_MPEG1 == codec_type)
    {
        shell_print(ctx_shell, "MPEG-1,2%s%s",
            (endpoint->info.sep.tsep == BT_A2DP_SINK) ? "(sink)" : "(source)",
            (endpoint->info.sep.inuse) ? "(in use)" : "(idle)");

        if (BT_A2DP_MPEG_1_2_IE_LENGTH != codec_ie_len)
        {
            shell_error(ctx_shell, "  wrong mpeg1,2 codec ie");
            return;
        }
        else
        {
            shell_print(ctx_shell, "  Layer:");
            if (0U != (codec_ie[0U]& MPEG_1_2_LAYER_I))
            {
                shell_print(ctx_shell, "    I ");
            }
            if (0U != (codec_ie[0U]& MPEG_1_2_LAYER_II))
            {
                shell_print(ctx_shell, "    II ");
            }
            if (0U != (codec_ie[0U]& MPEG_1_2_LAYER_III))
            {
                shell_print(ctx_shell, "    III ");
            }

            shell_print(ctx_shell, "  CRC:");
            if (0U != (codec_ie[0U]& 0x10U))
            {
                shell_print(ctx_shell, "    ON");
            }
            else
            {
                shell_print(ctx_shell, "    OFF");
            }

            shell_print(ctx_shell, "  Channel Mode:");
            if (0U != (codec_ie[0U] & MPEG_1_2_CHANNEL_MONO))
            {
                shell_print(ctx_shell, "    Mono ");
            }
            if (0U != (codec_ie[0U] & MPEG_1_2_CHANNEL_STEREO))
            {
                shell_print(ctx_shell, "    Strereo ");
            }
            if (0U != (codec_ie[0U] & MPEG_1_2_CHANNEL_DUAL))
            {
                shell_print(ctx_shell, "    Dual ");
            }
            if (0U != (codec_ie[0U] & MPEG_1_2_CHANNEL_JOINT_STEREO))
            {
                shell_print(ctx_shell, "    Joint Stereo ");
            }

            shell_print(ctx_shell, "  MPF:");
            if(0U != (codec_ie[1U]& 0x40U))
            {
                shell_print(ctx_shell, "    MPF-2");
            }
            else
            {
                shell_print(ctx_shell, "    MPF-1");
            }

            shell_print(ctx_shell, "  Sampling Frequency:");
            if (0U != (codec_ie[1U] & MPEG_1_2_SAMPLING_FREQUENCY_16000))
            {
                shell_print(ctx_shell, "    16000 ");
            }
            if (0U != (codec_ie[1U] & MPEG_1_2_SAMPLING_FREQUENCY_22050))
            {
                shell_print(ctx_shell, "    22050 ");
            }
            if (0U != (codec_ie[1U] & MPEG_1_2_SAMPLING_FREQUENCY_24000))
            {
                shell_print(ctx_shell, "    24000 ");
            }
            if (0U != (codec_ie[1U] & MPEG_1_2_SAMPLING_FREQUENCY_32000))
            {
                shell_print(ctx_shell, "    32000 ");
            }
            if (0U != (codec_ie[1U] & MPEG_1_2_SAMPLING_FREQUENCY_44100))
            {
                shell_print(ctx_shell, "    44100 ");
            }
            if (0U != (codec_ie[1U] & MPEG_1_2_SAMPLING_FREQUENCY_48000))
            {
                shell_print(ctx_shell, "    48000");
            }

            shell_print(ctx_shell, "  VBR:");
            if (0U != (codec_ie[2U]& 0x80U))
            {
                shell_print(ctx_shell, "    ON");
            }
            else
            {
                shell_print(ctx_shell, "    OFF");
            }

            shell_print(ctx_shell, "  Bit Rate Index = 0x%04x", (((codec_ie[2U] & 0x7FU) << 8U) | codec_ie[3U]));
        }
    }
    else if (BT_A2DP_MPEG2 == codec_type)
    {
        uint16_t sampling_rate;
        uint32_t bit_rate;

        shell_print(ctx_shell, "MPEG-2,4%s%s",
            (endpoint->info.sep.tsep == BT_A2DP_SINK) ? "(sink)" : "(source)",
            (endpoint->info.sep.inuse) ? "(in use)" : "(idle)");

        if (BT_A2DP_MPEG_2_4_IE_LENGTH != codec_ie_len)
        {
            shell_error(ctx_shell, "wrong mpeg2,4 codec ie");
            return;
        }
        else
        {
            shell_print(ctx_shell, "  Object Type:");
            if(0U != (codec_ie[0U] & MPEG_2_4_OBJ_TYPE_MPEG2_AAC_LC))
            {
                shell_print(ctx_shell, "    MPEG-2 AAC LC ");
            }
            if(0U != (codec_ie[0U] & MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LC))
            {
                shell_print(ctx_shell, "    MPEG-4 AAC LC ");
            }
            if(0U != (codec_ie[0U] & MPEG_2_4_OBJ_TYPE_MPEG4_AAC_LTP))
            {
                shell_print(ctx_shell, "    MPEG-4 AAC LTP ");
            }
            if(0U != (codec_ie[0U] & MPEG_2_4_OBJ_TYPE_MPEG4_AAC_SCALABLE))
            {
                shell_print(ctx_shell, "    MPEG-4 AAC SCALABLE");
            }

            sampling_rate = (uint16_t )codec_ie[1U];
            sampling_rate = ((sampling_rate << 8U) | (codec_ie[2U] & 0xF0U));

            /* Decode Sampling Frequency */
            shell_print(ctx_shell, "  Sampling Frequency:");
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_8000))
            {
                shell_print(ctx_shell, "    8000 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_11025))
            {
                shell_print(ctx_shell, "    11025 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_12000))
            {
                shell_print(ctx_shell, "    12000 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_16000))
            {
                shell_print(ctx_shell, "    16000 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_22050))
            {
                shell_print(ctx_shell, "    22050 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_24000))
            {
                shell_print(ctx_shell, "    24000 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_32000))
            {
                shell_print(ctx_shell, "    32000 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_44100))
            {
                shell_print(ctx_shell, "    44100 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_48000))
            {
                shell_print(ctx_shell, "    48000 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_64000))
            {
                shell_print(ctx_shell, "    64000 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_88200))
            {
                shell_print(ctx_shell, "    88200 ");
            }
            if (0U != (sampling_rate & MPEG_2_4_SAMPLING_FREQUENCY_96000))
            {
                shell_print(ctx_shell, "    96000");
            }

            /* Decode Channels */
            shell_print(ctx_shell, "  Channels:");
            if (0U != (codec_ie[2U] & MPEG_2_4_CHANNELS_1))
            {
                shell_print(ctx_shell, "    1");
            }
            if (0U != (codec_ie[2U] & MPEG_2_4_CHANNELS_2))
            {
                shell_print(ctx_shell, "    2");
            }

            /* Decode Variable Bit Rate */
            shell_print(ctx_shell, "  VBR:");
            if (0U != (codec_ie[3U]& 0x80U))
            {
                shell_print(ctx_shell, "    ON");
            }
            else
            {
                shell_print(ctx_shell, "    OFF");
            }

            /* Decode Bit Rate */
            bit_rate = 0U;
            bit_rate = codec_ie[3U] & 0x7FU;
            bit_rate = (bit_rate << 8U) | codec_ie[4U];
            bit_rate = (bit_rate << 8U) | codec_ie[5U];

            shell_print(ctx_shell, "  Bit Rate:");
            if (bit_rate)
            {
                shell_print(ctx_shell, "    %d", bit_rate);
            }
            else
            {
                shell_print(ctx_shell, "    Unknown");
            }
        }
    }
    else if (BT_A2DP_VENDOR == codec_type)
    {
        uint32_t vendor_id;
        uint16_t vendor_codec_id, rem_ie_len;

        shell_print(ctx_shell, "Vendor Specific%s%s",
            (endpoint->info.sep.tsep == BT_A2DP_SINK) ? "(sink)" : "(source)",
            (endpoint->info.sep.inuse) ? "(in use)" : "(idle)");

        if (codec_ie_len <= 6U)
        {
            shell_print(ctx_shell, "wrong vendor codec ie");
            return;
        }

        vendor_id = sys_get_le32(&codec_ie[0U]);
        shell_print(ctx_shell, "Vendor ID: 0x%08X", vendor_id);

        vendor_codec_id = sys_get_le16(&codec_ie[4]);
        shell_print(ctx_shell, "Vendor Codec ID: 0x%04X\n", vendor_codec_id);

        rem_ie_len = (codec_ie_len - 6U);

        shell_print(ctx_shell, "Vendor Codec Value(in Hex): ");
        if (0x00U != rem_ie_len)
        {
            for (uint8_t i = 0U; i < rem_ie_len; i++)
            {
                shell_print(ctx_shell, "0x%02X\n", codec_ie[6U+i]);
            }
        }
        else
        {
            shell_print(ctx_shell, "???\n");
        }
    }
}


static struct bt_a2dp_ep_test *get_a2dp_test(struct bt_a2dp_endpoint *ep)
{
    for (uint8_t index = 0; index < MAX_REGISTERED_EP; index++)
    {
        if (registered_eps[index].ep == ep)
        {
            return &registered_eps[index];
        }
    }

    return NULL;
}

static void reset_a2dp_test(void)
{
    for (uint8_t index = 0; index < MAX_REGISTERED_EP; index++)
    {
        registered_eps[index].media_started = 0;
        k_work_cancel_delayable(&registered_eps[index].consume_stream_work);
    }
}

void app_connected(struct bt_a2dp *a2dp, int err)
{
    if (!err)
    {
        default_a2dp = a2dp;
        shell_print(ctx_shell, "a2dp connected");
    }
    else
    {
        shell_print(ctx_shell, "a2dp connecting fail");
    }
}

void app_disconnected(struct bt_a2dp *a2dp)
{
    reset_a2dp_test();
    shell_print(ctx_shell, "a2dp disconnected");
}

static void consume_stream_work_cb(struct bt_work *work)
{
    struct bt_a2dp_ep_test *ep_test = CONTAINER_OF((work), struct bt_a2dp_ep_test, consume_stream_work);

    bt_a2dp_snk_media_sync(ep_test->ep, NULL, 0);
    if (ep_test->media_started) {
        k_work_schedule(&ep_test->consume_stream_work, BT_MSEC(ep_test->delay_ms));
    }
}

static void shell_a2dp_init(void)
{
    if (a2dp_initied == 0)
    {
	struct bt_a2dp_connect_cb cb;

        a2dp_initied = 1;
	cb.connected = app_connected;
	cb.disconnected = app_disconnected;

	bt_a2dp_register_connect_callback(&cb);
    }
}

static void sink_common_streamer_data(struct bt_a2dp_endpoint *ep, uint8_t *data, uint32_t length)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(ep);
    uint32_t temp;
    uint32_t *u32data;
    
    if (ep_test == NULL)
    {
        return;
    }
    
    temp = (uint32_t)data;
    temp = ((temp + 3u) & 0xFFFFFFFCu);
    u32data = (uint32_t*)temp;
    temp = ((uint32_t)(data + length) & 0xFFFFFFFCu);
    length = (uint32_t *)temp - u32data;

    for (temp = 0; temp < length; temp++) {
        if (u32data[temp] != 0U) {
            break;
        }
    }
    if (temp == length) {
        shell_print(ctx_shell, "streamer data:%d(silence)\r\n", length);
    } else {
        shell_print(ctx_shell, "streamer data:%d\r\n", length);
    }

    ep_test->delay_ms = (length / 44u / 4u);
    if (ep_test->media_started) {
        if (ep_test->fisrt_media == 0u) {
            ep_test->fisrt_media = 1u;
            k_work_schedule(&ep_test->consume_stream_work, BT_MSEC(ep_test->delay_ms));
        }
    }
}

void sink_sbc_streamer_data(uint8_t *data, uint32_t length)
{
    sink_common_streamer_data(&sink_sbc_endpoint, data, length);
}

void sink_mpeg12_streamer_data(uint8_t *data, uint32_t length)
{
    sink_common_streamer_data(&sink_mpeg12_endpoint, data, length);
}

void sink_mpeg24_streamer_data(uint8_t *data, uint32_t length)
{
    sink_common_streamer_data(&sink_mpeg24_endpoint, data, length);
}

void sink_vendor_streamer_data(uint8_t *data, uint32_t length)
{
    sink_common_streamer_data(&sink_vendor_endpoint, data, length);
}

void common_configured(struct bt_a2dp_ep_test *ep_test, struct bt_a2dp_endpoint_configure_result *configResult)
{
    if (configResult->err == 0) {
        shell_print(ctx_shell, "configure success");
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
        if (configResult->config.cp_config != NULL)
        {
            shell_print(ctx_shell, "content protection service is configured");
        }
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
        if (configResult->config.recovery_config != NULL)
        {
            shell_print(ctx_shell, "recovery service is configured");
        }
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
        if (configResult->config.delay_reporting_service_config != 0)
        {
            shell_print(ctx_shell, "delay reporting service is configured");
        }
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
        if (configResult->config.reporting_service_config != 0)
        {
            shell_print(ctx_shell, "reporting service is configured");
        }
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
        if (configResult->config.header_compression_config != 0)
        {
            shell_print(ctx_shell, "header compression service is configured");
        }
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
        if (configResult->config.multiplexing_service_config != 0)
        {
            shell_print(ctx_shell, "multiplexing service is configured");
        }
#endif
        shell_print(ctx_shell, "");
        default_ep = ep_test;
        shell_print(ctx_shell, "the default ep is set as the configured ep");
    } else {
        shell_print(ctx_shell, "configure err");
    }
}

void source_sbc_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
    common_configured(get_a2dp_test(&source_sbc_endpoint), configResult);
}

void source_mpeg12_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
    common_configured(get_a2dp_test(&source_mpeg12_endpoint), configResult);
}

void source_mpeg24_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
    common_configured(get_a2dp_test(&source_mpeg24_endpoint), configResult);
}

void source_vendor_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
    common_configured(get_a2dp_test(&source_vendor_endpoint), configResult);
}

void sink_sbc_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
    common_configured(get_a2dp_test(&sink_sbc_endpoint), configResult);
}

void sink_mpeg12_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
    common_configured(get_a2dp_test(&sink_mpeg12_endpoint), configResult);
}

void sink_mpeg24_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
    common_configured(get_a2dp_test(&sink_mpeg24_endpoint), configResult);
}

void sink_vendor_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
    common_configured(get_a2dp_test(&sink_vendor_endpoint), configResult);
}

void common_deconfigured(int err)
{
    shell_print(ctx_shell, "deconfigured");
}

void sbc_deconfigured(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);

    if ((ep_test != NULL) && (!err))
    {
        ep_test->media_started = 0;
    }
    common_deconfigured(err);
}

void mpeg12_deconfigured(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);

    if ((ep_test != NULL) && (!err))
    {
        ep_test->media_started = 0;
    }
    common_deconfigured(err);
}

void mpeg24_deconfigured(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);

    if ((ep_test != NULL) && (!err))
    {
        ep_test->media_started = 0;
    }
    common_deconfigured(err);
}

void vendor_deconfigured(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);

    if ((ep_test != NULL) && (!err))
    {
        ep_test->media_started = 0;
    }
    common_deconfigured(err);
}

static void common_start_play_internal(struct bt_a2dp_ep_test *ep_test, int err)
{
    if ((ep_test != NULL) && (!err))
    {
        ep_test->media_started = 1;
        ep_test->fisrt_media = 0;
    }

    if (err == 0) {
        shell_print(ctx_shell, "a2dp start playing");
    } else {
        shell_print(ctx_shell, "a2dp start fail");
    }
}

void common_start_play(int err)
{
    if (err == 0) {
        shell_print(ctx_shell, "a2dp start playing");
    } else {
        shell_print(ctx_shell, "a2dp start fail");
    }
}

void sbc_sink_start_play(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);

    common_start_play_internal(ep_test, err);
}

void mpeg12_sink_start_play(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_mpeg12_endpoint);
    
    common_start_play_internal(ep_test, err);
}

void mpeg24_sink_start_play(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_mpeg24_endpoint);
    
    common_start_play_internal(ep_test, err);
}

void vendor_sink_start_play(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_vendor_endpoint);
    
    common_start_play_internal(ep_test, err);
}

void common_stop_play_internal(struct bt_a2dp_ep_test *ep_test, int err)
{
    if ((ep_test != NULL) && (!err))
    {
        ep_test->media_started = 0;
    }

    if (err == 0) {
        shell_print(ctx_shell, "a2dp stop playing");
    } else {
        shell_print(ctx_shell, "a2dp stop fail");
    }
}

void common_stop_play(int err)
{
    if (err == 0) {
        shell_print(ctx_shell, "a2dp stop playing");
    } else {
        shell_print(ctx_shell, "a2dp stop fail");
    }
}

void sbc_stop_play(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);

    common_stop_play_internal(ep_test, err);
}

void mpeg12_stop_play(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);
    
    common_stop_play_internal(ep_test, err);
}

void mpeg24_stop_play(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);
    
    common_stop_play_internal(ep_test, err);
}

void vendor_stop_play(int err)
{
    struct bt_a2dp_ep_test *ep_test = get_a2dp_test(&sink_sbc_endpoint);
    
    common_stop_play_internal(ep_test, err);
}

void source_delay_report_callback(int err, int16_t delay)
{
    if (err == 0) {
        shell_print(ctx_shell, "received delay report:%d", delay);
    }
}

void sink_delay_report_callback(int err)
{
    if (err == 0) {
        shell_print(ctx_shell, "send delay report successfully");
    }
}

#define CONTENT_PROTECTION_INVALID                0x0000
#define CONTENT_PROTECTION_TYPE_DTCP              0x0001
#define CONTENT_PROTECTION_TYPE_SCMS_T            0x0002
#define RECOVERY_TYPE_RFC2733                     0x01
#define MIN_MRWS                                  0x01
#define MAX_MRWS                                  0x18
#define MIN_MNMP                                  0x01
#define MAX_MNMP                                  0x18

#define SCMS_T_COPY_UNLIMITED                     0x00
#define SCMS_T_COPY_NOT_USED                      0x01
#define SCMS_T_COPY_ONCE_ONLY                     0x02
#define SCMS_T_COPY_NOT_ALLOWED                   0x03
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
static uint8_t test_cp_ie[] = {2, (uint8_t)CONTENT_PROTECTION_TYPE_SCMS_T, (CONTENT_PROTECTION_TYPE_SCMS_T >> 8u)};
static uint8_t test_cp_config[] = {2, (uint8_t)CONTENT_PROTECTION_TYPE_SCMS_T, (CONTENT_PROTECTION_TYPE_SCMS_T >> 8u)};
uint8_t cp_header = SCMS_T_COPY_UNLIMITED;
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
static uint8_t test_recovery_ie[] = {3, RECOVERY_TYPE_RFC2733, MAX_MRWS, MAX_MNMP};
static uint8_t test_recovery_config[] = {3, RECOVERY_TYPE_RFC2733, MAX_MRWS, MAX_MNMP};
#endif

#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
static shell_status_t cmd_send_delay_report(shell_handle_t shell, int32_t argc, char *argv[])
{
    int err;
    int16_t delay;

    shell_a2dp_init();
    if (default_ep == NULL)
    {
        shell_print(shell, "default endpoint is not set");
        return kStatus_SHELL_Success;
    }

    if (argc > 1) {
        delay = strtoul(argv[1], NULL, 10);
    } else {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    err = bt_a2dp_send_delay_report(default_ep->ep, delay);
    if (err) {
        shell_print(shell, "sending fail");
    }

    return kStatus_SHELL_Success;
}
#endif

static shell_status_t cmd_register_sink_ep(shell_handle_t shell, int32_t argc, char *argv[])
{
    int select;
    int err = -1;
    struct bt_a2dp_endpoint *ep;

    shell_a2dp_init();
    if (a2dp_sink_sdp_registered == 0) {
        a2dp_sink_sdp_registered = 1;
        bt_sdp_register_service(&a2dp_sink_rec);
    }

    if (argc > 1) {
        select = strtoul(argv[1], NULL, 10);
    } else {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    if ((select <= 0) || (select > 6)) {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    switch (select) {
    case 1: /* SBC */
        ep = &sink_sbc_endpoint;
        sink_sbc_endpoint.control_cbs.configured = sink_sbc_configured;
        sink_sbc_endpoint.control_cbs.deconfigured = sbc_deconfigured;
        sink_sbc_endpoint.control_cbs.start_play = sbc_sink_start_play;
        sink_sbc_endpoint.control_cbs.stop_play = sbc_stop_play;
        sink_sbc_endpoint.control_cbs.sink_streamer_data = sink_sbc_streamer_data;
        err = bt_a2dp_register_endpoint(&sink_sbc_endpoint, BT_A2DP_AUDIO, BT_A2DP_SINK);
        if (!err)
        {
            shell_print(shell, "SBC sink endpoint is registered");
        }
        break;
    case 2: /* MPEG-1,2 */
        ep = &sink_mpeg12_endpoint;
        sink_mpeg12_endpoint.control_cbs.configured = sink_mpeg12_configured;
        sink_mpeg12_endpoint.control_cbs.deconfigured = mpeg12_deconfigured;
        sink_mpeg12_endpoint.control_cbs.start_play = mpeg12_sink_start_play;
        sink_mpeg12_endpoint.control_cbs.stop_play = mpeg12_stop_play;
        sink_mpeg12_endpoint.control_cbs.sink_streamer_data = sink_mpeg12_streamer_data;
        err = bt_a2dp_register_endpoint(&sink_mpeg12_endpoint, BT_A2DP_AUDIO, BT_A2DP_SINK);
        if (!err)
        {
            shell_print(shell, "MPEG-1,2 sink endpoint is registered");
        }
        break;
    case 3: /* 3:MPEG-2,4 */
        ep = &sink_mpeg24_endpoint;
        sink_mpeg24_endpoint.control_cbs.configured = sink_mpeg24_configured;
        sink_mpeg24_endpoint.control_cbs.deconfigured = mpeg24_deconfigured;
        sink_mpeg24_endpoint.control_cbs.start_play = mpeg24_sink_start_play;
        sink_mpeg24_endpoint.control_cbs.stop_play = mpeg24_stop_play;
        sink_mpeg24_endpoint.control_cbs.sink_streamer_data = sink_mpeg24_streamer_data;
        err = bt_a2dp_register_endpoint(&sink_mpeg24_endpoint, BT_A2DP_AUDIO, BT_A2DP_SINK);
        if (!err)
        {
            shell_print(shell, "MPEG-2,4 sink endpoint is registered");
        }
        break;
    case 4: /*  4:vendor */
        ep = &sink_vendor_endpoint;
        sink_vendor_endpoint.control_cbs.configured = sink_vendor_configured;
        sink_vendor_endpoint.control_cbs.deconfigured = vendor_deconfigured;
        sink_vendor_endpoint.control_cbs.start_play = vendor_sink_start_play;
        sink_vendor_endpoint.control_cbs.stop_play = vendor_stop_play;
        sink_vendor_endpoint.control_cbs.sink_streamer_data = sink_vendor_streamer_data;
        err = bt_a2dp_register_endpoint(&sink_vendor_endpoint, BT_A2DP_AUDIO, BT_A2DP_SINK);
        if (!err)
        {
            shell_print(shell, "Vendor sink endpoint is registered");
        }
        break;
    case 5: /* sbc with delay report and content protection services */
        ep = &sink_sbc_endpoint;
        sink_sbc_endpoint.control_cbs.configured = sink_sbc_configured;
        sink_sbc_endpoint.control_cbs.deconfigured = sbc_deconfigured;
        sink_sbc_endpoint.control_cbs.start_play = sbc_sink_start_play;
        sink_sbc_endpoint.control_cbs.stop_play = sbc_stop_play;
        sink_sbc_endpoint.control_cbs.sink_streamer_data = sink_sbc_streamer_data;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
        sink_sbc_endpoint.cp_ie = (struct bt_a2dp_codec_ie *)&test_cp_ie[0];
        sink_sbc_endpoint.cp_ie_count = 1;
        sink_sbc_endpoint.cp_config = NULL;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
        sink_sbc_endpoint.delay_reporting_service_enable = 1;
        sink_sbc_endpoint.control_cbs.sink_delay_report_cb = sink_delay_report_callback;
#endif
        err = bt_a2dp_register_endpoint(&sink_sbc_endpoint, BT_A2DP_AUDIO, BT_A2DP_SINK);
        if (!err)
        {
            shell_print(shell, "SBC sink endpoint is registered");
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
            bt_a2dp_set_cp_header(&sink_sbc_endpoint, &cp_header, 1);
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
            bt_a2dp_set_initial_delay_report(&sink_sbc_endpoint, 1);
            shell_print(shell, "set initial delay report value as 1");
#endif
        }
        break;
    case 6: /* sbc with all other services */
        ep = &sink_sbc_endpoint;
        sink_sbc_endpoint.control_cbs.configured = sink_sbc_configured;
        sink_sbc_endpoint.control_cbs.deconfigured = sbc_deconfigured;
        sink_sbc_endpoint.control_cbs.start_play = sbc_sink_start_play;
        sink_sbc_endpoint.control_cbs.stop_play = sbc_stop_play;
        sink_sbc_endpoint.control_cbs.sink_streamer_data = sink_sbc_streamer_data;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
        sink_sbc_endpoint.cp_ie = (struct bt_a2dp_codec_ie *)&test_cp_ie[0];
        sink_sbc_endpoint.cp_ie_count = 1;
        sink_sbc_endpoint.cp_config = NULL;
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
        sink_sbc_endpoint.recovery_ie = (struct bt_a2dp_codec_ie *)&test_recovery_ie[0];
        sink_sbc_endpoint.recovery_config = NULL;
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
        sink_sbc_endpoint.reporting_service_enable = 1;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
        sink_sbc_endpoint.delay_reporting_service_enable = 1;
        sink_sbc_endpoint.control_cbs.sink_delay_report_cb = sink_delay_report_callback;
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
        /** delay reporting service enable flag */
        sink_sbc_endpoint.header_compression_cap = 0x40u;
        sink_sbc_endpoint.header_compression_config = 0x00u;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
        sink_sbc_endpoint.multiplexing_service_enable = 1u;
#endif
        err = bt_a2dp_register_endpoint(&sink_sbc_endpoint, BT_A2DP_AUDIO, BT_A2DP_SINK);
        if (!err)
        {
            shell_print(shell, "SBC sink endpoint is registered");
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
            bt_a2dp_set_cp_header(&sink_sbc_endpoint, &cp_header, 1);
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
            bt_a2dp_set_initial_delay_report(&sink_sbc_endpoint, 1);
            shell_print(shell, "set initial delay report value as 1");
#endif
        }
        break;
    default:
        break;
    }

    if (err)
    {
        shell_print(shell, "fail to register endpoint");
    }
    else
    {
        if (registered_index < MAX_REGISTERED_EP)
        {
            registered_eps[registered_index].ep = ep;
            k_work_init_delayable(&registered_eps[registered_index].consume_stream_work, consume_stream_work_cb);
            registered_index++;
        }
        else
        {
            shell_print(shell, "please increase the ep number");
        }
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_register_source_ep(shell_handle_t shell, int32_t argc, char *argv[])
{
    int select;
    int err = -1;
    struct bt_a2dp_endpoint *ep;

    shell_a2dp_init();
    if (a2dp_source_sdp_registered == 0) {
        a2dp_source_sdp_registered = 1;
        bt_sdp_register_service(&a2dp_source_rec);
    }

    if (argc > 1) {
        select = strtoul(argv[1], NULL, 10);
    } else {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    if ((select <= 0) || (select > 6)) {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    switch (select) {
    case 1: /* SBC */
        ep = &source_sbc_endpoint;
        source_sbc_endpoint.control_cbs.configured = source_sbc_configured;
        source_sbc_endpoint.control_cbs.deconfigured = common_deconfigured;
        source_sbc_endpoint.control_cbs.start_play = common_start_play;
        source_sbc_endpoint.control_cbs.stop_play = common_stop_play;
        err = bt_a2dp_register_endpoint(&source_sbc_endpoint, BT_A2DP_AUDIO, BT_A2DP_SOURCE);
        if (!err)
        {
            shell_print(shell, "SBC source endpoint is registered");
        }
        break;
    case 2: /* MPEG-1,2 */
        ep = &source_mpeg12_endpoint;
        source_mpeg12_endpoint.control_cbs.configured = source_mpeg12_configured;
        source_mpeg12_endpoint.control_cbs.deconfigured = common_deconfigured;
        source_mpeg12_endpoint.control_cbs.start_play = common_start_play;
        source_mpeg12_endpoint.control_cbs.stop_play = common_stop_play;
        err = bt_a2dp_register_endpoint(&source_mpeg12_endpoint, BT_A2DP_AUDIO, BT_A2DP_SOURCE);
        if (!err)
        {
            shell_print(shell, "MPEG-1,2 source endpoint is registered");
        }
        break;
    case 3: /* 3:MPEG-2,4 */
        ep = &source_mpeg24_endpoint;
        source_mpeg24_endpoint.control_cbs.configured = source_mpeg24_configured;
        source_mpeg24_endpoint.control_cbs.deconfigured = common_deconfigured;
        source_mpeg24_endpoint.control_cbs.start_play = common_start_play;
        source_mpeg24_endpoint.control_cbs.stop_play = common_stop_play;
        err = bt_a2dp_register_endpoint(&source_mpeg24_endpoint, BT_A2DP_AUDIO, BT_A2DP_SOURCE);
        if (!err)
        {
            shell_print(shell, "MPEG-2,4 source endpoint is registered");
        }
        break;
    case 4: /*  4:vendor */
        ep = &source_vendor_endpoint;
        source_vendor_endpoint.control_cbs.configured = source_vendor_configured;
        source_vendor_endpoint.control_cbs.deconfigured = common_deconfigured;
        source_vendor_endpoint.control_cbs.start_play = common_start_play;
        source_vendor_endpoint.control_cbs.stop_play = common_stop_play;
        err = bt_a2dp_register_endpoint(&source_vendor_endpoint, BT_A2DP_AUDIO, BT_A2DP_SOURCE);
        if (!err)
        {
            shell_print(shell, "Vendor source endpoint is registered");
        }
        break;
    case 5: /* sbc with delay report and content protection services */
        ep = &source_sbc_endpoint;
        source_sbc_endpoint.control_cbs.configured = source_sbc_configured;
        source_sbc_endpoint.control_cbs.deconfigured = common_deconfigured;
        source_sbc_endpoint.control_cbs.start_play = common_start_play;
        source_sbc_endpoint.control_cbs.stop_play = common_stop_play;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
        source_sbc_endpoint.cp_ie = (struct bt_a2dp_codec_ie *)&test_cp_ie[0];
        source_sbc_endpoint.cp_ie_count = 1;
        source_sbc_endpoint.cp_config = (struct bt_a2dp_codec_ie *)&test_cp_config[0];
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
        source_sbc_endpoint.delay_reporting_service_enable = 1;
        source_sbc_endpoint.control_cbs.source_delay_report_cb = source_delay_report_callback;
#endif
        err = bt_a2dp_register_endpoint(&source_sbc_endpoint, BT_A2DP_AUDIO, BT_A2DP_SOURCE);
        if (!err)
        {
            shell_print(shell, "SBC source endpoint is registered");
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
            bt_a2dp_set_cp_header(&source_sbc_endpoint, &cp_header, 1);
#endif
        }
        break;
    case 6: /* sbc with all other services */
        ep = &source_sbc_endpoint;
        source_sbc_endpoint.control_cbs.configured = source_sbc_configured;
        source_sbc_endpoint.control_cbs.deconfigured = common_deconfigured;
        source_sbc_endpoint.control_cbs.start_play = common_start_play;
        source_sbc_endpoint.control_cbs.stop_play = common_stop_play;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
        source_sbc_endpoint.cp_ie = (struct bt_a2dp_codec_ie *)&test_cp_ie[0];
        source_sbc_endpoint.cp_ie_count = 1;
        source_sbc_endpoint.cp_config = (struct bt_a2dp_codec_ie *)&test_cp_config[0];
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
        source_sbc_endpoint.recovery_ie = (struct bt_a2dp_codec_ie *)&test_recovery_ie[0];
        source_sbc_endpoint.recovery_config = (struct bt_a2dp_codec_ie *)&test_recovery_config[0];
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
        source_sbc_endpoint.reporting_service_enable = 1;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
        source_sbc_endpoint.delay_reporting_service_enable = 1;
        source_sbc_endpoint.control_cbs.source_delay_report_cb = source_delay_report_callback;
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
        /** delay reporting service enable flag */
        source_sbc_endpoint.header_compression_cap = 0x40u;
        source_sbc_endpoint.header_compression_config = 0x40u;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
        source_sbc_endpoint.multiplexing_service_enable = 1u;
#endif
        err = bt_a2dp_register_endpoint(&source_sbc_endpoint, BT_A2DP_AUDIO, BT_A2DP_SOURCE);
        if (!err)
        {
            shell_print(shell, "SBC source endpoint is registered");
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
            bt_a2dp_set_cp_header(&source_sbc_endpoint, &cp_header, 1);
#endif
        }
        break;
    default:
        break;
    }

    if (err)
    {
        shell_print(shell, "fail to register endpoint");
    }
    else
    {
        if (registered_index < MAX_REGISTERED_EP)
        {
            registered_eps[registered_index++].ep = ep;
        }
        else
        {
            shell_print(shell, "please increase the ep number");
        }
    }

    return kStatus_SHELL_Success;
}

static shell_status_t cmd_connect(shell_handle_t shell, int32_t argc, char *argv[])
{
    shell_a2dp_init();
    if (!default_conn) {
        shell_error(shell, "Not connected");
        return kStatus_SHELL_Error;
    }

    default_a2dp = bt_a2dp_connect(default_conn);
    if (NULL == default_a2dp) {
        shell_error(shell, "fail to connect a2dp");
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_disconnect(shell_handle_t shell, int32_t argc, char *argv[])
{
    shell_a2dp_init();
    default_ep = NULL;
    if (default_a2dp != NULL) {
        bt_a2dp_disconnect(default_a2dp);
        default_a2dp = NULL;
    } else {
        shell_error(shell, "a2dp is not connected");
    }
    return kStatus_SHELL_Success;
}

void app_configured(int err)
{
    if (err) {
        shell_print(ctx_shell, "configure fail");
    }
}

static shell_status_t cmd_configure(shell_handle_t shell, int32_t argc, char *argv[])
{
    shell_a2dp_init();
    if (default_a2dp != NULL) {
        bt_a2dp_configure(default_a2dp, app_configured);
    } else {
        shell_error(shell, "a2dp is not connected");
    }
    return kStatus_SHELL_Success;
}

static uint8_t bt_a2dp_discover_peer_endpoint_cb(struct bt_a2dp *a2dp,
					struct bt_a2dp_endpoint *endpoint, int err)
{
    if ((!err) &&(endpoint != NULL)) {
        shell_a2dp_print_capabilities(endpoint);
        return BT_A2DP_DISCOVER_ENDPOINT_CONTINUE;
    } else {
        shell_print(ctx_shell, "discover done");
        return BT_A2DP_DISCOVER_ENDPOINT_STOP;
    }
}

static shell_status_t cmd_get_peer_eps(shell_handle_t shell, int32_t argc, char *argv[])
{
    shell_a2dp_init();
    if (default_a2dp != NULL) {
        int err = bt_a2dp_discover_peer_endpoints(default_a2dp, bt_a2dp_discover_peer_endpoint_cb);
        if (err) {
            shell_error(shell, "discover fail");
        }
    } else {
        shell_error(shell, "a2dp is not connected");
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_get_registered_eps(shell_handle_t shell, int32_t argc, char *argv[])
{
    uint8_t *str;
    shell_a2dp_init();
    for (uint8_t index = 0; index < registered_index; index++)
    {
        switch (registered_eps[index].ep->codec_id)
        {
        case BT_A2DP_SBC:
            str = (uint8_t *)"SBC";
            break;
        case BT_A2DP_MPEG1:
            str = (uint8_t *)"MPEG1,2";
	    break;
	case BT_A2DP_MPEG2:
            str = (uint8_t *)"MPEG2,4";
	    break;
	case BT_A2DP_ATRAC:
            str = (uint8_t *)"ATRAC";
	    break;
	case BT_A2DP_VENDOR:
            str = (uint8_t *)"VENDOER";
            break;
        default:
            str = (uint8_t *)"UNKOWN";
            break;
        }
        shell_print(shell, "%d-%s", (index + 1), str);
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_set_default_ep(shell_handle_t shell, int32_t argc, char *argv[])
{
    int select;

    shell_a2dp_init();
    if (argc > 1) {
        select = strtoul(argv[1], NULL, 10);
    } else {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    if ((select <= 0) || (select > registered_index)) {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }
    default_ep = &registered_eps[select - 1];
    shell_print(shell, "success");
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_configure_ep(shell_handle_t shell, int32_t argc, char *argv[])
{
    shell_a2dp_init();
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_deconfigure(shell_handle_t shell, int32_t argc, char *argv[])
{
    shell_a2dp_init();
    if (default_ep == NULL)
    {
        shell_print(shell, "default endpoint is not set");
        return kStatus_SHELL_Success;
    }
    if (bt_a2dp_deconfigure(default_ep->ep)) {
          shell_print(shell, "fail");
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_start(shell_handle_t shell, int32_t argc, char *argv[])
{
    shell_a2dp_init();
    if (default_ep == NULL)
    {
        shell_print(shell, "default endpoint is not set");
        return kStatus_SHELL_Success;
    }
    if (bt_a2dp_start(default_ep->ep)) {
          shell_print(shell, "fail");
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_stop(shell_handle_t shell, int32_t argc, char *argv[])
{
    shell_a2dp_init();
    if (default_ep == NULL)
    {
        shell_print(shell, "default endpoint is not set");
        return kStatus_SHELL_Success;
    }
    if (bt_a2dp_stop(default_ep->ep)) {
          shell_print(shell, "fail");
    }
    return kStatus_SHELL_Success;
}

static shell_status_t cmd_send_media(shell_handle_t shell, int32_t argc, char *argv[])
{
    uint32_t time;

    shell_a2dp_init();
    if (default_ep == NULL)
    {
        shell_print(shell, "default endpoint is not set");
        return kStatus_SHELL_Success;
    }

    if (argc > 1) {
        time = strtoul(argv[1], NULL, 10);
    } else {
        shell_print(shell, "wrong parameter");
        return kStatus_SHELL_Success;
    }

    time *= 1000u;
    while (time--) {
        bt_a2dp_src_media_write(default_ep->ep, media_data, sizeof(media_data));
        vTaskDelay((((1)*configTICK_RATE_HZ + 999U) / 1000U));
    }

    return kStatus_SHELL_Success;
}

#define HELP_NONE "[none]"

SHELL_STATIC_SUBCMD_SET_CREATE(a2dp_cmds,
        SHELL_CMD_ARG(register_sink_ep, NULL, "<select codec.\r\n\
			1:SBC\r\n\
			2:MPEG-1,2\r\n\
			3:MPEG-2,4\r\n\
			4:vendor\r\n\
			5:sbc with delay report and content protection services\r\n\
			6:sbc with all other services(don't support data transfer yet)>", cmd_register_sink_ep, 2, 0),
        SHELL_CMD_ARG(register_source_ep, NULL, "<select codec.\r\n\
			1:SBC\r\n\
			2:MPEG-1,2\r\n\
			3:MPEG-2,4\r\n\
			4:vendor\r\n\
			5:sbc with delay report and content protection services\r\n\
			6:sbc with all other services(don't support data transfer yet)>", cmd_register_source_ep, 2, 0),
	SHELL_CMD_ARG(connect, NULL, HELP_NONE, cmd_connect, 1, 0),
	SHELL_CMD_ARG(disconnect, NULL, HELP_NONE, cmd_disconnect, 1, 0),
	SHELL_CMD_ARG(configure, NULL, HELP_NONE, cmd_configure, 1, 0),
        SHELL_CMD_ARG(discover_peer_eps, NULL, HELP_NONE, cmd_get_peer_eps, 1, 0),
        SHELL_CMD_ARG(get_registered_eps, NULL, HELP_NONE, cmd_get_registered_eps, 1, 0),
        SHELL_CMD_ARG(set_default_ep, NULL, "<select endpoint>", cmd_set_default_ep, 2, 0),
        SHELL_CMD_ARG(configure_ep, NULL, "\"configure the default selected ep\"", cmd_configure_ep, 1, 0),
        SHELL_CMD_ARG(deconfigure, NULL, "\"de-configure the default selected ep\"", cmd_deconfigure, 1, 0),
	SHELL_CMD_ARG(start, NULL, "\"start the default selected ep\"", cmd_start, 1, 0),
	SHELL_CMD_ARG(stop, NULL, "\"stop the default selected ep\"", cmd_stop, 1, 0),
	SHELL_CMD_ARG(send_media, NULL, "<second> \"send media data to the default selected ep\"", cmd_send_media, 2, 0),
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
        SHELL_CMD_ARG(send_delay_report, NULL, "<delay> \"a2dp sink send delay report to default selected ep\"", cmd_send_delay_report, 2, 0),
#endif
	SHELL_SUBCMD_SET_END
);

static shell_status_t cmd_a2dp(shell_handle_t shell, int32_t argc, char **argv)
{
    if (argc == 1) {
        shell_help(shell);
        /* shell returns 1 when help is printed */
        return kStatus_SHELL_PrintCmdHelp;
    }

    shell_error(shell, "%s unknown parameter: %s", argv[0], argv[1]);

    return kStatus_SHELL_Error;
}

SHELL_CMD_REGISTER(a2dp, a2dp_cmds, "Bluetooth A2DP shell commands",
		       cmd_a2dp, 1, 1);

void bt_ShellA2dpInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommanda2dp))
    {
        shell_print(shell, "Shell register command %s failed!", g_shellCommanda2dp.pcCommand);
    }
}
#endif /* CONFIG_BT_A2DP */
#endif /* CONFIG_BT_BREDR */