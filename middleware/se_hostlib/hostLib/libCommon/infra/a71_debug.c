/**
 * @file a71_debug.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2016
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
 * This file implements the matching debug API for the Debug Mode specific
 * APDU's of the A71CH module.
 * @par History
 *
 *****************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
    pApdu->ins   = A71CH_INS_ERASE_MODULE;
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
    pApdu->ins   = A71CH_INS_FREEZE_MODULE;
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

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CH_INS_GET_MODULE;
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

    assert(sndBuf != NULL);
    assert(rcvBuf != NULL);
    assert(rcvBufLen != NULL);

    pApdu->cla   = A71CH_CLA;
    pApdu->ins   = A71CH_INS_REFLECT;
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
