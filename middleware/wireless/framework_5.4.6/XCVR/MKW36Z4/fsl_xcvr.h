/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP.
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef FSL_XCVR_H
/* clang-format off */
#define FSL_XCVR_H
/* clang-format on */

/*******************************************************************************
 * The radio define must be before the driver include files
 ******************************************************************************/

/* KW4xZ/KW3xZ/KW2xZ Radio type */
#define RADIO_IS_GEN_2P1  (1)

#include "fsl_device_registers.h"

#include "fsl_xcvr_trim.h"
#if defined(gMWS_UseCoexistence_d)
#if gMWS_UseCoexistence_d
#include "MWS.h"
#endif /* gMWS_UseCoexistence_d */
#else
#define gMWS_UseCoexistence_d 0
#endif /* defined(gMWS_UseCoexistence_d) */

/* Multi-protocol support capability */
#ifndef XCVR_BLE_ENABLED
#define XCVR_BLE_ENABLED 1
#endif

#ifndef XCVR_ANT_ENABLED
#define XCVR_ANT_ENABLED 0
#endif

#ifndef XCVR_GENFSK_ENABLED
#define XCVR_GENFSK_ENABLED 0
#endif

#ifndef XCVR_802_15_4_ENABLED
#define XCVR_802_15_4_ENABLED 0
#endif

#if RADIO_IS_GEN_3P0
#ifndef XCVR_ANA /* only active if the symbol is undefined */
#define XCVR_ANA   ((XCVR_ANALOG_Type *)XCVR_ANALOG_BASE)
#endif
#endif /* RADIO_IS_GEN_3P0 */

#if !RADIO_IS_GEN_3P0 /* for other than GEN 3 radio, default definition of this symbol to disabled */
#ifndef GFSK_250KBPS_TIMING_ADJUST /* Fix for 250KBPS GFSK modes */
#define GFSK_250KBPS_TIMING_ADJUST (0)
#endif /* ifndef GFSK_250KBPS_TIMING_ADJUST */
#endif /* RADIO_IS_GEN_3P0 */

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
#ifndef USE_ZERO_FDEV /* make sure the USE_ZERO_FDEV  flag is defined to support XCVR compilation */
#define USE_ZERO_FDEV (0)
#endif /* USE_ZERO_FDEV */
#endif /* RADIO_IS_GEN_3P0 */
/*!
 * @addtogroup xcvr
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Default RF OSC definition. Allows for compile time clock frequency definition */
#ifdef CLOCK_MAIN

#else
#if RF_OSC_26MHZ == 1
#define CLOCK_MAIN (EXT_CLK_26_MHZ) /* See ext_clock_config_t for this value */
#else
#define CLOCK_MAIN (EXT_CLK_32_MHZ) /* See ext_clock_config_t for this value */
#endif /* RF_OSC_26MHZ == 1 */
#endif /* CLOCK_MAIN */
 

#define FSL_XCVR_DRIVER_VERSION (MAKE_VERSION(0, 1, 0))

#define B0(x)   (((uint32_t)(((uint32_t)(x)) << 0)) & 0xFFU)
#define B1(x)   (((uint32_t)(((uint32_t)(x)) << 8)) & 0xFF00U)
#define B2(x)   (((uint32_t)(((uint32_t)(x)) << 16)) & 0xFF0000U)
#define B3(x)   (((uint32_t)(((uint32_t)(x)) << 24)) & 0xFF000000U)

#define USE_DEFAULT_PRE_REF             (0)
#define TRIM_BBA_DCOC_DAC_AT_INIT       (1)

#if RADIO_IS_GEN_2P0
#define PRESLOW_ENA                     (1)
#endif /* RADIO_IS_GEN_2P0 */
#if RADIO_IS_GEN_2P1
#define PRESLOW_ENA                     (0)
#endif /* RADIO_IS_GEN_2P1 */
#if RADIO_IS_GEN_3P0
#define PRESLOW_ENA                     (1)
#endif /* RADIO_IS_GEN_3P0 */
#define PHY_AGC_PD_SWITCH_IDX           (0x1A)

/* ==========================  GEN3 & GEN 2.1 TSM defines ========================================== */
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)

