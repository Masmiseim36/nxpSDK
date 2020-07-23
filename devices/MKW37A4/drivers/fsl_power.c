/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <setjmp.h>
#include "fsl_power.h"
#include "fsl_smc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.power"
#endif

#ifdef SUPPORT_WARMBOOT
static jmp_buf pwr_CPUContext;
uint32_t g_savedControl;
uint32_t scb_vtor;
uint32_t scb_icsr;
uint32_t scb_aircr;
uint32_t scb_shp[2];
uint32_t sim_scgc5;
uint32_t sim_sopt2;
uint32_t nvic_iser;
uint32_t nvic_ip[8];

/*
 * @brief WarmbootRestore.
 */
void WarmbootRestore(void)
{
    /* Interrupts should be disabled */
    __disable_irq();
    /* Restore the Control Registers */
    __set_CONTROL(g_savedControl);
    /* Restore previous context */
    longjmp(pwr_CPUContext, true);
}

/*
 * @brief POWER_CoreEnterLowpowerHook.
 */
__attribute__((weak)) void POWER_CoreEnterLowpowerHook(void)
{
    uint32_t i;

    /* Store SCB registers */
    scb_vtor   = SCB->VTOR;
    scb_icsr   = SCB->ICSR;
    scb_aircr  = SCB->AIRCR;
    scb_shp[0] = SCB->SHP[0];
    scb_shp[1] = SCB->SHP[1];
    sim_scgc5  = SIM->SCGC5;
    sim_sopt2  = SIM->SOPT2;
    nvic_iser  = NVIC->ISER[0U];

    for (i = 0; i < 8; i++)
    {
        nvic_ip[i] = NVIC->IP[i];
    }
}

/*
 * @brief POWER_CoreExitLowpowerHook.
 */
__attribute__((weak)) void POWER_CoreExitLowpowerHook(void)
{
    uint32_t i;

    /* Restore SCB registers */
    SCB->VTOR   = scb_vtor;
    SCB->ICSR   = scb_icsr;
    SCB->AIRCR  = scb_aircr;
    SCB->SHP[0] = scb_shp[0];
    SCB->SHP[1] = scb_shp[1];
    /* Restore peripheral clocks */
    SIM->SCGC5 = sim_scgc5;
    /* Restore peripheral clock source option */
    SIM->SOPT2 = sim_sopt2;

    for (i = 0; i < 8; i++)
    {
        NVIC->IP[i] = nvic_ip[i];
    }
    NVIC->ICPR[0U] = nvic_iser;
    NVIC->ISER[0U] = nvic_iser;
}
#endif

/**
 * @brief Pre enter low power mode function hook.
 *
 * This weak function allows application to call specific settings before enter low power mode
 * Please note that global interrupt is disabled when calling this function, so do not wait
 * for an interrupt in an infinite loop in this function.
 */
__attribute__((weak)) void POWER_PeripheralEnterLowpowerHook(void)
{
}

/**
 * @brief Post exit low power mode function hook.
 *
 * This weak function allows application to call specific settings after exit low power mode
 * Please note that global interrupt is disabled when calling this function, so do not wait
 * for an interrupt in an infinite loop in this function.
 */
__attribute__((weak)) void POWER_PeripheralExitLowpowerHook(void)
{
}

/*
 * @brief POWER_EnterPowerMode.
 */
void POWER_EnterPowerMode(power_mode_t powerMode)
{
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
    smc_power_mode_vlls_config_t vlls_config;
    vlls_config.enablePorDetectInVlls0 = true;
#if FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION
    vlls_config.enableRam2InVlls2 = true; /*!< Enable RAM2 power in VLLS2 */
#endif
#if FSL_FEATURE_SMC_HAS_LPOPO
    vlls_config.enableLpoClock = true; /*!< Enable LPO clock in VLLS mode */
#endif
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

#if ((defined(FSL_FEATURE_SMC_HAS_LLS_SUBMODE) && FSL_FEATURE_SMC_HAS_LLS_SUBMODE) || \
     (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO))
    smc_power_mode_lls_config_t lls_config;
#if FSL_FEATURE_SMC_HAS_LPOPO
    lls_config.enableLpoClock = true;
