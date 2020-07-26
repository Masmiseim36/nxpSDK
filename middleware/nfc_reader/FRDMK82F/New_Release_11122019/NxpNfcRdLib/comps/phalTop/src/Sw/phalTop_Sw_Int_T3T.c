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

#include <phalFelica.h>
#include <phalTop.h>

#ifdef NXPBUILD__PHAL_TOP_SW
#ifdef NXPBUILD__PHAL_FELICA_SW

#include "phalTop_Sw_Int_T3T.h"

void phalTop_Sw_Int_T3T_CreateBlockList(
                                        uint8_t * pBlockList,
                                        uint16_t wStartIndex,
                                        uint8_t bCount,
                                        uint8_t * pBlockListLength
                                        )
{
    uint8_t  PH_MEMLOC_COUNT bIndex;
    uint16_t PH_MEMLOC_REM   wLastBlock;

    /* Last block number */
    wLastBlock = wStartIndex + bCount;

    /* Create block list */
    bIndex = 0;
    for(; wStartIndex < wLastBlock; wStartIndex++)
    {
        /* Check for 2 byte or 3 byte format */
        if(wStartIndex <= 0xFFU)
        {
            /* 2 byte format */
            pBlockList[bIndex++] = 0x80;
            pBlockList[bIndex++] = (uint8_t)wStartIndex;
        }
        else
        {
            /* 3 byte format */
            pBlockList[bIndex++] = 0x00;
            pBlockList[bIndex++] = (uint8_t)wStartIndex;
            pBlockList[bIndex++] = (uint8_t)(wStartIndex >> 8U);
        }
    }

    /* Return length of block list */
    *pBlockListLength = bIndex;
}

