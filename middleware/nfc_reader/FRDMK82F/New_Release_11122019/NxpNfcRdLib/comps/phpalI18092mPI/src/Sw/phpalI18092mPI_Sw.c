/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
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
* Software ISO18092 passive initiator mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CEn: Generated 07. September 2010
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI18092mPI.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I18092MPI_SW

#include "phpalI18092mPI_Sw_Int.h"
#include "phpalI18092mPI_Sw.h"

/* static const uint16_t PH_MEMLOC_CONST_ROM aI18092_FsTable[4] = {64, 128, 192, 253};
*  We need to add one because for calculating the maximum payload length, we should have byte for length field
*  which will be added as a part of payload...The Lower tester confirms the payload is the string without length
*  field.
*  */
static const uint16_t PH_MEMLOC_CONST_ROM aI18092_FsTable[4] = {65, 129, 193, 255};
static const uint16_t PH_MEMLOC_CONST_ROM aI18092_DrTable[4] = {PHHAL_HW_RF_DATARATE_106, PHHAL_HW_RF_DATARATE_212, PHHAL_HW_RF_DATARATE_424, PHHAL_HW_RF_DATARATE_848};

phStatus_t phpalI18092mPI_Sw_Init(
                                  phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                  uint16_t wSizeOfDataParams,
                                  void * pHalDataParams
                                  )
{
    if (sizeof(phpalI18092mPI_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_I18092MPI);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* Init private data */
    pDataParams->wId            = PH_COMP_PAL_I18092MPI | PHPAL_I18092MPI_SW_ID;
    pDataParams->pHalDataParams = pHalDataParams;

    /* Reset protocol to defaults */
    return phpalI18092mPI_Sw_ResetProtocol(pDataParams);
}

phStatus_t phpalI18092mPI_Sw_ResetProtocol(
    phpalI18092mPI_Sw_DataParams_t * pDataParams
    )
{
    /* Apply default parameters */
    pDataParams->bNfcIdValid    = PH_OFF;
    pDataParams->bStateNow      = PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX;
    pDataParams->bDid           = 0;    /* set DID to 0, i.e. disable usage of DID */
    pDataParams->bNadEnabled    = PH_OFF;
    pDataParams->bNad           = 0;
    pDataParams->bWt            = PHPAL_I18092MPI_WT_MAX;
    pDataParams->bFsl           = PHPAL_I18092MPI_FRAMESIZE_64;
    pDataParams->bLri           = PHPAL_I18092MPI_FRAMESIZE_64;
    pDataParams->bLrt           = PHPAL_I18092MPI_FRAMESIZE_64;
    pDataParams->bPni           = 0;    /* NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.1 */
    pDataParams->bDsi           = 0;
    pDataParams->bDri           = 0;
    pDataParams->bMaxRetryCount = PHPAL_I18092MPI_SW_RETRIES_DEFAULT;
    pDataParams->bAtnDisabled   = 0;
    pDataParams->bActiveMode    = 0;
    pDataParams->bOpeMode       = RD_LIB_MODE_NFC;
    pDataParams->bPropPSLMode   = PH_OFF;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_Atr(
                                 phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                 uint8_t * pNfcid3i,
                                 uint8_t bDid,
                                 uint8_t bLri,
                                 uint8_t bNadEnabled,
                                 uint8_t bNad,
                                 uint8_t * pGi,
                                 uint8_t bGiLength,
                                 uint8_t * pAtrRes,
                                 uint8_t * pAtrResLength
                                 )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM wPreviousStatus;
    uint8_t     PH_MEMLOC_REM aCmd[14];
    uint8_t     PH_MEMLOC_REM bCmdLen;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wResponseLength = 0;
    uint16_t    PH_MEMLOC_REM wFrameLength = 0;
    uint8_t *   PH_MEMLOC_REM pFrame = NULL;
    uint8_t *   PH_MEMLOC_REM pPayload = NULL;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint16_t    PH_MEMLOC_REM wActiveMode;
    uint8_t     PH_MEMLOC_REM bRetry;
    uint8_t     PH_MEMLOC_COUNT bRetryCount;
    uint8_t     PH_MEMLOC_REM bResponseReceived;
    uint8_t     PH_MEMLOC_REM bTo;
    uint16_t    PH_MEMLOC_REM wDsi = 0;
    uint16_t    PH_MEMLOC_REM wDri;
    uint32_t    PH_MEMLOC_REM dwRwt;
    uint32_t    PH_MEMLOC_REM dwShift = 1U;

    wPreviousStatus = 0;
    bRetryCount = 0;
    /* Parameter check */
    if ((bDid > PHPAL_I18092MPI_DID_MAX) ||
        (bLri > PHPAL_I18092MPI_FRAMESIZE_254) ||
        (bGiLength > PHPAL_I18092MPI_MAX_GI_LENGTH))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
    }

    /* Make NfcID invalid */
    pDataParams->bNfcIdValid = PH_OFF;

    /* Set new DID */
    pDataParams->bDid = bDid;

    /* Build up frame data. */
    (void)memcpy(aCmd, pNfcid3i, PHPAL_I18092MPI_NFCID3_LENGTH);
    bCmdLen = PHPAL_I18092MPI_NFCID3_LENGTH;
    aCmd[bCmdLen++] = bDid;

    /* Next two fields (BSi and BRi) only used for active mode, so set them to 0. */
    aCmd[bCmdLen++] = 0x00;
    aCmd[bCmdLen++] = 0x00;

    /* Configure PPi field */
    aCmd[bCmdLen++] = (bLri << PHPAL_I18092MPI_SW_ATR_PPI_LRI_BIT);
    if (0U != (bNadEnabled))
    {
        aCmd[bCmdLen - 1U] |= 0x01U;
    }

    /* Get initial set data rate from HAL since NFC allows starting at different data rates. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXDATARATE_FRAMING, &wDsi));
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ConvertDatarate(wDsi, &pDataParams->bDsi));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXDATARATE_FRAMING, &wDri));
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ConvertDatarate(wDri, &pDataParams->bDri));

    /* Configure HAL for handling StartByte in case Transmit datarate is 106kbps. */
    if (pDataParams->bDsi == PHHAL_HW_RF_DATARATE_106)
    {
        /* Configure HAL for NFCIP to enable handling StartByte at 106kbps. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_ON));
    }
    else
    {
        /* Disabled handling of StartByte if datarate is other than 106kbps. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_OFF));
    }

    /* Set Activation timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
        PHPAL_I18092MPI_SW_RWT_ACTIVATION_MS + PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS));

    /* Reset response received flag */
    bResponseReceived = 0;

    /* Retry loop */
    do
    {
        /* Reset retry flag */
        bRetry = PH_OFF;

        /* Write frame header. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WriteFrameHeader(pDataParams, PH_EXCHANGE_BUFFER_FIRST, PHPAL_I18092MPI_SW_CMD_ATR_REQ, (bCmdLen + bGiLength)));

        /* Append general bytes if present */
        if (bGiLength > 0U)
        {
            /* Mark that general bytes are present */
            aCmd[bCmdLen - 1U] |= PHPAL_I18092MPI_SW_ATR_RES_PPT_GT_AVAILABLE;

            /* Write data to internal buffer. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
                pDataParams->pHalDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                aCmd,
                (uint16_t)bCmdLen,
                NULL,
                NULL));

            /* Write general bytes, transmit the whole buffer and receive response. */
            status = phhalHw_Exchange(pDataParams->pHalDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                pGi,
                bGiLength,
                &pResp,
                &wResponseLength);
        }
        else
        {
            /* No additional general bytes to be sent so transmit the buffer now. */
            status = phhalHw_Exchange(pDataParams->pHalDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                aCmd,
                (uint16_t)bCmdLen,
                &pResp,
                &wResponseLength);
        }

        /* Get the state of activation mode status either indication active/passive mode. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_ACTIVEMODE,
            &wActiveMode));
        pDataParams->bActiveMode = (uint8_t)wActiveMode;

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Signal that we've received something */
            bResponseReceived = 1;

            /* Check for valid response frame and remove start byte if present. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ValidateAndNormalizeResponseFrame(
                pDataParams,
                pResp,
                wResponseLength,
                PHPAL_I18092MPI_SW_CMD_ATR_RES,
                &pFrame,
                &wFrameLength,
                &pPayload,
                &wPayloadLength));

            /* Ignore length byte */
            ++pFrame;
            --wFrameLength;

            /* Check for minimum and maximum frame size. NFCForum-TS-DigitalProtocol-1.0 section 14.6.1.3. */
            if ((wFrameLength > PHPAL_I18092MPI_SW_MAX_ACTIVATION_FRAME_LENGTH) ||
                (wFrameLength < PHPAL_I18092MPI_SW_MIN_ACTIVATION_FRAME_LENGTH))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }

            /* Check Gt presence / absence */
            if (0U != (pFrame[PHPAL_I18092MPI_SW_ATR_RES_PPT_POS] & PHPAL_I18092MPI_SW_ATR_RES_PPT_GT_AVAILABLE))
            {
                /* Gt should be present */
                if (wFrameLength <= PHPAL_I18092MPI_SW_MIN_ACTIVATION_FRAME_LENGTH)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }
            else
            {
                /* Gt should be absent */
                if (wFrameLength > PHPAL_I18092MPI_SW_MIN_ACTIVATION_FRAME_LENGTH)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }

            /* Protocol error in case DID is not the same. NFCForum-TS-DigitalProtocol-1.0 section 14.6.3.2. */
            if (pFrame[PHPAL_I18092MPI_SW_ATR_RES_DID_POS] != bDid)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }

            /* Check NadUsed bit */
            if (0U != (pFrame[PHPAL_I18092MPI_SW_ATR_RES_PPT_POS] & PHPAL_I18092MPI_SW_ATR_RES_PPT_NAD_USED))
            {
                /* Protocol error if NAD use is requested to be disabled */
                if (bNadEnabled == PH_OFF)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }

                pDataParams->bNad = bNad;
                pDataParams->bNadEnabled = PH_ON;
            }
            else
            {
                pDataParams->bNad = 0x00;
                pDataParams->bNadEnabled = PH_OFF;
            }

            /* Copy ATR_RES to response buffer. */
            (void)memcpy(pAtrRes, pFrame, wFrameLength);
            *pAtrResLength = (uint8_t)wFrameLength;

            /* Update LRi */
            pDataParams->bLri = bLri;

            /* Retrieve TO */
            bTo = pAtrRes[PHPAL_I18092MPI_SW_ATR_RES_TO_POS] & 0xFU;

            /* Adjust timeout field if necessary. NFCForum-TS-DigitalProtocol-1.0 section 14.6.3.8. */
            if (bTo > PHPAL_I18092MPI_WT_MAX)
            {
                bTo = PHPAL_I18092MPI_WT_MAX;
            }

            /* Parse the response to see what frame size the target supports. */
            pDataParams->bLrt = (pAtrRes[PHPAL_I18092MPI_SW_ATR_RES_PPT_POS] >> 4U) & 0x3U;

            /* Set FSL to the lowest frame size (lowest of LRi and LRt) */
            pDataParams->bFsl = (pDataParams->bLrt < pDataParams->bLri) ? pDataParams->bLrt : pDataParams->bLri;

            /* Store TO */
            pDataParams->bWt = bTo;

            /* NfcID is now valid */
            pDataParams->bNfcIdValid = PH_ON;
            (void)memcpy(pDataParams->aNfcid3i, &pAtrRes[PHPAL_I18092MPI_SW_ATR_RES_NFCID3_POS], PHPAL_I18092MPI_NFCID3_LENGTH);

            /* Calculate Response Waiting Time. */
            dwShift = 1U;
            dwRwt = (uint32_t)(PHPAL_I18092MPI_SW_RWT_MIN_US * ((dwShift << bTo)));
            dwRwt += PHPAL_I18092MPI_SW_DELTA_RWT_US;
            dwRwt /= 1000U;
            dwRwt += 1U;
            dwRwt += PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS;

            /* Set Response Waiting Time. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                (uint16_t)dwRwt));
        }
        else
        {
            /* If HAL returned Integrity Error first time then save and return this status at the end of Retry. */
            if (((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR) && (bRetryCount == 0U))
            {
                wPreviousStatus = status;
            }

            /* Retransmit in case of timeout or receive error, ISO/IEC 18092:2004(E) section 12.5.1.3.1. */
            if (0u != (PHPAL_I18092MPI_SW_IS_INVALID_BLOCK_STATUS(status)))
            {
                /* Check if we shall perform another retry. */
                if (bRetryCount < pDataParams->bMaxRetryCount)
                {
                    ++bRetryCount;
                    bRetry = PH_ON;

                    if (wActiveMode == PH_ON)
                    {
                        /* Wait for T_IRFG (Initial RF guard time) before ATR retry. */
                        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(
                            pDataParams->pHalDataParams,
                            PHHAL_HW_TIME_MILLISECONDS,
                            (uint16_t)PHPAL_I18092MPI_SW_TIRFG_MIN_MS));
                    }
                }
                else
                {
                    /* Deselect target ISO/IEC 18092:2004(E) section 12.5.1.3.1. */
                    statusTmp = phpalI18092mPI_Sw_Deselect(pDataParams, PHPAL_I18092MPI_DESELECT_RLS);

                    /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behaviour) */
                    if ((pDataParams->bMaxRetryCount > 0U) && (0u != bResponseReceived))
                    {
                        status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                    }
                }
            }
        }
    }
    /* Retry loop */
    while (bRetry != PH_OFF);

    if ((wPreviousStatus != 0U) && (status != PH_ERR_SUCCESS))
    {
        return wPreviousStatus;
    }
    return status;
}

