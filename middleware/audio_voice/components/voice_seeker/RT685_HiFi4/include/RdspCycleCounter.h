/*
 * Copyright 2021 by Retune DSP
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RDSP_CYCLE_COUNTER_H
#define RDSP_CYCLE_COUNTER_H

#include "RdspTypes.h"

#ifdef _WIN32
#include <intrin.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if RDSP_SIM
#define RDSP_ENABLE_CYCLE_COUNT 1
#else
#define RDSP_ENABLE_CYCLE_COUNT 0
#endif

#if RDSP_ENABLE_CYCLE_COUNT==0

#define RDSP_INIT_CCOUNT
#define RDSP_READ_CCOUNT(Aenable) 0

#else

#define RDSP_INIT_CCOUNT init_ccount()
#define RDSP_READ_CCOUNT(Aenable) (Aenable ? read_ccount() : 0)

#if _WIN32==1

	inline void init_ccount() {}
	inline uint32_t read_ccount() {
		return (uint32_t)__rdtsc();
	}

#elif __ARM_NEON==1

	inline void init_ccount() {}
		// If these cycle counter instruction causes an illegal instruction error, go to the folder arm_enable_cycle_counter/ and follow instructions on how to enable reading the cycle counter in user-space for your platform
#if defined(__arm__)
	inline uint32_t read_ccount() {
		uint32_t cc = 0;
		__asm__ volatile ("mrc p15, 0, %0, c9, c13, 0":"=r" (cc)); // Read 32-bit cycle counter into cc
		return cc;
	}
#elif defined(__aarch64__)
	inline uint32_t read_ccount() {
		uint64_t cc = 0;
		asm volatile("mrs %0, pmccntr_el0" : "=r"(cc)); // Read 64-bit cycle counter into val
		return (uint32_t)cc;
	}
#endif

#elif __arm__==1

/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define RDSP_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))  /*!< DWT Control register */
#define RDSP_DWT_CYCCNTENA_BIT       (1UL<<0)                             /*!< CYCCNTENA bit in DWT_CONTROL register */
#define RDSP_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))  /*!< DWT Cycle Counter register */
#define RDSP_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))  /*!< DEMCR: Debug Exception and Monitor Control Register */
#define RDSP_TRCENA_BIT              (1UL<<24)                            /*!< Trace enable bit in DEMCR register */

#define RDSP_INIT_CYCLE_COUNTER() \
RDSP_DEMCR |= RDSP_TRCENA_BIT     /*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */

#define RDSP_RESET_CYCLE_COUNTER() \
RDSP_DWT_CYCCNT = 0               /*!< Reset cycle counter */

#define RDSP_ENABLE_CYCLE_COUNTER() \
RDSP_DWT_CONTROL |= RDSP_DWT_CYCCNTENA_BIT    /*!< Enable cycle counter */

#define RDSP_DISABLE_CYCLE_COUNTER() \
RDSP_DWT_CONTROL &= ~RDSP_DWT_CYCCNTENA_BIT   /*!< Disable cycle counter */

#define RDSP_GET_CYCLE_COUNTER() \
RDSP_DWT_CYCCNT                   /*!< Read cycle counter register */

	inline void init_ccount() {
		RDSP_INIT_CYCLE_COUNTER();
		RDSP_ENABLE_CYCLE_COUNTER();
	}
	inline uint32_t read_ccount() {
		return RDSP_GET_CYCLE_COUNTER();
	}

#else

	inline void init_ccount() {}
	inline uint32_t read_ccount() {
		unsigned int ccount;
		__asm__(
			"rsr %0, ccount"
			: "=a" (ccount) : : "memory"
		);
		return ccount;
	}
#endif // PLATFORM
#endif // RDSP_ENABLE_CYCLE_COUNT

#ifdef __cplusplus
}
#endif

#endif // RDSP_CYCLE_COUNTER_H
