/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 */

#ifndef _FSL_SDRAM_H_
#define _FSL_SDRAM_H_

#include "fsl_sdramc.h"

/*!
 * @addtogroup sdram_mt48lc2m32b2
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief SDRAM driver version. */
#define FSL_SDRAM_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */
/*! @brief Define the start address of the SDRAM. */
#define SDRAM_START_ADDRESS (0x70000000U)
/*! @brief Define the access value of the SDRAM location. */
#define SDRAM_COMMAND_ACCESSVALUE 0xaaU

/*! @brief Define the right address line for the mrs register setting. */
#define ADDR9 9U
#define ADDR10 10U
#define ADDR11 11U
#define ADDR12 12U
#define ADDR13 13U
#define ADDR14 14U
#define ADDR15 15U
#define ADDR16 16U
#define ADDR17 17U
#define ADDR18 18U
#define ADDR19 19U
#define ADDR20 20U
#define ADDR21 21U
#define ADDR22 22U
#define ADDR23 23U

/*! @brief Define the physical connection--32bit port 8 column. */
#define SDRAM_A0 ADDR15
#define SDRAM_A1 ADDR14
#define SDRAM_A2 ADDR13
#define SDRAM_A3 ADDR12
#define SDRAM_A4 ADDR11
#define SDRAM_A5 ADDR10
#define SDRAM_A6 ADDR9
#define SDRAM_A7 ADDR17
#define SDRAM_A8 ADDR18
#define SDRAM_A9 ADDR19
#define SDRAM_A10 ADDR20
#define SDRAM_A11 ADDR21
#define SDRAM_A12 ADDR22
#define SDRAM_A13 ADDR23

#define BURSTLENGTH 0U
#define BURSTTYPE 0U
#define CASLATENCY 2U
#define OPMODE 0U
#define WRITEBURSTMODE 0U

/*! @brief SDRAM Mode register write burst mode setting. */
typedef enum _sdram_write_burst_mode
{
    kSDRAM_MrsWriteBurst = 0x0U, /*!< Write burst mode. */
    kSDRAM_MrsWriteSingle        /*!< Write single location mode.  */
} sdram_write_burst_mode_t;

/*! @brief SDRAM Mode register operation mode setting. */
typedef enum _sdram_operation_mode
{
    kSDRAM_MrsStandOperation = 0x0U, /*!< Standard operation mode. */
    kSDRAM_MrsAllOtherReserve        /*!< All other states reserved.  */
} sdram_operation_mode_t;

/*! @brief SDRAM Mode register CAS latency setting. */
typedef enum _sdram_cas_latency
{
    kSDRAM_MrsLatencyOne = 0x1U, /*!< Latency one. */
    kSDRAM_MrsLatencyTwo,        /*!< Latency two. */
    kSDRAM_MrsLatencyThree       /*!< Latency three. */
} sdram_cas_latency_t;

/*! @brief SDRAM Mode register burst type setting. */
typedef enum _sdram_burst_type
{
    kSDRAM_MrsSequential = 0x0U, /*!< Sequential. */
    kSDRAM_MrsInterleaved,       /*!< Interleaved. */
} sdram_burst_type_t;

/*! @brief SDRAM Mode register burst length setting. */
typedef enum _sdram_burst_len
{
    kSDRAM_MrsBurstLenOne = 0x0U,    /*!< 1. */
    kSDRAM_MrsBurstLenTwo = 0x1U,    /*!< 2. */
    kSDRAM_MrsBurstLenFour = 0x2U,   /*!< 4. */
    kSDRAM_MrsBurstLenEight = 0x3U,  /*!< 8. */
    kSDRAM_MrsBurstLenAllPage = 0x7U /*!< Full page only for sequential burst type. */
} sdram_burst_len_t;
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the SDRAM device.
 * The function is used to initialize the MT48LC4M32B2 SDRAM external memory.
 * @param base SDRAM controller peripheral base address.
 * @param busClock_Hz The bus clock frequency for SDRAM controller.
 * @return the execution result.
 *
 */
status_t SDRAM_Init(SDRAM_Type *base, uint32_t busClock_Hz);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /*_FSL_SDRAM_H_*/