/* TSM timings initializations for Gen3 radio */
/* NOTE: These timings are stored in 32MHz or 26MHz "baseline" settings, selected by conditional compile below */
/* The init structures for 32Mhz and 26MHz are made identical to allow the same code in fsl_xcvr.c to apply the */
/* settings for all radio generations. The Gen2 radio init value storage had a different structure so this preserves compatibility */
#if RF_OSC_26MHZ == 1
/* ********** 26MHz settings ********* */
#define TSM_TIMING00init  (0x6d006f00UL) /* (bb_ldo_hf_en) */
#define TSM_TIMING01init  (0x6d006f00UL) /* (bb_ldo_adcdac_en) */
#define TSM_TIMING02init  (0x6d00ffffUL) /* (bb_ldo_bba_en) */
#define TSM_TIMING03init  (0x6d006f00UL) /* (bb_ldo_pd_en) */
#define TSM_TIMING04init  (0x6d006f00UL) /* (bb_ldo_fdbk_en) */
#define TSM_TIMING05init  (0x6d006f00UL) /* (bb_ldo_vcolo_en) */
#define TSM_TIMING06init  (0x6d006f00UL) /* (bb_ldo_vtref_en) */
#define TSM_TIMING07init  (0x05000500UL) /* (bb_ldo_fdbk_bleed_en) */
#define TSM_TIMING08init  (0x03000300UL) /* (bb_ldo_vcolo_bleed_en) */
#define TSM_TIMING09init  (0x03000300UL) /* (bb_ldo_vcolo_fastcharge_en) */
#define TSM_TIMING10init  (0x6d036f03UL) /* (bb_xtal_pll_ref_clk_en) */
#define TSM_TIMING11init  (0xffff6f03UL) /* (bb_xtal_dac_ref_clk_en) */
#define TSM_TIMING12init  (0x6d03ffffUL) /* (rxtx_auxpll_vco_ref_clk_en) */
#define TSM_TIMING13init  (0x18004c00UL) /* (pll_loop_is_open) */
#define TSM_TIMING14init  (0xffffffffUL) /* (sy_pd_cycle_slip_ld_ft_en) */
#define TSM_TIMING15init  (0x6d036f03UL) /* (sy_vco_en) */
#define TSM_TIMING16init  (0x6d20ffffUL) /* (sy_lo_rx_buf_en) */
#define TSM_TIMING17init  (0xffff6f56UL) /* (sy_lo_tx_buf_en) */
#define TSM_TIMING18init  (0x6d056f05UL) /* (sy_divn_en) */
#define TSM_TIMING19init  (0x18034c03UL) /* (sy_pd_filter_charge_en) */
#define TSM_TIMING20init  (0x6d036f03UL) /* (sy_pd_en) */
#define TSM_TIMING21init  (0x6d046f04UL) /* (sy_lo_divn_en) */
#define TSM_TIMING22init  (0x6d04ffffUL) /* (sy_lo_rx_en) */
#define TSM_TIMING23init  (0xffff6f04UL) /* (sy_lo_tx_en) */
#define TSM_TIMING24init  (0x18004c00UL) /* (sy_divn_cal_en) */
#define TSM_TIMING25init  (0x6d21ffffUL) /* (rx_lna_mixer_en) */
#define TSM_TIMING26init  (0xffff6e59UL) /* (tx_pa_en) */
#define TSM_TIMING27init  (0x6d24ffffUL) /* (rx_adc_i_q_en) */
#define TSM_TIMING28init  (0x2524ffffUL) /* (rx_adc_reset_en) */
#define TSM_TIMING29init  (0x6d22ffffUL) /* (rx_bba_i_q_en) */
#define TSM_TIMING30init  (0x6d24ffffUL) /* (rx_bba_pdet_en) */
#define TSM_TIMING31init  (0x6d23ffffUL) /* (rx_bba_tza_dcoc_en) */
#define TSM_TIMING32init  (0x6d21ffffUL) /* (rx_tza_i_q_en) */
#define TSM_TIMING33init  (0x6d24ffffUL) /* (rx_tza_pdet_en) */
#define TSM_TIMING34init  (0x6d076f07UL) /* (pll_dig_en) */
#if RADIO_IS_GEN_3P0
#define TSM_TIMING35init  (0xffff6f60UL) /* (tx_dig_en) */
#else /* RADIO_IS_GEN_3P0 */
#define TSM_TIMING35init  (0xffff6f5CUL) /* (tx_dig_en) */
#endif /* RADIO_IS_GEN_3P0 */
#define TSM_TIMING36init  (0x6d6affffUL) /* (rx_dig_en) */
#define TSM_TIMING37init  (0x6b6affffUL) /* (rx_init) */
#define TSM_TIMING38init  (0x6d0e6f42UL) /* (sigma_delta_en) */
#define TSM_TIMING39init  (0x6d6affffUL) /* (rx_phy_en) */
#define TSM_TIMING40init  (0x6d2affffUL) /* (dcoc_en) */
#define TSM_TIMING41init  (0x2b2affffUL) /* (dcoc_init) */
#define TSM_TIMING42init  (0xffffffffUL) /* (sar_adc_trig_en) */
#define TSM_TIMING43init  (0xffffffffUL) /* (tsm_spare0_en) */
#define TSM_TIMING44init  (0xffffffffUL) /* (tsm_spare1_en) */
#define TSM_TIMING45init  (0xffffffffUL) /* (tsm_spare2_en) */
#define TSM_TIMING46init  (0xffffffffUL) /* (tsm_spare3_en) */
#define TSM_TIMING47init  (0xffffffffUL) /* (gpio0_trig_en) */
#define TSM_TIMING48init  (0xffffffffUL) /* (gpio1_trig_en) */
#define TSM_TIMING49init  (0xffffffffUL) /* (gpio2_trig_en) */
#define TSM_TIMING50init  (0xffffffffUL) /* (gpio3_trig_en) */
#define TSM_TIMING51init  (0x6d03ffffUL) /* (rxtx_auxpll_bias_en) */
#define TSM_TIMING52init  (0x1b06ffffUL) /* (rxtx_auxpll_fcal_en) */
#define TSM_TIMING53init  (0x6d03ffffUL) /* (rxtx_auxpll_lf_pd_en) */
#define TSM_TIMING54init  (0x1b03ffffUL) /* (rxtx_auxpll_pd_lf_filter_charge_en) */
#define TSM_TIMING55init  (0x6d24ffffUL) /* (rxtx_auxpll_adc_buf_en) */
#define TSM_TIMING56init  (0x6d24ffffUL) /* (rxtx_auxpll_dig_buf_en) */
#define TSM_TIMING57init  (0x1a03ffffUL) /* (rxtx_rccal_en) */
#define TSM_TIMING58init  (0xffff6f03UL) /* (tx_hpm_dac_en) */
#define END_OF_SEQinit    (0x6d6c6f68UL) /*  */
#if RADIO_IS_GEN_3P0
#define TX_RX_ON_DELinit  (0x00008a86UL) /*  */
#else /* RADIO_IS_GEN_3P0 */
#define TX_RX_ON_DELinit  (0x0000868aUL) /*  */
#endif /* RADIO_IS_GEN_3P0 */
#define TX_RX_SYNTH_init  (0x00002a18UL) /*  */
#else /* RF_OSC_26MHZ == 1 */
/* ********** 32MHz settings ********* */
#define TSM_TIMING00init  (0x69006f00UL) /* (bb_ldo_hf_en) */
#define TSM_TIMING01init  (0x69006f00UL) /* (bb_ldo_adcdac_en) */
#define TSM_TIMING02init  (0x6900ffffUL) /* (bb_ldo_bba_en) */
#define TSM_TIMING03init  (0x69006f00UL) /* (bb_ldo_pd_en) */
#define TSM_TIMING04init  (0x69006f00UL) /* (bb_ldo_fdbk_en) */
#define TSM_TIMING05init  (0x69006f00UL) /* (bb_ldo_vcolo_en) */
#define TSM_TIMING06init  (0x69006f00UL) /* (bb_ldo_vtref_en) */
#define TSM_TIMING07init  (0x05000500UL) /* (bb_ldo_fdbk_bleed_en) */
#define TSM_TIMING08init  (0x03000300UL) /* (bb_ldo_vcolo_bleed_en) */
#define TSM_TIMING09init  (0x03000300UL) /* (bb_ldo_vcolo_fastcharge_en) */
#define TSM_TIMING10init  (0x69036f03UL) /* (bb_xtal_pll_ref_clk_en) */
#define TSM_TIMING11init  (0xffff6f03UL) /* (bb_xtal_dac_ref_clk_en) */
#define TSM_TIMING12init  (0x6903ffffUL) /* (rxtx_auxpll_vco_ref_clk_en) */
#define TSM_TIMING13init  (0x18004c00UL) /* (pll_loop_is_open) */
#define TSM_TIMING14init  (0xffffffffUL) /* (sy_pd_cycle_slip_ld_ft_en) */
#define TSM_TIMING15init  (0x69036f03UL) /* (sy_vco_en) */
#define TSM_TIMING16init  (0x691cffffUL) /* (sy_lo_rx_buf_en) */
#define TSM_TIMING17init  (0xffff6f55UL) /* (sy_lo_tx_buf_en) */
#define TSM_TIMING18init  (0x69056f05UL) /* (sy_divn_en) */
#define TSM_TIMING19init  (0x18034c03UL) /* (sy_pd_filter_charge_en) */
#define TSM_TIMING20init  (0x69036f03UL) /* (sy_pd_en) */
#define TSM_TIMING21init  (0x69046f04UL) /* (sy_lo_divn_en) */
#define TSM_TIMING22init  (0x6904ffffUL) /* (sy_lo_rx_en) */
#define TSM_TIMING23init  (0xffff6f04UL) /* (sy_lo_tx_en) */
#define TSM_TIMING24init  (0x18004c00UL) /* (sy_divn_cal_en) */
#define TSM_TIMING25init  (0x691dffffUL) /* (rx_lna_mixer_en) */
#define TSM_TIMING26init  (0xffff6e58UL) /* (tx_pa_en) */
#define TSM_TIMING27init  (0x6920ffffUL) /* (rx_adc_i_q_en) */
#define TSM_TIMING28init  (0x2120ffffUL) /* (rx_adc_reset_en) */
#define TSM_TIMING29init  (0x691effffUL) /* (rx_bba_i_q_en) */
#define TSM_TIMING30init  (0x6920ffffUL) /* (rx_bba_pdet_en) */
#define TSM_TIMING31init  (0x691fffffUL) /* (rx_bba_tza_dcoc_en) */
#define TSM_TIMING32init  (0x691dffffUL) /* (rx_tza_i_q_en) */
#define TSM_TIMING33init  (0x6920ffffUL) /* (rx_tza_pdet_en) */
#define TSM_TIMING34init  (0x69076f07UL) /* (pll_dig_en) */
#define TSM_TIMING35init  (0xffff6f5fUL) /* (tx_dig_en) */
#define TSM_TIMING36init  (0x6966ffffUL) /* (rx_dig_en) */
#define TSM_TIMING37init  (0x6766ffffUL) /* (rx_init) */
#define TSM_TIMING38init  (0x690e6f42UL) /* (sigma_delta_en) */
#define TSM_TIMING39init  (0x6966ffffUL) /* (rx_phy_en) */
#define TSM_TIMING40init  (0x6926ffffUL) /* (dcoc_en) */
#define TSM_TIMING41init  (0x2726ffffUL) /* (dcoc_init) */
#define TSM_TIMING42init  (0xffffffffUL) /* (sar_adc_trig_en) */
#define TSM_TIMING43init  (0xffffffffUL) /* (tsm_spare0_en) */
#define TSM_TIMING44init  (0xffffffffUL) /* (tsm_spare1_en) */
#define TSM_TIMING45init  (0xffffffffUL) /* (tsm_spare2_en) */
#define TSM_TIMING46init  (0xffffffffUL) /* (tsm_spare3_en) */
#define TSM_TIMING47init  (0xffffffffUL) /* (gpio0_trig_en) */
#define TSM_TIMING48init  (0xffffffffUL) /* (gpio1_trig_en) */
#define TSM_TIMING49init  (0xffffffffUL) /* (gpio2_trig_en) */
#define TSM_TIMING50init  (0xffffffffUL) /* (gpio3_trig_en) */
#define TSM_TIMING51init  (0x6903ffffUL) /* (rxtx_auxpll_bias_en) */
#define TSM_TIMING52init  (0x1706ffffUL) /* (rxtx_auxpll_fcal_en) */
#define TSM_TIMING53init  (0x6903ffffUL) /* (rxtx_auxpll_lf_pd_en) */
#define TSM_TIMING54init  (0x1703ffffUL) /* (rxtx_auxpll_pd_lf_filter_charge_en) */
#define TSM_TIMING55init  (0x6920ffffUL) /* (rxtx_auxpll_adc_buf_en) */
#define TSM_TIMING56init  (0x6920ffffUL) /* (rxtx_auxpll_dig_buf_en) */
#define TSM_TIMING57init  (0x1a03ffffUL) /* (rxtx_rccal_en) */
#define TSM_TIMING58init  (0xffff6f03UL) /* (tx_hpm_dac_en) */
#define END_OF_SEQinit    (0x69686f67UL) /*  */
#if RADIO_IS_GEN_3P0
#define TX_RX_ON_DELinit  (0x00008286UL) /*  */
#else /* RADIO_IS_GEN_3P0 */
#define TX_RX_ON_DELinit  (0x0000848aUL) /*  */
#endif /* RADIO_IS_GEN_3P0 */
#define TX_RX_SYNTH_init  (0x00002318UL) /*  */
#endif /* RF_OSC_26MHZ == 1 */

#define AUX_PLL_DELAY       (0UL)
/* TSM bitfield shift and value definitions */
#define TX_DIG_EN_ASSERT    (95L) /* Assertion time for TX_DIG_EN, used in mode specific settings */
#define ZGBE_TX_DIG_EN_ASSERT (TX_DIG_EN_ASSERT - 1UL) /* Zigbee TX_DIG_EN must assert 1 tick sooner, see adjustment below based on data padding */
#if RADIO_IS_GEN_2P1
#define TX_DIG_EN_TX_HI_ADJ (0UL)
#endif
/* EDIT THIS LINE TO CONTROL PA_RAMP! */
#define PA_RAMP_TIME        (1) /* Only allowable values are [0, 1, 2, or 4] in Gen3 */
#define PA_RAMP_SEL_0US     (0)
#define PA_RAMP_SEL_1US     (1)
#define PA_RAMP_SEL_2US     (2)
#define PA_RAMP_SEL_4US     (3)
#if !((PA_RAMP_TIME == 0) || (PA_RAMP_TIME == 1) || (PA_RAMP_TIME == 2) || (PA_RAMP_TIME == 4))
#error "Invalid value for PA_RAMP_TIME macro"
#endif /* Error check of PA RAMP TIME */

#define ADD_FOR_26MHZ           (4UL)
#define END_OF_TX_WU_NORAMP     (103UL) /* NOTE: NORAMP and 2us ramp time behaviors are identical for TX WU and WD */
#define END_OF_TX_WD_NORAMP     (111UL) /* NOTE: NORAMP and 2us ramp time behaviors are identical for TX WU and WD */
/* Redefine the values of END_OF_TX_WU and END_OF_TX_WD based on whether DATA PADDING is enabled and the selection of ramp time */
/* These two constants are then used on both common configuration and mode specific configuration files to define the TSM timing values */
#if ((PA_RAMP_TIME == 0) || (PA_RAMP_TIME == 1) || (PA_RAMP_TIME == 2))
    #define END_OF_TX_WU   (END_OF_TX_WU_NORAMP)
    #define END_OF_TX_WD   (END_OF_TX_WD_NORAMP)
    #if (PA_RAMP_TIME == 0)
        #define PA_RAMP_SEL PA_RAMP_SEL_0US
        #define DATA_PADDING_EN (0)
    #else
        #define DATA_PADDING_EN (1)
        #if (PA_RAMP_TIME == 1)
            #define PA_RAMP_SEL PA_RAMP_SEL_1US
        #else
            #define PA_RAMP_SEL PA_RAMP_SEL_2US
        #endif /* (PA_RAMP_TIME == 1) */
    #endif /* (PA_RAMP_TIME == 0) */
#else /* ((PA_RAMP_TIME == 0) || (PA_RAMP_TIME == 1) || (PA_RAMP_TIME == 2)) */
    #if (PA_RAMP_TIME == 4)
        #define END_OF_TX_WU    (END_OF_TX_WU_NORAMP + 2UL)
        #define END_OF_TX_WD    (END_OF_TX_WD_NORAMP + 4UL)
        #define PA_RAMP_SEL PA_RAMP_SEL_4US
        #define DATA_PADDING_EN (1)
    #else /* (PA_RAMP_TIME == 4) */
        #error "Invalid value for PA_RAMP_TIME macro"
    #endif /* (PA_RAMP_TIME == 4) */
#endif/* (PA_RAMP_TIME == 4) */

#define END_OF_RX_WU    (104 + AUX_PLL_DELAY)

#if RF_OSC_26MHZ == 1
#define END_OF_RX_WD    (END_OF_RX_WU + 1 + ADD_FOR_26MHZ) /* Need to handle normal signals extending when 26MHZ warmdown is extended */
#else
#define END_OF_RX_WD    (END_OF_RX_WU + 1)
#endif /* RF_OSC_26MHZ == 1 */

