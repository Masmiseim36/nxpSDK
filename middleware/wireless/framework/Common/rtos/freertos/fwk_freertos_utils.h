/* -------------------------------------------------------------------------- */
/*                             Copyright 2023 NXP                             */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Works in pair with FWK_PostIdleHookTickCompensation and aims to estimate the ticks missed by FreeRTOS
 * scheduler during vApplicationIdleHook by using SOC timers. This mechanism is useful when the idle hook is used for
 * flash operation, which can take time and during which the interrupts may be masked.
 * This function takes a temporal point of reference and should be called at the beginning of vApplicationIdleHook.
 *
 */
void FWK_PreIdleHookTickCompensation(void);

/*!
 * \brief Works in pair with FWK_PostIdleHookTickCompensation and aims to estimate the ticks missed by FreeRTOS
 * scheduler during vApplicationIdleHook by using SOC timers. This mechanism is useful when the idle hook is used for
 * flash operation, which can take time and during which the interrupts may be masked.
 * This function should be called at the end of vApplicationIdleHook.  It will estimate the time elapsed since
 * FWK_PreIdleHookTickCompensation was called and if it detects than one or more ticks have been missed, it will notify
 * the FreeRTOS kernel using xTaskCatchUpTicks API.
 *
 */
void FWK_PostIdleHookTickCompensation(void);