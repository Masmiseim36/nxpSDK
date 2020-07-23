/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nxp_xcvr_common_config.h"

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
 *******************************************************************************/
 /*! @brief Configuration structure for settings that are common across all modes and data rates. */
const xcvr_common_config_t xcvr_common_config =
{
    /***********************************************/
    /*********** START OF GENERATED CODE ***********/
    /************** xcvr_common_config *************/
    /***********************************************/
    
    /* XCVR_2P4GHZ_PHY configs */
    /***************************/

    /* DMD_CTRL0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .dmd_ctrl0 = XCVR_2P4GHZ_PHY_DMD_CTRL0_DEMOD_MOD(3)|
                XCVR_2P4GHZ_PHY_DMD_CTRL0_DREP_SCALE_FREQ(0x04)|
                XCVR_2P4GHZ_PHY_DMD_CTRL0_DREP_SINE_EN(0)|
                XCVR_2P4GHZ_PHY_DMD_CTRL0_FED_ACT_WIN(0x1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL0_FERR_TRK_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL0_REPEAT_FACTOR(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN(0x2)|
                XCVR_2P4GHZ_PHY_DMD_CTRL0_TERR_TRK_EN(1),

    /* DMD_CTRL1 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .dmd_ctrl1 = XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IDLE_WIN(0x008)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_FED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_ERR_SCALE(0x04)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IDLE_WIN(0x004)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TED_IMM_MEAS_EN(1)|
                XCVR_2P4GHZ_PHY_DMD_CTRL1_TTRK_INT_RANGE(0x0A),

    /* FSK_CFG0 configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'MD', 'MD'] */
    .fsk_cfg0 = XCVR_2P4GHZ_PHY_FSK_CFG0_AA_OUT_SEL(1)|
                XCVR_2P4GHZ_PHY_FSK_CFG0_BLE_NTW_ADR_THR(0)|
                XCVR_2P4GHZ_PHY_FSK_CFG0_FSK_BIT_INVERT(0)|
                XCVR_2P4GHZ_PHY_FSK_CFG0_HAM_CHK_LOW_PWR(0),

    /* FSK_CFG2 configuration, dependencies: ['MD+DR', 'MD+DR', 'COM'] */
    .fsk_cfg2 = XCVR_2P4GHZ_PHY_FSK_CFG2_MAG_WIN(0x04),

    /* FSK_FAD_CFG configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .fsk_fad_cfg = XCVR_2P4GHZ_PHY_FSK_FAD_CFG_WIN_FAD_SEARCH_PD(0x00)|
                XCVR_2P4GHZ_PHY_FSK_FAD_CFG_WIN_FAD_WAIT_PD(0x00)|
                XCVR_2P4GHZ_PHY_FSK_FAD_CFG_WIN_FAD_WAIT_SYNCH(0x00)|
                XCVR_2P4GHZ_PHY_FSK_FAD_CFG_WIN_SEARCH_PD(0x00),

    /* FSK_FAD_CTRL configuration, dependencies: ['COM'] */
    .fsk_fad_ctrl = XCVR_2P4GHZ_PHY_FSK_FAD_CTRL_FAD_EN(0),

    /* FSK_PT configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM'] */
    .fsk_pt = XCVR_2P4GHZ_PHY_FSK_PT_AGC_TIMEOUT(0x0048)|
                XCVR_2P4GHZ_PHY_FSK_PT_BYPASS_WITH_RSSI(0)|
                XCVR_2P4GHZ_PHY_FSK_PT_COND_AA_BUFF_EN(0)|
                XCVR_2P4GHZ_PHY_FSK_PT_COND_SIG_PRST_EN(0)|
                XCVR_2P4GHZ_PHY_FSK_PT_POW_TH_BYPASS(0),

    /* MISC configuration, dependencies: ['COM', 'COM', 'COM'] */
    .misc = XCVR_2P4GHZ_PHY_MISC_PHY_CLK_CTRL(0x03FF)|
                XCVR_2P4GHZ_PHY_MISC_PHY_CLK_ON(1)|
                XCVR_2P4GHZ_PHY_MISC_RSSI_CORR_TH(0xA0),

    /* PREPHY_MISC configuration, dependencies: ['COM', 'COM'] */
    .prephy_misc = XCVR_2P4GHZ_PHY_PREPHY_MISC_BUFF_PTR_GFSK(0x0C)|
                XCVR_2P4GHZ_PHY_PREPHY_MISC_BUFF_PTR_LR(0x10),

    /* SM_CFG configuration, dependencies: ['MD', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .sm_cfg = XCVR_2P4GHZ_PHY_SM_CFG_ACQ_MODE(2)|
                XCVR_2P4GHZ_PHY_SM_CFG_AGC_FRZ_ON_PD_FOUND_ACQ1_LR(0)|
                XCVR_2P4GHZ_PHY_SM_CFG_EARLY_PD_TIMEOUT(0x24)|
                XCVR_2P4GHZ_PHY_SM_CFG_EN_PHY_SM_EXT_RST(0)|
#if RF_OSC_26MHZ == 1
                XCVR_2P4GHZ_PHY_SM_CFG_PH_BUFF_PTR_SYM(1),
#else
                XCVR_2P4GHZ_PHY_SM_CFG_PH_BUFF_PTR_SYM(0),
#endif /* RF_OSC_26MHZ == 1 */
    
    /* XCVR_ANALOG configs */
    /***********************/

    /* BB_LDO_1 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .bb_ldo_1 = XCVR_ANALOG_BB_LDO_1_BB_LDO_ADCDAC_BYP(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_ADCDAC_DIAGSEL(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_ADCDAC_SPARE(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_ADCDAC_TRIM(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_BBA_BYP(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_BBA_DIAGSEL(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_BBA_SPARE(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_BBA_TRIM(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_FDBK_BYP(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_FDBK_DIAGSEL(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_FDBK_SPARE(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_FDBK_TRIM(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_BYP(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_DIAGSEL(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_SPARE(0)|
                XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM_RX(3),

    /* BB_LDO_2 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .bb_ldo_2 = XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_PD_BYP(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_PD_DIAGSEL(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_PD_SPARE(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_PD_TRIM(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_VCOLO_BYP(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_VCOLO_DIAGSEL(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_VCOLO_TRIM(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_VCO_SPARE(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_VTREF_DIAGSEL(0)|
                XCVR_ANALOG_BB_LDO_2_BB_LDO_VTREF_TC(0),

    /* RX_AUXPLL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .rx_auxpll = XCVR_ANALOG_RX_AUXPLL_BIAS_TRIM(2)|
                XCVR_ANALOG_RX_AUXPLL_DIAGSEL1(0)|
                XCVR_ANALOG_RX_AUXPLL_DIAGSEL2(0)|
                XCVR_ANALOG_RX_AUXPLL_LF_CNTL(0)|
                XCVR_ANALOG_RX_AUXPLL_RXTX_BAL_BIAST(0)|
                XCVR_ANALOG_RX_AUXPLL_RXTX_BAL_SPARE(0)|
                XCVR_ANALOG_RX_AUXPLL_RXTX_RCCAL_DIAGSEL(0)|
                XCVR_ANALOG_RX_AUXPLL_SPARE(0x01)|
                XCVR_ANALOG_RX_AUXPLL_VCO_DAC_REF_ADJUST(0x07)|
                XCVR_ANALOG_RX_AUXPLL_VTUNE_TESTMODE(0),

    /* RX_BBA configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .rx_bba = XCVR_ANALOG_RX_BBA_RX_BBA2_SPARE(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_CUR_BUMP(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_DIAGSEL1(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_DIAGSEL2(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_DIAGSEL3(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_DIAGSEL4(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_SPARE(0x00),

    /* RX_LNA configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .rx_lna = XCVR_ANALOG_RX_LNA_RX_LNA_BUMP(0x00)|
                XCVR_ANALOG_RX_LNA_RX_LNA_HG_DIAGSEL(0)|
                XCVR_ANALOG_RX_LNA_RX_LNA_HIZ_ENABLE(0)|
                XCVR_ANALOG_RX_LNA_RX_LNA_LG_DIAGSEL(0)|
                XCVR_ANALOG_RX_LNA_RX_LNA_SPARE(1)|
                XCVR_ANALOG_RX_LNA_RX_MIXER_BUMP(0x00)|
                XCVR_ANALOG_RX_LNA_RX_MIXER_SPARE(0),

    /* RX_TZA configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM'] */
    .rx_tza = XCVR_ANALOG_RX_TZA_RX_TZA1_DIAGSEL(0)|
                XCVR_ANALOG_RX_TZA_RX_TZA2_DIAGSEL(0)|
                XCVR_ANALOG_RX_TZA_RX_TZA3_DIAGSEL(0)|
                XCVR_ANALOG_RX_TZA_RX_TZA4_DIAGSEL(0)|
                XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL_DRS(0)|
                XCVR_ANALOG_RX_TZA_RX_TZA_CUR_BUMP(0)|
                XCVR_ANALOG_RX_TZA_RX_TZA_GAIN_BUMP(0)|
                XCVR_ANALOG_RX_TZA_RX_TZA_SPARE(0x00),

    /* SY_CTRL_1 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .sy_ctrl_1 = XCVR_ANALOG_SY_CTRL_1_SY_DIVN_SPARE(0)|
                XCVR_ANALOG_SY_CTRL_1_SY_FCAL_SPARE(0)|
                XCVR_ANALOG_SY_CTRL_1_SY_LO_BUMP_RTLO_FDBK(1)|
                XCVR_ANALOG_SY_CTRL_1_SY_LO_BUMP_RTLO_RX(1)|
                XCVR_ANALOG_SY_CTRL_1_SY_LO_BUMP_RTLO_TX(1)|
                XCVR_ANALOG_SY_CTRL_1_SY_LO_DIAGSEL(0)|
                XCVR_ANALOG_SY_CTRL_1_SY_LO_SPARE(0)|
                XCVR_ANALOG_SY_CTRL_1_SY_LPF_FILT_CTRL(3)|
                XCVR_ANALOG_SY_CTRL_1_SY_LPF_SPARE(0)|
                XCVR_ANALOG_SY_CTRL_1_SY_PD_DIAGSEL(0)|
                XCVR_ANALOG_SY_CTRL_1_SY_PD_PCH_SEL(0)|
                XCVR_ANALOG_SY_CTRL_1_SY_PD_PCH_TUNE(0)|
                XCVR_ANALOG_SY_CTRL_1_SY_PD_SPARE(0),

    /* SY_CTRL_2 configuration, dependencies: ['COM', 'COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM'] */
    .sy_ctrl_2 = XCVR_ANALOG_SY_CTRL_2_SY_VCO_BIAS(4)|
                XCVR_ANALOG_SY_CTRL_2_SY_VCO_DIAGSEL(0)|
                XCVR_ANALOG_SY_CTRL_2_SY_VCO_KV(1)|
                XCVR_ANALOG_SY_CTRL_2_SY_VCO_PK_DET_ON(0)|
                XCVR_ANALOG_SY_CTRL_2_SY_VCO_SPARE(0),

    /* TX_DAC_PA configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .tx_dac_pa = XCVR_ANALOG_TX_DAC_PA_TX_DAC_BUMP_CAP(0)|
                XCVR_ANALOG_TX_DAC_PA_TX_DAC_BUMP_IDAC(0)|
                XCVR_ANALOG_TX_DAC_PA_TX_DAC_BUMP_RLOAD(0)|
                XCVR_ANALOG_TX_DAC_PA_TX_DAC_DIAGSEL(0)|
                XCVR_ANALOG_TX_DAC_PA_TX_DAC_INVERT_CLK(0)|
                XCVR_ANALOG_TX_DAC_PA_TX_DAC_OPAMP_DIAGSEL(0)|
                XCVR_ANALOG_TX_DAC_PA_TX_DAC_SPARE(0)|
                XCVR_ANALOG_TX_DAC_PA_TX_PA_BUMP_VBIAS(1)|
                XCVR_ANALOG_TX_DAC_PA_TX_PA_DIAGSEL(0)|
                XCVR_ANALOG_TX_DAC_PA_TX_PA_SPARE(1),
    
    /* XCVR_MISC configs */
    /*********************/

    /* XCVR_CTRL configuration, dependencies: [nan, nan, nan, nan, 'COM', nan, nan, 'DR', 'DR'] */
#if RF_OSC_26MHZ == 1
    .xcvr_ctrl = XCVR_MISC_XCVR_CTRL_REF_CLK_FREQ(1),
#else
    .xcvr_ctrl = XCVR_MISC_XCVR_CTRL_REF_CLK_FREQ(0),
#endif /* RF_OSC_26MHZ == 1 */
    
    /* XCVR_PLL_DIG configs */
    /************************/

    /* BALUN_RX configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .balun_rx = XCVR_PLL_DIG_BALUN_RX_TUNING_RANGE_0(0x6)|
                XCVR_PLL_DIG_BALUN_RX_TUNING_RANGE_1(0x6)|
                XCVR_PLL_DIG_BALUN_RX_TUNING_RANGE_2(0x6)|
                XCVR_PLL_DIG_BALUN_RX_TUNING_RANGE_3(0x6)|
                XCVR_PLL_DIG_BALUN_RX_TUNING_RANGE_4(0x5)|
                XCVR_PLL_DIG_BALUN_RX_TUNING_RANGE_5(0x4)|
                XCVR_PLL_DIG_BALUN_RX_TUNING_RANGE_6(0x3)|
                XCVR_PLL_DIG_BALUN_RX_TUNING_RANGE_7(0x2),

    /* BALUN_TX configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .balun_tx = XCVR_PLL_DIG_BALUN_TX_TUNING_RANGE_0(0x6)|
                XCVR_PLL_DIG_BALUN_TX_TUNING_RANGE_1(0x6)|
                XCVR_PLL_DIG_BALUN_TX_TUNING_RANGE_2(0x6)|
                XCVR_PLL_DIG_BALUN_TX_TUNING_RANGE_3(0x6)|
                XCVR_PLL_DIG_BALUN_TX_TUNING_RANGE_4(0x5)|
                XCVR_PLL_DIG_BALUN_TX_TUNING_RANGE_5(0x4)|
                XCVR_PLL_DIG_BALUN_TX_TUNING_RANGE_6(0x3)|
                XCVR_PLL_DIG_BALUN_TX_TUNING_RANGE_7(0x2),

    /* CHAN_MAP configuration, dependencies: ['MD', 'COM', 'COM', 'COM', 'COM'] */
    .chan_map = XCVR_PLL_DIG_CHAN_MAP_BMR(0)|
                XCVR_PLL_DIG_CHAN_MAP_CHANNEL_NUM_OVRD(0x0000)|
                XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD(0)|
                XCVR_PLL_DIG_CHAN_MAP_HOP_TBL_CFG_OVRD_EN(0),

    /* CHAN_MAP_EXT configuration, dependencies: ['COM', 'COM'] */
    .chan_map_ext = XCVR_PLL_DIG_CHAN_MAP_EXT_CTUNE_TGT_OFFSET(0)|
                XCVR_PLL_DIG_CHAN_MAP_EXT_NUM_OFFSET(0x00000000),

    /* DATA_RATE_OVRD_CTRL1 configuration, dependencies: ['MD+DR', 'COM', 'COM', 'COM'] */
    .data_rate_ovrd_ctrl1 = XCVR_PLL_DIG_DATA_RATE_OVRD_CTRL1_HPM_FDB_RES_CAL_CFG1(1)|
                XCVR_PLL_DIG_DATA_RATE_OVRD_CTRL1_HPM_FDB_RES_TX_CFG1(0)|
                XCVR_PLL_DIG_DATA_RATE_OVRD_CTRL1_LPM_SCALE_CFG1(0x08),

    /* DELAY_MATCH configuration, dependencies: ['COM', 'COM', 'COM'] */
    .delay_match = XCVR_PLL_DIG_DELAY_MATCH_HPM_INTEGER_DELAY(0x00)|
                XCVR_PLL_DIG_DELAY_MATCH_HPM_SDM_DELAY(0x00)|
                XCVR_PLL_DIG_DELAY_MATCH_LPM_SDM_DELAY(0x03),

    /* HPM_BUMP configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .hpm_bump = XCVR_PLL_DIG_HPM_BUMP_HPM_FDB_RES_CAL(1)|
                XCVR_PLL_DIG_HPM_BUMP_HPM_FDB_RES_TX(0)|
                XCVR_PLL_DIG_HPM_BUMP_HPM_VCM_CAL(1)|
                XCVR_PLL_DIG_HPM_BUMP_HPM_VCM_TX(0),

    /* HPM_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .hpm_ctrl = XCVR_PLL_DIG_HPM_CTRL_HPM_CAL_INVERT(1)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_CAL_TIME(0)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_DTH_EN(1)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_DTH_SCL(0)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_INTEGER_INVERT(0)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_LFSR_SIZE(4)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_MOD_IN_INVERT(1)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_SCALE(0)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_SDM_IN_DISABLE(0)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_SDM_IN_MANUAL(0x0000)|
                XCVR_PLL_DIG_HPM_CTRL_HPM_SDM_OUT_INVERT(0),

    /* HPM_SDM_RES configuration, dependencies: ['COM', 'COM'] */
    .hpm_sdm_res = XCVR_PLL_DIG_HPM_SDM_RES_HPM_COUNT_ADJUST(0x00)|
                XCVR_PLL_DIG_HPM_SDM_RES_HPM_DENOM(0x00FF),

    /* LOCK_DETECT configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .lock_detect = XCVR_PLL_DIG_LOCK_DETECT_CTUNE_LDF_LEV(0x08)|
                XCVR_PLL_DIG_LOCK_DETECT_FCAL_HOLD_EN(0)|
                XCVR_PLL_DIG_LOCK_DETECT_FREQ_COUNT_GO(0)|
                XCVR_PLL_DIG_LOCK_DETECT_FREQ_COUNT_TIME(0)|
                XCVR_PLL_DIG_LOCK_DETECT_FTF_RX_THRSH(0x21)|
                XCVR_PLL_DIG_LOCK_DETECT_FTF_TX_THRSH(0x06)|
                XCVR_PLL_DIG_LOCK_DETECT_FTW_TXRX(7),

    /* LPM_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .lpm_ctrl = XCVR_PLL_DIG_LPM_CTRL_HPM_CAL_SCALE(0x0A)|
                XCVR_PLL_DIG_LPM_CTRL_LPM_DISABLE(0)|
                XCVR_PLL_DIG_LPM_CTRL_LPM_DTH_SCL(0x08)|
                XCVR_PLL_DIG_LPM_CTRL_LPM_D_CTRL(0)|
                XCVR_PLL_DIG_LPM_CTRL_LPM_D_OVRD(1)|
                XCVR_PLL_DIG_LPM_CTRL_LPM_SCALE(0x08)|
                XCVR_PLL_DIG_LPM_CTRL_LPM_SDM_INV(0)|
                XCVR_PLL_DIG_LPM_CTRL_LPM_SDM_USE_NEG(0)|
                XCVR_PLL_DIG_LPM_CTRL_PLL_LD_DISABLE(0)|
                XCVR_PLL_DIG_LPM_CTRL_PLL_LD_MANUAL(0x00),

    /* LPM_SDM_CTRL1 configuration, dependencies: ['COM', 'COM', 'COM'] */
    .lpm_sdm_ctrl1 = XCVR_PLL_DIG_LPM_SDM_CTRL1_HPM_ARRAY_BIAS(0x00)|
                XCVR_PLL_DIG_LPM_SDM_CTRL1_LPM_INTG(0x26)|
                XCVR_PLL_DIG_LPM_SDM_CTRL1_SDM_MAP_DISABLE(0),

    /* MOD_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .mod_ctrl = XCVR_PLL_DIG_MOD_CTRL_HPM_MOD_DISABLE(0)|
                XCVR_PLL_DIG_MOD_CTRL_HPM_MOD_MANUAL(0x00)|
                XCVR_PLL_DIG_MOD_CTRL_HPM_SDM_OUT_DISABLE(0)|
                XCVR_PLL_DIG_MOD_CTRL_HPM_SDM_OUT_MANUAL(0)|
                XCVR_PLL_DIG_MOD_CTRL_MODULATION_WORD_MANUAL(0x0000)|
                XCVR_PLL_DIG_MOD_CTRL_MOD_DISABLE(0),
    
    /* XCVR_PLL_DIG2 configs */
    /*************************/

    /* HPMCAL_CTRL configuration, dependencies: ['MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .hpmcal_ctrl = XCVR_PLL_DIG2_HPMCAL_CTRL_HPM_CAL_BUMPED(0)|
                XCVR_PLL_DIG2_HPMCAL_CTRL_HPM_CAL_COUNT_SCALE(0)|
                XCVR_PLL_DIG2_HPMCAL_CTRL_HPM_CAL_FACTOR_MANUAL(0)|
                XCVR_PLL_DIG2_HPMCAL_CTRL_HPM_CAL_SKIP(0)|
                XCVR_PLL_DIG2_HPMCAL_CTRL_HP_CAL_DISABLE(0),
    
    /* XCVR_RX_DIG configs */
    /***********************/

    /* AGC_CTRL_0 configuration, dependencies: ['COM', 'COM', 'MD+DR', 'COM', 'COM', 'DR', 'MD+DR', 'MD+DR', 'COM', 'COM'] */
    .agc_ctrl_0 = XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_BBA_STEP_SZ(0x02)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_LNA_STEP_SZ(0x02)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_FREEZE_EN(1)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_FREEZE_SRC(0)|
                XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_EN(1)|
                XCVR_RX_DIG_AGC_CTRL_0_SLOW_AGC_SRC(2),

    /* AGC_CTRL_1 configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .agc_ctrl_1 = XCVR_RX_DIG_AGC_CTRL_1_BBA_USER_GAIN(0x00)|
                XCVR_RX_DIG_AGC_CTRL_1_LNA_USER_GAIN(0x00)|
                XCVR_RX_DIG_AGC_CTRL_1_PDET_HI_SEL_HOLD(1)|
                XCVR_RX_DIG_AGC_CTRL_1_PRESLOW_DOWN_THRESH(0x00)|
                XCVR_RX_DIG_AGC_CTRL_1_PRESLOW_EN(0)|
                XCVR_RX_DIG_AGC_CTRL_1_PRESLOW_UP_THRESH(0x00)|
                XCVR_RX_DIG_AGC_CTRL_1_USER_BBA_GAIN_EN(0)|
                XCVR_RX_DIG_AGC_CTRL_1_USER_LNA_GAIN_EN(0),

    /* AGC_CTRL_1_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'COM'] */
    .agc_ctrl_1_drs = XCVR_RX_DIG_AGC_CTRL_1_DRS_AGC_UP_SRC(1)|
                XCVR_RX_DIG_AGC_CTRL_1_DRS_SLOW_AGC_SRC(2),

    /* AGC_CTRL_2 configuration, dependencies: ['DR', 'MD+DR', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR'] */
    .agc_ctrl_2 = XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_RST(0)|
                XCVR_RX_DIG_AGC_CTRL_2_LNA_HG_ON_OVR(0)|
                XCVR_RX_DIG_AGC_CTRL_2_LNA_LG_ON_OVR(0)|
                XCVR_RX_DIG_AGC_CTRL_2_MAN_PDET_RST(0)|
                XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_RST(0)|
                XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_HI(4),

    /* AGC_CTRL_2_DRS configuration, dependencies: ['MD+DR', 'COM'] */
    .agc_ctrl_2_drs = XCVR_RX_DIG_AGC_CTRL_2_DRS_TZA_PDET_SEL_HI(4),

    /* AGC_CTRL_3 configuration, dependencies: ['COM', 'COM', 'COM', 'MD+DR', 'COM'] */
    .agc_ctrl_3 = XCVR_RX_DIG_AGC_CTRL_3_AGC_H2S_STEP_SZ(0x06)|
                XCVR_RX_DIG_AGC_CTRL_3_AGC_PDET_LO_DLY(2)|
                XCVR_RX_DIG_AGC_CTRL_3_AGC_RSSI_DELT_H2S(0x14)|
                XCVR_RX_DIG_AGC_CTRL_3_AGC_UP_STEP_SZ(0x02),

    /* AGC_CTRL_4 configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'MD+DR'] */
    .agc_ctrl_4 = XCVR_RX_DIG_AGC_CTRL_4_BBA_PDET_LO_HOLD(1)|
                XCVR_RX_DIG_AGC_CTRL_4_IGNORE_BBA_PDET_HI(0)|
                XCVR_RX_DIG_AGC_CTRL_4_IGNORE_TZA_PDET_HI(0)|
                XCVR_RX_DIG_AGC_CTRL_4_TZA_PDET_LO_HOLD(1),

    /* AGC_GAIN_TBL_03_00 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .agc_gain_tbl_03_00 = XCVR_RX_DIG_AGC_GAIN_TBL_03_00_BBA_GAIN_00(0x00)|
                XCVR_RX_DIG_AGC_GAIN_TBL_03_00_BBA_GAIN_01(0x01)|
                XCVR_RX_DIG_AGC_GAIN_TBL_03_00_BBA_GAIN_02(0x01)|
                XCVR_RX_DIG_AGC_GAIN_TBL_03_00_BBA_GAIN_03(0x02)|
                XCVR_RX_DIG_AGC_GAIN_TBL_03_00_LNA_GAIN_00(0x00)|
                XCVR_RX_DIG_AGC_GAIN_TBL_03_00_LNA_GAIN_01(0x01)|
                XCVR_RX_DIG_AGC_GAIN_TBL_03_00_LNA_GAIN_02(0x02)|
                XCVR_RX_DIG_AGC_GAIN_TBL_03_00_LNA_GAIN_03(0x02),

    /* AGC_GAIN_TBL_07_04 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .agc_gain_tbl_07_04 = XCVR_RX_DIG_AGC_GAIN_TBL_07_04_BBA_GAIN_04(0x03)|
                XCVR_RX_DIG_AGC_GAIN_TBL_07_04_BBA_GAIN_05(0x04)|
                XCVR_RX_DIG_AGC_GAIN_TBL_07_04_BBA_GAIN_06(0x01)|
                XCVR_RX_DIG_AGC_GAIN_TBL_07_04_BBA_GAIN_07(0x02)|
                XCVR_RX_DIG_AGC_GAIN_TBL_07_04_LNA_GAIN_04(0x02)|
                XCVR_RX_DIG_AGC_GAIN_TBL_07_04_LNA_GAIN_05(0x02)|
                XCVR_RX_DIG_AGC_GAIN_TBL_07_04_LNA_GAIN_06(0x03)|
                XCVR_RX_DIG_AGC_GAIN_TBL_07_04_LNA_GAIN_07(0x03),

    /* AGC_GAIN_TBL_11_08 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .agc_gain_tbl_11_08 = XCVR_RX_DIG_AGC_GAIN_TBL_11_08_BBA_GAIN_08(0x03)|
                XCVR_RX_DIG_AGC_GAIN_TBL_11_08_BBA_GAIN_09(0x02)|
                XCVR_RX_DIG_AGC_GAIN_TBL_11_08_BBA_GAIN_10(0x03)|
                XCVR_RX_DIG_AGC_GAIN_TBL_11_08_BBA_GAIN_11(0x04)|
                XCVR_RX_DIG_AGC_GAIN_TBL_11_08_LNA_GAIN_08(0x03)|
                XCVR_RX_DIG_AGC_GAIN_TBL_11_08_LNA_GAIN_09(0x04)|
                XCVR_RX_DIG_AGC_GAIN_TBL_11_08_LNA_GAIN_10(0x04)|
                XCVR_RX_DIG_AGC_GAIN_TBL_11_08_LNA_GAIN_11(0x04),

    /* AGC_GAIN_TBL_15_12 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .agc_gain_tbl_15_12 = XCVR_RX_DIG_AGC_GAIN_TBL_15_12_BBA_GAIN_12(0x04)|
                XCVR_RX_DIG_AGC_GAIN_TBL_15_12_BBA_GAIN_13(0x05)|
                XCVR_RX_DIG_AGC_GAIN_TBL_15_12_BBA_GAIN_14(0x06)|
                XCVR_RX_DIG_AGC_GAIN_TBL_15_12_BBA_GAIN_15(0x05)|
                XCVR_RX_DIG_AGC_GAIN_TBL_15_12_LNA_GAIN_12(0x05)|
                XCVR_RX_DIG_AGC_GAIN_TBL_15_12_LNA_GAIN_13(0x05)|
                XCVR_RX_DIG_AGC_GAIN_TBL_15_12_LNA_GAIN_14(0x05)|
                XCVR_RX_DIG_AGC_GAIN_TBL_15_12_LNA_GAIN_15(0x06),

    /* AGC_GAIN_TBL_19_16 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .agc_gain_tbl_19_16 = XCVR_RX_DIG_AGC_GAIN_TBL_19_16_BBA_GAIN_16(0x06)|
                XCVR_RX_DIG_AGC_GAIN_TBL_19_16_BBA_GAIN_17(0x07)|
                XCVR_RX_DIG_AGC_GAIN_TBL_19_16_BBA_GAIN_18(0x06)|
                XCVR_RX_DIG_AGC_GAIN_TBL_19_16_BBA_GAIN_19(0x07)|
                XCVR_RX_DIG_AGC_GAIN_TBL_19_16_LNA_GAIN_16(0x06)|
                XCVR_RX_DIG_AGC_GAIN_TBL_19_16_LNA_GAIN_17(0x06)|
                XCVR_RX_DIG_AGC_GAIN_TBL_19_16_LNA_GAIN_18(0x07)|
                XCVR_RX_DIG_AGC_GAIN_TBL_19_16_LNA_GAIN_19(0x07),

    /* AGC_GAIN_TBL_23_20 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .agc_gain_tbl_23_20 = XCVR_RX_DIG_AGC_GAIN_TBL_23_20_BBA_GAIN_20(0x06)|
                XCVR_RX_DIG_AGC_GAIN_TBL_23_20_BBA_GAIN_21(0x07)|
                XCVR_RX_DIG_AGC_GAIN_TBL_23_20_BBA_GAIN_22(0x06)|
                XCVR_RX_DIG_AGC_GAIN_TBL_23_20_BBA_GAIN_23(0x07)|
                XCVR_RX_DIG_AGC_GAIN_TBL_23_20_LNA_GAIN_20(0x08)|
                XCVR_RX_DIG_AGC_GAIN_TBL_23_20_LNA_GAIN_21(0x08)|
                XCVR_RX_DIG_AGC_GAIN_TBL_23_20_LNA_GAIN_22(0x09)|
                XCVR_RX_DIG_AGC_GAIN_TBL_23_20_LNA_GAIN_23(0x09),

    /* AGC_GAIN_TBL_26_24 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .agc_gain_tbl_26_24 = XCVR_RX_DIG_AGC_GAIN_TBL_26_24_BBA_GAIN_24(0x08)|
                XCVR_RX_DIG_AGC_GAIN_TBL_26_24_BBA_GAIN_25(0x09)|
                XCVR_RX_DIG_AGC_GAIN_TBL_26_24_BBA_GAIN_26(0x0A)|
                XCVR_RX_DIG_AGC_GAIN_TBL_26_24_LNA_GAIN_24(0x09)|
                XCVR_RX_DIG_AGC_GAIN_TBL_26_24_LNA_GAIN_25(0x09)|
                XCVR_RX_DIG_AGC_GAIN_TBL_26_24_LNA_GAIN_26(0x09),

    /* AGC_MAN_AGC_IDX configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .agc_man_agc_idx = XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_DCOC_START_PT(0)|
                XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_IDX_CMP_PHY(0x1A)|
                XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_MAN_IDX(0x00)|
                XCVR_RX_DIG_AGC_MAN_AGC_IDX_AGC_MAN_IDX_EN(0),

    /* BBA_RES_TUNE_LIN_VAL_10_8 configuration, dependencies: ['COM', 'COM', 'COM'] */
    .bba_res_tune_lin_val_10_8 = XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_10_8_BBA_RES_TUNE_LIN_VAL_10(0x013A)|
                XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_10_8_BBA_RES_TUNE_LIN_VAL_8(0x0092)|
                XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_10_8_BBA_RES_TUNE_LIN_VAL_9(0x00D7),

    /* BBA_RES_TUNE_LIN_VAL_3_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .bba_res_tune_lin_val_3_0 = XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_3_0_BBA_RES_TUNE_LIN_VAL_0(0x08)|
                XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_3_0_BBA_RES_TUNE_LIN_VAL_1(0x0B)|
                XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_3_0_BBA_RES_TUNE_LIN_VAL_2(0x10)|
                XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_3_0_BBA_RES_TUNE_LIN_VAL_3(0x19),

    /* BBA_RES_TUNE_LIN_VAL_7_4 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .bba_res_tune_lin_val_7_4 = XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_7_4_BBA_RES_TUNE_LIN_VAL_4(0x25)|
                XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_7_4_BBA_RES_TUNE_LIN_VAL_5(0x34)|
                XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_7_4_BBA_RES_TUNE_LIN_VAL_6(0x49)|
                XCVR_RX_DIG_BBA_RES_TUNE_LIN_VAL_7_4_BBA_RES_TUNE_LIN_VAL_7(0x32),

    /* BBA_RES_TUNE_VAL_10_8 configuration, dependencies: ['COM', 'COM', 'COM'] */
    .bba_res_tune_val_10_8 = XCVR_RX_DIG_BBA_RES_TUNE_VAL_10_8_BBA_RES_TUNE_VAL_10(0x02)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_10_8_BBA_RES_TUNE_VAL_8(0x00)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_10_8_BBA_RES_TUNE_VAL_9(0x01),

    /* BBA_RES_TUNE_VAL_7_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .bba_res_tune_val_7_0 = XCVR_RX_DIG_BBA_RES_TUNE_VAL_7_0_BBA_RES_TUNE_VAL_0(0x00)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_7_0_BBA_RES_TUNE_VAL_1(0x00)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_7_0_BBA_RES_TUNE_VAL_2(0x00)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_7_0_BBA_RES_TUNE_VAL_3(0x00)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_7_0_BBA_RES_TUNE_VAL_4(0x00)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_7_0_BBA_RES_TUNE_VAL_5(0x00)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_7_0_BBA_RES_TUNE_VAL_6(0x00)|
                XCVR_RX_DIG_BBA_RES_TUNE_VAL_7_0_BBA_RES_TUNE_VAL_7(0x00),

    /* CCA_ED_LQI_CTRL_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .cca_ed_lqi_ctrl_0 = XCVR_RX_DIG_CCA_ED_LQI_CTRL_0_CORR_CNTR_THRESH(0x00)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_0_LQI_CNTR(0x10)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_0_LQI_CORR_THRESH(0x00)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_0_SNR_ADJ(0x00),

    /* CCA_ED_LQI_CTRL_1 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .cca_ed_lqi_ctrl_1 = XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_CCA1_ED_EN_DIS(0)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_CORR_FLAG_SEL(0)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_LQI_BIAS(0x0C)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_LQI_RSSI_SENS(0x01)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_LQI_RSSI_WEIGHT(2)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_MAN_MEAS_COMPLETE(0)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_MEAS_TRANS_TO_IDLE(0)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_NB_WB_OVRD(0)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_NB_WB_OVRD_EN(0)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_RSSI_NOISE_AVG_DELAY(0x00)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_RSSI_NOISE_AVG_FACTOR(1)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_SEL_SNR_MODE(0)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_SNR_LQI_DIS(0)|
                XCVR_RX_DIG_CCA_ED_LQI_CTRL_1_SNR_LQI_WEIGHT(0x04),

    /* DCOC_BBA_STEP configuration, dependencies: ['COM', 'COM'] */
    .dcoc_bba_step = XCVR_RX_DIG_DCOC_BBA_STEP_BBA_DCOC_STEP(0x0117)|
                XCVR_RX_DIG_DCOC_BBA_STEP_BBA_DCOC_STEP_RECIP(0x03AB),

    /* DCOC_CAL_FAIL_TH configuration, dependencies: ['COM', 'COM'] */
    .dcoc_cal_fail_th = XCVR_RX_DIG_DCOC_CAL_FAIL_TH_DCOC_CAL_ALPHA_F_TH(0x0008)|
                XCVR_RX_DIG_DCOC_CAL_FAIL_TH_DCOC_CAL_BETA_F_TH(0x04B0),

    /* DCOC_CAL_GAIN configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .dcoc_cal_gain = XCVR_RX_DIG_DCOC_CAL_GAIN_DCOC_BBA_CAL_GAIN1(0x01)|
                XCVR_RX_DIG_DCOC_CAL_GAIN_DCOC_BBA_CAL_GAIN2(0x01)|
                XCVR_RX_DIG_DCOC_CAL_GAIN_DCOC_BBA_CAL_GAIN3(0x05)|
                XCVR_RX_DIG_DCOC_CAL_GAIN_DCOC_LNA_CAL_GAIN1(0x01)|
                XCVR_RX_DIG_DCOC_CAL_GAIN_DCOC_LNA_CAL_GAIN2(0x07)|
                XCVR_RX_DIG_DCOC_CAL_GAIN_DCOC_LNA_CAL_GAIN3(0x01),

    /* DCOC_CAL_PASS_TH configuration, dependencies: ['COM', 'COM'] */
    .dcoc_cal_pass_th = XCVR_RX_DIG_DCOC_CAL_PASS_TH_DCOC_CAL_ALPHA_P_TH(0x0006)|
                XCVR_RX_DIG_DCOC_CAL_PASS_TH_DCOC_CAL_BETA_P_TH(0x041A),

    /* DCOC_CAL_RCP configuration, dependencies: ['COM', 'COM'] */
    .dcoc_cal_rcp = XCVR_RX_DIG_DCOC_CAL_RCP_ALPHA_CALC_RECIP(0x001C)|
                XCVR_RX_DIG_DCOC_CAL_RCP_DCOC_TMP_CALC_RECIP(0x00F6),

    /* DCOC_CTRL_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .dcoc_ctrl_0 = XCVR_RX_DIG_DCOC_CTRL_0_BBA_CORR_POL(0)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_CHECK_EN(1)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORRECT_EN(1)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORRECT_SRC(1)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_MAN(0)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_MIDPWR_TRK_DIS(0)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_TRK_EST_OVR(0)|
                XCVR_RX_DIG_DCOC_CTRL_0_TRACK_FROM_ZERO(1)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_DURATION(0x10)|
#else
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CAL_DURATION(0x14)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_RX_DIG_DCOC_CTRL_0_TZA_CORR_POL(0),

    /* DCOC_CTRL_1 configuration, dependencies: ['COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM'] */
    .dcoc_ctrl_1 = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHAC_SCALE_ALT_IDX(3)|
                XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHAC_SCALE_IDX(3)|
                XCVR_RX_DIG_DCOC_CTRL_1_DCOC_SIGN_SCALE_ALT_IDX(1)|
                XCVR_RX_DIG_DCOC_CTRL_1_DCOC_SIGN_SCALE_IDX(1)|
                XCVR_RX_DIG_DCOC_CTRL_1_DCOC_TRK_MIN_AGC_IDX(0x00)|
                XCVR_RX_DIG_DCOC_CTRL_1_DCOC_TRK_MIN_AGC_IDX_CFG(0),

    /* DCOC_TZA_STEP_0 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_0 = XCVR_RX_DIG_DCOC_TZA_STEP_0_DCOC_TZA_STEP_GAIN_0(0x004D)|
                XCVR_RX_DIG_DCOC_TZA_STEP_0_DCOC_TZA_STEP_RCP_0(0x0D4C),

    /* DCOC_TZA_STEP_1 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_1 = XCVR_RX_DIG_DCOC_TZA_STEP_1_DCOC_TZA_STEP_GAIN_1(0x006C)|
                XCVR_RX_DIG_DCOC_TZA_STEP_1_DCOC_TZA_STEP_RCP_1(0x0987),

    /* DCOC_TZA_STEP_10 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_10 = XCVR_RX_DIG_DCOC_TZA_STEP_10_DCOC_TZA_STEP_GAIN_10(0x0A5D)|
                XCVR_RX_DIG_DCOC_TZA_STEP_10_DCOC_TZA_STEP_RCP_10(0x0063),

    /* DCOC_TZA_STEP_2 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_2 = XCVR_RX_DIG_DCOC_TZA_STEP_2_DCOC_TZA_STEP_GAIN_2(0x009B)|
                XCVR_RX_DIG_DCOC_TZA_STEP_2_DCOC_TZA_STEP_RCP_2(0x069B),

    /* DCOC_TZA_STEP_3 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_3 = XCVR_RX_DIG_DCOC_TZA_STEP_3_DCOC_TZA_STEP_GAIN_3(0x00DC)|
                XCVR_RX_DIG_DCOC_TZA_STEP_3_DCOC_TZA_STEP_RCP_3(0x04A8),

    /* DCOC_TZA_STEP_4 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_4 = XCVR_RX_DIG_DCOC_TZA_STEP_4_DCOC_TZA_STEP_GAIN_4(0x013A)|
                XCVR_RX_DIG_DCOC_TZA_STEP_4_DCOC_TZA_STEP_RCP_4(0x0343),

    /* DCOC_TZA_STEP_5 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_5 = XCVR_RX_DIG_DCOC_TZA_STEP_5_DCOC_TZA_STEP_GAIN_5(0x01B4)|
                XCVR_RX_DIG_DCOC_TZA_STEP_5_DCOC_TZA_STEP_RCP_5(0x0259),

    /* DCOC_TZA_STEP_6 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_6 = XCVR_RX_DIG_DCOC_TZA_STEP_6_DCOC_TZA_STEP_GAIN_6(0x0266)|
                XCVR_RX_DIG_DCOC_TZA_STEP_6_DCOC_TZA_STEP_RCP_6(0x01AB),

    /* DCOC_TZA_STEP_7 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_7 = XCVR_RX_DIG_DCOC_TZA_STEP_7_DCOC_TZA_STEP_GAIN_7(0x034D)|
                XCVR_RX_DIG_DCOC_TZA_STEP_7_DCOC_TZA_STEP_RCP_7(0x0136),

    /* DCOC_TZA_STEP_8 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_8 = XCVR_RX_DIG_DCOC_TZA_STEP_8_DCOC_TZA_STEP_GAIN_8(0x04E8)|
                XCVR_RX_DIG_DCOC_TZA_STEP_8_DCOC_TZA_STEP_RCP_8(0x00D1),

    /* DCOC_TZA_STEP_9 configuration, dependencies: ['COM', 'COM'] */
    .dcoc_tza_step_9 = XCVR_RX_DIG_DCOC_TZA_STEP_9_DCOC_TZA_STEP_GAIN_9(0x070D)|
                XCVR_RX_DIG_DCOC_TZA_STEP_9_DCOC_TZA_STEP_RCP_9(0x0091),

    /* DC_RESID_CTRL configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'MD+DR', 'DR', 'MD+DR', 'COM'] */
    .dc_resid_ctrl = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_ALPHA(1)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_EXT_DC_EN(1)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_GEARSHIFT(4)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_GS_EN(0)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_SECOND_RUN_EN(1),

    /* LNA_GAIN_LIN_VAL_2_0 configuration, dependencies: ['COM', 'COM', 'COM'] */
    .lna_gain_lin_val_2_0 = XCVR_RX_DIG_LNA_GAIN_LIN_VAL_2_0_LNA_GAIN_LIN_VAL_0(0x0002)|
                XCVR_RX_DIG_LNA_GAIN_LIN_VAL_2_0_LNA_GAIN_LIN_VAL_1(0x0003)|
                XCVR_RX_DIG_LNA_GAIN_LIN_VAL_2_0_LNA_GAIN_LIN_VAL_2(0x0006),

    /* LNA_GAIN_LIN_VAL_5_3 configuration, dependencies: ['COM', 'COM', 'COM'] */
    .lna_gain_lin_val_5_3 = XCVR_RX_DIG_LNA_GAIN_LIN_VAL_5_3_LNA_GAIN_LIN_VAL_3(0x0015)|
                XCVR_RX_DIG_LNA_GAIN_LIN_VAL_5_3_LNA_GAIN_LIN_VAL_4(0x0026)|
                XCVR_RX_DIG_LNA_GAIN_LIN_VAL_5_3_LNA_GAIN_LIN_VAL_5(0x0039),

    /* LNA_GAIN_LIN_VAL_8_6 configuration, dependencies: ['COM', 'COM', 'COM'] */
    .lna_gain_lin_val_8_6 = XCVR_RX_DIG_LNA_GAIN_LIN_VAL_8_6_LNA_GAIN_LIN_VAL_6(0x0071)|
                XCVR_RX_DIG_LNA_GAIN_LIN_VAL_8_6_LNA_GAIN_LIN_VAL_7(0x00D6)|
                XCVR_RX_DIG_LNA_GAIN_LIN_VAL_8_6_LNA_GAIN_LIN_VAL_8(0x01A1),

    /* LNA_GAIN_LIN_VAL_9 configuration, dependencies: ['COM'] */
    .lna_gain_lin_val_9 = XCVR_RX_DIG_LNA_GAIN_LIN_VAL_9_LNA_GAIN_LIN_VAL_9(0x02D7),

    /* LNA_GAIN_VAL_3_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .lna_gain_val_3_0 = XCVR_RX_DIG_LNA_GAIN_VAL_3_0_LNA_GAIN_VAL_0(0x2B)|
                XCVR_RX_DIG_LNA_GAIN_VAL_3_0_LNA_GAIN_VAL_1(0x33)|
                XCVR_RX_DIG_LNA_GAIN_VAL_3_0_LNA_GAIN_VAL_2(0x0A)|
                XCVR_RX_DIG_LNA_GAIN_VAL_3_0_LNA_GAIN_VAL_3(0x39),

    /* LNA_GAIN_VAL_7_4 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .lna_gain_val_7_4 = XCVR_RX_DIG_LNA_GAIN_VAL_7_4_LNA_GAIN_VAL_4(0x51)|
                XCVR_RX_DIG_LNA_GAIN_VAL_7_4_LNA_GAIN_VAL_5(0x5C)|
                XCVR_RX_DIG_LNA_GAIN_VAL_7_4_LNA_GAIN_VAL_6(0x74)|
                XCVR_RX_DIG_LNA_GAIN_VAL_7_4_LNA_GAIN_VAL_7(0x8C),

    /* LNA_GAIN_VAL_9_8 configuration, dependencies: ['COM', 'COM'] */
    .lna_gain_val_9_8 = XCVR_RX_DIG_LNA_GAIN_VAL_9_8_LNA_GAIN_VAL_8(0xA1)|
                XCVR_RX_DIG_LNA_GAIN_VAL_9_8_LNA_GAIN_VAL_9(0xB7),

    /* RSSI_CTRL_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .rssi_ctrl_0 = XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ(0xEC)|
                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_HOLD_DELAY(0x04)|
                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_HOLD_EN(1)|
                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_HOLD_SRC(0)|
                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_CW_WEIGHT(0)|
                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_WT_NB(1)|
                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_N_WINDOW_NB(3)|
                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_USE_VALS(0)|
                XCVR_RX_DIG_RSSI_CTRL_0_RSSI_VLD_SETTLE(3),

    /* RSSI_CTRL_1 configuration, dependencies: ['COM', 'COM'] */
    .rssi_ctrl_1 = XCVR_RX_DIG_RSSI_CTRL_1_RSSI_IIR_WT_WB(1)|
                XCVR_RX_DIG_RSSI_CTRL_1_RSSI_N_WINDOW_WB(2),

    /* RX_DIG_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .rx_dig_ctrl = XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_NEGEDGE(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_POL(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_RAW_EN(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_BYPASS(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_CAL_EN(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_EN(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_GAIN(0x10)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_HZD_CORR_DIS(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_LP(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DMA_DTEST_EN(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_D_CH_FILT_BYPASS(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_FSK_ZB_SEL(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_IQ_SWAP(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_RSSI_EN(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_SAMPLE_BUF_DIS(0)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_EN(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_RATE(0),
#else
                XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_EN(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_RATE(1),
#endif /* RF_OSC_26MHZ == 1 */
    
    /* XCVR_TSM configs */
    /********************/

    /* CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .ctrl = XCVR_TSM_CTRL_ABORT_ON_CTUNE(0)|
                XCVR_TSM_CTRL_ABORT_ON_CYCLE_SLIP(0)|
                XCVR_TSM_CTRL_ABORT_ON_FREQ_TARG(0)|
                XCVR_TSM_CTRL_BKPT(0xFF)|
                XCVR_TSM_CTRL_FORCE_RX_EN(0)|
                XCVR_TSM_CTRL_FORCE_TX_EN(0)|
                XCVR_TSM_CTRL_RX_ABORT_DIS(0)|
                XCVR_TSM_CTRL_TSM_IRQ0_EN(0)|
                XCVR_TSM_CTRL_TSM_IRQ1_EN(0)|
                XCVR_TSM_CTRL_TSM_SOFT_RESET(0)|
                XCVR_TSM_CTRL_TX_ABORT_DIS(0),

    /* END_OF_SEQ configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .end_of_seq = XCVR_TSM_END_OF_SEQ_END_OF_RX_WD(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_END_OF_SEQ_END_OF_RX_WU(0x6C+PHY_EN_DELAY)|
                XCVR_TSM_END_OF_SEQ_END_OF_TX_WD(0x65)|
                XCVR_TSM_END_OF_SEQ_END_OF_TX_WU(0x64),
#else
    .end_of_seq = XCVR_TSM_END_OF_SEQ_END_OF_RX_WD(0x69+PHY_EN_DELAY)|
                XCVR_TSM_END_OF_SEQ_END_OF_RX_WU(0x68+PHY_EN_DELAY)|
                XCVR_TSM_END_OF_SEQ_END_OF_TX_WD(0x65)|
                XCVR_TSM_END_OF_SEQ_END_OF_TX_WU(0x64),
#endif /* RF_OSC_26MHZ == 1 */

    /* FAST_CTRL1 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .fast_ctrl1 = XCVR_TSM_FAST_CTRL1_FAST_RX2TX_EN(0)|
                XCVR_TSM_FAST_CTRL1_FAST_RX2TX_START(0x00)|
                XCVR_TSM_FAST_CTRL1_FAST_RX_WU_EN(0)|
                XCVR_TSM_FAST_CTRL1_FAST_TX_WU_EN(0)|
                XCVR_TSM_FAST_CTRL1_PWRSAVE_RX_WU_EN(0)|
                XCVR_TSM_FAST_CTRL1_PWRSAVE_TX_WU_EN(0)|
                XCVR_TSM_FAST_CTRL1_PWRSAVE_WU_CLEAR(0),

    /* FAST_CTRL2 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .fast_ctrl2 = XCVR_TSM_FAST_CTRL2_FAST_DEST_RX(0x6A)|
                XCVR_TSM_FAST_CTRL2_FAST_DEST_TX(0x42)|
                XCVR_TSM_FAST_CTRL2_FAST_START_RX(0x2C)|
                XCVR_TSM_FAST_CTRL2_FAST_START_TX(0x08),
#else
    .fast_ctrl2 = XCVR_TSM_FAST_CTRL2_FAST_DEST_RX(0x66)|
                XCVR_TSM_FAST_CTRL2_FAST_DEST_TX(0x42)|
                XCVR_TSM_FAST_CTRL2_FAST_START_RX(0x28)|
                XCVR_TSM_FAST_CTRL2_FAST_START_TX(0x08),
#endif /* RF_OSC_26MHZ == 1 */

    /* OVRD1 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .ovrd1 = XCVR_TSM_OVRD1_RX_BBA_Q_EN_OVRD(0),

    /* OVRD2 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .ovrd2 = XCVR_TSM_OVRD2_DCOC_EN_OVRD(0)|
                XCVR_TSM_OVRD2_DCOC_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_DCOC_INIT_OVRD(0)|
                XCVR_TSM_OVRD2_DCOC_INIT_OVRD_EN(0)|
                XCVR_TSM_OVRD2_FREQ_TARG_LD_EN_OVRD(0)|
                XCVR_TSM_OVRD2_FREQ_TARG_LD_EN_OVRD_EN(1)|
                XCVR_TSM_OVRD2_PLL_DIG_EN_OVRD(0)|
                XCVR_TSM_OVRD2_PLL_DIG_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_BBA_DCOC_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_BBA_DCOC_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_BBA_PDET_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_BBA_PDET_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_DIG_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_DIG_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_INIT_OVRD(0)|
                XCVR_TSM_OVRD2_RX_INIT_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_LNA_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_LNA_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_PHY_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_PHY_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_TZA_DCOC_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_TZA_DCOC_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_TZA_I_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_TZA_I_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_TZA_PDET_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_TZA_PDET_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_RX_TZA_Q_EN_OVRD(0)|
                XCVR_TSM_OVRD2_RX_TZA_Q_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_SIGMA_DELTA_EN_OVRD(0)|
                XCVR_TSM_OVRD2_SIGMA_DELTA_EN_OVRD_EN(0)|
                XCVR_TSM_OVRD2_TX_DIG_EN_OVRD(0)|
                XCVR_TSM_OVRD2_TX_DIG_EN_OVRD_EN(0),

    /* RECYCLE_COUNT configuration, dependencies: ['COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .recycle_count = XCVR_TSM_RECYCLE_COUNT_RECYCLE_COUNT0(0x6A)|
                XCVR_TSM_RECYCLE_COUNT_RECYCLE_COUNT1(0x06)|
                XCVR_TSM_RECYCLE_COUNT_RECYCLE_COUNT2(0x20),
#else
    .recycle_count = XCVR_TSM_RECYCLE_COUNT_RECYCLE_COUNT0(0x66)|
                XCVR_TSM_RECYCLE_COUNT_RECYCLE_COUNT1(0x06)|
                XCVR_TSM_RECYCLE_COUNT_RECYCLE_COUNT2(0x1C),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING00 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing00 = XCVR_TSM_TIMING00_BB_LDO_HF_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING00_BB_LDO_HF_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING00_BB_LDO_HF_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING00_BB_LDO_HF_EN_TX_LO(0x65),
#else
    .timing00 = XCVR_TSM_TIMING00_BB_LDO_HF_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING00_BB_LDO_HF_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING00_BB_LDO_HF_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING00_BB_LDO_HF_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING01 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing01 = XCVR_TSM_TIMING01_BB_LDO_ADCDAC_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING01_BB_LDO_ADCDAC_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING01_BB_LDO_ADCDAC_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING01_BB_LDO_ADCDAC_EN_TX_LO(0x65),
#else
    .timing01 = XCVR_TSM_TIMING01_BB_LDO_ADCDAC_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING01_BB_LDO_ADCDAC_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING01_BB_LDO_ADCDAC_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING01_BB_LDO_ADCDAC_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING02 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing02 = XCVR_TSM_TIMING02_BB_LDO_BBA_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING02_BB_LDO_BBA_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing02 = XCVR_TSM_TIMING02_BB_LDO_BBA_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING02_BB_LDO_BBA_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING03 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing03 = XCVR_TSM_TIMING03_BB_LDO_PD_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING03_BB_LDO_PD_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING03_BB_LDO_PD_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING03_BB_LDO_PD_EN_TX_LO(0x65),
#else
    .timing03 = XCVR_TSM_TIMING03_BB_LDO_PD_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING03_BB_LDO_PD_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING03_BB_LDO_PD_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING03_BB_LDO_PD_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING04 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing04 = XCVR_TSM_TIMING04_BB_LDO_FDBK_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING04_BB_LDO_FDBK_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING04_BB_LDO_FDBK_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING04_BB_LDO_FDBK_EN_TX_LO(0x65),
#else
    .timing04 = XCVR_TSM_TIMING04_BB_LDO_FDBK_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING04_BB_LDO_FDBK_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING04_BB_LDO_FDBK_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING04_BB_LDO_FDBK_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING05 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing05 = XCVR_TSM_TIMING05_BB_LDO_VCOLO_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING05_BB_LDO_VCOLO_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING05_BB_LDO_VCOLO_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING05_BB_LDO_VCOLO_EN_TX_LO(0x65),
#else
    .timing05 = XCVR_TSM_TIMING05_BB_LDO_VCOLO_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING05_BB_LDO_VCOLO_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING05_BB_LDO_VCOLO_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING05_BB_LDO_VCOLO_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING06 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing06 = XCVR_TSM_TIMING06_BB_LDO_VTREF_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING06_BB_LDO_VTREF_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING06_BB_LDO_VTREF_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING06_BB_LDO_VTREF_EN_TX_LO(0x65),
#else
    .timing06 = XCVR_TSM_TIMING06_BB_LDO_VTREF_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING06_BB_LDO_VTREF_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING06_BB_LDO_VTREF_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING06_BB_LDO_VTREF_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING07 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing07 = XCVR_TSM_TIMING07_BB_LDO_FDBK_BLEED_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING07_BB_LDO_FDBK_BLEED_EN_RX_LO(0x05)|
                XCVR_TSM_TIMING07_BB_LDO_FDBK_BLEED_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING07_BB_LDO_FDBK_BLEED_EN_TX_LO(0x05),

    /* TIMING08 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing08 = XCVR_TSM_TIMING08_BB_LDO_VCOLO_BLEED_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING08_BB_LDO_VCOLO_BLEED_EN_RX_LO(0x03)|
                XCVR_TSM_TIMING08_BB_LDO_VCOLO_BLEED_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING08_BB_LDO_VCOLO_BLEED_EN_TX_LO(0x03),

    /* TIMING09 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing09 = XCVR_TSM_TIMING09_BB_LDO_VCOLO_FASTCHARGE_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING09_BB_LDO_VCOLO_FASTCHARGE_EN_RX_LO(0x03)|
                XCVR_TSM_TIMING09_BB_LDO_VCOLO_FASTCHARGE_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING09_BB_LDO_VCOLO_FASTCHARGE_EN_TX_LO(0x03),

    /* TIMING10 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing10 = XCVR_TSM_TIMING10_BB_XTAL_PLL_REF_CLK_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING10_BB_XTAL_PLL_REF_CLK_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING10_BB_XTAL_PLL_REF_CLK_EN_TX_HI(0x03)|
                XCVR_TSM_TIMING10_BB_XTAL_PLL_REF_CLK_EN_TX_LO(0x65),
#else
    .timing10 = XCVR_TSM_TIMING10_BB_XTAL_PLL_REF_CLK_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING10_BB_XTAL_PLL_REF_CLK_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING10_BB_XTAL_PLL_REF_CLK_EN_TX_HI(0x03)|
                XCVR_TSM_TIMING10_BB_XTAL_PLL_REF_CLK_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING11 configuration, dependencies: ['COM', 'COM'] */
    .timing11 = XCVR_TSM_TIMING11_BB_XTAL_DAC_REF_CLK_EN_TX_HI(0x03)|
                XCVR_TSM_TIMING11_BB_XTAL_DAC_REF_CLK_EN_TX_LO(0x65),

    /* TIMING12 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing12 = XCVR_TSM_TIMING12_RXTX_AUXPLL_VCO_REF_CLK_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING12_RXTX_AUXPLL_VCO_REF_CLK_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing12 = XCVR_TSM_TIMING12_RXTX_AUXPLL_VCO_REF_CLK_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING12_RXTX_AUXPLL_VCO_REF_CLK_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING13 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing13 = XCVR_TSM_TIMING13_PLL_LOOP_IS_OPEN_RX_HI(0x00)|
                XCVR_TSM_TIMING13_PLL_LOOP_IS_OPEN_RX_LO(0x18)|
                XCVR_TSM_TIMING13_PLL_LOOP_IS_OPEN_TX_HI(0x00)|
                XCVR_TSM_TIMING13_PLL_LOOP_IS_OPEN_TX_LO(0x4C),

    /* TIMING14 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing14 = XCVR_TSM_TIMING14_SY_PD_CYCLE_SLIP_LD_FT_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING14_SY_PD_CYCLE_SLIP_LD_FT_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING14_SY_PD_CYCLE_SLIP_LD_FT_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING14_SY_PD_CYCLE_SLIP_LD_FT_EN_TX_LO(0xFF),
#else
    .timing14 = XCVR_TSM_TIMING14_SY_PD_CYCLE_SLIP_LD_FT_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING14_SY_PD_CYCLE_SLIP_LD_FT_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING14_SY_PD_CYCLE_SLIP_LD_FT_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING14_SY_PD_CYCLE_SLIP_LD_FT_EN_TX_LO(0xFF),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING15 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing15 = XCVR_TSM_TIMING15_SY_VCO_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING15_SY_VCO_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING15_SY_VCO_EN_TX_HI(0x03)|
                XCVR_TSM_TIMING15_SY_VCO_EN_TX_LO(0x65),
#else
    .timing15 = XCVR_TSM_TIMING15_SY_VCO_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING15_SY_VCO_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING15_SY_VCO_EN_TX_HI(0x03)|
                XCVR_TSM_TIMING15_SY_VCO_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING16 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing16 = XCVR_TSM_TIMING16_SY_LO_RX_BUF_EN_RX_HI(0x20)|
                XCVR_TSM_TIMING16_SY_LO_RX_BUF_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing16 = XCVR_TSM_TIMING16_SY_LO_RX_BUF_EN_RX_HI(0x1C)|
                XCVR_TSM_TIMING16_SY_LO_RX_BUF_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING17 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing17 = XCVR_TSM_TIMING17_SY_LO_TX_BUF_EN_TX_HI(0x05)|
                XCVR_TSM_TIMING17_SY_LO_TX_BUF_EN_TX_LO(0x65),
#else
    .timing17 = XCVR_TSM_TIMING17_SY_LO_TX_BUF_EN_TX_HI(0x05)|
                XCVR_TSM_TIMING17_SY_LO_TX_BUF_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING18 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing18 = XCVR_TSM_TIMING18_SY_DIVN_EN_RX_HI(0x05)|
                XCVR_TSM_TIMING18_SY_DIVN_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING18_SY_DIVN_EN_TX_HI(0x05)|
                XCVR_TSM_TIMING18_SY_DIVN_EN_TX_LO(0x65),
#else
    .timing18 = XCVR_TSM_TIMING18_SY_DIVN_EN_RX_HI(0x05)|
                XCVR_TSM_TIMING18_SY_DIVN_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING18_SY_DIVN_EN_TX_HI(0x05)|
                XCVR_TSM_TIMING18_SY_DIVN_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING19 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing19 = XCVR_TSM_TIMING19_SY_PD_FILTER_CHARGE_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING19_SY_PD_FILTER_CHARGE_EN_RX_LO(0x18)|
                XCVR_TSM_TIMING19_SY_PD_FILTER_CHARGE_EN_TX_HI(0x03)|
                XCVR_TSM_TIMING19_SY_PD_FILTER_CHARGE_EN_TX_LO(0x4C),

    /* TIMING20 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing20 = XCVR_TSM_TIMING20_SY_PD_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING20_SY_PD_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING20_SY_PD_EN_TX_HI(0x03)|
                XCVR_TSM_TIMING20_SY_PD_EN_TX_LO(0x65),
#else
    .timing20 = XCVR_TSM_TIMING20_SY_PD_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING20_SY_PD_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING20_SY_PD_EN_TX_HI(0x03)|
                XCVR_TSM_TIMING20_SY_PD_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING21 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing21 = XCVR_TSM_TIMING21_SY_LO_DIVN_EN_RX_HI(0x04)|
                XCVR_TSM_TIMING21_SY_LO_DIVN_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING21_SY_LO_DIVN_EN_TX_HI(0x04)|
                XCVR_TSM_TIMING21_SY_LO_DIVN_EN_TX_LO(0x65),
#else
    .timing21 = XCVR_TSM_TIMING21_SY_LO_DIVN_EN_RX_HI(0x04)|
                XCVR_TSM_TIMING21_SY_LO_DIVN_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING21_SY_LO_DIVN_EN_TX_HI(0x04)|
                XCVR_TSM_TIMING21_SY_LO_DIVN_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING22 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing22 = XCVR_TSM_TIMING22_SY_LO_RX_EN_RX_HI(0x04)|
                XCVR_TSM_TIMING22_SY_LO_RX_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing22 = XCVR_TSM_TIMING22_SY_LO_RX_EN_RX_HI(0x04)|
                XCVR_TSM_TIMING22_SY_LO_RX_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING23 configuration, dependencies: ['COM', 'COM'] */
    .timing23 = XCVR_TSM_TIMING23_SY_LO_TX_EN_TX_HI(0x04)|
                XCVR_TSM_TIMING23_SY_LO_TX_EN_TX_LO(0x65),

    /* TIMING24 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing24 = XCVR_TSM_TIMING24_SY_DIVN_CAL_EN_RX_HI(0x00)|
                XCVR_TSM_TIMING24_SY_DIVN_CAL_EN_RX_LO(0x18)|
                XCVR_TSM_TIMING24_SY_DIVN_CAL_EN_TX_HI(0x00)|
                XCVR_TSM_TIMING24_SY_DIVN_CAL_EN_TX_LO(0x4C),

    /* TIMING25 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing25 = XCVR_TSM_TIMING25_RX_LNA_MIXER_EN_RX_HI(0x21)|
                XCVR_TSM_TIMING25_RX_LNA_MIXER_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing25 = XCVR_TSM_TIMING25_RX_LNA_MIXER_EN_RX_HI(0x1D)|
                XCVR_TSM_TIMING25_RX_LNA_MIXER_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING27 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing27 = XCVR_TSM_TIMING27_RX_ADC_I_Q_EN_RX_HI(0x24)|
                XCVR_TSM_TIMING27_RX_ADC_I_Q_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing27 = XCVR_TSM_TIMING27_RX_ADC_I_Q_EN_RX_HI(0x20)|
                XCVR_TSM_TIMING27_RX_ADC_I_Q_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING28 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing28 = XCVR_TSM_TIMING28_RX_ADC_RESET_EN_RX_HI(0x24)|
                XCVR_TSM_TIMING28_RX_ADC_RESET_EN_RX_LO(0x25),
#else
    .timing28 = XCVR_TSM_TIMING28_RX_ADC_RESET_EN_RX_HI(0x20)|
                XCVR_TSM_TIMING28_RX_ADC_RESET_EN_RX_LO(0x21),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING29 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing29 = XCVR_TSM_TIMING29_RX_BBA_I_Q_EN_RX_HI(0x22)|
                XCVR_TSM_TIMING29_RX_BBA_I_Q_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing29 = XCVR_TSM_TIMING29_RX_BBA_I_Q_EN_RX_HI(0x1E)|
                XCVR_TSM_TIMING29_RX_BBA_I_Q_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING30 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing30 = XCVR_TSM_TIMING30_RX_BBA_PDET_EN_RX_HI(0x24)|
                XCVR_TSM_TIMING30_RX_BBA_PDET_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing30 = XCVR_TSM_TIMING30_RX_BBA_PDET_EN_RX_HI(0x20)|
                XCVR_TSM_TIMING30_RX_BBA_PDET_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING31 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing31 = XCVR_TSM_TIMING31_RX_BBA_TZA_DCOC_EN_RX_HI(0x23)|
                XCVR_TSM_TIMING31_RX_BBA_TZA_DCOC_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing31 = XCVR_TSM_TIMING31_RX_BBA_TZA_DCOC_EN_RX_HI(0x1F)|
                XCVR_TSM_TIMING31_RX_BBA_TZA_DCOC_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING32 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing32 = XCVR_TSM_TIMING32_RX_TZA_I_Q_EN_RX_HI(0x21)|
                XCVR_TSM_TIMING32_RX_TZA_I_Q_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing32 = XCVR_TSM_TIMING32_RX_TZA_I_Q_EN_RX_HI(0x1D)|
                XCVR_TSM_TIMING32_RX_TZA_I_Q_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING33 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing33 = XCVR_TSM_TIMING33_RX_TZA_PDET_EN_RX_HI(0x24)|
                XCVR_TSM_TIMING33_RX_TZA_PDET_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing33 = XCVR_TSM_TIMING33_RX_TZA_PDET_EN_RX_HI(0x20)|
                XCVR_TSM_TIMING33_RX_TZA_PDET_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING34 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing34 = XCVR_TSM_TIMING34_PLL_DIG_EN_RX_HI(0x07)|
                XCVR_TSM_TIMING34_PLL_DIG_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING34_PLL_DIG_EN_TX_HI(0x07)|
                XCVR_TSM_TIMING34_PLL_DIG_EN_TX_LO(0x65),
#else
    .timing34 = XCVR_TSM_TIMING34_PLL_DIG_EN_RX_HI(0x07)|
                XCVR_TSM_TIMING34_PLL_DIG_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING34_PLL_DIG_EN_TX_HI(0x07)|
                XCVR_TSM_TIMING34_PLL_DIG_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING35 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing35 = XCVR_TSM_TIMING35_TX_DIG_EN_TX_HI(0x5F)|
                XCVR_TSM_TIMING35_TX_DIG_EN_TX_LO(0x65),
#else
    .timing35 = XCVR_TSM_TIMING35_TX_DIG_EN_TX_HI(0x5F)|
                XCVR_TSM_TIMING35_TX_DIG_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING36 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing36 = XCVR_TSM_TIMING36_RX_DIG_EN_RX_HI(0x6A)|
                XCVR_TSM_TIMING36_RX_DIG_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing36 = XCVR_TSM_TIMING36_RX_DIG_EN_RX_HI(0x66)|
                XCVR_TSM_TIMING36_RX_DIG_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING37 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing37 = XCVR_TSM_TIMING37_RX_INIT_RX_HI(0x6A)|
                XCVR_TSM_TIMING37_RX_INIT_RX_LO(0x6B),
#else
    .timing37 = XCVR_TSM_TIMING37_RX_INIT_RX_HI(0x66)|
                XCVR_TSM_TIMING37_RX_INIT_RX_LO(0x67),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING38 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing38 = XCVR_TSM_TIMING38_SIGMA_DELTA_EN_RX_HI(0x0E)|
                XCVR_TSM_TIMING38_SIGMA_DELTA_EN_RX_LO(0x6D+PHY_EN_DELAY)|
                XCVR_TSM_TIMING38_SIGMA_DELTA_EN_TX_HI(0x42)|
                XCVR_TSM_TIMING38_SIGMA_DELTA_EN_TX_LO(0x65),
#else
    .timing38 = XCVR_TSM_TIMING38_SIGMA_DELTA_EN_RX_HI(0x0E)|
                XCVR_TSM_TIMING38_SIGMA_DELTA_EN_RX_LO(0x69+PHY_EN_DELAY)|
                XCVR_TSM_TIMING38_SIGMA_DELTA_EN_TX_HI(0x42)|
                XCVR_TSM_TIMING38_SIGMA_DELTA_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING39 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing39 = XCVR_TSM_TIMING39_RX_PHY_EN_RX_HI(0x6A+PHY_EN_DELAY)|
                XCVR_TSM_TIMING39_RX_PHY_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing39 = XCVR_TSM_TIMING39_RX_PHY_EN_RX_HI(0x66+PHY_EN_DELAY)|
                XCVR_TSM_TIMING39_RX_PHY_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING40 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing40 = XCVR_TSM_TIMING40_DCOC_EN_RX_HI(0x2A)|
                XCVR_TSM_TIMING40_DCOC_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing40 = XCVR_TSM_TIMING40_DCOC_EN_RX_HI(0x26)|
                XCVR_TSM_TIMING40_DCOC_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING41 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing41 = XCVR_TSM_TIMING41_DCOC_INIT_RX_HI(0x2A)|
                XCVR_TSM_TIMING41_DCOC_INIT_RX_LO(0x2B),
#else
    .timing41 = XCVR_TSM_TIMING41_DCOC_INIT_RX_HI(0x26)|
                XCVR_TSM_TIMING41_DCOC_INIT_RX_LO(0x27),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING42 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing42 = XCVR_TSM_TIMING42_SAR_ADC_TRIG_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING42_SAR_ADC_TRIG_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING42_SAR_ADC_TRIG_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING42_SAR_ADC_TRIG_EN_TX_LO(0xFF),

    /* TIMING43 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing43 = XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING43_TSM_SPARE0_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING43_TSM_SPARE0_EN_TX_LO(0xFF),

    /* TIMING44 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing44 = XCVR_TSM_TIMING44_TSM_SPARE1_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING44_TSM_SPARE1_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING44_TSM_SPARE1_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING44_TSM_SPARE1_EN_TX_LO(0xFF),

    /* TIMING45 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing45 = XCVR_TSM_TIMING45_TSM_SPARE2_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING45_TSM_SPARE2_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING45_TSM_SPARE2_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING45_TSM_SPARE2_EN_TX_LO(0xFF),

    /* TIMING46 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing46 = XCVR_TSM_TIMING46_TSM_SPARE3_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING46_TSM_SPARE3_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING46_TSM_SPARE3_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING46_TSM_SPARE3_EN_TX_LO(0xFF),

    /* TIMING47 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing47 = XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING47_GPIO0_TRIG_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING47_GPIO0_TRIG_EN_TX_LO(0xFF),

    /* TIMING48 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing48 = XCVR_TSM_TIMING48_GPIO1_TRIG_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING48_GPIO1_TRIG_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING48_GPIO1_TRIG_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING48_GPIO1_TRIG_EN_TX_LO(0xFF),

    /* TIMING49 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing49 = XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING49_GPIO2_TRIG_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING49_GPIO2_TRIG_EN_TX_LO(0xFF),

    /* TIMING50 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .timing50 = XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_HI(0xFF)|
                XCVR_TSM_TIMING50_GPIO3_TRIG_EN_RX_LO(0xFF)|
                XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_HI(0xFF)|
                XCVR_TSM_TIMING50_GPIO3_TRIG_EN_TX_LO(0xFF),

    /* TIMING51 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing51 = XCVR_TSM_TIMING51_RXTX_AUXPLL_BIAS_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING51_RXTX_AUXPLL_BIAS_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing51 = XCVR_TSM_TIMING51_RXTX_AUXPLL_BIAS_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING51_RXTX_AUXPLL_BIAS_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING52 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing52 = XCVR_TSM_TIMING52_RXTX_AUXPLL_FCAL_EN_RX_HI(0x06)|
                XCVR_TSM_TIMING52_RXTX_AUXPLL_FCAL_EN_RX_LO(0x1B),
#else
    .timing52 = XCVR_TSM_TIMING52_RXTX_AUXPLL_FCAL_EN_RX_HI(0x06)|
                XCVR_TSM_TIMING52_RXTX_AUXPLL_FCAL_EN_RX_LO(0x17),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING53 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing53 = XCVR_TSM_TIMING53_RXTX_AUXPLL_LF_PD_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING53_RXTX_AUXPLL_LF_PD_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing53 = XCVR_TSM_TIMING53_RXTX_AUXPLL_LF_PD_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING53_RXTX_AUXPLL_LF_PD_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING54 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing54 = XCVR_TSM_TIMING54_RXTX_AUXPLL_PD_LF_FILTER_CHARGE_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING54_RXTX_AUXPLL_PD_LF_FILTER_CHARGE_EN_RX_LO(0x1B),
#else
    .timing54 = XCVR_TSM_TIMING54_RXTX_AUXPLL_PD_LF_FILTER_CHARGE_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING54_RXTX_AUXPLL_PD_LF_FILTER_CHARGE_EN_RX_LO(0x17),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING55 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing55 = XCVR_TSM_TIMING55_RXTX_AUXPLL_ADC_BUF_EN_RX_HI(0x24)|
                XCVR_TSM_TIMING55_RXTX_AUXPLL_ADC_BUF_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing55 = XCVR_TSM_TIMING55_RXTX_AUXPLL_ADC_BUF_EN_RX_HI(0x20)|
                XCVR_TSM_TIMING55_RXTX_AUXPLL_ADC_BUF_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING56 configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .timing56 = XCVR_TSM_TIMING56_RXTX_AUXPLL_DIG_BUF_EN_RX_HI(0x24)|
                XCVR_TSM_TIMING56_RXTX_AUXPLL_DIG_BUF_EN_RX_LO(0x6D+PHY_EN_DELAY),
#else
    .timing56 = XCVR_TSM_TIMING56_RXTX_AUXPLL_DIG_BUF_EN_RX_HI(0x20)|
                XCVR_TSM_TIMING56_RXTX_AUXPLL_DIG_BUF_EN_RX_LO(0x69+PHY_EN_DELAY),
#endif /* RF_OSC_26MHZ == 1 */

    /* TIMING57 configuration, dependencies: ['COM', 'COM'] */
    .timing57 = XCVR_TSM_TIMING57_RXTX_RCCAL_EN_RX_HI(0x03)|
                XCVR_TSM_TIMING57_RXTX_RCCAL_EN_RX_LO(0x1A),

    /* TIMING58 configuration, dependencies: ['COM', 'COM'] */
    .timing58 = XCVR_TSM_TIMING58_TX_HPM_DAC_EN_TX_HI(0x03)|
#if RF_OSC_26MHZ == 1
                XCVR_TSM_TIMING58_TX_HPM_DAC_EN_TX_LO(0x65),
#else
                XCVR_TSM_TIMING58_TX_HPM_DAC_EN_TX_LO(0x65),
#endif /* RF_OSC_26MHZ == 1 */

    /* WU_LATENCY configuration, dependencies: ['COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .wu_latency = XCVR_TSM_WU_LATENCY_RX_SETTLING_LATENCY(0x2A)|
#else
    .wu_latency = XCVR_TSM_WU_LATENCY_RX_SETTLING_LATENCY(0x2E)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_TSM_WU_LATENCY_TX_DATAPATH_LATENCY(0x08),
    
    /* XCVR_TX_DIG configs */
    /***********************/

    /* DATA_PADDING_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .data_padding_ctrl = XCVR_TX_DIG_DATA_PADDING_CTRL_CTE_DATA(1)|
                XCVR_TX_DIG_DATA_PADDING_CTRL_DATA_PADDING_SEL(0)|
                XCVR_TX_DIG_DATA_PADDING_CTRL_PAD_DLY(0x00)|
                XCVR_TX_DIG_DATA_PADDING_CTRL_PAD_DLY_EN(1)|
                XCVR_TX_DIG_DATA_PADDING_CTRL_RAMP_DN_PAD_EN(0)|
                XCVR_TX_DIG_DATA_PADDING_CTRL_TX_CAPTURE_POL(0),

    /* DATA_PADDING_CTRL_1 configuration, dependencies: ['COM', 'COM', 'MD+DR'] */
    .data_padding_ctrl_1 = XCVR_TX_DIG_DATA_PADDING_CTRL_1_PA_PUP_ADJ(0x0)|
                XCVR_TX_DIG_DATA_PADDING_CTRL_1_RAMP_UP_DLY(0x06),

    /* DATA_PADDING_CTRL_2 configuration, dependencies: ['COM', 'COM'] */
    .data_padding_ctrl_2 = XCVR_TX_DIG_DATA_PADDING_CTRL_2_DATA_PAD_MFDEV(0x0000)|
                XCVR_TX_DIG_DATA_PADDING_CTRL_2_DATA_PAD_PFDEV(0x0000),

    /* GFSK_CTRL configuration, dependencies: ['COM', 'COM', 'MD+DR'] */
    .gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_BT_EQ_OR_GTR_ONE(0)|
                XCVR_TX_DIG_GFSK_CTRL_GFSK_COEFF_MAN(1),

    /* IMAGE_FILTER_CTRL configuration, dependencies: ['COM', 'MD+DR', 'COM', 'MD+DR', 'COM', 'COM'] */
    .image_filter_ctrl = XCVR_TX_DIG_IMAGE_FILTER_CTRL_FREQ_WORD_ADJ(0x0000)|
                XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_FIR_FILTER_OVRD(0)|
                XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_SYNC0_FILTER_OVRD(0)|
                XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_SYNC1_FILTER_OVRD(0),

    /* PA_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM'] */
    .pa_ctrl = XCVR_TX_DIG_PA_CTRL_PA_RAMP_SEL(1)|
                XCVR_TX_DIG_PA_CTRL_PA_TGT_POWER(0x28)|
                XCVR_TX_DIG_PA_CTRL_TGT_PWR_SRC(1)|
                XCVR_TX_DIG_PA_CTRL_TX_PA_PUP_OVRD(0)|
                XCVR_TX_DIG_PA_CTRL_TX_PA_PUP_OVRD_EN(0),

    /* PA_RAMP_TBL0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .pa_ramp_tbl0 = XCVR_TX_DIG_PA_RAMP_TBL0_PA_RAMP0(0x01)|
                XCVR_TX_DIG_PA_RAMP_TBL0_PA_RAMP1(0x02)|
                XCVR_TX_DIG_PA_RAMP_TBL0_PA_RAMP2(0x04)|
                XCVR_TX_DIG_PA_RAMP_TBL0_PA_RAMP3(0x06),

    /* PA_RAMP_TBL1 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .pa_ramp_tbl1 = XCVR_TX_DIG_PA_RAMP_TBL1_PA_RAMP4(0x08)|
                XCVR_TX_DIG_PA_RAMP_TBL1_PA_RAMP5(0x0C)|
                XCVR_TX_DIG_PA_RAMP_TBL1_PA_RAMP6(0x10)|
                XCVR_TX_DIG_PA_RAMP_TBL1_PA_RAMP7(0x14),

    /* PA_RAMP_TBL2 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .pa_ramp_tbl2 = XCVR_TX_DIG_PA_RAMP_TBL2_PA_RAMP10(0x22)|
                XCVR_TX_DIG_PA_RAMP_TBL2_PA_RAMP11(0x26)|
                XCVR_TX_DIG_PA_RAMP_TBL2_PA_RAMP8(0x18)|
                XCVR_TX_DIG_PA_RAMP_TBL2_PA_RAMP9(0x1C),

    /* PA_RAMP_TBL3 configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .pa_ramp_tbl3 = XCVR_TX_DIG_PA_RAMP_TBL3_PA_RAMP12(0x2C)|
                XCVR_TX_DIG_PA_RAMP_TBL3_PA_RAMP13(0x32)|
                XCVR_TX_DIG_PA_RAMP_TBL3_PA_RAMP14(0x38)|
                XCVR_TX_DIG_PA_RAMP_TBL3_PA_RAMP15(0x3C),

    /* SWITCH_TX_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM'] */
    .switch_tx_ctrl = XCVR_TX_DIG_SWITCH_TX_CTRL_SWITCH_FIR_SEL(0)|
                XCVR_TX_DIG_SWITCH_TX_CTRL_SWITCH_GFSK_COEFF(0)|
                XCVR_TX_DIG_SWITCH_TX_CTRL_SWITCH_MOD(0)|
                XCVR_TX_DIG_SWITCH_TX_CTRL_SWITCH_TGT_PWR(0x00),

    /* TXDIG_CTRL configuration, dependencies: ['COM', 'MD+DR', 'MD+DR'] */
    .txdig_ctrl = XCVR_TX_DIG_TXDIG_CTRL_DATA_STREAM_SEL(0),
    /***********************************************/
    /************ END OF GENERATED CODE ************/
    /************** xcvr_common_config *************/
    /***********************************************/
};




