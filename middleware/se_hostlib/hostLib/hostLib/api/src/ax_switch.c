/**
* @file ax_switch.c
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
#include <string.h>

#include "scp.h"
#include "ax_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"
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
