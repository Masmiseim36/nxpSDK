/**
* @file sm_errors.c
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
* This file implements generic APDU response evaluation.
* @par History
* 1.0   20-feb-2012 : Initial version
*
*****************************************************************************/

#include <stddef.h>
#include <assert.h>

#include "scp.h"
#include "sm_apdu.h"
#include "sm_errors.h"

/**
* Returns the status word of a response APDU when no response data is expected.
* @param[in] pApdu Pointer to the APDU.
* @retval ::ERR_WRONG_RESPONSE In case the response contains more than only the status word
* @returns status word (in case the response is 2 byte long)
*/
U16 CheckNoResponseData(apdu_t * pApdu)
{
    U16 rv;
    assert(pApdu != NULL);

    if (pApdu->rxlen != 2)
    {
        // printf("%d: pApdu->rxlen = %d", __LINE__, pApdu->rxlen);
        rv = ERR_WRONG_RESPONSE;
    }
    else
    {
        // printBytestring("CheckNoResponseData", pApdu->pBuf, pApdu->rxlen);
        rv = smGetSw(pApdu, NULL);
    }

    return rv;
}

/**
* Returns the status word of a response APDU when no response data is expected.
* @param[in] rawResponse      Pointer to the raw response
* @param[in] rawResponseLen   Length of \p rawResponse
* @retval ::ERR_WRONG_RESPONSE In case the response contains more than only the status word
* @returns status word (in case the response is 2 byte long)
*/
U16 CheckNoResponseDataRaw(U8 *rawResponse, U16 rawResponseLen)
{
    U16 rv;
    assert(rawResponse != NULL);

    if (rawResponseLen != 2)
    {
        rv = ERR_WRONG_RESPONSE;
    }
    else
    {
        rv = (rawResponse[0] << 8) + rawResponse[1];
    }

    return rv;
}
