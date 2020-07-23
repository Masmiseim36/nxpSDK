/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "nxp2p4_xcvr.h"
#include "nxp_xcvr_mode_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Performs register configuration for the radio that handles A0 vs. B0 revision.
 *
 * This function initializes the radio registers with settings that must be manually configured in 
 * order to support both A0 and B0 silicon revisions in the same driver.
 *
 * @param[in] xcvr_config  Pointer to a pointer to the complete radio settings structure.
 *
 * @return Status of the call.
 */
static xcvrStatus_t configure_A0_B0_Radio_deltas(const xcvr_config_t * xcvr_config);

/*!
 * @brief Performs register configuration for the RBME that handles A0 vs. B0 revision.
 *
 * This function initializes the RBME registers with settings that must be manually configured in 
 * order to support both A0 and B0 silicon revisions in the same driver.
 *
 * @return Status of the call.
 */
static xcvrStatus_t configure_A0_B0_RBME_deltas(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 *******************************************************************************/

xcvrStatus_t XCVR_RadioGenRegSetup(const xcvr_config_t * xcvr_config)
{
    xcvrStatus_t status = gXcvrInvalidConfiguration_c;
    
    /* Check parameter */
    if(xcvr_config != NULLPTR)
    {
        /* Generated XCVR_RadioRegSetup for all XCVR registers goes here */
        /***********************************************/
        /*********** START OF GENERATED CODE ***********/
        /**************** XCVR_RadioRegSetup ***********/
        /***********************************************/
        const xcvr_common_config_t *common_cfg = xcvr_config->common_cfg;
        const xcvr_mode_datarate_config_t * mode_datarate_cfg = xcvr_config->mode_data_rate_cfg;
        /**********************/
        /* RADIO_CTRL configs */
        /**********************/
        RADIO_CTRL->LL_CTRL = mode_datarate_cfg->ll_ctrl;
        /***************************/
        /* XCVR_2P4GHZ_PHY configs */
        /***************************/
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[0].DMD_WAVE_REG0 = mode_datarate_cfg->demod_wave[0].dmd_wave_reg0;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[0].DMD_WAVE_REG1 = mode_datarate_cfg->demod_wave[0].dmd_wave_reg1;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[0].DMD_WAVE_REG2 = mode_datarate_cfg->demod_wave[0].dmd_wave_reg2;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[1].DMD_WAVE_REG0 = mode_datarate_cfg->demod_wave[1].dmd_wave_reg0;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[1].DMD_WAVE_REG1 = mode_datarate_cfg->demod_wave[1].dmd_wave_reg1;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[1].DMD_WAVE_REG2 = mode_datarate_cfg->demod_wave[1].dmd_wave_reg2;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[2].DMD_WAVE_REG0 = mode_datarate_cfg->demod_wave[2].dmd_wave_reg0;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[2].DMD_WAVE_REG1 = mode_datarate_cfg->demod_wave[2].dmd_wave_reg1;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[2].DMD_WAVE_REG2 = mode_datarate_cfg->demod_wave[2].dmd_wave_reg2;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[3].DMD_WAVE_REG0 = mode_datarate_cfg->demod_wave[3].dmd_wave_reg0;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[3].DMD_WAVE_REG1 = mode_datarate_cfg->demod_wave[3].dmd_wave_reg1;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[3].DMD_WAVE_REG2 = mode_datarate_cfg->demod_wave[3].dmd_wave_reg2;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[4].DMD_WAVE_REG0 = mode_datarate_cfg->demod_wave[4].dmd_wave_reg0;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[4].DMD_WAVE_REG1 = mode_datarate_cfg->demod_wave[4].dmd_wave_reg1;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[4].DMD_WAVE_REG2 = mode_datarate_cfg->demod_wave[4].dmd_wave_reg2;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[5].DMD_WAVE_REG0 = mode_datarate_cfg->demod_wave[5].dmd_wave_reg0;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[5].DMD_WAVE_REG1 = mode_datarate_cfg->demod_wave[5].dmd_wave_reg1;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[5].DMD_WAVE_REG2 = mode_datarate_cfg->demod_wave[5].dmd_wave_reg2;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[6].DMD_WAVE_REG0 = mode_datarate_cfg->demod_wave[6].dmd_wave_reg0;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[6].DMD_WAVE_REG1 = mode_datarate_cfg->demod_wave[6].dmd_wave_reg1;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[6].DMD_WAVE_REG2 = mode_datarate_cfg->demod_wave[6].dmd_wave_reg2;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[7].DMD_WAVE_REG0 = mode_datarate_cfg->demod_wave[7].dmd_wave_reg0;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[7].DMD_WAVE_REG1 = mode_datarate_cfg->demod_wave[7].dmd_wave_reg1;
        XCVR_2P4GHZ_PHY->DEMOD_WAVE[7].DMD_WAVE_REG2 = mode_datarate_cfg->demod_wave[7].dmd_wave_reg2;
        XCVR_2P4GHZ_PHY->DMD_CTRL0 = common_cfg->dmd_ctrl0;
        XCVR_2P4GHZ_PHY->DMD_CTRL1 = common_cfg->dmd_ctrl1;
        XCVR_2P4GHZ_PHY->FSK_CFG0 = mode_datarate_cfg->fsk_cfg0 |
                                    common_cfg->fsk_cfg0;
        XCVR_2P4GHZ_PHY->FSK_CFG1 = mode_datarate_cfg->fsk_cfg1;
        XCVR_2P4GHZ_PHY->FSK_CFG2 = mode_datarate_cfg->fsk_cfg2 |
                                    common_cfg->fsk_cfg2;
        XCVR_2P4GHZ_PHY->FSK_CFG3 = mode_datarate_cfg->fsk_cfg3;
        XCVR_2P4GHZ_PHY->FSK_FAD_CFG = common_cfg->fsk_fad_cfg;
        XCVR_2P4GHZ_PHY->FSK_FAD_CTRL = common_cfg->fsk_fad_ctrl;
        XCVR_2P4GHZ_PHY->FSK_PD_CFG0 = mode_datarate_cfg->fsk_pd_cfg0;
        XCVR_2P4GHZ_PHY->FSK_PD_CFG1 = mode_datarate_cfg->fsk_pd_cfg1;
        XCVR_2P4GHZ_PHY->FSK_PD_CFG2 = mode_datarate_cfg->fsk_pd_cfg2;
        XCVR_2P4GHZ_PHY->FSK_PD_PH[0] = mode_datarate_cfg->fsk_pd_ph[0];
        XCVR_2P4GHZ_PHY->FSK_PD_PH[1] = mode_datarate_cfg->fsk_pd_ph[1];
        XCVR_2P4GHZ_PHY->FSK_PT = common_cfg->fsk_pt;
        XCVR_2P4GHZ_PHY->LR_AA_CFG = mode_datarate_cfg->lr_aa_cfg;
        XCVR_2P4GHZ_PHY->LR_PD_CFG = mode_datarate_cfg->lr_pd_cfg;
        XCVR_2P4GHZ_PHY->LR_PD_PH[0] = mode_datarate_cfg->lr_pd_ph[0];
        XCVR_2P4GHZ_PHY->LR_PD_PH[1] = mode_datarate_cfg->lr_pd_ph[1];
        XCVR_2P4GHZ_PHY->LR_PD_PH[2] = mode_datarate_cfg->lr_pd_ph[2];
        XCVR_2P4GHZ_PHY->LR_PD_PH[3] = mode_datarate_cfg->lr_pd_ph[3];
        XCVR_2P4GHZ_PHY->MISC = common_cfg->misc;
        XCVR_2P4GHZ_PHY->PREPHY_MISC = common_cfg->prephy_misc;
        XCVR_2P4GHZ_PHY->SM_CFG = mode_datarate_cfg->sm_cfg |
                                    common_cfg->sm_cfg;
        /***********************/
        /* XCVR_ANALOG configs */
        /***********************/
        XCVR_ANALOG->BB_LDO_1 = common_cfg->bb_ldo_1;
        XCVR_ANALOG->BB_LDO_2 = common_cfg->bb_ldo_2;
        XCVR_ANALOG->RX_AUXPLL = common_cfg->rx_auxpll;
        XCVR_ANALOG->RX_BBA = mode_datarate_cfg->rx_bba |
                                    common_cfg->rx_bba;
        XCVR_ANALOG->RX_LNA = common_cfg->rx_lna;
        XCVR_ANALOG->RX_TZA = common_cfg->rx_tza |
                                    mode_datarate_cfg->rx_tza;
        XCVR_ANALOG->SY_CTRL_1 = common_cfg->sy_ctrl_1;
        XCVR_ANALOG->SY_CTRL_2 = common_cfg->sy_ctrl_2 |
                                    mode_datarate_cfg->sy_ctrl_2;
        XCVR_ANALOG->TX_DAC_PA = common_cfg->tx_dac_pa;
        /*********************/
        /* XCVR_MISC configs */
        /*********************/
        XCVR_MISC->XCVR_CTRL = common_cfg->xcvr_ctrl |
                                    mode_datarate_cfg->xcvr_ctrl;
        /************************/
        /* XCVR_PLL_DIG configs */
        /************************/
        XCVR_PLL_DIG->BALUN_RX = common_cfg->balun_rx;
        XCVR_PLL_DIG->BALUN_TX = common_cfg->balun_tx;
        XCVR_PLL_DIG->CHAN_MAP = mode_datarate_cfg->chan_map |
                                    common_cfg->chan_map;
        XCVR_PLL_DIG->CHAN_MAP_EXT = common_cfg->chan_map_ext;
        XCVR_PLL_DIG->DATA_RATE_OVRD_CTRL1 = mode_datarate_cfg->data_rate_ovrd_ctrl1 |
                                    common_cfg->data_rate_ovrd_ctrl1;
        XCVR_PLL_DIG->DELAY_MATCH = common_cfg->delay_match;
        XCVR_PLL_DIG->HPM_BUMP = common_cfg->hpm_bump;
        XCVR_PLL_DIG->HPM_CTRL = common_cfg->hpm_ctrl;
        XCVR_PLL_DIG->HPM_SDM_RES = common_cfg->hpm_sdm_res;
        XCVR_PLL_DIG->LOCK_DETECT = common_cfg->lock_detect;
        XCVR_PLL_DIG->LPM_CTRL = common_cfg->lpm_ctrl;
        XCVR_PLL_DIG->LPM_SDM_CTRL1 = common_cfg->lpm_sdm_ctrl1;
        XCVR_PLL_DIG->MOD_CTRL = common_cfg->mod_ctrl;
        /*************************/
        /* XCVR_PLL_DIG2 configs */
        /*************************/
        XCVR_PLL_DIG2->HPMCAL_CTRL = mode_datarate_cfg->hpmcal_ctrl |
                                    common_cfg->hpmcal_ctrl;
        /***********************/
        /* XCVR_RX_DIG configs */
        /***********************/
        XCVR_RX_DIG->AGC_CTRL_0 = common_cfg->agc_ctrl_0 |
                                    mode_datarate_cfg->agc_ctrl_0;
        XCVR_RX_DIG->AGC_CTRL_0_DRS = mode_datarate_cfg->agc_ctrl_0_drs;
        XCVR_RX_DIG->AGC_CTRL_1 = common_cfg->agc_ctrl_1 |
                                    mode_datarate_cfg->agc_ctrl_1;
        XCVR_RX_DIG->AGC_CTRL_1_DRS = mode_datarate_cfg->agc_ctrl_1_drs |
                                    common_cfg->agc_ctrl_1_drs;
        XCVR_RX_DIG->AGC_CTRL_2 = mode_datarate_cfg->agc_ctrl_2 |
                                    common_cfg->agc_ctrl_2;
        XCVR_RX_DIG->AGC_CTRL_2_DRS = mode_datarate_cfg->agc_ctrl_2_drs |
                                    common_cfg->agc_ctrl_2_drs;
        XCVR_RX_DIG->AGC_CTRL_3 = common_cfg->agc_ctrl_3 |
                                    mode_datarate_cfg->agc_ctrl_3;
        XCVR_RX_DIG->AGC_CTRL_4 = common_cfg->agc_ctrl_4 |
                                    mode_datarate_cfg->agc_ctrl_4;
        XCVR_RX_DIG->AGC_GAIN_TBL_03_00 = common_cfg->agc_gain_tbl_03_00;
        XCVR_RX_DIG->AGC_GAIN_TBL_07_04 = common_cfg->agc_gain_tbl_07_04;
        XCVR_RX_DIG->AGC_GAIN_TBL_11_08 = common_cfg->agc_gain_tbl_11_08;
        XCVR_RX_DIG->AGC_GAIN_TBL_15_12 = common_cfg->agc_gain_tbl_15_12;
        XCVR_RX_DIG->AGC_GAIN_TBL_19_16 = common_cfg->agc_gain_tbl_19_16;
        XCVR_RX_DIG->AGC_GAIN_TBL_23_20 = common_cfg->agc_gain_tbl_23_20;
        XCVR_RX_DIG->AGC_GAIN_TBL_26_24 = common_cfg->agc_gain_tbl_26_24;
        XCVR_RX_DIG->AGC_MAN_AGC_IDX = common_cfg->agc_man_agc_idx;
        XCVR_RX_DIG->BBA_RES_TUNE_LIN_VAL_10_8 = common_cfg->bba_res_tune_lin_val_10_8;
        XCVR_RX_DIG->BBA_RES_TUNE_LIN_VAL_3_0 = common_cfg->bba_res_tune_lin_val_3_0;
        XCVR_RX_DIG->BBA_RES_TUNE_LIN_VAL_7_4 = common_cfg->bba_res_tune_lin_val_7_4;
        XCVR_RX_DIG->BBA_RES_TUNE_VAL_10_8 = common_cfg->bba_res_tune_val_10_8;
        XCVR_RX_DIG->BBA_RES_TUNE_VAL_7_0 = common_cfg->bba_res_tune_val_7_0;
        XCVR_RX_DIG->CCA_ED_LQI_CTRL_0 = common_cfg->cca_ed_lqi_ctrl_0;
        XCVR_RX_DIG->CCA_ED_LQI_CTRL_1 = common_cfg->cca_ed_lqi_ctrl_1;
        XCVR_RX_DIG->DCOC_BBA_STEP = common_cfg->dcoc_bba_step;
        XCVR_RX_DIG->DCOC_CAL_FAIL_TH = common_cfg->dcoc_cal_fail_th;
        XCVR_RX_DIG->DCOC_CAL_GAIN = common_cfg->dcoc_cal_gain;
        XCVR_RX_DIG->DCOC_CAL_IIR = mode_datarate_cfg->dcoc_cal_iir;
        XCVR_RX_DIG->DCOC_CAL_PASS_TH = common_cfg->dcoc_cal_pass_th;
        XCVR_RX_DIG->DCOC_CAL_RCP = common_cfg->dcoc_cal_rcp;
        XCVR_RX_DIG->DCOC_CTRL_0 = common_cfg->dcoc_ctrl_0 |
                                    mode_datarate_cfg->dcoc_ctrl_0;
        XCVR_RX_DIG->DCOC_CTRL_1 = common_cfg->dcoc_ctrl_1 |
                                    mode_datarate_cfg->dcoc_ctrl_1;
        XCVR_RX_DIG->DCOC_CTRL_2 = mode_datarate_cfg->dcoc_ctrl_2;
        XCVR_RX_DIG->DCOC_CTRL_DRS = mode_datarate_cfg->dcoc_ctrl_drs;
        XCVR_RX_DIG->DCOC_TZA_STEP_0 = common_cfg->dcoc_tza_step_0;
        XCVR_RX_DIG->DCOC_TZA_STEP_1 = common_cfg->dcoc_tza_step_1;
        XCVR_RX_DIG->DCOC_TZA_STEP_10 = common_cfg->dcoc_tza_step_10;
        XCVR_RX_DIG->DCOC_TZA_STEP_2 = common_cfg->dcoc_tza_step_2;
        XCVR_RX_DIG->DCOC_TZA_STEP_3 = common_cfg->dcoc_tza_step_3;
        XCVR_RX_DIG->DCOC_TZA_STEP_4 = common_cfg->dcoc_tza_step_4;
        XCVR_RX_DIG->DCOC_TZA_STEP_5 = common_cfg->dcoc_tza_step_5;
        XCVR_RX_DIG->DCOC_TZA_STEP_6 = common_cfg->dcoc_tza_step_6;
        XCVR_RX_DIG->DCOC_TZA_STEP_7 = common_cfg->dcoc_tza_step_7;
        XCVR_RX_DIG->DCOC_TZA_STEP_8 = common_cfg->dcoc_tza_step_8;
        XCVR_RX_DIG->DCOC_TZA_STEP_9 = common_cfg->dcoc_tza_step_9;
        XCVR_RX_DIG->DC_RESID_CTRL = common_cfg->dc_resid_ctrl |
                                    mode_datarate_cfg->dc_resid_ctrl;
        XCVR_RX_DIG->DC_RESID_CTRL_DRS = mode_datarate_cfg->dc_resid_ctrl_drs;
        XCVR_RX_DIG->LNA_GAIN_LIN_VAL_2_0 = common_cfg->lna_gain_lin_val_2_0;
        XCVR_RX_DIG->LNA_GAIN_LIN_VAL_5_3 = common_cfg->lna_gain_lin_val_5_3;
        XCVR_RX_DIG->LNA_GAIN_LIN_VAL_8_6 = common_cfg->lna_gain_lin_val_8_6;
        XCVR_RX_DIG->LNA_GAIN_LIN_VAL_9 = common_cfg->lna_gain_lin_val_9;
        XCVR_RX_DIG->LNA_GAIN_VAL_3_0 = common_cfg->lna_gain_val_3_0;
        XCVR_RX_DIG->LNA_GAIN_VAL_7_4 = common_cfg->lna_gain_val_7_4;
        XCVR_RX_DIG->LNA_GAIN_VAL_9_8 = common_cfg->lna_gain_val_9_8;
        XCVR_RX_DIG->RSSI_CTRL_0 = common_cfg->rssi_ctrl_0;
        XCVR_RX_DIG->RSSI_CTRL_1 = common_cfg->rssi_ctrl_1;
        XCVR_RX_DIG->RX_CHF_COEF_0 = mode_datarate_cfg->rx_chf_coef_0;
        XCVR_RX_DIG->RX_CHF_COEF_0_DRS = mode_datarate_cfg->rx_chf_coef_0_drs;
        XCVR_RX_DIG->RX_CHF_COEF_1 = mode_datarate_cfg->rx_chf_coef_1;
        XCVR_RX_DIG->RX_CHF_COEF_10 = mode_datarate_cfg->rx_chf_coef_10;
        XCVR_RX_DIG->RX_CHF_COEF_10_DRS = mode_datarate_cfg->rx_chf_coef_10_drs;
        XCVR_RX_DIG->RX_CHF_COEF_11 = mode_datarate_cfg->rx_chf_coef_11;
        XCVR_RX_DIG->RX_CHF_COEF_11_DRS = mode_datarate_cfg->rx_chf_coef_11_drs;
        XCVR_RX_DIG->RX_CHF_COEF_1_DRS = mode_datarate_cfg->rx_chf_coef_1_drs;
        XCVR_RX_DIG->RX_CHF_COEF_2 = mode_datarate_cfg->rx_chf_coef_2;
        XCVR_RX_DIG->RX_CHF_COEF_2_DRS = mode_datarate_cfg->rx_chf_coef_2_drs;
        XCVR_RX_DIG->RX_CHF_COEF_3 = mode_datarate_cfg->rx_chf_coef_3;
        XCVR_RX_DIG->RX_CHF_COEF_3_DRS = mode_datarate_cfg->rx_chf_coef_3_drs;
        XCVR_RX_DIG->RX_CHF_COEF_4 = mode_datarate_cfg->rx_chf_coef_4;
        XCVR_RX_DIG->RX_CHF_COEF_4_DRS = mode_datarate_cfg->rx_chf_coef_4_drs;
        XCVR_RX_DIG->RX_CHF_COEF_5 = mode_datarate_cfg->rx_chf_coef_5;
        XCVR_RX_DIG->RX_CHF_COEF_5_DRS = mode_datarate_cfg->rx_chf_coef_5_drs;
        XCVR_RX_DIG->RX_CHF_COEF_6 = mode_datarate_cfg->rx_chf_coef_6;
        XCVR_RX_DIG->RX_CHF_COEF_6_DRS = mode_datarate_cfg->rx_chf_coef_6_drs;
        XCVR_RX_DIG->RX_CHF_COEF_7 = mode_datarate_cfg->rx_chf_coef_7;
        XCVR_RX_DIG->RX_CHF_COEF_7_DRS = mode_datarate_cfg->rx_chf_coef_7_drs;
        XCVR_RX_DIG->RX_CHF_COEF_8 = mode_datarate_cfg->rx_chf_coef_8;
        XCVR_RX_DIG->RX_CHF_COEF_8_DRS = mode_datarate_cfg->rx_chf_coef_8_drs;
        XCVR_RX_DIG->RX_CHF_COEF_9 = mode_datarate_cfg->rx_chf_coef_9;
        XCVR_RX_DIG->RX_CHF_COEF_9_DRS = mode_datarate_cfg->rx_chf_coef_9_drs;
        XCVR_RX_DIG->RX_DIG_CTRL = common_cfg->rx_dig_ctrl |
                                    mode_datarate_cfg->rx_dig_ctrl;
        XCVR_RX_DIG->RX_DIG_CTRL_DRS = mode_datarate_cfg->rx_dig_ctrl_drs;
        XCVR_RX_DIG->RX_D_CHF_COEF_0 = mode_datarate_cfg->rx_d_chf_coef_0;
        XCVR_RX_DIG->RX_D_CHF_COEF_0_DRS = mode_datarate_cfg->rx_d_chf_coef_0_drs;
        XCVR_RX_DIG->RX_D_CHF_COEF_1 = mode_datarate_cfg->rx_d_chf_coef_1;
        XCVR_RX_DIG->RX_D_CHF_COEF_1_DRS = mode_datarate_cfg->rx_d_chf_coef_1_drs;
        XCVR_RX_DIG->RX_D_CHF_COEF_2 = mode_datarate_cfg->rx_d_chf_coef_2;
        XCVR_RX_DIG->RX_D_CHF_COEF_2_DRS = mode_datarate_cfg->rx_d_chf_coef_2_drs;
        XCVR_RX_DIG->RX_D_CHF_COEF_3 = mode_datarate_cfg->rx_d_chf_coef_3;
        XCVR_RX_DIG->RX_D_CHF_COEF_3_DRS = mode_datarate_cfg->rx_d_chf_coef_3_drs;
        XCVR_RX_DIG->RX_D_CHF_COEF_4 = mode_datarate_cfg->rx_d_chf_coef_4;
        XCVR_RX_DIG->RX_D_CHF_COEF_4_DRS = mode_datarate_cfg->rx_d_chf_coef_4_drs;
        XCVR_RX_DIG->RX_RCCAL_CTRL0 = mode_datarate_cfg->rx_rccal_ctrl0;
        XCVR_RX_DIG->RX_RCCAL_CTRL1 = mode_datarate_cfg->rx_rccal_ctrl1;
        /********************/
        /* XCVR_TSM configs */
        /********************/
        XCVR_TSM->CTRL = common_cfg->ctrl;
        XCVR_TSM->END_OF_SEQ = common_cfg->end_of_seq;
        XCVR_TSM->FAST_CTRL1 = common_cfg->fast_ctrl1;
        XCVR_TSM->FAST_CTRL2 = common_cfg->fast_ctrl2;
        XCVR_TSM->OVRD1 = common_cfg->ovrd1;
        XCVR_TSM->OVRD2 = common_cfg->ovrd2;
        XCVR_TSM->RECYCLE_COUNT = common_cfg->recycle_count;
        XCVR_TSM->TIMING00 = common_cfg->timing00;
        XCVR_TSM->TIMING01 = common_cfg->timing01;
        XCVR_TSM->TIMING02 = common_cfg->timing02;
        XCVR_TSM->TIMING03 = common_cfg->timing03;
        XCVR_TSM->TIMING04 = common_cfg->timing04;
        XCVR_TSM->TIMING05 = common_cfg->timing05;
        XCVR_TSM->TIMING06 = common_cfg->timing06;
        XCVR_TSM->TIMING07 = common_cfg->timing07;
        XCVR_TSM->TIMING08 = common_cfg->timing08;
        XCVR_TSM->TIMING09 = common_cfg->timing09;
        XCVR_TSM->TIMING10 = common_cfg->timing10;
        XCVR_TSM->TIMING11 = common_cfg->timing11;
        XCVR_TSM->TIMING12 = common_cfg->timing12;
        XCVR_TSM->TIMING13 = common_cfg->timing13;
        XCVR_TSM->TIMING14 = common_cfg->timing14;
        XCVR_TSM->TIMING15 = common_cfg->timing15;
        XCVR_TSM->TIMING16 = common_cfg->timing16;
        XCVR_TSM->TIMING17 = common_cfg->timing17;
        XCVR_TSM->TIMING18 = common_cfg->timing18;
        XCVR_TSM->TIMING19 = common_cfg->timing19;
        XCVR_TSM->TIMING20 = common_cfg->timing20;
        XCVR_TSM->TIMING21 = common_cfg->timing21;
        XCVR_TSM->TIMING22 = common_cfg->timing22;
        XCVR_TSM->TIMING23 = common_cfg->timing23;
        XCVR_TSM->TIMING24 = common_cfg->timing24;
        XCVR_TSM->TIMING25 = common_cfg->timing25;
        XCVR_TSM->TIMING27 = common_cfg->timing27;
        XCVR_TSM->TIMING28 = common_cfg->timing28;
        XCVR_TSM->TIMING29 = common_cfg->timing29;
        XCVR_TSM->TIMING30 = common_cfg->timing30;
        XCVR_TSM->TIMING31 = common_cfg->timing31;
        XCVR_TSM->TIMING32 = common_cfg->timing32;
        XCVR_TSM->TIMING33 = common_cfg->timing33;
        XCVR_TSM->TIMING34 = common_cfg->timing34;
        XCVR_TSM->TIMING35 = common_cfg->timing35;
        XCVR_TSM->TIMING36 = common_cfg->timing36;
        XCVR_TSM->TIMING37 = common_cfg->timing37;
        XCVR_TSM->TIMING38 = common_cfg->timing38;
        XCVR_TSM->TIMING39 = common_cfg->timing39;
        XCVR_TSM->TIMING40 = common_cfg->timing40;
        XCVR_TSM->TIMING41 = common_cfg->timing41;
        XCVR_TSM->TIMING42 = common_cfg->timing42;
        XCVR_TSM->TIMING43 = common_cfg->timing43;
        XCVR_TSM->TIMING44 = common_cfg->timing44;
        XCVR_TSM->TIMING45 = common_cfg->timing45;
        XCVR_TSM->TIMING46 = common_cfg->timing46;
        XCVR_TSM->TIMING47 = common_cfg->timing47;
        XCVR_TSM->TIMING48 = common_cfg->timing48;
        XCVR_TSM->TIMING49 = common_cfg->timing49;
        XCVR_TSM->TIMING50 = common_cfg->timing50;
        XCVR_TSM->TIMING51 = common_cfg->timing51;
        XCVR_TSM->TIMING52 = common_cfg->timing52;
        XCVR_TSM->TIMING53 = common_cfg->timing53;
        XCVR_TSM->TIMING54 = common_cfg->timing54;
        XCVR_TSM->TIMING55 = common_cfg->timing55;
        XCVR_TSM->TIMING56 = common_cfg->timing56;
        XCVR_TSM->TIMING57 = common_cfg->timing57;
        XCVR_TSM->TIMING58 = common_cfg->timing58;
        XCVR_TSM->WU_LATENCY = common_cfg->wu_latency;
        /***********************/
        /* XCVR_TX_DIG configs */
        /***********************/
        XCVR_TX_DIG->DATARATE_CONFIG_FILTER_CTRL = mode_datarate_cfg->datarate_config_filter_ctrl;
        XCVR_TX_DIG->DATARATE_CONFIG_FSK_CTRL = mode_datarate_cfg->datarate_config_fsk_ctrl;
        XCVR_TX_DIG->DATARATE_CONFIG_GFSK_CTRL = mode_datarate_cfg->datarate_config_gfsk_ctrl;
        XCVR_TX_DIG->DATA_PADDING_CTRL = common_cfg->data_padding_ctrl;
        XCVR_TX_DIG->DATA_PADDING_CTRL_1 = common_cfg->data_padding_ctrl_1 |
                                    mode_datarate_cfg->data_padding_ctrl_1;
        XCVR_TX_DIG->DATA_PADDING_CTRL_2 = common_cfg->data_padding_ctrl_2;
        XCVR_TX_DIG->FSK_CTRL = mode_datarate_cfg->fsk_ctrl;
        XCVR_TX_DIG->GFSK_COEFF_0_1 = mode_datarate_cfg->gfsk_coeff_0_1;
        XCVR_TX_DIG->GFSK_COEFF_2_3 = mode_datarate_cfg->gfsk_coeff_2_3;
        XCVR_TX_DIG->GFSK_COEFF_4_5 = mode_datarate_cfg->gfsk_coeff_4_5;
        XCVR_TX_DIG->GFSK_COEFF_6_7 = mode_datarate_cfg->gfsk_coeff_6_7;
        XCVR_TX_DIG->GFSK_CTRL = common_cfg->gfsk_ctrl |
                                    mode_datarate_cfg->gfsk_ctrl;
        XCVR_TX_DIG->IMAGE_FILTER_CTRL = common_cfg->image_filter_ctrl |
                                    mode_datarate_cfg->image_filter_ctrl;
        XCVR_TX_DIG->PA_CTRL = common_cfg->pa_ctrl;
        XCVR_TX_DIG->PA_RAMP_TBL0 = common_cfg->pa_ramp_tbl0;
        XCVR_TX_DIG->PA_RAMP_TBL1 = common_cfg->pa_ramp_tbl1;
        XCVR_TX_DIG->PA_RAMP_TBL2 = common_cfg->pa_ramp_tbl2;
        XCVR_TX_DIG->PA_RAMP_TBL3 = common_cfg->pa_ramp_tbl3;
        XCVR_TX_DIG->SWITCH_TX_CTRL = common_cfg->switch_tx_ctrl;
        XCVR_TX_DIG->TXDIG_CTRL = common_cfg->txdig_ctrl |
                                    mode_datarate_cfg->txdig_ctrl;
        /* Return status */
        status = gXcvrSuccess_c;
        /***********************************************/
        /************ END OF GENERATED CODE ************/
        /**************** XCVR_RadioRegSetup ***********/
        /***********************************************/
    }

    /* Perform A0/B0 custom configurations */
    if (status == gXcvrSuccess_c)
    {
        status = configure_A0_B0_Radio_deltas(xcvr_config); /* NULLPTR already checked above */
    }

    return status;    
}

xcvrStatus_t XCVR_RadioGenRBMESetup(const xcvr_coding_config_t * rbme)
{
    xcvrStatus_t status = gXcvrInvalidConfiguration_c;

    /* Check parameter */    
    if(rbme != NULLPTR)
    {
        /* Generated XCVR_RadioGenRBMESetup for all RBME registers goes here */
        /***********************************************/
        /*********** START OF GENERATED CODE ***********/
        /************** XCVR_RadioGenRBMESetup *********/
        /***********************************************/
        /****************/
        /* RBME configs */
        /****************/
        RBME->CRCW_CFG = rbme->crcw_cfg;
        RBME->CRCW_CFG2 = rbme->crcw_cfg2;
        RBME->CRCW_CFG3 = rbme->crcw_cfg3;
        RBME->CRC_INIT = rbme->crc_init;
        RBME->CRC_PHR_SZ = rbme->crc_phr_sz;
        RBME->CRC_POLY = rbme->crc_poly;
        RBME->CRC_XOR_OUT = rbme->crc_xor_out;
        RBME->FCP_CFG = rbme->fcp_cfg;
        RBME->FEC_BSZ_OV_B4SP = rbme->fec_bsz_ov_b4sp;
        RBME->FEC_CFG1 = rbme->fec_cfg1;
        RBME->FEC_CFG2 = rbme->fec_cfg2;
        RBME->FRAME_OVER_SZ = rbme->frame_over_sz;
        RBME->LEG0_CFG = rbme->leg0_cfg;
        RBME->NPAYL_OVER_SZ = rbme->npayl_over_sz;
        RBME->PKT_SZ = rbme->pkt_sz;
        RBME->RAM_D_ADDR = rbme->ram_d_addr;
        RBME->RAM_IF_CFG = rbme->ram_if_cfg;
        RBME->RAM_S_ADDR = rbme->ram_s_addr;
        RBME->RBME_RST = rbme->rbme_rst;
        RBME->SPREAD_CFG = rbme->spread_cfg;
        RBME->WHITEN_CFG = rbme->whiten_cfg;
        RBME->WHITEN_POLY = rbme->whiten_poly;
        RBME->WHITEN_SZ_THR = rbme->whiten_sz_thr;
        RBME->WHT_CFG = rbme->wht_cfg;
        /***************************/
        /* XCVR_2P4GHZ_PHY configs */
        /***************************/
        XCVR_2P4GHZ_PHY->DMD_CTRL1 = rbme->dmd_ctrl1;
        /* Return status */
        status = gXcvrSuccess_c;
        /***********************************************/
        /************ END OF GENERATED CODE ************/
        /************** XCVR_RadioGenRBMESetup *********/
        /***********************************************/
    }

    /* Perform A0/B0 custom configurations */
    if (status == gXcvrSuccess_c)
    {
        status = configure_A0_B0_RBME_deltas();
    }
    
    return status;    
}

static xcvrStatus_t configure_A0_B0_Radio_deltas(const xcvr_config_t * xcvr_config)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    /* Perform radio configuration per radio version */
    if ((XCVR_ReadRadioVer() == (uint16_t)XCVR_RADIO_GEN_3P5) )
    {
        /* KW37/38/39 A0 revision  *********************************************** */
        /* A0 silicon support requires custom defines from the A0 header file. */
#define A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN_MASK (0x3FU)
#define A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN_SHIFT (0U)
#define A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN(x) (((uint32_t)(((uint32_t)(x)) << A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN_SHIFT))&A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN_MASK)
#define A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_FED_ACT_WIN_MASK (0x3F00U)
#define A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_FED_ACT_WIN_SHIFT (8U)
#define A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_FED_ACT_WIN(x) (((uint32_t)(((uint32_t)(x)) << A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_FED_ACT_WIN_SHIFT))&A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_FED_ACT_WIN_MASK)

        /* Configure XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN */
        XCVR_2P4GHZ_PHY->DMD_CTRL0 &= ~(A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN_MASK | A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_FED_ACT_WIN_MASK);
        XCVR_2P4GHZ_PHY->DMD_CTRL0 |= (A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN(0x10U) | A0_XCVR_2P4GHZ_PHY_DMD_CTRL0_FED_ACT_WIN(0x10U));

        const xcvr_common_config_t *common_cfg = xcvr_config->common_cfg;
        if (common_cfg == NULLPTR)
        {
            status = gXcvrInvalidParameters_c;
        }
        else
        {
            /* Create a additive factor to add the tx_ofst_usec to the TSM timings on the fly. This factor applies only to the A0 silicon */
            /* For A0 support, this replaces previous code which was only driven by a compile option. */
            uint32_t offset_adder = B1(tx_ofst_usec) | B0(tx_ofst_usec); /* Must apply offset to both TX_HI and TX_LO fields of TSM */
            XCVR_TSM->END_OF_SEQ = common_cfg->end_of_seq+offset_adder;
            XCVR_TSM->TIMING00 = common_cfg->timing00+offset_adder;
            /* No offset to TIMING02 */
            XCVR_TSM->TIMING01 = common_cfg->timing01+offset_adder;
            XCVR_TSM->TIMING03 = common_cfg->timing03+offset_adder;
            XCVR_TSM->TIMING04 = common_cfg->timing04+offset_adder;
            XCVR_TSM->TIMING05 = common_cfg->timing05+offset_adder;
            XCVR_TSM->TIMING06 = common_cfg->timing06+offset_adder;
            XCVR_TSM->TIMING07 = common_cfg->timing07+offset_adder;
            XCVR_TSM->TIMING08 = common_cfg->timing08+offset_adder;
            XCVR_TSM->TIMING09 = common_cfg->timing09+offset_adder;
            XCVR_TSM->TIMING10 = common_cfg->timing10+offset_adder;
            XCVR_TSM->TIMING11 = common_cfg->timing11+offset_adder;
            /* No offset to TIMING12 */
            XCVR_TSM->TIMING13 = common_cfg->timing13+offset_adder;
            /* No offset to TIMING14 */
            XCVR_TSM->TIMING15 = common_cfg->timing15+offset_adder;
            /* No offset to TIMING16 */
            XCVR_TSM->TIMING17 = common_cfg->timing17+offset_adder;
            XCVR_TSM->TIMING18 = common_cfg->timing18+offset_adder;
            XCVR_TSM->TIMING19 = common_cfg->timing19+offset_adder;
            XCVR_TSM->TIMING20 = common_cfg->timing20+offset_adder;
            XCVR_TSM->TIMING21 = common_cfg->timing21+offset_adder;
            /* No offset to TIMING22 */
            XCVR_TSM->TIMING23 = common_cfg->timing23+offset_adder;
            XCVR_TSM->TIMING24 = common_cfg->timing24+offset_adder;
            /* No offset to TIMING25 hrough TIMING33 */
            XCVR_TSM->TIMING34 = common_cfg->timing34+offset_adder;
            XCVR_TSM->TIMING35 = common_cfg->timing35+offset_adder;
            /* No offset to TIMING36 */
            /* No offset to TIMING37 */
            XCVR_TSM->TIMING38 = common_cfg->timing38+offset_adder;
            /* No offset to TIMING39 through TIMING57 */
            XCVR_TSM->TIMING58 = common_cfg->timing58+offset_adder;
        }

        tx_rx_synth_delay = (uint16_t)TX_RX_SYNTH_init_A0; /* override the value with A0 settings */
    }
    else
    {
         if (XCVR_ReadRadioVer() == (uint16_t)XCVR_RADIO_GEN_3P5_B0)
         {
             /* KW37/38/39 B0 revision *********************************************** */
             /* B0 silicon support assumes that the B0 header file is active in the MCU SDK */
             /* Configure XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN */
             XCVR_2P4GHZ_PHY->DMD_CTRL0 &= ~(XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN_MASK);
             XCVR_2P4GHZ_PHY->DMD_CTRL0 |= XCVR_2P4GHZ_PHY_DMD_CTRL0_TED_ACT_WIN(0x3U);
             /* Configure DC_RESID_CTRL2, only exists in B0 */
             XCVR_RX_DIG->DC_RESID_CTRL2 = ( XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_NWIN2(128U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_PHY_STOP_EN(1U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_CC_EN(1U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_SR2_EN(0U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_ALPHA2(3U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_GS2_EN(0U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_ITER_FREEZE2(10U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_SLEWRATE2(3U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_MIN_AGC_IDX2(26U) |
                                             XCVR_RX_DIG_DC_RESID_CTRL2_DC_RESID_GEARSHIFT2(2U) );
             tx_rx_synth_delay = (uint16_t)TX_RX_SYNTH_init_B0; /* override the value with B0 settings */
         }
         else
         {
             status = gXcvrInvalidConfiguration_c; /* RADIO_VER value incorrect for this software */
         }
    }
        
    return status;
}

static xcvrStatus_t configure_A0_B0_RBME_deltas(void)
{
    xcvrStatus_t status = gXcvrSuccess_c;
    /* Perform RBME configuration per radio version */
    if ((XCVR_ReadRadioVer() == (uint16_t)XCVR_RADIO_GEN_3P5) )
    {
        /* KW37/38/39 A0 revision  *********************************************** */
        /* Nothing to do here, only included to allow error checking */
    }
    else
    {
         if (XCVR_ReadRadioVer() == (uint16_t)XCVR_RADIO_GEN_3P5_B0)
         {
             /* KW37/38/39 B0 revision *********************************************** */
             /* Configure the RBME FLUSH_CFG & DATALL_CFG registers (only exist in B0) */
             RBME->FLUSH_CFG = RBME_FLUSH_CFG_FLUSH_CFG(RF_OSC_26MHZ);
             RBME->DATALL_CFG = RBME_DATALL_CFG_DELAY_EN(RF_OSC_26MHZ);
         }
         else
         {
             status = gXcvrInvalidConfiguration_c; /* RADIO_VER value incorrect for this software */
         }
    }

    return status;
}