#define END_OF_RX_WU_26MHZ  (END_OF_RX_WU + ADD_FOR_26MHZ)
#define END_OF_RX_WD_26MHZ  (END_OF_RX_WU + 1 + ADD_FOR_26MHZ)

/* PA Bias Table - Gen3 version */
#define PA_RAMP_0 0x1
#define PA_RAMP_1 0x2
#define PA_RAMP_2 0x4
#define PA_RAMP_3 0x6
#define PA_RAMP_4 0x8
#define PA_RAMP_5 0xc
#define PA_RAMP_6 0x10
#define PA_RAMP_7 0x14
#define PA_RAMP_8 0x18
#define PA_RAMP_9 0x1c
#define PA_RAMP_10 0x22
#define PA_RAMP_11 0x28
#define PA_RAMP_12 0x2c
#define PA_RAMP_13 0x30
#define PA_RAMP_14 0x36
#define PA_RAMP_15 0x3c

#if (RADIO_IS_GEN_3P0 && GFSK_250KBPS_TIMING_ADJUST)
/* Timing adjustment factors for 250Kbps GENFSK DCOC TSM timings */
#define GFSK_250K_DC_CAL_STEP_DELAY     (11UL) /* delay to add to each step of the DCOC DC cal */
#define GFSK_250K_RX_WU_WD_ADJUST       (((GFSK_250K_DC_CAL_STEP_DELAY*3U)<<24) | ((GFSK_250K_DC_CAL_STEP_DELAY*3U)<<16)) /* RX WU and WD positions in TSM timings */
#define GFSK_250K_RX_WD_ONLY_ADJUST       ((GFSK_250K_DC_CAL_STEP_DELAY*3U)<<24)  /* RX WD only position in TSM timings */
#endif /* RADIO_IS_GEN_3P0 && GFSK_250KBPS_TIMING_ADJUST */

#else /* ===================== Gen2.0 TSM definitions ===============================================================================*/
/* GEN2 TSM defines */
#define AUX_PLL_DELAY       (0UL)
/* TSM bitfield shift and value definitions */
#define TX_DIG_EN_ASSERT    (95L) /* Assertion time for TX_DIG_EN, used in mode specific settings */
#define ZGBE_TX_DIG_EN_ASSERT (TX_DIG_EN_ASSERT - 1UL) /* Zigbee TX_DIG_EN must assert 1 tick sooner, see adjustment below based on data padding */
/* EDIT THIS LINE TO CONTROL PA_RAMP! */
#define PA_RAMP_TIME        (2) /* Only allowable values are [0, 2, 4, or 8] for PA RAMP times in Gen2.0 */
#define PA_RAMP_SEL_0US     (0)
#define PA_RAMP_SEL_2US     (1)
#define PA_RAMP_SEL_4US     (2)
#define PA_RAMP_SEL_8US     (3)

#if !((PA_RAMP_TIME == 0) || (PA_RAMP_TIME == 2) || (PA_RAMP_TIME == 4) || (PA_RAMP_TIME == 8))
#error "Invalid value for PA_RAMP_TIME macro"
#endif /* Error check of PA RAMP TIME */
#define ADD_FOR_26MHZ           (4UL)
#define END_OF_TX_WU_NORAMP     (103UL) /* NOTE: NORAMP and 2us ramp time behaviors are identical for TX WU and WD */
#define END_OF_TX_WD_NORAMP     (111UL) /* NOTE: NORAMP and 2us ramp time behaviors are identical for TX WU and WD */
/* Redefine the values of END_OF_TX_WU and END_OF_TX_WD based on whether DATA PADDING is enabled and the selection of ramp time */
/* These two constants are then used on both common configuration and mode specific configuration files to define the TSM timing values */
#if ((PA_RAMP_TIME == 0) || (PA_RAMP_TIME == 2))
    #define END_OF_TX_WU   (END_OF_TX_WU_NORAMP)
    #define END_OF_TX_WD   (END_OF_TX_WD_NORAMP)
    #define TX_SYNTH_DELAY_ADJ       (0)
    #define PD_CYCLE_SLIP_TX_HI_ADJ  (0UL)
    #define PD_CYCLE_SLIP_TX_LO_ADJ  (1UL)
    #define ZGBE_TX_DIG_EN_TX_HI_ADJ (-5) /* Only applies to Zigbee mode */
    #if (PA_RAMP_TIME == 0)
        #define PA_RAMP_SEL PA_RAMP_SEL_0US
        #define DATA_PADDING_EN     (0)
        #define TX_DIG_EN_TX_HI_ADJ (-2)
    #else
        #define DATA_PADDING_EN     (1)
        #define TX_DIG_EN_TX_HI_ADJ (0)
        #define PA_RAMP_SEL PA_RAMP_SEL_2US
    #endif /* (PA_RAMP_TIME == 0) */
#else /* ((PA_RAMP_TIME == 0) || (PA_RAMP_TIME == 2)) */
    #if (PA_RAMP_TIME == 4)
        #define END_OF_TX_WU    (END_OF_TX_WU_NORAMP + 2)
        #define END_OF_TX_WD    (END_OF_TX_WD_NORAMP + 4)
        #define TX_SYNTH_DELAY_ADJ       (2)
        #define PD_CYCLE_SLIP_TX_HI_ADJ  (2)
        #define PD_CYCLE_SLIP_TX_LO_ADJ  (1)
        #define TX_DIG_EN_TX_HI_ADJ      (0)
        #define ZGBE_TX_DIG_EN_TX_HI_ADJ (-3) /* Only applies to Zigbee mode */
        #define PA_RAMP_SEL PA_RAMP_SEL_4US
        #define DATA_PADDING_EN (1)
    #else /* (PA_RAMP_TIME==4) */
        #if ((PA_RAMP_TIME == 8) && (!RADIO_IS_GEN_3P0))
            #define END_OF_TX_WU    (END_OF_TX_WU_NORAMP + 6)
            #define END_OF_TX_WD    (END_OF_TX_WD_NORAMP + 12)
            #define TX_SYNTH_DELAY_ADJ       (6)
            #define PD_CYCLE_SLIP_TX_HI_ADJ  (6)
            #define PD_CYCLE_SLIP_TX_LO_ADJ  (1)
            #define TX_DIG_EN_TX_HI_ADJ      (4)
            #define ZGBE_TX_DIG_EN_TX_HI_ADJ (1) /* Only applies to Zigbee mode */
            #define PA_RAMP_SEL PA_RAMP_SEL_8US
            #define DATA_PADDING_EN (1)
        #else /* (PA_RAMP_TIME == 8) */
            #error "Invalid value for PA_RAMP_TIME macro"
        #endif /* (PA_RAMP_TIME == 8) */
    #endif/* (PA_RAMP_TIME == 4) */
#endif /* ((PA_RAMP_TIME == 0) || (PA_RAMP_TIME == 2)) */

#define TX_DIG_EN_ASSERT_MSK500    (END_OF_TX_WU - 3)

#define END_OF_RX_WU    (104 + AUX_PLL_DELAY)
#if RF_OSC_26MHZ == 1
#define END_OF_RX_WD    (END_OF_RX_WU + 1 + ADD_FOR_26MHZ)  /* Need to handle normal signals extending when 26MHZ warmdown is extended */
#else
#define END_OF_RX_WD    (END_OF_RX_WU + 1)
#endif /* RF_OSC_26MHZ == 1 */
#define END_OF_RX_WU_26MHZ  (END_OF_RX_WU + ADD_FOR_26MHZ)
#define END_OF_RX_WD_26MHZ  (END_OF_RX_WU + 1 + ADD_FOR_26MHZ)

/* PA Bias Table */
#define PA_RAMP_0 0x1
#define PA_RAMP_1 0x2
#define PA_RAMP_2 0x4
#define PA_RAMP_3 0x8
#define PA_RAMP_4 0xe
#define PA_RAMP_5 0x16
#define PA_RAMP_6 0x22
#define PA_RAMP_7 0x2e

/* BLE LL timing definitions */
#define TX_ON_DELAY     (0x85) /* Adjusted TX_ON_DELAY to make turnaround time 150usec */
#define RX_ON_DELAY     (29 + END_OF_RX_WU + 4)
#define RX_ON_DELAY_26MHZ     (29 + END_OF_RX_WU_26MHZ + 4)
#define TX_RX_ON_DELAY_VAL  (TX_ON_DELAY << 8 | RX_ON_DELAY)
#define TX_RX_ON_DELAY_VAL_26MHZ  (TX_ON_DELAY << 8 | RX_ON_DELAY_26MHZ)
#define TX_SYNTH_DELAY  (TX_ON_DELAY - END_OF_TX_WU - TX_SYNTH_DELAY_ADJ) /* Adjustment to TX_SYNTH_DELAY due to DATA_PADDING */
#define RX_SYNTH_DELAY  (0x18)
#define TX_RX_SYNTH_DELAY_VAL (TX_SYNTH_DELAY << 8 | RX_SYNTH_DELAY)
/* ================================================ end of GEN 2.0 definitions ================================= */
#endif /* RADIO_IS_GEN_3P0 */

/* PHY reference waveform assembly */
#define RW0PS(loc, val)      (((val) & 0x1F) << ((loc) * 5)) /* Ref Word 0 - loc is the phase info symbol number, val is the value of the phase info */
#define RW1PS(loc, val)      (((val) & 0x1F) << (((loc) * 5) - 32)) /* Ref Word 1 - loc is the phase info symbol number, val is the value of the phase info */
#define RW2PS(loc, val)      (((val) & 0x1F) << (((loc) * 5) - 64)) /* Ref Word 2 - loc is the phase info symbol number, val is the value of the phase info */

/*! @brief  Error codes for the XCVR driver. */
typedef enum _xcvrStatus
{
    gXcvrSuccess_c = 0,
    gXcvrInvalidParameters_c,
    gXcvrUnsupportedOperation_c,
    gXcvrTrimFailure_c,
    gXcvrInvalidConfiguration_c,
    gXcvrConfigurationFailure_c,
} xcvrStatus_t;

/*! @brief  Health status returned from PHY upon status check function return. */
typedef enum _healthStatus
{
    NO_ERRORS = 0,
    PLL_CTUNE_FAIL = 1,
    PLL_CYCLE_SLIP_FAIL = 2,
    PLL_FREQ_TARG_FAIL = 4,
    PLL_TSM_ABORT_FAIL = 8,
} healthStatus_t;

/*! @brief  Health status returned from PHY upon status check function return. */
typedef enum _ext_clock_config
{
    EXT_CLK_32_MHZ = 0,
    EXT_CLK_26_MHZ = 1,
} ext_clock_config_t;

