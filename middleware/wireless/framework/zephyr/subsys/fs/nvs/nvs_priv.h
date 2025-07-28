/*! *********************************************************************************
 *
 * Copyright (c) 2018 Laczen
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __NVS_PRIV_H_
#define __NVS_PRIV_H_

#include <zephyr/sys/crc.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * MASKS AND SHIFT FOR ADDRESSES
 * an address in nvs is an uint32_t where:
 *   high 2 bytes represent the sector number
 *   low 2 bytes represent the offset in a sector
 */
#define ADDR_SECT_MASK 0xFFFF0000
#define ADDR_SECT_SHIFT 16
#define ADDR_OFFS_MASK 0x0000FFFF

/*
 * Status return values
 */
#define NVS_STATUS_NOSPACE 1

#define NVS_BLOCK_SIZE 32

#define NVS_LOOKUP_CACHE_NO_ADDR 0xFFFFFFFF


#undef BUILD_ASSERT /* clear out common version */
/* C++11 has static_assert built in */
#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define BUILD_ASSERT(EXPR, MSG...) static_assert(EXPR, "" MSG)

/*
 * GCC 4.6 and higher have the C11 _Static_assert built in and its
 * output is easier to understand than the common BUILD_ASSERT macros.
 * Don't use this in C++98 mode though (which we can hit, as
 * static_assert() is not available)
 */
#elif !defined(__cplusplus) && \
	((__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) ||	\
	 (__STDC_VERSION__) >= 201100)
#define BUILD_ASSERT(EXPR, MSG...) _Static_assert(EXPR, "" MSG)
#else
#define BUILD_ASSERT(EXPR, MSG...)
#endif


/* Allocation Table Entry */
struct nvs_ate {
	uint16_t id;	/* data id */
	uint16_t offset;	/* data offset within sector */
	uint16_t len;	/* data len within sector */
	uint8_t part;	/* part of a multipart data - future extension */
	uint8_t crc8;	/* crc8 check of the entry */
};

BUILD_ASSERT(offsetof(struct nvs_ate, crc8) ==
		 sizeof(struct nvs_ate) - sizeof(uint8_t),
		 "crc8 must be the last member");

#ifdef __cplusplus
}
#endif

#endif /* __NVS_PRIV_H_ */
