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
* Software Tag Operation Application Layer Component of
* Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 5316 $ (v06.01.00)
* $Date: 2016-06-29 12:19:39 +0530 (Wed, 29 Jun 2016) $
*/

#include <ph_TypeDefs.h>
#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phNxpNfcRdLib_Config.h>

#ifdef NXPBUILD__PHAL_TOP_SW
#ifdef NXPBUILD__PHPAL_I14443P3A_SW

#include <phhalHw.h>
#include <phpalI14443p3a.h>
#include <phTools.h>
#include <phalTop.h>
#include "phalTop_Sw_Int_MfcTop.h"

static phStatus_t phalMfcTop_Sw_MifareWrite(phalTop_Sw_DataParams_t * pDataParams, uint8_t * pTxBuffer, uint16_t wTxLength);
static phStatus_t phalMfcTop_Sw_Write(phalTop_Sw_DataParams_t * pDataParams, uint8_t * pData, uint8_t bBlockNumber);
static phStatus_t phalMfcTop_Sw_ReadNfcSector(phalTop_Sw_DataParams_t * pDataParams, uint64_t lwNFCSectorCount, uint8_t * pTagState);
static void phMemCopy(void* dest, void* src, uint16_t wLength);

phStatus_t phalTop_Sw_Int_MfcTop_CheckNdef(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint8_t * pTagState
                                )
{
    uint8_t PH_MEMLOC_BUF aPublicKeySettingsMadSectorA[6] = {0xA0, 0xA1, 0xA2, 0xA3,  0xA4, 0xA5}; /* Public Key for MAD */
    uint8_t PH_MEMLOC_BUF aAid[2] = {0x03, 0xE1}; /* Function Cluster code, Application code, Identifies presence of NFC */
    uint8_t PH_MEMLOC_BUF aCommand[6];
    uint8_t PH_MEMLOC_REM bBreakLoop = 0;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t PH_MEMLOC_REM wRxLength = 0;
    uint64_t PH_MEMLOC_REM lwNFCSectorCount = 0;
    uint64_t PH_MEMLOC_REM lwTemp = 0;
    uint8_t PH_MEMLOC_REM bMadByte = 2;
    uint8_t PH_MEMLOC_BUF aMADBlock[5] = {0x01, 0x02, 0x40, 0x41, 0x42};
    uint8_t PH_MEMLOC_REM bCount = 0;
    uint8_t PH_MEMLOC_REM aNFCSectorSubstratum[5] = {0, 8, 16, 24, 32};
    phStatus_t PH_MEMLOC_REM status = PHAL_TOP_ERR_NON_NDEF_TAG;
    phalTop_MfcTop_t PH_MEMLOC_REM * pMfcTop = &pDataParams->ualTop.salTop_MfcTop;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pMfcTop->pPalI14443paDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Resetting the tag state */
    *pTagState = PHAL_MFCTOP_STATE_NONE;

    /* Looping around to Read the MAD Sectors */
    for(bCount = 0; bCount < 5U; bCount++)
    {
        /* Authentication needs to be done only once */
        if((bCount == 0U) || (bCount == 2U))
        {
            status =  phhalHw_MfcAuthenticate(
                ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                (bCount == 0U) ? PHAL_MFCTOP_MAD1_SECTORTRAILER_BLOCK : PHAL_MFCTOP_MAD2_SECTORTRAILER_BLOCK,
                PHHAL_HW_MFC_KEYA,
                &aPublicKeySettingsMadSectorA[0],
                (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                    &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
               );

            if(status != PH_ERR_SUCCESS)
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_NON_NDEF_TAG, PH_COMP_AL_TOP);
            }
        }

        /* Reading the MAD Sector Blocks */
        aCommand[0] = PH_ALMFCTOP_MFC_READ;
        aCommand[1] = aMADBlock[bCount];

        /* This is expected to fail only if the card is not present in proximity any more */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_Exchange(
                ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                PH_EXCHANGE_DEFAULT,
                aCommand,
                2,
                &pRxBuffer,
                &wRxLength));

        /* Assign the correct value of MadByte to calculate the No of Ndef Sectors Present */
        if((aMADBlock[bCount] == 0x01U) || (aMADBlock[bCount] == 0x40U))
        {
            bMadByte = 2;
        }
        else
        {
            bMadByte = 0;
        }

        /* Comparing the MAD Sector to Find out the Number of NFC AIDs present */
        /* wNFCSectorCount will store the bit representation of the sector which has AID present */
        while(bMadByte < 15U)
        {
            if((*(pRxBuffer + bMadByte) == aAid[0]) && (*(pRxBuffer + (++bMadByte)) == aAid[1]))
            {
                lwTemp = 1;
                lwTemp <<=  ( aNFCSectorSubstratum[bCount] + ((bMadByte - 1U)/2U) );
                lwNFCSectorCount |= lwTemp;
                pMfcTop->bNdefSectorCount++;
            }
            ++bMadByte;
        }

        /* If the card is MFC 1K exit after reading the blocks 1 and 2 */
        if((pMfcTop->bCardType == PHAL_MFCTOP_MFC_1K) && (bCount == 1U))
        {
            break;
        }

        /* If the card is MFP 2K exit after reading the blocks 1 , 2 0x40 and 0x41 */
        if((pMfcTop->bCardType == PHAL_MFCTOP_MFP_2K) && (bCount == 3U))
        {
            break;
        }
    }
    bMadByte = 0;

    /* Identify the First Ndef Sector */
    while(bMadByte < 40U)
    {
        if((pMfcTop->bCardType == PHAL_MFCTOP_MFP_2K) && (bMadByte == 32U))
        {
            bBreakLoop = 1U;
        }

        if(bBreakLoop != 1U)
        {
            lwTemp = 1;
            if(0u != (lwNFCSectorCount & (lwTemp << (++bMadByte))))
            {
                pMfcTop->bFirstNdefSector = bMadByte;
                bBreakLoop = 1U;
            }
        }

        if(bBreakLoop == 1U)
        {
            break;
        }
    }

    /* Check whether atleast one sector has NFC AID */
    if(0U != (pMfcTop->bFirstNdefSector))
    {
        status = phalMfcTop_Sw_ReadNfcSector(
                                        pDataParams,
                                        lwNFCSectorCount,
                                        pTagState
                                        );
    }
    else
    {
        pMfcTop->bPreFormatted = 1;
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_NON_NDEF_TAG, PH_COMP_AL_TOP);
    }


    return PH_ADD_COMPCODE(status, PH_COMP_AL_TOP);
}

