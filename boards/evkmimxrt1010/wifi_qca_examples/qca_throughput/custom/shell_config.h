/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __SHELL_CONFIG_H__
#define __SHELL_CONFIG_H__

/* config if use standard output */
#define SHELL_USE_FILE_STREAM 0
/* config if use auto complete */
#define SHELL_AUTO_COMPLETE 1
/* config if use history */
#define SHELL_USE_HISTORY 1

/* Those macros should not be too large for it will cause large RAM consumption */
/* consult buffer size */
#define SHELL_BUFFER_SIZE (100)
/* maximum function args */
#define SHELL_MAX_ARGS (20)
/* maximum number of history record. */
#define SHELL_HIST_MAX (10)
/* buffer size of shell_printf() */
#define SHELL_PRINTF_BUF_SIZE (120)

#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
