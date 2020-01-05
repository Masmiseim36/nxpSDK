/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SSS_APIS_INC_FSL_SSS_FTR_H_
#define SSS_APIS_INC_FSL_SSS_FTR_H_

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

/* clang-format off */

#define SSS_HAVE_MBEDTLS         1
#define SSS_HAVE_OPENSSL         0

/* A71CH IC */
#define SSS_HAVE_A71CH           0

/* A71CL IC */
#define SSS_HAVE_A71CL           0

/* A71CH or A71CL IC */
#define SSS_HAVE_A71XX           (SSS_HAVE_A71CH + SSS_HAVE_A71CL)

/** Early access release Applet, CH/CL Compatible implementation */
#define SSS_HAVE_SE050_EAR_CH    0
#define SSS_HAVE_SE050_EAR_CL    0

#define SSS_HAVE_SE050_EAR       (SSS_HAVE_SE050_EAR_CH + SSS_HAVE_SE050_EAR_CL)

/** Next generation of SE050 Applet */
/* Successor of A71CH */
#define SSS_HAVE_SE050_A         0
/* Successor of A71CL */
#define SSS_HAVE_SE050_B         0
/* Super set of A and B */
#define SSS_HAVE_SE050_C         1

/* To be selected/modified by user */
#define SSS_HAVE_SE050_M_SR1     0
#define SSS_HAVE_SE050_M_SR2     0

/* Life Cycle States: User Release State
 * Only applicable with SE050M */
#define SSS_SE050M_LCS_URS       1

#define SSS_HAVE_SE05X           (SSS_HAVE_SE050_A + SSS_HAVE_SE050_B + SSS_HAVE_SE050_C)
#define SSS_HAVE_SE050M          (SSS_HAVE_SE050_M_SR1 + SSS_HAVE_SE050_M_SR2)


#define SSS_HAVE_LOOPBACK        0

#define SSS_HAVE_SSCP            (SSS_HAVE_A71XX + SSS_HAVE_SE050_EAR)

/* mbedTLS ALT implementations */
#define SSS_HAVE_ALT_A71CH       0
#define SSS_HAVE_ALT_SSS         0

#define SSS_HAVE_ALT             (SSS_HAVE_ALT_A71CH + SSS_HAVE_ALT_SSS)


#if (SSS_HAVE_A71XX + SSS_HAVE_SE05X + SSS_HAVE_SE050_EAR) >= 2
#   error Multiple SEs selected. Select only one Secure Element at a time.
#endif

/* Set to 1 for any SE */
#define SSS_HAVE_SE              (0 + SSS_HAVE_SE050M + SSS_HAVE_SE05X )

#define SSS_HAVE_SSS             (SSS_HAVE_SE + SSS_HAVE_MBEDTLS +  SSS_HAVE_OPENSSL + SSS_HAVE_SSCP + SSS_HAVE_SE05X)

/* Which cloud is being used/configured */

#define SSS_HAVE_GCP             0 /* Google Cloud Platform */
#define SSS_HAVE_IBM             0 /* IBM Watson */
#define SSS_HAVE_AWS             0 /* Amazon web services */
#define SSS_HAVE_AZURE           0 /* Azure web services */

/* ********************************* */
/* Individual features of sub-system */
/* ********************************* */

/* ECC Mode is availabe */
#define SSS_HAVE_ECC 1

/* RSA is available */
#define SSS_HAVE_RSA 1

/* Tests and examples are aware that FIPS mode
 * is enabled.
 */
#define SSS_HAVE_FIPS 0

/* TPM BARRETO_NAEHRIG Curve is enabled */
#define SSS_HAVE_TPM_BN 1

/* Edwards Curve is enabled */
#define SSS_HAVE_EC_ED 1

/* Montgomery Curve is enabled */
#define SSS_HAVE_EC_MONT 1

/* Enable usage of secure messaging channels */

#define SSS_HAVE_SCP_SCP03_SSS 1

/* With NXP NFC Reader Library */
#define SSS_HAVE_NXPNFCRDLIB 0

