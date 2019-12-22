/**
 * @file ax_mbedtls.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Implementation of key association between A71CH and mbedtls.
 * @par History
 * 1.0   30-jan-2018 : Initial version
 *
 *****************************************************************************/

#ifndef AX_MBEDTLS_H
#define AX_MBEDTLS_H

/** @ingroup ax_mbed_tls */
/** @{ */

#include "ax_api.h"
#include "mbedtls/pk.h"
#include "mbedtls/ssl.h"

/**
 * @brief      Associate a keypair provisioned in the secure element for
 *             subsequent operations.
 *
 * @param[in]  key_index  Index in which the keypair is provisoned in the SE
 * @param[out] pkey       Pointer to the mbedtls_pk_context which will be
 *                        associated with data corresponding to the key_index
 *
 * @return     0 if successful, or 1 if unsuccessful
 */
int ax_mbedtls_associate_keypair(SST_Index_t key_index, mbedtls_pk_context * pkey);

/**
 * @brief      Associate a pubkey provisioned in the secure element for
 *             subsequent operations.
 *
 * @param[in]  key_index  Index in which the pub key is provisioned in the SE
 * @param[out] pkey       Pointer to the mbedtls_pk_context which will be
 *                        associated with data corresponding to the key index
 *
 * @return     0 if successful, or 1 if unsuccessful
 */
int ax_mbedtls_associate_pubkey(SST_Index_t key_index, mbedtls_pk_context * pkey);

/**
 * @brief         Update ECDSA HandShake key with given inded.
 *
 * @param[in]     key_index  Index in which the pub key is provisioned in the SE
 * @param[in,out] handshake  Pointer to the mbedtls_ssl_handshake_params which
 *                           will be associated with data corresponding to the
 *                           key index
 *
 * @return        0 if successful, or 1 if unsuccessful
 */

int ax_mbedtls_associate_ecdhctx(SST_Index_t key_index, mbedtls_ssl_handshake_params * handshake);

/**
 * \brief           This function frees the components of a key pair. Original implementation
 * \param key       The key pair to free.
 */
void mbedtls_ecp_keypair_free_o( mbedtls_ecp_keypair *key );

/**
 * same as @ref mbedtls_ecp_tls_read_group
 */
int mbedtls_ecp_tls_read_group_o( mbedtls_ecp_group *grp, const unsigned char **buf, size_t len );

/** @} */

#endif /* AX_MBEDTLS_H */
