/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Performs initialization of Direction Finding in Host */
int le_df_init(void);

void hci_df_prepare_connectionless_iq_report(struct net_buf *buf,
					     struct bt_df_per_adv_sync_iq_samples_report *report,
					     struct bt_le_per_adv_sync **per_adv_sync_to_report);
