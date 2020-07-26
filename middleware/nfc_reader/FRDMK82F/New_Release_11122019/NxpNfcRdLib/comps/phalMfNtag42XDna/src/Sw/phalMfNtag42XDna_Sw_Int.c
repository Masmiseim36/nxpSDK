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
#include <phpalMifare.h>
#include <phpalI14443p4.h>
#include <ph_RefDefs.h>
#include <ph_TypeDefs.h>
#include <phTools.h>
#include <string.h>
#ifdef NXPBUILD__PH_CRYPTOSYM
#include <phCryptoSym.h>
#include <phCryptoRng.h>
#endif /* NXPBUILD__PH_CRYPTOSYM */
#include <phKeyStore.h>
#include <phTMIUtils.h>
#include <phalVca.h>

#ifdef NXPBUILD__PHAL_MFNTAG42XDNA_SW

#include "../phalMfNtag42XDna_Int.h"
#include "phalMfNtag42XDna_Sw_Int.h"

phStatus_t phalMfNtag42XDna_Sw_Int_GetData(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t * pSendBuff, uint16_t wCmdLen,
    uint8_t ** pResponse, uint16_t * pRxlen)
{
    uint8_t    PH_MEMLOC_REM * pRecv;
    phStatus_t PH_MEMLOC_REM statusTmp = 0;
    uint8_t    PH_MEMLOC_REM bStatusByte = 0xFF;
    uint8_t    PH_MEMLOC_REM bCmdBuff[10];
    uint16_t   PH_MEMLOC_REM wNextPos = 0;
    uint16_t   PH_MEMLOC_REM wRxBufferSize = 0;
    uint8_t    PH_MEMLOC_REM bBackupBytes[3];
    uint8_t    PH_MEMLOC_REM pApdu[5] = { PHAL_MFNTAG42XDNA_WRAPPEDAPDU_CLA, 0x00, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P1, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P2, 0x00 };
    uint8_t    PH_MEMLOC_REM bBackUpByte;
    uint8_t    PH_MEMLOC_REM bBackUpByte1;
    uint16_t   PH_MEMLOC_REM wBackUpLen;
    uint16_t   PH_MEMLOC_REM wTmp = 0;

    /* Status and two other bytes to be backed up before getting new frame of data */
    memset(bBackupBytes, 0x00, 3); /* PRQA S 3200 */

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        &wTmp
        ));

        if (wCmdLen > PHAL_MFNTAG42XDNA_MAXWRAPPEDAPDU_SIZE)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFNTAG42XDNA);
        }

        pApdu[1] = pSendBuff[0];  /* Desfire command code. */
        /* Encode APDU Length*/
        pApdu[4]= (uint8_t)wCmdLen - 1; /* Set APDU Length. */

        statusTmp = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            PHAL_MFNTAG42XDNA_WRAP_HDR_LEN,
            &pRecv,
            pRxlen
            );
        if ((pApdu[4] != 0x00) && (statusTmp == PH_ERR_SUCCESS))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                &pSendBuff[1],
                wCmdLen - 1,
                &pRecv,
                pRxlen
                ));

            statusTmp = phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                &pApdu[2],
                0x01,
                &pRecv,
                pRxlen
                );
        }
        /* To handle the case where the card returns only status 91 and returns
        AF in the next frame */
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
        if (*pRxlen == 2)
            {
                /* AF should always be accompanied by data. Otherwise
                it is a protocol error */
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
            }
            /* One more status byte to read from DesFire */
        bBackUpByte = pRecv[0];
        bBackUpByte1 = pRecv[1];
        wBackUpLen = *pRxlen;

        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_RXCHAINING,
            &pApdu[2],
            0x01,
            &pRecv,
            pRxlen
            ));

        /* Received length can be one or two Ex: 0x91 0xAF */
        if (*pRxlen == 2)
        {
            pRecv[wBackUpLen] = pRecv[0];
            pRecv[wBackUpLen + 1] = pRecv[1];
            bStatusByte = pRecv[1];
        }
        else if (*pRxlen == 1)
        {
            bStatusByte = pRecv[0];
            pRecv[wBackUpLen] = bStatusByte;
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }

        *pRxlen = wBackUpLen + *pRxlen;

        /* Set back the backed up bytes */
        pRecv[0] = bBackUpByte;
        pRecv[1] = bBackUpByte1;
    }
    else
    {
        if (statusTmp != PH_ERR_SUCCESS)
        {
            return statusTmp;
        }
    }
    /* Storing the original pointer */
    *pResponse = pRecv;

    /* Status is 0xAF or 0x00? */
    if (*pRxlen > 0x0000)
    {
        bStatusByte = (*pResponse)[(*pRxlen) - 1];
    }

    if (bStatusByte == PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)
    {
        if (*pRxlen == 2)
        {
            /* AF should always be accompanied by data. Otherwise
            it is a protocol error */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }
         /* Next position will ensure overwriting on the
            SW1SW2 received from previous command */
            wNextPos = (*pRxlen) - 2;
            memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3], 3); /* PRQA S 3200 */


        PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
            &wRxBufferSize
            ));
    }

    while (bStatusByte == PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)
    {
        if ((*pRxlen == 2))
        {
            /* AF should always be accompanied by data. Otherwise
            it is a protocol error */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }
        if (wNextPos + PHAL_MFNTAG42XDNA_MAX_FRAME_SIZE >= wRxBufferSize)
        {
            /* Return 0xAF and let the caller recall the function with
            option = PH_EXCHANGE_RXCHAINING */
            /* Return the data accumulated till now and its length */
            (*pRxlen) -= 2;
            return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFNTAG42XDNA);
        }
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            wNextPos
            ));

        bCmdBuff[0] = PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME;
        wCmdLen = 1;
        pApdu[1] = bCmdBuff[0];  /* Desfire command code. */
        /* Encode APDU Length*/
         pApdu[4]= (uint8_t)wCmdLen - 1; /* Set APDU Length. */

        statusTmp = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            pApdu[4] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            PHAL_MFNTAG42XDNA_WRAP_HDR_LEN,
            &pRecv,
            pRxlen
            );
        if ((pApdu[4] != 0x00) && (statusTmp == PH_ERR_SUCCESS))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                bCmdBuff,
                wCmdLen,
                &pRecv,
                pRxlen
                ));

            bCmdBuff[0] = 0x00; /* Le */
            statusTmp = phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                bCmdBuff,
                0x01,
                &pRecv,
                pRxlen
                );
        }
        /* To handle the case where the card returns only status 91 and returns
        AF in the next frame */
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            /* One or two more status bytes to read from DesFire */
            bBackUpByte = pRecv[0];
            bBackUpByte1 = pRecv[1];
            wBackUpLen = *pRxlen;

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_RXCHAINING,
                &pApdu[2],
                0x01,
                &pRecv,
                pRxlen
                ));

            /* Received length can be one or two Ex: 0x91 0xAF */
            if (*pRxlen == 2)
            {
                pRecv[wBackUpLen] = pRecv[0];
                pRecv[wBackUpLen + 1] = pRecv[1];
                bStatusByte = pRecv[1];
            }
            else if (*pRxlen == 1)
            {
                bStatusByte = pRecv[0];
                pRecv[wBackUpLen] = bStatusByte;
            }
            else
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
            }

            *pRxlen = wBackUpLen + *pRxlen;

            /* Set back the backed up bytes */
            pRecv[0] = bBackUpByte;
            pRecv[1] = bBackUpByte1;
        }
        else
        {
            if (statusTmp != PH_ERR_SUCCESS)
            {
                return statusTmp;
            }
        }
            bStatusByte = (*pResponse)[(*pRxlen) - 1];

            /* Putback the backed up bytes */
            memcpy(&(*pResponse)[wNextPos - 3], bBackupBytes, 3); /* PRQA S 3200 */

            wNextPos = (*pRxlen) - 2;
            memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3], 3); /* PRQA S 3200 */

    }

    (*pRxlen) -= 2;
    return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

phStatus_t phalMfNtag42XDna_Sw_Int_ISOGetData(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t * pSendBuff, uint16_t wCmdLen,
    uint8_t ** pResponse, uint16_t * pRxlen)
{
    uint8_t    PH_MEMLOC_REM * pRecv;
    phStatus_t PH_MEMLOC_REM statusTmp = 0;
    phStatus_t PH_MEMLOC_REM status = 0;
    uint8_t    PH_MEMLOC_REM bCmdBuff[10];
    uint8_t    PH_MEMLOC_REM bApduLen = 4;  /* Initializing with 4 since Length of the Data(Lc) starts from 4th element of pApdu[] */
    uint16_t   PH_MEMLOC_REM wNextPos = 0;
    uint16_t   PH_MEMLOC_REM wRxBufferSize = 0;
    uint8_t    PH_MEMLOC_REM bBackupBytes[3];
    uint8_t    PH_MEMLOC_REM pApdu[7] = { PHAL_MFNTAG42XDNA_WRAPPEDAPDU_CLA, 0x00, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P1, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P2, 0x00,/* Extended Length Apdu */ 0x00, 0x00 };
    uint8_t    PH_MEMLOC_REM bLe[2] = { 0x00, 0x00 };
    uint8_t    PH_MEMLOC_REM bExtendedLenApdu = 0;
    uint16_t    PH_MEMLOC_REM wFSD = 0;
    uint16_t    PH_MEMLOC_REM wFSC = 0;
    uint16_t   PH_MEMLOC_REM wTmp = 0;
    uint16_t   pValue;

    /* Status and two other bytes to be backed up before getting new frame of data */
    memset(bBackupBytes, 0x00, 3); /* PRQA S 3200 */

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        &wTmp
        ));

    /*
       0xAF is just an indication that this is a call
       to the function to get remaining data
    */
    if (pSendBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)
    {
        /* Check for permissible CmdBuff size */
        if (wCmdLen > PHAL_MFNTAG42XDNA_MAXWRAPPEDAPDU_SIZE)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFNTAG42XDNA);
        }

            pApdu[1] = pSendBuff[0];  /* Desfire command code. */
            switch(pApdu[1])
            {
                case PHAL_MFNTAG42XDNA_CMD_READ_DATA:
                case PHAL_MFNTAG42XDNA_CMD_READ_DATA_ISO:
                    /* Get the format value(Whether to use short APDU or extended APDU */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_GetConfig(pDataParams,
                    PHAL_MFNTAG42XDNA_SHORT_LENGTH_APDU,
                        &pValue));
                    /* To Note: Short APDU will be used,
                     *  1. when user force the 'length' to be sent as short APDU in case of BIG ISO.
                     *  2. When the data to read is not BIG ISO(less than 256 bytes).
                     */
                    if(pValue & 0x0001)
                    {
                        /* Encode 'Length' in Short APDU format */
                        pApdu[bApduLen++]= (uint8_t)wCmdLen - 1; /* Set APDU Length. */
                    }
                    else
                    {
                        /* Encode 'Length' in extended Length format */
                        bExtendedLenApdu = 0x01;
                        pApdu[bApduLen++] = 0x00;
                        pApdu[bApduLen++] = 0x00;
                        pApdu[bApduLen++] = (uint8_t)wCmdLen - 1; /* Set APDU Length. */
                    }
                    break;

                default:
                    /* Rest other commands, retain existing implementation which is Short APDU */
                    pApdu[bApduLen++]= (uint8_t)wCmdLen - 1; /* Set APDU Length. */
                    break;
            }
            statusTmp = phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                pApdu[bApduLen-1] == 0x00 ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
                pApdu,
                bApduLen,
                &pRecv,
                pRxlen
                );
            /* Check if pApdu[4] is valid in case of Short APDU or
             * Check if pAdpu[6] is valid in case of Extended APDU
             */
            if (((pApdu[4] != 0x00) && (statusTmp == PH_ERR_SUCCESS)) ||
                ((bExtendedLenApdu && (pApdu[6] != 0x00))  && (statusTmp == PH_ERR_SUCCESS)))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    &pSendBuff[1],
                    wCmdLen - 1,
                    &pRecv,
                    pRxlen
                    ));

                PH_CHECK_SUCCESS_FCT(status,phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_LAST,
                    bLe,
                    bExtendedLenApdu ? 0x02 : 0x01,
                    &pRecv,
                    pRxlen
                    ));
            }
        }
        else
        {
        /* Send this on L4 */
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_RXCHAINING,
            pSendBuff,
            wCmdLen,
            &pRecv,
            pRxlen
            );
    }

    if ((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        return status;
    }

    /* Storing the original pointer */
    *pResponse = pRecv;

    if (status == PH_ERR_SUCCESS_CHAINING)
    {
        /* Next position will ensure overwriting on the
        SW1SW2 received from previous command */
        wNextPos = (*pRxlen) - 2;
        memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3], 3); /* PRQA S 3200 */

        PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
            &wRxBufferSize
            ));
    }

    while ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        /* Get the Frame length */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_GetFrameLength(
            pDataParams,
            &wFSD,
            &wFSC
            ));

        if (wFSD > wFSC)
        {
            if (wNextPos + wFSC >= wRxBufferSize)
            {
                /* Return Chaining and let the caller recall the function with
                option = PH_EXCHANGE_RXCHAINING */
                /* Return the data accumulated till now and its length */
                if((pSendBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME) && (!pDataParams->bWrappedMode))
                {
                    (*pRxlen) -= 1;
                    (*pResponse)++;
                }
                return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFNTAG42XDNA);
            }
        }
        else
        {
            if (wNextPos + wFSD >= wRxBufferSize)
            {
                /* Return Chaining and let the caller recall the function with
                option = PH_EXCHANGE_RXCHAINING */
                /* Return the data accumulated till now and its length */
               (*pRxlen) -= 2;

                return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFNTAG42XDNA);
            }
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            wNextPos
            ));

        /* Send this on L4 */
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_RXCHAINING,
            bCmdBuff,
            wCmdLen,
            &pRecv,
            pRxlen
           );

        /* Update wNextPos */

         /* Putback the backed up bytes */
         memcpy(&(*pResponse)[wNextPos - 3], bBackupBytes, 3); /* PRQA S 3200 */
         /* Update the Buffer Position */
         wNextPos = (*pRxlen) - 2;

        /* Backup 3 bytes. The nxt frame will overwrite these */
        memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3], 3); /* PRQA S 3200 */
    }

    /* Status is 0x00? */
    statusTmp = pRecv[(*pRxlen) - 1];
    (*pRxlen) -= 2;

    return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, statusTmp);
}

