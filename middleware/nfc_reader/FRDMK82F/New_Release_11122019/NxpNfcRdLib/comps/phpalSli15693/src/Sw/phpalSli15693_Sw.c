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
*  RSn: Generated 21. October 2009
*
*/

#include <ph_Status.h>
#include <phpalSli15693.h>
#include <ph_RefDefs.h>

#ifdef  NXPBUILD__PHPAL_SLI15693_SW

#include "phpalSli15693_Sw.h"
#include "phpalSli15693_Sw_Int.h"

phStatus_t phpalSli15693_Sw_Init(
                                 phpalSli15693_Sw_DataParams_t * pDataParams,
                                 uint16_t wSizeOfDataParams,
                                 void * pHalDataParams
                                 )
{
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* parameter structure length check */
    if (sizeof(phpalSli15693_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_SLI15693);
    }

    /* init private data */
    pDataParams->wId                = PH_COMP_PAL_SLI15693 | PHPAL_SLI15693_SW_ID;
    pDataParams->pHalDataParams     = pHalDataParams;
    pDataParams->wAdditionalInfo    = 0;
    pDataParams->bFlags             = PHPAL_SLI15693_SW_FLAGS_DEFAULT;
    pDataParams->bUidBitLength      = 0;
    pDataParams->bExplicitlyAddressed = 0;
    pDataParams->bOpeMode           = RD_LIB_MODE_ISO; /* Default Mode is ISO */
    pDataParams->bBuffering         = 0;


    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_Inventory(
                                      phpalSli15693_Sw_DataParams_t * pDataParams,
                                      uint8_t bFlags,
                                      uint8_t bAfi,
                                      uint8_t * pMask,
                                      uint8_t bMaskBitLength,
                                      uint8_t * pDsfid,
                                      uint8_t * pUid
                                      )
{
    uint8_t     PH_MEMLOC_REM bUidLength;
    uint16_t    PH_MEMLOC_REM wDataLength = 0;

    return phpalSli15693_Sw_InventoryEx(
        pDataParams,
        PHPAL_SLI15693_SW_CMD_INVENTORY,
        bFlags,
        bAfi,
        pMask,
        bMaskBitLength,
        0,
        0,
        pUid,
        &bUidLength,
        pDsfid,
        &wDataLength);
}

phStatus_t phpalSli15693_Sw_SendEof(
                                    phpalSli15693_Sw_DataParams_t * pDataParams,
                                    uint8_t bOption,
                                    uint8_t * pDsfid,
                                    uint8_t * pUid,
                                    uint8_t * pUidLength,
                                    uint8_t * pData,
                                    uint16_t * pDataLength
                                    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;
    uint8_t     PH_MEMLOC_REM bStoredUidLength;
    uint16_t    PH_MEMLOC_REM wAsk;
    uint16_t    PH_MEMLOC_REM wTimeout;

    /* Check bOption */
    switch (bOption)
    {
    case PHPAL_SLI15693_EOF_NEXT_SLOT:
    case PHPAL_SLI15693_EOF_NEXT_SLOT_INV_READ:
    case PHPAL_SLI15693_EOF_WRITE_ALIKE:
    case PHPAL_SLI15693_EOF_WRITE_ALIKE_WITH_WAIT:
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    /* Reset UID and data lengths */
    *pDsfid = 0;
    *pUidLength = 0;
    *pDataLength = 0;

    /* if requested, wait ~20ms upon sending EOF */
    if (bOption == PHPAL_SLI15693_EOF_WRITE_ALIKE_WITH_WAIT)
    {
        /* in case of write alike commands wait 20 ms. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(
            pDataParams->pHalDataParams,
            PHHAL_HW_TIME_MICROSECONDS,
            PHPAL_SLI15693_TIMEOUT_LONG_US));
    }

    /* Get the ASK 100 Condition */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_ASK100,
        &wAsk));

    if(bOption == PHPAL_SLI15693_EOF_NEXT_SLOT)
    {
        if(0U != (wAsk))
        {
            if(0U != (pDataParams->bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_HIGH_SOF_US;
            }
            else
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_LOW_SOF_US;
            }
        }
        else
        {
            if(0U != (pDataParams->bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_HIGH_NRT_US;
            }
            else
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_LOW_NRT_US;
            }
        }
        /* Set  timeout. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
            pDataParams,
            PHPAL_SLI15693_CONFIG_TIMEOUT_US,
            wTimeout));
    }
    else if(bOption == PHPAL_SLI15693_EOF_NEXT_SLOT_INV_READ)
    {
        if(0U != (wAsk))
        {
            if(0U != (pDataParams->bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_FASTHIGH_SOF_US;
            }
            else
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_FASTLOW_SOF_US;
            }
        }
        else
        {
            if(0U != (pDataParams->bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_FASTHIGH_NRT_US;
            }
            else
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_FASTLOW_NRT_US;
            }
        }

        /* Set  timeout. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
            pDataParams,
            PHPAL_SLI15693_CONFIG_TIMEOUT_US,
            wTimeout));
    }
    else
    {
        /* Set short timeout. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
            pDataParams,
            PHPAL_SLI15693_CONFIG_TIMEOUT_US,
            PHPAL_SLI15693_SW_TIMEOUT_SLOT_US));
    }

    /* Disable SOF, so only EOF is sent */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PH_OFF));

    /* Exchange command */
    status = phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        NULL,
        0,
        &pResp,
        &wRespLength);

    /* Reset HAL to send SOF and EOF */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_SYMBOL_START,
        PHHAL_HW_SYMBOL_I15693_SOF));

    /* Check Success */
    PH_CHECK_SUCCESS(status);

    /* Check RespLength */
    if (wRespLength == 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
    }

    /* check error flag */
    if (0U != (pResp[0] & PHPAL_SLI15693_FLAG_RESP_ERROR))
    {
        /* check the length in case of error */
        if (wRespLength != 2U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
        }

        pDataParams->wAdditionalInfo = pResp[1];
        return PH_ADD_COMPCODE_FIXED(PHPAL_SLI15693_ERR_ISO15693, PH_COMP_PAL_SLI15693);
    }

    if (bOption == PHPAL_SLI15693_EOF_NEXT_SLOT)
    {
        /* ignore flag byte */
        ++pResp;
        --wRespLength;

        /* the UID is only returned on a next slot command. */
        if (wRespLength != (1u + PHPAL_SLI15693_UID_LENGTH))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
        }

        /* wait T2 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(
            pDataParams->pHalDataParams,
            PHHAL_HW_TIME_MICROSECONDS,
            PHPAL_SLI15693_SW_T2_WAITING_TIME));

        /* Copy the DSFID to the data buffer */
        *pDsfid = pResp[0];

        /* retrieve the UID */
        (void)memcpy(pDataParams->pUid, &pResp[1], PHPAL_SLI15693_UID_LENGTH);
        pDataParams->bUidBitLength = PHPAL_SLI15693_SW_UID_COMPLETE;

        /* return the UID */
        (void)memcpy(pUid, &pResp[1], PHPAL_SLI15693_UID_LENGTH);
        *pUidLength = PHPAL_SLI15693_UID_LENGTH;

        /* set addressed flag */
        pDataParams->bFlags |= PHPAL_SLI15693_FLAG_ADDRESSED;
    }
    else if (bOption == PHPAL_SLI15693_EOF_NEXT_SLOT_INV_READ)
    {
        /* ignore flag byte */
        ++pResp;
        --wRespLength;

        /* Option flag is set -> we also received (partial) UID */
        if (0U != (pDataParams->bFlags & PHPAL_SLI15693_FLAG_OPTION))
        {
            /* The response length should not be less than the remaining UID */
            bStoredUidLength = pDataParams->bUidBitLength >> 3U;
            *pUidLength = PHPAL_SLI15693_UID_LENGTH - bStoredUidLength;
            if (wRespLength < *pUidLength)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
            }

            if (bStoredUidLength < PHPAL_SLI15693_UID_LENGTH)
            {
                /* We need to merge the contents of the mask buffer and the received data */
                if (0U != (pDataParams->bUidBitLength % 8U))
                {
                    if (bStoredUidLength < 7U)
                    {
                        /* copy the UID bytes we received from the card */
                        (void)memcpy(&(pDataParams->pUid[bStoredUidLength + 1U]), &pResp[1], ((uint32_t)(*pUidLength)-1U));
                    }

                    /* merge mask-bits with received bits */
                    pDataParams->pUid[bStoredUidLength] |= pResp[0];
                }
                else
                {
                    /* copy the UID bytes we received from the card */
                    (void)memcpy(&(pDataParams->pUid[bStoredUidLength]), pResp, *pUidLength);
                }

                /* Return the received (partial) UID */
                (void)memcpy(pUid, pResp, *pUidLength);
            }

            /* UID is now complete */
            pDataParams->bUidBitLength = PHPAL_SLI15693_SW_UID_COMPLETE;

            /* shift pointer and length */
            pResp += *pUidLength;
            wRespLength = wRespLength - *pUidLength;
        }

        /* copy the received data to the provided buffer */
        (void)memcpy(pData, pResp, wRespLength);

        /* the remaining bytes of the response are the data bytes */
        *pDataLength = wRespLength;

        /* set addressed flag */
        pDataParams->bFlags |= PHPAL_SLI15693_FLAG_ADDRESSED;

        /* wait T2 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(
            pDataParams->pHalDataParams,
            PHHAL_HW_TIME_MICROSECONDS,
            PHPAL_SLI15693_SW_T2_WAITING_TIME));
    }
    else
    {
        /* 1 byte is only returned on a eof for a write alike command. */
        if (wRespLength != 1U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_StayQuiet(
                                      phpalSli15693_Sw_DataParams_t * pDataParams
                                      )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* clear selected flag */
    pDataParams->bFlags &= (uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_SELECTED;

    /* set addressed flag */
    pDataParams->bFlags |= PHPAL_SLI15693_FLAG_ADDRESSED;

    /* reset to ready command */
    aCmd[0] = PHPAL_SLI15693_SW_CMD_STAY_QUIET;

    /* Set short timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_SHORT_US));

    /* Exchange command */
    status = phpalSli15693_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        &pResp,
        &wRespLength);

    /* No response to the command allowed */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS(status);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_StayQuietPersistent(
                                                phpalSli15693_Sw_DataParams_t * pDataParams
                                                )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* clear selected flag */
    pDataParams->bFlags &= (uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_SELECTED;

    /* set addressed flag */
    pDataParams->bFlags |= PHPAL_SLI15693_FLAG_ADDRESSED;

    /* stay quiet persistent command */
    aCmd[0] = PHPAL_SLI15693_SW_CMD_STAY_QUIET_PERSISTENT;

    /* Set short timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_SHORT_US));

    /* Exchange command */
    status = phpalSli15693_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        &pResp,
        &wRespLength);

    /* No response to the command allowed */
    if ((status & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS(status);
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_Select(
                                   phpalSli15693_Sw_DataParams_t * pDataParams
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* clear selected flag */
    pDataParams->bFlags &= (uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_SELECTED;

    /* set addressed flag */
    pDataParams->bFlags |= PHPAL_SLI15693_FLAG_ADDRESSED;

    /* reset to ready command */
    aCmd[0] = PHPAL_SLI15693_SW_CMD_SELECT;

    /* Set short timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_SHORT_US));

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        &pResp,
        &wRespLength));

    /* We shall not receive any data */
    if (wRespLength != 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
    }

    /* clear addressed flag */
    pDataParams->bFlags &= (uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_ADDRESSED;

    /* set selected flag */
    pDataParams->bFlags |= PHPAL_SLI15693_FLAG_SELECTED;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_ResetToReady(
    phpalSli15693_Sw_DataParams_t * pDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;

    /* reset to ready command */
    aCmd[0] = PHPAL_SLI15693_SW_CMD_RESET_TO_READY;

    /* Set short timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_SHORT_US));

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        &pResp,
        &wRespLength));

    /* We shall not receive any data */
    if (wRespLength != 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
    }

    /* clear selected flag */
    pDataParams->bFlags &= (uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_SELECTED;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_Exchange(
                                     phpalSli15693_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t * pTxBuffer,
                                     uint16_t wTxLength,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pRxLength
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bFirstBlock[3];
    uint16_t    PH_MEMLOC_REM wTransmitLength = 0;
    uint16_t    PH_MEMLOC_REM RxLength;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;

    /* the frame to exchange looks like the following:
    {optional} [one byte]
    [flags][CMD]{[MFC]}{[UID0] .. [UID3]}[CMDParam(0)] .. [CMDParam(N)] */

    /* Check for valid UID in addressed mode */
    if ((0U == ((pDataParams->bFlags & PHPAL_SLI15693_FLAG_INVENTORY))) && (0U != ((pDataParams->bFlags & PHPAL_SLI15693_FLAG_ADDRESSED))))
    {
        /* Return an error if UID is not valid */
        if (pDataParams->bUidBitLength != PHPAL_SLI15693_SW_UID_COMPLETE)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_SLI15693);
        }
    }

    /* Check if caller has provided valid RxBuffer */
    if (ppRxBuffer == NULL)
    {
        ppRxBuffer = &pRxBuffer;
    }
    if (pRxLength == NULL)
    {
        pRxLength = &RxLength;
    }

    if ((0U == ((wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))))
    {
        /* This is the first chunk of a command frame. Add the Flags byte and ... */
        bFirstBlock[wTransmitLength++] = pDataParams->bFlags;
        bFirstBlock[wTransmitLength++] = pTxBuffer[0];

        /* add MFC byte for custom commands */
        if ((pTxBuffer[0] > PHPAL_SLI15693_SW_CUSTOM_CMD_CODE_BEGIN) && (pTxBuffer[0] < PHPAL_SLI15693_SW_CUSTOM_CMD_CODE_END))
        {
            /* if no serial number is valid we assume that we use only NXP tags. */
            bFirstBlock[wTransmitLength++] = PHPAL_SLI15693_SW_NXP_MFG_CODE;
        }
        else
        {
            if((pTxBuffer[0] == PHPAL_SLI15693_SW_CMD_EXTENDED_GET_SYSTEM_INFORMATION)
               && (wTxLength > 1U))
           {
               /* Handle the Frame format preparation for Extended Get System Information specifically since this takes 1
                *  UID from 3rd Byte whereas rest other commands take UID from 2nd Byte
                *  SOF + FLAGS + COMMAND CODE +  INFO FLAGS  + UID  + CRC + EOF.
                */

               /* Copy Info Flags */
               bFirstBlock[wTransmitLength++] = pTxBuffer[1];

               /* First byte was the command byte */
               ++pTxBuffer;
               --wTxLength;
           }
        }

        /* Exchange command */
        /* clear buffer but do not send */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_BUFFERED_BIT,
            bFirstBlock,
            wTransmitLength,
            ppRxBuffer,
            pRxLength));

        /* First set of TxBuffer contains
         *  1. command code + Info Bytes
         *  2. command Code
         */
        ++pTxBuffer;
        --wTxLength;

        /* If we are not waiting any more we can transmit the UID bytes, if necessary. */
        if ((0U == ((pDataParams->bFlags & PHPAL_SLI15693_FLAG_INVENTORY))) && (0U != ((pDataParams->bFlags & PHPAL_SLI15693_FLAG_ADDRESSED))))
        {
            if(0U == (pDataParams->bExplicitlyAddressed))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
                    pDataParams->pHalDataParams,
                    PH_EXCHANGE_BUFFER_CONT,    /* do not clear, do not send */
                    pDataParams->pUid,
                    PHPAL_SLI15693_UID_LENGTH,
                    ppRxBuffer,
                    pRxLength));
            }
        }

        /* Now we need to keep our buffered contents */
        wOption |= PH_EXCHANGE_LEAVE_BUFFER_BIT;
    }

    /* Exchange command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        wOption,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength));

    /* If this was the last (or only) chunk, we shoult have received a response,
    so we extract the Flags byte and check the status */
    if ((0U == ((wOption & PH_EXCHANGE_BUFFERED_BIT))))
    {
        /* First Check RespLength*/
        if (*pRxLength == 0U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
        }

        /*check error flag*/
        if (0U != ((*ppRxBuffer[0]) & PHPAL_SLI15693_FLAG_RESP_ERROR))
        {
            /* check the length in case of error */
            if (*pRxLength != 2U)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
            }

            /* Copy error code to additional info */
            pDataParams->wAdditionalInfo = (*ppRxBuffer)[1];

            /* Return empty receive buffer */
            *ppRxBuffer = NULL;
            *pRxLength = 0;

            return PH_ADD_COMPCODE_FIXED(PHPAL_SLI15693_ERR_ISO15693, PH_COMP_PAL_SLI15693);
        }

        /* Copy response flag to additional info */
        pDataParams->wAdditionalInfo = (*ppRxBuffer)[0];

        /* adjust pointer and length */
        ++(*ppRxBuffer);
        --(*pRxLength);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_ActivateCard(
    phpalSli15693_Sw_DataParams_t * pDataParams,
    uint8_t  bOption,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t * pDsfid,
    uint8_t * pUid,
    uint8_t * pMoreCardsAvailable
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bSlot;
    uint8_t     PH_MEMLOC_REM bTimeOutNotAllowed;
    uint8_t     PH_MEMLOC_REM aMaskBuffer[PHPAL_SLI15693_UID_LENGTH];
    uint8_t     PH_MEMLOC_REM bMaskByteLength;
    uint8_t     PH_MEMLOC_REM bUidLengthDummy;
    uint8_t     PH_MEMLOC_REM bDataDummy[1];
    uint16_t    PH_MEMLOC_REM wDataLengthDummy;

    /* Check option byte */
    if ((bOption != PHPAL_SLI15693_ACTIVATE_ADDRESSED) && (bOption != PHPAL_SLI15693_ACTIVATE_SELECTED))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    /* init return value */
    *pMoreCardsAvailable = 0;

    /* Perform inventory command */
    status = phpalSli15693_Sw_Inventory(pDataParams, bFlags, bAfi, pMask, bMaskBitLength, pDsfid, pUid);

    /* Check for invalid parameter */
    if ((status & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    /* Retrieve mask byte length */
    bMaskByteLength = ((bMaskBitLength % 8U) != 0U) ? ((bMaskBitLength >> 3U) + 1U) : (bMaskBitLength >> 3U);

    /* init mask buffer */
    (void)memset(aMaskBuffer, 0, (size_t)sizeof(aMaskBuffer));
    (void)memcpy(aMaskBuffer, pMask, bMaskByteLength);

    /* if we have more than one slot, we need to send slot markers (b6 = 0) */
    if ((0U == ((bFlags & PHPAL_SLI15693_FLAG_NBSLOTS))))
    {
        /* Slot marker loop */
        bSlot = 0;
        while ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            switch (status & PH_ERR_MASK)
            {
                /*  If a CRC error or collision error occured --> resolve collision */
            case PH_ERR_COLLISION_ERROR:
            case PH_ERR_INTEGRITY_ERROR:

                *pMoreCardsAvailable = 1;

                /* Get bit-length of last valid byte */
                bUidLengthDummy = (uint8_t)(bMaskBitLength % 8U);

                /* Whole byte is valid -> append slot number to next byte */
                if (bUidLengthDummy == 0U)
                {
                    aMaskBuffer[bMaskBitLength >> 3U] = bSlot;
                }
                /* Partial byte is valid */
                else
                {
                    /* Fill the invalid bits of the incomple byte with the 4 bits slot number */
                    aMaskBuffer[bMaskBitLength >> 3U] &= (uint8_t)(0xFFU >> (8U - bUidLengthDummy));
                    aMaskBuffer[bMaskBitLength >> 3U] |= (uint8_t)(bSlot << bUidLengthDummy);

                    /* If not all 4 bits of the Slot number fit in the incomplete byte, put the rest in the next byte */
                    if (bUidLengthDummy > 4U)
                    {
                        aMaskBuffer[(bMaskBitLength >> 3U) + 1U] = (uint8_t)(bSlot >> bUidLengthDummy);
                    }
                }

                /* Increment the bitlength by the 4 bits slot number */
                bMaskBitLength = bMaskBitLength + 4U;

                /* Start again from Slot number 0 */
                bSlot = 0;
                status = phpalSli15693_Sw_Inventory(pDataParams, bFlags, bAfi, aMaskBuffer, bMaskBitLength, pDsfid, pUid);
                break;

                /* Timeout -> advance to next slot */
            case PH_ERR_IO_TIMEOUT:

                /* Send EOF and increment slot number */
                ++bSlot;
                status = phpalSli15693_Sw_SendEof(
                    pDataParams,
                    PHPAL_SLI15693_EOF_NEXT_SLOT,
                    pDsfid,
                    pUid,
                    &bUidLengthDummy,
                    bDataDummy,
                    &wDataLengthDummy);
                break;

                /* Other error -> bail out */
            default:
                return status;
            }

            /* if we ran through all slots, we are done */
            if (bSlot == 15U)
            {
                break;
            }
        }
    }
    /* use only one slot */
    else
    {
        /* Do not allow a time out after the first Inventory command since then, no
        card with the specified mask is present in the field. */
        bTimeOutNotAllowed = 1;

        /* Bit mask loop */
        while (bMaskBitLength < PHPAL_SLI15693_SW_UID_COMPLETE)
        {
            switch (status & PH_ERR_MASK)
            {
                /* If there was a card, the loop is done */
            case PH_ERR_SUCCESS:
                bMaskBitLength = PHPAL_SLI15693_SW_UID_COMPLETE;
                break;

                /* In case of a timeout, no card with the actual mask is in the field, so toggle the last bit of the mask */
            case PH_ERR_IO_TIMEOUT:
                /* Abort, if this bit was already tested */
                if (0U != (bTimeOutNotAllowed))
                {
                    return status;
                }

                /* the bit mask is > 0U, because in the case of bitmask = 0 a timeout can not happen */
                aMaskBuffer[(uint8_t)(bMaskBitLength - 1U) >> 3U] = aMaskBuffer[(uint8_t)(bMaskBitLength - 1U) >> 3U] ^ (uint8_t)(0x01U << (uint8_t)((bMaskBitLength - 1U) % 8U));
                bTimeOutNotAllowed = 1;
                break;

                /*  If a CRC error or collision error occured add one bit to the mask. */
            case PH_ERR_COLLISION_ERROR:
            case PH_ERR_INTEGRITY_ERROR:
                /* Note: The value of this bit does not realy matter since it will be toggled in case of a timeout */
                *pMoreCardsAvailable = 1;

                ++bMaskBitLength;
                bTimeOutNotAllowed = 0;
                break;

                /* If it is an other error than Collision, Crc or Timeout Error the function should return the error */
            default:
                return status;
            }

            /* Perform next inventory command to perform anticollision */
            if (bMaskBitLength < PHPAL_SLI15693_SW_UID_COMPLETE)
            {
                status = phpalSli15693_Sw_Inventory(pDataParams, bFlags, bAfi, aMaskBuffer, bMaskBitLength, pDsfid, pUid);
            }
        }
    }

    /* no need to proceed if an error occured. */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* clear option flag */
        pDataParams->bFlags &= (uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_OPTION;
        return status;
    }

    /* Select the card if neccessary */
    if (0U != (bOption & PHPAL_SLI15693_ACTIVATE_SELECTED))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Select(pDataParams));
    }

    /* Set long timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_GetSerialNo(
                                        phpalSli15693_Sw_DataParams_t * pDataParams,
                                        uint8_t * pUid,
                                        uint8_t * pUidLength
                                        )
{
    /* Return an error if UID is not valid */
    if (pDataParams->bUidBitLength != PHPAL_SLI15693_SW_UID_COMPLETE)
    {
        *pUidLength = 0;
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_PAL_SLI15693);
    }

    /* copy the UID */
    (void)memcpy(pUid, pDataParams->pUid, PHPAL_SLI15693_UID_LENGTH);
    *pUidLength = PHPAL_SLI15693_UID_LENGTH;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_SetSerialNo(
                                        phpalSli15693_Sw_DataParams_t * pDataParams,
                                        uint8_t * pUid,
                                        uint8_t bUidLength
                                        )
{
    /* Check for valid UID length */
    if(bUidLength != PHPAL_SLI15693_UID_LENGTH)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    /* Copy UID and UID length */
    (void)memcpy(pDataParams->pUid, pUid, PHPAL_SLI15693_UID_LENGTH);
    pDataParams->bUidBitLength = PHPAL_SLI15693_SW_UID_COMPLETE;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_SetConfig(
                                      phpalSli15693_Sw_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t wValue
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wValueOld;

    switch (wConfig)
    {
    case PHPAL_SLI15693_CONFIG_FLAGS:

        /* Retrieve current subcarrier setting */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_SUBCARRIER,
            &wValueOld));

        /* Set dual subcarrier if HAL is not already configured for it */
        if (0U != (wValue & PHPAL_SLI15693_FLAG_TWO_SUB_CARRIERS))
        {
            /* NFC mode supports only single sub-carrier mode */
            if(pDataParams->bOpeMode == RD_LIB_MODE_NFC)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
            }

            if (wValueOld != PHHAL_HW_SUBCARRIER_DUAL)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_SUBCARRIER,
                    PHHAL_HW_SUBCARRIER_DUAL));
            }
        }
        /* Set single subcarrier if HAL is not already configured for it */
        else
        {
            if (wValueOld != PHHAL_HW_SUBCARRIER_SINGLE)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_SUBCARRIER,
                    PHHAL_HW_SUBCARRIER_SINGLE));
            }
        }

        /* Retrieve current datarate setting */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
            &wValueOld));

        /* Set low data rate if HAL is not already configured for it */
        if (0U == (wValue & PHPAL_SLI15693_FLAG_DATA_RATE))
        {
            /* NFC mode supports only HIGH Data Rate */
            if(pDataParams->bOpeMode == RD_LIB_MODE_NFC)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
            }

            if (wValueOld != PHHAL_HW_RF_RX_DATARATE_LOW)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
                    PHHAL_HW_RF_RX_DATARATE_LOW));
            }

        }
        /* Set high data rate if HAL is not already configured for it */
        else
        {
            if (wValueOld != PHHAL_HW_RF_RX_DATARATE_HIGH)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
                    PHHAL_HW_RF_RX_DATARATE_HIGH));
            }
        }

        /* retrieve flags */
        pDataParams->bFlags = (uint8_t)wValue;

        break;

    case PHPAL_SLI15693_CONFIG_TIMEOUT_US:

        if (((uint32_t)wValue + PHPAL_SLI15693_SW_EXT_TIME_US) > 0xFFFFU)
        {
            /* Return error */
            /* Maximum allowed value is 0xFFC3 (0xFFFF - PHPAL_SLI15693_SW_EXT_TIME_US) */
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_PAL_SLI15693);
        }

        /* Extend timeout a little */
        wValue += PHPAL_SLI15693_SW_EXT_TIME_US;

        /* Retrieve current timeout */
        statusTmp = phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            &wValueOld);

        /* Set the new timeout */
        if ((((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS) && (wValue != wValueOld)) ||
            ((statusTmp & PH_ERR_MASK) == PH_ERR_PARAMETER_OVERFLOW))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
                wValue));
        }
        else
        {
            /* Return error if the error is other than PH_ERR_PARAMETER_OVERFLOW */
            PH_CHECK_SUCCESS(statusTmp);
        }
        break;

    case PHPAL_SLI15693_CONFIG_TIMEOUT_MS:

        /* Retrieve current timeout */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
            &wValueOld));

        /* Set the new timeout */
        if (wValue != wValueOld)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
                wValue));
        }
        break;


    case PHPAL_SLI15693_CONFIG_ENABLE_BUFFERING:
        pDataParams->bBuffering = (uint8_t)wValue;
        break;


    case PHPAL_SLI15693_CONFIG_RXDATARATE:

        if((pDataParams->bOpeMode == RD_LIB_MODE_NFC) && (wValue != PHPAL_SLI15693_26KBPS_DATARATE))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
        }

        switch(wValue)
        {
        case PHPAL_SLI15693_26KBPS_DATARATE:
            wValue = PHHAL_HW_RF_RX_DATARATE_HIGH;
            break;
        case PHPAL_SLI15693_53KBPS_DATARATE:
            wValue = PHHAL_HW_RF_RX_DATARATE_FAST_HIGH;
            break;
        case PHPAL_SLI15693_106KBPS_DATARATE:
            wValue = PHHAL_HW_RF_DATARATE_106;
            break;
        case PHPAL_SLI15693_212KBPS_DATARATE:
            wValue = PHHAL_HW_RF_DATARATE_212;
            break;
        default:
            break;
        }

        /* Set the new RX data rate */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
            wValue));


        break;


    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_GetConfig(
                                      phpalSli15693_Sw_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t * pValue
                                      )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    switch (wConfig)
    {
    case PHPAL_SLI15693_CONFIG_FLAGS:
        *pValue = pDataParams->bFlags;
        break;

    case PHPAL_SLI15693_CONFIG_ADD_INFO:
        *pValue = pDataParams->wAdditionalInfo;
        break;

    case PHPAL_SLI15693_CONFIG_TIMEOUT_US:
        /* Get HAL timeout value */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            pValue));

        /* Remove timeout extension */
        *pValue -= PHPAL_SLI15693_SW_EXT_TIME_US;
        break;

    case PHPAL_SLI15693_CONFIG_TIMEOUT_MS:
        /* Get HAL timeout value */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
            pValue));

        break;


    case PHPAL_SLI15693_CONFIG_RXDATARATE:
        /* Get HAL timeout value */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
            pValue));

        /* Map the HAL RX BaudRates to generic 15693 BaudRates */
        switch(*pValue)
        {
        case PHHAL_HW_RF_RX_DATARATE_HIGH:
            *pValue = PHPAL_SLI15693_26KBPS_DATARATE;
            break;
        case PHHAL_HW_RF_RX_DATARATE_FAST_HIGH:
            *pValue = PHPAL_SLI15693_53KBPS_DATARATE;
            break;
        case PHHAL_HW_RF_DATARATE_106:
            *pValue = PHPAL_SLI15693_106KBPS_DATARATE;
            break;
        case PHHAL_HW_RF_DATARATE_212:
            *pValue = PHPAL_SLI15693_212KBPS_DATARATE;
            break;
        default:
            break;
        }
        break;


    case PHPAL_SLI15693_CONFIG_ENABLE_BUFFERING:
        *pValue = pDataParams->bBuffering;
        break;

    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalSli15693_Sw_InventoryRead(
    phpalSli15693_Sw_DataParams_t * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bBlockNo,
    uint16_t wNoOfBlocks,
    uint8_t * pUid,
    uint8_t * bUidLength,
    uint8_t * pData,
    uint16_t * wLength
    )
{
    return phpalSli15693_Sw_InventoryEx(
        pDataParams,
        PHPAL_SLI15693_SW_CMD_INVENTORY_READ,
        bFlags,
        bAfi,
        pMask,
        bMaskBitLength,
        bBlockNo,
        wNoOfBlocks,
        pUid,
        bUidLength,
        pData,
        wLength);
}

