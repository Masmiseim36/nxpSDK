/*
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MBEDTLS_CONFIG_FRDM_KW41Z_H
#define MBEDTLS_CONFIG_FRDM_KW41Z_H

#include "a71ch_mbedtls_frdm_config.h"

#undef MBEDTLS_MPI_WINDOW_SIZE
#undef MBEDTLS_ECP_WINDOW_SIZE
#undef MBEDTLS_MPI_MAX_SIZE
#undef MBEDTLS_ECP_MAX_BITS

#define MBEDTLS_MPI_WINDOW_SIZE 1
#define MBEDTLS_ECP_WINDOW_SIZE 2

#define MBEDTLS_MPI_MAX_SIZE 256 /* Maximum number of bytes for usable MPIs. */
#define MBEDTLS_ECP_MAX_BITS 256 /* Maximum bit size of groups */

#endif /* MBEDTLS_CONFIG_FRDM_KW41Z_H */
