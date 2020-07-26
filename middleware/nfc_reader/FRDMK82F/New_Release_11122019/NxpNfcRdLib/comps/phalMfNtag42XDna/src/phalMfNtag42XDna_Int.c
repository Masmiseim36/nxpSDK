/*
 *                     Copyright 2016-2019, NXP
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

#include <ph_Status.h>

#ifdef NXPBUILD__PHAL_MFNTAG42XDNA

#include <string.h>
#include <phpalMifare.h>
#include <phalMfNtag42XDna.h>
#include "phalMfNtag42XDna_Int.h"
#include <ph_RefDefs.h>

phStatus_t phalMfNtag42XDna_Int_ComputeErrorResponse(void * pDataParams, uint16_t wStatus)
{
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusTmp;

    switch (wStatus)
    {
    case PHAL_MFNTAG42XDNA_RESP_OPERATION_OK:
    case PHAL_MFNTAG42XDNA_ISO7816_SUCCESS:
    case PHAL_MFNTAG42XDNA_RESP_OK:
        status = PH_ERR_SUCCESS;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_CMD_INVALID:
        status = PHAL_MFNTAG42XDNA_ERR_CMD_INVALID;
        break;
    case PHAL_MFNTAG42XDNA_RESP_ERR_FORMAT:
        status = PHAL_MFNTAG42XDNA_ERR_FORMAT;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_OUT_OF_EEPROM_ERROR:
        status = PHAL_MFNTAG42XDNA_ERR_OUT_OF_EEPROM_ERROR;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_INTEGRITY_ERROR:
        status = PHAL_MFNTAG42XDNA_ERR_PICC_CRYPTO;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_PARAMETER_ERROR:
        status = PHAL_MFNTAG42XDNA_ERR_PARAMETER_ERROR;
        break;

    case PHAL_MFNTAG42XDNA_RESP_NO_SUCH_KEY:
        status = PHAL_MFNTAG42XDNA_ERR_NO_SUCH_KEY;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_LENGTH_ERROR:
        status = PH_ERR_LENGTH_ERROR;
        break;

    case PHAL_MFNTAG42XDNA_RESP_PERMISSION_DENIED:
        status = PHAL_MFNTAG42XDNA_ERR_PERMISSION_DENIED;
        break;

    case PHAL_MFNTAG42XDNA_RESP_APPLICATION_NOT_FOUND:
        status = PHAL_MFNTAG42XDNA_ERR_APPLICATION_NOT_FOUND;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_AUTHENTICATION_ERROR:
        status = PH_ERR_AUTH_ERROR;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_AUTHENTICATION_DELAY:
        status = PH_ERR_AUTH_DELAY;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME:
        status = PH_ERR_SUCCESS_CHAINING;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_BOUNDARY_ERROR:
        status = PHAL_MFNTAG42XDNA_ERR_BOUNDARY_ERROR;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_COMMAND_ABORTED:
        status = PHAL_MFNTAG42XDNA_ERR_COMMAND_ABORTED;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_COUNT:
        status = PHAL_MFNTAG42XDNA_ERR_COUNT;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_DUPLICATE:
        status = PHAL_MFNTAG42XDNA_ERR_DUPLICATE;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_FILE_NOT_FOUND:
        status = PHAL_MFNTAG42XDNA_ERR_FILE_NOT_FOUND;
        break;

    case PHAL_MFNTAG42XDNA_RESP_ERR_APPL_INTEGRITY_ERROR:
    case PHAL_MFNTAG42XDNA_RESP_ERR_PICC_INTEGRITY:
    case PHAL_MFNTAG42XDNA_RESP_ERR_EEPROM:
    case PHAL_MFNTAG42XDNA_RESP_ERR_FILE_INTEGRITY:
    case PHAL_MFNTAG42XDNA_RESP_ERR_PIC_DISABLED:
    case PHAL_MFNTAG42XDNA_RESP_ILLEGAL_COMMAND_CODE:
        status = PHAL_MFNTAG42XDNA_ERR_DF_GEN_ERROR;
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_SetConfig(
            pDataParams,
            PHAL_MFNTAG42XDNA_ADDITIONAL_INFO,
            wStatus));
        break;

    case PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_LENGTH:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_LE:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_FILE_NOT_FOUND:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_PARAMS:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_LC:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_NO_PRECISE_DIAGNOSTICS:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_EOF_REACHED:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_LIMITED_FUNCTIONALITY_INS:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_FILE_ACCESS:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_FILE_EMPTY:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_MEMORY_FAILURE:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_INCORRECT_PARAMS:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_WRONG_CLA:
    case PHAL_MFNTAG42XDNA_ISO7816_ERR_UNSUPPORTED_INS:

        status = PHAL_MFNTAG42XDNA_ERR_DF_7816_GEN_ERROR;
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_SetConfig(
            pDataParams,
            PHAL_MFNTAG42XDNA_ADDITIONAL_INFO,
            wStatus));
        break;

    case PHAL_MFNTAG42XDNA_RESP_CHAINING:
        status = PH_ERR_SUCCESS_CHAINING;
        break;

    default:
        status = PH_ERR_PROTOCOL_ERROR;
    }
    return PH_ADD_COMPCODE(status, PH_COMP_AL_MFNTAG42XDNA);
}

/*
*
* Should take care of wrapping and unwrapping if ISO 7816 Wrapped mode
* Does not handle any chaining or CMAC generation/verification or encryption/decryption
*/
phStatus_t phalMfNtag42XDna_ExchangeCmd(void * pDataParams,  void * pPalMifareDataParams, uint8_t bWrappedMode,
    uint8_t * pSendBuff, uint16_t wCmdLen, uint8_t ** ppResponse, uint16_t * pRxlen)
{
    uint16_t    PH_MEMLOC_REM wFrameLen;
    uint8_t     PH_MEMLOC_REM * pRecv;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bStatusByte = 0xFF;
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFNTAG42XDNA_WRAPPEDAPDU_CLA, 0x00, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P1, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P2, 0x00 };

    if (bWrappedMode)
    {
        wFrameLen = PHAL_MFNTAG42XDNA_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        wFrameLen = PHAL_MFNTAG42XDNA_MAXDFAPDU_SIZE;
    }

    if (wCmdLen > wFrameLen)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Send the data to PICC */
    if (bWrappedMode)
    {
        pApdu[1] = pSendBuff[0]; /* Desfire cmd code in INS */
        pApdu[4] = (uint8_t)(wCmdLen) - 0x01u;

        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pPalMifareDataParams,
            pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            PHAL_MFNTAG42XDNA_WRAP_HDR_LEN,
            &pRecv,
            pRxlen));

        if (pApdu[4] != 0x00)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                &pSendBuff[1],
                wCmdLen - 1,
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
    if (bWrappedMode)
    {
        /* memcpy(pResponse, pRecv, (*pRxlen) - 2); PRQA S 3200 */
        *ppResponse = pRecv;
        bStatusByte = pRecv[(*pRxlen) - 1];
        (*pRxlen) -= 2;
    }
    else
    {
        /* memcpy(pResponse, &pRecv[1], (*pRxlen) - 1);  PRQA S 3200 */
        *ppResponse = pRecv + 1;
        bStatusByte = pRecv[0];
        (*pRxlen) -= 1;
    }
    return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

