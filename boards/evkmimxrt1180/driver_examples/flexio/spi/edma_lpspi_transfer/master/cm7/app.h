/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*Master related*/
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 150000U /*! Transfer baudrate - 150k */

#define MASTER_FLEXIO_SPI_BASEADDR (FLEXIO2)
#define FLEXIO_SPI_PCS_PIN         5U
#define FLEXIO_SPI_SOUT_PIN        4U
#define FLEXIO_SPI_SIN_PIN         3U
#define FLEXIO_SPI_CLK_PIN         2U

#define MASTER_FLEXIO_SPI_IRQ FLEXIO2_IRQn

#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio2)

#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA4
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       0U
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       1U
#define FLEXIO_TX_SHIFTER_INDEX          0U
#define FLEXIO_RX_SHIFTER_INDEX          2U
#define EXAMPLE_TX_DMA_SOURCE            kDma4RequestMuxFlexIO2Request0
#define EXAMPLE_RX_DMA_SOURCE            kDma4RequestMuxFlexIO2Request2

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR   (LPSPI3)
#define SLAVE_LPSPI_IRQ_HANDLE (LPSPI3_DriverIRQHandler)
#define SLAVE_LPSPI_IRQN       (LPSPI3_IRQn)

#define SLAVE_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define SLAVE_LPSPI_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)
#define BOARD_GetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
            .securityLevel             = kEDMA_ChannelSecurityLevelSecure,       \
            .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged, \
        };                                                                       \
        config.enableMasterIdReplication                 = true;                 \
        config.channelConfig[FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL] = &channelConfig;       \
        config.channelConfig[FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL] = &channelConfig;       \
    }
    
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
