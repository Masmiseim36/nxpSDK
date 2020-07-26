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
* Software ISO14443-3B Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI14443p3b.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#ifdef NXPBUILD__PHPAL_I14443P3B_SW

#include "phpalI14443p3b_Sw.h"
#include "phpalI14443p3b_Sw_Int.h"

phStatus_t phpalI14443p3b_Sw_Init(
                                  phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                  uint16_t wSizeOfDataParams,
                                  void * pHalDataParams
                                  )
{
    if (sizeof(phpalI14443p3b_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_ISO14443P3B);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_PAL_ISO14443P3B | PHPAL_I14443P3B_SW_ID;
    pDataParams->pHalDataParams         = pHalDataParams;
    pDataParams->bExtAtqb               = PH_OFF;
    pDataParams->bPupiValid             = PH_OFF;
    pDataParams->bCidSupported          = 0;
    pDataParams->bNadSupported          = 0;
    pDataParams->bCid                   = 0;
    pDataParams->bFwi                   = PHPAL_I14443P3B_SW_FWI_DEFAULT;
    pDataParams->bFsci                  = PHPAL_I14443P3B_SW_FSCI_DEFAULT;
    pDataParams->bFsdi                  = 0;
    pDataParams->bDri                   = 0;
    pDataParams->bDsi                   = 0;
    pDataParams->bAttribParam1          = PHPAL_I14443P3B_SW_ATTRIB_PARAM1_DEFAULT;
    pDataParams->pHigherLayerInf        = NULL;
    pDataParams->wTxWait                = PHPAL_I14443P3B_SW_TXWAIT_US_DEFAULT;
    pDataParams->wHigherLayerInfLen     = 0;
    pDataParams->pHigherLayerResp       = NULL;
    pDataParams->wHigherLayerRespSize   = 0;
    pDataParams->wHigherLayerRespLen    = 0;
    pDataParams->bOpeMode               = RD_LIB_MODE_NFC;
    pDataParams->bPollCmd               = PHPAL_I14443P3B_USE_REQB;
    pDataParams->bRetryCount            = PHPAL_I14443P3B_ATTRIB_RETRY_MAX;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_SetConfig(
                                       phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t wValue
                                       )
{
    switch (wConfig)
    {
    case PHPAL_I14443P3B_CONFIG_ATTRIB_PARAM1:
        pDataParams->bAttribParam1 = (uint8_t)wValue;
        break;

    /* Emvco: To Define Running Mode for RdLib: Either Normal, EMVCO, ISO */
    case PHPAL_I14443P3B_CONFIG_OPE_MODE:
      {
         pDataParams->bOpeMode = (uint8_t)wValue;
         break;
      }

    case PHPAL_I14443P3B_CONFIG_POLL_CMD:
    {
        pDataParams->bPollCmd = (uint8_t)wValue;
        break;
    }

    case PHPAL_I14443P3B_CONFIG_ATTRIB_RETRY_COUNT:
    {
        if(!(((wValue & 0xFFU) == PHPAL_I14443P3B_ATTRIB_RETRY_MIN) ||
             ((wValue & 0xFFU) == PHPAL_I14443P3B_ATTRIB_RETRY_MAX)))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
        }

        pDataParams->bRetryCount = (uint8_t)wValue;
        break;
    }
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_GetConfig(
                                       phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t* pValue
                                       )
{
    switch (wConfig)
    {
    case PHPAL_I14443P3B_CONFIG_ATTRIB_PARAM1:
        *pValue = pDataParams->bAttribParam1;
        break;

    case PHPAL_I14443P3B_CONFIG_ATTRIB_RETRY_COUNT:
        *pValue = pDataParams->bRetryCount;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    return PH_ERR_SUCCESS;
}


phStatus_t phpalI14443p3b_Sw_RequestB(
                                      phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                      uint8_t bNumSlots,
                                      uint8_t bAfi,
                                      uint8_t bExtAtqb,
                                      uint8_t * pAtqb,
                                      uint8_t * pAtqbLen
                                      )
{
    return phpalI14443p3b_Sw_RequestBEx(pDataParams, 0, bNumSlots, bAfi, bExtAtqb, pAtqb, pAtqbLen);
}

phStatus_t phpalI14443p3b_Sw_WakeUpB(
                                     phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                     uint8_t bNumSlots,
                                     uint8_t bAfi,
                                     uint8_t bExtAtqb,
                                     uint8_t * pAtqb,
                                     uint8_t * pAtqbLen
                                     )
{
    return phpalI14443p3b_Sw_RequestBEx(pDataParams, 1, bNumSlots, bAfi, bExtAtqb, pAtqb, pAtqbLen);
}

phStatus_t phpalI14443p3b_Sw_SlotMarker(
                                        phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                        uint8_t bSlotNumber,
                                        uint8_t * pAtqb,
                                        uint8_t * pAtqbLen
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* Parameter check */
    if ((bSlotNumber < 2U) || (bSlotNumber > 16U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    /* Note: No need to set time out and RxDeafTime since this is done in RequestB,
    which must be the previous command. */

    /* Build the command */
    aCmd[0] = (uint8_t)(((uint8_t)(bSlotNumber-1U) & 0x0FU) << 4U) | 0x05U;

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        &pResp,
        &wRespLength));

    return phpalI14443p3b_Sw_CheckATQBEx( pDataParams,
        pResp,
        wRespLength,
        pAtqb,
        pAtqbLen
        );
}

phStatus_t phpalI14443p3b_Sw_HaltB(
                                   phpalI14443p3b_Sw_DataParams_t * pDataParams
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[5];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I14443P3B_SW_ATTRIB_TIME_US + PHPAL_I14443P3B_SW_EXT_TIME_US));

    /* Build the command */
    aCmd[0] = PHPAL_I14443P3B_SW_HALT_CMD;
    (void)memcpy(&aCmd[1], pDataParams->pPupi, (uint32_t)(sizeof(pDataParams->pPupi)));

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        5,
        &pResp,
        &wRespLength));

    /* Response check */
    if ((wRespLength != 1U) || (pResp[0] != PHPAL_I14443P3B_SW_HALT_RESP))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3B);
    }

    /* PUPI is invalid */
    pDataParams->bPupiValid = PH_OFF;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_Attrib(
                                    phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                    uint8_t * pAtqb,
                                    uint8_t bAtqbLen,
                                    uint8_t bFsdi,
                                    uint8_t bCid,
                                    uint8_t bDri,
                                    uint8_t bDsi,
                                    uint8_t * pMbli
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[9];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;
    uint8_t     PH_MEMLOC_REM bTemp = 0;
    uint32_t    PH_MEMLOC_REM dwSfgt = 0;
    uint32_t    PH_MEMLOC_REM dwFwt = 0;
    float32_t   PH_MEMLOC_REM fFwt = 0;

    /* parameter check */
    /* ATQB length */
    if ((bAtqbLen != 12U) && (bAtqbLen != 13U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    /* check given DSI, DRI, FSDI */
    if ((bFsdi > PHPAL_I14443P3B_FRAMESIZE_MAX) || (bDri > 0x03U) || (bDsi > 0x03U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    if ((bFsdi < PHPAL_I14443P3B_EMVCO_FRAMESIZE_MIN) && (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    /* build the command frame */
    aCmd[0] = PHPAL_I14443P3B_SW_ATTRIB_CMD;

    /* copy PUPI to command frame */
    (void)memcpy(&aCmd[1], &pAtqb[1], 4);

    /* Param1: TR0, TR1, SOF/EOF suppression */
    aCmd[5] = pDataParams->bAttribParam1;

    /* Param2: Dsi, Dri, FSDI */
    aCmd[6] = (bDsi << 6U) | (bDri << 4U) | bFsdi;

    /* Emvco: case_id TB107_X
     * PCD shall disregards the value of bits b4-b2 of Protocol_Type
     */
    if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
    {
        aCmd[7] = pAtqb[10] & 0x01U;
    }
    else
    {
        /* Param3: confirmation of protocol info byte 2 (protocol type and minimum TR2) */
        aCmd[7] = pAtqb[10] & 0x07U;
    }

    /* Param4: Cid */
    /* check if supported by PICC, check given value for < 0x0F */
    if (0U != (pAtqb[11] & 0x01U))
    {
        if (bCid > 0x0EU)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
        }
    }
    else
    {
        bCid = 0x00;
    }
    aCmd[8] = bCid;

    /* Calculate FWT timeout */
    /* get the Fwt, because we need it for the timeout. */
    pDataParams->bFwi = (uint8_t)(pAtqb[11] >> 4U);
    if (pDataParams->bFwi == 0x0FU)
    {
        pDataParams->bFwi = PHPAL_I14443P3B_SW_FWI_DEFAULT;
    }

    /* Calculate FWT timeout */
    fFwt = (PHPAL_I14443P3B_SW_FWT_MIN_US * ((uint32_t)1U << pDataParams->bFwi));
    if(pDataParams->bOpeMode == RD_LIB_MODE_ISO)
    {
        /* Add extension time */
        fFwt += PHPAL_I14443P3B_SW_EXT_TIME_US;
    }
    else
    {
        /* As per Digital Spec V1.1 req 7.9.1.3, NFC Forum Device SHALL wait at least FWT + delta FWTB for a Response */
        fFwt += PHPAL_I14443P3B_SW_DELTA_FWT_US;
        if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO) {
            /* As per EMVCo, add delta Tpcd time along with delta FWT time */
            fFwt += PHPAL_I14443P3B_SW_DELTA_TPCD_US;
        }
    }
    dwFwt = (uint32_t)fFwt;

    /* Set Attrib timeout */
    /* ISO IEC 14443-3, 7.9.4.3 FWI:
    In case of extended ATQB ...
    ... FWT applies after the Answer to ATTRIB and
    ... the waiting time for the Answer to ATTRIB is fixed to ~4,8 ms */

    if (bAtqbLen == 13U)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            PHPAL_I14443P3B_SW_ATTRIB_TIME_US + PHPAL_I14443P3B_SW_EXT_TIME_US));
    }
    else
    {
        /* Set FWT timeout */
        if (dwFwt > 0xFFFFU)
        {
            /* +1 is added to the timeout in millisecond to compensate the
             * fractional microseconds lost in division by 1000 */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                (uint16_t)((dwFwt / 1000U) + 1U)));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
                (uint16_t)dwFwt));

        }
    }

    /* Exchange command */
    wRespLength = 0;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        9,
        &pResp,
        &wRespLength));

    /* the remaining part of the ATTRIB command frame holds the Higher Layer INF */
    statusTmp = phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pDataParams->pHigherLayerInf,
        pDataParams->wHigherLayerInfLen,
        &pResp,
        &wRespLength);

    bTemp = pDataParams->bRetryCount;

    /* check and perform re-transmission of ATTRIB in case of Transmission or Timeout error
     * Nfc Digital Specification ver 1.1, Req 14.7.1.1
     */
    while((bTemp > 0U) && (PH_ERR_SUCCESS != (statusTmp & PH_ERR_MASK)))
    {
        /* Return error other than Timeout or Noise Error */
        if (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
        {
            if((((statusTmp & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) || ((statusTmp & PH_ERR_MASK) == PH_ERR_NOISE_ERROR)))
            {
                /* Wait for at least Tmin retransmission delay. */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_TIME_MICROSECONDS,
                    PH_NXPNFCRDLIB_CONFIG_EMVCO_DEFAULT_RETRANSMISSION));
            }
            else
            {
                return statusTmp;
            }
        }

         /* Emvco:  case_id TB305_X  TB312_X and Req. 9.6.1.3 */
        wRespLength = 0;

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            9,
            &pResp,
            &wRespLength));

        /* the remaining part of the ATTRIB command frame holds the Higher Layer INF */
        statusTmp = phhalHw_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pDataParams->pHigherLayerInf,
            pDataParams->wHigherLayerInfLen,
            &pResp,
            &wRespLength);

        bTemp--;
    }

    PH_CHECK_SUCCESS(statusTmp);

    /* we hopefully received at least one byte */
    if (wRespLength < 1U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3B);
    }

    /* we receive MBLI and CID in the first byte: */
    /* check the received cid: */
    if (((pAtqb[11] & 0x01U) != 0U) && ((pResp[0] & 0x0FU) != bCid))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3B);
    }

    /* If the CID is not supported, PICC SHALL set the CID to 0 */
    /* PCD SHALL treat a CID field with a different value as Protocol Error */
    /* Digital Protocol 1.1, Requirements 166 and EMVCo 2.6, Requirements 6.24, 6.4.1.10 */
    if ((pDataParams->bOpeMode != RD_LIB_MODE_ISO) && (((pAtqb[11] & 0x01U) == 0U) && ((pResp[0] & 0x0FU) != 0x0U)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3B);
    }

    /* Note: ISO14443-3 7.11: The MBLI (maximum buffer size) gives information about
    the PICCs internal buffer */
    *pMbli = (uint8_t)(pResp[0] >> 4U);

    if (1U < wRespLength)
    {
        /* we received some Higher Layer INF bytes:
        ISO14443-3 TypeB, 7.11 Higher Response (optional 0 or "more" bytes) --> see page 46. */
        pDataParams->wHigherLayerRespLen = wRespLength - 1u;

        /* should we have received it? */
        if ( pDataParams->wHigherLayerInfLen == 0U )
        {
            pDataParams->wHigherLayerRespLen = 0;
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3B);
        }

        if (pDataParams->wHigherLayerRespLen > pDataParams->wHigherLayerRespSize)
        {
            pDataParams->wHigherLayerRespLen = 0;
            return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_PAL_ISO14443P3B);
        }
        (void)memcpy( pDataParams->pHigherLayerResp, &pResp[1], pDataParams->wHigherLayerRespLen );
    }

    /* Now, that the PICC accepted our protocol settings, we can store them in the structure */
    if (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
    {
        pDataParams->bCidSupported = 0x00;
        pDataParams->bNadSupported = 0x00;
    }
    else
    {
        pDataParams->bCidSupported = pAtqb[11] & 0x01U;
        pDataParams->bNadSupported = (pAtqb[11] & 0x02U) >> 1U;
    }
    pDataParams->bCid = pResp[0] & 0x0FU;
    pDataParams->bFsci = (uint8_t)(pAtqb[10] >> 4U);
    pDataParams->bFsdi = bFsdi;
    pDataParams->bDri = bDri;
    pDataParams->bDsi = bDsi;

    bTemp = 0;
    /* If we had an extended ATQB, the timeout was set for the reception
    of the ATTRIB command (see line 280). */
    if (bAtqbLen == 13U)
    {
        /* Set FWT timeout */
        if (dwFwt > 0xFFFFU)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                (uint16_t)(dwFwt / 1000U)));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
                (uint16_t)dwFwt));
        }
    }

    /* Set baud rate on Pcd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3b_Sw_SetReaderBaudRateEx(pDataParams));

    /* Applying Tx Wait saved according to the TR value of ATQb as setreaderbaudrateEx restores default */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXWAIT_US,
        pDataParams->wTxWait));

    /* If we have had received an extended ATQB with an SFGT (startup frame guard time)
    we need to wait this time here. */
    if (bAtqbLen == 13U)
    {
        /* fetch sfgi value from response */
        bTemp = (uint8_t)(pAtqb[12] >> 4U);

        if (bTemp == 0x0FU)
        {
            bTemp = PHPAL_I14443P3B_SW_SFGI_DEFAULT;
        }
        /* Calculate SFGT in Microseconds */
        dwSfgt = (PHPAL_I14443P3B_SW_FWT_MIN_US * ((uint32_t)1U << bTemp)) + PHPAL_I14443P3B_SW_EXT_TIME_US;

        /* Perform SFGT Wait */
        if (dwSfgt > 0xFFFFU)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(pDataParams->pHalDataParams, PHHAL_HW_TIME_MILLISECONDS, (uint16_t)(dwSfgt / 1000U)));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(pDataParams->pHalDataParams, PHHAL_HW_TIME_MICROSECONDS, (uint16_t)dwSfgt));
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_ActivateCard(
    phpalI14443p3b_Sw_DataParams_t * pDataParams,
    uint8_t * pPupi,
    uint8_t  bPupiLength,
    uint8_t bNumSlots,
    uint8_t bAfi,
    uint8_t bExtAtqb,
    uint8_t bFsdi,
    uint8_t bCid,
    uint8_t bDri,
    uint8_t bDsi,
    uint8_t * pAtqb,
    uint8_t * pAtqbLen,
    uint8_t * pMbli,
    uint8_t * pMoreCardsAvailable
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM statusFirstError;
    uint8_t     PH_MEMLOC_REM bSlot;
    uint8_t     PH_MEMLOC_REM bPICCsFound;
    uint8_t     PH_MEMLOC_REM pLocalAtqb[13];
    uint8_t     PH_MEMLOC_REM bLocalAtqbLen;
    uint8_t     PH_MEMLOC_REM *pLocalPupi = NULL;
    uint8_t     PH_MEMLOC_REM bBreakfromloop;
    uint8_t     PH_MEMLOC_COUNT bCount;

    bCount = 0;
    bPICCsFound = 0;
    bLocalAtqbLen = 0x00;
    pLocalPupi = &pLocalAtqb[1];
    *pMoreCardsAvailable = 0x00;
    bBreakfromloop = 0;

    /* initialise the statusFirstError with timeout, which is the trigger */
    statusFirstError = PH_ERR_IO_TIMEOUT;

    /* Parameter check */
    /* number of slots and length of PUPI */
    if ((bNumSlots > 4U) || ((bPupiLength > 0U) && (bPupiLength != 4U) && (pDataParams->bPollCmd != PHPAL_I14443P3B_USE_WUPB) ))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    /* check given DSI, DRI, FSDI */
    if ((bFsdi > PHPAL_I14443P3B_FRAMESIZE_MAX)
        || ((bDri & ~PHPAL_I14443P3B_DATARATE_FORCE) > 3U)
        || ((bDsi & ~PHPAL_I14443P3B_DATARATE_FORCE) > 3U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    /* Cid */
    if (bCid > 14U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
    {
        status = phpalI14443p3b_WakeUpB(pDataParams, bNumSlots, bAfi, bExtAtqb, pLocalAtqb, &bLocalAtqbLen);

        /* As per EMVCo 2.6 req 9.6.1.3, wait for at least Tmin retransmission in case of timeout error. */
        while (((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) && (bCount < PH_NXPNFCRDLIB_CONFIG_EMVCO_RETRYCOUNT))
        {
            bCount++;
            /* Wait for at least Tmin retransmission delay. */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Wait(
                pDataParams->pHalDataParams,
                PHHAL_HW_TIME_MICROSECONDS,
                PH_NXPNFCRDLIB_CONFIG_EMVCO_DEFAULT_RETRANSMISSION));

            status = phpalI14443p3b_WakeUpB(pDataParams, bNumSlots, bAfi, bExtAtqb, pLocalAtqb, &bLocalAtqbLen);
        }
        PH_CHECK_SUCCESS(status);
    }
    else
    {
        if (bPupiLength != 0U)
        {
            status = phpalI14443p3b_WakeUpB(pDataParams, bNumSlots, bAfi, bExtAtqb, pLocalAtqb, &bLocalAtqbLen);
        }
        else
        {
            status = phpalI14443p3b_RequestB(pDataParams, bNumSlots, bAfi, bExtAtqb, pLocalAtqb, &bLocalAtqbLen);
        }
    }

    /* SlotMarkers */
    /* If the bNumSlots == 0U, we do not send any SlotMarker commands (the following for loop). */
    if (bNumSlots != 0U)
    {
        /* calculate the number of slots reusing bNumSlots */
        bNumSlots = (uint8_t)(1U << bNumSlots);
    }

    bSlot = 2U;
    while(TRUE)
    {
        /* Integrity Error means collision */
        if ((status & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR)
        {
            *pMoreCardsAvailable = 0x01;
        }

        /* Update first error */
        if ((statusFirstError & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
        {
            statusFirstError = status;
        }

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* if the caller provided a PUPI, we try to find and activate only the according PICC */
            if (bPupiLength != 0U)
            {
                /* compare PUPIs */
                if (memcmp(pPupi, pLocalPupi, 4) == 0)
                {
                    ++bPICCsFound;
                    bBreakfromloop = 1U;
                }
            }
            /* no PUPI provided by caller */
            else
            {
                ++bPICCsFound;
                bBreakfromloop = 1U;
            }
        }

        if (bSlot > bNumSlots)
        {
            bBreakfromloop = 1U;
        }

        if(bBreakfromloop == 1U)
        {
            break;
        }

        status = phpalI14443p3b_SlotMarker(pDataParams, bSlot, pLocalAtqb, &bLocalAtqbLen);
        bSlot++;
    }

    /* check if we found no PICC */
    if (0U == (bPICCsFound))
    {
        return statusFirstError;
    }

    if(bLocalAtqbLen != 0U)
    {
        (void)memcpy(pAtqb, pLocalAtqb, bLocalAtqbLen );
    }
    *pAtqbLen = bLocalAtqbLen;

    /* Find the baud rates which shall be used. pAtqb[9] holds the VICCs bit rate capabilities. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p3b_Sw_DecideBaudRateEx(pAtqb[9], &bDri, &bDsi));

    /* AttriB: activate the first PICC found */
    status = phpalI14443p3b_Attrib(pDataParams, pAtqb, *pAtqbLen, bFsdi, bCid, bDri, bDsi, pMbli);

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* PUPI is invalid */
        pDataParams->bPupiValid = PH_OFF;
    }

    return status;
}

phStatus_t phpalI14443p3b_Sw_Exchange(
                                      phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t * pTxBuffer,
                                      uint16_t wTxLength,
                                      uint8_t ** ppRxBuffer,
                                      uint16_t * pRxLength
                                      )
{
    /* direct mapping of hal exchange function */
    return phhalHw_Exchange(
        pDataParams->pHalDataParams,
        wOption,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength);
}

phStatus_t phpalI14443p3b_Sw_GetSerialNo(
    phpalI14443p3b_Sw_DataParams_t * pDataParams,
    uint8_t * pPupi
    )
{
    /* Return an error if PUPI is not valid */
    if (pDataParams->bPupiValid == PH_OFF)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_ISO14443P3B);
    }

    /* Copy PUPI */
    (void)memcpy(pPupi, pDataParams->pPupi, (uint32_t)(sizeof(pDataParams->pPupi)));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_SetHigherLayerInf(
    phpalI14443p3b_Sw_DataParams_t * pDataParams,
    uint8_t * pTxBuffer,
    uint16_t  wTxLength,
    uint8_t * pRxBuffer,
    uint16_t wRxBufSize
    )
{
    pDataParams->pHigherLayerInf = pTxBuffer;
    pDataParams->wHigherLayerInfLen = wTxLength;
    pDataParams->pHigherLayerResp = pRxBuffer;
    pDataParams->wHigherLayerRespSize = wRxBufSize;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_GetHigherLayerResp(
    phpalI14443p3b_Sw_DataParams_t * pDataParams,
    uint8_t ** pRxBuffer,
    uint16_t * pRxLength
    )
{
    *pRxBuffer = pDataParams->pHigherLayerResp;
    *pRxLength = pDataParams->wHigherLayerRespLen;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_GetProtocolParams(
    phpalI14443p3b_Sw_DataParams_t * pDataParams,
    uint8_t * pCidEnabled,
    uint8_t * pCid,
    uint8_t * pNadSupported,
    uint8_t * pFwi,
    uint8_t * pFsdi,
    uint8_t * pFsci
    )
{
    *pCidEnabled    = pDataParams->bCidSupported;
    *pCid           = pDataParams->bCid;
    *pNadSupported  = pDataParams->bNadSupported;
    *pFwi           = pDataParams->bFwi;
    *pFsdi          = pDataParams->bFsdi;
    *pFsci          = pDataParams->bFsci;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_CheckATQBEx(
    phpalI14443p3b_Sw_DataParams_t * pDataParams,
    uint8_t * pResp,
    uint16_t wRespLength,
    uint8_t * pAtqb,
    uint8_t * pAtqbLen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bTR2;

    /* Response check */
    /* Digital Protocol Requirement 49, Section 5.6.1.5 */
    if ((wRespLength != 12U) && (wRespLength != 13U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3B);
    }

    /* The ATQB should look like the following (see ISO14443-3 7.9.1): */
    /* [0x50][PUPI0][PUPI1][PUPI2][PUPI3][PInfo0][PInfo1][PInfo2]([PInfo3])[CRC][CRC] */

    if ( pResp[0] != 0x50U )    /* ATQB byte */
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3B);
    }

    /* Copy received AtqB */
    (void)memcpy(pAtqb, pResp, wRespLength);
    *pAtqbLen = (uint8_t)wRespLength;

    if (pDataParams->bOpeMode != RD_LIB_MODE_NFC)
    {
        /* Max allowed frame size integer for ISO and EMVCo(v3.0) mode is less than or equals to 0x0C (4096 bytes) */
        if ((uint8_t)(pAtqb[10] & 0xF0U) > 0xC0U)
        {
            pAtqb[10] = (pAtqb[10] & 0x0FU) | 0xC0U;
        }
    }
    else
    {
        /* Max allowed frame size integer for NFC(DP 1.1, Requirements 64) mode is less than or equals to 0x08 (256 bytes) */
        if ((uint8_t)(pAtqb[10] & 0xF0U) > 0x80U)
        {
            pAtqb[10] = (pAtqb[10] & 0x0FU) | 0x80U;
        }
    }

/* Emvco: case_id 304_14
 * Conflict with TC304_14(added in 2.3.1a) and TC107_04
 * As per 6.3.2.8a in EMV Contactless Communication Protocol Specification Book D
 * Reset if b4 of Protocol Type set to (1)b
*/
    if (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
    {
        if((uint8_t)(pAtqb[10]& 0x08U) ==0x08U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3B);
        }
    }

    if (pDataParams->bOpeMode == RD_LIB_MODE_ISO)
    {
        /* Check TR2 value received in Atqb and set relevant TxWait as per ISO-IEC 14443-3. */
        bTR2 = (uint8_t)((pAtqb[10] & PHPAL_I14443P3B_SW_MINIMUM_TR2_MASK) >> PHPAL_I14443P3B_SW_MINIMUM_TR2_POS);
        switch(bTR2)
        {
        case PHPAL_I14443P3B_SW_MINIMUM_TR2_VALUE_0:
            pDataParams->wTxWait = (uint16_t)PHPAL_I14443P3B_SW_TXWAIT_US_TR2_0;
            break;
        case PHPAL_I14443P3B_SW_MINIMUM_TR2_VALUE_1:
            pDataParams->wTxWait = (uint16_t)PHPAL_I14443P3B_SW_TXWAIT_US_TR2_1;
            break;
        case PHPAL_I14443P3B_SW_MINIMUM_TR2_VALUE_2:
            pDataParams->wTxWait = (uint16_t)PHPAL_I14443P3B_SW_TXWAIT_US_TR2_2;
            break;
        default:
            pDataParams->wTxWait = (uint16_t)PHPAL_I14443P3B_SW_TXWAIT_US_TR2_3;
            break;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TXWAIT_US,
            pDataParams->wTxWait));
    }
    else
    {
        /* If the TR2 value is (11)b then the default TxWait of 500us (6780/fc) has to changed to 699us (9472/fc). */
        if ((uint8_t)((pAtqb[10] & PHPAL_I14443P3B_SW_MINIMUM_TR2_MASK) >> PHPAL_I14443P3B_SW_MINIMUM_TR2_POS)
            == PHPAL_I14443P3B_SW_MINIMUM_TR2_VALUE_3)
        {
            pDataParams->wTxWait = (uint16_t)PHPAL_I14443P3B_SW_TXWAIT_US_TR2_3;
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TXWAIT_US,
                PHPAL_I14443P3B_SW_TXWAIT_US_TR2_3));
        }
        else
        {
            pDataParams->wTxWait = PHPAL_I14443P3B_SW_TXWAIT_US_DEFAULT;
        }
    }

    /* and also copy the PUPI into the protocol params structure to be available
    for a possible following HaltB command */
    (void)memcpy( pDataParams->pPupi, &pAtqb[1], (uint32_t)(sizeof(pDataParams->pPupi)) );
    pDataParams->bPupiValid = PH_ON;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_RequestBEx(
                                        phpalI14443p3b_Sw_DataParams_t * pDataParams,
                                        uint8_t bIsWakeUp,
                                        uint8_t bNumSlots,
                                        uint8_t bAfi,
                                        uint8_t bExtAtqb,
                                        uint8_t * pAtqb,
                                        uint8_t * pAtqbLen
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[3];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* parameter check */
    if (bNumSlots > 4U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    /* Build the command */
    aCmd[0] = PHPAL_I14443P3B_SW_APF;
    aCmd[1] = bAfi;
    aCmd[2] = bNumSlots;    /* N = 2^bNumSlots*/

    /* Set wakeup bit if necessary */
    if (0U != (bIsWakeUp))
    {
        aCmd[2] |= PHPAL_I14443P3B_SW_PARAM_WUP_BIT;
    }

    /* Enable Extended ATQB if necessary */
    if (bExtAtqb != PH_OFF)
    {
        aCmd[2] |= PHPAL_I14443P3B_SW_PARAM_EXTATQB_BIT;
    }

    /* Store usage of Extended ATQB */
    pDataParams->bExtAtqb = bExtAtqb;

    /* PUPI is invalid */
    pDataParams->bPupiValid = PH_OFF;

    /* Reset default data rates */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXDATARATE_FRAMING,
        PHHAL_HW_RF_DATARATE_106));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
        PHHAL_HW_RF_DATARATE_106));

    /* Set RequestB timeout */
    if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            PHPAL_I14443P3B_SW_ATQB_TIME_US + (PHPAL_I14443P3B_SW_EXT_TIME_US * 2U)));
    }
    else if(pDataParams->bOpeMode == RD_LIB_MODE_ISO)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            PHPAL_I14443P3B_SW_ATQB_TIME_US + PHPAL_I14443P3B_SW_EXT_TIME_US));
    }
    else
    {
        /* As per Digital Protocol Ver1.1 Article 7.9.1.5
         * PCD should wait ~17ms (FWT(B,SENSB) + T(B,POLL)) * (566us + 16.4ms = ~17ms)
         */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                    PHPAL_I14443P3B_SW_DELTA_POLL));
    }

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        3,
        &pResp,
        &wRespLength));

    return phpalI14443p3b_Sw_CheckATQBEx( pDataParams,
        pResp,
        wRespLength,
        pAtqb,
        pAtqbLen
        );
}