phStatus_t phalMfNtag42XDna_Sw_Int_ReadData_Plain(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bOption, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t ** ppRxdata, uint16_t * pRxdataLen)
{
    uint16_t    PH_MEMLOC_REM status = 0;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[32];
#ifdef NXPBUILD__PH_CRYPTOSYM
    uint16_t    PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bCMAC[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint8_t     PH_MEMLOC_REM bMacLen = 0;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint16_t    PH_MEMLOC_REM wTmp = 0;
#endif /* NXPBUILD__PH_CRYPTOSYM */
    uint16_t    PH_MEMLOC_REM wWorkBufferLen = 0;

    memset(bWorkBuffer, 0x00, 32); /* PRQA S 3200 */
#ifdef NXPBUILD__PH_CRYPTOSYM
    memset(bCMAC, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */
    memset(bCMacCard, 0x00, 8); /* PRQA S 3200 */
#endif /* NXPBUILD__PH_CRYPTOSYM */

    /* Copy the bCmdBuff data to the bWorkBuff */
    memcpy(bWorkBuffer, bCmdBuff, wCmdLen); /* PRQA S 3200 */
    wWorkBufferLen = wCmdLen;

#ifdef NXPBUILD__PH_CRYPTOSYM
    if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }

    if(bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)
    {
        /* Check for 0xAF added above to ensure that we dont update the
        IV or calculate CMAC for cases where in the application has called
        this API with bOption = PH_EXCHANGE_RXCHAINING */

        if ((pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)&&
            ((bOption & 0xF0U) == PHAL_MFNTAG42XDNA_COMMUNICATION_MACD))
        {
            memset(pDataParams->bIv, 0x00, bIvLen);  /* PRQA S 3200 */
            wWorkBufferLen = 0;
            bWorkBuffer[wWorkBufferLen++] = bCmdBuff[0];
            /* Add CmdCtr and TI for MAC calculation */
            bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr);
            bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
            memcpy(&bWorkBuffer[wWorkBufferLen], pDataParams->bTi, PHAL_MFNTAG42XDNA_SIZE_TI); /* PRQA S 3200 */
            wWorkBufferLen += PHAL_MFNTAG42XDNA_SIZE_TI;

            /* Assumed here that a read-like commands cannot in any case have cmd+header+data > 24 bytes */

            if (wCmdLen > 1)
            {
                memcpy(&bWorkBuffer[wWorkBufferLen], &bCmdBuff[1], (wCmdLen - 1)); /* PRQA S 3200 */
                wWorkBufferLen += (wCmdLen - 1);
            }

            /* Load Iv */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                pDataParams->pCryptoDataParamsMac,
                pDataParams->bIv,
                bIvLen
                ));

            /* Calculate MAC */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                (PH_CRYPTOSYM_MAC_MODE_CMAC),
                bWorkBuffer,
                wWorkBufferLen,
                bCMAC,
                &bMacLen
                ));

            /* Truncate the MAC generated */
            phalMfNtag42XDna_Sw_Int_TruncateMac(bCMAC);

            /* Get the original command in the work buffer. */
            memcpy(bWorkBuffer, bCmdBuff, wCmdLen);  /* PRQA S 3200 */
            /* Append MAC for EV2 mode. */
            memcpy(&bWorkBuffer[wCmdLen], bCMAC, PHAL_MFNTAG42XDNA_TRUNCATED_MAC_SIZE);  /* PRQA S 3200 */
            wWorkBufferLen = wCmdLen + PHAL_MFNTAG42XDNA_TRUNCATED_MAC_SIZE;
        }
        else
        {
            /*Do Nothing. This is for PRQA compliance */
        }
    }
#endif /* NXPBUILD__PH_CRYPTOSYM */

    if (bOption & PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE)
    {
        status = phalMfNtag42XDna_Sw_Int_ISOGetData(
            pDataParams,
            bWorkBuffer,
            wWorkBufferLen,
            &pRecv,
            &wRxlen
            );
    }
    else
    {
        /* Send the command */
        status = phalMfNtag42XDna_Sw_Int_GetData(
            pDataParams,
            bWorkBuffer,
            wWorkBufferLen,
            &pRecv,
            &wRxlen
            );
    }

    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM
        /* Reset authentication status */
        if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
        {
            phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
        }
        /* Set the length pointer with valid value. Otherwise there will be an error in AL while logging. (Access violation in addess 0xccccccc) */
        *pRxdataLen = wRxlen;
        *ppRxdata = pRecv ;
        /* Component code is already added by GetData */
#endif /* NXPBUILD__PH_CRYPTOSYM */
        return status;
    }

    if ((pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2) &&
        (bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME))
    {
        /* Increment the command counter. This increments irrespective of
        * Plain mode or MAC mode. Ensuring here that it is incremented
        * only for the case where the user has called this for the first
        * time i.e., without PH_EXCHANGE_RXCHAINING option
        */
        pDataParams->wCmdCtr++;
    }

#ifdef NXPBUILD__PH_CRYPTOSYM
    if ((pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2) &&
        ((bOption & 0xF0U) == PHAL_MFNTAG42XDNA_COMMUNICATION_MACD))
    {
        if (bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                pDataParams->pCryptoDataParamsMac,
                pDataParams->bIv,
                bIvLen
                ));

            pDataParams->bNoUnprocBytes = 0;
            /* Return code */
            pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = 0x00;
            /* Lower byte of CmdCtr */
            pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr);
            /* Higher byte of CmdCtr */
            pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
            /* TI */
            memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pDataParams->bTi, PHAL_MFNTAG42XDNA_SIZE_TI); /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes += PHAL_MFNTAG42XDNA_SIZE_TI;
        }

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            memcpy(bCMacCard, &pRecv[wRxlen - 8], 8); /* PRQA S 3200 */
            wRxlen -= 8;

            /* If receieved data is not multiple of block size */
            wTmp = (PH_CRYPTOSYM_AES_BLOCK_SIZE - pDataParams->bNoUnprocBytes);
            if(wTmp >= wRxlen)
            {
                wTmp = wRxlen;
            }
            memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pRecv, wTmp);  /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes += (uint8_t)wTmp;

            if (wTmp == wRxlen)
            {
                /* Conclude the CMAC calculation. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST),
                    pDataParams->pUnprocByteBuff,
                    (pDataParams->bNoUnprocBytes),
                    bCMAC,
                    &bMacLen
                    ));
            }
            else
            {
                /* First send the 16 byte block for cmac calculation */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                    pDataParams->pUnprocByteBuff,
                    (pDataParams->bNoUnprocBytes),
                    bCMAC,
                    &bMacLen
                    ));

                /* Send rest of the received data */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST),
                    &pRecv[wTmp],
                    wRxlen - wTmp,
                    bCMAC,
                    &bMacLen
                    ));
            }

            /* Since end of response is reached reset the pDataParams bNoUnprocBytes member to 0 */
            pDataParams->bNoUnprocBytes = 0;

            /* Truncate the MAC generated */
            phalMfNtag42XDna_Sw_Int_TruncateMac(bCMAC);

            /* Compare the CMAC received and Calculated MAC */
            if (memcmp(bCMAC, bCMacCard, 8) != 0)
            {
                /* CMAC validation failed */
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFNTAG42XDNA);
            }
        }
        else
        {
            /* Handling of chaining. */
            /* Include the left over data for CMAC calculation */
            wTmp = (PH_CRYPTOSYM_AES_BLOCK_SIZE - pDataParams->bNoUnprocBytes);
            memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pRecv, wTmp);  /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes += (uint8_t)wTmp;

            /* Start MAC calculation with one full block size data */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                pDataParams->pUnprocByteBuff,
                (pDataParams->bNoUnprocBytes),
                bCMAC,
                &bMacLen
                ));

            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                &pRecv[wTmp],
                ((wRxlen - wTmp)/PH_CRYPTOSYM_AES_BLOCK_SIZE) * PH_CRYPTOSYM_AES_BLOCK_SIZE,
                bCMAC,
                &bMacLen
                ));

            /* Remaining bytes */
            wTmp = (wRxlen - wTmp) % PH_CRYPTOSYM_AES_BLOCK_SIZE;

            /* Update the UnprocByteBuffer with bytes not used for mac calculation */
            memcpy(pDataParams->pUnprocByteBuff, &pRecv[wRxlen - wTmp], wTmp);  /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes = (uint8_t)wTmp;
        }
    }
    else
    {
        /* Do Nothing. For PRQA Compliance.*/
        bIvLen = 0x00;
    }
#endif /* NXPBUILD__PH_CRYPTOSYM */
    *ppRxdata = pRecv;
    *pRxdataLen = wRxlen;

    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFNTAG42XDNA);;
    }

    return PH_ERR_SUCCESS;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_Int_ReadData_Enc(phalMfNtag42XDna_Sw_DataParams_t * UNALIGNED pDataParams, uint8_t bOption,
    uint8_t * bCmdBuff, uint16_t wCmdLen, uint8_t ** ppRxdata, uint16_t * pRxdataLen)
{
    uint16_t    PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCMAC[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bMacLen;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[32];
    uint16_t    PH_MEMLOC_REM wWorkBufferLen;
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint16_t    PH_MEMLOC_REM wTmp = 0;

    /* Copy the bCmdBuff data to the bWorkBuff */
    memcpy(bWorkBuffer, bCmdBuff, wCmdLen);  /* PRQA S 3200 */
    wWorkBufferLen = wCmdLen;

    if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
    else
    {
#ifdef RDR_LIB_PARAM_CHECK
        return PH_ADD_COMPCODE_FIXED(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFNTAG42XDNA);
#endif
    }

    if ((pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2) &&
        (bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME))
    {
        memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */
        wWorkBufferLen = 0;
        bWorkBuffer[wWorkBufferLen++] = bCmdBuff[0];
        /* Add CmdCtr and TI for MAC calculation */
        bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr);
        bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
        memcpy(&bWorkBuffer[wWorkBufferLen], pDataParams->bTi, PHAL_MFNTAG42XDNA_SIZE_TI); /* PRQA S 3200 */
        wWorkBufferLen += PHAL_MFNTAG42XDNA_SIZE_TI;

        if (wCmdLen > 1)
        {
            memcpy(&bWorkBuffer[wWorkBufferLen], &bCmdBuff[1], (wCmdLen - 1)); /* PRQA S 3200 */
            wWorkBufferLen += (wCmdLen - 1);
        }

        /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsMac,
            pDataParams->bIv,
            bIvLen
            ));

        /* Calculate MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            PH_CRYPTOSYM_MAC_MODE_CMAC,
            bWorkBuffer,
            wWorkBufferLen,
            bCMAC,
            &bMacLen
            ));

        /* Truncate the MAC generated */
        phalMfNtag42XDna_Sw_Int_TruncateMac(bCMAC);

        /* Get the original command in the work buffer. */
        memcpy(bWorkBuffer, bCmdBuff, wCmdLen); /* PRQA S 3200 */
        /* Append MAC for EV2 mode. */
        memcpy(&bWorkBuffer[wCmdLen], bCMAC, PHAL_MFNTAG42XDNA_TRUNCATED_MAC_SIZE);  /* PRQA S 3200 */
        wWorkBufferLen = wCmdLen + PHAL_MFNTAG42XDNA_TRUNCATED_MAC_SIZE;
    }
    else
    {
        /*Do Nothing. This is for PRQA compliance */
    }

    if (bOption & PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE)
    {
        /* Send the command */
        status = phalMfNtag42XDna_Sw_Int_ISOGetData(
            pDataParams,
            bWorkBuffer,
            wWorkBufferLen,
            &pRecv,
            &wRxlen
            );
    }
    else
    {
        /* Send the command */
        status = phalMfNtag42XDna_Sw_Int_GetData(
            pDataParams,
            bWorkBuffer,
            wWorkBufferLen,
            &pRecv,
            &wRxlen
            );
    }

    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        /* Reset authentication status */
        if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
        {
            phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
        }
        /* Set the length pointer with valid value. Otherwise there will be an error in AL while logging. (Access violation in addess 0xccccccc) */
        *pRxdataLen = wRxlen;
        *ppRxdata = pRecv ;
        return status;
    }

    if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
    {
        if (bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)
        {
            /* Increment the command counter */
            pDataParams->wCmdCtr++;

            pDataParams->bNoUnprocBytes = 0;
             /* Return code */
            if(bCmdBuff[0] == PHAL_MFNTAG42XDNA_CMD_READ_SIG)
                pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = 0x90;
            else
                 pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = 0x00;

            /* Lower byte of CmdCtr */
            pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr);
            /* Higher byte of CmdCtr */
            pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
            /* TI */
            memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pDataParams->bTi, PHAL_MFNTAG42XDNA_SIZE_TI); /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes += PHAL_MFNTAG42XDNA_SIZE_TI;

            {
                /* the IV is constructed by encrypting with KeyID.SesAuthENCKey according to the ECB mode
                 * As ECB encription doesnot use IV during the encription so we need not backup/ update with zero IV*/
                PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_ComputeIv(PH_ON,
                    pDataParams->bTi,
                    pDataParams->wCmdCtr,
                    pDataParams->bIv
                    ));

                /* Encrypt IV */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                    pDataParams->pCryptoDataParamsEnc,
                    PH_CRYPTOSYM_CIPHER_MODE_ECB,
                    pDataParams->bIv,
                    bIvLen,
                    pDataParams->bIv
                    ));

                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                    pDataParams->pCryptoDataParamsEnc,
                    pDataParams->bIv,
                    bIvLen
                    ));

                /* Handling of chaining. */

                memset(pDataParams->bIv, 0x00, bIvLen);  /* PRQA S 3200 */

                /* Load IV */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                    pDataParams->pCryptoDataParamsMac,
                    pDataParams->bIv,
                    bIvLen
                    ));
            }
        }
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            memcpy(bCMacCard, &pRecv[wRxlen - 8], 8); /* PRQA S 3200 */
            wRxlen -= 8;

            /* If receieved data is not multiple of block size */
            wTmp = (PH_CRYPTOSYM_AES_BLOCK_SIZE - pDataParams->bNoUnprocBytes);
            if(wTmp >= wRxlen)
            {
                wTmp = wRxlen;
            }
            memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pRecv, wTmp);  /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes += (uint8_t)wTmp;

            if (wTmp == wRxlen)
            {
                /* Conclude the CMAC calculation. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST),
                    pDataParams->pUnprocByteBuff,
                    (pDataParams->bNoUnprocBytes),
                    bCMAC,
                    &bMacLen
                    ));
            }
            else
            {
                /* First send the 16 byte block for CMAC calculation */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                    pDataParams->pUnprocByteBuff,
                    (pDataParams->bNoUnprocBytes),
                    bCMAC,
                    &bMacLen
                    ));

                /* Send rest of the received data */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST),
                    &pRecv[wTmp],
                    wRxlen - wTmp,
                    bCMAC,
                    &bMacLen
                    ));
            }

            /* Since end of response is reached reset the pDataParams bNoUnprocBytes member to 0 */
            pDataParams->bNoUnprocBytes = 0;

            /* Truncate the MAC generated */
            phalMfNtag42XDna_Sw_Int_TruncateMac(bCMAC);

            /* Compare the CMAC received and Calculated MAC */
            if (memcmp(bCMAC, bCMacCard, 8) != 0)
            {
                /* CMAC validation failed */
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFNTAG42XDNA);
            }

            /* the IV is constructed by encrypting with KeyID.SesAuthENCKey according to the ECB mode
             * As ECB encription doesnot use IV during the encription so we need not backup/ update with zero IV
             * This logic is applicable only in case of Non LRP Crypto Method */
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_ComputeIv(PH_ON,
                    pDataParams->bTi,
                    pDataParams->wCmdCtr,
                    pDataParams->bIv
                    ));

                /* Encrypt IV */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                    pDataParams->pCryptoDataParamsEnc,
                    PH_CRYPTOSYM_CIPHER_MODE_ECB,
                    pDataParams->bIv,
                    bIvLen,
                    pDataParams->bIv
                    ));

                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                    pDataParams->pCryptoDataParamsEnc,
                    pDataParams->bIv,
                    bIvLen
                    ));
            }

            /* Decrypt the received Encrypted buffer */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
                pDataParams->pCryptoDataParamsEnc,
                (PH_CRYPTOSYM_CIPHER_MODE_CBC),
                pRecv,
                wRxlen,
                pRecv
                ));


            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_RemovePadding(
                PH_CRYPTOSYM_PADDING_MODE_2,
                pRecv,
                wRxlen,
                bIvLen,
                wRxlen,
                pRecv,
                &wRxlen
                ));

        }
        else
        {
            /* Include the left over data for CMAC calculation */
            wTmp = (PH_CRYPTOSYM_AES_BLOCK_SIZE - pDataParams->bNoUnprocBytes);
            memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pRecv, wTmp);  /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes += (uint8_t)wTmp;

            /* Start MAC calculation with one full block size data */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                pDataParams->pUnprocByteBuff,
                (pDataParams->bNoUnprocBytes),
                bCMAC,
                &bMacLen
                ));

            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                &pRecv[wTmp],
                ((wRxlen - wTmp)/PH_CRYPTOSYM_AES_BLOCK_SIZE) * PH_CRYPTOSYM_AES_BLOCK_SIZE,
                bCMAC,
                &bMacLen
                ));

            /* Remaining bytes */
            wTmp = (wRxlen - wTmp) % PH_CRYPTOSYM_AES_BLOCK_SIZE;

            /* Update the UnprocByteBuffer with bytes not used for mac calculation */
            memcpy(pDataParams->pUnprocByteBuff, &pRecv[wRxlen - wTmp], wTmp);  /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes = (uint8_t)wTmp;

            /* Decrypt */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
                pDataParams->pCryptoDataParamsEnc,
                (PH_CRYPTOSYM_CIPHER_MODE_CBC),
                pRecv,
                wRxlen,
                pRecv
                ));
        }
    }

    *ppRxdata = pRecv;
    *pRxdataLen = wRxlen;

    return status;
}

