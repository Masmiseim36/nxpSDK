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
#include <phpalMifare.h>
#include <ph_RefDefs.h>
#include <ph_TypeDefs.h>
#include <phTools.h>
#include <string.h>

#ifdef NXPBUILD__PH_CRYPTOSYM
#include <phCryptoSym.h>
#endif /* NXPBUILD__PH_CRYPTOSYM */

#ifdef NXPBUILD__PHAL_MFDF_SW

#include "../phalMfdf_Int.h"
#include "phalMfdf_Sw.h"
#include "phalMfdf_Sw_Int.h"

phStatus_t phalMfdf_Sw_Int_GetData(
                                   phalMfdf_Sw_DataParams_t * pDataParams,
                                   uint8_t * pSendBuff,
                                   uint16_t wCmdLen,
                                   uint8_t ** pResponse,
                                   uint16_t * pRxlen
                                   )
{
    uint16_t   PH_MEMLOC_REM wOption;
    uint8_t    PH_MEMLOC_REM * pRecv = NULL;
    phStatus_t PH_MEMLOC_REM statusTmp = 0;
    uint8_t    PH_MEMLOC_REM bStatusByte = 0xFF;
    uint8_t    PH_MEMLOC_REM bCmdBuff[10];
    uint16_t   PH_MEMLOC_REM wNextPos = 0;
    uint16_t   PH_MEMLOC_REM wRxBufferSize = 0;
    uint8_t    PH_MEMLOC_REM bBackupBytes[3];
    uint8_t    PH_MEMLOC_REM pApdu[5] = { PHAL_MFDF_WRAPPEDAPDU_CLA, 0x00, PHAL_MFDF_WRAPPEDAPDU_P1, PHAL_MFDF_WRAPPEDAPDU_P2, 0x00 };
    uint8_t    PH_MEMLOC_REM bBackUpByte0;
    uint8_t    PH_MEMLOC_REM bBackUpByte1;
    uint8_t    PH_MEMLOC_REM bBackUpByte3 = 0;
    uint16_t   PH_MEMLOC_REM wBackUpLen = 0;
    uint16_t   PH_MEMLOC_REM wTmp = 0;

    /* Status and two other bytes to be backed up before getting new frame of data */
    (void)memset(bBackupBytes, 0x00, 3);

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        &wTmp
        ));

    wOption = PH_EXCHANGE_DEFAULT;
    if (0U != (pDataParams->bWrappedMode))
    {
        if (wCmdLen > PHAL_MFDF_MAXWRAPPEDAPDU_SIZE)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFDF);
        }

        pApdu[1] = pSendBuff[0];  /* MIFARE DESFire contactless IC command code. */
        /* Encode APDU Length*/
        pApdu[4]= (uint8_t)wCmdLen - 1u; /* Set APDU Length. */

        statusTmp = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            (pApdu[4] == 0x00U) ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            PHAL_MFDF_WRAP_HDR_LEN,
            &pRecv,
            pRxlen
            );
        if ((pApdu[4] != 0x00U) && (statusTmp == PH_ERR_SUCCESS))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                &pSendBuff[1],
                wCmdLen - 1u,
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
            if (((0U != (pDataParams->bWrappedMode)) && (*pRxlen == 2U)) ||
                ((0U==(pDataParams->bWrappedMode)) && (*pRxlen == 1U)))
            {
                /* AF should always be accompanied by data. Otherwise
                it is a protocol error */
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
            }
            /* One more status byte to read from MIFARE DESFire */
            bBackUpByte0 = pRecv[0];
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
            if (*pRxlen == 2U)
            {
                pRecv[wBackUpLen] = pRecv[0];
                pRecv[wBackUpLen + 1U] = pRecv[1];
                bStatusByte = pRecv[1];
            }
            else if (*pRxlen == 1U)
            {
                bStatusByte = pRecv[0];
                pRecv[wBackUpLen] = bStatusByte;
            }
            else
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
            }

            *pRxlen = wBackUpLen + *pRxlen;

            /* Set back the backed up bytes */
            pRecv[0] = bBackUpByte0;
            pRecv[1] = bBackUpByte1;
        }
        else
        {
            if (statusTmp != PH_ERR_SUCCESS)
            {
                return statusTmp;
            }
        }
    }
    else
    {
        /* Normal mode */
        if (wCmdLen > PHAL_MFDF_MAXDFAPDU_SIZE)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFDF);
        }

        /* Send this on L4 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            wOption,
            pSendBuff,
            wCmdLen,
            &pRecv,
            pRxlen
            ));
    }

    /* Storing the original pointer */
    *pResponse = pRecv;

    /* Status is 0xAF or Ox00? */
    if (*pRxlen > 0x0000U)
    {
        if (0U != (pDataParams->bWrappedMode))
        {
            bStatusByte = (*pResponse)[(*pRxlen) - 1U];
        }
        else
        {
            bStatusByte = (*pResponse)[wTmp];
        }
    }

    if (bStatusByte == PHAL_MFDF_RESP_ADDITIONAL_FRAME)
    {
        if (((pDataParams->bWrappedMode > 0U) && (*pRxlen == 2U)) ||
            ((!(pDataParams->bWrappedMode > 0U)) && (*pRxlen == 1U)))
        {
            /* AF should always be accompanied by data. Otherwise
            it is a protocol error */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        if (0U != (pDataParams->bWrappedMode))
        {
            /* Next position will ensure overwriting on the
            SW1SW2 received from previous command */
            wNextPos = (*pRxlen) - 2U;
            (void)memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3u], 3U);
        }
        else
        {
            /* Backup the last byte */
            bBackUpByte3 = (*pResponse)[(*pRxlen - 1U)];
            (void)memcpy(bBackupBytes, &(*pResponse)[(*pRxlen - 3U)], 3U);
            wNextPos = (*pRxlen) - 1U;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
            &wRxBufferSize
            ));
    }

    while (bStatusByte == PHAL_MFDF_RESP_ADDITIONAL_FRAME)
    {
        if (((pDataParams->bWrappedMode > 0U) && (*pRxlen == 2U)) ||
            ((!(pDataParams->bWrappedMode > 0U)) && (*pRxlen == 1U)))
        {
            /* AF should always be accompanied by data. Otherwise
            it is a protocol error */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        if ((wNextPos + PHAL_MFDF_MAX_FRAME_SIZE) >= wRxBufferSize)
        {
            /* Return 0xAF and let the caller recall the function with
            option = PH_EXCHANGE_RXCHAINING */
            /* Return the data accumulated till now and its length */
            if (0U != (pDataParams->bWrappedMode))
            {
                (*pRxlen) -= 2u;
            }
            else
            {
                (*pRxlen) -= 1u;
                (*pResponse)++;
            }
            return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
        }
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
            wNextPos
            ));

        bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
        wCmdLen = 1;
        if ( 0U != (pDataParams->bWrappedMode) )
        {
            pApdu[1] = bCmdBuff[0];  /* MIFARE DESFire command code. */
            /* Encode APDU Length*/
            pApdu[4]= (uint8_t)wCmdLen - 1u; /* Set APDU Length. */

            statusTmp = phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                (pApdu[4] == 0x00U) ? PH_EXCHANGE_DEFAULT : PH_EXCHANGE_BUFFER_FIRST,
                pApdu,
                PHAL_MFDF_WRAP_HDR_LEN,
                &pRecv,
                pRxlen
                );
            if ((pApdu[4] != 0x00U) && (statusTmp == PH_ERR_SUCCESS))
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
                /* One or two more status bytes to read from MIFARE DESFire */
                bBackUpByte0 = pRecv[0];
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
                if (*pRxlen == 2U)
                {
                    pRecv[wBackUpLen] = pRecv[0];
                    pRecv[wBackUpLen + 1U] = pRecv[1];
                }
                else if (*pRxlen == 1U)
                {
                    bStatusByte = pRecv[0];
                    pRecv[wBackUpLen] = bStatusByte;
                }
                else
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
                }

                *pRxlen = wBackUpLen + *pRxlen;

                /* Set back the backed up bytes */
                pRecv[0] = bBackUpByte0;
                pRecv[1] = bBackUpByte1;
            }
            else
            {
                if (statusTmp != PH_ERR_SUCCESS)
                {
                    return statusTmp;
                }
            }
        }
        else
        {
            /* Send this on L4 */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                wOption,
                bCmdBuff,
                wCmdLen,
                &pRecv,
                pRxlen
                ));
        }

        /* Update wNextPos */
        if (0U != (pDataParams->bWrappedMode))
        {
            bStatusByte = (*pResponse)[(*pRxlen) - 1U];

            /* Putback the backed up bytes */
            (void)memcpy(&(*pResponse)[wNextPos - 3u], bBackupBytes, 3);

            wNextPos = (*pRxlen) - 2U;
            (void)memcpy(bBackupBytes, &(*pResponse)[wNextPos - 3u], 3);
        }
        else
        {
            bStatusByte = (*pResponse)[wNextPos];

            /* Put back the previously backedup byte */
            (*pResponse)[wNextPos] = bBackUpByte3;

            /* Putback the backed up bytes */
            (void)memcpy(&(*pResponse)[wNextPos - 2u], bBackupBytes, 3);

            wNextPos = (*pRxlen) - 1U;
            bBackUpByte3 = (*pResponse)[wNextPos];

            /* Backup 3 bytes. The nxt frame will overwrite these */
            (void)memcpy(bBackupBytes, &(*pResponse)[wNextPos - 2u], 3);
        }
    }
    if (0U != (pDataParams->bWrappedMode))
    {
        (*pRxlen) -= 2u;
    }
    else
    {
        (*pRxlen) -= 1u;
        (*pResponse)++;
    }
    return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

