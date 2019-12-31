/*
 * Copyright (c) 2016-2018, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_lin.h"
#if (FSL_FEATURE_SOC_LPUART_COUNT > 0U)
    #include "fsl_lin_lpuart.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Table to save LIN user config buadrate values */
lin_baudrate_values_t g_linConfigBaudrates[LIN_NUM_OF_SUPP_BAUDRATES];
/*! @brief Pointers to LPUART bases for each instance. */

/*! @brief Table to save LPUART state structure pointers */
lin_state_t * g_linStatePtr[FSL_FEATURE_SOC_LPUART_COUNT] = {NULL};

/*! @brief Table to save LIN user config structure pointers */
lin_user_config_t * g_linUserconfigPtr[FSL_FEATURE_SOC_LPUART_COUNT] = {NULL};

/*******************************************************************************
 * Code
 ******************************************************************************/

static inline uint8_t BIT(uint8_t A, uint8_t B)
{
    return (uint8_t)(((uint8_t)((uint8_t)(A) >> (uint8_t)(B))) & 0x01U);
}

uint32_t LIN_CalcMaxHeaderTimeoutCnt(uint32_t baudRate)
{
    return (uint32_t) ((14U*2U*1000000U/(LIN_TIME_OUT_UNIT_US*baudRate)) + 1U);
}


uint32_t LIN_CalcMaxResTimeoutCnt(uint32_t baudRate, uint8_t size)
{
    return (uint32_t) ((14U*(1U+size)*1000000U/(LIN_TIME_OUT_UNIT_US*baudRate)) + 1U);
}


void LIN_GetSlaveDefaultConfig(lin_user_config_t * linUserConfig)
{
    linUserConfig->autobaudEnable = false;
    linUserConfig->baudRate = 19200U;
    linUserConfig->nodeFunction = LIN_SLAVE;
    linUserConfig->timerGetTimeIntervalCallback = NULL;
}


void LIN_GetMasterDefaultConfig(lin_user_config_t * linUserConfig)
{
    linUserConfig->autobaudEnable = false;
    linUserConfig->baudRate = 19200U;
    linUserConfig->nodeFunction = LIN_MASTER;
    linUserConfig->timerGetTimeIntervalCallback = NULL;
}


void LIN_CalculateBaudrate(uint32_t instance, uint32_t baudRate_Bps, uint32_t srcClock_Hz, uint32_t *osr, uint16_t *sbr)
{
    /* calculates baudrate registers values for required baudrate */
    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        LIN_LPUART_CalculateBaudRate(base, baudRate_Bps, srcClock_Hz, osr, sbr);
    }

}


void LIN_SetBaudrate(uint32_t instance, uint32_t osr, uint16_t sbr)
{
    /* sets baudrate registers values for LPUART instance */
    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        LIN_LPUART_SetBaudRate(base, &osr, &sbr);
    }

}


lin_status_t LIN_Init(uint32_t instance,
                          lin_user_config_t *linUserConfig, lin_state_t *linCurrentState, uint32_t clockSource)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_Init(base, linUserConfig, linCurrentState, clockSource);

        /* Save runtime structure pointer. */
        g_linStatePtr[instance] = linCurrentState;
        /* Save LIN user config structure pointer. */
        g_linUserconfigPtr[instance] = linUserConfig;

    }

    return retVal;
}


lin_status_t LIN_Deinit(uint32_t instance)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];

        /* Call lower level function */
        retVal = LIN_LPUART_Deinit(base);

        /* Clear runtime structure pointer. */
        g_linStatePtr[instance] = NULL;
        /* Clear LIN user config structure pointer. */
        g_linUserconfigPtr[instance] = NULL;
    }

    return retVal;
}


lin_callback_t LIN_InstallCallback(uint32_t instance,
                                       lin_callback_t function)
{
    lin_callback_t currentCallback = NULL;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        lin_state_t *linCurrentState = g_linStatePtr[instance];

        /* Get the current callback function. */
        currentCallback = linCurrentState->Callback;

        /* Install new callback function. */
        linCurrentState->Callback = function;
    }

    return currentCallback;
}


lin_status_t LIN_SendFrameDataBlocking(uint32_t instance,
                                           const uint8_t * txBuff,
                                           uint8_t txSize, uint32_t timeoutMSec)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_SendFrameDataBlocking(base, txBuff, txSize, timeoutMSec);
    }

    return retVal;
}


