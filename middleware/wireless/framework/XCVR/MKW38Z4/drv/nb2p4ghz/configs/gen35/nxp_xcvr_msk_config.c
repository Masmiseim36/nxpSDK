/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nxp_xcvr_msk_config.h"

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

/* MODE& DATA RATE combined configuration */
const xcvr_mode_datarate_config_t xcvr_msk_1mbps_config =
{
    /***********************************************/
    /*********** START OF GENERATED CODE ***********/
    /************ xcvr_msk_1mbps_config ************/
    /***********************************************/
    .radio_mode = MSK,
    .data_rate = DR_1MBPS,
    .alt_data_rate = DR_2MBPS,
    
    /* RADIO_CTRL configs */
    /**********************/

    /* LL_CTRL configuration, dependencies: ['MD'] */
    .ll_ctrl = RADIO_CTRL_LL_CTRL_ACTIVE_LL(2),
    
    /* XCVR_2P4GHZ_PHY configs */
    /***************************/

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x10)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x30),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x28)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x24),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x10)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x38),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x04),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x08),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x3C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x10),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x18)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x1C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x30),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x28)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x24),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x38),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x04),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x30)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x08),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x3C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x30)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x10),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x18)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x1C),

    /* FSK_CFG0 configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'MD', 'MD'] */
    .fsk_cfg0 = XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_1M(0x17)|
                XCVR_2P4GHZ_PHY_FSK_CFG0_MSK2FSK_SEED(1)|
#if RF_OSC_26MHZ == 1
                XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_2M(0x19)|
#else
                XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_2M(0x19)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_2P4GHZ_PHY_FSK_CFG0_MSK_EN(1),

    /* FSK_CFG1 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .fsk_cfg1 = XCVR_2P4GHZ_PHY_FSK_CFG1_OVERH(0x0040)|
                XCVR_2P4GHZ_PHY_FSK_CFG1_OVERH_INV(0x0100)|
                XCVR_2P4GHZ_PHY_FSK_CFG1_SYNCTSCALE(0x00),

    /* FSK_CFG2 configuration, dependencies: ['MD+DR', 'MD+DR', 'COM'] */
    .fsk_cfg2 = XCVR_2P4GHZ_PHY_FSK_CFG2_MAG_THRESH_1M(0x00D0)|
                XCVR_2P4GHZ_PHY_FSK_CFG2_MAG_THRESH_HI_1M(0x01D0),

    /* FSK_CFG3 configuration, dependencies: ['MD+DR', 'MD+DR'] */
    .fsk_cfg3 = XCVR_2P4GHZ_PHY_FSK_CFG3_MAG_THRESH_2M(0x0118)|
                XCVR_2P4GHZ_PHY_FSK_CFG3_MAG_THRESH_HI_2M(0x0280),

    /* FSK_PD_CFG0 configuration, dependencies: ['MD', 'MD'] */
    .fsk_pd_cfg0 = XCVR_2P4GHZ_PHY_FSK_PD_CFG0_PD_IIR_ALPHA(0xFF)|
                XCVR_2P4GHZ_PHY_FSK_PD_CFG0_PREAMBLE_T_SCALE(0x0C),

    /* FSK_PD_CFG1 configuration, dependencies: ['MD'] */
    .fsk_pd_cfg1 = XCVR_2P4GHZ_PHY_FSK_PD_CFG1_PREAMBLE_PATTERN(0x55),

    /* FSK_PD_CFG2 configuration, dependencies: ['MD+DR', 'MD+DR'] */
    .fsk_pd_cfg2 = XCVR_2P4GHZ_PHY_FSK_PD_CFG2_PD_THRESH_ACQ_1_3_1M(0xEB)|
                XCVR_2P4GHZ_PHY_FSK_PD_CFG2_PD_THRESH_ACQ_1_3_2M(0xEB),

    /* FSK_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .fsk_pd_ph[0] = XCVR_2P4GHZ_PHY_FSK_PD_PH_REF0(0x03)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF1(0x05)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF2(0x03)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF3(0x00),

    /* FSK_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .fsk_pd_ph[1] = XCVR_2P4GHZ_PHY_FSK_PD_PH_REF0(0x3D)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF1(0x3B)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF2(0x3D)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF3(0x00),

    /* LR_AA_CFG configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_aa_cfg = XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_COR_THRESH(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_HAM_THRESH(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_LR_CORR_GAIN(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_ACCESS_ADDR_HAM(0x00),

    /* LR_PD_CFG configuration, dependencies: ['MD', 'MD', 'MD'] */
    .lr_pd_cfg = XCVR_2P4GHZ_PHY_LR_PD_CFG_CORR_TH(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_CFG_FREQ_TH(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_CFG_NO_PEAKS(0),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[0] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[1] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[2] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[3] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* SM_CFG configuration, dependencies: ['MD', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .sm_cfg = XCVR_2P4GHZ_PHY_SM_CFG_AA_TIMEOUT_UNCODED(0x00A0),
    
    /* XCVR_ANALOG configs */
    /***********************/

    /* RX_BBA configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .rx_bba = XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_DRS(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_DRS(0),

    /* RX_TZA configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM'] */
    .rx_tza = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(0),

    /* SY_CTRL_2 configuration, dependencies: ['COM', 'COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM'] */
    .sy_ctrl_2 = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0)|
                XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_DRS(1),
    
    /* XCVR_MISC configs */
    /*********************/

    /* XCVR_CTRL configuration, dependencies: [nan, nan, nan, nan, 'COM', nan, nan, 'DR', 'DR'] */
    .xcvr_ctrl = XCVR_MISC_XCVR_CTRL_TX_DATA_RATE(1)|
                XCVR_MISC_XCVR_CTRL_TX_DATA_RATE_DRS(0),
    
    /* XCVR_PLL_DIG configs */
    /************************/

    /* CHAN_MAP configuration, dependencies: ['MD', 'COM', 'COM', 'COM', 'COM'] */
    .chan_map = XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT(7),

    /* DATA_RATE_OVRD_CTRL1 configuration, dependencies: ['MD+DR', 'COM', 'COM', 'COM'] */
    .data_rate_ovrd_ctrl1 = XCVR_PLL_DIG_DATA_RATE_OVRD_CTRL1_HPM_CAL_SCALE_CFG1(0x0A),
    
    /* XCVR_PLL_DIG2 configs */
    /*************************/

    /* HPMCAL_CTRL configuration, dependencies: ['MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .hpmcal_ctrl = XCVR_PLL_DIG2_HPMCAL_CTRL_HPM_CAL_ARRAY_SIZE(1),
    
    /* XCVR_RX_DIG configs */
    /***********************/

    /* AGC_CTRL_0 configuration, dependencies: ['COM', 'COM', 'MD+DR', 'COM', 'COM', 'DR', 'MD+DR', 'MD+DR', 'COM', 'COM'] */
    .agc_ctrl_0 = XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_EN(1)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xE0)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(1),

    /* AGC_CTRL_0_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .agc_ctrl_0_drs = XCVR_RX_DIG_AGC_CTRL_0_DRS_AGC_DOWN_RSSI_THRESH(0x05)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_AGC_UP_RSSI_THRESH(0xE0)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_AGC_CTRL_0_DRS_BBA_GAIN_SETTLE_TIME(0x07)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_LNA_GAIN_SETTLE_TIME(0x07),
#else
                XCVR_RX_DIG_AGC_CTRL_0_DRS_BBA_GAIN_SETTLE_TIME(0x08)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_LNA_GAIN_SETTLE_TIME(0x08),
#endif /* RF_OSC_26MHZ == 1 */

    /* AGC_CTRL_1 configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .agc_ctrl_1 = XCVR_RX_DIG_AGC_CTRL_1_LNA_GAIN_SETTLE_TIME(0x0C),
#else
    .agc_ctrl_1 = XCVR_RX_DIG_AGC_CTRL_1_LNA_GAIN_SETTLE_TIME(0x0A),
#endif /* RF_OSC_26MHZ == 1 */

    /* AGC_CTRL_1_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'COM'] */
    .agc_ctrl_1_drs = XCVR_RX_DIG_AGC_CTRL_1_DRS_AGC_FAST_EXPIRE(0x09)|
                XCVR_RX_DIG_AGC_CTRL_1_DRS_AGC_UNFREEZE_TIME(0x82),

    /* AGC_CTRL_2 configuration, dependencies: ['DR', 'MD+DR', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR'] */
    .agc_ctrl_2 = XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(0x0A)|
                XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(3)|
                XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(4)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(0x0B)|
#else
                XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(0x0E)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(2),

    /* AGC_CTRL_2_DRS configuration, dependencies: ['MD+DR', 'COM'] */
    .agc_ctrl_2_drs = XCVR_RX_DIG_AGC_CTRL_2_DRS_BBA_PDET_SEL_HI(3),

    /* AGC_CTRL_3 configuration, dependencies: ['COM', 'COM', 'COM', 'MD+DR', 'COM'] */
    .agc_ctrl_3 = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(0x82),

    /* AGC_CTRL_4 configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'MD+DR'] */
    .agc_ctrl_4 = XCVR_RX_DIG_AGC_CTRL_4_BBA_PDET_SEL_HI_HOLD(3)|
                XCVR_RX_DIG_AGC_CTRL_4_TZA_PDET_SEL_HI_HOLD(3),

    /* DCOC_CAL_IIR configuration, dependencies: ['DR', 'DR', 'DR'] */
#if RF_OSC_26MHZ == 1
    .dcoc_cal_iir = XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR1A_IDX(0)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR2A_IDX(2)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR3A_IDX(2),
#else
    .dcoc_cal_iir = XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR1A_IDX(1)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR2A_IDX(2)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR3A_IDX(1),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .dcoc_ctrl_0 = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_DLY(0x0A)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(0x07),
#else
    .dcoc_ctrl_0 = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_DLY(0x0C)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(0x08),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_1 configuration, dependencies: ['COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM'] */
    .dcoc_ctrl_1 = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_ALT_IDX(5)|
                XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),

    /* DCOC_CTRL_2 configuration, dependencies: ['DR', 'DR'] */
