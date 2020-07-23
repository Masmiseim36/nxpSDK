/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_dspi_edma.h"

/***********************************************************************************************************************
 * Definitions
 ***********************************************************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.dspi_edma"
#endif

/*!
 * @brief Structure definition for dspi_master_edma_private_handle_t. The structure is private.
 */
typedef struct _dspi_master_edma_private_handle
{
    SPI_Type *base;                    /*!< DSPI peripheral base address. */
    dspi_master_edma_handle_t *handle; /*!< dspi_master_edma_handle_t handle */
} dspi_master_edma_private_handle_t;

/*!
 * @brief Structure definition for dspi_slave_edma_private_handle_t. The structure is private.
 */
typedef struct _dspi_slave_edma_private_handle
{
    SPI_Type *base;                   /*!< DSPI peripheral base address. */
    dspi_slave_edma_handle_t *handle; /*!< dspi_master_edma_handle_t handle */
} dspi_slave_edma_private_handle_t;

/***********************************************************************************************************************
 * Prototypes
 ***********************************************************************************************************************/
/*!
 * @brief DSPI check whether the transfer size exceed the maximum size EDMA can provide.
 *
 * @param base DSPI peripheral base address.
 * @param transfer A pointer to the dspi_transfer_t structure.
 * @param bitsPerFrame Bits per frame, minimum 4, maximum 16.
 * @return status of status_t.
 */
static status_t DSPI_CheckTransferSize(SPI_Type *base, dspi_transfer_t *transfer, uint32_t bitsPerFrame);

/*!
 * @brief DSPI master prepares the handle structure for EDMA transfer.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 * @param transfer A pointer to the dspi_transfer_t structure.
 */
static void DSPI_MasterPrepareTransferEDMA(SPI_Type *base,
                                           dspi_master_edma_handle_t *handle,
                                           dspi_transfer_t *transfer);

/*!
 * @brief DSPI master prepares the first data to send using EDMA.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_MasterPrepareFirstDataEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle);

/*!
 * @brief DSPI master prepares the last data to send using EDMA.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_MasterPrepareLastDataEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle);

/*!
 * @brief DSPI master configure channel_A for receive.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_MasterConfigReceiveEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle);

/*!
 * @brief DSPI master configure intermediate TCD for send.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_MasterConfigIntermediateEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle);

/*!
 * @brief DSPI master configure channel_B and channel_C for send.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_MasterConfigSendEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle);

/*!
 * @brief DSPI master set the EDMA channel priority for the 3 channels for the transfer.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_MasterSetEDMATransferChannelPriority(SPI_Type *base, dspi_master_edma_handle_t *handle);

/*!
 * @brief EDMA_DspiMasterCallback after the DSPI master transfer completed by using EDMA.
 * This is not a public API.
 */
static void EDMA_DspiMasterCallback(edma_handle_t *edmaHandle,
                                    void *g_dspiEdmaPrivateHandle,
                                    bool transferDone,
                                    uint32_t tcds);

/*!
 * @brief DSPI slave prepares the handle structure for EDMA transfer.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_slave_edma_handle_t structure which stores the transfer state.
 * @param transfer A pointer to the dspi_transfer_t structure.
 */
static void DSPI_SlaverPrepareTransferEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle, dspi_transfer_t *transfer);

/*!
 * @brief DSPI slave prepares the first data to send using EDMA.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_slave_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_SlavePrepareFirstDataEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle);

/*!
 * @brief DSPI slave configure channel_A for receive.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_slave_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_SlaveConfigReceiveEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle);

/*!
 * @brief DSPI slave configure channel_B for send.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_slave_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_SlaveConfigSendEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle);

/*!
 * @brief DSPI slave set the EDMA channel priority for the 2 channels for the transfer.
 *
 * @param base DSPI peripheral base address.
 * @param handle A pointer to the dspi_slave_edma_handle_t structure which stores the transfer state.
 */
static void DSPI_SlaveSetEDMATransferChannelPriority(SPI_Type *base, dspi_slave_edma_handle_t *handle);

/*!
 * @brief EDMA_DspiSlaveCallback after the DSPI slave transfer completed by using EDMA.
 * This is not a public API.
 */
static void EDMA_DspiSlaveCallback(edma_handle_t *edmaHandle,
                                   void *g_dspiEdmaPrivateHandle,
                                   bool transferDone,
                                   uint32_t tcds);

/***********************************************************************************************************************
 * Variables
 ***********************************************************************************************************************/

/*! @brief Pointers to dspi edma handles for each instance. */
static dspi_master_edma_private_handle_t s_dspiMasterEdmaPrivateHandle[FSL_FEATURE_SOC_DSPI_COUNT];
static dspi_slave_edma_private_handle_t s_dspiSlaveEdmaPrivateHandle[FSL_FEATURE_SOC_DSPI_COUNT];

/***********************************************************************************************************************
 * Code
 ***********************************************************************************************************************/

/*!
 * brief Initializes the DSPI master eDMA handle.
 *
 * This function initializes the DSPI eDMA handle which can be used for other DSPI transactional APIs.  Usually, for a
 * specified DSPI instance, call this API once to get the initialized handle.
 *
 * Note that DSPI eDMA has separated (RX and TX as two sources) or shared (RX  and TX are the same source) DMA request
 * source.
 * (1) For the separated DMA request source, enable and set the RX DMAMUX source for edmaRxRegToRxDataHandle and
 * TX DMAMUX source for edmaIntermediaryToTxRegHandle.
 * (2) For the shared DMA request source, enable and set the RX/RX DMAMUX source for the edmaRxRegToRxDataHandle.
 *
 * param base DSPI peripheral base address.
 * param handle DSPI handle pointer to dspi_master_edma_handle_t.
 * param callback DSPI callback.
 * param userData A callback function parameter.
 * param edmaRxRegToRxDataHandle edmaRxRegToRxDataHandle pointer to edma_handle_t.
 * param edmaTxDataToIntermediaryHandle edmaTxDataToIntermediaryHandle pointer to edma_handle_t.
 * param edmaIntermediaryToTxRegHandle edmaIntermediaryToTxRegHandle pointer to edma_handle_t.
 */
void DSPI_MasterTransferCreateHandleEDMA(SPI_Type *base,
                                         dspi_master_edma_handle_t *handle,
                                         dspi_master_edma_transfer_callback_t callback,
                                         void *userData,
                                         edma_handle_t *edmaRxRegToRxDataHandle,
                                         edma_handle_t *edmaTxDataToIntermediaryHandle,
                                         edma_handle_t *edmaIntermediaryToTxRegHandle)
{
    assert(NULL != handle);
    assert(NULL != edmaRxRegToRxDataHandle);
#if (!(defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET))
    assert(NULL != edmaTxDataToIntermediaryHandle);
#endif
    assert(NULL != edmaIntermediaryToTxRegHandle);

    /* Zero the handle. */
    (void)memset(handle, 0, sizeof(*handle));

    uint32_t instance = DSPI_GetInstance(base);

    s_dspiMasterEdmaPrivateHandle[instance].base   = base;
    s_dspiMasterEdmaPrivateHandle[instance].handle = handle;

    handle->callback = callback;
    handle->userData = userData;

    handle->edmaRxRegToRxDataHandle        = edmaRxRegToRxDataHandle;
    handle->edmaTxDataToIntermediaryHandle = edmaTxDataToIntermediaryHandle;
    handle->edmaIntermediaryToTxRegHandle  = edmaIntermediaryToTxRegHandle;
}

