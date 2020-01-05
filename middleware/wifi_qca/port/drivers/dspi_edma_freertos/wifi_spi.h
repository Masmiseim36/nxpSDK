/*
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WIFI_DSPI_H__
#define __WIFI_DSPI_H__

#include <stdint.h>
#include "fsl_dspi.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_dspi_edma.h"

/* can be extended in future */
typedef struct {
    struct {
        uint32_t enabled;
        DMA_Type *dma_base;
        int32_t dma_rx_chnl;
        int32_t dma_im_chnl;
        int32_t dma_tx_chnl;
        uint32_t dma_irq_prio;
        DMAMUX_Type *dmamux_base;
        dma_request_source_t dmamux_rx_req;
        dma_request_source_t dmamux_tx_req;
    } dma_mode;
    struct {
        uint32_t enabled;
        uint32_t spi_irq_prio;
    } irq_mode;
    struct {
        SPI_Type *base;
        uint32_t clk_hz;
        uint32_t xfer_cs;
        uint32_t baudrate;
        uint32_t irq_threshold;
        dspi_master_config_t config;
    } spi;
} WIFIDRVS_SPI_config_t;

/* prototypes */
A_STATUS WIFIDRVS_SPI_Init(WIFIDRVS_SPI_config_t *config);
A_STATUS WIFIDRVS_SPI_Deinit(WIFIDRVS_SPI_config_t *config);
A_STATUS WIFIDRVS_SPI_InOutToken(uint32_t OutToken, uint8_t DataSize, uint32_t *pInToken);
A_STATUS WIFIDRVS_SPI_InOutBuffer(uint8_t *pBuffer, uint16_t length, uint8_t doRead, boolean sync);
A_STATUS WIFIDRVS_SPI_GetDefaultConfig(WIFIDRVS_SPI_config_t *config);
A_STATUS WIFIDRVS_SPI_GetSPIConfig(dspi_master_config_t *user_config, uint32_t baudrate, dspi_which_pcs_t cs);

#endif
