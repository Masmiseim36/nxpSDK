/**
 * @file global_platf.c
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
 * This file implements global platform commands.
 * @par History
 *
 *****************************************************************************/
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "global_platf.h"
#include "smCom.h"

#include "scp.h"
#include "sm_apdu.h"
#include "sm_errors.h"

#include "nxLog_hostLib.h"
#include "nxEnsure.h"

#ifndef SCP_MODE
#define	SCP_MODE NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC
#endif

/**
 * Send a select command to the card manager
 *
 * \param[in] appletName Pointer to a buffer containing the applet name.
 * \param[in] appletNameLen Length of the applet name.
 * \param[out] responseData Pointer to a buffer that will contain response data (excluding status word).
 * \param[in,out] responseDataLen IN: size of pResponse buffer passed as argument; OUT: Length of response data retrieved
 *
 * \retval ::SW_OK Upon successfull execution
 */
U16 GP_Select(U8 *appletName, U16 appletNameLen, U8 *responseData, U16 *responseDataLen)
{
    U16 rv = 0;
    apdu_t apdu = {0};
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk = 0x00;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((appletName == NULL) || (responseData == NULL) || (responseDataLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = CLA_ISO7816;
    pApdu->ins   = INS_GP_SELECT;
    pApdu->p1    = 0x04;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    if ( appletNameLen > 0)
    {
        smApduAppendCmdData(pApdu, appletName, appletNameLen);
    }
    rv = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, responseData, responseDataLen);
        }
    }


  FreeAPDUBuffer(pApdu);
  return rv;
}

