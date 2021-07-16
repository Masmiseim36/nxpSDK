/*
 *
 * Copyright 2016 NXP
 * SPDX-License-Identifier: Apache-2.0
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