/*! @brief  Radio operating mode setting types. */
typedef enum _radio_mode
{
    BLE_MODE = 0,
    ZIGBEE_MODE = 1,
    ANT_MODE = 2,

    /* BT=0.5, h=** */
    GFSK_BT_0p5_h_0p5  = 3, /* < BT=0.5, h=0.5 [BLE at 1MBPS data rate; CS4 at 250KBPS data rate] */
    GFSK_BT_0p5_h_0p32 = 4, /* < BT=0.5, h=0.32*/
    GFSK_BT_0p5_h_0p7  = 5, /* < BT=0.5, h=0.7 [ CS1 at 500KBPS data rate] */
    GFSK_BT_0p5_h_1p0  = 6, /* < BT=0.5, h=1.0 [ CS4 at 250KBPS data rate] */

    /* BT=** h=0.5 */
    GFSK_BT_0p3_h_0p5 = 7, /* < BT=0.3, h=0.5 [ CS2 at 1MBPS data rate] */
    GFSK_BT_0p7_h_0p5 = 8, /* < BT=0.7, h=0.5 */

    MSK = 9,
    NUM_RADIO_MODES = 10,
} radio_mode_t;

/*! @brief  Link layer types. */
typedef enum _link_layer
{
    BLE_LL = 0, /* Must match bit assignment in RADIO1_IRQ_SEL */
    ZIGBEE_LL = 1, /* Must match bit assignment in RADIO1_IRQ_SEL */
    ANT_LL = 2, /* Must match bit assignment in RADIO1_IRQ_SEL */
    GENFSK_LL = 3, /* Must match bit assignment in RADIO1_IRQ_SEL */
    UNASSIGNED_LL = 4, /* Must match bit assignment in RADIO1_IRQ_SEL */
} link_layer_t;

/*! @brief  Data rate selections. */
typedef enum _data_rate
{
    DR_1MBPS = 0, /* Must match bit assignment in BITRATE field */
    DR_500KBPS = 1, /* Must match bit assignment in BITRATE field */
    DR_250KBPS = 2, /* Must match bit assignment in BITRATE field */
#if RADIO_IS_GEN_3P0
    DR_2MBPS = 3, /* Must match bit assignment in BITRATE field */
#endif /* RADIO_IS_GEN_3P0 */
    DR_UNASSIGNED = 4, /* Must match bit assignment in BITRATE field */
} data_rate_t;

/*! @brief  Control settings for Fast Antenna Diversity */
typedef enum  _FAD_LPPS_CTRL
{
    NONE = 0,
    FAD_ENABLED = 1,
    LPPS_ENABLED = 2
} FAD_LPPS_CTRL_T;

/*! @brief  XCVR Panic codes for indicating panic reason. */
typedef enum _XCVR_PANIC_ID
{
    WRONG_RADIO_ID_DETECTED = 1,
    CALIBRATION_INVALID = 2,
    RADIO_INIT_FAILURE = 3,
} XCVR_PANIC_ID_T;

/*! @brief  Initialization or mode change selection for config routine. */
typedef enum _XCVR_INIT_MODE_CHG
{
    XCVR_MODE_CHANGE = 0,
    XCVR_FIRST_INIT = 1,
} XCVR_INIT_MODE_CHG_T;

/*! @brief  Coexistence arbitration priority settings. */
typedef enum _XCVR_COEX_PRIORITY
{
    XCVR_COEX_LOW_PRIO = 0,
    XCVR_COEX_HIGH_PRIO = 1
}   XCVR_COEX_PRIORITY_T;

/*! @brief  PA/FEM control mode settings. */
typedef enum _XCVR_ANTX_MODE
{
    XCVR_ANTX_SINGLE_MODE = 0,
    XCVR_ANTX_DUAL_MODE = 1
}   XCVR_ANTX_MODE_T;

/*! @brief  PA/FEM GPIO vs TSM settings. */
typedef enum _XCVR_FAD_NOT_GPIO_MODE
{
    XCVR_FAD_TSM_GPIO = 0,
    XCVR_FAD_STATE_MACHINE = 1
}   XCVR_FAD_NOT_GPIO_MODE_T;

/*! @brief  PA/FEM RX/TX polarity settings. */
typedef enum _XCVR_RX_TX_POLARITY_MODE
{
    XCVR_FAD_ACTIVE_HIGH = 0,
    XCVR_FAD_ACTIVE_LOW = 1
}   XCVR_RX_TX_POLARITY_MODE_T;

/*! @brief  PA/FEM RX/TX polarity settings. */
typedef enum _XCVR_FAD_OVRD_ANT_A_B_SEL_MODE
{
    XCVR_FAD_OVRD_SEL_ANT_A = 0,
    XCVR_FAD_OVRD_SEL_ANT_B = 1
}   XCVR_FAD_OVRD_ANT_A_B_SEL_MODE_T;

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
/*!@brief TX & RX PA/FEM function settings for PA/FEM control over coexistence pins.  */
typedef enum
{
    NO_FUNC = 0,
    TX_FUNC = 1,
    RX_FUNC = 2,
    BOTH_TX_RX_FUNC = 3
} tx_rx_coex_pin_func_t;
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */


/*! @brief PA/FEM  configuration structure. */
typedef struct xcvr_pa_fem_config
{
    XCVR_ANTX_MODE_T op_mode;                           /*!< operating mode for the PA/FEM interface */
    uint8_t ant_sel_pins_enable;                        /*!< control whether ANT_A & ANT_B pads should be active (0=Disabled, 1=Enabled) */
    uint8_t tx_rx_switch_pins_enable;                   /*!< control whether TX and RX_SWITCH pads should be active (0=Disabled, 1=Enabled) */
    uint8_t high_z_enable;                              /*!< control whether FEM/PA  pads should use High Z (0=Disabled, 1=Enabled) */
    uint8_t use_fad_state_machine;                      /*!< control whether FAD state machine is active (0=Disabled, 1=Enabled) */
    XCVR_FAD_NOT_GPIO_MODE_T ant_a_pad_control;         /*!< control whether ANT_A pad should be controlled by FAD state machine or TSM GPIO  */
    XCVR_FAD_NOT_GPIO_MODE_T ant_b_pad_control;         /*!< control whether ANT_B pad should be controlled by FAD state machine or TSM GPIO  */
    XCVR_FAD_NOT_GPIO_MODE_T tx_switch_pad_control;     /*!< control whether TX_SWITCH pad should be controlled by FAD state machine or TSM GPIO  */
    XCVR_FAD_NOT_GPIO_MODE_T rx_switch_pad_control;     /*!< control whether RX_SWITCH pad should be controlled by FAD state machine or TSM GPIO  */
    uint8_t pa_tx_wu;                                   /*!< Number of usec to start external PA warmup ahead of internal PA warmup (ramp) start  */
    uint8_t pa_tx_wd;                                   /*!< Number of usec to start external PA warmdown ahead of internal PA warmdown (ramp) completion  */
    uint8_t lna_rx_wu;                                  /*!< Number of usec to start external LNA warmup ahead of internal LNA warmup  start  */
    uint8_t lna_rx_wd;                                  /*!< Number of usec to start external LNA warmdown ahead of internal LNA warmdown completion  */
    XCVR_RX_TX_POLARITY_MODE_T tx_switch_pol_control;   /*!< control whether TX_SWITCH pad should be active high or low  (0=Active HIGH, 1=Active LOW) */
    XCVR_RX_TX_POLARITY_MODE_T rx_switch_pol_control;   /*!< control whether RX_SWITCH pad should be active high or low  (0=Active HIGH, 1=Active LOW) */

} xcvr_pa_fem_config_t;


/*! @brief Current configuration of the radio. */
typedef struct xcvr_curr_config
{
    radio_mode_t radio_mode;
    data_rate_t data_rate;
} xcvr_currConfig_t;


/*!
 * @brief XCVR RX_DIG channel filter coefficient storage
 * Storage of the coefficients varies from 6 bits to 10 bits so all use int16_t for storage.
 */
typedef struct _xcvr_rx_chf_coeffs
{
    uint16_t rx_chf_coef_0; /*!< 6 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_1; /*!< 6 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_2; /*!< 7 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_3; /*!< 7 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_4; /*!< 7 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_5; /*!< 7 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_6; /*!< 8 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_7; /*!< 8 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_8; /*!< 9 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_9; /*!< 9 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_10; /*!< 10 bit two's complement stored in a uint16_t */
    uint16_t rx_chf_coef_11; /*!< 10 bit two's complement stored in a uint16_t */
} xcvr_rx_chf_coeffs_t;

/*!
 * @brief XCVR DCOC DAC trim register value storage
 * Storage of the entire 32 bit register contents for all DCOC DAC trim registers.
 */
typedef struct _xcvr_dcoc_dac_trim
{
    uint32_t dcoc_bba_step_reg; /*!< XCVR_RX_DIG->DCOC_BBA_STEP contents */
    uint32_t dcoc_tza_step_0_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_0 contents */
    uint32_t dcoc_tza_step_1_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_1 contents */
    uint32_t dcoc_tza_step_2_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_2 contents */
    uint32_t dcoc_tza_step_3_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_3 contents */
    uint32_t dcoc_tza_step_4_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_4 contents */
    uint32_t dcoc_tza_step_5_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_5 contents */
    uint32_t dcoc_tza_step_6_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_6 contents */
    uint32_t dcoc_tza_step_7_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_7 contents */
    uint32_t dcoc_tza_step_8_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_8 contents */
    uint32_t dcoc_tza_step_9_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_9 contents */
    uint32_t dcoc_tza_step_10_reg; /*!< XCVR_RX_DIG->DCOC_TZA_STEP_10 contents */
    uint32_t dcoc_dac_init_reg; /*!< XCVR_RX_DIG->DCOC_DAC_INIT contents */
} xcvr_DcocDacTrim_t;

/*!
 * @brief XCVR masked init type for 32 bit registers
 * Initialization uses the mask to clear selected fields of the register and then OR's in the init value. All init values must be in their proper field position.
 */
typedef struct _xcvr_masked_init_32
{
    uint32_t mask;
    uint32_t init;
} xcvr_masked_init_32_t;

/*!
 * @brief XCVR common configure structure
 */
