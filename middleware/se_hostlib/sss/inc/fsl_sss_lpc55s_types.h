/*
 *
 * Copyright 2018,2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SSS_APIS_INC_FSL_SSS_LPC55S_TYPES_H_
#define SSS_APIS_INC_FSL_SSS_LPC55S_TYPES_H_

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <fsl_sss_api.h>

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if defined(SECURE_WORLD)
#if SSS_HAVE_HOSTCRYPTO_MBEDTLS
#include <fsl_hashcrypt.h>

/**
 * @addtogroup sss_sw_host_impl
 * @{
 */

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

#define SSS_SUBSYSTEM_TYPE_IS_LPC55S(subsystem) (subsystem == kType_SSS_mbedTLS)

#define SSS_SESSION_TYPE_IS_LPC55S(session) (session && SSS_SUBSYSTEM_TYPE_IS_LPC55S(session->subsystem))

#define SSS_KEY_STORE_TYPE_IS_LPC55S(keyStore) (keyStore && SSS_SESSION_TYPE_IS_LPC55S(keyStore->session))

#define SSS_OBJECT_TYPE_IS_LPC55S(pObject) (pObject && SSS_KEY_STORE_TYPE_IS_LPC55S(pObject->keyStore))

#define SSS_SYMMETRIC_TYPE_IS_LPC55S(context) (context && SSS_SESSION_TYPE_IS_LPC55S(context->session))

#define SSS_RNG_CONTEXT_TYPE_IS_LPC55S(context) (context && SSS_SESSION_TYPE_IS_LPC55S(context->session))

/* ************************************************************************** */
/* Structrues and Typedefs                                                    */
/* ************************************************************************** */

//typedef struct _sss_lpc55s_impl_symmetric
//{
//    /*! Virtual connection between application (user context) and specific
//     * security subsystem and function thereof. */
//    sss_lpc55s_impl_session_t *session;
//    /*** Reference to key and it's properties. */
//    sss_lpc55s_impl_object_t *keyObject;
//    sss_algorithm_t algorithm;
//    sss_mode_t mode;
//
//    hashcrypt_handle_t *p_hashcrypt_handle;
//
//} sss_lpc55s_impl_symmetric_t;

typedef struct _sss_lpc55s_impl_mac
{
    sss_session_t *session;
    /*! Reference to key and it's properties. */
    sss_object_t *keyObject;
    sss_algorithm_t algorithm; /*!  */
    sss_mode_t mode;           /*!  */

    /*! Implementation specific part */
    hashcrypt_handle_t *p_hashcrypt_handle;
    uint8_t *keyCode;
    uint8_t hashCryptHandle;
    // size_t keyCodeSize;
} sss_lpc55s_impl_mac_t;

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Functions                                                                  */
/* ************************************************************************** */

/** @}  */

#endif /* SSS_HAVE_HOSTCRYPTO_MBEDTLS */
#endif /* SECURE_WORLD */

#endif /* SSS_APIS_INC_FSL_SSS_LPC55S_TYPES_H_ */
