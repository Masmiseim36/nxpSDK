/*
 * Copyright 2018, 2023 NXP
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
/* The QTMR instance/channel used for board */
#define BOARD_QTMR_BASEADDR              TMR4
#define BOARD_QTMR_INPUT_CAPTURE_CHANNEL kQTMR_Channel_0
#define BOARD_QTMR_PWM_CHANNEL           kQTMR_Channel_1
#define QTMR_PWM_OUTPUT_FREQUENCY        50000
#define QTMR_DUTYCYCLE_PERCENT           50
#define QTMR_CounterInputPin             kQTMR_Counter0InputPin

/* QTMR Clock source divider for Ipg clock source, the value of two macros below should be aligned. */
#define QTMR_PRIMARY_SOURCE       (kQTMR_ClockDivide_8)
#define QTMR_CLOCK_SOURCE_DIVIDER (8U)

/* Get source clock for QTMR driver */
#define QTMR_SOURCE_CLOCK (CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Wakeup) / QTMR_CLOCK_SOURCE_DIVIDER)

#define EXAMPLE_QTMR_DMA (DMA4)

#define QTMR_EDMA_REQUEST_CAPT_SOURCE  kDma4RequestMuxQTIMER4CaptTimer0
#define QTMR_EDMA_REQUEST_CMPLD_SOURCE kDma4RequestMuxQTIMER4Cmpld1Timer0Cmpld2Timer1

#define BOARD_SetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
            .securityLevel             = kEDMA_ChannelSecurityLevelSecure,       \
            .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged, \
        };                                                                       \
        config.enableMasterIdReplication = true;                                 \
        config.channelConfig[0]          = &channelConfig;                       \
    }

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
