/*
 * Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * File:        smc.c
 * Purpose:     Provides routines for entering low power modes.
 *
 * Notes:    Since the wakeup mechanism for low power modes
 *              will be application specific, these routines
 *              do not include code to setup interrupts to exit
 *              from the low power modes. The desired means of
 *              low power mode exit should be configured before
 *              calling any of these functions.
 *
 *              These routines do not include protection to
 *              prevent illegal state transitions in the mode
 *              controller, and all routines that write to the
 *              PMPROT register write a value to allow all
 *              possible low power modes (it is write once, so
 *              if only the currently requested mode is enabled
 *              a different mode couldn't be enabled later on).
 *
 */

#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "smc.h"

#if defined(K32W042S1M2_M4_SERIES)
// fixme SMC0 or SCM1?
#define SMC SMC0
#endif // defined(K32W042S1M2_M4_SERIES)

/***************************************************************/
/*
 * Configures the ARM system control register for WAIT(sleep)mode
 * and then executes the WFI instruction to enter the mode.
 *
 * Parameters:
 * none
 *
 */

void sleep(void)
{
    /* Clear the SLEEPDEEP bit to make sure we go into WAIT (sleep)
     * mode instead of deep sleep.
     */
    SCB->SCR &= (uint32_t)~SCB_SCR_SLEEPDEEP_Msk;

    __WFI();
}
/***************************************************************/
/*
 * Configures the ARM system control register for STOP
 * (deepsleep) mode and then executes the WFI instruction
 * to enter the mode.
 *
 * Parameters:
 * none
 *
 */

void deepsleep(void)
{
    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= (uint32_t)SCB_SCR_SLEEPDEEP_Msk;

    __WFI();
}
/********************************************************************/
/* WAIT mode entry routine. Puts the processor into wait mode.
 * In this mode the core clock is disabled (no code executing), but
 * bus clocks are enabled (peripheral modules are operational).
 *
 * Mode transitions:
 * RUN -> WAIT
 * VLPR -> VLPW
 *
 * This function can be used to enter normal wait mode or VLPW
 * mode. If you are executing in normal run mode when calling this
 * function, then you will enter normal wait mode. If you are in VLPR
 * mode when calling this function, then you will enter VLPW mode instead.
 *
 * NOTE: Some modules include a programmable option to disable them in
 * wait mode. If those modules are programmed to disable in wait mode,
 * they will not be able to generate interrupts to wake up the core.
 *
 * WAIT mode is exited using any enabled interrupt or RESET, so no
 * exit_wait routine is needed.
 *
 *
 * Parameters:
 * none
 */
void enter_wait(void)
{
    /* Clear the SLEEPDEEP bit to make sure we go into WAIT (sleep) mode instead
     * of deep sleep.
     */
    SCB->SCR &= (uint32_t)~SCB_SCR_SLEEPDEEP_Msk;

    __WFI();
}
/********************************************************************/
/* STOP mode entry routine. Puts the processor into normal stop mode.
 * In this mode core, bus and peripheral clocks are disabled.
 *
 * Mode transitions:
 * RUN -> STOP
 *
 * This function can be used to enter normal stop mode.
 * If you are executing in normal run mode when calling this
 * function and AVLP = 0, then you will enter normal stop mode.
 * If AVLP = 1 with previous write to PMPROT
 * then you will enter VLPS mode instead.
 *
 * STOP mode is exited using any enabled interrupt or RESET, so no
 * exit_stop routine is needed.
 *
 * Parameters:
 * Partial Stop Option:
 *  0x00 = STOP - Normal Stop Mode
 *  0x40 = PSTOP1 - Partial Stop with both system and bus clocks disabled
 *  0x80 = PSTOP2 - Partial Stop with system clock disabled and bus clock enabled
 *  0xC0 = Reserved
 */
void enter_stop(uint8_t partial_stop_opt)
{
    /* The PMPROT register may have already been written by init code
       If so then this next write is not done since
       PMPROT is write once after RESET
       this write-once bit allows the MCU to enter the
       normal STOP mode.
       If AVLP is already a 1, VLPS mode is entered instead of normal STOP*/
    SMC->PMPROT = 0u;

    /* Set the STOPM field to 0b000 for normal STOP mode */
    SMC->PMCTRL &= (uint8_t)~SMC_PMCTRL_STOPM_MASK;
    SMC->PMCTRL |= (uint8_t)SMC_PMCTRL_STOPM(0u);

#if defined(FSL_FEATURE_SMC_HAS_PSTOPO) && FSL_FEATURE_SMC_HAS_PSTOPO
    SMC->STOPCTRL &= (uint8_t)~SMC_STOPCTRL_PSTOPO_MASK;
    SMC->STOPCTRL |= (uint8_t)partial_stop_opt;
#endif
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->PMCTRL;

    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= (uint32_t)SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}

/********************************************************************/
/* VLPR mode exit routine. Puts the processor into normal run mode
 * from VLPR mode. You can transition from VLPR to normal run using
 * this function.
 *
 * Mode transitions:
 * VLPR -> RUN
 *
 * Parameters:
 * none
 */
