/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __EDGEFAST_CONTROLLER_HCI_UART_H__
#define __EDGEFAST_CONTROLLER_HCI_UART_H__

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
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE> 0u))   
    uint8_t dma_instance;
    uint8_t rx_channel;
    uint8_t tx_channel;
    uint8_t dma_mux_instance;
    uint32_t rx_request;
    uint32_t tx_request;
#endif
} controller_hci_uart_config_t;

int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* __EDGEFAST_CONTROLLER_HCI_UART_H__ */