/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef LIB_COMMON_H
#define LIB_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "container.h"
#include "vector.h"
#include "file_utils.h"
#include "logging.h"
#include "error.h"
#include "general_utils.h"
#include "ringbuffer.h"

#define OSA_MEMTYPE_LIST      (OSA_FWMEMTYPE_BASE + 5)
#define OSA_MEMTYPE_VECTOR    (OSA_FWMEMTYPE_BASE + 6)

/* Convert BCD and integer from 0-99 */
#define BCD_TO_INT(bcd)  ((((bcd >> 4) & 0x0f) * 10) + (bcd & 0x0f))
#define INT_TO_BCD(val)   (((val/10) << 4) | (val % 10))

#ifdef __cplusplus
}
#endif

#endif

