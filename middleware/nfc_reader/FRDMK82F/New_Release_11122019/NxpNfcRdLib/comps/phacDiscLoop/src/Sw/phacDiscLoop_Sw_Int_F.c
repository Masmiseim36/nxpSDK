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
* Discovery Loop Activities for Type F polling.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  PC: Generated 23. Aug 2012
*
*/

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <ph_RefDefs.h>
#include <phacDiscLoop.h>
#include <phpalI18092mPI.h>
#include <phpalFelica.h>
#include "phacDiscLoop_Sw.h"

#ifdef NXPBUILD__PHAC_DISCLOOP_SW
#include "phacDiscLoop_Sw_Int.h"
#include "phacDiscLoop_Sw_Int_F.h"


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS

/* Perform check whether same TypeF tag/device already detected with higher baud rate */
static phStatus_t phacDiscLoop_Sw_Int_CheckForDuplication(
                                                          phacDiscLoop_Sw_DataParams_t * pDataParams,
                                                          uint8_t *pID,
                                                          uint8_t *pIsDuplicate
                                                          )
{
    uint8_t PH_MEMLOC_COUNT bIndex = 0x00;

    *pIsDuplicate = PH_OFF;

    for(bIndex = 0; bIndex < pDataParams->sTypeFTargetInfo.bTotalTagsFound; bIndex++)
    {   /* Skip Identical Entry */
        if((memcmp(pID, pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm, PHAC_DISCLOOP_FELICA_IDM_LENGTH + PHAC_DISCLOOP_FELICA_PMM_LENGTH )) == 0)
        {
            *pIsDuplicate = PH_ON;
            /* Update baud rate if device is detected with higher baud rate */
            break;
        }
    }
    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS

/* Parse and update greedy collection with Responses */
static phStatus_t phacDiscLoop_Sw_Int_UpdateSensFResp(
                                                      phacDiscLoop_Sw_DataParams_t * pDataParams,
                                                      uint8_t *pID,
                                                      uint8_t bIdLen,
                                                      uint16_t wCurrentBaudBitPos
                                                      )
{
    phStatus_t PH_MEMLOC_REM   wStatus = PH_ERR_SUCCESS;
    uint8_t    PH_MEMLOC_REM   bIsDuplicate;
    uint8_t    PH_MEMLOC_REM   bTotalTagsFound = 0x00;
    uint16_t   PH_MEMLOC_REM   wRequestCode;

    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_Sw_Int_CheckForDuplication(pDataParams, &pID[0x00], &bIsDuplicate));
    if(0U == (bIsDuplicate))
    {
        /* get Request code is enable */
        PH_CHECK_SUCCESS_FCT(wStatus, phpalFelica_GetConfig(pDataParams->pPalFelicaDataParams, PHPAL_FELICA_CONFIG_RC, &wRequestCode));

        /* Copy ReqCode */
        if((wRequestCode != PH_OFF) && (bIdLen > (PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH)))
        {
            (void)memcpy((uint8_t *)&(pDataParams->sTypeFTargetInfo.aTypeFTag[pDataParams->sTypeFTargetInfo.bTotalTagsFound + bTotalTagsFound].aRD), &pID[PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH], PHPAL_FELICA_RD_LENGTH);
        }
        /* Copy IDMmPMm */
        (void)memcpy( (uint8_t *)&(pDataParams->sTypeFTargetInfo.aTypeFTag[pDataParams->sTypeFTargetInfo.bTotalTagsFound + bTotalTagsFound].aIDmPMm), pID, (PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH));
        pDataParams->sTypeFTargetInfo.aTypeFTag[pDataParams->sTypeFTargetInfo.bTotalTagsFound + bTotalTagsFound].bBaud = (uint8_t)(wCurrentBaudBitPos & 0xFFU);

        /* Increment Tag count */
        bTotalTagsFound++;
    }

    pDataParams->sTypeFTargetInfo.bTotalTagsFound += bTotalTagsFound;

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS

/* Send SENSF_REQ command and process received response */
static phStatus_t phacDiscLoop_Sw_Int_SendSensFReq(
                                                      phacDiscLoop_Sw_DataParams_t * pDataParams,
                                                      uint8_t bNumTimeSlots,
                                                      uint16_t wCurrentBaudBitPos
                                                      )
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_INTERNAL_ERROR;
    uint8_t    PH_MEMLOC_BUF baSystemCode[2] = {PHPAL_FELICA_SYSTEM_CODE_BYTE_0,
                                                PHPAL_FELICA_SYSTEM_CODE_BYTE_1};
    uint16_t   PH_MEMLOC_REM wTemp = 0U;
    uint8_t *  PH_MEMLOC_REM pId = NULL;
    uint8_t *  PH_MEMLOC_REM pRxBuff = NULL;
    uint8_t    PH_MEMLOC_REM bIdLen;
    phStatus_t PH_MEMLOC_REM wFrameStatus;

    /* send ReqC command */
    wStatus = phpalFelica_ReqC(pDataParams->pPalFelicaDataParams, baSystemCode, bNumTimeSlots, &pRxBuff, &wTemp);

    if((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        if(bNumTimeSlots > PHPAL_FELICA_NUMSLOTS_1)
        {
            /* get total number of response frame */
            PH_CHECK_SUCCESS_FCT(wStatus, phpalFelica_GetConfig(pDataParams->pPalFelicaDataParams, PH_PALFELICA_CONFIG_NUM_RESPONSE_FRAMES, &wTemp));

            while(0U != wTemp)
            {
                bIdLen = 0x00;
                wFrameStatus = 0x00;
                /* retrieve IDmPMm one by one for all frames */
                PH_CHECK_SUCCESS_FCT(wStatus, phpalFelica_GetFrameInfo(
                                                    pDataParams->pPalFelicaDataParams,
                                                    (uint8_t)wTemp,
                                                    pRxBuff,
                                                    &wFrameStatus,
                                                    &pId,
                                                    &bIdLen));

                if(wFrameStatus == PH_ERR_SUCCESS)
                {
                    /* store received IDmPMm */
                    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_Sw_Int_UpdateSensFResp(pDataParams, pId, bIdLen, wCurrentBaudBitPos));
                }
                wTemp--;

                /* Check for device limit */
                if(pDataParams->sTypeFTargetInfo.bTotalTagsFound >= pDataParams->baPasConDevLim[PHAC_DISCLOOP_TECH_TYPE_F212])
                {
                    break;
                }
            }
        }
        else
        {
            /* store received IDmPMm */
            PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_Sw_Int_UpdateSensFResp(pDataParams, pRxBuff, (uint8_t)wTemp, wCurrentBaudBitPos));
        }

        /*Response is success but no device found*/
        if(pDataParams->sTypeFTargetInfo.bTotalTagsFound == 0U)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_AC_DISCLOOP);
        }
        else
        {
            return PH_ERR_SUCCESS;
        }
    }

    return wStatus;
}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