/*
* Should return SW1+SW2
*/
phStatus_t phalMfNtag42XDna_Int_Send7816Apdu(void * pDataParams, void * pPalMifareDataParams, uint8_t bOption, uint8_t bIns,
    uint8_t bP1, uint8_t bP2, uint32_t bLc, uint8_t bExtendedLenApdu, uint8_t * pDataIn, uint32_t bLe, uint8_t ** ppDataOut,
    uint16_t *pDataLen)
{
    phStatus_t  PH_MEMLOC_REM statusTmp = PH_ERR_SUCCESS;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    bCmdBuff[wCmdLen++] = 0x00; /* Class is always 0x00 */
    bCmdBuff[wCmdLen++] = bIns;
    bCmdBuff[wCmdLen++] = bP1;
    bCmdBuff[wCmdLen++] = bP2;

    switch(bIns)
    {
        case PHAL_MFNTAG42XDNA_CMD_ISO7816_SELECT_FILE:
            /* Both Lc and Le are required */
            statusTmp = phalMfNtag42XDna_Int_ISOSelectFile(pDataParams, pPalMifareDataParams, bOption, bCmdBuff, wCmdLen,
                (uint8_t)bLc, bExtendedLenApdu, pDataIn, (uint8_t)bLe, ppDataOut, pDataLen);
            break;

        case PHAL_MFNTAG42XDNA_CMD_ISO7816_UPDATE_BINARY:
            statusTmp = phalMfNtag42XDna_Int_ISOUpdateBinary(pDataParams, pPalMifareDataParams, bOption, bCmdBuff, wCmdLen,
                bLc, bExtendedLenApdu, pDataIn, ppDataOut, pDataLen);
            break;

        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_MFNTAG42XDNA);
            break;
    }
    return statusTmp;
}

