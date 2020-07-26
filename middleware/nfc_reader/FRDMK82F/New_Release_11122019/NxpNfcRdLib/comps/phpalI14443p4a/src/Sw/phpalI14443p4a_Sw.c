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
* Software ISO14443-4A Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 29. May 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI14443p4a.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#ifdef NXPBUILD__PHPAL_I14443P4A_SW

#include "phpalI14443p4a_Sw.h"
#include "phpalI14443p4a_Sw_Int.h"

#define I14443P4A_SW_IS_INVALID_BLOCK_STATUS(bStatus)                       \
    ((                                                                      \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) ||                     \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR) ||                \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_FRAMING_ERROR) ||                  \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR))                  \
    ? 1U : 0U                                                                 \
    )

phStatus_t phpalI14443p4a_Sw_Init(
                                  phpalI14443p4a_Sw_DataParams_t * pDataParams,
                                  uint16_t wSizeOfDataParams,
                                  void * pHalDataParams
                                  )
{
    if (sizeof(phpalI14443p4a_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_ISO14443P4A);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* init private data */
    pDataParams->wId            = PH_COMP_PAL_ISO14443P4A | PHPAL_I14443P4A_SW_ID;
    pDataParams->pHalDataParams = pHalDataParams;
    pDataParams->bCidSupported  = PH_OFF;
    pDataParams->bNadSupported  = PH_OFF;
    pDataParams->bCid           = 0x00;
    pDataParams->bBitRateCaps   = 0x00;
    pDataParams->bFwi           = PHPAL_I14443P4A_SW_FWI_DEFAULT;
    pDataParams->bFsci          = PHPAL_I14443P4A_SW_FSCI_DEFAULT;
    pDataParams->bFsdi          = 0x00;
    pDataParams->bDri           = 0x00;
    pDataParams->bDsi           = 0x00;
    pDataParams->bOpeMode       = RD_LIB_MODE_NFC;
    pDataParams->bRetryCount    = PHPAL_I14443P4A_RATS_RETRY_MAX;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4a_Sw_SetConfig(
                                       phpalI14443p4a_Sw_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t wValue
                                     )
{
    switch (wConfig)
        {
    /* Emvco: To Define Running Mode for RdLib: Either NFC, EMVCO or ISO */
        case PHPAL_I14443P4A_CONFIG_OPE_MODE:
          {
              pDataParams->bOpeMode = (uint8_t)wValue;
              break;
          }

        case PHPAL_I14443P4A_CONFIG_RATS_RETRY_COUNT:
        {
            if(!(((wValue & 0xFFU) == PHPAL_I14443P4A_RATS_RETRY_MIN) ||
                 ((wValue & 0xFFU) == PHPAL_I14443P4A_RATS_RETRY_MAX)))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
            }

            pDataParams->bRetryCount = (uint8_t)wValue;
            break;
        }

        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_ISO14443P4A);
        }

        return PH_ERR_SUCCESS;
}
phStatus_t phpalI14443p4a_Sw_Rats(
                                  phpalI14443p4a_Sw_DataParams_t * pDataParams,
                                  uint8_t bFsdi,
                                  uint8_t bCid,
                                  uint8_t * pAts
                                  )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM cmd[2];
    uint16_t    PH_MEMLOC_REM wCmdLength = 0;
    uint8_t     PH_MEMLOC_REM bAtsIndex;
    uint8_t     PH_MEMLOC_REM bSfgi;
    uint32_t    PH_MEMLOC_REM dwSfgt;
    uint32_t    PH_MEMLOC_REM dwFwt;
    float32_t   PH_MEMLOC_REM fDelay;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* Parameter check */
    if(pDataParams->bOpeMode != RD_LIB_MODE_NFC)
    {
        if ((bFsdi > PHPAL_I14443P4A_FRAMESIZE_MAX) || (bCid > 14U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
        }
        if ((bFsdi < PHPAL_I14443P4A_EMVCO_FRAMESIZE_MIN) && (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
        }
    }
    else
    {
        if ((bFsdi > PHPAL_I14443P4A_NFC_FRAMESIZE_MAX) || (bCid > 14U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
        }
    }

    /* Set Activation timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I14443P4A_SW_FWT_ACTIVATION_US + PHPAL_I14443P4A_SW_EXT_TIME_US));

    /* Send Rats command */
    cmd[0] = PHPAL_I14443P4A_SW_RATS;
    cmd[1] = (uint8_t)(((bFsdi << 4U) & 0xF0U) | (bCid & 0x0FU));

    status = phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        cmd,
        2,
        &pResp,
        &wRespLength);

    /* check and perform re-transmission of RATS in case of Transmission or Timeout error
     * Nfc Digital Specification ver 1.1, Req 13.7.1.1
     */
    bAtsIndex = pDataParams->bRetryCount;
    while((bAtsIndex > 0U) && (PH_ERR_SUCCESS != (status & PH_ERR_MASK)))
    {
        /* Return error other than Timeout or Noise Error */
        if (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
        {
            if((((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) || ((status & PH_ERR_MASK) == PH_ERR_NOISE_ERROR)))
            {
                /* Wait for at least Tmin retransmission delay. */
                PH_CHECK_SUCCESS_FCT(status, phhalHw_Wait(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_TIME_MICROSECONDS,
                    PH_NXPNFCRDLIB_CONFIG_EMVCO_DEFAULT_RETRANSMISSION));
            }
            else
            {
                return status;
            }
        }

        /* Emvco: case_id TA313, TA307_X, TA306_10-15, TA340_1 */
        status = phhalHw_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            cmd,
            2,
            &pResp,
            &wRespLength);

        bAtsIndex--;
    }

    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        /* Check for protocol error */
        if (((uint8_t)wRespLength != pResp[0]) || (wRespLength < 1U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
        }

        /* Copy ATS buffer */
        (void)memcpy(pAts, pResp, wRespLength);

        /* Set default values */
        if (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
        {
            pDataParams->bCidSupported = PH_OFF;
            pDataParams->bCid          = 0x00;
        }
        else
        {
            pDataParams->bCidSupported  = PH_ON;
            pDataParams->bCid           = bCid;
        }
        pDataParams->bNadSupported  = PH_OFF;
        pDataParams->bBitRateCaps   = 0x00;
        pDataParams->bFwi           = PHPAL_I14443P4A_SW_FWI_DEFAULT;
        pDataParams->bFsdi          = bFsdi;
        pDataParams->bFsci          = PHPAL_I14443P4A_SW_FSCI_DEFAULT;
        pDataParams->bDri           = 0x00;
        pDataParams->bDsi           = 0x00;
        bSfgi                       = PHPAL_I14443P4A_SW_SFGI_DEFAULT;

        /* Retrieve ATS information */
        /* Start parsing with T0 byte */
        bAtsIndex = PHPAL_I14443P4A_SW_ATS_T0;

        /* Parse T0/TA/TB/TC */
        if (wRespLength > 1U)
        {
            /* Parse T0 */
            pDataParams->bFsci = pAts[bAtsIndex] & 0x0FU;
            if(pDataParams->bOpeMode != RD_LIB_MODE_NFC)
            {
                if (pDataParams->bFsci > PHPAL_I14443P4A_FRAMESIZE_MAX)
                {
                    pDataParams->bFsci = PHPAL_I14443P4A_FRAMESIZE_MAX;
                }
            }
            else
            {   /* Nfc: Digital Specification Ver 1.1 Table 59 */
                if (pDataParams->bFsci > PHPAL_I14443P4A_NFC_FRAMESIZE_MAX)
                {
                    pDataParams->bFsci = PHPAL_I14443P4A_NFC_FRAMESIZE_MAX;
                }
            }

            bAtsIndex++;

            /* Parse TA(1) */
            if (0U != (pAts[PHPAL_I14443P4A_SW_ATS_T0] & PHPAL_I14443P4A_SW_ATS_TA1_PRESENT))
            {
                /* Check for protocol error */
                if (wRespLength <= bAtsIndex)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
                }

                /* Check for RFU bit b4 (b4 shall be 0; 1 is RFU) */
                if(0U != (pAts[bAtsIndex] & 0x08U))
                {
                    /* Only 106 kbps in both direction */
                    pDataParams->bBitRateCaps = 0x00;
                }
                else
                {
                    /* Store Bit rate capabilities */
                    pDataParams->bBitRateCaps = pAts[bAtsIndex];
                }

                bAtsIndex++;
            }

            /* Parse TB(1) */
            if (0U != (pAts[PHPAL_I14443P4A_SW_ATS_T0] & PHPAL_I14443P4A_SW_ATS_TB1_PRESENT))
            {
                /* Check for protocol error */
                if (wRespLength <= bAtsIndex)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
                }
                else
                {
                    /* Store Sfgi */
                    bSfgi = pAts[bAtsIndex] & 0x0FU;
                    if (bSfgi == 0x0FU)
                    {
                        bSfgi = PHPAL_I14443P4A_SW_SFGI_DEFAULT;
                    }

                    /* Store Fwi */
                    pDataParams->bFwi = (pAts[bAtsIndex] >> 4U) & 0x0FU;
                    if (pDataParams->bFwi == 0x0FU)
                    {
                        pDataParams->bFwi = PHPAL_I14443P4A_SW_FWI_DEFAULT;
                    }

                    bAtsIndex++;
                }
            }

            /* Parse TC(1) */
            if (0U != (pAts[PHPAL_I14443P4A_SW_ATS_T0] & PHPAL_I14443P4A_SW_ATS_TC1_PRESENT))
            {
                /* Check for protocol error */
                if (wRespLength <= bAtsIndex)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
                }
                else
                {
                    /* CID/NAD is not supported by EMVCo 2.5v specification. */
                    if (pDataParams->bOpeMode != RD_LIB_MODE_EMVCO)
                    {
                        /* NAD is supported only in ISO/IEC 14443-4 specification. */
                        if (pDataParams->bOpeMode == RD_LIB_MODE_ISO)
                        {
                            /* Check NAD Support */
                            if (0U != (pAts[bAtsIndex] & PHPAL_I14443P4A_SW_ATS_TC1_NAD_SUPPORT))
                            {
                                pDataParams->bNadSupported = PH_ON;
                            }
                        }

                        /* Check CID Support */
                        if ((pAts[bAtsIndex] & PHPAL_I14443P4A_SW_ATS_TC1_CID_SUPPORT) == 0U)
                        {
                            pDataParams->bCidSupported = PH_OFF;
                            pDataParams->bCid = 0x00;
                        }
                    }
                }
            }
        }

        /* Calculate SFGT in Microseconds */
        fDelay = PHPAL_I14443P4A_SW_FWT_MIN_US * ((uint32_t)1U << bSfgi);
        if(pDataParams->bOpeMode != RD_LIB_MODE_ISO)
        {
            fDelay += (PHPAL_I14443P4A_DELTA_SFGT_US) * ((uint32_t)1U << bSfgi);
        }
        dwSfgt = (uint32_t)fDelay;
        /* Perform SFGT Wait */
        if (dwSfgt > 0xFFFFU)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(
                pDataParams->pHalDataParams,
                PHHAL_HW_TIME_MILLISECONDS,
                (uint16_t)((dwSfgt / 1000U) + 1U)));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(
                pDataParams->pHalDataParams,
                PHHAL_HW_TIME_MICROSECONDS,
                (uint16_t)dwSfgt));
        }

        /* Calculate FWT timeout */
        fDelay = PHPAL_I14443P4A_SW_FWT_MIN_US;
        fDelay = (fDelay * ((uint32_t)1U << pDataParams->bFwi));

        if(pDataParams->bOpeMode == RD_LIB_MODE_ISO)
        {
            /* Add extension time */
            fDelay = fDelay + PHPAL_I14443P4A_SW_EXT_TIME_US;
        }
        else
        {
            /* As per Digital Spec V1.1 req 13.8.1.3, NFC Forum Device SHALL wait at least FWT + delta FWTT4AT for a Response */
            fDelay = fDelay + PHPAL_I14443P4A_SW_DELTA_FWT_US;

            if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO) {
                /* As per EMVCo, add delta Tpcd time along with delta FWT time */
                fDelay += PHPAL_I14443P4A_SW_DELTA_TPCD_US;
            }
        }
        dwFwt = (uint32_t)fDelay;

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
    /* Perform protocol error handling */
    else
    {
        /* Emvco: case_id TA306_XY and TA307 */
        if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
        {
            return status;
        }
        else
        {
            if (I14443P4A_SW_IS_INVALID_BLOCK_STATUS(status) == 0U)
            {
                return status;
            }

            /* Wait FWTmin after RATS */
            if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_TIME_MICROSECONDS,
                    PHPAL_I14443P4A_SW_FWT_MIN_US + PHPAL_I14443P4A_SW_EXT_TIME_US));
            }

            /* Send S(DESELECT) command */
            cmd[0] = PHPAL_I14443P4A_SW_S_BLOCK | PHPAL_I14443P4A_SW_S_BLOCK_RFU_BITS | PHPAL_I14443P4A_SW_PCB_DESELECT;
            wCmdLength = 1;

            statusTmp = phhalHw_Exchange(
                pDataParams->pHalDataParams,
                PH_EXCHANGE_DEFAULT,
                cmd,
                wCmdLength,
                &pResp,
                &wRespLength);

            /* S(DESELECT) failure */
            if (((statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS) || (wRespLength != 1U) || (pResp[0] != cmd[0]))
            {
                /* Set HLTA timeout */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
                    PHPAL_I14443P4A_SW_HALT_TIME_US));

                /* Send HLTA */
                cmd[0] = PHPAL_I14443P4A_SW_HALT_CMD;
                cmd[1] = 0x00;

                statusTmp = phhalHw_Exchange(
                    pDataParams->pHalDataParams,
                    PH_EXCHANGE_DEFAULT,
                    cmd,
                    2,
                    &pResp,
                    &wRespLength);
            }
        }
    }

    return PH_ADD_COMPCODE(status, PH_COMP_PAL_ISO14443P4A);
}

