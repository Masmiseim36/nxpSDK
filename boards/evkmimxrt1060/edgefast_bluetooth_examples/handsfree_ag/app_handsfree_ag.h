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

/*******************************************************************************
 * API
 ******************************************************************************/

void peripheral_hfp_ag_task(void *param);
int app_hfp_ag_disconnect();
int app_hfp_ag_discover(struct bt_conn *conn, uint8_t channel);
int app_hfp_ag_start_incoming_call();
int app_hfp_ag_accept_incoming_call();
int app_hfp_ag_stop_incoming_call();
#endif /* __APPL_PERIPHERAL_HFP_AG_MAIN_H__ */
