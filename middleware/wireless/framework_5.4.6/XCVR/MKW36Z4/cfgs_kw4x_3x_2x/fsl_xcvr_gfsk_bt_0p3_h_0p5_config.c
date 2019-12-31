/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "fsl_xcvr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* MODE only configuration */
const xcvr_mode_config_t gfsk_bt_0p3_h_0p5_mode_config =
{
    .radio_mode = GFSK_BT_0p3_h_0p5,
    .scgc5_clock_ena_bits = SIM_SCGC5_PHYDIG_MASK | SIM_SCGC5_GEN_FSK_MASK,

    /* XCVR_MISC configs */
    .xcvr_ctrl.mask = XCVR_CTRL_XCVR_CTRL_PROTOCOL_MASK |
                      XCVR_CTRL_XCVR_CTRL_TGT_PWR_SRC_MASK |
                      XCVR_CTRL_XCVR_CTRL_DEMOD_SEL_MASK,
    .xcvr_ctrl.init = XCVR_CTRL_XCVR_CTRL_PROTOCOL(8) |
                      XCVR_CTRL_XCVR_CTRL_TGT_PWR_SRC(7) |
                      XCVR_CTRL_XCVR_CTRL_DEMOD_SEL(1),

    /* XCVR_PHY configs */
    .phy_pre_ref0_init = 0x7BCDEB39,
    .phy_pre_ref1_init = 0xCEF7DEF7,
    .phy_pre_ref2_init = 0x0000CEB7,

    .phy_cfg1_init = XCVR_PHY_CFG1_AA_PLAYBACK(1) |
                     XCVR_PHY_CFG1_AA_OUTPUT_SEL(1) |
                     XCVR_PHY_CFG1_FSK_BIT_INVERT(0) |
                     XCVR_PHY_CFG1_BSM_EN_BLE(0) |
                     XCVR_PHY_CFG1_DEMOD_CLK_MODE(0) |
#if !RADIO_IS_GEN_2P1
                     XCVR_PHY_CFG1_CTS_THRESH(0xda) |
#else
                     XCVR_PHY_CFG1_CTS_THRESH(0xE4) |
#endif /* !RADIO_IS_GEN_2P1 */
                     XCVR_PHY_CFG1_FSK_FTS_TIMEOUT(2),

    .phy_el_cfg_init = XCVR_PHY_EL_CFG_EL_ENABLE(1)
#if !RADIO_IS_GEN_2P1
                     | XCVR_PHY_EL_CFG_EL_ZB_ENABLE(0)
#endif /* !RADIO_IS_GEN_2P1 */
    ,

    /* XCVR_RX_DIG configs */
    .rx_dig_ctrl_init_26mhz = XCVR_RX_DIG_RX_DIG_CTRL_RX_FSK_ZB_SEL(0) | /* Depends on protocol */
                              XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN(1) | /* Depends on protocol */
                              XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_RATE(0),

    .rx_dig_ctrl_init_32mhz = XCVR_RX_DIG_RX_DIG_CTRL_RX_FSK_ZB_SEL(0) | /* Depends on protocol */
                              XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN(1), /* Depends on protocol */

#if RADIO_IS_GEN_2P0 
    .agc_ctrl_0_init = XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0xFF),
#else
    .agc_ctrl_0_init = 0,  /* the AGC_DOWN_RSSI_THRESH is moved to mode/datarate for only this radio mode */ 
#endif /* RADIO_IS_GEN_2P0 */
    
    /* XCVR_TSM configs */
#if (DATA_PADDING_EN)
    .tsm_timing_35_init = B0(TX_DIG_EN_ASSERT+TX_DIG_EN_TX_HI_ADJ),
#else
    .tsm_timing_35_init = B0(TX_DIG_EN_ASSERT),