/********************************************************************/

void exit_vlpr(void)
{
    uint32_t i;
    /* check to make sure in VLPR before exiting    */
    if ((SMC->PMSTAT & (uint8_t)SMC_PMSTAT_PMSTAT_MASK) == 4u)
    {
        /* Clear RUNM */
        SMC->PMCTRL &= (uint8_t)~SMC_PMCTRL_RUNM_MASK;

        /* Wait for normal RUN regulation mode to be confirmed */
        // 1 PMSTAT MCU is in RUN  mode
        // 4 PMSTAT MCU is in VLPR mode
        for (i = 0u; i < 0xffu; i++)
        {
            if ((SMC->PMSTAT & (uint8_t)SMC_PMSTAT_PMSTAT_MASK) == 1u)
            {
#ifdef PMC_REGSC_REGONS_MASK
                if ((PMC->REGSC & PMC_REGSC_REGONS_MASK) != 0u)
#elif defined(PMC_REGSC_REGFPM_MASK)
                if ((PMC->REGSC & PMC_REGSC_REGFPM_MASK) != 0u)
#endif // PMC_REGSC_REGONS_MASK
                {
                    break;
                }
            }
        }
    } // if in VLPR mode
    // else if not in VLPR ignore call
}
/***************************************************************/
/* VLPS mode entry routine. Puts the processor into VLPS mode
 * directly from run or VLPR modes.
 *
 * Mode transitions:
 * RUN  -> VLPS
 * VLPR -> VLPS
 *
 * Note, when VLPS is entered directly from RUN or VLPR mode,
 * exit to VLPR is disabled by hardware and the system will
 * always exit back to RUN.
 *
 * Parameters:
 * none
 */
/****************************************************************/

