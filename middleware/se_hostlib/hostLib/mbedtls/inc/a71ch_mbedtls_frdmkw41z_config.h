/*
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
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