phStatus_t phalTop_Sw_Int_T3T_ClearState(
                                         phalTop_Sw_DataParams_t * pDataParams,
                                         phalTop_T3T_t * pT3T
                                         )
{
    /* Reset parameters */
    pT3T->bNbr = 0;
    pT3T->bNbw = 0;
    pT3T->bNmaxb = 0;
    pT3T->bRwa = 0;
    pDataParams->bVno = 0;
    pDataParams->wNdefLength = 0;
    pDataParams->bTagState = 0;
    /* Clearing the attribute block in type 3 tag context */
    (void)memset(&pT3T->bAttributeBlock[0], 0x00, PHAL_TOP_T3T_BLOCK_SIZE);

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T3T_CheckNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pTagState
                                        )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint8_t       PH_MEMLOC_REM   bLength;
    uint8_t       PH_MEMLOC_REM   bMoreCardsAvailable;
    uint8_t       PH_MEMLOC_REM   bRxNumBlocks;
    uint8_t       PH_MEMLOC_BUF   aNdefSystemCode[2] = {0x12, 0xFC};
    uint8_t       PH_MEMLOC_BUF   aBlockList[2] = {0x80, 0x00};
    uint8_t       PH_MEMLOC_BUF   aReadServiceList[2] = {0x0B, 0x00};
    uint8_t       PH_MEMLOC_BUF   aData[16];
    uint16_t      PH_MEMLOC_REM   wCheckSum = 0;
    uint8_t       PH_MEMLOC_COUNT bIndex = 0;
    phalTop_T3T_t PH_MEMLOC_REM * pT3T = &pDataParams->ualTop.salTop_T3T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT3T->pAlT3TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Reset tag state */
    *pTagState = PHAL_TOP_STATE_NONE;

    /* Clear values from previous detection, if any */
    PH_CHECK_SUCCESS_FCT(status, phalTop_Sw_Int_T3T_ClearState(pDataParams, pT3T));

    /* Send ReqC with system code 0x12FC */
    status = phalFelica_ActivateCard(
            pT3T->pAlT3TDataParams,
            aNdefSystemCode,
            0X00,
            aData,
            &bLength,
            &bMoreCardsAvailable);

    if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_NON_NDEF_TAG, PH_COMP_AL_TOP);
    }

    /* Read attribute information data */
    PH_CHECK_SUCCESS_FCT(status, phalFelica_Read(
            pT3T->pAlT3TDataParams,
            0x01,
            aReadServiceList,
            0x01,
            aBlockList,
            0x02,
            &bRxNumBlocks,
            aData));

    /* Update version number */
    pDataParams->bVno = aData[0];

    /* Check for supported version number */
    if((uint8_t)(pDataParams->bVno & 0xF0U) > (uint8_t)(PHAL_TOP_T3T_NDEF_SUPPORTED_VNO & 0xF0U))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_VERSION, PH_COMP_AL_TOP);
    }

    /* Number of blocks that can be read using one Check Command */
    pT3T->bNbr = aData[1];

    /* Validate Nbr */
    if(pT3T->bNbr == 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Number of blocks that can be written using one Update Command */
    pT3T->bNbw = aData[2];

    /* Validate Nbw */
    if(pT3T->bNbw == 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Maximum number of Blocks available for NDEF data */
    pT3T->bNmaxb = (uint16_t)(((uint16_t)aData[3] << 8U) | aData[4]);

    /* Validate Nmaxb */
    if(pT3T->bNmaxb == 0U)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Validate Write flag */
    if((aData[9] != 0x00U) && (aData[9] != 0x0FU))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Read/write access */
    pT3T->bRwa = aData[10];

    /* Validate read/write access */
    if((pT3T->bRwa != 0x00U) && (pT3T->bRwa != 0x01U))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* NDEF length */
    pDataParams->wNdefLength = (uint16_t)(((uint32_t)aData[11] << 16U) | (((uint16_t)aData[12] << 8U) | aData[13]));

    /* Validate NDEF length */
    if(pDataParams->wNdefLength > (uint16_t)(pT3T->bNmaxb * 16U))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    /* Calculate checksum */
    for(bIndex = 0; bIndex < 14U; bIndex++)
    {
        wCheckSum += aData[bIndex];
    }

    /* Validate checksum */
    if(((uint16_t)((((uint16_t)aData[14] << 8U) | aData[15]))) != wCheckSum)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
    }

    if(pDataParams->wNdefLength != 0U)
    {
        if(pT3T->bRwa == PHAL_TOP_T3T_AIB_RWA_RW)
        {
            pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;
        }
        else
        {
            pDataParams->bTagState = PHAL_TOP_STATE_READONLY;
        }
    }
    else
    {
        if(pT3T->bRwa == PHAL_TOP_T3T_AIB_RWA_RW)
        {
            pDataParams->bTagState = PHAL_TOP_STATE_INITIALIZED;
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_MISCONFIGURED_TAG, PH_COMP_AL_TOP);
        }
    }

    /* Update max. NDEF size */
    pDataParams->wMaxNdefLength = (pT3T->bNmaxb * PHAL_TOP_T3T_BLOCK_SIZE);
    /* Storing the attribute block in type 3 tag context */
    (void)memcpy(&pT3T->bAttributeBlock[0], &aData[0], PHAL_TOP_T3T_BLOCK_SIZE);
    /* Update state in out parameter */
    *pTagState = pDataParams->bTagState;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T3T_FormatNdef(
                                         phalTop_Sw_DataParams_t * pDataParams
                                         )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint16_t      PH_MEMLOC_REM   wCheckSum;
    uint8_t       PH_MEMLOC_COUNT bIndex;
    uint8_t       PH_MEMLOC_REM   aBlockData[16];
    uint8_t       PH_MEMLOC_REM   aBlockList[2] = {0x80, 0x00};
    uint8_t       PH_MEMLOC_BUF   aSystemCode[2] = {0x88, 0xB4};
    uint8_t       PH_MEMLOC_BUF   aReadServiceList[2] = {0x0B, 0x00};
    uint8_t       PH_MEMLOC_BUF   aWriteServiceList[2] = {0x09, 0x00};
    uint8_t       PH_MEMLOC_REM   bRxNumBlocks = 0;
    uint8_t       PH_MEMLOC_REM   aMcBlockList[2] = {0x80, 0x88};
    uint8_t       PH_MEMLOC_REM   bLength;
    uint8_t       PH_MEMLOC_REM   bMoreCardsAvailable;
    uint8_t       PH_MEMLOC_BUF   aNdefData[3] = {0xD0, 0x00, 0x00};

    phalTop_T3T_t PH_MEMLOC_REM * pT3T = &pDataParams->ualTop.salTop_T3T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT3T->pAlT3TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
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

    /* Send ReqC with system code 0x88B4 (for FeliCa lite tag) */
    status = phalFelica_ActivateCard(
        pT3T->pAlT3TDataParams,
        aSystemCode,
        0,
        aBlockData,
        &bLength,
        &bMoreCardsAvailable);

    if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
    }

    /* Read MC (Memory Configuration) block to get SYS_OP (System Option) */
    PH_CHECK_SUCCESS_FCT(status, phalFelica_Read(
        pT3T->pAlT3TDataParams,
        0x01,
        aReadServiceList,
        0x01,
        aMcBlockList,
        0x02,
        &bRxNumBlocks,
        aBlockData));

    /* Set SYS_OP if not set */
    if(aBlockData[3] != 0x01U)
    {
        aBlockData[3] = 0x01;

        PH_CHECK_SUCCESS_FCT(status, phalFelica_Write(
            pT3T->pAlT3TDataParams,
            0x01,
            aWriteServiceList,
            0x01,
            aMcBlockList,
            0x02,
            aBlockData));
    }

    /* Update version number */
    pDataParams->bVno = PHAL_TOP_T3T_NDEF_SUPPORTED_VNO;
    aBlockData[0] = pDataParams->bVno;

    /* Number of blocks that can be read using one Check Command */
    pT3T->bNbr = 0x04;    /* FeliCa lite default  */
    aBlockData[1] = pT3T->bNbr;

    /* Number of blocks that can be written using one Update Command */
    pT3T->bNbw = 0x01;    /* FeliCa lite default  */
    aBlockData[2] = pT3T->bNbw;

    /* Update Max blocks available for NDEF */
    pT3T->bNmaxb = 0x0D;  /* FeliCa lite default  */
    aBlockData[3] = (uint8_t)((uint16_t)(pT3T->bNmaxb & 0xFF00U) >> 8U);
    aBlockData[4] = (uint8_t)(pT3T->bNmaxb & 0x00FFU);

    /* Unused bytes */
    aBlockData[5] = 0x00;
    aBlockData[6] = 0x00;
    aBlockData[7] = 0x00;
    aBlockData[8] = 0x00;

    /* Write Flag */
    aBlockData[9] = 0x00U;

    /* RW access */
    pT3T->bRwa = PHAL_TOP_T3T_AIB_RWA_RW;
    aBlockData[10] = pT3T->bRwa;

    /* Default NDEF message length */
    aBlockData[11] = 0x00U;
    aBlockData[12] = 0x00U;
    aBlockData[13] = 0x03U;

    /* Calculate checksum */
    wCheckSum = 0;
    for(bIndex = 0; bIndex < 14U; bIndex++)
    {
        wCheckSum += aBlockData[bIndex];
    }

    /* Update checksum */
    aBlockData[14] = (uint8_t)(wCheckSum >> 8U);
    aBlockData[15] = (uint8_t)(wCheckSum & 0x00FFU);

    /* Write attribute information data */
    PH_CHECK_SUCCESS_FCT(status, phalFelica_Write(
        pT3T->pAlT3TDataParams,
        0x01,
        aWriteServiceList,
        0x01,
        aBlockList,
        0x02,
        aBlockData));

    aBlockList[1] = 0x01;

    /* Write empty NDEF message */
    PH_CHECK_SUCCESS_FCT(status, phalFelica_Write(
        pT3T->pAlT3TDataParams,
        0x01,
        aWriteServiceList,
        0x01,
        aBlockList,
        0x02,
        aNdefData));

    /* Update tag state parameters */
    pDataParams->wNdefLength = 0x03;
    pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;
    pDataParams->wMaxNdefLength = (pT3T->bNmaxb * PHAL_TOP_T3T_BLOCK_SIZE);
    /* Storing the attribute block in type 3 tag context */
    (void)memcpy(&pT3T->bAttributeBlock[0], &aBlockData[0], PHAL_TOP_T3T_BLOCK_SIZE);

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T3T_ReadNdef(
                                       phalTop_Sw_DataParams_t * pDataParams,
                                       uint8_t * pData,
                                       uint16_t * pLength
                                       )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint16_t      PH_MEMLOC_COUNT wIndex;
    uint16_t      PH_MEMLOC_COUNT wDataIndex;
    uint8_t       PH_MEMLOC_BUF   aLastBlockData[16];
    uint8_t       PH_MEMLOC_REM   bLastBlock = 0;
    uint16_t      PH_MEMLOC_REM   wReadCount;
    uint16_t      PH_MEMLOC_REM   wBlocksPerRead;
    uint16_t      PH_MEMLOC_REM   wNdefLength = 0;
    uint8_t       PH_MEMLOC_BUF   aReadServiceList[2] = {0x09, 0x00};
    uint8_t       PH_MEMLOC_BUF   aBlockList[3U * PHAL_TOP_T3T_READ_MAX_BLOCKS] = {0};
    uint8_t       PH_MEMLOC_REM   bRxNumBlocks;
    uint8_t       PH_MEMLOC_REM   bBlockListLength;
    phalTop_T3T_t PH_MEMLOC_REM * pT3T = &pDataParams->ualTop.salTop_T3T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT3T->pAlT3TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Reset NDEF length */
    *pLength = 0;

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

    /* If read-only use read-only service */
    if(pDataParams->bTagState == PHAL_TOP_STATE_READONLY)
    {
        aReadServiceList[0] = 0x0B;
        aReadServiceList[1] = 0x00;
    }

    /* Blocks read per read command */
    wBlocksPerRead = pT3T->bNbr;

    /* Max blocks supported per read is PHAL_TOP_T3T_READ_MAX_BLOCKS */
    if(wBlocksPerRead > PHAL_TOP_T3T_READ_MAX_BLOCKS)
    {
        wBlocksPerRead = PHAL_TOP_T3T_READ_MAX_BLOCKS;
    }

    /* NDEF data length to be read */
    wNdefLength = pDataParams->wNdefLength;

    /* Number of read commands required */
    wReadCount = (wNdefLength / (wBlocksPerRead * PHAL_TOP_T3T_BLOCK_SIZE));

    if(0U != (wNdefLength % (wBlocksPerRead * PHAL_TOP_T3T_BLOCK_SIZE)))
    {
        wReadCount += 1U;
    }

    wDataIndex = 0;
    for(wIndex = 0; wIndex < wReadCount; wIndex++)
    {
        /* If data to be read in last read is less that total data in number of
         * blocks read, then read last block separately. */
        if(wNdefLength < (wBlocksPerRead * PHAL_TOP_T3T_BLOCK_SIZE))
        {
            wBlocksPerRead = wNdefLength / PHAL_TOP_T3T_BLOCK_SIZE;

            /* Last block is partially filled */
            if(0U != (wNdefLength % PHAL_TOP_T3T_BLOCK_SIZE))
            {
                bLastBlock = 1;
            }
        }

        /* Read "wBlocksPerRead" number of blocks */
        if(0U != (wBlocksPerRead))
        {
            /* Create block list */
            phalTop_Sw_Int_T3T_CreateBlockList(
                aBlockList,
                ((wDataIndex / PHAL_TOP_T3T_BLOCK_SIZE) + 1U),
                (uint8_t)wBlocksPerRead,
                &bBlockListLength);

            PH_CHECK_SUCCESS_FCT(status, phalFelica_Read(
                pT3T->pAlT3TDataParams,
                0x01,
                aReadServiceList,
                (uint8_t)wBlocksPerRead,
                aBlockList,
                bBlockListLength,
                &bRxNumBlocks,
                &pData[wDataIndex]));

            /* Data read count */
            wDataIndex += (wBlocksPerRead * PHAL_TOP_T3T_BLOCK_SIZE);

            /* Balance data to be read */
            wNdefLength -= (wBlocksPerRead * PHAL_TOP_T3T_BLOCK_SIZE);
        }

        /* Read last block */
        if(0U != (bLastBlock))
        {
            /* Create block list */
            phalTop_Sw_Int_T3T_CreateBlockList(
                aBlockList,
                ((wDataIndex / PHAL_TOP_T3T_BLOCK_SIZE) + 1U),
                1,
                &bBlockListLength);

            PH_CHECK_SUCCESS_FCT(status, phalFelica_Read(
                pT3T->pAlT3TDataParams,
                0x01,
                aReadServiceList,
                0x01,
                aBlockList,
                bBlockListLength,
                &bRxNumBlocks,
                aLastBlockData));

            /* Copy valid NDEF data part from last read block */
            (void)memcpy(&pData[wDataIndex], aLastBlockData, wNdefLength);
        }
    }

    /* NDEF length */
    *pLength = (uint16_t) pDataParams->wNdefLength;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T3T_WriteNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pData,
                                        uint16_t wLength
                                        )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint16_t      PH_MEMLOC_COUNT wDataIndex;
    uint16_t      PH_MEMLOC_COUNT wIndex;
    uint16_t      PH_MEMLOC_REM   wCheckSum;
    uint16_t      PH_MEMLOC_REM   wWriteCount;
    uint16_t      PH_MEMLOC_REM   wBlocksPerWrite;
    uint8_t       PH_MEMLOC_REM   bLastBlock = 0;
    uint8_t       PH_MEMLOC_BUF   aBlockData[PHAL_TOP_T3T_BLOCK_SIZE];
    uint8_t       PH_MEMLOC_BUF   aAttBlock[2] = {0x80, 0x00};
    uint8_t       PH_MEMLOC_BUF   aBlockList[3U * PHAL_TOP_T3T_WRITE_MAX_BLOCKS] = {0};
    uint8_t       PH_MEMLOC_BUF   aWriteServiceList[2] = {0x09, 0x00};
    uint8_t       PH_MEMLOC_REM   bBlockListLength;
    phalTop_T3T_t PH_MEMLOC_REM * pT3T = &pDataParams->ualTop.salTop_T3T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT3T->pAlT3TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
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

    /* Check input parameters */
    if((wLength > (pT3T->bNmaxb * PHAL_TOP_T3T_BLOCK_SIZE)) || (wLength == 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Set Write Flag */
    pT3T->bAttributeBlock[9] = PHAL_TOP_T3T_WRITE_FLAG_SET;

  /* Reset NDEF length */
    pT3T->bAttributeBlock[11] = 0x00U;
    pT3T->bAttributeBlock[12] = 0x00U;
    pT3T->bAttributeBlock[13] = 0xF0U;

    /* Calculate CheckSum */
    wCheckSum = 0;
    for(wIndex = 0; wIndex < 14U; wIndex++)
    {
        wCheckSum += pT3T->bAttributeBlock[wIndex];
    }

    pT3T->bAttributeBlock[14] = (uint8_t)(wCheckSum >> 8U);
    pT3T->bAttributeBlock[15] = (uint8_t)(wCheckSum & 0x00FFU);

    /* Write Attribute Block */
    PH_CHECK_SUCCESS_FCT(status, phalFelica_Write(
        pT3T->pAlT3TDataParams,
        0x01,
        aWriteServiceList,
        0x01,
        aAttBlock,
        0x02,
        &pT3T->bAttributeBlock[0]));

    /* Blocks written per write command */
    wBlocksPerWrite = pT3T->bNbw;

    /* Max block supported per write is PHAL_TOP_T3T_WRITE_MAX_BLOCKS */
    if(wBlocksPerWrite > PHAL_TOP_T3T_WRITE_MAX_BLOCKS)
    {
        wBlocksPerWrite = PHAL_TOP_T3T_WRITE_MAX_BLOCKS;
    }

    /* Number of write commands required */
    wWriteCount = (uint8_t)(wLength / (wBlocksPerWrite * PHAL_TOP_T3T_BLOCK_SIZE));

    if(0U != (wLength % (wBlocksPerWrite * PHAL_TOP_T3T_BLOCK_SIZE)))
    {
        wWriteCount += 1U;
    }

    /* Write NDEF Data */
    wDataIndex = 0;
    for(wIndex = 0; wIndex < wWriteCount; wIndex++)
    {
        /* If data to be written in last write is less that total data written
         * per write, then write last block separately. */
        if((wLength - wDataIndex) < (wBlocksPerWrite * PHAL_TOP_T3T_BLOCK_SIZE))
        {
            wBlocksPerWrite = ((wLength - wDataIndex) / PHAL_TOP_T3T_BLOCK_SIZE);

            /* Last block is partially filled */
            if(0U != ((wLength - wDataIndex) % PHAL_TOP_T3T_BLOCK_SIZE))
            {
                bLastBlock = 1;
            }
        }

        /* Write "wBlocksPerWrite" number of blocks */
        if(0U != (wBlocksPerWrite))
        {
            /* Create block list */
            phalTop_Sw_Int_T3T_CreateBlockList(
                aBlockList,
                ((wDataIndex / PHAL_TOP_T3T_BLOCK_SIZE) + 1U),
                (uint8_t)wBlocksPerWrite,
                &bBlockListLength);

            PH_CHECK_SUCCESS_FCT(status, phalFelica_Write(
                pT3T->pAlT3TDataParams,
                0x01,
                aWriteServiceList,
                (uint8_t)wBlocksPerWrite,
                aBlockList,
                bBlockListLength,
                &pData[wDataIndex]));

            /* Written data count */
            wDataIndex += (wBlocksPerWrite * PHAL_TOP_T3T_BLOCK_SIZE);
        }

        /* Write last block */
        if(0U != (bLastBlock))
        {
            /* Copy balance data to be written */
            (void)memset(aBlockData, 0x00, PHAL_TOP_T3T_BLOCK_SIZE);
            (void)memcpy(aBlockData, &pData[wDataIndex], (size_t)((size_t)wLength - wDataIndex));

            /* Create block list */
            phalTop_Sw_Int_T3T_CreateBlockList(
                aBlockList,
                ((wDataIndex / PHAL_TOP_T3T_BLOCK_SIZE) + 1U),
                1,
                &bBlockListLength);

            PH_CHECK_SUCCESS_FCT(status, phalFelica_Write(
                pT3T->pAlT3TDataParams,
                0x01,
                aWriteServiceList,
                0x01,
                aBlockList,
                bBlockListLength,
                aBlockData));
        }
    }

    /* Reset Write Flag */
    pT3T->bAttributeBlock[9] = PHAL_TOP_T3T_WRITE_FLAG_RESET;

    /* Set NDEF length */
    pT3T->bAttributeBlock[11] = 0x00U;
    pT3T->bAttributeBlock[12] = (uint8_t)(wLength >> 8U);
    pT3T->bAttributeBlock[13] = (uint8_t)(wLength & 0x00FFU);

    /* Calculate CheckSum */
    wCheckSum = 0;
    for(wIndex = 0; wIndex < 14U; wIndex++)
    {
        wCheckSum += pT3T->bAttributeBlock[wIndex];
    }

    pT3T->bAttributeBlock[14] = (uint8_t)(wCheckSum >> 8U);
    pT3T->bAttributeBlock[15] = (uint8_t)(wCheckSum & 0x00FFU);

    /* Write Attribute Block */
    PH_CHECK_SUCCESS_FCT(status, phalFelica_Write(
        pT3T->pAlT3TDataParams,
        0x01,
        aWriteServiceList,
        0x01,
        aAttBlock,
        0x02,
        &pT3T->bAttributeBlock[0]));

    /* Update state variables */
    pDataParams->bTagState = PHAL_TOP_STATE_READWRITE;
    pDataParams->wNdefLength = wLength;

    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_T3T_EraseNdef(
                                        phalTop_Sw_DataParams_t * pDataParams
                                        )
{
    phStatus_t    PH_MEMLOC_REM   status;
    uint16_t      PH_MEMLOC_COUNT wIndex;
    uint16_t      PH_MEMLOC_REM   wCheckSum;
    uint8_t       PH_MEMLOC_BUF   aBlockData[16];
    uint8_t       PH_MEMLOC_BUF   aAttBlock[2] = { 0x80, 0x00};
    uint8_t       PH_MEMLOC_BUF   aReadServiceList[2] = { 0x0B, 0x00};
    uint8_t       PH_MEMLOC_BUF   aWriteServiceList[2] = { 0x09, 0x00};
    uint8_t       PH_MEMLOC_REM   bRxNumBlocks;
    phalTop_T3T_t PH_MEMLOC_REM * pT3T = &pDataParams->ualTop.salTop_T3T;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pT3T->pAlT3TDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
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

    /* Read Attribute Block */
    PH_CHECK_SUCCESS_FCT(status, phalFelica_Read(
        pT3T->pAlT3TDataParams,
        0x01,
        aReadServiceList,
        0x01,
        aAttBlock,
        0x02,
        &bRxNumBlocks,
        aBlockData));

    /* Reset NDEF length */
    aBlockData[11] = 0x00U;
    aBlockData[12] = 0x00U;
    aBlockData[13] = 0x00U;

    /* Calculate CheckSum */
    wCheckSum = 0;
    for(wIndex = 0; wIndex < 14U; wIndex++)
    {
        wCheckSum += aBlockData[wIndex];
    }

    aBlockData[14] = (uint8_t)(wCheckSum >> 8U);
    aBlockData[15] = (uint8_t)(wCheckSum & 0x00FFU);

    /* Update Attribute Block */
    PH_CHECK_SUCCESS_FCT(status, phalFelica_Write(
        pT3T->pAlT3TDataParams,
        0x01,
        aWriteServiceList,
        0x01,
        aAttBlock,
        0x02,
        aBlockData));

    /* Update state variables */
    pDataParams->bTagState = PHAL_TOP_STATE_INITIALIZED;
    pDataParams->wNdefLength = 0;
    /* Storing the attribute block in type 3 tag context */
    (void)memcpy(&pT3T->bAttributeBlock[0], &aBlockData[0], PHAL_TOP_T3T_BLOCK_SIZE);

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHAL_FELICA_SW */
#endif /* NXPBUILD__PHAL_TOP_SW */