#endif
#if FSL_FEATURE_SMC_HAS_LLS_SUBMODE
    lls_config.subMode = kSMC_StopSub3;
#endif
#endif
    switch (powerMode)
    {
        case kPWR_PowerModeVlpr:
#if (defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI)
            SMC_SetPowerModeVlpr(SMC, false);
#else
            SMC_SetPowerModeVlpr(SMC);
#endif
            while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
            {
            }
            break;

        case kPWR_PowerModeRun:
            SMC_SetPowerModeRun(SMC);
            while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
            {
            }
            break;

#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
        case kPWR_PowerModeHsrun:
            SMC_SetPowerModeHsrun(SMC);
            while (kSMC_PowerStateHsrun != SMC_GetPowerModeState(SMC))
            {
            }
            break;
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

        case kPWR_PowerModeWait:
            SMC_PreEnterWaitModes();
            POWER_PeripheralEnterLowpowerHook();
            SMC_SetPowerModeWait(SMC);
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitWaitModes();
            break;

        case kPWR_PowerModeStop:
            SMC_PreEnterStopModes();
            POWER_PeripheralEnterLowpowerHook();
            SMC_SetPowerModeStop(SMC, kSMC_PartialStop);
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitStopModes();
            break;

        case kPWR_PowerModeVlpw:
            SMC_PreEnterWaitModes();
            POWER_PeripheralEnterLowpowerHook();
            SMC_SetPowerModeVlpw(SMC);
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitWaitModes();
            break;

        case kPWR_PowerModeVlps:
            SMC_PreEnterStopModes();
            POWER_PeripheralEnterLowpowerHook();
            SMC_SetPowerModeVlps(SMC);
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitStopModes();
            break;

#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
        case kPWR_PowerModeLls:
            SMC_PreEnterStopModes();
            POWER_PeripheralEnterLowpowerHook();
#if ((defined(FSL_FEATURE_SMC_HAS_LLS_SUBMODE) && FSL_FEATURE_SMC_HAS_LLS_SUBMODE) || \
     (defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO))
            SMC_SetPowerModeLls(SMC, &lls_config);
#else
            SMC_SetPowerModeLls(SMC);
#endif
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitStopModes();
            break;
#endif /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */

#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
        case kPWR_PowerModeVlls0:
            vlls_config.subMode = kSMC_StopSub0;
            SMC_PreEnterStopModes();
            POWER_PeripheralEnterLowpowerHook();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitStopModes();
            break;
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */

        case kPWR_PowerModeVlls1:
            vlls_config.subMode = kSMC_StopSub1;
            SMC_PreEnterStopModes();
            POWER_PeripheralEnterLowpowerHook();
            SMC_SetPowerModeVlls(SMC, &vlls_config);
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitStopModes();
            break;

#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2)
        case kPWR_PowerModeVlls2:
            vlls_config.subMode = kSMC_StopSub2;
            SMC_PreEnterStopModes();
            POWER_PeripheralEnterLowpowerHook();
#ifdef SUPPORT_WARMBOOT
            POWER_CoreEnterLowpowerHook();
            /* Save the Control Registers */
            g_savedControl = __get_CONTROL();
            if (0 == setjmp(pwr_CPUContext))
            {
                SMC_SetPowerModeVlls(SMC, &vlls_config);
            }
            POWER_CoreExitLowpowerHook();
#else
            SMC_SetPowerModeVlls(SMC, &vlls_config);
#endif
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitStopModes();
            break;
#endif /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */

        case kPWR_PowerModeVlls3:
            vlls_config.subMode = kSMC_StopSub3;
            SMC_PreEnterStopModes();
            POWER_PeripheralEnterLowpowerHook();
#ifdef SUPPORT_WARMBOOT
            POWER_CoreEnterLowpowerHook();
            /* Save the Control Registers */
            g_savedControl = __get_CONTROL();
            if (0 == setjmp(pwr_CPUContext))
            {
                SMC_SetPowerModeVlls(SMC, &vlls_config);
            }
            POWER_CoreExitLowpowerHook();
#else
            SMC_SetPowerModeVlls(SMC, &vlls_config);
#endif
            POWER_PeripheralExitLowpowerHook();
            SMC_PostExitStopModes();
            break;
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */

        default:
            assert(0);
            break;
    }
}