phStatus_t phpalI18092mPI_Sw_Psl(
                                 phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                 uint8_t bDsi,
                                 uint8_t bDri,
                                 uint8_t bFsl
                                 )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint32_t    PH_MEMLOC_REM dwTimeout;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wResponseLength = 0;
    uint16_t    PH_MEMLOC_REM wFrameLength = 0;
    uint8_t *   PH_MEMLOC_REM pFrame = NULL;
    uint8_t *   PH_MEMLOC_REM pPayload = NULL;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint16_t    PH_MEMLOC_REM wTxSettings = 0;
    uint16_t    PH_MEMLOC_REM wRxSettings = 0;
    uint16_t    PH_MEMLOC_REM wActiveMode;
    uint8_t     PH_MEMLOC_REM aCmd[3];
    uint8_t     PH_MEMLOC_COUNT bRetryCount = 0;
    uint8_t     PH_MEMLOC_REM bResponseReceived;
    uint8_t     PH_MEMLOC_REM bRetry;
    uint8_t     PH_MEMLOC_REM bCheckBaudrate = PHHAL_HW_RF_DATARATE_424;
    uint8_t     PH_MEMLOC_REM bNFCMode = PH_OFF;
    uint32_t    PH_MEMLOC_REM dwShift = 1U;

    /* Parameter check */
    if ((bDsi > bCheckBaudrate) ||
        (bDri > bCheckBaudrate) ||
        (bFsl > PHPAL_I18092MPI_FRAMESIZE_254))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
    }

    /* FSL shall be less than or equal to lowest of LRi and LRt */
    if((bFsl > pDataParams->bLri) || (bFsl > pDataParams->bLrt))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
    }

    /* Prepare command */
    aCmd[0] = pDataParams->bDid;
    aCmd[1] = (bDsi << 3U) | bDri;
    aCmd[2] = bFsl;

    /* Reset response received flag */
    bResponseReceived = 0;

    /* Calculate Response Waiting Time. */
    dwTimeout = (uint32_t)(PHPAL_I18092MPI_SW_RWT_MIN_US * ((uint32_t)(dwShift << pDataParams->bWt)));
    dwTimeout += PHPAL_I18092MPI_SW_DELTA_RWT_US;
    dwTimeout /= 1000U;
    dwTimeout += 1U;
    dwTimeout += PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS;

    /* Set Response Waiting Time as per NFCForum-TS-DigitalProtocol-1.1 section 16.11.2.3. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
        (uint16_t)dwTimeout));

    /* Retry loop */
    do
    {
        /* Reset retry flag */
        bRetry = PH_OFF;

        /* Write frame header and update length field. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WriteFrameHeader(pDataParams, PH_EXCHANGE_BUFFER_FIRST, PHPAL_I18092MPI_SW_CMD_PSL_REQ, 3));

        /* Write data to internal buffer and start transmission. */
        status = phhalHw_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            aCmd,
            3,
            &pResp,
            &wResponseLength);

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Signal that we've received something */
            bResponseReceived = 1;

            /* Check for valid response frame. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ValidateAndNormalizeResponseFrame(
                pDataParams,
                pResp,
                wResponseLength,
                PHPAL_I18092MPI_SW_CMD_PSL_RES,
                &pFrame,
                &wFrameLength,
                &pPayload,
                &wPayloadLength));

            /* Protocol error in case DID is not the same. NFCForum-TS-DigitalProtocol-1.0 section 14.7.2.1. */
            if ((wPayloadLength != 1U) || (pPayload[0] != pDataParams->bDid))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_ACTIVEMODE, &wActiveMode));
            if (wActiveMode == PH_ON)
            {
                /* Set the active mode framing */
                wTxSettings = aI18092_DrTable[bDsi] | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING;
                wRxSettings = aI18092_DrTable[bDri] | PHHAL_HW_RF_TYPE_ACTIVE_FRAMING;
            }
            /* For Passive Mode Framing set corresponding Technology Framing */
            else
            {
                if (pDataParams->bPropPSLMode == PH_OFF)
                {
                    if (aI18092_DrTable[bDsi] == PHHAL_HW_RF_DATARATE_106)
                    {
                        wTxSettings = aI18092_DrTable[bDsi] | PHHAL_HW_RF_TYPE_A_FRAMING;
                    }
                    else
                    {
                        wTxSettings = aI18092_DrTable[bDsi] | PHHAL_HW_RF_TYPE_F_FRAMING;
                    }

                    if (aI18092_DrTable[bDri] == PHHAL_HW_RF_DATARATE_106)
                    {
                        wRxSettings = aI18092_DrTable[bDri] | PHHAL_HW_RF_TYPE_A_FRAMING;
                    }
                    else
                    {
                        wRxSettings = aI18092_DrTable[bDri] | PHHAL_HW_RF_TYPE_F_FRAMING;
                    }
                }
            }
            /* Apply Tx-datarate to HAL. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXDATARATE_FRAMING,
                wTxSettings));

            /* Apply Rx-datarate to HAL. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
                wRxSettings));


            /* Configure HAL for handling StartByte in case Transmit datarate is 106kbps. */
            if ((aI18092_DrTable[bDsi] == PHHAL_HW_RF_DATARATE_106) || (bNFCMode == PH_ON))
            {
                /* Configure HAL for NFCIP to enable handling StartByte at 106kbps. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_ON));
            }
            else
            {
                /* Disabled handling of StartByte if datarate is other than 106kbps. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_NFCIP_STARTBYTE, PH_OFF));
            }

            /* Store protocol params */
            pDataParams->bFsl = bFsl;
            pDataParams->bDsi = bDsi;
            pDataParams->bDri = bDri;

            /* Set LRi and LRt to FSL */
            pDataParams->bLri = bFsl;
            pDataParams->bLrt = bFsl;
        }
        else
        {
            /* Retransmit in case of timeout or receive error, ISO/IEC 18092:2004(E) section 12.5.3.3.1. */
            if (0u != (PHPAL_I18092MPI_SW_IS_INVALID_BLOCK_STATUS(status)))
            {
                /* Check if we shall perform a(nother) retry. */
                if (bRetryCount < pDataParams->bMaxRetryCount)
                {
                    ++bRetryCount;
                    bRetry = PH_ON;
                }
                else
                {
                    /* Deselect target ISO/IEC 18092:2004(E) section 12.5.1.3.1. */
                    statusTmp = phpalI18092mPI_Sw_Deselect(pDataParams, PHPAL_I18092MPI_DESELECT_RLS);

                    /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behaviour) */
                    if ((pDataParams->bMaxRetryCount > 0U) && (0u != bResponseReceived))
                    {
                        status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                    }
                }
            }
        }
    }
    /* Retry loop */
    while (bRetry != PH_OFF);

    return status;
}

