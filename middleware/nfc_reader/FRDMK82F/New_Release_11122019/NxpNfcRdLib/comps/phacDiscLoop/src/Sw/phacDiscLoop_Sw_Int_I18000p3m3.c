/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2014
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/** \file
* Discovery Loop Activities for ISO 18000p3m3 polling.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  PC: Generated 23. Aug 2012
*
*/

/* *****************************************************************************
 * Includes
 * ****************************************************************************/
#include <ph_RefDefs.h>
#include <phacDiscLoop.h>
#include <phpalI18000p3m3.h>
#include <phalI18000p3m3.h>

#ifdef NXPBUILD__PHAC_DISCLOOP_SW
#include "phacDiscLoop_Sw_Int.h"
#include "phacDiscLoop_Sw_Int_I18000p3m3.h"

/* *****************************************************************************
 * Private Functions
 * ****************************************************************************/
phStatus_t phacDiscLoop_Sw_DetTechTypeI18000p3m3(
                                                 phacDiscLoop_Sw_DataParams_t *pDataParams
                                                 )
{
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_BUF aSelectCmd[39] = {0};
    uint8_t    PH_MEMLOC_REM wSelectCmdLen;
    uint8_t    PH_MEMLOC_REM bSelectCmdValidBits;
    uint8_t    PH_MEMLOC_BUF aBeginRoundCmd[3] = {0};
    uint8_t    PH_MEMLOC_REM bEmptyPointer = 0;
    uint8_t    PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wRxBufferLen = 0;

    /* Create select command frame */
    PH_CHECK_SUCCESS_FCT(status, phpalI18000p3m3_CreateSelectCmd(
        pDataParams->pPal18000p3m3DataParams,
        PHPAL_I18000P3M3_TARGET_INVENTORIED_S0,
        0,
        PHPAL_I18000P3M3_MEMBANK_UII,
        &bEmptyPointer,
        0,
        NULL,
        0,
        0,
        aSelectCmd,
        &wSelectCmdLen,
        &bSelectCmdValidBits));

    /* Create BeginRound command frame */
    PH_CHECK_SUCCESS_FCT(status, phpalI18000p3m3_CreateBeginRoundCmd(
        pDataParams->pPal18000p3m3DataParams,
        pDataParams->sI18000p3m3TargetInfo.bDr,
        pDataParams->sI18000p3m3TargetInfo.bM,
        PH_OFF,
        PHPAL_I18000P3M3_SEL_ALL_00,
        PHPAL_I18000P3M3_SESSION_S0,
        0,
        0,
        aBeginRoundCmd));

    /* Inventory with 1 slot */
    status = phhalHw_I18000p3m3Inventory(
        pDataParams->pHalDataParams,
        aSelectCmd,
        (uint8_t)wSelectCmdLen,
        bSelectCmdValidBits,
        aBeginRoundCmd,
        PHHAL_HW_I18000P3M3_ONE_TS_ONLY,
        &pRxBuffer,
        &wRxBufferLen);

    if(0u != (phacDiscLoop_Sw_Int_IsValidPollStatus(status)))
    {
        return PH_ADD_COMPCODE_FIXED(PHAC_DISCLOOP_TECH_DETECTED, PH_COMP_AC_DISCLOOP);
    }
    else
    {
        return status;
    }
#else
    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AC_DISCLOOP);
#endif /* NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS */
}

