/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#include "mcux_psa_casper_key_generation_port.h"

#include "mbedtls/ecp.h"
#include "mbedtls/error.h"
#include "mbedtls/private_access.h"
#include "mbedtls/psa_util.h"

#include <psa/crypto.h>
#include "psa_crypto_random_impl.h"
#include "psa_crypto_core.h"

#include "mcux_psa_casper_ecp_port.h"

/*****************************************************************************
 * ecp.c
 *****************************************************************************/

/*
 * Generate a keypair with configurable base point
 */
static int casper_mbedtls_ecp_gen_keypair_base(mbedtls_ecp_group *grp,
                                               const mbedtls_ecp_point *G,
                                               mbedtls_mpi *d,
                                               mbedtls_ecp_point *Q,
                                               int (*f_rng)(void *, unsigned char *, size_t),
                                               void *p_rng)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK(mbedtls_ecp_gen_privkey(grp, d, f_rng, p_rng));
    MBEDTLS_MPI_CHK(casper_mbedtls_ecp_mul(grp, Q, d, G, f_rng, p_rng));

cleanup:
    return ret;
}

/*
 * Generate key pair, wrapper for conventional base point
 */
static int casper_mbedtls_ecp_gen_keypair(mbedtls_ecp_group *grp,
                                          mbedtls_mpi *d,
                                          mbedtls_ecp_point *Q,
                                          int (*f_rng)(void *, unsigned char *, size_t),
                                          void *p_rng)
{
    return casper_mbedtls_ecp_gen_keypair_base(grp, &grp->G, d, Q, f_rng, p_rng);
}

/*
 * Generate a keypair, prettier wrapper
 */
static int casper_mbedtls_ecp_gen_key(mbedtls_ecp_group_id grp_id,
                                      mbedtls_ecp_keypair *key,
                                      int (*f_rng)(void *, unsigned char *, size_t),
                                      void *p_rng)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    if ((ret = mbedtls_ecp_group_load(&key->MBEDTLS_PRIVATE(grp), grp_id)) != 0) {
        return ret;
    }

    return casper_mbedtls_ecp_gen_keypair(&key->MBEDTLS_PRIVATE(grp), &key->MBEDTLS_PRIVATE(d),
                                          &key->MBEDTLS_PRIVATE(Q), f_rng, p_rng);
}

/*****************************************************************************
 * psa_crypto_ecp.c
 *****************************************************************************/

psa_status_t casper_mbedtls_psa_ecp_generate_key(const psa_key_attributes_t *attributes,
                                                 uint8_t *key_buffer,
                                                 size_t key_buffer_size,
                                                 size_t *key_buffer_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    int ret             = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    psa_ecc_family_t curve      = PSA_KEY_TYPE_ECC_GET_FAMILY(psa_get_key_type(attributes));
    mbedtls_ecp_group_id grp_id = mbedtls_ecc_group_from_psa(curve, psa_get_key_bits(attributes));

    const mbedtls_ecp_curve_info *curve_info = mbedtls_ecp_curve_info_from_grp_id(grp_id);
    mbedtls_ecp_keypair ecp;

    if (grp_id == MBEDTLS_ECP_DP_NONE || curve_info == NULL) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    mbedtls_ecp_keypair_init(&ecp);
    ret =
        casper_mbedtls_ecp_gen_key(grp_id, &ecp, mbedtls_psa_get_random, MBEDTLS_PSA_RANDOM_STATE);
    if (ret != 0) {
        mbedtls_ecp_keypair_free(&ecp);
        return mbedtls_to_psa_error(ret);
    }

    status =
        mbedtls_to_psa_error(mbedtls_ecp_write_key_ext(&ecp, key_buffer_length, key_buffer,
                                                       key_buffer_size));

    mbedtls_ecp_keypair_free(&ecp);

    return status;
}
