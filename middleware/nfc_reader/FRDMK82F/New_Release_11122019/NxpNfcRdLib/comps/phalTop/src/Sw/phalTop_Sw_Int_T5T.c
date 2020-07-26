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
 * Internal functions for Tag Operation Application Layer Component of
 * Reader Library Framework.
 * $Author$
 * $Revision$ (v06.01.00)
 * $Date$
 */

#include <ph_TypeDefs.h>
#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#include <phalICode.h>
#include <phpalSli15693.h>
#include <phalTop.h>

#ifdef NXPBUILD__PHAL_TOP_SW

#ifdef NXPBUILD__PHAL_ICODE_SW

#include "phalTop_Sw_Int_T5T.h"

static void phMemcpy(void* dest, void* src, uint16_t wLength);

phStatus_t phalTop_Sw_Int_LockBlock(
                                    phalTop_T5T_t * pT5T,
                                    uint16_t wBlockNum
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    if ( (pT5T->bExtendedCommandSupport == 0U ) && (wBlockNum > PHAL_TOP_T5T_DEFAULT_BLOCK_NUMBER))
    {
       return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_TOP);
    }

    if(wBlockNum <= PHAL_TOP_T5T_DEFAULT_BLOCK_NUMBER)
    {
        PH_CHECK_SUCCESS_FCT(status, phalICode_LockBlock(
            pT5T->pAlI15693DataParams,
            PHPAL_SLI15693_ACTIVATE_DEFAULT,
            (uint8_t)wBlockNum));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(status, phalICode_ExtendedLockBlock(
            pT5T->pAlI15693DataParams,
            PHPAL_SLI15693_ACTIVATE_DEFAULT,
            wBlockNum));
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T5T_Read(
                                   phalTop_T5T_t * pT5T,
                                   uint16_t wBlockNum,
                                   uint16_t wNumberOfBlocks,
                                   uint8_t ** pRxBuffer,
                                   uint16_t * wLength
                                   )
{
    phStatus_t PH_MEMLOC_REM status;
    uint8_t    PH_MEMLOC_REM bOptionFlag = PHAL_ICODE_OPTION_OFF;
    uint8_t    PH_MEMLOC_REM   * pRxBufferTmp1 = NULL;
    uint8_t    PH_MEMLOC_REM   * pRxBufferTmp2 = NULL;
    uint16_t   PH_MEMLOC_COUNT wIndex;

    if(pT5T->bSplFrm == 1U)
    {
        bOptionFlag = PHAL_ICODE_OPTION_ON;
    }

    if(wNumberOfBlocks == PHAL_TOP_T5T_MIMIMUM_NO_OF_BLOCK)
    {
        if(wBlockNum <= PHAL_TOP_T5T_DEFAULT_BLOCK_NUMBER)
        {
            PH_CHECK_SUCCESS_FCT(status, phalICode_ReadSingleBlock(
                pT5T->pAlI15693DataParams,
                bOptionFlag,
                (uint8_t)(wBlockNum),
                pRxBuffer,
                wLength));

        }
        else
        {
            if(0U != (pT5T->bExtendedCommandSupport))
            {
                PH_CHECK_SUCCESS_FCT(status, phalICode_ExtendedReadSingleBlock(
                    pT5T->pAlI15693DataParams,
                    bOptionFlag,
                    wBlockNum,
                    pRxBuffer,
                    wLength));
            }
            else
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
            }
        }

        /* Check if Special Frame of Tag and the Option Flag during Transmit is
         * enabled. Then Discard the Block security Status Byte */
        if ((pT5T->bSplFrm == 1U) && (bOptionFlag == PH_ON ))
        {
            (*pRxBuffer)++;
            *wLength = *wLength -1u;
        }
    }
    else if(pT5T->bMbRead == 1U)
    {
        if(wBlockNum+wNumberOfBlocks <= PHAL_TOP_T5T_DEFAULT_BLOCK_NUMBER)
        {
            PH_CHECK_SUCCESS_FCT(status, phalICode_ReadMultipleBlocks(
                pT5T->pAlI15693DataParams,
                bOptionFlag,
                (uint8_t)(wBlockNum),
                (uint8_t)wNumberOfBlocks,
                *pRxBuffer,
                wLength));
        }
        else
        {
            if(0U != (pT5T->bExtendedCommandSupport))
            {
                PH_CHECK_SUCCESS_FCT(status, phalICode_ExtendedReadMultipleBlocks(
                    pT5T->pAlI15693DataParams,
                    bOptionFlag,
                    wBlockNum,
                    wNumberOfBlocks,
                    *pRxBuffer,
                    wLength));
            }
            else
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
            }
        }

        /*Check if Special Frame of Tag and the Option Flag during Transmit is enabled. Then discard the Block security Status Bytes depending on the number of blocks*/
        if ((pT5T->bSplFrm == 1U) && (bOptionFlag == PH_ON ))
        {
            pRxBufferTmp1 = (*pRxBuffer);
            pRxBufferTmp2 = (*pRxBuffer) + 1;

            for(wIndex = 0; wIndex <= wNumberOfBlocks; wIndex++)
            {
                 memcpy(pRxBufferTmp1, pRxBufferTmp2, pT5T->bBlockSize);
                 pRxBufferTmp1 += pT5T->bBlockSize;
                 pRxBufferTmp2 += pT5T->bBlockSize + 1;
            }

            *wLength = (*wLength - wNumberOfBlocks);
        }
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_TOP);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T5T_Write(
                                    phalTop_T5T_t * pT5T,
                                    uint16_t wBlockNum,
                                    uint8_t * pRxBuffer,
                                    uint16_t wLength
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    if(wBlockNum <= PHAL_TOP_T5T_DEFAULT_BLOCK_NUMBER)
    {
        PH_CHECK_SUCCESS_FCT(status, phalICode_WriteSingleBlock(
            pT5T->pAlI15693DataParams,
            PHPAL_SLI15693_ACTIVATE_DEFAULT,
            (uint8_t)(wBlockNum),
            pRxBuffer,
            (uint8_t)wLength));
    }
    else
    {
        if(0U != (pT5T->bExtendedCommandSupport))
        {
            PH_CHECK_SUCCESS_FCT(status, phalICode_ExtendedWriteSingleBlock(
                pT5T->pAlI15693DataParams,
                PHPAL_SLI15693_ACTIVATE_DEFAULT,
                wBlockNum,
                pRxBuffer,
                (uint8_t)wLength));
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T5T_ClearState(
                                         phalTop_Sw_DataParams_t * pDataParams,
                                         phalTop_T5T_t * pT5T
                                         )
{
    /* Reset parameters */
    pDataParams->bTagState = 0;
    pT5T->bTerminatorTlvPresence = 0;
    pDataParams->wMaxNdefLength = 0;
    pT5T->wNdefHeaderAddr = 0;
    pDataParams->wNdefLength = 0;
    pT5T->wNdefMsgAddr = 0;
    pT5T->bBlockSize = 0;
    pT5T->bExtendedCommandSupport = 0;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T5T_SetReadOnly(
                                          phalTop_Sw_DataParams_t * pDataParams
                                          )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint8_t       PH_MEMLOC_BUF   aData[PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE] = {0};
    uint8_t       PH_MEMLOC_REM   * pRxBuffer = NULL;
    uint16_t      PH_MEMLOC_REM   wRxLength = 0;
    uint16_t      PH_MEMLOC_COUNT wIndex;
    phalTop_T5T_t PH_MEMLOC_REM * pT5T = &pDataParams->ualTop.salTop_T5T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT5T->pAlI15693DataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Check if tag is in valid state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* Check for NDEF length > 0 (in initialized state NDEF length is 0) */
    if(pDataParams->bTagState == PHAL_TOP_STATE_INITIALIZED)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_EMPTY_NDEF, PH_COMP_AL_TOP);
    }

    /* Check if tag is already in read-only state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_READONLY)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_READONLY_TAG, PH_COMP_AL_TOP);
    }

    /* Read CC */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
        pT5T,
        PHAL_TOP_T5T_CC_BLOCK,
        1,
        &pRxBuffer,
        &wRxLength));

    if(wRxLength != 0U)
    {
        (void)memcpy(aData, pRxBuffer, wRxLength);
    }

    /* Update R/W access byte of CC to RO */
    aData[1] |= PHAL_TOP_T5T_READ_WRITE_ACCESS_BYTE;

    /* Write back CC */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
        pT5T,
        PHAL_TOP_T5T_CC_BLOCK,
        aData,
        wRxLength));

    if(pT5T->bLockBlock == 1U)
    {
        for(wIndex = 0; wIndex < pT5T->bMaxBlockNum; wIndex++)
        {
            PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_LockBlock(pT5T, wIndex));
        }
    }

    /* Update state */
    pT5T->bRwa = PHAL_TOP_T5T_CC_RWA_RO;
    pDataParams->bTagState = PHAL_TOP_STATE_READONLY;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T5T_DetectTlvBlocks(
                                              phalTop_Sw_DataParams_t * pDataParams,
                                              phalTop_T5T_t * pT5T
                                              )
{
    phStatus_t PH_MEMLOC_REM   status;
    uint8_t    PH_MEMLOC_REM   bIndexInit = 0;
    uint8_t    PH_MEMLOC_REM   aLength[3] = {0};
    uint8_t    PH_MEMLOC_REM   bExitLoop = 0;
    uint8_t    PH_MEMLOC_REM   * pRxBuffer = NULL;
    uint8_t    PH_MEMLOC_BUF   aData[40] = { 0 };
    uint16_t   PH_MEMLOC_REM   wRxLength = 0;
    uint16_t   PH_MEMLOC_REM   wBlockNum = 0;
    uint16_t   PH_MEMLOC_COUNT wIndex = 0;

    /* Assigning the address of data buffer to the rxBuffer */
    pRxBuffer = &aData[0];

    /* To calculate whether CC is spread across 4 bytes or 8 bytes */
    if ((pT5T->wMlen) > 0xFFU)
    {
        bIndexInit = 8;
    }
    else
    {
        bIndexInit = 4;
    }
    wBlockNum = 0;
    /* Read the whole memory of the tag and search for all the TLVs */
    while(wBlockNum <= pT5T->bMaxBlockNum)

    {
        /* Check if Multiple Blocks need to be read 32 Bytes of data a time ,
         * else read the next block to get the data*/
        if(wIndex < PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE)
        {
            if(0U != (pT5T->bMbRead))
            {
                PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
                    pT5T,
                    wBlockNum,
                    (PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE / pT5T->bBlockSize),
                    &pRxBuffer,
                    &wRxLength));
            }
            /* Loop to read the multiple blocks and read 32 bytes of data at a time */
            else
            {
                for(wIndex = 0;
                    wIndex < (PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE / pT5T->bBlockSize);
                    wIndex++)
                {
                    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
                        pT5T,
                        (wIndex + wBlockNum),
                        1,
                        &pRxBuffer,
                        &wRxLength));
                    if(wRxLength != 0U)
                    {
                        (void)memcpy(&aData[wIndex * wRxLength], pRxBuffer, wRxLength);
                    }
                }
            }

            wIndex = bIndexInit;

            /* Traverse through these 32 bytes to search for 'T' value in the
             * TLVs bytes */
            while(wIndex < PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE)
            {
                switch(aData[wIndex])
                {
                case PHAL_TOP_T5T_PROPRIETARY_TLV:
                    /* Get the length of the respective TLV */
                    status = phalTop_Sw_Int_T5T_CalculateTLVLength(pT5T, wIndex, aLength,aData);
                    if(aLength[0] == 0xFFU)
                    {
                        wIndex += (((uint16_t)aLength[1] << 8U) | aLength[2]) + 3U;
                    }
                    else
                    {
                        wIndex += (uint16_t)(aLength[0]) + 1U;
                    }
                    break;

                case PHAL_TOP_T5T_NDEF_TLV:
                     /* Initializing the NdefHeaderAddr */
                    pT5T->wNdefHeaderAddr = wIndex + (wBlockNum * pT5T->bBlockSize);

                    /* get the length of the respective TLV */
                    status = phalTop_Sw_Int_T5T_CalculateTLVLength(
                        pT5T,
                        wIndex,
                        aLength,
                        aData);

                    /* Updating the T5T NdefLength and NdefMsgAddr parameters */
                    if(aLength[0] == 0xFFU)
                    {
                        pDataParams->wNdefLength = ((uint16_t)aLength[1] << 8U) | aLength[2];
                        pT5T->wNdefMsgAddr = pT5T->wNdefHeaderAddr + 4U;
                    }
                    else
                    {
                        pDataParams->wNdefLength = aLength[0];
                        pT5T->wNdefMsgAddr = pT5T->wNdefHeaderAddr + 2U;
                    }

                    wIndex = pT5T->wNdefMsgAddr + pDataParams->wNdefLength - 1u;

                    /* Determine the maxNdefLength based on available memory */
                    /* If available memory is more than 258 bytes, entire memory can be used with 3 byte addressing */
                    if(((pT5T->wMlen * 8U /* */) - pT5T->wNdefHeaderAddr) > /* */ 0x102U)
                    {
                        pDataParams->wMaxNdefLength = ((pT5T->wMlen * 8U) - (pT5T->wNdefHeaderAddr + 4U));
                    }
                    /* If available memory is either 257 or 258 bytes entire space cant be used for Ndef storage */
                    else if((((pT5T->wMlen * 8U) - pT5T->wNdefHeaderAddr) == 0x101U) || (((pT5T->wMlen * 8U) - pT5T->wNdefHeaderAddr) == 0x102U))
                    {
                        pDataParams->wMaxNdefLength = 0xFE;
                    }
                    /* If available memory is less than 257 bytes, entire memory can be used with 1 byte addressing */
                    else
                    {
                        pDataParams->wMaxNdefLength = ((pT5T->wMlen * 8U) - (pT5T->wNdefHeaderAddr + 2U));
                    }


                    /* This indicates that next position is having Terminator
                     * TLV (If Present) or else simply exit */
                    bExitLoop = 1;
                    break;

                case PHAL_TOP_T5T_TERMINATOR_TLV:
                    /* If Terminator TLV exits, if sets the T5T Parameter */
                    pT5T->bTerminatorTlvPresence = PH_ON;
                    bExitLoop = 1;
                    break;

                default:
                    break;
                }

                wIndex++;
            }
            bIndexInit = 0;
            wRxLength = 0;
        }
        else
        {
            /* Bringing back the count for continuous sweep */
            wBlockNum -= (PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE / pT5T->bBlockSize);
            wIndex -= PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE;
        }
        wBlockNum += (PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE / pT5T->bBlockSize);
        if(bExitLoop == 1U)
        {
          /* Stop reading the rest of the Data Memory and exit */
            break;
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T5T_CheckNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pTagState
                                        )
{
    phStatus_t    PH_MEMLOC_REM status;
    uint8_t       PH_MEMLOC_REM * pRxBuffer = NULL;
    uint16_t      PH_MEMLOC_REM wRxLength = 0;
    phalTop_T5T_t PH_MEMLOC_REM * pT5T = &pDataParams->ualTop.salTop_T5T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT5T->pAlI15693DataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Reset tag state */
    *pTagState = PHAL_TOP_STATE_NONE;

    /* Clear values from previous detection, if any */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_ClearState(pDataParams, pT5T));

    /* Read CC Block */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
        pT5T,
        PHAL_TOP_T5T_CC_BLOCK,
        1,
        &pRxBuffer,
        &wRxLength));

    /* Update block size */
    pT5T->bBlockSize = wRxLength;

    /* Check for NMN */
    if((pRxBuffer[0] != PHAL_TOP_T5T_NDEF_MAGIC_NUMBER) &&
       (pRxBuffer[0] != PHAL_TOP_T5T_NDEF_MAGIC_NUMBER_2))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_NON_NDEF_TAG, PH_COMP_AL_TOP);
    }

    /* Check for extended support */
    if(*pRxBuffer == PHAL_TOP_T5T_NDEF_MAGIC_NUMBER_2)
    {
        pT5T->bExtendedCommandSupport = 1;
    }

    /* Version Check */
    pDataParams->bVno = (pRxBuffer[1] >> 4U);

    /* Validate version number */
    if(((pDataParams->bVno & /* */ 0x0CU) >> 2U) > (uint8_t)PHAL_TOP_T5T_MAJOR_VERSION)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_VERSION, PH_COMP_AL_TOP);
    }

    /* Update RW access */
    pT5T->bRwa = (pRxBuffer[1] & 0x0FU);

    /* Validate read write access */
    if((pT5T->bRwa != PHAL_TOP_T5T_CC_RWA_RW) && (pT5T->bRwa != PHAL_TOP_T5T_CC_RWA_RO))
    {
        /* RFU/Proprietary options; Not supported */
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
    }

    /* Update Read multiple block support */
    pT5T->bMbRead = (pRxBuffer[3] & 0x01U);

    /* Update Lock block support */
    pT5T->bLockBlock = ((pRxBuffer[3] >> 3U) & 0x01U);

    /* Update special frame support */
    pT5T->bSplFrm = ((pRxBuffer[3] >> 4U) & 0x01U);

    /* Update MLEN depending upon the CC Bytes value */
    if(pRxBuffer[2] != 0x00U)
    {
        pT5T->wMlen = pRxBuffer[2];
    }
    else if((pRxBuffer[2] == 0x00U) && (pT5T->bBlockSize > 4U))
    {
        pT5T->wMlen = (((uint16_t)pRxBuffer[6] << 8U) | (pRxBuffer[7]));
    }
    else
    {
        /* Read next block */
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
            pT5T,
            (PHAL_TOP_T5T_CC_BLOCK + 1U),
            1,
            &pRxBuffer,
            &wRxLength));

        pT5T->wMlen = (((uint16_t)pRxBuffer[2] << 8U) | (pRxBuffer[3]));
    }

    /* Set Maximum Blocks count */
    pT5T->bMaxBlockNum = (uint16_t) (((pT5T->wMlen * 8U /* */) / pT5T->bBlockSize) - 1U);

    /* Detect TLVs */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_DetectTlvBlocks(pDataParams, pT5T));

    if((pT5T->wNdefHeaderAddr != 0U) && (pDataParams->wNdefLength != 0U))
    {
        if(pT5T->bRwa == PHAL_TOP_T5T_CC_RWA_RW)
        {
            pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;
        }
        else
        {
            pDataParams->bTagState = PHAL_TOP_STATE_READONLY;
        }
    }
    else if((pDataParams->wNdefLength == 0U) && (pT5T->wNdefHeaderAddr != 0U))
    {
        if(pT5T->bRwa == PHAL_TOP_T5T_CC_RWA_RW)
        {
            pDataParams->bTagState = PHAL_TOP_STATE_INITIALIZED;
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
        }
    }
    else
    {
        /* No NDEF TLV detected */
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Update state in out parameter */
    *pTagState = pDataParams->bTagState;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T5T_EraseNdef(
                                        phalTop_Sw_DataParams_t * pDataParams
                                        )
{
    phStatus_t    PH_MEMLOC_REM status;
    uint8_t       PH_MEMLOC_REM * pRxBuffer = NULL;
    uint16_t      PH_MEMLOC_REM wRxLength = 0;
    phalTop_T5T_t PH_MEMLOC_REM * pT5T = &pDataParams->ualTop.salTop_T5T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT5T->pAlI15693DataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Check if tag is in valid state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* Check if tag is read-only */
    if(pDataParams->bTagState == PHAL_TOP_STATE_READONLY)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_READONLY_TAG, PH_COMP_AL_TOP);
    }

    /* Check if tag is already in initialized state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_INITIALIZED)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_EMPTY_NDEF, PH_COMP_AL_TOP);
    }

    /* Read the NDEF length block */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
        pT5T,
        ((pT5T->wNdefHeaderAddr + 1U) / pT5T->bBlockSize),
        1,
        &pRxBuffer,
        &wRxLength));

    /* Set NDEF length as '0' */
    pRxBuffer[((((pT5T->wNdefHeaderAddr) + 1U)) % (pT5T->bBlockSize))] = 0x00;

    /* Write NDEF length as '0' */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
        pT5T,
        ((pT5T->wNdefHeaderAddr + 1U) / pT5T->bBlockSize),
        pRxBuffer,
        pT5T->bBlockSize));

    /* Update tag state */
    pDataParams->bTagState = PHAL_TOP_STATE_INITIALIZED;
    pDataParams->wNdefLength = 0;
    pT5T->wNdefMsgAddr = pT5T->wNdefHeaderAddr + 2U;
    pDataParams->wMaxNdefLength = ((pT5T->wMlen * 8U) - pT5T->wNdefMsgAddr + 1U);

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T5T_FormatNdef(
                                         phalTop_Sw_DataParams_t * pDataParams
                                         )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint8_t       PH_MEMLOC_BUF   aData[PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE] = {0};
    uint8_t       PH_MEMLOC_REM   * pRxBuffer = NULL;
    uint16_t      PH_MEMLOC_REM   wRxLength = 0;
    uint16_t      PH_MEMLOC_COUNT wIndex = 0;
    uint16_t      PH_MEMLOC_COUNT wCount = 0;
    uint8_t       PH_MEMLOC_BUF   aNdefData[7] = {0x03, 0x03, 0xD0, 0x00, 0x00, 0xFE, 0x00};

    phalTop_T5T_t PH_MEMLOC_REM * pT5T = &pDataParams->ualTop.salTop_T5T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT5T->pAlI15693DataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Check for NDEF presence */
    if(pDataParams->bTagState != PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_FORMATTED_TAG, PH_COMP_AL_TOP);
    }

    /* Check Tag memory */
    if(pT5T->wMlen < 0x05U)
    {
        /* Set default (size of ICODE SLI-L) */
        pT5T->wMlen = 8;
    }

    /* Perform a read to get actual block size */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
        pT5T,
        PHAL_TOP_T5T_CC_BLOCK,
        1,
        &pRxBuffer,
        &wRxLength));

    /* Update block size */
    pT5T->bBlockSize = wRxLength;

    /* Configuring the CC Bytes */
    if(pT5T->wMlen > 0xFFU)
    {
        if(((pT5T->wMlen * 8U) / pT5T->bBlockSize) <= PHAL_TOP_T5T_DEFAULT_BLOCK_NUMBER)
        {
            /* 255 blocks of data read by 1-byte block addressing */
            aData[0] = 0xE1;
            pT5T->bExtendedCommandSupport = 0;

        }
        else
        {
            /* More than 255 blocks of data read by 2-byte block addressing */
            aData[0] = 0xE2;
            pT5T->bExtendedCommandSupport = 1;
        }
        pT5T->wNdefHeaderAddr = 8;
        aData[2] = 0x00;
        aData[6] = (uint8_t)((pT5T->wMlen & 0xFF00U) >> 8U);
        aData[7] = (uint8_t)(pT5T->wMlen & 0x00FFU);
    }
    else
    {
        if(((pT5T->wMlen * 8U) / pT5T->bBlockSize) <= PHAL_TOP_T5T_DEFAULT_BLOCK_NUMBER)
        {
            /* 255 blocks of data read by 1-byte block addressing */
            aData[0] = 0xE1;
            pT5T->bExtendedCommandSupport = 0;
        }
        else
        {
            /* More than 255 blocks of data read by 2-byte block addressing */
            aData[0] = 0xE2;
            pT5T->bExtendedCommandSupport = 1;
        }
        pT5T->wNdefHeaderAddr = 4;
        aData[2] = (uint8_t)pT5T->wMlen;
    }

    /* Load the Default Version No. */
    pDataParams->bVno = PHAL_TOP_T5T_NDEF_SUPPORTED_VNO;

    /* Set r/w access */
    pT5T->bRwa = PHAL_TOP_T5T_CC_RWA_RW;

    /* Version 1.0 and Read-Write Access */
    aData[1] = (uint8_t)(((pDataParams->bVno << 4U) & 0xF0U) | pT5T->bRwa);

    /* Additional information byte */
    aData[3] = (uint8_t)((pT5T->bMbRead & 0x01U) | ((pT5T->bLockBlock << 3U) & 0x08U) | ((pT5T->bSplFrm << 4U) & 0x10U));

    /* RFU bytes */
    aData[4] = 0x00;
    aData[5] = 0x00;

    /* Update NDEF length */
    pDataParams->wNdefLength = 0x03;
    pT5T->wNdefMsgAddr = (pT5T->wNdefHeaderAddr + 2U);

    /* Writing the Data For Block size = 4 */
    if(pT5T->bBlockSize == PHAL_TOP_T5T_BYTES_PER_BLOCK_DEFAULT)
    {
        for(wIndex = 0; wIndex < (pT5T->wNdefHeaderAddr / pT5T->bBlockSize); wIndex++)
        {
            PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write (
                pT5T,
                PHAL_TOP_T5T_CC_BLOCK + wIndex,
                &aData[((wIndex)*(pT5T->bBlockSize))],
                pT5T->bBlockSize));

            wCount += pT5T->bBlockSize;
        }

        /* Write empty NDEF Message */
        for(wIndex = (uint8_t)(pT5T->wNdefHeaderAddr / pT5T->bBlockSize);
            wIndex <= ((pDataParams->wNdefLength + pT5T->wNdefMsgAddr) / pT5T->bBlockSize);
            wIndex++)
        {
            PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write (
                pT5T,
                wIndex,
                (&aNdefData[((wIndex-((uint8_t)(pT5T->wNdefHeaderAddr / pT5T->bBlockSize)))*(pT5T->bBlockSize))]),
                pT5T->bBlockSize));
        }
    }
    /* Writing the Data For Block size > 4 */
    else
    {
        /* Padding of data bytes if CC length is less than the Block Size */
        if(pT5T->bExtendedCommandSupport == 0U)
        {
            for(wCount = 0; wCount < (pT5T->bBlockSize - 4U); wCount++)
            {
                aData[4U + wCount] = aNdefData[wCount];
            }
        }
        else
        {
            for(wCount = 0; wCount < (pT5T->bBlockSize - 8U); wCount++)
            {
                aData[8U + wCount] = aNdefData[wCount];
            }
        }

        /* Write CC block Separately */
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write (
            pT5T,
            PHAL_TOP_T5T_CC_BLOCK,
            aData,
            pT5T->bBlockSize));

        /* Write remaining NDEF Message */
        for(wIndex = 1; wIndex < ((pDataParams->wNdefLength - wCount) / pT5T->bBlockSize); wIndex++)
        {
            PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write (
                pT5T,
                PHAL_TOP_T5T_CC_BLOCK + wIndex,
                (&aNdefData[((wIndex-1u)*(pT5T->bBlockSize))]),
                pT5T->bBlockSize));

            wCount += pT5T->bBlockSize;
        }

        /* Checking for remaining data, where number of bytes left is less than block size */
        if(wCount < pDataParams->wNdefLength)
        {
            for(wIndex = 0; wIndex < (pDataParams->wNdefLength - wCount); wIndex++)
            {
                aData[wIndex] = aNdefData[wCount + wIndex];
            }

            /* Padding of zeros */
            while(wIndex < pT5T->bBlockSize)
            {
                aData[wIndex] = 0x00;
                wIndex++;
            }

            PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write (
                pT5T,
                (wCount / pT5T->bBlockSize)+1U,
                aData,
                pT5T->bBlockSize));
        }
    }

    /* Update tag state */
    pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;
    pDataParams->wMaxNdefLength = ((pT5T->wMlen * 8U) - pT5T->wNdefMsgAddr + 1U);

    return PH_ERR_SUCCESS;
}