phStatus_t phalMfdf_Sw_Int_ReadData_Plain(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * bCmdBuff,
    uint16_t wCmdLen,
    uint8_t ** ppRxdata,
    uint16_t * pRxdataLen
    )
{
    uint16_t    PH_MEMLOC_REM status = 0;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM * pRecv = NULL;

#ifndef NXPBUILD__PH_NDA_MFDF

    PHAL_MFDF_UNUSED_VARIABLE(bOption)

#endif /* NXPBUILD__PH_NDA_MFDF */


    /* Send the command */
    status = phalMfdf_Sw_Int_GetData(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        );

    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {

        return status;
    }


    *ppRxdata = pRecv;
    *pRxdataLen = wRxlen;

    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);;
    }

    return PH_ERR_SUCCESS;
}


phStatus_t phalMfdf_Sw_Int_Write_Plain(
                                       phalMfdf_Sw_DataParams_t * pDataParams,
                                       uint8_t * bCmdBuff,
                                       uint16_t wCmdLen,
                                       uint8_t bCommOption,
                                       uint8_t * pData,
                                       uint16_t wDataLen
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM status = 0;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM bWorkBuffer[32];
    uint16_t    PH_MEMLOC_REM wFrameLen = 0;
    uint16_t    PH_MEMLOC_REM wTotalLen = 0;
    uint16_t    PH_MEMLOC_REM wTmp = 0;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFDF_WRAPPEDAPDU_CLA, 0x00, PHAL_MFDF_WRAPPEDAPDU_P1, PHAL_MFDF_WRAPPEDAPDU_P2, 0x00 };