phStatus_t phalMfNtag42XDna_Int_ISOSelectFile(void * pDataParams, void * pPalMifareDataParams, uint8_t bOption, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t bLc, uint8_t bExtendedLenApdu, uint8_t * pDataIn, uint8_t bLe, uint8_t ** ppDataOut, uint16_t *pDataLen)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;
    bOption =  0x01; /* To avoid warnings */

    /* Do not frame LC for Selector zero. */
    if(bCmdBuff[2] != 0x03)
    {
        /* Check whether Length Lc is represented in short APDU or extended APDU */
        if(bExtendedLenApdu == 0x01)
        {
            /* Extended APDU */
            bCmdBuff[wCmdLen++] = 0x00; /* first byte will be 0x00 if Ext apdu present. Next 2 byte contains actual data */
            bCmdBuff[wCmdLen++] = 0x00; /* As of now this field will be set to 0x00 since maximum data that can be sent is 16 bytes. In case if data to be sent exceeds 255 bytes, this byte shall be used  */
            bCmdBuff[wCmdLen++] = bLc;  /* Actual data to be sent */
        }
        else
        {
            /* Short APDU */
            bCmdBuff[wCmdLen++] = bLc;
        }
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    if ((bLc > 0))
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

    /* As per ISO/IEC:7816-4(2005), Section 5, An extended Le field consists of either three bytes
     * (one byte set to '00' followed by two bytes with any
     * value) if the Lc field is absent, or two bytes (with any value) if an extended Lc field is present.
     */

    /* Check whether Length is represented in extended APDU format and Lc is present.
     * If true, then Le should represented in 2 bytes else Le should be represented in 3 bytes
     */
    if(bCmdBuff[2] != 0x03)
    {
        if(bExtendedLenApdu == 0x01)
        {
            /* Extended APDU */
            bCmdBuff[wCmdLen++] = 0x00;
            bCmdBuff[wCmdLen++] = bLe;
        }
        else
        {
            /* Short APDU */
            bCmdBuff[wCmdLen++] = bLe;
        }
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    statusTmp = pRecv[wRxlen - 2];
    statusTmp <<= 8;
    statusTmp |= pRecv[wRxlen - 1];
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, statusTmp));
    if (pDataLen != NULL)
    {
        *pDataLen = wRxlen -2;
    }
    if (ppDataOut != NULL)
    {
        *ppDataOut = pRecv;
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phalMfNtag42XDna_Int_ISOUpdateBinary(void * pDataParams, void * pPalMifareDataParams, uint8_t bOption, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint32_t dwLc, uint8_t bExtendedLenApdu, uint8_t * pDataIn, uint8_t ** ppDataOut, uint16_t *pDataLen)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM *bDataLenPtr;
    /* Initialise LcPtr */
    bDataLenPtr = (uint8_t *)&dwLc;
    /* Check whether Lc has to be passed */
    if (bOption & 0x01)
    {
        /* Check whether Length Lc is represented in short APDU or extended APDU */
        if(bExtendedLenApdu == 0x01)
        {
            /*
             * As per ISO-7816 Standard(2005), section 5.1,
             * A short Lc field consists of one byte not set to '00'.
             * From '01' to 'FF', the byte encodes Nc from one to 255.
             *
             * An extended Lc field consists of three bytes: one byte
             * set to '00' followed by two bytes not set to '0000'.From
             * '0001' to 'FFFF', the two bytes encode Nc from one to
             * 65535.
             */
            bCmdBuff[wCmdLen++] = 0x00;
            bCmdBuff[wCmdLen++] = *(bDataLenPtr + 1);
            bCmdBuff[wCmdLen++] = *(bDataLenPtr);
            /* Note: Need to handle the case where data to write is more than 0xFFFF(i.e 65535 bytes).
            */
        }
        else
        {
            /* Short APDU */
            bCmdBuff[wCmdLen++] = *bDataLenPtr;
        }
    }
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    if ((bOption & 0x01) && (dwLc > 0))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDataIn,
            (uint16_t)dwLc,
            &pRecv,
            &wRxlen
            ));
    }
    wCmdLen = 0;
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    statusTmp = pRecv[wRxlen - 2];
    statusTmp <<= 8;
    statusTmp |= pRecv[wRxlen - 1];
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, statusTmp));
    if (pDataLen != NULL)
    {
        *pDataLen = wRxlen -2;
    }
    if (ppDataOut != NULL)
    {
        *ppDataOut = pRecv;
    }
    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHAL_MFNTAG42XDNA */