typedef struct _xcvr_common_config
{
    /* XCVR_ANA configs */
    xcvr_masked_init_32_t ana_sy_ctrl1;

    /* XCVR_PLL_DIG configs */
    uint32_t pll_hpm_bump;
    uint32_t pll_mod_ctrl;
    uint32_t pll_chan_map;
    uint32_t pll_lock_detect;
    uint32_t pll_hpm_ctrl;
#if !RADIO_IS_GEN_2P1
    uint32_t pll_hpmcal_ctrl;
#endif /* !RADIO_IS_GEN_2P1 */
    uint32_t pll_hpm_sdm_res;
    uint32_t pll_lpm_ctrl;
    uint32_t pll_lpm_sdm_ctrl1;
    uint32_t pll_delay_match;
    uint32_t pll_ctune_ctrl;

    /* XCVR_RX_DIG configs */
    uint32_t rx_dig_ctrl_init; /* NOTE: Common init, mode init, and datarate init will be OR'd together for RX_DIG_CTRL to form complete register initialization */
    uint32_t dcoc_ctrl_0_init_26mhz; /* NOTE: This will be OR'd with mode specific init for DCOC_CTRL_0 to form complete register initialization */
    uint32_t dcoc_ctrl_0_init_32mhz; /* NOTE: This will be OR'd with mode specific init for DCOC_CTRL_0 to form complete register initialization */
    uint32_t dcoc_ctrl_1_init;
    uint32_t lna_gain_val_3_0;
    uint32_t lna_gain_val_7_4;
    uint32_t lna_gain_val_8;
    uint32_t bba_res_tune_val_7_0;
    uint32_t bba_res_tune_val_10_8;
    uint32_t lna_gain_lin_val_2_0_init;
    uint32_t lna_gain_lin_val_5_3_init;
    uint32_t lna_gain_lin_val_8_6_init;
    uint32_t lna_gain_lin_val_9_init;
    uint32_t bba_res_tune_lin_val_3_0_init;
    uint32_t bba_res_tune_lin_val_7_4_init;
    uint32_t bba_res_tune_lin_val_10_8_init;
    uint32_t dcoc_bba_step_init;
    uint32_t dcoc_tza_step_00_init;
    uint32_t dcoc_tza_step_01_init;
    uint32_t dcoc_tza_step_02_init;
    uint32_t dcoc_tza_step_03_init;
    uint32_t dcoc_tza_step_04_init;
    uint32_t dcoc_tza_step_05_init;
    uint32_t dcoc_tza_step_06_init;
    uint32_t dcoc_tza_step_07_init;
    uint32_t dcoc_tza_step_08_init;
    uint32_t dcoc_tza_step_09_init;
    uint32_t dcoc_tza_step_10_init;
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
    uint32_t dcoc_cal_fail_th_init;
    uint32_t dcoc_cal_pass_th_init;
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */
    uint32_t agc_ctrl_0_init; /* NOTE: Common init and mode init will be OR'd together for AGC_CTRL_0 to form complete register initialization */
    uint32_t agc_ctrl_1_init_26mhz; /* NOTE: This will be OR'd with datarate specific init to form complete register initialization */
    uint32_t agc_ctrl_1_init_32mhz; /* NOTE: This will be OR'd with datarate specific init to form complete register initialization */
    uint32_t agc_ctrl_3_init;
    /* Other agc config inits are in the modeXdatarate config table */
    uint32_t agc_gain_tbl_03_00_init;
    uint32_t agc_gain_tbl_07_04_init;
    uint32_t agc_gain_tbl_11_08_init;
    uint32_t agc_gain_tbl_15_12_init;
    uint32_t agc_gain_tbl_19_16_init;
    uint32_t agc_gain_tbl_23_20_init;
    uint32_t agc_gain_tbl_26_24_init;
    uint32_t rssi_ctrl_0_init;
#if !RADIO_IS_GEN_3P0
    uint32_t dc_resid_ctrl_init; /* NOTE: This will be OR'd with datarate specific init for DCOC_RESID_CTRL to form complete register initialization */
#endif
    uint32_t dcoc_cal_rcp_init; /* NOTE:  Common init and mode_datarate init will be OR'd together for DCOC_CAL_RCP to form complete register initialization */
    uint32_t dcoc_cal_gain_init; /* NOTE:  Common init and mode init will be OR'd together for DCOC_CAL_GAIN to form complete register initialization */
    uint32_t rssi_ctrl_1_init;
    uint32_t cca_ed_lqi_ctrl_0_init;
    uint32_t cca_ed_lqi_ctrl_1_init;

    /* XCVR_TSM configs */
    uint32_t tsm_ctrl;
    uint32_t tsm_ovrd2_init;
    uint32_t end_of_seq_init_26mhz;
    uint32_t end_of_seq_init_32mhz;
#if !RADIO_IS_GEN_2P1
    uint32_t lpps_ctrl_init;
#endif /* !RADIO_IS_GEN_2P1 */
    uint32_t tsm_fast_ctrl2_init_26mhz;
    uint32_t tsm_fast_ctrl2_init_32mhz;
    uint32_t recycle_count_init_26mhz;
    uint32_t recycle_count_init_32mhz;
    uint32_t pa_ramp_tbl_0_init;
    uint32_t pa_ramp_tbl_1_init;
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
    uint32_t pa_ramp_tbl_2_init;
    uint32_t pa_ramp_tbl_3_init;
#endif /* RADIO_IS_GEN_3P0 */
    uint32_t tsm_timing_00_init;
    uint32_t tsm_timing_01_init;
    uint32_t tsm_timing_02_init;
    uint32_t tsm_timing_03_init;
    uint32_t tsm_timing_04_init;
    uint32_t tsm_timing_05_init;
    uint32_t tsm_timing_06_init;
    uint32_t tsm_timing_07_init;
    uint32_t tsm_timing_08_init;
    uint32_t tsm_timing_09_init;
    uint32_t tsm_timing_10_init;
    uint32_t tsm_timing_11_init;
    uint32_t tsm_timing_12_init;
    uint32_t tsm_timing_13_init;
    uint32_t tsm_timing_14_init_26mhz; /* tsm_timing_14 has mode specific LSbyte (both LS bytes) */
    uint32_t tsm_timing_14_init_32mhz; /* tsm_timing_14 has mode specific LSbyte (both LS bytes) */
    uint32_t tsm_timing_15_init;
    uint32_t tsm_timing_16_init_26mhz;
    uint32_t tsm_timing_16_init_32mhz;
    uint32_t tsm_timing_17_init;
    uint32_t tsm_timing_18_init;
    uint32_t tsm_timing_19_init;
    uint32_t tsm_timing_20_init;
    uint32_t tsm_timing_21_init;
    uint32_t tsm_timing_22_init;
    uint32_t tsm_timing_23_init;
    uint32_t tsm_timing_24_init;
    uint32_t tsm_timing_25_init_26mhz;
    uint32_t tsm_timing_25_init_32mhz;
    uint32_t tsm_timing_26_init;
    uint32_t tsm_timing_27_init_26mhz;
    uint32_t tsm_timing_27_init_32mhz;
    uint32_t tsm_timing_28_init_26mhz;
    uint32_t tsm_timing_28_init_32mhz;
    uint32_t tsm_timing_29_init_26mhz;
    uint32_t tsm_timing_29_init_32mhz;
    uint32_t tsm_timing_30_init_26mhz;
    uint32_t tsm_timing_30_init_32mhz;
    uint32_t tsm_timing_31_init_26mhz;
    uint32_t tsm_timing_31_init_32mhz;
    uint32_t tsm_timing_32_init_26mhz;
    uint32_t tsm_timing_32_init_32mhz;
    uint32_t tsm_timing_33_init_26mhz;
    uint32_t tsm_timing_33_init_32mhz;
    uint32_t tsm_timing_34_init;
    uint32_t tsm_timing_35_init; /* tsm_timing_35 has a mode specific LSbyte */
    uint32_t tsm_timing_36_init_26mhz;
    uint32_t tsm_timing_36_init_32mhz;
    uint32_t tsm_timing_37_init_26mhz;
    uint32_t tsm_timing_37_init_32mhz;
    uint32_t tsm_timing_38_init;
    uint32_t tsm_timing_39_init_26mhz;
    uint32_t tsm_timing_39_init_32mhz;
    uint32_t tsm_timing_40_init_26mhz;
    uint32_t tsm_timing_40_init_32mhz;
    uint32_t tsm_timing_41_init_26mhz;
    uint32_t tsm_timing_41_init_32mhz;
    uint32_t tsm_timing_51_init;
    uint32_t tsm_timing_52_init_26mhz;
    uint32_t tsm_timing_52_init_32mhz;
    uint32_t tsm_timing_53_init;
    uint32_t tsm_timing_54_init_26mhz;
    uint32_t tsm_timing_54_init_32mhz;
    uint32_t tsm_timing_55_init_26mhz;
    uint32_t tsm_timing_55_init_32mhz;
    uint32_t tsm_timing_56_init_26mhz;
    uint32_t tsm_timing_56_init_32mhz;
    uint32_t tsm_timing_57_init;
    uint32_t tsm_timing_58_init;

    /* XCVR_TX_DIG configs */
    uint32_t tx_ctrl;
    uint32_t tx_data_padding;
    uint32_t tx_dft_pattern;
#if !RADIO_IS_GEN_2P1
    uint32_t rf_dft_bist_1;
    uint32_t rf_dft_bist_2;
#endif /* !RADIO_IS_GEN_2P1 */
} xcvr_common_config_t;

/*! @brief XCVR mode specific configure structure (varies by radio mode) */
typedef struct _xcvr_mode_config
{
    radio_mode_t radio_mode;
    uint32_t scgc5_clock_ena_bits;

    /* XCVR_MISC configs */
    xcvr_masked_init_32_t xcvr_ctrl;  

    /* XCVR_PHY configs */
#if RADIO_IS_GEN_3P0
    uint32_t phy_fsk_pd_cfg0;
    uint32_t phy_fsk_pd_cfg1;
    uint32_t phy_fsk_cfg;
    uint32_t phy_fsk_misc;
    uint32_t phy_fad_ctrl;
#else
    uint32_t phy_pre_ref0_init;
    uint32_t phy_pre_ref1_init;
    uint32_t phy_pre_ref2_init;
    uint32_t phy_cfg1_init;
    uint32_t phy_el_cfg_init; /* EL_WIN_SIZE and EL_INTERVAL are in the data_rate specific configuration */
#endif /* RADIO_IS_GEN_3P0 */

    /* XCVR_RX_DIG configs */
    uint32_t rx_dig_ctrl_init_26mhz; /* NOTE: Common init, mode init, and datarate init will be OR'd together for RX_DIG_CTRL to form complete register initialization */
    uint32_t rx_dig_ctrl_init_32mhz; /* NOTE: Common init, mode init, and datarate init will be OR'd together for RX_DIG_CTRL to form complete register initialization */
    uint32_t agc_ctrl_0_init; /* NOTE:  Common init and mode init will be OR'd together for AGC_CTRL_0 to form complete register initialization */ 
    uint32_t agc_man_agc_idx;
#if RADIO_IS_GEN_3P0
    uint32_t dc_resid_ctrl_init;/* NOTE: This will be OR'd with datarate specific init for DCOC_RESID_CTRL to form complete register initialization */
#endif

    /* XCVR_TSM configs */
#if (RADIO_IS_GEN_2P0 || RADIO_IS_GEN_2P1)
    uint32_t tsm_timing_35_init; /* Only the LSbyte is mode specific */
#endif /* (RADIO_IS_GEN_2P0 || RADIO_IS_GEN_2P1) */

    /* XCVR_TX_DIG configs */
    uint32_t tx_gfsk_ctrl;
    uint32_t tx_gfsk_coeff1_26mhz;
    uint32_t tx_gfsk_coeff2_26mhz;
    uint32_t tx_gfsk_coeff1_32mhz;
    uint32_t tx_gfsk_coeff2_32mhz;
} xcvr_mode_config_t;

