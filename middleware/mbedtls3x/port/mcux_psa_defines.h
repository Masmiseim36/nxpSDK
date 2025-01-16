/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MCUX_PSA_DEFINES_H
#define MCUX_PSA_DEFINES_H

#if defined(PSA_CRYPTO_DRIVER_ELE_S4XX) 
#define PSA_CRYPTO_ELE_S4XX_LOCATION 			0x000001
#endif /* PSA_CRYPTO_DRIVER_ELE_S4XX */

#if defined(PSA_CRYPTO_DRIVER_ELS_PKC) 
#include "mcuxClPsaDriver_Oracle_Interface_key_locations.h"
#endif /* PSA_CRYPTO_DRIVER_ELS_PKC */

#endif /* MCUX_PSA_DEFINES_H */
