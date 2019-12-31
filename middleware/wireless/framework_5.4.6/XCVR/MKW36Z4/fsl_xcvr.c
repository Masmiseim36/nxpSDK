/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "fsl_xcvr.h"
#include "fsl_xcvr_trim.h"
#include <math.h>
#include "ifr_radio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CAL_AT_FREQ_HZ  (2385000000UL)
#define channelMapTableSize (128U)
#define gPllDenom_c         0x02000000UL /* Denominator is a constant value */
#define ABS(x) ((x) > 0 ? (x) : -(x))

#ifndef TRUE
#define TRUE                    (true)
#endif

#ifndef FALSE
#define FALSE                   (false)
#endif
#define RF_OSCILLATOR_STAYS_ON     (false) /* Control whether RF_OSC can be left on all the time. */
#define RF_OSCILLATOR_READY        ((RSIM->CONTROL & RSIM_CONTROL_RF_OSC_READY_MASK) != 0x0UL)

#ifndef EXTERNAL_CLOCK_GEN
#define EXTERNAL_CLOCK_GEN   0
#endif

#define ANT_A   1
#define ANT_B   0

#ifndef XCVR_COEX_RF_ACTIVE_PIN
#define XCVR_COEX_RF_ACTIVE_PIN ANT_B
#endif /* XCVR_COEX_RF_ACTIVE_PIN */

typedef struct xcvr_pllChannel_tag
{
    unsigned int integer;
    unsigned int numerator;
} xcvr_pllChannel_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if RADIO_IS_GEN_3P0
static void rf_osc_startup(void);
static void rf_osc_shutdown(void);
#endif /* RADIO_IS_GEN_3P0 */
extern double trunc (double);
extern double round (double);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static panic_fptr s_PanicFunctionPtr = NULL;
static const xcvr_pllChannel_t mapTable [channelMapTableSize] =
{
    {0x00000025UL, 0x07C00000UL}, /* 0 */
    {0x00000025UL, 0x07C80000UL}, /* 1 */
    {0x00000025UL, 0x07D00000UL}, /* 2 */
    {0x00000025UL, 0x07D80000UL}, /* 3 */
    {0x00000025UL, 0x07E00000UL}, /* 4 */
    {0x00000025UL, 0x07E80000UL}, /* 5 */
    {0x00000025UL, 0x07F00000UL}, /* 6 */
    {0x00000025UL, 0x07F80000UL}, /* 7 */
    {0x00000025UL, 0x00000000UL}, /* 8 */
    {0x00000025UL, 0x00080000UL}, /* 9 */
    {0x00000025UL, 0x00100000UL}, /* 10 */
    {0x00000025UL, 0x00180000UL}, /* 11 */
    {0x00000025UL, 0x00200000UL}, /* 12 */
    {0x00000025UL, 0x00280000UL}, /* 13 */
    {0x00000025UL, 0x00300000UL}, /* 14 */
    {0x00000025UL, 0x00380000UL}, /* 15 */
    {0x00000025UL, 0x00400000UL}, /* 16 */
    {0x00000025UL, 0x00480000UL}, /* 17 */
    {0x00000025UL, 0x00500000UL}, /* 18 */
    {0x00000025UL, 0x00580000UL}, /* 19 */
    {0x00000025UL, 0x00600000UL}, /* 20 */
    {0x00000025UL, 0x00680000UL}, /* 21 */
    {0x00000025UL, 0x00700000UL}, /* 22 */
    {0x00000025UL, 0x00780000UL}, /* 23 */
    {0x00000025UL, 0x00800000UL}, /* 24 */
    {0x00000025UL, 0x00880000UL}, /* 25 */
    {0x00000025UL, 0x00900000UL}, /* 26 */
    {0x00000025UL, 0x00980000UL}, /* 27 */
    {0x00000025UL, 0x00A00000UL}, /* 28 */
    {0x00000025UL, 0x00A80000UL}, /* 29 */
    {0x00000025UL, 0x00B00000UL}, /* 30 */
    {0x00000025UL, 0x00B80000UL}, /* 31 */
    {0x00000025UL, 0x00C00000UL}, /* 32 */
    {0x00000025UL, 0x00C80000UL}, /* 33 */
    {0x00000025UL, 0x00D00000UL}, /* 34 */
    {0x00000025UL, 0x00D80000UL}, /* 35 */
    {0x00000025UL, 0x00E00000UL}, /* 36 */
    {0x00000025UL, 0x00E80000UL}, /* 37 */
    {0x00000025UL, 0x00F00000UL}, /* 38 */
    {0x00000025UL, 0x00F80000UL}, /* 39 */
    {0x00000025UL, 0x01000000UL}, /* 40 */
    {0x00000026UL, 0x07080000UL}, /* 41 */
    {0x00000026UL, 0x07100000UL}, /* 42 */
    {0x00000026UL, 0x07180000UL}, /* 43 */
    {0x00000026UL, 0x07200000UL}, /* 44 */
    {0x00000026UL, 0x07280000UL}, /* 45 */
    {0x00000026UL, 0x07300000UL}, /* 46 */
    {0x00000026UL, 0x07380000UL}, /* 47 */
    {0x00000026UL, 0x07400000UL}, /* 48 */
    {0x00000026UL, 0x07480000UL}, /* 49 */
    {0x00000026UL, 0x07500000UL}, /* 50 */
    {0x00000026UL, 0x07580000UL}, /* 51 */
    {0x00000026UL, 0x07600000UL}, /* 52 */
    {0x00000026UL, 0x07680000UL}, /* 53 */
    {0x00000026UL, 0x07700000UL}, /* 54 */
    {0x00000026UL, 0x07780000UL}, /* 55 */
    {0x00000026UL, 0x07800000UL}, /* 56 */
    {0x00000026UL, 0x07880000UL}, /* 57 */
    {0x00000026UL, 0x07900000UL}, /* 58 */
    {0x00000026UL, 0x07980000UL}, /* 59 */
    {0x00000026UL, 0x07A00000UL}, /* 60 */
    {0x00000026UL, 0x07A80000UL}, /* 61 */
    {0x00000026UL, 0x07B00000UL}, /* 62 */
    {0x00000026UL, 0x07B80000UL}, /* 63 */
    {0x00000026UL, 0x07C00000UL}, /* 64 */
    {0x00000026UL, 0x07C80000UL}, /* 65 */
    {0x00000026UL, 0x07D00000UL}, /* 66 */
    {0x00000026UL, 0x07D80000UL}, /* 67 */
    {0x00000026UL, 0x07E00000UL}, /* 68 */
    {0x00000026UL, 0x07E80000UL}, /* 69 */
    {0x00000026UL, 0x07F00000UL}, /* 70 */
    {0x00000026UL, 0x07F80000UL}, /* 71 */
    {0x00000026UL, 0x00000000UL}, /* 72 */
    {0x00000026UL, 0x00080000UL}, /* 73 */
    {0x00000026UL, 0x00100000UL}, /* 74 */
    {0x00000026UL, 0x00180000UL}, /* 75 */
    {0x00000026UL, 0x00200000UL}, /* 76 */
    {0x00000026UL, 0x00280000UL}, /* 77 */
    {0x00000026UL, 0x00300000UL}, /* 78 */
    {0x00000026UL, 0x00380000UL}, /* 79 */
    {0x00000026UL, 0x00400000UL}, /* 80 */
    {0x00000026UL, 0x00480000UL}, /* 81 */
    {0x00000026UL, 0x00500000UL}, /* 82 */
    {0x00000026UL, 0x00580000UL}, /* 83 */
    {0x00000026UL, 0x00600000UL}, /* 84 */
    {0x00000026UL, 0x00680000UL}, /* 85 */
    {0x00000026UL, 0x00700000UL}, /* 86 */
    {0x00000026UL, 0x00780000UL}, /* 87 */
    {0x00000026UL, 0x00800000UL}, /* 88 */
    {0x00000026UL, 0x00880000UL}, /* 89 */
    {0x00000026UL, 0x00900000UL}, /* 90 */
    {0x00000026UL, 0x00980000UL}, /* 91 */
    {0x00000026UL, 0x00A00000UL}, /* 92 */
    {0x00000026UL, 0x00A80000UL}, /* 93 */
    {0x00000026UL, 0x00B00000UL}, /* 94 */
    {0x00000026UL, 0x00B80000UL}, /* 95 */
    {0x00000026UL, 0x00C00000UL}, /* 96 */
    {0x00000026UL, 0x00C80000UL}, /* 97 */
    {0x00000026UL, 0x00D00000UL}, /* 98 */
    {0x00000026UL, 0x00D80000UL}, /* 99 */
    {0x00000026UL, 0x00E00000UL}, /* 100 */
    {0x00000026UL, 0x00E80000UL}, /* 101 */
    {0x00000026UL, 0x00F00000UL}, /* 102 */
    {0x00000026UL, 0x00F80000UL}, /* 103 */
    {0x00000026UL, 0x01000000UL}, /* 104 */
    {0x00000027UL, 0x07080000UL}, /* 105 */
    {0x00000027UL, 0x07100000UL}, /* 106 */
    {0x00000027UL, 0x07180000UL}, /* 107 */
    {0x00000027UL, 0x07200000UL}, /* 108 */
    {0x00000027UL, 0x07280000UL}, /* 109 */
    {0x00000027UL, 0x07300000UL}, /* 110 */
    {0x00000027UL, 0x07380000UL}, /* 111 */
    {0x00000027UL, 0x07400000UL}, /* 112 */
    {0x00000027UL, 0x07480000UL}, /* 113 */
    {0x00000027UL, 0x07500000UL}, /* 114 */
    {0x00000027UL, 0x07580000UL}, /* 115 */
    {0x00000027UL, 0x07600000UL}, /* 116 */
    {0x00000027UL, 0x07680000UL}, /* 117 */
    {0x00000027UL, 0x07700000UL}, /* 118 */
    {0x00000027UL, 0x07780000UL}, /* 119 */
    {0x00000027UL, 0x07800000UL}, /* 120 */
    {0x00000027UL, 0x07880000UL}, /* 121 */
    {0x00000027UL, 0x07900000UL}, /* 122 */
    {0x00000027UL, 0x07980000UL}, /* 123 */
    {0x00000027UL, 0x07A00000UL}, /* 124 */
    {0x00000027UL, 0x07A80000UL}, /* 125 */
    {0x00000027UL, 0x07B00000UL}, /* 126 */
    {0x00000027UL, 0x07B80000UL}  /* 127 */
};

/* Registers for timing of TX & RX */
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
uint16_t tx_rx_on_delay = (uint16_t)TX_RX_ON_DELinit;
uint16_t tx_rx_synth_delay = (uint16_t)TX_RX_SYNTH_init;
#else
#if RF_OSC_26MHZ == 1
uint16_t tx_rx_on_delay = (uint16_t)TX_RX_ON_DELAY_VAL_26MHZ;
#else
uint16_t tx_rx_on_delay = (uint16_t)TX_RX_ON_DELAY_VAL;
#endif /* RF_OSC_26MHZ == 1 */
uint16_t tx_rx_synth_delay = (uint16_t)TX_RX_SYNTH_DELAY_VAL;
#endif /* RADIO_IS_GEN_3P0 || RADIO_IS_2P1 */

/* NOTE: These arrays MUST be ordered in the same order as the radio_mode_t enumeration. */
#if RADIO_IS_GEN_3P0
static const xcvr_mode_datarate_config_t * mode_configs_dr_2mbps[NUM_RADIO_MODES]=
{
    (xcvr_mode_datarate_config_t *)NULL, /* 2Mbps rate not supported for this mode */
    (xcvr_mode_datarate_config_t *)NULL, /* 2Mbps rate not supported for this mode */
    (xcvr_mode_datarate_config_t *)NULL, /* 2Mbps rate not supported for this mode */
#if XCVR_GENFSK_ENABLED
    &xcvr_GFSK_BT_0p5_h_0p5_2mbps_config,
    &xcvr_GFSK_BT_0p5_h_0p32_2mbps_config,
#else
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_GENFSK_ENABLED */
    (xcvr_mode_datarate_config_t *)NULL, /* 2Mbps rate not supported for this mode */
    (xcvr_mode_datarate_config_t *)NULL, /* 2Mbps rate not supported for this mode */
#if XCVR_GENFSK_ENABLED
    &xcvr_GFSK_BT_0p3_h_0p5_2mbps_config,
    &xcvr_GFSK_BT_0p7_h_0p5_2mbps_config,
    &xcvr_MSK_2mbps_config,
#else
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_GENFSK_ENABLED */
};
#endif /* RADIO_IS_GEN_3P0 */

static const xcvr_mode_datarate_config_t * mode_configs_dr_1mbps[NUM_RADIO_MODES]=
{
#if XCVR_BLE_ENABLED
    &xcvr_BLE_1mbps_config,
#else
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_BLE_ENABLED */
#if RADIO_IS_GEN_2P1
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
#else
#if XCVR_802_15_4_ENABLED
    &xcvr_ZIGBEE_250kbps_config, /* 802.15.4 only supports one configuration */
#else
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_802_15_4_ENABLED */
#if XCVR_ANT_ENABLED
    &xcvr_ANT_1mbps_config,
#else
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_ANT_ENABLED */
#endif /* RADIO_IS_GEN_2P1 */
#if XCVR_GENFSK_ENABLED
    &xcvr_GFSK_BT_0p5_h_0p5_1mbps_config,
    &xcvr_GFSK_BT_0p5_h_0p32_1mbps_config,
    &xcvr_GFSK_BT_0p5_h_0p7_1mbps_config,
    &xcvr_GFSK_BT_0p5_h_1p0_1mbps_config,
    &xcvr_GFSK_BT_0p3_h_0p5_1mbps_config,
    &xcvr_GFSK_BT_0p7_h_0p5_1mbps_config,
    &xcvr_MSK_1mbps_config,
#else
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_GENFSK_ENABLED */

};

static const xcvr_mode_datarate_config_t * mode_configs_dr_500kbps[NUM_RADIO_MODES]=
{
    &xcvr_BLE_1mbps_config, /* Invalid option */
#if RADIO_IS_GEN_2P1
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
#else
#if XCVR_802_15_4_ENABLED
    &xcvr_ZIGBEE_250kbps_config, /* 802.15.4 only supports one configuration */
#else
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_802_15_4_ENABLED */
#if XCVR_ANT_ENABLED
    &xcvr_ANT_1mbps_config, /* Invalid option */
#else
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_ANT_ENABLED */
#endif /* RADIO_IS_GEN_2P1 */
#if XCVR_GENFSK_ENABLED
    &xcvr_GFSK_BT_0p5_h_0p5_500kbps_config,
    &xcvr_GFSK_BT_0p5_h_0p32_500kbps_config,
    &xcvr_GFSK_BT_0p5_h_0p7_500kbps_config,
    &xcvr_GFSK_BT_0p5_h_1p0_500kbps_config,
    &xcvr_GFSK_BT_0p3_h_0p5_500kbps_config,
    &xcvr_GFSK_BT_0p7_h_0p5_500kbps_config,
    &xcvr_MSK_500kbps_config,
#else
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_GENFSK_ENABLED */
};
static const xcvr_mode_datarate_config_t * mode_configs_dr_250kbps[NUM_RADIO_MODES]=
{
    &xcvr_BLE_1mbps_config, /* Invalid option */
#if RADIO_IS_GEN_2P1
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
#else
#if XCVR_802_15_4_ENABLED
    &xcvr_ZIGBEE_250kbps_config, /* 802.15.4 only supports one configuration */
#else
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_802_15_4_ENABLED */
#if XCVR_ANT_ENABLED
    &xcvr_ANT_1mbps_config, /* Invalid option */
#else
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_ANT_ENABLED */
#endif /* RADIO_IS_GEN_2P1 */
#if XCVR_GENFSK_ENABLED
    &xcvr_GFSK_BT_0p5_h_0p5_250kbps_config,
    &xcvr_GFSK_BT_0p5_h_0p32_250kbps_config,
    &xcvr_GFSK_BT_0p5_h_0p7_250kbps_config,
    &xcvr_GFSK_BT_0p5_h_1p0_250kbps_config,
    &xcvr_GFSK_BT_0p3_h_0p5_250kbps_config,
    &xcvr_GFSK_BT_0p7_h_0p5_250kbps_config,
    &xcvr_MSK_250kbps_config,
#else
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
    (xcvr_mode_datarate_config_t *)NULL,
#endif /* XCVR_GENFSK_ENABLED */
};

static xcvr_currConfig_t current_xcvr_config;

#if RADIO_IS_GEN_3P0
static void rf_osc_startup(void)
{
    if (!RF_OSCILLATOR_READY)
    {
        RSIM->CONTROL |= RSIM_CONTROL_RF_OSC_EN_MASK;
    }
    while (!RF_OSCILLATOR_READY)
    {
        /* Wait for RF_OSC_READY to be asserted before continuing */
    }
}

static void rf_osc_shutdown(void)
{
    /* RF_OSCILLATOR_STAYS_ON flag controls whether XCVR can shut off RF OSC or not */
#if (!RF_OSCILLATOR_STAYS_ON)
        RSIM->CONTROL &= ~RSIM_CONTROL_RF_OSC_EN_MASK;
#endif /* (!RF_OSCILLATOR_STAYS_ON) */
}
#endif /* RADIO_IS_GEN_3P0 */

/*******************************************************************************
 * Code
 *******************************************************************************/
xcvrStatus_t XCVR_Init(radio_mode_t radio_mode, data_rate_t data_rate)
{
    const xcvr_mode_datarate_config_t * mode_datarate_config;
    const xcvr_datarate_config_t * datarate_config ;
    const xcvr_mode_config_t * radio_mode_cfg;
    const xcvr_common_config_t * radio_common_config;
    
    xcvrStatus_t status;

    /* Startup the radio (power mode) and turn on required clocks */
    status = XCVR_RadioStartup();
    
    /* Perform the desired XCVR initialization and configuration */
    if (status == gXcvrSuccess_c)
    {
        status = XCVR_GetDefaultConfig(radio_mode, data_rate, 
                                   (const xcvr_common_config_t **)&radio_common_config, 
                                   (const xcvr_mode_config_t **)&radio_mode_cfg, 
                                   (const xcvr_mode_datarate_config_t **)&mode_datarate_config, 
                                   (const xcvr_datarate_config_t **)&datarate_config );
    }
    
    if (status == gXcvrSuccess_c)
    {
        status = XCVR_Configure((const xcvr_common_config_t *)radio_common_config, 
                                (const xcvr_mode_config_t *)radio_mode_cfg, 
                                (const xcvr_mode_datarate_config_t *)mode_datarate_config, 
                                (const xcvr_datarate_config_t *)datarate_config, XCVR_FIRST_INIT);
        current_xcvr_config.radio_mode = radio_mode;
        current_xcvr_config.data_rate = datarate_config->data_rate;
    }
    return status;
}

