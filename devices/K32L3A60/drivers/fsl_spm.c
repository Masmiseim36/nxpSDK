/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spm.h"
#include "math.h" /* Using floor() function to convert float variable to int. */

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.spm"
#endif

/*!
 * brief Gets the regulators Status.
 *
 * param base SPM peripheral base address.
 * param info Pointer to status structure, see to #spm_regulator_status_t.
 */
void SPM_GetRegulatorStatus(SPM_Type *base, spm_regulator_status_t *info)
{
    assert(NULL != info);

    volatile uint32_t tmp32 = base->RSR; /* volatile here is to make sure this value is actually from the hardware. */

    info->mcuLowPowerModeStatus =
        (spm_mcu_low_power_mode_status_t)((tmp32 & SPM_RSR_MCUPMSTAT_MASK) >> SPM_RSR_MCUPMSTAT_SHIFT);
    info->isDcdcLdoOn =
        (0x4 == (0x4 & ((tmp32 & SPM_RSR_REGSEL_MASK) >> SPM_RSR_REGSEL_SHIFT))); /* 1<<2 responses DCDC LDO. */
    info->isAuxLdoOn =
        (0x2 == (0x2 & ((tmp32 & SPM_RSR_REGSEL_MASK) >> SPM_RSR_REGSEL_SHIFT))); /* 1<<1 responses AUX LDO. */
    info->isCoreLdoOn =
        (0x1 == (0x1 & ((tmp32 & SPM_RSR_REGSEL_MASK) >> SPM_RSR_REGSEL_SHIFT))); /* 1<<0 responses CORE LDO. */
}

/*!
 * brief Configures the low-voltage detect setting.
 *
 * This function configures the low-voltage detect setting, including the trip
 * point voltage setting, enables or disables the interrupt, enables or disables the system reset.
 *
 * param base SPM peripheral base address.
 * param config Pointer to low-voltage detect configuration structure, see to #spm_low_volt_detect_config_t.
 */
void SPM_SetLowVoltDetectConfig(SPM_Type *base, const spm_low_volt_detect_config_t *config)
{
    uint32_t tmp32 = base->LVDSC1 & ~(SPM_LVDSC1_VDD_LVDIE_MASK | SPM_LVDSC1_VDD_LVDRE_MASK | SPM_LVDSC1_VDD_LVDV_MASK |
                                      SPM_LVDSC1_COREVDD_LVDIE_MASK | SPM_LVDSC1_COREVDD_LVDRE_MASK);

    /* VDD voltage detection. */
    tmp32 |= SPM_LVDSC1_VDD_LVDV(config->vddLowVoltDetectSelect);
    if (true == config->enableIntOnVddLowVolt)
    {
        tmp32 |= SPM_LVDSC1_VDD_LVDIE_MASK;
    }
    if (true == config->enableResetOnVddLowVolt)
    {
        tmp32 |= SPM_LVDSC1_VDD_LVDRE_MASK;
    }
    /* Clear the Low Voltage Detect Flag with previous power detect setting. */
    tmp32 |= SPM_LVDSC1_VDD_LVDACK_MASK;

    /* COREVDD voltage detection. */
    if (true == config->enableIntOnCoreLowVolt)
    {
        tmp32 |= SPM_LVDSC1_COREVDD_LVDIE_MASK;
    }
    if (true == config->enableResetOnCoreLowVolt)
    {
        tmp32 |= SPM_LVDSC1_COREVDD_LVDRE_MASK;
    }
    tmp32 |= SPM_LVDSC1_COREVDD_LVDACK_MASK; /* Clear previous error flag. */

    base->LVDSC1 = tmp32;
}

/*!
 * brief Configures the low-voltage warning setting.
 *
 * This function configures the low-voltage warning setting, including the trip
 * point voltage setting and enabling or disabling the interrupt.
 *
 * param base SPM peripheral base address.
 * param config Pointer to Low-voltage warning configuration structure, see to #spm_low_volt_warning_config_t.
 */
void SPM_SetLowVoltWarningConfig(SPM_Type *base, const spm_low_volt_warning_config_t *config)
{
    uint32_t tmp32 = base->LVDSC2 & ~(SPM_LVDSC2_VDD_LVWV_MASK | SPM_LVDSC2_VDD_LVWIE_MASK);

    tmp32 |= SPM_LVDSC2_VDD_LVWV(config->vddLowVoltDetectSelect);
    if (true == config->enableIntOnVddLowVolt)
    {
        tmp32 |= SPM_LVDSC2_VDD_LVWIE_MASK;
    }
    tmp32 |= SPM_LVDSC2_VDD_LVWACK_MASK; /* Clear previous error flag. */

    base->LVDSC2 = tmp32;
}

/*!
 * brief Configures the high-voltage detect setting.
 *
 * This function configures the high-voltage detect setting, including the trip
 * point voltage setting, enabling or disabling the interrupt, enabling or disabling the system reset.
 *
 * param base SPM peripheral base address.
 * param config  High-voltage detect configuration structure, see to #spm_high_volt_detect_config_t.
 */
