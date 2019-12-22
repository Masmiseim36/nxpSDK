/*
 * Copyright 2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/** @file
 *
 * ex_sss_boot_int.h:  *The purpose and scope of this file*
 *
 * Project:  SecureIoTMW-Debug@appboot-top-eclipse_x86
 *
 * $Date: Mar 10, 2019 $
 * $Author: ing05193 $
 * $Revision$
 */

#ifndef SSS_EX_SRC_EX_SSS_BOOT_INT_H_
#define SSS_EX_SRC_EX_SSS_BOOT_INT_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include <ex_sss_boot.h>

#include "fsl_sss_se05x_apis.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
#if SSS_HAVE_SE
sss_status_t ex_sss_boot_se_open(ex_sss_boot_ctx_t *pCtx, const char *portName);
#endif

/** Entry Point for SE050 based build */

#if SSS_HAVE_SE05X
sss_status_t ex_sss_boot_se05x_open(
    ex_sss_boot_ctx_t *pCtx, const char *portName);
#endif

#if SSS_HAVE_MBEDTLS
sss_status_t ex_sss_boot_mbedtls_open(
    ex_sss_boot_ctx_t *pCtx, const char *portName);
#endif

#if SSS_HAVE_OPENSSL
sss_status_t ex_sss_boot_openssl_open(
    ex_sss_boot_ctx_t *pCtx, const char *portName);
#endif

#if SSS_HAVE_A71CH || SSS_HAVE_SE050_EAR_CH
sss_status_t ex_sss_boot_a71ch_open(
    ex_sss_boot_ctx_t *pCtx, const char *portName);
#endif

#if SSS_HAVE_A71CL || SSS_HAVE_SE050_L
sss_status_t ex_sss_boot_a71cl_open(
    ex_sss_boot_ctx_t *pCtx, const char *portName);
#endif

#endif /* SSS_EX_SRC_EX_SSS_BOOT_INT_H_ */
