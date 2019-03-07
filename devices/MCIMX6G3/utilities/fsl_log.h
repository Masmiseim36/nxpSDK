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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
 *
 */

#ifndef _FSL_LOG_H
#define _FSL_LOG_H

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*************************************************************************************************
 * Prototypes
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes
 *
 * Call this function to init the buffer
 * @return              	Indicates whether initialization was successful or not.
 * @retval kStatus_Success  Execution successfully
 * @retval kStatus_Fail     Execution failure
 */
status_t LOG_Init(uint32_t baseAddr, uint32_t baudRate, uint32_t clkSrcFreq);

/*!
 * @brief De-Initializes
 *
 * Call this function to deinit the buffer
 *
 * @return Indicates whether Deinit was successful or not.
 */
void LOG_Deinit(void);

/*!
 * @brief buffer push interface
 *
 * Call this function to print log
 * @param fmt, buffer pointer
 * @param size, avaliable size
 * @return
 */
int LOG_Print(char *buf, size_t size);

/*!
 * @brief buffer pop interface
 *
 * Call this function to print log
 * @param fmt, buffer pointer
 * @param size, avaliable size
 * @reutrn
 */
int LOG_Scanf(char *buf, size_t size);

/*!
 * @brief wait log and io idle
 *
 * Call this function to wait log buffer empty and io idle before enter low power mode.
 * @return Indicates whether wait idle was successful or not.
 */
status_t LOG_WaitIdle(void);

#if defined(__cplusplus)
}
#endif

#endif
