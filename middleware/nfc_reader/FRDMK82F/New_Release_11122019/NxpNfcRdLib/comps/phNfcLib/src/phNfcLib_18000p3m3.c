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
* $Date: 2016-08-30 18:04:59 +0530 (Fri, 30 Aug 2016) $
*
* History:
*  KK: Generated 30. Aug 2016
*/

#include <ph_Status.h>


#ifdef NXPBUILD__PH_NFCLIB_ISO_18000

#include "phNfcLib_Initialization.h"
#include "phNfcLib_Int.h"

#define PH_NFCLIB_18000P3M3_CMD_REQ_RN          0xC1U    /**< ReqRn command code. */
#define PH_NFCLIB_18000P3M3_CMD_READ            0xC2U    /**< Read command code. */
#define PH_NFCLIB_18000P3M3_CMD_WRITE           0xC3U    /**< Write command code. */
#define PH_NFCLIB_18000P3M3_CMD_KILL            0xC4U    /**< Kill command code. */
#define PH_NFCLIB_18000P3M3_CMD_LOCK            0xC5U    /**< Lock command code. */
#define PH_NFCLIB_18000P3M3_CMD_ACCESS          0xC6U    /**< Access command code. */
#define PH_NFCLIB_18000P3M3_CMD_BLOCKWRITE      0xC7U    /**< BlockWrite command code. */
#define PH_NFCLIB_18000P3M3_CMD_BLOCKERASE      0xC8U    /**< BlockErase command code. */
#define PH_NFCLIB_18000P3M3_CMD_BLOCKPERMALOCK  0xC9U    /**< BlockPermaLock command code. */
#define PH_NFCLIB_18000P3M3_CMD_ACK             0x01U    /**< ACK command code. */


#define PH_NFCLIB_18000P3M3_SW_T2_MIN_US        151U  /* T2 Time for 18000p3m3 */
#define PH_NFCLIB_18000P3M3_SW_TIMEOUT          113U  /* Timeout for 18000p3m3 */

#define PH_NFCLIB_18000P3M3_SKIP_COMMAND        0xFFU   /**< Skip the command */

static phStatus_t phNfcLib_18000p3m3_ReqRN(void);
static phStatus_t phNfcLib_18000p3m3_Exchange(uint8_t bSymbol, uint8_t bTxLastBits, uint16_t wTimeout, uint8_t* pCmd, uint16_t wCmdLength, uint16_t *pRxLastBits);

phStatus_t phNfcLib_ISO18000_Transmit(void * const pTxBuffer, uint16_t wTxBufferLength)
{
    uint8_t    bCmd[10] = {0};
    uint16_t   wCmdLength = 0;
    phStatus_t statusTmp = PH_ERR_INVALID_PARAMETER;
    uint8_t    bIndex;
    uint8_t    bUiiMaskBytes = 0;
    uint8_t    bHandle[2] = {0};
    uint16_t   wRxLastBits = 0;
    uint8_t    aSelectCmd[39] = {0};
    uint8_t    wSelectCmdLen;
    uint8_t    bSelectCmdValidBits;
    uint8_t    aBeginRoundCmd[3] = {0};
    uint8_t    bUiiStartAddress = 0x20;

  if   ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength % 8U) != 0U)
  {
		bUiiMaskBytes = (((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength)/8U) + 1U);
  }
  else
  {
    	bUiiMaskBytes = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength)/8U;
  }

    #if 0
    ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength % 8U) != 0U) ? (bUiiMaskBytes = (((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength)/8U) + 1U)) : (bUiiMaskBytes = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength)/8U);


    /* Since truncation option is not set thus logically the chance of it getting non complete bytes is very rare */
    bUiiMaskBytes = ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength % 8U) ?
                    ((((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength)/8U) + 1U)) :
                    ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength)/8U));