phStatus_t phpalSli15693_Sw_InventoryReadExtended(
    phpalSli15693_Sw_DataParams_t * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bExtendedOptions,
    uint8_t * pCID,
    uint8_t bBlockNo,
    uint16_t wNoOfBlocks,
    uint8_t * pCidOut,
    uint8_t * pUid,
    uint8_t * pUidLength,
    uint8_t * pData,
    uint16_t * wLength
    )
{
    return phpalSli15693_Sw_InventoryExt(
            pDataParams,
            PHPAL_SLI15693_SW_CMD_INVENTORY_READ,
            bFlags,
            bAfi,
            1,
            bExtendedOptions,
            pCID,
            pMask,
            bMaskBitLength,
            bBlockNo,
            wNoOfBlocks,
            pCidOut,
            pUid,
            pUidLength,
            pData,
            wLength);
}

phStatus_t phpalSli15693_Sw_FastInventoryRead(
    phpalSli15693_Sw_DataParams_t * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bBlockNo,
    uint16_t wNoOfBlocks,
    uint8_t * pUid,
    uint8_t * pUidLength,
    uint8_t * pData,
    uint16_t * pDataLength
    )
{
    return phpalSli15693_Sw_InventoryEx(
        pDataParams,
        PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_READ,
        bFlags,
        bAfi,
        pMask,
        bMaskBitLength,
        bBlockNo,
        wNoOfBlocks,
        pUid,
        pUidLength,
        pData,
        pDataLength);
}