phStatus_t phalTop_Sw_Int_MfcTop_ReadNdef(
                               phalTop_Sw_DataParams_t * pDataParams,
                               uint8_t * pData,
                               uint16_t * pLength
                               )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    phStatus_t PH_MEMLOC_REM bReadOperations = 0;
    uint8_t PH_MEMLOC_BUF aCommand[2];
    uint8_t PH_MEMLOC_REM bFirstReadLength = 0;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t PH_MEMLOC_BUF aPublicKeyA[6] = {0xD3, 0xF7, 0xD3, 0xF7,  0xD3, 0xF7}; /* Public Key for NFC Sectors */
    phalTop_MfcTop_t PH_MEMLOC_REM * pMfcTop = &pDataParams->ualTop.salTop_MfcTop;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pMfcTop->pPalI14443paDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Checking the valid state of the tag for Read Operation */
    if((pDataParams->bTagState != PHAL_MFCTOP_STATE_READWRITE) && (pDataParams->bTagState != PHAL_MFCTOP_STATE_READONLY))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* Buffer allocated by user is less than NDEF message size on Tag. */
    if (pDataParams->wNdefLength > PH_NXPNFCRDLIB_CONFIG_MAX_NDEF_DATA)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_AL_TOP);
    }

    /* Reading the First Ndef Sector*/
    aCommand[0] = PH_ALMFCTOP_MFC_READ;
    aCommand[1] = (pMfcTop->bFirstNdefSector > 0x1FU) ?
                  (((pMfcTop->bFirstNdefSector + 1U) * 4U) + (((pMfcTop->bFirstNdefSector + 1U) * 16U) - (16U - (pMfcTop->bNdefMessageStart/PHAL_MFCTOP_BLOCK_SIZE)))) :
                  (((pMfcTop->bFirstNdefSector + 1U) * 4U) - (4U - (pMfcTop->bNdefMessageStart/PHAL_MFCTOP_BLOCK_SIZE)));

    PH_CHECK_SUCCESS_FCT(status, phhalHw_MfcAuthenticate(
            ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
            aCommand[1],
            PHHAL_HW_MFC_KEYA,
            &aPublicKeyA[0],
            (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
    ));

    status = phhalHw_Exchange(
            ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            aCommand,
            2,
            &pRxBuffer,
            &wRxLength);

    if(status == PH_ERR_SUCCESS)
    {
        /* The Ndef message Length could be of one byte or 3 bytes */
        bFirstReadLength = (uint8_t) ((PHAL_MFCTOP_BLOCK_SIZE - ((pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + ((pDataParams->wNdefLength < 0xFFU) ? 2U : 4U))));

        /* assigning the data and length to the pointer provided by the application */
        (void)memcpy(pData, (pRxBuffer + (pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + ((pDataParams->wNdefLength < 0xFFU) ? 2U : 4U)) , bFirstReadLength);
        *pLength = bFirstReadLength;

        /* If the total Ndef Message Present in the tag is lesser than or equal to what is read in one exchange we return back */
        if(pDataParams->wNdefLength <= bFirstReadLength)
        {
            return PH_ERR_SUCCESS;
        }
        else
        {
            if(0U != ((pDataParams->wNdefLength - bFirstReadLength) % PHAL_MFCTOP_BLOCK_SIZE))
            {
                bReadOperations = ((pDataParams->wNdefLength - bFirstReadLength) /  PHAL_MFCTOP_BLOCK_SIZE) + 1U ;
            }
            else
            {
                bReadOperations = ((pDataParams->wNdefLength - bFirstReadLength) /  PHAL_MFCTOP_BLOCK_SIZE);
            }
        }
    }

    /* Performing the Read Operations to Read all the Ndef Message present in the tag */
    while(0U != bReadOperations)
    {
        aCommand[0] = PH_ALMFCTOP_MFC_READ;
        aCommand[1]++;

        /* In case of Ndef Message spread across MADs, the blocks of MAD2 needs to be ignored */
        if(aCommand[1] == 0x40U)
        {
            aCommand[1] = 0x44;
        }

        /* Check the start of new sector */
        if(((aCommand[1] > 0x7FU) ?  ((aCommand[1] - 0x80U) % PHAL_MFCTOP_MAD2_SECTOR_SIZE) : (aCommand[1] % PHAL_MFCTOP_SECTOR_SIZE)) == 0x00U)
        {
            /* Authenticate the sector */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_MfcAuthenticate(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    aCommand[1],
                    PHHAL_HW_MFC_KEYA,
                    &aPublicKeyA[0],
                    (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                        &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
            ));
        }

        if(!((aCommand[1] > 0x7FU) ?  (((aCommand[1] - 0x80U) % PHAL_MFCTOP_MAD2_SECTOR_SIZE) == 0x0FU) : ((aCommand[1] % PHAL_MFCTOP_SECTOR_SIZE) == 0x03U)))
        {
            /* Read the Block */
            PH_CHECK_SUCCESS_FCT(status, phhalHw_Exchange(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    PH_EXCHANGE_DEFAULT,
                    aCommand,
                    2,
                    &pRxBuffer,
                    &wRxLength));

            bReadOperations--;
            if(wRxLength != 0U)
            {
                (void)memcpy((pData + (*pLength)), pRxBuffer, wRxLength);
            }
            *pLength += wRxLength;
        }

        /* If all the read operations are over update the length */
        if(0U == (bReadOperations))
        {
            *pLength = pDataParams->wNdefLength;
        }
    }

    return PH_ADD_COMPCODE(status, PH_COMP_AL_TOP);
}

phStatus_t phalTop_Sw_Int_MfcTop_WriteNdef(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint8_t * pData,
                                uint16_t wLength
                                )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    uint8_t PH_MEMLOC_REM bLength = 0;
    uint8_t PH_MEMLOC_REM  bBlockNumber = 0;
    uint8_t PH_MEMLOC_REM  bCount = 0;
    uint8_t PH_MEMLOC_REM bSpillOver = 0;
    uint8_t PH_MEMLOC_REM wDataOffset = 0;
    uint8_t PH_MEMLOC_REM bLoopCount  = 0;
    uint8_t PH_MEMLOC_REM bLengthByte = 0;
    uint8_t PH_MEMLOC_REM bTerminatorBlock = 0;
    uint8_t PH_MEMLOC_REM bTerminatorByte = 0;
    uint8_t * PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t PH_MEMLOC_REM wRxLength = 0;
    uint8_t PH_MEMLOC_BUF aPublicKeyA[6] = {0xD3, 0xF7, 0xD3, 0xF7,  0xD3, 0xF7}; /* Public Key for NFC Sectors */
    uint8_t PH_MEMLOC_BUF aCommand[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t PH_MEMLOC_BUF aWriteLength[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    phalTop_MfcTop_t PH_MEMLOC_REM * pMfcTop = &pDataParams->ualTop.salTop_MfcTop;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pMfcTop->pPalI14443paDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    /* Checking the Valid state of the tag for Write Operation*/
    if((pDataParams->bTagState != PHAL_MFCTOP_STATE_INITIALIZED) && (pDataParams->bTagState != PHAL_MFCTOP_STATE_READWRITE))
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
    }

    /* If Length to be written is more than the maximum Ndef supported or 0 */
    if((wLength > pDataParams->wMaxNdefLength) || (wLength == 0U))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* If length is greater than 0xFE */
    if(wLength > 0xFEU)
    {
        bLengthByte = 3;
    }
    else
    {
        bLengthByte = 1;
    }

    /* Identifying the block from where to start the Ndef Message */
    if(pMfcTop->bFirstNdefSector > 0x1FU)
    {
        bBlockNumber = (uint8_t) (0x80U + (((pMfcTop->bFirstNdefSector - 0x20U) + 1U) * 16U) - (16U - (pMfcTop->bNdefMessageStart / PHAL_MFCTOP_BLOCK_SIZE)));
    }
    else
    {
        bBlockNumber = (uint8_t) ((pMfcTop->bFirstNdefSector + 1U) * 4U) - (4U - (pMfcTop->bNdefMessageStart / PHAL_MFCTOP_BLOCK_SIZE));
    }

    if((pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) >= (PHAL_MFCTOP_BLOCK_SIZE - bLengthByte))
    {
        bSpillOver = 1;
    }

    if(0U == (bSpillOver))
    {
        if(0u != ((pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE)))
        {
            /* Reading the Incomplete Block*/
            aCommand[0] = PH_ALMFCTOP_MFC_READ;
            aCommand[1] = bBlockNumber;

            status = phhalHw_Exchange(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    PH_EXCHANGE_DEFAULT,
                    aCommand,
                    2,
                    &pRxBuffer,
                    &wRxLength);

            if(status != PH_ERR_SUCCESS)
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
            }
            phMemCopy(&aWriteLength[0], pRxBuffer, (((uint16_t)(pMfcTop->bNdefMessageStart)) % PHAL_MFCTOP_BLOCK_SIZE));
        }

        /* Updating the TLV Length to 0 */
        aWriteLength[(pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE)] = 0x03;
        if(bLengthByte == 1U)
        {
            aWriteLength[(pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U] = 0x00;
        }
        else
        {
            if((pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) < (PHAL_MFCTOP_BLOCK_SIZE - bLengthByte))
            {
                (void)memset(&aWriteLength[(pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U], 0x00, bLengthByte);
            }
        }

        /* Update the length of the ndef field to 0 */
        PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams, aWriteLength, bBlockNumber));

        /* Update the value field with offset that will be start address of mandatory ndef tlv + 2 */
        pMfcTop->bOffset = (pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U + bLengthByte;
    }
    else
    {
        bBlockNumber++;
        pMfcTop->bOffset = (pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) - (PHAL_MFCTOP_BLOCK_SIZE - bLengthByte);
        aCommand[0] = 0x00;
    }

    /* Data to be written is bigger than the remaining byte of the first block */
    if(wLength > (PHAL_MFCTOP_BLOCK_SIZE - ((uint16_t)(pMfcTop->bOffset))))
    {
        /* Calculate the count of Write Cycles */
        bCount = (uint8_t)((wLength - (PHAL_MFCTOP_BLOCK_SIZE - ((uint16_t)(pMfcTop->bOffset)))) / PHAL_MFCTOP_BLOCK_SIZE);
        bTerminatorByte = (uint8_t)((wLength - (PHAL_MFCTOP_BLOCK_SIZE - ((uint16_t)(pMfcTop->bOffset)))) % PHAL_MFCTOP_BLOCK_SIZE);
        if(0U != (bTerminatorByte))
        {
            bCount++;
        }
    }
    /* Data to be written is lesser than or equal to the remaining byte of the first block */
    else
    {
        bCount = 0;
    }
    if(pMfcTop->bOffset < PHAL_MFCTOP_BLOCK_SIZE)
    {
        /* Backing up the command and preparing he buffer to send the first command */
        (void)memcpy(&aCommand[0], &aWriteLength[0], pMfcTop->bOffset);
        phMemCopy(&aWriteLength[pMfcTop->bOffset], pData, (PHAL_MFCTOP_BLOCK_SIZE - ((uint16_t)(pMfcTop->bOffset))));
    }

    bCount++;
    while(0U != bCount)
    {
        bLength = PHAL_MFCTOP_BLOCK_SIZE - pMfcTop->bOffset;
        if(pMfcTop->bOffset < 16U)
        {
            (void)memcpy(&aCommand[pMfcTop->bOffset], (pData + wDataOffset), bLength);
        }
        /* Skip the MAD2 Sector Block */
        if(bBlockNumber == 0x40U)
        {
            bBlockNumber = 0x44;
        }

        /* Update the value field of the Ndef Message */
        /*Auhenticating the block in case the block has not  been authenticated and we are entering new sector */
        if(((bBlockNumber > 0x7FU) ?  ((bBlockNumber - 0x80U) % PHAL_MFCTOP_MAD2_SECTOR_SIZE) : (bBlockNumber % PHAL_MFCTOP_SECTOR_SIZE)) == 0U)
        {
            /* Authenticating the First Sector */
            PH_CHECK_SUCCESS_FCT(status,phhalHw_MfcAuthenticate(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    bBlockNumber,
                    PHHAL_HW_MFC_KEYA,
                    &aPublicKeyA[0],
                    (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                        &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
            ));
        }

        /* Writing only on the data blocks and skipping the sector trailer */
        if(!((bBlockNumber > 0x7FU) ?  (((bBlockNumber - 0x80U) % PHAL_MFCTOP_MAD2_SECTOR_SIZE) == 0x0FU) : ((bBlockNumber % PHAL_MFCTOP_SECTOR_SIZE) == 0x03U)))
        {
            PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams, aCommand, bBlockNumber));
            wDataOffset =   ((PHAL_MFCTOP_BLOCK_SIZE - ((pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U + bLengthByte)) + (PHAL_MFCTOP_BLOCK_SIZE * bLoopCount));
            bLoopCount++;
            bCount--;
        }

        bBlockNumber++;
        pMfcTop->bOffset = 0;

    }

    /* Last Block of the Ndef Data */
    bTerminatorBlock = --bBlockNumber;
    /* Update the Length field of the ndef message */
    /* Writing to the First block of Ndef Sector with Ndef Message */
    if(pMfcTop->bFirstNdefSector > 0x1FU)
    {
        bBlockNumber = (uint8_t) (0x80U + (((pMfcTop->bFirstNdefSector - 0x20U) + 1U) * 16U) - (16U - (pMfcTop->bNdefMessageStart / PHAL_MFCTOP_BLOCK_SIZE)));
    }
    else
    {
        bBlockNumber = (uint8_t) ((pMfcTop->bFirstNdefSector + 1U) * 4U) - (4U - (pMfcTop->bNdefMessageStart / PHAL_MFCTOP_BLOCK_SIZE));
    }

    if(((pMfcTop->bNdefMessageStart) % PHAL_MFCTOP_BLOCK_SIZE) == 15U)
    {
        bBlockNumber++;
    }
    if(bLengthByte == 1U)
    {
        if(0U == (bSpillOver))
        {
            aWriteLength[(pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U] = (uint8_t) wLength;
        }
        else
        {
            aWriteLength[0] = (uint8_t) wLength;
        }
    }
    else
    {
        if(0U == (bSpillOver))
        {
            aWriteLength[(pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U] = 0xFF;
            aWriteLength[(pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 2U] = (uint8_t)((wLength & 0xFF00U) >> 8U);
            aWriteLength[(pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 3U] = (uint8_t)(wLength & 0x00FFU);
        }
        else
        {
            aWriteLength[0] = 0xFF;
            aWriteLength[1] = (uint8_t)((wLength & 0xFF00U) >> 8U);
            aWriteLength[2] = (uint8_t)(wLength & 0x00FFU);
        }
    }


    /* Authenticating the First Sector */
    PH_CHECK_SUCCESS_FCT(status,phhalHw_MfcAuthenticate(
            ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
            bBlockNumber,
            PHHAL_HW_MFC_KEYA,
            &aPublicKeyA[0],
            (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
    ));
    PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams, aWriteLength, bBlockNumber));

    /* Updating the Terminator TLV */
    if(pDataParams->wMaxNdefLength > wLength)
    {
        aCommand[bTerminatorByte] = PHAL_MFCTOP_TERMINATOR_TLV;
        /* Authenticating the First Sector */
        PH_CHECK_SUCCESS_FCT(status,phhalHw_MfcAuthenticate(
                ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                bTerminatorBlock,
                PHHAL_HW_MFC_KEYA,
                &aPublicKeyA[0],
                (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                    &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
        ));
        PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams, aCommand, bTerminatorBlock));
    }


    /* Update the Length of the Ndef Message Present */
    pDataParams->wNdefLength = wLength;
    /* Update the Tag State */
    pDataParams->bTagState = PHAL_MFCTOP_STATE_READWRITE;
    return PH_ERR_SUCCESS;
}

phStatus_t phalTop_Sw_Int_MfcTop_FormatNdef(
                                 phalTop_Sw_DataParams_t * pDataParams
                                 )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    uint8_t PH_MEMLOC_REM bBlockNo = 0;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t PH_MEMLOC_REM bAuthKey = 0;
    uint8_t PH_MEMLOC_BUF aCommand[2] = {0x00, 0x00};
    uint8_t PH_MEMLOC_BUF aDefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF}; /* Default Public Key for MFC after Production Phase */
    uint8_t PH_MEMLOC_BUF aNdefPublickKeyA[6] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7}; /* NDEF Public Key */
    uint8_t PH_MEMLOC_BUF aPublicKeySettingsMadSectorA[6] = {0xA0, 0xA1, 0xA2, 0xA3,  0xA4, 0xA5}; /* Public Key Settings for MAD */
    uint8_t PH_MEMLOC_REM bFirstSector = 0x01; /* this is first sector */
    uint8_t PH_MEMLOC_REM bSectorCount = 0x01; /* Total NFC Sector Count */
    uint8_t PH_MEMLOC_REM bLastSector = 0x00; /* Last Sector of the Card */
    uint8_t PH_MEMLOC_BUF aMADSectorTrailer[16] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x78, 0x77, 0x88, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; /* Keeping the secret key same as the default key  */
    uint8_t PH_MEMLOC_BUF aNFCSectorTrailer[16] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 0x07, 0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; /* Keeping the secret key same as the default key  */
    uint8_t PH_MEMLOC_BUF aMADSectorBlock1[16] = {0x14, 0x01, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1};  /* Block containing the aid of nfc sector, info and crc */
    uint8_t PH_MEMLOC_BUF aMADSectorBlock2[16] = {0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1};  /* Block containing the aid of nfc sector */
    uint8_t PH_MEMLOC_BUF aMAD2MFPBlock2[16] = {0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05, 0x00, 0x05};  /* Block containing the aid of nfc sector */
    uint8_t PH_MEMLOC_BUF aNFCSectorBlock0[3] = {0x03, 0x00, 0xFE}; /* Empty Ndef Message TLV alongwith the terminator TLV */
    uint8_t PH_MEMLOC_BUF aMAD2SectorTrailer[16] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x78, 0x77, 0x88, 0xC2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; /* Keeping the secret key same as the default key  */
    uint8_t PH_MEMLOC_BUF aMAD2SectorBlock0[16] = {0xE8, 0x01, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1}; /* MAD 2 Sector 16 block 0  */
    uint8_t PH_MEMLOC_BUF aMAD2MFPBlock0[16] = {0xBE, 0x01, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1}; /* MAD 2 Sector 16 block 0  */
    phalTop_MfcTop_t PH_MEMLOC_REM * pMfcTop = &pDataParams->ualTop.salTop_MfcTop;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pMfcTop->pPalI14443paDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_USE_CONDITION, PH_COMP_AL_TOP);
    }

    if(0U != (pMfcTop->bPreFormatted))
    {
        /* Authenticating the MAD Sector */
        status = phhalHw_MfcAuthenticate(
            ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
            bBlockNo,
            PHHAL_HW_MFC_KEYA,
            &aPublicKeySettingsMadSectorA[0],
            (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
           );
    }
    else
    {
        /* Authenticating the MAD Sector */
        status = phhalHw_MfcAuthenticate(
            ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
            bBlockNo,
            PHHAL_HW_MFC_KEYA,
            &aDefaultKey[0],
            (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
           );
    }

    if(status != PH_ERR_SUCCESS)
    {
        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
    }

    /* Reading the sector trailer of MAD sector */

    aCommand[0] = PH_ALMFCTOP_MFC_READ;
    aCommand[1] = PHAL_MFCTOP_MAD1_SECTORTRAILER_BLOCK;

    PH_CHECK_SUCCESS_FCT(status, phhalHw_Exchange(
            ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            aCommand,
            2,
            &pRxBuffer,
            &wRxLength));

    if(0U != (pMfcTop->bPreFormatted))
    {
        if((pMfcTop->bCardType == PHAL_MFCTOP_MFC_4K) || (pMfcTop->bCardType == PHAL_MFCTOP_MFP_4K) || (pMfcTop->bCardType == PHAL_MFCTOP_MFP_2K))
        {
            /* GPB matches to a formatted MAD2 card */
            if(*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) != PHAL_MFCTOP_MFC4K_GPB)
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
            }
        }
        else
        {
            /* GPB matches to a formatted 1k card */
            if(*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) != PHAL_MFCTOP_MFC1K_GPB)
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
            }
        }

    }
    else
    {
        /* Determining which key to be used on wards for sector trailer authentication of subsequent sectors */
        if((*(pRxBuffer + 6U) == 0xFFU) && (*(pRxBuffer + 7U) == 0x07U) && (*(pRxBuffer + 8U) == 0x80U))
        {
            bAuthKey = PHHAL_HW_MFC_KEYA;
        }
        else if((*(pRxBuffer + 6U) == 0x7FU) && (*(pRxBuffer + 7U) == 0x07U) && (*(pRxBuffer + 8U) == 0x88U))
        {
            bAuthKey = PHHAL_HW_MFC_KEYB;
        }
        else
        {
            return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
        }

        /* GPB matches to a factory default card*/
        if(*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) != PHAL_MFCTOP_DEFAULT_GPB)
        {
            return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
        }

    }


    bLastSector = PHAL_MFCTOP_MFC1K_LAST_SECTOR;

    /* Writing the first block  of MAD sector */
    PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMADSectorBlock1[0] , 0x01));

    /* Writing the second block  of MAD sector */
    PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMADSectorBlock2[0] , 0x02));

    /* Writing the sector trailer of MAD sector */
    if((pMfcTop->bCardType == PHAL_MFCTOP_MFC_4K) || (pMfcTop->bCardType == PHAL_MFCTOP_MFP_4K) || (pMfcTop->bCardType == PHAL_MFCTOP_MFP_2K))
    {
        PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMAD2SectorTrailer[0] , 0x03));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMADSectorTrailer[0] , 0x03));
    }

    /** Authenticate the 0x10 sector of the 4k card */
    if((pMfcTop->bCardType == PHAL_MFCTOP_MFC_4K) || (pMfcTop->bCardType == PHAL_MFCTOP_MFP_4K)|| (pMfcTop->bCardType == PHAL_MFCTOP_MFP_2K))
    {
        bLastSector = PHAL_MFCTOP_MFC4K_LAST_SECTOR;
        bBlockNo = PHAL_MFCTOP_MAD2_SECTORTRAILER_BLOCK;

        if(0U != (pMfcTop->bPreFormatted))
        {
            /* Authenticating the MAD Sector */
            status = phhalHw_MfcAuthenticate(
                ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                bBlockNo,
                PHHAL_HW_MFC_KEYA,
                &aPublicKeySettingsMadSectorA[0],
                (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                    &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
               );
        }
        else
        {
            status = phhalHw_MfcAuthenticate(
                ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                bBlockNo,
                (bAuthKey == PHHAL_HW_MFC_KEYA) ? PHHAL_HW_MFC_KEYA :PHHAL_HW_MFC_KEYB,
                &aDefaultKey[0],
                (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                    &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
               );
        }

        if(status != PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
        }

        /* Reading the sector trailer of MAD2 sector */
        aCommand[0] = PH_ALMFCTOP_MFC_READ;
        aCommand[1] = PHAL_MFCTOP_MAD2_SECTORTRAILER_BLOCK;

        PH_CHECK_SUCCESS_FCT(status, phhalHw_Exchange(
                ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                PH_EXCHANGE_DEFAULT,
                aCommand,
                2,
                &pRxBuffer,
                &wRxLength));

        if(0U != (pMfcTop->bPreFormatted))
        {
            /* GPB matches to a 4k formatted card */
            if(*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) != PHAL_MFCTOP_MFC4K_GPB)
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
            }

        }
        else
        {
            /* GPB mathces to a factory default card*/
            if(*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) != PHAL_MFCTOP_DEFAULT_GPB)
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
            }

        }

        if(pMfcTop->bCardType == PHAL_MFCTOP_MFP_2K)
        {
            /* Writing the zeroth block  of MAD2 sector */
            PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMAD2MFPBlock0[0] , 0x40));
        }
        else
        {
            /* Writing the zeroth block  of MAD2 sector */
            PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMAD2SectorBlock0[0] , 0x40));
        }

        /* Writing the first block  of MAD2 sector */
        PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMADSectorBlock2[0] , 0x41));

        if(pMfcTop->bCardType == PHAL_MFCTOP_MFP_2K)
        {
            /* Writing the second block  of MAD2 sector */
            PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMAD2MFPBlock2[0] , 0x42));
        }
        else
        {
            /* Writing the second block  of MAD2 sector */
            PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMADSectorBlock2[0] , 0x42));
        }


        /* Updating the Sector Trailer */
        PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aMAD2SectorTrailer[0] , 0x43));

    }

    /* Preparing the data block of first NFC Sector */
    do
    {
        if(bSectorCount >  0x1FU)
        {
            if(pMfcTop->bCardType == PHAL_MFCTOP_MFP_2K)
            {
                break;
            }
            bBlockNo = (uint8_t) ((128u + ((bSectorCount - 0x1FU) * 16U) - 1U));
        }
        else
        {
            bBlockNo = (((bSectorCount + 1U) * 4U) - 1U);
        }

        if(bSectorCount != 0x10U)
        {
            if(0U != (pMfcTop->bPreFormatted))
            {
                /* Authenticating the First Sector */
                status = phhalHw_MfcAuthenticate(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    bBlockNo,
                    PHHAL_HW_MFC_KEYA,
                    &aNdefPublickKeyA[0],
                    (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                        &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
                   );
            }
            else
            {
                /* Authenticating the First Sector */
                status = phhalHw_MfcAuthenticate(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    bBlockNo,
                    (bAuthKey == PHHAL_HW_MFC_KEYA) ? PHHAL_HW_MFC_KEYA :PHHAL_HW_MFC_KEYB,
                    &aDefaultKey[0],
                    (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                        &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
                   );
            }

            if(status != PH_ERR_SUCCESS)
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_TAG, PH_COMP_AL_TOP);
            }

            /* Writing the sector trailer of MAD sector */

            PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams, &aNFCSectorTrailer[0] , bBlockNo));

            if(0U != (bFirstSector))
            {

                /* Authenticating the First Sector */
                status = phhalHw_MfcAuthenticate(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    bBlockNo,
                    PHHAL_HW_MFC_KEYA,
                    &aNdefPublickKeyA[0],
                    (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                        &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
                   );

                /* Writing the first block  of MAD sector */
                PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams,&aNFCSectorBlock0[0] , bBlockNo -3u));

                /* Disabling the first sector flag */
                bFirstSector = 0;
            }
        }
        bSectorCount++;


    }while(bSectorCount <= bLastSector);

    pDataParams->bVno = (uint8_t)((aNFCSectorTrailer[PHAL_MFCTOP_GPB_BYTE] & PHAL_MFCTOP_GPB_MAJORVERSION) >> 6U);
    pDataParams->bVno |= (aNFCSectorTrailer[PHAL_MFCTOP_GPB_BYTE] & PHAL_MFCTOP_GPB_MINORVERSION) >> 4U;

    return PH_ADD_COMPCODE(status, PH_COMP_AL_TOP);
}


