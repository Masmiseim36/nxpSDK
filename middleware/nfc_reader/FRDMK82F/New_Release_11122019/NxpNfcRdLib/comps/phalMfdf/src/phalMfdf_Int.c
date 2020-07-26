/*
*                     Copyright 2010-2019, NXP
*
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
* Internal functions of Software implementation of MIFARE DESFire EV1 contactless IC
* Application layer.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PHAL_MFDF

#include <string.h>
#include <phpalMifare.h>
#include <phalMfdf.h>
#include "phalMfdf_Int.h"
#include <ph_RefDefs.h>

phStatus_t phalMfdf_Int_ComputeErrorResponse(
    void * pDataParams,
    uint16_t wStatus
    )
{
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusTmp;

    switch (wStatus)
    {
    case PHAL_MFDF_RESP_OPERATION_OK:
    case PHAL_MFDF_ISO7816_SUCCESS:
        status = PH_ERR_SUCCESS;
        break;

    case PHAL_MFDF_RESP_NO_CHANGES:
        status = PHAL_MFDF_NO_CHANGES;
        break;

    case PHAL_MFDF_RESP_ERR_OUT_OF_EEPROM_ERROR:
        status = PHAL_MFDF_ERR_OUT_OF_EEPROM_ERROR;
        break;


    case PHAL_MFDF_RESP_ERR_PARAMETER_ERROR:
        status = PHAL_MFDF_ERR_PARAMETER_ERROR;
        break;


    case PHAL_MFDF_RESP_ERR_LENGTH_ERROR:
        status = PH_ERR_LENGTH_ERROR;
        break;

    case PHAL_MFDF_RESP_PERMISSION_DENIED:
        status = PHAL_MFDF_ERR_PERMISSION_DENIED;
        break;

    case PHAL_MFDF_RESP_APPLICATION_NOT_FOUND:
        status = PHAL_MFDF_ERR_APPLICATION_NOT_FOUND;
        break;


    case PHAL_MFDF_RESP_ADDITIONAL_FRAME:
        status = PH_ERR_SUCCESS_CHAINING;
        break;

    case PHAL_MFDF_RESP_ERR_BOUNDARY_ERROR:
        status = PHAL_MFDF_ERR_BOUNDARY_ERROR;
        break;

    case PHAL_MFDF_RESP_ERR_COMMAND_ABORTED:
        status = PHAL_MFDF_ERR_COMMAND_ABORTED;
        break;

    case PHAL_MFDF_RESP_ERR_COUNT:
        status = PHAL_MFDF_ERR_COUNT;
        break;

    case PHAL_MFDF_RESP_ERR_DUPLICATE:
        status = PHAL_MFDF_ERR_DUPLICATE;
        break;

    case PHAL_MFDF_RESP_ERR_FILE_NOT_FOUND:
        status = PHAL_MFDF_ERR_FILE_NOT_FOUND;
        break;

    case PHAL_MFDF_RESP_ERR_APPL_INTEGRITY_ERROR:
    case PHAL_MFDF_RESP_ERR_PICC_INTEGRITY:
    case PHAL_MFDF_RESP_ERR_EEPROM:
    case PHAL_MFDF_RESP_ERR_FILE_INTEGRITY:
    case PHAL_MFDF_RESP_ERR_PIC_DISABLED:
    case PHAL_MFDF_RESP_ILLEGAL_COMMAND_CODE:
        status = PHAL_MFDF_ERR_DF_GEN_ERROR;
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SetConfig(
            pDataParams,
            PHAL_MFDF_ADDITIONAL_INFO,
            wStatus));
        break;

    case PHAL_MFDF_ISO7816_ERR_WRONG_LENGTH:
    case PHAL_MFDF_ISO7816_ERR_WRONG_LE:
    case PHAL_MFDF_ISO7816_ERR_FILE_NOT_FOUND:
    case PHAL_MFDF_ISO7816_ERR_WRONG_PARAMS:
    case PHAL_MFDF_ISO7816_ERR_WRONG_LC:
    case PHAL_MFDF_ISO7816_ERR_NO_PRECISE_DIAGNOSTICS:
    case PHAL_MFDF_ISO7816_ERR_EOF_REACHED:
    case PHAL_MFDF_ISO7816_ERR_FILE_ACCESS:
    case PHAL_MFDF_ISO7816_ERR_FILE_EMPTY:
    case PHAL_MFDF_ISO7816_ERR_MEMORY_FAILURE:
    case PHAL_MFDF_ISO7816_ERR_INCORRECT_PARAMS:
    case PHAL_MFDF_ISO7816_ERR_WRONG_CLA:
    case PHAL_MFDF_ISO7816_ERR_UNSUPPORTED_INS:
        status = PHAL_MFDF_ERR_DF_7816_GEN_ERROR;
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SetConfig(
            pDataParams,
            PHAL_MFDF_ADDITIONAL_INFO,
            wStatus));
        break;

    default:
        status = PH_ERR_PROTOCOL_ERROR;
    }
    return PH_ADD_COMPCODE(status, PH_COMP_AL_MFDF);
}

/*
*
* Should take care of wrapping and unwrapping if ISO 7816 Wrapped mode
* Does not handle any chaining or CMAC generation/verification or encryption/decryption
*/
phStatus_t phalMfdf_ExchangeCmd(
                                void * pDataParams,
                                void * pPalMifareDataParams,
                                uint8_t bWrappedMode,
                                uint8_t * pSendBuff,
                                uint16_t wCmdLen,
                                uint8_t ** ppResponse,
                                uint16_t * pRxlen
                                )
{
    uint16_t    PH_MEMLOC_REM wFrameLen = 0;
    uint8_t     PH_MEMLOC_REM * pRecv = NULL;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bStatusByte;
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFDF_WRAPPEDAPDU_CLA, 0x00, PHAL_MFDF_WRAPPEDAPDU_P1, PHAL_MFDF_WRAPPEDAPDU_P2, 0x00 };

    if (0U != (bWrappedMode))
    {
        wFrameLen = PHAL_MFDF_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        wFrameLen = PHAL_MFDF_MAXDFAPDU_SIZE;
    }

    if (wCmdLen > wFrameLen)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFDF);
    }

    /* Send the data to PICC */
    if (0U != (bWrappedMode))
    {
        pApdu[1] = pSendBuff[0]; /* MIFARE DESFire contactless IC cmd code in INS */
        pApdu[4] = (uint8_t)(wCmdLen) - 0x01u;

        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pPalMifareDataParams,
            (pApdu[4] == 0x00U) ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            PHAL_MFDF_WRAP_HDR_LEN,
            &pRecv,
            pRxlen));

        if (pApdu[4] != 0x00U)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                &pSendBuff[1],
                wCmdLen - 1u,
                &pRecv,
                pRxlen));

            /* Le byte */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                &pApdu[2], /* Le is always zero in wrapped mode. */
                0x01,
                &pRecv,
                pRxlen));
        }
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            pSendBuff,
            wCmdLen,
            &pRecv,
            pRxlen));
    }
    if (0U != (bWrappedMode))
    {
        /* memcpy(pResponse, pRecv, (*pRxlen) - 2); PRQA S 3200 */
        *ppResponse = pRecv;
        bStatusByte = pRecv[(*pRxlen) - 1U];
        (*pRxlen) -= 2u;
    }
    else
    {
        /* memcpy(pResponse, &pRecv[1], (*pRxlen) - 1);  PRQA S 3200 */
        *ppResponse = pRecv + 1U;
        bStatusByte = pRecv[0];
        (*pRxlen) -= 1u;
    }
    return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