phStatus_t phpalSli15693_Sw_FastInventoryReadExtended(
    phpalSli15693_Sw_DataParams_t * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bExtendedOptions,
    uint8_t * pCID,
    uint8_t bBlockNo,
    uint16_t wNoOfBlocks,
    uint8_t * pCidOut,
    uint8_t * pUid,
    uint8_t * bUidLength,
    uint8_t * pData,
    uint16_t * wLength
    )
{
    return phpalSli15693_Sw_InventoryExt(
            pDataParams,
            PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_READ,
            bFlags,
            bAfi,
            1,
            bExtendedOptions,
            pCID,
            pMask,
            bMaskBitLength,
            bBlockNo,
            wNoOfBlocks,
            pCidOut,
            pUid,
            bUidLength,
            pData,
            wLength);
}

phStatus_t phpalSli15693_Sw_InventoryPageRead(
    phpalSli15693_Sw_DataParams_t * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bPageNo,
    uint16_t wNoOfPages,
    uint8_t * pUid,
    uint8_t * pUidLength,
    uint8_t * pData,
    uint16_t * pDataLength
    )
{
    return phpalSli15693_Sw_InventoryEx(
        pDataParams,
        PHPAL_SLI15693_SW_CMD_INVENTORY_PAGE_READ,
        bFlags,
        bAfi,
        pMask,
        bMaskBitLength,
        bPageNo,
        wNoOfPages,
        pUid,
        pUidLength,
        pData,
        pDataLength);
}

