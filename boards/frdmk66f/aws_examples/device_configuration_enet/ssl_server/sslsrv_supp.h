/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 *   SSLSRV support functions header.
 */

#ifndef _SSLSRV_SUPP_H_
#define _SSLSRV_SUPP_H_

#include "sslsrv_prv.h"
#include "sslsrv.h"

#ifdef __cplusplus
extern "C" {
#endif

void sslsrv_destroy_server(SSLSRV_STRUCT *server);
SSLSRV_STRUCT *sslsrv_create_server(SSLSRV_PARAM_STRUCT *params);
void sslsrv_abort(int sock);
void *sslsrv_mem_alloc_zero(size_t xSize);

#ifdef __cplusplus
}
#endif

#endif /* _SSLSRV_SUPP_H_ */
