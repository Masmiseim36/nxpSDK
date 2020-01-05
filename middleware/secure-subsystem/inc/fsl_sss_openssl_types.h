/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SSS_APIS_INC_FSL_SSS_OPENSSL_TYPES_H_
#define SSS_APIS_INC_FSL_SSS_OPENSSL_TYPES_H_

/* ************************************************************************** */
/* Includes                                                                   */
/* ************************************************************************** */

#include <fsl_sss_api.h>
#if !defined(SSS_FTR_FILE)
#include "fsl_sss_ftr_default.h"
#else
#include SSS_FTR_FILE
#endif

/**
 * @addtogroup sss_sw_openssl
 * @{
 */

/* ************************************************************************** */
/* Defines                                                                    */
/* ************************************************************************** */

#define SSS_SUBSYSTEM_TYPE_IS_OPENSSL(subsystem) (subsystem == kType_SSS_OpenSSL)

#define SSS_SESSION_TYPE_IS_OPENSSL(session) (SSS_SUBSYSTEM_TYPE_IS_OPENSSL(session->subsystem))

#define SSS_OBJECT_TYPE_IS_OPENSSL(pObject) SSS_SUBSYSTEM_TYPE_IS_OPENSSL(pObject->keyStore->session->subsystem)

#define SSS_KEY_STORE_TYPE_IS_OPENSSL(keyStore) SSS_SUBSYSTEM_TYPE_IS_OPENSSL(keyStore->session->subsystem)

/* ************************************************************************** */
/* Structrues and Typedefs                                                    */
/* ************************************************************************** */

typedef struct _sss_openssl_session
{
    /*! Indicates which security subsystem is selected to be used. */
    sss_type_t subsystem;

} sss_openssl_session_t;

typedef struct _sss_openssl_key_store
{
    sss_openssl_session_t *session;
    /*! Implementation specific part */
} sss_openssl_key_store_t;

typedef struct _sss_openssl_object
{
    SSS_OBJECT_MEMBERS(_openssl_);

    /*! Implementation specific part */
} sss_openssl_object_t;

typedef struct _sss_openssl_asymmetric
{
    SSS_ASYMMETRIC_MEMBERS(_openssl_)

} sss_openssl_asymmetric_t;

/* ************************************************************************** */
/* Global Variables                                                           */
/* ************************************************************************** */

/* ************************************************************************** */
/* Functions                                                                  */
/* ************************************************************************** */

/** @} */

#endif /* SSS_APIS_INC_FSL_SSS_OPENSSL_TYPES_H_ */
