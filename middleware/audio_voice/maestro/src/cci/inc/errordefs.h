/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! @file errordefs.h
 *  @brief  Contains error codes.
 *
 */

#ifndef _ERRORDEFS_H
#define _ERRORDEFS_H

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef __LANGUAGE_ASM__
/**
 * @brief Return status
 *
 */
typedef int RtStatus_t;
#endif
/**
 * @brief Status success
 *
 */
#define SUCCESS (0)
/**
 * @brief Generic error
 *
 */
#define ERROR_GENERIC (-1)

#if defined(__cplusplus)
}
#endif

#endif /* _ERRORDEFS_H  */
