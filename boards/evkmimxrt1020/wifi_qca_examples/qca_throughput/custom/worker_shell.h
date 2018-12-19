/*
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WORKER_SHELL__H__
#define __WORKER_SHELL__H__

#include <stdint.h>

/* maximum line characters */
#ifndef WORKER_SHELL_ARGV_LINE
#define WORKER_SHELL_ARGV_LINE 64
#endif

/* maximum number of arguments */
#ifndef WORKER_SHELL_ARGC
#define WORKER_SHELL_ARGC 10
#endif

/* arguments are packed into line
 * so 'argv' always points into 'argv_line' */
typedef struct
{
    int32_t argc;
    char *argv[WORKER_SHELL_ARGC];
    char argv_line[WORKER_SHELL_ARGV_LINE];
} worker_shell_msg_t;

#endif
