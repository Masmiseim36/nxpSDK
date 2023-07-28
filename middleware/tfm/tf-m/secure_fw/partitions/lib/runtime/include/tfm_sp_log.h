/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SP_LOG_H__
#define __TFM_SP_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* The Secure Partition log levels */
#define TFM_PARTITION_LOG_LEVEL_DEBUG     3  /* All log APIs output */
#define TFM_PARTITION_LOG_LEVEL_INFO      2  /*
                                              * All log APIs output except
                                              * LOG_DBGFMT
                                              */
#define TFM_PARTITION_LOG_LEVEL_ERROR     1  /*
                                              * Only LOG_ERRFMT APIs output.
                                              */
#define TFM_PARTITION_LOG_LEVEL_SILENCE   0  /* All log APIs are suppressed */

#ifndef TFM_PARTITION_LOG_LEVEL
#error "TFM_PARTITION_LOG_LEVEL not defined!"
#endif

#if (TFM_PARTITION_LOG_LEVEL > TFM_PARTITION_LOG_LEVEL_DEBUG)  || \
    (TFM_PARTITION_LOG_LEVEL < TFM_PARTITION_LOG_LEVEL_SILENCE)
#error "Incorrect TFM_PARTITION_LOG_LEVEL value!"
#endif

#if (TFM_PARTITION_LOG_LEVEL == TFM_PARTITION_LOG_LEVEL_DEBUG)
#define LOG_DBGFMT(...) tfm_sp_log_printf(__VA_ARGS__) //NXP was printf(), workaround Keil error
#else
#define LOG_DBGFMT(...)
#endif

#if (TFM_PARTITION_LOG_LEVEL >= TFM_PARTITION_LOG_LEVEL_INFO)
#define LOG_INFFMT(...) tfm_sp_log_printf(__VA_ARGS__)  //NXP was printf(), workaround Keil error
#else
#define LOG_INFFMT(...)
#endif

#if (TFM_PARTITION_LOG_LEVEL >= TFM_PARTITION_LOG_LEVEL_ERROR)
#define LOG_ERRFMT(...) tfm_sp_log_printf(__VA_ARGS__)  //NXP was printf(), workaround Keil error
#else
#define LOG_ERRFMT(...)
#endif

int tfm_sp_log_printf(const char *fmt, ...);   //NXP was printf(), workaround Keil error

#ifdef __cplusplus
}
#endif

#endif /* __TFM_SP_LOG_H__ */