void SPM_SetHighVoltDetectConfig(SPM_Type *base, const spm_high_volt_detect_config_t *config)
{
    uint32_t tmp32;

    tmp32 = base->HVDSC1 & ~(SPM_HVDSC1_VDD_HVDIE_MASK | SPM_HVDSC1_VDD_HVDRE_MASK | SPM_HVDSC1_VDD_HVDV_MASK);
    tmp32 |= SPM_HVDSC1_VDD_HVDV(config->vddHighVoltDetectSelect);
    if (true == config->enableIntOnVddHighVolt)
    {
        tmp32 |= SPM_HVDSC1_VDD_HVDIE_MASK;
    }
    if (true == config->enableResetOnVddHighVolt)
    {
        tmp32 |= SPM_HVDSC1_VDD_HVDRE_MASK;
    }
    tmp32 |= SPM_HVDSC1_VDD_HVDACK_MASK; /* Clear previous error flag. */

    base->HVDSC1 = tmp32;
}

/*!
 * brief Configures the Aux LDO.
 *
 * param base SPM peripheral base address.
 * param config Pointer to configuration structure, see to #spm_aux_ldo_config_t.
 */
void SPM_SetAuxLdoConfig(SPM_Type *base, const spm_aux_ldo_config_t *config)
{
    uint32_t tmp32 = 0U;

    switch (config->lowPowerMode)
    {
        case kSPM_AuxLdoRemainInHighPowerInLowPowerModes:
            tmp32 |= SPM_AUXLDOLPCNFG_LPSEL_MASK;
            break;
        default: /* kSPM_RfLdoEnterLowPowerInLowPowerModes. */
            assert(false);
            break;
    }
    base->AUXLDOLPCNFG = tmp32;

    tmp32 = SPM_AUXLDOSC_IOSSSEL(config->ioSoftStartDuration) | SPM_AUXLDOSC_AUXREGVSEL(config->ioRegulatorVolt);
    base->AUXLDOSC = tmp32;
}

/*!
 * brief Sets DCDC battery monitor with its ADC value.
 *
 * For better accuracy, software would call this function to set the battery voltage value into DCDC
 * measured by ADC.
 *
 * param base SPM peripheral base address.
 * param batAdcVal ADC measured battery value with an 8mV LSB resolution.
 *              Value 0 would disable the battery monitor.
 */
void SPM_SetDcdcBattMonitor(SPM_Type *base, uint32_t batAdcVal)
{
    /* Clear the value and disable it at first. */
    base->DCDCC2 &= ~(SPM_DCDCC2_DCDC_BATTMONITOR_BATT_VAL_MASK | SPM_DCDCC2_DCDC_BATTMONITOR_EN_BATADJ_MASK);
    if (0U != batAdcVal)
    {
        /* When setting the value to BATT_VAL field, it should be zero before. */
        base->DCDCC2 |= SPM_DCDCC2_DCDC_BATTMONITOR_BATT_VAL(batAdcVal);
        base->DCDCC2 |= SPM_DCDCC2_DCDC_BATTMONITOR_EN_BATADJ_MASK;
    }
}

/*!
 * brief Bypasses the ADC measure value
 *
 * Forces DCDC to bypass the adc measuring state and loads the user-defined value in this function.
 *
 * param base SPM peripheral base address.
 * param enable Enable the bypass or not.
 * param value User-setting value to be available instead of ADC measured value.
 */
void SPM_BypassDcdcBattMonitor(SPM_Type *base, bool enable, uint32_t value)
{
    if (true == enable)
    {
        /* Set the user-defined value before enable the bypass. */
        base->DCDCC3 = (base->DCDCC3 & ~SPM_DCDCC3_DCDC_VBAT_VALUE_MASK) | SPM_DCDCC3_DCDC_VBAT_VALUE(value);
        /* Enable the bypass and load the user-defined value. */
        base->DCDCC3 |= SPM_DCDCC3_DCDC_BYPASS_ADC_MEAS_MASK;
    }
    else
    {
        base->DCDCC3 &= ~SPM_DCDCC3_DCDC_BYPASS_ADC_MEAS_MASK;
    }
}

/*!
 * brief Configure the DCDC integrator value.
 *
 * Integrator value can be loaded in pulsed mode. Software can program this value according to
 * battery voltage and VDD_CORE output target value before goes to the pulsed mode.
 *
 code
    spm_dcdc_integrator_config_t SpmDcdcIntegratorConfigStruct =
    {
        .vddCoreValue = 1.25f,
        .vBatValue    = 3.34f
    };
 endcode
 *
 * param base SPM peripheral base address.
 * param config Pointer to configuration structure, see to #spm_dcdc_integrator_config_t.
 *               Passing NULL would clear all user-defined setting and use hardware default setting.
 */
void SPM_SetDcdcIntegratorConfig(SPM_Type *base, const spm_dcdc_integrator_config_t *config)
{
    int32_t tmp32;

    assert(NULL != config);

    tmp32 = (int32_t)(config->vddCoreValue / config->vBatValue * 32 - 16) *
            8192; /* Target value = ((VDD_CORE/Vbat)*32 - 16) * 2^13 */
    base->DCDCC4 = SPM_DCDCC4_PULSE_RUN_SPEEDUP_MASK | SPM_DCDCC4_INTEGRATOR_VALUE_SELECT_MASK |
                   SPM_DCDCC4_INTEGRATOR_VALUE(tmp32);
}
