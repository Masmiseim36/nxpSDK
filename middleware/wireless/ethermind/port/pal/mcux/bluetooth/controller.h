/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EDGEFAST_CONTROLLER_H__
#define __EDGEFAST_CONTROLLER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _controller_hci_uart_config
{
    uint32_t clockSrc;
    uint32_t defaultBaudrate;
    uint32_t runningBaudrate;
    uint8_t  instance;
    uint8_t  enableRxRTS;
    uint8_t  enableTxCTS;
} controller_hci_uart_config_t;

/* Initialize the platform */
void controller_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __EDGEFAST_CONTROLLER_H__ */