/*!
 * @brief XCVR modeXdatarate specific configure structure (varies by radio mode AND data rate)
 * This structure is used to store all of the XCVR settings which are dependent upon both radio mode and data rate. It is used as an overlay
 * on top of the xcvr_mode_config_t structure to supply definitions which are either not in that table or which must be overridden for data rate.
 */
typedef struct _xcvr_mode_datarate_config
{
    radio_mode_t radio_mode;
    data_rate_t data_rate;

    /* XCVR_ANA configs */
    xcvr_masked_init_32_t ana_sy_ctrl2;
    xcvr_masked_init_32_t ana_rx_bba;
    xcvr_masked_init_32_t ana_rx_tza;

    /* XCVR_PHY configs */
#if RADIO_IS_GEN_3P0
    uint32_t phy_fsk_pd_cfg1_mode_datarate_26mhz;
    uint32_t phy_fsk_pd_cfg1_mode_datarate_32mhz;
    uint32_t phy_fsk_cfg_mode_datarate_26mhz;
    uint32_t phy_fsk_cfg_mode_datarate_32mhz;
    uint32_t phy_fsk_misc_mode_datarate_26mhz;
    uint32_t phy_fsk_misc_mode_datarate_32mhz;
    uint32_t agc_ctrl_0_init_26mhz;
    uint32_t agc_ctrl_0_init_32mhz;
    uint32_t agc_ctrl_1_init_26mhz;
    uint32_t agc_ctrl_1_init_32mhz;
    uint32_t agc_ctrl_3_init;
    uint32_t dcoc_ctrl_0_init_26mhz; /* NOTE: This will be OR'd with common init for DCOC_CTRL_0 to form complete register initialization */
    uint32_t dcoc_ctrl_0_init_32mhz; /* NOTE: This will be OR'd with common init for DCOC_CTRL_0 to form complete register initialization */
    uint32_t dcoc_ctrl_1_init;
#else
#if RADIO_IS_GEN_2P1
    uint32_t agc_ctrl_0_init_26mhz;
    uint32_t agc_ctrl_0_init_32mhz;
    uint32_t agc_ctrl_3_init;
    uint32_t dcoc_ctrl_0_init_26mhz;
    uint32_t dcoc_ctrl_0_init_32mhz;
    uint32_t dcoc_ctrl_1_init_26mhz;
    uint32_t dcoc_ctrl_1_init_32mhz;
    uint32_t dc_resid_ctrl_init_26mhz;/* NOTE: This will be OR'd with mode specific init for DCOC_RESID_CTRL to form complete register initialization */
    uint32_t dc_resid_ctrl_init_32mhz;/* NOTE: This will be OR'd with mode specific init for DCOC_RESID_CTRL to form complete register initialization */
#endif /* RADIO_IS_GEN_2P1 */
    uint32_t phy_cfg2_init;
#endif /* RADIO_IS_GEN_3P0 */

    uint32_t agc_ctrl_2_init_26mhz;
    uint32_t agc_ctrl_2_init_32mhz;
    xcvr_rx_chf_coeffs_t rx_chf_coeffs_26mhz; /* 26MHz ext clk */
    xcvr_rx_chf_coeffs_t rx_chf_coeffs_32mhz; /* 32MHz ext clk */
    uint32_t rx_rccal_ctrl_0;
    uint32_t rx_rccal_ctrl_1;
#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
    uint32_t dc_resid_ctrl_init; /* NOTE: This will be OR'd with mode specific init for DCOC_RESID_CTRL to form complete register initialization */
#endif
#if RADIO_IS_GEN_3P0
    uint32_t dcoc_cal_gain_init; /* NOTE: Common init and mode init will be OR'd together for DCOC_CAL_GAIN to form complete register initialization */
    uint32_t dcoc_cal_rcp_init;  /* NOTE: Common init and mode_datarate init will be OR'd together for DCOC_CAL_RCP to form complete register initialization */
    uint32_t dcoc_cal_iir_init_26mhz;
    uint32_t dcoc_cal_iir_init_32mhz;
#endif /* RADIO_IS_GEN_3P0 */

    /* XCVR_TX_DIG configs */
    uint32_t tx_fsk_scale_26mhz; /* Only used by MSK mode, but dependent on datarate */
    uint32_t tx_fsk_scale_32mhz; /* Only used by MSK mode, but dependent on datarate */
} xcvr_mode_datarate_config_t;

/*!
 * @brief XCVR datarate specific configure structure (varies by data rate)
 * This structure is used to store all of the XCVR settings which are dependent upon data rate. It is used as an overlay
 * on top of the xcvr_mode_config_t structure to supply definitions which are either not in that table or which must be overridden for data rate.
 */
typedef struct _xcvr_datarate_config
{
    data_rate_t data_rate;

#if !RADIO_IS_GEN_3P0
    /* XCVR_PHY configs */
    uint32_t phy_el_cfg_init; /* Note: EL_ENABLE  is set in xcvr_mode_config_t settings */
#endif /* !RADIO_IS_GEN_3P0 */

    /* XCVR_RX_DIG configs */
    uint32_t rx_dig_ctrl_init_26mhz; /* NOTE: Common init, mode init, and datarate init will be OR'd together for RX_DIG_CTRL to form complete register initialization */
    uint32_t rx_dig_ctrl_init_32mhz; /* NOTE: Common init, mode init, and datarate init will be OR'd together for RX_DIG_CTRL to form complete register initialization */
#if !RADIO_IS_GEN_3P0
    uint32_t agc_ctrl_1_init_26mhz;
    uint32_t agc_ctrl_1_init_32mhz;
    uint32_t dcoc_ctrl_0_init_26mhz; /* NOTE: This will be OR'd with common init for DCOC_CTRL_0 to form complete register initialization */
    uint32_t dcoc_ctrl_0_init_32mhz; /* NOTE: This will be OR'd with common init for DCOC_CTRL_0 to form complete register initialization */
#endif /* !RADIO_IS_GEN_3P0 */
    uint32_t dcoc_ctrl_1_init_26mhz; /* NOTE: This will be OR'd with common init for DCOC_CTRL_1 to form complete register initialization */
    uint32_t dcoc_ctrl_1_init_32mhz; /* NOTE: This will be OR'd with common init for DCOC_CTRL_1 to form complete register initialization */
    uint32_t dcoc_ctrl_2_init_26mhz;
    uint32_t dcoc_ctrl_2_init_32mhz;
#if !RADIO_IS_GEN_3P0
    uint32_t dcoc_cal_iir_init_26mhz;
    uint32_t dcoc_cal_iir_init_32mhz;
#endif /* !RADIO_IS_GEN_3P0 */
    uint32_t dc_resid_ctrl_26mhz;/* NOTE: This will be OR'd with common init for DCOC_RESID_CTRL to form complete register initialization */
    uint32_t dc_resid_ctrl_32mhz;/* NOTE: This will be OR'd with common init for DCOC_RESID_CTRL to form complete register initialization */
} xcvr_datarate_config_t;

/*!
 * @brief LPUART callback function type
 *
 * The panic callback function is defined by system if system need to be informed of XCVR fatal errors.
 * refer to #XCVR_RegisterPanicCb
 */
typedef void (*panic_fptr)(uint32_t panic_id, uint32_t location, uint32_t extra1, uint32_t extra2);

/* Make available const structures from config files */
extern const xcvr_common_config_t xcvr_common_config;
extern const xcvr_mode_config_t zgbe_mode_config;
extern const xcvr_mode_config_t ble_mode_config;
extern const xcvr_mode_config_t ant_mode_config;
extern const xcvr_mode_config_t gfsk_bt_0p5_h_0p5_mode_config;
extern const xcvr_mode_config_t gfsk_bt_0p5_h_0p7_mode_config;
extern const xcvr_mode_config_t gfsk_bt_0p5_h_0p32_mode_config;
extern const xcvr_mode_config_t gfsk_bt_0p5_h_1p0_mode_config;
extern const xcvr_mode_config_t gfsk_bt_0p3_h_0p5_mode_config;
extern const xcvr_mode_config_t gfsk_bt_0p7_h_0p5_mode_config;
extern const xcvr_mode_config_t msk_mode_config;

#if RADIO_IS_GEN_3P0
extern const xcvr_datarate_config_t xcvr_2mbps_config;
#endif /* RADIO_IS_GEN_3P0 */
extern const xcvr_datarate_config_t xcvr_1mbps_config;
extern const xcvr_datarate_config_t xcvr_500kbps_config;
extern const xcvr_datarate_config_t xcvr_250kbps_config;
extern const xcvr_datarate_config_t xcvr_802_15_4_250kbps_config; /* Custom datarate settings for 802.15.4 since it is 2MChips/sec */

#if RADIO_IS_GEN_3P0
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p5_2mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p32_2mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p3_h_0p5_2mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p7_h_0p5_2mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_MSK_2mbps_config;
#endif /* RADIO_IS_GEN_3P0 */
extern const xcvr_mode_datarate_config_t xcvr_BLE_1mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_ZIGBEE_250kbps_config;
#if XCVR_ANT_ENABLED
extern const xcvr_mode_datarate_config_t xcvr_ANT_1mbps_config;
#endif /* XCVR_ANT_ENABLED */
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p5_1mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p5_500kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p5_250kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p32_1mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p32_500kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p32_250kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p7_1mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p7_500kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_0p7_250kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_1p0_1mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_1p0_500kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p5_h_1p0_250kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p3_h_0p5_1mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p3_h_0p5_500kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p3_h_0p5_250kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p7_h_0p5_1mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p7_h_0p5_500kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p7_h_0p5_250kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_MSK_1mbps_config;
extern const xcvr_mode_datarate_config_t xcvr_MSK_500kbps_config;
extern const xcvr_mode_datarate_config_t xcvr_MSK_250kbps_config;

