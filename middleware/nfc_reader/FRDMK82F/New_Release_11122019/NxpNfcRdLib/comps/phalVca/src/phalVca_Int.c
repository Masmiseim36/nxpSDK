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
* Internal functions of Virtual Card Architecture Application Component.
* $Author: nxp79567 $
* $Revision: 1833 $ (v06.01.00)
* $Date: 2016-05-24 12:21:08 +0530 (Tue, 24 May 2016) $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#include <ph_Status.h>
#include <phpalMifare.h>
#include <phalVca.h>
#include "phalVca_Int.h"
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_VCA

phStatus_t phalVca_Int_ComputeErrorResponse(
        uint16_t wNumBytesReceived,
        uint8_t bStatus
    )
{
    phStatus_t PH_MEMLOC_REM status;

    /* validate received response */
    if (wNumBytesReceived > 1U)
    {
        if (bStatus != PHAL_VCA_RESP_ACK_ISO4)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
        }

        /* proper error response */
        return PH_ERR_SUCCESS;
    }
    else if (wNumBytesReceived == 1U)
    {
        switch (bStatus)
        {
        case PHAL_VCA_RESP_ACK_ISO4:

            status = PH_ERR_SUCCESS;
            break;

        case PHAL_VCA_RESP_ERR_CMD_INVALID:

            status = PHAL_VCA_ERR_CMD_INVALID;
            break;

        case PHAL_VCA_RESP_ERR_FORMAT:

            status = PHAL_VCA_ERR_FORMAT;
            break;

        case PHAL_VCA_RESP_ERR_GEN:

            status = PHAL_VCA_ERR_GEN;
            break;

        case PHAL_VCA_RESP_ERR_CMD_OVERFLOW:

            status = PHAL_VCA_ERR_CMD_OVERFLOW;
            break;
        default:

            status = PH_ERR_PROTOCOL_ERROR;
            break;
        }

        return PH_ADD_COMPCODE(status, PH_COMP_AL_VCA);
    }
    /* Invalid error response */
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }
}

phStatus_t phalVca_Int_ComputeErrorResponse_Extended(
        void * pDataParams,
        uint16_t wStatus
    )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    switch (wStatus)
    {
    case PHAL_VCA_RESP_ACK_ISO4:
    case PHAL_VCA_ISO7816_SUCCESS:
    case PHAL_VCA_ISO7816_PC_SUCCESS:
        status = PH_ERR_SUCCESS;
        break;
    case PHAL_VCA_RESP_ERR_CMD_INVALID:
        status = PHAL_VCA_ERR_CMD_INVALID;
        break;
    case PHAL_VCA_RESP_ERR_FORMAT:
        status = PHAL_VCA_ERR_FORMAT;
        break;
    case PHAL_VCA_RESP_ERR_GEN:
        status = PHAL_VCA_ERR_GEN;
        break;
    case PHAL_VCA_RESP_ERR_CMD_OVERFLOW:
        status = PHAL_VCA_ERR_CMD_OVERFLOW;
        break;
    case PHAL_VCA_RESP_ERR_COMMAND_ABORTED:
        status = PHAL_VCA_ERR_COMMAND_ABORTED;
        break;
    case PHAL_VCA_ISO7816_ERR_WRONG_LENGTH:
    case PHAL_VCA_ISO7816_ERR_WRONG_LE:
    case PHAL_VCA_ISO7816_ERR_FILE_NOT_FOUND:
    case PHAL_VCA_ISO7816_ERR_WRONG_PARAMS:
    case PHAL_VCA_ISO7816_ERR_WRONG_LC:
    case PHAL_VCA_ISO7816_ERR_NO_PRECISE_DIAGNOSTICS:
    case PHAL_VCA_ISO7816_ERR_EOF_REACHED:
    case PHAL_VCA_ISO7816_ERR_FILE_ACCESS:
    case PHAL_VCA_ISO7816_ERR_FILE_EMPTY:
    case PHAL_VCA_ISO7816_ERR_MEMORY_FAILURE:
    case PHAL_VCA_ISO7816_ERR_INCORRECT_PARAMS:
    case PHAL_VCA_ISO7816_ERR_WRONG_CLA:
    case PHAL_VCA_ISO7816_ERR_UNSUPPORTED_INS:
        status = PHAL_VCA_ERR_DF_7816_GEN_ERROR;
        /* Set the error code to VC param structure*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_SetConfig(
            pDataParams,
            PHAL_VCA_ADDITIONAL_INFO,
            wStatus));
        break;
    default:
        status = PH_ERR_PROTOCOL_ERROR;
        break;
    }
    return PH_ADD_COMPCODE(status, PH_COMP_AL_VCA);
}

phStatus_t phalVca_Int_PrepareProximityCheck(
        void * pPalMifareDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd[1];
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;

    /* command code */
    bCmd[0] = PHAL_VCA_CMD_PPC;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        1,
        &pResponse,
        &wRxLength));

    /* check response */
    if (wRxLength == 1U)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ERR_SUCCESS;
}