#ifndef NXPBUILD__PH_NDA_MFDF
    PHAL_MFDF_UNUSED_VARIABLE(bCommOption)
#endif /* NXPBUILD__PH_NDA_MFDF */

    (void)memset(bWorkBuffer, 0x00, 16);


#ifndef NXPBUILD__PH_NDA_MFDF

    wTotalLen = wDataLen;

#endif /* NXPBUILD__PH_NDA_MFDF */

    if (0U != (pDataParams->bWrappedMode))
    {
        wFrameLen = PHAL_MFDF_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        wFrameLen = PHAL_MFDF_MAXDFAPDU_SIZE;
    }

    if (wTotalLen == 0x0000U)
    {
        /* Single frame cmd without any data. Just send it */
        status = phalMfdf_ExchangeCmd(
            pDataParams,
            pDataParams->pPalMifareDataParams,
            pDataParams->bWrappedMode,
            bCmdBuff,
            wCmdLen,
            &pRecv,
            &wRxlen
            );
        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {

            return PH_ADD_COMPCODE(status, PH_COMP_AL_MFDF);
        }

        /* TBD: SA */
        if (wRxlen > 32U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        if(wRxlen > 0U)
        {
            (void)memcpy(bWorkBuffer, pRecv,(size_t) wRxlen);
        }
    }
    else
    {
        if (0U != (pDataParams->bWrappedMode))
        {
            wFrameLen = PHAL_MFDF_MAXWRAPPEDAPDU_SIZE;
        }
        else
        {
            wFrameLen = PHAL_MFDF_MAXDFAPDU_SIZE;
        }

        wTmp = wTotalLen;

        if (wTmp <= (wFrameLen - wCmdLen))
        {
            /* Send in one shot */
            if (0U != (pDataParams->bWrappedMode))
            {
                pApdu[1] = bCmdBuff[0]; /* MIFARE DESFire cmd code in INS */
                pApdu[4] = (uint8_t)(wCmdLen + wTotalLen) - 0x01u;

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_FIRST,
                    pApdu,
                    PHAL_MFDF_WRAP_HDR_LEN,
                    &pRecv,
                    &wRxlen));

                PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    &bCmdBuff[1],
                    wCmdLen - 1u,
                    &pRecv,
                    &wRxlen));

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    pData,
                    wDataLen,
                    &pRecv,
                    &wRxlen));


                /* Le byte */
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_LAST,
                    &pApdu[2],
                    0x01,
                    &pRecv,
                    &wRxlen));
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_FIRST,
                    bCmdBuff,
                    wCmdLen,
                    &pRecv,
                    &wRxlen));



