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
* Software ISO14443-4 Component of Reader Library Framework.
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
#include <phpalI14443p4.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I14443P4_SW

#include "phpalI14443p4_Sw_Int.h"
#include "phpalI14443p4_Sw.h"

static const uint16_t PH_MEMLOC_CONST_ROM bI14443p4_FsTable[13] = {16, 24, 32,
                                                                   40, 48, 64,
                                                                   96, 128, 256,
                                                                   512, 1024,
                                                                   2048, 4096};

#define PHPAL_I14443P4_SW_IS_BLOCKNR_EQUAL(bPcb)                        \
    (                                                                   \
        ((((bPcb) & PHPAL_I14443P4_SW_PCB_BLOCKNR) ^ pDataParams->bPcbBlockNum) == 0U) \
        ? 1u : 0u                                                       \
    )

#define PHPAL_I14443P4_SW_IS_ACK(bPcb)                                  \
    (                                                                   \
    (((bPcb) & PHPAL_I14443P4_SW_PCB_NAK) == PHPAL_I14443P4_SW_PCB_ACK) \
    ? 1u : 0u                                                             \
    )

#define PHPAL_I14443P4_SW_IS_CHAINING(bPcb)                                         \
    (                                                                               \
    (((bPcb) & PHPAL_I14443P4_SW_PCB_CHAINING) == PHPAL_I14443P4_SW_PCB_CHAINING)   \
    ? 1u : 0u                                                                         \
    )

#define PHPAL_I14443P4_SW_IS_WTX(bPcb)                                  \
    (                                                                   \
    (((bPcb) & PHPAL_I14443P4_SW_PCB_WTX) == PHPAL_I14443P4_SW_PCB_WTX) \
    ? 1u : 0u                                                             \
    )

#define PHPAL_I14443P4_SW_IS_DESELECT(bPcb)                                     \
    (                                                                           \
    (((bPcb) & PHPAL_I14443P4_SW_PCB_WTX) == PHPAL_I14443P4_SW_PCB_DESELECT)    \
    ? 1u : 0u                                                                     \
    )

#define PHPAL_I14443P4_SW_IS_I_BLOCK(bPcb)                                  \
    (                                                                       \
    (((bPcb) & PHPAL_I14443P4_SW_BLOCK_MASK) == PHPAL_I14443P4_SW_I_BLOCK)  \
    ? 1u : 0u                                                                 \
    )

#define PHPAL_I14443P4_SW_IS_R_BLOCK(bPcb)                                  \
    (                                                                       \
    (((bPcb) & PHPAL_I14443P4_SW_BLOCK_MASK) == PHPAL_I14443P4_SW_R_BLOCK)  \
    ? 1u : 0u                                                                 \
    )

#define PHPAL_I14443P4_SW_IS_S_BLOCK(bPcb)                                  \
    (                                                                       \
    (((bPcb) & PHPAL_I14443P4_SW_BLOCK_MASK) == PHPAL_I14443P4_SW_S_BLOCK)  \
    ? 1u : 0u                                                                 \
    )

/* Emvco: case_id TA415_4 */
#define PHPAL_I14443P4_SW_EMVCO_IS_INVALID_BLOCK_STATUS(bStatus)            \
    ((                                                                      \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) ||                     \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR) ||                \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_NOISE_ERROR) ||                    \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_FRAMING_ERROR) ||                  \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR) ||                \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_PROTOCOL_ERROR))                   \
    ? 1u : 0u                                                                 \
    )

#define PHPAL_I14443P4_SW_IS_INVALID_BLOCK_STATUS(bStatus)                  \
    ((                                                                      \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) ||                     \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR) ||                \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_FRAMING_ERROR) ||                  \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR))                  \
    ? 1u : 0u                                                                 \
    )

#define PHPAL_I14443P4_SW_DESELECT_IS_INVALID_BLOCK_STATUS(bStatus)         \
    ((                                                                      \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) ||                     \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR) ||                \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_FRAMING_ERROR) ||                  \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE) ||        \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_PROTOCOL_ERROR) ||                 \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR))                  \
    ? 1u : 0u                                                                 \
    )

phStatus_t phpalI14443p4_Sw_Init(
                                 phpalI14443p4_Sw_DataParams_t * pDataParams,
                                 uint16_t wSizeOfDataParams,
                                 void * pHalDataParams
                                 )
{
    if (sizeof(phpalI14443p4_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_ISO14443P4);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* Init private data */
    pDataParams->wId            = PH_COMP_PAL_ISO14443P4 | PHPAL_I14443P4_SW_ID;
    pDataParams->pHalDataParams = pHalDataParams;
    pDataParams->bOpeMode       = RD_LIB_MODE_NFC;

    /* Reset protocol to defaults */
    return phpalI14443p4_Sw_ResetProtocol(pDataParams);
}

phStatus_t phpalI14443p4_Sw_SetProtocol(
                                        phpalI14443p4_Sw_DataParams_t * pDataParams,
                                        uint8_t   bCidEnable,
                                        uint8_t   bCid,
                                        uint8_t   bNadEnable,
                                        uint8_t   bNad,
                                        uint8_t   bFwi,
                                        uint8_t   bFsdi,
                                        uint8_t   bFsci
                                        )
{
    /* Rule A, ISO/IEC 14443-4:2008(E), init. Blocknumber to zero */
    pDataParams->bPcbBlockNum = 0;

    /* Check parameters */
    if ((bCid > PHPAL_I14443P4_CID_MAX) ||
        (bFwi > PHPAL_I14443P4_FWI_MAX) ||
        (bFsdi > PHPAL_I14443P4_FRAMESIZE_MAX) ||
        (bFsci > PHPAL_I14443P4_FRAMESIZE_MAX))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4);
    }

    /* Apply parameters */
    pDataParams->bCidEnabled = bCidEnable;
    pDataParams->bCid = bCid;
    pDataParams->bNadEnabled = bNadEnable;
    pDataParams->bNad = bNad;
    pDataParams->bFwi = bFwi;
    pDataParams->bFsdi = bFsdi;
    pDataParams->bFsci = bFsci;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_ResetProtocol(
    phpalI14443p4_Sw_DataParams_t * pDataParams
    )
{
    /* Set default state */
    pDataParams->bStateNow = PHPAL_I14443P4_SW_STATE_I_BLOCK_TX;

    /* Rule A, ISO/IEC 14443-4:2008(E), init. Blocknumber to zero */
    pDataParams->bPcbBlockNum = 0;

    /* Apply default parameters */
    pDataParams->bCidEnabled        = 0x00;
    pDataParams->bCid               = 0x00;
    pDataParams->bNadEnabled        = 0x00;
    pDataParams->bNad               = 0x00;
    pDataParams->bFwi               = PHPAL_I14443P4_SW_FWI_DEFAULT;
    pDataParams->bFsdi              = 0x00;
    pDataParams->bFsci              = PHPAL_I14443P4_SW_FSCI_DEFAULT;
    pDataParams->bMaxRetryCount     = PHPAL_I14443P4_SW_MAX_RETRIES_DEFAULT;

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_Deselect(
                                     phpalI14443p4_Sw_DataParams_t * pDataParams
                                     )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bIsoFrame[3];
    uint16_t    PH_MEMLOC_REM wIsoFrameLen = 0;
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLen = 0;
    uint8_t     PH_MEMLOC_REM bInvalidBlock;
    uint8_t     PH_MEMLOC_REM bResponseReceived;
    uint16_t    PH_MEMLOC_REM wRetries;

    /* Build S(DESELECT) frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_BuildSBlock(
        pDataParams->bCidEnabled,
        pDataParams->bCid,
        0,
        0,
        bIsoFrame,
        &wIsoFrameLen));

    /* Set DESELECT timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_TIMEOUT_VALUE_US,
        PHPAL_I14443P4_SW_FWT_DESELECT_US + PHPAL_I14443P4_EXT_TIME_US));

    /* Retrieve max. retry count */
    wRetries = (uint16_t)pDataParams->bMaxRetryCount + 1U;

    /* Reset response received flag */
    bResponseReceived = 0;

    /* Do as long as invalid responses are received
    and the retry counter has not reached zero.*/
    do
    {
        /* Send the S(DESELECT) request */
        status = phhalHw_Exchange(
            pDataParams->pHalDataParams,
            PH_EXCHANGE_DEFAULT,
            bIsoFrame,
            wIsoFrameLen,
            &pResp,
            &wRespLen);

        /* Status --> InvalidBlock mapping */
        bInvalidBlock = (uint8_t)PHPAL_I14443P4_SW_DESELECT_IS_INVALID_BLOCK_STATUS(status);

        if (0U == (bInvalidBlock))
        {
            /* Check for other errors */
            PH_CHECK_SUCCESS(status);

            /* Signal that we've received something */
            bResponseReceived = 1;

            /* Rule 8, ISO/IEC 14443-4:2008(E), "...the S(DESELECT) request may be re-transmitted..." */
            if ((wRespLen == wIsoFrameLen) && (wRespLen > 0U) && (wRespLen < 3U))
            {
                /* Mask out Power-Level-Indication */
                if (0U != (pDataParams->bCidEnabled))
                {
                    pResp[1] &= 0x3FU;
                }

                /* Verify S(DESELECT) Response */
                if (memcmp(bIsoFrame, pResp, wRespLen) != 0)
                {
                    status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
                }
            }
            else
            {
                status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
            }
        }
        --wRetries;
    }
    /* Retry as long as neccessary */
    while ((0U != bInvalidBlock) && (wRetries != 0U));

    /* Operation not successful */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        /* Return ERR_RECOVERY_FAILED if some response has been received before (bMaxRetryCount = 0 suppresses the retry behaviour) */
        if ((pDataParams->bMaxRetryCount > 0U) && (bResponseReceived > 0U))
        {
            status = PH_ADD_COMPCODE_FIXED(PHPAL_I14443P4_ERR_RECOVERY_FAILED, PH_COMP_PAL_ISO14443P4);
        }
    }

    /* Apply default parameters */
    (void) phpalI14443p4_Sw_ResetProtocol(pDataParams);

    return status;
}

