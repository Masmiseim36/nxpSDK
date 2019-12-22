/**
 * @file scp.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2016 NXP
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

#if (SSS_HAVE_SE05X) || (SSS_HAVE_LOOPBACK)


U32 scp_Transceive(apdu_t * pApdu, scp_CommandType_t type)
{
    U32 rv = ERR_COMM_ERROR;

    if (pApdu->hasLe)
        smApduAdaptLcLe(pApdu, pApdu->lc, pApdu->le);
    else
        smApduAdaptLcLe(pApdu, pApdu->lc, 0);

    rv = smCom_Transceive(pApdu);

    return rv;
}

#endif  // (SSS_HAVE_SE05X) || (SSS_HAVE_LOOPBACK)