static status_t DSPI_CheckTransferSize(SPI_Type *base, dspi_transfer_t *transfer, uint32_t bitsPerFrame)
{
    status_t result       = (status_t)kStatus_Success;
    uint32_t limited_size = 0;
    /* If using a shared RX/TX DMA request, then this limits the amount of data we can transfer
     * due to the linked channel. The max bytes is 511 if 8-bit/frame or 1022 if 16-bit/frame
     */
    if (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
    {
        limited_size = 32767u;
    }
    else
    {
        limited_size = 511u;
    }

    if (bitsPerFrame > 8U)
    {
        if (transfer->dataSize > (limited_size << 1u))
        {
            result = (status_t)kStatus_DSPI_OutOfRange;
        }
        /*The data size should be even if the bitsPerFrame is greater than 8 (that is 2 bytes per frame in dspi) */
        if (0U != (transfer->dataSize & 0x1U))
        {
            result = (status_t)kStatus_InvalidArgument;
        }
    }
    else
    {
        if (transfer->dataSize > limited_size)
        {
            result = (status_t)kStatus_DSPI_OutOfRange;
        }
    }

    return result;
}

static void DSPI_MasterPrepareTransferEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle, dspi_transfer_t *transfer)
{
    assert(NULL != handle);
    assert(NULL != transfer);

    uint32_t tmpMCR                          = 0;
    dspi_command_data_config_t commandStruct = {false, (uint8_t)kDSPI_Ctar0, (uint8_t)kDSPI_Pcs0, false, false};
    uint8_t dummyData                        = DSPI_GetDummyDataInstance(base);

    DSPI_StopTransfer(base);
    DSPI_DisableInterrupts(base, (uint32_t)kDSPI_AllInterruptEnable);
    DSPI_FlushFifo(base, true, true);
    DSPI_ClearStatusFlags(base, (uint32_t)kDSPI_AllStatusFlag);
    DSPI_DisableDMA(base, (uint32_t)kDSPI_RxDmaEnable | (uint32_t)kDSPI_TxDmaEnable);

    commandStruct.whichPcs =
        (uint8_t)(1UL << ((transfer->configFlags & DSPI_MASTER_PCS_MASK) >> DSPI_MASTER_PCS_SHIFT));
    commandStruct.isEndOfQueue       = false;
    commandStruct.clearTransferCount = false;
    commandStruct.whichCtar = (uint8_t)((transfer->configFlags & DSPI_MASTER_CTAR_MASK) >> DSPI_MASTER_CTAR_SHIFT);
    commandStruct.isPcsContinuous =
        (0U != (transfer->configFlags & (uint32_t)kDSPI_MasterPcsContinuous)) ? true : false;
    handle->command = DSPI_MasterGetFormattedCommand(&(commandStruct));

    commandStruct.isEndOfQueue = true;
    commandStruct.isPcsContinuous =
        (0U != (transfer->configFlags & (uint32_t)kDSPI_MasterActiveAfterTransfer)) ? true : false;
    handle->lastCommand = DSPI_MasterGetFormattedCommand(&(commandStruct));

    handle->bitsPerFrame = ((base->CTAR[commandStruct.whichCtar] & SPI_CTAR_FMSZ_MASK) >> SPI_CTAR_FMSZ_SHIFT) + 1U;

    tmpMCR = base->MCR;
    if ((0U != (tmpMCR & SPI_MCR_DIS_RXF_MASK)) || (0U != (tmpMCR & SPI_MCR_DIS_TXF_MASK)))
    {
        handle->fifoSize = 1;
    }
    else
    {
        handle->fifoSize = FSL_FEATURE_DSPI_FIFO_SIZEn(base);
    }
    handle->txData                    = transfer->txData;
    handle->rxData                    = transfer->rxData;
    handle->remainingSendByteCount    = transfer->dataSize;
    handle->remainingReceiveByteCount = transfer->dataSize;
    handle->totalByteCount            = transfer->dataSize;
    handle->txBuffIfNull              = ((uint32_t)dummyData << 8U) | dummyData;
}

static void DSPI_MasterPrepareFirstDataEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle)
{
    uint16_t wordToSend;
    uint8_t byteCountEachSend;
    uint32_t currentCmd;
    uint8_t dummyData = DSPI_GetDummyDataInstance(base);

    /*If dspi has separate dma request , prepare the first data in 'intermediary',
      otherwise, that is dspi has shared dma request, send first 2 data if there is fifo
      or send first 1 data if there is no fifo
    */
    if (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
    {
        /* For DSPI instances with separate RX/TX DMA requests, we'll use the TX DMA request to
         * trigger the TX DMA channel and RX DMA request to trigger the RX DMA channel
         */
        /*Prepare the firt data*/
        if (handle->bitsPerFrame > 8U)
        {
            if (NULL != handle->txData)
            {
                wordToSend = *(handle->txData);
                ++handle->txData; /* increment to next data byte */
                wordToSend |= (uint16_t)(*(handle->txData)) << 8U;
                if (handle->remainingSendByteCount > 2U)
                {
                    ++handle->txData; /* increment to next data byte */
                }
            }
            else
            {
                wordToSend = (((uint16_t)dummyData << 8U) | (uint16_t)dummyData);
            }
            byteCountEachSend = 2U;
        }
        /* Optimized for bits/frame less than or equal to one byte. */
        else
        {
            if (NULL != handle->txData)
            {
                wordToSend = *(handle->txData);
                /* increment to next data word*/
                ++handle->txData;
            }
            else
            {
                wordToSend = dummyData;
            }
            byteCountEachSend = 1U;
        }
        /* If it's the last word */
        if (handle->remainingSendByteCount <= byteCountEachSend)
        {
            handle->lastCommand = (handle->lastCommand & 0xffff0000U) | wordToSend;
            handle->command     = handle->lastCommand;
        }
        /* For all words except the last word , frame > 8bits */
        else
        {
            handle->command = (handle->command & 0xffff0000U) | wordToSend;
        }
    }
    /*dspi has shared dma request*/
    else
    {
        /* For DSPI instances with shared RX/TX DMA requests, we'll use the RX DMA request to
         * trigger ongoing transfers and link it to the TX DMA channel.
         */
        uint8_t dataAlreadyFed = 0;
        uint8_t dataFedMax     = 2;

        /* If bits/frame is greater than one byte */
        if (handle->bitsPerFrame > 8U)
        {
            currentCmd = handle->command;
            while (0U != (DSPI_GetStatusFlags(base) & (uint32_t)kDSPI_TxFifoFillRequestFlag))
            {
                /* For the last word */
                if (handle->remainingSendByteCount <= 2U)
                {
                    currentCmd = handle->lastCommand;
                }
                /* For all words */
                if (NULL != handle->txData)
                {
                    wordToSend = *(handle->txData);
                    ++handle->txData;
                    wordToSend |= (uint16_t)(*(handle->txData)) << 8U;
                    if (handle->remainingSendByteCount > 2U)
                    {
                        ++handle->txData;
                    }
                }
                else
                {
                    wordToSend = (((uint16_t)dummyData << 8U) | (uint16_t)dummyData);
                }
                if (handle->remainingSendByteCount > 2U)
                {
                    handle->remainingSendByteCount -= 2U;
                }
                else
                {
                    handle->remainingSendByteCount = 0U;
                }
                base->PUSHR = (currentCmd & 0xffff0000U) | wordToSend;

                /* Try to clear the TFFF; if the TX FIFO is full this will clear */
                DSPI_ClearStatusFlags(base, (uint32_t)kDSPI_TxFifoFillRequestFlag);

                dataAlreadyFed += 2U;

                /* exit loop if send count is zero, else update local variables for next loop */
                if ((handle->remainingSendByteCount == 0U) || (dataAlreadyFed == (dataFedMax * 2U)))
                {
                    break;
                }
            } /* End of TX FIFO fill while loop */
        }
        /* Optimized for bits/frame less than or equal to one byte. */
        else
        {
            while (0U != (DSPI_GetStatusFlags(base) & (uint32_t)kDSPI_TxFifoFillRequestFlag))
            {
                if (NULL != handle->txData)
                {
                    wordToSend = *(handle->txData);
                    ++handle->txData;
                }
                else
                {
                    wordToSend = dummyData;
                }

                if (handle->remainingSendByteCount == 1U)
                {
                    base->PUSHR = (handle->lastCommand & 0xffff0000U) | wordToSend;
                }
                else
                {
                    base->PUSHR = (handle->command & 0xffff0000U) | wordToSend;
                }

                /* Try to clear the TFFF; if the TX FIFO is full this will clear */
                DSPI_ClearStatusFlags(base, (uint32_t)kDSPI_TxFifoFillRequestFlag);

                --handle->remainingSendByteCount;

                dataAlreadyFed++;

                /* exit loop if send count is zero, else update local variables for next loop */
                if ((handle->remainingSendByteCount == 0U) || (dataAlreadyFed == dataFedMax))
                {
                    break;
                }
            } /* End of TX FIFO fill while loop */
        }
    }
}

static void DSPI_MasterPrepareLastDataEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle)
{
    uint16_t wordToSend;
    uint8_t dummyData                = DSPI_GetDummyDataInstance(base);
    size_t tmpRemainingSendByteCount = handle->remainingSendByteCount;

    /*Calculate the last data: handle->lastCommand*/
    if (((tmpRemainingSendByteCount > 0U) && (1U != (uint8_t)FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))) ||
        ((((tmpRemainingSendByteCount > 1U) && (handle->bitsPerFrame <= 8U)) ||
          ((tmpRemainingSendByteCount > 2U) && (handle->bitsPerFrame > 8U))) &&
         (1U == (uint8_t)FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))))
    {
        if (NULL != handle->txData)
        {
            uint32_t bufferIndex = 0;

            if (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
            {
                if (handle->bitsPerFrame <= 8U)
                {
                    bufferIndex = handle->remainingSendByteCount - 1U;
                }
                else
                {
                    bufferIndex = handle->remainingSendByteCount - 2U;
                }
            }
            else
            {
                bufferIndex = handle->remainingSendByteCount;
            }

            uint32_t tmpLastCommand = handle->lastCommand;
            uint8_t *tmpTxData      = handle->txData;

            if (handle->bitsPerFrame <= 8U)
            {
                tmpLastCommand = (tmpLastCommand & 0xffff0000U) | tmpTxData[bufferIndex - 1U];
            }
            else
            {
                tmpLastCommand = (tmpLastCommand & 0xffff0000U) | ((uint32_t)tmpTxData[bufferIndex - 1U] << 8U) |
                                 tmpTxData[bufferIndex - 2U];
            }

            handle->lastCommand = tmpLastCommand;
        }
        else
        {
            if (handle->bitsPerFrame <= 8U)
            {
                wordToSend = dummyData;
            }
            else
            {
                wordToSend = (((uint16_t)dummyData << 8U) | (uint16_t)dummyData);
            }
            handle->lastCommand = (handle->lastCommand & 0xffff0000U) | wordToSend;
        }
    }
}

