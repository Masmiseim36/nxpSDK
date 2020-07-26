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
* Software ISO14443-3A Component of Reader Library Framework.
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
#include <phpalI14443p3a.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#ifdef NXPBUILD__PHPAL_I14443P3A_SW

#include "phpalI14443p3a_Sw.h"
#include "phpalI14443p3a_Sw_Int.h"

phStatus_t phpalI14443p3a_Sw_Init(
                                  phpalI14443p3a_Sw_DataParams_t * pDataParams,
                                  uint16_t wSizeOfDataParams,
                                  void * pHalDataParams
                                  )
{
    if (sizeof(phpalI14443p3a_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_ISO14443P3A);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* init private data */
    pDataParams->wId            = PH_COMP_PAL_ISO14443P3A | PHPAL_I14443P3A_SW_ID;
    pDataParams->pHalDataParams = pHalDataParams;
    pDataParams->bUidLength     = 0;
    pDataParams->bUidComplete   = 0;
    pDataParams->bOpeMode       = RD_LIB_MODE_NFC;
    pDataParams->bPollCmd       = PHPAL_I14443P3A_USE_REQA;

    return PH_ERR_SUCCESS;
}

/* Emvco: Added for EMVCO This function is used to config parameter for phpalI14443p3a */
phStatus_t phpalI14443p3a_Sw_SetConfig(
                                       phpalI14443p3a_Sw_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t wValue
                                     )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    switch (wConfig)
    {
    /* Emvco: To Define Running Mode for RdLib: Either Nfc, EMVCO, ISO */
    case PHPAL_I14443P3A_CONFIG_OPE_MODE:
    {
        pDataParams->bOpeMode = (uint8_t)wValue;
        break;
    }

    case PHPAL_I14443P3A_CONFIG_POLL_CMD:
    {
        pDataParams->bPollCmd = (uint8_t)wValue;
        break;
    }

    case PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US:
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            wValue));
        break;
    }

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_ISO14443P3A);
    }

    return PH_ERR_SUCCESS;

}

/* Emvco: Added for EMVCO This function is used to config parameter for phpalI14443p3a */
phStatus_t phpalI14443p3a_Sw_GetConfig(
                                       phpalI14443p3a_Sw_DataParams_t * pDataParams,
                                       uint16_t wConfig,
                                       uint16_t *pValue
                                     )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    switch (wConfig)
    {
    /* Emvco: To Define Running Mode for RdLib: Either Nfc, EMVCO, ISO */
    case PHPAL_I14443P3A_CONFIG_OPE_MODE:
    {
        *pValue = (uint16_t)pDataParams->bOpeMode;
        break;
    }

    case PHPAL_I14443P3A_CONFIG_POLL_CMD:
    {
        *pValue = (uint16_t)pDataParams->bPollCmd;
        break;
    }

    case PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US:
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
                pValue));
        break;
    }

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_ISO14443P3A);
    }

    return PH_ERR_SUCCESS;

}

phStatus_t phpalI14443p3a_Sw_RequestA(
                                      phpalI14443p3a_Sw_DataParams_t * pDataParams,
                                      uint8_t * pAtqa
                                      )
{
    return phpalI14443p3a_Sw_RequestAEx(pDataParams, PHPAL_I14443P3A_REQUEST_CMD, pAtqa);
}

phStatus_t phpalI14443p3a_Sw_WakeUpA(
                                     phpalI14443p3a_Sw_DataParams_t * pDataParams,
                                     uint8_t * pAtqa
                                     )
{
    return phpalI14443p3a_Sw_RequestAEx(pDataParams, PHPAL_I14443P3A_WAKEUP_CMD, pAtqa);
}

phStatus_t phpalI14443p3a_Sw_HaltA(
                                   phpalI14443p3a_Sw_DataParams_t * pDataParams
                                   )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM cmd[2];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* Set halt timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I14443P3A_HALT_TIME_US + PHPAL_I14443P3A_EXT_TIME_US));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));

    /* Send HltA command */
    cmd[0] = PHPAL_I14443P3A_HALT_CMD;
    cmd[1] = 0x00;

    status = phhalHw_Exchange(pDataParams->pHalDataParams, PH_EXCHANGE_DEFAULT, cmd, 2, &pResp, &wRespLength);

    switch (status & PH_ERR_MASK)
    {
        /* HltA command should timeout -> success */
    case PH_ERR_IO_TIMEOUT:
        return PH_ERR_SUCCESS;
        /* Return protocol error */
    case PH_ERR_SUCCESS:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
        /* Return other errors */
    default:
        return status;
    }
}