phStatus_t phalMfNtag42XDna_Sw_Int_Write_Enc(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t bPaddingOption, uint8_t bCommOption, uint8_t * pData, uint16_t wDataLen)

{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM status = 0;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint16_t    PH_MEMLOC_REM wTmp = 0;
    uint8_t     PH_MEMLOC_REM bCMAC[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bWorkBuffer[32];
    uint16_t    PH_MEMLOC_REM wWorkBufferLen = 0;
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint8_t     PH_MEMLOC_REM pResp[16];
    uint8_t     PH_MEMLOC_REM bMacLen = 0;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint16_t    PH_MEMLOC_REM wFrameLen = 0;
    uint16_t    PH_MEMLOC_REM wTotalLen = 0;
    uint16_t    PH_MEMLOC_REM wLastChunkLen = 0;
    uint16_t    PH_MEMLOC_REM wDataLen1 = 0;
    uint16_t    PH_MEMLOC_REM wNumDataBlocks = 0;
    uint8_t     PH_MEMLOC_REM bLastChunk[32];
    uint16_t    PH_MEMLOC_REM wCommMode = PH_EXCHANGE_BUFFER_CONT;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFNTAG42XDNA_WRAPPEDAPDU_CLA, 0x00, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P1, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P2, 0x00 };
    uint16_t    PH_MEMLOC_REM wFSD = 0;
    uint16_t    PH_MEMLOC_REM wFSC = 0;
    uint8_t     PH_MEMLOC_REM bIvBackup[16];
    uint16_t    PH_MEMLOC_REM wApduLen = 0;

    memset(bWorkBuffer, 0x00, 20); /* PRQA S 3200 */
    memset(bCMacCard, 0x00, 8); /* PRQA S 3200 */
    memset(bCMAC, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */
    memset(bLastChunk, 0x00, 32); /* PRQA S 3200 */

#ifdef RDR_LIB_PARAM_CHECK
    if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_NOT_AUTHENTICATED)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFNTAG42XDNA);
    }
#endif

    if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;

        /* Encrypt the CmdData with KsesAuthENC, If required padding needs to be done */
        /* the IV is constructed by encrypting with KeyID.SesAuthENCKey according to the ECB mode
         * As ECB encription doesnot use IV during the encription so we need not backup/ update with zero IV*/

        /* encrypt only Cmd data with KsesauthEnc */
        if (bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)
        {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_ComputeIv(PH_OFF,
                pDataParams->bTi,
                pDataParams->wCmdCtr,
                pDataParams->bIv
                ));

            /* Encrypt IV */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                pDataParams->pCryptoDataParamsEnc,
                PH_CRYPTOSYM_CIPHER_MODE_ECB,
                pDataParams->bIv,
                bIvLen,
                pDataParams->bIv
                ));

            wCommMode = PH_EXCHANGE_BUFFER_FIRST;


            /* Load Iv */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                pDataParams->pCryptoDataParamsEnc,
                pDataParams->bIv,
                bIvLen
                ));

            wNumDataBlocks = (wDataLen / bIvLen);

            if (wNumDataBlocks > 0)
            {
                /* Encrypt these blocks. Encrypted data put back on pData */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
                    pDataParams->pCryptoDataParamsEnc,
                    (PH_CRYPTOSYM_CIPHER_MODE_CBC | wCommMode),
                    pData,
                    wNumDataBlocks * bIvLen,
                    pData
                    ));

                /* Update the data index this hold the data encrypted */
                wDataLen1 = wNumDataBlocks * bIvLen;

                /* Update the IV */
                memcpy(pDataParams->bIv, &pData[(wNumDataBlocks * bIvLen) - bIvLen], bIvLen); /* PRQA S 3200 */
            }
        }

        if((bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE) != PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)
        {
            /*check for remaining bytes if present*/
            if (wDataLen - wDataLen1)
            {
                /* Prepare the last frame of data */
                memcpy(bLastChunk, &pData[wDataLen1], wDataLen - wDataLen1); /* PRQA S 3200 */
                /* Has a last frame */
                wLastChunkLen = wDataLen - wDataLen1;
            }

            bPaddingOption =  PH_CRYPTOSYM_PADDING_MODE_2;

            /* Apply padding */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_ApplyPadding(
                PH_CRYPTOSYM_PADDING_MODE_2,
                bLastChunk,
                wLastChunkLen,
                16,
                sizeof(bLastChunk),
                bLastChunk,
                &wLastChunkLen
                ));

            /* Load Iv */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                pDataParams->pCryptoDataParamsEnc,
                pDataParams->bIv,
                bIvLen
                ));

            /* Encrypt the last frame*/
            PH_CHECK_SUCCESS_FCT(statusTmp,phCryptoSym_Encrypt(
                pDataParams->pCryptoDataParamsEnc,
                PH_CRYPTOSYM_CIPHER_MODE_CBC | (((wDataLen1 == 0)&&(bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)) ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_LAST),
                bLastChunk,
                wLastChunkLen,
                bLastChunk
                ));
        }

        /* size of encrypted data */
        wTotalLen = wDataLen1 + wLastChunkLen;

        if (bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)
        {
            /* back up encryption IV */
            memcpy(bIvBackup, pDataParams->bIv, bIvLen); /* PRQA S 3200 */

            /* set Iv value  for CMAC caluclation*/
            memset(pDataParams->bIv, 0x00, bIvLen);  /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes = 0;

            /* Load Iv */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                pDataParams->pCryptoDataParamsMac,
                pDataParams->bIv,
                bIvLen
                ));

            /* copy original encrypted IV */
            memcpy( pDataParams->bIv,bIvBackup, bIvLen); /* PRQA S 3200 */

            /* Calculate MAC on Cmd || wCmdCtr || TI || CmdHeader || CmdData */
            bWorkBuffer[wWorkBufferLen++] = bCmdBuff[0];
            bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr);
            bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
            memcpy(&bWorkBuffer[wWorkBufferLen], pDataParams->bTi, PHAL_MFNTAG42XDNA_SIZE_TI); /* PRQA S 3200 */
            wWorkBufferLen += PHAL_MFNTAG42XDNA_SIZE_TI;
        }
        else
        {
            memcpy(bWorkBuffer, pDataParams->pUnprocByteBuff, pDataParams->bNoUnprocBytes); /* PRQA S 3200 */
            wWorkBufferLen = pDataParams->bNoUnprocBytes;
            pDataParams->bNoUnprocBytes = 0;
        }

        /* Check for presence of command header */
        if (wCmdLen > 1)
        {
            /* Calculate the total length of data for MAC calculation */
            wTmp = ((wCmdLen - 1) + (wWorkBufferLen));

            /* Since bWorkbuffer can accomodate 32 bytes, check for buffer overflow */
            if (wTmp > 32)
            {
                memcpy(&bWorkBuffer[wWorkBufferLen], &bCmdBuff[1], (32 - wWorkBufferLen)); /* PRQA S 3200 */
                /* Calculate CMAC */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_FIRST,
                    bWorkBuffer,
                    32,
                    bCMAC,
                    &bMacLen
                    ));

                /* Copy the remaining bCmdBuff into bWorkBuffer */
                memcpy(bWorkBuffer, &bCmdBuff[(32 - wWorkBufferLen) + 1], (wTmp - 32)); /* PRQA S 3200 */
                wWorkBufferLen = (wTmp - 32);
                wCommMode = PH_EXCHANGE_BUFFER_CONT;
            }
            else
            {
                memcpy(&bWorkBuffer[wWorkBufferLen], &bCmdBuff[1], (wCmdLen - 1)); /* PRQA S 3200 */
                wWorkBufferLen += (wCmdLen - 1);
            }
        }

        /* calculate the MAC value for encrypted CmdData */
        if (wDataLen1)
        {
            if((wTmp < 32) && ((bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME)))
            {
                wCommMode = PH_EXCHANGE_BUFFER_FIRST;
            }

            /* check for remaining number of data to make multiple of IV length */
            wTmp = (PH_CRYPTOSYM_AES_BLOCK_SIZE - (wWorkBufferLen % PH_CRYPTOSYM_AES_BLOCK_SIZE));

            memcpy(&bWorkBuffer[wWorkBufferLen], pData, wTmp); /* PRQA S 3200 */

            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                pDataParams->pCryptoDataParamsMac,
                (PH_CRYPTOSYM_MAC_MODE_CMAC | wCommMode),
                bWorkBuffer,
                wWorkBufferLen + wTmp,
                bCMAC,
                &bMacLen
                ));

            if((wDataLen1 - wTmp) > PH_CRYPTOSYM_AES_BLOCK_SIZE )
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                    &pData[wTmp],
                    (((wDataLen1 - wTmp) / PH_CRYPTOSYM_AES_BLOCK_SIZE) * PH_CRYPTOSYM_AES_BLOCK_SIZE),
                    bCMAC,
                    &bMacLen
                    ));
            }

            wCommMode = PH_EXCHANGE_BUFFER_LAST;

            /* copy reaming data present in pdata */
            wWorkBufferLen = ((wDataLen1 - wTmp) % PH_CRYPTOSYM_AES_BLOCK_SIZE);
            memcpy(bWorkBuffer, &pData[wDataLen1 - wWorkBufferLen], wWorkBufferLen); /* PRQA S 3200 */
        }
        /* if Last packet of data is sent */
        if ((bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE) != PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)
        {
            /* copy remaing data to work buffer */
            wTmp = wWorkBufferLen + wLastChunkLen;

            if(wTmp < 32)
            {
                memcpy(&bWorkBuffer[wWorkBufferLen], &bLastChunk[0], wLastChunkLen); /* PRQA S 3200 */

                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST),
                    bWorkBuffer,
                    wTmp,
                    bCMAC,
                    &bMacLen
                    ));
            }
            else
            {
                wTmp = 32 - wWorkBufferLen;
                memcpy(&bWorkBuffer[wWorkBufferLen], bLastChunk, wTmp); /* PRQA S 3200 */

                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                    bWorkBuffer,
                    32,
                    bCMAC,
                    &bMacLen
                    ));

                /* this If condition is added to suppress QAC warning */
                if(wLastChunkLen > 0 )
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                        pDataParams->pCryptoDataParamsMac,
                        (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST),
                        &bLastChunk[wTmp],
                        wLastChunkLen - wTmp,
                        bCMAC,
                        &bMacLen
                        ));
                }
            }

            /* Truncate the MAC generated */
            phalMfNtag42XDna_Sw_Int_TruncateMac(bCMAC);
            memcpy(&bLastChunk[wLastChunkLen], bCMAC, 8); /* PRQA S 3200 */
            wLastChunkLen += 8;
        }
        else
        {
            /* calculate CMAC for if data is multiple of IV */
            if(wWorkBufferLen > PH_CRYPTOSYM_AES_BLOCK_SIZE )
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                    bWorkBuffer,
                    ((wWorkBufferLen / PH_CRYPTOSYM_AES_BLOCK_SIZE) * PH_CRYPTOSYM_AES_BLOCK_SIZE),
                    bCMAC,
                    &bMacLen
                    ));
            }
            else
            {
                /* copy reaming data present in pdata */
                pDataParams->bNoUnprocBytes =(uint8_t) (wWorkBufferLen % PH_CRYPTOSYM_AES_BLOCK_SIZE);
                memcpy(pDataParams->pUnprocByteBuff, bWorkBuffer, pDataParams->bNoUnprocBytes); /* PRQA S 3200 */
            }
        }
        /* Update Total Length */
        wTotalLen = wDataLen1 + wLastChunkLen;
    }

    /* At this point have proper data to be transmitted in the
    * buffer provided by the user.
    * wTotalLength should have the total length to be transmitted
    * First Frame: Cmd+Params+Data
    * wTotalLength -= wDataLen;
    while (wTotalLength)
    {
    Break the data into chunks of maxdata size and transmit.
    For ISO wrapped mode, max of 55 bytes can be sent in one frame.
    For native mode, max of 60 bytes can be sent in one frame.
    }
    */

    /* First copy the cmd+params+data(upto 52 bytes) and transmit
    * Next put AF+upto 59 bytes of data and transmit.
    * Continue till all data in Pdata is transferred, lastly include
    * the contents of bLastChunk also
    */

    if(bIns != PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE)
    {
            wFrameLen = PHAL_MFNTAG42XDNA_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        /* Get the Frame length */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_GetFrameLength(
            pDataParams,
            &wFSD,
            &wFSC
            ));

       wFrameLen = wFSC - 9;

    }

    wTmp = wTotalLen;

    /* Force the date to enter into this loop if it is LRP mode */
    if (wTmp <= (wFrameLen - wCmdLen))
    {
        wApduLen = ((wCmdLen == 0x01 ) && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE ))? 0 : PHAL_MFNTAG42XDNA_WRAP_HDR_LEN;
        wCmdLen = ((wCmdLen == 0x01 ) && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE ))? 0 : wCmdLen;

        /* Send in one shot */
       pApdu[1] = bCmdBuff[0]; /* Desfire cmd code in INS */
        if(wCmdLen > 0)
        {
            pApdu[4] = (uint8_t)(wCmdLen + wTotalLen) - 0x01u;
        }

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_FIRST |
                (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                pApdu,
                wApduLen,
                &pRecv,
                &wRxlen));

            if( wCmdLen > 0)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT |
                    (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                    &bCmdBuff[1],
                    wCmdLen - 1,
                    &pRecv,
                    &wRxlen));
            }

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT |
                (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                pData,
                wDataLen1,
                &pRecv,
                &wRxlen));

            if (wLastChunkLen != 0x0000)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT |
                    (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                    bLastChunk,
                    wLastChunkLen,
                    &pRecv,
                    &wRxlen));
            }

            /* Le byte */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST |
                (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                &pApdu[2],
                (uint16_t)((pDataParams->dwPayLoadLen > 0xFE ) && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE ))? 0x02U : 0x01U,
                &pRecv,
                &wRxlen));

            status = (uint8_t)pRecv[wRxlen - 1];
            wRxlen -= 2;
        if (status != PH_ERR_SUCCESS)
        {
            /* Reset authentication status */
            if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
            {
                phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
            }
            return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, (uint8_t)status);
        }
        /* At this point, Handle the SetConfiguration command if LRP Mode is to be enabled.
         * In Desfire Lite, In set configuration command, for enabling LRP secure messaging type,
         * clearing the authenticate state at the end of the command processing is done.
         * Hence command response will be in PLAIN mode which shall be handled
         */
        if((pApdu[1] == PHAL_MFNTAG42XDNA_CMD_SET_CONFIG) &&
            (pDataParams->bCryptoMethod == PH_CRYPTOSYM_CONFIG_LRP))
        {
            /* Since ERROR handling is already done, just return SUCCESS */
            return PH_ERR_SUCCESS;
        }

        memcpy(pResp, pRecv, wRxlen); /* PRQA S 3200 */
    }
    else
    {
        if(wDataLen1 > 0x0200)
        {
            statusTmp = phalMfNtag42XDna_Sw_Int_SendDataAndAddDataToPICC(
                pDataParams,
                bIns,
                bCmdBuff,
                wCmdLen,
                pData,
                wDataLen1,
                bLastChunk,
                wLastChunkLen,
                pResp,
                &wRxlen
                );
        }
        else
        {
            statusTmp = phalMfNtag42XDna_Sw_Int_SendDataToPICC(
                pDataParams,
                bIns,
                (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE),
                bCmdBuff,
                wCmdLen,
                pData,
                wDataLen1,
                bLastChunk,
                wLastChunkLen,
                pResp,
                &wRxlen
                );
        }

        if ((statusTmp & PH_ERR_MASK) == PHAL_MFNTAG42XDNA_RESP_CHAINING)
        {
            return statusTmp;
        }

        if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS &&
            (statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
        {
            /* Reset authentication status */
            if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
            {
                phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
            }
            return statusTmp;
        }
    }

    if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
    {
        /* Increment the command counter */
        pDataParams->wCmdCtr++;

        if (wRxlen < 8 ) /* If no CMAC received */
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }

        memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */

        /* Load IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsMac,
            pDataParams->bIv,
            bIvLen
            ));

        /* copy CMAC received from card*/
        memcpy(bCMacCard, &pResp[wRxlen - 8], 8);  /* PRQA S 3200 */
        wRxlen -= 8;

        /*
        * Calculate MAC on RC || wCmdCtr || TI || RespData
        */
        pDataParams->bNoUnprocBytes = 0x00;
        pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = 0x00;
        pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr);
        pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
        memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pDataParams->bTi, PHAL_MFNTAG42XDNA_SIZE_TI); /* PRQA S 3200 */
        pDataParams->bNoUnprocBytes += PHAL_MFNTAG42XDNA_SIZE_TI;

        /*Required ?*/
        memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pResp, wRxlen); /* PRQA S 3200 */
        pDataParams->bNoUnprocBytes += (uint8_t)wRxlen;

        /* verify the MAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
            pDataParams->pUnprocByteBuff,
            pDataParams->bNoUnprocBytes,
            bCMAC,
            &bMacLen
            ));

        /* Truncate the MAC generated */
        phalMfNtag42XDna_Sw_Int_TruncateMac(bCMAC);

        if (memcmp(bCMacCard, bCMAC, 8) != 0)
        {
            phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }
    }
    else
    {
        /*Do Nothing. This is for PRQA compliance */
    }
    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfNtag42XDna_Sw_Int_Write_Plain(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t bCommOption, uint8_t * pData, uint16_t wDataLen)
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM status = 0;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[32];
    uint16_t    PH_MEMLOC_REM wFrameLen = 0;
    uint16_t    PH_MEMLOC_REM wTotalLen = 0;
    uint16_t    PH_MEMLOC_REM wTmp = 0;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFNTAG42XDNA_WRAPPEDAPDU_CLA, 0x00, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P1, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P2, 0x00 };