static void DSPI_MasterConfigReceiveEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle)
{
    edma_transfer_config_t transferConfigA;
    uint32_t rxAddr = DSPI_GetRxRegisterAddress(base);
    /***channel_A *** used for carry the data from Rx_Data_Register(POPR) to User_Receive_Buffer(rxData)*/
    EDMA_ResetChannel(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel);

    transferConfigA.srcAddr   = (uint32_t)rxAddr;
    transferConfigA.srcOffset = 0;

    if (NULL != handle->rxData)
    {
        transferConfigA.destAddr   = (uint32_t) & (handle->rxData[0]);
        transferConfigA.destOffset = 1;
    }
    else
    {
        transferConfigA.destAddr   = (uint32_t) & (handle->rxBuffIfNull);
        transferConfigA.destOffset = 0;
    }

    transferConfigA.destTransferSize = kEDMA_TransferSize1Bytes;

    if (handle->bitsPerFrame <= 8U)
    {
        transferConfigA.srcTransferSize = kEDMA_TransferSize1Bytes;
        transferConfigA.minorLoopBytes  = 1;
        transferConfigA.majorLoopCounts = handle->remainingReceiveByteCount;
    }
    else
    {
        transferConfigA.srcTransferSize = kEDMA_TransferSize2Bytes;
        transferConfigA.minorLoopBytes  = 2;
        transferConfigA.majorLoopCounts = handle->remainingReceiveByteCount / 2U;
    }

    /* Store the initially configured eDMA minor byte transfer count into the DSPI handle */
    handle->nbytes = (uint8_t)(transferConfigA.minorLoopBytes);

    EDMA_SetTransferConfig(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                           (const edma_transfer_config_t *)(uint32_t)&transferConfigA, NULL);
    EDMA_EnableChannelInterrupts(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                 (uint32_t)kEDMA_MajorInterruptEnable);
    /*Start the EDMA channel_A. */
    EDMA_StartTransfer(handle->edmaRxRegToRxDataHandle);
}