phStatus_t phpalSli15693_Sw_FastInventoryPageRead(
    phpalSli15693_Sw_DataParams_t * pDataParams,
    uint8_t bFlags,
    uint8_t bAfi,
    uint8_t * pMask,
    uint8_t bMaskBitLength,
    uint8_t bPageNo,
    uint16_t wNoOfPages,
    uint8_t * pUid,
    uint8_t * pUidLength,
    uint8_t * pData,
    uint16_t * pDataLength
    )
{
    return phpalSli15693_Sw_InventoryEx(
        pDataParams,
        PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_PAGE_READ,
        bFlags,
        bAfi,
        pMask,
        bMaskBitLength,
        bPageNo,
        wNoOfPages,
        pUid,
        pUidLength,
        pData,
        pDataLength);
}


phStatus_t phpalSli15693_Sw_InventoryEx(
                                        phpalSli15693_Sw_DataParams_t * pDataParams,
                                        uint8_t bCmd,
                                        uint8_t bFlags,
                                        uint8_t bAfi,
                                        uint8_t * pMask,
                                        uint8_t bMaskBitLength,
                                        uint8_t bPage_Block_No,
                                        uint16_t wNoOfPages_Blocks,
                                        uint8_t * pUid,
                                        uint8_t * pUidLength,
                                        uint8_t * pData,
                                        uint16_t * pDataLength
                                        )
{
        return  phpalSli15693_Sw_InventoryExt(
                pDataParams,
                bCmd,
                bFlags,
                bAfi,
                0,
                0,
                NULL,
                pMask,
                bMaskBitLength,
                bPage_Block_No,
                wNoOfPages_Blocks,
                NULL,
                pUid,
                pUidLength,
                pData,
                pDataLength
                );
}