#ifdef NXPBUILD__PH_CRYPTOSYM
    uint8_t     PH_MEMLOC_REM bCMAC[PH_CRYPTOSYM_AES_BLOCK_SIZE];
#endif /* NXPBUILD__PH_CRYPTOSYM */
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint8_t     PH_MEMLOC_REM bMacLen = 0;
    uint8_t     PH_MEMLOC_REM bIvLen = 0;
    uint16_t    PH_MEMLOC_REM wWorkBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wFSD = 0;
    uint16_t    PH_MEMLOC_REM wFSC = 0;
    uint16_t    PH_MEMLOC_REM wApduLen = 0;
#ifdef  NXPBUILD__PH_CRYPTOSYM
    memset(bCMAC, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */
    memset(bCMacCard, 0x00, 8); /* PRQA S 3200 */
#endif /* NXPBUILD__PH_CRYPTOSYM */
    memset(bWorkBuffer, 0x00, 32); /* PRQA S 3200 */

#ifdef  NXPBUILD__PH_CRYPTOSYM
    if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
    {
        bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
#endif /* NXPBUILD__PH_CRYPTOSYM */

    if ((pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2) &&
        ((bCommOption & 0xF0U) == PHAL_MFNTAG42XDNA_COMMUNICATION_MACD))
    {
        if ((bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME))
        {
            memset(pDataParams->bIv, 0x00, bIvLen);  /* PRQA S 3200 */
            pDataParams->bNoUnprocBytes = 0;
#ifdef  NXPBUILD__PH_CRYPTOSYM
            /* Load Iv */
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                pDataParams->pCryptoDataParamsMac,
                pDataParams->bIv,
                bIvLen
                ));
#endif /* NXPBUILD__PH_CRYPTOSYM */

            /* Calculate MAC on Cmd || wCmdCtr || TI || CmdHeader || CmdData */
            bWorkBuffer[wWorkBufferLen++] = bCmdBuff[0];
            bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr);
            bWorkBuffer[wWorkBufferLen++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
            memcpy(&bWorkBuffer[wWorkBufferLen], pDataParams->bTi, PHAL_MFNTAG42XDNA_SIZE_TI); /* PRQA S 3200 */
            wWorkBufferLen += PHAL_MFNTAG42XDNA_SIZE_TI;
        }
        else
        {
            memcpy(bWorkBuffer, pDataParams->pUnprocByteBuff, pDataParams->bNoUnprocBytes); /* PRQA S 3200 */
            wWorkBufferLen = pDataParams->bNoUnprocBytes;
            pDataParams->bNoUnprocBytes = 0;
        }
        /* Check for presence of command header */
        if (wCmdLen > 1)
        {
            /* Calculate the total length of data for MAC calculation */
            wTmp = ((wCmdLen - 1) + (wWorkBufferLen));
            /* Since bWorkbuffer can accomodate 32 bytes, check for buffer overflow */
            if (wTmp > 32)
            {
                memcpy(&bWorkBuffer[wWorkBufferLen], &bCmdBuff[1], (32 - wWorkBufferLen)); /* PRQA S 3200 */
#ifdef  NXPBUILD__PH_CRYPTOSYM
                /* Calculate CMAC */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT,
                    bWorkBuffer,
                    32,
                    bCMAC,
                    &bMacLen
                    ));
#endif /* NXPBUILD__PH_CRYPTOSYM */
                /* Copy the remaining bCmdBuff into bWorkBuffer */
                memcpy(bWorkBuffer, &bCmdBuff[(32 - wWorkBufferLen) + 1], (wTmp - 32)); /* PRQA S 3200 */
                wWorkBufferLen = (wTmp - 32);
            }
            else
            {
                memcpy(&bWorkBuffer[wWorkBufferLen], &bCmdBuff[1], (wCmdLen - 1)); /* PRQA S 3200 */
                wWorkBufferLen += (wCmdLen - 1);
            }
        }
#ifdef  NXPBUILD__PH_CRYPTOSYM
        if ((bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE) != PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)
        {
            if (!(wDataLen))
            {
                /* Calculate CMAC in one shot */
                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | ((wTmp > 32) ? PH_EXCHANGE_BUFFER_LAST : PH_EXCHANGE_DEFAULT)),
                    bWorkBuffer,
                    wWorkBufferLen,
                    bCMAC,
                    &bMacLen
                    ));
            }
            else
            {
                wTmp = (PH_CRYPTOSYM_AES_BLOCK_SIZE - (wWorkBufferLen % PH_CRYPTOSYM_AES_BLOCK_SIZE));
                if (wDataLen < wTmp)
                {
                    wTmp = wDataLen;
                }
                memcpy(&bWorkBuffer[wWorkBufferLen], pData, wTmp); /* PRQA S 3200 */

                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | ((wTmp == wDataLen) ? PH_EXCHANGE_BUFFER_LAST : PH_EXCHANGE_BUFFER_CONT)),
                    bWorkBuffer,
                    wWorkBufferLen + wTmp,
                    bCMAC,
                    &bMacLen
                    ));

                if (wTmp != wDataLen)
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                        pDataParams->pCryptoDataParamsMac,
                        PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_LAST,
                        &pData[wTmp],
                        wDataLen - wTmp,
                        bCMAC,
                        &bMacLen
                        ));
                }
            }

            /* Truncate the MAC generated */
            phalMfNtag42XDna_Sw_Int_TruncateMac(bCMAC);
            bMacLen = 8;
        }
        else
        {
            if (!(wDataLen))
            {

                if(((wWorkBufferLen / PH_CRYPTOSYM_AES_BLOCK_SIZE) * PH_CRYPTOSYM_AES_BLOCK_SIZE) != 0U)
                {
                    /* Calculate CMAC */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                        pDataParams->pCryptoDataParamsMac,
                        (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                        bWorkBuffer,
                        ((wWorkBufferLen / PH_CRYPTOSYM_AES_BLOCK_SIZE) * PH_CRYPTOSYM_AES_BLOCK_SIZE),
                        bCMAC,
                        &bMacLen
                        ));
                }

                pDataParams->bNoUnprocBytes =(uint8_t) (wWorkBufferLen % PH_CRYPTOSYM_AES_BLOCK_SIZE);
                memcpy(pDataParams->pUnprocByteBuff, &bWorkBuffer[wWorkBufferLen - pDataParams->bNoUnprocBytes], pDataParams->bNoUnprocBytes); /* PRQA S 3200 */
            }
            else
            {
                wTmp = (PH_CRYPTOSYM_AES_BLOCK_SIZE - (wWorkBufferLen % PH_CRYPTOSYM_AES_BLOCK_SIZE));
                if (wDataLen < wTmp)
                {
                    wTmp = wDataLen;
                    pDataParams->bNoUnprocBytes = (uint8_t)(wTmp % PH_CRYPTOSYM_AES_BLOCK_SIZE);
                    memcpy(pDataParams->pUnprocByteBuff, &pData[wTmp - pDataParams->bNoUnprocBytes], pDataParams->bNoUnprocBytes); /* PRQA S 3200 */
                }
                memcpy(&bWorkBuffer[wWorkBufferLen], pData, (wTmp - pDataParams->bNoUnprocBytes)); /* PRQA S 3200 */

                PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                    pDataParams->pCryptoDataParamsMac,
                    (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT),
                    bWorkBuffer,
                    (wWorkBufferLen + (wTmp - pDataParams->bNoUnprocBytes)),
                    bCMAC,
                    &bMacLen
                    ));

                if (wTmp != wDataLen)
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                        pDataParams->pCryptoDataParamsMac,
                        PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_BUFFER_CONT,
                        &pData[wTmp],
                        (((wDataLen - wTmp) / PH_CRYPTOSYM_AES_BLOCK_SIZE) * PH_CRYPTOSYM_AES_BLOCK_SIZE),
                        bCMAC,
                        &bMacLen
                        ));

                    pDataParams->bNoUnprocBytes = (uint8_t)((wDataLen - wTmp) % PH_CRYPTOSYM_AES_BLOCK_SIZE);
                    memcpy(pDataParams->pUnprocByteBuff, &pData[wDataLen - pDataParams->bNoUnprocBytes], pDataParams->bNoUnprocBytes); /* PRQA S 3200 */
                }
            }
        }
#endif /* NXPBUILD__PH_CRYPTOSYM */
        if ((bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE) == PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)
        {
            bMacLen = 0;
        }
    }
    else
    {
        /* Else statement due to else if above. */
        bMacLen = 0;
    }

    /* Update wTotalLen = datalen + CMAClen*/
    wTotalLen = wDataLen + bMacLen;

    if(bIns != PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE)
    {
            wFrameLen = PHAL_MFNTAG42XDNA_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        /* Get the Frame length */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_GetFrameLength(
            pDataParams,
            &wFSD,
            &wFSC
            ));

        wFrameLen = wFSC - 9;
    }

    if (wTotalLen == 0x0000)
    {
        /* Single frame cmd without any data. Just send it */
        status = phalMfNtag42XDna_ExchangeCmd(
            pDataParams,
            pDataParams->pPalMifareDataParams,
            pDataParams->bWrappedMode,
            bCmdBuff,
            wCmdLen,
            &pRecv,
            &wRxlen
            );
        if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
        {
            /* Reset authentication status */
            if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
            {
                phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
            }
            return PH_ADD_COMPCODE(status, PH_COMP_AL_MFNTAG42XDNA);
        }
        if (wRxlen > 32)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }
        memcpy(bWorkBuffer, pRecv, wRxlen);  /* PRQA S 3200 */
    }
    else
    {
        if(bIns != PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE)
        {
                wFrameLen = PHAL_MFNTAG42XDNA_MAXWRAPPEDAPDU_SIZE;
         }
        else
        {
            /* Get the Frame length */
            PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_GetFrameLength(
                pDataParams,
                &wFSD,
                &wFSC
                ));


                wFrameLen = wFSC - 9;

        }

        wTmp = wTotalLen;

        if (wTmp <= (wFrameLen - wCmdLen))
        {
            wApduLen = ((wCmdLen == 0x01 ) && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE ))? 0 : PHAL_MFNTAG42XDNA_WRAP_HDR_LEN;
            wCmdLen = ((wCmdLen == 0x01 ) && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE ))? 0 : wCmdLen;

            /* Send in one shot */
            if (pDataParams->bWrappedMode)
            {
                pApdu[1] = bCmdBuff[0]; /* Desfire cmd code in INS */

                if(wCmdLen > 0)
                {
                    pApdu[4] = (uint8_t)(wCmdLen + wTotalLen) - 0x01u;
                }

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_FIRST  |
                    (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                    pApdu,
                    wApduLen,
                    &pRecv,
                    &wRxlen));

                if( wCmdLen > 0)
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                        pDataParams->pPalMifareDataParams,
                        PH_EXCHANGE_BUFFER_CONT |
                        (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                        &bCmdBuff[1],
                        wCmdLen - 1,
                        &pRecv,
                        &wRxlen));
                }

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT |
                    (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                    pData,
                    wDataLen,
                    &pRecv,
                    &wRxlen));