phStatus_t phpalI18092mPI_Sw_ActivateCard(
    phpalI18092mPI_Sw_DataParams_t * pDataParams,
    uint8_t * pNfcid3i,
    uint8_t bDid,
    uint8_t bNadEnable,
    uint8_t bNad,
    uint8_t bDsi,
    uint8_t bDri,
    uint8_t bFsl,
    uint8_t * pGi,
    uint8_t bGiLength,
    uint8_t * pAtrRes,
    uint8_t * pAtrResLength
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* Parameter check */
    if ((bDid > PHPAL_I18092MPI_DID_MAX) ||
        (bDsi > PHPAL_I18092MPI_DATARATE_848) ||
        (bDri > PHPAL_I18092MPI_DATARATE_848) ||
        (bFsl > PHPAL_I18092MPI_FRAMESIZE_254))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
    }

    /* First, send ATR request command. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_Atr(pDataParams,
        pNfcid3i,
        bDid,
        bFsl,
        bNadEnable,
        bNad,
        pGi,
        bGiLength,
        pAtrRes,
        pAtrResLength));

    /* FSL shall be less than or equal to lowest of LRi and LRt */
    bFsl = (pDataParams->bLrt < pDataParams->bLri) ? pDataParams->bLrt : pDataParams->bLri;

    return phpalI18092mPI_Sw_Psl(pDataParams, bDsi, bDri, bFsl);
}

phStatus_t phpalI18092mPI_Sw_Deselect(
                                      phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                      uint8_t bDeselectCommand
                                      )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint32_t    PH_MEMLOC_REM dwTimeout;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bExpectedResponse;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wResponseLength = 0;
    uint8_t *   PH_MEMLOC_REM pFrame = NULL;
    uint16_t    PH_MEMLOC_REM wFrameLength = 0;
    uint8_t *   PH_MEMLOC_REM pPayload = NULL;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t     PH_MEMLOC_COUNT bRetryCount = 0;
    uint8_t     PH_MEMLOC_REM bResponseReceived;
    uint8_t     PH_MEMLOC_REM bRetry;
    uint32_t    PH_MEMLOC_REM dwShift = 1U;

    /* Make NfcID invalid */
    pDataParams->bNfcIdValid = PH_OFF;

    /* Calculate Response Waiting Time. */
    dwShift = 1U;
    dwTimeout = (uint32_t)(PHPAL_I18092MPI_SW_RWT_MIN_US * ((uint32_t)(dwShift << pDataParams->bWt)));
    dwTimeout += PHPAL_I18092MPI_SW_DELTA_RWT_US;
    dwTimeout /= 1000U;
    dwTimeout += 1U;
    dwTimeout += PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS;

    /* Set Response Waiting Time as per NFCForum-TS-DigitalProtocol-1.1 section 16.11.2.3. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
        (uint16_t)dwTimeout));

    /* Append DID if enabled */
    if (pDataParams->bDid > 0U)
    {
        aCmd[bCmdLen++] = pDataParams->bDid;
    }

    /* Reset response received flag */
    bResponseReceived = 0;

    /* Retry loop */
    do
    {
        /* Reset retry flag */
        bRetry = PH_OFF;

        /* Write frame header and update length field. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WriteFrameHeader(pDataParams,PH_EXCHANGE_BUFFER_FIRST, bDeselectCommand, bCmdLen));

        /* Start transmission. */
        status = phhalHw_Exchange(pDataParams->pHalDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            aCmd,
            (uint16_t)bCmdLen,
            &pResp,
            &wResponseLength);

        /* Exchange was successful */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Signal that we've received something */
            bResponseReceived = 1;

            if (bDeselectCommand == PHPAL_I18092MPI_DESELECT_DSL)
            {
                bExpectedResponse = PHPAL_I18092MPI_SW_CMD_DSL_RES;
            }
            else
            {
                bExpectedResponse = PHPAL_I18092MPI_SW_CMD_RLS_RES;
            }

            /* Check for valid response frame. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ValidateAndNormalizeResponseFrame(
                pDataParams,
                pResp,
                wResponseLength,
                bExpectedResponse,
                &pFrame,
                &wFrameLength,
                &pPayload,
                &wPayloadLength));

            /* Check DID */
            if (pDataParams->bDid > 0U)
            {
                /* Protocol error in case DID is not the same. NFCForum-TS-DigitalProtocol-1.0 section 14.9.2.1. */
                if ((wPayloadLength != 1U) || (pPayload[0] != pDataParams->bDid))
                {
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }
            else
            {
                /* DID must not be present in response. NFCForum-TS-DigitalProtocol-1.0 section 14.9.2.1. */
                if (wPayloadLength != 0U)
                {
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }
            }
        }
        else
        {
            /* Check if we shall perform a(nother) retry. */
            if (bRetryCount < pDataParams->bMaxRetryCount)
            {
                ++bRetryCount;
                bRetry = PH_ON;
            }
            else
            {
                /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behaviour) */
                if ((pDataParams->bMaxRetryCount > 0U) && (0u != bResponseReceived))
                {
                    status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                }
            }
        }
    }
    /* Retry loop */
    while (bRetry != PH_OFF);

    return status;
}