static phStatus_t phalMfcTop_Sw_Write(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint8_t * pData,
                                uint8_t bBlockNumber
                                )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    uint8_t PH_MEMLOC_BUF aCommand[2];

    aCommand[0] = PH_ALMFCTOP_MFC_WRITE;
    aCommand[1] = bBlockNumber;

    PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_MifareWrite(pDataParams,
        aCommand,
        2));

    status = phalMfcTop_Sw_MifareWrite(pDataParams,
            pData,
            PHAL_MFCTOP_BLOCK_SIZE);

    return status;
}


static phStatus_t phalMfcTop_Sw_MifareWrite(phalTop_Sw_DataParams_t * pDataParams, uint8_t * pTxBuffer, uint16_t wTxLength)
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_INTERNAL_ERROR;
    uint16_t   PH_MEMLOC_REM wValidBits = 0;
    uint8_t *  PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t   PH_MEMLOC_REM wRxLength = 0;
    phalTop_MfcTop_t PH_MEMLOC_REM * pMfcTop = &pDataParams->ualTop.salTop_MfcTop;

    /* Disable RxCrc */
    (void) phhalHw_SetConfig(((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF);

    status = phhalHw_Exchange(
        ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        pTxBuffer,
        wTxLength,
        &pRxBuffer,
        &wRxLength);

    /* ACK/NAK Handling */
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        /* Retrieve bitcount */
        PH_CHECK_SUCCESS_FCT(status, phhalHw_GetConfig(((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams, PHHAL_HW_CONFIG_RXLASTBITS, &wValidBits));

        /* Check for protocol error and check for ack*/
        if ((wValidBits != 4U) || (wRxLength != 1U) || (*pRxBuffer != 0x0AU))
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_TOP);
        }
        wRxLength--;
    }

    return status;
}

