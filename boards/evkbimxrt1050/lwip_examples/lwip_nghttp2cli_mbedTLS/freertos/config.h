/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __HAVE_CONFIG_H_
#define __HAVE_CONFIG_H_

#define _U_

#define SIZEOF_INT_P 2

/* For debug ouput from nghttp2. */
/* #define DEBUGBUILD */

#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FREERTOS

#if (!defined(nghttp_unlikely))
#define nghttp_unlikely(Expression) !!(Expression)
#endif

#define nghttp_ASSERT(Expression)     \
    do                                \
    {                                 \
        if (!(Expression))            \
            PRINTF("%d\n", __LINE__); \
    } while (0)

#define CU_ASSERT(a) nghttp_ASSERT(a)
#define CU_ASSERT_FATAL(a) nghttp_ASSERT(a)

#if 1
#define NGHTTP2_DEBUG_INFO() PRINTF("%s %d\n", __FILE__, __LINE__)
#else
#define NGHTTP2_DEBUG_INFO()
#endif

#include "lwip/opt.h"
#include "lwip/tcpip.h"

#endif
