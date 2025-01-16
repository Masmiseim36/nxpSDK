/*
 * Copyright 2024 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "power_demo_config.h"
#include "pmic_support.h"
#include "fsl_iopctl.h"
/*${header:end}*/

/*${macro:start}*/
#define DEMO_LOW_POWER_RUN_VOLT 700000U /* The VDD1 voltage during low power run. Used for CPU1 DS while CPU0 active.*/
/*${macro:end}*/

/*${function:start}*/
uint32_t g_runVolt = 1000000U; /* The VDD1 voltage during normal run, 1000mV, will be update per CPU clock. */

void BOARD_ClockLPPreConfig(void)
{
    CLOCK_AttachClk(kFRO1_DIV3_to_SENSE_BASE);
    CLOCK_SetClkDiv(kCLOCK_DivSenseMainClk, 1);
    CLOCK_AttachClk(kSENSE_BASE_to_SENSE_MAIN);

    BOARD_BootClockRUN_InitClockModule(kClockModule_XTAL_OSC); /* Enable SOSC used for FRO trim. */
}

void BOARD_ClockLPPostConfig(void)
{
}

void BOARD_RestorePeripheralsAfterDSR(void)
{
    BOARD_InitDebugConsole();
}

void BOARD_NotifyBoot(void)
{
    RESET_ClearPeripheralReset(kMU1_RST_SHIFT_RSTn);
    MU_Init(MU1_MUB);
    MU_SetFlags(MU1_MUB, BOOT_FLAG);
    MU_Deinit(MU1_MUB);
}

void BOARD_InitPowerConfig(void)
{
    /* Enable the used modules in sense side. */
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_GATE_FRO2);
    POWER_DisablePD(kPDRUNCFG_PD_FRO2);
    POWER_DisablePD(kPDRUNCFG_PD_LPOSC); /* Used by RTC. */

    SYSCON3->SENSE_AUTOGATE_EN = 0x3U;
    CLOCK_EnableClock(kCLOCK_Cpu1); /*Let CPU1 control it's clock. */

    /* Disable unused clock. */
    CLOCK_DisableClock(kCLOCK_Glikey1);
    CLOCK_DisableClock(kCLOCK_Glikey2);
    CLOCK_DisableClock(kCLOCK_Glikey4);
    CLOCK_DisableClock(kCLOCK_Glikey5);
    CLOCK_DisableClock(kCLOCK_SenseAccessRamArbiter0);
    CLOCK_DisableClock(kCLOCK_MediaAccessRamArbiter1);
    CLOCK_AttachClk(kNONE_to_SYSTICK);
    CLOCK_AttachClk(kNONE_to_MICFIL0);

    /* Disable unused modules. */
    POWER_EnablePD(kPDRUNCFG_SHUT_RAM0_CLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_COMNN_MAINCLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_MEDIA_MAINCLK);
    POWER_EnablePD(kPDRUNCFG_PD_SYSXTAL);
    POWER_EnablePD(kPDRUNCFG_PD_PLLANA);
    POWER_EnablePD(kPDRUNCFG_PD_PLLLDO);
    POWER_EnablePD(kPDRUNCFG_PD_AUDPLLANA);
    POWER_EnablePD(kPDRUNCFG_PD_AUDPLLLDO);
    POWER_EnablePD(kPDRUNCFG_PD_ADC0);
    POWER_EnablePD(kPDRUNCFG_SHUT_RAM1_CLK); /* Compute access RAM arbiter1 clock. */
    POWER_EnablePD(kPDRUNCFG_LP_DCDC);
    PMC1->PDRUNCFG1 = 0x7FFFFFFFU;
    PMC1->PDRUNCFG2 &= ~(0x3FFC0000U); /* Power up all the SRAM partitions in Sense domain. */
    PMC1->PDRUNCFG3 &= ~(0x3FFC0000U);
    POWER_EnablePD(kPDRUNCFG_PPD_OCOTP);
    POWER_ApplyPD();

    /* Request the domains out of sense into RBB mode. */
    POWER_EnableRunAFBB(kPower_BodyBiasVdd1);
    POWER_EnableRunNBB(kPower_BodyBiasVdd1Sram);
    POWER_EnableRunRBB(kPower_BodyBiasVdd2 | kPower_BodyBiasVddn | kPower_BodyBiasVdd2Sram);
    POWER_EnableSleepRBB(kPower_BodyBiasVdd2 | kPower_BodyBiasVddn | kPower_BodyBiasVdd2Sram | kPower_BodyBiasVdd1 |
                         kPower_BodyBiasVdd1Sram);
    POWER_ApplyPD();

    /* Set the four LDO setpoints per predefined CPU frequency, must in ascending order*/
    uint32_t freqs[4] = {0};
    freqs[0] = 0U; /* For DeepSleep. */
    freqs[1] = 64000000U; /* 64MHz */
    freqs[2] = SystemCoreClock; /* Only setpoint 2 and 0 are used. */
    freqs[3] = 250000000U;

    uint32_t miniVolts[4] = {0U}; 
    miniVolts[0] = 630000U; /* For DeepSleep, 0.63V. */

    POWER_ConfigRegulatorSetpointsForFreq(kRegulator_Vdd1LDO, freqs, miniVolts, 0U, 4U);

    g_runVolt = POWER_CalcVoltLevel(kRegulator_Vdd1LDO, SystemCoreClock, 0U); /* Calculate the voltage per frequency. */