phStatus_t phpalI14443p4a_Sw_Pps(
                                 phpalI14443p4a_Sw_DataParams_t * pDataParams,
                                 uint8_t bDri,
                                 uint8_t bDsi
                                 )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM cmd[3];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;
    uint16_t    PH_MEMLOC_REM wTxDataRate;
    uint16_t    PH_MEMLOC_REM wRxDataRate;

    /* Check and convert Dri value */
    switch (bDri)
    {
    case PHPAL_I14443P4A_DATARATE_106:
        wTxDataRate = PHHAL_HW_RF_DATARATE_106;
        break;
    case PHPAL_I14443P4A_DATARATE_212:
        wTxDataRate = PHHAL_HW_RF_DATARATE_212;
        break;
    case PHPAL_I14443P4A_DATARATE_424:
        wTxDataRate = PHHAL_HW_RF_DATARATE_424;
        break;
    case PHPAL_I14443P4A_DATARATE_848:
        wTxDataRate = PHHAL_HW_RF_DATARATE_848;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Check and convert Dsi value */
    switch (bDsi)
    {
    case PHPAL_I14443P4A_DATARATE_106:
        wRxDataRate = PHHAL_HW_RF_DATARATE_106;
        break;
    case PHPAL_I14443P4A_DATARATE_212:
        wRxDataRate = PHHAL_HW_RF_DATARATE_212;
        break;
    case PHPAL_I14443P4A_DATARATE_424:
        wRxDataRate = PHHAL_HW_RF_DATARATE_424;
        break;
    case PHPAL_I14443P4A_DATARATE_848:
        wRxDataRate = PHHAL_HW_RF_DATARATE_848;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Send Pps command */
    cmd[0] = PHPAL_I14443P4A_SW_PPSS | pDataParams->bCid;
    cmd[1] = PHPAL_I14443P4A_SW_PPS0;
    cmd[2] = ((bDsi << 2U) & 0x0CU) | (bDri & 0x03U);
    status = phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        cmd,
        3,
        &pResp,
        &wRespLength);

    /* NAK means protocol error */
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
    }

    /* Success check */
    PH_CHECK_SUCCESS(status);

    /* Check length and start byte of PPS */
    if ((wRespLength != 1U) || (pResp[0] != (PHPAL_I14443P4A_SW_PPSS | pDataParams->bCid)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4A);
    }

    /* Store data rates */
    pDataParams->bDri = bDri;
    pDataParams->bDsi = bDsi;

    /* Apply new data rates to Pcd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXDATARATE_FRAMING, wTxDataRate));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXDATARATE_FRAMING, wRxDataRate));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4a_Sw_ActivateCard(
    phpalI14443p4a_Sw_DataParams_t * pDataParams,
    uint8_t bFsdi,
    uint8_t bCid,
    uint8_t bDri,
    uint8_t bDsi,
    uint8_t * pAts
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* Check Dri value */
    switch (bDri)
    {
    case PHPAL_I14443P4A_DATARATE_106:
        /* fall through */
    case PHPAL_I14443P4A_DATARATE_212:
        /* fall through */
    case PHPAL_I14443P4A_DATARATE_424:
        /* fall through */
    case PHPAL_I14443P4A_DATARATE_848:
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Check Dri value */
    switch (bDsi)
    {
    case PHPAL_I14443P4A_DATARATE_106:
        /* fall through */
    case PHPAL_I14443P4A_DATARATE_212:
        /* fall through */
    case PHPAL_I14443P4A_DATARATE_424:
        /* fall through */
    case PHPAL_I14443P4A_DATARATE_848:
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4A);
    }

    /* Perform RATS */
    statusTmp = phpalI14443p4a_Sw_Rats(pDataParams, bFsdi, bCid, pAts);

    if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        /* Perform PPS */
        if ((bDri != PHPAL_I14443P4A_DATARATE_106) || (bDsi != PHPAL_I14443P4A_DATARATE_106))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4a_Sw_Pps(pDataParams, bDri, bDsi));
        }
    }

    return PH_ADD_COMPCODE(statusTmp, PH_COMP_PAL_ISO14443P4A);
}

phStatus_t phpalI14443p4a_Sw_GetProtocolParams(
    phpalI14443p4a_Sw_DataParams_t * pDataParams,
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

#endif /* NXPBUILD__PHPAL_I14443P4A_SW */