xcvrStatus_t XCVR_InitNoDacTrim(radio_mode_t radio_mode, data_rate_t data_rate)
{
    const xcvr_mode_datarate_config_t * mode_datarate_config;
    const xcvr_datarate_config_t * datarate_config ;
    const xcvr_mode_config_t * radio_mode_cfg;
    const xcvr_common_config_t * radio_common_config;
    
    xcvrStatus_t status;

    /* Startup the radio (power mode) and turn on required clocks */
    status = XCVR_RadioStartup();
    
    /* Perform the desired XCVR initialization and configuration */
    if (status == gXcvrSuccess_c)
    {
        status = XCVR_GetDefaultConfig(radio_mode, data_rate, 
                                   (const xcvr_common_config_t **)&radio_common_config, 
                                   (const xcvr_mode_config_t **)&radio_mode_cfg, 
                                   (const xcvr_mode_datarate_config_t **)&mode_datarate_config, 
                                   (const xcvr_datarate_config_t **)&datarate_config );
    }
    
    if (status == gXcvrSuccess_c)
    {
        status = XCVR_RadioRegSetup((const xcvr_common_config_t *)radio_common_config, 
                                (const xcvr_mode_config_t *)radio_mode_cfg, 
                                (const xcvr_mode_datarate_config_t *)mode_datarate_config, 
                                (const xcvr_datarate_config_t *)datarate_config, XCVR_FIRST_INIT);
        /* normally DCOC DAC trim would go here */
        current_xcvr_config.radio_mode = radio_mode;
        current_xcvr_config.data_rate = datarate_config->data_rate;
    }
    return status;
}

void XCVR_Deinit(void)
{
#if RADIO_IS_GEN_3P0
    rf_osc_shutdown();
    RSIM->POWER |= RSIM_POWER_RSIM_STOP_MODE_MASK; /* Set radio stop mode to RVLLS */
    RSIM->POWER &= ~RSIM_POWER_RSIM_RUN_REQUEST_MASK; /* Clear RUN request */
#else
#endif /* RADIO_IS_GEN_3P0 */
}

xcvrStatus_t XCVR_RadioStartup(void)
{
    xcvrStatus_t status = gXcvrSuccess_c;
#if !RADIO_IS_GEN_3P0 /* SW trim table is not used in Gen3 radio */
    IFR_SW_TRIM_TBL_ENTRY_T sw_trim_tbl[] =
    {
        {TRIM_STATUS, 0, FALSE}, /*!< Fetch the trim status word if available.*/
        {TRIM_VERSION, 0, FALSE} /*!< Fetch the trim version number if available.*/
    };
    const uint8_t NUM_TRIM_TBL_ENTRIES = sizeof(sw_trim_tbl)/sizeof(IFR_SW_TRIM_TBL_ENTRY_T);
#endif /* !RADIO_IS_GEN_3P0 */

#if (EXTERNAL_CLOCK_GEN)
    RSIM->RF_OSC_CTRL |= RSIM_RF_OSC_CTRL_RF_OSC_BYPASS_EN_MASK; /* Only when external clock is being used */
#endif /* EXTERNAL_CLOCK_GEN */

#if RADIO_IS_GEN_2P0
    RSIM->RF_OSC_CTRL &= ~RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_MASK; /* Set EXT_OSC_OVRD value to zero */
    RSIM->RF_OSC_CTRL |= RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_EN_MASK; /* Enable over-ride with zero value */   
#endif /* RADIO_IS_GEN_2P0 */

#if RADIO_IS_GEN_3P0
    /* Assert Radio Run Request and wait for ack from SPM. */
    RSIM->POWER |= RSIM_POWER_RSIM_RUN_REQUEST_MASK;
    while ((RSIM->POWER & RSIM_POWER_SPM_RUN_ACK_STAT_MASK) == 0UL)
    {
    }
    RSIM->CONTROL |= RSIM_CONTROL_RSIM_CGC_XCVR_EN_MASK; 
    rf_osc_startup(); /* Start RF_OSC to allow radio registers access */
#else
    SIM->SCGC5 |= SIM_SCGC5_PHYDIG_MASK;

    /* Load IFR trim values */
    handle_ifr(&sw_trim_tbl[0], NUM_TRIM_TBL_ENTRIES);
#endif /* RADIO_IS_GEN_3P0 */

    /* Check that this is the proper radio version */
    {
#if (0)
    uint8_t radio_id = (uint8_t)((RSIM->MISC & RSIM_MISC_RADIO_VERSION_MASK)>>RSIM_MISC_RADIO_VERSION_SHIFT);
#else
    /* Make sure a 32 bit read is made otherwise when optimizations are enabled some compilers */
    /*    may generate code which makes a 8 bit read which will cause a hard fault. */
    uint8_t radio_id = (*((volatile uint32_t*)(RSIM_BASE+0x10)) & 0xFF000000U)>>24;
#endif
        if (
#if RADIO_IS_GEN_3P0
            ((radio_id != 0x4U) /* KW3 Gen3, A1 */
            && (radio_id != 0x5U) /* KW3 Gen3, A2 */
            && (radio_id != 0x0U))       /* KW3 Gen3 reads zero at this time */
#elif RADIO_IS_GEN_2P1
            ((radio_id != 0x8U) /* KW35 Gen2.1 A0 */
            && (radio_id != 0xAU)) /* KW35 Gen2.1 A1 */
#else
            (radio_id != 0x3U) &&  /* KW41/31/21 v1 */
            (radio_id != 0xBU) /* KW41/31/21 v1.1 */
#endif /* RADIO_IS_GEN_3P0 */
            )
        {
            XcvrPanic(WRONG_RADIO_ID_DETECTED,(uint32_t)&XCVR_Init);
        }
    }

    return status;
}

xcvrStatus_t XCVR_GetDefaultConfig(radio_mode_t radio_mode, 
                                   data_rate_t data_rate, 
                                   const xcvr_common_config_t ** com_config, 
                                   const xcvr_mode_config_t ** mode_config, 
                                   const xcvr_mode_datarate_config_t ** mode_datarate_config, 
                                   const xcvr_datarate_config_t ** datarate_config)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    /* Common configuration pointer */
    *com_config = (const xcvr_common_config_t *)&xcvr_common_config;

    /* Mode dependent configuration pointer */
    switch (radio_mode)
    {
#if !RADIO_IS_GEN_2P1
#if XCVR_802_15_4_ENABLED
        case ZIGBEE_MODE:
            *mode_config = ( const xcvr_mode_config_t *)&zgbe_mode_config;    /* Zigbee configuration */
            break;
#endif /* XCVR_802_15_4_ENABLED */
#if XCVR_ANT_ENABLED
        case ANT_MODE:
            *mode_config = ( const xcvr_mode_config_t *)&ant_mode_config;     /* ANT configuration */
            break;
#endif /* XCVR_ANT_ENABLED */
#endif /* !RADIO_IS_GEN_2P1 */
#if XCVR_BLE_ENABLED
        case BLE_MODE:
            *mode_config = ( const xcvr_mode_config_t *)&ble_mode_config;     /* BLE configuration */
            break;
#endif /* XCVR_BLE_ENABLED */
#if XCVR_GENFSK_ENABLED
        case GFSK_BT_0p5_h_0p5:
            *mode_config = ( const xcvr_mode_config_t *)&gfsk_bt_0p5_h_0p5_mode_config;     /* GFSK_BT_0p5_h_0p5 configuration */
            break;
        case GFSK_BT_0p5_h_0p32:
            *mode_config = ( const xcvr_mode_config_t *)&gfsk_bt_0p5_h_0p32_mode_config;     /* GFSK_BT_0p5_h_0p32 configuration */
            break;
        case GFSK_BT_0p5_h_0p7:
            *mode_config = ( const xcvr_mode_config_t *)&gfsk_bt_0p5_h_0p7_mode_config;     /* GFSK_BT_0p5_h_0p7 configuration */
            break;
        case GFSK_BT_0p5_h_1p0:
            *mode_config = ( const xcvr_mode_config_t *)&gfsk_bt_0p5_h_1p0_mode_config;     /* GFSK_BT_0p5_h_1p0 configuration */
            break;
        case GFSK_BT_0p3_h_0p5:
            *mode_config = ( const xcvr_mode_config_t *)&gfsk_bt_0p3_h_0p5_mode_config;     /* GFSK_BT_0p3_h_0p5 configuration */
            break;
        case GFSK_BT_0p7_h_0p5:
            *mode_config = ( const xcvr_mode_config_t *)&gfsk_bt_0p7_h_0p5_mode_config;     /* GFSK_BT_0p7_h_0p5 configuration */
            break;
        case MSK:
            *mode_config = ( const xcvr_mode_config_t *)&msk_mode_config;     /* MSK configuration */
            break;
#endif /* XCVR_GENFSK_ENABLED */

        default:    
            status = gXcvrInvalidParameters_c;
            break;
    }

    /* Data rate dependent and modeXdatarate dependent configuration pointers */
    if (status == gXcvrSuccess_c) /* Only attempt this pointer assignment process if prior switch() statement completed successfully */
    {
        if (radio_mode == ZIGBEE_MODE)
        {
            /* See fsl_xcvr_zgbe_config.c for settings */
#if !RADIO_IS_GEN_2P1 && XCVR_802_15_4_ENABLED
            *mode_datarate_config = (const xcvr_mode_datarate_config_t *)&xcvr_ZIGBEE_250kbps_config; /* 802.15.4 setting */
            *datarate_config = (const xcvr_datarate_config_t *)&xcvr_802_15_4_250kbps_config; /* 500Kbps datarate configurations */
#else
            status = gXcvrInvalidParameters_c;
#endif /* !RADIO_IS_GEN_2P1 */
        }
        else
        {
            switch (data_rate)
            {
#if RADIO_IS_GEN_3P0
                case DR_2MBPS:
                    if ((radio_mode == GFSK_BT_0p5_h_0p7) || (radio_mode == GFSK_BT_0p5_h_1p0) || (radio_mode == BLE_MODE) || (radio_mode == ANT_MODE))
                    {
                        status = gXcvrInvalidParameters_c;
                    }
                    else
                    {
                        *datarate_config = (const xcvr_datarate_config_t *)&xcvr_2mbps_config; /* 2Mbps datarate configurations */
                        *mode_datarate_config = (const xcvr_mode_datarate_config_t *)mode_configs_dr_2mbps[radio_mode];
                    }
                    break;
#endif /* RADIO_IS_GEN_3P0 */
                case DR_1MBPS:
                    *datarate_config = (const xcvr_datarate_config_t *)&xcvr_1mbps_config; /* 1Mbps datarate configurations */
                    *mode_datarate_config = (const xcvr_mode_datarate_config_t *)mode_configs_dr_1mbps[radio_mode];
                    break;
                case DR_500KBPS:
                    *datarate_config = (const xcvr_datarate_config_t *)&xcvr_500kbps_config; /* 500Kbps datarate configurations */
                    *mode_datarate_config = (const xcvr_mode_datarate_config_t *)mode_configs_dr_500kbps[radio_mode];
                    break;
                case DR_250KBPS:
                    *datarate_config = (const xcvr_datarate_config_t *)&xcvr_250kbps_config; /* 250Kbps datarate configurations */
                    *mode_datarate_config = (const xcvr_mode_datarate_config_t *)mode_configs_dr_250kbps[radio_mode];
                    break;
                default:
                    status = gXcvrInvalidParameters_c;
                    break;
            }
        }
    }
    return status;
}

xcvrStatus_t XCVR_Configure(const xcvr_common_config_t *com_config, 
                            const xcvr_mode_config_t *mode_config, 
                            const xcvr_mode_datarate_config_t *mode_datarate_config, 
                            const xcvr_datarate_config_t *datarate_config, 
                            XCVR_INIT_MODE_CHG_T first_init)
{
    xcvr_DcocDacTrim_t dac_trims;
    xcvrStatus_t config_status = gXcvrSuccess_c;

    config_status =  XCVR_RadioRegSetup(com_config, mode_config, mode_datarate_config, datarate_config, first_init);
    
    if ((first_init) && (config_status == gXcvrSuccess_c))
    {
        config_status = XCVR_CalculateDcocDacTrims(&dac_trims);
    }

    return config_status;
}


xcvrStatus_t XCVR_RadioRegSetup(const xcvr_common_config_t *com_config, 
                            const xcvr_mode_config_t *mode_config, 
                            const xcvr_mode_datarate_config_t *mode_datarate_config, 
                            const xcvr_datarate_config_t *datarate_config, 
                            XCVR_INIT_MODE_CHG_T first_init)