#ifdef NXPBUILD__PH_CRYPTOSYM
                if (bMacLen != 0x0000)
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                        pDataParams->pPalMifareDataParams,
                        PH_EXCHANGE_BUFFER_CONT |
                        (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                        bCMAC,
                        bMacLen,
                        &pRecv,
                        &wRxlen));
                }
#endif /* NXPBUILD__PH_CRYPTOSYM */
                /* Le byte */
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_LAST | (((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE)) ? PH_EXCHANGE_TXCHAINING : 0),
                    &pApdu[2],
                    (uint16_t)((pDataParams->dwPayLoadLen > 0xFE ) && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE ))? 0x02U : 0x01U,
                    &pRecv,
                    &wRxlen));

                    status = pRecv[wRxlen - 1];
                    wRxlen -= 2;
            }
            if ((status != PH_ERR_SUCCESS) &&
                ((status & PH_ERR_MASK) != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME))
            {

                /* Reset authentication status */
                if(pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
                {
                    phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
                }

                return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, (uint8_t)status);
            }

            memcpy(bWorkBuffer, pRecv, wRxlen); /* PRQA S 3200 */
        }
        else
        {
#ifdef NXPBUILD__PH_CRYPTOSYM
            /* Send command and data. Chain data to PICC */
            if(wDataLen > 0x0200)
            {
                statusTmp = phalMfNtag42XDna_Sw_Int_SendDataAndAddDataToPICC(
                pDataParams,
                bIns,
                bCmdBuff,
                wCmdLen,
                pData,
                wDataLen,
                bCMAC,
                bMacLen,
                bWorkBuffer,
                &wRxlen
                );
            }
            else
            {
                statusTmp = phalMfNtag42XDna_Sw_Int_SendDataToPICC(
                    pDataParams,
                    bIns,
                    (bCommOption & PHAL_MFNTAG42XDNA_MAC_DATA_INCOMPLETE),
                    bCmdBuff,
                    wCmdLen,
                    pData,
                    wDataLen,
                    bCMAC,
                    bMacLen,
                    bWorkBuffer,
                    &wRxlen
                    );
            }

            if ((statusTmp & PH_ERR_MASK) == PHAL_MFNTAG42XDNA_RESP_CHAINING)
            {
                return statusTmp;
            }

            if ((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS &&
                (statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
            {
                /* Reset authentication status */
                if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
                {
                    phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
                }
                return statusTmp;
            }
#endif /* NXPBUILD__PH_CRYPTOSYM */
        }
    }

    if ((status == PH_ERR_SUCCESS) && ((bCommOption & PHAL_MFNTAG42XDNA_AUTHENTICATE_RESET) == PHAL_MFNTAG42XDNA_AUTHENTICATE_RESET))
    {
        /* Reset authentication status */
        if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
        {
            phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
        }
        return PH_ERR_SUCCESS;
    }

    if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
    {
        if  ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING) /* if no chaining ongoing */
        {

            if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                /* Increment the command counter.
                *  This increments irrespective of Plain mode or MAC mode.
                */
                pDataParams->wCmdCtr++;
            }

            if ((bCommOption & 0xF0U) == PHAL_MFNTAG42XDNA_COMMUNICATION_MACD)
            {
                if (wRxlen < 8 ) /* If no CMAC received */
                {
                        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
                }
#ifdef NXPBUILD__PH_CRYPTOSYM
                if ((bCmdBuff[0] != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME))
                {
                    /* Load IV */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
                        pDataParams->pCryptoDataParamsMac,
                        pDataParams->bIv,
                        bIvLen
                        ));
                }

                if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    /* copy CMAC received from card*/
                    memcpy(bCMacCard, &bWorkBuffer[wRxlen - 8], 8);  /* PRQA S 3200 */
                    wRxlen -= 8;

                    /*
                    * Calculate MAC on RC || wCmdCtr || TI || RespData
                    * bWorkBuffer is used as receive buffer so pDataParams->pUnprocByteBuff is used
                    */
                    pDataParams->bNoUnprocBytes = 0x00;
                    pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = 0x00;
                    pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr);
                    pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes++] = (uint8_t)(pDataParams->wCmdCtr >> 8);
                    memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], pDataParams->bTi, PHAL_MFNTAG42XDNA_SIZE_TI); /* PRQA S 3200 */
                    pDataParams->bNoUnprocBytes += PHAL_MFNTAG42XDNA_SIZE_TI;

                    /* TBD:Required */
                    memcpy(&pDataParams->pUnprocByteBuff[pDataParams->bNoUnprocBytes], bWorkBuffer, wRxlen); /* PRQA S 3200 */
                    pDataParams->bNoUnprocBytes += (uint8_t)wRxlen;

                    /* verify the MAC */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
                        pDataParams->pCryptoDataParamsMac,
                        (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
                        pDataParams->pUnprocByteBuff,
                        pDataParams->bNoUnprocBytes,
                        bCMAC,
                        &bMacLen
                        ));

                    /* Truncate the MAC generated */
                    phalMfNtag42XDna_Sw_Int_TruncateMac(bCMAC);

                    /* Compare the CMAC from card and CMAC calculated */
                    if (memcmp(bCMacCard, bCMAC, 8) != 0)
                    {
                        phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
                        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFNTAG42XDNA);
                    }
                }
#endif /* NXPBUILD__PH_CRYPTOSYM */
            }

        }

    }
    else
    {
        /* Should not get more bytes than the status bytes in case
        of no authentication */
        if (wRxlen > 0)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }
    }

    return PH_ERR_SUCCESS;
}

void phalMfNtag42XDna_Sw_Int_ResetAuthStatus(phalMfNtag42XDna_Sw_DataParams_t * pDataParams)
{
    memset(pDataParams->bSesAuthENCKey, 0x00, sizeof(pDataParams->bSesAuthENCKey)); /* PRQA S 3200 */
    memset(pDataParams->bSesAuthMACKey, 0x00, sizeof(pDataParams->bSesAuthMACKey)); /* PRQA S 3200 */
    pDataParams->bKeyNo = 0xFF;
    memset(pDataParams->bIv, 0x00, sizeof(pDataParams->bIv)); /* PRQA S 3200 */
    pDataParams->bAuthMode = PHAL_MFNTAG42XDNA_NOT_AUTHENTICATED;
    pDataParams->bCryptoMethod = 0xFF;
    pDataParams->wCmdCtr = 0;
    memset(pDataParams->bTi, 0x00, PHAL_MFNTAG42XDNA_SIZE_TI);  /* PRQA S 3200 */
    pDataParams->bNoUnprocBytes = 0;
    pDataParams->bLastBlockIndex = 0;
}

phStatus_t phalMfNtag42XDna_Sw_Int_SendDataToPICC(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t bCommOption,
    uint8_t * pCmd, uint16_t wCmdLen, uint8_t * pData, uint16_t wDataLen, uint8_t * bLastChunk, uint16_t wLastChunkLen,
    uint8_t * pResp, uint16_t * pRespLen)
{
    /* Utility function to send data to PICC if more then wFrameLen*/
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bStatusByte;
    uint16_t    PH_MEMLOC_REM wIndexDataLen = 0;
    uint16_t    PH_MEMLOC_REM wFrameLen;
    uint8_t     PH_MEMLOC_REM pApdu[7] = { PHAL_MFNTAG42XDNA_WRAPPEDAPDU_CLA, 0x00, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P1, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P2, 0x00, 0x00, 0x00};
    uint8_t     PH_MEMLOC_REM * pRecv = NULL;
    uint8_t     PH_MEMLOC_REM bExtended7816 = 0;
    uint8_t     PH_MEMLOC_REM bCommOptionTmp;
    uint16_t    PH_MEMLOC_REM wFSD = 0;
    uint16_t    PH_MEMLOC_REM wFSC = 0;
    uint16_t    PH_MEMLOC_REM wDataToBeSent = 0;
    uint16_t    PH_MEMLOC_REM wCopyDataLen = 0;
    uint16_t    PH_MEMLOC_REM wTmpDataLen = 0;
    uint16_t    PH_MEMLOC_REM wCopyLastChunkLen = 0;
    uint16_t    PH_MEMLOC_REM wTmpLastChunkLen = 0;
    uint16_t    PH_MEMLOC_REM wIndexLastChunkLen = 0;
    uint16_t    PH_MEMLOC_REM wApduHeaderLen = 0;
    uint16_t    PH_MEMLOC_REM wLeFieldLen = 0;
    uint16_t    PH_MEMLOC_REM wIndexCmdLen = 1;
    uint16_t    PH_MEMLOC_REM wTmpData;
    uint16_t    PH_MEMLOC_REM wTemLen = 0;

    if(bIns != PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE)
    {

            wFrameLen = PHAL_MFNTAG42XDNA_MAXWRAPPEDAPDU_SIZE;
            wApduHeaderLen = PHAL_MFNTAG42XDNA_WRAP_HDR_LEN;

    }
    else
    {
        /* Get the Frame length */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_GetFrameLength(
            pDataParams,
            &wFSD,
            &wFSC
            ));


            bExtended7816 = (uint8_t)(pDataParams->dwPayLoadLen > 0xFEU)? 1 : 0;
            /* if Lc is more then 0xFF, length of Lc should be 3 bytes */
            wApduHeaderLen = PHAL_MFNTAG42XDNA_WRAP_HDR_LEN + (bExtended7816 ? 2 : 0);

        wFrameLen = wFSC - 4;
    }

    /* Send the data to PICC */
    wTmpDataLen = wDataLen;
    wTmpLastChunkLen = wLastChunkLen;

    do
    {
        /* If Ins mode and wrapped mode are enable Le is sent with the last packet. So update Le when only last packet is sent */
        wLeFieldLen = ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (pDataParams->bWrappedMode == 1)) ? 1 : 0;

        /* In case of wrapped mode, cmd byte is added as a part of Apdu Header. */
        wIndexCmdLen = wApduHeaderLen ? 1: 0;

        /* this If condition is added to suppress QAC warning */
        wTemLen = wCmdLen;
        if(wCmdLen > 0)
        {
            wTemLen = wCmdLen - wIndexCmdLen ;
        }
        /* If Ins mode and wrapped mode are enable Le is sent with the last packet */
        if(pDataParams->bWrappedMode && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && ((wTmpDataLen + wTmpLastChunkLen+ wTemLen) < wFrameLen))
        {
            wLeFieldLen =  1 + bExtended7816 ;
        }
        if(wTmpDataLen > 0)
        {
            wCopyDataLen = (wTmpDataLen < (wFrameLen - (wTemLen + wApduHeaderLen)))?
                            wTmpDataLen : (wFrameLen - (wTemLen + wApduHeaderLen));
        }

        if(wTmpLastChunkLen > 0)
        {
            wTmpData = wTemLen + wCopyDataLen + wApduHeaderLen;
            wCopyLastChunkLen = (wTmpLastChunkLen < (wFrameLen - wTmpData))? wTmpLastChunkLen : (wFrameLen - wTmpData);
        }

        /* remaining data to be sent */
        /* this If condition is added to suppress QAC warning */
        wDataToBeSent = (wTmpDataLen - wCopyDataLen) + (wTmpLastChunkLen - wCopyLastChunkLen);

        wCmdLen = ((wCmdLen == 0x01 ) && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE ))? 0 : wCmdLen;

        bCommOptionTmp = (uint8_t)(((!wDataToBeSent) && ( ! bCommOption )) ? 0 : 1);

        if (pDataParams->bWrappedMode)
        {
            pApdu[1] = pCmd[0]; /* Desfire cmd code in INS */

            /* in case of ISO chaining mode, total length of data should be sent with the first frame*/
            if(bExtended7816 && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE))
            {
                pDataParams->dwPayLoadLen = pDataParams->dwPayLoadLen +wTemLen ;
                pApdu[4] = (uint8_t)((pDataParams->dwPayLoadLen >> 16)& 0xFF);
                pApdu[5] = (uint8_t)((pDataParams->dwPayLoadLen >> 8)& 0xFF);
                pApdu[6] = (uint8_t) (pDataParams->dwPayLoadLen & 0xFF);
            }
            else
            {
                pApdu[4] = (uint8_t)(wTemLen + ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) ?  pDataParams->dwPayLoadLen  : (wCopyDataLen + wCopyLastChunkLen)));
            }

            wApduHeaderLen = ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && (wTemLen == 0)) ? 0:wApduHeaderLen;

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_FIRST |  ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE && bCommOptionTmp) ? PH_EXCHANGE_TXCHAINING : 0),
                pApdu,
                wApduHeaderLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE && bCommOptionTmp) ? PH_EXCHANGE_TXCHAINING : 0),
                &pCmd[wIndexCmdLen],
                wTemLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE && bCommOptionTmp) ? PH_EXCHANGE_TXCHAINING : 0),
                &pData[wIndexDataLen],
                wCopyDataLen,
                &pRecv,
                pRespLen));

            /* send last chunk */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE && bCommOptionTmp) ? PH_EXCHANGE_TXCHAINING : 0),
                &bLastChunk[wIndexLastChunkLen],
                wCopyLastChunkLen,
                &pRecv,
                pRespLen));

            wLeFieldLen = ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE) && bCommOptionTmp) ? 0 : wLeFieldLen;

            /* Le byte */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE && bCommOptionTmp) ? PH_EXCHANGE_TXCHAINING : 0),
                &pApdu[2],
                wLeFieldLen,
                &pRecv,
                pRespLen));
        }

        /* copy number of data sent */
        wIndexDataLen += wCopyDataLen;
        wIndexLastChunkLen += wCopyLastChunkLen;

        /* copy the remaining data to be sent */
        /* this If condition is added to suppress QAC warning */
        if(wTmpDataLen > 0)
        {
            wTmpDataLen = wTmpDataLen - wCopyDataLen;
        }

        /* this If condition is added to suppress QAC warning */
        if(wTmpLastChunkLen > 0)
        {
            wTmpLastChunkLen = wTmpLastChunkLen - wCopyLastChunkLen;
        }

        wCopyDataLen = 0;
        wCopyLastChunkLen = 0;

        /* in case of 14443-4 chaining R-block that indicates a positive acknowledge */
        if(bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE && bCommOptionTmp)
        {
            bStatusByte = (uint8_t)((pRecv[0] & 0xF0) == 0xA0) ? PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME : PH_ERR_PROTOCOL_ERROR;
        }
        else
        {

            memcpy(pResp, pRecv, (*pRespLen) - 2); /* PRQA S 3200 */
            bStatusByte = pRecv[(*pRespLen) - 1];
            (*pRespLen) -= 2;

        }

        if ((bStatusByte != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME) &&
            (bStatusByte != PH_ERR_SUCCESS))
        {

            return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, bStatusByte);
        }

        /* Success returned even before writing all data? protocol error */
        if ((bStatusByte == PH_ERR_SUCCESS) && (bCommOptionTmp != 0))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }

        if(bStatusByte != 0x00)
        {
            pCmd[0] = PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME;
            wCmdLen = (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE)? 0: 1;
            wApduHeaderLen = (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE)? 0: wApduHeaderLen;
        }

    }while(wDataToBeSent);

    return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