#endif

    if(0 != (memcmp(gphNfcLib_State.pActivatedUid, ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pUii, bUiiMaskBytes)))
    {
        /* Create select command frame (with selected UII in mask) */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_CreateSelectCmd(
            gphNfcLib_Params.sDiscLoop.pPal18000p3m3DataParams,
            PHPAL_I18000P3M3_TARGET_INVENTORIED_S0,
            0,
            PHPAL_I18000P3M3_MEMBANK_UII,
            &bUiiStartAddress,
            0,
            ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pUii,
            (uint8_t)((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.wUiiMaskLength,
            0,
            aSelectCmd,
            &wSelectCmdLen,
            &bSelectCmdValidBits));

        /* Create BeginRound command frame */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI18000p3m3_CreateBeginRoundCmd(
            gphNfcLib_Params.sDiscLoop.pPal18000p3m3DataParams,
            gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.bDr,
            gphNfcLib_Params.sDiscLoop.sI18000p3m3TargetInfo.bM,
            (uint8_t)PH_OFF,
            PHPAL_I18000P3M3_SEL_ALL_00,
            PHPAL_I18000P3M3_SESSION_S0,
            0,
            0,
            aBeginRoundCmd));

        /* Activate tag with specified UII */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_I18000p3m3Inventory(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            aSelectCmd,
            (uint8_t)wSelectCmdLen,
            bSelectCmdValidBits,
            aBeginRoundCmd,
            PHHAL_HW_I18000P3M3_GET_TAG_HANDLE,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength));

        /* Update handle */
        /* Update the activated UII here */
        gphNfcLib_State.pActivatedUid = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pUii;
        (void)memcpy(&gphNfcLib_Params.salI18000p3m3.abHandle[0], &gphNfcLib_State.pRxBuffer[gphNfcLib_State.wRxLength - 2u], 2);
        gphNfcLib_Params.salI18000p3m3.bHandleValid = PH_ON;
    }


    /* TODO: If ever the handle concept changes the check has to be performed here */
    /* Valid Handle is mandatory */
    if (gphNfcLib_Params.salI18000p3m3.bHandleValid == PH_OFF)
    {
        return PH_ERR_USE_CONDITION;
    }

    switch(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bCommand)
    {
    case ISO18000_Ack:

        /* Build Command frame */
        bCmd[0] = (PH_NFCLIB_18000P3M3_CMD_ACK << 6U) | (gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 2U);    /* 0 bits left */
        bCmd[1] = (gphNfcLib_Params.salI18000p3m3.abHandle[0] << 6U) | (gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 2U);                    /* 0 bits left */
        bCmd[2] = (gphNfcLib_Params.salI18000p3m3.abHandle[1] << 6U);                                      /* 6 bits left */

        /* Set frame-sync as start symbol */
        (void)phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_SYMBOL_START,
            PHHAL_HW_SYMBOL_I18000P3M3_FSYNC);

        /* Set TxWait to T2 */
        (void) phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_TXWAIT_US,
            PH_NFCLIB_18000P3M3_SW_T2_MIN_US);


        /* Set T1+T3 Timeout */
        (void)phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            PH_NFCLIB_18000P3M3_SW_TIMEOUT);


        /* Switch off HAL-CRC */
        (void)phhalHw_SetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF);
        (void)phhalHw_SetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF);

        statusTmp = phNfcLib_18000p3m3_Exchange(PH_NFCLIB_18000P3M3_SKIP_COMMAND, 0x02, PH_NFCLIB_18000P3M3_SKIP_COMMAND, &bCmd[0], 3, &wRxLastBits);

        /* Switch on HAL-CRC from now on */
        (void)phhalHw_SetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON);
        (void)phhalHw_SetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON);

        /* set Tx-CRC as CRC16  */
        (void)phhalHw_SetConfig(gphNfcLib_Params.sDiscLoop.pHalDataParams, PHHAL_HW_CONFIG_CRCTYPE, PHHAL_HW_CRCTYPE_CRC16|PHHAL_HW_CRCOPTION_INVERTED);

        if(statusTmp == PH_ERR_SUCCESS)
        {

        /* Get bit length */
        if (0U != (wRxLastBits & 0x07U))
        {
            --gphNfcLib_State.wRxLength;
        }
        wRxLastBits = (uint16_t)((gphNfcLib_State.wRxLength << 3U) + wRxLastBits);

        /* Check response length */
        if ((wRxLastBits < 5U) || (wRxLastBits > 528U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        }
        break;

    case ISO18000_ReqRn:

        statusTmp = phNfcLib_18000p3m3_ReqRN();

        break;

    case ISO18000_Read:

        /* Parameter check */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank > 0x03U) || (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength > 3U))
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        (void)phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
            (uint16_t)0x158);

        /* prepare command header */
        bCmd[wCmdLength++] = PH_NFCLIB_18000P3M3_CMD_READ;
        bCmd[wCmdLength]   = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank << 6U) | (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength << 4U);  /* 4 bits left */


        /* append pointer */
        for (bIndex = 0; bIndex < (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength + 1U); ++bIndex)
        {
            bCmd[wCmdLength++] |= (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pWordPtr[bIndex]) >> 4U;
            bCmd[wCmdLength]    = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pWordPtr[bIndex]) << 4U;
        }
        /* 4 bits left */

        /* append word-count */
        bCmd[wCmdLength++] |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount >> 4U;  /* 0 bits left */
        bCmd[wCmdLength]    = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount << 4U;  /* 4 bits left */

        /* TODO: if ever the handle concept changes the change has to be done here */
        /* append handle */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 4U;    /* 0 bits left */
        bCmd[wCmdLength]    = gphNfcLib_Params.salI18000p3m3.abHandle[0] << 4U;    /* 4 bits left */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 4U;    /* 0 bits left */
        bCmd[wCmdLength++]  = gphNfcLib_Params.salI18000p3m3.abHandle[1] << 4U;    /* 4 bits left */

        PH_CHECK_SUCCESS_FCT(statusTmp, phNfcLib_18000p3m3_Exchange(PH_OFF, 0x04, PH_NFCLIB_18000P3M3_SKIP_COMMAND, &bCmd[0], wCmdLength, &wRxLastBits));

        /* Check received length and header bit */
        if (((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount != 0x00U) &&
            (gphNfcLib_State.wRxLength != ((((uint16_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount)) << 1U) + 3U))) ||
            (wRxLastBits != 1U) || ((gphNfcLib_State.pRxBuffer[0] & 0x80U) != 0U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        for (bIndex = 0; bIndex < (gphNfcLib_State.wRxLength - 1U); bIndex++)
        {
            gphNfcLib_State.pRxBuffer[bIndex] = (gphNfcLib_State.pRxBuffer[bIndex] << 1U) |
                                                (gphNfcLib_State.pRxBuffer[bIndex + 1U] >> 7U);
        }

        /* Status byte is removed */
        gphNfcLib_State.wRxLength--;

        /* TODO: if ever the handle concept changes the change has to be done here */
        /* Check Handle */
        if ((gphNfcLib_State.pRxBuffer[gphNfcLib_State.wRxLength - 2u] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (gphNfcLib_State.pRxBuffer[gphNfcLib_State.wRxLength - 1u] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Handle is not part of data */
        gphNfcLib_State.wRxLength -= 2u;

        break;

    case ISO18000_Write:

        /* Parameter check */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank  > 0x03U) ||
            (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength > 3U) ||
            ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption != PH_NFCLIB_18000P3M3_AC_NO_COVER_CODING) &&
            (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption != PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING)))
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* prepare command header */
        wCmdLength = 0;
        bCmd[wCmdLength++] = PH_NFCLIB_18000P3M3_CMD_WRITE;
        bCmd[wCmdLength]   = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank << 6U) |
                             (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength << 4U);  /* 4 bits left */

        /* append pointer */
        for (bIndex = 0; bIndex < (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength + 1U); ++bIndex)
        {
            bCmd[wCmdLength++] |= (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pWordPtr[bIndex]) >> 4U;
            bCmd[wCmdLength]    = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pWordPtr[bIndex]) << 4U;
        }
        /* 4 bits left */

        /* append data */
        bCmd[wCmdLength++] |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pBuffer[0] >> 4U;                        /* 0 bits left */
        bCmd[wCmdLength++]  = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pBuffer[0] << 4U) |
                              (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pBuffer[1] >> 4U);                      /* 0 bits left */
        bCmd[wCmdLength]    = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pBuffer[1] << 4U;                        /* 4 bits left */

        /* perform cover-coding if neccessary */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption == PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING)
        {
            /* New RN16 needs to be obtained for cover coding */
            (void)phNfcLib_18000p3m3_ReqRN();

            /* perform XOR operation */
            bCmd[wCmdLength - 2u] ^= gphNfcLib_State.pRxBuffer[0] >> 4U;
            bCmd[wCmdLength - 1u] ^= (gphNfcLib_State.pRxBuffer[0] << 4U) |  (gphNfcLib_State.pRxBuffer[1] >> 4U);
            bCmd[wCmdLength]     ^= gphNfcLib_State.pRxBuffer[1] << 4U;
        }

        /* append handle */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 4U;    /* 0 bits left */
        bCmd[wCmdLength]    = gphNfcLib_Params.salI18000p3m3.abHandle[0] << 4U;    /* 4 bits left */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 4U;    /* 0 bits left */
        bCmd[wCmdLength++]  = gphNfcLib_Params.salI18000p3m3.abHandle[1] << 4U;    /* 4 bits left */


        PH_CHECK_SUCCESS_FCT(statusTmp, phNfcLib_18000p3m3_Exchange(PH_OFF, 0x04, 5, &bCmd[0], wCmdLength, &wRxLastBits));

        /* Check header bit */
        if ((gphNfcLib_State.wRxLength != 3U) || (wRxLastBits != 1U) || ((gphNfcLib_State.pRxBuffer[0] & 0x80U) != 0U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check Handle */
        bHandle[0] = (uint8_t)((gphNfcLib_State.pRxBuffer[0] << 1U) | (gphNfcLib_State.pRxBuffer[1] >> 7U));
        bHandle[1] = (uint8_t)((gphNfcLib_State.pRxBuffer[1] << 1U) | (gphNfcLib_State.pRxBuffer[2] >> 7U));
        if ((bHandle[0] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (bHandle[1] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Making Received length as 0 since no user data has been received */
        gphNfcLib_State.wRxLength = 0;
        break;

    case ISO18000_Kill:

        /* Parameter check */
        if (((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption != PH_NFCLIB_18000P3M3_AC_NO_COVER_CODING) &&
            (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption != PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING)) ||
            (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bRecom > 0x07U))
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* */
        /* Perform FIRST PART of KILL command */
        /* */

        /* prepare command header */
        bCmd[0] = PH_NFCLIB_18000P3M3_CMD_KILL;
        bCmd[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pPassword[0];
        bCmd[2] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pPassword[1];

        /* perform cover-coding if necessary */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption == PHAL_I18000P3M3_AC_USE_COVER_CODING)
        {
            /* retrieve RN16 */
            PH_CHECK_SUCCESS_FCT(statusTmp,phNfcLib_18000p3m3_ReqRN());

            /* perform XOR operation */
            bCmd[1] ^= gphNfcLib_State.pRxBuffer[0];
            bCmd[2] ^= gphNfcLib_State.pRxBuffer[1];
        }

        /* append RFU/Recom bits (000) and handle */
        bCmd[3]  = 0x00;                            /* 5 bits left */
        bCmd[3] |= gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 3U;   /* 0 bits left */
        bCmd[4]  = gphNfcLib_Params.salI18000p3m3.abHandle[0] << 5U;   /* 5 bits left */
        bCmd[4] |= gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 3U;   /* 0 bits left */
        bCmd[5]  = gphNfcLib_Params.salI18000p3m3.abHandle[1] << 5U;   /* 5 bits left */

        PH_CHECK_SUCCESS_FCT(statusTmp, phNfcLib_18000p3m3_Exchange(PH_OFF, 0x03, 500, &bCmd[0], 6, NULL));

        /* Check received length */
        if (gphNfcLib_State.wRxLength != 2U)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check Handle */
        if ((gphNfcLib_State.pRxBuffer[0] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (gphNfcLib_State.pRxBuffer[1] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /*
        * Perform second part of kill command
        */

        /* prepare command header */
        bCmd[0] = PH_NFCLIB_18000P3M3_CMD_KILL;
        bCmd[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pPassword[2];
        bCmd[2] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pPassword[3];

        /* perform cover-coding if neccessary */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption == PHAL_I18000P3M3_AC_USE_COVER_CODING)
        {
            /* retrieve RN16 */
            /* retrieve RN16 */
            PH_CHECK_SUCCESS_FCT(statusTmp,phNfcLib_18000p3m3_ReqRN());

            /* perform XOR operation */
            bCmd[1] ^= gphNfcLib_State.pRxBuffer[0];
            bCmd[2] ^= gphNfcLib_State.pRxBuffer[1];
        }

        /* append RFU/Recom bits (000) and handle */
        bCmd[3]  = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bRecom << 5U;                     /* 5 bits left */
        bCmd[3] |= gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 3U;   /* 0 bits left */
        bCmd[4]  = gphNfcLib_Params.salI18000p3m3.abHandle[0] << 5U;   /* 5 bits left */
        bCmd[4] |= gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 3U;   /* 0 bits left */
        bCmd[5]  = gphNfcLib_Params.salI18000p3m3.abHandle[1] << 5U;   /* 5 bits left */

        PH_CHECK_SUCCESS_FCT(statusTmp, phNfcLib_18000p3m3_Exchange(PH_OFF, 0x03, PH_NFCLIB_18000P3M3_SKIP_COMMAND, &bCmd[0], 6, &wRxLastBits));

        /* Check received length and header */
        if ((gphNfcLib_State.wRxLength != 3U) ||
            (wRxLastBits != 1U) ||
            ((gphNfcLib_State.pRxBuffer[0] & 0x80U) != 0U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check Handle */
        bHandle[0] = (uint8_t)((gphNfcLib_State.pRxBuffer[0] << 1U) | (gphNfcLib_State.pRxBuffer[1] >> 7U));
        bHandle[1] = (uint8_t)((gphNfcLib_State.pRxBuffer[1] << 1U) | (gphNfcLib_State.pRxBuffer[2] >> 7U));
        if ((bHandle[0] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (bHandle[1] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Making Received length as 0 since no user data has been received */
        gphNfcLib_State.wRxLength = 0;
        break;

    case ISO18000_Lock:

        /* prepare command header */
        bCmd[0] = PH_NFCLIB_18000P3M3_CMD_LOCK;                                     /* 0 bits left */
        bCmd[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pMask[0];                                                     /* 0 bits left */
        bCmd[2] = (uint8_t)((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pMask[1] & 0xC0U) | (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pAction[0] >> 2U));             /* 0 bits left */
        bCmd[3] = (uint8_t)((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pAction[0] << 6U) | ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pAction[1] & 0xC0U) >> 2U));    /* 4 bits left */

        /* append handle */
        bCmd[3] |= gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 4U;   /* 0 bits left */
        bCmd[4]  = gphNfcLib_Params.salI18000p3m3.abHandle[0] << 4U;   /* 4 bits left */
        bCmd[4] |= gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 4U;   /* 0 bits left */
        bCmd[5]  = gphNfcLib_Params.salI18000p3m3.abHandle[1] << 4U;   /* 4 bits left */

        PH_CHECK_SUCCESS_FCT(statusTmp, phNfcLib_18000p3m3_Exchange(PH_OFF, 0x04, 5, &bCmd[0], 6, &wRxLastBits));

        /* Check received length and header */
        if ((gphNfcLib_State.wRxLength != 3U) ||
            (wRxLastBits != 1U) ||
            ((gphNfcLib_State.pRxBuffer[0] & 0x80U) != 0U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check Handle */
        bHandle[0] = (uint8_t)((gphNfcLib_State.pRxBuffer[0] << 1U) | (gphNfcLib_State.pRxBuffer[1] >> 7U));
        bHandle[1] = (uint8_t)((gphNfcLib_State.pRxBuffer[1] << 1U) | (gphNfcLib_State.pRxBuffer[2] >> 7U));
        if ((bHandle[0] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (bHandle[1] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        /* Making Received length as 0 since no user data has been received */
        gphNfcLib_State.wRxLength = 0;
        break;

    case ISO18000_Access:

        /* Parameter check */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption != PH_NFCLIB_18000P3M3_AC_NO_COVER_CODING) &&
            (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption != PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING))
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* */
        /* Perform FIRST PART of ACCESS command */
        /* */

        /* prepare command header */
        bCmd[0] = PH_NFCLIB_18000P3M3_CMD_ACCESS;
        bCmd[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pPassword[0];
        bCmd[2] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pPassword[1];

        /* perform cover-coding if neccessary */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption  == PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING)
        {
            /* New RN16 needs to be obtained for cover coding */
            (void)phNfcLib_18000p3m3_ReqRN();

            /* perform XOR operation */
            bCmd[1] ^= gphNfcLib_State.pRxBuffer[0];
            bCmd[2] ^= gphNfcLib_State.pRxBuffer[1];

        }

        /* append handle */
        bCmd[3] = gphNfcLib_Params.salI18000p3m3.abHandle[0];
        bCmd[4] = gphNfcLib_Params.salI18000p3m3.abHandle[1];

        PH_CHECK_SUCCESS_FCT(statusTmp, phNfcLib_18000p3m3_Exchange(PH_OFF, PH_NFCLIB_18000P3M3_SKIP_COMMAND, PH_NFCLIB_18000P3M3_SKIP_COMMAND, &bCmd[0], 5, NULL));

        /* Check received length */
        if (gphNfcLib_State.wRxLength != 2U)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check Handle */
        if ((gphNfcLib_State.pRxBuffer[0] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (gphNfcLib_State.pRxBuffer[1] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* */
        /* Perform SECOND PART of ACCESS command */
        /* */

        /* prepare command header */
        bCmd[0] = PH_NFCLIB_18000P3M3_CMD_ACCESS;
        bCmd[1] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pPassword[2];
        bCmd[2] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pPassword[3];

        /* perform cover-coding if neccessary */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bOption == PH_NFCLIB_18000P3M3_AC_USE_COVER_CODING)
        {
            /* New RN16 needs to be obtained for cover coding */
            (void)phNfcLib_18000p3m3_ReqRN();

            /* perform XOR operation */
            bCmd[1] ^= gphNfcLib_State.pRxBuffer[0];
            bCmd[2] ^= gphNfcLib_State.pRxBuffer[1];
        }

        /* append handle */
        bCmd[3] = gphNfcLib_Params.salI18000p3m3.abHandle[0];
        bCmd[4] = gphNfcLib_Params.salI18000p3m3.abHandle[1];

        /* perform the exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            bCmd,
            5,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength));

        /* Check received length */
        if (gphNfcLib_State.wRxLength != 2U)
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check Handle */
        if ((gphNfcLib_State.pRxBuffer[0] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (gphNfcLib_State.pRxBuffer[1] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        /* Making Received length as 0 since no user data has been received */
        gphNfcLib_State.wRxLength = 0;
        break;

    case ISO18000_BlockWrite:

        /* Parameter check */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank > 0x03U) || (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength > 3U))
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* prepare command header */
        bCmd[wCmdLength++] = PH_NFCLIB_18000P3M3_CMD_BLOCKWRITE;
        bCmd[wCmdLength]   = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank << 6U) | (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength << 4U);  /* 4 bits left */

        /* append pointer */
        for (bIndex = 0; bIndex < (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength + 1U); ++bIndex)
        {
            bCmd[wCmdLength++] |= (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pWordPtr[bIndex]) >> 4U;
            bCmd[wCmdLength]    = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pWordPtr[bIndex]) << 4U;
        }
        /* 4 bits left */

        /* append (4bits) of Word-Count */
        bCmd[wCmdLength++] |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount >> 4U;  /* 0 bits left */

        (void)phNfcLib_18000p3m3_Exchange(PH_OFF, 0x04, 20, NULL, PH_NFCLIB_18000P3M3_SKIP_COMMAND, NULL);

        /* buffer the current contents */
        statusTmp = phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            bCmd,
            wCmdLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);


        /* reset command length*/
        wCmdLength = 0;

        /* append the rest bits of block-range*/
        bCmd[wCmdLength] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount << 4U;  /* 4 bits left */

        /* append data */
        for (bIndex = 0; bIndex < (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount << 1U); ++bIndex)
        {
            bCmd[wCmdLength++] |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pBuffer[bIndex] >> 4U;  /* 0 bits left */

            /* buffer the current contents if command buffer is full */
            if (wCmdLength == 7U)
            {
                /* buffer the current contents */
                statusTmp = phhalHw_Exchange(
                    gphNfcLib_Params.sDiscLoop.pHalDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    bCmd,
                    wCmdLength,
                    &gphNfcLib_State.pRxBuffer,
                    &gphNfcLib_State.wRxLength);

                /* reset command length*/
                wCmdLength = 0;
            }

            bCmd[wCmdLength] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pBuffer[bIndex] << 4U;  /* 4 bits left */
        }

        /* append handle */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 4U;    /* 0 bits left */

        /* buffer the current contents if command buffer is full */
        if (wCmdLength == 7U)
        {
            statusTmp = phhalHw_Exchange(
                gphNfcLib_Params.sDiscLoop.pHalDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                bCmd,
                wCmdLength,
                &gphNfcLib_State.pRxBuffer,
                &gphNfcLib_State.wRxLength);

            /* reset command length*/
            wCmdLength = 0;
        }

        bCmd[wCmdLength]    = gphNfcLib_Params.salI18000p3m3.abHandle[0] << 4U;    /* 4 bits left */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 4U;    /* 0 bits left */
        bCmd[wCmdLength++]  = gphNfcLib_Params.salI18000p3m3.abHandle[1] << 4U;    /* 4 bits left */

        /* perform the exchange */
        statusTmp = phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            bCmd,
            wCmdLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Ignore incomplete byte status, bail out on error */
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
        {
            (void)phhalHw_GetConfig(
                gphNfcLib_Params.sDiscLoop.pHalDataParams,
                PHHAL_HW_CONFIG_RXLASTBITS,
                &wRxLastBits);
            statusTmp = PH_ERR_SUCCESS;

        }

        if(statusTmp == PH_ERR_SUCCESS)
        {
        /* Check header bit */
        if ((gphNfcLib_State.wRxLength != 3U) || (wRxLastBits != 1U) || ((gphNfcLib_State.pRxBuffer[0] & 0x80U) != 0U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check Handle */
        bHandle[0] = (uint8_t)((gphNfcLib_State.pRxBuffer[0] << 1U) | (gphNfcLib_State.pRxBuffer[1] >> 7U));
        bHandle[1] = (uint8_t)((gphNfcLib_State.pRxBuffer[1] << 1U) | (gphNfcLib_State.pRxBuffer[2] >> 7U));
        if ((bHandle[0] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (bHandle[1] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        /* Making Received length as 0 since no user data has been received */
        gphNfcLib_State.wRxLength = 0;
        }
        break;

    case ISO18000_BlockErase:

        /* Parameter check */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank > 0x03U) || (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength > 3U))
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* prepare command header */
        wCmdLength = 0;
        bCmd[wCmdLength++] = PH_NFCLIB_18000P3M3_CMD_BLOCKERASE;
        bCmd[wCmdLength]   = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank << 6U) | (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength << 4U);  /* 4 bits left */

        /* append pointer */
        for (bIndex = 0; bIndex < (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordPtrLength + 1U); ++bIndex)
        {
            bCmd[wCmdLength++] |= (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pWordPtr[bIndex]) >> 4U;
            bCmd[wCmdLength]    = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pWordPtr[bIndex]) << 4U;
        }
        /* 4 bits left */

        /* append word-count */
        bCmd[wCmdLength++] |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount >> 4U;  /* 0 bits left */
        bCmd[wCmdLength]    = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bWordCount << 4U;  /* 4 bits left */

        /* append handle */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 4U;    /* 0 bits left */
        bCmd[wCmdLength]    = gphNfcLib_Params.salI18000p3m3.abHandle[0] << 4U;    /* 4 bits left */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 4U;    /* 0 bits left */
        bCmd[wCmdLength++]  = gphNfcLib_Params.salI18000p3m3.abHandle[1] << 4U;    /* 4 bits left */

        PH_CHECK_SUCCESS_FCT(statusTmp, phNfcLib_18000p3m3_Exchange(PH_ON, 0x04, 20, &bCmd[0], wCmdLength, &wRxLastBits));

        /* Check received length and header bit */
        if ((gphNfcLib_State.wRxLength != 3U) || (wRxLastBits != 1U) || ((gphNfcLib_State.pRxBuffer[0] & 0x80U) != 0U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check Handle */
        bHandle[0] = (uint8_t)((gphNfcLib_State.pRxBuffer[0] << 1U) | (gphNfcLib_State.pRxBuffer[1] >> 7U));
        bHandle[1] = (uint8_t)((gphNfcLib_State.pRxBuffer[1] << 1U) | (gphNfcLib_State.pRxBuffer[2] >> 7U));
        if ((bHandle[0] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (bHandle[1] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }
        /* Making Received length as 0 since no user data has been received */
        gphNfcLib_State.wRxLength = 0;
        break;

    case ISO18000_BlockPermaLock:
        /* Parameter check */
        if ((((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bReadLock > 0x01U) || (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank > 0x03U) || (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bBlockPtrLength > 3U))
        {
            return PH_ERR_INVALID_PARAMETER;
        }

        /* prepare command header */
        wCmdLength = 0;
        bCmd[wCmdLength++] = PH_NFCLIB_18000P3M3_CMD_BLOCKPERMALOCK;
        bCmd[wCmdLength++] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bRFU;
        bCmd[wCmdLength]   = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bReadLock << 7U) | (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bMemBank << 5U) | (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bBlockPtrLength << 3U);  /* 3 bits left */

        /* append pointer */
        for (bIndex = 0; bIndex < (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bBlockPtrLength + 1U); ++bIndex)
        {
            bCmd[wCmdLength++] |= (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pBlockPtr[bIndex]) >> 5U;
            bCmd[wCmdLength]    = (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pBlockPtr[bIndex]) << 3U;
        }
        /* 3 bits left */

        /* append (3bits) of block-range */
        bCmd[wCmdLength++] |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bBlockRange >> 5U;  /* 0 bits left */

        /* buffer the current contents */
        statusTmp = phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            bCmd,
            wCmdLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* reset command length*/
        wCmdLength = 0;

        /* append the rest bits of block-range*/
        bCmd[wCmdLength] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bBlockRange << 3U;  /* 3 bits left */

        /* append mask */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bReadLock == 0x01U)
        {
            for (bIndex = 0; bIndex < (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bBlockRange << 1U); ++bIndex)
            {
                bCmd[wCmdLength++] |= ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pMask[bIndex] >> 5U;  /* 0 bits left */

                /* buffer the current contents if command buffer is full */
                if (wCmdLength == 7U)
                {
                    /* buffer the current contents */
                    statusTmp = phhalHw_Exchange(
                        gphNfcLib_Params.sDiscLoop.pHalDataParams,
                        PH_EXCHANGE_BUFFER_CONT,
                        bCmd,
                        wCmdLength,
                        &gphNfcLib_State.pRxBuffer,
                        &gphNfcLib_State.wRxLength);

                    /* reset command length*/
                    wCmdLength = 0;
                }

                bCmd[wCmdLength] = ((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pMask[bIndex] << 3U;  /* 3 bits left */
            }
        }

        /* append handle */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[0] >> 5U;    /* 0 bits left */

        /* buffer the current contents if command buffer is full */
        if (wCmdLength == 7U)
        {

            statusTmp = phhalHw_Exchange(
                gphNfcLib_Params.sDiscLoop.pHalDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                bCmd,
                wCmdLength,
                &gphNfcLib_State.pRxBuffer,
                &gphNfcLib_State.wRxLength);

            /* reset command length*/
            wCmdLength = 0;
        }

        bCmd[wCmdLength]    = gphNfcLib_Params.salI18000p3m3.abHandle[0] << 3U;    /* 3 bits left */
        bCmd[wCmdLength++] |= gphNfcLib_Params.salI18000p3m3.abHandle[1] >> 5U;    /* 0 bits left */
        bCmd[wCmdLength++]  = gphNfcLib_Params.salI18000p3m3.abHandle[1] << 3U;    /* 3 bits left */

        (void)phNfcLib_18000p3m3_Exchange(PH_OFF, 0x05, 500, NULL, PH_NFCLIB_18000P3M3_SKIP_COMMAND, NULL);

        /* perform the exchange */
        statusTmp = phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            bCmd,
            wCmdLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);

        /* Ignore incomplete byte status, bail out on error */
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
        {
            (void)phhalHw_GetConfig(
                gphNfcLib_Params.sDiscLoop.pHalDataParams,
                PHHAL_HW_CONFIG_RXLASTBITS,
                &wRxLastBits);
            statusTmp = PH_ERR_SUCCESS;
        }

        if(statusTmp == PH_ERR_SUCCESS)
        {
        /* Check header bit */
        if ((gphNfcLib_State.wRxLength < 3U) || (wRxLastBits != 1U) || ((gphNfcLib_State.pRxBuffer[0] & 0x80U) != 0U))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Check data length */
        if (((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bReadLock == 0x00U)
        {
            if (gphNfcLib_State.wRxLength != (3u + (((uint16_t)(((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.bBlockRange)) << 1U)))
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }
        else
        {
            if (gphNfcLib_State.wRxLength != 3U)
            {
                return PH_ERR_PROTOCOL_ERROR;
            }
        }

        for (bIndex = 0; bIndex < (gphNfcLib_State.wRxLength - 1U); bIndex++)
        {
            gphNfcLib_State.pRxBuffer[bIndex] = (gphNfcLib_State.pRxBuffer[bIndex] << 1U) | (gphNfcLib_State.pRxBuffer[bIndex + 1U] >> 7U);
        }

        /* Status byte is removed */
        gphNfcLib_State.wRxLength--;

        /* Check Handle */
        if ((gphNfcLib_State.pRxBuffer[gphNfcLib_State.wRxLength - 2u] != gphNfcLib_Params.salI18000p3m3.abHandle[0]) ||
            (gphNfcLib_State.pRxBuffer[gphNfcLib_State.wRxLength - 1u] != gphNfcLib_Params.salI18000p3m3.abHandle[1]))
        {
            return PH_ERR_PROTOCOL_ERROR;
        }

        /* Handle is not part of data */
        gphNfcLib_State.wRxLength -= 2u;
        }
        break;

    case ISO18000_SetHandle:
        (void)memcpy(&gphNfcLib_Params.salI18000p3m3.abHandle[0], &((phNfcLib_Transmit_t *)pTxBuffer)->phNfcLib_ISO18000.pHandle[0], 2);
        gphNfcLib_Params.salI18000p3m3.bHandleValid = PH_ON;
        statusTmp = PH_ERR_SUCCESS;
        break;

    default:
        break;
    }
    PH_UNUSED_VARIABLE(wTxBufferLength);
    return statusTmp;
}

static phStatus_t phNfcLib_18000p3m3_ReqRN(void)
{
    phStatus_t statusTmp;
    uint8_t bCmd[3] = {0};


    /* Prepare and buffer command header */
    bCmd[0] = PH_NFCLIB_18000P3M3_CMD_REQ_RN;
    bCmd[1] = gphNfcLib_Params.salI18000p3m3.abHandle[0];
    bCmd[2] = gphNfcLib_Params.salI18000p3m3.abHandle[1];

    statusTmp = phhalHw_Exchange(
        gphNfcLib_Params.sDiscLoop.pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        3,
        &gphNfcLib_State.pRxBuffer,
        &gphNfcLib_State.wRxLength);
    return statusTmp;

}


static phStatus_t phNfcLib_18000p3m3_Exchange(uint8_t bSymbol, uint8_t bTxLastBits, uint16_t wTimeout, uint8_t* pCmd, uint16_t wCmdLength, uint16_t *pRxLastBits)
{
    phStatus_t statusTmp = PH_ERR_SUCCESS;

    if(bSymbol != PH_NFCLIB_18000P3M3_SKIP_COMMAND)
    {
        /* Setting the Symbol send. */
        (void)phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_SET_SYMBOL_SEND,
            (uint16_t)bSymbol);
    }

    if(bTxLastBits != PH_NFCLIB_18000P3M3_SKIP_COMMAND)
    {
        /* Setting the TxLastbits. */
        (void)phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_TXLASTBITS,
            (uint16_t)bTxLastBits);
    }

    if(wTimeout != PH_NFCLIB_18000P3M3_SKIP_COMMAND)
    {
        /* Set config for timeout */
        (void)phhalHw_SetConfig(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
            (uint16_t)wTimeout);
    }

    if(wCmdLength != PH_NFCLIB_18000P3M3_SKIP_COMMAND)
    {
        /* Perform HAL exchange. */
        statusTmp = phhalHw_Exchange(
            gphNfcLib_Params.sDiscLoop.pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            pCmd,
            wCmdLength,
            &gphNfcLib_State.pRxBuffer,
            &gphNfcLib_State.wRxLength);
    }

    if(pRxLastBits != NULL)
    {
        /* Ignore incomplete byte status, bail out on error */
        if ((statusTmp & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
        {
            (void)phhalHw_GetConfig(
                gphNfcLib_Params.sDiscLoop.pHalDataParams,
                PHHAL_HW_CONFIG_RXLASTBITS,
                pRxLastBits);

            statusTmp = PH_ERR_SUCCESS;
        }
    }

    return statusTmp;
}

#endif /* NXPBUILD__PH_NFCLIB_ISO_18000 */