{
    xcvrStatus_t config_status = gXcvrSuccess_c;
    uint32_t temp; 

    if ((com_config == NULL) || (mode_config == NULL) || (mode_datarate_config == NULL) || (datarate_config == NULL))
    {
        config_status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* Turn on the module clocks before doing anything */
#if RADIO_IS_GEN_3P0
        RSIM->CONTROL |= mode_config->scgc5_clock_ena_bits; /* Same bit storage is used but RSIM bit assignments are applied */
#else
        SIM->SCGC5 |= mode_config->scgc5_clock_ena_bits;
#endif /* RADIO_IS_GEN_3P0 */
        
        /*******************************************************************************/
        /* XCVR_ANA configs */
        /*******************************************************************************/
        /* Configure PLL Loop Filter */
        if (first_init)
        {
            XCVR_ANA->SY_CTRL_1 &= ~com_config->ana_sy_ctrl1.mask;
            XCVR_ANA->SY_CTRL_1 |= com_config->ana_sy_ctrl1.init;
        }

        /* Configure VCO KVM */
        XCVR_ANA->SY_CTRL_2 &= ~mode_datarate_config->ana_sy_ctrl2.mask;
        XCVR_ANA->SY_CTRL_2 |= mode_datarate_config->ana_sy_ctrl2.init;

        /* Configure analog filter bandwidth */
        XCVR_ANA->RX_BBA &= ~mode_datarate_config->ana_rx_bba.mask;
        XCVR_ANA->RX_BBA |= mode_datarate_config->ana_rx_bba.init;
        XCVR_ANA->RX_TZA &= ~mode_datarate_config->ana_rx_tza.mask;
        XCVR_ANA->RX_TZA |= mode_datarate_config->ana_rx_tza.init;

        if (first_init)
        {
#if RADIO_IS_GEN_2P0 /* Analog bump settings specific to Gen 2.0 (KW41) */
            temp = XCVR_ANA->TX_DAC_PA;
            temp &= ~XCVR_ANALOG_TX_DAC_PA_TX_PA_BUMP_VBIAS_MASK;
            temp |= XCVR_ANALOG_TX_DAC_PA_TX_PA_BUMP_VBIAS(4);
            XCVR_ANA->TX_DAC_PA = temp;

            temp = XCVR_ANA->BB_LDO_2;
            temp &= ~XCVR_ANALOG_BB_LDO_2_BB_LDO_VCOLO_TRIM_MASK;
            temp |= XCVR_ANALOG_BB_LDO_2_BB_LDO_VCOLO_TRIM(0);
            XCVR_ANA->BB_LDO_2 = temp;

            temp = XCVR_ANA->RX_LNA;
            temp &= ~XCVR_ANALOG_RX_LNA_RX_LNA_BUMP_MASK;
            temp |= XCVR_ANALOG_RX_LNA_RX_LNA_BUMP(1);
            XCVR_ANA->RX_LNA = temp;

            temp = XCVR_ANA->BB_LDO_1;
            temp &= ~XCVR_ANALOG_BB_LDO_1_BB_LDO_FDBK_TRIM_MASK;
            temp |= XCVR_ANALOG_BB_LDO_1_BB_LDO_FDBK_TRIM(1);
            XCVR_ANA->BB_LDO_1 = temp;
#else /* Analog bump settings (different fields!!!) specific to Gen 2.1 & Gen 3.0 analog */
            /* Note: this code is ONLY for setting the bits, it must be rewritten in order to clear any bits */
            XCVR_ANA->RX_LNA |= XCVR_ANALOG_RX_LNA_RX_LNA_SPARE(0x1UL); /* set Lsbit only */
            XCVR_ANA->TX_DAC_PA |= XCVR_ANALOG_TX_DAC_PA_TX_PA_SPARE(0x1UL);  /* set Lsbit only */
            XCVR_ANA->RX_AUXPLL |= XCVR_ANALOG_RX_AUXPLL_SPARE(0x1UL);  /* set Lsbit only */
            temp = XCVR_ANA->BB_LDO_2;
            temp &= XCVR_ANALOG_BB_LDO_2_BB_LDO_VTREF_TC_MASK; /* clear both bits in the field*/
            temp |= XCVR_ANALOG_BB_LDO_2_BB_LDO_VTREF_TC(0x1UL); /* then set the Lsbit */
            XCVR_ANA->BB_LDO_2 = temp;
#endif /* RADIO_IS_GEN_2P0 */
        }

        /*******************************************************************************/
        /* XCVR_MISC configs */
        /*******************************************************************************/
        temp = XCVR_MISC->XCVR_CTRL;
        temp &= ~(mode_config->xcvr_ctrl.mask | XCVR_CTRL_XCVR_CTRL_REF_CLK_FREQ_MASK);
        temp |= mode_config->xcvr_ctrl.init;
#if RF_OSC_26MHZ == 1
        {
            temp |= XCVR_CTRL_XCVR_CTRL_REF_CLK_FREQ(1UL);
        }
#endif /* RF_OSC_26MHZ == 1 */
        XCVR_MISC->XCVR_CTRL = temp;

#if RADIO_IS_GEN_2P1
        XCVR_MISC->FAD_CTRL &= ~XCVR_CTRL_FAD_CTRL_FAD_NOT_GPIO_MASK;
#endif /* RADIO_IS_GEN_2P1 */
        /*******************************************************************************/
        /* XCVR_PHY configs */
        /*******************************************************************************/
#if RADIO_IS_GEN_3P0
        XCVR_PHY->PHY_FSK_PD_CFG0 = mode_config->phy_fsk_pd_cfg0;
#if RF_OSC_26MHZ == 1
        XCVR_PHY->PHY_FSK_PD_CFG1 = mode_config->phy_fsk_pd_cfg1 | mode_datarate_config->phy_fsk_pd_cfg1_mode_datarate_26mhz;
        XCVR_PHY->PHY_FSK_CFG = mode_config->phy_fsk_cfg | mode_datarate_config->phy_fsk_cfg_mode_datarate_26mhz;
#else /* RF_OSC_26MHZ == 1 */
        XCVR_PHY->PHY_FSK_PD_CFG1 = mode_config->phy_fsk_pd_cfg1 | mode_datarate_config->phy_fsk_pd_cfg1_mode_datarate_32mhz;
        XCVR_PHY->PHY_FSK_CFG = mode_config->phy_fsk_cfg | mode_datarate_config->phy_fsk_cfg_mode_datarate_32mhz;
#endif /* RF_OSC_26MHZ == 1 */

#if (0) /* error in PHY header definitions! */
#if RF_OSC_26MHZ == 1
        XCVR_PHY->PHY_FSK_MISC = mode_config->phy_fsk_misc | mode_datarate_config->phy_fsk_misc_mode_datarate_26mhz; /* Need to combine mode specific and mode/datarate specific settings */
#else
        XCVR_PHY->PHY_FSK_MISC = mode_config->phy_fsk_misc | mode_datarate_config->phy_fsk_misc_mode_datarate_32mhz; /* Need to combine mode specific and mode/datarate specific settings */
#endif /* RF_OSC_26MHZ == 1 */
        XCVR_PHY->FSK_FAD_CTRL = mode_config->phy_fad_ctrl;
#else /* error in PHY header definitions! */
#if RF_OSC_26MHZ == 1
        *(uint32_t *)(XCVR_PHY_BASE+0xCUL) = mode_config->phy_fsk_misc | mode_datarate_config->phy_fsk_misc_mode_datarate_26mhz; /* Need to combine mode specific and mode/datarate specific settings */
#else
        *(uint32_t *)(XCVR_PHY_BASE+0xCUL) = mode_config->phy_fsk_misc | mode_datarate_config->phy_fsk_misc_mode_datarate_32mhz; /* Need to combine mode specific and mode/datarate specific settings */
#endif /* RF_OSC_26MHZ == 1 */
        *(uint32_t *)(XCVR_PHY_BASE+0x18UL) = mode_config->phy_fad_ctrl;
#endif /* error in PHY header definitions! */

#else /* RADIO_IS_GEN_3P0 */
        XCVR_PHY->PHY_PRE_REF0 = mode_config->phy_pre_ref0_init;
        XCVR_PHY->PRE_REF1 = mode_config->phy_pre_ref1_init;
        XCVR_PHY->PRE_REF2 = mode_config->phy_pre_ref2_init;
        XCVR_PHY->CFG1 = mode_config->phy_cfg1_init;
        XCVR_PHY->CFG2 = mode_datarate_config->phy_cfg2_init;
        XCVR_PHY->EL_CFG = mode_config->phy_el_cfg_init | datarate_config->phy_el_cfg_init; /* EL_WIN_SIZE and EL_INTERVAL are datarate dependent, */
#endif /* RADIO_IS_GEN_3P0 */

        /*******************************************************************************/
        /* XCVR_PLL_DIG configs */
        /*******************************************************************************/
        if (first_init)
        {
            XCVR_PLL_DIG->HPM_BUMP  = com_config->pll_hpm_bump;
            XCVR_PLL_DIG->MOD_CTRL  = com_config->pll_mod_ctrl;
            XCVR_PLL_DIG->CHAN_MAP  = com_config->pll_chan_map;
            XCVR_PLL_DIG->LOCK_DETECT  = com_config->pll_lock_detect;
            XCVR_PLL_DIG->HPM_CTRL  = com_config->pll_hpm_ctrl;
#if ((!RADIO_IS_GEN_2P1) && (!RADIO_IS_GEN_3P0))
            XCVR_PLL_DIG->HPMCAL_CTRL  = com_config->pll_hpmcal_ctrl;
#endif /* ((!RADIO_IS_GEN_2P1) && (!RADIO_IS_GEN_3P0)) */
            XCVR_PLL_DIG->HPM_SDM_RES  = com_config->pll_hpm_sdm_res;
            XCVR_PLL_DIG->LPM_CTRL  = com_config->pll_lpm_ctrl;
            XCVR_PLL_DIG->LPM_SDM_CTRL1  = com_config->pll_lpm_sdm_ctrl1;
            XCVR_PLL_DIG->DELAY_MATCH  = com_config->pll_delay_match;
            XCVR_PLL_DIG->CTUNE_CTRL  = com_config->pll_ctune_ctrl;
        }

        /*******************************************************************************/
        /* XCVR_RX_DIG configs */
        /*******************************************************************************/
        /* Configure RF Aux PLL for proper operation based on external clock frequency */
        if (first_init)
        {
            temp = XCVR_ANA->RX_AUXPLL;
            temp &= ~XCVR_ANALOG_RX_AUXPLL_VCO_DAC_REF_ADJUST_MASK;
#if RF_OSC_26MHZ == 1
            temp |= XCVR_ANALOG_RX_AUXPLL_VCO_DAC_REF_ADJUST(4UL);
#else
            temp |= XCVR_ANALOG_RX_AUXPLL_VCO_DAC_REF_ADJUST(7UL);
#endif /* RF_OSC_26MHZ == 1 */
            XCVR_ANA->RX_AUXPLL = temp;
        }
        
        /* Configure RX_DIG_CTRL */
#if RF_OSC_26MHZ == 1
        {
            temp = com_config->rx_dig_ctrl_init | /* Common portion of RX_DIG_CTRL init */
                   mode_config->rx_dig_ctrl_init_26mhz | /* Mode  specific portion of RX_DIG_CTRL init */
                   datarate_config->rx_dig_ctrl_init_26mhz | /* Datarate specific portion of RX_DIG_CTRL init */
                XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_EN_MASK; /* Always enable the sample rate converter for 26MHz */
        }
#else
        {
            temp = com_config->rx_dig_ctrl_init | /* Common portion of RX_DIG_CTRL init */
                   mode_config->rx_dig_ctrl_init_32mhz | /* Mode specific portion of RX_DIG_CTRL init */
                   datarate_config->rx_dig_ctrl_init_32mhz | /* Datarate specific portion of RX_DIG_CTRL init */
                0UL; /* Always disable the sample rate converter for 32MHz */
        }
#endif /* RF_OSC_26MHZ == 1 */

        temp |= com_config->rx_dig_ctrl_init;  /* Common portion of RX_DIG_CTRL init */
        XCVR_RX_DIG->RX_DIG_CTRL = temp;

        /* DCOC_CAL_IIR */
#if RADIO_IS_GEN_3P0
#if RF_OSC_26MHZ == 1
        {
            XCVR_RX_DIG->DCOC_CAL_IIR = mode_datarate_config->dcoc_cal_iir_init_26mhz;
        }
#else
        {
            XCVR_RX_DIG->DCOC_CAL_IIR = mode_datarate_config->dcoc_cal_iir_init_32mhz;
        }
#endif /* RF_OSC_26MHZ == 1 */
#else
#if RF_OSC_26MHZ == 1
        {
            XCVR_RX_DIG->DCOC_CAL_IIR = datarate_config->dcoc_cal_iir_init_26mhz;
        }
#else
        {
            XCVR_RX_DIG->DCOC_CAL_IIR = datarate_config->dcoc_cal_iir_init_32mhz;
        }
#endif /* RF_OSC_26MHZ == 1 */
#endif /* RADIO_IS_GEN_3P0 */

        /* DC_RESID_CTRL */
#if RF_OSC_26MHZ == 1
        {
#if RADIO_IS_GEN_3P0
        XCVR_RX_DIG->DC_RESID_CTRL = mode_config->dc_resid_ctrl_init | mode_datarate_config->dc_resid_ctrl_init | datarate_config->dc_resid_ctrl_26mhz;
#else
#if RADIO_IS_GEN_2P0
        XCVR_RX_DIG->DC_RESID_CTRL = com_config->dc_resid_ctrl_init | datarate_config->dc_resid_ctrl_26mhz;
#else /* GEN 2.1 */
        XCVR_RX_DIG->DC_RESID_CTRL = com_config->dc_resid_ctrl_init | mode_datarate_config->dc_resid_ctrl_init_26mhz | datarate_config->dc_resid_ctrl_26mhz;
#endif /* RADIO_IS_GEN_2P0 */
#endif /* RADIO_IS_GEN_3P0 */
        }
#else /* 32MHz */
        {
#if RADIO_IS_GEN_3P0
        XCVR_RX_DIG->DC_RESID_CTRL = mode_config->dc_resid_ctrl_init | mode_datarate_config->dc_resid_ctrl_init | datarate_config->dc_resid_ctrl_32mhz;
#else
#if RADIO_IS_GEN_2P0
        XCVR_RX_DIG->DC_RESID_CTRL = com_config->dc_resid_ctrl_init | datarate_config->dc_resid_ctrl_32mhz;
#else /* GEN 2.1 */
        XCVR_RX_DIG->DC_RESID_CTRL = com_config->dc_resid_ctrl_init | mode_datarate_config->dc_resid_ctrl_init_32mhz | datarate_config->dc_resid_ctrl_32mhz;
#endif /* RADIO_IS_GEN_2P0 */
#endif /* RADIO_IS_GEN_3P0 */
        }
#endif /* RF_OSC_26MHZ == 1 */

        /* DCOC_CTRL_0  & _1 */
#if RF_OSC_26MHZ == 1
        {

#if RADIO_IS_GEN_2P1
            XCVR_RX_DIG->DCOC_CTRL_0 = com_config->dcoc_ctrl_0_init_26mhz | mode_datarate_config->dcoc_ctrl_0_init_26mhz | datarate_config->dcoc_ctrl_0_init_26mhz; /* Combine common and datarate specific settings */
            XCVR_RX_DIG->DCOC_CTRL_1 = com_config->dcoc_ctrl_1_init | mode_datarate_config->dcoc_ctrl_1_init_26mhz | datarate_config->dcoc_ctrl_1_init_26mhz; /* Combine common and datarate specific settings */
#endif /* RADIO_IS_GEN_2P1 */
#if RADIO_IS_GEN_2P0
            XCVR_RX_DIG->DCOC_CTRL_0 = com_config->dcoc_ctrl_0_init_26mhz | datarate_config->dcoc_ctrl_0_init_26mhz; /* Combine common and datarate specific settings */
            XCVR_RX_DIG->DCOC_CTRL_1 = com_config->dcoc_ctrl_1_init | datarate_config->dcoc_ctrl_1_init_26mhz; /* Combine common and datarate specific settings */
#endif /* RADIO_IS_GEN_2P0 */
#if RADIO_IS_GEN_3P0
            XCVR_RX_DIG->DCOC_CTRL_0 = com_config->dcoc_ctrl_0_init_26mhz | mode_datarate_config->dcoc_ctrl_0_init_26mhz; /* Combine common and datarate specific settings */
            XCVR_RX_DIG->DCOC_CTRL_1 = com_config->dcoc_ctrl_1_init |  mode_datarate_config->dcoc_ctrl_1_init | datarate_config->dcoc_ctrl_1_init_26mhz; /* Combine common and datarate specific settings */
            XCVR_RX_DIG->DCOC_CTRL_2 = datarate_config->dcoc_ctrl_2_init_26mhz;
#endif /* RADIO_IS_GEN_3P0 */
        }
#else
        {
#if RADIO_IS_GEN_2P1
            XCVR_RX_DIG->DCOC_CTRL_0 = com_config->dcoc_ctrl_0_init_32mhz | mode_datarate_config->dcoc_ctrl_0_init_32mhz | datarate_config->dcoc_ctrl_0_init_32mhz; /* Combine common and datarate specific settings */
            XCVR_RX_DIG->DCOC_CTRL_1 = com_config->dcoc_ctrl_1_init | mode_datarate_config->dcoc_ctrl_1_init_32mhz | datarate_config->dcoc_ctrl_1_init_32mhz; /* Combine common and datarate specific settings */
#endif /* RADIO_IS_GEN_2P1 */
#if RADIO_IS_GEN_2P0
            XCVR_RX_DIG->DCOC_CTRL_0 = com_config->dcoc_ctrl_0_init_32mhz | datarate_config->dcoc_ctrl_0_init_32mhz; /* Combine common and datarate specific settings */
            XCVR_RX_DIG->DCOC_CTRL_1 = com_config->dcoc_ctrl_1_init | datarate_config->dcoc_ctrl_1_init_32mhz; /* Combine common and datarate specific settings */
#endif /* RADIO_IS_GEN_2P0 */
#if RADIO_IS_GEN_3P0
            XCVR_RX_DIG->DCOC_CTRL_0 = com_config->dcoc_ctrl_0_init_32mhz | mode_datarate_config->dcoc_ctrl_0_init_32mhz; /* Combine common and datarate specific settings */
            XCVR_RX_DIG->DCOC_CTRL_1 = com_config->dcoc_ctrl_1_init | mode_datarate_config->dcoc_ctrl_1_init | datarate_config->dcoc_ctrl_1_init_32mhz; /* Combine common and datarate specific settings */
            XCVR_RX_DIG->DCOC_CTRL_2 = datarate_config->dcoc_ctrl_2_init_32mhz;
#endif /* RADIO_IS_GEN_3P0 */
        }
#endif /* RF_OSC_26MHZ == 1 */

#if RADIO_IS_GEN_3P0
        /* DCOC_CAL_GAIN */
        XCVR_RX_DIG->DCOC_CAL_GAIN = com_config->dcoc_cal_gain_init | mode_datarate_config->dcoc_cal_gain_init;

        /* DCOC_CAL_RCP */
        XCVR_RX_DIG->DCOC_CAL_RCP = com_config->dcoc_cal_rcp_init | mode_datarate_config->dcoc_cal_rcp_init;
#else
        /* DCOC_CAL_GAIN */
        XCVR_RX_DIG->DCOC_CAL_GAIN = com_config->dcoc_cal_gain_init;

        /* DCOC_CAL_RCP */
        XCVR_RX_DIG->DCOC_CAL_RCP = com_config->dcoc_cal_rcp_init;
#endif /* RADIO_IS_GEN_3P0 */

        if (first_init)
        {

            XCVR_RX_DIG->LNA_GAIN_VAL_3_0 = com_config->lna_gain_val_3_0;
            XCVR_RX_DIG->LNA_GAIN_VAL_7_4 = com_config->lna_gain_val_7_4;
            XCVR_RX_DIG->LNA_GAIN_VAL_8 = com_config->lna_gain_val_8;
            XCVR_RX_DIG->BBA_RES_TUNE_VAL_7_0 = com_config->bba_res_tune_val_7_0;
            XCVR_RX_DIG->BBA_RES_TUNE_VAL_10_8 = com_config->bba_res_tune_val_10_8;

            /* LNA_GAIN_LIN_VAL */
            XCVR_RX_DIG->LNA_GAIN_LIN_VAL_2_0 = com_config->lna_gain_lin_val_2_0_init;
            XCVR_RX_DIG->LNA_GAIN_LIN_VAL_5_3 = com_config->lna_gain_lin_val_5_3_init;
            XCVR_RX_DIG->LNA_GAIN_LIN_VAL_8_6 = com_config->lna_gain_lin_val_8_6_init;
            XCVR_RX_DIG->LNA_GAIN_LIN_VAL_9 = com_config->lna_gain_lin_val_9_init;

            /* BBA_RES_TUNE_LIN_VAL */
            XCVR_RX_DIG->BBA_RES_TUNE_LIN_VAL_3_0 = com_config->bba_res_tune_lin_val_3_0_init;
            XCVR_RX_DIG->BBA_RES_TUNE_LIN_VAL_7_4 = com_config->bba_res_tune_lin_val_7_4_init;
            XCVR_RX_DIG->BBA_RES_TUNE_LIN_VAL_10_8 = com_config->bba_res_tune_lin_val_10_8_init;

            /* BBA_STEP */
            XCVR_RX_DIG->DCOC_BBA_STEP = com_config->dcoc_bba_step_init;

            /* DCOC_TZA_STEP */
            XCVR_RX_DIG->DCOC_TZA_STEP_0 = com_config->dcoc_tza_step_00_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_1 = com_config->dcoc_tza_step_01_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_2 = com_config->dcoc_tza_step_02_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_3 = com_config->dcoc_tza_step_03_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_4 = com_config->dcoc_tza_step_04_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_5 = com_config->dcoc_tza_step_05_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_6 = com_config->dcoc_tza_step_06_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_7 = com_config->dcoc_tza_step_07_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_8 = com_config->dcoc_tza_step_08_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_9 = com_config->dcoc_tza_step_09_init;
            XCVR_RX_DIG->DCOC_TZA_STEP_10 = com_config->dcoc_tza_step_10_init;

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
            /* DCOC_CAL_FAIL and DCOC_CAL_PASS */
            XCVR_RX_DIG->DCOC_CAL_FAIL_TH = com_config->dcoc_cal_fail_th_init;
            XCVR_RX_DIG->DCOC_CAL_PASS_TH = com_config->dcoc_cal_pass_th_init;
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */
        }
        /* AGC_CTRL_0 .. _3 */
#if !RADIO_IS_GEN_3P0 /* Gen3 radio has refactored code below to cover both clock and mode/datarate dependencies */
        XCVR_RX_DIG->AGC_CTRL_0 = com_config->agc_ctrl_0_init | mode_config->agc_ctrl_0_init;
#endif

#if RF_OSC_26MHZ == 1
        {
#if RADIO_IS_GEN_3P0
            XCVR_RX_DIG->AGC_CTRL_0 = com_config->agc_ctrl_0_init| mode_config->agc_ctrl_0_init | mode_datarate_config->agc_ctrl_0_init_26mhz;
            XCVR_RX_DIG->AGC_CTRL_1 = com_config->agc_ctrl_1_init_26mhz | mode_datarate_config->agc_ctrl_1_init_26mhz; /* Combine common and datarate specific settings */
#else
#if RADIO_IS_GEN_2P1
            XCVR_RX_DIG->AGC_CTRL_0 = com_config->agc_ctrl_0_init| mode_config->agc_ctrl_0_init | mode_datarate_config->agc_ctrl_0_init_26mhz;
#endif /* RADIO_IS_GEN_2P1 */
            XCVR_RX_DIG->AGC_CTRL_1 = com_config->agc_ctrl_1_init_26mhz | datarate_config->agc_ctrl_1_init_26mhz; /* Combine common and datarate specific settings */
#endif /* RADIO_IS_GEN_3P0 */
            XCVR_RX_DIG->AGC_CTRL_2 = mode_datarate_config->agc_ctrl_2_init_26mhz;
        }
#else
        {
#if RADIO_IS_GEN_3P0
            XCVR_RX_DIG->AGC_CTRL_0 = com_config->agc_ctrl_0_init| mode_config->agc_ctrl_0_init | mode_datarate_config->agc_ctrl_0_init_32mhz;
            XCVR_RX_DIG->AGC_CTRL_1 = com_config->agc_ctrl_1_init_32mhz | mode_datarate_config->agc_ctrl_1_init_32mhz; /* Combine common and datarate specific settings */
#else
#if RADIO_IS_GEN_2P1
            XCVR_RX_DIG->AGC_CTRL_0 = com_config->agc_ctrl_0_init| mode_config->agc_ctrl_0_init | mode_datarate_config->agc_ctrl_0_init_32mhz;
#endif /* RADIO_IS_GEN_2P1 */       

            XCVR_RX_DIG->AGC_CTRL_1 = com_config->agc_ctrl_1_init_32mhz | datarate_config->agc_ctrl_1_init_32mhz; /* Combine common and datarate specific settings */
#endif /* RADIO_IS_GEN_3P0 */
            XCVR_RX_DIG->AGC_CTRL_2 = mode_datarate_config->agc_ctrl_2_init_32mhz;
        }
#endif /* RF_OSC_26MHZ == 1 */

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
                XCVR_RX_DIG->AGC_CTRL_3 = com_config->agc_ctrl_3_init | mode_datarate_config->agc_ctrl_3_init;
#else
                XCVR_RX_DIG->AGC_CTRL_3 = com_config->agc_ctrl_3_init;
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

        if (first_init)
        {
            

            /* AGC_GAIN_TBL_** */
            XCVR_RX_DIG->AGC_GAIN_TBL_03_00 = com_config->agc_gain_tbl_03_00_init;
            XCVR_RX_DIG->AGC_GAIN_TBL_07_04 = com_config->agc_gain_tbl_07_04_init;
            XCVR_RX_DIG->AGC_GAIN_TBL_11_08 = com_config->agc_gain_tbl_11_08_init;
            XCVR_RX_DIG->AGC_GAIN_TBL_15_12 = com_config->agc_gain_tbl_15_12_init;
            XCVR_RX_DIG->AGC_GAIN_TBL_19_16 = com_config->agc_gain_tbl_19_16_init;
            XCVR_RX_DIG->AGC_GAIN_TBL_23_20 = com_config->agc_gain_tbl_23_20_init;
            XCVR_RX_DIG->AGC_GAIN_TBL_26_24 = com_config->agc_gain_tbl_26_24_init;
            
            /* RSSI_CTRL_0 */
            XCVR_RX_DIG->RSSI_CTRL_0 = com_config->rssi_ctrl_0_init;
#if RADIO_IS_GEN_3P0
            XCVR_RX_DIG->RSSI_CTRL_1 = com_config->rssi_ctrl_1_init;
#endif /* RADIO_IS_GEN_3P0 */

            /* CCA_ED_LQI_0 and _1 */
            XCVR_RX_DIG->CCA_ED_LQI_CTRL_0 = com_config->cca_ed_lqi_ctrl_0_init;
            XCVR_RX_DIG->CCA_ED_LQI_CTRL_1 = com_config->cca_ed_lqi_ctrl_1_init;
        }
        
        /* Channel filter coefficients */
#if RF_OSC_26MHZ == 1
        {
            XCVR_RX_DIG->RX_CHF_COEF_0 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_0;
            XCVR_RX_DIG->RX_CHF_COEF_1 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_1;
            XCVR_RX_DIG->RX_CHF_COEF_2 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_2;
            XCVR_RX_DIG->RX_CHF_COEF_3 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_3;
            XCVR_RX_DIG->RX_CHF_COEF_4 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_4;
            XCVR_RX_DIG->RX_CHF_COEF_5 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_5;
            XCVR_RX_DIG->RX_CHF_COEF_6 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_6;
            XCVR_RX_DIG->RX_CHF_COEF_7 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_7;
            XCVR_RX_DIG->RX_CHF_COEF_8 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_8;
            XCVR_RX_DIG->RX_CHF_COEF_9 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_9;
            XCVR_RX_DIG->RX_CHF_COEF_10 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_10;
            XCVR_RX_DIG->RX_CHF_COEF_11 = mode_datarate_config->rx_chf_coeffs_26mhz.rx_chf_coef_11;
        }
#else
        {
            XCVR_RX_DIG->RX_CHF_COEF_0 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_0;
            XCVR_RX_DIG->RX_CHF_COEF_1 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_1;
            XCVR_RX_DIG->RX_CHF_COEF_2 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_2;
            XCVR_RX_DIG->RX_CHF_COEF_3 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_3;
            XCVR_RX_DIG->RX_CHF_COEF_4 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_4;
            XCVR_RX_DIG->RX_CHF_COEF_5 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_5;
            XCVR_RX_DIG->RX_CHF_COEF_6 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_6;
            XCVR_RX_DIG->RX_CHF_COEF_7 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_7;
            XCVR_RX_DIG->RX_CHF_COEF_8 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_8;
            XCVR_RX_DIG->RX_CHF_COEF_9 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_9;
            XCVR_RX_DIG->RX_CHF_COEF_10 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_10;
            XCVR_RX_DIG->RX_CHF_COEF_11 = mode_datarate_config->rx_chf_coeffs_32mhz.rx_chf_coef_11;
        }
#endif /* RF_OSC_26MHZ == 1 */
        XCVR_RX_DIG->RX_RCCAL_CTRL0 = mode_datarate_config->rx_rccal_ctrl_0;
        XCVR_RX_DIG->RX_RCCAL_CTRL1 = mode_datarate_config->rx_rccal_ctrl_1;

        XCVR_RX_DIG->AGC_MAN_AGC_IDX = mode_config->agc_man_agc_idx;
        /*******************************************************************************/
        /* XCVR_TSM configs */
        /*******************************************************************************/
        XCVR_TSM->CTRL = com_config->tsm_ctrl;

#if RADIO_IS_GEN_2P0
        if ((mode_config->radio_mode != ZIGBEE_MODE) && (mode_config->radio_mode != BLE_MODE))
        {
            XCVR_TSM->CTRL &= ~XCVR_TSM_CTRL_DATA_PADDING_EN_MASK;
        }
#endif /* RADIO_IS_GEN_2P0 */
        if (first_init)
        {
#if !RADIO_IS_GEN_2P1
            XCVR_MISC->LPPS_CTRL = com_config->lpps_ctrl_init; /* Register is in XCVR_MISC but grouped with TSM for intialization */
#endif /* !RADIO_IS_GEN_2P1 */

            XCVR_TSM->OVRD2 = com_config->tsm_ovrd2_init;
            /* TSM registers and timings - dependent upon clock frequency */
#if RF_OSC_26MHZ == 1
            {
                XCVR_TSM->END_OF_SEQ = com_config->end_of_seq_init_26mhz;
                XCVR_TSM->FAST_CTRL2 = com_config->tsm_fast_ctrl2_init_26mhz;
                XCVR_TSM->RECYCLE_COUNT = com_config->recycle_count_init_26mhz;
                XCVR_TSM->TIMING14 = com_config->tsm_timing_14_init_26mhz;
                XCVR_TSM->TIMING16 = com_config->tsm_timing_16_init_26mhz;
                XCVR_TSM->TIMING25 = com_config->tsm_timing_25_init_26mhz;
                XCVR_TSM->TIMING27 = com_config->tsm_timing_27_init_26mhz;
                XCVR_TSM->TIMING28 = com_config->tsm_timing_28_init_26mhz;
                XCVR_TSM->TIMING29 = com_config->tsm_timing_29_init_26mhz;
                XCVR_TSM->TIMING30 = com_config->tsm_timing_30_init_26mhz;
                XCVR_TSM->TIMING31 = com_config->tsm_timing_31_init_26mhz;
                XCVR_TSM->TIMING32 = com_config->tsm_timing_32_init_26mhz;
                XCVR_TSM->TIMING33 = com_config->tsm_timing_33_init_26mhz;
                XCVR_TSM->TIMING36 = com_config->tsm_timing_36_init_26mhz;
                XCVR_TSM->TIMING37 = com_config->tsm_timing_37_init_26mhz;
                XCVR_TSM->TIMING39 = com_config->tsm_timing_39_init_26mhz;
                XCVR_TSM->TIMING40 = com_config->tsm_timing_40_init_26mhz;
                XCVR_TSM->TIMING41 = com_config->tsm_timing_41_init_26mhz;
                XCVR_TSM->TIMING52 = com_config->tsm_timing_52_init_26mhz;
                XCVR_TSM->TIMING54 = com_config->tsm_timing_54_init_26mhz;
                XCVR_TSM->TIMING55 = com_config->tsm_timing_55_init_26mhz;
                XCVR_TSM->TIMING56 = com_config->tsm_timing_56_init_26mhz;
            }
#else
            {
                XCVR_TSM->END_OF_SEQ = com_config->end_of_seq_init_32mhz;
                XCVR_TSM->FAST_CTRL2 = com_config->tsm_fast_ctrl2_init_32mhz;
                XCVR_TSM->RECYCLE_COUNT = com_config->recycle_count_init_32mhz;
                XCVR_TSM->TIMING14 = com_config->tsm_timing_14_init_32mhz;
                XCVR_TSM->TIMING16 = com_config->tsm_timing_16_init_32mhz;
                XCVR_TSM->TIMING25 = com_config->tsm_timing_25_init_32mhz;
                XCVR_TSM->TIMING27 = com_config->tsm_timing_27_init_32mhz;
                XCVR_TSM->TIMING28 = com_config->tsm_timing_28_init_32mhz;
                XCVR_TSM->TIMING29 = com_config->tsm_timing_29_init_32mhz;
                XCVR_TSM->TIMING30 = com_config->tsm_timing_30_init_32mhz;
                XCVR_TSM->TIMING31 = com_config->tsm_timing_31_init_32mhz;
                XCVR_TSM->TIMING32 = com_config->tsm_timing_32_init_32mhz;
                XCVR_TSM->TIMING33 = com_config->tsm_timing_33_init_32mhz;
                XCVR_TSM->TIMING36 = com_config->tsm_timing_36_init_32mhz;
                XCVR_TSM->TIMING37 = com_config->tsm_timing_37_init_32mhz;
                XCVR_TSM->TIMING39 = com_config->tsm_timing_39_init_32mhz;
                XCVR_TSM->TIMING40 = com_config->tsm_timing_40_init_32mhz;
                XCVR_TSM->TIMING41 = com_config->tsm_timing_41_init_32mhz;
                XCVR_TSM->TIMING52 = com_config->tsm_timing_52_init_32mhz;
                XCVR_TSM->TIMING54 = com_config->tsm_timing_54_init_32mhz;
                XCVR_TSM->TIMING55 = com_config->tsm_timing_55_init_32mhz;
                XCVR_TSM->TIMING56 = com_config->tsm_timing_56_init_32mhz;
            }
#endif /* RF_OSC_26MHZ == 1 */
            /* TSM timings independent of clock frequency */
            XCVR_TSM->TIMING00 = com_config->tsm_timing_00_init;
            XCVR_TSM->TIMING01 = com_config->tsm_timing_01_init;
            XCVR_TSM->TIMING02 = com_config->tsm_timing_02_init;
            XCVR_TSM->TIMING03 = com_config->tsm_timing_03_init;
            XCVR_TSM->TIMING04 = com_config->tsm_timing_04_init;
            XCVR_TSM->TIMING05 = com_config->tsm_timing_05_init;
            XCVR_TSM->TIMING06 = com_config->tsm_timing_06_init;
            XCVR_TSM->TIMING07 = com_config->tsm_timing_07_init;
            XCVR_TSM->TIMING08 = com_config->tsm_timing_08_init;
            XCVR_TSM->TIMING09 = com_config->tsm_timing_09_init;
            XCVR_TSM->TIMING10 = com_config->tsm_timing_10_init;
            XCVR_TSM->TIMING11 = com_config->tsm_timing_11_init;
            XCVR_TSM->TIMING12 = com_config->tsm_timing_12_init;
            XCVR_TSM->TIMING13 = com_config->tsm_timing_13_init;
            XCVR_TSM->TIMING15 = com_config->tsm_timing_15_init;
            XCVR_TSM->TIMING17 = com_config->tsm_timing_17_init;
            XCVR_TSM->TIMING18 = com_config->tsm_timing_18_init;
            XCVR_TSM->TIMING19 = com_config->tsm_timing_19_init;
            XCVR_TSM->TIMING20 = com_config->tsm_timing_20_init;
            XCVR_TSM->TIMING21 = com_config->tsm_timing_21_init;
            XCVR_TSM->TIMING22 = com_config->tsm_timing_22_init;
            XCVR_TSM->TIMING23 = com_config->tsm_timing_23_init;
            XCVR_TSM->TIMING24 = com_config->tsm_timing_24_init;
            XCVR_TSM->TIMING26 = com_config->tsm_timing_26_init;
            XCVR_TSM->TIMING34 = com_config->tsm_timing_34_init;
            XCVR_TSM->TIMING35 = com_config->tsm_timing_35_init;
            XCVR_TSM->TIMING38 = com_config->tsm_timing_38_init;
            XCVR_TSM->TIMING51 = com_config->tsm_timing_51_init;
            XCVR_TSM->TIMING53 = com_config->tsm_timing_53_init;
            XCVR_TSM->TIMING57 = com_config->tsm_timing_57_init;
            XCVR_TSM->TIMING58 = com_config->tsm_timing_58_init;

            XCVR_TSM->PA_RAMP_TBL0 = com_config->pa_ramp_tbl_0_init;
            XCVR_TSM->PA_RAMP_TBL1 = com_config->pa_ramp_tbl_1_init;
#if RADIO_IS_GEN_3P0
            XCVR_TSM->PA_RAMP_TBL2 = com_config->pa_ramp_tbl_2_init;
            XCVR_TSM->PA_RAMP_TBL3 = com_config->pa_ramp_tbl_3_init;

            /* Apply PA_RAMP_TIME == 4usec adjustments to TX_WD signals */
#if (PA_RAMP_TIME == 4)
            XCVR_TSM->TIMING00 += B1(2UL); /* (Controlled signal is bb_ldo_hf_en) */
            XCVR_TSM->TIMING01 += B1(2UL); /* (Controlled signal is bb_ldo_adcdac_en) */
            XCVR_TSM->TIMING03 += B1(2UL); /* (Controlled signal is bb_ldo_pd_en) */
            XCVR_TSM->TIMING04 += B1(2UL); /* (Controlled signal is bb_ldo_fdbk_en) */
            XCVR_TSM->TIMING05 += B1(2UL); /* (Controlled signal is bb_ldo_vcolo_en) */
            XCVR_TSM->TIMING06 += B1(2UL); /* (Controlled signal is bb_ldo_vtref_en) */
            XCVR_TSM->TIMING10 += B1(2UL); /* (Controlled signal is bb_xtal_pll_ref_clk_en) */
            XCVR_TSM->TIMING11 += B1(2UL); /* (Controlled signal is bb_xtal_dac_ref_clk_en) */
            XCVR_TSM->TIMING15 += B1(2UL); /* (Controlled signal is sy_vco_en) */
            XCVR_TSM->TIMING17 += B1(2UL); /* (Controlled signal is sy_lo_tx_buf_en) */
            XCVR_TSM->TIMING18 += B1(2UL); /* (Controlled signal is sy_divn_en) */
            XCVR_TSM->TIMING20 += B1(2UL); /*  (Controlled signal is sy_pd_en) */
            XCVR_TSM->TIMING21 += B1(2UL); /* (Controlled signal is sy_lo_divn_en) */
            XCVR_TSM->TIMING23 += B1(2UL); /* (Controlled signal is sy_lo_tx_en) */
            XCVR_TSM->TIMING26 += B1(2UL); /* (Controlled signal is tx_pa_en) */ 
            XCVR_TSM->TIMING34 += B1(2UL); /* (Controlled signal is pll_dig_en) */
            XCVR_TSM->TIMING35 += B1(2UL); /* (Controlled signal is tx_dig_en) */
            XCVR_TSM->TIMING38 += B1(2UL); /* (Controlled signal is sigma_delta_en) */
            XCVR_TSM->TIMING58 += B1(2UL)  /* (Controlled signal is tx_hpm_dac_en) */
            temp = XCVR_TSM->TIMING14;
            temp &= 0xFFFF0000UL; 
            temp |= B0(END_OF_TX_WU - 4UL) | B1(END_OF_TX_WU + 1UL); /* (Controlled signal is sy_pd_cycle_slip_ld_ft_en) */
            XCVR_TSM->TIMING14 = temp;
#endif /* (PA_RAMP_TIME == 4) */
#endif /* RADIO_IS_GEN_3P0 */
        }
        
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
        if (mode_config->radio_mode == ZIGBEE_MODE)
        {
            temp = XCVR_TSM->TIMING35;
            temp &= ~(B0(0xFF));
            if (DATA_PADDING_EN == 1)
            {
                temp |= B0(END_OF_TX_WU - 2UL - 8UL); /* Adjust for data padding time */
            }
            else
            {
                temp |= B0(END_OF_TX_WU - 2UL); /* No data padding adjustment */
            }
            XCVR_TSM->TIMING35 = temp;
        }
#else

        if ((mode_datarate_config->radio_mode == MSK) && ((mode_datarate_config->data_rate == DR_500KBPS) || (mode_datarate_config->data_rate == DR_250KBPS)))
        {
            /* Apply a specific value of TX_DIG_EN which assumes no DATA PADDING */
            XCVR_TSM->TIMING35 = com_config->tsm_timing_35_init | B0(TX_DIG_EN_ASSERT_MSK500); /* LSbyte is mode specific */
        }
        else
        {
            XCVR_TSM->TIMING35 = com_config->tsm_timing_35_init | mode_config->tsm_timing_35_init; /* LSbyte is mode specific, other bytes are common */
        }
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */
    
#if (RADIO_IS_GEN_3P0 && GFSK_250KBPS_TIMING_ADJUST)
        /* Adjust TSM RX WU and WD timings to allow for custom DCOC calibration time for 250Kbps data rates */
        if ((mode_config->radio_mode != ZIGBEE_MODE) && (mode_datarate_config->data_rate == DR_250KBPS))
        {
#if RF_OSC_26MHZ == 1
            {
                XCVR_RX_DIG->DCOC_CTRL_0 = (com_config->dcoc_ctrl_0_init_26mhz+XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_DURATION(GFSK_250K_DC_CAL_STEP_DELAY-4UL)) |  /* -4 adjustment is due to sensitivity fallout that showed up in v1.99 settings */
                                mode_datarate_config->dcoc_ctrl_0_init_26mhz; /* Combine common and datarate specific settings */
                XCVR_TSM->END_OF_SEQ = com_config->end_of_seq_init_26mhz+GFSK_250K_RX_WU_WD_ADJUST; /* Adjust both WU and WD */
                XCVR_TSM->RECYCLE_COUNT = com_config->recycle_count_init_26mhz+(B2(GFSK_250K_DC_CAL_STEP_DELAY*3) | B0(GFSK_250K_DC_CAL_STEP_DELAY*3));
                XCVR_TSM->TIMING14 = com_config->tsm_timing_14_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING16 = com_config->tsm_timing_16_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING25 = com_config->tsm_timing_25_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING27 = com_config->tsm_timing_27_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING29 = com_config->tsm_timing_29_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING30 = com_config->tsm_timing_30_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING31 = com_config->tsm_timing_31_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING32 = com_config->tsm_timing_32_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING33 = com_config->tsm_timing_33_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING36 = com_config->tsm_timing_36_init_26mhz+GFSK_250K_RX_WU_WD_ADJUST; /* Adjust both WU and WD */
                XCVR_TSM->TIMING37 = com_config->tsm_timing_37_init_26mhz+GFSK_250K_RX_WU_WD_ADJUST; /* Adjust both WU and WD */
                XCVR_TSM->TIMING39 = com_config->tsm_timing_39_init_26mhz+GFSK_250K_RX_WU_WD_ADJUST; /* Adjust both WU and WD */
                XCVR_TSM->TIMING40 = com_config->tsm_timing_40_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING55 = com_config->tsm_timing_55_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING56 = com_config->tsm_timing_56_init_26mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
            }
#else
            {
                XCVR_RX_DIG->DCOC_CTRL_0 = (com_config->dcoc_ctrl_0_init_32mhz+XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_DURATION(GFSK_250K_DC_CAL_STEP_DELAY)) | 
                    mode_datarate_config->dcoc_ctrl_0_init_32mhz; /* Combine common and datarate specific settings */
                XCVR_TSM->END_OF_SEQ = com_config->end_of_seq_init_32mhz+GFSK_250K_RX_WU_WD_ADJUST; /* Adjust both WU and WD */
                XCVR_TSM->RECYCLE_COUNT = com_config->recycle_count_init_32mhz+(B2(GFSK_250K_DC_CAL_STEP_DELAY*3) | B0(GFSK_250K_DC_CAL_STEP_DELAY*3));
                XCVR_TSM->TIMING14 = com_config->tsm_timing_14_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING16 = com_config->tsm_timing_16_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING25 = com_config->tsm_timing_25_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING27 = com_config->tsm_timing_27_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING29 = com_config->tsm_timing_29_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING30 = com_config->tsm_timing_30_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING31 = com_config->tsm_timing_31_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING32 = com_config->tsm_timing_32_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING33 = com_config->tsm_timing_33_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING36 = com_config->tsm_timing_36_init_32mhz+GFSK_250K_RX_WU_WD_ADJUST; /* Adjust both WU and WD */
                XCVR_TSM->TIMING37 = com_config->tsm_timing_37_init_32mhz+GFSK_250K_RX_WU_WD_ADJUST; /* Adjust both WU and WD */
                XCVR_TSM->TIMING39 = com_config->tsm_timing_39_init_32mhz+GFSK_250K_RX_WU_WD_ADJUST; /* Adjust both WU and WD */
                XCVR_TSM->TIMING40 = com_config->tsm_timing_40_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING52 = com_config->tsm_timing_52_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING54 = com_config->tsm_timing_54_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING55 = com_config->tsm_timing_55_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
                XCVR_TSM->TIMING56 = com_config->tsm_timing_56_init_32mhz+GFSK_250K_RX_WD_ONLY_ADJUST;
            }
#endif /* RF_OSC_26MHZ == 1 */
            /* TSM timings independent of clock frequency */
            XCVR_TSM->TIMING00 = com_config->tsm_timing_00_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING01 = com_config->tsm_timing_01_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING02 = com_config->tsm_timing_02_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING03 = com_config->tsm_timing_03_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING04 = com_config->tsm_timing_04_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING05 = com_config->tsm_timing_05_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING06 = com_config->tsm_timing_06_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING10 = com_config->tsm_timing_10_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING12 = com_config->tsm_timing_12_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING15 = com_config->tsm_timing_15_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING18 = com_config->tsm_timing_18_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING20 = com_config->tsm_timing_20_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING21 = com_config->tsm_timing_21_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING22 = com_config->tsm_timing_22_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING34 = com_config->tsm_timing_34_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING38 = com_config->tsm_timing_38_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING51 = com_config->tsm_timing_51_init+GFSK_250K_RX_WD_ONLY_ADJUST;
            XCVR_TSM->TIMING53 = com_config->tsm_timing_53_init+GFSK_250K_RX_WD_ONLY_ADJUST;
        }
        else
        /* program original TSM RX WU & WD timings */        
        {
#if RF_OSC_26MHZ == 1
            {
                XCVR_RX_DIG->DCOC_CTRL_0 = com_config->dcoc_ctrl_0_init_26mhz | mode_datarate_config->dcoc_ctrl_0_init_26mhz; /* Combine common and datarate specific settings */
                XCVR_TSM->END_OF_SEQ = com_config->end_of_seq_init_26mhz;
                XCVR_TSM->RECYCLE_COUNT = com_config->recycle_count_init_26mhz;
                XCVR_TSM->TIMING14 = com_config->tsm_timing_14_init_26mhz;
                XCVR_TSM->TIMING16 = com_config->tsm_timing_16_init_26mhz;
                XCVR_TSM->TIMING25 = com_config->tsm_timing_25_init_26mhz;
                XCVR_TSM->TIMING27 = com_config->tsm_timing_27_init_26mhz;
                XCVR_TSM->TIMING29 = com_config->tsm_timing_29_init_26mhz;
                XCVR_TSM->TIMING30 = com_config->tsm_timing_30_init_26mhz;
                XCVR_TSM->TIMING31 = com_config->tsm_timing_31_init_26mhz;
                XCVR_TSM->TIMING32 = com_config->tsm_timing_32_init_26mhz;
                XCVR_TSM->TIMING33 = com_config->tsm_timing_33_init_26mhz;
                XCVR_TSM->TIMING36 = com_config->tsm_timing_36_init_26mhz;
                XCVR_TSM->TIMING37 = com_config->tsm_timing_37_init_26mhz;
                XCVR_TSM->TIMING39 = com_config->tsm_timing_39_init_26mhz;
                XCVR_TSM->TIMING40 = com_config->tsm_timing_40_init_26mhz;
                XCVR_TSM->TIMING55 = com_config->tsm_timing_55_init_26mhz;
                XCVR_TSM->TIMING56 = com_config->tsm_timing_56_init_26mhz;
            }
#else
            {
                XCVR_RX_DIG->DCOC_CTRL_0 = com_config->dcoc_ctrl_0_init_32mhz | mode_datarate_config->dcoc_ctrl_0_init_32mhz; /* Combine common and datarate specific settings */
                XCVR_TSM->END_OF_SEQ = com_config->end_of_seq_init_32mhz;
                XCVR_TSM->FAST_CTRL2 = com_config->tsm_fast_ctrl2_init_32mhz;
                XCVR_TSM->RECYCLE_COUNT = com_config->recycle_count_init_32mhz;
                XCVR_TSM->TIMING14 = com_config->tsm_timing_14_init_32mhz;
                XCVR_TSM->TIMING16 = com_config->tsm_timing_16_init_32mhz;
                XCVR_TSM->TIMING25 = com_config->tsm_timing_25_init_32mhz;
                XCVR_TSM->TIMING27 = com_config->tsm_timing_27_init_32mhz;
                XCVR_TSM->TIMING29 = com_config->tsm_timing_29_init_32mhz;
                XCVR_TSM->TIMING30 = com_config->tsm_timing_30_init_32mhz;
                XCVR_TSM->TIMING31 = com_config->tsm_timing_31_init_32mhz;
                XCVR_TSM->TIMING32 = com_config->tsm_timing_32_init_32mhz;
                XCVR_TSM->TIMING33 = com_config->tsm_timing_33_init_32mhz;
                XCVR_TSM->TIMING36 = com_config->tsm_timing_36_init_32mhz;
                XCVR_TSM->TIMING37 = com_config->tsm_timing_37_init_32mhz;
                XCVR_TSM->TIMING39 = com_config->tsm_timing_39_init_32mhz;
                XCVR_TSM->TIMING40 = com_config->tsm_timing_40_init_32mhz;
                XCVR_TSM->TIMING52 = com_config->tsm_timing_52_init_32mhz;
                XCVR_TSM->TIMING54 = com_config->tsm_timing_54_init_32mhz;
                XCVR_TSM->TIMING55 = com_config->tsm_timing_55_init_32mhz;
                XCVR_TSM->TIMING56 = com_config->tsm_timing_56_init_32mhz;
            }
#endif /* RF_OSC_26MHZ == 1 */
            /* TSM timings independent of clock frequency */
            XCVR_TSM->TIMING00 = com_config->tsm_timing_00_init;
            XCVR_TSM->TIMING01 = com_config->tsm_timing_01_init;
            XCVR_TSM->TIMING02 = com_config->tsm_timing_02_init;
            XCVR_TSM->TIMING03 = com_config->tsm_timing_03_init;
            XCVR_TSM->TIMING04 = com_config->tsm_timing_04_init;
            XCVR_TSM->TIMING05 = com_config->tsm_timing_05_init;
            XCVR_TSM->TIMING06 = com_config->tsm_timing_06_init;
            XCVR_TSM->TIMING10 = com_config->tsm_timing_10_init;
            XCVR_TSM->TIMING12 = com_config->tsm_timing_12_init;
            XCVR_TSM->TIMING15 = com_config->tsm_timing_15_init;
            XCVR_TSM->TIMING18 = com_config->tsm_timing_18_init;
            XCVR_TSM->TIMING20 = com_config->tsm_timing_20_init;
            XCVR_TSM->TIMING21 = com_config->tsm_timing_21_init;
            XCVR_TSM->TIMING22 = com_config->tsm_timing_22_init;
            XCVR_TSM->TIMING34 = com_config->tsm_timing_34_init;
            XCVR_TSM->TIMING38 = com_config->tsm_timing_38_init;
            XCVR_TSM->TIMING51 = com_config->tsm_timing_51_init;
            XCVR_TSM->TIMING53 = com_config->tsm_timing_53_init;
        }

#endif /* RADIO_IS_GEN_3P0 && GFSK_250KBPS_TIMING_ADJUST */


        /*******************************************************************************/
        /* XCVR_TX_DIG configs */
        /*******************************************************************************/
#if RF_OSC_26MHZ == 1
        {
            XCVR_TX_DIG->FSK_SCALE = mode_datarate_config->tx_fsk_scale_26mhz; /* Applies only to 802.15.4 & MSK but won't harm other protocols */
            XCVR_TX_DIG->GFSK_COEFF1 = mode_config->tx_gfsk_coeff1_26mhz;
            XCVR_TX_DIG->GFSK_COEFF2 = mode_config->tx_gfsk_coeff2_26mhz;
        }
#else
        {
            XCVR_TX_DIG->FSK_SCALE = mode_datarate_config->tx_fsk_scale_32mhz; /* Applies only to 802.15.4 & MSK but won't harm other protocols */
            XCVR_TX_DIG->GFSK_COEFF1 = mode_config->tx_gfsk_coeff1_32mhz;
            XCVR_TX_DIG->GFSK_COEFF2 = mode_config->tx_gfsk_coeff2_32mhz;
        }
#endif /* RF_OSC_26MHZ == 1 */
        if (first_init)
        {
            XCVR_TX_DIG->CTRL = com_config->tx_ctrl;
            XCVR_TX_DIG->DATA_PADDING = com_config->tx_data_padding;
            XCVR_TX_DIG->DFT_PATTERN = com_config->tx_dft_pattern;
#if ((!RADIO_IS_GEN_2P1) && (!RADIO_IS_GEN_3P0))
            XCVR_TX_DIG->RF_DFT_BIST_1 = com_config->rf_dft_bist_1;
            XCVR_TX_DIG->RF_DFT_BIST_2 = com_config->rf_dft_bist_2;
#endif /* !RADIO_IS_GEN_2P1 */
        }
        XCVR_TX_DIG->GFSK_CTRL = mode_config->tx_gfsk_ctrl;
    }

    return config_status;
}