phStatus_t phacDiscLoop_Sw_DetTechTypeF(
                                        phacDiscLoop_Sw_DataParams_t *pDataParams
                                        )
{
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_INTERNAL_ERROR;
    uint16_t   PH_MEMLOC_REM wCurrentBaudBitPos;

    PH_CHECK_SUCCESS_FCT(wStatus,phpalFelica_SetConfig(pDataParams->pPalFelicaDataParams, PHPAL_FELICA_CONFIG_RC, PH_OFF));

    /* get current FeliCa baud rate */
    PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEF_CUR_BAUD, &wCurrentBaudBitPos));

    /* if any tag has detected, do not reset the tag count */
    if(0U == (pDataParams->bDetectedTechs & (PHAC_DISCLOOP_POS_BIT_MASK_F424 | PHAC_DISCLOOP_POS_BIT_MASK_F212)))
    {
        pDataParams->sTypeFTargetInfo.bTotalTagsFound = 0;
    }

    /* re-set Collision Pending bit */
    pDataParams->bCollPend &= (uint8_t)~(uint8_t)(wCurrentBaudBitPos & 0xFFU);
    wStatus = phacDiscLoop_Sw_Int_SendSensFReq(pDataParams, PHPAL_FELICA_NUMSLOTS_4, wCurrentBaudBitPos);

    if(0u != (phacDiscLoop_Sw_Int_IsValidPollStatus(wStatus)))
    {
        if((wStatus & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR)
        {
            /* set Collision Pending bit */
            pDataParams->bCollPend |= (uint8_t)(wCurrentBaudBitPos & 0xFFU);
        }
    }
    else
    {
        return wStatus;
    }

    return PH_ADD_COMPCODE_FIXED(PHAC_DISCLOOP_TECH_DETECTED, PH_COMP_AC_DISCLOOP);

#else
    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AC_DISCLOOP);
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */
}