phStatus_t phpalI18092mPI_Sw_PresCheck(
                                       phpalI18092mPI_Sw_DataParams_t * pDataParams
                                       )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint32_t    PH_MEMLOC_REM dwTimeout;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wResponseLength = 0;
    uint8_t *   PH_MEMLOC_REM pFrame = NULL;
    uint16_t    PH_MEMLOC_REM wFrameLength = 0;
    uint8_t *   PH_MEMLOC_REM pPayload = NULL;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t     PH_MEMLOC_COUNT bRetryCount = 0;
    uint8_t     PH_MEMLOC_REM bResponseReceived;
    uint8_t     PH_MEMLOC_REM bRetry;
    uint32_t    PH_MEMLOC_REM dwShift = 1U;

    /* Reset response received flag */
    bResponseReceived = 0;

    /* Calculate Response Waiting Time. */
    dwTimeout = (uint32_t)(PHPAL_I18092MPI_SW_RWT_MIN_US * ((uint32_t)(dwShift << pDataParams->bWt)));
    dwTimeout += PHPAL_I18092MPI_SW_DELTA_RWT_US;
    dwTimeout /= 1000U;
    dwTimeout += 1U;
    dwTimeout += PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS;

    /* Set Response Waiting Time as per NFCForum-TS-DigitalProtocol-1.1 section 16.11.2.3. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
        (uint16_t)dwTimeout));

    /* Retry loop. */
    do
    {
        /* Reset retry flag */
        bRetry = PH_OFF;

        /* Write Attention PDU header. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
            pDataParams,
            PHPAL_I18092MPI_SW_PFB_SUPERVISORY_PDU_MASK,
            PH_OFF,
            0));

        /* Start transmission. */
        status = phhalHw_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            NULL,
            0,
            &pResp,
            &wResponseLength);

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Signal that we've received something */
            bResponseReceived = 1;

            /* Check for valid response frame. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ValidateAndNormalizeResponseFrame(
                pDataParams,
                pResp,
                wResponseLength,
                PHPAL_I18092MPI_SW_CMD_DEP_RES,
                &pFrame,
                &wFrameLength,
                &pPayload,
                &wPayloadLength));

            /* Check for attention bit set. */
            if ((PHPAL_I18092MPI_SW_IS_ATTENTION(pFrame[3])) == 0U)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
        }
        else
        {
            if (0u != (PHPAL_I18092MPI_SW_IS_INVALID_BLOCK_STATUS(status)))
            {
                /* Check if we shall perform a(nother) retry. */
                if (bRetryCount < pDataParams->bMaxRetryCount)
                {
                    ++bRetryCount;
                    bRetry = PH_ON;
                }
                else
                {
                    /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behaviour) */
                    if ((pDataParams->bMaxRetryCount > 0U) && (0u != bResponseReceived))
                    {
                        status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                    }
                }
            }
        }
    }
    while (bRetry != PH_OFF);

    return status;
}

