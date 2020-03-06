/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "board.h"
#include "pmic_support.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PMIC_DECREASE_LVD_LEVEL_IF_HIGHER_THAN(currVolt, targetVolt) \
    do                                                               \
    {                                                                \
        if ((uint32_t)(currVolt) > (uint32_t)(targetVolt))           \
        {                                                            \
            POWER_SetLvdFallingTripVoltage(kLvdFallingTripVol_720);  \
        }                                                            \
    } while (0)

/*******************************************************************************
 * Variables
 ******************************************************************************/
pca9420_handle_t pca9420Handle;
static pca9420_modecfg_t pca9420CurrModeCfg;
static pca9420_mode_t pca9420CurrMode;
static const pca9420_sw1_out_t pca9420VoltLevel[5] = {
    kPCA9420_Sw1OutVolt1V150, kPCA9420_Sw1OutVolt1V000, kPCA9420_Sw1OutVolt0V900,
    kPCA9420_Sw1OutVolt0V800, kPCA9420_Sw1OutVolt0V700,
};

/* Frequency levels defined in power library. */
extern const uint32_t powerMainFreqLevel[2][4];
extern const uint32_t powerDspFreqLevel[2][4];
/* Functions defined in power library. */
extern void POWER_DisableLVD(void);
extern void POWER_RestoreLVD(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
static pca9420_sw1_out_t BOARD_CalcVoltLevel(const uint32_t *freqLevels, uint32_t freq)
{
    uint32_t i;

    for (i = 0; i < 4; i++)
    {
        if (freq > freqLevels[i])
        {
            break;
        }
    }

    return pca9420VoltLevel[i];
}

void BOARD_InitPmic(void)
{
    pca9420_config_t pca9420Config;

    CLOCK_AttachClk(kSFRO_to_FLEXCOMM15);
    BOARD_PMIC_I2C_Init();
    PCA9420_GetDefaultConfig(&pca9420Config);
    pca9420Config.I2C_SendFunc    = BOARD_PMIC_I2C_Send;
    pca9420Config.I2C_ReceiveFunc = BOARD_PMIC_I2C_Receive;
    pca9420Config.powerGoodEnable = kPCA9420_PGoodDisabled;
    PCA9420_Init(&pca9420Handle, &pca9420Config);
}

void BOARD_SetPmicVoltageForFreq(power_part_temp_range temp_range, uint32_t main_clk_freq, uint32_t dsp_main_clk_freq)
{
    power_lvd_falling_trip_vol_val_t lvdVolt;
    uint32_t idx = (uint32_t)temp_range;
    pca9420_sw1_out_t mainVolt, dspVolt, volt;

    PCA9420_GetCurrentMode(&pca9420Handle, &pca9420CurrMode);
    PCA9420_ReadModeConfigs(&pca9420Handle, pca9420CurrMode, &pca9420CurrModeCfg, 1);

    lvdVolt = POWER_GetLvdFallingTripVoltage();

    /* Enter FBB mode first */
    if (POWER_GetBodyBiasMode(kCfg_Run) != kPmu_Fbb)
    {
        POWER_EnterFbb();
    }

    mainVolt = BOARD_CalcVoltLevel(&powerMainFreqLevel[idx][0], main_clk_freq);
    dspVolt  = BOARD_CalcVoltLevel(&powerDspFreqLevel[idx][0], dsp_main_clk_freq);
    volt     = MAX(mainVolt, dspVolt);

    if (volt < kPCA9420_Sw1OutVolt0V800)
    {
        POWER_DisableLVD();
    }
    else
    {
        if (volt < kPCA9420_Sw1OutVolt0V900)
        {
            PMIC_DECREASE_LVD_LEVEL_IF_HIGHER_THAN(lvdVolt, kLvdFallingTripVol_795);
        }
        else if (volt < kPCA9420_Sw1OutVolt1V000)
        {
            PMIC_DECREASE_LVD_LEVEL_IF_HIGHER_THAN(lvdVolt, kLvdFallingTripVol_885);
        }
    }

    /* Configure vddcore voltage value */
    pca9420CurrModeCfg.sw1OutVolt = volt;
    PCA9420_WriteModeConfigs(&pca9420Handle, pca9420CurrMode, &pca9420CurrModeCfg, 1);

    if (volt >= kPCA9420_Sw1OutVolt0V800)
    {
        POWER_RestoreLVD();
    }
}
