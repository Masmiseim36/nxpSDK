/**
 * @file a71_debug.c
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
 * This file implements the matching debug API for the Debug Mode specific
 * APDU's of the A71CH module.
 * @par History
 *
 *****************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "scp.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include "ax_api.h"
#include "ax_common.h"

#include "a71_debug.h"

/**
* Resets the Secure Module to the initial state.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_DbgReset(void)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71XX_INS_ERASE_MODULE;
    pApdu->p1    = P1_RESET;
    pApdu->p2    = P2_RESET;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC);

    // no response data expected
    if (rv == SMCOM_OK)
    {
        // No response data expected
        rv = CheckNoResponseData(pApdu);
    }
    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* Permanently disables the Debug API.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_DbgDisableDebug(void)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71XX_INS_FREEZE_MODULE;
    pApdu->p1    = P1_DISABLE_DEBUG;
    pApdu->p2    = P2_DISABLE_DEBUG;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    rv = (U16)scp_Transceive(pApdu, NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC);

    // no response data expected
    if (rv == SMCOM_OK)
    {
        // No response data expected
        rv = CheckNoResponseData(pApdu);
    }
    FreeAPDUBuffer(pApdu);
    return rv;
}

/// @cond
static U16 DBG_GetFreeMem(U8 type, S16 *freeMem)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 buf[256];
    U16 bufLen = sizeof(buf);
    U8 isOk;

#ifndef A71_IGNORE_PARAM_CHECK
    if (freeMem == NULL) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71XX_INS_GET_MODULE;
    pApdu->p1    = type;
    pApdu->p2    = P2_FREE_MEMORY;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
    *freeMem = 0;

    rv = (U16)scp_Transceive(pApdu, NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC);
    if (rv == SMCOM_OK)
    {
        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            rv = smApduGetResponseBody(pApdu, buf, &bufLen);
            if (rv == SW_OK)
            {
                if (bufLen == 2)
                {
                    *freeMem = (S16)((buf[0] << 8) + (buf[1] & 0x000000FF));
                }
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
/// @endcond

/**
* Reports the available persistent memory in the Security Module.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_DbgGetFreePersistentMemory(S16 *freeMem)
{
    return DBG_GetFreeMem(P1_EEPROM, freeMem);
}

/**
* Reports the available transient memory in the Security Module.
* @retval ::SW_OK Upon successful execution
*/
U16 A71_DbgGetFreeTransientMemory(S16 *freeMem)
{
    return DBG_GetFreeMem(P1_TRANSIENT_MEMORY, freeMem);
}

/**
* Invokes data reflection APDU (facilitates link testing). No check of data payload returned
* @param[in] sndBuf
* @param[in] sndBufLen
* @param[in,out] rcvBuf
* @param[in,out] rcvBufLen
* @return
*/
U16 A71_DbgReflect(U8 *sndBuf, U16 sndBufLen, U8 *rcvBuf, U16 *rcvBufLen)
{
    U16 err = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk = 0x00;

#ifndef A71_IGNORE_PARAM_CHECK
    if ((sndBuf == NULL) || (rcvBuf== NULL) || (rcvBufLen == NULL)) {
        return ERR_API_ERROR;
    }
#endif

    pApdu->cla   = A71XX_CLA;
    pApdu->ins   = A71XX_INS_REFLECT;
    pApdu->p1    = 0x00;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    smApduAppendCmdData(pApdu, sndBuf, sndBufLen);

    err = (U16)scp_Transceive(pApdu, SCP_MODE);
    if (err == SMCOM_OK)
    {
        err = smGetSw( pApdu, &isOk);

        if (isOk)
        {
            if ( (pApdu->rxlen-2) > *rcvBufLen )
            {
                *rcvBufLen = 0;
                return ERR_BUF_TOO_SMALL;
            }
            else
            {
                *rcvBufLen = pApdu->rxlen-2;
                memcpy(rcvBuf, pApdu->pBuf, *rcvBufLen);
            }
        }
    }

    return err;
}