void XCVR_Reset(void)
{
#if RADIO_IS_GEN_3P0
    RSIM->SW_CONFIG |= RSIM_SW_CONFIG_RADIO_RESET_BIT_MASK; /* Assert radio software reset */
    asm("NOP");
    asm("NOP");
    RSIM->SW_CONFIG &= ~RSIM_SW_CONFIG_RADIO_RESET_BIT_MASK; /* De-assert radio software reset */
    asm("NOP");
    asm("NOP");
    RSIM->SW_CONFIG &= ~RSIM_SW_CONFIG_RADIO_RESET_BIT_MASK; /* De-assert radio software reset a second time (following Gen2 behavior) */
#else
    RSIM->CONTROL |= RSIM_CONTROL_RADIO_RESET_BIT_MASK; /* Assert radio software reset */
    asm("NOP");
    asm("NOP");
    RSIM->CONTROL &= ~RSIM_CONTROL_RADIO_RESET_BIT_MASK; /* De-assert radio software reset */
    asm("NOP");
    asm("NOP");
    RSIM->CONTROL &= ~RSIM_CONTROL_RADIO_RESET_BIT_MASK; /* De-assert radio software reset a second time per RADIO_RESET bit description */
#endif /* RADIO_IS_GEN_3P0 */
}

xcvrStatus_t XCVR_ChangeMode (radio_mode_t new_radio_mode, data_rate_t new_data_rate) /* Change from one radio mode to another */
{
    xcvrStatus_t status;
    const xcvr_mode_datarate_config_t * mode_datarate_config;
    const xcvr_datarate_config_t * datarate_config ;
    const xcvr_mode_config_t * radio_mode_cfg;
    const xcvr_common_config_t * radio_common_config;

    status = XCVR_GetDefaultConfig(new_radio_mode, new_data_rate, (void *)&radio_common_config, (void *)&radio_mode_cfg, (void *)&mode_datarate_config, (void *)&datarate_config );
    
    if (status == gXcvrSuccess_c)
    {
        status = XCVR_Configure((const xcvr_common_config_t *)radio_common_config, 
                                (const xcvr_mode_config_t *)radio_mode_cfg, 
                                (const xcvr_mode_datarate_config_t *)mode_datarate_config, 
                                (const xcvr_datarate_config_t *)datarate_config, XCVR_MODE_CHANGE);
        current_xcvr_config.radio_mode = new_radio_mode;
        current_xcvr_config.data_rate = datarate_config->data_rate;
    }

    return status;
}