#if defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_MIXED)
    POWER_SelectRunSetpoint(kRegulator_Vdd1LDO, 2U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd1LDO, 0U);
    POWER_SelectRunSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_ApplyPD();
#elif defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    POWER_DisableLPRequestMask(kPower_MaskLpi2c15);
    BOARD_InitPmic();
    /* Select the lowest LVD setpoint. */
    POWER_SelectRunSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectRunSetpoint(kRegulator_Vdd1LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd1LDO, 0U);
    POWER_ApplyPD();

    BOARD_SetPmicVdd1Voltage(g_runVolt);
#endif

#if (DEMO_POWER_ENABLE_DEBUG == 0U)
    CLOCK_DisableClock(kCLOCK_Dbg);
#endif
}

/* Set IO pads to default. */
void BOARD_DisableIoPads(void)
{
    uint8_t port, pin;

    RESET_ClearPeripheralReset(kIOPCTL1_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Iopctl1);

    port = 8;
    pin  = 5U; /* Keep JTAG pin unchanged. */

    for (; pin <= 31U; pin++)
    {
        IOPCTL_PinMuxSet(port, pin, 0U);
    }

    port = 9U;
    for (pin = 0U; pin <= 2U; pin++)
    {
        IOPCTL_PinMuxSet(port, pin, 0U);
    }

    port = 10U;
    for (pin = 0U; pin <= 17U; pin++)
    {
        IOPCTL_PinMuxSet(port, pin, 0U);
    }
}

void BOARD_InitHardware(void)
{
    BOARD_DisableIoPads();
    POWER_DisablePD(kPDRUNCFG_PD_FRO2); /* Sense uses FRO2. */
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    BOARD_InitPowerConfig();

    BOARD_NotifyBoot(); /* Set boot flag. */
}

void BOARD_RunActiveTest(void)
{
    DEMO_LOG("\r\nThis test mode will keep CPU in run mode but close all other unused modules for a while.\n");
    DEMO_LOG("\r\nPlease don't input any charator until the mode finished.\n");

    DbgConsole_Deinit();

    /* Disable clocks - CLKCTL1(Sense private) */
    CLOCK_DisableClock(kCLOCK_Syscon1); /* CLKCTL1->PSCCTL0 */
    CLKCTL1->PSCCTL1 = 0U; /* Disable clock for INPUTMUX, WWDT2-3, MU3, SEMA42_3, UTICK1, MRT1, CTIMER5-7, PINT, GPIO,
                              FLEXCOMM, eDMA, HiFI1, SenseAccessRAM0. */

    /* Disable clocks - CLKCTL3(Sense shared) */
    CLOCK_DisableClock(kCLOCK_Mu1);
    CLOCK_DisableClock(kCLOCK_Iopctl1);
    CLOCK_DisableClock(kCLOCK_Syscon3);
    CLOCK_DisableClock(kCLOCK_Sema420);
    CLOCK_DisableClock(kCLOCK_LPI2c15);
    CLOCK_DisableClock(kCLOCK_Rtc);

    /* Disable clock slice. */
    CLOCK_AttachClk(kNONE_to_LPI2C15);
    CLOCK_AttachClk(kNONE_to_FLEXCOMM19);

    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv1OutEn);

    POWER_EnablePD(kPDRUNCFG_APD_OCOTP); /* ERR052483 */
    POWER_ApplyPD();
    /* Note, the debug will not work anymore when the sense shared mainclk is disabled. */
    POWER_EnablePD(kPDRUNCFG_PD_LPOSC);
    POWER_EnablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);
    CLOCK_DisableClock(kCLOCK_Sleepcon1);

    /* Simulate a task. */
    uint32_t i;
    for (i = 0U; i < 500U; i++)
    {
        SDK_DelayAtLeastUs(10000U, CLOCK_GetCoreSysClkFreq());
    }

    CLOCK_EnableClock(kCLOCK_Sleepcon1);
    CLOCK_EnableClock(kCLOCK_Syscon1); /* CLKCTL1->PSCCTL0 */
    POWER_DisablePD(kPDRUNCFG_PD_LPOSC);
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);
    CLOCK_EnableClock(kCLOCK_Iopctl1); /* In CLKCTL3, requires sense shared clock. */
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv1OutEn | kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn);
    POWER_DisablePD(kPDRUNCFG_APD_OCOTP);
    POWER_ApplyPD();

    CLOCK_AttachClk(kSENSE_BASE_to_LPI2C15);
    CLOCK_AttachClk(kSENSE_BASE_to_FLEXCOMM19);

    /* Enable clocks - CLKCTL3(Sense shared) */
    CLOCK_EnableClock(kCLOCK_Mu1);
    CLOCK_EnableClock(kCLOCK_Syscon3);
    CLOCK_EnableClock(kCLOCK_Sema420);
    CLOCK_EnableClock(kCLOCK_LPI2c15);

    BOARD_InitDebugConsole();
}

