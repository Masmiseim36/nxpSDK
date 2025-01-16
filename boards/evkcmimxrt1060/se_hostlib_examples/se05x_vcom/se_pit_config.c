/* Copyright 2018-2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if SSS_HAVE_APPLET_SE05X_IOT || SSS_HAVE_APPLET_LOOPBACK

#include <board.h>
#include "ax_reset.h"

#include "sm_timer.h"
#include "sm_types.h"
#include "fsl_common.h"
#include "se_board_config.h"
#include "se05x_apis.h"

#if FSL_FEATURE_SOC_PIT_COUNT

void SE_PIT_RESET_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT_BASE_ADDR, kPIT_Chnl_0, kPIT_TimerFlag);
#ifdef CPU_MIMXRT1176DVMAA_cm7
    __DSB();
#endif
    PIT_Deinit(PIT_BASE_ADDR);
    se05x_ic_reset();
}

void se_pit_SetTimer(uint32_t time_ms)
{
    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);
    /* Init pit module */
    PIT_Init(PIT_BASE_ADDR, &pitConfig);
    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(
        PIT_BASE_ADDR, kPIT_Chnl_0, MSEC_TO_COUNT(time_ms, PIT_SOURCE_CLOCK));
    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT_BASE_ADDR, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);
    /* Start channel 0 */
    PIT_StartTimer(PIT_BASE_ADDR, kPIT_Chnl_0);
}
#endif

#endif /* #if SSS_HAVE_APPLET_SE05X_IOT || SSS_HAVE_APPLET_LOOPBACK */