void XCVR_EnaNBRSSIMeas( uint8_t IIRnbEnable )
{
    if (IIRnbEnable)
    {
        XCVR_RX_DIG->RSSI_CTRL_0 |= XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_CW_WEIGHT_MASK;
    }
    else
    {
        XCVR_RX_DIG->RSSI_CTRL_0 &= ~XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_CW_WEIGHT_MASK;
    }
}

xcvrStatus_t XCVR_OverrideFrequency ( uint32_t freq, uint32_t refOsc )
{
    double integer_used_in_Hz, 
           integer_used_in_LSB, 
           numerator_fraction, 
           numerator_in_Hz, 
           numerator_in_LSB, 
           numerator_unrounded,
           real_int_and_fraction, 
           real_fraction, 
           requested_freq_in_LSB,
           sdm_lsb;
    uint32_t temp;
    static uint32_t integer_truncated,
                    integer_to_use;
    static int32_t numerator_rounded;
    const  uint32_t denominator = 0x08000000UL - 1UL; /* Configure the PLL Denominator to the max for best resolution */
    /* Configure for Coarse Tune */
    uint32_t coarse_tune_target = freq / 1000000;
    
    /* Write the Low Port Denomintor value */
    XCVR_PLL_DIG->LPM_SDM_CTRL3 = denominator;
    
    temp  =  XCVR_PLL_DIG->CTUNE_CTRL;
    temp &= ~XCVR_PLL_DIG_CTUNE_CTRL_CTUNE_TARGET_MANUAL_MASK;
    temp |=  XCVR_PLL_DIG_CTUNE_CTRL_CTUNE_TARGET_MANUAL(coarse_tune_target);
    XCVR_PLL_DIG->CTUNE_CTRL = temp;

    /* Calculate the Low Port values */

    sdm_lsb = (double)refOsc / (denominator/2);
    real_int_and_fraction = freq / (refOsc * 2.0);
    integer_truncated = (uint32_t) trunc(real_int_and_fraction);
    real_fraction = real_int_and_fraction - integer_truncated;

    if (real_fraction > 0.5) 
    {
        integer_to_use = integer_truncated + 1;
    } 
    else                     
    {
        integer_to_use = integer_truncated;
    }

    numerator_fraction = real_int_and_fraction - integer_to_use;

    integer_used_in_Hz  = integer_to_use * refOsc * 2;
    integer_used_in_LSB = integer_used_in_Hz / sdm_lsb;

    numerator_in_Hz  = numerator_fraction * refOsc * 2;
    numerator_in_LSB = numerator_in_Hz    / sdm_lsb;

    requested_freq_in_LSB = integer_used_in_LSB + numerator_in_LSB;
    numerator_unrounded = (requested_freq_in_LSB - integer_used_in_LSB);
    numerator_rounded = (int32_t) round(numerator_unrounded);

    /* Write the Low Port Integer and Numerator */
    temp  =  XCVR_PLL_DIG->LPM_SDM_CTRL1;
    temp &= ~XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_MASK;
    temp |= (XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG(integer_to_use) | 
             XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE_MASK);
    XCVR_PLL_DIG->LPM_SDM_CTRL1 = temp;
    XCVR_PLL_DIG->LPM_SDM_CTRL2 = numerator_rounded;

    return gXcvrSuccess_c;
}

void XCVR_RegisterPanicCb ( panic_fptr fptr ) /* Allow upper layers to provide PANIC callback */
{
    s_PanicFunctionPtr = fptr;
}

void XcvrPanic(XCVR_PANIC_ID_T panic_id, uint32_t panic_address)
{
    if ( s_PanicFunctionPtr != NULL)
    {
        s_PanicFunctionPtr(panic_id, panic_address, 0, 0);
    }
    else
    {
        uint8_t dummy;

        while(1)
        {
            dummy = dummy;
        }
    }
}

healthStatus_t XCVR_HealthCheck ( void ) /* Allow upper layers to poll the radio health */
{
    return (healthStatus_t)NO_ERRORS;
}

void XCVR_FadLppsControl(FAD_LPPS_CTRL_T control)
{
}

/* Helper function to map radio mode to LL usage */
link_layer_t map_mode_to_ll(radio_mode_t mode)
{
    link_layer_t llret;
    switch (mode)
    {
        case BLE_MODE:
            llret = BLE_LL;
            break;
        case ZIGBEE_MODE:
            llret = ZIGBEE_LL;
            break;
        case ANT_MODE:
            llret = ANT_LL;
            break;
        case GFSK_BT_0p5_h_0p5:
        case GFSK_BT_0p5_h_0p32:
        case GFSK_BT_0p5_h_0p7:
        case GFSK_BT_0p5_h_1p0:
        case GFSK_BT_0p3_h_0p5:
        case GFSK_BT_0p7_h_0p5:
        case MSK: 
            llret = GENFSK_LL;
            break;
        default:
            llret = UNASSIGNED_LL;
            break;
    }
    return llret;
}

#if RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1
void XCVR_SetBSM_NTW_Address(uint32_t bsm_ntw_address)
{
    XCVR_PHY->NTW_ADR_BSM = bsm_ntw_address;
}

uint32_t XCVR_GetBSM_NTW_Address(void)
{
    return XCVR_PHY->NTW_ADR_BSM;
}
#endif /* RADIO_IS_GEN_3P0 */

/* Setup IRQ mapping to LL interrupt outputs in XCVR_CTRL */
xcvrStatus_t XCVR_SetIRQMapping(radio_mode_t irq0_mapping, radio_mode_t irq1_mapping)
{
    link_layer_t int0 = map_mode_to_ll(irq0_mapping);
    link_layer_t int1 = map_mode_to_ll(irq1_mapping);
    xcvrStatus_t statusret;
    /* Make sure the two LL's requested aren't the same */
    if (int0 == int1)
    {
        statusret = gXcvrInvalidParameters_c;
    }
    else
    {
        uint32_t temp;
        temp = XCVR_MISC->XCVR_CTRL;
        temp &= ~(XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK | XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK);
        temp |= (XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL(int0) | XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL(int1));
        XCVR_MISC->XCVR_CTRL = temp;
        statusret = gXcvrSuccess_c;
    }
    return statusret;
}

/* Get current state of IRQ mapping for either  radio INT0 or INT1 */
link_layer_t XCVR_GetIRQMapping(uint8_t int_num)
{
    link_layer_t ll_int;
    if (int_num == 0)
    {
        ll_int = (link_layer_t)((XCVR_MISC->XCVR_CTRL & XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK)>>XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_SHIFT);
    }
    else
    {
        ll_int = (link_layer_t)((XCVR_MISC->XCVR_CTRL & XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK)>>XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_SHIFT);
    }
    return ll_int;
}

/* Get current state of radio mode and data rate */
xcvrStatus_t XCVR_GetCurrentConfig(xcvr_currConfig_t * curr_config)
{
    xcvrStatus_t status = gXcvrInvalidParameters_c;
    if (curr_config != NULL)
    {
        curr_config->radio_mode = current_xcvr_config.radio_mode;
        curr_config->data_rate = current_xcvr_config.data_rate;
        status = gXcvrSuccess_c;
    }
    return status;
}

/* ************************ */
/* Customer level trim functions */
/* ************************ */
xcvrStatus_t XCVR_SetXtalTrim(uint8_t xtalTrim)
{
    xcvrStatus_t status = gXcvrInvalidParameters_c;

    if ((xtalTrim & 0x80) == 0)
    {
        uint32_t temp;
        temp = RSIM->ANA_TRIM;
        temp &= ~RSIM_ANA_TRIM_BB_XTAL_TRIM_MASK;
        RSIM->ANA_TRIM = temp | RSIM_ANA_TRIM_BB_XTAL_TRIM(xtalTrim);
        status = gXcvrSuccess_c;
    }
    return status;
}

uint8_t  XCVR_GetXtalTrim(void)
{
    uint8_t temp_xtal;
    temp_xtal = ((RSIM->ANA_TRIM & RSIM_ANA_TRIM_BB_XTAL_TRIM_MASK)>>RSIM_ANA_TRIM_BB_XTAL_TRIM_SHIFT);
    return temp_xtal;
}

