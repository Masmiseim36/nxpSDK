/* hci_ecc.h - HCI ECC emulation */

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __HCI_ECC_H__
#define __HCI_ECC_H__

void bt_hci_ecc_init(void);
int bt_hci_ecc_send(struct net_buf *buf);

#endif /* __HCI_ECC_H__ */