/*
* Should return SW1+SW2
*/
phStatus_t phalMfdf_Int_Send7816Apdu(
                                     void * pDataParams,
                                     void * pPalMifareDataParams,
                                     uint8_t bOption,
                                     uint8_t bIns,
                                     uint8_t bP1,
                                     uint8_t bP2,
                                     uint8_t bLc,
                                     uint8_t * pDataIn,
                                     uint8_t bLe,
                                     uint8_t ** ppDataOut,
                                     uint16_t *pDataLen
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;

    bCmdBuff[wCmdLen++] = 0x00; /* Class is always 0x00 */
    bCmdBuff[wCmdLen++] = bIns;
    bCmdBuff[wCmdLen++] = bP1;
    bCmdBuff[wCmdLen++] = bP2;

    if (0U != (bOption & 0x01U))
    {
        bCmdBuff[wCmdLen++] = bLc;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    if ((0U != ((bOption & 0x01U))) && (bLc > 0U))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDataIn,
            bLc,
            &pRecv,
            &wRxlen
            ));
    }

    wCmdLen = 0;
    if (0U != (bOption & 0x02U))
    {
        bCmdBuff[wCmdLen++] = bLe;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    statusTmp = pRecv[wRxlen - 2u];
    statusTmp <<= 8U;
    statusTmp |= pRecv[wRxlen - 1u];

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_ComputeErrorResponse(pDataParams, statusTmp));

    if (pDataLen != NULL)
    {
        *pDataLen = wRxlen -2u;
    }

    if (ppDataOut != NULL)
    {
        *ppDataOut = pRecv;
    }

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHAL_MFDF */