#ifndef NXPBUILD__PH_NDA_MFDF

                PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                    pDataParams->pPalMifareDataParams,
                    PH_EXCHANGE_BUFFER_LAST,
                    pData,
                    wDataLen,
                    &pRecv,
                    &wRxlen));

#endif /* NXPBUILD__PH_NDA_MFDF */

            }
            if (0U != (pDataParams->bWrappedMode))
            {
                status = pRecv[wRxlen - 1u];
                wRxlen -= 2u;
            }
            else
            {
                status = pRecv[0];
                pRecv++; /* Increment pointer to point only to data */
                wRxlen -= 1u;
            }

            if (status != PH_ERR_SUCCESS)
            {


                return phalMfdf_Int_ComputeErrorResponse(pDataParams, (uint8_t)status);
            }

            (void)memcpy(bWorkBuffer, pRecv, wRxlen);
        }
        else
        {
            /* Send command and data. Chain data to PICC */
            statusTmp = phalMfdf_Sw_Int_SendDataToPICC(
                pDataParams,
                bCmdBuff,
                wCmdLen,
                pData,
                wDataLen,
                bWorkBuffer,
                &wRxlen
                );


        }
    }


#ifndef NXPBUILD__PH_NDA_MFDF

    /* Should not get more bytes than the status bytes in case
    of no authentication */
    if (wRxlen > 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

#endif /* NXPBUILD__PH_NDA_MFDF */

    return PH_ERR_SUCCESS;
}


phStatus_t phalMfdf_Sw_Int_SendDataToPICC(
    phalMfdf_Sw_DataParams_t * pDataParams,
    uint8_t * pCmd,
    uint16_t wCmdLen,
    uint8_t * pData,
    uint16_t wDataLen,
    uint8_t * pResp,
    uint16_t * pRespLen
    )
{
    /* Utility function to send encrypted data to PICC as and when it is available from SAM */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bStatusByte;
    uint16_t    PH_MEMLOC_REM wIndex;
    uint16_t    PH_MEMLOC_REM wTmp;
    uint16_t    PH_MEMLOC_REM wLen;
    uint16_t    PH_MEMLOC_REM wFrameLen = 0;
    uint8_t     PH_MEMLOC_REM pApdu[5] = { PHAL_MFDF_WRAPPEDAPDU_CLA, 0x00, PHAL_MFDF_WRAPPEDAPDU_P1, PHAL_MFDF_WRAPPEDAPDU_P2, 0x00 };
    uint8_t     PH_MEMLOC_REM * pRecv = NULL;

    if (0U != (pDataParams->bWrappedMode))
    {
        wFrameLen = PHAL_MFDF_MAXWRAPPEDAPDU_SIZE;
    }
    else
    {
        wFrameLen = PHAL_MFDF_MAXDFAPDU_SIZE;
    }

    /* Send the data to PICC */
    wIndex = 0;
    wTmp = wDataLen;

    do
    {
        wLen = (wTmp < (wFrameLen - wCmdLen))? wTmp : (wFrameLen - wCmdLen);

        if (0U != (pDataParams->bWrappedMode))
        {
            pApdu[1] = pCmd[0]; /* MIFARE DESFire cmd code in INS */
            pApdu[4] = (uint8_t)(wCmdLen + wLen) - 0x01u;

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_FIRST,
                pApdu,
                PHAL_MFDF_WRAP_HDR_LEN,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp,  phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                &pCmd[1],
                wCmdLen - 1u,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                &pData[wIndex],
                wLen,
                &pRecv,
                pRespLen));

            /* Le byte */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                &pApdu[2],
                0x01,
                &pRecv,
                pRespLen));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_FIRST,
                pCmd,
                wCmdLen,
                &pRecv,
                pRespLen));

            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                &pData[wIndex],
                wLen,
                &pRecv,
                pRespLen));
        }
        wIndex = wIndex + wLen;
        wTmp = wTmp - wLen;

        if (0U != (pDataParams->bWrappedMode))
        {
            (void)memcpy(pResp, pRecv, ((uint32_t)(*pRespLen) - 2U));
            bStatusByte = pRecv[(*pRespLen) - 1U];
            (*pRespLen) -= 2u;
        }
        else
        {
            (void)memcpy(pResp, &pRecv[1], ((uint32_t)(*pRespLen) - 1U));
            bStatusByte = pRecv[0];
            (*pRespLen) -= 1u;
        }

        if ((bStatusByte != PHAL_MFDF_RESP_ADDITIONAL_FRAME) &&
            (bStatusByte != PH_ERR_SUCCESS))
        {


            return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
        }

        /* Success returned even before writing all data? protocol error */
        if ((bStatusByte == PH_ERR_SUCCESS) && (wTmp != 0U))
        {


            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        if(bStatusByte != 0x00U)
        {
            pCmd[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
            wCmdLen = 1;
        }
    }
    while(0U != wTmp);

    return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
}