static void DSPI_MasterConfigIntermediateEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle)
{
    edma_transfer_config_t transferConfigB;
    size_t tmpRemainingSendByteCount = 0;
    edma_tcd_t *softwareTCD          = (edma_tcd_t *)((uint32_t)(&handle->dspiSoftwareTCD[1]) & (~0x1FU));

    /***channel_B *** is used for carry the data from User_Send_Buffer to "intermediary" because the SPIx_PUSHR should
    write the 32bits at once time . Then use channel_C to carry the "intermediary" to SPIx_PUSHR. Note that the
    SPIx_PUSHR upper 16 bits are the "command" and the low 16bits are data */

    EDMA_ResetChannel(handle->edmaTxDataToIntermediaryHandle->base, handle->edmaTxDataToIntermediaryHandle->channel);

    /*For DSPI instances with separate RX and TX DMA requests: use the scatter/gather to prepare the last data
     * (handle->lastCommand) to handle->Command*/
    if (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
    {
        transferConfigB.srcAddr          = (uint32_t) & (handle->lastCommand);
        transferConfigB.destAddr         = (uint32_t) & (handle->command);
        transferConfigB.srcTransferSize  = kEDMA_TransferSize4Bytes;
        transferConfigB.destTransferSize = kEDMA_TransferSize4Bytes;
        transferConfigB.srcOffset        = 0;
        transferConfigB.destOffset       = 0;
        transferConfigB.minorLoopBytes   = 4;
        transferConfigB.majorLoopCounts  = 1;

        EDMA_TcdReset(softwareTCD);
        EDMA_TcdSetTransferConfig(softwareTCD, (const edma_transfer_config_t *)(uint32_t)&transferConfigB, NULL);
    }

    tmpRemainingSendByteCount = handle->remainingSendByteCount;
    /*User_Send_Buffer(txData) to intermediary(handle->command)*/
    if (((((tmpRemainingSendByteCount > 2U) && (handle->bitsPerFrame <= 8U)) ||
          ((tmpRemainingSendByteCount > 4U) && (handle->bitsPerFrame > 8U))) &&
         (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))) ||
        (1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base)))
    {
        if (NULL != handle->txData)
        {
            transferConfigB.srcAddr   = (uint32_t)(handle->txData);
            transferConfigB.srcOffset = 1;
        }
        else
        {
            transferConfigB.srcAddr   = (uint32_t)(&handle->txBuffIfNull);
            transferConfigB.srcOffset = 0;
        }

        transferConfigB.destAddr   = (uint32_t)(&handle->command);
        transferConfigB.destOffset = 0;

        transferConfigB.srcTransferSize = kEDMA_TransferSize1Bytes;

        if (handle->bitsPerFrame <= 8U)
        {
            transferConfigB.destTransferSize = kEDMA_TransferSize1Bytes;
            transferConfigB.minorLoopBytes   = 1;

            if (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
            {
                transferConfigB.majorLoopCounts = handle->remainingSendByteCount - 2U;
            }
            else
            {
                /*Only enable channel_B minorlink to channel_C , so need to add one count due to the last time is
                majorlink , the majorlink would not trigger the channel_C*/
                transferConfigB.majorLoopCounts = handle->remainingSendByteCount + 1U;
            }
        }
        else
        {
            transferConfigB.destTransferSize = kEDMA_TransferSize2Bytes;
            transferConfigB.minorLoopBytes   = 2;
            if (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
            {
                transferConfigB.majorLoopCounts = handle->remainingSendByteCount / 2U - 2U;
            }
            else
            {
                /*Only enable channel_B minorlink to channel_C , so need to add one count due to the last time is
                 * majorlink*/
                transferConfigB.majorLoopCounts = handle->remainingSendByteCount / 2U + 1U;
            }
        }

        if (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
        {
            EDMA_SetTransferConfig(handle->edmaTxDataToIntermediaryHandle->base,
                                   handle->edmaTxDataToIntermediaryHandle->channel,
                                   (const edma_transfer_config_t *)(uint32_t)&transferConfigB, softwareTCD);
            EDMA_EnableAutoStopRequest(handle->edmaIntermediaryToTxRegHandle->base,
                                       handle->edmaIntermediaryToTxRegHandle->channel, false);
        }
        else
        {
            EDMA_SetTransferConfig(handle->edmaTxDataToIntermediaryHandle->base,
                                   handle->edmaTxDataToIntermediaryHandle->channel,
                                   (const edma_transfer_config_t *)(uint32_t)&transferConfigB, NULL);
        }
    }
    else
    {
        EDMA_SetTransferConfig(handle->edmaTxDataToIntermediaryHandle->base,
                               handle->edmaTxDataToIntermediaryHandle->channel,
                               (const edma_transfer_config_t *)(uint32_t)&transferConfigB, NULL);
    }
}

static void DSPI_MasterConfigSendEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle)
{
    edma_transfer_config_t transferConfigC;
    uint32_t txAddr = DSPI_MasterGetTxRegisterAddress(base);

    /* The feature of GASKET is that the SPI supports 8-bit or 16-bit writes to the SPIx_PUSHR(PUSH TX FIFO Register),
     * which allows a single write to the command half-word followed by multiple writes to the transmit word.
     * The TX FIFO will save the last command word written, and convert a 8-bit/16-bit write to the
     * transmit word into a 32-bit write that pushes both the command word and transmit word into
     * the TX FIFO (PUSH TX FIFO Register In Master Mode)
     * So, if this feature is supported, we can use use one channel to carry the receive data from
     * receive regsiter to user data buffer, use the other channel to carry the data from user data buffer
     * to transmit register,and use the scatter/gather function to prepare the last data.
     * That is to say, if GASKET feature is supported, we can use only two channels for tansferring data.
     */
#if defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET
    /*  For DSPI instances with separate RX and TX DMA requests: use the scatter/gather to prepare the last data
     * (handle->lastCommand) to PUSHR register.
     */
    EDMA_ResetChannel(handle->edmaIntermediaryToTxRegHandle->base, handle->edmaIntermediaryToTxRegHandle->channel);

    if ((1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base)) ||
        ((handle->remainingSendByteCount > 0) && (1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))))
    {
        transferConfigC.srcAddr          = (uint32_t) & (handle->lastCommand);
        transferConfigC.destAddr         = (uint32_t)txAddr;
        transferConfigC.srcTransferSize  = kEDMA_TransferSize4Bytes;
        transferConfigC.destTransferSize = kEDMA_TransferSize4Bytes;
        transferConfigC.srcOffset        = 0;
        transferConfigC.destOffset       = 0;
        transferConfigC.minorLoopBytes   = 4;
        transferConfigC.majorLoopCounts  = 1;

        EDMA_TcdReset(softwareTCD);
        EDMA_TcdSetTransferConfig(softwareTCD, &transferConfigC, NULL);
    }

    /*User_Send_Buffer(txData) to PUSHR register. */
    if (((handle->remainingSendByteCount > 2U) && (handle->bitsPerFrame <= 8U)) ||
        ((handle->remainingSendByteCount > 4U) && (handle->bitsPerFrame > 8U)))
    {
        if (handle->txData)
        {
            if (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
            {
                /* For DSPI with separate RX and TX DMA requests, one frame data has been carry
                 * to handle->command, so need to reduce the pointer of txData.
                 */
                transferConfigC.srcAddr =
                    (uint32_t)((uint8_t *)(handle->txData) - ((handle->bitsPerFrame <= 8U) ? (1U) : (2U)));
                transferConfigC.srcOffset = 1;
            }
            else
            {
                /* For DSPI with shared RX and TX DMA requests, one or two frame data have been carry
                 * to PUSHR register, so no need to change the pointer of txData.
                 */
                transferConfigC.srcAddr   = (uint32_t)((uint8_t *)(handle->txData));
                transferConfigC.srcOffset = 1;
            }
        }
        else
        {
            transferConfigC.srcAddr   = (uint32_t)(&handle->txBuffIfNull);
            transferConfigC.srcOffset = 0;
        }

        transferConfigC.destAddr   = (uint32_t)txAddr;
        transferConfigC.destOffset = 0;

        transferConfigC.srcTransferSize = kEDMA_TransferSize1Bytes;

        if (handle->bitsPerFrame <= 8U)
        {
            transferConfigC.destTransferSize = kEDMA_TransferSize1Bytes;
            transferConfigC.minorLoopBytes   = 1;

            transferConfigC.majorLoopCounts = handle->remainingSendByteCount - 1U;
        }
        else
        {
            transferConfigC.destTransferSize = kEDMA_TransferSize2Bytes;
            transferConfigC.minorLoopBytes   = 2;
            transferConfigC.majorLoopCounts  = (handle->remainingSendByteCount / 2U) - 1U;
        }

        EDMA_SetTransferConfig(handle->edmaIntermediaryToTxRegHandle->base,
                               handle->edmaIntermediaryToTxRegHandle->channel, &transferConfigC, softwareTCD);
    }
    /* If only one word to transmit, only carry the lastcommand. */
    else
    {
        EDMA_SetTransferConfig(handle->edmaIntermediaryToTxRegHandle->base,
                               handle->edmaIntermediaryToTxRegHandle->channel, &transferConfigC, NULL);
    }
    /*Start the EDMA channel_C transfer. */
    EDMA_StartTransfer(handle->edmaIntermediaryToTxRegHandle);
#else
    DSPI_MasterConfigIntermediateEDMA(base, handle);

    size_t tmpRemainingSendByteCount = 0;
    edma_tcd_t *softwareTCD          = (edma_tcd_t *)((uint32_t)(&handle->dspiSoftwareTCD[1]) & (~0x1FU));

    /***channel_C ***carry the "intermediary" to SPIx_PUSHR. used the edma Scatter Gather function on channel_C to
    handle the last data */
    EDMA_ResetChannel(handle->edmaIntermediaryToTxRegHandle->base, handle->edmaIntermediaryToTxRegHandle->channel);

    tmpRemainingSendByteCount = handle->remainingSendByteCount;
    /*For DSPI instances with shared RX/TX DMA requests: use the scatter/gather to prepare the last data
     * (handle->lastCommand) to SPI_PUSHR*/
    if (((1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base)) && (tmpRemainingSendByteCount > 0U)))
    {
        transferConfigC.srcAddr          = (uint32_t) & (handle->lastCommand);
        transferConfigC.destAddr         = (uint32_t)txAddr;
        transferConfigC.srcTransferSize  = kEDMA_TransferSize4Bytes;
        transferConfigC.destTransferSize = kEDMA_TransferSize4Bytes;
        transferConfigC.srcOffset        = 0;
        transferConfigC.destOffset       = 0;
        transferConfigC.minorLoopBytes   = 4;
        transferConfigC.majorLoopCounts  = 1;

        EDMA_TcdReset(softwareTCD);
        EDMA_TcdSetTransferConfig(softwareTCD, (const edma_transfer_config_t *)(uint32_t)&transferConfigC, NULL);
    }

    tmpRemainingSendByteCount = handle->remainingSendByteCount;
    if (((tmpRemainingSendByteCount > 1U) && (handle->bitsPerFrame <= 8U)) ||
        ((tmpRemainingSendByteCount > 2U) && (handle->bitsPerFrame > 8U)) ||
        (1 == FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base)))
    {
        transferConfigC.srcAddr  = (uint32_t)(&(handle->command));
        transferConfigC.destAddr = (uint32_t)txAddr;

        transferConfigC.srcTransferSize  = kEDMA_TransferSize4Bytes;
        transferConfigC.destTransferSize = kEDMA_TransferSize4Bytes;
        transferConfigC.srcOffset        = 0;
        transferConfigC.destOffset       = 0;
        transferConfigC.minorLoopBytes   = 4;
        if (1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
        {
            if (handle->bitsPerFrame <= 8U)
            {
                transferConfigC.majorLoopCounts = handle->remainingSendByteCount - 1U;
            }
            else
            {
                transferConfigC.majorLoopCounts = (handle->remainingSendByteCount / 2U) - 1U;
            }

            EDMA_SetTransferConfig(handle->edmaIntermediaryToTxRegHandle->base,
                                   handle->edmaIntermediaryToTxRegHandle->channel,
                                   (const edma_transfer_config_t *)(uint32_t)&transferConfigC, softwareTCD);
        }
        else
        {
            transferConfigC.majorLoopCounts = 1;

            EDMA_SetTransferConfig(handle->edmaIntermediaryToTxRegHandle->base,
                                   handle->edmaIntermediaryToTxRegHandle->channel,
                                   (const edma_transfer_config_t *)(uint32_t)&transferConfigC, NULL);
        }

        EDMA_EnableAutoStopRequest(handle->edmaIntermediaryToTxRegHandle->base,
                                   handle->edmaIntermediaryToTxRegHandle->channel, false);
    }
    else
    {
        EDMA_SetTransferConfig(handle->edmaIntermediaryToTxRegHandle->base,
                               handle->edmaIntermediaryToTxRegHandle->channel,
                               (const edma_transfer_config_t *)(uint32_t)&transferConfigC, NULL);
    }
    /*Start the EDMA channel_C transfer. */
    EDMA_StartTransfer(handle->edmaIntermediaryToTxRegHandle);
    /*Start the EDMA channel_B transfer*/
    EDMA_StartTransfer(handle->edmaTxDataToIntermediaryHandle);
#endif
}