phStatus_t phalMfNtag42XDna_Sw_Int_SendDataAndAddDataToPICC(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bIns, uint8_t * pCmd,
    uint16_t wCmdLen, uint8_t * pData, uint16_t wDataLen, uint8_t * pAddData, uint16_t wAddDataLen, uint8_t * pResp, uint16_t * pRespLen)
{
    /* Utility function to send encrypted data to PICC as and when it is available from SAM */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bStatusByte = 0;
    uint16_t    PH_MEMLOC_REM wIndex = 0;
    uint16_t    PH_MEMLOC_REM wDataLeft;
    uint16_t    PH_MEMLOC_REM wFrameLen;
    uint8_t     PH_MEMLOC_REM pApdu[7] = { PHAL_MFNTAG42XDNA_WRAPPEDAPDU_CLA, 0x00, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P1, PHAL_MFNTAG42XDNA_WRAPPEDAPDU_P2, 0x00, 0x00, 0x00 };
    uint8_t     PH_MEMLOC_REM pLe[2] = {0x00, 0x00};
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint16_t    PH_MEMLOC_REM wFSD = 0;
    uint16_t    PH_MEMLOC_REM wFSC = 0;
    uint16_t    PH_MEMLOC_REM wrappedApduHeaderLen = PHAL_MFNTAG42XDNA_WRAP_HDR_LEN;
    uint16_t    PH_MEMLOC_REM wrappedApduTrailerLen = 1;
    uint8_t     PH_MEMLOC_REM bIsExtended7816 = 0;
    uint16_t    PH_MEMLOC_REM wHeaderIdx = 0;
    uint16_t    PH_MEMLOC_REM wCmdIdx = 0;
    uint16_t    PH_MEMLOC_REM wAddDataIdx = 0;
    uint16_t    PH_MEMLOC_REM wTrailerIdx = 0;
    uint16_t    PH_MEMLOC_REM wWrappedApduHeaderLenLeft = 0;
    uint16_t    PH_MEMLOC_REM wWrappedApduTrailerLenLeft = 0;
    uint16_t    PH_MEMLOC_REM wCmdLenLeft = 0;
    uint16_t    PH_MEMLOC_REM wAddDataLenLeft = 0;
    uint16_t    PH_MEMLOC_REM wAddDataLenTotal = 0;
    uint16_t    PH_MEMLOC_REM wCmdLenTotal = 0;
    uint16_t    PH_MEMLOC_REM wDataLenTotal = 0;
    uint16_t    PH_MEMLOC_REM wMoreDataToTransmit = 0;

    if(bIns != PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED)
    {
        if (pDataParams->bWrappedMode)
        {
            wFrameLen = PHAL_MFNTAG42XDNA_MAXDFAPDU_SIZE;
        }
        else
        {
            wFrameLen = PHAL_MFNTAG42XDNA_MAXDFAPDU_SIZE;
        }
    }
    else
    {
        /* Get the Frame length */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfNtag42XDna_Sw_Int_GetFrameLength(
            pDataParams,
            &wFSD,
            &wFSC
            ));

        if (pDataParams->bWrappedMode)
        {
            bIsExtended7816 = (uint8_t)((wCmdLen + wDataLen + wAddDataLen - 0x01u) > 255);
            wrappedApduHeaderLen += (bIsExtended7816 ? 2 : 0);
            wrappedApduTrailerLen += (bIsExtended7816 ? 1 : 0);
        }
        wFrameLen = wFSC - 4;
    }

    wWrappedApduHeaderLenLeft = wrappedApduHeaderLen;
    wCmdLenLeft = wCmdLenTotal = (pDataParams->bWrappedMode) ? ((wCmdLen > 0) ? wCmdLen - 1 : 0) : wCmdLen; /* subtract instruction byte */
    wDataLeft = wDataLenTotal = wDataLen;
    wAddDataLenLeft = wAddDataLenTotal = wAddDataLen;
    wWrappedApduTrailerLenLeft = wrappedApduTrailerLen;

    do
    {
        if (pDataParams->bWrappedMode)
        {
            if(bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED)
            {
                if(wWrappedApduHeaderLenLeft > 0)
                    wrappedApduHeaderLen = (wFrameLen / wWrappedApduHeaderLenLeft) ? wWrappedApduHeaderLenLeft : wFrameLen % wWrappedApduHeaderLenLeft;
                else
                    wrappedApduHeaderLen = 0;
                if(wCmdLenLeft > 0)
                    wCmdLen = ((wFrameLen - wrappedApduHeaderLen) / wCmdLenLeft) ? wCmdLenLeft : (wFrameLen - wrappedApduHeaderLen) % wCmdLenLeft;
                else
                    wCmdLen = 0;
                if(wDataLeft > 0)
                    wDataLen = ((wFrameLen - wrappedApduHeaderLen - wCmdLen) / wDataLeft) ? wDataLeft : (wFrameLen - wrappedApduHeaderLen - wCmdLen) % wDataLeft;
                else
                    wDataLen = 0;
                if(wAddDataLenLeft > 0)
                    wAddDataLen = ((wFrameLen - wrappedApduHeaderLen - wCmdLen - wDataLen) / wAddDataLenLeft)
                        ? wAddDataLenLeft
                        : (wFrameLen - wrappedApduHeaderLen - wCmdLen - wDataLen) % wAddDataLenLeft;
                else
                    wAddDataLen = 0;
                if(wWrappedApduTrailerLenLeft > 0)
                    wrappedApduTrailerLen = ((wFrameLen - wrappedApduHeaderLen - wCmdLen - wDataLen - wAddDataLen) / wWrappedApduTrailerLenLeft)
                        ? wWrappedApduTrailerLenLeft
                        : ((wFrameLen - wrappedApduHeaderLen - wCmdLen - wDataLen - wAddDataLen) % wWrappedApduTrailerLenLeft);
            }
            else
            {
                if(wWrappedApduHeaderLenLeft > 0)
                    wrappedApduHeaderLen = (wFrameLen / wWrappedApduHeaderLenLeft) ? wWrappedApduHeaderLenLeft : wFrameLen % wWrappedApduHeaderLenLeft;
                else
                    wrappedApduHeaderLen = 0;
                if(wWrappedApduTrailerLenLeft > 0)
                    wrappedApduTrailerLen = ((wFrameLen - wrappedApduHeaderLen) / wWrappedApduTrailerLenLeft)
                        ? wWrappedApduTrailerLenLeft
                        : ((wFrameLen - wrappedApduHeaderLen) % wWrappedApduTrailerLenLeft);
                else
                    wrappedApduTrailerLen = 0;
                if(wCmdLenLeft > 0)
                    wCmdLen = ((wFrameLen - wrappedApduHeaderLen - wrappedApduTrailerLen) / wCmdLenLeft)
                        ? wCmdLenLeft
                        : (wFrameLen - wrappedApduHeaderLen - wrappedApduTrailerLen) % wCmdLenLeft;
                else
                    wCmdLen = 0;
                if(wDataLeft > 0)
                    wDataLen = ((wFrameLen - wrappedApduHeaderLen - wrappedApduTrailerLen - wCmdLen) / wDataLeft)
                        ? wDataLeft
                        : (wFrameLen - wrappedApduHeaderLen - wrappedApduTrailerLen - wCmdLen) % wDataLeft;
                else
                    wDataLen = 0;
                if(wAddDataLenLeft > 0)
                    wAddDataLen = ((wFrameLen - wrappedApduHeaderLen - wrappedApduTrailerLen - wCmdLen - wDataLen) / wAddDataLenLeft)
                        ? wAddDataLenLeft
                        : (wFrameLen - wrappedApduHeaderLen - wrappedApduTrailerLen - wCmdLen - wDataLen) % wAddDataLenLeft;
                else
                    wAddDataLen = 0;
            }

            pApdu[1] = pCmd[0]; /* Desfire cmd code in INS */
            if(bIsExtended7816 && (bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED))
            {
                pApdu[6] = (uint8_t)(wCmdLenTotal +  wDataLenTotal + wAddDataLenTotal);
                pApdu[5] = (uint8_t)((wCmdLenTotal +  wDataLenTotal + wAddDataLenTotal)>>8);
                /*pApdu[4] = 0; */
            }
            else
                pApdu[4] = (uint8_t)(wCmdLen + ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED) ? wDataLenTotal + wAddDataLenTotal : wDataLen + wAddDataLen));

            wMoreDataToTransmit = (wWrappedApduHeaderLenLeft - wrappedApduHeaderLen) +
                                  (wCmdLenLeft - wCmdLen) +
                                  (wDataLeft - wDataLen) +
                                  (wAddDataLenLeft - wAddDataLen) +
                                  (wWrappedApduTrailerLenLeft - wrappedApduTrailerLen);

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_FIRST | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit) ? PH_EXCHANGE_TXCHAINING : 0),
                &pApdu[wHeaderIdx],
                wrappedApduHeaderLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit) ? PH_EXCHANGE_TXCHAINING : 0),
                &pCmd[1 + wCmdIdx],
                wCmdLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit) ? PH_EXCHANGE_TXCHAINING : 0),
                &pData[wIndex],
                wDataLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit) ? PH_EXCHANGE_TXCHAINING : 0),
                &pAddData[wAddDataIdx],
                wAddDataLen,
                &pRecv,
                pRespLen));

            /* Le byte */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit) ? PH_EXCHANGE_TXCHAINING : 0),
                &pLe[wTrailerIdx],
                wrappedApduTrailerLen,
                &pRecv,
                pRespLen));

            if(bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED)
            {
                wHeaderIdx += wrappedApduHeaderLen;
                wWrappedApduHeaderLenLeft -= wrappedApduHeaderLen;
                wCmdIdx += wCmdLen;
                wCmdLenLeft -= wCmdLen;
                wTrailerIdx += wrappedApduTrailerLen;
                wWrappedApduTrailerLenLeft -= wrappedApduTrailerLen;
            }
        }
        else
        {
            if(wCmdLenLeft > 0)
                wCmdLen = ((wFrameLen) / wCmdLenLeft) ? wCmdLenLeft : (wFrameLen) % wCmdLenLeft;
            else
                wCmdLen = 0;
            if(wDataLeft > 0)
                wDataLen = ((wFrameLen - wCmdLen) / wDataLeft) ? wDataLeft : (wFrameLen - wCmdLen) % wDataLeft;
            else
                wDataLen = 0;
            if(wAddDataLenLeft > 0)
                wAddDataLen = ((wFrameLen - wCmdLen - wDataLen) / wAddDataLenLeft) ? wAddDataLenLeft : (wFrameLen - wCmdLen - wDataLen) % wAddDataLenLeft;
            else
                wAddDataLen = 0;

            wMoreDataToTransmit = (wCmdLenLeft - wCmdLen) +
                                  (wDataLeft - wDataLen) +
                                  (wAddDataLenLeft - wAddDataLen);

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_FIRST | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit) ? PH_EXCHANGE_TXCHAINING : 0),
                &pCmd[wCmdIdx],
                wCmdLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit) ? PH_EXCHANGE_TXCHAINING : 0),
                &pData[wIndex],
                wDataLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST | ((bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit) ? PH_EXCHANGE_TXCHAINING : 0),
                &pAddData[wAddDataIdx],
                wAddDataLen,
                &pRecv,
                pRespLen));

            wCmdIdx += wCmdLen;
            wCmdLenLeft -= wCmdLen;
        }

        wIndex += wDataLen;
        wDataLeft -= wDataLen;
        wAddDataIdx += wAddDataLen;
        wAddDataLenLeft -= wAddDataLen;

        /* in case of BIGISO, iso chaining is expected, and therefore R(ACK) block*/
        if(bIns == PHAL_MFNTAG42XDNA_ISO_CHAINING_MODE_MAPPED && wMoreDataToTransmit)
        {
            /* in case of ACK */
            if((pRecv[0] & 0xF0) == 0xA0)
            {
                continue;
            }
        }

        if (pDataParams->bWrappedMode)
        {
            if((*pRespLen)<2)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
            }
            memcpy(pResp, pRecv, (*pRespLen) - 2); /* PRQA S 3200 */
            bStatusByte = pRecv[(*pRespLen) - 1];
            (*pRespLen) -= 2;
        }
        else
        {
            if((*pRespLen)<1)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
            }
            memcpy(pResp, &pRecv[1], (*pRespLen) - 1); /* PRQA S 3200 */
            bStatusByte = pRecv[0];
            (*pRespLen) -= 1;
        }

        if ((bStatusByte != PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME) &&
            (bStatusByte != PH_ERR_SUCCESS))
        {

            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEISO) ||
                (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEAES))
            {
                phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
            }

            return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, bStatusByte);
        }

        /* Success returned even before writing all data? protocol error */
        if ((bStatusByte == PH_ERR_SUCCESS) && (wDataLeft != 0))
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEISO) ||
                (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEAES))
            {
                phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
            }

            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }

        if(bStatusByte != 0x00)
        {
            pCmd[0] = PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME;
            wCmdIdx = 0;
            wCmdLenLeft = (pDataParams->bWrappedMode) ? 0 : 1;
        }
    }
    while(wMoreDataToTransmit);

    return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

phStatus_t phalMfNtag42XDna_Sw_Int_IsoRead(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint16_t wOption, uint8_t * bCmdBuff,
    uint16_t wCmdLen, uint8_t ** ppRxBuffer, uint32_t * pBytesRead)
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxBufferSize;
    uint32_t    PH_MEMLOC_REM wNextPos;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM bBackupBytes[3];
#ifdef NXPBUILD__PH_CRYPTOSYM
    uint8_t     PH_MEMLOC_REM bMacLenComputed = 0;
    uint8_t     PH_MEMLOC_REM bCMacCard[8];
    uint8_t     PH_MEMLOC_REM bCMAC[16];
    uint8_t     PH_MEMLOC_REM bMacLen;
    uint8_t     PH_MEMLOC_REM bIvLen;

    memset(bCMAC, 0, 16);/* PRQA S 3200 */


    bIvLen = PH_CRYPTOSYM_DES_BLOCK_SIZE;


    if ( wOption == PH_EXCHANGE_DEFAULT )
    {
        if(pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEEV2)
        {
            memset(pDataParams->bIv, 0x00, bIvLen);  /* PRQA S 3200 */
        }
    }