static phStatus_t phalMfcTop_Sw_ReadNfcSector(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint64_t lwNFCSectorCount,
                                uint8_t * pTagState
                                )
{
    uint8_t PH_MEMLOC_REM bBlockNo = 0;
    uint8_t PH_MEMLOC_REM bBlockCount = 0;
    uint8_t PH_MEMLOC_REM bSectorLimit  = 0;
    uint8_t PH_MEMLOC_REM bDetectNdefTLV = 0;
    uint8_t PH_MEMLOC_REM bCheckForLength = 0;
    uint8_t PH_MEMLOC_REM bSpillOver = 0;
    uint8_t PH_MEMLOC_REM bWriteAccess = 0;
    uint8_t PH_MEMLOC_REM bBreakLoop = 0;
    uint64_t PH_MEMLOC_REM lwTemp = 1;
    uint8_t PH_MEMLOC_BUF aCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t    PH_MEMLOC_REM wRxLength = 0;
    uint8_t PH_MEMLOC_BUF aPublicKeyA[6] = {0xD3, 0xF7, 0xD3, 0xF7,  0xD3, 0xF7}; /* Public Key for NFC Sectors */
    phStatus_t PH_MEMLOC_REM status = PHAL_TOP_ERR_NON_NDEF_TAG;
    phalTop_MfcTop_t PH_MEMLOC_REM * pMfcTop = &pDataParams->ualTop.salTop_MfcTop;

    /* Check if other NFC Sectors are Present */
    while(0u != (lwNFCSectorCount & (lwTemp << (pMfcTop->bFirstNdefSector))))
    {
        do
        {
            if(pMfcTop->bFirstNdefSector >  0x1FU)
            {
                bBlockNo = (uint8_t) ((128u + ((pMfcTop->bFirstNdefSector - 0x1FU) * 16U) - 1U));
                bSectorLimit = 0x0F;
            }
            else
            {
                bBlockNo = (((pMfcTop->bFirstNdefSector + 1U) * 4U) - 1U);
                bSectorLimit = 0x03;
            }
            /* Authenticating the First NFC Sector */
            status = phhalHw_MfcAuthenticate(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    bBlockNo,
                    PHHAL_HW_MFC_KEYA,
                    &aPublicKeyA[0],
                    (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                        &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
            );

            /* Authenticate of NFC Sector not successful*/
            if(status != PH_ERR_SUCCESS)
            {
                break;
            }

            /* Reading the Sector Trailer Block of the First NFC Sector*/
            aCommand[0] = PH_ALMFCTOP_MFC_READ;
            aCommand[1] = bBlockNo;

            status = phhalHw_Exchange(
                    ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                    PH_EXCHANGE_DEFAULT,
                    aCommand,
                    2,
                    &pRxBuffer,
                    &wRxLength);

            /* Sector Trailer Read not successful */
            if(status != PH_ERR_SUCCESS)
            {
                break;
            }

            /* GPB is the 9th Byte */
            /* To determine the version number */
            if(((*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) & PHAL_MFCTOP_GPB_MAJORVERSION) >> 6U) > PHAL_MFCTOP_MAJORVERSION)
            {
                return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_UNSUPPORTED_VERSION, PH_COMP_AL_TOP);
            }
            else
            {
                pDataParams->bVno = (uint8_t)((*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) & PHAL_MFCTOP_GPB_MAJORVERSION) >> 6U);
                pDataParams->bVno |= ((*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) & PHAL_MFCTOP_GPB_MINORVERSION) >> 4U);

                /* Validating Read and write access */
                if((((*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) & PHAL_MFCTOP_GPB_READACCESS) >> 2U) != PHAL_MFCTOP_ACCESS_GRANTED) ||
                        (((*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) & PHAL_MFCTOP_GPB_WRITEACCESS) != PHAL_MFCTOP_ACCESS_GRANTED) &&
                                (((*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) & PHAL_MFCTOP_GPB_WRITEACCESS) != PHAL_MFCTOP_ACCESS_DENIED))) )
                {
                    break;
                }

                bWriteAccess = (*(pRxBuffer + PHAL_MFCTOP_GPB_BYTE) & PHAL_MFCTOP_GPB_WRITEACCESS);

                for(bBlockCount = bSectorLimit; bBlockCount > 0U; bBlockCount--)
                {
                    /* Reading the First Block Of the NFC Sector*/
                    aCommand[0] = PH_ALMFCTOP_MFC_READ;
                    aCommand[1] = bBlockNo - bBlockCount;

                    status = phhalHw_Exchange(
                            ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                            PH_EXCHANGE_DEFAULT,
                            aCommand,
                            2,
                            &pRxBuffer,
                            &wRxLength);

                    /* According to spec this is never expected to fail refer 6.4.1 of AN1304 */
                    if(status != PH_ERR_SUCCESS)
                    {
                        return PH_ADD_COMPCODE_FIXED(PHAL_TOP_ERR_INVALID_STATE, PH_COMP_AL_TOP);
                    }

                    if(0U != (bSpillOver))
                    {
                        if(*pRxBuffer == 0x00U)
                        {
                            /* The tag state is Initialized */
                            *pTagState = PHAL_MFCTOP_STATE_INITIALIZED;
                            bBreakLoop = 1U;
                        }
                        else
                        {
                            if((*pRxBuffer) == 0xFFU)
                            {
                                /* Length field is 3 bytes */
                                pDataParams->wNdefLength = ((uint16_t)(*(pRxBuffer + 1U)) << 8U);
                                pDataParams->wNdefLength |= *(pRxBuffer + 2U);
                            }
                            else
                            {
                                /*Length Field is 1 byte */
                                pDataParams->wNdefLength = (*pRxBuffer);
                            }


                            if(bWriteAccess == PHAL_MFCTOP_ACCESS_GRANTED)
                            {
                                /* The tag state is Read/Write */
                                *pTagState = PHAL_MFCTOP_STATE_READWRITE;
                                bBreakLoop = 1U;
                            }
                            else
                            {
                                /* The tag state is Read Only */
                                *pTagState = PHAL_MFCTOP_STATE_READONLY;
                                bBreakLoop = 1U;
                            }
                        }
                    }

                    if(bBreakLoop != 1U)
                    {
                        for(bDetectNdefTLV = 0; bDetectNdefTLV < 16U; bDetectNdefTLV++)
                        {
                            /* Identifying the Ndef message TLV */
                            if( *(pRxBuffer + bDetectNdefTLV) == PHAL_MFCTOP_NDEF_TLV)
                            {
                                bCheckForLength = 1;
                                if(pMfcTop->bFirstNdefSector >  0x1FU)
                                {
                                    pMfcTop->bNdefMessageStart = ((PHAL_MFCTOP_MAD2_SECTOR_DATA_SIZE - bBlockCount) * PHAL_MFCTOP_BLOCK_SIZE) + bDetectNdefTLV;
                                }
                                else
                                {
                                    pMfcTop->bNdefMessageStart = ((PHAL_MFCTOP_SECTOR_DATA_SIZE - bBlockCount) * PHAL_MFCTOP_BLOCK_SIZE) + bDetectNdefTLV;
                                }
                                if(bDetectNdefTLV == 15U)
                                {
                                    bSpillOver = 1;
                                }
                                break;
                            }
                        }

                        if((0U != (bCheckForLength)) && (0u == bSpillOver))
                        {
                            if(*(pRxBuffer + bDetectNdefTLV + 1U) == 0x00U)
                            {
                                /* The tag state is Initialized */
                                *pTagState = PHAL_MFCTOP_STATE_INITIALIZED;
                                bBreakLoop = 1U;
                            }
                            else
                            {
                                if(*(pRxBuffer+ (pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U) == 0xFFU)
                                {
                                    /* Length field is 3 bytes */
                                    pDataParams->wNdefLength = ((uint16_t)(*(pRxBuffer + (pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 2U)) << 8U);
                                    pDataParams->wNdefLength |= *(pRxBuffer + (pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 3U);
                                }
                                else
                                {
                                    /*Length Field is 1 byte */
                                    pDataParams->wNdefLength = *(pRxBuffer + (pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U);
                                }


                                if(bWriteAccess == PHAL_MFCTOP_ACCESS_GRANTED)
                                {
                                    /* The tag state is Read/Write */
                                    *pTagState = PHAL_MFCTOP_STATE_READWRITE;
                                    bBreakLoop = 1U;
                                }
                                else
                                {
                                    /* The tag state is Read Only */
                                    *pTagState = PHAL_MFCTOP_STATE_READONLY;
                                    bBreakLoop = 1U;
                                }
                            }
                        }
                    }

                    if (bBreakLoop == 1U)
                    {
                        bBreakLoop = 0;
                        break;
                    }
                }
            }
        }while(FALSE);

        if(0u != (*pTagState))
        {
            /* Assign  the tag state*/
            pDataParams->bTagState = *pTagState;
            /* Calculate the Max Ndef Length suppoted by the Tag */
            pDataParams->wMaxNdefLength = (uint16_t)((PHAL_MFCTOP_SECTOR_DATA_SIZE * PHAL_MFCTOP_BLOCK_SIZE) - ((uint16_t)(pMfcTop->bNdefMessageStart)));
            if((pMfcTop->bFirstNdefSector + pMfcTop->bNdefSectorCount) > 31U)
            {
                pDataParams->wMaxNdefLength += (uint16_t) (((((uint16_t)(pMfcTop->bFirstNdefSector)) > 16u) ? (31U - ((uint16_t)(pMfcTop->bFirstNdefSector))): (30U - ((uint16_t)(pMfcTop->bFirstNdefSector)))) *
                    PHAL_MFCTOP_SECTOR_DATA_SIZE * PHAL_MFCTOP_BLOCK_SIZE);
                pDataParams->wMaxNdefLength += (uint16_t) ((((((uint16_t)(pMfcTop->bNdefSectorCount)) + ((uint16_t)(pMfcTop->bFirstNdefSector)))) - 31U) *
                    PHAL_MFCTOP_MAD2_SECTOR_DATA_SIZE * PHAL_MFCTOP_BLOCK_SIZE);
            }
            else
            {
                pDataParams->wMaxNdefLength += (uint16_t) (((((uint16_t)(pMfcTop->bNdefSectorCount)) - ((uint16_t)(pMfcTop->bFirstNdefSector)))) * PHAL_MFCTOP_SECTOR_DATA_SIZE * PHAL_MFCTOP_BLOCK_SIZE);
            }

            if(pDataParams->wMaxNdefLength > 0x100U)
            {
                pDataParams->wMaxNdefLength -= 4u;
            }
            else
            {
                pDataParams->wMaxNdefLength -= 2u;
            }
            break;
        }
        else
        {
            status = PHAL_TOP_ERR_MISCONFIGURED_TAG;
            pMfcTop->bFirstNdefSector++;
            pMfcTop->bPreFormatted = 1;
        }
    }
    return PH_ADD_COMPCODE(status, PH_COMP_AL_TOP);
}

