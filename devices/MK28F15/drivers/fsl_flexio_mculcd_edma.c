/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_flexio_mculcd_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.flexio_mculcd_edma"
#endif


#define EDMA_MAX_MAJOR_COUNT (DMA_CITER_ELINKNO_CITER_MASK >> DMA_CITER_ELINKNO_CITER_SHIFT)

enum _MCULCD_transfer_state
{
    kFLEXIO_MCULCD_StateIdle,           /*!< No transfer in progress. */
    kFLEXIO_MCULCD_StateReadArray,      /*!< Reading array in progress. */
    kFLEXIO_MCULCD_StateWriteArray,     /*!< Writing array in progress. */
    kFLEXIO_MCULCD_StateWriteSameValue, /*!< Writing the same value in progress.
                                           */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief EDMA callback function for FLEXIO MCULCD TX.
 *
 * For details, see @ref edma_callback.
 */
static void FLEXIO_MCULCD_TxEDMACallback(edma_handle_t *edmaHandle, void *param, bool transferDone, uint32_t tcds);

/*!
 * @brief EDMA callback function for FLEXIO MCULCD RX.
 *
 * For details, see @ref edma_callback.
 */
static void FLEXIO_MCULCD_RxEDMACallback(edma_handle_t *edmaHandle, void *param, bool transferDone, uint32_t tcds);

/*!
 * @brief Set EDMA config for FLEXIO MCULCD transfer.
 *
 * @param base pointer to FLEXIO_MCULCD_Type structure.
 * @param handle pointer to flexio_mculcd_edma_handle_t structure to store the
 * transfer state.
 */
static void FLEXIO_MCULCD_EDMAConfig(FLEXIO_MCULCD_Type *base, flexio_mculcd_edma_handle_t *handle);

/*!
 * @brief Convert the FlexIO shifter number to eDMA modulo.
 *
 * @param shifterNum The FlexIO shifter number.
 * @param modulo The modulo number.
 * @retval Get the modulo successfully.
 * @retval Could not get the modulo for the shifter number.
 */
static bool FLEXIO_MCULCD_GetEDMAModulo(uint8_t shifterNum, edma_modulo_t *modulo);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
* Code
******************************************************************************/

static void FLEXIO_MCULCD_TxEDMACallback(edma_handle_t *edmaHandle, void *param, bool transferDone, uint32_t tcds)
{
    tcds = tcds;
    flexio_mculcd_edma_handle_t *flexioLcdMcuHandle = (flexio_mculcd_edma_handle_t *)param;
    FLEXIO_MCULCD_Type *flexioLcdMcuBase = flexioLcdMcuHandle->base;

    if (transferDone)
    {
        if (flexioLcdMcuHandle->remainingCount >= flexioLcdMcuHandle->minorLoopBytes)
        {
            FLEXIO_MCULCD_EDMAConfig(flexioLcdMcuBase, flexioLcdMcuHandle);
            EDMA_StartTransfer(flexioLcdMcuHandle->txEdmaHandle);
        }
        else
        {
            FLEXIO_MCULCD_EnableTxDMA(flexioLcdMcuBase, false);

            /* Now the data are in shifter, wait for the data send out from the shifter. */
            FLEXIO_MCULCD_WaitTransmitComplete();

            /* Disable the TX shifter and the timer. */
            FLEXIO_MCULCD_ClearMultiBeatsWriteConfig(flexioLcdMcuBase);

            /* Send the remaining data. */
            if (flexioLcdMcuHandle->remainingCount)
            {
                if (kFLEXIO_MCULCD_StateWriteSameValue == flexioLcdMcuHandle->state)
                {
                    FLEXIO_MCULCD_WriteSameValueBlocking(flexioLcdMcuBase, flexioLcdMcuHandle->dataAddrOrSameValue,
                                                         flexioLcdMcuHandle->remainingCount);
                }
                else
                {
                    FLEXIO_MCULCD_WriteDataArrayBlocking(flexioLcdMcuBase,
                                                         (void *)flexioLcdMcuHandle->dataAddrOrSameValue,
                                                         flexioLcdMcuHandle->remainingCount);
                }
            }

            /* De-assert nCS. */
            FLEXIO_MCULCD_StopTransfer(flexioLcdMcuBase);

            /* Change the state. */
            flexioLcdMcuHandle->state = kFLEXIO_MCULCD_StateIdle;
            flexioLcdMcuHandle->dataCount = 0;
            flexioLcdMcuHandle->remainingCount = 0;

            /* Callback to inform upper layer. */
            if (flexioLcdMcuHandle->completionCallback)
            {
                flexioLcdMcuHandle->completionCallback(flexioLcdMcuBase, flexioLcdMcuHandle, kStatus_FLEXIO_MCULCD_Idle,
                                                       flexioLcdMcuHandle->userData);
            }
        }
    }
}

static void FLEXIO_MCULCD_RxEDMACallback(edma_handle_t *edmaHandle, void *param, bool transferDone, uint32_t tcds)
{
    tcds = tcds;
    uint32_t i;
    uint32_t rxBufAddr;
    flexio_mculcd_edma_handle_t *flexioLcdMcuHandle = (flexio_mculcd_edma_handle_t *)param;
    FLEXIO_MCULCD_Type *flexioLcdMcuBase = flexioLcdMcuHandle->base;
    FLEXIO_Type *flexioBase = flexioLcdMcuBase->flexioBase;

    if (transferDone)
    {
        if (flexioLcdMcuHandle->remainingCount >= (2 * flexioLcdMcuHandle->minorLoopBytes))
        {
            FLEXIO_MCULCD_EDMAConfig(flexioLcdMcuBase, flexioLcdMcuHandle);
            EDMA_StartTransfer(flexioLcdMcuHandle->rxEdmaHandle);
        }
        else
        {
            FLEXIO_MCULCD_EnableRxDMA(flexioLcdMcuBase, false);

            /* Wait the data saved to the shifter buffer. */
            while (!((1U << flexioLcdMcuBase->rxShifterEndIndex) & FLEXIO_GetShifterStatusFlags(flexioBase)))
            {
            }

            /* Disable the RX shifter and the timer. */
            FLEXIO_MCULCD_ClearMultiBeatsReadConfig(flexioLcdMcuBase);

            rxBufAddr = FLEXIO_MCULCD_GetRxDataRegisterAddress(flexioLcdMcuBase);

/* Read out the data. */
#if (defined(__CORTEX_M) && (__CORTEX_M == 0))
            /* Cortex M0 and M0+ only support aligned access. */
            for (i = 0; i < flexioLcdMcuHandle->rxShifterNum * 4; i++)
            {
                ((uint8_t *)(flexioLcdMcuHandle->dataAddrOrSameValue))[i] = ((volatile uint8_t *)rxBufAddr)[i];
            }
#else
            for (i = 0; i < flexioLcdMcuHandle->rxShifterNum; i++)
            {
                ((uint32_t *)(flexioLcdMcuHandle->dataAddrOrSameValue))[i] = ((volatile uint32_t *)rxBufAddr)[i];
            }
#endif
            flexioLcdMcuHandle->remainingCount -= flexioLcdMcuHandle->minorLoopBytes;

            if (flexioLcdMcuHandle->remainingCount)
            {
                FLEXIO_MCULCD_ReadDataArrayBlocking(flexioLcdMcuBase, (void *)(flexioLcdMcuHandle->dataAddrOrSameValue +
                                                                               flexioLcdMcuHandle->minorLoopBytes),
                                                    flexioLcdMcuHandle->remainingCount);
            }

            /* De-assert nCS. */
            FLEXIO_MCULCD_StopTransfer(flexioLcdMcuBase);

            /* Change the state. */
            flexioLcdMcuHandle->state = kFLEXIO_MCULCD_StateIdle;
            flexioLcdMcuHandle->dataCount = 0;
            flexioLcdMcuHandle->remainingCount = 0;

            /* Callback to inform upper layer. */
            if (flexioLcdMcuHandle->completionCallback)
            {
                flexioLcdMcuHandle->completionCallback(flexioLcdMcuBase, flexioLcdMcuHandle, kStatus_FLEXIO_MCULCD_Idle,
                                                       flexioLcdMcuHandle->userData);
            }
        }
    }
}

static void FLEXIO_MCULCD_EDMAConfig(FLEXIO_MCULCD_Type *base, flexio_mculcd_edma_handle_t *handle)
{
    edma_transfer_config_t xferConfig;
    edma_transfer_size_t transferSize;
    int16_t offset;
    uint32_t majorLoopCounts;
    uint32_t transferCount;

#if (8 == FLEXIO_MCULCD_DATA_BUS_WIDTH)
    transferSize = kEDMA_TransferSize1Bytes;
    offset = 1;
#else
    transferSize = kEDMA_TransferSize2Bytes;
    offset = 2;
#endif

    majorLoopCounts = handle->remainingCount / handle->minorLoopBytes;

    /* For reading, the last minor loop data is not tranfered by DMA. */
    if (kFLEXIO_MCULCD_StateReadArray == handle->state)
    {
        majorLoopCounts--;
    }

    if (majorLoopCounts > EDMA_MAX_MAJOR_COUNT)
    {
        majorLoopCounts = EDMA_MAX_MAJOR_COUNT;
    }

    transferCount = majorLoopCounts * handle->minorLoopBytes;

    if (kFLEXIO_MCULCD_StateReadArray == handle->state)
    {
        xferConfig.srcAddr = FLEXIO_MCULCD_GetRxDataRegisterAddress(base);
        xferConfig.destAddr = handle->dataAddrOrSameValue;
        xferConfig.srcTransferSize = kEDMA_TransferSize4Bytes;
        xferConfig.destTransferSize = transferSize;
        xferConfig.srcOffset = 4;
        xferConfig.destOffset = offset;
        xferConfig.minorLoopBytes = handle->minorLoopBytes;
        xferConfig.majorLoopCounts = majorLoopCounts;
        handle->remainingCount -= transferCount;
        handle->dataAddrOrSameValue += transferCount;
        EDMA_SubmitTransfer(handle->rxEdmaHandle, &xferConfig);
        EDMA_SetModulo(handle->rxEdmaHandle->base, handle->rxEdmaHandle->channel, handle->rxEdmaModulo,
                       kEDMA_ModuloDisable);
    }
    else
    {
        if (kFLEXIO_MCULCD_StateWriteArray == handle->state)
        {
            xferConfig.srcAddr = handle->dataAddrOrSameValue;
            xferConfig.srcOffset = offset;
            handle->dataAddrOrSameValue += transferCount;
        }
        else
        {
            xferConfig.srcAddr = (uint32_t)(&(handle->dataAddrOrSameValue));
            xferConfig.srcOffset = 0;
        }
        xferConfig.destAddr = FLEXIO_MCULCD_GetTxDataRegisterAddress(base);
        xferConfig.srcTransferSize = transferSize;
        xferConfig.destTransferSize = kEDMA_TransferSize4Bytes;
        xferConfig.destOffset = 4;
        xferConfig.minorLoopBytes = handle->minorLoopBytes;
        xferConfig.majorLoopCounts = majorLoopCounts;
        handle->remainingCount -= transferCount;
        EDMA_SubmitTransfer(handle->txEdmaHandle, &xferConfig);
        EDMA_SetModulo(handle->txEdmaHandle->base, handle->txEdmaHandle->channel, kEDMA_ModuloDisable,
                       handle->txEdmaModulo);
    }
}

static bool FLEXIO_MCULCD_GetEDMAModulo(uint8_t shifterNum, edma_modulo_t *modulo)
{
    bool ret = true;

    switch (shifterNum)
    {
        case 1U:
            *modulo = kEDMA_Modulo4bytes;
            break;
        case 2U:
            *modulo = kEDMA_Modulo8bytes;
            break;
        case 4U:
            *modulo = kEDMA_Modulo16bytes;
            break;
        case 8U:
            *modulo = kEDMA_Modulo32bytes;
            break;
        default:
            ret = false;
            break;
    }

    return ret;
}

status_t FLEXIO_MCULCD_TransferCreateHandleEDMA(FLEXIO_MCULCD_Type *base,
                                                flexio_mculcd_edma_handle_t *handle,
                                                flexio_mculcd_edma_transfer_callback_t callback,
                                                void *userData,
                                                edma_handle_t *txEdmaHandle,
                                                edma_handle_t *rxEdmaHandle)
{
    assert(handle);

    /* Zero the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Initialize the state. */
    handle->state = kFLEXIO_MCULCD_StateIdle;

    /* Register callback and userData. */
    handle->completionCallback = callback;
    handle->userData = userData;

    handle->base = base;
    handle->txShifterNum = base->txShifterEndIndex - base->txShifterStartIndex + 1U;
    handle->rxShifterNum = base->rxShifterEndIndex - base->rxShifterStartIndex + 1U;

    if (rxEdmaHandle)
    {
        if (!FLEXIO_MCULCD_GetEDMAModulo(handle->rxShifterNum, &handle->rxEdmaModulo))
        {
            return kStatus_InvalidArgument;
        }

        handle->rxEdmaHandle = rxEdmaHandle;
        EDMA_SetCallback(rxEdmaHandle, FLEXIO_MCULCD_RxEDMACallback, handle);
    }

    if (txEdmaHandle)
    {
        if (!FLEXIO_MCULCD_GetEDMAModulo(handle->txShifterNum, &handle->txEdmaModulo))
        {
            return kStatus_InvalidArgument;
        }

        handle->txEdmaHandle = txEdmaHandle;
        EDMA_SetCallback(txEdmaHandle, FLEXIO_MCULCD_TxEDMACallback, handle);
    }

    return kStatus_Success;
}

status_t FLEXIO_MCULCD_TransferEDMA(FLEXIO_MCULCD_Type *base,
                                    flexio_mculcd_edma_handle_t *handle,
                                    flexio_mculcd_transfer_t *xfer)
{
    assert(handle);
    assert(xfer);

    /*
     * The data transfer mechanism:
     *
     * Read:
     * Assume the data length is Lr = (n1 * minorLoopBytes + n2), where
     * n2 < minorLoopBytes.
     * If (n1 <= 1), then all data are sent using blocking method.
     * If (n1 > 1), then the beginning ((n1-1) * minorLoopBytes) are read
     * using DMA, the left (minorLoopBytes + n2) are read using blocking method.
     *
     * Write:
     * Assume the data length is Lw = (n1 * minorLoopBytes + n2), where
     * n2 < minorLoopBytes.
     * If (n1 = 0), then all data are sent using blocking method.
     * If (n1 >= 1), then the beginning (n1 * minorLoopBytes) are sent
     * using DMA, the left n2 are sent using blocking method.
     */

    /* Check if the device is busy. */
    if (kFLEXIO_MCULCD_StateIdle != handle->state)
    {
        return kStatus_FLEXIO_MCULCD_Busy;
    }

    /* Set the state in handle. */
    if (kFLEXIO_MCULCD_ReadArray == xfer->mode)
    {
        handle->state = kFLEXIO_MCULCD_StateReadArray;
        handle->minorLoopBytes = handle->rxShifterNum * 4U;
    }
    else
    {
        handle->minorLoopBytes = handle->txShifterNum * 4U;

        if (kFLEXIO_MCULCD_WriteArray == xfer->mode)
        {
            handle->state = kFLEXIO_MCULCD_StateWriteArray;
        }
        else
        {
            handle->state = kFLEXIO_MCULCD_StateWriteSameValue;
        }
    }

    /*
     * For TX, if data is less than one minor loop, then use polling method.
     * For RX, if data is less than two minor loop, then use polling method.
     */
    if ((xfer->dataSize < handle->minorLoopBytes) ||
        ((kFLEXIO_MCULCD_ReadArray == xfer->mode) && (xfer->dataSize < 2 * (handle->minorLoopBytes))))
    {
        FLEXIO_MCULCD_TransferBlocking(base, xfer);

        handle->state = kFLEXIO_MCULCD_StateIdle;

        /* Callback to inform upper layer. */
        if (handle->completionCallback)
        {
            handle->completionCallback(base, handle, kStatus_FLEXIO_MCULCD_Idle, handle->userData);
        }
    }
    else
    {
        handle->dataCount = xfer->dataSize;
        handle->remainingCount = xfer->dataSize;
        handle->dataAddrOrSameValue = xfer->dataAddrOrSameValue;

        /* Setup DMA to transfer data.  */
        /* Assert the nCS. */
        FLEXIO_MCULCD_StartTransfer(base);
        /* Send the command. */
        FLEXIO_MCULCD_WriteCommandBlocking(base, xfer->command);

        /* Setup the DMA configuration. */
        FLEXIO_MCULCD_EDMAConfig(base, handle);

        /* Start the tranfer. */
        if (kFLEXIO_MCULCD_ReadArray == xfer->mode)
        {
            /* For 6800, assert the RDWR pin. */
            if (kFLEXIO_MCULCD_6800 == base->busType)
            {
                base->setRDWRPin(true);
            }
            FLEXIO_MCULCD_SetMultiBeatsReadConfig(base);
            FLEXIO_MCULCD_EnableRxDMA(base, true);
            EDMA_StartTransfer(handle->rxEdmaHandle);
        }
        else
        {
            /* For 6800, de-assert the RDWR pin. */
            if (kFLEXIO_MCULCD_6800 == base->busType)
            {
                base->setRDWRPin(false);
            }
            FLEXIO_MCULCD_SetMultiBeatsWriteConfig(base);
            FLEXIO_MCULCD_EnableTxDMA(base, true);
            EDMA_StartTransfer(handle->txEdmaHandle);
        }
    }

    return kStatus_Success;
}

void FLEXIO_MCULCD_TransferAbortEDMA(FLEXIO_MCULCD_Type *base, flexio_mculcd_edma_handle_t *handle)
{
    assert(handle);

    /* Disable dma. */
    if (handle->txEdmaHandle)
    {
        EDMA_AbortTransfer(handle->txEdmaHandle);
    }
    if (handle->rxEdmaHandle)
    {
        EDMA_AbortTransfer(handle->rxEdmaHandle);
    }

    /* Disable DMA enable bit. */
    FLEXIO_MCULCD_EnableTxDMA(handle->base, false);
    FLEXIO_MCULCD_EnableRxDMA(handle->base, false);

    /* Set the handle state. */
    handle->state = kFLEXIO_MCULCD_StateIdle;
    handle->dataCount = 0;
}

status_t FLEXIO_MCULCD_TransferGetCountEDMA(FLEXIO_MCULCD_Type *base,
                                            flexio_mculcd_edma_handle_t *handle,
                                            size_t *count)
{
    assert(handle);
    assert(count);
    uint32_t state = handle->state;

    if (kFLEXIO_MCULCD_StateIdle == state)
    {
        return kStatus_NoTransferInProgress;
    }
    else
    {
        *count = handle->dataCount - handle->remainingCount;

        if (kFLEXIO_MCULCD_StateReadArray == state)
        {
            *count -= handle->minorLoopBytes *
                      EDMA_GetRemainingMajorLoopCount(handle->rxEdmaHandle->base, handle->rxEdmaHandle->channel);
        }
        else
        {
            *count -= handle->minorLoopBytes *
                      EDMA_GetRemainingMajorLoopCount(handle->txEdmaHandle->base, handle->txEdmaHandle->channel);
        }
    }

    return kStatus_Success;
}