static void DSPI_MasterSetEDMATransferChannelPriority(SPI_Type *base, dspi_master_edma_handle_t *handle)
{
#if defined(FSL_FEATURE_DSPI_HAS_GASKET) && FSL_FEATURE_DSPI_HAS_GASKET
    /* Set the channel link.
     * For DSPI instances with shared TX and RX DMA requests, setup channel minor link, first receive data from the
     * receive register, and then carry transmit data to PUSHER register.
     * For DSPI instance with separate TX and RX DMA requests, there is no need to set up channel link.
     */
    if (1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
    {
        /*Set channel priority*/
        uint8_t channelPriorityLow  = handle->edmaRxRegToRxDataHandle->channel;
        uint8_t channelPriorityHigh = handle->edmaIntermediaryToTxRegHandle->channel;
        uint8_t t                   = 0;

        if (channelPriorityLow > channelPriorityHigh)
        {
            t                   = channelPriorityLow;
            channelPriorityLow  = channelPriorityHigh;
            channelPriorityHigh = t;
        }

        edma_channel_Preemption_config_t preemption_config_t;
        preemption_config_t.enableChannelPreemption = true;
        preemption_config_t.enablePreemptAbility    = true;
        preemption_config_t.channelPriority         = channelPriorityLow;

        EDMA_SetChannelPreemptionConfig(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                        &preemption_config_t);

        preemption_config_t.channelPriority = channelPriorityHigh;
        EDMA_SetChannelPreemptionConfig(handle->edmaIntermediaryToTxRegHandle->base,
                                        handle->edmaIntermediaryToTxRegHandle->channel, &preemption_config_t);
        /*if there is Rx DMA request , carry the 32bits data (handle->command) to user data first , then link to
          channelC to carry the next data to PUSHER register.(txData to PUSHER) */
        if (handle->remainingSendByteCount > 0U)
        {
            EDMA_SetChannelLink(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                kEDMA_MinorLink, handle->edmaIntermediaryToTxRegHandle->channel);
        }
    }

    DSPI_EnableDMA(base, kDSPI_RxDmaEnable | kDSPI_TxDmaEnable);
    /* Setup control info to PUSHER register. */
    *((uint16_t *)&(base->PUSHR) + 1) = (handle->command >> 16U);
#else
    /*Set channel priority*/
    uint8_t channelPriorityLow  = handle->edmaRxRegToRxDataHandle->channel;
    uint8_t channelPriorityMid  = handle->edmaTxDataToIntermediaryHandle->channel;
    uint8_t channelPriorityHigh = handle->edmaIntermediaryToTxRegHandle->channel;
    uint8_t t                   = 0;
    if (channelPriorityLow > channelPriorityMid)
    {
        t                  = channelPriorityLow;
        channelPriorityLow = channelPriorityMid;
        channelPriorityMid = t;
    }

    if (channelPriorityLow > channelPriorityHigh)
    {
        t                   = channelPriorityLow;
        channelPriorityLow  = channelPriorityHigh;
        channelPriorityHigh = t;
    }

    if (channelPriorityMid > channelPriorityHigh)
    {
        t                   = channelPriorityMid;
        channelPriorityMid  = channelPriorityHigh;
        channelPriorityHigh = t;
    }
    edma_channel_Preemption_config_t preemption_config_t;
    preemption_config_t.enableChannelPreemption = true;
    preemption_config_t.enablePreemptAbility    = true;
    preemption_config_t.channelPriority         = channelPriorityLow;

    if (1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
    {
        EDMA_SetChannelPreemptionConfig(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);

        preemption_config_t.channelPriority = channelPriorityMid;
        EDMA_SetChannelPreemptionConfig(handle->edmaTxDataToIntermediaryHandle->base,
                                        handle->edmaTxDataToIntermediaryHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);

        preemption_config_t.channelPriority = channelPriorityHigh;
        EDMA_SetChannelPreemptionConfig(handle->edmaIntermediaryToTxRegHandle->base,
                                        handle->edmaIntermediaryToTxRegHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);
        /*Set the channel link.*/
        if (handle->remainingSendByteCount > 0U)
        {
            EDMA_SetChannelLink(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                kEDMA_MinorLink, handle->edmaTxDataToIntermediaryHandle->channel);

            EDMA_SetChannelLink(handle->edmaTxDataToIntermediaryHandle->base,
                                handle->edmaTxDataToIntermediaryHandle->channel, kEDMA_MinorLink,
                                handle->edmaIntermediaryToTxRegHandle->channel);
        }

        DSPI_EnableDMA(base, (uint32_t)kDSPI_RxDmaEnable);
    }
    else
    {
        EDMA_SetChannelPreemptionConfig(handle->edmaIntermediaryToTxRegHandle->base,
                                        handle->edmaIntermediaryToTxRegHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);

        preemption_config_t.channelPriority = channelPriorityMid;
        EDMA_SetChannelPreemptionConfig(handle->edmaTxDataToIntermediaryHandle->base,
                                        handle->edmaTxDataToIntermediaryHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);

        preemption_config_t.channelPriority = channelPriorityHigh;
        EDMA_SetChannelPreemptionConfig(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);
        /*Set the channel link.*/
        /*if there is Tx DMA request , carry the 32bits data (handle->command) to PUSHR first , then link to channelB
        to prepare the next 32bits data (txData to handle->command) */
        if (handle->remainingSendByteCount > 1U)
        {
            EDMA_SetChannelLink(handle->edmaIntermediaryToTxRegHandle->base,
                                handle->edmaIntermediaryToTxRegHandle->channel, kEDMA_MajorLink,
                                handle->edmaTxDataToIntermediaryHandle->channel);
        }

        DSPI_EnableDMA(base, (uint32_t)kDSPI_RxDmaEnable | (uint32_t)kDSPI_TxDmaEnable);
    }
#endif
}

/*!
 * brief DSPI master transfer data using eDMA.
 *
 * This function transfers data using eDMA. This is a non-blocking function, which returns right away. When all data
 * is transferred, the callback function is called.
 *
 * param base DSPI peripheral base address.
 * param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 * param transfer A pointer to the dspi_transfer_t structure.
 * return status of status_t.
 */
status_t DSPI_MasterTransferEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle, dspi_transfer_t *transfer)
{
    assert((NULL != handle) && (NULL != transfer));

    /* If send/receive length is zero, or both tx and rx data is NULL */
    if ((transfer->dataSize == 0U) || ((NULL == transfer->txData) && (NULL == transfer->rxData)))
    {
        return kStatus_InvalidArgument;
    }

    /* Check that we're not busy.*/
    if (handle->state == (uint8_t)kDSPI_Busy)
    {
        return kStatus_DSPI_Busy;
    }

    status_t result   = kStatus_Success;
    uint32_t instance = DSPI_GetInstance(base);
    handle->state     = (uint8_t)kDSPI_Busy;

    EDMA_SetCallback(handle->edmaRxRegToRxDataHandle, EDMA_DspiMasterCallback,
                     &s_dspiMasterEdmaPrivateHandle[instance]);

    /* Prepare the handle structure for EDMA transfer. */
    DSPI_MasterPrepareTransferEDMA(base, handle, transfer);

    /* Check whether the transfer size exceed the maximum size EDMA can provide. */
    result = DSPI_CheckTransferSize(base, transfer, handle->bitsPerFrame);
    if ((status_t)kStatus_Success != result)
    {
        handle->state = (uint8_t)kDSPI_Idle;
        return result;
    }

    /*
    (1)For DSPI instances with shared RX/TX DMA requests:
    Rx DMA request -> channel_A -> channel_B-> channel_C.
    channel_A minor link to channel_B , channel_B minor link to channel_C.

    In DSPI_MasterPrepareFirstDataEDMA, 1 or 2 data is pushed to SPI_PUSHR,
    just start the DMA tansfer.
    channel_A: SPI_POPR(POP RX FIFO Register) to rxData,
    channel_B: next txData to handle->command (low 16 bits),
    channel_C: handle->command (32 bits) to SPI_PUSHR, and use the scatter/gather to transfer the last data
    (handle->lastCommand to SPI_PUSHR).

    (2)For DSPI instances with separate RX and TX DMA requests:
    Rx DMA request -> channel_A
    Tx DMA request -> channel_C -> channel_B .
    channel_C major link to channel_B.
    In DSPI_MasterPrepareFirstDataEDMA, the first data is prepared in 'intermediary' before the DMA transfer
    and then channel_B is used to prepare the next data to 'intermediary'.

    channel_A: SPI_POPR(POP RX FIFO Register) to rxData,
    channel_C: handle->command (32 bits) to SPI_PUSHR,
    channel_B: next txData to handle->command (low 16 bits), and use the scatter/gather to prepare the last data
    (handle->lastCommand to handle->Command).
    */

    /* Prepare the first and the last data to send using EDMA. */
    DSPI_MasterPrepareFirstDataEDMA(base, handle);
    DSPI_MasterPrepareLastDataEDMA(base, handle);

    /* Prepare EDMA channel_A for receive */
    DSPI_MasterConfigReceiveEDMA(base, handle);
    /* Prepare EDMA channel_B and channel_C for send */
    DSPI_MasterConfigSendEDMA(base, handle);

    /* Set the EDMA channel priority for the 3 channels for the transfer. */
    DSPI_MasterSetEDMATransferChannelPriority(base, handle);

    DSPI_StartTransfer(base);

    return kStatus_Success;
}

/*!
 * brief Transfers a block of data using a eDMA method.
 *
 * This function transfers data using eDNA, the transfer mechanism is half-duplex. This is a non-blocking function,
 * which returns right away. When all data is transferred, the callback function is called.
 *
 * param base DSPI base pointer
 * param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 * param transfer A pointer to the dspi_half_duplex_transfer_t structure.
 * return status of status_t.
 */
status_t DSPI_MasterHalfDuplexTransferEDMA(SPI_Type *base,
                                           dspi_master_edma_handle_t *handle,
                                           dspi_half_duplex_transfer_t *xfer)
{
    assert(NULL != xfer);
    assert(NULL != handle);
    dspi_transfer_t tempXfer = {0};
    status_t status;

    if (true == xfer->isTransmitFirst)
    {
        tempXfer.txData   = xfer->txData;
        tempXfer.rxData   = NULL;
        tempXfer.dataSize = xfer->txDataSize;
    }
    else
    {
        tempXfer.txData   = NULL;
        tempXfer.rxData   = xfer->rxData;
        tempXfer.dataSize = xfer->rxDataSize;
    }
    /* If the pcs pin keep assert between transmit and receive. */
    if (true == xfer->isPcsAssertInTransfer)
    {
        tempXfer.configFlags = (xfer->configFlags) | (uint32_t)kDSPI_MasterActiveAfterTransfer;
    }
    else
    {
        tempXfer.configFlags = (xfer->configFlags) & (~(uint32_t)kDSPI_MasterActiveAfterTransfer);
    }

    status = DSPI_MasterTransferBlocking(base, &tempXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (true == xfer->isTransmitFirst)
    {
        tempXfer.txData   = NULL;
        tempXfer.rxData   = xfer->rxData;
        tempXfer.dataSize = xfer->rxDataSize;
    }
    else
    {
        tempXfer.txData   = xfer->txData;
        tempXfer.rxData   = NULL;
        tempXfer.dataSize = xfer->txDataSize;
    }
    tempXfer.configFlags = xfer->configFlags;

    status = DSPI_MasterTransferEDMA(base, handle, &tempXfer);

    return status;
}

static void EDMA_DspiMasterCallback(edma_handle_t *edmaHandle,
                                    void *g_dspiEdmaPrivateHandle,
                                    bool transferDone,
                                    uint32_t tcds)
{
    assert(NULL != edmaHandle);
    assert(NULL != g_dspiEdmaPrivateHandle);

    dspi_master_edma_private_handle_t *dspiEdmaPrivateHandle;

    dspiEdmaPrivateHandle = (dspi_master_edma_private_handle_t *)g_dspiEdmaPrivateHandle;

    DSPI_DisableDMA((dspiEdmaPrivateHandle->base), (uint32_t)kDSPI_RxDmaEnable | (uint32_t)kDSPI_TxDmaEnable);

    dspiEdmaPrivateHandle->handle->state = (uint8_t)kDSPI_Idle;

    if (NULL != dspiEdmaPrivateHandle->handle->callback)
    {
        dspiEdmaPrivateHandle->handle->callback(dspiEdmaPrivateHandle->base, dspiEdmaPrivateHandle->handle,
                                                kStatus_Success, dspiEdmaPrivateHandle->handle->userData);
    }
}

/*!
 * brief DSPI master aborts a transfer which is using eDMA.
 *
 * This function aborts a transfer which is using eDMA.
 *
 * param base DSPI peripheral base address.
 * param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 */
void DSPI_MasterTransferAbortEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle)
{
    assert(NULL != handle);

    DSPI_StopTransfer(base);

    DSPI_DisableDMA(base, (uint32_t)kDSPI_RxDmaEnable | (uint32_t)kDSPI_TxDmaEnable);

    EDMA_AbortTransfer(handle->edmaRxRegToRxDataHandle);
    EDMA_AbortTransfer(handle->edmaTxDataToIntermediaryHandle);
    EDMA_AbortTransfer(handle->edmaIntermediaryToTxRegHandle);

    handle->state = (uint8_t)kDSPI_Idle;
}

/*!
 * brief Gets the master eDMA transfer count.
 *
 * This function gets the master eDMA transfer count.
 *
 * param base DSPI peripheral base address.
 * param handle A pointer to the dspi_master_edma_handle_t structure which stores the transfer state.
 * param count A number of bytes transferred by the non-blocking transaction.
 * return status of status_t.
 */
status_t DSPI_MasterTransferGetCountEDMA(SPI_Type *base, dspi_master_edma_handle_t *handle, size_t *count)
{
    assert(NULL != handle);

    if (NULL == count)
    {
        return kStatus_InvalidArgument;
    }

    /* Catch when there is not an active transfer. */
    if (handle->state != (uint8_t)kDSPI_Busy)
    {
        *count = 0;
        return kStatus_NoTransferInProgress;
    }

    size_t bytes;

    bytes = (uint32_t)handle->nbytes * EDMA_GetRemainingMajorLoopCount(handle->edmaRxRegToRxDataHandle->base,
                                                                       handle->edmaRxRegToRxDataHandle->channel);

    *count = handle->totalByteCount - bytes;

    return kStatus_Success;
}

/*!
 * brief Initializes the DSPI slave eDMA handle.
 *
 * This function initializes the DSPI eDMA handle which can be used for other DSPI transactional APIs.  Usually, for a
 * specified DSPI instance, call this API once to get the initialized handle.
 *
 * Note that DSPI eDMA has separated (RN and TX in 2 sources) or shared (RX  and TX are the same source) DMA request
 * source.
 * (1)For the separated DMA request source, enable and set the RX DMAMUX source for edmaRxRegToRxDataHandle and
 * TX DMAMUX source for edmaTxDataToTxRegHandle.
 * (2)For the shared DMA request source,  enable and set the RX/RX DMAMUX source for the edmaRxRegToRxDataHandle.
 *
 * param base DSPI peripheral base address.
 * param handle DSPI handle pointer to dspi_slave_edma_handle_t.
 * param callback DSPI callback.
 * param userData A callback function parameter.
 * param edmaRxRegToRxDataHandle edmaRxRegToRxDataHandle pointer to edma_handle_t.
 * param edmaTxDataToTxRegHandle edmaTxDataToTxRegHandle pointer to edma_handle_t.
 */
void DSPI_SlaveTransferCreateHandleEDMA(SPI_Type *base,
                                        dspi_slave_edma_handle_t *handle,
                                        dspi_slave_edma_transfer_callback_t callback,
                                        void *userData,
                                        edma_handle_t *edmaRxRegToRxDataHandle,
                                        edma_handle_t *edmaTxDataToTxRegHandle)
{
    assert(NULL != handle);
    assert(NULL != edmaRxRegToRxDataHandle);
    assert(NULL != edmaTxDataToTxRegHandle);

    /* Zero the handle. */
    (void)memset(handle, 0, sizeof(*handle));

    uint32_t instance = DSPI_GetInstance(base);

    s_dspiSlaveEdmaPrivateHandle[instance].base   = base;
    s_dspiSlaveEdmaPrivateHandle[instance].handle = handle;

    handle->callback = callback;
    handle->userData = userData;

    handle->edmaRxRegToRxDataHandle = edmaRxRegToRxDataHandle;
    handle->edmaTxDataToTxRegHandle = edmaTxDataToTxRegHandle;
}

static void DSPI_SlaverPrepareTransferEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle, dspi_transfer_t *transfer)
{
    assert(NULL != handle);
    assert(NULL != transfer);

    DSPI_StopTransfer(base);
    DSPI_DisableInterrupts(base, (uint32_t)kDSPI_AllInterruptEnable);
    DSPI_FlushFifo(base, true, true);
    DSPI_ClearStatusFlags(base, (uint32_t)kDSPI_AllStatusFlag);
    DSPI_DisableDMA(base, (uint32_t)kDSPI_RxDmaEnable | (uint32_t)kDSPI_TxDmaEnable);

    uint8_t whichCtar = (uint8_t)((transfer->configFlags & DSPI_SLAVE_CTAR_MASK) >> DSPI_SLAVE_CTAR_SHIFT);
    handle->bitsPerFrame =
        (((base->CTAR_SLAVE[whichCtar]) & SPI_CTAR_SLAVE_FMSZ_MASK) >> SPI_CTAR_SLAVE_FMSZ_SHIFT) + 1U;

    /* Store transfer information */
    handle->txData                    = transfer->txData;
    handle->rxData                    = transfer->rxData;
    handle->remainingSendByteCount    = transfer->dataSize;
    handle->remainingReceiveByteCount = transfer->dataSize;
    handle->totalByteCount            = transfer->dataSize;
}

static void DSPI_SlavePrepareFirstDataEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle)
{
    uint32_t wordToSend    = 0;
    uint8_t dummyData      = DSPI_GetDummyDataInstance(base);
    uint8_t dataAlreadyFed = 0;
    uint8_t dataFedMax     = 2;
    /*if DSPI has separate RX/TX DMA request, do not need to prepare data first .
      otherwise if dspi has shared dma request, push first 2 piece of data to fifo if there is fifo,
      or push first 1 piece of data to slaveGetTxRegister if there is no fifo*/
    if (1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
    {
        /* For DSPI instance has shared RX/TX DMA requests, we'll use the RX DMA request to
         * trigger ongoing transfer and will link to the TX DMA channel from the RX DMA channel. */
        /* If bits/frame is greater than one byte */
        if (handle->bitsPerFrame > 8U)
        {
            while (0U != (DSPI_GetStatusFlags(base) & (uint32_t)kDSPI_TxFifoFillRequestFlag))
            {
                if (NULL != handle->txData)
                {
                    wordToSend = *(handle->txData);
                    ++handle->txData; /* Increment to next data byte */

                    wordToSend |= (uint32_t)(*(handle->txData)) << 8U;
                    ++handle->txData; /* Increment to next data byte */
                }
                else
                {
                    wordToSend = ((uint32_t)dummyData << 8U) | dummyData;
                }
                handle->remainingSendByteCount -= 2U; /* decrement remainingSendByteCount by 2 */
                base->PUSHR_SLAVE = wordToSend;

                /* Try to clear the TFFF; if the TX FIFO is full this will clear */
                DSPI_ClearStatusFlags(base, (uint32_t)kDSPI_TxFifoFillRequestFlag);

                dataAlreadyFed += 2U;

                /* Exit loop if send count is zero, else update local variables for next loop */
                if ((handle->remainingSendByteCount == 0U) || (dataAlreadyFed == (dataFedMax * 2U)))
                {
                    break;
                }
            } /* End of TX FIFO fill while loop */
        }
        /* Optimized for bits/frame less than or equal to one byte. */
        else
        {
            while (0U != (DSPI_GetStatusFlags(base) & (uint32_t)kDSPI_TxFifoFillRequestFlag))
            {
                if (NULL != handle->txData)
                {
                    wordToSend = *(handle->txData);
                    /* Increment to next data word*/
                    ++handle->txData;
                }
                else
                {
                    wordToSend = dummyData;
                }
                /* Decrement remainingSendByteCount*/
                --handle->remainingSendByteCount;
                base->PUSHR_SLAVE = wordToSend;

                /* Try to clear the TFFF; if the TX FIFO is full this will clear */
                DSPI_ClearStatusFlags(base, (uint32_t)kDSPI_TxFifoFillRequestFlag);

                dataAlreadyFed++;

                /* Exit loop if send count is zero, else update local variables for next loop */
                if ((handle->remainingSendByteCount == 0U) || (dataAlreadyFed == dataFedMax))
                {
                    break;
                }
            } /* End of TX FIFO fill while loop */
        }
    }
}

static void DSPI_SlaveConfigReceiveEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle)
{
    /* channel_A *** used for carry the data from Rx_Data_Register(POPR) to User_Receive_Buffer*/
    uint32_t rxAddr = DSPI_GetRxRegisterAddress(base);
    edma_transfer_config_t transferConfigA;

    EDMA_ResetChannel(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel);

    transferConfigA.srcAddr   = (uint32_t)rxAddr;
    transferConfigA.srcOffset = 0;

    if (NULL != handle->rxData)
    {
        transferConfigA.destAddr   = (uint32_t) & (handle->rxData[0]);
        transferConfigA.destOffset = 1;
    }
    else
    {
        transferConfigA.destAddr   = (uint32_t) & (handle->rxBuffIfNull);
        transferConfigA.destOffset = 0;
    }

    transferConfigA.destTransferSize = kEDMA_TransferSize1Bytes;

    if (handle->bitsPerFrame <= 8U)
    {
        transferConfigA.srcTransferSize = kEDMA_TransferSize1Bytes;
        transferConfigA.minorLoopBytes  = 1;
        transferConfigA.majorLoopCounts = handle->remainingReceiveByteCount;
    }
    else
    {
        transferConfigA.srcTransferSize = kEDMA_TransferSize2Bytes;
        transferConfigA.minorLoopBytes  = 2;
        transferConfigA.majorLoopCounts = handle->remainingReceiveByteCount / 2U;
    }

    /* Store the initially configured eDMA minor byte transfer count into the DSPI handle */
    handle->nbytes = (uint8_t)(transferConfigA.minorLoopBytes);

    EDMA_SetTransferConfig(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                           (const edma_transfer_config_t *)(uint32_t)&transferConfigA, NULL);
    EDMA_EnableChannelInterrupts(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                 (uint32_t)kEDMA_MajorInterruptEnable);
    EDMA_StartTransfer(handle->edmaRxRegToRxDataHandle);
}