#endif /* NXPBUILD__PH_CRYPTOSYM */

    status = phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        (wOption & PH_EXCHANGE_MODE_MASK),
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        (uint16_t *)pBytesRead
        );

    /* First put everything on the reader Rx buffer upto buffer size - 60 */
    pRecv = *ppRxBuffer;

    if ((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
#ifdef NXPBUILD__PH_CRYPTOSYM
        /* Authentication should be reset */
        phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
#endif /* NXPBUILD__PH_CRYPTOSYM */
        return status;
    }

    while ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
            &wRxBufferSize
            ));

        wNextPos = *pBytesRead;
        memcpy(bBackupBytes, &pRecv[wNextPos - 3], 3); /* PRQA S 3200 */

        if (wNextPos + PHAL_MFNTAG42XDNA_MAX_FRAME_SIZE >= wRxBufferSize)
        {
            /* Calculate partical cmac if authenticated and return PH_ERR_SUCCESS_CHAINING */
            break;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            (uint16_t)wNextPos
            ));
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_RXCHAINING,
            bCmdBuff,
            wCmdLen,
            ppRxBuffer,
            (uint16_t *)pBytesRead
            );

        /* Put back the backed up bytes */
        memcpy(&pRecv[wNextPos - 3], bBackupBytes, 3); /* PRQA S 3200 */

        if ((status != PH_ERR_SUCCESS) &&
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
        {
#ifdef NXPBUILD__PH_CRYPTOSYM
            /* Authentication should be reset */
            phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
#endif /* NXPBUILD__PH_CRYPTOSYM */
            return status;
        }
    }

    /* The data is now in *ppRxBuffer, length = wRxlen */
#ifdef NXPBUILD__PH_CRYPTOSYM
    /* Size of MAC bytes */
    bMacLen =  0x08;
#endif /* NXPBUILD__PH_CRYPTOSYM */
    if (status == PH_ERR_SUCCESS)
    {
        statusTmp = (*ppRxBuffer)[*pBytesRead - 2]; /* SW1 */
        statusTmp = statusTmp << 8; /* Shift SW1 to MSB */
        statusTmp |= (*ppRxBuffer)[*pBytesRead - 1]; /* SW2 */

        if (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_NOT_AUTHENTICATED ||
           (pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATE && ((wOption & PH_EXCHANGE_CUSTOM_BITS_MASK) == PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN)))
        {
            *pBytesRead -= 2;
            return phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, statusTmp);
        }
        statusTmp = phalMfNtag42XDna_Int_ComputeErrorResponse(pDataParams, statusTmp);

        if (statusTmp != PH_ERR_SUCCESS)
        {
#ifdef NXPBUILD__PH_CRYPTOSYM
            /* Authentication should be reset */
            phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
#endif /* NXPBUILD__PH_CRYPTOSYM */
            return statusTmp;
        }
#ifdef NXPBUILD__PH_CRYPTOSYM
        /* Verify MAC. Dont consider SW1SW2 as MAC bytes */
        memcpy(bCMacCard, &(*ppRxBuffer)[*pBytesRead - ( bMacLen + 2)], bMacLen); /* PRQA S 3200 */

        /* Subtract the MAC bytes */
        *pBytesRead -= ( bMacLen + 2);

        (*ppRxBuffer)[(*pBytesRead)] = 0x00;

        if (memcmp(bCMAC, bCMacCard, bMacLen) != 0x00)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }

        if(pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEISO ||
           pDataParams->bAuthMode == PHAL_MFNTAG42XDNA_AUTHENTICATEAES)
        {
            /* Update IV */
            memcpy(pDataParams->bIv, bCMAC, bMacLenComputed); /* PRQA S 3200 */
        }
        #endif /* NXPBUILD__PH_CRYPTOSYM */
    }
    else
    {

    }
    return PH_ADD_COMPCODE((status & PH_ERR_MASK), PH_COMP_AL_MFNTAG42XDNA);
}

void phalMfNtag42XDna_Sw_Int_TruncateMac(uint8_t * pMac)
{
    uint8_t PH_MEMLOC_REM bIndex;
    uint8_t PH_MEMLOC_REM bIndex2;

    for (bIndex = 1, bIndex2 = 0; bIndex < 16; bIndex += 2, bIndex2++)
    {
        pMac[bIndex2] = pMac[bIndex];
    }
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_Int_ComputeIv(uint8_t bIsResponse, uint8_t * pTi, uint16_t wCmdCtr, uint8_t * pIv)
{
    uint8_t PH_MEMLOC_REM bIndex = 0;
    uint8_t PH_MEMLOC_REM bCmdCtrMsb = (uint8_t)(wCmdCtr >> 8);
    uint8_t PH_MEMLOC_REM bCmdCtrLsb = (uint8_t)(wCmdCtr & 0x00ff);

    memset(pIv, 0, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */

    /* parameter checking */
    if ((pTi == NULL) || (pIv == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
    }

    if (bIsResponse)
    {
        /* Form the IV for RespData as 0x5A||0xA5||TI||CmdCtr||0x0000000000000000 */
        pIv[bIndex++] = 0x5A;
        pIv[bIndex++] = 0xA5;
    }
    else
    {
        /* Form the IV for CmdData as 0xA5||0x5A||TI||CmdCtr||0x0000000000000000  */
        pIv[bIndex++] = 0xA5;
        pIv[bIndex++] = 0x5A;
    }

    pIv[bIndex++] = pTi[0];
    pIv[bIndex++] = pTi[1];
    pIv[bIndex++] = pTi[2];
    pIv[bIndex++] = pTi[3];
    pIv[bIndex++] = bCmdCtrLsb;
    pIv[bIndex++] = bCmdCtrMsb;

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfNtag42XDna_Sw_Int_GetFrameLength(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint16_t * pFSD, uint16_t * pFSC)
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wTmp = 0;

    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_GetConfig(
        pDataParams->pPalMifareDataParams,
        PHPAL_I14443P4_CONFIG_FSI,
        &wTmp
        ));

    /* Get FSD */
    switch((uint8_t)(wTmp >> 8))
    {
    case 0:
        *pFSD = 16;
        break;
    case 1:
        *pFSD = 24;
        break;
    case 2:
        *pFSD = 32;
        break;
    case 3:
        *pFSD = 40;
        break;
    case 4:
        *pFSD = 48;
        break;
    case 5:
        *pFSD = 64;
        break;
    case 6:
        *pFSD = 96;
        break;
    case 7:
        *pFSD = 128;
        break;
    case 8:
        *pFSD = 256;
        break;
    default:
        break;
    }

    /* Get FSC */
    switch((uint8_t)(wTmp))
    {
    case 0:
        *pFSC = 16;
        break;
    case 1:
        *pFSC = 24;
        break;
    case 2:
        *pFSC = 32;
        break;
    case 3:
        *pFSC = 40;
        break;
    case 4:
        *pFSC = 48;
        break;
    case 5:
        *pFSC = 64;
        break;
    case 6:
        *pFSC = 96;
        break;
    case 7:
        *pFSC = 128;
        break;
    case 8:
        *pFSC = 256;
        break;
    default:
        break;
    }

    return PH_ERR_SUCCESS;
}

#ifdef NXPBUILD__PH_CRYPTOSYM
phStatus_t phalMfNtag42XDna_Sw_Int_AuthenticateEv2(phalMfNtag42XDna_Sw_DataParams_t * pDataParams, uint8_t bAuthOption, uint16_t wOption,
    uint16_t wKeyNo, uint16_t wKeyVer, uint8_t bKeyNoCard, uint8_t * pDivInput, uint8_t bDivLen, uint8_t bLenPcdCapsIn, uint8_t *pPcdCapsIn,
    uint8_t *pPcdCapsOut, uint8_t *pPdCapsOut)
{
/* Validate parameters
    * bAuthOption should be either one(PHAL_MFNTAG42XDNA_AUTHFIRST_NON_LRP) or zero(PHAL_MFNTAG42XDNA_AUTHNONFIRST_NON_LRP).
    * wOption should be validated and interpreted similar to other authenticate
    * functions above.
    * Check that key referenced by wKeyNo and wKeyVer is an AES128 key else throw error (KEY_ERROR)
    * bKeyNoCard cannot be greater than 0xF.
    * if wOption != 0xFFFF then check for bDivLen. If bDivLen is zero, then ignore div input.
    * bLenPcdCapsIn <= 6
    */

    /* Form the command Cmd + bKeyNo + bLenPcdCapsIn + pPcdCapsIn
    * phpalMifare_ExchangeL4(cmdarray);
    * if response != 16+1, return PROTOCOL_ERROR and also first byte should be 0xAF
    * Load the AES 128 key specified by wKeyNo and wKeyVer in the cryptodataparams
    * Decrypt the RndB
    * Insert a seed.
    * Generate RndA
    *
    * Left rotate RndB to generate RndB`
    * Concatenate RndA||RndB` and encrypt this.
    * phpalMifare_ExchangeL4(EXCHANGE_Default, AF || Enc(RndA ||RndB`));
    * if (bFirstAuth) then PD Caps and PCD Caps are returned. Not otherwise.
    * Verify RndA by decrypting the response. Store TI into the pDataParams.
    * As per 4.9.7, Generate SV1 and Sv2
    * The session key generation is according to NIST SP 800-108 [14] in counter mode.
    * The pseudo random function PRF(key, message) applied during the key generation is the the CMAC algorithm
    * Encipher SV1 to generate EncSessionkey
    * Encipher SV2 to generate MACSessionKey.
    * Assign these in pDataParams.
    * Load EncSessionkey and MACSessionkey.
    */
    uint8_t     PH_MEMLOC_REM bRndA[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bRndB[PH_CRYPTOSYM_AES_BLOCK_SIZE+1];
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bKey[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCmdBuff[60U];
    uint16_t    PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint8_t     PH_MEMLOC_REM bIvLen;
    uint8_t     PH_MEMLOC_REM bSV1[32];
    uint8_t     PH_MEMLOC_REM bTmp;
    uint8_t *   PH_MEMLOC_REM pRecv;
    uint8_t     PH_MEMLOC_REM bAppId[3] = {0x00, 0x00, 0x00};
    uint8_t     PH_MEMLOC_REM bMacLen;

    /* Set the current authentication status to NOT AUTHENTICATED i.e., invalid key number */
    pDataParams->bKeyNo = 0xFF;
#ifdef RDR_LIB_PARAM_CHECK
    if (bAuthOption > PHAL_MFNTAG42XDNA_AUTHFIRST_LRP)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
    }
   /* Validate Card Key Number */
    if(memcmp(pDataParams->pAid, bAppId, 3) == 0x00)
    {
        /* At PICC level, 1,2,3,4 are valid ones. 0 is excluded */
        if((bKeyNoCard > PHAL_MFNTAG42XDNA_ORIGINALITY_KEY_LAST ) || (bKeyNoCard < PHAL_MFNTAG42XDNA_ORIGINALITY_KEY_FIRST))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
        }
    }
    else
    {
        /* At App level, 0,1,2,3,4 are valid ones. */
        if((bKeyNoCard > PHAL_MFNTAG42XDNA_ORIGINALITY_KEY_LAST))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
        }
    }
    if ((wOption != PHAL_MFNTAG42XDNA_NO_DIVERSIFICATION) &&
        (wOption != PH_CRYPTOSYM_DIV_MODE_DESFIRE) &&
        (wOption != PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
    }
    if ((wOption != PHAL_MFNTAG42XDNA_NO_DIVERSIFICATION) && (bDivLen > 31))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
    }
    if(bLenPcdCapsIn > 6)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
    }
#endif
    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        PH_CRYPTOSYM_AES128_KEY_SIZE,
        bKey,
        &wKeyType
        ));

    /* Invalid key type at wKeyNo and wKeyVer */
    if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_KEY, PH_COMP_AL_MFNTAG42XDNA);
    }

    if ((wOption != PHAL_MFNTAG42XDNA_NO_DIVERSIFICATION) && (bDivLen != 0x00))
    {
        /* Key is diversified and put back in bKey */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_DiversifyDirectKey(
            pDataParams->pCryptoDataParamsEnc,
            wOption,
            bKey,
            wKeyType,
            pDivInput,
            bDivLen,
            bKey
            ));
    }
    bRndLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;
    bIvLen = PH_CRYPTOSYM_AES_BLOCK_SIZE;

    /* load key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        bKey,
        wKeyType));

    /* Send the cmd and receive the encrypted RndB */
    if (bAuthOption == PHAL_MFNTAG42XDNA_AUTHFIRST_NON_LRP)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFNTAG42XDNA_CMD_AUTHENTICATE_EV2_FIRST;
    }
    else
    {
        bCmdBuff[wCmdLen++] = PHAL_MFNTAG42XDNA_CMD_AUTHENTICATE_EV2_NON_FIRST;
    }
    bCmdBuff[wCmdLen++] = bKeyNoCard; /* key number card */
    if (bAuthOption == PHAL_MFNTAG42XDNA_AUTHFIRST_NON_LRP)
    {
        /* Maximum frame size of card is 64 bytes, so the data should be of max 57 bytes */
        bLenPcdCapsIn = (bLenPcdCapsIn > 57) ? 57 : bLenPcdCapsIn;

        bCmdBuff[wCmdLen++] = bLenPcdCapsIn; /* PCD Caps In length */
        /* PCD Caps In */
        memcpy(&bCmdBuff[wCmdLen], pPcdCapsIn, bLenPcdCapsIn);  /* PRQA S 3200 */
        wCmdLen += bLenPcdCapsIn;
    }

    status = phalMfNtag42XDna_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        );
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        if (pDataParams->bAuthMode != PHAL_MFNTAG42XDNA_NOT_AUTHENTICATED)
        {
            phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
        }

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }
        else
        {
            return status;
        }
    }
    if (wRxlen != bRndLen)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Store the unencrypted RndB */
    memcpy(bRndB, pRecv, bRndLen); /* PRQA S 3200 */

    /* Load Zero IV */
    memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */

    /* Load Iv.*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Decrypt the RndB received from PICC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_DEFAULT,
        bRndB,
        bRndLen,
        bRndB
        ));

    /* Generate RndA */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Seed(pDataParams->pCryptoRngDataParams, bRndB, bRndLen));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, bRndLen, bRndA));

    /* Concat RndA and RndB' */
    bCmdBuff[0] = PHAL_MFNTAG42XDNA_RESP_ADDITIONAL_FRAME;
    memcpy(&bCmdBuff[1], bRndA, bRndLen); /* PRQA S 3200 */
    memcpy(&bCmdBuff[bRndLen + 1], &bRndB[1], bRndLen - 1); /* PRQA S 3200 */
    bCmdBuff[2 * bRndLen] = bRndB[0]; /* RndB left shifted by 8 bits */

    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Encrypt RndA + RndB' */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_DEFAULT,
        &bCmdBuff[1],
        2 * bRndLen,
        &bCmdBuff[1]
    ));

    wCmdLen = 2 * bRndLen + 1;

    /* Get the encrypted TI || RndA' || PDCaps || PCDCaps into bWorkBuffer */
    PH_CHECK_SUCCESS_FCT(status, phalMfNtag42XDna_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    /* If First Auth, then 32 bytes of data is obtained after exchange */
    if (bAuthOption == PHAL_MFNTAG42XDNA_AUTHFIRST_NON_LRP)
    {
        if (wRxlen != (2 * bRndLen))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }
        /* Load IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            bIvLen));

        /* Decrypt TI || RndA' || PDCaps || PCDCaps */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_DEFAULT,
            pRecv,
            wRxlen,
            bCmdBuff
            ));

        /* Checking the integrity of RndA */
        bTmp = bCmdBuff[3];            /* Store temporarily TI[3] */
        bCmdBuff[3] = bCmdBuff[19];    /* Rotate RndA` to get RndA */

        /* Compare RndA and buff */
        if (memcmp(&bCmdBuff[3], bRndA, PH_CRYPTOSYM_AES128_KEY_SIZE) != 0)
        {
            /* Authentication failed */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }

        bCmdBuff[3] = bTmp;                                            /* Restore TI[3] */
        memcpy(pDataParams->bTi, &bCmdBuff[0], PHAL_MFNTAG42XDNA_SIZE_TI);  /* PRQA S 3200 */ /* Store the Transaction Identifier */
        memcpy(pPcdCapsOut, &bCmdBuff[26], 6);                         /* PRQA S 3200 */ /* Update pPcdCapsOut */
        memcpy(pPdCapsOut, &bCmdBuff[20], 6);                          /* PRQA S 3200 */ /* Update pPdCapsOut */
    }
    else
    {
        /* If Auth is Non First, then 16 bytes of data is expected */
        if (wRxlen != bRndLen)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }

        /* Load IV */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            bIvLen));

        /* Decrypt RndA'*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_BUFFER_CONT,
            pRecv,
            bRndLen,
            &bCmdBuff[1]
        ));

        bCmdBuff[0] = bCmdBuff[bRndLen]; /* Rotate right to get back RndA */

        /* Compare RndA and bCmdBuff */
        if (memcmp(bCmdBuff, bRndA, bRndLen) != 0)
        {
            /* Authentication failed */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFNTAG42XDNA);
        }
    }

    /* In case of Originality Key - reset authentication state */
    if((bKeyNoCard <= PHAL_MFNTAG42XDNA_ORIGINALITY_KEY_LAST) &&
        (bKeyNoCard >= PHAL_MFNTAG42XDNA_ORIGINALITY_KEY_FIRST) &&
        (memcmp(pDataParams->pAid, bAppId, 3) == 0x00))
    {
        phalMfNtag42XDna_Sw_Int_ResetAuthStatus(pDataParams);
        return PH_ERR_SUCCESS;
    }

    /* Generate the session key SV1
     *  SV 1 = 0xA5||0x5A||0x00||0x01||0x00||0x80||RndA[15:14]||(RndA[13::8] XOR RndB[15::8])||RndB[7::0]||RndA[7::0]
     */
    bSV1[0] = 0xA5;
    bSV1[1] = 0x5A;
    bSV1[2] = 0x00;
    bSV1[3] = 0x01;
    bSV1[4] = 0x00;
    bSV1[5] = 0x80;
    bSV1[6] = bRndA[0];
    bSV1[7] = bRndA[1];

    bSV1[8]  = bRndA[2] ^ bRndB[0];
    bSV1[9]  = bRndA[3] ^ bRndB[1];
    bSV1[10] = bRndA[4] ^ bRndB[2];
    bSV1[11] = bRndA[5] ^ bRndB[3];
    bSV1[12] = bRndA[6] ^ bRndB[4];
    bSV1[13] = bRndA[7] ^ bRndB[5];

    memcpy(&bSV1[14], &bRndB[6], 10);     /* PRQA S 3200 */
    memcpy(&bSV1[24], &bRndA[8], 8);     /* PRQA S 3200 */

    /* Load Zero IV */
    memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */

    /* Load IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

    /* Start CMAC calculation */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pCryptoDataParamsEnc,
        (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
        &bSV1[0],
        32,
        pDataParams->bSesAuthENCKey,
        &bMacLen
        ));

    /* Generate the session key SV2
     *  SV 2 = 0x5A||0xA5||0x00||0x01||0x00||0x80||RndA[15:14]|| (RndA[13::8] XOR RndB[15::10])||RndB[9::0]||RndA[7::0]
     */
    bSV1[0] = 0x5A;
    bSV1[1] = 0xA5;

    /* Load Zero IV */
    memset(pDataParams->bIv, 0x00, bIvLen); /* PRQA S 3200 */

    /* Load IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        bIvLen));

   /* Calculate MAC for SV2  */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pCryptoDataParamsEnc,
        (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
        &bSV1[0],
        32,
        pDataParams->bSesAuthMACKey,
        &bMacLen
        ));

    /* Session key is generated */
    if(bAuthOption == PHAL_MFNTAG42XDNA_AUTHFIRST_NON_LRP)
    {
        pDataParams->wCmdCtr = 0x00;
    }

    pDataParams->bAuthMode = PHAL_MFNTAG42XDNA_AUTHENTICATEEV2;
    pDataParams->bCryptoMethod = PH_CRYPTOSYM_KEY_TYPE_AES128;
    pDataParams->bKeyNo = bKeyNoCard;

    /* Load the ENC session key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bSesAuthENCKey,
        pDataParams->bCryptoMethod
        ));

    /* Load the MAC session key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsMac,
        pDataParams->bSesAuthMACKey,
        pDataParams->bCryptoMethod
        ));

    /* Set the keep Iv ON */
    return phCryptoSym_SetConfig(
        pDataParams->pCryptoDataParamsEnc,
        PH_CRYPTOSYM_CONFIG_KEEP_IV,
        PH_CRYPTOSYM_VALUE_KEEP_IV_ON
        );
}

