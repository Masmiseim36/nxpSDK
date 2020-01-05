/* Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */


#ifndef AZURE_CLIENT_CRED_H
#define AZURE_CLIENT_CRED_H

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if !SSS_HAVE_AZURE
#error Only with AZURE_IOT
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

#if SSS_HAVE_ALT
static const char keyCLIENT_CERTIFICATE_PEM[] = {0};
static const char keyCLIENT_PRIVATE_KEY_PEM[] = {0};

#else

static const char keyCLIENT_CERTIFICATE_PEM[] = {0};
static const char keyCLIENT_PRIVATE_KEY_PEM[] = {0};

#endif

static const char *USED keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = "";

#endif // !AZURE_CLIENT_CRED_H