phStatus_t phacDiscLoop_Sw_Int_CollisionResolutionI18000p3m3(
                                                             phacDiscLoop_Sw_DataParams_t *pDataParams
                                                             )
{
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_COUNT bQ = 4;
    uint8_t    PH_MEMLOC_BUF aSelectCmd[39] = {0};
    uint8_t   PH_MEMLOC_REM wSelectCmdLen;
    uint8_t    PH_MEMLOC_REM bSelectCmdValidBits;
    uint8_t    PH_MEMLOC_BUF aBeginRoundCmd[3] = {0};
    uint8_t    PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wRxBufferLen = 0;
    uint8_t    PH_MEMLOC_REM bEmptyPointer = 0;
    uint8_t    PH_MEMLOC_REM bMoreCardsPresent;
    uint16_t   PH_MEMLOC_REM wIndex;
    uint8_t    PH_MEMLOC_REM bPcLength;
    uint8_t    PH_MEMLOC_REM bPacketCrcLen;

    wRxBufferLen = 0;
    /* Reset collision pending flag */
    pDataParams->bCollPend &= (uint8_t)~(uint8_t)(PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3);

    /* Reset detected tag count */
    pDataParams->sI18000p3m3TargetInfo.bTotalTagsFound = 0;
    pDataParams->bNumOfCards = 0;

    /* Reset more card present flag */
    bMoreCardsPresent = PH_OFF;

    /* Apply Guard time. */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_POLL_GUARD_TIME_US,
        pDataParams->waPasPollGTimeUs[PHAC_DISCLOOP_TECH_TYPE_18000P3M3]));

    /* Configure HW for the Type 18000p3m3 technology */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_ApplyProtocolSettings(
        pDataParams->pHalDataParams,
        PHHAL_HW_CARDTYPE_I18000P3M3));

    /* Reset detected tag count */
    pDataParams->sI18000p3m3TargetInfo.bTotalTagsFound = 0;
    pDataParams->bNumOfCards = 0;

    /* Create select command frame */
    PH_CHECK_SUCCESS_FCT(status, phpalI18000p3m3_CreateSelectCmd(
        pDataParams->pPal18000p3m3DataParams,
        PHPAL_I18000P3M3_TARGET_INVENTORIED_S0,
        0,
        PHPAL_I18000P3M3_MEMBANK_UII,
        &bEmptyPointer,
        0,
        NULL,
        0,
        0,
        aSelectCmd,
        &wSelectCmdLen,
        &bSelectCmdValidBits));

    do
    {
        if((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            /* Resume Inventory */
            status = phhalHw_I18000p3m3ResumeInventory(
                pDataParams->pHalDataParams,
                &pRxBuffer,
                &wRxBufferLen);
            /* Return if status is aborted. */
            PH_CHECK_ABORT(status);
        }
        else
        {
            /* Create BeginRound command frame with Q value */
            PH_CHECK_SUCCESS_FCT(status, phpalI18000p3m3_CreateBeginRoundCmd(
                pDataParams->pPal18000p3m3DataParams,
                pDataParams->sI18000p3m3TargetInfo.bDr,
                pDataParams->sI18000p3m3TargetInfo.bM,
                PH_OFF,
                PHPAL_I18000P3M3_SEL_ALL_00,
                PHPAL_I18000P3M3_SESSION_S0,
                0,
                bQ++,
                aBeginRoundCmd));

            /* Inventory */
            status = phhalHw_I18000p3m3Inventory(
                pDataParams->pHalDataParams,
                aSelectCmd,
                (uint8_t)wSelectCmdLen,
                bSelectCmdValidBits,
                aBeginRoundCmd,
                PHHAL_HW_I18000P3M3_GET_MAX_RESPS,
                &pRxBuffer,
                &wRxBufferLen);
            /* Return if status is aborted. */
            PH_CHECK_ABORT(status);

            /* Reset more card present flag */
            bMoreCardsPresent = PH_OFF;

            /* Reset collision pending flag */
            pDataParams->bCollPend &= (uint8_t)~(uint8_t)(PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3);
        }

        /* Reset receive buffer index */
        wIndex = 0;

        while(wIndex < wRxBufferLen)
        {
            if(pRxBuffer[wIndex] == 0U)
            {
                /* Partial bits shall be 0 as truncation is not set */
                if(pRxBuffer[wIndex + 2U] != 0U)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_AC_DISCLOOP);
                }

                /* StoredPC/PacketPC length */
                bPcLength = 2;

                /* PacketCRC length */
                bPacketCrcLen = 0;

                /* Check for XPC Indicator (XI) */
                if(0u != (pRxBuffer[wIndex + 3U] & 0x02U))
                {
                    /* XPC_W1 length */
                    bPcLength = (uint8_t)(bPcLength + 2U);

                    /* PacketCRC length */
                    bPacketCrcLen = 2;

                    /* Check for XPC Extension Bit (XEB) */
                    if(0u != (pRxBuffer[wIndex + 5U] & 0x80U))
                    {
                        /* XPC_W2 length */
                        bPcLength = (uint8_t)(bPcLength + 2U);
                    }
                }

                if(pRxBuffer[wIndex + 1U] > (bPcLength + bPacketCrcLen))
                {
                    /* To avoid case of corruption */
                    if((pRxBuffer[wIndex + 1U] - (bPcLength + bPacketCrcLen)) <= (uint8_t)sizeof(pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[pDataParams->bNumOfCards].aUii))
                    {
                        /* Copy UII */
                        (void)memcpy(
                            pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[pDataParams->bNumOfCards].aUii,
                            &pRxBuffer[wIndex + 3U + bPcLength],
                            ((size_t)pRxBuffer[wIndex + 1U] - (bPcLength + bPacketCrcLen)));



                        /* Calculate UII length in bits */
                        pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[pDataParams->bNumOfCards].wUiiLength =
                        		(uint16_t)(((uint16_t)pRxBuffer[wIndex + 1U] - ((uint16_t)bPcLength + (uint16_t)bPacketCrcLen)) * 8U);
                    }
                    else
                    {
                        /* In case of a compliant card this scenario should never come */
                        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_AC_DISCLOOP);
                    }
                }
                else
                {
                    pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[pDataParams->bNumOfCards].wUiiLength = 0;
                }

                /* Increment device count */
                pDataParams->sI18000p3m3TargetInfo.bTotalTagsFound++;
                pDataParams->bNumOfCards++;

                if(pDataParams->sI18000p3m3TargetInfo.bTotalTagsFound >=
                    pDataParams->baPasConDevLim[PHAC_DISCLOOP_TECH_TYPE_18000P3M3])
                {
                    return PH_ERR_SUCCESS;
                }

                wIndex =  (uint16_t)(wIndex + pRxBuffer[wIndex + 1U] + 3U);
            }
            else if(pRxBuffer[wIndex] == 1U)
            {
                /* This should not come as ReqRN is not send */
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_AC_DISCLOOP);
            }
            else if(pRxBuffer[wIndex] == 2U)
            {
                /* Timeout */
                wIndex = (uint16_t)(wIndex + 3U);
            }
            else
            {
                /* Collision */
                wIndex = (uint16_t)(wIndex + 3U);
                bMoreCardsPresent = PH_ON;
                pDataParams->bCollPend |= PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3;

                /* Check for device count */
                if(pDataParams->baPasConDevLim[PHAC_DISCLOOP_TECH_TYPE_18000P3M3] == 0x00U)
                {
                    /* return collision error */
                    return PH_ADD_COMPCODE_FIXED(PHAC_DISCLOOP_COLLISION_PENDING, PH_COMP_AC_DISCLOOP);
                }
            }
        }
    }
    while(((bMoreCardsPresent != PH_OFF) && (bQ <= 0x0FU)) || ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING));

    if(0U != (pDataParams->sI18000p3m3TargetInfo.bTotalTagsFound))
    {
        return PH_ERR_SUCCESS;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PHAC_DISCLOOP_NO_DEVICE_RESOLVED, PH_COMP_AC_DISCLOOP);
    }