#endif /* (DATA_PADDING_EN) */

    /* XCVR_TX_DIG configs */
    .tx_gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_MULTIPLY_TABLE_MANUAL(0x4000) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MI(1) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MLD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_FLD(1) | /* Use GFSK Manual Filter Coeffs */
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MOD_INDEX_SCALING(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_OVRD_EN(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_0_OVRD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_1_OVRD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_2_OVRD(0),
    .tx_gfsk_coeff1_26mhz = (127U)<<0 | /* coeff 2/13 */
                            (169U)<<7 | /* coeff 6/9 */
                            (127U)<<16 | /* coef 3/12 */
                            (169U)<<23, /* coeff 7/8 */
    .tx_gfsk_coeff2_26mhz = (31U)<<0 | /* coeff 0/15 */
                            (63U)<<8 | /* coeff 1/14 */
                            (169U)<<16 | /* coeff 4/11 */
                            (169U)<<24, /* coeff 5/10 */
    .tx_gfsk_coeff1_32mhz = (70U) << 0 | /* coeff 2/13 */
                            (216U) << 7 | /* coeff 6/9 */
                            (105U) << 16 | /* coef 3/12 */
                            (233U) << 23, /* coeff 7/8 */
    .tx_gfsk_coeff2_32mhz = (25U) << 0 | /* coeff 0/15 */
                            (44U) << 8 | /* coeff 1/14 */
                            (145U) << 16 | /* coeff 4/11 */
                            (184U) << 24, /* coeff 5/10 */
};

/* MODE & DATA RATE combined configuration */
const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p3_h_0p5_1mbps_config =
{
    .radio_mode = GFSK_BT_0p3_h_0p5,
    .data_rate = DR_1MBPS,

    .ana_sy_ctrl2.mask = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_MASK,
    .ana_sy_ctrl2.init = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0), /* VCO KVM */
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(4) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(4), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(4), /*TZA_BW_SEL */ 

    .phy_cfg2_init = XCVR_PHY_CFG2_PHY_FIFO_PRECHG(8) |
                     XCVR_PHY_CFG2_X2_DEMOD_GAIN(0xA) ,

    /* AGC configs */
#if RADIO_IS_GEN_2P1
    .agc_ctrl_0_init_26mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05),
    .agc_ctrl_0_init_32mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05),               
#endif
                        
    .agc_ctrl_2_init_26mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(11) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(16),
#endif
    .agc_ctrl_2_init_32mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(12) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(16),
#endif

#if RADIO_IS_GEN_2P1
    .agc_ctrl_3_init = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(21),
    .dcoc_ctrl_0_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(4),
    .dcoc_ctrl_0_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(4),
    .dcoc_ctrl_1_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dcoc_ctrl_1_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dc_resid_ctrl_init_26mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(108) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
    .dc_resid_ctrl_init_32mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(108) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
#endif
    
    /* All constant values are represented as 16 bits, register writes will remove unused bits */
#if RADIO_IS_GEN_2P0
    /* 26MHz Channel Filter */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0 = 0xFFFF,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1 = 0xFFFD,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2 = 0xFFF9,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3 = 0xFFF4,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4 = 0xFFF2,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5 = 0xFFF5,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6 = 0x0000,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7 = 0x0011,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8 = 0x0028,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9 = 0x0041,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0055,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0061,

    /* 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0 = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1 = 0xFFFF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2 = 0xFFFA,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3 = 0xFFF4,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4 = 0xFFF0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5 = 0xFFF0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6 = 0xFFF9,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7 = 0x000B,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8 = 0x0025,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9 = 0x0043,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x005C,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x006A,
#else
    /* 26MHz Channel Filter */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0xFFFB,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0xFFF5,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFF4,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFF7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFFE,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0x000A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0x001A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x002C,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x003E,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x004C,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0053,

    /* 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0xFFFF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0xFFF9,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0xFFF0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFEB,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFEF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0x0000,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0x001D,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0042,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0064,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0078,
#endif /* RADIO_IS_GEN_2P0 */

    .rx_rccal_ctrl_0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(0) ,
    .rx_rccal_ctrl_1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(0) ,

    .tx_fsk_scale_26mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1627) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x09d9),
    .tx_fsk_scale_32mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1800) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x0800),
};

const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p3_h_0p5_500kbps_config =
{
    .radio_mode = GFSK_BT_0p3_h_0p5,
    .data_rate = DR_500KBPS,

    .ana_sy_ctrl2.mask = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_MASK,
    .ana_sy_ctrl2.init = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0), /* VCO KVM */
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(5) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(5), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(5), /* TZA_BW_SEL */ 

    .phy_cfg2_init = XCVR_PHY_CFG2_PHY_FIFO_PRECHG(8) |
                     XCVR_PHY_CFG2_X2_DEMOD_GAIN(0x8) ,

    /* AGC configs */
#if RADIO_IS_GEN_2P1
    .agc_ctrl_0_init_26mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x5),
    .agc_ctrl_0_init_32mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x40),               
#endif

    .agc_ctrl_2_init_26mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(15) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(24),
#endif
    .agc_ctrl_2_init_32mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(18) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(24),
#endif

