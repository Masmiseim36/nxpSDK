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
* NFC Library Top Level API of Reader Library Framework.
* $Author: jenkins_ cm (nxp92197) $
* $Revision: 4184 $ (v06.01.00)
* $Date: 2016-08-29 18:04:59 +0530 (Fri, 29 Aug 2016) $
*
* History:
*  KK: Generated 29. Aug 2016
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PH_NFCLIB_ISO_15693

#include "phNfcLib_Initialization.h"
#include "phNfcLib_Int.h"

#define PH_NFCLIB_15693_TIMEOUT_SHORT_US    384U
#define PH_NFCLIB_15693_TIMEOUT_LONG_US   20060U


#define PH_NFCLIB_15693_FLAGS_DSFID    0x01U   /**< Response flag DSFID. */
#define PH_NFCLIB_15693_FLAGS_AFI      0x02U   /**< Response flag AFI. */
#define PH_NFCLIB_15693_FLAGS_MEMSIZE  0x04U   /**< Response flag MEMSIZE. */
#define PH_NFCLIB_15693_FLAGS_ICREF    0x08U   /**< Response flag ICREF. */


#define PH_NFCLIB_15693_READ_SINGLE_BLOCK                 0x20U    /**< ISO15693 read single block command. */
#define PH_NFCLIB_15693_WRITE_SINGLE_BLOCK                0x21U    /**< ISO15693 write single block command. */
#define PH_NFCLIB_15693_LOCK_BLOCK                        0x22U    /**< ISO15693 lock block command. */
#define PH_NFCLIB_15693_READ_MULTIPLE_BLOCKS              0x23U    /**< ISO15693 read multiple blocks command. */
#define PH_NFCLIB_15693_WRITE_MULTIPLE_BLOCKS             0x24U    /**< ISO15693 write multiple blocks command. */
#define PH_NFCLIB_15693_WRITE_AFI                         0x27U    /**< ISO15693 write Application Family Identifier command. */
#define PH_NFCLIB_15693_LOCK_AFI                          0x28U    /**< ISO15693 lock Application Family Identifier command. */
#define PH_NFCLIB_15693_WRITE_DSFID                       0x29U    /**< ISO15693 write Data Storage Format IDentifier command. */
#define PH_NFCLIB_15693_LOCK_DSFID                        0x2AU    /**< ISO15693 lock Data Storage Format IDentifier command. */
#define PH_NFCLIB_15693_GET_SYSTEM_INFO                   0x2BU    /**< ISO15693 get system information command. */
#define PH_NFCLIB_15693_GET_MULTIPLE_BLOCK_SEC            0x2CU    /**< ISO15693 get multiple block security status command. */

#define PH_NFCLIB_15693_EXTENDED_READ_SINGLE_BLOCK        0x30U    /**< ISO15693 extended read single block command. */
#define PH_NFCLIB_15693_EXTENDED_WRITE_SINGLE_BLOCK       0x31U    /**< ISO15693 extended write single block command. */
#define PH_NFCLIB_15693_EXTENDED_LOCK_BLOCK               0x32U    /**< ISO15693 extended lock block command. */
#define PH_NFCLIB_15693_EXTENDED_READ_MULTIPLE_BLOCKS     0x33U    /**< ISO15693 extended read multiple blocks command. */
#define PH_NFCLIB_15693_EXTENDED_WRITE_MULTIPLE_BLOCKS    0x34U    /**< ISO15693 extended write multiple blocks command. */
#define PH_NFCLIB_15693_EXTENDED_GET_SYSTEM_INFO          0x3BU    /**< ISO15693 extended get system information command. */
#define PH_NFCLIB_15693_EXTENDED_GET_MULTIPLE_BLOCK_SEC   0x3CU    /**< ISO15693 extended get multiple block security status command. */


static phStatus_t NfcLib_WriteAlikeHandling(
                                     phStatus_t wExchangeStatus
                                     );