/* TLS handshake support on SE is enabled */
#define SSS_HAVE_TLS_HANDSHAKE 1

/* Import Export Key is enabled */
#define SSS_HAVE_IMPORT 1



/* # CMake Features : Start */

/* ====================================================================== *
 * == Feature selection/values ========================================== *
 * ====================================================================== */


/** SE05X Secure Element : Symmetric AES */
#define SSSFTR_SE05X_AES 1

/** SE05X Secure Element : Elliptic Curve Cryptography */
#define SSSFTR_SE05X_ECC 1

/** SE05X Secure Element : RSA */
#define SSSFTR_SE05X_RSA 1

/** SE05X Secure Element : KEY operations : SET Key */
#define SSSFTR_SE05X_KEY_SET 1

/** SE05X Secure Element : KEY operations : GET Key */
#define SSSFTR_SE05X_KEY_GET 1

/** SE05X Secure Element : Authenticate via FastSCP */
#define SSSFTR_SE05X_AuthFastSCP 1

/** SE05X Secure Element : Allow creation of user/authenticated session.
 *
 * If the intended deployment only uses Platform SCP
 * Or it is a pure session less integration, this can
 * save some code size. */
#define SSSFTR_SE05X_AuthSession 1

/** SE05X Secure Element : Allow creation/deletion of Crypto Objects
 *
 * If disabled, new Crytpo Objects are neither created and
 * old/existing Crypto Objects are not deleted.
 * It is assumed that during provisioning phase, the required
 * Crypto Objects are pre-created or they are never going to
 * be needed. */
#define SSSFTR_SE05X_CREATE_DELETE_CRYPTOOBJ 1

/** Software : Symmetric AES */
#define SSSFTR_SW_AES 1

/** Software : Elliptic Curve Cryptography */
#define SSSFTR_SW_ECC 1

/** Software : RSA */
#define SSSFTR_SW_RSA 1

/** Software : KEY operations : SET Key */
#define SSSFTR_SW_KEY_SET 1

/** Software : KEY operations : GET Key */
#define SSSFTR_SW_KEY_GET 1

/** Software : Used as a test counterpart
 *
 * e.g. Major part of the mebdTLS SSS layer is purely used for
 * testing of Secure Element implementation, and can be avoided
 * fully during many production scenarios. */
#define SSSFTR_SW_TESTCOUNTERPART 0

/* ====================================================================== *
 * == Computed Options ================================================== *
 * ====================================================================== */

/** Symmetric AES */
#define SSSFTR_AES               (SSSFTR_SE05X_AES + SSSFTR_SW_AES)
/** Elliptic Curve Cryptography */
#define SSSFTR_ECC               (SSSFTR_SE05X_ECC + SSSFTR_SW_ECC)
/** RSA */
#define SSSFTR_RSA               (SSSFTR_SE05X_RSA + SSSFTR_SW_RSA)
/** KEY operations : SET Key */
#define SSSFTR_KEY_SET           (SSSFTR_SE05X_KEY_SET + SSSFTR_SW_KEY_SET)
/** KEY operations : GET Key */
#define SSSFTR_KEY_GET           (SSSFTR_SE05X_KEY_GET + SSSFTR_SW_KEY_GET)
/** KEY operations */
#define SSSFTR_KEY               (SSSFTR_KEY_SET + SSSFTR_KEY_GET)
/** KEY operations */
#define SSSFTR_SE05X_KEY         (SSSFTR_SE05X_KEY_SET + SSSFTR_SE05X_KEY_GET)
/** KEY operations */
#define SSSFTR_SW_KEY            (SSSFTR_SW_KEY_SET + SSSFTR_SW_KEY_GET)

/* # CMake Features : END */

/* For backwards compatibility */
#define SSS_HAVE_TESTCOUNTERPART (SSSFTR_SW_TESTCOUNTERPART)

/* clang-format on */

#endif /* SSS_APIS_INC_FSL_SSS_FTR_H_ */