#ifdef NXPBUILD__PHAL_VCA_SW

phStatus_t phalVca_Int_PrepareProximityCheckNew(
        phalVca_Sw_DataParams_t * pDataParams,
        uint8_t *pOption,
        uint8_t *pPubRespTime,
        uint8_t *pPPS
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bCmd[1] = {PHAL_VCA_CMD_PPC};
    uint8_t     PH_MEMLOC_REM bIndex = 0;

    /* Check for ISO Wrapped Mode */
    if(0U != (pDataParams->bWrappedMode))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_SendISOWrappedCmd(
            pDataParams,
            bCmd,
            0x00,   /* Lc Value */
            &pResponse,
            &wRxLength
            ));
        if(wRxLength >= 2U)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse_Extended(pDataParams, pResponse[wRxLength - 1u]));
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
        }
        /* Adjusting the response length i.e. removing the consideration of response data */
        wRxLength -= 2u;
    }
    else
    {
        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            bCmd,
            1,
            &pResponse,
            &wRxLength));
        /* Computing the error response on SW1 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse_Extended(pDataParams, pResponse[0]));
        /* Incrementing the Index to point the response data */
        bIndex++;
        /* Adjusting the response length i.e. removing the consideration of response data */
        wRxLength--;
    }
    /* Check and save the contents of response data */
    if ((wRxLength == 3U) || (wRxLength == 4U))
    {
        /* Save Option from response data */
        *pOption = pResponse[bIndex++];
        /* Save Published Response Time from response data */
        pPubRespTime[0] = pResponse[bIndex++];
        pPubRespTime[1] = pResponse[bIndex++];
        if ((0U != ((*pOption & 0x01U))) && (wRxLength == 4U))
        {
            /* Save PPS from response data */
            *pPPS = pResponse[bIndex];
        }
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHAL_VCA_SW */

phStatus_t phalVca_Int_ProximityCheck(
        void * pPalMifareDataParams,            /**< [In] Pointer to a palMifare component context. */
        uint8_t bNumSteps,                      /**< [In] Number of steps for the PC (1..7). */
        uint8_t * pRndC,                        /**< [In] RndC[7]. */
        uint8_t * pRndRC                        /**< [Out] RndRC[14]. */
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd[1 /* command code */ + 1 /* length */ + 7 /* max RndC length */];
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bPayloadLen;
    uint8_t     PH_MEMLOC_REM bRndCLen = 0;
    uint8_t     PH_MEMLOC_REM bRndRCLen = 0;

    /* parameter checking */
    if ((bNumSteps == 0U) || (bNumSteps > PHAL_VCA_PC_RND_LEN) || (pRndC == NULL) || (pRndRC == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* command code */
    bCmd[0] = PHAL_VCA_CMD_PC;

    /* Proximity Check loop */
    while (0U != (bNumSteps--))
    {
        /* RndC length */
        if (0U != (bNumSteps))
        {
            bPayloadLen = 1;
        }
        else
        {
            bPayloadLen = PHAL_VCA_PC_RND_LEN - bRndCLen;
        }

        /* Length */
        bCmd[1] = bPayloadLen;

        /* RndC */
        (void)memcpy(&bCmd[2], &pRndC[bRndCLen], bPayloadLen);

        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangePc(
            pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            bCmd,
            (uint16_t)(2U + ((uint16_t)bPayloadLen)),
            &pResponse,
            &wRxLength));

        /* check response */
        if (wRxLength == (uint16_t)bPayloadLen)
        {
            /* copy RndR */
            (void)memcpy(&pRndRC[bRndRCLen], pResponse, wRxLength);
            bRndRCLen = bRndRCLen + (uint8_t)wRxLength;

            /* copy RndC */
            (void)memcpy(&pRndRC[bRndRCLen], &pRndC[bRndCLen], wRxLength);
            bRndRCLen = bRndRCLen + (uint8_t)wRxLength;
            bRndCLen = bRndCLen + (uint8_t)wRxLength;
        }
    }

    /* We expect to have exactly 7 bytes RndR + 7 bytes RndC */
    if (bRndRCLen != (PHAL_VCA_PC_RND_LEN * 2U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ERR_SUCCESS;
}

#ifdef NXPBUILD__PHAL_VCA_SW

phStatus_t phalVca_Int_ProximityCheckNew(
        phalVca_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to a palMifare component context. */
        uint8_t bNumSteps,                      /**< [In] Number of steps for the PC (1..7). */
        uint8_t * pPubRespTime,                 /**< [In] Published response time. */
        uint8_t * pRndC,                        /**< [In] RndC[7]. */
        uint8_t * pRndRC                        /**< [Out] RndRC[14]. */
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd[1 /* command code */ + 1 /* length */ + 8 /* max RndC length */];
    uint8_t *   PH_MEMLOC_REM pResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t     PH_MEMLOC_REM bPayloadLen;
    uint8_t     PH_MEMLOC_REM bRndCLen = 0;
    uint8_t     PH_MEMLOC_REM bRndRCLen = 0;
    uint16_t    PH_MEMLOC_REM wValue;
    uint16_t    PH_MEMLOC_REM wThresholdTime;
    phpalMifare_Sw_DataParams_t PH_MEMLOC_REM *pPalDataParamsTmp = NULL;


    pPalDataParamsTmp = (phpalMifare_Sw_DataParams_t *)pDataParams->pPalMifareDataParams;
    /* parameter checking */
    if ((bNumSteps == 0U) || (bNumSteps > PHAL_VCA_PC_RND_LEN_NEW) || (pRndC == NULL) || (pRndRC == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_VCA);
    }

    /* command code */
    bCmd[0] = PHAL_VCA_CMD_PC;

    /* Proximity Check loop */
    while (0U != (bNumSteps--))
    {
        /* RndC length */
        if (0U != (bNumSteps))
        {
            bPayloadLen = 1;
        }
        else
        {
            bPayloadLen = PHAL_VCA_PC_RND_LEN_NEW - bRndCLen;
        }

        /* Length */
        bCmd[1] = bPayloadLen;

        /* RndC */
        (void)memcpy(&bCmd[2], &pRndC[bRndCLen], bPayloadLen);

        /* Get the bOption value for the checking the timing measurement ON/OFF */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_GetConfig(pDataParams, PHAL_VCA_TIMING_MODE, &wValue));
        if(0U != (wValue & 0x01U))
        {
            /* Start collecting the RC timeout */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pPalDataParamsTmp->pHalDataParams, PHHAL_HW_CONFIG_TIMING_MODE, PHHAL_HW_TIMING_MODE_FDT));
        }
        /* Check for ISO Wrapped Mode */
        if(0U != (pDataParams->bWrappedMode))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_SendISOWrappedCmd(
                pDataParams,
                bCmd,
                (uint8_t)(1U + bPayloadLen),    /* bPayloadLen + RndC */
                &pResponse,
                &wRxLength
                ));
            if(wRxLength >= 2U)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse_Extended(pDataParams, pResponse[wRxLength - 1u]));
            }
            else
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
            }
            /* Decrementing wRxLength by 2 i.e. removing status word from pResponse */
            wRxLength -= 2u;
        }
        else
        {
            /* command exchange */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_DEFAULT,
                bCmd,
                (uint16_t)(2U + ((uint16_t)bPayloadLen)),    /* (INS + bPayloadLen) + RndC */
                &pResponse,
                &wRxLength));
        }

        /* check response */
        if (wRxLength == (uint16_t)bPayloadLen)
        {
            /* copy RndR */
            (void)memcpy(&pRndRC[bRndRCLen], pResponse, wRxLength);
            bRndRCLen = bRndRCLen + (uint8_t)wRxLength;

            /* copy RndC */
            (void)memcpy(&pRndRC[bRndRCLen], &pRndC[bRndCLen], wRxLength);
            bRndRCLen = bRndRCLen + (uint8_t)wRxLength;
            bRndCLen = bRndCLen + (uint8_t)wRxLength;
        }

        /* Get the bOption value for the checking the timing measurement ON/OFF */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_GetConfig(pDataParams, PHAL_VCA_TIMING_MODE, &wValue));
        if(0U != (wValue & 0x01U))
        {
            /* Compute threshold time from PubRespTime. Threshold time = pubRespTime + 10% of pubRespTime */
            wThresholdTime = pPubRespTime[0];
            wThresholdTime <<= 8U;
            wThresholdTime |= pPubRespTime[1];

            /* Check whether user has enabled the lower boundary threshold*/
            PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_GetConfig(pDataParams, PHAL_VCA_PC_LOWER_THRESHOLD, &wValue));
            if(0U != (wValue & 0x01U))
            {
                /* As per the ref arch V0.17, the threshold time should not be 20% beyond the Lower bound of PubResp Time. */
                wThresholdTime = wThresholdTime - (((uint16_t)(wThresholdTime * 20U))/100U);
            }

            /* Get the last command execution time */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pPalDataParamsTmp->pHalDataParams, PHHAL_HW_CONFIG_TIMING_US, &wValue));

            /* If the response is not received within the threshold time, return internal error */
            if(wValue > wThresholdTime)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_AL_VCA);
            }
        }
    }
    /* We expect to have exactly 8 bytes RndR + 8 bytes RndC */
    if (bRndRCLen != (PHAL_VCA_PC_RND_LEN_NEW * 2U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phalVca_Int_SendISOWrappedCmd(
        phalVca_Sw_DataParams_t * pDataParams,
        uint8_t * pSendBuff,
        uint8_t  bLc,
        uint8_t ** pResponse,
        uint16_t * pRxlen
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp = 0;
    phStatus_t PH_MEMLOC_REM status = 0;
    uint8_t    PH_MEMLOC_REM bApduLen = 4;  /* Initializing with 4 since Length of the Data(Lc) starts from 4th element of pApdu[] */
    uint8_t    PH_MEMLOC_REM pApdu[8] = { 0x90 /* CLS */, 0x00, 0x00, 0x00, 0x00 /* Lc */, 0x00, 0x00 /*  Lc for Extended Length */, 0x00 /* Le */ };

    /* Check for permissible CmdBuff size */
    if (bLc > PHAL_VCA_MAXWRAPPEDAPDU_SIZE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_MFDFEV2);
    }

    pApdu[1] = pSendBuff[0];  /* Proximity Check Command Code. */

    switch(pApdu[1])
    {
    case PHAL_VCA_CMD_PPC:
        pApdu[4] = 0x00;    /* These bytes will be treated as Le */
        pApdu[5] = 0x00;    /* For extended length Apdu support */
        /* Transmit CLS INS P1 P2 Lc(not Lc for PPC) as buffer first */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            pApdu,
            (uint16_t)((pDataParams->bExtendedLenApdu != 0U) ? 7U : 5U),  /* 2 bytes Le should be passed in case of Extended Length Apdu since Lc field is not present */
            pResponse,
            pRxlen
            ));
        break;
    case PHAL_VCA_CMD_PC:
    case PHAL_VCA_CMD_VPC:
        /* To Note: Extended APDU will be used,
         *  When user forces the 'length' to be sent as Extended length APDU. */
        if(0U == (pDataParams->bExtendedLenApdu))
        {
            /* Encode 'Length' in Short APDU format */
            pApdu[bApduLen++]= (uint8_t)bLc; /* Set Data Length. */
        }
        else
        {
            /* Encode 'Length' in extended Length format */
            pApdu[bApduLen++] = 0x00;
            pApdu[bApduLen++] = 0x00;
            pApdu[bApduLen++] = (uint8_t)bLc; /* Set Data Length. */
        }
        /* Transmit CLS INS P1 P2 Lc as buffer first */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            pApdu,
            bApduLen,
            pResponse,
            pRxlen
            ));
        /* Check for Lc value */
        if (bLc > 0U)
        {
            /* Transmit data as continued buffer */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
                pDataParams->pPalMifareDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                &pSendBuff[1],
                bLc,
                pResponse,
                pRxlen
                ));
        }
        /* Resetting bApduLen for further use in case of Le */
        bApduLen = 0;
        if(0U == (pDataParams->bExtendedLenApdu))
        {
            /* Encode 'Length' in Short APDU format */
            pApdu[bApduLen++]= 0x00; /* Set the expected data length as full. */
        }
        else
        {
            /* Encode 'Length' in extended Length format */
            pApdu[bApduLen++] = 0x00;
            pApdu[bApduLen++] = 0x00; /* Set the expected data length as full. */
        }
        /* Transmit Le as buffer Last */
        PH_CHECK_SUCCESS_FCT(status,phpalMifare_ExchangeL4(
            pDataParams->pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pApdu,
            bApduLen,
            pResponse,
            pRxlen
            ));
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_VCA);
    }

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHAL_VCA_SW */

#endif /* NXPBUILD__PHAL_VCA */
