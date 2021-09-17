/*
 * Copyright (c) 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <string.h>
#include <stdio.h>
#include <errno/errno.h>
#include <sys/atomic.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <sys/byteorder.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp_codec_sbc.h>

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_A2DP)
#define LOG_MODULE_NAME bt_a2dp_sbc
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

uint8_t bt_a2dp_sbc_get_subband_num(struct bt_a2dp_codec_sbc_params *sbc_codec)
{
	assert(sbc_codec != NULL);

	if (sbc_codec->config[1] & A2DP_SBC_SUBBAND_4) {
		return 4U;
	} else if (sbc_codec->config[1] & A2DP_SBC_SUBBAND_8) {
		return 8U;
	} else {
		return 0U;
	}
}

uint8_t bt_a2dp_sbc_get_block_length(struct bt_a2dp_codec_sbc_params *sbc_codec)
{
	assert(sbc_codec != NULL);

	if (sbc_codec->config[1] & A2DP_SBC_BLK_LEN_4) {
		return 4U;
	} else if (sbc_codec->config[1] & A2DP_SBC_BLK_LEN_8) {
		return 8U;
	} else if (sbc_codec->config[1] & A2DP_SBC_BLK_LEN_12) {
		return 12U;
	} else if (sbc_codec->config[1] & A2DP_SBC_BLK_LEN_16) {
		return 16U;
	} else {
		return 0U;
	}
}

uint8_t bt_a2dp_sbc_get_channel_num(struct bt_a2dp_codec_sbc_params *sbc_codec)
{
	assert(sbc_codec != NULL);

	if (sbc_codec->config[0] & A2DP_SBC_CH_MODE_MONO) {
		return 1U;
	} else if (sbc_codec->config[0] & A2DP_SBC_CH_MODE_DUAL) {
		return 2U;
	} else if (sbc_codec->config[0] & A2DP_SBC_CH_MODE_STREO) {
		return 2U;
	} else if (sbc_codec->config[0] & A2DP_SBC_CH_MODE_JOINT) {
		return 2U;
	} else {
		return 0U;
	}
}

uint32_t bt_a2dp_sbc_get_sampling_frequency(struct bt_a2dp_codec_sbc_params *sbc_codec)
{
	assert(sbc_codec != NULL);

    if (sbc_codec->config[0] & A2DP_SBC_SAMP_FREQ_16000) {
        return 16000U;
    } else if (sbc_codec->config[0] & A2DP_SBC_SAMP_FREQ_32000) {
        return 32000U;
    } else if (sbc_codec->config[0] & A2DP_SBC_SAMP_FREQ_44100) {
        return 44100U;
    } else if (sbc_codec->config[0] & A2DP_SBC_SAMP_FREQ_48000) {
        return 48000U;
    } else {
        return 0U;
    }
}
