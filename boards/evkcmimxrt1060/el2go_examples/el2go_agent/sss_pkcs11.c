/*
 * Copyright 2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ********************** Include files ********************** */
#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <sss_pkcs11_pal.h>

/* ********************** Constants ***************************** */
#define EX_SSS_BOOT_DO_ERASE 0

/* ********************** Global variables ********************** */
static ex_sss_boot_ctx_t gex_sss_demo_boot_ctx;
ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx = &gex_sss_demo_boot_ctx;

#if SSS_PKCS11_ENABLE_CLOUD_DEMO
static ex_sss_cloud_ctx_t gex_sss_demo_tls_ctx;
ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx = &gex_sss_demo_tls_ctx;
#endif