#if RF_OSC_26MHZ == 1
    .dcoc_ctrl_2 = XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_DLY_ALT(0x0A)|
                XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_HOLD_TIME_ALT(0x07),
#else
    .dcoc_ctrl_2 = XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_DLY_ALT(0x0C)|
                XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_HOLD_TIME_ALT(0x08),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR'] */
    .dcoc_ctrl_drs = XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_ALPHA_RADIUS_IDX(3)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY(0x08)|
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY_ALT(0x08),
#else
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY(0x0A)|
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY_ALT(0x0A),
#endif /* RF_OSC_26MHZ == 1 */

    /* DC_RESID_CTRL configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'MD+DR', 'DR', 'MD+DR', 'COM'] */
    .dc_resid_ctrl = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_DLY(4)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_ITER_FREEZE(0x03)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0x04)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(0x3C),

    /* DC_RESID_CTRL_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR'] */
    .dc_resid_ctrl_drs = XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_DLY(6)|
                XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_NWIN(0x60)|
                XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_NWIN2(0x060),

    /* RX_CHF_COEF_0 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_0 = XCVR_RX_DIG_RX_CHF_COEF_0_RX_CH_FILT_H0(0x0000),
#else
    .rx_chf_coef_0 = XCVR_RX_DIG_RX_CHF_COEF_0_RX_CH_FILT_H0(0x0002),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_0_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_0_drs = XCVR_RX_DIG_RX_CHF_COEF_0_DRS_RX_CH_FILT_H0(0xFFF9),
#else
    .rx_chf_coef_0_drs = XCVR_RX_DIG_RX_CHF_COEF_0_DRS_RX_CH_FILT_H0(0xFFFD),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_1 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_1 = XCVR_RX_DIG_RX_CHF_COEF_1_RX_CH_FILT_H1(0xFFF8),
#else
    .rx_chf_coef_1 = XCVR_RX_DIG_RX_CHF_COEF_1_RX_CH_FILT_H1(0xFFFD),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_10 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_10 = XCVR_RX_DIG_RX_CHF_COEF_10_RX_CH_FILT_H10(0x007B),
#else
    .rx_chf_coef_10 = XCVR_RX_DIG_RX_CHF_COEF_10_RX_CH_FILT_H10(0x000A),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_10_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_10_drs = XCVR_RX_DIG_RX_CHF_COEF_10_DRS_RX_CH_FILT_H10(0x0098),
#else
    .rx_chf_coef_10_drs = XCVR_RX_DIG_RX_CHF_COEF_10_DRS_RX_CH_FILT_H10(0x0019),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_11 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_11 = XCVR_RX_DIG_RX_CHF_COEF_11_RX_CH_FILT_H11(0x00DA),
#else
    .rx_chf_coef_11 = XCVR_RX_DIG_RX_CHF_COEF_11_RX_CH_FILT_H11(0x014F),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_11_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_11_drs = XCVR_RX_DIG_RX_CHF_COEF_11_DRS_RX_CH_FILT_H11(0x010F),
#else
    .rx_chf_coef_11_drs = XCVR_RX_DIG_RX_CHF_COEF_11_DRS_RX_CH_FILT_H11(0x0196),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_1_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_1_drs = XCVR_RX_DIG_RX_CHF_COEF_1_DRS_RX_CH_FILT_H1(0xFFF2),
#else
    .rx_chf_coef_1_drs = XCVR_RX_DIG_RX_CHF_COEF_1_DRS_RX_CH_FILT_H1(0xFFF9),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_2 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_2 = XCVR_RX_DIG_RX_CHF_COEF_2_RX_CH_FILT_H2(0xFFF5),
#else
    .rx_chf_coef_2 = XCVR_RX_DIG_RX_CHF_COEF_2_RX_CH_FILT_H2(0xFFF9),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_2_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_2_drs = XCVR_RX_DIG_RX_CHF_COEF_2_DRS_RX_CH_FILT_H2(0xFFF9),
#else
    .rx_chf_coef_2_drs = XCVR_RX_DIG_RX_CHF_COEF_2_DRS_RX_CH_FILT_H2(0x000A),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_3 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_3 = XCVR_RX_DIG_RX_CHF_COEF_3_RX_CH_FILT_H3(0x0001),
#else
    .rx_chf_coef_3 = XCVR_RX_DIG_RX_CHF_COEF_3_RX_CH_FILT_H3(0x000C),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_3_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_3_drs = XCVR_RX_DIG_RX_CHF_COEF_3_DRS_RX_CH_FILT_H3(0x0013),
#else
    .rx_chf_coef_3_drs = XCVR_RX_DIG_RX_CHF_COEF_3_DRS_RX_CH_FILT_H3(0x0014),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_4 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_4 = XCVR_RX_DIG_RX_CHF_COEF_4_RX_CH_FILT_H4(0x0016),
#else
    .rx_chf_coef_4 = XCVR_RX_DIG_RX_CHF_COEF_4_RX_CH_FILT_H4(0x000B),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_4_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_4_drs = XCVR_RX_DIG_RX_CHF_COEF_4_DRS_RX_CH_FILT_H4(0x002A),
#else
    .rx_chf_coef_4_drs = XCVR_RX_DIG_RX_CHF_COEF_4_DRS_RX_CH_FILT_H4(0xFFE9),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_5 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_5 = XCVR_RX_DIG_RX_CHF_COEF_5_RX_CH_FILT_H5(0x001A),
#else
    .rx_chf_coef_5 = XCVR_RX_DIG_RX_CHF_COEF_5_RX_CH_FILT_H5(0xFFE1),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_5_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_5_drs = XCVR_RX_DIG_RX_CHF_COEF_5_DRS_RX_CH_FILT_H5(0x001C),
#else
    .rx_chf_coef_5_drs = XCVR_RX_DIG_RX_CHF_COEF_5_DRS_RX_CH_FILT_H5(0xFFD3),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_6 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_6 = XCVR_RX_DIG_RX_CHF_COEF_6_RX_CH_FILT_H6(0xFFFB),
#else
    .rx_chf_coef_6 = XCVR_RX_DIG_RX_CHF_COEF_6_RX_CH_FILT_H6(0xFFF5),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_6_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_6_drs = XCVR_RX_DIG_RX_CHF_COEF_6_DRS_RX_CH_FILT_H6(0xFFE1),
#else
    .rx_chf_coef_6_drs = XCVR_RX_DIG_RX_CHF_COEF_6_DRS_RX_CH_FILT_H6(0x0029),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_7 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_7 = XCVR_RX_DIG_RX_CHF_COEF_7_RX_CH_FILT_H7(0xFFCC),
#else
    .rx_chf_coef_7 = XCVR_RX_DIG_RX_CHF_COEF_7_RX_CH_FILT_H7(0x0040),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_7_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_7_drs = XCVR_RX_DIG_RX_CHF_COEF_7_DRS_RX_CH_FILT_H7(0xFFA0),
#else
    .rx_chf_coef_7_drs = XCVR_RX_DIG_RX_CHF_COEF_7_DRS_RX_CH_FILT_H7(0x005C),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_8 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_8 = XCVR_RX_DIG_RX_CHF_COEF_8_RX_CH_FILT_H8(0xFFC1),
#else
    .rx_chf_coef_8 = XCVR_RX_DIG_RX_CHF_COEF_8_RX_CH_FILT_H8(0x0002),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_8_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_8_drs = XCVR_RX_DIG_RX_CHF_COEF_8_DRS_RX_CH_FILT_H8(0xFF9D),
#else
    .rx_chf_coef_8_drs = XCVR_RX_DIG_RX_CHF_COEF_8_DRS_RX_CH_FILT_H8(0xFFC2),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_9 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_9 = XCVR_RX_DIG_RX_CHF_COEF_9_RX_CH_FILT_H9(0x0003),
#else
    .rx_chf_coef_9 = XCVR_RX_DIG_RX_CHF_COEF_9_RX_CH_FILT_H9(0xFF7E),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_9_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_9_drs = XCVR_RX_DIG_RX_CHF_COEF_9_DRS_RX_CH_FILT_H9(0xFFFD),
#else
    .rx_chf_coef_9_drs = XCVR_RX_DIG_RX_CHF_COEF_9_DRS_RX_CH_FILT_H9(0xFF39),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_DIG_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .rx_dig_ctrl = XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_LEN(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(0x01),
#else
    .rx_dig_ctrl = XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_LEN(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(0x02),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_DIG_CTRL_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_dig_ctrl_drs = XCVR_RX_DIG_RX_DIG_CTRL_DRS_RX_DEC_FILT_OSR(0x00),
#else
    .rx_dig_ctrl_drs = XCVR_RX_DIG_RX_DIG_CTRL_DRS_RX_DEC_FILT_OSR(0x01),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_D_CHF_COEF_0 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_0 = XCVR_RX_DIG_RX_D_CHF_COEF_0_RX_D_CH_FILT_H0(0x0002),

    /* RX_D_CHF_COEF_0_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_0_drs = XCVR_RX_DIG_RX_D_CHF_COEF_0_DRS_RX_D_CH_FILT_H0(0x01FE),

    /* RX_D_CHF_COEF_1 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_1 = XCVR_RX_DIG_RX_D_CHF_COEF_1_RX_D_CH_FILT_H1(0x01F6),

    /* RX_D_CHF_COEF_1_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_1_drs = XCVR_RX_DIG_RX_D_CHF_COEF_1_DRS_RX_D_CH_FILT_H1(0x01F5),

    /* RX_D_CHF_COEF_2 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_2 = XCVR_RX_DIG_RX_D_CHF_COEF_2_RX_D_CH_FILT_H2(0x03F3),

    /* RX_D_CHF_COEF_2_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_2_drs = XCVR_RX_DIG_RX_D_CHF_COEF_2_DRS_RX_D_CH_FILT_H2(0x000B),

    /* RX_D_CHF_COEF_3 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_3 = XCVR_RX_DIG_RX_D_CHF_COEF_3_RX_D_CH_FILT_H3(0x008A),

    /* RX_D_CHF_COEF_3_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_3_drs = XCVR_RX_DIG_RX_D_CHF_COEF_3_DRS_RX_D_CH_FILT_H3(0x008C),

    /* RX_D_CHF_COEF_4 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_4 = XCVR_RX_DIG_RX_D_CHF_COEF_4_RX_D_CH_FILT_H4(0x0116),

    /* RX_D_CHF_COEF_4_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_4_drs = XCVR_RX_DIG_RX_D_CHF_COEF_4_DRS_RX_D_CH_FILT_H4(0x00EB),

    /* RX_RCCAL_CTRL0 configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .rx_rccal_ctrl0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0x00),

    /* RX_RCCAL_CTRL1 configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .rx_rccal_ctrl1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0x00),
    
    /* XCVR_TX_DIG configs */
    /***********************/

    /* DATARATE_CONFIG_FILTER_CTRL configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .datarate_config_filter_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_FIR_FILTER_OVRD(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FILTER_OVRD_EN(1)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_FILTER_OVRD(0)|
#if RF_OSC_26MHZ == 1
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_GFSK_FILT_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FIR_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_CLK_SEL(0)|
#else
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_GFSK_FILT_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FIR_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_CLK_SEL(0)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_FILTER_OVRD(0),

    /* DATARATE_CONFIG_FSK_CTRL configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .datarate_config_fsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV0(0x1610)|
                XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV1(0x09f0),
#else
    .datarate_config_fsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV0(0x1800)|
                XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV1(0x0800),
#endif /* RF_OSC_26MHZ == 1 */

    /* DATARATE_CONFIG_GFSK_CTRL configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .datarate_config_gfsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_GFSK_CTRL_DATARATE_CONFIG_GFSK_FDEV(0x0986),
#else
    .datarate_config_gfsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_GFSK_CTRL_DATARATE_CONFIG_GFSK_FDEV(0x07BE),
#endif /* RF_OSC_26MHZ == 1 */

    /* DATA_PADDING_CTRL_1 configuration, dependencies: ['COM', 'COM', 'MD+DR'] */
    .data_padding_ctrl_1 = XCVR_TX_DIG_DATA_PADDING_CTRL_1_TX_DATA_FLUSH_DLY(4),

    /* FSK_CTRL configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .fsk_ctrl = XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_0(0x1B0D)|
                XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_1(0x04F2),
#else
    .fsk_ctrl = XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_0(0x1BF9)|
                XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_1(0x0406),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_0_1 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_0_1 = XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_0(0x0048)|
                XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_1(0x0059),
#else
    .gfsk_coeff_0_1 = XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_0(0x0019)|
                XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_1(0x002C),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_2_3 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_2_3 = XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_2(0x006B)|
                XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_3(0x007D),
#else
    .gfsk_coeff_2_3 = XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_2(0x0046)|
                XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_3(0x0069),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_4_5 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_4_5 = XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_4(0x008D)|
                XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_5(0x009A),
#else
    .gfsk_coeff_4_5 = XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_4(0x0091)|
                XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_5(0x00B8),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_6_7 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_6_7 = XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_6(0x00A4)|
                XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_7(0x00A9),
#else
    .gfsk_coeff_6_7 = XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_6(0x00D8)|
                XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_7(0x00EA),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_CTRL configuration, dependencies: ['COM', 'COM', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_FDEV(0x04E6),
#else
    .gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_FDEV(0x03FC),
#endif /* RF_OSC_26MHZ == 1 */

    /* IMAGE_FILTER_CTRL configuration, dependencies: ['COM', 'MD+DR', 'COM', 'MD+DR', 'COM', 'COM'] */
    .image_filter_ctrl = XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_FILTER_OVRD_EN(1)|
                XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_FIR_FILTER_SEL(0),

    /* TXDIG_CTRL configuration, dependencies: ['COM', 'MD+DR', 'MD+DR'] */
    .txdig_ctrl = XCVR_TX_DIG_TXDIG_CTRL_MODULATOR_SEL(1)|
                XCVR_TX_DIG_TXDIG_CTRL_PFC_EN(1),
    /***********************************************/
    /************ END OF GENERATED CODE ************/
    /************ xcvr_msk_1mbps_config ************/
    /***********************************************/
};