phStatus_t phpalI18092mPI_Sw_Exchange(
                                      phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t * pTxBuffer,
                                      uint16_t wTxLength,
                                      uint8_t ** ppRxBuffer,
                                      uint16_t * pRxLength
                                      )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* Used to build block frames */
    uint16_t    PH_MEMLOC_REM wTxBufferSize;
    uint16_t    PH_MEMLOC_REM wTxBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wMaxFrameSize;
    uint16_t    PH_MEMLOC_REM wBytesToSend;
    uint8_t     PH_MEMLOC_REM bHeaderLength;

    /* Used for Exchange */
    uint16_t    PH_MEMLOC_REM wHalExchangeOption;
    uint16_t    PH_MEMLOC_REM wRxBufferSize;
    uint16_t    PH_MEMLOC_REM wRxOverlappedStartPos;
    uint8_t     PH_MEMLOC_REM aRxOverlapped[7];
    uint8_t     PH_MEMLOC_REM bBufferOverflow;
    uint8_t   * PH_MEMLOC_REM pRawResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRawResponseLength = 0;
    uint8_t   * PH_MEMLOC_REM pNormalizedResponse = NULL;
    uint16_t    PH_MEMLOC_REM wNormalizedResponseLength = 0;
    uint8_t   * PH_MEMLOC_REM pPayloadResponse = NULL;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;

    /* Option parameter check */
    if (0u != (wOption &  (uint16_t)~(uint16_t)
        (
        PH_EXCHANGE_BUFFERED_BIT | PH_EXCHANGE_LEAVE_BUFFER_BIT |
        PH_EXCHANGE_TXCHAINING | PH_EXCHANGE_RXCHAINING | PH_EXCHANGE_RXCHAINING_BUFSIZE
        )))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
    }

    /* Retrieve HAL buffer sizes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE, &wRxBufferSize));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE, &wTxBufferSize));

    /* Find the biggest possible frame size before chaining is needed. */
    wMaxFrameSize = (wTxBufferSize < aI18092_FsTable[pDataParams->bLrt]) ? wTxBufferSize : aI18092_FsTable[pDataParams->bLrt];

    /* ACK transmission in case of Rx-Chaining */
    if (((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING) ||
        ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING_BUFSIZE))
    {
        pDataParams->bStateNow = PHPAL_I18092MPI_SW_STATE_ACK_PDU_TX | PHPAL_I18092MPI_SW_STATE_CHAINING_BIT;
    }
    /* PDU transmission */
    else
    {
        /* Reset to default state if not in Tx-Mode */
        if ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) != PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX)
        {
            pDataParams->bStateNow = PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX;
        }

        /* Reset Retransmission requested state. */
        pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;
    }


    /* Reset receive length */
    *pRxLength = 0;

    /* Reset Header length */
    bHeaderLength = 0;

    /* Reset BufferOverflow flag */
    bBufferOverflow = 0;

    /* ******************************** */
    /*       PDU TRANSMISSION LOOP      */
    /* ******************************** */
    do
    {
        /* Set exchange option */
        wHalExchangeOption = (uint16_t)(wOption & (uint16_t)~(uint16_t)PH_EXCHANGE_MODE_MASK);

        /* Init. number of bytes to send */
        wBytesToSend = wTxLength;

        switch (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK)
        {
        case PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX:

            /* Forecast header length */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_GetPduHeaderLength(pDataParams, &bHeaderLength));

            /* Retrieve Number of preloaded bytes */
            if (0U != (wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                    &wTxBufferLen));

                /* Header is already included in TxBufferLen, so subtract it */
                wTxBufferLen = wTxBufferLen - (uint16_t)bHeaderLength;
            }
            else
            {
                /* Nothing has been buffered */
                wTxBufferLen = 0;
            }

            /* Check if we need to chain -> Data to be sent exceeds maximum frame size or HAL buffer */
            if (((uint16_t)bHeaderLength + wTxBufferLen + wBytesToSend) > wMaxFrameSize)
            {
                /* If this is the first frame */
                if ((0U == ((wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))))
                {
                    /* TxBuffer needs space for at least the header */
                    if ((bHeaderLength + wTxBufferLen + /* */ 1U) > wMaxFrameSize)
                    {
                        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_PAL_I18092MPI);
                    }

                    /* Limit bytes to send */
                    wBytesToSend = wMaxFrameSize - (uint16_t)(wTxBufferLen + bHeaderLength);
                }
                /* Else send the buffered frame and transmit this frame afterwards */
                else
                {
                    wBytesToSend = 0;
                }

                /* If there was no chaining indicate that this is the first chain */
                if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))))
                {
                    /* Indicate first chaining */
                    pDataParams->bStateNow |= (PHPAL_I18092MPI_SW_STATE_CHAINING_BIT | PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT);
                }

                /* Force sending. */
                wHalExchangeOption = PH_EXCHANGE_BUFFER_LAST;
            }

            /* Check if we are forced to chain */
            if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_TXCHAINING)
            {
                /* If there was no chaining indicate that this is the first chain */
                if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))))
                {
                    /* Indicate first chaining */
                    pDataParams->bStateNow |= (PHPAL_I18092MPI_SW_STATE_CHAINING_BIT | PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT);
                }

                /* Force sending. */
                wHalExchangeOption = PH_EXCHANGE_BUFFER_LAST;
            }
            /* We are not forced to chain */
            else
            {
                /* Nor do we need to chain */
                if ((wBytesToSend == wTxLength) && (0U == ((wOption & PH_EXCHANGE_BUFFERED_BIT))))
                {
                    /* But chaining has been performed -> this is the last chain */
                    if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))
                    {
                        pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_CHAINING_BIT;
                        pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_CHAINING_LAST_BIT;
                    }
                }
            }

            /* Write PDU header */
            if ((0U == ((wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                    pDataParams,
                    PHPAL_I18092MPI_SW_PFB_INFORMATION_PDU_MASK,
                    ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT) > 0U) ? PH_ON : PH_OFF,
                    (uint8_t)(wBytesToSend + wTxBufferLen)));

                /* Preserve buffered data from now on */
                wHalExchangeOption |= PH_EXCHANGE_LEAVE_BUFFER_BIT;
            }
            /* PDU Header has already been written */
            else
            {
                /* Exchange is imminent, update length byte in HAL buffer */
                if ((0U == ((wHalExchangeOption & PH_EXCHANGE_BUFFERED_BIT))))
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_UpdatePduHeader(pDataParams, (uint8_t)(wBytesToSend + wTxBufferLen)));
                }
            }
            break;

        case PHPAL_I18092MPI_SW_STATE_ACK_PDU_TX:

            /* Write PDU header to internal HAL buffer. Don't set the option bit (timeout in this case). */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                pDataParams,
                PHPAL_I18092MPI_SW_PFB_ACK_PDU_MASK,
                PH_OFF,
                0));

            /* No additional data */
            wBytesToSend = 0;

            /* Force sending. */
            wHalExchangeOption = PH_EXCHANGE_BUFFER_LAST;
            break;

        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18092MPI);
        }

        /* Transmit and receive data. */
        status = phpalI18092mPI_Sw_TransceivePdu(
            pDataParams,
            wHalExchangeOption,
            pTxBuffer,
            wBytesToSend,
            &pRawResponse,
            &wRawResponseLength,
            &pNormalizedResponse,
            &wNormalizedResponseLength,
            &pPayloadResponse,
            &wPayloadLength);

        /* Complete chaining if buffer is full */
        if (((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW) &&
            ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING_BUFSIZE))
        {
            /* Indicate Buffer Overflow */
            bBufferOverflow = 1;

            /* Valid information PDU received, increment PNI. NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.3. */
            pDataParams->bPni = PHPAL_I18092MPI_SW_INCREMENT_PNI(pDataParams->bPni);
        }
        /* Else bail out on error */
        else
        {
            PH_CHECK_SUCCESS(status);
        }

        if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT))
        {
            /* Retransmission requested. */
            pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;
        }
        else
        {
            /* Advance input data buffer. */
            pTxBuffer = pTxBuffer + wBytesToSend;

            /* below if condition is added to make QAC compliant  */
            if(wTxLength > 0U)
            {
                wTxLength = wTxLength - wBytesToSend;
            }

            /* Clean-up state */
            pDataParams->bStateNow &= (uint8_t)~(uint8_t)(PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT | PHPAL_I18092MPI_SW_STATE_CHAINING_LAST_BIT);

            /* TX is finished if no data is left to send */
            if (wTxLength == 0U)
            {
                /* Buffered / TxChaining mode -> finished after sending */
                if (((wOption & PH_EXCHANGE_BUFFERED_BIT) != 0U) ||
                    ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_TXCHAINING))
                {
                    return PH_ERR_SUCCESS;
                }

                /* Advance to RX state */
                pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_MASK;
                pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_RX;

                /* Chained frame received, set first-chain bit */
                if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))
                {
                    pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT;
                }
            }
            /* TX is not finished -> reset leave-buffer bit */
            else
            {
                wOption &= (uint16_t)~(uint16_t)PH_EXCHANGE_LEAVE_BUFFER_BIT;
            }
        }
    }
    /* Loop as long as the state does not transit to RX mode. */
    while ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) != PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_RX);

    /* Get the biggest possible frame size we'll receive. */
    wMaxFrameSize = aI18092_FsTable[pDataParams->bLri];

    /* Set RxBuffer pointer to start at first received PDU */
    *ppRxBuffer = pPayloadResponse;
    *pRxLength = wPayloadLength;

    /* Calculate overlapped byte count */
    bHeaderLength = (uint8_t)(wRawResponseLength - wPayloadLength);
    if (((wOption & PH_EXCHANGE_MODE_MASK) != PH_EXCHANGE_RXCHAINING) &&
        ((wOption & PH_EXCHANGE_MODE_MASK) != PH_EXCHANGE_RXCHAINING_BUFSIZE))
    {
        if ((0U !=(pDataParams->bNadEnabled)) && (bHeaderLength != 0U))
        {
            --bHeaderLength;
        }
    }

    /* Init RxStartPos */
    wRxOverlappedStartPos = 0;

    /* ******************************** */
    /*        PDU RECEPTION LOOP        */
    /* ******************************** */
    do
    {
        /* Only allow receive state at this point */
        if ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) != PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_RX)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18092MPI);
        }

        /* Retransmission requested. */
        if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT))
        {
            pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;
        }
        /* We're not done yet, chaining is in progress */
        else if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))
        {
            /* Skip overlapping / SUCCESS_CHAINING checks in case of BufferOverflow */
            if (0U == (bBufferOverflow))
            {
                /* Skip SUCCESS_CHAINING check for RXCHAINING_BUFSIZE mode */
                if ((wOption & PH_EXCHANGE_MODE_MASK) != PH_EXCHANGE_RXCHAINING_BUFSIZE)
                {
                    /* Return with chaining status if the next chain may not fit into our buffer */
                    if ((*pRxLength + wMaxFrameSize) > wRxBufferSize)
                    {
                        /* Reset RxBuffer Start Position */
                        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));

                        return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_PAL_I18092MPI);
                    }
                }

                /* Calculate start-position for overlapping */
                wRxOverlappedStartPos = wRawResponseLength - (uint16_t)bHeaderLength;

                if (bHeaderLength <= sizeof(aRxOverlapped))
                {
                    /* Backup overlapped bytes */
                    (void)memcpy(aRxOverlapped, &pRawResponse[wRxOverlappedStartPos], bHeaderLength);
                }
                else
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
                }

                /* Adjust RX buffer start position. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                    wRxOverlappedStartPos));
            }

            /* Chaining information PDU received, send ACK. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                pDataParams,
                PHPAL_I18092MPI_SW_PFB_ACK_PDU_MASK,
                PH_OFF,
                0));

            /* Perform actual exchange */
            status = phpalI18092mPI_Sw_TransceivePdu(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                &pRawResponse,
                &wRawResponseLength,
                &pNormalizedResponse,
                &wNormalizedResponseLength,
                &pPayloadResponse,
                &wPayloadLength);

            /* Complete chaining if buffer is full */
            if (((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW) &&
                ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING_BUFSIZE))
            {
                /* Reset wRxStartPos */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                    0));

                /* Indicate Buffer Overflow */
                bBufferOverflow = 1;

                /* Valid information PDU received, increment PNI. NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.3. */
                pDataParams->bPni = PHPAL_I18092MPI_SW_INCREMENT_PNI(pDataParams->bPni);
            }
            /* Next chunk of PDU has been received */
            else
            {
                /* Bail out on error */
                if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                {
                    /* Reset output length */
                    *pRxLength = 0;

                    /* Reset RxBuffer Start Position */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));

                    return status;
                }

                if (0U == (bBufferOverflow))
                {
                    /* Restore overlapped bytes */
                    (void)memcpy(&pRawResponse[wRxOverlappedStartPos], aRxOverlapped, bHeaderLength);  /* PRQA S 3200 */ /* PRQA S 3354 */

                    /* Increment length */
                    *pRxLength = *pRxLength + wPayloadLength;
                }
            }
        }
        /* No chaining -> reception finished */
        else
        {
            /* Reset RxBuffer Start Position */
            if (wRxOverlappedStartPos != 0U)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));
            }

            /* Return data */
            if (0U == (bBufferOverflow))
            {
                pDataParams->bStateNow = PHPAL_I18092MPI_SW_STATE_FINISHED;
            }
            /* do not return any data in case of Buffer Overflow */
            else
            {
                *pRxLength = 0;
                return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_PAL_I18092MPI);
            }
        }
    }
    while (pDataParams->bStateNow != PHPAL_I18092MPI_SW_STATE_FINISHED);

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_Transmit(
                                      phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t * pTxBuffer,
                                      uint16_t wTxLength
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* Used to build block frames */
    uint16_t    PH_MEMLOC_REM wTxBufferSize;
    uint16_t    PH_MEMLOC_REM wTxBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wMaxFrameSize;
    uint16_t    PH_MEMLOC_REM wBytesToSend;
    uint8_t     PH_MEMLOC_REM bHeaderLength;

    /* Used for Exchange */
    uint32_t    PH_MEMLOC_REM dwTimeout;
    uint8_t   * PH_MEMLOC_REM pRawResponse = NULL;
    uint8_t   * PH_MEMLOC_REM pNormalizedResponse = NULL;
    uint8_t   * PH_MEMLOC_REM pPayloadResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRawResponseLength = 0;
    uint16_t    PH_MEMLOC_REM wNormalizedResponseLength = 0;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint16_t    PH_MEMLOC_REM wHalExchangeOption;
    uint32_t    PH_MEMLOC_REM dwShift = 1U;

    /* Option parameter check */
    if (0u != (wOption &  (uint16_t)~(uint16_t)
        (
        PH_TRANSMIT_BUFFERED_BIT | PH_TRANSMIT_LEAVE_BUFFER_BIT | PH_TRANSMIT_TXCHAINING
        )))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
    }

    /* Retrieve HAL buffer sizes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE, &wTxBufferSize));

    /* Find the biggest possible frame size before chaining is needed. */
    wMaxFrameSize = (wTxBufferSize < aI18092_FsTable[pDataParams->bLrt]) ? wTxBufferSize : aI18092_FsTable[pDataParams->bLrt];

    /* PDU transmission : Reset to default state if not in Tx-Mode */
    if ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) != PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX)
    {
        pDataParams->bStateNow = PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX;
    }

    /* Reset Header length */
    bHeaderLength = 0;

    /* ******************************** */
    /*       PDU TRANSMISSION LOOP      */
    /* ******************************** */
    do
    {
        /* Set exchange option */
        wHalExchangeOption = (uint16_t)(wOption & (uint16_t)~(uint16_t)PH_TRANSMIT_MODE_MASK);

        /* Init. number of bytes to send */
        wBytesToSend = wTxLength;

        switch (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK)
        {
        case PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_TX:

            /* Forecast header length */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_GetPduHeaderLength(pDataParams, &bHeaderLength));

            /* Retrieve Number of preloaded bytes */
            if (0U != (wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                    &wTxBufferLen));

                /* Header is already included in TxBufferLen, so subtract it */
                wTxBufferLen = wTxBufferLen - (uint16_t)bHeaderLength;

            }
            else
            {
                /* Nothing has been buffered */
                wTxBufferLen = 0;
            }

            /* Check if we need to chain -> Data to be sent exceeds maximum frame size or HAL buffer */
            if (((uint16_t)bHeaderLength + wTxBufferLen + wBytesToSend) > wMaxFrameSize)
            {
                /* If this is the first frame */
                if ((0U == ((wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))))
                {
                    /* TxBuffer needs space for at least the header */
                    if ((bHeaderLength + wTxBufferLen + /* */ 1U) > wMaxFrameSize)
                    {
                        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_PAL_I18092MPI);
                    }
                }
                /* Limit bytes to send. Send Buffered Frame + New Data that can fit into One Frame. */
                wBytesToSend = wMaxFrameSize - (uint16_t)(wTxBufferLen + bHeaderLength);

                /* If there was no chaining indicate that this is the first chain */
                if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))))
                {
                    /* Indicate first chaining */
                    pDataParams->bStateNow |= (PHPAL_I18092MPI_SW_STATE_CHAINING_BIT | PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT);
                }

                /* Force sending. */
                wHalExchangeOption = PH_EXCHANGE_BUFFER_LAST;
            }

            /* Check if we are forced to chain */
            if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_TXCHAINING)
            {
                /* If there was no chaining indicate that this is the first chain */
                if ((0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))))
                {
                    /* Indicate first chaining */
                    pDataParams->bStateNow |= (PHPAL_I18092MPI_SW_STATE_CHAINING_BIT | PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT);
                }

                /* Force sending. */
                wHalExchangeOption = PH_EXCHANGE_BUFFER_LAST;
            }
            /* We are not forced to chain */
            else
            {
                /* Nor do we need to chain */
                if ((wBytesToSend == wTxLength) && (0U == ((wOption & PH_EXCHANGE_BUFFERED_BIT))))
                {
                    /* But chaining has been performed -> this is the last chain */
                    if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))
                    {
                        pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_CHAINING_BIT;
                        pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_CHAINING_LAST_BIT;
                    }
                }
            }

            /* Write PDU header */
            if ((0U == ((wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                    pDataParams,
                    PHPAL_I18092MPI_SW_PFB_INFORMATION_PDU_MASK,
                    ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT) != 0U) ? PH_ON : PH_OFF,
                    (uint8_t)(wBytesToSend + wTxBufferLen)));

                /* Preserve buffered data from now on */
                wHalExchangeOption |= PH_EXCHANGE_LEAVE_BUFFER_BIT;
            }
            /* PDU Header has already been written */
            else
            {
                /* Exchange is imminent, update length byte in HAL buffer */
                if ((0U == ((wHalExchangeOption & PH_EXCHANGE_BUFFERED_BIT))))
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_UpdatePduHeader(pDataParams, (uint8_t)(wBytesToSend + wTxBufferLen)));
                }
            }
            break;

        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18092MPI);
        }

        /* If no chaining and wOption doesn't suggest Tx Buffering, then just do a transmit */
        if (((0U == ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT)))) && ((0U == ((wOption & PH_TRANSMIT_BUFFERED_BIT)))) && (wBytesToSend == wTxLength))
        {
            /* Update Backup length with Last Bytes that will be sent. */
            pDataParams->wLastTxLen = wBytesToSend + wTxBufferLen + bHeaderLength;

            /* Calculate Response Waiting Time. */
            dwTimeout = (uint32_t)(PHPAL_I18092MPI_SW_RWT_MIN_US * ((uint32_t)(dwShift << pDataParams->bWt)));
            dwTimeout += PHPAL_I18092MPI_SW_DELTA_RWT_US;
            dwTimeout /= 1000U;
            dwTimeout += 1U;
            dwTimeout += PHPAL_I18092MPI_SW_DELTA_T_INITIATOR_MS;

            /* Set Response Waiting Time. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                (uint16_t)dwTimeout));

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Transmit(
                pDataParams->pHalDataParams,
                wHalExchangeOption,
                pTxBuffer,
                wBytesToSend
                ));

            /* Advance to RX state */
            pDataParams->bStateNow = PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_RX;
        }
        else
        {
            /* Send Transmit Chaining data. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_TransceivePdu(
                pDataParams,
                wHalExchangeOption,
                pTxBuffer,
                wBytesToSend,
                &pRawResponse,
                &wRawResponseLength,
                &pNormalizedResponse,
                &wNormalizedResponseLength,
                &pPayloadResponse,
                &wPayloadLength));

            if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT))
            {
                /* Retransmission requested. */
                pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;
            }
            else
            {
                /* Advance input data buffer. */
                pTxBuffer = pTxBuffer + wBytesToSend;

                /* below if condition is added to make QAC compliant  */
                if(wTxLength > 0U)
                {
                    wTxLength = wTxLength - wBytesToSend;
                }

                /* Clean-up state */
                pDataParams->bStateNow &= (uint8_t)~(uint8_t)(PHPAL_I18092MPI_SW_STATE_CHAINING_FIRST_BIT | PHPAL_I18092MPI_SW_STATE_CHAINING_LAST_BIT);

                /* TX is finished if no data is left to send */
                if (wTxLength == 0U)
                {
                    /* Buffered / TxChaining mode -> finished after sending */
                    if (((wOption & PH_EXCHANGE_BUFFERED_BIT) != 0U) ||
                        ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_TXCHAINING))
                    {
                        return PH_ERR_SUCCESS;
                    }

                    /* Advance to RX state */
                    pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_MASK;
                    pDataParams->bStateNow |= PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_RX;

                    /* State should not be set as Chaining, because Tx operation should only performed. */
                    if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))
                    {
                        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18092MPI);
                    }
                }
                /* TX is not finished -> reset leave-buffer bit */
                else
                {
                    wOption &= (uint16_t)~(uint16_t)PH_EXCHANGE_LEAVE_BUFFER_BIT;
                }
            }
        }
    }
    /* Loop as long as the state does not transit to RX mode. */
    while ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) != PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_RX);

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_Receive(
                                     phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pRxLength
                                     )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;

    /* Used to build block frames */
    uint16_t    PH_MEMLOC_REM wMaxFrameSize;
    uint16_t    PH_MEMLOC_REM wBytesToSend;
    uint8_t     PH_MEMLOC_REM bHeaderLength;

    /* Used for Exchange */
    uint16_t    PH_MEMLOC_REM wHalExchangeOption;
    uint8_t     PH_MEMLOC_REM aRxOverlapped[7];
    uint16_t    PH_MEMLOC_REM wRxOverlappedStartPos;
    uint16_t    PH_MEMLOC_REM wRxBufferSize;
    uint8_t     PH_MEMLOC_REM bBufferOverflow;
    uint8_t   * PH_MEMLOC_REM pRawResponse = NULL;
    uint16_t    PH_MEMLOC_REM wRawResponseLength = 0;
    uint8_t   * PH_MEMLOC_REM pNormalizedResponse = NULL;
    uint16_t    PH_MEMLOC_REM wNormalizedResponseLength = 0;
    uint8_t   * PH_MEMLOC_REM pPayloadResponse = NULL;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;

    /* Option parameter check */
    if ((uint16_t)0U != (wOption &  (uint16_t)~((uint16_t)PH_RECEIVE_CHAINING | (uint16_t)PH_RECEIVE_CHAINING_BUFSIZE)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
    }

    /* Only allow receive state at this point */
    if ((pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_MASK) != PHPAL_I18092MPI_SW_STATE_INFORMATION_PDU_RX)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_I18092MPI);
    }

    /* Retrieve HAL buffer sizes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE, &wRxBufferSize));

    /* Reset BufferOverflow flag */
    bBufferOverflow = 0;

    /* Get the biggest possible frame size we'll receive. */
    wMaxFrameSize = aI18092_FsTable[pDataParams->bLri];

    switch(wOption)
    {
    case PH_RECEIVE_DEFAULT:

        /* First Receive the Packet in-case Chaining is not indicated. */
        statusTmp = phhalHw_Receive(
            pDataParams->pHalDataParams,
            PH_RECEIVE_DEFAULT,
            &pRawResponse,
            &wRawResponseLength);

        /* Validate and update status. */
        status = phpalI18092mPI_Sw_ValidateReceivedPdu(
            pDataParams,
            statusTmp,
            &pRawResponse,
            &wRawResponseLength,
            &pNormalizedResponse,
            &wNormalizedResponseLength,
            &pPayloadResponse,
            &wPayloadLength);
        break;

    case PH_RECEIVE_CHAINING:
    case PH_RECEIVE_CHAINING_BUFSIZE:

        /* Update state based on the Option. */
        pDataParams->bStateNow = PHPAL_I18092MPI_SW_STATE_ACK_PDU_TX | PHPAL_I18092MPI_SW_STATE_CHAINING_BIT;

        /* Write PDU header to internal HAL buffer. Don't set the option bit (timeout in this case). */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
            pDataParams,
            PHPAL_I18092MPI_SW_PFB_ACK_PDU_MASK,
            PH_OFF,
            0));

        /* No additional data */
        wBytesToSend = 0;

        /* Force sending. */
        wHalExchangeOption = PH_EXCHANGE_BUFFER_LAST;

        /* Transmit and receive data. */
        status = phpalI18092mPI_Sw_TransceivePdu(
            pDataParams,
            wHalExchangeOption,
            NULL,
            wBytesToSend,
            &pRawResponse,
            &wRawResponseLength,
            &pNormalizedResponse,
            &wNormalizedResponseLength,
            &pPayloadResponse,
            &wPayloadLength);
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
    }

    /* Complete chaining if buffer is full */
    if (((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW) &&
        (wOption == PH_RECEIVE_CHAINING_BUFSIZE))
    {
        /* Indicate Buffer Overflow */
        bBufferOverflow = 1;

        /* Valid information PDU received, increment PNI. NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.3. */
        pDataParams->bPni = PHPAL_I18092MPI_SW_INCREMENT_PNI(pDataParams->bPni);
    }
    /* Else bail out on error */
    else
    {
        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            /* Reset output length */
            *pRxLength = 0;

            /* Reset RxBuffer Start Position */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));

            return status;
        }
    }

    /* Set RxBuffer pointer to start at first received PDU. */
    *ppRxBuffer = pPayloadResponse;
    *pRxLength = wPayloadLength;

    /* Calculate overlapped byte count */
    bHeaderLength = (uint8_t)(wRawResponseLength - wPayloadLength);
    if ((wOption != PH_RECEIVE_CHAINING) &&
        (wOption != PH_RECEIVE_CHAINING_BUFSIZE))
    {
        if (0U != (pDataParams->bNadEnabled))
        {
            --bHeaderLength;
        }
    }

    /* Init RxStartPos */
    wRxOverlappedStartPos = 0;

    /* ******************************** */
    /*        PDU RECEPTION LOOP        */
    /* ******************************** */
    do
    {
        /* Retransmission requested. */
        if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT))
        {
            pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I18092MPI_SW_STATE_RETRANSMIT_BIT;
        }

        /* We're not done yet, chaining is in progress */
        if (0U != (pDataParams->bStateNow & PHPAL_I18092MPI_SW_STATE_CHAINING_BIT))
        {
            /* Skip overlapping / SUCCESS_CHAINING checks in case of BufferOverflow */
            if (0U == (bBufferOverflow))
            {
                /* Skip SUCCESS_CHAINING check for RXCHAINING_BUFSIZE mode */
                if (wOption != PH_RECEIVE_CHAINING_BUFSIZE)
                {
                    /* Return with chaining status if the next chain may not fit into our buffer */
                    if ((*pRxLength + wMaxFrameSize) > wRxBufferSize)
                    {
                        /* Reset RxBuffer Start Position */
                        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));

                        return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_PAL_I18092MPI);
                    }
                }

                /* Calculate start-position for overlapping */
                wRxOverlappedStartPos = wRawResponseLength - (uint16_t)bHeaderLength;

                /* Backup overlapped bytes */
                (void)memcpy(aRxOverlapped, &pRawResponse[wRxOverlappedStartPos], bHeaderLength);

                /* Adjust RX buffer start position. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                    wRxOverlappedStartPos));
            }

            /* Chaining information PDU received, send ACK. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WritePduHeader(
                pDataParams,
                PHPAL_I18092MPI_SW_PFB_ACK_PDU_MASK,
                PH_OFF,
                0));

            /* Perform actual exchange */
            status = phpalI18092mPI_Sw_TransceivePdu(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                &pRawResponse,
                &wRawResponseLength,
                &pNormalizedResponse,
                &wNormalizedResponseLength,
                &pPayloadResponse,
                &wPayloadLength);

            /* Complete chaining if buffer is full */
            if (((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW) &&
                (wOption == PH_RECEIVE_CHAINING_BUFSIZE))
            {
                /* Reset wRxStartPos */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                    0));

                /* Indicate Buffer Overflow */
                bBufferOverflow = 1;

                /* Valid information PDU received, increment PNI. NFCForum-TS-DigitalProtocol-1.0 section 14.12.3.3. */
                pDataParams->bPni = PHPAL_I18092MPI_SW_INCREMENT_PNI(pDataParams->bPni);
            }
            /* Next chunk of PDU has been received */
            else
            {
                /* Bail out on error */
                if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                {
                    /* Reset output length */
                    *pRxLength = 0;

                    /* Reset RxBuffer Start Position */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));

                    return status;
                }

                if (0U == (bBufferOverflow))
                {
                    /* Restore overlapped bytes */
                    (void)memcpy(&pRawResponse[wRxOverlappedStartPos], aRxOverlapped, bHeaderLength);  /* PRQA S 3200 */ /* PRQA S 3354 */

                    /* Increment length */
                    *pRxLength = *pRxLength + wPayloadLength;
                }
            }
        }
        /* No chaining -> reception finished */
        else
        {
            /* Reset RxBuffer Start Position */
            if (wRxOverlappedStartPos != 0U)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));
            }

            /* Return data */
            if (0U == (bBufferOverflow))
            {
                pDataParams->bStateNow = PHPAL_I18092MPI_SW_STATE_FINISHED;
            }
            /* do not return any data in case of Buffer Overflow */
            else
            {
                *pRxLength = 0;
                return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_PAL_I18092MPI);
            }
        }
    }
    while (pDataParams->bStateNow != PHPAL_I18092MPI_SW_STATE_FINISHED);

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_SetConfig(
                                       phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t wValue
                                       )
{
    switch (wConfig)
    {
    case PHPAL_I18092MPI_CONFIG_PACKETNO:
        if (wValue > PHPAL_I18092MPI_PNI_MAX)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);

        }
        pDataParams->bPni = (uint8_t)wValue;
        break;

    case PHPAL_I18092MPI_CONFIG_DID:
        if (wValue > PHPAL_I18092MPI_DID_MAX)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
        }
        pDataParams->bDid = (uint8_t)wValue;
        break;

    case PHPAL_I18092MPI_CONFIG_NAD:
        if (0U != (wValue & 0xFF00U))
        {
            pDataParams->bNadEnabled = 1;
            pDataParams->bNad = (uint8_t)wValue;
        }
        else
        {
            pDataParams->bNadEnabled = 0;
            pDataParams->bNad = 0;
        }
        break;

    case PHPAL_I18092MPI_CONFIG_WT:
        if (wValue > PHPAL_I18092MPI_WT_MAX)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
        }
        pDataParams->bWt = (uint8_t)wValue;
        break;

    case PHPAL_I18092MPI_CONFIG_MAXRETRYCOUNT:
        pDataParams->bMaxRetryCount = (uint8_t)wValue;
        break;

    case PHPAL_I18092MPI_CONFIG_ATN:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_I18092MPI);
        }
        pDataParams->bAtnDisabled = (uint8_t)(wValue ^ 0x0001U);
        break;


    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_I18092MPI);

    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_GetConfig(
                                       phpalI18092mPI_Sw_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t * pValue
                                       )
{
    switch (wConfig)
    {
    case PHPAL_I18092MPI_CONFIG_PACKETNO:
        *pValue = (uint16_t)pDataParams->bPni;
        break;

    case PHPAL_I18092MPI_CONFIG_DID:
        *pValue = (uint16_t)pDataParams->bDid;
        break;

    case PHPAL_I18092MPI_CONFIG_NAD:
        if (0U != (pDataParams->bNadEnabled))
        {
            *pValue = (uint16_t)(0x0100u | pDataParams->bNad);
        }
        else
        {
            *pValue = (uint16_t)pDataParams->bNad;
        }
        break;

    case PHPAL_I18092MPI_CONFIG_WT:
        *pValue = (uint16_t)pDataParams->bWt;
        break;

    case PHPAL_I18092MPI_CONFIG_FSL:
        *pValue  = (uint16_t)pDataParams->bFsl;
        break;

    case PHPAL_I18092MPI_CONFIG_MAXRETRYCOUNT:
        *pValue = (uint16_t)pDataParams->bMaxRetryCount;
        break;

    case PHPAL_I18092MPI_CONFIG_ATN:
        *pValue = (uint16_t)(((uint16_t)(pDataParams->bAtnDisabled)) ^ 0x01U);
        break;

    case PHPAL_I18092MPI_CONFIG_ACTIVEMODE:
        *pValue = (uint16_t)pDataParams->bActiveMode;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_I18092MPI);

    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI18092mPI_Sw_GetSerialNo(
    phpalI18092mPI_Sw_DataParams_t * pDataParams,
    uint8_t * pNfcId3Out
    )
{
    if (pDataParams->bNfcIdValid != PH_OFF)
    {
        (void)memcpy(pNfcId3Out, pDataParams->aNfcid3i, PHPAL_I18092MPI_NFCID3_LENGTH);
        return PH_ERR_SUCCESS;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_I18092MPI);
    }
}