#else
    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AC_DISCLOOP);
#endif /* NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS */
}

phStatus_t phacDiscLoop_Sw_Int_ActivateI18000p3m3(
                                                  phacDiscLoop_Sw_DataParams_t * pDataParams,
                                                  uint8_t bI18000p3m3TagIdx
                                                  )
{
#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_BUF aSelectCmd[39] = {0};
    uint8_t   PH_MEMLOC_REM wSelectCmdLen;
    uint8_t    PH_MEMLOC_REM bSelectCmdValidBits;
    uint8_t    PH_MEMLOC_BUF aBeginRoundCmd[3] = {0};
    uint8_t    PH_MEMLOC_REM *pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wRxBufferLen = 0;
    uint8_t    PH_MEMLOC_REM bUiiStartAddress = 0x20;
    uint8_t    PH_MEMLOC_REM bMaskLength;

    /* Out of range check */
    if ((bI18000p3m3TagIdx >= pDataParams->sI18000p3m3TargetInfo.bTotalTagsFound) || ( bI18000p3m3TagIdx >= PHAC_DISCLOOP_CFG_MAX_VICINITY_CARDS_SUPPORTED ))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AC_DISCLOOP);
    }

    /* Maximum mask length possible is 255 */
    if(pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bI18000p3m3TagIdx].wUiiLength > 255U)
    {
        bMaskLength = 255;
    }
    else
    {
        bMaskLength = (uint8_t)pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bI18000p3m3TagIdx].wUiiLength;
    }

    /* Create select command frame (with selected UII in mask) */
    PH_CHECK_SUCCESS_FCT(status, phpalI18000p3m3_CreateSelectCmd(
        pDataParams->pPal18000p3m3DataParams,
        PHPAL_I18000P3M3_TARGET_INVENTORIED_S0,
        0,
        PHPAL_I18000P3M3_MEMBANK_UII,
        &bUiiStartAddress,
        0,
        pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bI18000p3m3TagIdx].aUii,
        bMaskLength,
        0,
        aSelectCmd,
        &wSelectCmdLen,
        &bSelectCmdValidBits));

    /* Create BeginRound command frame */
    PH_CHECK_SUCCESS_FCT(status, phpalI18000p3m3_CreateBeginRoundCmd(
        pDataParams->pPal18000p3m3DataParams,
        pDataParams->sI18000p3m3TargetInfo.bDr,
        pDataParams->sI18000p3m3TargetInfo.bM,
        PH_OFF,
        PHPAL_I18000P3M3_SEL_ALL_00,
        PHPAL_I18000P3M3_SESSION_S0,
        0,
        0,
        aBeginRoundCmd));

    /* Activate tag with specified UII */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_I18000p3m3Inventory(
        pDataParams->pHalDataParams,
        aSelectCmd,
        (uint8_t)wSelectCmdLen,
        bSelectCmdValidBits,
        aBeginRoundCmd,
        PHHAL_HW_I18000P3M3_GET_TAG_HANDLE,
        &pRxBuffer,
        &wRxBufferLen));

    /* Update handle */
    PH_CHECK_SUCCESS_FCT(status, phalI18000p3m3_SetHandle(
        pDataParams->pAl18000p3m3DataParams,
        &pRxBuffer[wRxBufferLen - 2u]));

    return PH_ERR_SUCCESS;
#else
    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AC_DISCLOOP);
#endif /* NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS */
}
#endif /* NXPBUILD__PHAC_DISCLOOP_SW */