const xcvr_mode_datarate_config_t xcvr_msk_500kbps_config =
{
    /***********************************************/
    /*********** START OF GENERATED CODE ***********/
    /*********** xcvr_msk_500kbps_config ***********/
    /***********************************************/
    .radio_mode = MSK,
    .data_rate = DR_1MBPS,
    .alt_data_rate = DR_500KBPS,
    
    /* RADIO_CTRL configs */
    /**********************/

    /* LL_CTRL configuration, dependencies: ['MD'] */
    .ll_ctrl = RADIO_CTRL_LL_CTRL_ACTIVE_LL(2),
    
    /* XCVR_2P4GHZ_PHY configs */
    /***************************/

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x10)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x30),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x28)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x24),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x10)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x38),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x04),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x08),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x3C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x10),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x18)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x1C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x30),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x28)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x24),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x38),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x04),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x30)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x08),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x3C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x30)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x10),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x18)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x1C),

    /* FSK_CFG0 configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'MD', 'MD'] */
    .fsk_cfg0 = XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_1M(0x17)|
                XCVR_2P4GHZ_PHY_FSK_CFG0_MSK2FSK_SEED(1)|
#if RF_OSC_26MHZ == 1
                XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_2M(0x19)|
#else
                XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_2M(0x19)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_2P4GHZ_PHY_FSK_CFG0_MSK_EN(1),

    /* FSK_CFG1 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .fsk_cfg1 = XCVR_2P4GHZ_PHY_FSK_CFG1_OVERH(0x0040)|
                XCVR_2P4GHZ_PHY_FSK_CFG1_OVERH_INV(0x0100)|
                XCVR_2P4GHZ_PHY_FSK_CFG1_SYNCTSCALE(0x00),

    /* FSK_CFG2 configuration, dependencies: ['MD+DR', 'MD+DR', 'COM'] */
    .fsk_cfg2 = XCVR_2P4GHZ_PHY_FSK_CFG2_MAG_THRESH_1M(0x00D0)|
                XCVR_2P4GHZ_PHY_FSK_CFG2_MAG_THRESH_HI_1M(0x01D0),

    /* FSK_CFG3 configuration, dependencies: ['MD+DR', 'MD+DR'] */
    .fsk_cfg3 = XCVR_2P4GHZ_PHY_FSK_CFG3_MAG_THRESH_2M(0x0098)|
                XCVR_2P4GHZ_PHY_FSK_CFG3_MAG_THRESH_HI_2M(0x0160),

    /* FSK_PD_CFG0 configuration, dependencies: ['MD', 'MD'] */
    .fsk_pd_cfg0 = XCVR_2P4GHZ_PHY_FSK_PD_CFG0_PD_IIR_ALPHA(0xFF)|
                XCVR_2P4GHZ_PHY_FSK_PD_CFG0_PREAMBLE_T_SCALE(0x0C),

    /* FSK_PD_CFG1 configuration, dependencies: ['MD'] */
    .fsk_pd_cfg1 = XCVR_2P4GHZ_PHY_FSK_PD_CFG1_PREAMBLE_PATTERN(0x55),

    /* FSK_PD_CFG2 configuration, dependencies: ['MD+DR', 'MD+DR'] */
    .fsk_pd_cfg2 = XCVR_2P4GHZ_PHY_FSK_PD_CFG2_PD_THRESH_ACQ_1_3_1M(0xEB)|
                XCVR_2P4GHZ_PHY_FSK_PD_CFG2_PD_THRESH_ACQ_1_3_2M(0xEB),

    /* FSK_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .fsk_pd_ph[0] = XCVR_2P4GHZ_PHY_FSK_PD_PH_REF0(0x03)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF1(0x05)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF2(0x03)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF3(0x00),

    /* FSK_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .fsk_pd_ph[1] = XCVR_2P4GHZ_PHY_FSK_PD_PH_REF0(0x3D)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF1(0x3B)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF2(0x3D)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF3(0x00),

    /* LR_AA_CFG configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_aa_cfg = XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_COR_THRESH(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_HAM_THRESH(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_LR_CORR_GAIN(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_ACCESS_ADDR_HAM(0x00),

    /* LR_PD_CFG configuration, dependencies: ['MD', 'MD', 'MD'] */
    .lr_pd_cfg = XCVR_2P4GHZ_PHY_LR_PD_CFG_CORR_TH(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_CFG_FREQ_TH(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_CFG_NO_PEAKS(0),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[0] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[1] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[2] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[3] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* SM_CFG configuration, dependencies: ['MD', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .sm_cfg = XCVR_2P4GHZ_PHY_SM_CFG_AA_TIMEOUT_UNCODED(0x00A0),
    
    /* XCVR_ANALOG configs */
    /***********************/

    /* RX_BBA configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .rx_bba = XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_DRS(4)|
                XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_DRS(4),

    /* RX_TZA configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM'] */
    .rx_tza = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(0),

    /* SY_CTRL_2 configuration, dependencies: ['COM', 'COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM'] */
    .sy_ctrl_2 = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0)|
                XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_DRS(0),
    
    /* XCVR_MISC configs */
    /*********************/

    /* XCVR_CTRL configuration, dependencies: [nan, nan, nan, nan, 'COM', nan, nan, 'DR', 'DR'] */
    .xcvr_ctrl = XCVR_MISC_XCVR_CTRL_TX_DATA_RATE(1)|
                XCVR_MISC_XCVR_CTRL_TX_DATA_RATE_DRS(2),
    
    /* XCVR_PLL_DIG configs */
    /************************/

    /* CHAN_MAP configuration, dependencies: ['MD', 'COM', 'COM', 'COM', 'COM'] */
    .chan_map = XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT(7),

    /* DATA_RATE_OVRD_CTRL1 configuration, dependencies: ['MD+DR', 'COM', 'COM', 'COM'] */
    .data_rate_ovrd_ctrl1 = XCVR_PLL_DIG_DATA_RATE_OVRD_CTRL1_HPM_CAL_SCALE_CFG1(0x0A),
    
    /* XCVR_PLL_DIG2 configs */
    /*************************/

    /* HPMCAL_CTRL configuration, dependencies: ['MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .hpmcal_ctrl = XCVR_PLL_DIG2_HPMCAL_CTRL_HPM_CAL_ARRAY_SIZE(1),
    
    /* XCVR_RX_DIG configs */
    /***********************/

    /* AGC_CTRL_0 configuration, dependencies: ['COM', 'COM', 'MD+DR', 'COM', 'COM', 'DR', 'MD+DR', 'MD+DR', 'COM', 'COM'] */
    .agc_ctrl_0 = XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_EN(0)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xE0)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(1),

    /* AGC_CTRL_0_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .agc_ctrl_0_drs = XCVR_RX_DIG_AGC_CTRL_0_DRS_AGC_DOWN_RSSI_THRESH(0x05)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_AGC_UP_RSSI_THRESH(0xE0)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_AGC_CTRL_0_DRS_BBA_GAIN_SETTLE_TIME(0x0F)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_LNA_GAIN_SETTLE_TIME(0x0F),
#else
                XCVR_RX_DIG_AGC_CTRL_0_DRS_BBA_GAIN_SETTLE_TIME(0x12)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_LNA_GAIN_SETTLE_TIME(0x12),
#endif /* RF_OSC_26MHZ == 1 */

    /* AGC_CTRL_1 configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .agc_ctrl_1 = XCVR_RX_DIG_AGC_CTRL_1_LNA_GAIN_SETTLE_TIME(0x0C),
#else
    .agc_ctrl_1 = XCVR_RX_DIG_AGC_CTRL_1_LNA_GAIN_SETTLE_TIME(0x0A),
#endif /* RF_OSC_26MHZ == 1 */

    /* AGC_CTRL_1_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'COM'] */
    .agc_ctrl_1_drs = XCVR_RX_DIG_AGC_CTRL_1_DRS_AGC_FAST_EXPIRE(0x18)|
                XCVR_RX_DIG_AGC_CTRL_1_DRS_AGC_UNFREEZE_TIME(0x0015),

    /* AGC_CTRL_2 configuration, dependencies: ['DR', 'MD+DR', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR'] */
    .agc_ctrl_2 = XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(0x0A)|
                XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(3)|
                XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(4)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(0x0B)|
#else
                XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(0x0E)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(2),

    /* AGC_CTRL_2_DRS configuration, dependencies: ['MD+DR', 'COM'] */
    .agc_ctrl_2_drs = XCVR_RX_DIG_AGC_CTRL_2_DRS_BBA_PDET_SEL_HI(3),

    /* AGC_CTRL_3 configuration, dependencies: ['COM', 'COM', 'COM', 'MD+DR', 'COM'] */
    .agc_ctrl_3 = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(0x82),

    /* AGC_CTRL_4 configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'MD+DR'] */
    .agc_ctrl_4 = XCVR_RX_DIG_AGC_CTRL_4_BBA_PDET_SEL_HI_HOLD(3)|
                XCVR_RX_DIG_AGC_CTRL_4_TZA_PDET_SEL_HI_HOLD(3),

    /* DCOC_CAL_IIR configuration, dependencies: ['DR', 'DR', 'DR'] */
#if RF_OSC_26MHZ == 1
    .dcoc_cal_iir = XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR1A_IDX(0)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR2A_IDX(2)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR3A_IDX(2),
#else
    .dcoc_cal_iir = XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR1A_IDX(1)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR2A_IDX(2)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR3A_IDX(1),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .dcoc_ctrl_0 = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_DLY(0x0A)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(0x07),
#else
    .dcoc_ctrl_0 = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_DLY(0x0C)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(0x08),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_1 configuration, dependencies: ['COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM'] */
    .dcoc_ctrl_1 = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_ALT_IDX(2)|
                XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),

    /* DCOC_CTRL_2 configuration, dependencies: ['DR', 'DR'] */