phStatus_t phalTop_Sw_Int_T5T_ReadNdef(
                                       phalTop_Sw_DataParams_t * pDataParams,
                                       uint8_t * pData,
                                       uint16_t * pLength
                                       )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint8_t       PH_MEMLOC_REM   * pRxBuffer = NULL;
    uint16_t      PH_MEMLOC_COUNT wIndex;
    uint16_t      PH_MEMLOC_COUNT wLoopCount;
    uint16_t      PH_MEMLOC_REM   wTempLength = 0;
    uint16_t      PH_MEMLOC_REM   wRxLength = 0;
    phalTop_T5T_t PH_MEMLOC_REM * pT5T = &pDataParams->ualTop.salTop_T5T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT5T->pAlI15693DataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Reset NDEF length */
    if((pData != NULL) && (pLength != NULL))
    {
        *pLength = 0;
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Check if tag is in valid state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* Check for NDEF length > 0 (in initialized state NDEF length is 0) */
    if(pDataParams->bTagState == PHAL_TOP_STATE_INITIALIZED)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_EMPTY_NDEF, PH_COMP_AL_TOP);
    }

    /* Buffer allocated by user is less than NDEF message size on Tag. */
    if (pDataParams->wNdefLength > PH_NXPNFCRDLIB_CONFIG_MAX_NDEF_DATA)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_TOP);
    }

    /* Number of blocks to be read for reading entire Ndef Message */
    wLoopCount = (pDataParams->wNdefLength + ((pT5T->wNdefMsgAddr) % (pT5T->bBlockSize))) / pT5T->bBlockSize;

    /* If entire data is not covered read one more block */
    if(0U != ((pDataParams->wNdefLength + ((pT5T->wNdefMsgAddr) % (pT5T->bBlockSize))) % (pT5T->bBlockSize)))
    {
        wLoopCount++;
    }

    /* Check whether Multiple Block read is supported or not */
    if(pT5T->bMbRead == 0U)
    {
        /* Read one block at a time till entire data is read */
        for(wIndex = 0;
            wIndex < wLoopCount;
            wIndex++)
        {
            PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
                pT5T,
                ((pT5T->wNdefMsgAddr / pT5T->bBlockSize) + wIndex),
                1,
                &pRxBuffer,
                &wRxLength));
                if(0U != (wIndex))
                {
                    /* Copying the received data block by block */
                    if(wRxLength != 0U)
                    {
                        (void)memcpy(pData + wTempLength, pRxBuffer, wRxLength);
                    }
                    wTempLength += wRxLength;
                }
                else
                {
                    /* Copy the data from the first block starting from the message address */
                    phMemcpy(pData, (pRxBuffer + ((pT5T->wNdefMsgAddr) % (pT5T->bBlockSize))), (wRxLength - ((pT5T->wNdefMsgAddr) % (pT5T->bBlockSize))));
                    wTempLength = wRxLength - ((pT5T->wNdefMsgAddr) % (pT5T->bBlockSize));
                }
        }
    }
    else
    {
        /* Read complete Loop Count blocks in one attempt itself */
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
            pT5T,
            (pT5T->wNdefMsgAddr / pT5T->bBlockSize),
            wLoopCount,
            &pData,
            &wRxLength));

        /* Copy the received data to the buffer provided */
        phMemcpy(pData, (pData + ((pT5T->wNdefMsgAddr) % (pT5T->bBlockSize))), (wRxLength - ((pT5T->wNdefMsgAddr) % (pT5T->bBlockSize))));

    }

    *pLength = pDataParams->wNdefLength;
    return PH_ERR_SUCCESS;
}