phStatus_t phpalI14443p4_Sw_PresCheck(
                                      phpalI14443p4_Sw_DataParams_t * pDataParams
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bIsoFrame[3];
    uint8_t *   PH_MEMLOC_REM pIsoFrameResp = NULL;
    uint16_t    PH_MEMLOC_REM wIsoFrameLen = 0;

    /* Build R(NAK) frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_BuildRBlock(
        pDataParams->bCidEnabled,
        pDataParams->bCid,
        pDataParams->bPcbBlockNum,
        0,
        bIsoFrame,
        &wIsoFrameLen));

    /* Send frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
        pDataParams->pHalDataParams,
        PH_EXCHANGE_DEFAULT,
        bIsoFrame,
        wIsoFrameLen,
        &pIsoFrameResp,
        &wIsoFrameLen));

    /* Check if we got a valid R(ACK) response */
    /* */
    if ((PHPAL_I14443P4_SW_IS_R_BLOCK(pIsoFrameResp[PHPAL_I14443P4_SW_PCB_POS]) > 0U) &&
        (PHPAL_I14443P4_SW_IS_ACK(pIsoFrameResp[PHPAL_I14443P4_SW_PCB_POS]) > 0U))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_IsValidRBlock(
            pDataParams->bCidEnabled,
            pDataParams->bCid,
            pIsoFrameResp,
            wIsoFrameLen));
    }
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_Exchange(
                                     phpalI14443p4_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t * pTxBuffer,
                                     uint16_t wTxLength,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pRxLength
                                     )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bBufferOverflow;

    /* Used to build I/R/S block frames */
    uint8_t     PH_MEMLOC_REM bIsoFrame[3];
    uint16_t    PH_MEMLOC_REM wIsoFrameLen = 0;
    uint8_t     PH_MEMLOC_REM bRxOverlapped[3];
    uint16_t    PH_MEMLOC_REM wRxOverlappedLen = 0;
    uint16_t    PH_MEMLOC_REM wRxStartPos;
    uint8_t     PH_MEMLOC_REM bUseNad = 0;
    uint8_t     PH_MEMLOC_REM bForceSend;

    /* Used for Transmission */
    uint16_t    PH_MEMLOC_REM wRxBufferSize;
    uint16_t    PH_MEMLOC_REM wTxBufferSize;
    uint16_t    PH_MEMLOC_REM wTxBufferLen = 0;
    uint16_t    PH_MEMLOC_REM wInfLength = 0;
    uint16_t    PH_MEMLOC_REM wMaxPcdFrameSize;
    uint16_t    PH_MEMLOC_REM wMaxCardFrameSize;
    uint16_t    PH_MEMLOC_REM wPcb = 0;
    uint8_t     PH_MEMLOC_REM bRetryCountRetransmit;

    /* Used for Reception */
    uint16_t    PH_MEMLOC_REM RxLength;
    uint8_t *   PH_MEMLOC_REM pRxBuffer = NULL;

    /* Option parameter check */
    if (0u != (wOption &  (uint16_t)~(uint16_t)
        (
        PH_EXCHANGE_BUFFERED_BIT | PH_EXCHANGE_LEAVE_BUFFER_BIT |
        PH_EXCHANGE_TXCHAINING | PH_EXCHANGE_RXCHAINING | PH_EXCHANGE_RXCHAINING_BUFSIZE
        )))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4);
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

    /* Retrieve HAL buffer sizes */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE, &wRxBufferSize));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_BUFSIZE, &wTxBufferSize));

    /* Retrieve maximum frame sizes */
    wMaxPcdFrameSize = bI14443p4_FsTable[pDataParams->bFsdi] - (uint16_t)2U;
    wMaxCardFrameSize = bI14443p4_FsTable[pDataParams->bFsci] - (uint16_t)2U;

    /* R(ACK) transmission in case of Rx-Chaining */
    if (((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING) ||
        ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING_BUFSIZE))
    {
        pDataParams->bStateNow = PHPAL_I14443P4_SW_STATE_I_BLOCK_RX | PHPAL_I14443P4_SW_STATE_CHAINING_BIT;
    }
    /* I-Block transmission */
    else
    {
        /* Reset to default state if not in Tx-Mode */
        if ((pDataParams->bStateNow & PH_EXCHANGE_MODE_MASK) != PHPAL_I14443P4_SW_STATE_I_BLOCK_TX)
        {
            pDataParams->bStateNow = PHPAL_I14443P4_SW_STATE_I_BLOCK_TX;
        }
    }

    /* Reset receive length */
    *pRxLength = 0;

    /* Reset RetryCount */
    bRetryCountRetransmit = 0;

    /* Reset BufferOverflow flag */
    bBufferOverflow = 0;

    /* ******************************** */
    /*     I-BLOCK TRANSMISSION LOOP    */
    /* ******************************** */
    do
    {
        /* Reset Preloaded bytes and ForceSend */
        wTxBufferLen = 0;
        bForceSend = 0;

        switch (pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_MASK)
        {
        case PHPAL_I14443P4_SW_STATE_I_BLOCK_TX:

            /* Retrieve Number of preloaded bytes */
            if (0U != (wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                    &wTxBufferLen));
            }

            /* Set initial INF length to (remaining) input data length */
            wInfLength = wTxLength;

            /* Frame has already been preloaded -> IsoFrameLen is zero */
            if (wTxBufferLen > 0U)
            {
                /* do not generate the iso frame */
                wIsoFrameLen = 0;
            }
            /* Else evaluate IsoFrameLen*/
            else
            {
                /* 7.1.1.3 c), ISO/IEC 14443-4:2008(E), "During chaining the NAD shall only be transmitted in the first block of chain." */
                if ((0U == ((pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_CHAINING_BIT))))
                {
                    bUseNad = pDataParams->bNadEnabled;
                }
                else
                {
                    bUseNad = 0;
                }

                /* Evaluate frame overhead */
                wIsoFrameLen = 1;
                if (0U != (bUseNad))
                {
                    ++wIsoFrameLen;
                }
                if (0U != (pDataParams->bCidEnabled))
                {
                    ++wIsoFrameLen;
                }
            }

            /* Check if chaining is intended or not */
            if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_TXCHAINING)
            {
                pDataParams->bStateNow |= PHPAL_I14443P4_SW_STATE_CHAINING_BIT;
            }
            else
            {
                pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I14443P4_SW_STATE_CHAINING_BIT;
            }

            /* Force frame exchange if
            a) the maximum frame size of the card has been reached;
            */
            if ((wTxBufferLen + wIsoFrameLen + wInfLength) > wMaxCardFrameSize)
            {
                /* force frame exchange */
                bForceSend = 1;

                /* force chaining */
                pDataParams->bStateNow |= PHPAL_I14443P4_SW_STATE_CHAINING_BIT;

                /* limit number of bytes to send */
                wInfLength = wMaxCardFrameSize - wTxBufferLen - wIsoFrameLen;
            }

            /* Force frame exchange if
            b) the TxBuffer is full;
            */
            if ((0U != ((wOption & PH_EXCHANGE_BUFFERED_BIT))) &&
                ((wTxBufferLen + wIsoFrameLen + wInfLength) >= wTxBufferSize))
            {
                /* force frame exchange */
                bForceSend = 1;

                /* force chaining */
                pDataParams->bStateNow |= PHPAL_I14443P4_SW_STATE_CHAINING_BIT;
            }

            /* Generate / Buffer ISO frame */
            if (wIsoFrameLen > 0U)
            {
                /* Generate I-Block frame header */
                PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_BuildIBlock(
                    pDataParams->bCidEnabled,
                    pDataParams->bCid,
                    bUseNad,
                    pDataParams->bNad,
                    pDataParams->bPcbBlockNum,
                    (0U != (pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_CHAINING_BIT)) ? 1U : 0U,
                    bIsoFrame,
                    &wIsoFrameLen));

                /* Write Frame to HAL TxBuffer but do not preform Exchange */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
                    pDataParams->pHalDataParams,
                    PH_EXCHANGE_BUFFER_FIRST,
                    bIsoFrame,
                    wIsoFrameLen,
                    NULL,
                    NULL));

                /* Retain the preloaded bytes from now on */
                wOption |= PH_EXCHANGE_LEAVE_BUFFER_BIT;
            }

            /* Tx-Buffering mode (and no forced exchange) */
            if ((0U == bForceSend) && (0U != (wOption & PH_EXCHANGE_BUFFERED_BIT)))
            {
                /* Preload the data into the TxBuffer */
                return phhalHw_Exchange(
                    pDataParams->pHalDataParams,
                    (wOption & (uint16_t)~(uint16_t)PH_EXCHANGE_MODE_MASK),
                    pTxBuffer,
                    wInfLength,
                    NULL,
                    NULL);
            }

            /* Content has been buffered before */
            if (wTxBufferLen > 0U)
            {
                /* retrieve PCB byte */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, 0x00));
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, &wPcb));

                /* Preloaded Data or ForceSend -> Modify PCB byte if neccessary */
                if ((0U != bForceSend) || ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_TXCHAINING))
                {
                    /* modify PCB byte */
                    wPcb |= PHPAL_I14443P4_SW_PCB_CHAINING;
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, wPcb));
                }
            }
            break;

        case PHPAL_I14443P4_SW_STATE_I_BLOCK_RX:

            /* Build R(ACK) frame */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_BuildRBlock(
                pDataParams->bCidEnabled,
                pDataParams->bCid,
                pDataParams->bPcbBlockNum,
                1,
                bIsoFrame,
                &wIsoFrameLen));

            /* Write Frame to HAL TxBuffer but do not preform Exchange */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Exchange(
                pDataParams->pHalDataParams,
                PH_EXCHANGE_BUFFERED_BIT,
                bIsoFrame,
                wIsoFrameLen,
                NULL,
                NULL));

            /* Retain the preloaded bytes from now on */
            wOption |= PH_EXCHANGE_LEAVE_BUFFER_BIT;

            /* do not append any data */
            wInfLength = 0;
            break;

            /* Should NEVER happen! */
        default:
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_ISO14443P4);
        }

        /* Perform Exchange using complete ISO handling */
        status = phpalI14443p4_Sw_IsoHandling(
            pDataParams,
            wOption & (uint16_t)~(uint16_t)PH_EXCHANGE_BUFFERED_BIT,
            bRetryCountRetransmit,
            pTxBuffer,
            wInfLength,
            ppRxBuffer,
            pRxLength);

        /* Complete chaining if buffer is full */
        if (((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW) &&
            ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING_BUFSIZE))
        {
            /* Indicate Buffer Overflow */
            bBufferOverflow = 1;

            /* Toggle Blocknumber */
            pDataParams->bPcbBlockNum ^= PHPAL_I14443P4_SW_PCB_BLOCKNR;
        }
        /* Else bail out on error */
        else
        {
            PH_CHECK_SUCCESS(status);
        }

        /* Retransmission in progress */
        if (0U != (pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_RETRANSMIT_BIT))
        {
            /* Increment Retransmit RetryCount */
            ++bRetryCountRetransmit;

            /* Restore internal TxBuffer. */
            /* Neccessary in case RxBuffer and TxBuffer are the same. */
            if (wTxBufferLen > 0U)
            {
                /* restore PCB byte */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER_OFFSET, 0x00));
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXBUFFER, wPcb));

                /* restore TxBufferLen */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                    pDataParams->pHalDataParams,
                    PHHAL_HW_CONFIG_TXBUFFER_LENGTH,
                    wTxBufferLen));
            }

            /* Clear retransmission bit */
            pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I14443P4_SW_STATE_RETRANSMIT_BIT;
        }
        /* No retransmission in progress */
        else
        {
            /* Reset Retransmit RetryCount */
            bRetryCountRetransmit = 0;

            /* Chaining is active */
            if (pDataParams->bStateNow == (PHPAL_I14443P4_SW_STATE_I_BLOCK_TX | PHPAL_I14443P4_SW_STATE_CHAINING_BIT))
            {
                /* Bytes to send cannot be less than sent bytes */
                if (wTxLength < wInfLength)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_ISO14443P4);
                }

                /* Remove sent bytes from TxBuffer */
                pTxBuffer = pTxBuffer + wInfLength;

                /* below if condition is added to make QAC compliant  */
                if(wTxLength > 0U)
                {
                    wTxLength = wTxLength - wInfLength;
                }
            }

            /* Buffered / TxChaining mode -> finished after sending */
            if ((wTxLength == 0U) &&
                (
                ((wOption & PH_EXCHANGE_BUFFERED_BIT) > 0U) ||
                ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_TXCHAINING)
                ))
            {
                return PH_ERR_SUCCESS;
            }
        }
    }
    /* Loop as long as the state does not transit to RX mode */
    while ((pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_MASK) != PHPAL_I14443P4_SW_STATE_I_BLOCK_RX);

    /* Overlap PCB */
    wRxOverlappedLen = 1;

    /* Overlap CID */
    if (0u != ((*ppRxBuffer)[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_CID_FOLLOWING))
    {
        wRxOverlappedLen++;
    }

    /* Overlap NAD */
    if (0u != ((*ppRxBuffer)[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_NAD_FOLLOWING))
    {
        wRxOverlappedLen++;
    }

    /* Reset RxStartPos */
    wRxStartPos = 0;

    /* ******************************** */
    /*      I-BLOCK RECEPTION LOOP      */
    /* ******************************** */
    do
    {
        /* Only allow receive state at this point */
        if ((pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_MASK) != PHPAL_I14443P4_SW_STATE_I_BLOCK_RX)
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_ISO14443P4);
        }

        /* Rule 2, ISO/IEC 14443-4:2008(E), PICC chaining */
        if (0U != (pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_CHAINING_BIT))
        {
            /* Skip overlapping / SUCCESS_CHAINING checks in case of BufferOverflow */
            if (0U == (bBufferOverflow))
            {
                /* This is first chained response */
                if (wRxStartPos == 0U)
                {
                    /* Special NAD chaining handling */
                    /* 7.1.1.3 c), ISO/IEC 14443-4:2008(E), "During chaining the NAD shall only be transmitted in the first block of chain." */
                    if (0U != (pDataParams->bNadEnabled))
                    {
                        --wRxOverlappedLen;
                    }
                }

                /* Backup overlapped bytes */
                (void)memcpy(bRxOverlapped, &(*ppRxBuffer)[((*pRxLength) - wRxOverlappedLen)], wRxOverlappedLen);

                /* Calculate RxBuffer Start Position */
                wRxStartPos = (*pRxLength) - wRxOverlappedLen;

                /* Skip SUCCESS_CHAINING check for RXCHAINING_BUFSIZE mode */
                if ((wOption & PH_EXCHANGE_MODE_MASK) != PH_EXCHANGE_RXCHAINING_BUFSIZE)
                {
                    /* Return with chaining status if the next chain may not fit into our buffer */
                    if ((*pRxLength + wMaxPcdFrameSize) > wRxBufferSize)
                    {
                        /* Adjust RxBuffer position */
                        (*ppRxBuffer) = (*ppRxBuffer) + wRxOverlappedLen;
                        *pRxLength = *pRxLength - wRxOverlappedLen;

                        return PH_ADD_COMPCODE_FIXED(PH_ERR_SUCCESS_CHAINING, PH_COMP_PAL_ISO14443P4);
                    }
                }
            }

            /* Set RxBuffer Start Position */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
                pDataParams->pHalDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                wRxStartPos));

            /* Prepare R(ACK) frame */
            PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_BuildRBlock(
                pDataParams->bCidEnabled,
                pDataParams->bCid,
                pDataParams->bPcbBlockNum,
                1,
                bIsoFrame,
                &wIsoFrameLen));

            /* Perform Exchange using complete ISO handling */
            status = phpalI14443p4_Sw_IsoHandling(
                pDataParams,
                PH_EXCHANGE_DEFAULT,
                0,
                bIsoFrame,
                wIsoFrameLen,
                ppRxBuffer,
                pRxLength);

            /* Complete chaining if buffer is full */
            if (((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW) &&
                ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING_BUFSIZE))
            {
                /* Reset wRxStartPos */
                wRxStartPos = 0;

                /* Indicate Buffer Overflow */
                bBufferOverflow = 1;

                /* Toggle Blocknumber */
                pDataParams->bPcbBlockNum ^= PHPAL_I14443P4_SW_PCB_BLOCKNR;
            }
            /* Default behaviour */
            else
            {
                /* In case of buffer overflow error from HAL, reset the HAL Rx Buffer Start position */
                if ((pDataParams->bOpeMode != RD_LIB_MODE_ISO) && ((status & PH_ERR_MASK) == PH_ERR_BUFFER_OVERFLOW))
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));
                }
                /* Bail out on error */
                PH_CHECK_SUCCESS(status);

                /* Restore overlapped INF bytes */
                (void)memcpy(&(*ppRxBuffer)[wRxStartPos], bRxOverlapped, wRxOverlappedLen);  /* PRQA S 3200 */ /* PRQA S 3354 */
            }
        }
        /* No chaining -> reception finished */
        else
        {
            /* Return data */
            if (0U == (bBufferOverflow))
            {
                /* Special NAD chaining handling */
                /* 7.1.1.3 c), ISO/IEC 14443-4:2008(E), "During chaining the NAD shall only be transmitted in the first block of chain." */
                if ((wRxStartPos > 0U) && (pDataParams->bNadEnabled > 0U))
                {
                    ++wRxOverlappedLen;
                }

                /* Do not return protocol bytes, advance to INF field */
                (*ppRxBuffer) = (*ppRxBuffer) + wRxOverlappedLen;
                *pRxLength = *pRxLength - wRxOverlappedLen;
                /* Reception successful */
                status = PH_ERR_SUCCESS;
            }
            /* do not return any data in case of Buffer Overflow */
            else
            {
                *pRxLength = 0;
                status = PH_ADD_COMPCODE_FIXED(PH_ERR_BUFFER_OVERFLOW, PH_COMP_PAL_ISO14443P4);
            }

            /* Reception finished */
            pDataParams->bStateNow = PHPAL_I14443P4_SW_STATE_FINISHED;
        }
    }
    while (pDataParams->bStateNow != PHPAL_I14443P4_SW_STATE_FINISHED);

    return status;
}