static void DSPI_SlaveConfigSendEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle)
{
    /***channel_B *** used for carry the data from User_Send_Buffer to Tx_Data_Register(PUSHR_SLAVE)*/
    uint32_t txAddr   = DSPI_SlaveGetTxRegisterAddress(base);
    uint8_t dummyData = DSPI_GetDummyDataInstance(base);
    edma_transfer_config_t transferConfigB;

    EDMA_ResetChannel(handle->edmaTxDataToTxRegHandle->base, handle->edmaTxDataToTxRegHandle->channel);

    transferConfigB.destAddr   = (uint32_t)txAddr;
    transferConfigB.destOffset = 0;

    if (NULL != handle->txData)
    {
        transferConfigB.srcAddr   = (uint32_t) & (handle->txData[0]);
        transferConfigB.srcOffset = 1;
    }
    else
    {
        transferConfigB.srcAddr   = (uint32_t) & (handle->txBuffIfNull);
        transferConfigB.srcOffset = 0;

        if (handle->bitsPerFrame <= 8U)
        {
            handle->txBuffIfNull = dummyData;
        }
        else
        {
            handle->txBuffIfNull = ((uint32_t)dummyData << 8U) | dummyData;
        }
    }

    transferConfigB.srcTransferSize = kEDMA_TransferSize1Bytes;

    if (handle->bitsPerFrame <= 8U)
    {
        transferConfigB.destTransferSize = kEDMA_TransferSize1Bytes;
        transferConfigB.minorLoopBytes   = 1;
        transferConfigB.majorLoopCounts  = handle->remainingSendByteCount;
    }
    else
    {
        transferConfigB.destTransferSize = kEDMA_TransferSize2Bytes;
        transferConfigB.minorLoopBytes   = 2;
        transferConfigB.majorLoopCounts  = handle->remainingSendByteCount / 2U;
    }

    EDMA_SetTransferConfig(handle->edmaTxDataToTxRegHandle->base, handle->edmaTxDataToTxRegHandle->channel,
                           (const edma_transfer_config_t *)(uint32_t)&transferConfigB, NULL);
    EDMA_StartTransfer(handle->edmaTxDataToTxRegHandle);
}

