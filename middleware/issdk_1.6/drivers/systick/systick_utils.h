/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

/**
 * @file systick_utils.h
 * @brief ARM Systick Utilities.

    This file provides systick based counts for the developer
    to get cycle counts for measuring time delays.
*/

#ifndef __SYSTICK_UTILS_H__
#define __SYSTICK_UTILS_H__

/*! @brief       Function to enable systicks framework.
 *  @details     This function initializes the CMSIS define ARM core specific systick implementation.
 *  @param[in]   void.
 *  @return      void.
 *  @constraints This should be the first function to be invoked before other Systick APIs.
 *  @reeentrant  Yes
 */
void BOARD_SystickEnable(void);

/*! @brief       Function to Record the Start systick.
 *  @details     This function records the current systick count for other APIs to compute difference.
 *  @param[in]   pStart Pointer to the variable to store the current/start systick.
 *  @return      void.
 *  @constraints None.
 *  @reeentrant  Yes
 */
void BOARD_SystickStart(int32_t *pStart);

/*! @brief       Function to compute the Elapsed systicks.
 *  @details     This function computes the elapsed ticks by getting the difference
 *               of the current tick to the one in the arguement.
 *  @param[in]   pStart Pointer to the variable contating the start systick.
 *  @return      int32_t The elapsed systicks.
 *  @constraints None.
 *  @reeentrant  Yes
 */
int32_t BOARD_SystickElapsedTicks(int32_t *pStart);

/*! @brief       Function to compute the Elapsed Time.
 *  @details     This function computes the elapsed Time in micro seconds by getting the difference
 *               of the current tick to the one in the arguement.
 *               It also updates the start tick to the current systick.
 *  @param[in]   pStart Pointer to the variable contating the start systick.
 *  @return      uint32_t The elapsed Time in micro seconds.
 *  @constraints None.
 *  @reeentrant  Yes
 */
uint32_t BOARD_SystickElapsedTime_us(int32_t *pStart);

/*! @brief       Function to insert delays.
 *  @details     This function blocks for the specified milli seconds by using the elapsed systicks
 *               to determine time delays.
 *  @param[in]   delay_ms The required time to block.
 *  @return      void.
 *  @constraints None.
 *  @reeentrant  Yes
 */
void BOARD_DELAY_ms(uint32_t delay_ms);

#endif // __SYSTICK_UTILS_H__
