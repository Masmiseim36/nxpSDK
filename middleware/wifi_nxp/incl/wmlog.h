/** @file wmlog.h
 *
 *  @brief This file contains macros to print logs
 *
 *  Copyright 2008-2020 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#ifndef __WMLOG_H__
#define __WMLOG_H__

#include "fsl_debug_console.h"

#ifdef CONFIG_ENABLE_ERROR_LOGS
#define wmlog_e(_mod_name_, _fmt_, ...) (void)PRINTF("[%s]%s" _fmt_ "\n\r", _mod_name_, " Error: ", ##__VA_ARGS__)
#else
#define wmlog_e(...)
#endif /* CONFIG_ENABLE_ERROR_LOGS */

#ifdef CONFIG_ENABLE_WARNING_LOGS
#define wmlog_w(_mod_name_, _fmt_, ...) (void)PRINTF("[%s]%s" _fmt_ "\n\r", _mod_name_, " Warn: ", ##__VA_ARGS__)
#else
#define wmlog_w(...)
#endif /* CONFIG_ENABLE_WARNING_LOGS */

/* General debug function. User can map his own debug functions to this
ne */
#define wmlog(_mod_name_, _fmt_, ...) (void)PRINTF("[%s] " _fmt_ "\n\r", _mod_name_, ##__VA_ARGS__)

/* Function entry */
#define wmlog_entry(_fmt_, ...) (void)PRINTF("> %s (" _fmt_ ")\n\r", __func__, ##__VA_ARGS__)

/* function exit */
#define wmlog_exit(_fmt_, ...) (void)PRINTF("< %s" _fmt_ "\n\r", __func__, ##__VA_ARGS__)

#endif /* __WMLOG_H__ */