lin_status_t LIN_SendFrameData(uint32_t instance, const uint8_t * txBuff, uint8_t txSize)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_SendFrameData(base, txBuff, txSize);
    }

    return retVal;
}


lin_status_t LIN_GetTransmitStatus(uint32_t instance, uint8_t * bytesRemaining)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_GetTransmitStatus(base, bytesRemaining);
    }

    return retVal;
}


lin_status_t LIN_ReceiveFrameDataBlocking(uint32_t instance, uint8_t * rxBuff, uint8_t rxSize, uint32_t timeoutMSec)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_RecvFrmDataBlocking(base, rxBuff, rxSize, timeoutMSec);
    }

    return retVal;
}


lin_status_t LIN_ReceiveFrameData(uint32_t instance,
                                              uint8_t * rxBuff,
                                              uint8_t rxSize)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_RecvFrmData(base, rxBuff, rxSize);
    }

    return retVal;
}


lin_status_t LIN_AbortTransferData(uint32_t instance)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_AbortTransferData(base);
    }


    return retVal;
}


lin_status_t LIN_GetReceiveStatus(uint32_t instance, uint8_t * bytesRemaining)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_GetReceiveStatus(base, bytesRemaining);
    }

    return retVal;
}


lin_status_t LIN_GoToSleepMode(uint32_t instance)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_GoToSleepMode(base);
    }

    return retVal;
}


lin_status_t LIN_GotoIdleState(uint32_t instance)
{

    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_GotoIdleState(base);
    }

    return retVal;
}


lin_status_t LIN_SendWakeupSignal(uint32_t instance)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_SendWakeupSignal(base);
    }

    return retVal;
}


lin_node_state_t LIN_GetCurrentNodeState(uint32_t instance)
{
    lin_node_state_t retVal = LIN_NODE_STATE_UNINIT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        const lin_state_t *linCurrentState = g_linStatePtr[instance];

        if (linCurrentState != NULL)
        {
            /* return current state of node */
            retVal = linCurrentState->currentNodeState;
        }
    }

    return retVal;
}


void LIN_TimeoutService(uint32_t instance)
{

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        /* Get the current LIN state of this LPUART instance. */
        lin_state_t *linCurrentState = g_linStatePtr[instance];

        /* Get LIN node's current state */
        lin_node_state_t state = linCurrentState->currentNodeState;

        switch (state)
        {
            /* If the node is SENDING DATA */
            case LIN_NODE_STATE_SEND_DATA:
                /* Check if timeout Counter is 0 */
                if (linCurrentState->timeoutCounter == 0U)
                {
                    /* Set timeout Counter flag */
                    linCurrentState->timeoutCounterFlag = true;
                    /* Callback to handle timeout Counter flag */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }

                    /* Check if the transmission is non-blocking */
                    if (linCurrentState->isTxBlocking == false)
                    {
                        /* Clear Bus busy flag */
                        linCurrentState->isBusBusy = false;
                        linCurrentState->isTxBusy = false;
                        linCurrentState->txCompleted = true;
                    }
                    /* Change the node's current state to IDLE */
                    (void)LIN_GotoIdleState(instance);

                }
                /* If timeout Counter is not 0, then decrease timeout Counter by one */
                else
                {
                    linCurrentState->timeoutCounter--;
                }
                break;
            /* If the node is RECEIVING DATA */
            case LIN_NODE_STATE_RECV_DATA:

                /* Check if timeout Counter is 0 */
                if (linCurrentState->timeoutCounter == 0U)
                {
                    /* Set timeout Counter flag */
                    linCurrentState->timeoutCounterFlag = true;

                    /* Callback to handle timeout Counter flag */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }

                    /* Check if the reception is non-blocking */
                    if (linCurrentState->isRxBlocking == false)
                    {
                        /* Clear Bus busy flag */
                        linCurrentState->isBusBusy = false;
                        linCurrentState->isRxBusy = false;
                        linCurrentState->rxCompleted = true;
                    }
                    /* Change the node's current state to IDLE */
                    (void)LIN_GotoIdleState(instance);

                }
                /* If timeout Counter is not 0, then decrease timeout Counter by one */
                else
                {
                    linCurrentState->timeoutCounter--;
                }
                break;
            default:
                /* The node state is not SENDING nor RECEIVING data */
                break;
        }
    }

}

void LIN_SetTimeoutCounter(uint32_t instance, uint32_t timeoutValue)
{

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        /* Get the current LIN state of this LPUART instance. */
        lin_state_t *linCurrentState = g_linStatePtr[instance];

        /* Clear Timeout Counter Flag */
        linCurrentState->timeoutCounterFlag = false;

        /* Set new value for Timeout Counter */
        linCurrentState->timeoutCounter = timeoutValue;
    }
}