phStatus_t phpalSli15693_Sw_InventoryExt(
                                        phpalSli15693_Sw_DataParams_t * pDataParams,
                                        uint8_t bCmd,
                                        uint8_t bFlags,
                                        uint8_t bAfi,
                                        uint8_t bExtended,
                                        uint8_t bExtendedOptions,
                                        uint8_t * pCID,
                                        uint8_t * pMask,
                                        uint8_t bMaskBitLength,
                                        uint8_t bPage_Block_No,
                                        uint16_t wNoOfPages_Blocks,
                                        uint8_t * pCIDOut,
                                        uint8_t * pUid,
                                        uint8_t * pUidLength,
                                        uint8_t * pData,
                                        uint16_t * pDataLength
                                        )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuffer[15];
    uint8_t     PH_MEMLOC_REM bCmdBufferLen = 0;
    uint8_t     PH_MEMLOC_REM bCmdOffset = 0;
    uint8_t     PH_MEMLOC_REM bMaskByteLength;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLength = 0;
    uint8_t     PH_MEMLOC_REM bStoredUidLength = 0;
    uint16_t    PH_MEMLOC_REM wAsk = 0;
    uint16_t    PH_MEMLOC_REM wTimeout = 0;
    uint16_t    PH_MEMLOC_REM wCurrBlocksToRead = 0;
    uint16_t    PH_MEMLOC_REM wCurrBlockNo = 0;
    uint16_t    PH_MEMLOC_REM wMaxNoBlocks = 0;
    uint8_t     PH_MEMLOC_REM bAllBlocksRead = 0;
    uint8_t     PH_MEMLOC_REM bFirst = 0;


    /* Reset UID and data length */
    pDataParams->bUidBitLength = 0;

    if(((pUidLength == NULL) || (pUid == NULL)) || ((0U != ((bExtendedOptions & PHPAL_SLI15693_FLAG_SKIP_DATA))) && ((pDataLength == NULL) || (pData == NULL))))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    /* If Extended Option is looking for CID response then CID should not be null */
    if ((0U != ((bExtendedOptions & PHPAL_SLI15693_FLAG_CID_COMPARE))) && (pCID == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
    }
    /* If Extended Option is looking for CID response then CID should not be null */
    if((0U != ((bExtendedOptions & PHPAL_SLI15693_FLAG_CID_RESPONSE))) && (pCIDOut == NULL))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    *pUidLength = 0;
    *pDataLength = 0;

    /* first of all we check the bMaskBitLength according to ISO/IEC15693, 8.1, assuming
    the inventory flag to be set, si b6 deines the number of slots
    b6 = 1: --> one slot --> mask length = [0 .. 64] bits
    b6 = 0: --> 16 slots --> mask length = [0 .. 60] bits */
    if (((0U != ((bFlags & PHPAL_SLI15693_FLAG_NBSLOTS))) && (bMaskBitLength > 64U)) ||
        ((0U == ((bFlags & PHPAL_SLI15693_FLAG_NBSLOTS))) && (bMaskBitLength > 60U)))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
    }

    /* Number of Pages/Blocks check */
    if (bCmd != PHPAL_SLI15693_SW_CMD_INVENTORY)
    {
        /* Number of Pages/Blocks can't be zero */
        if (wNoOfPages_Blocks == 0U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
        }

        /* check if the block number exceeds the limit */
        if (((uint16_t)bPage_Block_No + wNoOfPages_Blocks) >= PHPAL_SLI15693_SW_MAX_BLOCKS)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_SLI15693);
        }
    }

    /* Add command code */
    bCmdBuffer[bCmdBufferLen++] = bCmd;

    /* Apply flag settings */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams,
        PHPAL_SLI15693_CONFIG_FLAGS,
        bFlags));

    /* Overwrite datarate for fast inventory (page-)read */
    if ((bCmd == PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_READ) ||
        (bCmd == PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_PAGE_READ))
    {
        if (0U != (bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
                PHHAL_HW_RF_RX_DATARATE_FAST_HIGH));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXDATARATE_FRAMING,
                PHHAL_HW_RF_RX_DATARATE_FAST_LOW));
        }
    }

    /* check if AFI shall be sent: */
    if ((0U != ((bFlags & PHPAL_SLI15693_FLAG_AFI))) && (0U != ((bFlags & PHPAL_SLI15693_FLAG_INVENTORY))))
    {
        bCmdBuffer[bCmdBufferLen++] = bAfi;
    }

    /* Add mask length */
    /* Making the MSB of mask length byte as 1 to show extended mode */
    /* In case of extended mode add the extended option in command */
    if (0U != (bExtended))
    {
        bCmdBuffer[bCmdBufferLen++] = bMaskBitLength | PHPAL_SLI15693_FLAG_INVENTORY_READ_EXTENSION;
        bCmdBuffer[bCmdBufferLen++] = bExtendedOptions;
        if (0U != (bExtendedOptions & PHPAL_SLI15693_FLAG_CID_COMPARE))
        {
            bCmdBuffer[bCmdBufferLen++] = pCID[0];
            bCmdBuffer[bCmdBufferLen++] = pCID[1];
        }
    }
    else
    {
        bCmdBuffer[bCmdBufferLen++] = bMaskBitLength;
    }

    /* Retrieve mask byte length */
    bMaskByteLength = ((bMaskBitLength % 8U) != 0U) ? ((bMaskBitLength >> 3U) + 1U) : (bMaskBitLength >> 3U);

    /* prepare mask in dataparams */
    (void)memcpy(pDataParams->pUid, pMask, bMaskByteLength );
    pDataParams->bUidBitLength = bMaskBitLength;

    /* prepare mask to send */
    (void)memcpy(&bCmdBuffer[bCmdBufferLen], pMask, bMaskByteLength );
    bCmdBufferLen = bCmdBufferLen + bMaskByteLength;

    /* mask out invalid bits */
    if (0U != (bMaskBitLength & 0x07U))
    {
        bCmdBuffer[bCmdBufferLen - 1U] &= (uint8_t)(0xFFU >> (8U - (bMaskBitLength & 0x07U)));
    }

    /* Get the ASK 100 Condition */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_ASK100,
        &wAsk));

    /* Calculate Timeout based on ask and baud rate */
    if(wAsk != 0U)
    {
        if ((bCmd == PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_READ) ||
            (bCmd == PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_PAGE_READ))
        {
            if(0U != (bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_FASTHIGH_SOF_US;
            }
            else
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_FASTLOW_SOF_US;
            }
        }
        else
        {
            if(0U != (bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_HIGH_SOF_US;
            }
            else
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_LOW_SOF_US;
            }
        }
    }
    else
    {
        if ((bCmd == PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_READ) ||
            (bCmd == PHPAL_SLI15693_SW_CMD_FAST_INVENTORY_PAGE_READ))
        {
            if(0U != (bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_FASTHIGH_NRT_US;
            }
            else
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_FASTLOW_NRT_US;
            }
        }
        else
        {
            if(0U != (bFlags & PHPAL_SLI15693_FLAG_DATA_RATE))
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_HIGH_NRT_US;
            }
            else
            {
                wTimeout = PHPAL_SLI15693_TIMEOUT_SHORT_US + PHPAL_SLI15693_SW_LOW_NRT_US;
            }
        }
    }

    /* Set  timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        wTimeout));

    /* Update the command offset variable. */
    bCmdOffset = bCmdBufferLen;

    /* If buffering is set. */
    if((pDataParams->bBuffering) != 0U)
    {
        /* Update the maximum number of blocks with respect to Option flag setting. The value for the blocks is fixed to 60 and 40 to avoid multiple
         * handling of different data in response. RD70x can respond with more amount of data but CM1 cannot. So fixing the blocks count to a lower
         * value.
         */
        wMaxNoBlocks = (uint8_t) (((bFlags & PHPAL_SLI15693_FLAG_OPTION) != 0U) ? 40U : 60U);

        /* Blocks to read. */
        wCurrBlocksToRead = wMaxNoBlocks;

        /* Update the number of blocks to read if its less than the internal required one. */
        if(wNoOfPages_Blocks < wMaxNoBlocks)
        {
            wCurrBlocksToRead = wNoOfPages_Blocks;
            bAllBlocksRead = 1U;
        }
    }
    else
    {
        wCurrBlocksToRead = wNoOfPages_Blocks;
        bAllBlocksRead = 1U;
    }


    if((((pDataParams->bFlags & PHPAL_SLI15693_FLAG_INVENTORY) == 0U) && ((pDataParams->bFlags & PHPAL_SLI15693_FLAG_ADDRESSED) != 0U)) != 0U)
    {
        pDataParams->bExplicitlyAddressed = 1U;
    }


    /* Set First variable. This variable will be used to validate the response only once in case if chaining is enabled. */
    bFirst = PH_ON;

    /* Exchange the information. */
    do
    {
        /* Page (or block) number and count */
        if (bCmd != PHPAL_SLI15693_SW_CMD_INVENTORY)
        {
         /* Adjust number of blocks. Adjustment is made because the User or the application will pass
         * the number of blocks starting from 1 to N. But as per Iso15693 specification the number
         * of blocks ranges from 0 - (N - 1).
         */
            --wCurrBlocksToRead;

            if (!((bExtended != 0U) && ((bExtendedOptions & PHPAL_SLI15693_FLAG_SKIP_DATA) != 0U)))
            {
                bCmdBuffer[bCmdOffset++] = (uint8_t) ((wCurrBlockNo + bPage_Block_No) & 0x00FFU);
                bCmdBuffer[bCmdOffset++] = (uint8_t) wCurrBlocksToRead;
            }
        }

        /* Exchange the command information to Hal layer. */
        status = phpalSli15693_Exchange(
            pDataParams,
            PH_EXCHANGE_DEFAULT,
            bCmdBuffer,
            bCmdOffset,
            &pResp,
            &wRespLength);

        /* Clear INVENTORY, AFI and NBSLOTS flag */
        if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            pDataParams->bFlags &= (uint8_t)~(uint8_t)(PHPAL_SLI15693_FLAG_INVENTORY | PHPAL_SLI15693_FLAG_AFI | PHPAL_SLI15693_FLAG_NBSLOTS);
        }

        /* Verify the exchange status. */
        PH_CHECK_SUCCESS(status);

        /* Extracting  the CID as sent by the VICC. */
        if ((bExtended != 0U) && ((bExtendedOptions & PHPAL_SLI15693_FLAG_CID_RESPONSE) != 0U))
        {
            (void)memcpy(pCIDOut, pResp, 2U);  /* PRQA S 3200 */

            /* Update the response pointer address and length to Skip CID. */
            pResp += 2U;
            wRespLength -= 2U;
        }

        /* Set the length (amount) of expected UID bytes. */
        if (bCmd == PHPAL_SLI15693_SW_CMD_INVENTORY)
        {
            /* The response length should be exactly the complete UID */
            if (wRespLength != (1U + PHPAL_SLI15693_UID_LENGTH))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
            }

            /* Copy the DSFID to the data buffer. */
            pData[0] = pResp[0];
            *pDataLength = 1U;

            /* Store the UID */
            (void)memcpy(pDataParams->pUid, &pResp[1], PHPAL_SLI15693_UID_LENGTH); /* PRQA S 3200 */
            pDataParams->bUidBitLength = PHPAL_SLI15693_SW_UID_COMPLETE;

            /* Return the UID */
            (void)memcpy(pUid, pDataParams->pUid, PHPAL_SLI15693_UID_LENGTH); /* PRQA S 3200 */
            *pUidLength = PHPAL_SLI15693_UID_LENGTH;
        }
        else
        {
            if ((bFlags & PHPAL_SLI15693_FLAG_OPTION) != 0U)
            {
                *pUidLength = PHPAL_SLI15693_UID_LENGTH - bStoredUidLength;

                /* The response length should not be less than the remaining UID. */
                if(bFirst != 0U)
                {
                    bStoredUidLength = pDataParams->bUidBitLength >> 3U;
                    *pUidLength = PHPAL_SLI15693_UID_LENGTH - bStoredUidLength;

                    /* Update the UID length if extended. */
                    if((bExtended != 0U) && ((bExtendedOptions & PHPAL_SLI15693_FLAG_UID_MODE) != 0U))
                    {
                        *pUidLength = PHPAL_SLI15693_UID_LENGTH;
                    }
                }
                /* Validate the response length against UID length. */
                if (wRespLength < (*pUidLength))
                {
                    return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
                }

                if (bStoredUidLength < PHPAL_SLI15693_UID_LENGTH)
                {
                    if ((*pUidLength) == 0U)
                    {
                        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_SLI15693);
                    }
                    /* Return the received (partial) UID */
                    (void)memcpy(pUid, pResp, (size_t)(*pUidLength)); /* PRQA S 3200 */

                    /* We need to merge the contents of the mask buffer and the received data */
                    if ((bMaskBitLength % 8U) != 0u)
                    {
                        if (bStoredUidLength < 7U)
                        {
                            /* copy the UID bytes we received from the card */
                            (void)memcpy(&(pDataParams->pUid[bStoredUidLength + 1U]), &pResp[1], (size_t)(*pUidLength)-1U); /* PRQA S 3200 */
                        }

                        /* merge mask-bits with received bits */
                        pDataParams->pUid[bStoredUidLength] |= pResp[0];
                    }
                    else
                    {
                        /* Copy the UID bytes we received from the card */
                        (void)memcpy(&(pDataParams->pUid[bStoredUidLength]), pResp, *pUidLength); /* PRQA S 3200 */
                    }
                }

                /* Update UID length. */
                pDataParams->bUidBitLength = PHPAL_SLI15693_SW_UID_COMPLETE;

                /* Shift pointer and length */
                pResp += *pUidLength;
                wRespLength = (uint16_t) (wRespLength - *pUidLength);
            }
            /* Copy the received data to internal buffer. */
            (void)memcpy(&pData[*pDataLength], pResp, wRespLength);         /* PRQA S 3200 */
            *pDataLength += wRespLength;
        }

        /* Update the variables to read the remaining data. */
        wCurrBlockNo += wMaxNoBlocks;

        /* Update the Current blocks to read. */
        wCurrBlocksToRead = wMaxNoBlocks;

        /* Reset the command buffer offset. */
        bCmdOffset = bCmdBufferLen;

        /* Set the remaining blocks to read. */
        if((wNoOfPages_Blocks - wCurrBlockNo) < wMaxNoBlocks)
        {
            wCurrBlocksToRead = (uint16_t) (wNoOfPages_Blocks - wCurrBlockNo);
        }

        /* Set the flag to finish the loop. */
        if((wNoOfPages_Blocks * 4U) == *pDataLength)
        {
            bAllBlocksRead = 1;
        }

        /* Set First variable. This variable will be used to validate the response only once in case if chaining is enabled. */
        bFirst = PH_OFF;

    }while(bAllBlocksRead == 0U);


    /* Clear INVENTORY, AFI and NBSLOTS flag */
    pDataParams->bFlags &= (uint8_t)~(uint8_t)(PHPAL_SLI15693_FLAG_INVENTORY | PHPAL_SLI15693_FLAG_AFI | PHPAL_SLI15693_FLAG_NBSLOTS);

    pDataParams->bExplicitlyAddressed = 0;

    /* Error check */
    PH_CHECK_SUCCESS(status);

    /* set addressed flag */
    pDataParams->bFlags |= PHPAL_SLI15693_FLAG_ADDRESSED;

    return PH_ERR_SUCCESS;
}


#endif /* NXPBUILD__PHPAL_SLI15693_SW */