phStatus_t phalTop_Sw_Int_MfcTop_EraseNdef(
                                 phalTop_Sw_DataParams_t * pDataParams
                                 )
{

    phStatus_t    PH_MEMLOC_REM status;
    uint8_t bBlockNumber = 0;
    uint8_t bSpillOver = 0;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;
    uint16_t PH_MEMLOC_REM wRxLength = 0;
    uint8_t PH_MEMLOC_BUF aPublicKeyA[6] = {0xD3, 0xF7, 0xD3, 0xF7,  0xD3, 0xF7}; /* Public Key for NFC Sectors */
    uint8_t aCommand[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    phalTop_MfcTop_t PH_MEMLOC_REM * pMfcTop = &pDataParams->ualTop.salTop_MfcTop;

    if((pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U])  != NULL)
    {
        pMfcTop->pPalI14443paDataParams = pDataParams->pTopTagsDataParams[pDataParams->bTagType - 1U];
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

    /* Identifying the block from where to start the Ndef Message */
    if(pMfcTop->bFirstNdefSector > 0x1FU)
    {
        bBlockNumber = (uint8_t) (0x80U + (((pMfcTop->bFirstNdefSector - 0x20U) + 1U) * 16U) - (16U - (pMfcTop->bNdefMessageStart / PHAL_MFCTOP_BLOCK_SIZE)));
    }
    else
    {
        bBlockNumber = (uint8_t)((pMfcTop->bFirstNdefSector + 1U) * 4U) - (4U - (pMfcTop->bNdefMessageStart / PHAL_MFCTOP_BLOCK_SIZE));
    }

    if((pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) == 15U)
    {
        bSpillOver = 1;
        bBlockNumber++;
    }

    /* Skip the MAD2 Sector Block */
    if(bBlockNumber == 0x40U)
    {
        bBlockNumber = 0x44;
    }

    PH_CHECK_SUCCESS_FCT(status, phhalHw_MfcAuthenticate(
            ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
            bBlockNumber,
            PHHAL_HW_MFC_KEYA,
            &aPublicKeyA[0],
            (((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->bUidLength == 4U) ?
                &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[0]: &((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->abUid[3]
    ));

    if(0U == (bSpillOver))
    {
        aCommand[0] = PH_ALMFCTOP_MFC_READ;
        aCommand[1] = bBlockNumber;

        PH_CHECK_SUCCESS_FCT(status, phhalHw_Exchange(
                ((phpalI14443p3a_Sw_DataParams_t*)(pMfcTop->pPalI14443paDataParams))->pHalDataParams,
                PH_EXCHANGE_DEFAULT,
                aCommand,
                2,
                &pRxBuffer,
                &wRxLength));

        (void)memcpy(&aCommand[0], pRxBuffer, 16);
        aCommand[(pMfcTop->bNdefMessageStart % PHAL_MFCTOP_BLOCK_SIZE) + 1U] = 0x00;
    }
    else
    {
        aCommand[0] = 0x00;
    }
    PH_CHECK_SUCCESS_FCT(status, phalMfcTop_Sw_Write(pDataParams, aCommand, bBlockNumber));


    /* Update state variables */
    pDataParams->bTagState = PHAL_TOP_STATE_INITIALIZED;
    pDataParams->wNdefLength = 0;

    return PH_ERR_SUCCESS;
}

static void phMemCopy(void* dest, void* src, uint16_t wLength)
{
    uint32_t dwLength = 0;
    dwLength = wLength;
    (void)memcpy(dest, src, dwLength);

}

#endif /* NXPBUILD__PHPAL_I14443P3A_SW*/
#endif /* NXPBUILD__PHAL_TOP_SW */

