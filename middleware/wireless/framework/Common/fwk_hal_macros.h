/*! *********************************************************************************
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#ifndef _HAL_MACROS_H_
#define _HAL_MACROS_H_

#include <stdint.h>

/* Required for __REV definition */
#if defined(__IAR_SYSTEMS_ICC__)
#include "cmsis_iccarm.h"
#elif defined(__CC_ARM)
#include "cmsis_armcc.h"
#endif

/*============================================================================
                         USEFUL MACROS
=============================================================================*/

/*!
 ****************************************************************************************
 * @brief
 * HAL_CLZ implementation : count leading zeroes from MSB
 * HAL_CTZ implementation : count trailing zeroes from LSB
 * HAL_RBIT implementation : bit mirror 32-bit operation:bit n moves to bit (31-n)
 *
 *****************************************************************************************/
#if defined(__GNUC__)

#define HAL_CLZ(x) __builtin_clz(x)
#define HAL_CTZ(x) __builtin_ctz(x)
static inline uint32_t __hal_revb(uint32_t x)
{
    unsigned int res;
    __asm volatile("rbit    %0, %1  \n\t" : "=r"(res) : "r"(x));
    return res;
}
#define HAL_RBIT(x) __hal_revb(x)

#elif defined(__IAR_SYSTEMS_ICC__)

#define HAL_CLZ(x)  __iar_builtin_CLZ(x)
#define HAL_RBIT(x) __iar_builtin_RBIT(x)

static inline uint32_t __hal_ctz(uint32_t x)
{
    uint32_t res;
    x   = (uint32_t)HAL_RBIT((unsigned int)x);
    res = (uint32_t)HAL_CLZ((unsigned int)x);
    return res;
}

#define HAL_CTZ(x) __hal_ctz(x)

#else
#define HAL_CLZ(x) HAL_CLZ_UNDEFINED(x)
#define HAL_CTZ(x) HAL_CTZ_UNDEFINED(x)
#endif

/*!
 ****************************************************************************************
 * @brief
 * HAL_BSR Bit Scan Reverse ( find MSB bit set in a bit field )
 * HAL_BSF Bit Scan Forward ( find LSB bit set in a bit field )
 * HAL_FFS Find First Bit Set Find LSB bit position + 1 per standard ffs definition
 *
 ****************************************************************************************
 */
#define HAL_BSR(x) (31 - HAL_CLZ(x))
#define HAL_BSF(x) HAL_CTZ(x)
#define HAL_FFS(x) (HAL_CTZ(x) + 1)

/*!
 ****************************************************************************************
 * @brief
 * HAL_REV16 swap bytes in a 16 bit word : useful for htons/ntohs and all enddianness conversions
 * HAL_REV32 swap bytes in a 32 bit word : useful for htonl/ntohl and all enddianness conversions
 ****************************************************************************************
 */
#if defined(__GNUC__)
#define HAL_REV16(x) __builtin_bswap16(x)
#define HAL_REV32(x) __builtin_bswap32(x)

#elif defined(__IAR_SYSTEMS_ICC__) || defined(__CC_ARM)

#define HAL_REV16(x) __REV16(x)
#define HAL_REV32(x) __REV(x)

#endif

#define HAL_HTONS(_x_) HAL_REV16((_x_))
#define HAL_NTOHS(_x_) HAL_REV16((_x_))

#define HAL_HTONL(_x_) HAL_REV32((_x_))
#define HAL_NTOHL(_x_) HAL_REV32((_x_))

#define HAL_BSWAP16(_x_) HAL_REV16((_x_))
#define HAL_BSWAP32(_x_) HAL_REV32((_x_))

/*!
 ****************************************************************************************
 * @brief
 * KB used to define memory sizes expressed in kilobytes
 * MB used to define memory sizes expressed in megabytes
 ****************************************************************************************
 */
#ifndef KB
#define KB(x) (((uint32_t)x) << 10u)
#endif
#ifndef MB
#define MB(x) (((uint32_t)x) << 20u)
#endif

/*!
 ****************************************************************************************
 * @brief
 * KHz used to define frequencies in kHz
 * MHz used to define frequencies in MHz
 ****************************************************************************************
 */
#define KHz(x) (((uint32_t)x) * 1000U)
#define MHz(x) (((uint32_t)x) * 1000000U)

#define SET_BIT(bitmap, i) bitmap[((i) >> 5)] |= (1U << ((i)&0x1f))
#define CLR_BIT(bitmap, i) bitmap[((i) >> 5)] &= ~(1U << ((i)&0x1f))
#define GET_BIT(bitmap, i) (((bitmap[(i) >> 5] & (1U << ((i)&0x1f))) >> ((i)&0x1f)) != 0U)

/* LOG_1, LOG_2, LOG_4, LOG_8: used by LOG macro */
#define LOG_1(n) (((n) >= 2) ? 1 : 0)
#define LOG_2(n) (((n) >= (1 << 2)) ? (2 + LOG_1((n) >> 2)) : LOG_1(n))
#define LOG_4(n) (((n) >= (1 << 4)) ? (4 + LOG_2((n) >> 4)) : LOG_2(n))
#define LOG_8(n) (((n) >= (1 << 8)) ? (8 + LOG_4((n) >> 8)) : LOG_4(n))
/*
 * Macro to compute log2 of a constant at compile time.
 * Does not make sense for runtime log2 calculation.
 * Computation should be based on __builtin_clz.
 * For in
 *
 */
#define LOG(n) (((n) >= (1 << 16)) ? (16 + LOG_8((n) >> 16)) : LOG_8(n))

#endif