/* DCOC DAC Trim */
xcvrStatus_t XCVR_CalculateDcocDacTrims(xcvr_DcocDacTrim_t * trim_values)
{
    xcvrStatus_t status = gXcvrInvalidParameters_c;
    uint32_t temp; 
    if (trim_values != NULL)
    {
        uint32_t end_of_rx_wu = 0;
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
        /* store current DCOC_CAL_CHECK_EN value and disable it during initial dc calibration */
        uint8_t dcoc_cal_check_en_stack = (XCVR_RX_DIG->DCOC_CTRL_0&XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN_MASK)>>XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN_SHIFT;
        XCVR_RX_DIG->DCOC_CTRL_0 &= ~XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN_MASK;
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

#if RF_OSC_26MHZ == 1
        XCVR_OverrideFrequency(CAL_AT_FREQ_HZ, 26000000UL);
#else
        XCVR_OverrideFrequency(CAL_AT_FREQ_HZ, 32000000UL);        
#endif

        XCVR_ForceRxWu();
        /* Wait for TSM to reach the end of warmup (unless you want to capture some samples during DCOC cal phase) */
        temp = XCVR_TSM->END_OF_SEQ;
        end_of_rx_wu = (temp & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >> XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;
        while ((( XCVR_MISC->XCVR_STATUS & XCVR_CTRL_XCVR_STATUS_TSM_COUNT_MASK) >> XCVR_CTRL_XCVR_STATUS_TSM_COUNT_SHIFT ) != end_of_rx_wu) {};
        if (!rx_bba_dcoc_dac_trim_DCest())
        {
            status = gXcvrTrimFailure_c;
        }
        else
        {
            status = gXcvrSuccess_c;
        }
        DCOC_DAC_INIT_Cal(0);
        XCVR_ForceRxWd();
        
        /* Release channel over-rides and return PLL to Link Layer Control */
        /* Remove any PLL settings that caused out-of-band receive operations (for safety) */
#if RF_OSC_26MHZ == 1
        XCVR_OverrideFrequency(2402000000UL, 26000000UL);
#else
        XCVR_OverrideFrequency(2402000000UL, 32000000UL);        
#endif
        XCVR_OverrideChannel(0xFF,0);

        /* Store trim values to a structure for later re-use */
        if (status == gXcvrSuccess_c)
        {
            status = XCVR_GetDcocDacTrims(trim_values);
        }

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
        XCVR_RX_DIG->DCOC_CTRL_0 |= XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN(dcoc_cal_check_en_stack); /* restore previously programmed value */
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

    }
    return status;
}

xcvrStatus_t XCVR_SetDcocDacTrims(const xcvr_DcocDacTrim_t * trim_values)
{
    xcvrStatus_t status = gXcvrInvalidParameters_c;
    if (trim_values != NULL)
    {
        XCVR_RX_DIG->DCOC_BBA_STEP = trim_values->dcoc_bba_step_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_0 = trim_values->dcoc_tza_step_0_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_1 = trim_values->dcoc_tza_step_1_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_2 = trim_values->dcoc_tza_step_2_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_3 = trim_values->dcoc_tza_step_3_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_4 = trim_values->dcoc_tza_step_4_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_5 = trim_values->dcoc_tza_step_5_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_6 = trim_values->dcoc_tza_step_6_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_7 = trim_values->dcoc_tza_step_7_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_8 = trim_values->dcoc_tza_step_8_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_9 = trim_values->dcoc_tza_step_9_reg;
        XCVR_RX_DIG->DCOC_TZA_STEP_10 = trim_values->dcoc_tza_step_10_reg;
        XCVR_RX_DIG->DCOC_DAC_INIT = trim_values->dcoc_dac_init_reg;
        status = gXcvrSuccess_c;
    }
    return status;
}

xcvrStatus_t XCVR_GetDcocDacTrims(xcvr_DcocDacTrim_t * trim_values)
{
    xcvrStatus_t status = gXcvrInvalidParameters_c;
    if (trim_values != NULL)
    {
        trim_values->dcoc_bba_step_reg = XCVR_RX_DIG->DCOC_BBA_STEP;
        trim_values->dcoc_tza_step_0_reg = XCVR_RX_DIG->DCOC_TZA_STEP_0;
        trim_values->dcoc_tza_step_1_reg = XCVR_RX_DIG->DCOC_TZA_STEP_1;
        trim_values->dcoc_tza_step_2_reg = XCVR_RX_DIG->DCOC_TZA_STEP_2;
        trim_values->dcoc_tza_step_3_reg = XCVR_RX_DIG->DCOC_TZA_STEP_3;
        trim_values->dcoc_tza_step_4_reg = XCVR_RX_DIG->DCOC_TZA_STEP_4;
        trim_values->dcoc_tza_step_5_reg = XCVR_RX_DIG->DCOC_TZA_STEP_5;
        trim_values->dcoc_tza_step_6_reg = XCVR_RX_DIG->DCOC_TZA_STEP_6;
        trim_values->dcoc_tza_step_7_reg = XCVR_RX_DIG->DCOC_TZA_STEP_7;
        trim_values->dcoc_tza_step_8_reg = XCVR_RX_DIG->DCOC_TZA_STEP_8;
        trim_values->dcoc_tza_step_9_reg = XCVR_RX_DIG->DCOC_TZA_STEP_9;
        trim_values->dcoc_tza_step_10_reg= XCVR_RX_DIG->DCOC_TZA_STEP_10;
        trim_values->dcoc_dac_init_reg = XCVR_RX_DIG->DCOC_DAC_INIT;
        status = gXcvrSuccess_c;
    }
    return status;
}

/* RSSI adjustment */
xcvrStatus_t XCVR_SetRssiAdjustment(int8_t adj)
{
    XCVR_RX_DIG->RSSI_CTRL_0 &= ~XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_MASK;
    XCVR_RX_DIG->RSSI_CTRL_0 |= XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ(adj);
    return gXcvrSuccess_c;
}

int8_t  XCVR_GetRssiAdjustment(void)
{
    int8_t adj;
    adj = (XCVR_RX_DIG->RSSI_CTRL_0 & XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_MASK) >> XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_SHIFT;
    return adj;
}

/* Radio debug functions */
xcvrStatus_t XCVR_OverrideChannel ( uint8_t channel, uint8_t useMappedChannel )
{
    uint32_t temp;
    xcvrStatus_t status = gXcvrSuccess_c;

    
    if ((channel >= 128) && (channel < 0xFF))
    {
      status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* valid channel parameter range */
        /* this is the disable case that resets an active channel override */
        if(channel == 0xFF)
        {
            /* Clear all of the overrides and restore to LL channel control */
          temp = XCVR_PLL_DIG->CHAN_MAP;
          temp &= ~(XCVR_PLL_DIG_CHAN_MAP_CHANNEL_NUM_MASK | XCVR_PLL_DIG_CHAN_MAP_BOC_MASK 
#if !RADIO_IS_GEN_2P1
            | XCVR_PLL_DIG_CHAN_MAP_ZOC_MASK
#endif /* !RADIO_IS_GEN_2P1 */
#if RADIO_IS_GEN_3P0
            | XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_EN_MASK
#endif /* RADIO_IS_GEN_3P0 */
            );
          XCVR_PLL_DIG->CHAN_MAP = temp; 
        
          /* Stop using the manual frequency setting */
          XCVR_PLL_DIG->LPM_SDM_CTRL1 &= ~XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE_MASK;
        
          status = gXcvrSuccess_c;
        }
        else
        {
            if(useMappedChannel)
            {
                temp = (XCVR_MISC->XCVR_CTRL & XCVR_CTRL_XCVR_CTRL_PROTOCOL_MASK)>>XCVR_CTRL_XCVR_CTRL_PROTOCOL_SHIFT; /* Extract PROTOCOL bitfield */
                switch (temp)
                {
#if !RADIO_IS_GEN_2P1
#if XCVR_ANT_ENABLED
                    case 0x3: /* ANT protocol */
                        ANT->CHANNEL_NUM = channel;
                        break;
#endif /* XCVR_ANT_ENABLED */
#endif /* !RADIO_IS_GEN_2P1 */
                    case 0x8: /* GENFSK protocol */
                    case 0x9: /* MSK protocol */
                        GENFSK->CHANNEL_NUM = channel;
                        break;
                    default: /* All other protocols */
                        temp = XCVR_PLL_DIG->CHAN_MAP;
                        temp &= ~(XCVR_PLL_DIG_CHAN_MAP_CHANNEL_NUM_MASK
#if RADIO_IS_GEN_3P0
                                    | XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_EN_MASK
#endif /* RADIO_IS_GEN_3P0 */
                            );
                        temp |= (XCVR_PLL_DIG_CHAN_MAP_CHANNEL_NUM(channel) | XCVR_PLL_DIG_CHAN_MAP_BOC_MASK
#if !RADIO_IS_GEN_2P1
                            | XCVR_PLL_DIG_CHAN_MAP_ZOC_MASK
#endif /* !RADIO_IS_GEN_2P1 */
                            );
                        XCVR_PLL_DIG->CHAN_MAP = temp; 
                        break;
                }
                
            }
            else
            {
                XCVR_PLL_DIG->CHAN_MAP |= (XCVR_PLL_DIG_CHAN_MAP_BOC_MASK
#if !RADIO_IS_GEN_2P1
                    | XCVR_PLL_DIG_CHAN_MAP_ZOC_MASK
#endif /* !RADIO_IS_GEN_2P1 */
                    ); 
                
                XCVR_PLL_DIG->LPM_SDM_CTRL3 = XCVR_PLL_DIG_LPM_SDM_CTRL3_LPM_DENOM(gPllDenom_c);
                XCVR_PLL_DIG->LPM_SDM_CTRL2 = XCVR_PLL_DIG_LPM_SDM_CTRL2_LPM_NUM(mapTable[channel].numerator);
                
                temp = XCVR_PLL_DIG->LPM_SDM_CTRL1;
                temp &= ~XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_MASK;
                temp |= XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG(mapTable[channel].integer);
                XCVR_PLL_DIG->LPM_SDM_CTRL1 = temp;

                /* Stop using the LL channel map and use the manual frequency setting */
                XCVR_PLL_DIG->LPM_SDM_CTRL1 |= XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE_MASK;
            }
        }
    }
      
    return status;
}

uint32_t XCVR_GetFreq ( void )
{
    uint32_t pll_int;
    uint32_t pll_num_unsigned;
    int32_t pll_num;
    uint32_t pll_denom;
    float freq_float;

    if (XCVR_PLL_DIG->LPM_SDM_CTRL1 & XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE_MASK) /* Not using mapped channels */
    {
        pll_int = (XCVR_PLL_DIG->LPM_SDM_CTRL1 & XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_MASK) >>
                   XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_SHIFT;

        pll_num_unsigned = XCVR_PLL_DIG->LPM_SDM_CTRL2;
        pll_denom = XCVR_PLL_DIG->LPM_SDM_CTRL3;
    }
    else
    {
        /* Using mapped channels so need to read from the _SELECTED fields to get the values being used */
        pll_int = (XCVR_PLL_DIG->LPM_SDM_CTRL1 & XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_SELECTED_MASK) >>
                   XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG_SELECTED_SHIFT;

        pll_num_unsigned = XCVR_PLL_DIG->LPM_SDM_RES1;
        pll_denom = XCVR_PLL_DIG->LPM_SDM_RES2;
    }

    uint32_t freq = 0;

#if RF_OSC_26MHZ == 1
    uint32_t ref_clk = 26UL;
#else
    uint32_t ref_clk = 32UL;
#endif /* RF_OSC_26MHZ == 1 */

    /* Check if sign bit is asserted */
    if (pll_num_unsigned & 0x04000000UL)
    {
        /* Sign extend the numerator */
        pll_num = (~pll_num_unsigned + 1) & 0x03FFFFFFUL; 
        
        /* Calculate the frequency in MHz */
        freq_float = (ref_clk * 2 * (pll_int - ((float)pll_num / pll_denom))); 
    }
    else
    {
        /* Calculate the frequency in MHz */
        pll_num = pll_num_unsigned;
        freq_float = (ref_clk * 2 * (pll_int + ((float)pll_num / (float)pll_denom))); 
    }

    freq = (uint32_t)freq_float;

    return freq;
}

void XCVR_ForceRxWu ( void )
{
    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_RX_EN_MASK;
}

void XCVR_ForceRxWd ( void )
{
    XCVR_TSM->CTRL &= ~XCVR_TSM_CTRL_FORCE_RX_EN_MASK;
}

void XCVR_ForceTxWu ( void )
{
    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
}

void XCVR_ForceTxWd ( void )
{
    XCVR_TSM->CTRL &= ~XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
}

xcvrStatus_t XCVR_DftTxCW(uint16_t rf_channel_freq, uint8_t protocol)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    uint32_t temp;
    /* CW TX only operates in FSK and GFSK protocols */
    if ((protocol != 6) && (protocol != 7))
    {
        status = gXcvrInvalidParameters_c; /* Failure */
    }

    /* only operate in the supported frequencies */
    if ((rf_channel_freq < 2360) || (rf_channel_freq >2487))
    {
        status =  gXcvrInvalidParameters_c; /* Failure */
    }

    /* no parameter errors so launch CW TX */
    if (status == gXcvrSuccess_c)
    {
        /* Set the DFT Mode */
        temp  =  XCVR_TX_DIG->CTRL;
        temp &= ~XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK;
        temp |=  XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(1);
        XCVR_TX_DIG->CTRL = temp;

        /* Choose Protocol 6 or 7 if using the Channel Number register */
        temp  =  XCVR_MISC->XCVR_CTRL;
        temp &= ~XCVR_CTRL_XCVR_CTRL_PROTOCOL_MASK;
        temp |=  XCVR_CTRL_XCVR_CTRL_PROTOCOL(protocol);
        XCVR_MISC->XCVR_CTRL = temp;

        /* Select the RF Channel, using the Channel Number register */
        XCVR_OverrideChannel(rf_channel_freq-2360,1);

        /* Warm-up the Radio */
        XCVR_ForceTxWu();
    }

    return status; /* Success */
}

xcvrStatus_t XCVR_DftTxPatternReg(uint16_t channel_num, radio_mode_t radio_mode, data_rate_t data_rate, uint32_t tx_pattern)
{
    uint32_t temp;
    uint8_t dft_mode = 0;
    uint8_t dft_clk_sel = 0;
    xcvrStatus_t status = gXcvrSuccess_c;
    
    XCVR_ChangeMode(radio_mode, data_rate);
    
    /* Select the RF Channel, using the Channel Number register */
    XCVR_OverrideChannel(channel_num, 1);

    switch (radio_mode)
    {
        case ZIGBEE_MODE:
            dft_mode = 6;    /* OQPSK configuration */
            break;
        case ANT_MODE:
        case BLE_MODE:
        case GFSK_BT_0p5_h_0p5:
        case GFSK_BT_0p5_h_0p32:
        case GFSK_BT_0p5_h_0p7:
        case GFSK_BT_0p5_h_1p0:
        case GFSK_BT_0p3_h_0p5:
        case GFSK_BT_0p7_h_0p5:
            dft_mode = 2; /* GFSK configuration */
            break;
        case MSK:
            dft_mode = 4;     /* MSK configuration */
            break;
        default:
            status = gXcvrInvalidParameters_c;
            break;
    }

    if (status == gXcvrSuccess_c) /* Only attempt this pointer assignment process if prior switch() statement completed successfully */
    {
        switch (data_rate)
        {
            case DR_1MBPS:
                dft_clk_sel = 4UL;
                break;
            case DR_500KBPS:
                dft_clk_sel = 3UL;
                break;
            case DR_250KBPS:
                dft_clk_sel = 2UL;
                break;
            default:
                status = gXcvrInvalidParameters_c;
                break;
        }
    }

    temp = XCVR_TX_DIG->CTRL;
    temp &= ~(XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK | XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK | XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK | XCVR_TX_DIG_CTRL_LFSR_EN_MASK);
    temp |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(dft_mode) |
            XCVR_TX_DIG_CTRL_DFT_CLK_SEL(dft_clk_sel) |
            XCVR_TX_DIG_CTRL_TX_DFT_EN(1UL) |
            XCVR_TX_DIG_CTRL_LFSR_EN(0UL);
    XCVR_TX_DIG->CTRL = temp;

    XCVR_TX_DIG->DFT_PATTERN = tx_pattern;

    if (status == gXcvrSuccess_c)
    {
        /* Warm-up the Radio */
        XCVR_ForceTxWu();
    }

    return status;
}

xcvrStatus_t XCVR_DftTxLfsrReg(uint16_t channel_num, radio_mode_t radio_mode, data_rate_t data_rate, uint8_t lfsr_length)
{
    uint32_t temp;
    uint8_t dft_mode = 0;
    uint8_t dft_clk_sel = 0;
    xcvrStatus_t status = gXcvrSuccess_c;

    if (lfsr_length > 5)
    {
        status = gXcvrInvalidParameters_c;
    }

    if (status == gXcvrSuccess_c)
    {
        XCVR_ChangeMode(radio_mode, data_rate);
        
        /* Select the RF Channel, using the Channel Number register */
        XCVR_OverrideChannel(channel_num,1);

        switch (radio_mode)
        {
            case ZIGBEE_MODE:
                dft_mode = 7;    /* OQPSK configuration */
                break;
            case ANT_MODE:
            case BLE_MODE:
            case GFSK_BT_0p5_h_0p5:
            case GFSK_BT_0p5_h_0p32:
            case GFSK_BT_0p5_h_0p7:
            case GFSK_BT_0p5_h_1p0:
            case GFSK_BT_0p3_h_0p5:
            case GFSK_BT_0p7_h_0p5:
                dft_mode = 3; /* GFSK configuration */
                break;
            case MSK:
                dft_mode = 5;     /* MSK configuration */
                break;

            default:    
                status = gXcvrInvalidParameters_c;
                break;
        }

        if (status == gXcvrSuccess_c)
        {
            switch (data_rate)
            {
                case DR_1MBPS:
                    dft_clk_sel = 4;
                    break;
                case DR_500KBPS:
                    dft_clk_sel = 3;
                    break;
                case DR_250KBPS:
                    dft_clk_sel = 2;
                    break;
                default:
                    status = gXcvrInvalidParameters_c;
                    break;
            }
        }

        if (status == gXcvrSuccess_c)
        {
            temp = XCVR_TX_DIG->CTRL;
            temp &= ~(XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK |
                XCVR_TX_DIG_CTRL_LFSR_LENGTH_MASK |
                XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK | 
                XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK | 
                XCVR_TX_DIG_CTRL_LFSR_EN_MASK);
            temp |=  XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(dft_mode) | 
                    XCVR_TX_DIG_CTRL_LFSR_LENGTH(lfsr_length) |
                    XCVR_TX_DIG_CTRL_DFT_CLK_SEL(dft_clk_sel) |
                    XCVR_TX_DIG_CTRL_TX_DFT_EN(0) |
                    XCVR_TX_DIG_CTRL_LFSR_EN(1);
            XCVR_TX_DIG->CTRL = temp;

            /* Warm-up the Radio */
            XCVR_ForceTxWu();
        }
    }

    return status;
}

void XCVR_DftTxOff(void)
{
    XCVR_ForceTxWd();
    XCVR_MISC->XCVR_CTRL |= XCVR_CTRL_XCVR_CTRL_TGT_PWR_SRC_MASK; /* Use PA_POWER in LL registers */
    /* Clear the RF Channel over-ride */
    XCVR_OverrideChannel(0xFF,1);
    XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK | /* Clear DFT_MODE */
                           XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK | /* Clear DFT_CLK_SEL */
                           XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK | /* Clear DFT_EN */
                           XCVR_TX_DIG_CTRL_LFSR_EN_MASK);/* Clear LFSR_EN */
}

xcvrStatus_t XCVR_ForcePAPower(uint8_t pa_power)
{
    xcvrStatus_t status = gXcvrSuccess_c;

    /* PA power range check */
    if (pa_power > 0x3F)
    {
        status = gXcvrInvalidParameters_c; /* Failure */
    }

    if (status == gXcvrSuccess_c)
    {
        if (pa_power != 1)
        {
            pa_power = pa_power & 0xFEU; /* Ensure LSbit is cleared */
        }
        
        XCVR_MISC->XCVR_CTRL &= ~XCVR_CTRL_XCVR_CTRL_TGT_PWR_SRC_MASK; /* Use PA_POWER in TSM registers */
        XCVR_TSM->PA_POWER = pa_power;
    }
    
    return status; /* Success */
}

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
xcvrStatus_t XCVR_SetManAgc(uint8_t man_agc_idx)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    if (man_agc_idx >26)
    {
        /* error case */
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        SetManAgcAutoDCOC(man_agc_idx);
    }
    return status;
}