#endif /* NXPBUILD__PH_CRYPTOSYM */

phStatus_t phalMfNtag42XDna_Sw_Int_ComputeSDMIV(phalMfNtag42XDna_Sw_DataParams_t *pDataParams, uint8_t *pSessEncKey, uint8_t *pSDMReadCtr,
    uint8_t *pIV)
{
    phStatus_t statusTmp =  PH_ERR_SUCCESS;
    uint8_t PH_MEMLOC_REM bTmpIV[16] = {'\0'};
    uint8_t PH_MEMLOC_REM bDataBuf[16] = {'\0'};

    /* Validate the IP Parameters */
    if(pSessEncKey == NULL || pSDMReadCtr == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
    }
    /* Create a Back up of the current IV */
    memcpy(bTmpIV, pDataParams->bIv, PH_CRYPTOSYM_AES_BLOCK_SIZE);

    /* Load zero to IV */
    memset(pDataParams->bIv, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE);

    /* Load Iv */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsEnc,
            pDataParams->bIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE
            ));

    /* load key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pCryptoDataParamsEnc,
        pSessEncKey,
        PH_CRYPTOSYM_KEY_TYPE_AES128));

    /* Clear data buffer */
    memset(bDataBuf, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE);

    /* Prepare Data buffer to derive IV */
    bDataBuf[0] = pSDMReadCtr[0];
    bDataBuf[1] = pSDMReadCtr[1];
    bDataBuf[2] = pSDMReadCtr[2];

    /* Now Encrypt the data Buffer to derive IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CIPHER_MODE_CBC,
            bDataBuf,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            bDataBuf
            ));

    memcpy(pIV, bDataBuf, PH_CRYPTOSYM_AES_BLOCK_SIZE);

    /* Restore the IV */
    memcpy(pDataParams->bIv,bTmpIV, PH_CRYPTOSYM_AES_BLOCK_SIZE);

    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pDataParams->bIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE
        ));

    return PH_ERR_SUCCESS;
}

phStatus_t phalMfNtag42XDna_Sw_Int_DecryptSDMData(phalMfNtag42XDna_Sw_DataParams_t *pDataParams, uint8_t *pSessEncKey, uint8_t *pIv,
    uint8_t wIvSize, uint8_t *pInputOutputData, uint16_t wInputDataLen)
{
    phStatus_t statusTmp =  PH_ERR_SUCCESS;
    uint16_t   wAuthMode = 0x00;

#ifndef NXPBUILD__PH_CRYPTOSYM_LRP
    PHAL_MFNTAG42XDNA_UNUSED_VARIABLE(wIvSize);
#endif /* NXPBUILD__PH_CRYPTOSYM_LRP */

    if(pSessEncKey == NULL || pIv == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(
        pDataParams->pCryptoDataParamsMac,
        PH_CRYPTOSYM_CONFIG_LRP,
        &wAuthMode
        ));

    /* Load the session ENC Key to Crypto Object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParamsEnc,
            pSessEncKey,
            PH_CRYPTOSYM_KEY_TYPE_AES128));

    {

    /* Load IV */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsEnc,
        pIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE
        ));

    /* Decrypt Data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
            pDataParams->pCryptoDataParamsEnc,
            PH_CRYPTOSYM_CIPHER_MODE_CBC | PH_EXCHANGE_DEFAULT,
            pInputOutputData,
            wInputDataLen,
            pInputOutputData
            ));
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phalMfNtag42XDna_Sw_Int_GenerateSDMSessionKeysAES(phalMfNtag42XDna_Sw_DataParams_t *pDataParams, uint8_t bSdmOption, uint16_t wKeyNo,
    uint16_t wKeyVer, uint8_t *pSDMReadCtr, uint8_t * pVCUID, uint8_t bUidLen, uint8_t *pSessEncKey, uint8_t *pSessMacKey)
{
    phStatus_t statusTmp =  PH_ERR_SUCCESS;
    uint8_t PH_MEMLOC_REM bSV1[16] = {'\0'};
    uint8_t PH_MEMLOC_REM bSV2[16] = {'\0'};
    uint8_t PH_MEMLOC_REM bKey[16] = {'\0'};
    uint8_t PH_MEMLOC_REM bCmdLenMac= 0;
    uint8_t PH_MEMLOC_REM bCmdLenEnc= 0;
    uint16_t PH_MEMLOC_REM wKeyType = 0x0000;
    uint8_t PH_MEMLOC_REM bTmpIV[16] = {'\0'};
    uint8_t PH_MEMLOC_REM bMacLen = 0x00;

    /* Validate the Output Buffers */
    if((pSessEncKey == NULL) || (pSessMacKey == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* Clear the session vector.  */
    memset(bSV1, 0x00, sizeof(bSV1));    /* PRQA S 3200 */
    memset(bSV2, 0x00, sizeof(bSV2));    /* PRQA S 3200 */

    /* Prepare SV1 & SV2 Buffer */
    /* SV1 = 0xC3 || 0x3C || 0x00 || 0x01 || 0x00 || 0x80 ||VCUID || SDMReadCtr [||ZeroPadding]*/
    /* SV2 = 0x3C || 0xC3 || 0x00 || 0x01 || 0x00 || 0x80 [||VCUID][||SDMReadCtr][||ZeroPadding]*/
    bSV1[bCmdLenEnc++] = 0xC3;
    bSV1[bCmdLenEnc++] = 0x3C;

    bSV2[bCmdLenMac++] = 0x3C;
    bSV2[bCmdLenMac++] = 0xC3;

    bSV2[bCmdLenMac++] = bSV1[bCmdLenEnc++] = 0x00;
    bSV2[bCmdLenMac++] = bSV1[bCmdLenEnc++] = 0x01;
    bSV2[bCmdLenMac++] = bSV1[bCmdLenEnc++] = 0x00;
    bSV2[bCmdLenMac++] = bSV1[bCmdLenEnc++] = 0x80;

    if(pVCUID != NULL)
    {
        memcpy(&bSV1[bCmdLenEnc], pVCUID, bUidLen);  /* PRQA S 3200 */
        bCmdLenEnc += bUidLen;
    }

    if (pSDMReadCtr != NULL)
    {
        memcpy(&bSV1[bCmdLenEnc], pSDMReadCtr, 0x03); /* PRQA S 3200 */
        bCmdLenEnc += 0x03;
    }

    /* Copy UID into SV - UID should not more then 7 byte in
            * this case else bSV array size needs to be changed
            */
    if(bSdmOption & PHAL_MFNTAG42XDNA_VCUID_PRESENT)
    {
        /* If VCUID should be considered for MAC calcluation, and pUID is passed as NULL, throw error */
        if(pVCUID != NULL)
        {
            memcpy(&bSV2[bCmdLenMac], pVCUID, bUidLen); /* PRQA S 3200 */
            bCmdLenMac += bUidLen;
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
        }
    }

    if(bSdmOption & PHAL_MFNTAG42XDNA_RDCTR_PRESENT)
    {
        /* If SDMReadCounter should be considered for MAC calcluation, and pUID is passed as NULL, throw error */
        if(pSDMReadCtr != NULL)
        {
            memcpy(&bSV2[bCmdLenMac], pSDMReadCtr, 0x03);  /* PRQA S 3200 */
            bCmdLenMac += 0x03;
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFNTAG42XDNA);
        }
    }

    /* Now Get the Keys from SW Key Store */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wKeyNo,
        wKeyVer,
        PH_CRYPTOSYM_AES128_KEY_SIZE,
        bKey,
        &wKeyType
        ));

    if (wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_KEY, PH_COMP_AL_MFNTAG42XDNA);
    }

    /* load key */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
            pDataParams->pCryptoDataParamsMac,
            bKey,
            wKeyType));

    /* Create a Back up of the current IV */
    memcpy(bTmpIV, pDataParams->bIv, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */

    /* Load zero to IV */
    memset(pDataParams->bIv, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE);  /* PRQA S 3200 */

    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pCryptoDataParamsMac,
        pDataParams->bIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE
        ));

    /* MAC SV1 to obtain KSesSDMFileReadEnc */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
            bSV1,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            bKey,
            &bMacLen
            ));

    /* Copy the session Enc Key */
    memcpy(pSessEncKey, bKey, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */

    memset(bKey, 0x00, PH_CRYPTOSYM_AES_BLOCK_SIZE); /* PRQA S 3200 */

    /* MAC SV2 to obtain KSesSDMFileReadMac */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
            pDataParams->pCryptoDataParamsMac,
            (PH_CRYPTOSYM_MAC_MODE_CMAC | PH_EXCHANGE_DEFAULT),
            bSV2,
            PH_CRYPTOSYM_AES_BLOCK_SIZE,
            bKey,
            &bMacLen
            ));

    /* Copy session Mac Key */
    memcpy(pSessMacKey, bKey, PH_CRYPTOSYM_AES_BLOCK_SIZE);

    /* Reload Back up IV */
    memcpy(pDataParams->bIv, bTmpIV, PH_CRYPTOSYM_AES_BLOCK_SIZE);

    /* Load Iv */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParams->pCryptoDataParamsMac,
            pDataParams->bIv,
            PH_CRYPTOSYM_AES_BLOCK_SIZE
            ));

    return PH_ERR_SUCCESS;
}


#endif /* NXPBUILD__PHAL_MFNTAG42XDNA_SW */
