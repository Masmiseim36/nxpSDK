/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SSS_APIS_INC_FSL_SSS_FTR_DEFAULT_H_
#define SSS_APIS_INC_FSL_SSS_FTR_DEFAULT_H_

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

/* clang-format off */

#define SSS_HAVE_MBEDTLS     (1)
#define SSS_HAVE_OPENSSL     (0)

/* A71CH IC */
#define SSS_HAVE_A71CH       (0)

/* A71CL IC */
#define SSS_HAVE_A71CL       (0)

/* A71CH or A71CL IC */
#define SSS_HAVE_A71XX       (SSS_HAVE_A71CH + SSS_HAVE_A71CL)

/** Early access release Applet, CH/CL Compatible implementation */
#define SSS_HAVE_SE050_EAR_CH   (0)
#define SSS_HAVE_SE050_EAR_CL   (0)

#define SSS_HAVE_SE050_EAR   (SSS_HAVE_SE050_EAR_CH + SSS_HAVE_SE050_EAR_CL)

/** Next generation of SE050 Applet */
/* Successor of A71CH */
#define SSS_HAVE_SE050_A       (0)
/* Successor of A71CL */
#define SSS_HAVE_SE050_B       (0)
/* Super set of A and B */
#define SSS_HAVE_SE050_C       (1)

#define SSS_HAVE_SE05X (SSS_HAVE_SE050_A + SSS_HAVE_SE050_B + SSS_HAVE_SE050_C)

#define SSS_HAVE_LOOPBACK    (0)

#define SSS_HAVE_SSCP        (SSS_HAVE_A71XX + SSS_HAVE_SE050_EAR)

/* mbedTLS ALT implementations */
#define SSS_HAVE_ALT_A71CH      (0)
#define SSS_HAVE_ALT_SSS        (0)

#define SSS_HAVE_ALT         (SSS_HAVE_ALT_A71CH + SSS_HAVE_ALT_SSS)


#define SSS_HAVE_TESTCOUNTERPART (1)

#if (SSS_HAVE_A71XX + SSS_HAVE_SE05X + SSS_HAVE_SE050_EAR) >= 2
#   error Multiple SEs selected. Select only one Secure Element at a time.
#endif

#define SSS_HAVE_SSS         (SSS_HAVE_MBEDTLS +  SSS_HAVE_OPENSSL + SSS_HAVE_SSCP + SSS_HAVE_SE05X)

/* Which cloud is being used/configured */

#define SSS_HAVE_GCP (1)     /* Google Cloud Platform */
#define SSS_HAVE_IBM (0)     /* IBM Watson */
#define SSS_HAVE_AWS (0)     /* Amazon web services */
#define SSS_HAVE_AZURE (0) /* Azure web services */
#define SSS_HAVE_GGC (0) /* Greengrass core services */

/* ********************************* */
/* Individual features of sub-system */
/* ********************************* */

/* ECC Mode is availabe */
#define SSS_HAVE_ECC (1)

/* RSA is available */
#define SSS_HAVE_RSA (1)

/* FIPS Mode is enabled */
#define SSS_HAVE_FIPS (0)

/* TPM BARRETO_NAEHRIG Curve is enabled */
#define SSS_HAVE_TPM_BN (1)

/* Edwards Curve is enabled */
#define SSS_HAVE_EC_ED (1)

/* Montgomery Curve is enabled */
#define SSS_HAVE_EC_MONT (1)

/* Enable usage of secure messaging channels */

#define SSS_HAVE_SCP_SCP03_SSS (1)

/* With NXP NFC Reader Library */
#define SSS_HAVE_NXPNFCRDLIB (0)

/* TLS handshake support on SE is enabled */
#define SSS_HAVE_TLS_HANDSHAKE (1)

/* Import Export Key is enabled */
#define SSS_HAVE_IMPORT (1)

/* clang-format on */
#endif /* SSS_APIS_INC_FSL_SSS_FTR_DEFAULT_H_ */
