/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright  2016 NXP
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
#ifndef _FSL_CACHE_H_
#define _FSL_CACHE_H_

#include "fsl_common.h"

/*!
 * @addtogroup cache
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief cache driver version 2.1.0. */
#define FSL_CACHE_DRIVER_VERSION (MAKE_VERSION(2, 1, 0))
/*@}*/
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name cache control for the L1 low power local memory controller
 *@{
 */

/*!
 * @brief Enables the processor code bus cache.
 *
 */
static inline void L1CACHE_EnableCodeCache(void)
{
    MCM->CPCR2 &= ~MCM_CPCR2_DCC_MASK;
}

/*!
 * @brief Disables the processor code bus cache.
 *
 */
static inline void L1CACHE_DisableCodeCache(void)
{
    MCM->CPCR2 |= MCM_CPCR2_DCC_MASK;
}

/*!
 * @brief Invalidates the processor code bus cache.
 *
 */
static inline void L1CACHE_InvalidateCodeCache(void)
{
    MCM->CPCR2 |= MCM_CPCR2_CCBC_MASK;
}

/*@}*/

/*!
 * @name The unified L1 cache controller
 *  The LPLMEM is write-through design, so there is no cache maintain by range
 *  control operation. So all cache maintain by range unified functions are
 *  directly call the cache maintain all functions since they have the same effect.
 *@{
 */

/*!
 * @brief Invalidates L1 instrument cache by range.
 *
 * @param address  The start address of the memory to be invalidated.
 * @param size_byte  The memory size.
 */
void L1CACHE_InvalidateICacheByRange(uint32_t address, uint32_t size_byte);

/*!
 * @brief Invalidates L1 data cache by range.
 *
 * @param address  The start address of the memory to be invalidated.
 * @param size_byte  The memory size.
 */
static inline void L1CACHE_InvalidateDCacheByRange(uint32_t address, uint32_t size_byte)
{
    L1CACHE_InvalidateICacheByRange(address, size_byte);
}

/*!
 * @brief Cleans L1 data cache by range.
 *
 * The cache is write through mode, so there is nothing to do with
 * the cache flush/clean operation.
 *
 * @param address  The start address of the memory to be cleaned.
 * @param size_byte  The memory size.
 */
static inline void L1CACHE_CleanDCacheByRange(uint32_t address, uint32_t size_byte)
{
}

/*!
 * @brief Cleans and Invalidates L1 data cache by range.
 *
 * @param address  The start address of the memory to be clean and invalidated.
 * @param size_byte  The memory size.
 */
static inline void L1CACHE_CleanInvalidateDCacheByRange(uint32_t address, uint32_t size_byte)
{
    L1CACHE_InvalidateDCacheByRange(address, size_byte);
}

/*@}*/

/*!
 * @name Unified Cache Control for caches in all levels
 *@{
 */

/*!
 * @brief Invalidates instruction cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be invalidated.
 */
static inline void ICACHE_InvalidateByRange(uint32_t address, uint32_t size_byte)
{
    L1CACHE_InvalidateICacheByRange(address, size_byte);
}

/*!
 * @brief Invalidates data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be invalidated.
 */
static inline void DCACHE_InvalidateByRange(uint32_t address, uint32_t size_byte)
{
    L1CACHE_InvalidateDCacheByRange(address, size_byte);
}

/*!
 * @brief Clean data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be cleaned.
 */
static inline void DCACHE_CleanByRange(uint32_t address, uint32_t size_byte)
{
    L1CACHE_CleanDCacheByRange(address, size_byte);
}

/*!
 * @brief Cleans and Invalidates data cache by range.
 *
 * @param address The physical address.
 * @param size_byte size of the memory to be Cleaned and Invalidated.
 */
static inline void DCACHE_CleanInvalidateByRange(uint32_t address, uint32_t size_byte)
{
    L1CACHE_CleanInvalidateDCacheByRange(address, size_byte);
}

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_CACHE_H_*/
