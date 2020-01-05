/* Copyright 2018-2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_SE05X || SSS_HAVE_LOOPBACK

#include <board.h>
#include "ax_reset.h"

#include "sm_timer.h"
#include "sm_types.h"
#include "fsl_common.h"
#if FSL_FEATURE_SOC_PIT_COUNT
#include "fsl_pit.h"
#endif
#include "se05x_apis.h"

#if FSL_FEATURE_SOC_PIT_COUNT
#define SE_PIT_RESET_HANDLER PIT0_IRQHandler
#ifdef CPU_MIMXRT1052DVL6B
#define PIT_IRQ_ID PIT_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)
#endif
#ifdef FRDM_K64F
#define PIT_IRQ_ID PIT0_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#endif

void SE_PIT_RESET_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
    PIT_Deinit(PIT);
    se05x_ic_reset();
}

void se_pit_SetTimer(U16 time_us)
{
    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);
    /* Init pit module */
    PIT_Init(PIT, &pitConfig);
    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(
        PIT, kPIT_Chnl_0, USEC_TO_COUNT(time_us, PIT_SOURCE_CLOCK));
    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);
    /* Start channel 0 */
    PIT_StartTimer(PIT, kPIT_Chnl_0);
}
#endif

#endif /* #if SSS_HAVE_SE05X || SSS_HAVE_LOOPBACK */
