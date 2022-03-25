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
const xcvr_mode_config_t msk_mode_config =
{
    .radio_mode = MSK,
    .scgc5_clock_ena_bits = SIM_SCGC5_PHYDIG_MASK | SIM_SCGC5_GEN_FSK_MASK,

    /* XCVR_MISC configs */
    .xcvr_ctrl.mask = XCVR_CTRL_XCVR_CTRL_PROTOCOL_MASK |
                      XCVR_CTRL_XCVR_CTRL_TGT_PWR_SRC_MASK |
                      XCVR_CTRL_XCVR_CTRL_DEMOD_SEL_MASK,
    .xcvr_ctrl.init = XCVR_CTRL_XCVR_CTRL_PROTOCOL(9) |
                      XCVR_CTRL_XCVR_CTRL_TGT_PWR_SRC(4) |
                      XCVR_CTRL_XCVR_CTRL_DEMOD_SEL(1),

    /* XCVR_PHY configs */
    .phy_pre_ref0_init = 0x79CDEB38,
    .phy_pre_ref1_init = 0xCE77DFF7,
    .phy_pre_ref2_init = 0x0000CEB7,

    .phy_cfg1_init = XCVR_PHY_CFG1_AA_PLAYBACK(0) |
                     XCVR_PHY_CFG1_AA_OUTPUT_SEL(1) |
                     XCVR_PHY_CFG1_FSK_BIT_INVERT(0) |
                     XCVR_PHY_CFG1_BSM_EN_BLE(0) |
                     XCVR_PHY_CFG1_DEMOD_CLK_MODE(0) |
#if RADIO_IS_GEN_2P0 
                     XCVR_PHY_CFG1_CTS_THRESH(208U) |
#else
                     XCVR_PHY_CFG1_CTS_THRESH(230U) |
#endif /* RADIO_IS_GEN_2P0 */
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
#endif /* RADIO_IS_GEN_2P0 */

    /* XCVR_TSM configs */
#if (DATA_PADDING_EN)
    .tsm_timing_35_init = B0(TX_DIG_EN_ASSERT+TX_DIG_EN_TX_HI_ADJ),
#else
    .tsm_timing_35_init = B0(TX_DIG_EN_ASSERT),
#endif /* (DATA_PADDING_EN) */

    /* XCVR_TX_DIG configs */
    .tx_gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_MULTIPLY_TABLE_MANUAL(0x4000) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MI(0) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MLD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_FLD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_GFSK_MOD_INDEX_SCALING(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_OVRD_EN(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_0_OVRD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_1_OVRD(0) |
                    XCVR_TX_DIG_GFSK_CTRL_TX_IMAGE_FILTER_2_OVRD(0),
    .tx_gfsk_coeff1_26mhz = 0,
    .tx_gfsk_coeff2_26mhz = 0,
    .tx_gfsk_coeff1_32mhz = 0,
    .tx_gfsk_coeff2_32mhz = 0,
};

/* MODE & DATA RATE combined configuration */
const xcvr_mode_datarate_config_t xcvr_MSK_1mbps_config =
{
    .radio_mode = MSK,
    .data_rate = DR_1MBPS,

    .ana_sy_ctrl2.mask = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_MASK,
    .ana_sy_ctrl2.init = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0), /* VCO KVM */
#if RADIO_IS_GEN_2P0
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(3) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(3), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(3), /*TZA_BW_SEL */
#else
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(1) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(1), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(1), /*TZA_BW_SEL */
#endif /* RADIO_IS_GEN_2P0 */

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
    /* MSK 1MBPS channel filter  @ 26MHz RF OSC */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0x0002,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0x0002,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0x0000,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFF9,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFF0,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFEA,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0xFFEC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0xFFFC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x001B,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x0042,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0066,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x007C,

    /* MSK 1MBPS channel filter  @ 32MHz RF OSC */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0002,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0x0002,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0xFFFC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0xFFF2,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFE9,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFE9,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFF7,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0x0016,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0040,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0069,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x0082,
#else
    /* MSK 1MBPS channel filter  @ 26MHz RF OSC */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0x0001,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0xFFFD,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0xFFF7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFF1,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFED,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFEF,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0xFFFA,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0x000E,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x0029,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x0045,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x005D,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x006B,

    /* MSK 1MBPS channel filter  @ 32MHz RF OSC */
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0000,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0x0007,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0x0008,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0x0000,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFF0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFE0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFE1,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0xFFFD,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0034,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0072,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x009A,
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

const xcvr_mode_datarate_config_t xcvr_MSK_500kbps_config =
{
    .radio_mode = MSK,
    .data_rate = DR_500KBPS,

    .ana_sy_ctrl2.mask = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_MASK,
    .ana_sy_ctrl2.init = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0), /* VCO KVM */
    .ana_rx_bba.mask = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_MASK | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_MASK,
    .ana_rx_bba.init = XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(5) | XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(5), /* BBA_BW_SEL and BBA2_BW_SEL */ 
    .ana_rx_tza.mask = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_MASK,
    .ana_rx_tza.init = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(5), /* TZA_BW_SEL */ 

    .phy_cfg2_init = XCVR_PHY_CFG2_PHY_FIFO_PRECHG(8) |
                     XCVR_PHY_CFG2_X2_DEMOD_GAIN(0xa) ,

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
#endif /* RADIO_IS_GEN_2P0 */
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
#endif /* RADIO_IS_GEN_2P0 */

#if RADIO_IS_GEN_2P1
    .agc_ctrl_3_init = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(21),
    .dcoc_ctrl_0_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(12),
    .dcoc_ctrl_0_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(14),
    .dcoc_ctrl_1_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dcoc_ctrl_1_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dc_resid_ctrl_init_26mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(50) |
                                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
    .dc_resid_ctrl_init_32mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(50) |
                                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
#endif

    /* All constant values are represented as 16 bits, register writes will remove unused bits */
#if RADIO_IS_GEN_2P0
    /* MSK 500KBPS channel filter @ 26MHz RF OSC */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0x0001,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0x0004,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0x0006,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0x0005,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFFC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFED,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0xFFE2,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0xFFE7,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x0005,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x0038,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x006F,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0092,

    /* MSK 500KBPS channel filter @ 32MHz RF OSC */ 
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0xFFFF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0x0002,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0x0006,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0x0009,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0xFFF3,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFE2,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFE0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0xFFFA,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0031,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0071,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x009C,
#else
    /* MSK 500KBPS channel filter @ 26MHz RF OSC */
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0x0006,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0x0006,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0x0002,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFEB,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFE2,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0xFFE3,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0xFFF6,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x0019,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x0045,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x006E,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x0087,

    /* MSK 500KBPS channel filter @ 32MHz RF OSC */ 
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0xFFFF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0xFFFC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0xFFFF,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0x0008,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0x0012,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0x000A,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFED,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFCE,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0xFFD4,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x0014,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0079,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x00C5,
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

const xcvr_mode_datarate_config_t xcvr_MSK_250kbps_config =
{
    .radio_mode = MSK,
    .data_rate = DR_250KBPS,

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
#endif
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
#endif

#if RADIO_IS_GEN_2P1
    .agc_ctrl_3_init = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(21),
    .dcoc_ctrl_0_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(15),
    .dcoc_ctrl_0_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(18),
    .dcoc_ctrl_1_init_26mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dcoc_ctrl_1_init_32mhz = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),
    .dc_resid_ctrl_init_26mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(115) |
                                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
    .dc_resid_ctrl_init_32mhz = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(115) |
                                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0),
#endif

    /* All constant values are represented as 16 bits, register writes will remove unused bits */
#if RADIO_IS_GEN_2P0
    /* MSK 250KBPS channel filter @ 26MHz RF OSC */ 
    .rx_chf_coeffs_26mhz.rx_chf_coef_0 = 0x0002,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1 = 0xFFFF,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2 = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3 = 0xFFFA,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4 = 0x000A,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5 = 0x0019,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6 = 0x0009,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7 = 0xFFDB,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8 = 0xFFC1,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9 = 0xFFF6,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0072,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x00DD,

    /* MSK 250KBPS channel filter @ 32MHz RF OSC */ 
    .rx_chf_coeffs_32mhz.rx_chf_coef_0 = 0x0002,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1 = 0x0003,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2 = 0xFFFC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3 = 0xFFF4,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4 = 0xFFFD,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5 = 0x0016,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6 = 0x001A,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7 = 0xFFEC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8 = 0xFFBC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9 = 0xFFE0,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0069,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x00ED,
#else
    .rx_chf_coeffs_26mhz.rx_chf_coef_0  = 0x0004,
    .rx_chf_coeffs_26mhz.rx_chf_coef_1  = 0x0006,
    .rx_chf_coeffs_26mhz.rx_chf_coef_2  = 0x0004,
    .rx_chf_coeffs_26mhz.rx_chf_coef_3  = 0xFFFC,
    .rx_chf_coeffs_26mhz.rx_chf_coef_4  = 0xFFF1,
    .rx_chf_coeffs_26mhz.rx_chf_coef_5  = 0xFFE8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_6  = 0xFFE8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_7  = 0xFFF8,
    .rx_chf_coeffs_26mhz.rx_chf_coef_8  = 0x0017,
    .rx_chf_coeffs_26mhz.rx_chf_coef_9  = 0x0040,
    .rx_chf_coeffs_26mhz.rx_chf_coef_10 = 0x0067,
    .rx_chf_coeffs_26mhz.rx_chf_coef_11 = 0x007E,

    /* MSK 250KBPS channel filter @ 32MHz RF OSC */ 
    .rx_chf_coeffs_32mhz.rx_chf_coef_0  = 0x0000,
    .rx_chf_coeffs_32mhz.rx_chf_coef_1  = 0xFFFD,
    .rx_chf_coeffs_32mhz.rx_chf_coef_2  = 0xFFFC,
    .rx_chf_coeffs_32mhz.rx_chf_coef_3  = 0x0002,
    .rx_chf_coeffs_32mhz.rx_chf_coef_4  = 0x000E,
    .rx_chf_coeffs_32mhz.rx_chf_coef_5  = 0x000E,
    .rx_chf_coeffs_32mhz.rx_chf_coef_6  = 0xFFF8,
    .rx_chf_coeffs_32mhz.rx_chf_coef_7  = 0xFFD8,
    .rx_chf_coeffs_32mhz.rx_chf_coef_8  = 0xFFD7,
    .rx_chf_coeffs_32mhz.rx_chf_coef_9  = 0x000F,
    .rx_chf_coeffs_32mhz.rx_chf_coef_10 = 0x0072,
    .rx_chf_coeffs_32mhz.rx_chf_coef_11 = 0x00BF,
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