void XCVR_ClearManAgc(void)
{
    ClearManAgc();
}
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

xcvrStatus_t XCVR_CoexistenceInit(void)
{
#if gMWS_UseCoexistence_d  
    uint32_t temp = 0x00U;
    uint32_t end_of_tx_wu = 0x00U;
    uint32_t end_of_rx_wu = 0x00U;
    
#if (defined(CPU_MKW20Z160VHT4) || defined(CPU_MKW30Z160VHM4) || defined(CPU_MKW40Z160VHT4) || \
     defined(CPU_MKW21Z256VHT4) || defined(CPU_MKW21Z512VHT4) || defined(CPU_MKW31Z256CAx4) || \
     defined(CPU_MKW31Z256VHT4) || defined(CPU_MKW31Z512CAx4) || defined(CPU_MKW31Z512VHT4) || \
     defined(CPU_MKW41Z256VHT4) || defined(CPU_MKW41Z512VHT4))

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
#if (XCVR_COEX_RF_ACTIVE_PIN == ANT_A)
    uint32_t tsm_timing47 = 0x00U;
#else /* (XCVR_COEX_RF_ACTIVE_PIN == ANT_B) */
    uint32_t tsm_timing48 = 0x00U;
#endif /* (XCVR_COEX_RF_ACTIVE_PIN == ANT_A) */
    uint32_t tsm_timing50 = 0x00U;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */

#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
    // RF_ACTIVE = ANT_B (PTC1, gpio1_trig_en)
    uint32_t tsm_timing48 = 0x00U;
    // RF_PRIORITY = ANT_A (PTC4, gpio0_trig_en)
    uint32_t tsm_timing47 = 0x00U;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */

    uint16_t tsm_timing43_rx = 0x00;
    uint16_t tsm_timing43_tx = 0x00;

    /* Select GPIO mode for FAD pins */
    temp = XCVR_MISC->FAD_CTRL;
    temp &= ~(XCVR_CTRL_FAD_CTRL_FAD_NOT_GPIO_MASK);
    XCVR_MISC->FAD_CTRL = temp;

    /* Read the END_OF_TX_WU and END_OF_RX_WU for XCVR */
    end_of_tx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >>
                    XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;
    end_of_rx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >>
                    XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;

/*****************
 *  TX SEQUENCE  *
 *****************/

    if (end_of_tx_wu < gMWS_CoexRfActiveAssertTime_d)
    {
        temp = end_of_tx_wu;
    }
    else
    {
        temp = gMWS_CoexRfActiveAssertTime_d;
    }
    
    /* Save the TX RF_ACTIVE start time. */
    tsm_timing43_tx = end_of_tx_wu - temp; 

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
    /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence. */
#if (XCVR_COEX_RF_ACTIVE_PIN == ANT_A)
    tsm_timing47 = (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_SHIFT) & 
                                                        XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_MASK);
#else
    tsm_timing48 = (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING48_GPIO1_TRIG_EN_TX_HI_SHIFT) & 
                                                        XCVR_TSM_TIMING48_GPIO1_TRIG_EN_TX_HI_MASK);    
#endif /* (XCVR_COEX_RF_ACTIVE_PIN == ANT_A) */

    /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence. */
    tsm_timing50 = (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) & 
                                                        XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */

#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
    /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence. */
    tsm_timing48 = (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING48_GPIO1_TRIG_EN_TX_HI_SHIFT) & 
                                                        XCVR_TSM_TIMING48_GPIO1_TRIG_EN_TX_HI_MASK);

    /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence. */
    tsm_timing47 = (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_SHIFT) & 
                                                        XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */  

/*****************
 *  RX SEQUENCE  *
 *****************/

    if (end_of_rx_wu < gMWS_CoexRfActiveAssertTime_d)
    {
        temp = end_of_rx_wu;
    }
    else
    {
        temp = gMWS_CoexRfActiveAssertTime_d;
    }

    /* Save the RX RF_ACTIVE start time. */
    tsm_timing43_rx = end_of_rx_wu - temp; 

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
    /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence. */
#if (XCVR_COEX_RF_ACTIVE_PIN == ANT_A)
    tsm_timing47 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_SHIFT) & 
                                                         XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_MASK);
#else
    tsm_timing48 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING48_GPIO1_TRIG_EN_RX_HI_SHIFT) & 
                                                         XCVR_TSM_TIMING48_GPIO1_TRIG_EN_RX_HI_MASK);