phStatus_t phacDiscLoop_Sw_Int_CollisionResolutionF(
                                                    phacDiscLoop_Sw_DataParams_t * pDataParams
                                                    )
{
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    phStatus_t PH_MEMLOC_REM   status;
    uint16_t   PH_MEMLOC_REM   wCurrentBaudBitPos;
    uint8_t    PH_MEMLOC_COUNT bIndex = 0;

    PH_CHECK_SUCCESS_FCT(status, phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEF_CUR_BAUD, &wCurrentBaudBitPos));

    /* Collision_Pending = 1 and Device limit  = 0 */
    if((0U != ((pDataParams->bCollPend & wCurrentBaudBitPos))) && ((pDataParams->baPasConDevLim[PHAC_DISCLOOP_TECH_TYPE_F212] == 0x00U)))
    {
        pDataParams->sTypeFTargetInfo.bTotalTagsFound = 0;
        return PH_ADD_COMPCODE_FIXED(PHAC_DISCLOOP_NO_DEVICE_RESOLVED, PH_COMP_AC_DISCLOOP);
    }

    /* Symbol 1 */
    /* Symbol 2*/
    /* Device limit for F424 and F212 is same */
    if(pDataParams->sTypeFTargetInfo.bTotalTagsFound >= pDataParams->baPasConDevLim[PHAC_DISCLOOP_TECH_TYPE_F212])
    {
        if (wCurrentBaudBitPos == PHAC_DISCLOOP_CON_BITR_212)
        {
            /* Configure HW for the TypeF technology */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_ApplyProtocolSettings(
                pDataParams->pHalDataParams,
                PHHAL_HW_CARDTYPE_FELICA_212));
        }
        else
        {
            /* Configure HW for the TypeF technology */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_ApplyProtocolSettings(
                pDataParams->pHalDataParams,
                PHHAL_HW_CARDTYPE_FELICA_424));
        }

        /* Symbol 8 */
        pDataParams->bNumOfCards = pDataParams->sTypeFTargetInfo.bTotalTagsFound;
        return PH_ERR_SUCCESS;
    }

    /* Re-set Collision Pending bit */
    pDataParams->bCollPend &= (uint8_t)~(uint8_t)(wCurrentBaudBitPos & 0xFFU);

    /* Symbol 3 */
    /* SENSF_REQ with TSN := 0Fh, RC := 00h, SC := FFFFh */
    status = phpalFelica_SetConfig(pDataParams->pPalFelicaDataParams, PHPAL_FELICA_CONFIG_RC, 0);
    PH_CHECK_SUCCESS(status);

    /* Apply Guard time.(GTFB) */
    PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_POLL_GUARD_TIME_US,
        pDataParams->waPasPollGTimeUs[PHAC_DISCLOOP_TECH_TYPE_F212]));

    if (wCurrentBaudBitPos == PHAC_DISCLOOP_CON_BITR_212)
    {
        /* Configure HW for the TypeF technology */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_ApplyProtocolSettings(
            pDataParams->pHalDataParams,
            PHHAL_HW_CARDTYPE_FELICA_212));
    }
    else
    {
        /* Configure HW for the TypeF technology */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_ApplyProtocolSettings(
            pDataParams->pHalDataParams,
            PHHAL_HW_CARDTYPE_FELICA_424));
    }

    status = phacDiscLoop_Sw_Int_SendSensFReq(pDataParams, PHPAL_FELICA_NUMSLOTS_16, wCurrentBaudBitPos);
    PH_CHECK_SUCCESS(status);

    /* Symbol 5 */
    /* Check for NFC-DEP Protocol presence within all received responses */
    bIndex = 0x00;
    while(bIndex < pDataParams->sTypeFTargetInfo.bTotalTagsFound)
    {
        if ((pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[0] == 0x01U) &&
            (pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[1] == 0xFEU))
        {
            /* Symbol 6 */
            /* SENSF_REQ with TSN := 0Fh, RC := 01h, SC := FFFFh */
            status = phpalFelica_SetConfig(pDataParams->pPalFelicaDataParams, PHPAL_FELICA_CONFIG_RC, 0x01);
            PH_CHECK_SUCCESS(status);

            status = phacDiscLoop_Sw_Int_SendSensFReq(pDataParams, PHPAL_FELICA_NUMSLOTS_16, wCurrentBaudBitPos);
            /* Symbol 7 */
            /* The NFC Forum Device SHALL check for any Valid SENSF_RES Response(s) received during processing of Symbol 4
             * whether an identical entry already exists in GRE_SENSF_RES[], and if not, store the SENSF_RES in GRE_SENSF_RES[]
             */
         }
        bIndex++;
    } /* while(bIndex < pDataParams->sTypeFTargetInfo.bTotalTagsFound) */

    pDataParams->bNumOfCards = pDataParams->sTypeFTargetInfo.bTotalTagsFound;
    return PH_ERR_SUCCESS;
