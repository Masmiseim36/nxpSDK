/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
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