/* Expose TX_RX_ON_DELAY and SYNTH_DELAY for BLE LL to access for setting registers appropriately */
extern uint16_t tx_rx_on_delay;
extern uint16_t tx_rx_synth_delay;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name XCVR functional Operation
 * @{
 */

/*!
 * @brief Initializes an XCVR instance.
 *
 * This function initializes the XCVR module according to the radio_mode and data_rate settings. This the only function call required to 
 * start up the XCVR in most situations. It includes a DCOC DAC trim step that is left out in the ::XCVR_InitNoDacTrim() version.
 *
 * @param radio_mode The radio mode for which the XCVR should be configured.
 * @param data_rate The data rate for which the XCVR should be configured. Only matters when GFSK/MSK radio_mode is selected.
 * @note This function encompasses the ::XCVRGetDefafultConfig() and ::XCVR_Configure() functions.
 */
xcvrStatus_t XCVR_Init(radio_mode_t radio_mode, data_rate_t data_rate);

/*!
 * @brief Initializes an XCVR instance.
 *
 * This function initializes the XCVR module according to the radio_mode and data_rate settings. This function call must be combined with
 * the ::XCVR_SetDcocDacTrims() function to properly start up the XCVR. This allows a stored set of DCOC DAC trims to be used rather
 * than running the trim process during every startup. 
 *
 * @param radio_mode The radio mode for which the XCVR should be configured.
 * @param data_rate The data rate for which the XCVR should be configured. Only matters when GFSK/MSK radio_mode is selected.
 * @note This function encompasses the ::XCVRGetDefafultConfig() and ::XCVR_Configure() functions but omits the DCOC DAC trim.
 */
xcvrStatus_t XCVR_InitNoDacTrim(radio_mode_t radio_mode, data_rate_t data_rate);


/*!
 * @brief Deinitializes an XCVR instance.
 *
 * This function gate the XCVR module clock and set all register value to reset value.
 *
 */
void XCVR_Deinit(void);

/*!
 * @brief Initializes XCVR configure structure.
 *
 * This function updates pointers to the XCVR configure structures with default values. 
 * The configurations are divided into a common structure, a set of radio mode specific 
 * structures (one per radio_mode), a set of mode&datarate specific structures (for each mode at
 * each datarate), and a set of data rate specific structures.
 * The pointers provided by this routine point to const structures which can be
 * copied to variable structures if changes to settings are required.
 *
 * @param radio_mode [in] The radio mode for which the configuration structures are requested.
 * @param data_rate [in] The data rate for which the configuration structures are requested.
 * @param com_config [in,out] Pointer to a pointer to the common configuration settings structure.
 * @param mode_config  [in,out] Pointer to a pointer to the mode specific configuration settings structure.
 * @param mode_datarate_config  [in,out] Pointer to a pointer to the modeXdata rate specific configuration settings structure.
 * @param datarate_config  [in,out] Pointer to a pointer to the data rate specific configuration settings structure.
 * @return 0 success, others failure
 * @see XCVR_Configure
 */
xcvrStatus_t XCVR_GetDefaultConfig(radio_mode_t radio_mode, 
                                   data_rate_t data_rate, 
                                   const xcvr_common_config_t ** com_config, 
                                   const xcvr_mode_config_t ** mode_config, 
                                   const xcvr_mode_datarate_config_t ** mode_datarate_config, 
                                   const xcvr_datarate_config_t ** datarate_config);

/*!
 * @brief  Initializes an XCVR instance.
 *
 * This function initializes the XCVR module with user-defined settings.
 *
 * @param com_config Pointer to the common configuration settings structure.
 * @param mode_config Pointer to the mode specific configuration settings structure.
 * @param mode_datarate_config Pointer to a pointer to the modeXdata rate specific configuration settings structure.
 * @param datarate_config Pointer to a pointer to the data rate specific configuration settings structure.
 * @param ext_clk indicates the external clock setting, 32MHz or 26MHz.
 * @param first_init indicates whether the call is to initialize (== 1) or the call is to perform a mode change (== 0)
 * @return 0 succeed, others failed
 */
xcvrStatus_t XCVR_Configure(const xcvr_common_config_t *com_config, 
                            const xcvr_mode_config_t *mode_config, 
                            const xcvr_mode_datarate_config_t *mode_datarate_config, 
                            const xcvr_datarate_config_t *datarate_config, 
                            XCVR_INIT_MODE_CHG_T first_init);

/*!
 * @brief Startup the radio.
 *
 * This function brings the radio into RUN mode and enables radio clocks..
 *
 * @return status of the mode change.
 */
xcvrStatus_t XCVR_RadioStartup(void);

/*!
 * @brief Performs all register setup for XCVR initialization or mode change.
 *
 * This function sets up all of the registers in the XCVR module with user-defined settings.
 *
 * @param com_config Pointer to the common configuration settings structure.
 * @param mode_config Pointer to the mode specific configuration settings structure.
 * @param mode_datarate_config Pointer to a pointer to the modeXdata rate specific configuration settings structure.
 * @param datarate_config Pointer to a pointer to the data rate specific configuration settings structure.
 * @param ext_clk indicates the external clock setting, 32MHz or 26MHz.
 * @param first_init indicates whether the call is to initialize (== 1) or the call is to perform a mode change (== 0)
 * @return 0 succeed, others failed
 */
xcvrStatus_t XCVR_RadioRegSetup(const xcvr_common_config_t *com_config, 
                                const xcvr_mode_config_t *mode_config, 
                                const xcvr_mode_datarate_config_t *mode_datarate_config, 
                                const xcvr_datarate_config_t *datarate_config, 
                                XCVR_INIT_MODE_CHG_T first_init);


/*!
 * @brief Set XCVR register to reset value.
 *
 * This function set XCVR register to the reset value.
 *
 */
void XCVR_Reset(void);

/*!
 * @brief Change the operating mode of the radio.
 *
 * This function changes the XCVR to a new radio operating mode.
 *
 * @param new_radio_mode The radio mode for which the XCVR should be configured.
 * @param new_data_rate The data rate for which the XCVR should be configured. Only matters when GFSK/MSK radio_mode is selected.
 * @return status of the mode change.
 */
 xcvrStatus_t XCVR_ChangeMode (radio_mode_t new_radio_mode, data_rate_t new_data_rate);

/*!
 * @brief Enable Narrowband RSSI measurement.
 *
 * This function enables the narrowband RSSI measurement
 *
 * @param IIRnbEnable true causes the NB RSSI to be enabled, 0 == disabled.
 */
void XCVR_EnaNBRSSIMeas(uint8_t IIRnbEnable);

/*!
 * @brief Set an arbitrary frequency for RX and TX for the radio.
 *
 * This function sets the radio frequency used for RX and RX..
 *
 * @param freq target frequency setting in Hz.
 * @param refOsc reference oscillator setting in Hz.
 * @return status of the frequency change.
 * @details
 */
 xcvrStatus_t XCVR_OverrideFrequency ( uint32_t freq, uint32_t refOsc );

/*!
 * @brief Register a callback from upper layers.
 *
 * This function registers a callback from the upper layers for the radio to call in case of fatal errors.
 *
 * @param fptr The function pointer to a panic callback.
 */
void XCVR_RegisterPanicCb ( panic_fptr fptr ); /* allow upper layers to provide PANIC callback */

/*!
 * @brief Panic function for XCVR related fatal errors.
 *
 * This function executes a callback to the upper layers for the radio to call in case of fatal errors. It enters an endless loop if no 
 * callback function has been registered.
 *
 * @param panic_id The ID for the failure.
 * @param panic_address The address of the routine calling the panic.
 */
void XcvrPanic(XCVR_PANIC_ID_T panic_id, uint32_t panic_address);

/*!
 * @brief Read the health status of the XCVR to detect errors.
 *
 * This function enables the upper layers to request the current radio health.
 *
 * @return The health status of the radio..
 */
healthStatus_t XCVR_HealthCheck ( void ); /* allow upper layers to poll the radio health */

/*!
 * @brief Control FAD and LPPS features.
 *
 * This function controls the Fast Antenna Diversity (FAD) and Low Power Preamble Search.
 *
 * @param fptr control the FAD and LPPS settings.
 *
 */
 void XCVR_FadLppsControl(FAD_LPPS_CTRL_T control);

/*!
 * @brief Change the mapping of the radio IRQs.
 *
 * This function changes the mapping of the radio LL IRQ signals to the 2.4G Radio INT0 and 2.4G Radio INT1 lines.
 *
 * @param irq0_mapping  the LL which should be mapped to the INT0 line.
 * @param irq1_mapping  the LL which should be mapped to the INT1 line.
 * @return status of the mapping request.
 * @ note The radio_mode_t parameters map to ::link_layer_t selections for the LL which is connected to the INT line.
 * @warning
 *  The same LL must NOT be mapped to both INT lines.
 */
 xcvrStatus_t XCVR_SetIRQMapping(radio_mode_t irq0_mapping, radio_mode_t irq1_mapping);

#if RADIO_IS_GEN_3P0
/*!
 * @brief Sets the network address used by the PHY during BLE Bit Streaming Mode.
 *
 * This function programs the register in the PHY which contains the network address used during BSM.
 *
 * @param bsm_ntw_address  the address to be used during BSM.
 * @ note This routine does NOT enable BSM.
 */
void XCVR_SetBSM_NTW_Address(uint32_t bsm_ntw_address);

/*!
 * @brief Reads the currently programmed network address used by the PHY during BLE Bit Streaming Mode.
 *
 * This function reads the register in the PHY which contains the network address used during BSM.
 *
 * @return bsm_ntw_address  the address to be used during BSM.
 * @ note This routine does NOT enable BSM.
 */
uint32_t XCVR_GetBSM_NTW_Address(void);
#endif /* RADIO_IS_GEN_3P0 */

/*!
 * @brief Get the mapping of the one of the radio IRQs.
 *
 * This function reads the setting for the mapping of one of the radio LL IRQ signals to the 2.4G Radio INT0 and 2.4G Radio INT1 lines.
 *
 * @param int_num the number, 0 or 1, of the INT line to fetched.
 * @return the mapping setting of the specified line.
 * @note Any value passed into this routine other than 0 will be treated as a 1.
 */
 link_layer_t XCVR_GetIRQMapping(uint8_t int_num);

/*!
 * @brief Get the current configuration of the XCVR.
 *
 * This function fetches the current configuration (radio mode and radio data rate) of the XCVR to allow LL to properly config data rates, etc
 *
 * @param curr_config pointer to a structure to be updated with the current mode and data rate.
 * @return the status of the request, success or invalid parameter (null pointer).
 * @note This API will return meaningless results if called before the radio is initialized...
 */
xcvrStatus_t XCVR_GetCurrentConfig(xcvr_currConfig_t * curr_config);

/*******************************************************************************
 * Customer level trim functions
 ******************************************************************************/
/*!
 * @brief Controls setting the XTAL trim value..
 *
 * This function enables the upper layers set a crystal trim compensation facor
 *
 * @param xtalTrim the trim value to apply to the XTAL trimming register. Only the 7 LSB are valid, setting the 8th bit returns an error.
 * @return The health status of the radio..
 */
xcvrStatus_t XCVR_SetXtalTrim(uint8_t xtalTrim);

/*!
 * @brief Controls getting the XTAL trim value..
 *
 * This function enables the upper layers to read the current XTAL compensation factors.
 * The returned value is in the range 0..127 (7 bits).
 *
 * @return The XTAL trim compensation factors..
 */ 
uint8_t  XCVR_GetXtalTrim(void);

/*!
 * @brief  Controls setting the RSSI adjustment..
 *
 * This function enables the upper layers to set an RSSI adjustment value.
 *
 * @param adj the adjustment value to apply to the RSSI adjustment register. The value must be a signed 8-bit value, in 1/4 dBm step.
 * @return The health status of the radio..
 */
xcvrStatus_t XCVR_SetRssiAdjustment(int8_t adj);

/*!
 * @brief  Controls getting the RSSI adjustment..
 *
 * This function enables the upper layers to read the current XCVR RSSI adjustment value.
 * The returned value is a signed 8-bit value, in 1/4 dBm step.
 *
 * @return The RSSI adjustment value..
 */ 
int8_t  XCVR_GetRssiAdjustment(void);

/*!
 * @brief  Performs the DCOC DAC trim process.
 *
 * This function enables the upper layers to perform the DCOC DAC trim process.
 * The routine write the contents of memory pointed to by the input pointer. The contents are invalid in case of error return values.
 *
 * @return The status of the routine.
 */ 
xcvrStatus_t XCVR_CalculateDcocDacTrims(xcvr_DcocDacTrim_t * trim_values);

/*!
 * @brief  Controls setting the DCOC DAC trim register contents.
 *
 * This function enables the upper layers to write the current DCOC DAC trim register contents from a structure for usage in initialization of the radio.
 * The routine reads the contents of memory pointed to by the input pointer.
 *
 * @return The status of the routine.
 */ 
xcvrStatus_t XCVR_SetDcocDacTrims(const xcvr_DcocDacTrim_t * trim_values);

/*!
 * @brief  Controls getting the DCOC DAC trim register contents.
 *
 * This function enables the upper layers to read the current DCOC DAC trim register contents into a structure for storage.
 * The routine modifies the contents of memory pointed to by the input pointer.
 *
 * @return The status of the routine.
 */ 
xcvrStatus_t XCVR_GetDcocDacTrims(xcvr_DcocDacTrim_t * trim_values);

/*!
 * @brief Controls setting the PLL to a particular channel.
 *
 * This function enables setting the radio channel for TX and RX.
 *
 * @param channel the channel number to set
 * @param useMappedChannel when true, channel is assumed to be from the protocol specific channel map. when false, channel is assumed to be from the 128 general channel list..
 * @return The status of the channel over-ride.
 */ 
xcvrStatus_t XCVR_OverrideChannel ( uint8_t channel, uint8_t useMappedChannel );

/*!
 * @brief Reads the current frequency for RX and TX for the radio.
 *
 * This function reads the radio frequency used for RX and RX..
 *
 * @return Current radio frequency setting.
 */
uint32_t XCVR_GetFreq ( void );

/*!
 * @brief Force receiver warmup.
 *
 * This function forces the initiation of a receiver warmup sequence.
 *
 */
void XCVR_ForceRxWu ( void );

/*!
 * @brief Force receiver warmdown.
 *
 * This function forces the initiation of a receiver warmdown sequence.
 *
 */
 void XCVR_ForceRxWd ( void );

/*!
 * @brief Force transmitter warmup.
 *
 * This function forces the initiation of a transmit warmup sequence.
 *
 */
 void XCVR_ForceTxWu ( void );

/*!
 * @brief Force transmitter warmdown.
 *
 * This function forces the initiation of a transmit warmdown sequence.
 *
 */
void XCVR_ForceTxWd ( void );

/*!
 * @brief Starts transmit with a TX pattern register data sequence.
 *
 * This function starts transmitting using the DFT pattern register mode.
 *
 * @param channel_num -  the protocol specific channel to transmit on. Valid values are defined in the CHANNEL_NUM register documentation.
 * @param radio_mode The radio mode for which the XCVR should be configured.
 * @param data_rate The data rate for which the XCVR should be configured. Only matters when GFSK/MSK radio_mode is selected.
 * @param tx_pattern -  the data pattern to transmit on.
 * @return The status of the pattern reg transmit.
 * @note The XCVR_DftTxOff() function must be called to turn off TX and revert all settings. This routine calls XCVR_ChangeMode() with the desired radio mode
 *   and data rate. 
 */ 
xcvrStatus_t XCVR_DftTxPatternReg(uint16_t channel_num, radio_mode_t radio_mode, data_rate_t data_rate, uint32_t tx_pattern);

/*!
 * @brief Starts transmit with a TX LFSR register data sequence.
 *
 * This function starts transmitting using the DFT LFSR register mode.
 *
 * @param channel_num -  the protocol specific channel to transmit on. Valid values are defined in the CHANNEL_NUM register documentation.
 * @param radio_mode The radio mode for which the XCVR should be configured.
 * @param data_rate The data rate for which the XCVR should be configured. Only matters when GFSK/MSK radio_mode is selected.
 * @param lfsr_length -  the length of the LFSR sequence to use.
 * @return The status of the LFSR reg transmit.
 * @note The XCVR_DftTxOff() function must be called to turn off TX and revert all settings. This routine calls XCVR_ChangeMode() with the desired radio mode
 *   and data rate. 
 */ 
xcvrStatus_t XCVR_DftTxLfsrReg(uint16_t channel_num, radio_mode_t radio_mode, data_rate_t data_rate, uint8_t lfsr_length);

/*!
 * @brief Controls clearing all TX DFT settings.
 *
 * This function reverts all TX DFT settings from the test modes to normal operating mode.
 *
 */ 
void XCVR_DftTxOff(void);

/*!
 * @brief Controls setting the PA power level.
 *
 * This function enables setting the PA power level to a specific setting, overriding any link layer settings.
 *
 * @param pa_power -  the power level to set. Valid values are 0, 1, and even values from 2 to 0x3E, inclusive.
 * @return The status of the PA power over-ride.
 */ 
xcvrStatus_t XCVR_ForcePAPower(uint8_t pa_power);

/*!
 * @brief Starts CW TX.
 *
 * This function starts transmitting CW (no modulation).
 *
 * @param rf_channel_freq -  the RF channel to transmit on. Valid values are integer values from 2360 to 2487MHz, inclusive.
 * @param protocol -  the protocol setting to use, valid settings are 6 (GFSK) and 7 (FSK).
 * @return The status of the CW transmit.
 */ 
xcvrStatus_t XCVR_DftTxCW(uint16_t rf_channel_freq, uint8_t  protocol);

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
/*!
 * @brief Function to set AGC to manual and DCOC to auto.
 *
 * This function sets full manual control of AGC and allows DCOC automatic processing to operate normally.
 *
 * @param man_agc_idx the desired manual AGC index to use (26 to 0).
 * @note
 *    This function disables AGC as a side effect. 
 */
 xcvrStatus_t XCVR_SetManAgc(uint8_t man_agc_idx);

/*!
 * @brief Function to clear AGC manual operation.
 *
 * This function clears AGC manual mode and allows AGC and DCOC automatic processing to operate normally.
 *
 * @note
 *    This function leaves AGC enabled!
 */ 
void XCVR_ClearManAgc(void);
#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

xcvrStatus_t XCVR_CoexistenceInit(void);
xcvrStatus_t XCVR_CoexistenceSetPriority(XCVR_COEX_PRIORITY_T rxPriority, XCVR_COEX_PRIORITY_T txPriority);
xcvrStatus_t XCVR_CoexistenceSaveRestoreTimings(uint8_t saveTimings);

#if (((RADIO_IS_GEN_3P0) || (RADIO_IS_GEN_2P1)) && defined(PA_FEM_SUPPORTED))
/*!
 * @brief Function to initialize support for external PA and FEM module control.
 *
 * This function initializes the PA and FEM control register and timing registers before TX or RX operations using those modules.
 *
 * @param pa_fem_settings_ptr the pointer to a settings structure for PA/FEM initialization.
 * 
 */
xcvrStatus_t XCVR_ExternalFadPaFemInit(xcvr_pa_fem_config_t * pa_fem_settings_ptr);

/*!
 * @brief Function to de-initialize support for external PA and FEM module control.
 *
 * This function resets the PA and FEM control register and timing registers to allow them to be used for other purposes if required.
 *
 */
xcvrStatus_t XCVR_ExternalFadPaFemDeInit(void);

/*!
 * @brief Function to over-ride the antenna selection when FAD is active .
 *
 * This function enables the over-ride of the FAD antenna selection and sets the software requested selection.
 *
 * @param antenna_sel the software antenna selection.
 * 
 */
void XCVR_ExtAntOvrd(XCVR_FAD_OVRD_ANT_A_B_SEL_MODE_T antenna_sel);

/*!
 * @brief Function to release any over-ride the antenna selection and return the antenna to  FAD control .
 *
 * This function enables the over-ride of the FAD antenna selection and sets the software requested selection.
 *
 */
void XCVR_ExtAntRelease(void);

/*!
 * @brief Function to initialize support for external PA and FEM module control using Coexistence pins.
 *
 * This function initializes the PA and FEM control register and timing registers before TX or RX operations using those modules. It
 * sets up to use one or both of the RF_STATUS and RF_PRIORITY pins for PA/FEM control instead of coexistence.
 *
 * @param pa_fem_settings_ptr The pointer to a settings structure for PA/FEM initialization.
 * @param rf_status_func t The function to be applies on the RF_STATUS pin.
 * @param rf_priority_func The function to be applies on the RF_PRIORITY pin.
 * @note
 *  This function is intended to be used when ::XCVR_ExternalFadPaFemInit() cannot be used (if preservation of debugger connection
 *  is required. 
 *  This function does NOT perform any pin muxing settings, that must be performed by a higher layer of software.
 * 
 */
xcvrStatus_t XCVR_FadPaFemOnCoexInit(xcvr_pa_fem_config_t * test_settings, tx_rx_coex_pin_func_t rf_status_func, tx_rx_coex_pin_func_t rf_priority_func);

/*!
 * @brief Function to de-initialize support for external PA and FEM module control over coexistence pins.
 *
 * This function resets the PA and FEM control register and timing registers to allow them to be used for other purposes if required.
 *
 */
void XCVR_FadPaFemOnCoexIDeInit(void);


#endif /* (((RADIO_IS_GEN_3P0) || (RADIO_IS_GEN_2P1)) &&  PA_FEM_SUPPORTED */

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FSL_XCVR_H */
