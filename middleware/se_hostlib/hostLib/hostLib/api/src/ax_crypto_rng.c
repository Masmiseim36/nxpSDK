/**
 * @file ax_crypto_rng.c
 * @author NXP Semiconductors
 * @version 1.0
 * @section LICENSE
 * ----------------------------------------------------------------------------
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 * ----------------------------------------------------------------------------
 * @section DESCRIPTION
 *
 * ----------------------------------------------------------------------------
 * @section HISTORY
 * 1.0   20-feb-2012 : Initial version
 *
 *****************************************************************************/
#include <stdio.h>
#include <nxEnsure.h>

#include "scp.h"
#include "ax_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"

/**
 * @function RND_GetRandom
 * @description Generates a random bytestring.
 * @param pRandom Pointer to the generated random bytestring.
 * @param randomLength Requested length of the requested random bytestring.
 * @return
 */

#if defined(TGT_A71CL)
U16 RND_GetRandom(U8 * pRandom, U16 randomLength)
{
    U8 isOk = 0;
    U16 rv;
    U8 tmp[2];

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    ENSURE_OR_GO_EXIT(pRandom != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = A71CL_INS_MODULE_GET_RANDOM;
    pApdu->p1    = P1_GENERATE_RANDOM;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

    tmp[0] = randomLength >> 8;
    tmp[1] = randomLength & 0xff;
    AddTlvItem(pApdu, TAG_SIZE, 2, tmp);

    scp_Transceive(pApdu, SCP_MODE);

    rv = smGetSw(pApdu, &isOk);
    if (isOk)
    {
        U16 length = randomLength;
        U16 retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, &length, pRandom);

        if ((retval != SW_OK) || (length != randomLength))
        {
            rv = ERR_WRONG_RESPONSE;
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
#endif
