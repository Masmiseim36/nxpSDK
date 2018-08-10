/**
 * @file ax_mbedtls.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2018
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

/** @} */

#endif /* AX_MBEDTLS_H */