phStatus_t phalMfdf_Sw_Int_IsoRead(
                                   phalMfdf_Sw_DataParams_t * pDataParams,
                                   uint16_t wOption,
                                   uint8_t * bCmdBuff,
                                   uint16_t wCmdLen,
                                   uint8_t ** ppRxBuffer,
                                   uint16_t * pBytesRead
                                   )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxBufferSize = 0;
    uint16_t    PH_MEMLOC_REM wNextPos;
    uint8_t     PH_MEMLOC_REM *pRecv = NULL;
    uint8_t     PH_MEMLOC_REM bBackupBytes[3];


    status = phpalMifare_ExchangeL4(
        pDataParams->pPalMifareDataParams,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        );

    /* First put everything on the reader Rx buffer upto buffer size - 60 */

    pRecv = *ppRxBuffer;

    if ((status != PH_ERR_SUCCESS) && ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {


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
        (void)memcpy(bBackupBytes, &pRecv[wNextPos - 3u], 3);

        if ((wNextPos + PHAL_MFDF_MAX_FRAME_SIZE) >= wRxBufferSize)
        {
            /* Calculate partical cmac if authenticated and return PH_ERR_SUCCESS_CHAINING */
            break;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        wNextPos
            ));
        status = phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_RXCHAINING,
            bCmdBuff,
            wCmdLen,
            ppRxBuffer,
            pBytesRead
            );

        /* Put back the backed up bytes */
        (void)memcpy(&pRecv[wNextPos - 3u], bBackupBytes, 3);

        if ((status != PH_ERR_SUCCESS) &&
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
        {


            return status;
        }
    }

    /* The data is now in *ppRxBuffer, length = wRxlen */

    /* Size of MAC bytes */

    if (status == PH_ERR_SUCCESS)
    {
        statusTmp = (*ppRxBuffer)[*pBytesRead - 2U]; /* SW1 */
        statusTmp = statusTmp << 8U; /* Shift SW1 to MSB */
        statusTmp |= (*ppRxBuffer)[*pBytesRead - 1U]; /* SW2 */

        if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
        {
            *pBytesRead -= 2U;
            return phalMfdf_Int_ComputeErrorResponse(pDataParams, statusTmp);
        }

        statusTmp = phalMfdf_Int_ComputeErrorResponse(pDataParams, statusTmp);
        if (statusTmp != PH_ERR_SUCCESS)
        {


            return statusTmp;
        }

    }
    return PH_ADD_COMPCODE((status & PH_ERR_MASK), PH_COMP_AL_MFDF);
}
#endif /* NXPBUILD__PHAL_MFDF_SW */
