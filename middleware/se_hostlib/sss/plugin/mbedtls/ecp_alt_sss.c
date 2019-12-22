/**
 * @file ecp_alt_ax.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017-2019 NXP
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
 * Implementation of key association between NXP Secure Element and mbedtls.
 *
 *****************************************************************************/

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECP_C)

#include <string.h>

#include "mbedtls/ecp.h"
#include "mbedtls/version.h"

#if defined(MBEDTLS_ECP_ALT) && SSS_HAVE_ALT_SSS

#include <sss_mbedtls.h>

/*
 * Unallocate (the components of) a key pair
 */
void mbedtls_ecp_keypair_free_o(mbedtls_ecp_keypair *key);

void mbedtls_ecp_keypair_free(mbedtls_ecp_keypair *key)
{
    if (key == NULL)
        return;

    // if ( key->grp.hlse_handle != 0 )
    // {
    //     key->grp.hlse_handle = 0;
    // }
    mbedtls_ecp_keypair_free_o(key);
}

/*
 * Set a group from an ECParameters record (RFC 4492)
 */
int mbedtls_ecp_tls_read_group(
    mbedtls_ecp_group *grp, const unsigned char **buf, size_t len)
{
    int ret;
    sss_object_t *backup_type_SSS_Object = grp->pSSSObject;
    sss_key_store_t *backup_type_hostKs = grp->hostKs;
    ret = mbedtls_ecp_tls_read_group_o(grp, buf, len);
    grp->pSSSObject = backup_type_SSS_Object;
    grp->hostKs = backup_type_hostKs;

    return ret;
}

#endif /* MBEDTLS_ECP_ALT */

#endif /* MBEDTLS_ECP_C */
