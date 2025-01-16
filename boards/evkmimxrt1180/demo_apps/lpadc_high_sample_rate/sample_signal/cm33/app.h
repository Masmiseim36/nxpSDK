/*
 * Copyright 2020, 2024 NXP
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
#define DEMO_LPADC_BASE        ADC1
#define DEMO_LPADC_CHANNEL_NUM 1U
#define DEMO_LPADC_IRQn        ADC1_IRQn
#define DEMO_LPADC_IRQ_HANDLER ADC1_IRQHandler

/* ERRATA051385: ADC INL/DNL degrade under high ADC clock frequency when VREFH selected as reference. */
#define DEMO_LPADC_VREF_SOURCE kLPADC_ReferenceVoltageAlt2
#define DEMO_DMA_BASE          DMA4
#define DEMO_DMA_CHANNEL       0U
#define ADC_DMA_REQUEST_SOURCE kDma4RequestMuxADC1Request0

#define BOARD_GetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
            .securityLevel             = kEDMA_ChannelSecurityLevelSecure,       \
            .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged, \
        };                                                                       \
        config.enableMasterIdReplication = true;                                 \
        config.channelConfig[0]          = &channelConfig;                       \
        config.channelConfig[1]          = &channelConfig;                       \
    }
#define DEMO_LPADC_DO_OFFSET_CALIBRATION   true
#define DEMO_LPADC_OFFSET_CALIBRATION_MODE kLPADC_OffsetCalibration12bitMode
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitADCClock(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
