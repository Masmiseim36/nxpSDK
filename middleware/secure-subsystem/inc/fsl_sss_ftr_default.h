/*
* Copyright 2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SSS_APIS_INC_FSL_SSS_FTR_DEFAULT_H_
#define SSS_APIS_INC_FSL_SSS_FTR_DEFAULT_H_

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

#define SSS_HAVE_MBEDTLS 1
#define SSS_HAVE_OPENSSL 0

#define SSS_HAVE_A71CH 0

/** Next generation of SE050 Applet compatible with SSCP Layer */
#define SSS_HAVE_SE050 0

/** Early access release Applet, CH + CL Compatible implementation */
#define SSS_HAVE_SE050_EAR 1
#define SSS_HAVE_SSCP (SSS_HAVE_A71CH + SSS_HAVE_SE050 + SSS_HAVE_SE050_EAR)

#if (SSS_HAVE_A71CH + SSS_HAVE_SE050 + SSS_HAVE_SE050_EAR) >= 2
#error Select only Secure Element
#endif

#define SSS_HAVE_SSS (SSS_HAVE_MBEDTLS + SSS_HAVE_OPENSSL + SSS_HAVE_SSCP)

#endif /* SSS_APIS_INC_FSL_SSS_FTR_DEFAULT_H_ */