phStatus_t phpalI18092mPI_Sw_Wakeup(phpalI18092mPI_Sw_DataParams_t * pDataParams)
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wResponseLength = 0;
    uint8_t     PH_MEMLOC_REM aCmd[11];
    uint8_t     PH_MEMLOC_REM bCmdLen;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint8_t *   PH_MEMLOC_REM pFrame = NULL;
    uint16_t    PH_MEMLOC_REM wFrameLength = 0;
    uint16_t    PH_MEMLOC_REM wPayloadLength = 0;
    uint8_t *   PH_MEMLOC_REM pPayload = NULL;
    uint8_t     PH_MEMLOC_COUNT bRetryCount = 0;
    uint8_t     PH_MEMLOC_REM bResponseReceived;
    uint8_t     PH_MEMLOC_REM bRetry;

    /* Build up frame data. */
    (void)memcpy(aCmd, pDataParams->aNfcid3i, PHPAL_I18092MPI_NFCID3_LENGTH);
    bCmdLen = PHPAL_I18092MPI_NFCID3_LENGTH;
    aCmd[bCmdLen++] = pDataParams->bDid;

    /* Reset response received flag */
    bResponseReceived = 0;

    /* Retry loop */
    do
    {
        /* Reset retry flag */
        bRetry = PH_OFF;

        /* Write frame header and update length field. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_WriteFrameHeader(pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            PHPAL_I18092MPI_SW_CMD_WUP_REQ,
            bCmdLen));

        /* Start transmission. */
        status = phhalHw_Exchange(pDataParams->pHalDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            aCmd,
            (uint16_t)bCmdLen,
            &pResp,
            &wResponseLength);

        /* Exchange was successful */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Signal that we've received something */
            bResponseReceived = 1;

            /* Check for valid response frame. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18092mPI_Sw_ValidateAndNormalizeResponseFrame(
                pDataParams,
                pResp,
                wResponseLength,
                PHPAL_I18092MPI_SW_CMD_WUP_RES,
                &pFrame,
                &wFrameLength,
                &pPayload,
                &wPayloadLength));

            /* Protocol error in case DID is not the same. NFCForum-TS-DigitalProtocol-1.0 section 14.6.3.2. */
            if ((wPayloadLength != 1U) || (pPayload[0] != pDataParams->bDid))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_I18092MPI);
            }
        }
        else
        {
            /* Check if we shall perform a(nother) retry. */
            if (bRetryCount < pDataParams->bMaxRetryCount)
            {
                ++bRetryCount;
                bRetry = PH_ON;
            }
            else
            {
                /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behaviour) */
                if ((pDataParams->bMaxRetryCount > 0U) && (0u != bResponseReceived))
                {
                    status = PH_ADD_COMPCODE_FIXED(PHPAL_I18092MPI_ERR_RECOVERY_FAILED, PH_COMP_PAL_I18092MPI);
                }
            }
        }
    }
    /* Retry loop */
    while (bRetry != PH_OFF);

    return status;
}

#endif /* NXPBUILD__PHPAL_I18092MPI_SW */
