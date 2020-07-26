/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* NFC Library Top Level API of Reader Library Framework.
* $Author: jenkins_ cm (nxp92197) $
* $Revision: 4184 $ (v06.01.00)
* $Date: 2016-05-20 18:04:59 +0530 (Fri, 20 May 2016) $
*
* History:
*  KK: Generated 20. May 2016
*/

#include <ph_Status.h>

#if defined(NXPBUILD__PH_NFCLIB_ISO_MFC) || defined(NXPBUILD__PH_NFCLIB_ISO_MFUL)

#include "phNfcLib_Initialization.h"
#include "phNfcLib_Int.h"

phStatus_t phNfcLib_Mifare_Transmit(uint8_t * pTxBuffer, uint16_t wTxLength)
{
    phStatus_t wStatus = PH_ERR_INVALID_PARAMETER;
    uint16_t wValidBits = 0;

    /* Disable RxCrc */
    (void) phhalHw_SetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF);

    wStatus = phhalHw_Exchange(
        gphNfcLib_Params.sDiscLoop.pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        pTxBuffer,
        wTxLength,
        &gphNfcLib_State.pRxBuffer,
        &gphNfcLib_State.wRxLength);

    /* ACK/NAK Handling */
    if ((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        /* Retrieve bitcount */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_GetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_RXLASTBITS, &wValidBits));

        /* Check for protocol error and check for ack*/
        if ((wValidBits != 4U) || (gphNfcLib_State.wRxLength != 1U) || ((*&gphNfcLib_State.pRxBuffer)[0] != 0x0AU))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        gphNfcLib_State.wRxLength--;
    }

    return wStatus;
}
#endif