phStatus_t phpalI14443p4_Sw_IsoHandling(
                                        phpalI14443p4_Sw_DataParams_t * pDataParams,
                                        uint16_t wOption,
                                        uint8_t bRetryCount,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength,
                                        uint8_t ** ppRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    phStatus_t  PH_MEMLOC_REM status = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusBkUp = PH_ERR_SUCCESS;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bIsoFrame[3];
    uint8_t *   PH_MEMLOC_REM pResp = NULL;
    uint16_t    PH_MEMLOC_REM wRespLen = 0;
    uint16_t    PH_MEMLOC_REM wRxBufferStartPos;
    uint8_t     PH_MEMLOC_REM bInvalidBlock;
    uint8_t     PH_MEMLOC_REM bResponseReceived;
    uint8_t     PH_MEMLOC_REM bWtxm = 0;
    uint8_t     PH_MEMLOC_REM bCheckNad;
    uint32_t    PH_MEMLOC_REM dwTimeout;
    uint32_t    PH_MEMLOC_REM dwTimeoutMax = PHPAL_I14443P4_SW_FWT_MAX_US;
    uint16_t    PH_MEMLOC_REM wTimeoutPrev = 0;
    uint8_t     PH_MEMLOC_REM bTimeoutInMs = 0;
    uint8_t     bNAKCount = 0;
    uint8_t     bACKCount = 0;

    /* Buffered Exchange is not allowed here */
    if (0U != (wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    /* Ignore custom modes */
    wOption &= (uint16_t)~(uint16_t)PH_EXCHANGE_MODE_MASK;

    /* Reset response received flag */
    bResponseReceived = 0;

    /* Loops in case of an error */
    do
    {
        /* WTX Timeout - set temporary FWT */
        if (bWtxm > 0U)
        {
            /* Retrieve current timeout */
            status = phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, &wTimeoutPrev);

            /* Timeout is out of range, retrieve it in milliseconds */
            if ((status & PH_ERR_MASK) == PH_ERR_PARAMETER_OVERFLOW)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, &wTimeoutPrev));
                bTimeoutInMs = 1;
            }
            else
            {
                PH_CHECK_SUCCESS(status);
                bTimeoutInMs = 0;
            }

            /* Calculate temporary WTX timeout */
            if(pDataParams->bOpeMode == RD_LIB_MODE_ISO)
            {
                dwTimeout = (((uint32_t)PHPAL_I14443P4_SW_FWT_MIN_US * ((uint32_t)1U << pDataParams->bFwi)) * (uint32_t)bWtxm) +
                            (uint32_t)PHPAL_I14443P4_EXT_TIME_US;
            }
            else
            {
                /* As per Digital Spec V1.1 req 15.3.2.1, NFC Forum Device SHALL wait at least FWTtemp + delta FWTT4AT for a Response */
                dwTimeout = (((uint32_t)PHPAL_I14443P4_SW_FWT_MIN_US * ((uint32_t)1U << pDataParams->bFwi)) * (uint32_t)bWtxm) +
                            (uint32_t)PHPAL_I14443P4_SW_DELTA_FWT_US;

                dwTimeoutMax = PHPAL_I14443P4_SW_NFC_FWT_MAX_US;

                if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
                {
                    dwTimeout += (uint32_t)PHPAL_I14443P4_SW_DELTA_TPCD_US;

                    dwTimeoutMax = PHPAL_I14443P4_SW_EMVCO_FWT_MAX_US;
                }
            }

            /* Limit timeout to FWT max */
            if (dwTimeout > dwTimeoutMax)
            {
                dwTimeout = dwTimeoutMax;
            }

            /* Set temporary WTX timeout */
            if (dwTimeout > 0xFFFFU)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, (uint16_t)((dwTimeout / 1000U) + 1U)));
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, (uint16_t)dwTimeout));
            }
        }

        /* Call HAL exchange function */
        status = phhalHw_Exchange(
            pDataParams->pHalDataParams,
            wOption,
            pTxBuffer,
            wTxLength,
            ppRxBuffer,
            pRxLength);
        statusBkUp = status;

        /* Reset Exchange Option */
        wOption = PH_EXCHANGE_DEFAULT;

        /* Reset transmission length */
        wTxLength = 0;

        /* WTX Timeout - restore previous value */
        if (bWtxm > 0U)
        {
            /* Restore previous state and timeout after */
            if (0U == (bTimeoutInMs))
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TIMEOUT_VALUE_US, wTimeoutPrev));
            }
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS, wTimeoutPrev));
            }
        }

        /* Status --> InvalidBlock mapping */
        if(pDataParams->bOpeMode == RD_LIB_MODE_EMVCO)
        {
            bInvalidBlock = (uint8_t)PHPAL_I14443P4_SW_EMVCO_IS_INVALID_BLOCK_STATUS(status);
        }
        else
        {
           bInvalidBlock = (uint8_t)PHPAL_I14443P4_SW_IS_INVALID_BLOCK_STATUS(status);
        }
        if (0U == (bInvalidBlock))
        {
            /* MIFARE compliancy: force protocol error on NAK */
            if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
            {
                *pRxLength = 0;
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));
                return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
            }
            else
            {
                PH_CHECK_SUCCESS(status);
            }
        }

        /* Reset defaults */
        bWtxm = 0;

        /* Emvco: case_id TA404_04, TA411_04, TB404_04, TB411_04 */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, &wRxBufferStartPos));
        /* Check for FSD */
        if(((*pRxLength) - wRxBufferStartPos) > (bI14443p4_FsTable[pDataParams->bFsdi] - 2))
         {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));
            status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
            *pRxLength = 0;
            return status;
         }
        /* Do not enter if we received an invalid block */
        if (0U == (bInvalidBlock))
        {
            /* Signal that we've received something */
            bResponseReceived = 1;

            /* Map receive buffer pointer to current data */
            pResp = *ppRxBuffer + wRxBufferStartPos;
            wRespLen = *pRxLength - wRxBufferStartPos;

            /* I-Block handling */
            if (0u != (PHPAL_I14443P4_SW_IS_I_BLOCK(pResp[PHPAL_I14443P4_SW_PCB_POS])))
            {
                /* Rule 2/10, ISO/IEC 14443-4:2008(E) */
                if (pDataParams->bStateNow == (PHPAL_I14443P4_SW_STATE_I_BLOCK_TX | PHPAL_I14443P4_SW_STATE_CHAINING_BIT))
                {
                    /* Protocol violation */
                    bInvalidBlock = 1;
                }
                else
                {
                    /* Evaluate if NAD should be present */
                    /* 7.1.1.3 c), ISO/IEC 14443-4:2008(E), "During chaining the NAD shall only be transmitted in the first block of chain." */
                    if ((pDataParams->bNadEnabled > 0U) && (0U == ((pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_CHAINING_BIT))))
                    {
                        bCheckNad = 1;
                    }
                    else
                    {
                        bCheckNad = 0;
                    }

                    /* Check if I-Block is valid */
                    status = phpalI14443p4_Sw_IsValidIBlock(
                        pDataParams->bCidEnabled,
                        pDataParams->bCid,
                        bCheckNad,
                        pDataParams->bNad,
                        pResp,
                        wRespLen);

                    /* Blocknumber is equal, I-Block Rx mode is active */
                    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS) && (PHPAL_I14443P4_SW_IS_BLOCKNR_EQUAL(pResp[PHPAL_I14443P4_SW_PCB_POS]) > 0U))
                    {
                        /* Switch from Tx-Mode to Rx-Mode */
                        if ((pDataParams->bStateNow & PHPAL_I14443P4_SW_STATE_MASK) == PHPAL_I14443P4_SW_STATE_I_BLOCK_TX)
                        {
                            pDataParams->bStateNow = PHPAL_I14443P4_SW_STATE_I_BLOCK_RX;
                        }

                        /* Rule B, ISO/IEC 14443-4:2008(E), toggle Blocknumber */
                        pDataParams->bPcbBlockNum ^= PHPAL_I14443P4_SW_PCB_BLOCKNR;

                        /* Set chaining bit if card is chaining */
                        if (0u != (PHPAL_I14443P4_SW_IS_CHAINING(pResp[PHPAL_I14443P4_SW_PCB_POS])))
                        {
                            pDataParams->bStateNow |= PHPAL_I14443P4_SW_STATE_CHAINING_BIT;
                        }
                        /* Clear chaining bit otherwise */
                        else
                        {
                            pDataParams->bStateNow &= (uint8_t)~(uint8_t)PHPAL_I14443P4_SW_STATE_CHAINING_BIT;
                        }
                    }
                    /* We received an invalid block */
                    else
                    {
                        /* Protocol violation */
                        bInvalidBlock = 1;
                    }
                }
            }
            /* R(ACK) handling */
            else if ((PHPAL_I14443P4_SW_IS_R_BLOCK(pResp[PHPAL_I14443P4_SW_PCB_POS]) > 0U) && (PHPAL_I14443P4_SW_IS_ACK(pResp[PHPAL_I14443P4_SW_PCB_POS]) > 0U))
            {
                /* Check if R-Block is valid */
                status = phpalI14443p4_Sw_IsValidRBlock(
                    pDataParams->bCidEnabled,
                    pDataParams->bCid,
                    pResp,
                    wRespLen);

                /* R-Block invalid or not in transmission mode */
                if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                {
                    /* Protocol violation */
                    bInvalidBlock = 1;
                }
                else
                {
                    /* Blocknumber is equal */
                    if (0u != (PHPAL_I14443P4_SW_IS_BLOCKNR_EQUAL(pResp[PHPAL_I14443P4_SW_PCB_POS])))
                    {
                        /* Continue TX chaining */
                        if (pDataParams->bStateNow == (PHPAL_I14443P4_SW_STATE_I_BLOCK_TX | PHPAL_I14443P4_SW_STATE_CHAINING_BIT))
                        {
                            /* Rule B, ISO/IEC 14443-4:2008(E), toggle Blocknumber */
                            pDataParams->bPcbBlockNum ^= PHPAL_I14443P4_SW_PCB_BLOCKNR;
                        }
                        /* Not in chaining mode */
                        else
                        {
                            /* Protocol violation */
                            bInvalidBlock = 1;
                        }
                    }
                    /* Rule 6, ISO/IEC 14443-4:2008(E), unequal block number */
                    else
                    {
                        /* Limit this behaviour with bMaxRetryCount, bRetry count is set by the caller in this case */
                        if ((bRetryCount > pDataParams->bMaxRetryCount) || (pDataParams->bMaxRetryCount == 0U))
                        {
                            /* Protocol violation */
                            bInvalidBlock = 1;
                        }
                        /* Send last I-Block again */
                        else
                        {
                            pDataParams->bStateNow |= PHPAL_I14443P4_SW_STATE_RETRANSMIT_BIT;
                        }
                    }
                }
            }
            /* S(WTX) handling */
            else if ((PHPAL_I14443P4_SW_IS_S_BLOCK(pResp[PHPAL_I14443P4_SW_PCB_POS]) > 0U) && (PHPAL_I14443P4_SW_IS_WTX(pResp[PHPAL_I14443P4_SW_PCB_POS]) > 0U))
            {
                /* Check if S-Block is valid */
                status = phpalI14443p4_Sw_IsValidSBlock(
                    pDataParams->bCidEnabled,
                    pDataParams->bCid,
                    pResp,
                    wRespLen);

                /* Rule 3, ISO/IEC 14443-4:2008(E), S(WTX) handling */
                if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
                {
                    /* Retrieve WTXM */
                    bWtxm = pResp[wRespLen-1u];

                    /* EMV 2.5 */
                    if ((0U != ((bWtxm & PHPAL_I14443P4_SW_S_BLOCK_INF_PLI_MASK))) && (pDataParams->bOpeMode == RD_LIB_MODE_EMVCO))
                    {
                        status =  PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
                        break;
                    }

                    /* Ignore and clear the Power Level Indication */
                    bWtxm &= 0x3FU;

                    /* Treat invalid WTXM value as protocol error, do not perform error correction. */
                    if ((bWtxm == 0U) || (bWtxm > 59U))
                    {
                        status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
                        break;
                    }

                    /* Generate S(WTX) frame */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_BuildSBlock(
                        pDataParams->bCidEnabled,
                        pDataParams->bCid,
                        1,
                        bWtxm,
                        bIsoFrame,
                        &wTxLength));
                }
                /* We received an invalid block */
                else
                {
                    /* Protocol violation */
                    bInvalidBlock = 1;
                }
            }
            /* We received an invalid block */
            else
            {
                /* Protocol violation */
                bInvalidBlock = 1;
            }

            /* Protocol violation */
            if (0U != (bInvalidBlock))
            {
                /* Emvco:  case_id TA404_XY and TA401_15 */
                /* bMaxRetryCount = 0 suppresses the S(DESELECT) behaviour */
                if((pDataParams->bMaxRetryCount > 0U) && (pDataParams->bOpeMode != RD_LIB_MODE_EMVCO))
                {
                    /* send S(DESELECT) (ignore return code) */
                    statusTmp = phpalI14443p4_Sw_Deselect(pDataParams);
                }

                /* bail out with protocol error */
                status = PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
                break;
            }
            /* Reset retry counter on no error */
            else
            {
                bRetryCount = 0;
            }
        }
        /* Invalid Block received */
        else
        {
            /* Only perform error correction if the max. retry count is not reached */
            /* For ISO (7.5.6.1 Errors detected by the PCD)
             * aU) Transmission error (Frame error or EDC error) or FWT time-out
             *      The PCD shall attempt error recovery by the following techniques in the order shown:
             *   - Application of PCD rules (see 7.5.4.2),
             *   - Optionally apply PCD rules (see 7.5.4.2) once more,
             *   - Use of S(DESELECT) request,
             *
             *   ISO 10373-6 Scenario H.29
             */

            /* DTA: Scenario 110 case: TC_POL_NFCA_T4AT_BI_31_xy, Article(13.2.5.4). Page 210
             * and For EMVCo: TA401_15 Article(10.3.4.4)
             * Says, After 2nd failed retry of I-block exchange. Send NAk
             */

            /* This is added to restrict retry count less than 1 for ISO mode */
            if( (bRetryCount >= pDataParams->bMaxRetryCount) && (pDataParams->bOpeMode == RD_LIB_MODE_ISO))
            {
                if (pDataParams->bMaxRetryCount > 0U)
                {
                    /* backup retry count */
                    bRetryCount = pDataParams->bMaxRetryCount;

                    /* set retry count to zero to send only one S(DESELECT) */
                    pDataParams->bMaxRetryCount = 0;

                    /* send deselect (ignore return code) */
                    statusTmp = phpalI14443p4_Sw_Deselect(pDataParams);

                    /* restore retry count setting */
                    pDataParams->bMaxRetryCount = bRetryCount;

                    /* Return ERR_RECOVERY_FAILED if some response has been received before */
                    if (0U != (bResponseReceived))
                    {
                        status = PH_ADD_COMPCODE_FIXED(PHPAL_I14443P4_ERR_RECOVERY_FAILED, PH_COMP_PAL_ISO14443P4);
                    }
                }
                break;
            }

            else if (bRetryCount <= pDataParams->bMaxRetryCount)
            {
                /* Rule 5, ISO/IEC 14443-4:2008(E) */
                if (pDataParams->bStateNow == (PHPAL_I14443P4_SW_STATE_I_BLOCK_RX | PHPAL_I14443P4_SW_STATE_CHAINING_BIT))
                {
                    /* Generate R(ACK) frame */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_BuildRBlock(
                        pDataParams->bCidEnabled,
                        pDataParams->bCid,
                        pDataParams->bPcbBlockNum,
                        1,
                        bIsoFrame,
                        &wTxLength));

                    bNAKCount = 0;

                    /* Emvco: case_id TA410 */
                    if(bACKCount >= pDataParams->bMaxRetryCount)
                    {
                        break;
                    }

                    if((statusBkUp & PH_ERR_MASK) != PH_ERR_SUCCESS )
                    {
                        bACKCount++;
                    }
                    else
                    {
                        bACKCount = 0;
                    }
                }
                /* Rule 4, ISO/IEC 14443-4:2008(E) */
                else
                {
                    /* Generate R(NAK) frame */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phpalI14443p4_Sw_BuildRBlock(
                        pDataParams->bCidEnabled,
                        pDataParams->bCid,
                        pDataParams->bPcbBlockNum,
                        0,
                        bIsoFrame,
                        &wTxLength));
                    bACKCount = 0;

                    /* Emvco: case_id TA402 TA403 */
                    if(bNAKCount >= pDataParams->bMaxRetryCount)
                    {
                        break;
                    }
                    /* Emvco: case_id: TA415_X */
                    if((statusBkUp & PH_ERR_MASK) != PH_ERR_SUCCESS )
                    {
                        bNAKCount++;
                    }
                    else
                    {
                        bNAKCount = 0;
                    }
                }
            }
            /* Bail out if the max. retry count is reached */
            else
            {
                /* Deselect card if behaviour is enabled */
                if (pDataParams->bMaxRetryCount > 0U)
                {
                    /* backup retry count */
                    bRetryCount = pDataParams->bMaxRetryCount;

                    /* set retry count to zero to send only one S(DESELECT) */
                    pDataParams->bMaxRetryCount = 0;

                    /* Emvco Doesnot expect DeSelect Command*/
                    if(pDataParams->bOpeMode != RD_LIB_MODE_EMVCO)
                    {
                        /* send deselect (ignore return code) */
                        statusTmp = phpalI14443p4_Sw_Deselect(pDataParams);
                    }
                    /* restore retry count setting */
                    pDataParams->bMaxRetryCount = bRetryCount;

                    /* Return ERR_RECOVERY_FAILED if some response has been received before */
                    if (0U != (bResponseReceived)) {
                        status = PH_ADD_COMPCODE_FIXED(PHPAL_I14443P4_ERR_RECOVERY_FAILED, PH_COMP_PAL_ISO14443P4);
                    }
                }
                break;
            }

            /* Increment retry count */
            ++bRetryCount;
        }

        /* Map TxBuffer to ISO Frame buffer for next loop */
        pTxBuffer = bIsoFrame;
    }
    /* Loop as long as we generate transmissions */
    while (0U != wTxLength);

    /* Reset RxBuffer Start Position */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXBUFFER_STARTPOS, 0));

    return status;
}