#if RF_OSC_26MHZ == 1
    .dcoc_ctrl_2 = XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_DLY_ALT(0x0A)|
                XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_HOLD_TIME_ALT(0x0B),
#else
    .dcoc_ctrl_2 = XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_DLY_ALT(0x0C)|
                XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_HOLD_TIME_ALT(0x0E),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR'] */
    .dcoc_ctrl_drs = XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_ALPHA_RADIUS_IDX(1)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY(0x0D)|
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY_ALT(0x0D),
#else
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY(0x10)|
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY_ALT(0x10),
#endif /* RF_OSC_26MHZ == 1 */

    /* DC_RESID_CTRL configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'MD+DR', 'DR', 'MD+DR', 'COM'] */
    .dc_resid_ctrl = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_DLY(4)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_ITER_FREEZE(0x05)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0x1A)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(0x3C),

    /* DC_RESID_CTRL_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR'] */
    .dc_resid_ctrl_drs = XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_DLY(4)|
                XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_NWIN(0x28)|
                XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_NWIN2(0x028),

    /* RX_CHF_COEF_0 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_0 = XCVR_RX_DIG_RX_CHF_COEF_0_RX_CH_FILT_H0(0x0000),
#else
    .rx_chf_coef_0 = XCVR_RX_DIG_RX_CHF_COEF_0_RX_CH_FILT_H0(0x0002),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_0_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_0_drs = XCVR_RX_DIG_RX_CHF_COEF_0_DRS_RX_CH_FILT_H0(0x0007),
#else
    .rx_chf_coef_0_drs = XCVR_RX_DIG_RX_CHF_COEF_0_DRS_RX_CH_FILT_H0(0x0000),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_1 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_1 = XCVR_RX_DIG_RX_CHF_COEF_1_RX_CH_FILT_H1(0xFFF8),
#else
    .rx_chf_coef_1 = XCVR_RX_DIG_RX_CHF_COEF_1_RX_CH_FILT_H1(0xFFFD),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_10 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_10 = XCVR_RX_DIG_RX_CHF_COEF_10_RX_CH_FILT_H10(0x007B),
#else
    .rx_chf_coef_10 = XCVR_RX_DIG_RX_CHF_COEF_10_RX_CH_FILT_H10(0x000A),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_10_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_10_drs = XCVR_RX_DIG_RX_CHF_COEF_10_DRS_RX_CH_FILT_H10(0x0073),
#else
    .rx_chf_coef_10_drs = XCVR_RX_DIG_RX_CHF_COEF_10_DRS_RX_CH_FILT_H10(0xFFCE),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_11 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_11 = XCVR_RX_DIG_RX_CHF_COEF_11_RX_CH_FILT_H11(0x00DA),
#else
    .rx_chf_coef_11 = XCVR_RX_DIG_RX_CHF_COEF_11_RX_CH_FILT_H11(0x014F),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_11_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_11_drs = XCVR_RX_DIG_RX_CHF_COEF_11_DRS_RX_CH_FILT_H11(0x00FC),
#else
    .rx_chf_coef_11_drs = XCVR_RX_DIG_RX_CHF_COEF_11_DRS_RX_CH_FILT_H11(0x0177),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_1_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_1_drs = XCVR_RX_DIG_RX_CHF_COEF_1_DRS_RX_CH_FILT_H1(0x0002),
#else
    .rx_chf_coef_1_drs = XCVR_RX_DIG_RX_CHF_COEF_1_DRS_RX_CH_FILT_H1(0x0006),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_2 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_2 = XCVR_RX_DIG_RX_CHF_COEF_2_RX_CH_FILT_H2(0xFFF5),
#else
    .rx_chf_coef_2 = XCVR_RX_DIG_RX_CHF_COEF_2_RX_CH_FILT_H2(0xFFF9),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_2_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_2_drs = XCVR_RX_DIG_RX_CHF_COEF_2_DRS_RX_CH_FILT_H2(0xFFF3),
#else
    .rx_chf_coef_2_drs = XCVR_RX_DIG_RX_CHF_COEF_2_DRS_RX_CH_FILT_H2(0xFFF8),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_3 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_3 = XCVR_RX_DIG_RX_CHF_COEF_3_RX_CH_FILT_H3(0x0001),
#else
    .rx_chf_coef_3 = XCVR_RX_DIG_RX_CHF_COEF_3_RX_CH_FILT_H3(0x000C),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_3_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_3_drs = XCVR_RX_DIG_RX_CHF_COEF_3_DRS_RX_CH_FILT_H3(0xFFEF),
#else
    .rx_chf_coef_3_drs = XCVR_RX_DIG_RX_CHF_COEF_3_DRS_RX_CH_FILT_H3(0xFFF8),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_4 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_4 = XCVR_RX_DIG_RX_CHF_COEF_4_RX_CH_FILT_H4(0x0016),
#else
    .rx_chf_coef_4 = XCVR_RX_DIG_RX_CHF_COEF_4_RX_CH_FILT_H4(0x000B),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_4_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_4_drs = XCVR_RX_DIG_RX_CHF_COEF_4_DRS_RX_CH_FILT_H4(0x0006),
#else
    .rx_chf_coef_4_drs = XCVR_RX_DIG_RX_CHF_COEF_4_DRS_RX_CH_FILT_H4(0x001A),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_5 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_5 = XCVR_RX_DIG_RX_CHF_COEF_5_RX_CH_FILT_H5(0x001A),
#else
    .rx_chf_coef_5 = XCVR_RX_DIG_RX_CHF_COEF_5_RX_CH_FILT_H5(0xFFE1),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_5_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_5_drs = XCVR_RX_DIG_RX_CHF_COEF_5_DRS_RX_CH_FILT_H5(0x0025),
#else
    .rx_chf_coef_5_drs = XCVR_RX_DIG_RX_CHF_COEF_5_DRS_RX_CH_FILT_H5(0xFFFB),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_6 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_6 = XCVR_RX_DIG_RX_CHF_COEF_6_RX_CH_FILT_H6(0xFFFB),
#else
    .rx_chf_coef_6 = XCVR_RX_DIG_RX_CHF_COEF_6_RX_CH_FILT_H6(0xFFF5),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_6_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_6_drs = XCVR_RX_DIG_RX_CHF_COEF_6_DRS_RX_CH_FILT_H6(0x001B),
#else
    .rx_chf_coef_6_drs = XCVR_RX_DIG_RX_CHF_COEF_6_DRS_RX_CH_FILT_H6(0xFFCF),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_7 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_7 = XCVR_RX_DIG_RX_CHF_COEF_7_RX_CH_FILT_H7(0xFFCC),
#else
    .rx_chf_coef_7 = XCVR_RX_DIG_RX_CHF_COEF_7_RX_CH_FILT_H7(0x0040),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_7_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_7_drs = XCVR_RX_DIG_RX_CHF_COEF_7_DRS_RX_CH_FILT_H7(0xFFDB),
#else
    .rx_chf_coef_7_drs = XCVR_RX_DIG_RX_CHF_COEF_7_DRS_RX_CH_FILT_H7(0x0031),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_8 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_8 = XCVR_RX_DIG_RX_CHF_COEF_8_RX_CH_FILT_H8(0xFFC1),
#else
    .rx_chf_coef_8 = XCVR_RX_DIG_RX_CHF_COEF_8_RX_CH_FILT_H8(0x0002),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_8_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_8_drs = XCVR_RX_DIG_RX_CHF_COEF_8_DRS_RX_CH_FILT_H8(0xFFA9),
#else
    .rx_chf_coef_8_drs = XCVR_RX_DIG_RX_CHF_COEF_8_DRS_RX_CH_FILT_H8(0x003C),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_9 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_9 = XCVR_RX_DIG_RX_CHF_COEF_9_RX_CH_FILT_H9(0x0003),
#else
    .rx_chf_coef_9 = XCVR_RX_DIG_RX_CHF_COEF_9_RX_CH_FILT_H9(0xFF7E),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_9_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_9_drs = XCVR_RX_DIG_RX_CHF_COEF_9_DRS_RX_CH_FILT_H9(0xFFDC),
#else
    .rx_chf_coef_9_drs = XCVR_RX_DIG_RX_CHF_COEF_9_DRS_RX_CH_FILT_H9(0xFF73),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_DIG_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .rx_dig_ctrl = XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_LEN(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(0x01),
#else
    .rx_dig_ctrl = XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_LEN(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(0x02),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_DIG_CTRL_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_dig_ctrl_drs = XCVR_RX_DIG_RX_DIG_CTRL_DRS_RX_DEC_FILT_OSR(0x02),
#else
    .rx_dig_ctrl_drs = XCVR_RX_DIG_RX_DIG_CTRL_DRS_RX_DEC_FILT_OSR(0x04),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_D_CHF_COEF_0 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_0 = XCVR_RX_DIG_RX_D_CHF_COEF_0_RX_D_CH_FILT_H0(0x0002),

    /* RX_D_CHF_COEF_0_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_0_drs = XCVR_RX_DIG_RX_D_CHF_COEF_0_DRS_RX_D_CH_FILT_H0(0x0001),

    /* RX_D_CHF_COEF_1 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_1 = XCVR_RX_DIG_RX_D_CHF_COEF_1_RX_D_CH_FILT_H1(0x01F6),

    /* RX_D_CHF_COEF_1_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_1_drs = XCVR_RX_DIG_RX_D_CHF_COEF_1_DRS_RX_D_CH_FILT_H1(0x01F5),

    /* RX_D_CHF_COEF_2 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_2 = XCVR_RX_DIG_RX_D_CHF_COEF_2_RX_D_CH_FILT_H2(0x03F3),

    /* RX_D_CHF_COEF_2_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_2_drs = XCVR_RX_DIG_RX_D_CHF_COEF_2_DRS_RX_D_CH_FILT_H2(0x03F6),

    /* RX_D_CHF_COEF_3 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_3 = XCVR_RX_DIG_RX_D_CHF_COEF_3_RX_D_CH_FILT_H3(0x008A),

    /* RX_D_CHF_COEF_3_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_3_drs = XCVR_RX_DIG_RX_D_CHF_COEF_3_DRS_RX_D_CH_FILT_H3(0x008B),

    /* RX_D_CHF_COEF_4 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_4 = XCVR_RX_DIG_RX_D_CHF_COEF_4_RX_D_CH_FILT_H4(0x0116),

    /* RX_D_CHF_COEF_4_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_4_drs = XCVR_RX_DIG_RX_D_CHF_COEF_4_DRS_RX_D_CH_FILT_H4(0x0110),

    /* RX_RCCAL_CTRL0 configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .rx_rccal_ctrl0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(0x1F)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(0x1F)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0x00),

    /* RX_RCCAL_CTRL1 configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .rx_rccal_ctrl1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0x00),
    
    /* XCVR_TX_DIG configs */
    /***********************/

    /* DATARATE_CONFIG_FILTER_CTRL configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .datarate_config_filter_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_FIR_FILTER_OVRD(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FILTER_OVRD_EN(1)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_FILTER_OVRD(0)|
#if RF_OSC_26MHZ == 1
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_GFSK_FILT_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FIR_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_CLK_SEL(0)|
#else
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_GFSK_FILT_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FIR_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_CLK_SEL(0)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_FILTER_OVRD(0),

    /* DATARATE_CONFIG_FSK_CTRL configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .datarate_config_fsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV0(0x1D80)|
                XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV1(0x0240),
#else
    .datarate_config_fsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV0(0x1DE0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV1(0x0200),
#endif /* RF_OSC_26MHZ == 1 */

    /* DATARATE_CONFIG_GFSK_CTRL configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .datarate_config_gfsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_GFSK_CTRL_DATARATE_CONFIG_GFSK_FDEV(0x0279),
#else
    .datarate_config_gfsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_GFSK_CTRL_DATARATE_CONFIG_GFSK_FDEV(0x0203),
#endif /* RF_OSC_26MHZ == 1 */

    /* DATA_PADDING_CTRL_1 configuration, dependencies: ['COM', 'COM', 'MD+DR'] */
    .data_padding_ctrl_1 = XCVR_TX_DIG_DATA_PADDING_CTRL_1_TX_DATA_FLUSH_DLY(4),

    /* FSK_CTRL configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .fsk_ctrl = XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_0(0x1B0D)|
                XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_1(0x04F2),
#else
    .fsk_ctrl = XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_0(0x1BF9)|
                XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_1(0x0406),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_0_1 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_0_1 = XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_0(0x0048)|
                XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_1(0x0059),
#else
    .gfsk_coeff_0_1 = XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_0(0x0019)|
                XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_1(0x002C),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_2_3 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_2_3 = XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_2(0x006B)|
                XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_3(0x007D),
#else
    .gfsk_coeff_2_3 = XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_2(0x0046)|
                XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_3(0x0069),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_4_5 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_4_5 = XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_4(0x008D)|
                XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_5(0x009A),
#else
    .gfsk_coeff_4_5 = XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_4(0x0091)|
                XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_5(0x00B8),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_6_7 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_6_7 = XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_6(0x00A4)|
                XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_7(0x00A9),
#else
    .gfsk_coeff_6_7 = XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_6(0x00D8)|
                XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_7(0x00EA),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_CTRL configuration, dependencies: ['COM', 'COM', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_FDEV(0x04E6),
#else
    .gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_FDEV(0x03FC),
#endif /* RF_OSC_26MHZ == 1 */

    /* IMAGE_FILTER_CTRL configuration, dependencies: ['COM', 'MD+DR', 'COM', 'MD+DR', 'COM', 'COM'] */
    .image_filter_ctrl = XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_FILTER_OVRD_EN(1)|
                XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_FIR_FILTER_SEL(0),

    /* TXDIG_CTRL configuration, dependencies: ['COM', 'MD+DR', 'MD+DR'] */
    .txdig_ctrl = XCVR_TX_DIG_TXDIG_CTRL_MODULATOR_SEL(1)|
                XCVR_TX_DIG_TXDIG_CTRL_PFC_EN(1),
    /***********************************************/
    /************ END OF GENERATED CODE ************/
    /*********** xcvr_msk_500kbps_config ***********/
    /***********************************************/
};