void BOARD_EnterSleep(void)
{
    uint32_t irqMask;
    /* Disable clock for unused modules. */
    DbgConsole_Deinit();
    CLOCK_DisableClock(kCLOCK_Syscon1);
    CLOCK_DisableClock(kCLOCK_Iopctl1);
    CLOCK_DisableClock(kCLOCK_Mu1);
    CLOCK_DisableClock(kCLOCK_Syscon3);
    CLOCK_DisableClock(kCLOCK_Sema420);
    CLOCK_DisableClock(kCLOCK_LPI2c15);
    CLOCK_AttachClk(kLPOSC_to_SENSE_BASE); /* To disable FRO2 DIV3, switch sense base to LPOSC. */
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv1OutEn);

    irqMask = DisableGlobalIRQ();
    POWER_EnablePD(kPDRUNCFG_PD_LPOSC);
    POWER_EnablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);

    POWER_EnterSleep();

    POWER_DisablePD(kPDRUNCFG_PD_LPOSC);
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);

    EnableGlobalIRQ(irqMask);
    __ISB();

    /* Re-enable clock for modules. */
    CLOCK_EnableClock(kCLOCK_Syscon1);
    CLOCK_EnableClock(kCLOCK_Iopctl1);
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv1OutEn | kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn);
    CLOCK_AttachClk(kFRO2_DIV3_to_SENSE_BASE);
    CLOCK_EnableClock(kCLOCK_Mu1);
    CLOCK_EnableClock(kCLOCK_Syscon3);
    CLOCK_EnableClock(kCLOCK_Sema420);
    CLOCK_EnableClock(kCLOCK_LPI2c15);
    BOARD_InitDebugConsole();
}

/*! @brief Decrease the CPU frequency and supply voltage for lower power consumption.
 */
static inline void BOARD_PrepareForDS(void)
{
    /* Change to a lower frequency to safely decrease the VDD1 voltage when CPU1 enter low power mode but CPU0 is active
     * and still requires sense shared main clock. */
    CLOCK_AttachClk(kLPOSC_to_SENSE_BASE);
    CLOCK_AttachClk(kSENSE_BASE_to_SENSE_MAIN);
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv6OutEn); /* Need Keep DIV6. */
#if defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    BOARD_SetPmicVdd1Voltage(DEMO_LOW_POWER_RUN_VOLT);
#endif
#if (defined(BOARD_PMIC_CONFIG_USE_SEMA4) && (BOARD_PMIC_CONFIG_USE_SEMA4 != 0U))
    CLOCK_DisableClock(kCLOCK_Sema420);
#endif
    CLOCK_DisableClock(kCLOCK_LPI2c15);
}

/*! @brief Decrease the CPU frequency and supply voltage for lower power consumption.
 */
static inline void BOARD_RestoreAfterDS(void)
{
#if (defined(BOARD_PMIC_CONFIG_USE_SEMA4) && (BOARD_PMIC_CONFIG_USE_SEMA4 != 0U))
    CLOCK_EnableClock(kCLOCK_Sema420);
#endif
    CLOCK_EnableClock(kCLOCK_LPI2c15);
#if defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    BOARD_SetPmicVdd1Voltage(g_runVolt);
#endif
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv1OutEn | kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn);
    CLOCK_AttachClk(kFRO2_DIV1_to_SENSE_MAIN);
    CLOCK_AttachClk(kFRO2_DIV3_to_SENSE_BASE);
}

void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7])
{
    BOARD_PrepareForDS();
    POWER_EnterDeepSleep(exclude_from_pd);
    BOARD_RestoreAfterDS();
}

void BOARD_RequestDSR(const uint32_t exclude_from_pd[7])
{
    BOARD_PrepareForDS();
    POWER_RequestDSR(exclude_from_pd);
    BOARD_RestoreAfterDS();
}

void BOARD_RequestDPD(const uint32_t exclude_from_pd[7])
{
    /* Need keep sense shared main clock in case CPU0 enters power down mode after CPU1. */
    CLOCK_AttachClk(kLPOSC_to_SENSE_BASE);
    CLOCK_AttachClk(kSENSE_BASE_to_SENSE_MAIN);
    POWER_RequestDeepPowerDown(exclude_from_pd);
}

void BOARD_RequestFDPD(const uint32_t exclude_from_pd[7])
{
    CLOCK_AttachClk(kLPOSC_to_SENSE_BASE);
    CLOCK_AttachClk(kSENSE_BASE_to_SENSE_MAIN);
    POWER_RequestFullDeepPowerDown(exclude_from_pd);
}
/*${function:end}*/
