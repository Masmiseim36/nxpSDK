/* Copyright 2019,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AZURE_CLIENT_CRED_H
#define AZURE_CLIENT_CRED_H

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#include <stdio.h>

#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif

/*
* PEM-encoded client certificate*
*
*/

#if SSS_HAVE_MBEDTLS_ALT
static const char keyCLIENT_CERTIFICATE_PEM[] = {0};
static const char keyCLIENT_PRIVATE_KEY_PEM[] = {0};

#else

static const char keyCLIENT_CERTIFICATE_PEM[] = {0};
static const char keyCLIENT_PRIVATE_KEY_PEM[] = {0};

#endif

static const char *USED keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = "";

#endif // !AZURE_CLIENT_CRED_H