phStatus_t phpalI14443p3a_Sw_Anticollision(
    phpalI14443p3a_Sw_DataParams_t * pDataParams,
    uint8_t bCascadeLevel,
    uint8_t * pUidIn,
    uint8_t bNvbUidIn,
    uint8_t * pUidOut,
    uint8_t * pNvbUidOut
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bIsSelect;
    uint8_t     PH_MEMLOC_REM bCmdBuffer[10];
    uint8_t *   PH_MEMLOC_REM pRcvBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wSndBytes;
    uint16_t    PH_MEMLOC_REM wRcvBytes = 0;
    uint16_t    PH_MEMLOC_REM wRcvBits = 0;
    uint8_t     PH_MEMLOC_REM bUidStartIndex;
    uint8_t     PH_MEMLOC_REM bBitCount;

    /* Check for invalid bNvbUidIn parameter */
    if ((bNvbUidIn > 0x40U) || ((bNvbUidIn & 0x0FU) > 0x07U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3A);
    }

    /* Check for invalid bCascadeIndex parameter */
    switch (bCascadeLevel)
    {
    case PHPAL_I14443P3A_CASCADE_LEVEL_1:
        /* Reset UID length */
        pDataParams->bUidLength = 0;
    case PHPAL_I14443P3A_CASCADE_LEVEL_2:
    case PHPAL_I14443P3A_CASCADE_LEVEL_3:
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3A);
    }

    /* Reset UID complete flag */
    pDataParams->bUidComplete = 0;

    /* ANTICOLLISION: Disable CRC */
    if (bNvbUidIn != 0x40U)
    {
        bIsSelect = 0;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));
    }
    /* SELECT: Enable CRC */
    else
    {
        bIsSelect = 1;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));
    }

    /* Init. command buffer */
    bCmdBuffer[0] = bCascadeLevel;
    (void)memset(&bCmdBuffer[2], 0x00, 5);

    /* Copy valid UID bits */
    wSndBytes = (uint16_t)(((((uint16_t)bNvbUidIn) & 0xF0U) >> 4U) + (((bNvbUidIn & 0x0FU) != 0U) ? 1U : 0U));
    (void)memcpy(&bCmdBuffer[2], pUidIn, (size_t)wSndBytes);
    wSndBytes += 2U;

    /* SELECT: Add BCC */
    if (0U != (bIsSelect))
    {
        bNvbUidIn = 0x50;
        bCmdBuffer[6] = pUidIn[0] ^ pUidIn[1] ^ pUidIn[2] ^ pUidIn[3];
        ++wSndBytes;
    }

    /* Encode NVB */
    bCmdBuffer[1] = bNvbUidIn + 0x20U;

    /* Adjust Rx-Align */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXALIGN, (((uint16_t)bNvbUidIn) & 0x07U)));

    /* Adjust TxBits */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXLASTBITS, (((uint16_t)bNvbUidIn) & 0x07U)));

    /* Send the ANTICOLLISION command */
    status = phhalHw_Exchange(pDataParams->pHalDataParams, PH_EXCHANGE_DEFAULT, bCmdBuffer, wSndBytes, &pRcvBuffer, &wRcvBytes);

    /* Reset RxAlignment */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXALIGN, 0));

    /* Check status, Collision is allowed for anti-collision command. */
    if ((bIsSelect == 0U) && ((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR))
    {
        /* Retrieve number of valid bits of last byte */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXLASTBITS, &wRcvBits));
    }
    else
    {
        /* Check for protocol error */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
        }
        /* Return on other errors */
        else
        {
            PH_CHECK_SUCCESS(status);
        }

        /* whole byte valid */
        wRcvBits = 0;
    }

    /* Add received data to UID */
    if (bIsSelect == 0U)
    {
        /* Retrieve byte-starting-index of received Uid */
        bUidStartIndex = (uint8_t)((bNvbUidIn & 0xF0U) >> 4U);

        /* Add new bitcount */
        bBitCount = (uint8_t)(((((uint16_t)bNvbUidIn) >> 4U) << 3U) + (wRcvBytes << 3U) + wRcvBits);

        /* Last incomplete byte is added to wRcvBytes, so remove that again */
        if (wRcvBits > 0U)
        {
            bBitCount -= 8U;
        }

        /* Convert bitcount to NVB format */
        *pNvbUidOut = (uint8_t)(((bBitCount >> 3U) << 4U) + (bBitCount & 0x07U));

        /* We do not tolerate more than (5u * 8 =)40 bits because it would lead to buffer overflows */
        if (*pNvbUidOut > 0x50U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
        }

        /* After successfull reception, the UID must be exact 40 bits */
        if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS) && (*pNvbUidOut != 0x50U))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
        }

        /* Copy received bytes to uid */
        if (wRcvBytes > 0U)
        {
            /* Incomplete byte sent: Merge Rx-Aligned first byte */
            if (0U != (bNvbUidIn & 0x07U))
            {
                bCmdBuffer[2U + bUidStartIndex] = (bCmdBuffer[2U + bUidStartIndex] & (uint8_t)((1U << (bNvbUidIn & 0x07U)) - 1U)) |
                                                 (pRcvBuffer[0] & (uint8_t)(0xFFU << (bNvbUidIn & 0x07U)));
            }
            /* Else just copy the first byte */
            else
            {
                bCmdBuffer[2U + bUidStartIndex] = pRcvBuffer[0];
            }

            /* Add the rest of the uid bytes */
            (void)memcpy(&bCmdBuffer[2U + bUidStartIndex + 1U], &pRcvBuffer[1], (size_t)(((uint32_t)wRcvBytes) - 1u));
        }

        /* Anticollision finished */
        if (*pNvbUidOut > 0x40U)
        {
            /* Collision in BCC byte can never happen */
            if (*pNvbUidOut < 0x50U)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_FRAMING_ERROR, PH_COMP_PAL_ISO14443P3A);
            }

            /* Remove BCC from NvbUidOut */
            *pNvbUidOut = 0x40;
            --wRcvBytes;

            /* BCC Check */
            if ((bCmdBuffer[2] ^ bCmdBuffer[3] ^ bCmdBuffer[4] ^ bCmdBuffer[5]) != bCmdBuffer[6])
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_FRAMING_ERROR, PH_COMP_PAL_ISO14443P3A);
            }
        }

        /* Copy UID */
        (void)memcpy(pUidOut, &bCmdBuffer[2], (size_t)(bUidStartIndex + ((uint32_t)wRcvBytes)));
    }
    /* Return SAK instead of the UID */
    else
    {
        /* only one byte allowed */
        if (wRcvBytes != 1U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
        }

        /* Cascade Bit is set */
        if (0U != (pRcvBuffer[0] & 0x04U))
        {
            /* If additional cascade levels are impossible -> protocol error */
            if (bCascadeLevel == PHPAL_I14443P3A_CASCADE_LEVEL_3)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
            }

            /* Cascade tag does not match -> protocol error */
            if (pUidIn[0] != PHPAL_I14443P3A_CASCADE_TAG)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
            }

            /* Ignore Cascade Tag */
            (void)memcpy(&pDataParams->abUid[pDataParams->bUidLength], &pUidIn[1], 3);

            /* Increment Uid length */
            pDataParams->bUidLength += 3U;
        }
        /* Cascade Bit is cleared -> no further cascade levels */
        else
        {
            /* Cascade tag does not match -> protocol error */
            if (pUidIn[0] == PHPAL_I14443P3A_CASCADE_TAG)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
            }

            /* Copy all uid bytes except BCC */
            (void)memcpy(&pDataParams->abUid[pDataParams->bUidLength], &pUidIn[0], 4);

            /* Increment Uid length */
            pDataParams->bUidLength += 4U;

            /* Set UID complete flag */
            pDataParams->bUidComplete = 1;

            /* set default card timeout */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                PHPAL_I14443P3A_TIMEOUT_DEFAULT_MS));
        }

        /* Copy SAK */
        pUidOut[0] = pRcvBuffer[0];
    }

    return PH_ADD_COMPCODE(status, PH_COMP_PAL_ISO14443P3A);
}