phStatus_t phpalI14443p3b_Sw_SetReaderBaudRateEx( phpalI14443p3b_Sw_DataParams_t * pDataParams )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wTxDataRate;
    uint16_t    PH_MEMLOC_REM wRxDataRate;

    /* Check and convert Dri value */
    switch (pDataParams->bDri)
    {
    case PHPAL_I14443P3B_DATARATE_106:
        wTxDataRate = PHHAL_HW_RF_DATARATE_106;
        break;
    case PHPAL_I14443P3B_DATARATE_212:
        wTxDataRate = PHHAL_HW_RF_DATARATE_212;
        break;
    case PHPAL_I14443P3B_DATARATE_424:
        wTxDataRate = PHHAL_HW_RF_DATARATE_424;
        break;
    case PHPAL_I14443P3B_DATARATE_848:
        wTxDataRate = PHHAL_HW_RF_DATARATE_848;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    /* Check and convert Dsi value */
    switch (pDataParams->bDsi)
    {
    case PHPAL_I14443P3B_DATARATE_106:
        wRxDataRate = PHHAL_HW_RF_DATARATE_106;
        break;
    case PHPAL_I14443P3B_DATARATE_212:
        wRxDataRate = PHHAL_HW_RF_DATARATE_212;
        break;
    case PHPAL_I14443P3B_DATARATE_424:
        wRxDataRate = PHHAL_HW_RF_DATARATE_424;
        break;
    case PHPAL_I14443P3B_DATARATE_848:
        wRxDataRate = PHHAL_HW_RF_DATARATE_848;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3B);
    }

    /* Apply new data rates to Pcd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXDATARATE_FRAMING, wTxDataRate));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXDATARATE_FRAMING, wRxDataRate));

    return PH_ERR_SUCCESS;

}

phStatus_t phpalI14443p3b_Sw_DecideBaudRateEx( uint8_t bBitRateCapability, uint8_t* pbDri, uint8_t* pbDsi )
{
    uint8_t bMaxSymBitRate = 0;

    /* Use the given values for Dri and Dsi, if the caller wants so, or
    calculate Dsi and Dri depending on the given parameters and what the PICC is supporting,
    assuming that the reader supports all data rates in bouth directions. */

    /* Check if only symmetric bit rates are supported, check also if b4 is set.
    If the caller forces one of the data rates, it is not possible to make a decision */

    if ((0U == ((*pbDsi & PHPAL_I14443P3B_DATARATE_FORCE))) &&
        (0U == ((*pbDri & PHPAL_I14443P3B_DATARATE_FORCE))))
    {
        /* ISO/IEC14443-3 p41: If b4=(1)b should interpret the Bit_Rate_Capability byte as if 0x00 */
        if (0U != (bBitRateCapability & 0x08U))
        {
            *pbDsi = 0;
            *pbDri = 0;
            return PH_ERR_SUCCESS;
        }

        /* Symmetric baudrates */
        if ((0U != ((bBitRateCapability & 0x80U))) && (*pbDsi != *pbDri))
        {
            /* Start with the lower one of these both */
            if (*pbDsi > *pbDri)
            {
                bMaxSymBitRate = *pbDri;
            }
            else
            {
                bMaxSymBitRate = *pbDsi;
            }

            /* Start with the desired bit rate and break, as soon as a supported one is found. */
            switch(bMaxSymBitRate)
            {
            case PHPAL_I14443P3B_DATARATE_848:
                if ( 0x44U == (bBitRateCapability & 0x44U))
                {
                    break;
                }
            case PHPAL_I14443P3B_DATARATE_424:
                bMaxSymBitRate = PHPAL_I14443P3B_DATARATE_424;
                if ( 0x22U == (bBitRateCapability & 0x22U))
                {
                    break;
                }
            case PHPAL_I14443P3B_DATARATE_212:
                bMaxSymBitRate = PHPAL_I14443P3B_DATARATE_212;
                if ( 0x11U == (bBitRateCapability & 0x11U))
                {
                    break;
                }
            case PHPAL_I14443P3B_DATARATE_106:    /* is supported anyway */
                bMaxSymBitRate = PHPAL_I14443P3B_DATARATE_106;
                break;
            default:
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_ISO14443P3B);
            }

            *pbDri = bMaxSymBitRate;
            *pbDsi = *pbDri;
            return PH_ERR_SUCCESS;
        }
    }

    if (0U != ( *pbDsi & PHPAL_I14443P3B_DATARATE_FORCE))
    {
        *pbDsi = *pbDsi & 0x0FU;
    }
    else
    {
        /* Start with the desired bit rate and break, as soon as a supported one is found. */
        switch(*pbDsi)
        {
        case PHPAL_I14443P3B_DATARATE_848:
            if (0U != (bBitRateCapability & 0x40U))
            {
                break;
            }
        case PHPAL_I14443P3B_DATARATE_424:
            *pbDsi = PHPAL_I14443P3B_DATARATE_424;
            if (0U != (bBitRateCapability & 0x20U))
            {
                break;
            }
        case PHPAL_I14443P3B_DATARATE_212:
            *pbDsi = PHPAL_I14443P3B_DATARATE_212;
            if (0U != (bBitRateCapability & 0x10U))
            {
                break;
            }
        case PHPAL_I14443P3B_DATARATE_106:    /* is supported anyway */
            *pbDsi = PHPAL_I14443P3B_DATARATE_106;
            break;
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_ISO14443P3B);
        }
    }
    if (0U != ( *pbDri & PHPAL_I14443P3B_DATARATE_FORCE))
    {
        *pbDri = *pbDri & 0x0FU;
    }
    else
    {
        /* Start with the desired bit rate and break, as soon as a supported one is found. */
        switch(*pbDri)
        {
        case PHPAL_I14443P3B_DATARATE_848:
            if (0U != (bBitRateCapability & 0x04U))
            {
                break;
            }
        case PHPAL_I14443P3B_DATARATE_424:
            *pbDri = PHPAL_I14443P3B_DATARATE_424;
            if (0U != (bBitRateCapability & 0x02U))
            {
                break;
            }
        case PHPAL_I14443P3B_DATARATE_212:
            *pbDri = PHPAL_I14443P3B_DATARATE_212;
            if (0U != (bBitRateCapability & 0x01U))
            {
                break;
            }
        case PHPAL_I14443P3B_DATARATE_106:    /* is supported anyway */
            *pbDri = PHPAL_I14443P3B_DATARATE_106;
            break;
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_ISO14443P3B);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3b_Sw_SetSerialNo(phpalI14443p3b_Sw_DataParams_t * pDataParams, uint8_t *pPupi)
{
    (void)memcpy(pDataParams->pPupi, pPupi, (uint32_t)(sizeof(pDataParams->pPupi)));
    pDataParams->bPupiValid = PH_ON;

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHPAL_I14443P3B_SW */