phStatus_t phNfcLib_ISO15693_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength)
{
    phStatus_t   statusTmp = PH_ERR_INVALID_PARAMETER;
    uint8_t      bCommand[5];
    uint8_t bExpLength = 0;


    (void)phpalSli15693_SetSerialNo(&gphNfcLib_Params.spalSli15693,
        ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bUid,
        8);
    /*set the addressed bit */
    gphNfcLib_Params.spalSli15693.bFlags |= PHPAL_SLI15693_FLAG_ADDRESSED;

    /*reset the selected bit */
    gphNfcLib_Params.spalSli15693.bFlags  = gphNfcLib_Params.spalSli15693.bFlags &(uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_SELECTED;

    /* Set Option bit */
    if (0u !=  (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption))
    {
        gphNfcLib_Params.spalSli15693.bFlags |= PHPAL_SLI15693_FLAG_OPTION;
    }
    /* Clear option bit */
    else
    {
        gphNfcLib_Params.spalSli15693.bFlags  = gphNfcLib_Params.spalSli15693.bFlags &(uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_OPTION;
    }

    /* lOGIC depends upon the enum value please make sure whether the api needs PH_NFCLIB_15693_TIMEOUT_LONG_US or
     * PH_NFCLIB_15693_TIMEOUT_SHORT_US before changing this
     */
    if(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bCommand > ISO15693_Extended_GetMultipleBlockSecurityStatus)
    {
        /* Set long timeout. */
        (void)phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            PH_NFCLIB_15693_TIMEOUT_LONG_US);
    }
    else
    {
        /* Set short timeout. */
        (void)phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            PH_NFCLIB_15693_TIMEOUT_SHORT_US);
    }


    switch(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bCommand)
    {

    case ISO15693_ReadSingleBlock:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_READ_SINGLE_BLOCK;
        bCommand[1] = (uint8_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber;

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            2,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Length check */
        if (0u != (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption))
        {
            if (gphNfcLib_State.wRxLength < 2U)
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }
        else
        {
            if (gphNfcLib_State.wRxLength == 0U)
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }
        break;

    case ISO15693_Extended_ReadSingleBlock:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_EXTENDED_READ_SINGLE_BLOCK;
        bCommand[1] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber);
        bCommand[2] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber >> 8U);

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            3,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Length check */
        if (0u != (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption))
        {
            if (gphNfcLib_State.wRxLength < (4u + 1U))
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }
        else
        {
            if (gphNfcLib_State.wRxLength < 4U)
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }

        break;

    case ISO15693_ReadMultipleBlocks:

        /* Number of wNumBlocks can't be zero */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks == 0U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* adjust number of blocks. */
        --((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Check number of blocks doesn't exceed 256 */
        if (((uint16_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber + ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks) >= 0x100U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_READ_MULTIPLE_BLOCKS;
        bCommand[1] = (uint8_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber;
        bCommand[2] =(uint8_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            3,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Length check */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            if (gphNfcLib_State.wRxLength <((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks + 1U) * 2U))
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }
        else
        {
            if (gphNfcLib_State.wRxLength <(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks + 1U))
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }
        break;

    case ISO15693_Extended_ReadMultipleBlocks:

        /* Number of wNumBlocks can't be zero */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks == 0U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* adjust number of blocks. */
        --((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Check number of blocks doesn't exceed 256 */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber + ((uint32_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks))) >= 0x10000U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_EXTENDED_READ_MULTIPLE_BLOCKS;
        bCommand[1] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber);
        bCommand[2] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber >> 8U);
        bCommand[3] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks);
        bCommand[4] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks >> 8U);

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            3,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Length check */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            if (gphNfcLib_State.wRxLength < ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks + 1U) * (4u + 1U)))
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }
        else
        {
            if (gphNfcLib_State.wRxLength < ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks + 1U) * 4U))
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }

        break;

    case ISO15693_WriteSingleBlock:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_WRITE_SINGLE_BLOCK;
        bCommand[1] = (uint8_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber;

        /* Proceed with the command in lower layers */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_BUFFER_FIRST,
            bCommand,
            2,
            NULL,
            NULL));

        /* Append given data */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_BUFFER_LAST,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.pBuffer,
            wTxBufferLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }
        break;

    case ISO15693_Extended_WriteSingleBlock:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_EXTENDED_WRITE_SINGLE_BLOCK;
        bCommand[1] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber);
        bCommand[2] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber >> 8U);


        /* Proceed with the command in lower layers */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_BUFFER_FIRST,
            bCommand,
            3,
            NULL,
            NULL));

        /* Append given data */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_BUFFER_LAST,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.pBuffer,
            wTxBufferLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }

        break;

    case ISO15693_WriteMultipleBlocks:

        /* Number of wNumBlocks can't be zero */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks == 0U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* adjust number of blocks. */
        --((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Check number of blocks doesn't exceed 256 */
        if (((uint16_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber + ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks) >= 0x100U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_WRITE_MULTIPLE_BLOCKS;
        bCommand[1] = (uint8_t) ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber ;
        bCommand[2] =(uint8_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Proceed with the command in lower layers */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_BUFFER_FIRST,
            bCommand,
            3,
            NULL,
            NULL));

        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_BUFFER_LAST,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.pBuffer,
            wTxBufferLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }

        break;

    case ISO15693_Extended_WriteMultipleBlocks:

        /* Number of wNumBlocks can't be zero */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks == 0U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* adjust number of blocks. */
        --((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Check number of blocks doesn't exceed 256 */
        if (((uint16_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber + ((uint32_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks))) >= 0x10000U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_EXTENDED_WRITE_MULTIPLE_BLOCKS;
        bCommand[1] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber);
        bCommand[2] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber >> 8U);
        bCommand[3] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks);
        bCommand[4] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks >> 8U);


        /* Proceed with the command in lower layers */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_BUFFER_FIRST,
            bCommand,
            5,
            NULL,
            NULL));

        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_BUFFER_LAST,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.pBuffer,
            wTxBufferLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }

        break;

    case ISO15693_LockBlock:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_LOCK_BLOCK;
        bCommand[1] = (uint8_t) ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber;

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            2,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }

        break;

    case ISO15693_Extended_LockBlock:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_EXTENDED_LOCK_BLOCK;
        bCommand[1] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber);
        bCommand[2] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber >> 8U);


        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            3,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }
        break;

    case ISO15693_WriteAFI:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_WRITE_AFI;
        bCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bAfi;


        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            2,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }

        break;

    case ISO15693_LockAFI:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_LOCK_AFI;

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            1,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }

        break;

    case ISO15693_WriteDSFID:

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_WRITE_DSFID;
        bCommand[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bDsfid;


        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            2,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }

        break;

    case ISO15693_LockDSFID:
        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_LOCK_DSFID;

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            1,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Write-alike handling */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.bOption) != 0U)
        {
            return NfcLib_WriteAlikeHandling(statusTmp);
        }
        break;

    case ISO15693_GetSystemInformation:
        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_GET_SYSTEM_INFO;

        /* Clear Option bit */
        gphNfcLib_Params.spalSli15693.bFlags  = gphNfcLib_Params.spalSli15693.bFlags &(uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_OPTION;

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            1,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Check number of data received */
        if (gphNfcLib_State.wRxLength == 0U)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Expected length is 9 by default */
        bExpLength = 9;

        /* Check for DSFID byte */
        if (0U != ((gphNfcLib_State.pRxBuffer[0]) & PH_NFCLIB_15693_FLAGS_DSFID))
        {
            ++bExpLength;
        }

        /* Check for AFI byte */
        if (0U != ((gphNfcLib_State.pRxBuffer[0]) & PH_NFCLIB_15693_FLAGS_AFI))
        {
            ++bExpLength;
        }

        /* Check for MEMSIZE bytes */
        if (0U != ((gphNfcLib_State.pRxBuffer[0]) & PH_NFCLIB_15693_FLAGS_MEMSIZE))
        {
            bExpLength += 2U;
        }

        /* Check for ICREF byte */
        if (0U != ((gphNfcLib_State.pRxBuffer[0]) & PH_NFCLIB_15693_FLAGS_ICREF))
        {
            ++bExpLength;
        }

        /* Check response length */
        if (gphNfcLib_State.wRxLength < bExpLength)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        break;

    case ISO15693_ExtendedGetSystemInformation:
        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_EXTENDED_GET_SYSTEM_INFO;

        /* Clear Option bit */
        gphNfcLib_Params.spalSli15693.bFlags  = gphNfcLib_Params.spalSli15693.bFlags &(uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_OPTION;

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            1,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Check number of data received */
        if (gphNfcLib_State.wRxLength == 0U)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Expected length is 9 by default */
        bExpLength = 9;

        /* Check for DSFID byte */
        if (0U != ((gphNfcLib_State.pRxBuffer[0]) & PH_NFCLIB_15693_FLAGS_DSFID))
        {
            ++bExpLength;
        }

        /* Check for AFI byte */
        if (0U != ((gphNfcLib_State.pRxBuffer[0]) & PH_NFCLIB_15693_FLAGS_AFI))
        {
            ++bExpLength;
        }

        /* Check for MEMSIZE bytes */
        if (0U != ((gphNfcLib_State.pRxBuffer[0]) & PH_NFCLIB_15693_FLAGS_MEMSIZE))
        {
            bExpLength += 3U;
        }

        /* Check for ICREF byte */
        if (0U != ((gphNfcLib_State.pRxBuffer[0]) & PH_NFCLIB_15693_FLAGS_ICREF))
        {
            ++bExpLength;
        }

        /* Check response length */
        if (gphNfcLib_State.wRxLength < bExpLength)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        break;

    case ISO15693_GetMultipleBlockSecurityStatus:
        /* Number of wNumBlocks can't be zero */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks == 0U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* adjust number of blocks. */
        --((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Check number of blocks doesn't exceed 256 */
        if (((uint16_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber + ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks) >= 0x100U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* Clear Option bit */
        gphNfcLib_Params.spalSli15693.bFlags  = gphNfcLib_Params.spalSli15693.bFlags &(uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_OPTION;

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_GET_MULTIPLE_BLOCK_SEC;
        bCommand[1] = (uint8_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber;
        bCommand[2] = (uint8_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            3,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        break;

    case ISO15693_Extended_GetMultipleBlockSecurityStatus:

        /* Number of wNumBlocks can't be zero */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks == 0U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* adjust number of blocks. */
        --((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks;

        /* Check number of blocks doesn't exceed 256 */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber + ((uint32_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks))) >= 0x10000U)
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* Clear Option bit */
        gphNfcLib_Params.spalSli15693.bFlags  = gphNfcLib_Params.spalSli15693.bFlags &(uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_OPTION;

        /* Build command frame */
        bCommand[0] = PH_NFCLIB_15693_EXTENDED_GET_MULTIPLE_BLOCK_SEC;
        bCommand[1] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber);
        bCommand[2] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wBlockNumber >> 8U);
        bCommand[3] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks);
        bCommand[4] = (uint8_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks >> 8U);

        /* Proceed with the command in lower layers */
        statusTmp = phpalSli15693_Exchange(
            &gphNfcLib_Params.spalSli15693,
            PH_EXCHANGE_DEFAULT,
            bCommand,
            5,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Check response length */
        if (gphNfcLib_State.wRxLength != (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO15693.wNumBlocks + 1U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        break;

    default:
        break;
    }

    if ((statusTmp & PH_ERR_MASK) == PHPAL_SLI15693_ERR_ISO15693)
    {
        return PH_ERR_PROTOCOL_ERROR;
    }
    return statusTmp;
}

static phStatus_t NfcLib_WriteAlikeHandling(
                                     phStatus_t wExchangeStatus
                                     )
{
    uint8_t      bDsfid;
    uint8_t      bUid[PHPAL_SLI15693_UID_LENGTH];
    uint8_t      bUidLength;
    uint8_t      bData[1];
    uint16_t     wDataLength = 0;

    switch (wExchangeStatus & PH_ERR_MASK)
    {
        /* Check for protocol error  */
    case PH_ERR_SUCCESS:
        return PH_ERR_PROTOCOL_ERROR;

        /* Timeout is correct behaviour, send EOF */
    case PH_ERR_IO_TIMEOUT:
        /* card answers after next EOF -> correct status is timeout */
        return phpalSli15693_SendEof(
            &gphNfcLib_Params.spalSli15693,
            PHPAL_SLI15693_EOF_WRITE_ALIKE,
            &bDsfid,
            bUid,
            &bUidLength,
            bData,
            &wDataLength);

        /* Framing errors etc. are ignored and the waiting until EOF sending is continued */
    case PH_ERR_INTEGRITY_ERROR:
    case PH_ERR_COLLISION_ERROR:
    case PH_ERR_FRAMING_ERROR:
        return phpalSli15693_SendEof(
            &gphNfcLib_Params.spalSli15693,
            PHPAL_SLI15693_EOF_WRITE_ALIKE_WITH_WAIT,
            &bDsfid,
            bUid,
            &bUidLength,
            bData,
            &wDataLength);

        /* Directly return all other errors */
    default:
        return wExchangeStatus;
    }
}
#endif /* NXPBUILD__PH_NFCLIB_ISO_15693*/