#if RADIO_IS_GEN_2P1
    .agc_ctrl_3_init = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(21),
    .dcoc_ctrl_0_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(12),
    .dcoc_ctrl_0_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(14),
    .dcoc_ctrl_1_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dcoc_ctrl_1_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dc_resid_ctrl_init_26mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(108) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
    .dc_resid_ctrl_init_32mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(108) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
#endif
    
    /* All constant values are represented as 16 bits, register writes will remove unused bits */
#if RADIO_IS_GEN_2P0
    /* 26MHz Channel Filter */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0 = 0x0001,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1 = 0x0000,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2 = 0xFFFC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3 = 0xFFF7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4 = 0xFFF3,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5 = 0xFFF2,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6 = 0xFFF9,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7 = 0x000A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8 = 0x0023,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9 = 0x0040,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0059,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0068,

    /* 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0 = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1 = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2 = 0xFFFF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3 = 0xFFFA,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4 = 0xFFF3,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5 = 0xFFEF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6 = 0xFFF3,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7 = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8 = 0x001D,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9 = 0x003F,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x005F,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0072,
#else
    /* 26MHz Channel Filter */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0xFFFC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0xFFF9,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0xFFF7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFF6,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFFF,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0x000A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0x001A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x002B,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x003C,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x004A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0051,

    /* 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0002,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0x0000,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0xFFFC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0xFFF3,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFED,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFEE,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFFD,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0x0019,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x003F,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0063,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0079,
#endif /* RADIO_IS_GEN_2P0 */

    .rx_rccal_ctrl_0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(1) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(1) ,
    .rx_rccal_ctrl_1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(1) ,

    .tx_fsk_scale_26mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1627) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x09d9),
    .tx_fsk_scale_32mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1800) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x0800),
};

const xcvr_mode_datarate_config_t xcvr_GFSK_BT_0p3_h_0p5_250kbps_config =
{
    .radio_mode = GFSK_BT_0p3_h_0p5,
    .data_rate = DR_250KBPS,
    
    .ana_sy_ctrl2.mask = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_MASK,
    .ana_sy_ctrl2.init = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0), /* VCO KVM */
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(5) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(5), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(5), /* TZA_BW_SEL */ 
             
    .phy_cfg2_init = XCVR_PHY_CFG2_PHY_FIFO_PRECHG(8) |
                     XCVR_PHY_CFG2_X2_DEMOD_GAIN(0x6) ,

    /* AGC configs */
#if RADIO_IS_GEN_2P1
    .agc_ctrl_0_init_26mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05),
    .agc_ctrl_0_init_32mhz = XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(1) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xF0) |
                        XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x40),               
#endif

    .agc_ctrl_2_init_26mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(18) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |       
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(2) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(48),
#endif /* RADIO_IS_GEN_2P0 */
    .agc_ctrl_2_init_32mhz = XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(22) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(6) |
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(3) |
#if RADIO_IS_GEN_2P0
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(7) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(5),
#else
                             XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(5) |
                             XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(48),
#endif /* RADIO_IS_GEN_2P0 */

#if RADIO_IS_GEN_2P1
    .agc_ctrl_3_init = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(21),
    .dcoc_ctrl_0_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(15),
    .dcoc_ctrl_0_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(18),
    .dcoc_ctrl_1_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dcoc_ctrl_1_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dc_resid_ctrl_init_26mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(108) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
    .dc_resid_ctrl_init_32mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(108) |
                            XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
#endif
    
    /* All constant values are represented as 16 bits, register writes will remove unused bits */
#if RADIO_IS_GEN_2P0
    /* 26MHz Channel Filter */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0x0001,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0x0003,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0x0003,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFFF,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFF7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFEE,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0xFFEC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0xFFF7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x0014,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x003C,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0064,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x007D,

    /* 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0x0005,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0xFFFC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFF0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFE8,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFEF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0x000B,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0038,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0068,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0086,
#else
    /* 26MHz Channel Filter */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0xFFFF,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0xFFFC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0xFFF9,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFF6,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFF6,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFFB,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0x0005,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0x0015,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x0029,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x003D,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x004D,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0057,

    /* 32MHz Channel Filter */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0001,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0x0005,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0xFFFC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFF0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFE9,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFF0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0x000C,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0038,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0067,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0084,
#endif /* RADIO_IS_GEN_2P0 */

    .rx_rccal_ctrl_0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(1) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(1) ,
    .rx_rccal_ctrl_1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(31) |
                       XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(1),

    .tx_fsk_scale_26mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1627) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x09d9),
    .tx_fsk_scale_32mhz = XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_0(0x1800) | XCVR_TX_DIG_FSK_SCALE_FSK_MODULATION_SCALE_1(0x0800),
};

