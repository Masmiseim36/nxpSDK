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


#ifndef GCP_CLIENT_CRED_H
#define GCP_CLIENT_CRED_H

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if !SSS_HAVE_GCP
#error Only with GCP_IOT
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

static const char keyCLIENT_CERTIFICATE_PEM[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBvzCCAWUCCQCIuLi+EEYNWjAKBggqhkjOPQQDAjCBjDELMAkGA1UEBhMCQkUx\n"
    "FjAUBgNVBAgMDVZsYWFtc0JyYWJhbnQxDzANBgNVBAcMBkxldXZlbjEUMBIGA1UE\n"
    "CgwLTlhQLURlbW8tQ0ExEjAQBgNVBAsMCURlbW8tVW5pdDEPMA0GA1UEAwwGZGVt\n"
    "b0NBMRkwFwYJKoZIhvcNAQkBFgpkZW1vQ0FAbnhwMB4XDTE3MTIyMTA5MDY0NloX\n"
    "DTI1MDgyMTA5MDY0NlowQjELMAkGA1UEBhMCQkUxEDAOBgNVBAoMB05YUERlbW8x\n"
    "DTALBgNVBAsMBFVuaXQxEjAQBgNVBAMMCWxvY2FsaG9zdDBZMBMGByqGSM49AgEG\n"
    "CCqGSM49AwEHA0IABCXaPWClCaYePdRUi4goZ9eicAYBKee6citt2+VZaDx+v/kD\n"
    "xRGJ1HfmHcSijGafgspu3K/GVMrE7/JSMMN8PM8wCgYIKoZIzj0EAwIDSAAwRQIg\n"
    "Esc7MAdvgsLFzw2ujU5f07lQ0a1ptvGtBAu+SQir6h0CIQCXzEmYwEV1jImh/5Em\n"
    "L8+x5gptkfXmGglKNYpGTDK7xQ==\n"
    "-----END CERTIFICATE-----\n";

/*
* PEM-encoded client private key.
*
*/
static const char keyCLIENT_PRIVATE_KEY_PEM[] =
    "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEIIQoZICeNWLWWhl25ioQC3kqnRtdc6R9cHeE5lhZVIYSoAoGCCqGSM49\n"
    "AwEHoUQDQgAEJdo9YKUJph491FSLiChn16JwBgEp57pyK23b5VloPH6/+QPFEYnU\n"
    "d+YdxKKMZp+Cym7cr8ZUysTv8lIww3w8zw==\n"
    "-----END EC PRIVATE KEY-----\n";

#endif

static const char *USED keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = "";

#endif // !GCP_CLIENT_CRED_H
