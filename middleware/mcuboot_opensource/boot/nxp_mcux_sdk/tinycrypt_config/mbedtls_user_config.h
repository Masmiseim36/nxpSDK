/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MBEDTLS_USER_CONFIG_H
#define MBEDTLS_USER_CONFIG_H

#ifndef CONFIG_MCXN_CUSTOM_CFG_MAIN_FLASH_ONLY
#undef MBEDTLS_FS_IO
#undef MBEDTLS_BIGNUM_C
#endif

#endif // MBEDTLS_USER_CONFIG_H