const xcvr_mode_datarate_config_t xcvr_msk_250kbps_config =
{
    /***********************************************/
    /*********** START OF GENERATED CODE ***********/
    /*********** xcvr_msk_250kbps_config ***********/
    /***********************************************/
    .radio_mode = MSK,
    .data_rate = DR_1MBPS,
    .alt_data_rate = DR_250KBPS,
    
    /* RADIO_CTRL configs */
    /**********************/

    /* LL_CTRL configuration, dependencies: ['MD'] */
    .ll_ctrl = RADIO_CTRL_LL_CTRL_ACTIVE_LL(2),
    
    /* XCVR_2P4GHZ_PHY configs */
    /***************************/

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x10)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x30),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[0].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x28)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x24),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x10)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x38),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[1].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x04),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x08),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[2].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x3C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x10),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[3].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x18)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x1C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x30),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[4].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x28)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x24),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x04),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x38),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[5].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x3C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x04),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x30)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x08),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[6].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x3C),

    /* DMD_WAVE_REG0 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg0 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL0(0x2C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL1(0x30)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL2(0x34)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL3(0x38)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG0_SMPL4(0x3C),

    /* DMD_WAVE_REG1 configuration, dependencies: ['MD', 'MD', 'MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg1 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL5(0x00)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL6(0x04)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL7(0x08)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL8(0x0C)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG1_SMPL9(0x10),

    /* DMD_WAVE_REG2 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .demod_wave[7].dmd_wave_reg2 = XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL10(0x14)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL11(0x18)|
                XCVR_2P4GHZ_PHY_DMD_WAVE_REG2_SMPL12(0x1C),

    /* FSK_CFG0 configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'MD', 'MD'] */
    .fsk_cfg0 = XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_1M(0x17)|
                XCVR_2P4GHZ_PHY_FSK_CFG0_MSK2FSK_SEED(1)|