static void DSPI_SlaveSetEDMATransferChannelPriority(SPI_Type *base, dspi_slave_edma_handle_t *handle)
{
    /*Set channel priority*/
    edma_channel_Preemption_config_t preemption_config_t;
    uint8_t channelPriorityLow  = handle->edmaRxRegToRxDataHandle->channel;
    uint8_t channelPriorityHigh = handle->edmaTxDataToTxRegHandle->channel;
    uint8_t t                   = 0;

    if (channelPriorityLow > channelPriorityHigh)
    {
        t                   = channelPriorityLow;
        channelPriorityLow  = channelPriorityHigh;
        channelPriorityHigh = t;
    }

    preemption_config_t.enableChannelPreemption = true;
    preemption_config_t.enablePreemptAbility    = true;
    preemption_config_t.channelPriority         = channelPriorityLow;

    if (1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
    {
        EDMA_SetChannelPreemptionConfig(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);

        preemption_config_t.channelPriority = channelPriorityHigh;
        EDMA_SetChannelPreemptionConfig(handle->edmaTxDataToTxRegHandle->base, handle->edmaTxDataToTxRegHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);
    }
    else
    {
        EDMA_SetChannelPreemptionConfig(handle->edmaTxDataToTxRegHandle->base, handle->edmaTxDataToTxRegHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);

        preemption_config_t.channelPriority = channelPriorityHigh;
        EDMA_SetChannelPreemptionConfig(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                        (const edma_channel_Preemption_config_t *)(uint32_t)&preemption_config_t);
    }

    /*Set the channel link.
    For DSPI instances with shared RX/TX DMA requests: Rx DMA request -> channel_A -> channel_C.
    For DSPI instances with separate RX and TX DMA requests:
    Rx DMA request -> channel_A
    Tx DMA request -> channel_C */
    if (1 != FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(base))
    {
        if (handle->remainingSendByteCount > 0U)
        {
            EDMA_SetChannelLink(handle->edmaRxRegToRxDataHandle->base, handle->edmaRxRegToRxDataHandle->channel,
                                kEDMA_MinorLink, handle->edmaTxDataToTxRegHandle->channel);
        }
        DSPI_EnableDMA(base, (uint32_t)kDSPI_RxDmaEnable);
    }
    else
    {
        DSPI_EnableDMA(base, (uint32_t)kDSPI_RxDmaEnable | (uint32_t)kDSPI_TxDmaEnable);
    }
}

/*!
 * brief DSPI slave transfer data using eDMA.
 *
 * This function transfers data using eDMA. This is a non-blocking function, which returns right away. When all data
 * is transferred, the callback function is called.
 * Note that the slave eDMA transfer doesn't support transfer_size is 1 when the bitsPerFrame is greater
 * than eight.

 * param base DSPI peripheral base address.
 * param handle A pointer to the dspi_slave_edma_handle_t structure which stores the transfer state.
 * param transfer A pointer to the dspi_transfer_t structure.
 * return status of status_t.
 */
status_t DSPI_SlaveTransferEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle, dspi_transfer_t *transfer)
{
    assert((NULL != handle) && (NULL != transfer));

    status_t result = kStatus_Success;

    /* If send/receive length is zero, or both tx and rx data is NULL */
    if ((transfer->dataSize == 0U) || ((NULL == transfer->txData) && (NULL == transfer->rxData)))
    {
        return kStatus_InvalidArgument;
    }

    /* Check that we're not busy.*/
    if (handle->state == (uint8_t)kDSPI_Busy)
    {
        return kStatus_DSPI_Busy;
    }

    handle->state     = (uint8_t)kDSPI_Busy;
    uint32_t instance = DSPI_GetInstance(base);

    EDMA_SetCallback(handle->edmaRxRegToRxDataHandle, EDMA_DspiSlaveCallback, &s_dspiSlaveEdmaPrivateHandle[instance]);
    /* Prepare the handle structure for EDMA transfer. */
    DSPI_SlaverPrepareTransferEDMA(base, handle, transfer);

    /* Check whether the transfer size exceed the maximum size EDMA can provide. */
    result = DSPI_CheckTransferSize(base, transfer, handle->bitsPerFrame);
    if ((status_t)kStatus_Success != result)
    {
        handle->state = (uint8_t)kDSPI_Idle;
        return result;
    }

    /* Prepare the first data to send using EDMA. */
    DSPI_SlavePrepareFirstDataEDMA(base, handle);

    if (handle->remainingReceiveByteCount > 0U)
    {
        /* Prepare EDMA channel_A for receive */
        DSPI_SlaveConfigReceiveEDMA(base, handle);
    }

    if (handle->remainingSendByteCount > 0U)
    {
        /* Prepare EDMA channel_B for send */
        DSPI_SlaveConfigSendEDMA(base, handle);
    }

    /* Set the EDMA channel priority for the 2 channels for the transfer. */
    DSPI_SlaveSetEDMATransferChannelPriority(base, handle);

    DSPI_StartTransfer(base);
    return kStatus_Success;
}

static void EDMA_DspiSlaveCallback(edma_handle_t *edmaHandle,
                                   void *g_dspiEdmaPrivateHandle,
                                   bool transferDone,
                                   uint32_t tcds)
{
    assert(NULL != edmaHandle);
    assert(NULL != g_dspiEdmaPrivateHandle);

    dspi_slave_edma_private_handle_t *dspiEdmaPrivateHandle;

    dspiEdmaPrivateHandle = (dspi_slave_edma_private_handle_t *)g_dspiEdmaPrivateHandle;

    DSPI_DisableDMA((dspiEdmaPrivateHandle->base), (uint32_t)kDSPI_RxDmaEnable | (uint32_t)kDSPI_TxDmaEnable);

    dspiEdmaPrivateHandle->handle->state = (uint8_t)kDSPI_Idle;

    if (NULL != dspiEdmaPrivateHandle->handle->callback)
    {
        dspiEdmaPrivateHandle->handle->callback(dspiEdmaPrivateHandle->base, dspiEdmaPrivateHandle->handle,
                                                kStatus_Success, dspiEdmaPrivateHandle->handle->userData);
    }
}

/*!
 * brief DSPI slave aborts a transfer which is using eDMA.
 *
 * This function aborts a transfer which is using eDMA.
 *
 * param base DSPI peripheral base address.
 * param handle A pointer to the dspi_slave_edma_handle_t structure which stores the transfer state.
 */
void DSPI_SlaveTransferAbortEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle)
{
    assert(NULL != handle);

    DSPI_StopTransfer(base);

    DSPI_DisableDMA(base, (uint32_t)kDSPI_RxDmaEnable | (uint32_t)kDSPI_TxDmaEnable);

    EDMA_AbortTransfer(handle->edmaRxRegToRxDataHandle);
    EDMA_AbortTransfer(handle->edmaTxDataToTxRegHandle);

    handle->state = (uint8_t)kDSPI_Idle;
}

/*!
 * brief Gets the slave eDMA transfer count.
 *
 * This function gets the slave eDMA transfer count.
 *
 * param base DSPI peripheral base address.
 * param handle A pointer to the dspi_slave_edma_handle_t structure which stores the transfer state.
 * param count A number of bytes transferred so far by the non-blocking transaction.
 * return status of status_t.
 */
status_t DSPI_SlaveTransferGetCountEDMA(SPI_Type *base, dspi_slave_edma_handle_t *handle, size_t *count)
{
    assert(NULL != handle);

    if (NULL == count)
    {
        return kStatus_InvalidArgument;
    }

    /* Catch when there is not an active transfer. */
    if (handle->state != (uint8_t)kDSPI_Busy)
    {
        *count = 0;
        return kStatus_NoTransferInProgress;
    }

    size_t bytes;

    bytes = (uint32_t)handle->nbytes * EDMA_GetRemainingMajorLoopCount(handle->edmaRxRegToRxDataHandle->base,
                                                                       handle->edmaRxRegToRxDataHandle->channel);

    *count = handle->totalByteCount - bytes;

    return kStatus_Success;
}
