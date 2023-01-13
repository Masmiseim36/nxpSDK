/**
 * @file scp.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2016,2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * @par Description
 * This file defines A7-series specific types
 * @par History
 *
 * This file is reduced down to only have code needed for SE05X.
 * The original file is copied and renamed to scp_a7x.c
 *
 *
 */

#include <string.h>
#include "scp.h"
#include "smCom.h"
#include "sm_printf.h"
#include "sm_apdu.h"
#include <nxLog_scp.h>
#include "nxEnsure.h"

#if defined(SSS_USE_FTR_FILE)
#include "fsl_sss_ftr.h"
#else
#include "fsl_sss_ftr_default.h"
#endif

#if ! (SSS_HAVE_APPLET_A71CH)


U32 scp_Transceive(void *conn_ctx, apdu_t * pApdu, scp_CommandType_t type)
{
    U32 rv = ERR_COMM_ERROR;

    AX_UNUSED_ARG(type);
    if (pApdu->hasLe) {
        smApduAdaptLcLe(pApdu, pApdu->lc, pApdu->le);
    }
    else {
        smApduAdaptLcLe(pApdu, pApdu->lc, 0);
    }

    rv = smCom_Transceive(conn_ctx, pApdu);

    return rv;
}

#endif  // (SSS_HAVE_APPLET_SE05X_IOT) || (SSS_HAVE_APPLET_LOOPBACK)