#endif /* (XCVR_COEX_RF_ACTIVE_PIN == ANT_A) */

    /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start. */
    tsm_timing50 |= ((((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) & 
                                                          XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                     (((uint32_t)(end_of_rx_wu - gMWS_CoexPrioSignalTime_d) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) & 
                                                                             XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));

#if (XCVR_COEX_RF_ACTIVE_PIN == ANT_A)
    temp = XCVR_TSM->TIMING47;
    temp &= ~(XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_MASK | XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_MASK);
    temp |= tsm_timing47;
    XCVR_TSM->TIMING47 = temp;
#else
    temp = XCVR_TSM->TIMING48;
    temp &= ~(XCVR_TSM_TIMING48_GPIO1_TRIG_EN_TX_HI_MASK | XCVR_TSM_TIMING48_GPIO1_TRIG_EN_RX_HI_MASK);
    temp |= tsm_timing48;
    XCVR_TSM->TIMING48 = temp;    
#endif /* (XCVR_COEX_RF_ACTIVE_PIN == ANT_A) */

    temp = XCVR_TSM->TIMING50;
    temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK | 
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK);
    temp |= tsm_timing50;
    XCVR_TSM->TIMING50 = temp;

#if (XCVR_COEX_RF_ACTIVE_PIN == ANT_A)
    GPIOC->PDDR |= 0x18;
    PORTC->PCR[4] = (PORTC->PCR[4] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);
    PORTC->PCR[3] = (PORTC->PCR[3] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);
#else
    GPIOC->PDDR |= 0x0A;
    PORTC->PCR[1] = (PORTC->PCR[1] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);
    PORTC->PCR[3] = (PORTC->PCR[3] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);
#endif /* (XCVR_COEX_RF_ACTIVE_PIN == ANT_A) */
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */

#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
    /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence. */
    tsm_timing48 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING48_GPIO1_TRIG_EN_RX_HI_SHIFT) &
                                                         XCVR_TSM_TIMING48_GPIO1_TRIG_EN_RX_HI_MASK);

    /* Set PRIORITY pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start. */
    tsm_timing47 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_SHIFT) &
                                                          XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_MASK);

    /* RF_ACTIVE */
    temp = XCVR_TSM->TIMING48;
    temp &= ~(XCVR_TSM_TIMING48_GPIO1_TRIG_EN_TX_HI_MASK | XCVR_TSM_TIMING48_GPIO1_TRIG_EN_RX_HI_MASK);
    temp |= tsm_timing48;
    XCVR_TSM->TIMING48 = temp;

    /* RF_PRIORITY */
    temp = XCVR_TSM->TIMING47;
    temp &= ~(XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_MASK | XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_MASK);
    temp |= tsm_timing47;
    XCVR_TSM->TIMING47 = temp;

    /* Overwrite pins settings */
    GPIOC->PDDR |= 0x12;
    PORTC->PCR[4] = (PORTC->PCR[4] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);
    PORTC->PCR[1] = (PORTC->PCR[1] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */

    tsm_timing43_tx += gMWS_CoexConfirmWaitTime_d;

    if (tsm_timing43_tx > end_of_tx_wu - 1)
    {
        tsm_timing43_tx = end_of_tx_wu - 1;
    }

    tsm_timing43_rx += gMWS_CoexConfirmWaitTime_d;
    
    if (tsm_timing43_rx > end_of_rx_wu - 1)
    {
        tsm_timing43_rx = end_of_rx_wu - 1;
    }

    XCVR_TSM->TIMING43 = ((((uint32_t)(tsm_timing43_tx) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_HI_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_HI_MASK) |
                          (((uint32_t)(tsm_timing43_tx + 2) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_LO_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_LO_MASK) | 
                          (((uint32_t)(tsm_timing43_rx) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_HI_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_HI_MASK) |
                          (((uint32_t)(tsm_timing43_rx + 2) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_LO_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_LO_MASK));

    BTLE_RF->MISC_CTRL = 0x02;

    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_TSM_IRQ0_EN_MASK;

    /* Save the updated registers values. */
    XCVR_CoexistenceSaveRestoreTimings(1);    

/*********************/
/*   KW35x / KW36x   */
/*********************/
#elif (defined(CPU_MKW35A512VFP4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW36A512VFP4) || \
    defined(CPU_MKW36A512VHT4) || defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4))

    // RF_ACTIVE = TX_SWITCH (PTC2, gpio2_trig_en)
    uint32_t tsm_timing49 = 0x00U;

    // RF_PRIORITY = RX_SWITCH (PTC3, gpio3_trig_en)
    uint32_t tsm_timing50 = 0x00U;

    // TSM_TIMING43 is used to control the tsm_irq0_trig signal
    uint16_t tsm_timing43_rx = 0x00;
    uint16_t tsm_timing43_tx = 0x00;

    /* Select GPIO mode for FAD pins */
    temp = XCVR_MISC->FAD_CTRL;
    temp &= ~(XCVR_CTRL_FAD_CTRL_FAD_NOT_GPIO_MASK);
    XCVR_MISC->FAD_CTRL = temp;

    /* Read the END_OF_TX_WU and END_OF_RX_WU for XCVR */
    end_of_tx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >>
                    XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;
    end_of_rx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >>
                    XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;

/*****************
 *  TX SEQUENCE  *
 *****************/

    if (end_of_tx_wu < gMWS_CoexRfActiveAssertTime_d)
    {
        temp = end_of_tx_wu;
    }
    else
    {
        temp = gMWS_CoexRfActiveAssertTime_d;
    }

    /* Save the TX RF_ACTIVE start time. */
    tsm_timing43_tx = end_of_tx_wu - temp;

    /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence. */
    tsm_timing49 = (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_SHIFT) &
                                                        XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_MASK);

    /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence. */
    tsm_timing50 = (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) &
                                                        XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
/*****************
 *  RX SEQUENCE  *
 *****************/

    if (end_of_rx_wu < gMWS_CoexRfActiveAssertTime_d)
    {
        temp = end_of_rx_wu;
    }
    else
    {
        temp = gMWS_CoexRfActiveAssertTime_d;
    }

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
    /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence. */
    tsm_timing49 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_SHIFT) &
                                                         XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_MASK);

    /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start. */
    tsm_timing50 |= ((((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                                          XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                     (((uint32_t)(end_of_rx_wu - gMWS_CoexPrioSignalTime_d) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) &
                                                                             XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));

    temp = XCVR_TSM->TIMING49;
    temp &= ~(XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_MASK |
              XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_MASK);
    temp |= tsm_timing49;
    XCVR_TSM->TIMING49 = temp;

    temp = XCVR_TSM->TIMING50;
    temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK);
    temp |= tsm_timing50;
    XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */

#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
    /* Set RF_ACTIVE pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence. */
    tsm_timing49 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_SHIFT) &
                                                         XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_MASK);

    /* Set PRIORITY pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start. */
    tsm_timing50 |= (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                                          XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK);

    /* RF_ACTIVE */
    temp = XCVR_TSM->TIMING49;
    temp &= ~(XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI_MASK | XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI_MASK);
    temp |= tsm_timing49;
    XCVR_TSM->TIMING49 = temp;

    /* RF_PRIORITY */
    temp = XCVR_TSM->TIMING50;
    temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK | XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK);
    temp |= tsm_timing50;
    XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */

    /* Overwrite pins settings */
    GPIOC->PDDR |= 0x0C;
    PORTC->PCR[3] = (PORTC->PCR[3] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);
    PORTC->PCR[2] = (PORTC->PCR[2] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);

    /* It should assert 2 ticks prior to END_OF_RX_WU and de-assert 1 tick later at 1 tick prior to END_OF_TX_WU */
    tsm_timing43_tx = end_of_tx_wu - 2;
    /* It should assert 2 ticks prior to END_OF_RX_WU and de-assert 1 tick later at 1 tick prior to END_OF_RX_WU */
    tsm_timing43_rx = end_of_rx_wu - 2;

    XCVR_TSM->TIMING43 = ((((uint32_t)(tsm_timing43_tx) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_HI_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_HI_MASK) |
                          (((uint32_t)(tsm_timing43_tx + 1) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_LO_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_LO_MASK) |
                          (((uint32_t)(tsm_timing43_rx) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_HI_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_HI_MASK) |
                          (((uint32_t)(tsm_timing43_rx + 1) << XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_LO_SHIFT) & XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_LO_MASK));

    BTLE_RF->MISC_CTRL = 0x02;

    /* Enable TSM_IRQ0 interrupt */
    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_TSM_IRQ0_EN_MASK;

    /* Save the updated registers values. */
    XCVR_CoexistenceSaveRestoreTimings(1);

#endif /* CPU_MKW35Axxx / CPU_MKW36Axxx */

#endif /* gMWS_UseCoexistence_d */ 

    return gXcvrSuccess_c;
}

xcvrStatus_t XCVR_CoexistenceSetPriority(XCVR_COEX_PRIORITY_T rxPriority, XCVR_COEX_PRIORITY_T txPriority)
{
#if gMWS_UseCoexistence_d
    uint32_t temp = 0x00U;
    uint32_t end_of_tx_wu = 0x00U;
    uint32_t end_of_rx_wu = 0x00U;

#if (defined(CPU_MKW20Z160VHT4) || defined(CPU_MKW30Z160VHM4) || defined(CPU_MKW40Z160VHT4) || \
     defined(CPU_MKW21Z256VHT4) || defined(CPU_MKW21Z512VHT4) || defined(CPU_MKW31Z256CAx4) || \
     defined(CPU_MKW31Z256VHT4) || defined(CPU_MKW31Z512CAx4) || defined(CPU_MKW31Z512VHT4) || \
     defined(CPU_MKW41Z256VHT4) || defined(CPU_MKW41Z512VHT4))

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
    uint32_t tsm_timing50 = 0x00U;
#endif
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
    uint32_t tsm_timing47 = 0x00U;
#endif

    /* Read the END_OF_TX_WU and END_OF_RX_WU for XCVR */
    end_of_tx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >>
                    XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;
    end_of_rx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >>
                    XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;    

/*****************
 *      RX       *
 *****************/

    if (XCVR_COEX_HIGH_PRIO == rxPriority)
    {
        if (end_of_rx_wu < gMWS_CoexRfActiveAssertTime_d)
        {
            temp = end_of_rx_wu;
        }
        else
        {
            temp = gMWS_CoexRfActiveAssertTime_d;
        }

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
        /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start for high priority RX. */
        tsm_timing50 = ((((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) & 
                                                             XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                        (((uint32_t)(end_of_rx_wu - gMWS_CoexPrioSignalTime_d) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) & 
                                                                                  XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
     /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence */
        tsm_timing47 = (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_SHIFT) & 
                                                            XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_MASK);   
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
    }
    else
    {
        /* Low priority RX */
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
        tsm_timing50 = (((0xFFU << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) & 
                                   XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                        ((0xFFU << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) & 
                                   XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
        tsm_timing47 = (((0xFFU << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_SHIFT) & 
                                   XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_MASK) |
                        ((0xFFU << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_LO_SHIFT) & 
                                   XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_LO_MASK));
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
    }

/*****************
 *      TX       *
 *****************/
    if (XCVR_COEX_HIGH_PRIO == txPriority)
    {
        if (end_of_tx_wu < gMWS_CoexRfActiveAssertTime_d)
        {
            temp = end_of_tx_wu;
        }
        else
        {
            temp = gMWS_CoexRfActiveAssertTime_d;
        }

        /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence for HIGH priority TX. */
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
        tsm_timing50 |= (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) & 
                                                             XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)        
        tsm_timing47 |= (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_SHIFT) & 
                                                             XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
    }
    else
    {
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
        /* Set STATUS pin HIGH at END_OF_TX_WU prior to any TX sequence for LOW priority TX. */
        tsm_timing50 |= (((uint32_t)(end_of_tx_wu) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) & 
                                                      XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
        /* Set STATUS pin LOW at END_OF_TX_WU prior to any TX sequence for LOW priority TX. */
        tsm_timing47 = (((0xFFU << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_SHIFT) & 
                                   XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_MASK) |
                        ((0xFFU << XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_LO_SHIFT) & 
                                   XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_LO_MASK));
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
    }

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
    temp = XCVR_TSM->TIMING50;
    temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK);
    temp |= tsm_timing50;
    XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
    temp = XCVR_TSM->TIMING47;
    temp &= ~(XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI_MASK |
              XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_LO_MASK |
              XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI_MASK |
              XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_LO_MASK);
    temp |= tsm_timing47;
    XCVR_TSM->TIMING47 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */

/*********************/
/*   KW35x / KW36x   */
/*********************/
#elif (defined(CPU_MKW35A512VFP4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW36A512VFP4) || \
       defined(CPU_MKW36A512VHT4) || defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4))

    uint32_t tsm_timing50 = 0x00U;

    /* Read the END_OF_TX_WU and END_OF_RX_WU for XCVR */
    end_of_tx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >>
                    XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;
    end_of_rx_wu = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK) >>
                    XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT;

/*****************
 *      RX       *
 *****************/

    if (XCVR_COEX_HIGH_PRIO == rxPriority)
    {
        if (end_of_rx_wu < gMWS_CoexRfActiveAssertTime_d)
        {
            temp = end_of_rx_wu;
        }
        else
        {
            temp = gMWS_CoexRfActiveAssertTime_d;
        }

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
        /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence and clear it gMWS_CoexPrioSignalTime_d uS before RX start for high priority RX. */
        tsm_timing50 = ((((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                                             XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                        (((uint32_t)(end_of_rx_wu - gMWS_CoexPrioSignalTime_d) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) &
                                                                                  XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */

#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
     /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any RX sequence */
        tsm_timing50 = (((uint32_t)(end_of_rx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                                            XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
    }
    else
    {
        /* Low priority RX */
        tsm_timing50 = (((0xFFU << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_SHIFT) &
                                   XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK) |
                        ((0xFFU << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_SHIFT) &
                                   XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK));
    }

/*****************
 *      TX       *
 *****************/
    if (XCVR_COEX_HIGH_PRIO == txPriority)
    {
        if (end_of_tx_wu < gMWS_CoexRfActiveAssertTime_d)
        {
            temp = end_of_tx_wu;
        }
        else
        {
            temp = gMWS_CoexRfActiveAssertTime_d;
        }

        /* Set STATUS pin HIGH gMWS_CoexRfActiveAssertTime_d uS prior to any TX sequence for HIGH priority TX. */
        tsm_timing50 |= (((uint32_t)(end_of_tx_wu - temp) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) &
                                                             XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
    }
    else
    {
#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
        /* Set STATUS pin HIGH at END_OF_TX_WU prior to any TX sequence for LOW priority TX. */
        tsm_timing50 |= (((uint32_t)(end_of_tx_wu) << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) &
                                                      XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK);
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
        /* Set STATUS pin LOW at END_OF_TX_WU prior to any TX sequence for LOW priority TX. */
        tsm_timing50 = (((0xFFU << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_SHIFT) &
                                   XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK) |
                        ((0xFFU << XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_LO_SHIFT) &
                                   XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_LO_MASK));
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */
    }

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
    temp = XCVR_TSM->TIMING50;
    temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK);
    temp |= tsm_timing50;
    XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */
#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
    temp = XCVR_TSM->TIMING50;
    temp &= ~(XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_LO_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI_MASK |
              XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO_MASK);
    temp |= tsm_timing50;
    XCVR_TSM->TIMING50 = temp;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */

#endif /* MCU_KW35Axxx / MCU_KW36Axxx */

    /* Save the updated registers values. */
    XCVR_CoexistenceSaveRestoreTimings(1);
#endif /* gMWS_UseCoexistence_d */

    return gXcvrSuccess_c;
}

xcvrStatus_t XCVR_CoexistenceSaveRestoreTimings(uint8_t saveTimings)
{
#if gMWS_UseCoexistence_d
    static uint32_t tsm_ovrd0_saved = 0x00;
    static uint32_t tsm_ovrd1_saved = 0x00;
    static uint32_t tsm_ovrd2_saved = 0x00;
    static uint32_t tsm_ovrd3_saved = 0x00;

#if (defined(CPU_MKW20Z160VHT4) || defined(CPU_MKW30Z160VHM4) || defined(CPU_MKW40Z160VHT4) || \
     defined(CPU_MKW21Z256VHT4) || defined(CPU_MKW21Z512VHT4) || defined(CPU_MKW31Z256CAx4) || \
     defined(CPU_MKW31Z256VHT4) || defined(CPU_MKW31Z512CAx4) || defined(CPU_MKW31Z512VHT4) || \
     defined(CPU_MKW41Z256VHT4) || defined(CPU_MKW41Z512VHT4))

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
#if (XCVR_COEX_RF_ACTIVE_PIN == ANT_A)
    static uint32_t tsm_timing47_saved = 0x00;
#else /* (XCVR_COEX_RF_ACTIVE_PIN == ANT_B) */
    static uint32_t tsm_timing48_saved = 0x00;
#endif /* (XCVR_COEX_RF_ACTIVE_PIN == ANT_A) */
    static uint32_t tsm_timing50_saved = 0x00;
    static uint32_t tsm_timing49_saved = 0x00;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d) */

#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
    // RF_ACTIVE = ANT_B (PTC1, gpio1_trig_en)
    static uint32_t tsm_timing48_saved = 0x00;
    // RF_PRIORITY = ANT_A (PTC4, gpio0_trig_en)
    static uint32_t tsm_timing47_saved = 0x00;
#endif /* (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d) */

    if (saveTimings == 0)
    {
        /* Restore registers values. */
        XCVR_TSM->OVRD0 = tsm_ovrd0_saved;
        XCVR_TSM->OVRD1 = tsm_ovrd1_saved;
        XCVR_TSM->OVRD2 = tsm_ovrd2_saved;
        XCVR_TSM->OVRD3 = tsm_ovrd3_saved;

#if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
#if (XCVR_COEX_RF_ACTIVE_PIN == ANT_A)
        XCVR_TSM->TIMING47 = tsm_timing47_saved;
#else
        XCVR_TSM->TIMING48 = tsm_timing48_saved;
#endif
        XCVR_TSM->TIMING49 = tsm_timing49_saved;
        XCVR_TSM->TIMING50 = tsm_timing50_saved;
#endif /* gMWS_Coex_Status_Prio_d */

#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
        XCVR_TSM->TIMING47 = tsm_timing47_saved;
        XCVR_TSM->TIMING48 = tsm_timing48_saved;
#endif /* gMWS_Coex_Prio_Only_d */
    }
    else
    {
        /* Save registers values. */
        tsm_ovrd0_saved = XCVR_TSM->OVRD0;
        tsm_ovrd1_saved = XCVR_TSM->OVRD1;
        tsm_ovrd2_saved = XCVR_TSM->OVRD2;
        tsm_ovrd3_saved = XCVR_TSM->OVRD3;

        #if (gMWS_Coex_Model_d == gMWS_Coex_Status_Prio_d)
#if (XCVR_COEX_RF_ACTIVE_PIN == ANT_A)
        tsm_timing47_saved = XCVR_TSM->TIMING47;
#else
        tsm_timing48_saved = XCVR_TSM->TIMING48;
#endif
        tsm_timing49_saved = XCVR_TSM->TIMING49;
        tsm_timing50_saved = XCVR_TSM->TIMING50;
#endif /* gMWS_Coex_Status_Prio_d */

#if (gMWS_Coex_Model_d == gMWS_Coex_Prio_Only_d)
        tsm_timing47_saved = XCVR_TSM->TIMING47;
        tsm_timing48_saved = XCVR_TSM->TIMING48;
#endif /* gMWS_Coex_Prio_Only_d */
    }

#elif (defined(CPU_MKW35A512VFP4) || defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW36A512VFP4) || \
       defined(CPU_MKW36A512VHT4) || defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4))

    static uint32_t tsm_timing49_saved = 0x00;
    static uint32_t tsm_timing50_saved = 0x00;

    if (saveTimings == 0)
    {
        /* Restore registers values. */
        XCVR_TSM->OVRD0 = tsm_ovrd0_saved;
        XCVR_TSM->OVRD1 = tsm_ovrd1_saved;
        XCVR_TSM->OVRD2 = tsm_ovrd2_saved;
        XCVR_TSM->OVRD3 = tsm_ovrd3_saved;

        XCVR_TSM->TIMING47 = tsm_timing47_saved;
        XCVR_TSM->TIMING48 = tsm_timing48_saved;
        XCVR_TSM->TIMING49 = tsm_timing49_saved;
        XCVR_TSM->TIMING50 = tsm_timing50_saved;
    }
    else
    {
        /* Save registers values. */
        tsm_ovrd0_saved = XCVR_TSM->OVRD0;
        tsm_ovrd1_saved = XCVR_TSM->OVRD1;
        tsm_ovrd2_saved = XCVR_TSM->OVRD2;
        tsm_ovrd3_saved = XCVR_TSM->OVRD3;
        tsm_timing47_saved = XCVR_TSM->TIMING47;
        tsm_timing48_saved = XCVR_TSM->TIMING48;
        tsm_timing49_saved = XCVR_TSM->TIMING49;
        tsm_timing50_saved = XCVR_TSM->TIMING50;
    }

#endif /* CPU_MKW35Axxx / CPU_MKW36Axxx */

#endif /* gMWS_UseCoexistence_d */

    return gXcvrSuccess_c;
}


#if (((RADIO_IS_GEN_3P0) || (RADIO_IS_GEN_2P1)) && defined(PA_FEM_SUPPORTED))
xcvrStatus_t XCVR_ExternalFadPaFemInit(xcvr_pa_fem_config_t * pa_fem_settings_ptr)
{
    xcvrStatus_t status = gXcvrSuccess_c; 
    uint32_t temp_reg = 0;
    uint8_t subfield;
    uint8_t wu_time;
    uint8_t wd_time;

    /* Perform any input verification checks needed and update status appropriately */
    if (pa_fem_settings_ptr == NULL)
    {
        status = gXcvrInvalidParameters_c;
    }
    else
    {
        /* Check contents of the initialization structure */

#if RADIO_IS_GEN_2P1
        /* FAD is not supported at all on KW35 */
        if ((pa_fem_settings_ptr->use_fad_state_machine) ||
            (pa_fem_settings_ptr->tx_switch_pad_control) || 
            (pa_fem_settings_ptr->rx_switch_pad_control))
#else
        /* when FAD state machine is in use, ANT_A and ANT_B must be under FAD state machine control */
        if ((pa_fem_settings_ptr->use_fad_state_machine) && 
            ((pa_fem_settings_ptr->ant_a_pad_control == XCVR_FAD_TSM_GPIO) || 
            (pa_fem_settings_ptr->ant_b_pad_control == XCVR_FAD_TSM_GPIO)))
#endif /* RADIO_IS_GEN_2P1 */
        {
            status = gXcvrInvalidConfiguration_c;
        }

        /* All of the below parameters must be either 0 or 1 and are treated as booleans so OR them together and check for > 1 to validate them */
        if (((uint32_t)pa_fem_settings_ptr->op_mode | 
            (uint32_t)(pa_fem_settings_ptr->high_z_enable) | 
            (uint32_t)pa_fem_settings_ptr->ant_sel_pins_enable | 
            (uint32_t)pa_fem_settings_ptr->tx_rx_switch_pins_enable | 
            (uint32_t)pa_fem_settings_ptr->use_fad_state_machine | 
            (uint32_t)pa_fem_settings_ptr->ant_a_pad_control | 
            (uint32_t)pa_fem_settings_ptr->ant_b_pad_control | 
            (uint32_t)pa_fem_settings_ptr->tx_switch_pad_control | 
            (uint32_t)pa_fem_settings_ptr->rx_switch_pad_control |
            (uint32_t)pa_fem_settings_ptr->tx_switch_pol_control |
            (uint32_t)pa_fem_settings_ptr->rx_switch_pol_control) > 1U)
        {
            status = gXcvrInvalidParameters_c;
        }
    }

    /* If input verification checks have passed then perform the PA/FEM initialization */
    if (status == gXcvrSuccess_c)
    {
        temp_reg |= XCVR_CTRL_FAD_CTRL_ANTX_HZ(pa_fem_settings_ptr->high_z_enable);      /* Set High Z if enabled */
        temp_reg |= XCVR_CTRL_FAD_CTRL_ANTX_CTRLMODE(pa_fem_settings_ptr->op_mode);     /* Set control mode */
        subfield = (((uint32_t)pa_fem_settings_ptr->rx_switch_pol_control<<3) | ((uint32_t)pa_fem_settings_ptr->tx_switch_pol_control<<2)); /* Build field value for ANTX_POL */
        temp_reg |= XCVR_CTRL_FAD_CTRL_ANTX_POL(subfield);
        subfield= (((uint32_t)pa_fem_settings_ptr->rx_switch_pad_control<<3) |    /* Build field value for FAD_NOT_GPIO */
                    ((uint32_t)pa_fem_settings_ptr->tx_switch_pad_control<<2) |
                    ((uint32_t)pa_fem_settings_ptr->ant_b_pad_control<<1) |
                    ((uint32_t)pa_fem_settings_ptr->ant_a_pad_control<<0));
        temp_reg |= XCVR_CTRL_FAD_CTRL_FAD_NOT_GPIO(subfield);
#if RADIO_IS_GEN_3P0
        /* build and program ANTX_EN field for K3. Doesn't exist on KW35 */
        subfield = (((uint32_t)pa_fem_settings_ptr->ant_sel_pins_enable<<1) | ((uint32_t)pa_fem_settings_ptr->tx_rx_switch_pins_enable<<0));
        temp_reg |= XCVR_CTRL_FAD_CTRL_ANTX_EN(subfield);
        temp_reg |= XCVR_CTRL_FAD_CTRL_FAD_EN(pa_fem_settings_ptr->use_fad_state_machine);
#endif /* RADIO_IS_GEN_3P0 */
        XCVR_MISC->FAD_CTRL = temp_reg;

        /* Program TSM Timing registers for TSM_GPIO_2 (tx switch) and TSM_GPIO_3 (rx switch) */
        wu_time = ((XCVR_TSM->TIMING26&XCVR_TSM_TIMING26_TX_PA_EN_TX_HI_MASK)>>XCVR_TSM_TIMING26_TX_PA_EN_TX_HI_SHIFT)-pa_fem_settings_ptr->pa_tx_wu;
        wd_time = ((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_SHIFT)-pa_fem_settings_ptr->pa_tx_wd;
        /* The TX timing advance value must be strictly less than the difference between wd_time and wu_time due to TSM counter restrictions. */
        if (pa_fem_settings_ptr->pa_tx_wd < (wd_time-wu_time))
        {
            XCVR_TSM->TIMING49 = B3(0xFFU) | B2(0xFFU) | B1(wd_time) | B0(wu_time); /* TSM_GPIO_2 (tx switch) */
        }
        else
        {
            status = gXcvrInvalidConfiguration_c;
        }
        wu_time = ((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT)-pa_fem_settings_ptr->lna_rx_wu;
        wd_time = ((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_SHIFT)-pa_fem_settings_ptr->lna_rx_wd;
        /* The RX timing advance value must be strictly less than the difference between wd_time and wu_time due to TSM counter restrictions. */
        if (pa_fem_settings_ptr->lna_rx_wd < (wd_time-wu_time))
        {
            XCVR_TSM->TIMING50 = B3(wd_time) | B2(wu_time) | B1(0xFFU) | B0(0xFFU); /* TSM_GPIO_3 (rx switch) */
        }
        else
        {
            status = gXcvrInvalidConfiguration_c;
        }
    }
             
     return status;
}

xcvrStatus_t XCVR_ExternalFadPaFemDeInit(void)
{
    XCVR_TSM->TIMING49 = 0xFFFFFFFFUL;
    XCVR_TSM->TIMING50 = 0xFFFFFFFFUL;
    XCVR_MISC->FAD_CTRL = 0x0UL;
    return gXcvrSuccess_c;
}

#if RADIO_IS_GEN_3P0
void XCVR_ExtAntOvrd(XCVR_FAD_OVRD_ANT_A_B_SEL_MODE_T antenna_sel)
{
    uint32_t temp_reg = XCVR_MISC->FAD_CTRL;
    if (antenna_sel == XCVR_FAD_OVRD_SEL_ANT_A)
    {
        temp_reg &= ~XCVR_CTRL_FAD_CTRL_ANTX_OVRD_MASK;
    }
    else
    {
        temp_reg |= XCVR_CTRL_FAD_CTRL_ANTX_OVRD_MASK;
    }
    XCVR_MISC->FAD_CTRL = temp_reg | XCVR_CTRL_FAD_CTRL_ANTX_OVRD_EN_MASK;
}

void XCVR_ExtAntRelease(void)
{
    XCVR_MISC->FAD_CTRL &= ~XCVR_CTRL_FAD_CTRL_ANTX_OVRD_EN_MASK;
}


xcvrStatus_t XCVR_FadPaFemOnCoexInit(xcvr_pa_fem_config_t * test_settings, tx_rx_coex_pin_func_t rf_status_func, tx_rx_coex_pin_func_t rf_priority_func)
{
    uint8_t tx_wu_time;
    uint8_t tx_wd_time;
    uint8_t rx_wu_time;
    uint8_t rx_wd_time;
    uint32_t temp;
    xcvrStatus_t lastStatus = gXcvrSuccess_c;

    /* Perform any input verification checks needed and update status appropriately */
    if (test_settings == NULLPTR)
    {
        lastStatus = gXcvrInvalidParameters_c;
    }
    else
    {
        RSIM->CONTROL &= ~(RSIM_CONTROL_IPP_OBE_RF_PRIORITY_MASK | RSIM_CONTROL_IPP_OBE_RF_STATUS_MASK); /* Ensure both enable bits are cleared to start out. */
        tx_wu_time = ((XCVR_TSM->TIMING26&XCVR_TSM_TIMING26_TX_PA_EN_TX_HI_MASK)>>XCVR_TSM_TIMING26_TX_PA_EN_TX_HI_SHIFT)-test_settings->pa_tx_wu;
        tx_wd_time = ((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_TX_WD_SHIFT)-test_settings->pa_tx_wd;
        rx_wu_time = ((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_RX_WU_SHIFT)-test_settings->lna_rx_wu;
        rx_wd_time = ((XCVR_TSM->END_OF_SEQ&XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_MASK)>>XCVR_TSM_END_OF_SEQ_END_OF_RX_WD_SHIFT)-test_settings->lna_rx_wd;
        /* The TX & RX timing advance values must be strictly less than the difference between wd_time and wu_time due to TSM counter restrictions. */
        if ((test_settings->pa_tx_wd < (tx_wd_time-tx_wu_time)) && (test_settings->lna_rx_wd < (rx_wd_time-rx_wu_time)))
        {
            lastStatus = gXcvrSuccess_c;
            if (rf_status_func != NO_FUNC)
            {
                switch (rf_status_func)
                {
                    case TX_FUNC:
                        temp = B3(0xFFU) | B2(0xFFU) | B1(tx_wd_time) | B0(tx_wu_time); /*  (tx switch) */
                        break;
                    case RX_FUNC:
                        temp = B3(rx_wd_time) | B2(rx_wu_time) | B1(0xFFU) | B0(0xFFU); /*  (rx switch) */
                        break;
                    case BOTH_TX_RX_FUNC:
                        temp = B3(rx_wd_time) | B2(rx_wu_time) | B1(tx_wd_time) | B0(tx_wu_time); /*  (both tx switch and rx switch together) */
                        break;
                    default:
                        temp = B3(0xFFU) | B2(0xFFU) | B1(0xFFU) | B0(0xFFU); /* default to no function in case of error */
                        lastStatus = gXcvrInvalidConfiguration_c;
                        break;
                }
                /* setup RF_STATUS pin for TSM_SPARE2 output */
                RSIM->DSM_CONTROL &= ~(RSIM_DSM_CONTROL_WIFI_COEXIST_2_MASK); /* Clear WIFI_COEXIST_2 bit to use TSM_SPARE2 on RF_STATUS pin */
                XCVR_TSM->TIMING45 = temp; /* set the timing */
                /* Enable the output drivers for RF_STATUS and RF_PRIORITY */
                RSIM->CONTROL |= RSIM_CONTROL_IPP_OBE_RF_STATUS_MASK;
            }
            if ((rf_priority_func != NO_FUNC) && (lastStatus == gXcvrSuccess_c))
            {
                switch (rf_priority_func)
                {
                    case TX_FUNC:
                        temp = B3(0xFFU) | B2(0xFFU) | B1(tx_wd_time) | B0(tx_wu_time); /*  (tx switch) */
                        break;
                    case RX_FUNC:
                        temp = B3(rx_wd_time) | B2(rx_wu_time) | B1(0xFFU) | B0(0xFFU); /*  (rx switch) */
                        break;
                    case BOTH_TX_RX_FUNC:
                        temp = B3(rx_wd_time) | B2(rx_wu_time) | B1(tx_wd_time) | B0(tx_wu_time); /*  (both tx switch and rx switch together) */
                        break;
                    default:
                        temp = B3(0xFFU) | B2(0xFFU) | B1(0xFFU) | B0(0xFFU); /* default to no function in case of error */
                        lastStatus = gXcvrInvalidConfiguration_c;
                        break;
                }
                /* setup RF_STATUS pin for TSM_SPARE3 output */
                RSIM->DSM_CONTROL |= RSIM_DSM_CONTROL_WIFI_COEXIST_3_MASK; /* Set WIFI_COEXIST_3 bit to use TSM_SPARE3 on RF_PRIORITY pin */
                XCVR_TSM->TIMING46 = temp; /* set the timing */
                /* Enable the output drivers for RF_STATUS and RF_PRIORITY */
                RSIM->CONTROL |= RSIM_CONTROL_IPP_OBE_RF_PRIORITY_MASK;
            }
        }
        else
        {
            lastStatus = gXcvrInvalidConfiguration_c;
        }
    }

    return lastStatus;
}

void XCVR_FadPaFemOnCoexIDeInit(void)
{
    RSIM->CONTROL &= ~(RSIM_CONTROL_IPP_OBE_RF_PRIORITY_MASK | RSIM_CONTROL_IPP_OBE_RF_STATUS_MASK); /* Ensure both enable bits are cleared to prevent usage. */
}

#endif /* RADIO_IS_GEN_3P0 */
#endif /* (((RADIO_IS_GEN_3P0) || (RADIO_IS_GEN_2P1)) &&  PA_FEM_SUPPORTED */

