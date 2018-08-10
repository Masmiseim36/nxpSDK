/**
 * @file ax_crypto_rng.c
 * @author NXP Semiconductors
 * @version 1.0
 * @section LICENSE
 * ----------------------------------------------------------------------------
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
 * ----------------------------------------------------------------------------
 * @section DESCRIPTION
 *
 * ----------------------------------------------------------------------------
 * @section HISTORY
 * 1.0   20-feb-2012 : Initial version
 *
 *****************************************************************************/
#include <stdio.h>
#include <assert.h>

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
#if !defined(TGT_A70CM) && !defined(TGT_A71CH)
U16 RND_GetRandom(U8 * pRandom, U16 randomLength)
{
    U8 isOk = 0;
    U16 rv;
    U8 tmp[2];

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    assert(pRandom != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_RND_GET_RANDOM;
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
    return rv;
}
#endif // TGT_A70CM