phStatus_t phpalI14443p4_Sw_IsValidIBlock(
    uint8_t bCheckCid,
    uint8_t bCid,
    uint8_t bCheckNad,
    uint8_t bNad,
    uint8_t * pRxBuffer,
    uint16_t wRxLength
    )
{
    uint16_t PH_MEMLOC_REM wExpRxLength = 0;

    /* Check RFU bits */
    if ((pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_I_BLOCK_RFU_MASK) != PHPAL_I14443P4_SW_I_BLOCK_RFU_BITS)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    /* We always expect the PCB byte to be present */
    wExpRxLength = 1;

    /* Enable CID checking if neccessary */
    if (0U != (bCheckCid))
    {
        wExpRxLength++;
    }

    /* Enable NAD checking if neccessary */
    if (0U != (bCheckNad))
    {
        wExpRxLength++;
    }

    /* The frame should have the minimum frame length */
    if (wRxLength < wExpRxLength)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    /* CID presence check */
    if ((0u != bCheckCid) &&
        ((pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_CID_FOLLOWING) > 0U) &&
        ((pRxBuffer[PHPAL_I14443P4_SW_PCB_POS+1U] & PHPAL_I14443P4_SW_CID_MASK) == bCid)
        )
    {
        /* CHECK SUCCEEDED */
    }
    /* CID absence check */
    else if ((0u == bCheckCid) && (0u == (pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_CID_FOLLOWING)))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID protocol error */
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    /* NAD presence check */
    if ((0u != bCheckNad) &&
        (0U != (pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_NAD_FOLLOWING)) &&
        (pRxBuffer[wExpRxLength-1u] == bNad)
        )
    {
        /* CHECK SUCCEEDED */
    }
    /* NAD absence check */
    else if ((bCheckNad == 0U) && ((pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_NAD_FOLLOWING) == 0U))
    {
        /* CHECK SUCCEEDED */
    }
    /* NAD protocol error */
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_IsValidRBlock(
    uint8_t bCheckCid,
    uint8_t bCid,
    uint8_t * pRxBuffer,
    uint16_t wRxLength
    )
{
    uint16_t PH_MEMLOC_REM wExpRxLength = 0;

    /* Check RFU bits */
    if ((pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_R_BLOCK_RFU_MASK) != PHPAL_I14443P4_SW_R_BLOCK_RFU_BITS)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    /* We always expect the PCB byte to be present */
    wExpRxLength = 1;

    /* If CID is enabled, we always expect it */
    if (0U != (bCheckCid))
    {
        wExpRxLength++;
    }

    /* The frame should have the exact frame length */
    if (wRxLength != wExpRxLength)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    /* CID presence check */
    if ((0U != bCheckCid) &&
        (0U != (pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_CID_FOLLOWING)) &&
        ((pRxBuffer[PHPAL_I14443P4_SW_PCB_POS+1U] & PHPAL_I14443P4_SW_CID_MASK) == bCid))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID absence check */
    else if ((0u == bCheckCid) && (0u == (pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_CID_FOLLOWING)))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID protocol error */
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_IsValidSBlock(
    uint8_t bCheckCid,
    uint8_t bCid,
    uint8_t * pRxBuffer,
    uint16_t wRxLength
    )
{
    uint16_t PH_MEMLOC_REM wExpRxLength = 0;

    /* Check RFU bits, return with Protocol error if received PCB byte is not related to "Waiting time extension" */
    if ((pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_S_BLOCK_RFU_MASK) != PHPAL_I14443P4_SW_S_BLOCK_RFU_BITS)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    /* We always expect the PCB byte to be present */
    wExpRxLength = 1;

    /* If CID is enabled, we always expect it */
    if (0U != (bCheckCid))
    {
        wExpRxLength++;
    }

    /* If this is a WTX request, we expect an additional INF byte */
    if (0u != (PHPAL_I14443P4_SW_IS_WTX(pRxBuffer[PHPAL_I14443P4_SW_PCB_POS])))
    {
        wExpRxLength++;
    }

    /* The frame should have the exact frame length */
    if (wRxLength != wExpRxLength)
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    /* CID presence check */
    if ((0u != bCheckCid) &&
        (0U != (pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_CID_FOLLOWING)) &&
        ((pRxBuffer[PHPAL_I14443P4_SW_PCB_POS+1U] & PHPAL_I14443P4_SW_CID_MASK) == bCid))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID absence check */
    else if ((0u == bCheckCid) && (0u == (pRxBuffer[PHPAL_I14443P4_SW_PCB_POS] & PHPAL_I14443P4_SW_PCB_CID_FOLLOWING)))
    {
        /* CHECK SUCCEEDED */
    }
    /* CID protocol error */
    else
    {
        return PH_ADD_COMPCODE_FIXED(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_ISO14443P4);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_BuildIBlock(
                                        uint8_t bCidEnabled,
                                        uint8_t bCid,
                                        uint8_t bNadEnabled,
                                        uint8_t bNad,
                                        uint8_t bPcbBlockNum,
                                        uint8_t bChaining,
                                        uint8_t * pTxBuffer,
                                        uint16_t * pTxLength
                                        )
{
    /* I-Block PCB */
    pTxBuffer[PHPAL_I14443P4_SW_PCB_POS] = PHPAL_I14443P4_SW_I_BLOCK | PHPAL_I14443P4_SW_I_BLOCK_RFU_BITS | bPcbBlockNum;
    *pTxLength = 1;

    /* Check if chaining is neccessary */
    if (0U != (bChaining))
    {
        pTxBuffer[PHPAL_I14443P4_SW_PCB_POS] |= PHPAL_I14443P4_SW_PCB_CHAINING;
    }

    /* Append CID if neccessary */
    if (0U != (bCidEnabled))
    {
        pTxBuffer[PHPAL_I14443P4_SW_PCB_POS] |= PHPAL_I14443P4_SW_PCB_CID_FOLLOWING;
        pTxBuffer[(*pTxLength)++] = bCid;
    }

    /* Append NAD if neccessary */
    if (0U != (bNadEnabled))
    {
        pTxBuffer[PHPAL_I14443P4_SW_PCB_POS] |= PHPAL_I14443P4_SW_PCB_NAD_FOLLOWING;
        pTxBuffer[(*pTxLength)++] = bNad;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_BuildRBlock(
                                        uint8_t bCidEnabled,
                                        uint8_t bCid,
                                        uint8_t bPcbBlockNum,
                                        uint8_t bIsAck,
                                        uint8_t * pTxBuffer,
                                        uint16_t * pTxLength
                                        )
{
    /* R(ACK) PCB */
    pTxBuffer[PHPAL_I14443P4_SW_PCB_POS]  = PHPAL_I14443P4_SW_R_BLOCK | PHPAL_I14443P4_SW_R_BLOCK_RFU_BITS | bPcbBlockNum;
    *pTxLength = 1;

    /* Switch to R(NAK) if neccessary */
    if (0U == (bIsAck))
    {
        pTxBuffer[PHPAL_I14443P4_SW_PCB_POS] |= PHPAL_I14443P4_SW_PCB_NAK;
    }

    /* Append CID if supported */
    if (bCidEnabled != 0U)
    {
        pTxBuffer[PHPAL_I14443P4_SW_PCB_POS] |= PHPAL_I14443P4_SW_PCB_CID_FOLLOWING;
        pTxBuffer[(*pTxLength)++] = bCid;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_BuildSBlock(
                                        uint8_t bCidEnabled,
                                        uint8_t bCid,
                                        uint8_t bIsWtx,
                                        uint8_t bWtxm,
                                        uint8_t * pTxBuffer,
                                        uint16_t * pTxLength
                                        )
{
    /* S-Block PCB */
    pTxBuffer[PHPAL_I14443P4_SW_PCB_POS]  = PHPAL_I14443P4_SW_S_BLOCK | PHPAL_I14443P4_SW_S_BLOCK_RFU_BITS;
    *pTxLength = 1;

    /* Append CID if supported */
    if (bCidEnabled != 0U)
    {
        pTxBuffer[PHPAL_I14443P4_SW_PCB_POS] |= PHPAL_I14443P4_SW_PCB_CID_FOLLOWING;
        pTxBuffer[(*pTxLength)++] = bCid;
    }

    /* By default, PCB byte(bit 5 and 6) is configured to DESELECT command.
     * Configure PCB byte to WTX(instead of DESELECT) and append WTXM based on bIsWtx input value.
     */
    if (0U != (bIsWtx))
    {
        pTxBuffer[PHPAL_I14443P4_SW_PCB_POS] |= PHPAL_I14443P4_SW_PCB_WTX;
        pTxBuffer[(*pTxLength)++] = bWtxm;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_SetConfig(
                                      phpalI14443p4_Sw_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t wValue
                                      )
{
    switch (wConfig)
    {
    case PHPAL_I14443P4_CONFIG_OPE_MODE:
        {
            pDataParams->bOpeMode = (uint8_t)wValue;
            break;
        }
    case PHPAL_I14443P4_CONFIG_BLOCKNO:
        {
            if (wValue == 0U)
            {
                pDataParams->bPcbBlockNum = 0;
            }
            else
            {
                pDataParams->bPcbBlockNum = 1;
            }

            break;
        }
    case PHPAL_I14443P4_CONFIG_CID:
        {
            if (0U != (wValue & 0xFF00U))
            {
                if ((wValue & 0x00FFU) > PHPAL_I14443P4_CID_MAX)
                {
                    return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4);
                }
                pDataParams->bCidEnabled = 1;
                pDataParams->bCid = (uint8_t)(wValue);
            }
            else
            {
                pDataParams->bCidEnabled = 0;
                pDataParams->bCid = 0;
            }

            break;
        }
    case PHPAL_I14443P4_CONFIG_NAD:
        {
            if (0U != (wValue & 0xFF00U))
            {
                pDataParams->bNadEnabled = 1;
                pDataParams->bNad = (uint8_t)(wValue);
            }
            else
            {
                pDataParams->bNadEnabled = 0;
                pDataParams->bNad = 0;
            }

            break;
        }
    case PHPAL_I14443P4_CONFIG_FWI:
        {
            if (wValue > PHPAL_I14443P4_FWI_MAX)
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4);
            }
            pDataParams->bFwi = (uint8_t)(wValue);
            break;
        }
    case PHPAL_I14443P4_CONFIG_FSI:
        {
            if (((wValue >> 8U) > PHPAL_I14443P4_FRAMESIZE_MAX) ||
                ((wValue & 0xFFU) > PHPAL_I14443P4_FRAMESIZE_MAX))
            {
                return PH_ADD_COMPCODE_FIXED(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P4);
            }

            pDataParams->bFsdi = (uint8_t)(wValue >> 8U);
            pDataParams->bFsci = (uint8_t)(wValue);
            break;
        }
    case PHPAL_I14443P4_CONFIG_MAXRETRYCOUNT:
        {
            pDataParams->bMaxRetryCount = (uint8_t)(wValue);
            break;
        }
    default:
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_ISO14443P4);
        }
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phpalI14443p4_Sw_GetConfig(
                                      phpalI14443p4_Sw_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t * pValue
                                      )
{
    switch (wConfig)
    {
    case PHPAL_I14443P4_CONFIG_BLOCKNO:
        {
            if (pDataParams->bPcbBlockNum == 0U)
            {
                *pValue = 0;
            }
            else
            {
                *pValue = 1;
            }

            break;
        }
    case PHPAL_I14443P4_CONFIG_CID:
        {
            if (0U != (pDataParams->bCidEnabled))
            {
                *pValue = (uint16_t)(0x0100u | pDataParams->bCid);
            }
            else
            {
                *pValue = (uint16_t)(pDataParams->bCid);
            }

            break;
        }
    case PHPAL_I14443P4_CONFIG_NAD:
        {
            if (0U != (pDataParams->bNadEnabled))
            {
                *pValue = (uint16_t)(0x0100u | pDataParams->bNad);
            }
            else
            {
                *pValue = (uint16_t)(pDataParams->bNad);
            }

            break;
        }
    case PHPAL_I14443P4_CONFIG_FWI:
        {
            *pValue = (uint16_t)pDataParams->bFwi;
            break;
        }
    case PHPAL_I14443P4_CONFIG_FSI:
        {
            *pValue  = (uint16_t)((uint16_t)(pDataParams->bFsdi) << 8U);
            *pValue |= (uint16_t)(pDataParams->bFsci);
            break;
        }
    case PHPAL_I14443P4_CONFIG_MAXRETRYCOUNT:
        {
            *pValue = (uint16_t)pDataParams->bMaxRetryCount;
            break;
        }
    default:
        {
            return PH_ADD_COMPCODE_FIXED(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_PAL_ISO14443P4);
        }
    }

    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHPAL_I14443P4_SW */
