/**
 * @file ecp_alt_ax.c
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


#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/version.h"

#include <string.h>

#if defined(MBEDTLS_ECP_ALT)

#if (MBEDTLS_VERSION_MAJOR != 2) || (MBEDTLS_VERSION_MINOR  != 8 )
#   error "This port is only developed/validated against v2.8.0 of mbed TLS."
#endif

/*
 * Unallocate (the components of) a key pair
 */
void mbedtls_ecp_keypair_free( mbedtls_ecp_keypair *key )
{
    if( key == NULL )
        return;

    if ( key->grp.hlse_handle != 0 )
    {
        key->grp.hlse_handle = 0;
    }
    mbedtls_ecp_keypair_free_o(key);
}

/*
 * Set a group from an ECParameters record (RFC 4492)
 */
int mbedtls_ecp_tls_read_group( mbedtls_ecp_group *grp, const unsigned char **buf, size_t len )
{
    int ret;
    HLSE_OBJECT_HANDLE backup_type_ax_index;
    backup_type_ax_index = grp->hlse_handle;
    ret = mbedtls_ecp_tls_read_group_o(grp, buf, len);
    grp->hlse_handle = backup_type_ax_index;
    return ret;
}

#endif /* MBEDTLS_ECP_ALT */

#endif /* MBEDTLS_ECP_C */
