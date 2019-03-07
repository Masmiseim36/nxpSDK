/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LPM_H_
#define _LPM_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Power mode definition of low power management.
 * Waken up duration Off > Dsm > Idle > Wait > Run.
 */
typedef enum _lpm_power_mode
{
    LPM_PowerModeRun = 0, /* Normal RUN mode, CPU won't stop running */

    /* In system wait mode, cpu clock is gated.
     * All peripheral can remain active, clock gating decided by CCGR setting.
     * DRAM enters auto-refresh mode when there is no access.
     */
    LPM_PowerModeWait, /* System WAIT mode */

    /* In low power idle mode, all PLL/PFD is off, cpu power is off.
     * Analog modules running in low power mode.
     * All high-speed peripherals are power gated
     * Low speed peripherals can remain running at low frequency
     * DRAM in self-refresh.
     */
    LPM_PowerModeIdle, /* Low Power Idle mode */

    /* In deep sleep mode, all PLL/PFD is off, XTAL is off, cpu power is off.
     * All clocks are shut off except 32K RTC clock
     * All high-speed peripherals are power gated
     * Low speed peripherals are clock gated
     * DRAM in self-refresh.
     * If RTOS is used, systick will be disabled in DSM
     */
    LPM_PowerModeDsm, /* Deep Sleep mode. */

    LPM_PowerModeOff, /* Power off mode, or shutdown mode */
} lpm_power_mode_t;

typedef bool (*lpm_power_mode_callback_t)(lpm_power_mode_t curMode, lpm_power_mode_t newMode, void *data);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/* Initialize the Low Power Management */
bool LPM_Init(void);

/* Deinitialize the Low Power Management */
void LPM_Deinit(void);

/* Enable wakeup source in low power mode */
void LPM_EnableWakeupSource(uint32_t irq);

/* Disable wakeup source in low power mode */
void LPM_DisableWakeupSource(uint32_t irq);

#ifdef FSL_RTOS_FREE_RTOS
/* Notify power mode, all registered listeners will be notified.
 * Return true if all the registered listeners return true.
 */
bool LPM_NotifyPowerMode(lpm_power_mode_t mode);
#endif

/* Switch power mode */
void LPM_SetPowerMode(lpm_power_mode_t mode);

/* LPM_SetPowerMode() won't switch system power status immediately,
 * instead, such operation is done by LPM_WaitForInterrupt().
 * It can be callled in idle task of FreeRTOS, or main loop in bare
 * metal application. The sleep depth of this API depends
 * on current power mode set by LPM_SetPowerMode().
 * The timeoutMilliSec means if no interrupt occurs before timeout, the
 * system will be waken up by systick. If timeout exceeds hardware timer
 * limitation, timeout will be reduced to maximum time of hardware.
 * timeoutMilliSec only works in FreeRTOS, in bare metal application,
 * it's just ignored.
 */
void LPM_WaitForInterrupt(uint32_t timeoutMilliSec);

#ifdef FSL_RTOS_FREE_RTOS
/* Register power mode switch listener. When LPM_SetPowerMode()
 * is called, all the registered listeners will be invoked. The
 * listener returns true if it allows the power mode switch,
 * otherwise returns FALSE.
 */
void LPM_RegisterPowerListener(lpm_power_mode_callback_t callback, void *data);

/* Unregister power mode switch listener */
void LPM_UnregisterPowerListener(lpm_power_mode_callback_t callback, void *data);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
