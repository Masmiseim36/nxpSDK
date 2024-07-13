/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#ifndef _IOT_AGENT_CLAIMCODE_ENCRYPT_H_
#define _IOT_AGENT_CLAIMCODE_ENCRYPT_H_

#if defined(SSS_USE_FTR_FILE)
#include <fsl_sss_ftr.h>
#else
#include <fsl_sss_ftr_default.h>
#endif

#include "nxp_iot_agent_status.h"
#include "stdint.h"
#include "stddef.h"

#if SSS_HAVE_MBEDTLS_ALT_PSA

#ifdef __cplusplus
extern "C" {
#endif

/*! @defgroup edgelock2go_agent_claimcode_encrypt Device-specifically encrypt a claimcode 
 * for EdgeLock 2GO.
*
* @ingroup edgelock2go_agent_claimcode_encrypt
*
* @brief Provides functionality to encrypt claimcode and store as binary object in trusted storage.
*
*/

/*!
* @addtogroup edgelock2go_agent_claimcode_encrypt
* @{
*/

/**
 * @brief Encrypt a claimcode.
 */
iot_agent_status_t iot_agent_claimcode_encrypt(const char * claimcode,
        const uint8_t* el2go_public_key, size_t el2go_public_key_size, 
        uint8_t* claimcode_blob, size_t* claimcode_blob_size);

/**
 * @brief Encrypt a claimcode and put into trusted storage.
 * @param[in] claimcode: Pointer to null-terminated string containing claimcode.
 * @return Success if storage of new claimcode is successful.
 */
iot_agent_status_t iot_agent_claimcode_encrypt_and_import(char *claimcode, 
        const uint8_t* el2go_public_key, size_t el2go_public_key_size);


#ifdef __cplusplus
} // extern "C"
#endif

/*!
*@}
*/ /* end of edgelock2go_agent_claimcode_encrypt */

#endif // SSS_HAVE_MBEDTLS_ALT_PSA

#endif // #ifndef _IOT_AGENT_CLAIMCODE_ENCRYPT_H_