phStatus_t phalTop_Sw_Int_T5T_WriteNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pData,
                                        uint16_t wLength
                                        )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint8_t       PH_MEMLOC_REM   * pRxBuffer = NULL;
    uint8_t       PH_MEMLOC_BUF   aTemp[PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE] = {0};
    uint8_t       PH_MEMLOC_BUF   aLengthTemp[PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE] = {0};
    uint8_t       PH_MEMLOC_BUF   bLengthSplit = 0;
    uint16_t      PH_MEMLOC_REM   wBlockNum;
    uint16_t      PH_MEMLOC_REM   wByteNum;
    uint16_t      PH_MEMLOC_REM   wLoopCount = 0;
    uint16_t      PH_MEMLOC_REM   wRxLength = 0;
    uint16_t      PH_MEMLOC_REM   wBytesUsed = 0;
    phalTop_T5T_t PH_MEMLOC_REM * pT5T = &pDataParams->ualTop.salTop_T5T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT5T->pAlI15693DataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Check if tag is in valid state */
    if(pDataParams->bTagState == PHAL_TOP_STATE_NONE)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* Check if tag is read-only */
    if(pDataParams->bTagState == PHAL_TOP_STATE_READONLY)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_READONLY_TAG, PH_COMP_AL_TOP);
    }

    /* Validate input parameters */
    if((wLength > pDataParams->wMaxNdefLength) || (wLength == 0x00U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Validate Input Buffer */
    if(pData == NULL)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Determine the block number where to update the Length */
    if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (uint16_t) (pT5T->bBlockSize - 1U))
    {
        wBlockNum = (pT5T->wNdefHeaderAddr / pT5T->bBlockSize) + 1U;
    }
    else
    {
        wBlockNum = (pT5T->wNdefHeaderAddr / pT5T->bBlockSize);
    }

    /* Reading the First Ndef block of the tag so as to keep other data intact */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
        pT5T,
        wBlockNum,
        1,
        &pRxBuffer,
        &wRxLength));

    /* Copying the First block data received so as to use it for Later Length Updation */
    if(wRxLength != 0U)
    {
        (void)memcpy(&aLengthTemp[0], pRxBuffer, wRxLength);
    }

    /* Writing the NdefLength as Zero */
    if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (uint16_t) (pT5T->bBlockSize - 1U))
    {
        /* Making the very first byte after Ndef TLV as zero so as to make Ndef Length 0 */
        aLengthTemp[0] = 0x00;
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
            pT5T,
            ((pT5T->wNdefHeaderAddr / pT5T->bBlockSize) + 1U),
            &aLengthTemp[0],
            pT5T->bBlockSize));
    }
    else
    {
        /* Making the very first byte after Ndef TLV as zero so as to make Ndef Length 0 */
        aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 1U] = 0x00;
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
            pT5T,
            (pT5T->wNdefHeaderAddr / pT5T->bBlockSize),
            &aLengthTemp[0],
            pT5T->bBlockSize));
    }

    /* Update the ndef length to zero */
    pDataParams->wNdefLength = 0;

    /* Writing the entire Ndef Data */
    if(wLength > 0xFEU)
    {
        /* Ndef Header lies at the last byte and thus entire length info will lie in next block */
        if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (pT5T->bBlockSize - 1U))
        {
            phMemcpy(&aLengthTemp[3], &pData[0], (((pT5T->bBlockSize - 3U) < wLength) ? (pT5T->bBlockSize - 3U) : wLength));
            wBytesUsed =  pT5T->bBlockSize - 3U;
            wByteNum = 0;
        }
        /* Ndef Header lies at the second last byte and thus length info will split */
        else if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (pT5T->bBlockSize - 2U))
        {
            phMemcpy(&aLengthTemp[2], &pData[0], (((pT5T->bBlockSize - 2U) < wLength) ? (pT5T->bBlockSize - 2U) : wLength));
            wBytesUsed = pT5T->bBlockSize - 2U;
            wBlockNum++;
            bLengthSplit = 1;
        }
        /* Ndef Header lies at the third last byte and thus length info will split */
        else if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (pT5T->bBlockSize - 3U))
        {
            phMemcpy(&aLengthTemp[1], &pData[0], (((pT5T->bBlockSize - 1U) < wLength) ? (pT5T->bBlockSize - 1U) : wLength));
            wBytesUsed = pT5T->bBlockSize - 1U;
            wBlockNum++;
            bLengthSplit = 1;
        }
        /* Ndef Header lies at the fourth last byte and thus length info will split for all block length except block length of 4 */
        else if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (pT5T->bBlockSize - 4U))
        {
            phMemcpy(&aLengthTemp[0], &pData[0], (((pT5T->bBlockSize) < wLength) ? (pT5T->bBlockSize) : wLength));
            wBytesUsed = pT5T->bBlockSize;
            wBlockNum++;
            bLengthSplit = 1;
        }
        else
        {
            phMemcpy(&aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 4U], &pData[0],
            (((pT5T->bBlockSize - (((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 4U ) %  pT5T->bBlockSize)) < wLength) ? (pT5T->bBlockSize - (((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 4U ) %  pT5T->bBlockSize)) : wLength));   /* PRQA S 3200 */
            wBytesUsed = pT5T->bBlockSize - (((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 4U ) % (pT5T->bBlockSize));
            wByteNum = ((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 1U ) % (pT5T->bBlockSize);
        }

    }
    else
    {
        /* Ndef Header lies at the  last byte and thus length info will not split */
        if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (uint16_t) (pT5T->bBlockSize - 1U))
        {
            phMemcpy(&aLengthTemp[1], &pData[0], (((pT5T->bBlockSize - 1U) < wLength) ? (pT5T->bBlockSize - 1U) : wLength));
            wBytesUsed = pT5T->bBlockSize - 1U;
            wByteNum = 0;
        }
        /* Ndef Header lies at the second last byte and thus length info will not split */
        else if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (uint16_t) (pT5T->bBlockSize - 2U))
        {
            (void)memcpy(&aLengthTemp[0], &pData[0], (((pT5T->bBlockSize) < wLength) ? (pT5T->bBlockSize) : wLength));
            wBytesUsed = pT5T->bBlockSize;
            wBlockNum++;
            bLengthSplit = 1;
        }
        else
        {
            phMemcpy(&aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 2U], &pData[0],
            (((pT5T->bBlockSize - (((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 2U ) % (pT5T->bBlockSize))) < wLength) ? (pT5T->bBlockSize - (((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 2U ) % (pT5T->bBlockSize))) : wLength));   /* PRQA S 3200 */
            wBytesUsed = pT5T->bBlockSize - (((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 2U) % (pT5T->bBlockSize));
            wByteNum = ((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 1U ) % (pT5T->bBlockSize);
        }
    }

    /* Writing the first block of Ndef Data */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
        pT5T,
        wBlockNum,
        &aLengthTemp[0],
        pT5T->bBlockSize));


    /* IF entire data is already written in first step then there is no data to be written */
    if(wBytesUsed > wLength)
    {
        wLoopCount = 0;
    }
    /* Count how many write cycles are needed to be executed */
    else
    {
        wLoopCount = (wLength - wBytesUsed)/ pT5T->bBlockSize;
    }

    /* If atleast one complete block is to be written */
    if(0U != wLoopCount)
    {
        for(wLoopCount = 0; wLoopCount < ((wLength - wBytesUsed) / pT5T->bBlockSize); wLoopCount++)
        {
            PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
                pT5T,
                (wBlockNum + 1U + wLoopCount),
                &pData[wBytesUsed + (wLoopCount * pT5T->bBlockSize)],
                pT5T->bBlockSize));
        }
    }

    /* If transaction is already complete no remaining bytes will be there */
    if(wLength > wBytesUsed)
    {
        /* Remaining data is not enough for one complete block */
        if(0u != ((wLength - wBytesUsed) % (pT5T->bBlockSize)))
        {
            phMemcpy(&aTemp[0], &pData[((wLoopCount * pT5T->bBlockSize) + wBytesUsed)], ((wLength - wBytesUsed) % (pT5T->bBlockSize)));
            PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
                pT5T,
                (wBlockNum + 1U + wLoopCount),
                &aTemp[0],
                pT5T->bBlockSize));
        }
    }

    /* Update the Length */
    if(0U != (bLengthSplit))
    {
        wBlockNum--;
        /* Reading the First Ndef block of the tag so as to keep other data intact */
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
            pT5T,
            wBlockNum,
            1,
            &pRxBuffer,
            &wRxLength));
        if(wRxLength != 0U)
        {
            (void)memcpy(&aLengthTemp[0], pRxBuffer, wRxLength);
        }

        /* More than 0xFE bytes data will require 3 bytes to fill up the Length */
        if(wLength > 0xFEU)
        {
            if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (uint16_t) (pT5T->bBlockSize - 2U))
            {
                aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 1U] = 0xFF;

                /* Reading the First Ndef block of the tag so as to keep other data intact */
                PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
                    pT5T,
                    wBlockNum + 1U,
                    1,
                    &pRxBuffer,
                    &wRxLength));
                if(wRxLength != 0U)
                {
                    (void)memcpy(&aTemp[0], pRxBuffer, wRxLength);
                }
                aTemp[0] = (uint8_t)((wLength & 0xFF00U) >> 8U);
                aTemp[1] = (uint8_t)(wLength & 0x00FFU);
                /* Two exchanges are needed to be perfomed to update the length */
                bLengthSplit = 2;

            }
            else if((pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) == (uint16_t) (pT5T->bBlockSize - 3U))
            {
                aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 1U] = 0xFF;
                aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 2U] = (uint8_t)((wLength & 0xFF00U) >> 8U);

                /* Reading the First Ndef block of the tag so as to keep other data intact */
                PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
                    pT5T,
                    wBlockNum + 1U,
                    1,
                    &pRxBuffer,
                    &wRxLength));

                (void)memcpy(&aTemp[0], pRxBuffer, wRxLength);
                aTemp[1] = (uint8_t)(wLength & 0x00FFU);
                /* Two exchanges are needed to be perfomed to update the length */
                bLengthSplit = 2;
            }
            else
            {
                aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 1U] = 0xFF;
                aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 2U] = (uint8_t)((wLength & 0xFF00U) >> 8U);
                aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 3U] = (uint8_t)(wLength & 0x00FFU);
            }
        }
        else
        {
            aLengthTemp[(pT5T->wNdefHeaderAddr % (pT5T->bBlockSize)) + 1U] = (uint8_t)(wLength & 0x00FFU);
        }
    }
    else
    {
        if(wLength > 0xFEU)
        {
            aLengthTemp[wByteNum] = 0xFF;
            aLengthTemp[wByteNum + 1U] = (uint8_t)((wLength & 0xFF00U) >> 8U);
            aLengthTemp[wByteNum + 2U] = (uint8_t)(wLength & 0x00FFU);
        }
        else
        {
            aLengthTemp[wByteNum] = (uint8_t)(wLength & 0x00FFU);
        }
    }

    /* Update the length */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
        pT5T,
        wBlockNum,
        &aLengthTemp[0],
        pT5T->bBlockSize));

    /* check whether two exchanges are needed to update the Length */
    if(bLengthSplit == 2U)
    {
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
            pT5T,
            wBlockNum + 1U,
            &aTemp[0],
            pT5T->bBlockSize));
    }

    /* Update the terminator TLV if applicable  */
    if( pDataParams->wMaxNdefLength > wLength)
    {
        wBlockNum = ((pT5T->wNdefHeaderAddr + ((wLength > 0xFEU) ? 3U : 1U) +  wLength) / pT5T->bBlockSize);
        if(((pT5T->wNdefHeaderAddr + ((wLength > 0xFEU) ? 3U : 1U) +  wLength) % (pT5T->bBlockSize)) == (uint16_t) (pT5T->bBlockSize - 1U))
        {
            wBlockNum++;
        }

        /* Reading the Last Ndef block of the tag so as to write Terminator tlv without disturbing other data */
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
            pT5T,
            wBlockNum,
            1,
            &pRxBuffer,
            &wRxLength));

        if(wRxLength != 0U)
        {
            (void)memcpy(&aTemp[0], pRxBuffer, wRxLength);
        }

        /* Checking where to append terminator TLV */
        if(((pT5T->wNdefHeaderAddr + ((wLength > 0xFEU) ? 3U : 1U) +  wLength) % (pT5T->bBlockSize)) != (uint16_t) (pT5T->bBlockSize - 1U))
        {
            aTemp[(((pT5T->wNdefHeaderAddr + ((wLength > 0xFEU) ? 3U : 1U) +  wLength) % (pT5T->bBlockSize))) + 1U] = PHAL_TOP_T5T_TERMINATOR_TLV;
        }
        else
        {
            aTemp[0] = PHAL_TOP_T5T_TERMINATOR_TLV;
        }

        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Write(
            pT5T,
            wBlockNum,
            &aTemp[0],
            pT5T->bBlockSize));
    }

    /* Updating the context parameter */
    if(wLength > 0xFEU)
    {
        pT5T->wNdefMsgAddr = pT5T->wNdefHeaderAddr + 4U;
    }
    else
    {
        pT5T->wNdefMsgAddr = pT5T->wNdefHeaderAddr + 2U;
    }
    pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;
    pDataParams->wNdefLength = wLength;

    return PH_ERR_SUCCESS;

}
phStatus_t phalTop_Sw_Int_T5T_CalculateTLVLength(
                                                 phalTop_T5T_t * pT5T,
                                                 uint16_t wIndex,
                                                 uint8_t *pLength,
                                                 uint8_t *pData
                                                 )
{
   phStatus_t PH_MEMLOC_REM status;
   uint8_t    PH_MEMLOC_REM * pRxBuffer = NULL;
   uint16_t   PH_MEMLOC_REM wRxLength = 0;

    /* Get the length of the respective TLV if the tag field is within PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE -1
       and length is less than 0xFF */
    if((wIndex < (PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE - 1U)) && (pData[wIndex + 1U] != 0xFFU))
    {
          pLength[0] = pData[wIndex + 1U];
    }
    /* Get the length of the respective TLV if the tag field is at PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE-2
       and length is greater than 0xFF */
    else if ((wIndex == (PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE - 2U)) && (pData[wIndex + 1U] == 0xFFU))
    {
        pLength[0] = pData[wIndex + 1U];
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
            pT5T,
            ((wIndex / pT5T->bBlockSize) + 1U),
            1,
            &pRxBuffer,
            &wRxLength));

        pLength[1] = pRxBuffer[0];
        pLength[2] = pRxBuffer[1];
    }
    /* Get the length of the respective TLV if the tag field is at PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE-3
       and length is greater than 0xFF */
    else if ((wIndex == (PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE - 3U)) && (pData[wIndex + 1U] == 0xFFU))
    {
        pLength[0] = pData[wIndex + 1U];
        pLength[1] = pData[wIndex + 2U];

        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
            pT5T,
            ((wIndex / pT5T->bBlockSize) + 1U),
            1,
            &pRxBuffer,
            &wRxLength));

        pLength[2] = pRxBuffer[0];
    }
   /* Get the length of the respective TLV if the tag field is within PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE-3
       and length is greater than 0xFF */
    else if((wIndex < (PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE - 3U)) && (pData[wIndex + 1U] == 0xFFU))
    {
        pLength[0] = pData[wIndex + 1U];
        pLength[1] = pData[wIndex + 2U];
        pLength[2] = pData[wIndex + 3U];
    }
    /* Get the length of the respective TLV if the tag field is at PHAL_TOP_T5T_MAXIMUM_BLOCK_SIZE-1 */
    else
    {
        PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T5T_Read(
            pT5T,
            ((wIndex / pT5T->bBlockSize) + 1U),
            1,
            &pRxBuffer,
            &wRxLength));

        pLength[0] = pRxBuffer[0];

        if(pLength[0] == 0xFFU)
        {
            pLength[1] = pRxBuffer[1];
            pLength[2] = pRxBuffer[2];
        }
    }

    return PH_ERR_SUCCESS;
}

static void phMemcpy(void* dest, void* src, uint16_t wLength)
{
    uint32_t dwCpyLength = 0;
    dwCpyLength = wLength;
    (void)memcpy(dest, src, dwCpyLength);
}

#endif /* NXPBUILD__PHAL_ICODE_SW */
#endif  /* NXPBUILD__PHAL_TOP_SW */