#else
    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AC_DISCLOOP);
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */
}

phStatus_t phacDiscLoop_Sw_Int_ActivateF(
                                         phacDiscLoop_Sw_DataParams_t * pDataParams,
                                         uint8_t  bTypeFTagIdx
                                         )
{
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    uint8_t    PH_MEMLOC_REM bIsDuplicate = 0x00;
    uint8_t    PH_MEMLOC_BUF baSystemCode[2] = {PHPAL_FELICA_SYSTEM_CODE_BYTE_0,
                                                PHPAL_FELICA_SYSTEM_CODE_BYTE_1};
    uint8_t *  PH_MEMLOC_REM pRxBuff = NULL;
    uint8_t *  PH_MEMLOC_REM pId = NULL;
    uint16_t   PH_MEMLOC_REM wLen;
    uint16_t   PH_MEMLOC_REM wFrames;
    uint8_t    PH_MEMLOC_REM IdLen;
    uint16_t   PH_MEMLOC_REM wFrameStatus;
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS
    uint8_t    PH_MEMLOC_BUF aNfcId3[10];
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS */

    if ((bTypeFTagIdx >= pDataParams->sTypeFTargetInfo.bTotalTagsFound) || ( bTypeFTagIdx >= PHAC_DISCLOOP_CFG_MAX_CARDS_SUPPORTED ))
    {
        /* Out of range or no such card found yet */
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AC_DISCLOOP);
    }

    /* set given card ID to FeliCa data param */
    PH_CHECK_SUCCESS_FCT(status, phpalFelica_SetSerialNo(
                                                         pDataParams->pPalFelicaDataParams,
                                                         pDataParams->sTypeFTargetInfo.aTypeFTag[bTypeFTagIdx].aIDmPMm)
                                                        );
    /* Symbol 0 */
    if ((pDataParams->sTypeFTargetInfo.aTypeFTag[bTypeFTagIdx].aIDmPMm[0] == 0x01U) &&
        (pDataParams->sTypeFTargetInfo.aTypeFTag[bTypeFTagIdx].aIDmPMm[1] == 0xFEU))
    {
        /* Single device is detected. Activation is already done during collision resolution */
        /* Symbol 1 */
        if(pDataParams->sTypeFTargetInfo.bTotalTagsFound != 1U)
        {
            /* Symbol 2 */
            /* SENSF_REQ with TSN := 0Fh, RC := 00h, SC := FFFFh */
            status = phpalFelica_SetConfig(pDataParams->pPalFelicaDataParams, PHPAL_FELICA_CONFIG_RC, 0);
            PH_CHECK_SUCCESS(status);

            status = phpalFelica_ReqC(pDataParams->pPalFelicaDataParams, baSystemCode, PHPAL_FELICA_NUMSLOTS_16, &pRxBuff, &wLen);
            PH_CHECK_SUCCESS(status);

            PH_CHECK_SUCCESS_FCT(status, phpalFelica_GetConfig(pDataParams->pPalFelicaDataParams, PH_PALFELICA_CONFIG_NUM_RESPONSE_FRAMES, &wFrames));

                while((0U != wFrames) && (0U == bIsDuplicate))
                {
                    IdLen = 0x00;
                    wFrameStatus = 0x00;

                    PH_CHECK_SUCCESS_FCT(status, phpalFelica_GetFrameInfo(
                        pDataParams->pPalFelicaDataParams,
                        (uint8_t)wFrames,
                        pRxBuff,
                        &wFrameStatus,
                        &pId,
                        &IdLen));

                    /* Check Device ID into greedy collection  */
                    /* Check Same response already present or not */
                    PH_CHECK_SUCCESS_FCT(status, phacDiscLoop_Sw_Int_CheckForDuplication(
                        pDataParams,
                        pId,
                        &bIsDuplicate));

                    wFrames--;
                }

            /* If same response is not present means Device not present
             * or some other error has occurred
             * Report DiscLoop Failure in this case */
            if(0U == (bIsDuplicate))
            {
                return PH_ADD_COMPCODE_FIXED(PHAC_DISCLOOP_FAILURE, PH_COMP_AC_DISCLOOP);
            }
        }
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS
        /* Symbol 4 */
        (void)memcpy(aNfcId3, pDataParams->sTypeFTargetInfo.aTypeFTag[bTypeFTagIdx].aIDmPMm, PHAC_DISCLOOP_FELICA_IDM_LENGTH);

        aNfcId3[8] = 0;
        aNfcId3[9] = 0;

        /* Activate the P2P capable FeliCa card */
        PH_CHECK_SUCCESS_FCT(status, phpalI18092mPI_Atr(
            pDataParams->pPal18092mPIDataParams,
            aNfcId3,
            pDataParams->sTypeFTargetInfo.sTypeF_P2P.bDid,
            pDataParams->sTypeFTargetInfo.sTypeF_P2P.bLri,
            pDataParams->sTypeFTargetInfo.sTypeF_P2P.bNadEnable,
            pDataParams->sTypeFTargetInfo.sTypeF_P2P.bNad,
            pDataParams->sTypeFTargetInfo.sTypeF_P2P.pGi,
            pDataParams->sTypeFTargetInfo.sTypeF_P2P.bGiLength,
            pDataParams->sTypeFTargetInfo.sTypeF_P2P.pAtrRes,
            &(pDataParams->sTypeFTargetInfo.sTypeF_P2P.bAtrResLength)));

        return PH_ADD_COMPCODE_FIXED(PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED, PH_COMP_AC_DISCLOOP);
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS */
    }
    return PH_ERR_SUCCESS;