phStatus_t phpalI14443p3a_Sw_Select(
                                    phpalI14443p3a_Sw_DataParams_t * pDataParams,
                                    uint8_t bCascadeLevel,
                                    uint8_t * pUidIn,
                                    uint8_t * pSak
                                    )
{
    uint8_t PH_MEMLOC_REM bDummy;
    return phpalI14443p3a_Sw_Anticollision(pDataParams, bCascadeLevel, pUidIn, 0x40, pSak, &bDummy);
}

phStatus_t phpalI14443p3a_Sw_ActivateCard(
    phpalI14443p3a_Sw_DataParams_t * pDataParams,
    uint8_t * pUidIn,
    uint8_t bLenUidIn,
    uint8_t * pUidOut,
    uint8_t * pLenUidOut,
    uint8_t * pSak,
    uint8_t * pMoreCardsAvailable
    )
{
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCascadeLevel;
    uint8_t     PH_MEMLOC_COUNT bCascadeIndex;
    uint8_t     PH_MEMLOC_REM bUidIndex;
    uint8_t     PH_MEMLOC_REM bNvbUid;
    uint8_t     PH_MEMLOC_REM bAtqa[2] = {0};
    uint8_t     PH_MEMLOC_REM bUid[4];
    uint8_t     PH_MEMLOC_COUNT bRetryCount;
    uint8_t     PH_MEMLOC_REM bCollDetected = PH_OFF;

    bRetryCount = 0;

    /* Parameter check */
    if ((bLenUidIn != 0U) &&
        (bLenUidIn != 4U) &&
        (bLenUidIn != 7U) &&
        (bLenUidIn != 10U)&&
        (pDataParams->bPollCmd != PHPAL_I14443P3A_USE_WUPA))
    {
        /* Given UID length is invalid, return error */
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3A);
    }
    /* initialise to zero, for VS studio warning */
    bCascadeLevel = 0;
    /* Clear Uid */
    (void)memset(bUid, 0x00, 4);

    /* Clear Uid lengths */
    bUidIndex = 0;
    *pLenUidOut = 0;

    /* Clear the more cards available flag */
    *pMoreCardsAvailable = PH_OFF;

    if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
    {
        status = phpalI14443p3a_Sw_WakeUpA(pDataParams, bAtqa);
        /* As per EMVCo 2.6 req 9.6.1.3, wait for at least Tmin retransmission in case of timeout error. */
        while (((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) && (bRetryCount < PH_NXPNFCRDLIB_CONFIG_EMVCO_RETRYCOUNT))
        {
            bRetryCount++;
            /* Wait for at least Tmin retransmission delay. */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Wait(
                pDataParams->pHalDataParams,
                PHHAL_HW_TIME_MICROSECONDS,
                PH_NXPNFCRDLIB_CONFIG_EMVCO_DEFAULT_RETRANSMISSION));

            status = phpalI14443p3a_Sw_WakeUpA(pDataParams, bAtqa);
        }
    }
    else
    {
        if( (pDataParams->bOpeMode == RD_LIB_MODE_ISO) && (pDataParams->bPollCmd == PHPAL_I14443P3A_USE_WUPA))
        {
            status = phpalI14443p3a_Sw_WakeUpA(pDataParams, bAtqa);
        }
        else
        {
            if (bLenUidIn == 0U)
            {
                status = phpalI14443p3a_Sw_RequestA(pDataParams, bAtqa);
            }
            /* UidIn is given -> WupA */
            else
            {
                status = phpalI14443p3a_Sw_WakeUpA(pDataParams, bAtqa);
            }
        }
    }

    /* Collision error may happen */
    if ((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
    {
        /* Emvco: case_id TA304_XY */
        if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
        {
            return status;
        }
        bCollDetected = PH_ON;

        /* Set the more cards available flag */
        *pMoreCardsAvailable = PH_ON;
    }
    /* Status check */
    else
    {
        PH_CHECK_SUCCESS(status);
    }

    /* Go through all cascade levels */
    for (bCascadeIndex = 0; bCascadeIndex < 3U; bCascadeIndex++)
    {
        /* Set cascade level tags */
        switch (bCascadeIndex)
        {
        case 0:
            bCascadeLevel = PHPAL_I14443P3A_CASCADE_LEVEL_1;
            break;
        case 1:
            bCascadeLevel = PHPAL_I14443P3A_CASCADE_LEVEL_2;
            break;
        case 2:
            bCascadeLevel = PHPAL_I14443P3A_CASCADE_LEVEL_3;
            break;
        /* Default Case is not required as it will be a Dead Code due to the condition in for() loop statement. */
        } /* PRQA S 2002 */

        /* Copy know Uid part if neccessary */
        if (bLenUidIn == (bUidIndex + 4U))
        {
            /* Copy whole Uid if this is the only cascade level */
            (void)memcpy(&bUid[0], &pUidIn[bUidIndex], 4);

            /* [Incrementing Uid index would have no effect] */

            /* All bits except BCC are valid */
            bNvbUid = 0x40;
        }
        else if (bLenUidIn > (bUidIndex + /* */ 4U))
        {
            /* Prepend cascade tag if we expect more cascade levels*/
            bUid[0] = PHPAL_I14443P3A_CASCADE_TAG;

            /* Only three more uid bytes are valid for this level */
            (void)memcpy(&bUid[1], &pUidIn[bUidIndex], 3);

            /* Increment Uid index */
            bUidIndex += 3U;

            /* All bits except BCC are valid */
            bNvbUid = 0x40;
        }
        else
        {
            /* No Uid given */
            bNvbUid = 0;
        }

        /* Anticollision Loop */
        while (bNvbUid != 0x40U)
        {
            /* Perform single anticollision command */
            status = phpalI14443p3a_Sw_Anticollision(pDataParams, bCascadeLevel, bUid, bNvbUid, bUid, &bNvbUid);

            /* As per EMVCo 2.6 req 9.6.1.3, wait for at least Tmin retransmission in case of timeout error. */
            if (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
            {
                bRetryCount = 0;
                while (((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) && (bRetryCount < PH_NXPNFCRDLIB_CONFIG_EMVCO_RETRYCOUNT))
                {
                    bRetryCount++;
                    /* Wait for at least Tmin retransmission delay. */
                    PH_CHECK_SUCCESS_FCT(status, phhalHw_Wait(
                        pDataParams->pHalDataParams,
                        PHHAL_HW_TIME_MICROSECONDS,
                        PH_NXPNFCRDLIB_CONFIG_EMVCO_DEFAULT_RETRANSMISSION));

                    status = phpalI14443p3a_Sw_Anticollision(pDataParams, bCascadeLevel, bUid, bNvbUid, bUid, &bNvbUid);
                }
            }
            /* Collision error occured, add one (zero-)bit to Uid */
            if ((status & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
            {
                /* Emvco: case_id TA302_00 */
                if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
                {
                    /* Report Error to Application and Application will perform PICC Reset */
                    return status;
                }

                /* Set the more cards available flag */
                *pMoreCardsAvailable = PH_ON;

                /* Increment NvbUid by one bit */
                if ((bNvbUid & 0x07U) < 7U)
                {
                    bNvbUid++;
                }
                /* This is more difficult if we have to increment the bytecount */
                else
                {
                    bNvbUid = (uint8_t)((((bNvbUid & 0xF0U) >> 4U) + 1U) << 4U);
                }
            }
            else
            {
                /* Check success */
                PH_CHECK_SUCCESS(status);
            }

            if(bCollDetected == PH_OFF)
            {
                /* Emvco: case_id 302_10 */
                /*
                 * Digital Proto: Article 4.7.2 and 4.7.2.4
                 * ISO14443-3 Article 6.5.2.1
                 * EMVCo article 5.4.2
                 * So For Single UID Size The NFC Forum Device MUST set nfcid10 of a single-size NFCID1 and nfcid13
                 * of a double-size NFCID1 to a value different from 88h.
                 */
                if(
                    ( (((bAtqa[0] & 0xC0U) == 0x00U) && (bCascadeLevel == PHPAL_I14443P3A_CASCADE_LEVEL_1))||
                      (((bAtqa[0] & 0xC0U) == 0x40U) && (bCascadeLevel == PHPAL_I14443P3A_CASCADE_LEVEL_2)) ) &&
                      (bUid[0] == PHPAL_I14443P3A_CASCADE_TAG)
                  )
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
                }
            }
        }

        statusTmp = phpalI14443p3a_Sw_Select(pDataParams, bCascadeLevel, bUid, pSak);

        /* As per EMVCo 2.6 req 9.6.1.3, wait for at least Tmin retransmission in case of timeout error. */
        if (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
        {
            bRetryCount = 0;
            while (((statusTmp & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) && (bRetryCount < PH_NXPNFCRDLIB_CONFIG_EMVCO_RETRYCOUNT))
            {
                bRetryCount++;
                /* Wait for at least Tmin retransmission delay. */
                PH_CHECK_SUCCESS_FCT(status, phhalHw_Wait(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_TIME_MICROSECONDS,
                    PH_NXPNFCRDLIB_CONFIG_EMVCO_DEFAULT_RETRANSMISSION));

                statusTmp = phpalI14443p3a_Sw_Select(pDataParams, bCascadeLevel, bUid, pSak);
            }
        }

        /* Emvco: Case_id TA305. EMVCo Req. 9.6.1.2 */
        PH_CHECK_SUCCESS(statusTmp);

        /* Cascade Bit is cleared -> no further cascade levels */
        if ((pSak[0] & 0x04U) == 0U)
        {
            break;
        }
    }

    /* Return UID */
    (void)memcpy(pUidOut, pDataParams->abUid, pDataParams->bUidLength);
    *pLenUidOut = pDataParams->bUidLength;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3a_Sw_Exchange(
                                      phpalI14443p3a_Sw_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t * pTxBuffer,
                                      uint16_t wTxLength,
                                      uint8_t ** ppRxBuffer,
                                      uint16_t * pRxLength
                                      )
{
    /* direct mapping of HAL exchange function */
    return phhalHw_Exchange(
        pDataParams->pHalDataParams,
        wOption,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength);
}

phStatus_t phpalI14443p3a_Sw_RequestAEx(
                                        phpalI14443p3a_Sw_DataParams_t * pDataParams,
                                        uint8_t bReqCode,
                                        uint8_t * pAtqa
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM Status;
    uint8_t     PH_MEMLOC_REM cmd[1];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;
    uint16_t    PH_MEMLOC_REM wRegister;

    /* Disable MIFARE Classic Crypto1 */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1,
        PH_ON));

    /* Reset default data rates */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TXDATARATE_FRAMING,
        PHHAL_HW_RF_DATARATE_106));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
        PHHAL_HW_RF_DATARATE_106));

    /* Set selection timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I14443P3A_SELECTION_TIME_US + PHPAL_I14443P3A_EXT_TIME_US));

    /* Retrieve RxWaitTime */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXWAIT_US, &wRegister));
    /* Set RxWaitTime to 76 microseconds equivalent to 8 Bits. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXWAIT_US, 76));

    /* Switch off CRC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));

    /* Only 7 bits are valid */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 7));

    /* Send ReqA command */
    cmd[0] = bReqCode;
    statusTmp = phhalHw_Exchange(pDataParams->pHalDataParams, PH_EXCHANGE_DEFAULT, cmd, 1, &pResp, &wRespLength);

    /* Restore previous RxWaitTime */
    PH_CHECK_SUCCESS_FCT(Status, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXWAIT_US, wRegister));

    PH_CHECK_SUCCESS(statusTmp);
    /* Check and copy ATQA */
    if (wRespLength == PHPAL_I14443P3A_ATQA_LENGTH)
    {
        (void)memcpy(pAtqa, pResp, PHPAL_I14443P3A_ATQA_LENGTH);
        /* Emvco: case_id: TA304_10, TA304_11, TA304_12 */
        if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
        {
            /*5.3.2*/
            if(0U != (pAtqa[1] & 0xF0U))    /* Most significant nibble of byte 2 must be 0 */
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
            }
        }
        else
        {
            /* NFCForum-TS-DigitalProtocol-1.0, Requirement 18, Section 4.6.3.3 */
            if((((pAtqa[0] & 0x1FU) == 0x00U) && ((pAtqa[1] & 0x0FU) != 0x0CU)) || (((pAtqa[1] & 0x0FU) == 0x0CU) && ((pAtqa[0] & 0x1FU) != 0x00U)))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
            }
        }
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P3A);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p3a_Sw_GetSerialNo(
    phpalI14443p3a_Sw_DataParams_t * pDataParams,
    uint8_t * pUidOut,
    uint8_t * pLenUidOut
    )
{
    /* Return an error if UID is not complete */
    if (0U == (pDataParams->bUidComplete))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_ISO14443P3A);
    }

    /* Copy UID */
    (void)memcpy(pUidOut, pDataParams->abUid, pDataParams->bUidLength);
    *pLenUidOut = pDataParams->bUidLength;

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHPAL_I14443P3A_SW */
