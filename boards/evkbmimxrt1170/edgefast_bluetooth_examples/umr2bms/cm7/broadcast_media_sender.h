/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APPL_BROADCAST_MEDIA_SENDER_H_
#define __APPL_BROADCAST_MEDIA_SENDER_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
void broadcast_media_sender_task(void *param);

int config_broadcast_code(uint8_t *data, int len);

int bms_codec_cfg(struct bt_audio_codec_cfg *cfg);
int bms_qos(struct bt_audio_codec_qos *qos);

int bms_init(void);
int bms_start(void);
int bms_stop(void);
int bms_deinit(void);
int bms_stream_send(enum bt_audio_location chan_allocation, struct net_buf *buf);
int bms_streaming_simulate_start(void);
int bms_streaming_simulate_stop(void);

#endif /* __APPL_BROADCAST_MEDIA_SENDER_H_ */