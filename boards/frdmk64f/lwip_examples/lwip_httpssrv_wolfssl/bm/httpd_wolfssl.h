/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LWIP_HDR_APPS_HTTPD_WOLFSSL_H
#define LWIP_HDR_APPS_HTTPD_WOLSSL_H

#include "wolfssl/ssl.h"

#ifdef __cplusplus
extern "C" {
#endif

void httpd_wolfssl_init(WOLFSSL_CTX* ctx);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_APPS_HTTPD_WOLSSL_H */