#if RF_OSC_26MHZ == 1
                XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_2M(0x19)|
#else
                XCVR_2P4GHZ_PHY_FSK_CFG0_AA_ACQ_1_2_3_THRESH_2M(0x19)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_2P4GHZ_PHY_FSK_CFG0_MSK_EN(1),

    /* FSK_CFG1 configuration, dependencies: ['MD', 'MD', 'MD'] */
    .fsk_cfg1 = XCVR_2P4GHZ_PHY_FSK_CFG1_OVERH(0x0040)|
                XCVR_2P4GHZ_PHY_FSK_CFG1_OVERH_INV(0x0100)|
                XCVR_2P4GHZ_PHY_FSK_CFG1_SYNCTSCALE(0x00),

    /* FSK_CFG2 configuration, dependencies: ['MD+DR', 'MD+DR', 'COM'] */
    .fsk_cfg2 = XCVR_2P4GHZ_PHY_FSK_CFG2_MAG_THRESH_1M(0x00D0)|
                XCVR_2P4GHZ_PHY_FSK_CFG2_MAG_THRESH_HI_1M(0x01D0),

    /* FSK_CFG3 configuration, dependencies: ['MD+DR', 'MD+DR'] */
    .fsk_cfg3 = XCVR_2P4GHZ_PHY_FSK_CFG3_MAG_THRESH_2M(0x0070)|
                XCVR_2P4GHZ_PHY_FSK_CFG3_MAG_THRESH_HI_2M(0x0100),

    /* FSK_PD_CFG0 configuration, dependencies: ['MD', 'MD'] */
    .fsk_pd_cfg0 = XCVR_2P4GHZ_PHY_FSK_PD_CFG0_PD_IIR_ALPHA(0xFF)|
                XCVR_2P4GHZ_PHY_FSK_PD_CFG0_PREAMBLE_T_SCALE(0x0C),

    /* FSK_PD_CFG1 configuration, dependencies: ['MD'] */
    .fsk_pd_cfg1 = XCVR_2P4GHZ_PHY_FSK_PD_CFG1_PREAMBLE_PATTERN(0x55),

    /* FSK_PD_CFG2 configuration, dependencies: ['MD+DR', 'MD+DR'] */
    .fsk_pd_cfg2 = XCVR_2P4GHZ_PHY_FSK_PD_CFG2_PD_THRESH_ACQ_1_3_1M(0xB8)|
                XCVR_2P4GHZ_PHY_FSK_PD_CFG2_PD_THRESH_ACQ_1_3_2M(0xB8),

    /* FSK_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .fsk_pd_ph[0] = XCVR_2P4GHZ_PHY_FSK_PD_PH_REF0(0x03)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF1(0x05)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF2(0x03)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF3(0x00),

    /* FSK_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .fsk_pd_ph[1] = XCVR_2P4GHZ_PHY_FSK_PD_PH_REF0(0x3D)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF1(0x3B)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF2(0x3D)|
                XCVR_2P4GHZ_PHY_FSK_PD_PH_REF3(0x00),

    /* LR_AA_CFG configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_aa_cfg = XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_COR_THRESH(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_HAM_THRESH(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_AA_LR_CORR_GAIN(0x00)|
                XCVR_2P4GHZ_PHY_LR_AA_CFG_ACCESS_ADDR_HAM(0x00),

    /* LR_PD_CFG configuration, dependencies: ['MD', 'MD', 'MD'] */
    .lr_pd_cfg = XCVR_2P4GHZ_PHY_LR_PD_CFG_CORR_TH(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_CFG_FREQ_TH(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_CFG_NO_PEAKS(0),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[0] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[1] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[2] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* LR_PD_PH configuration, dependencies: ['MD', 'MD', 'MD', 'MD'] */
    .lr_pd_ph[3] = XCVR_2P4GHZ_PHY_LR_PD_PH_REF0(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF1(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF2(0x00)|
                XCVR_2P4GHZ_PHY_LR_PD_PH_REF3(0x00),

    /* SM_CFG configuration, dependencies: ['MD', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .sm_cfg = XCVR_2P4GHZ_PHY_SM_CFG_AA_TIMEOUT_UNCODED(0x00A0),
    
    /* XCVR_ANALOG configs */
    /***********************/

    /* RX_BBA configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .rx_bba = XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA2_BW_SEL_DRS(5)|
                XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL(0)|
                XCVR_ANALOG_RX_BBA_RX_BBA_BW_SEL_DRS(5),

    /* RX_TZA configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM'] */
    .rx_tza = XCVR_ANALOG_RX_TZA_RX_TZA_BW_SEL(0),

    /* SY_CTRL_2 configuration, dependencies: ['COM', 'COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM'] */
    .sy_ctrl_2 = XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM(0)|
                XCVR_ANALOG_SY_CTRL_2_SY_VCO_KVM_DRS(0),
    
    /* XCVR_MISC configs */
    /*********************/

    /* XCVR_CTRL configuration, dependencies: [nan, nan, nan, nan, 'COM', nan, nan, 'DR', 'DR'] */
    .xcvr_ctrl = XCVR_MISC_XCVR_CTRL_TX_DATA_RATE(1)|
                XCVR_MISC_XCVR_CTRL_TX_DATA_RATE_DRS(3),
    
    /* XCVR_PLL_DIG configs */
    /************************/

    /* CHAN_MAP configuration, dependencies: ['MD', 'COM', 'COM', 'COM', 'COM'] */
    .chan_map = XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT(7),

    /* DATA_RATE_OVRD_CTRL1 configuration, dependencies: ['MD+DR', 'COM', 'COM', 'COM'] */
    .data_rate_ovrd_ctrl1 = XCVR_PLL_DIG_DATA_RATE_OVRD_CTRL1_HPM_CAL_SCALE_CFG1(0x0A),
    
    /* XCVR_PLL_DIG2 configs */
    /*************************/

    /* HPMCAL_CTRL configuration, dependencies: ['MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM'] */
    .hpmcal_ctrl = XCVR_PLL_DIG2_HPMCAL_CTRL_HPM_CAL_ARRAY_SIZE(1),
    
    /* XCVR_RX_DIG configs */
    /***********************/

    /* AGC_CTRL_0 configuration, dependencies: ['COM', 'COM', 'MD+DR', 'COM', 'COM', 'DR', 'MD+DR', 'MD+DR', 'COM', 'COM'] */
    .agc_ctrl_0 = XCVR_RX_DIG_AGC_CTRL_0_AGC_DOWN_RSSI_THRESH(0x05)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_EN(0)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_RSSI_THRESH(0xE0)|
                XCVR_RX_DIG_AGC_CTRL_0_AGC_UP_SRC(1),

    /* AGC_CTRL_0_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .agc_ctrl_0_drs = XCVR_RX_DIG_AGC_CTRL_0_DRS_AGC_DOWN_RSSI_THRESH(0x05)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_AGC_UP_RSSI_THRESH(0xE0)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_AGC_CTRL_0_DRS_BBA_GAIN_SETTLE_TIME(0x12)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_LNA_GAIN_SETTLE_TIME(0x16),
#else
                XCVR_RX_DIG_AGC_CTRL_0_DRS_BBA_GAIN_SETTLE_TIME(0x16)|
                XCVR_RX_DIG_AGC_CTRL_0_DRS_LNA_GAIN_SETTLE_TIME(0x16),
#endif /* RF_OSC_26MHZ == 1 */

    /* AGC_CTRL_1 configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .agc_ctrl_1 = XCVR_RX_DIG_AGC_CTRL_1_LNA_GAIN_SETTLE_TIME(0x0C),
#else
    .agc_ctrl_1 = XCVR_RX_DIG_AGC_CTRL_1_LNA_GAIN_SETTLE_TIME(0x0A),
#endif /* RF_OSC_26MHZ == 1 */

    /* AGC_CTRL_1_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'COM', 'COM'] */
    .agc_ctrl_1_drs = XCVR_RX_DIG_AGC_CTRL_1_DRS_AGC_FAST_EXPIRE(0x30)|
                XCVR_RX_DIG_AGC_CTRL_1_DRS_AGC_UNFREEZE_TIME(0x0015),

    /* AGC_CTRL_2 configuration, dependencies: ['DR', 'MD+DR', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR'] */
    .agc_ctrl_2 = XCVR_RX_DIG_AGC_CTRL_2_AGC_FAST_EXPIRE(0x0A)|
                XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_HI(3)|
                XCVR_RX_DIG_AGC_CTRL_2_BBA_PDET_SEL_LO(4)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(0x0B)|
#else
                XCVR_RX_DIG_AGC_CTRL_2_BBA_GAIN_SETTLE_TIME(0x0E)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_RX_DIG_AGC_CTRL_2_TZA_PDET_SEL_LO(2),

    /* AGC_CTRL_2_DRS configuration, dependencies: ['MD+DR', 'COM'] */
    .agc_ctrl_2_drs = XCVR_RX_DIG_AGC_CTRL_2_DRS_BBA_PDET_SEL_HI(3),

    /* AGC_CTRL_3 configuration, dependencies: ['COM', 'COM', 'COM', 'MD+DR', 'COM'] */
    .agc_ctrl_3 = XCVR_RX_DIG_AGC_CTRL_3_AGC_UNFREEZE_TIME(0x82),

    /* AGC_CTRL_4 configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'MD+DR'] */
    .agc_ctrl_4 = XCVR_RX_DIG_AGC_CTRL_4_BBA_PDET_SEL_HI_HOLD(3)|
                XCVR_RX_DIG_AGC_CTRL_4_TZA_PDET_SEL_HI_HOLD(3),

    /* DCOC_CAL_IIR configuration, dependencies: ['DR', 'DR', 'DR'] */
#if RF_OSC_26MHZ == 1
    .dcoc_cal_iir = XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR1A_IDX(0)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR2A_IDX(2)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR3A_IDX(2),
#else
    .dcoc_cal_iir = XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR1A_IDX(1)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR2A_IDX(2)|
                XCVR_RX_DIG_DCOC_CAL_IIR_DCOC_CAL_IIR3A_IDX(1),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_0 configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .dcoc_ctrl_0 = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_DLY(0x0A)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(0x07),
#else
    .dcoc_ctrl_0 = XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_DLY(0x0C)|
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORR_HOLD_TIME(0x08),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_1 configuration, dependencies: ['COM', 'COM', 'MD+DR', 'MD+DR', 'COM', 'COM', 'COM', 'COM'] */
    .dcoc_ctrl_1 = XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_ALT_IDX(1)|
                XCVR_RX_DIG_DCOC_CTRL_1_DCOC_ALPHA_RADIUS_IDX(5),

    /* DCOC_CTRL_2 configuration, dependencies: ['DR', 'DR'] */
#if RF_OSC_26MHZ == 1
    .dcoc_ctrl_2 = XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_DLY_ALT(0x0A)|
                XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_HOLD_TIME_ALT(0x0F),
#else
    .dcoc_ctrl_2 = XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_DLY_ALT(0x0C)|
                XCVR_RX_DIG_DCOC_CTRL_2_DCOC_CORR_HOLD_TIME_ALT(0x12),
#endif /* RF_OSC_26MHZ == 1 */

    /* DCOC_CTRL_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR'] */
    .dcoc_ctrl_drs = XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_ALPHA_RADIUS_IDX(0)|
#if RF_OSC_26MHZ == 1
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY(0x0D)|
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY_ALT(0x10),
#else
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY(0x14)|
                XCVR_RX_DIG_DCOC_CTRL_DRS_DCOC_CORR_DLY_ALT(0x14),
#endif /* RF_OSC_26MHZ == 1 */

    /* DC_RESID_CTRL configuration, dependencies: ['COM', 'MD+DR', 'COM', 'COM', 'COM', 'MD+DR', 'DR', 'MD+DR', 'COM'] */
    .dc_resid_ctrl = XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_DLY(4)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_ITER_FREEZE(0x07)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_MIN_AGC_IDX(0x1A)|
                XCVR_RX_DIG_DC_RESID_CTRL_DC_RESID_NWIN(0x3C),

    /* DC_RESID_CTRL_DRS configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR'] */
    .dc_resid_ctrl_drs = XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_DLY(4)|
                XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_NWIN(0x20)|
                XCVR_RX_DIG_DC_RESID_CTRL_DRS_DC_RESID_NWIN2(0x020),

    /* RX_CHF_COEF_0 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_0 = XCVR_RX_DIG_RX_CHF_COEF_0_RX_CH_FILT_H0(0x0000),
#else
    .rx_chf_coef_0 = XCVR_RX_DIG_RX_CHF_COEF_0_RX_CH_FILT_H0(0x0002),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_0_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_0_drs = XCVR_RX_DIG_RX_CHF_COEF_0_DRS_RX_CH_FILT_H0(0x0002),
#else
    .rx_chf_coef_0_drs = XCVR_RX_DIG_RX_CHF_COEF_0_DRS_RX_CH_FILT_H0(0xFFFD),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_1 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_1 = XCVR_RX_DIG_RX_CHF_COEF_1_RX_CH_FILT_H1(0xFFF8),
#else
    .rx_chf_coef_1 = XCVR_RX_DIG_RX_CHF_COEF_1_RX_CH_FILT_H1(0xFFFD),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_10 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_10 = XCVR_RX_DIG_RX_CHF_COEF_10_RX_CH_FILT_H10(0x007B),
#else
    .rx_chf_coef_10 = XCVR_RX_DIG_RX_CHF_COEF_10_RX_CH_FILT_H10(0x000A),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_10_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_10_drs = XCVR_RX_DIG_RX_CHF_COEF_10_DRS_RX_CH_FILT_H10(0x0060),
#else
    .rx_chf_coef_10_drs = XCVR_RX_DIG_RX_CHF_COEF_10_DRS_RX_CH_FILT_H10(0xFF9D),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_11 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_11 = XCVR_RX_DIG_RX_CHF_COEF_11_RX_CH_FILT_H11(0x00DA),
#else
    .rx_chf_coef_11 = XCVR_RX_DIG_RX_CHF_COEF_11_RX_CH_FILT_H11(0x014F),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_11_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_11_drs = XCVR_RX_DIG_RX_CHF_COEF_11_DRS_RX_CH_FILT_H11(0x0101),
#else
    .rx_chf_coef_11_drs = XCVR_RX_DIG_RX_CHF_COEF_11_DRS_RX_CH_FILT_H11(0x0185),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_1_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_1_drs = XCVR_RX_DIG_RX_CHF_COEF_1_DRS_RX_CH_FILT_H1(0x0009),
#else
    .rx_chf_coef_1_drs = XCVR_RX_DIG_RX_CHF_COEF_1_DRS_RX_CH_FILT_H1(0x0001),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_2 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_2 = XCVR_RX_DIG_RX_CHF_COEF_2_RX_CH_FILT_H2(0xFFF5),
#else
    .rx_chf_coef_2 = XCVR_RX_DIG_RX_CHF_COEF_2_RX_CH_FILT_H2(0xFFF9),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_2_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_2_drs = XCVR_RX_DIG_RX_CHF_COEF_2_DRS_RX_CH_FILT_H2(0x0003),
#else
    .rx_chf_coef_2_drs = XCVR_RX_DIG_RX_CHF_COEF_2_DRS_RX_CH_FILT_H2(0x0009),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_3 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_3 = XCVR_RX_DIG_RX_CHF_COEF_3_RX_CH_FILT_H3(0x0001),
#else
    .rx_chf_coef_3 = XCVR_RX_DIG_RX_CHF_COEF_3_RX_CH_FILT_H3(0x000C),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_3_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_3_drs = XCVR_RX_DIG_RX_CHF_COEF_3_DRS_RX_CH_FILT_H3(0xFFEF),
#else
    .rx_chf_coef_3_drs = XCVR_RX_DIG_RX_CHF_COEF_3_DRS_RX_CH_FILT_H3(0xFFF0),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_4 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_4 = XCVR_RX_DIG_RX_CHF_COEF_4_RX_CH_FILT_H4(0x0016),
#else
    .rx_chf_coef_4 = XCVR_RX_DIG_RX_CHF_COEF_4_RX_CH_FILT_H4(0x000B),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_4_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_4_drs = XCVR_RX_DIG_RX_CHF_COEF_4_DRS_RX_CH_FILT_H4(0xFFEF),
#else
    .rx_chf_coef_4_drs = XCVR_RX_DIG_RX_CHF_COEF_4_DRS_RX_CH_FILT_H4(0x0001),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_5 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_5 = XCVR_RX_DIG_RX_CHF_COEF_5_RX_CH_FILT_H5(0x001A),
#else
    .rx_chf_coef_5 = XCVR_RX_DIG_RX_CHF_COEF_5_RX_CH_FILT_H5(0xFFE1),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_5_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_5_drs = XCVR_RX_DIG_RX_CHF_COEF_5_DRS_RX_CH_FILT_H5(0x0013),
#else
    .rx_chf_coef_5_drs = XCVR_RX_DIG_RX_CHF_COEF_5_DRS_RX_CH_FILT_H5(0x0021),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_6 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_6 = XCVR_RX_DIG_RX_CHF_COEF_6_RX_CH_FILT_H6(0xFFFB),
#else
    .rx_chf_coef_6 = XCVR_RX_DIG_RX_CHF_COEF_6_RX_CH_FILT_H6(0xFFF5),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_6_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_6_drs = XCVR_RX_DIG_RX_CHF_COEF_6_DRS_RX_CH_FILT_H6(0x002A),
#else
    .rx_chf_coef_6_drs = XCVR_RX_DIG_RX_CHF_COEF_6_DRS_RX_CH_FILT_H6(0xFFD3),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_7 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_7 = XCVR_RX_DIG_RX_CHF_COEF_7_RX_CH_FILT_H7(0xFFCC),
#else
    .rx_chf_coef_7 = XCVR_RX_DIG_RX_CHF_COEF_7_RX_CH_FILT_H7(0x0040),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_7_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_7_drs = XCVR_RX_DIG_RX_CHF_COEF_7_DRS_RX_CH_FILT_H7(0xFFFD),
#else
    .rx_chf_coef_7_drs = XCVR_RX_DIG_RX_CHF_COEF_7_DRS_RX_CH_FILT_H7(0xFFF8),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_8 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_8 = XCVR_RX_DIG_RX_CHF_COEF_8_RX_CH_FILT_H8(0xFFC1),
#else
    .rx_chf_coef_8 = XCVR_RX_DIG_RX_CHF_COEF_8_RX_CH_FILT_H8(0x0002),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_8_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_8_drs = XCVR_RX_DIG_RX_CHF_COEF_8_DRS_RX_CH_FILT_H8(0xFFB3),
#else
    .rx_chf_coef_8_drs = XCVR_RX_DIG_RX_CHF_COEF_8_DRS_RX_CH_FILT_H8(0x0061),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_9 configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_9 = XCVR_RX_DIG_RX_CHF_COEF_9_RX_CH_FILT_H9(0x0003),
#else
    .rx_chf_coef_9 = XCVR_RX_DIG_RX_CHF_COEF_9_RX_CH_FILT_H9(0xFF7E),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_CHF_COEF_9_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_chf_coef_9_drs = XCVR_RX_DIG_RX_CHF_COEF_9_DRS_RX_CH_FILT_H9(0xFFC6),
#else
    .rx_chf_coef_9_drs = XCVR_RX_DIG_RX_CHF_COEF_9_DRS_RX_CH_FILT_H9(0xFF99),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_DIG_CTRL configuration, dependencies: ['COM', 'COM', 'COM', 'COM', 'COM', 'MD+DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'DR', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM', 'COM'] */
#if RF_OSC_26MHZ == 1
    .rx_dig_ctrl = XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_LEN(1)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(0x01),
#else
    .rx_dig_ctrl = XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_LEN(0)|
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(0x02),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_DIG_CTRL_DRS configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .rx_dig_ctrl_drs = XCVR_RX_DIG_RX_DIG_CTRL_DRS_RX_DEC_FILT_OSR(0x04),
#else
    .rx_dig_ctrl_drs = XCVR_RX_DIG_RX_DIG_CTRL_DRS_RX_DEC_FILT_OSR(0x08),
#endif /* RF_OSC_26MHZ == 1 */

    /* RX_D_CHF_COEF_0 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_0 = XCVR_RX_DIG_RX_D_CHF_COEF_0_RX_D_CH_FILT_H0(0x0002),

    /* RX_D_CHF_COEF_0_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_0_drs = XCVR_RX_DIG_RX_D_CHF_COEF_0_DRS_RX_D_CH_FILT_H0(0x0002),

    /* RX_D_CHF_COEF_1 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_1 = XCVR_RX_DIG_RX_D_CHF_COEF_1_RX_D_CH_FILT_H1(0x01F6),

    /* RX_D_CHF_COEF_1_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_1_drs = XCVR_RX_DIG_RX_D_CHF_COEF_1_DRS_RX_D_CH_FILT_H1(0x01F5),

    /* RX_D_CHF_COEF_2 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_2 = XCVR_RX_DIG_RX_D_CHF_COEF_2_RX_D_CH_FILT_H2(0x03F3),

    /* RX_D_CHF_COEF_2_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_2_drs = XCVR_RX_DIG_RX_D_CHF_COEF_2_DRS_RX_D_CH_FILT_H2(0x03F5),

    /* RX_D_CHF_COEF_3 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_3 = XCVR_RX_DIG_RX_D_CHF_COEF_3_RX_D_CH_FILT_H3(0x008A),

    /* RX_D_CHF_COEF_3_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_3_drs = XCVR_RX_DIG_RX_D_CHF_COEF_3_DRS_RX_D_CH_FILT_H3(0x008B),

    /* RX_D_CHF_COEF_4 configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_4 = XCVR_RX_DIG_RX_D_CHF_COEF_4_RX_D_CH_FILT_H4(0x0116),

    /* RX_D_CHF_COEF_4_DRS configuration, dependencies: ['MD+DR'] */
    .rx_d_chf_coef_4_drs = XCVR_RX_DIG_RX_D_CHF_COEF_4_DRS_RX_D_CH_FILT_H4(0x0112),

    /* RX_RCCAL_CTRL0 configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .rx_rccal_ctrl0 = XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_DIS(1)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_MANUAL(0x1F)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_BBA_RCCAL_OFFSET(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_COMP_INV(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_RCCAL_SMP_DLY(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_DIS(1)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_MANUAL(0x1F)|
                XCVR_RX_DIG_RX_RCCAL_CTRL0_TZA_RCCAL_OFFSET(0x00),

    /* RX_RCCAL_CTRL1 configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .rx_rccal_ctrl1 = XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_DIS(0)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_MANUAL(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_ADC_RCCAL_OFFSET(0x00)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_DIS(1)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_MANUAL(0x1F)|
                XCVR_RX_DIG_RX_RCCAL_CTRL1_BBA2_RCCAL_OFFSET(0x00),
    
    /* XCVR_TX_DIG configs */
    /***********************/

    /* DATARATE_CONFIG_FILTER_CTRL configuration, dependencies: ['MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR', 'MD+DR'] */
    .datarate_config_filter_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_FIR_FILTER_OVRD(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FILTER_OVRD_EN(1)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_FILTER_OVRD(0)|
#if RF_OSC_26MHZ == 1
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_GFSK_FILT_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FIR_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_CLK_SEL(0)|
#else
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_GFSK_FILT_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_IMAGE_FIR_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC0_CLK_SEL(0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_CLK_SEL(0)|
#endif /* RF_OSC_26MHZ == 1 */
                XCVR_TX_DIG_DATARATE_CONFIG_FILTER_CTRL_DATARATE_CONFIG_SYNC1_FILTER_OVRD(0),

    /* DATARATE_CONFIG_FSK_CTRL configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .datarate_config_fsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV0(0x1EA0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV1(0x0120),
#else
    .datarate_config_fsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV0(0x1EE0)|
                XCVR_TX_DIG_DATARATE_CONFIG_FSK_CTRL_DATARATE_CONFIG_FSK_FDEV1(0x0100),
#endif /* RF_OSC_26MHZ == 1 */

    /* DATARATE_CONFIG_GFSK_CTRL configuration, dependencies: ['MD+DR'] */
#if RF_OSC_26MHZ == 1
    .datarate_config_gfsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_GFSK_CTRL_DATARATE_CONFIG_GFSK_FDEV(0x013D),
#else
    .datarate_config_gfsk_ctrl = XCVR_TX_DIG_DATARATE_CONFIG_GFSK_CTRL_DATARATE_CONFIG_GFSK_FDEV(0x0101),
#endif /* RF_OSC_26MHZ == 1 */

    /* DATA_PADDING_CTRL_1 configuration, dependencies: ['COM', 'COM', 'MD+DR'] */
    .data_padding_ctrl_1 = XCVR_TX_DIG_DATA_PADDING_CTRL_1_TX_DATA_FLUSH_DLY(4),

    /* FSK_CTRL configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .fsk_ctrl = XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_0(0x1B0D)|
                XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_1(0x04F2),
#else
    .fsk_ctrl = XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_0(0x1BF9)|
                XCVR_TX_DIG_FSK_CTRL_FSK_FDEV_1(0x0406),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_0_1 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_0_1 = XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_0(0x0048)|
                XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_1(0x0059),
#else
    .gfsk_coeff_0_1 = XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_0(0x0019)|
                XCVR_TX_DIG_GFSK_COEFF_0_1_GFSK_COEFF_1(0x002C),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_2_3 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_2_3 = XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_2(0x006B)|
                XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_3(0x007D),
#else
    .gfsk_coeff_2_3 = XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_2(0x0046)|
                XCVR_TX_DIG_GFSK_COEFF_2_3_GFSK_COEFF_3(0x0069),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_4_5 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_4_5 = XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_4(0x008D)|
                XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_5(0x009A),
#else
    .gfsk_coeff_4_5 = XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_4(0x0091)|
                XCVR_TX_DIG_GFSK_COEFF_4_5_GFSK_COEFF_5(0x00B8),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_COEFF_6_7 configuration, dependencies: ['MD+DR', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_coeff_6_7 = XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_6(0x00A4)|
                XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_7(0x00A9),
#else
    .gfsk_coeff_6_7 = XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_6(0x00D8)|
                XCVR_TX_DIG_GFSK_COEFF_6_7_GFSK_COEFF_7(0x00EA),
#endif /* RF_OSC_26MHZ == 1 */

    /* GFSK_CTRL configuration, dependencies: ['COM', 'COM', 'MD+DR'] */
#if RF_OSC_26MHZ == 1
    .gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_FDEV(0x04E6),
#else
    .gfsk_ctrl = XCVR_TX_DIG_GFSK_CTRL_GFSK_FDEV(0x03FC),
#endif /* RF_OSC_26MHZ == 1 */

    /* IMAGE_FILTER_CTRL configuration, dependencies: ['COM', 'MD+DR', 'COM', 'MD+DR', 'COM', 'COM'] */
    .image_filter_ctrl = XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_FILTER_OVRD_EN(1)|
                XCVR_TX_DIG_IMAGE_FILTER_CTRL_IMAGE_FIR_FILTER_SEL(0),

    /* TXDIG_CTRL configuration, dependencies: ['COM', 'MD+DR', 'MD+DR'] */
    .txdig_ctrl = XCVR_TX_DIG_TXDIG_CTRL_MODULATOR_SEL(1)|
                XCVR_TX_DIG_TXDIG_CTRL_PFC_EN(1),
    /***********************************************/
    /************ END OF GENERATED CODE ************/
    /*********** xcvr_msk_250kbps_config ***********/
    /***********************************************/
};

/* COMPLETE CONFIG STRUCTURES */
/*! @brief  MSK mode & datarate 1Mbps with 2Mbps alternate rate configuration. */
const xcvr_config_t xcvr_msk_1mbps_full_config = 
{
    .common_cfg = &xcvr_common_config,
    .mode_data_rate_cfg = &xcvr_msk_1mbps_config,
};

/*! @brief  MSK mode & datarate 1Mbps with 500Kbps alternate rate configuration. */
const xcvr_config_t xcvr_msk_500kbps_full_config = 
{
    .common_cfg = &xcvr_common_config,
    .mode_data_rate_cfg = &xcvr_msk_500kbps_config,
};

/*! @brief  MSK mode & datarate 1Mbps with 250Kbps alternate rate configuration. */
const xcvr_config_t xcvr_msk_250kbps_full_config =
{
    .common_cfg = &xcvr_common_config,
    .mode_data_rate_cfg = &xcvr_msk_250kbps_config,
};


