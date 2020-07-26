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

#ifdef NXPBUILD__PH_NFCLIB_ISO_MFUL

#include "phNfcLib_Initialization.h"
#include "phNfcLib_Int.h"

#define PH_NFCLIB_MFUL_READ      0x30U
#define PH_NFCLIB_MFUL_COMPWRITE 0xA0U
#define PH_NFCLIB_MFUL_WRITE     0xA2U

#define PH_NFCLIB_MFUL_READ_BLOCK_LENGTH         16U    /**< Length of a read MIFARE(R) Ultralight contactless IC data block. */

phStatus_t phNfcLib_MFUL_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength)
{
    phStatus_t wStatus = PH_ERR_INVALID_PARAMETER;
    uint8_t    aCommand[6] = {0};

    switch(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareUltraLight.bCommand)
    {
    case MFUL_Read:
        /* build command frame */
        aCommand[0] = PH_NFCLIB_MFUL_READ;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareUltraLight.bPageNumber;

        /* Transmit the Command to recieve back the block data */
        wStatus = phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            aCommand,
            2,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);
        if ((wStatus & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR)
        {
            wStatus = PH_ERR_PROTOCOL_ERROR;
        }
        PH_CHECK_SUCCESS(wStatus);

        /* check received length */
        if (gphNfcLib_State.wRxLength != PH_NFCLIB_MFUL_READ_BLOCK_LENGTH)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        break;

    case MFUL_Write:
        /* build command frame */
        aCommand[0] = PH_NFCLIB_MFUL_WRITE;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareUltraLight.bPageNumber;
        (void)memcpy(&aCommand[2], ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareUltraLight.pBuffer, 4);

        /* Transmit the Command to write the block data */
        PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Mifare_Transmit(
            aCommand,
            6));
        break;

    case MFUL_Compatibility_Write:
        /* build command frame */
        aCommand[0] = PH_NFCLIB_MFUL_COMPWRITE;
        aCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareUltraLight.bPageNumber;

        /* Transmit the Command to write the block data */
        wStatus = phNfcLib_Mifare_Transmit(
            aCommand,
            2);

        /* Either ACK (newer UL cards) or TO (older UL cards) is expected */
        if ((wStatus & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
        {
            PH_CHECK_SUCCESS(wStatus);
        }

        /* Transmit the Command to write the block data */
        PH_CHECK_SUCCESS_FCT(wStatus, phNfcLib_Mifare_Transmit(
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_MifareUltraLight.pBuffer,
            16));
        break;

    default:
        break;
    }
    PH_UNUSED_VARIABLE(wTxBufferLength);
    return wStatus;
}

#endif /* NXPBUILD__PH_NFCLIB_ISO_MFUL*/