void enter_vlps(void)
{
    /* The PMPROT register may have already been written by init code
       If so then this next write is not done since
       PMPROT is write once after RESET
       allows the MCU to enter the VLPR, VLPW, and VLPS modes.
       If AVLP is already writen to 0
       Stop is entered instead of VLPS*/
    SMC->PMPROT = (uint8_t)SMC_PMPROT_AVLP_MASK;
    /* Set the STOPM field to 0b010 for VLPS mode */
    SMC->PMCTRL &= (uint8_t)~SMC_PMCTRL_STOPM_MASK;
    SMC->PMCTRL |= (uint8_t)SMC_PMCTRL_STOPM(0x2u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->PMCTRL;
    /* Now execute the stop instruction to go into VLPS */

    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= (uint8_t)SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}
/****************************************************************/
/* LLS mode entry routine. Puts the processor into LLS mode from
 * normal run mode or VLPR.
 *
 * Mode transitions:
 * RUN -> LLS
 * VLPR -> LLS
 *
 * NOTE: LLS mode will always exit to RUN mode even if you were
 * in VLPR mode before entering LLS.
 *
 * Wakeup from LLS mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in LLS mode, so make
 * sure to setup the desired wakeup sources in the LLWU before
 * calling this function.
 *
 * Parameters:
 * none
 */
/********************************************************************/
#if !defined(CPU_PKE18F512VLH15)
void enter_lls(void)
{
    /* Write to PMPROT to allow LLS power modes this write-once
       bit allows the MCU to enter the LLS low power mode*/
    SMC->PMPROT = (uint8_t)SMC_PMPROT_AVLLS_MASK;
    /* Set the STOPM field to 0b011 for LLS mode  */
    SMC->PMCTRL &= (uint8_t)~SMC_PMCTRL_STOPM_MASK;
    SMC->PMCTRL |= (uint8_t)SMC_PMCTRL_STOPM(0x3u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->PMCTRL;
    /* Now execute the stop instruction to go into LLS */

    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= (uint8_t)SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}
/***************************************************************/
/* VLLS3 mode entry routine. Puts the processor into
 * VLLS3 mode from normal run mode or VLPR.
 *
 * Mode transitions:
 * RUN -> VLLS3
 * VLPR -> VLLS3
 *
 * NOTE: VLLSx modes will always exit to RUN mode even if you were
 * in VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before
 * calling this function.
 *
 * Parameters:
 * none
 */
/********************************************************************/

void enter_vlls3(void)
{
    /* Write to PMPROT to allow VLLS3 power modes */
    SMC->PMPROT = (uint8_t)SMC_PMPROT_AVLLS_MASK;
    /* Set the STOPM field to 0b100 for VLLS3 mode */
    SMC->PMCTRL &= (uint8_t)~SMC_PMCTRL_STOPM_MASK;
    SMC->PMCTRL |= (uint8_t)SMC_PMCTRL_STOPM(0x4u);

#if defined(FSL_FEATURE_SMC_USE_VLLSCTRL_REG) && FSL_FEATURE_SMC_USE_VLLSCTRL_REG
    /* set VLLSM = 0b11 */
    SMC->VLLSCTRL = (uint8_t)SMC_VLLSCTRL_VLLSM(3u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->VLLSCTRL;
#elif defined(FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM) && FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM
    /* set VLLSM = 0b11 */
    SMC->STOPCTRL = (uint8_t)SMC_STOPCTRL_VLLSM(3u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->STOPCTRL;
#elif defined(FSL_FEATURE_SMC_HAS_SUB_STOP_MODE) && FSL_FEATURE_SMC_HAS_SUB_STOP_MODE
    /* set LLSM = 0b11 */
    SMC->STOPCTRL = (uint8_t)SMC_STOPCTRL_LLSM(3u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->STOPCTRL;
#endif
    /* Now execute the stop instruction to go into VLLS3 */

    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= (uint32_t)SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}
/***************************************************************/
/* VLLS2 mode entry routine. Puts the processor into
 * VLLS2 mode from normal run mode or VLPR.
 *
 * Mode transitions:
 * RUN -> VLLS2
 * VLPR -> VLLS2
 *
 * NOTE: VLLSx modes will always exit to RUN mode even
 *       if you werein VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before
 * calling this function.
 *
 * Parameters:
 * none
 */
/********************************************************************/

void enter_vlls2(void)
{
    /* Write to PMPROT to allow VLLS2 power modes */
    SMC->PMPROT = (uint8_t)SMC_PMPROT_AVLLS_MASK;
    /* Set the STOPM field to 0b100 for VLLS2 mode */
    SMC->PMCTRL &= (uint8_t)~SMC_PMCTRL_STOPM_MASK;
    SMC->PMCTRL |= (uint8_t)SMC_PMCTRL_STOPM(0x4u);

#if defined(FSL_FEATURE_SMC_USE_VLLSCTRL_REG) && FSL_FEATURE_SMC_USE_VLLSCTRL_REG
    /* set VLLSM = 0b10 */
    SMC->VLLSCTRL = (uint8_t)SMC_VLLSCTRL_VLLSM(2u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->VLLSCTRL;
#elif defined(FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM) && FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM
    /* set VLLSM = 0b10 */
    SMC->STOPCTRL = (uint8_t)SMC_STOPCTRL_VLLSM(2u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->STOPCTRL;
#elif defined(FSL_FEATURE_SMC_HAS_SUB_STOP_MODE) && FSL_FEATURE_SMC_HAS_SUB_STOP_MODE
    /* set LLSM = 0b10 */
    SMC->STOPCTRL = (uint8_t)SMC_STOPCTRL_LLSM(2u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->STOPCTRL;
#endif
    /* Now execute the stop instruction to go into VLLS2 */

    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= (uint32_t)SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}
/***************************************************************/
/* VLLS1 mode entry routine. Puts the processor into
 * VLLS1 mode from normal run mode or VLPR.
 *
 * Mode transitions:
 * RUN -> VLLS1
 * VLPR -> VLLS1
 *
 * NOTE: VLLSx modes will always exit to RUN mode even if you were
 * in VLPR mode before entering VLLSx.
 *
 * Wakeup from VLLSx mode is controlled by the LLWU module. Most
 * modules cannot issue a wakeup interrupt in VLLSx mode, so make
 * sure to setup the desired wakeup sources in the LLWU before
 * calling this function.
 *
 * Parameters:
 * none
 */
/********************************************************************/

void enter_vlls1(void)
{
    SMC->PMPROT = (uint8_t)SMC_PMPROT_AVLLS_MASK;

    /* Write to PMPROT to allow all possible power modes */
    /* Set the STOPM field to 0b100 for VLLS1 mode */
    SMC->PMCTRL &= (uint8_t)~SMC_PMCTRL_STOPM_MASK;
    SMC->PMCTRL |= (uint8_t)SMC_PMCTRL_STOPM(0x4u);

#if defined(FSL_FEATURE_SMC_USE_VLLSCTRL_REG) && FSL_FEATURE_SMC_USE_VLLSCTRL_REG
    /* set VLLSM = 0b01 */
    SMC->VLLSCTRL = (uint8_t)SMC_VLLSCTRL_VLLSM(1u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->VLLSCTRL;
#elif defined(FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM) && FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM
    /* set VLLSM = 0b01 */
    SMC->STOPCTRL = (uint8_t)SMC_STOPCTRL_VLLSM(1u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->STOPCTRL;
#elif defined(FSL_FEATURE_SMC_HAS_SUB_STOP_MODE) && FSL_FEATURE_SMC_HAS_SUB_STOP_MODE
    /* set LLSM = 0b01 */
    SMC->STOPCTRL = (uint8_t)SMC_STOPCTRL_LLSM(1u);
    /*wait for write to complete to SMC before stopping core */
    (void)SMC->STOPCTRL;
#endif

    /* Now execute the stop instruction to go into VLLS1 */

    /* Set the SLEEPDEEP bit to enable deep sleep mode (STOP) */
    SCB->SCR |= (uint32_t)SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}
#endif

/********************************************************************/
/********************End of Functions *******************************/
/********************************************************************/
