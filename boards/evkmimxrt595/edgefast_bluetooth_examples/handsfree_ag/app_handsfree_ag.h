/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __APPL_PERIPHERAL_HFP_AG_MAIN_H__
#define __APPL_PERIPHERAL_HFP_AG_MAIN_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define APP_MAX_HFP_AG_CONN (2U)

typedef struct app_hfp_ag_
{
    struct bt_hfp_ag *hfp_agHandle;
    struct k_work ataRespWork;
    TimerHandle_t xTimers;
    TimerHandle_t xTwcTimers;
    uint8_t selectCodec;
    uint8_t hfp_in_calling_status;
} app_hfp_ag_t;

extern app_hfp_ag_t g_HfpAgs[CONFIG_BT_HFP_AG_MAX_CONN];

/*******************************************************************************
 * API
 ******************************************************************************/

void peripheral_hfp_ag_task(void *param);
int app_hfp_ag_discover(struct bt_conn *conn, uint8_t channel);
int app_hfp_ag_start_incoming_call(void);
int app_hfp_ag_accept_incoming_call(void);
int app_hfp_ag_stop_incoming_call(void);
int app_hfp_ag_start_twc_incoming_call(void);
void app_hfp_ag_set_phnum_tag(char *name);
void app_hfp_ag_volume_update(hf_ag_volume_type_t type, int volume);
int app_hfp_ag_codec_select(uint8_t codec);
void app_hfp_ag_open_audio();
void app_hfp_ag_close_audio();
int app_hfp_ag_set_hf_indicator(uint8_t indicator, uint8_t control);
void app_hfp_ag_select_conn(uint8_t index);
#endif /* __APPL_PERIPHERAL_HFP_AG_MAIN_H__ */