lin_status_t LIN_MasterSendHeader(uint32_t instance, uint8_t id)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_MasterSendHeader(base, id);
    }

    return retVal;
}

lin_status_t LIN_EnableIRQ(uint32_t instance)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        retVal = LIN_LPUART_EnableIRQ(base);
    }

    return retVal;
}

void LIN_IRQHandler(uint8_t instance)
{
    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        LPUART_Type *base = g_linLpuartBase[instance];
        /* Call lower level function */
        LIN_LPUART_IRQHandler(base);
    }

}


lin_status_t LIN_AutoBaudCapture(uint32_t instance)
{
    lin_status_t retVal = LIN_IFC_NOT_SUPPORT;

    if(instance < FSL_FEATURE_SOC_LPUART_COUNT)
    {
        retVal = LIN_LPUART_AutoBaudCapture(instance);
    }

    return retVal;
}

uint8_t LIN_MakeChecksumByte(const uint8_t *buffer,
                                 uint8_t sizeBuffer,
                                 uint8_t PID)
{
    uint8_t length;
    uint16_t checksum;

    assert(PID);
    /* For PID is 0x3C or 0x3D apply classic checksum, and for other PID: Enhanced checksum */
    if ((0x3CU != PID) && (0x3DU != PID))
    {
        /* For PID other than 0x3c and 0x7D: Add PID in checksum calculation */
        checksum = PID;
    }
    else
    {
        /* For 0x3C and 0x3D: Do not add PID in checksum calculation */
        checksum = 0U;
    }

    for (length = sizeBuffer; 0U < length; length--)
    {
        checksum += *(buffer);
        buffer++;
        /* Deal with the carry */
        if (checksum > 0xFFU)
        {
            checksum -= 0xFFU;
        }
    }

    /* Return reversed checksum */
    return (~(uint8_t)checksum);
}

uint8_t LIN_ProcessParity(uint8_t PID, uint8_t typeAction)
{
    uint8_t parity;
    uint8_t retVal;

    parity = ( (uint8_t)((uint8_t)((uint8_t)((uint8_t)(BIT(PID, 0U) ^ BIT(PID, 1U)) ^ BIT(PID, 2U)) ^ BIT(PID, 4U)) << 6U) |
             (uint8_t)(((uint8_t)(~(uint8_t)((uint8_t)((uint8_t)(BIT(PID, 1U) ^ BIT(PID, 3U)) ^ BIT(PID, 4U)) ^ BIT(PID, 5U)))) << 7U));

    /* Check if action is checking parity bits */
    if (LIN_CHECK_PARITY == typeAction)
    {
        /* If parity bits are incorrect */
        if ((PID & 0xC0U) != parity)
        {
            /* Return 0xFF if parity bits are incorrect */
            retVal = 0xFFU;
        }
        /* If parity bits are correct */
        else
        {
            /* Return ID if parity bits are correct */
            retVal = (uint8_t)(PID & 0x3FU);
        }
    }
    /* If action is making parity bits */
    else
    {
        /* Return PID in case of making parity bits */
        retVal = (uint8_t)(PID|parity);
    }

    return retVal;
}


lin_status_t LIN_SetResponse(uint8_t instance, uint8_t* response_buff, uint8_t response_length, uint8_t max_frame_res_timeout)
{
    lin_status_t retVal = LIN_ERROR;
    /* Set correct timeout value */
    LIN_SetTimeoutCounter(instance, max_frame_res_timeout);
    /* Start sending data from the buffer */
    retVal = LIN_SendFrameData(instance, response_buff, response_length);

    return retVal;
}

lin_status_t LIN_RxResponse(uint8_t instance, uint8_t* response_buff, uint8_t response_length, uint8_t max_frame_res_timeout)
{
    lin_status_t retVal = LIN_ERROR;
    /* Set correct timeout value */
    LIN_SetTimeoutCounter(instance, max_frame_res_timeout);
    /* Start receiving frame data into input buffer */
    retVal = LIN_ReceiveFrameData(instance, response_buff, response_length);


    return retVal;
}

lin_status_t LIN_IgnoreResponse(uint8_t instance)
{
    lin_status_t retVal = LIN_ERROR;

    /* Abort frame data transferring */
    retVal = LIN_GotoIdleState(instance);

    return retVal;
}

