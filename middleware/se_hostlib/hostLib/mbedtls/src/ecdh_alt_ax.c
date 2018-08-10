/**
 * @file ecdh_alt_ax.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) 2017-2018, NXP. All rights reserved.
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
 *
 *****************************************************************************/

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif


#if defined(MBEDTLS_ECDH_C) && defined(MBEDTLS_ECDH_ALT)

#include <string.h>
#include <stdio.h>

#include "mbedtls/ecdh.h"
#include "mbedtls/version.h"
#include "a71ch_api.h"
#include "HLSEAPI.h"
#if defined(FLOW_VERBOSE) && FLOW_VERBOSE == 1
#   include "sm_printf.h"
#endif /* FLOW_VERBOSE */

#if (MBEDTLS_VERSION_MAJOR != 2) || (MBEDTLS_VERSION_MINOR  != 8 )
#   error "This port is only developed/validated against v2.8.0 of mbed TLS."
#endif

extern int mbedtls_ecdh_gen_public_o( mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q,
                             int (*f_rng)(void *, unsigned char *, size_t),
                             void *p_rng );
extern int mbedtls_ecdh_compute_shared_o( mbedtls_ecp_group *grp, mbedtls_mpi *z,
                                 const mbedtls_ecp_point *Q, const mbedtls_mpi *d,
                                 int (*f_rng)(void *, unsigned char *, size_t),
                                 void *p_rng );
extern int mbedtls_ecdh_get_params_o( mbedtls_ecdh_context *ctx, const mbedtls_ecp_keypair *key,
                             mbedtls_ecdh_side side );

/*
 * Generate public key: simple wrapper around mbedtls_ecp_gen_keypair
 */
int mbedtls_ecdh_gen_public( mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q,
                             int (*f_rng)(void *, unsigned char *, size_t),
                             void *p_rng )
{
    if (grp->hlse_handle == 0)
    {
        return mbedtls_ecdh_gen_public_o(grp, d, Q, f_rng, p_rng);
    }
    else if (HLSE_GET_OBJECT_TYPE(grp->hlse_handle) == HLSE_KEY_PAIR)
    {
        U8 publickey[65];
        U16 publickeylen = sizeof(publickey);

        mbedtls_mpi_free(d);
        if (SW_OK == A71_GetPublicKeyEccKeyPair(
                HLSE_GET_OBJECT_INDEX(grp->hlse_handle), publickey, &publickeylen))
        {
            return mbedtls_ecp_point_read_binary(grp,Q,publickey,publickeylen);
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
}

/*
 * Compute shared secret (SEC1 3.3.1)
 */
int mbedtls_ecdh_compute_shared( mbedtls_ecp_group *grp, mbedtls_mpi *z,
                                 const mbedtls_ecp_point *Q, const mbedtls_mpi *d,
                                 int (*f_rng)(void *, unsigned char *, size_t),
                                 void *p_rng )
{
    int ret = 1; /* Fail by default */
    if (grp->hlse_handle == 0)
    {
        ret = mbedtls_ecdh_compute_shared_o(grp, z, Q, d, f_rng, p_rng);
    }
    else if (HLSE_GET_OBJECT_TYPE(grp->hlse_handle) == HLSE_KEY_PAIR)
    {
        U8 OtherPublicKey[65];
        size_t keylen =  sizeof(OtherPublicKey);
        U8 SharedSecret[32];
        U16 SharedSecretlen = sizeof(SharedSecret);

#if defined(FLOW_VERBOSE) && FLOW_VERBOSE == 1
        sm_printf(DBGOUT, "Using ECC key '%d' to compute shared secret.\r\n",HLSE_GET_OBJECT_INDEX(grp->hlse_handle));
#endif /* FLOW_VERBOSE */
        if(0 == mbedtls_ecp_point_write_binary(grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED, &keylen, OtherPublicKey, sizeof(OtherPublicKey)))
        {
            if(SW_OK == A71_EcdhGetSharedSecret(
                HLSE_GET_OBJECT_INDEX(grp->hlse_handle), OtherPublicKey, keylen, SharedSecret, &SharedSecretlen))
            {
                ret = mbedtls_mpi_read_binary(z, SharedSecret,SharedSecretlen);
            }
        }
    }
    else
    {
        ret = 1;
    }
    return( ret );
}

/*
 * Get parameters from a keypair
 */
int mbedtls_ecdh_get_params( mbedtls_ecdh_context *ctx, const mbedtls_ecp_keypair *key,
                             mbedtls_ecdh_side side )
{
    int ret;
    HLSE_OBJECT_HANDLE backup_type_ax_index = ctx->grp.hlse_handle;
    ret = mbedtls_ecdh_get_params_o(ctx, key, side);
    ctx->grp.hlse_handle = backup_type_ax_index;

    return( ret );
}

#endif /* defined(MBEDTLS_ECDH_C) && defined(MBEDTLS_ECDH_ALT) */
