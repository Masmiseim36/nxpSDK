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

#ifndef TGT_A71CH
/**
 * Send a select command to the card manager
 * @deprecated Use ::GP_Select instead. Please note the semantics of ::GP_SelectApplet and ::GP_Select are not identical
 *
 * @param[in] pAppletName Pointer to the applet name.
 * @param[in] appletNameLength Length of the applet name.
 * @param[out] pResponse Pointer to the response (data and status word).
 * @param[in,out] pResponseLength IN: size of pResponse buffer passed as argument; OUT: Length of response retrieved
 *
 * \retval Return value reflects communication status
 */
U32 GP_SelectApplet(U8 * pAppletName, U8 appletNameLength, U8 * pResponse, U32 * pResponseLength)
{
    U32 st = 0;
    U8 txBuf[128];
    U8 len = appletNameLength;

    txBuf[0] = CLA_ISO7816;
    txBuf[1] = INS_GP_SELECT;
    txBuf[2] = 0x04;
    txBuf[3] = 0x00;
    txBuf[4] = len;
    memcpy(&txBuf[5], pAppletName, len);
    txBuf[5+ len] = 0x00;

    assert(pAppletName != NULL);

    st = smCom_TransceiveRaw(txBuf, 6+len, pResponse, pResponseLength);

    return st;
}
#endif // TGT_A71CH

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
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk = 0x00;

    assert(appletName != NULL);
    assert(responseData != NULL);

    pApdu->cla   = CLA_ISO7816;
    pApdu->ins   = INS_GP_SELECT;
    pApdu->p1    = 0x04;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, appletName, appletNameLen);

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