#else
    return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AC_DISCLOOP);
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */
}

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
phStatus_t phacDiscLoop_Sw_Int_Config_GTF(
                                          phacDiscLoop_Sw_DataParams_t * pDataParams,
                                          uint8_t bTechIndex
                                          )
{
    phStatus_t PH_MEMLOC_REM wStatus = PH_ERR_INTERNAL_ERROR;

    /*
     * If either one of FeliCa 212 or FeliCa 424 is polled after Type B polling,
     * then GTbf is applied. Default value is 15.3 ms (PH_NXPNFCRDLIB_CONFIG_B_TO_F_GT).
     * In this case Type B poll is followed by FeliCa poll.
     */
    if((0U != ((pDataParams->bPasPollTechCfg & PHAC_DISCLOOP_POS_BIT_MASK_B))) &&
        ((pDataParams->bFelicaBaud == PHAC_DISCLOOP_CON_BITR_212) ||
        ((pDataParams->bFelicaBaud == PHAC_DISCLOOP_CON_BITR_424) &&
        (0U == ((pDataParams->bPasPollTechCfg & PHAC_DISCLOOP_POS_BIT_MASK_F212))))))
    {
        /* apply guard time GTBF */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_POLL_GUARD_TIME_US,
            pDataParams->waPasPollGTimeUs[PHAC_DISCLOOP_TECH_TYPE_F424]));
    }
    else
    {
        /*
         * If Type F polling is not preceded by a Type B poll apply guard time GTFB
         * Default value of GTFB is 20.4ms (PH_NXPNFCRDLIB_CONFIG_TYPEF_GT).
         *
         */
        PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_POLL_GUARD_TIME_US,
            pDataParams->waPasPollGTimeUs[PHAC_DISCLOOP_TECH_TYPE_F212]));
    }

    if ((uint8_t)PHAC_DISCLOOP_TECH_TYPE_F424 == bTechIndex)
    {
        PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEF_CUR_BAUD, PHAC_DISCLOOP_CON_BITR_424));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(wStatus, phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEF_CUR_BAUD, PHAC_DISCLOOP_CON_BITR_212));
    }

    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS */

#endif /* NXPBUILD__PHAC_DISCLOOP_SW */
