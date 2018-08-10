/**
* @file ax_switch.c
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
#include <string.h>
#include <assert.h>

#include "scp.h"
#include "ax_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
#include "sm_printf.h"
#include "ax_common.h"

#ifdef INS_AX_SWITCH

static U16 SWITCH_Generic(U8 targetState)
{
    U16 rv = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SWITCH;
    pApdu->p1    = targetState;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, 0);

    scp_Transceive(pApdu, C_MAC_C_ENC_R_MAC_R_ENC);

    // no response data expected
    rv = CheckNoResponseData(pApdu);
    FreeAPDUBuffer(pApdu);
    return rv;
}

/**
* @description Releases the security module from its initial Locked state.
* It brings the Security Module into the ConfigureInitial state.
* @return status
*/
U16 SWITCH_Unlock(void)
{
    return SWITCH_Generic(P1_UNLOCK);
}

/**
* @function SWITCH_Configure
* @description Switches the security module from its ConfigureInitial state to the Configure state.
* @return status
*/
U16 SWITCH_Configure(void)
{
    return SWITCH_Generic(P1_CONFIGURE);
}

/**
* @function SWITCH_Operate
* @description Switches the security module from the Configure state, the Diagnostic state
*    or from the Deactivated state to the Operate state.
* @return status
*/
U16 SWITCH_Operate()
{
    return SWITCH_Generic(P1_OPERATE);
}

#ifndef AX_SF
U16 SWITCH_Deactivate(void)
{
    return SWITCH_Generic(P1_DEACTIVATE);
}

U16 SWITCH_Diagnose(void)
{
    return SWITCH_Generic(P1_DIAGNOSE);
}
#endif // AX_SF

U16 SWITCH_Dead(void)
{
    return SWITCH_Generic(P1_DEAD);
}

U16 SWITCH_GetState(U8 *state)
{
    U16 err = 0;
    U8 buf[256];
    U16 bufLen = sizeof(buf);
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 retval;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SWITCH;
    pApdu->p1    = P1_GETSTATE;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, 0);

    scp_Transceive(pApdu, C_MAC_C_ENC_R_MAC_R_ENC);

    retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, &bufLen, buf);
    if ( (retval != SW_OK) || (bufLen != 1) )
    {
        err = ERR_WRONG_RESPONSE;
    }
    else
    {
        *state = buf[0];
        err = SW_OK;
    }

    FreeAPDUBuffer(pApdu);
    return err;
}

#endif /* INS_AX_SWITCH */
