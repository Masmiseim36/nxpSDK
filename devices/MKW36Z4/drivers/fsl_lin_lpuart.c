/*
 * Copyright (c) 2016-2018, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_lin_lpuart.h"
#include "fsl_lin.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Array of LPUART clock name. */
static const clock_ip_name_t s_lpuartClock[] = LPUART_CLOCKS;

#if defined(LPUART_PERIPH_CLOCKS)
/* Array of LPUART functional clock name. */
static const clock_ip_name_t s_lpuartPeriphClocks[] = LPUART_PERIPH_CLOCKS;
#endif
#endif

LPUART_Type *const g_linLpuartBase[FSL_FEATURE_SOC_LPUART_COUNT] = LPUART_BASE_PTRS;

const IRQn_Type g_linLpuartRxTxIrqId[FSL_FEATURE_SOC_LPUART_COUNT] = LPUART_RX_TX_IRQS;

#ifdef LPUART_ERR_IRQS
const IRQn_Type g_linLpuartErrIrqId[FSL_FEATURE_SOC_LPUART_COUNT] = LPUART_ERR_IRQS;
#endif

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static uint32_t s_previousTwoBitTimeLength[FSL_FEATURE_SOC_LPUART_COUNT] = {0U};
static uint8_t s_wakeupSignal[FSL_FEATURE_SOC_LPUART_COUNT] = {0U};
static uint8_t s_countMeasure[FSL_FEATURE_SOC_LPUART_COUNT] = {0U};
static uint32_t s_timeMeasure[FSL_FEATURE_SOC_LPUART_COUNT] = {0U};

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/

static void LIN_LPUART_EnableInterrupts(LPUART_Type *base, uint32_t mask);
static void LIN_LPUART_DisableInterrupts(LPUART_Type *base, uint32_t mask);
static uint32_t LIN_LPUART_GetInstance(LPUART_Type *base);
static status_t LIN_LPUART_ClearStatusFlags(LPUART_Type *base, uint32_t mask);
static uint32_t LIN_LPUART_GetStatusFlags(LPUART_Type *base);
static void LIN_LPUART_ProcessBreakDetect(uint32_t instance);
static void LIN_LPUART_CheckWakeupSignal(uint32_t instance);
static void LIN_LPUART_AutobaudTimerValEval(uint32_t instance, uint32_t twoBitTimeLength);
static void LIN_LPUART_EvalTwoBitTimeLength(uint32_t instance, uint32_t twoBitTimeLength);
static void LIN_LPUART_ProcessFrame(uint32_t instance, uint8_t tmpByte);
static void LIN_LPUART_ProcessFrameHeader(uint32_t instance, uint8_t tmpByte);
static void LIN_LPUART_ProcessReceiveFrameData(uint32_t instance, uint8_t tmpByte);
static void LIN_LPUART_ProcessSendFrameData(uint32_t instance, uint8_t tmpByte);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void LIN_LPUART_EnableInterrupts(LPUART_Type *base, uint32_t mask)
{
    base->BAUD |= ((mask << 8) & (LPUART_BAUD_LBKDIE_MASK | LPUART_BAUD_RXEDGIE_MASK));
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    base->FIFO = (base->FIFO & ~(LPUART_FIFO_TXOF_MASK | LPUART_FIFO_RXUF_MASK)) |
                 ((mask << 8) & (LPUART_FIFO_TXOFE_MASK | LPUART_FIFO_RXUFE_MASK));
#endif
    mask &= 0xFFFFFF00U;
    base->CTRL |= mask;
}

static void LIN_LPUART_DisableInterrupts(LPUART_Type *base, uint32_t mask)
{
    base->BAUD &= ~((mask << 8) & (LPUART_BAUD_LBKDIE_MASK | LPUART_BAUD_RXEDGIE_MASK));
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    base->FIFO = (base->FIFO & ~(LPUART_FIFO_TXOF_MASK | LPUART_FIFO_RXUF_MASK)) &
                 ~((mask << 8) & (LPUART_FIFO_TXOFE_MASK | LPUART_FIFO_RXUFE_MASK));
#endif
    mask &= 0xFFFFFF00U;
    base->CTRL &= ~mask;
}

status_t LIN_LPUART_CalculateBaudRate(
    LPUART_Type *base, uint32_t baudRate_Bps, uint32_t srcClock_Hz, uint32_t *osr, uint16_t *sbr)
{
    assert(baudRate_Bps);
    status_t status = kStatus_Success;

    uint16_t sbrTemp;
    uint32_t osrTemp, tempDiff, calculatedBaud, baudDiff;

    /* This LPUART instantiation uses a slightly different baud rate calculation
     * The idea is to use the best OSR (over-sampling rate) possible
     * Note, OSR is typically hard-set to 16 in other LPUART instantiations
     * loop to find the best OSR value possible, one that generates minimum baudDiff
     * iterate through the rest of the supported values of OSR */

    baudDiff = baudRate_Bps;
    for (osrTemp = 4; osrTemp <= 32; osrTemp++)
    {
        /* calculate the temporary sbr value   */
        sbrTemp = (srcClock_Hz / (baudRate_Bps * osrTemp));
        /*set sbrTemp to 1 if the sourceClockInHz can not satisfy the desired baud rate*/
        if (sbrTemp == 0)
        {
            sbrTemp = 1;
        }
        /* Calculate the baud rate based on the temporary OSR and SBR values */
        calculatedBaud = (srcClock_Hz / (osrTemp * sbrTemp));

        tempDiff = calculatedBaud - baudRate_Bps;

        /* Select the better value between srb and (sbr + 1) */
        if (tempDiff > (baudRate_Bps - (srcClock_Hz / (osrTemp * (sbrTemp + 1)))))
        {
            tempDiff = baudRate_Bps - (srcClock_Hz / (osrTemp * (sbrTemp + 1)));
            sbrTemp++;
        }

        if (tempDiff <= baudDiff)
        {
            baudDiff = tempDiff;
            *osr = osrTemp; /* update and store the best OSR value calculated */
            *sbr = sbrTemp; /* update store the best SBR value calculated */
        }
    }

    if (baudDiff >= ((baudRate_Bps / 100) * 3))
    {
        status = kStatus_Fail;
    }
    return status;
}

void LIN_LPUART_SetBaudRate(LPUART_Type *base, uint32_t *osr, uint16_t *sbr)
{
    uint32_t temp, oldCtrl;
    /* Store CTRL before disable Tx and Rx */
    oldCtrl = base->CTRL;

    /* Disable LPUART TX RX before setting. */
    base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

    temp = base->BAUD;

    /* Acceptable baud rate, check if OSR is between 4x and 7x oversampling.
     * If so, then "BOTHEDGE" sampling must be turned on */
    if ((*osr > 3) && (*osr < 8))
    {
        temp |= LPUART_BAUD_BOTHEDGE_MASK;
    }

    /* program the osr value (bit value is one less than actual value) */
    temp &= ~LPUART_BAUD_OSR_MASK;
    temp |= LPUART_BAUD_OSR(*osr - 1);

    /* write the sbr value to the BAUD registers */
    temp &= ~LPUART_BAUD_SBR_MASK;
    base->BAUD = temp | LPUART_BAUD_SBR(*sbr);

    /* Restore CTRL. */
    base->CTRL = oldCtrl;
}

static void LIN_LPUART_SetBitCountPerChar(LPUART_Type *base, lin_lpuart_bit_count_per_char_t bitCountPerChar)
{
    if (bitCountPerChar == LPUART_10_BITS_PER_CHAR)
    {
        base->BAUD = (base->BAUD & ~LPUART_BAUD_M10_MASK) | ((uint32_t)1U << LPUART_BAUD_M10_SHIFT);
    }
    else
    {
        /* config 8-bit (M=0) or 9-bits (M=1) */
        base->CTRL = (base->CTRL & ~LPUART_CTRL_M_MASK) | ((uint32_t)bitCountPerChar << LPUART_CTRL_M_SHIFT);
        /* clear M10 to make sure not 10-bit mode */
        base->BAUD &= ~LPUART_BAUD_M10_MASK;
    }
}

static status_t LIN_LPUART_ClearStatusFlags(LPUART_Type *base, uint32_t mask)
{
    uint32_t temp;
    status_t status;
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    temp = (uint32_t)base->FIFO;
    temp &= (uint32_t)(~(LPUART_FIFO_TXOF_MASK | LPUART_FIFO_RXUF_MASK));
    temp |= (mask << 16) & (LPUART_FIFO_TXOF_MASK | LPUART_FIFO_RXUF_MASK);
    base->FIFO = temp;
#endif
    temp = (uint32_t)base->STAT;
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
    temp &= (uint32_t)(~(LPUART_STAT_LBKDIF_MASK));
    temp |= mask & LPUART_STAT_LBKDIF_MASK;
#endif
    temp &= (uint32_t)(~(LPUART_STAT_RXEDGIF_MASK | LPUART_STAT_IDLE_MASK | LPUART_STAT_OR_MASK | LPUART_STAT_NF_MASK |
                         LPUART_STAT_FE_MASK | LPUART_STAT_PF_MASK));
    temp |= mask & (LPUART_STAT_RXEDGIF_MASK | LPUART_STAT_IDLE_MASK | LPUART_STAT_OR_MASK | LPUART_STAT_NF_MASK |
                    LPUART_STAT_FE_MASK | LPUART_STAT_PF_MASK);
#if defined(FSL_FEATURE_LPUART_HAS_ADDRESS_MATCHING) && FSL_FEATURE_LPUART_HAS_ADDRESS_MATCHING
    temp &= (uint32_t)(~(LPUART_STAT_MA2F_MASK | LPUART_STAT_MA1F_MASK));
    temp |= mask & (LPUART_STAT_MA2F_MASK | LPUART_STAT_MA1F_MASK);
#endif
    base->STAT = temp;
    /* If some flags still pending. */
    if (mask & LIN_LPUART_GetStatusFlags(base))
    {
        /* Some flags can only clear or set by the hardware itself, these flags are: kLPUART_TxDataRegEmptyFlag,
        kLPUART_TransmissionCompleteFlag, kLPUART_RxDataRegFullFlag, kLPUART_RxActiveFlag,
        kLPUART_NoiseErrorInRxDataRegFlag, kLPUART_ParityErrorInRxDataRegFlag,
        kLPUART_TxFifoEmptyFlag, kLPUART_RxFifoEmptyFlag. */
        status = kStatus_LPUART_FlagCannotClearManually; /* flags can not clear manually */
    }
    else
    {
        status = kStatus_Success;
    }

    return status;
}

static uint32_t LIN_LPUART_GetStatusFlags(LPUART_Type *base)
{
    uint32_t temp;
    temp = base->STAT;
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    temp |= (base->FIFO &
             (LPUART_FIFO_TXEMPT_MASK | LPUART_FIFO_RXEMPT_MASK | LPUART_FIFO_TXOF_MASK | LPUART_FIFO_RXUF_MASK)) >>
            16;
#endif
    return temp;
}

static uint32_t LIN_LPUART_GetInstance(LPUART_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < FSL_FEATURE_SOC_LPUART_COUNT; instance++)
    {
        if (g_linLpuartBase[instance] == base)
        {
            break;
        }
    }

    assert(instance < FSL_FEATURE_SOC_LPUART_COUNT);

    return instance;
}

static lin_status_t LIN_LPUART_QueueBreakChar(LPUART_Type *base)
{
    /* mask T0-T9 bits from data register */
    volatile uint16_t *dataRegBytes = (volatile uint16_t *)(&(base->DATA));

    dataRegBytes[0] = LPUART_DATA_FRETSC(1U);

    return LIN_SUCCESS;
}

static void LIN_LPUART_ProcessBreakDetect(uint32_t instance)
{
    LPUART_Type *base = g_linLpuartBase[instance];
    /* Get the current LIN user configure structure of this LPUART instance. */
    const lin_user_config_t *linUserConfig = g_linUserconfigPtr[instance];

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Clear LIN Break Detect Interrupt Flag */
    LIN_LPUART_ClearStatusFlags(base, kLPUART_LinBreakFlag);

    /* Check if the current node is in SLEEP MODE */
    if (linCurrentState->currentNodeState == LIN_NODE_STATE_SLEEP_MODE)
    {
        /* Change the node's current state to IDLE */
        (void)LIN_LPUART_GotoIdleState(base);
    }
    else
    {
/* Set Break char detect length as 10 bits minimum */
/* Disable LIN Break Detect Interrupt */
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
        /* Set Break char detect length as 10 bits minimum and interrupt */
        base->STAT &= ~LPUART_STAT_LBKDE_MASK;
        LIN_LPUART_DisableInterrupts(base, kLPUART_LinBreakInterruptEnable);
#endif
        /* Set flag LIN bus busy */
        linCurrentState->isBusBusy = true;

        /* Check if the current node is MASTER */
        if (linUserConfig->nodeFunction == (bool)LIN_MASTER) /* Master */
        {
            if (linCurrentState->currentNodeState == LIN_NODE_STATE_SEND_BREAK_FIELD)
            {
                /* Change the node's current state to SENDING PID */
                linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_PID;

                LIN_LPUART_WriteByte(base, 0x55);
            }
        }
        /* If the current node is SLAVE */
        else
        {
            /* Change the node's current state to RECEIVED BREAK FIELD */
            linCurrentState->currentEventId = LIN_RECV_BREAK_FIELD_OK;

            /* Callback function */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Change the node's current state to RECEIVING SYNC FIELD */
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_SYNC;
        }
    }
}

static void LIN_LPUART_CheckWakeupSignal(uint32_t instance)
{
    uint32_t wakeupSignalLength = 0U;

    LPUART_Type *base = g_linLpuartBase[instance];
    /* Get the current LIN user config structure of this LPUART instance. */
    const lin_user_config_t *linUserConfig = g_linUserconfigPtr[instance];

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* if LPUART_HAL_GetRxDataPolarity is 0: Receive Data is not inverted */
    if (LIN_LPUART_GetRxDataPolarity(base) == false)
    {
        /* Start measure time */
        (void)linUserConfig->timerGetTimeIntervalCallback(&wakeupSignalLength);

        /* Set Receive Data Inverted */
        LIN_LPUART_SetRxDataPolarity(base, true);
    }
    else
    {
        /* Set Receive Data is Not Inverted */
        LIN_LPUART_SetRxDataPolarity(base, false);

        /* Calculate time interval between the falling and rising edge */
        if (linUserConfig->timerGetTimeIntervalCallback == NULL)
        {
            return;
        }
        (void)linUserConfig->timerGetTimeIntervalCallback(&wakeupSignalLength);

        /* If length of the dominant signal is from 150us to 5ms, it is a wakeup signal */
        if ((wakeupSignalLength >= 150000U) && (wakeupSignalLength <= 5000000U))
        {
            linCurrentState->currentEventId = LIN_WAKEUP_SIGNAL;

            /* Callback to handle event: Received a wakeup signal */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Change node's state to IDLE */
            (void)LIN_LPUART_GotoIdleState(base);
        }
    }
}

static void LIN_LPUART_AutobaudTimerValEval(uint32_t instance, uint32_t twoBitTimeLength)
{
    uint32_t MasterBaudrate = 0U;

    /* Get the current LIN user config structure of this LPUART instance. */
    lin_user_config_t *linUserConfig = g_linUserconfigPtr[instance];

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Check whether current node state is receive sync */
    bool checkNodeState = (linCurrentState->currentNodeState == LIN_NODE_STATE_RECV_SYNC);

    /* Evaluate average value against baudrate */
    LIN_LPUART_EvalTwoBitTimeLength(instance, twoBitTimeLength);

    if ((linCurrentState->fallingEdgeInterruptCount > 4U) && checkNodeState)
    {
        if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_19200) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_19200))
        {
            MasterBaudrate = 19200U;
        }
        else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_14400) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_14400))
        {
            MasterBaudrate = 14400U;
        }
        else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_9600) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_9600))
        {
            MasterBaudrate = 9600U;
        }
        else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_4800) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_4800))
        {
            MasterBaudrate = 4800U;
        }
        else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_2400) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_2400))
        {
            MasterBaudrate = 2400U;
        }
        else
        {
            /* Complete if-elseif-else block to avoid violating MISRA 2012 Rule 15.7 */
        }

        /* Check Master Baudrate against node's current baudrate */
        if ((MasterBaudrate != 0U) && (linUserConfig->baudRate != MasterBaudrate))
        {
            linUserConfig->baudRate = MasterBaudrate;

            /* Assign wakeup signal to satisfy LIN Specifications specifies that
             * wakeup signal shall be in range from 250us to 5 ms.
             */
            if (linUserConfig->baudRate > 10000U)
            {
                /* Wakeup signal will be range from 400us to 800us depend on baudrate */
                s_wakeupSignal[instance] = 0x80U;
            }
            else
            {
                /* Wakeup signal will be range from 400us to 4ms depend on baudrate */
                s_wakeupSignal[instance] = 0xF8U;
            }
        }
        linCurrentState->currentEventId = LIN_BAUDRATE_ADJUSTED;
        /* Disable baudrate evaluation process */
        linCurrentState->baudrateEvalEnable = false;
        /* Callback function to handle this event */
        if (linCurrentState->Callback != NULL)
        {
            linCurrentState->Callback(instance, linCurrentState);
        }
        /* Interrupt in rising edge */

        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
        linCurrentState->currentEventId = LIN_RECV_SYNC_OK;
    }
}

static void LIN_LPUART_EvalTwoBitTimeLength(uint32_t instance, uint32_t twoBitTimeLength)
{
    LPUART_Type *base = g_linLpuartBase[instance];
    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    if (linCurrentState->fallingEdgeInterruptCount < 5U)
    {
        if (linCurrentState->fallingEdgeInterruptCount > 0U)
        {
            if ((twoBitTimeLength < TWO_BIT_DURATION_MIN_19200) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_19200) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_14400)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_14400) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_9600)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_9600) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_4800)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_4800) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_2400)) ||
                (twoBitTimeLength > TWO_BIT_DURATION_MAX_2400))
            {
                /* cancel capturing */
                (void)LIN_LPUART_GotoIdleState(base);
                linCurrentState->fallingEdgeInterruptCount = 0U;
            }
            else
            {
                if (linCurrentState->fallingEdgeInterruptCount > 1U)
                {
                    if ((twoBitTimeLength <
                         ((100U - BIT_RATE_TOLERANCE_UNSYNC) * s_previousTwoBitTimeLength[instance] / 100U)) ||
                        (twoBitTimeLength >
                         ((100U + BIT_RATE_TOLERANCE_UNSYNC) * s_previousTwoBitTimeLength[instance] / 100U)))
                    {
                        /* cancel capturing */
                        (void)LIN_LPUART_GotoIdleState(base);
                        linCurrentState->fallingEdgeInterruptCount = 0U;
                    }
                }
            }
            s_previousTwoBitTimeLength[instance] = twoBitTimeLength;
        }
    }
    linCurrentState->fallingEdgeInterruptCount += 1U;
}

static void LIN_LPUART_ProcessFrame(uint32_t instance, uint8_t tmpByte)
{
    /* Get the current LIN state of this LPUART instance. */
    const lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Check node's current state */
    switch (linCurrentState->currentNodeState)
    {
        /* if current state is RECEIVE SYNC FIELD */
        case LIN_NODE_STATE_RECV_SYNC:

        /* if current state is MASTER SENDING PID */
        case LIN_NODE_STATE_SEND_PID:

        /* if current state is RECEIVE PID */
        case LIN_NODE_STATE_RECV_PID:
            LIN_LPUART_ProcessFrameHeader(instance, tmpByte);
            break;
        /* if current state is RECEIVE DATA */
        case LIN_NODE_STATE_RECV_DATA:
            LIN_LPUART_ProcessReceiveFrameData(instance, tmpByte);
            break;
        /* if current state is SENDING DATA */
        case LIN_NODE_STATE_SEND_DATA:
            LIN_LPUART_ProcessSendFrameData(instance, tmpByte);
            break;

        default:
            /* Other node state */
            break;
    }
}

static void LIN_LPUART_ProcessFrameHeader(uint32_t instance, uint8_t tmpByte)
{
    LPUART_Type *base = g_linLpuartBase[instance];
    /* Get the current LIN user config structure of this LPUART instance. */
    const lin_user_config_t *linUserConfig = g_linUserconfigPtr[instance];

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Check node's current state */
    switch (linCurrentState->currentNodeState)
    {
        /* if current state is RECEIVE SYNC FIELD */
        case LIN_NODE_STATE_RECV_SYNC:
            if (tmpByte == 0x55U)
            {
                linCurrentState->currentEventId = LIN_RECV_SYNC_OK;

                /* Change node's current state to RECEIVE PID */
                linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
            }
            else
            {
                linCurrentState->currentEventId = LIN_RECV_SYNC_ERROR;

                /* Callback function to handle event RECEIVED SYNC FIELD ERROR */
                if (linCurrentState->Callback != NULL)
                {
                    linCurrentState->Callback(instance, linCurrentState);
                }

                /* Clear Bus busy flag */
                linCurrentState->isBusBusy = false;

                /* Change node's current state to IDLE */
                (void)LIN_LPUART_GotoIdleState(base);
            }
            break;
        /* if current state is MASTER SENDING PID */
        case LIN_NODE_STATE_SEND_PID:
            /* Check if master node sent SYNC byte correctly before send PID */
            if (tmpByte == 0x55U)
            {
                /* Change node's current state to RECEIVING PID */
                linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
                /* Send the current PID byte */
                LIN_LPUART_WriteByte(base, linCurrentState->currentPid);
            }
            /* In case of errors during header transmission, it is up to the implementer
             * how to handle these errors (stop/continue transmission) and to decide if the
             * corresponding response is valid or not.
             * By default, LIN Driver set isBusBusy to false, and change node's state to IDLE.
             */
            else
            {
                linCurrentState->currentEventId = LIN_RECV_SYNC_ERROR;
                /* Clear Bus busy flag */
                linCurrentState->isBusBusy = false;
                /* Change node's current state to IDLE */
                linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;
                /* Callback function to handle event RECEIVED SYNC FIELD ERROR */
                if (linCurrentState->Callback != NULL)
                {
                    linCurrentState->Callback(instance, linCurrentState);
                }
            }
            break;
        /* if current state is RECEIVE PID */
        case LIN_NODE_STATE_RECV_PID:
            /* if the node is MASTER */
            if (linUserConfig->nodeFunction == (bool)LIN_MASTER)
            {
                /* Check if master node sent PID correctly */
                if (tmpByte == linCurrentState->currentPid)
                {
                    /* Set current event ID to PID correct */
                    linCurrentState->currentEventId = LIN_PID_OK;

                    /* Clear Bus bus flag */
                    linCurrentState->isBusBusy = false;

                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                }
                /* In case of errors during header transmission, it is up to the implementer
                 * how to handle these errors (stop/continue transmission) and to decide if the
                 * corresponding response is valid or not.
                 * By default, LIN Driver set isBusBusy to false, and change node's state to IDLE.
                 */
                else
                {
                    /* Set current event ID to PID incorrect */
                    linCurrentState->currentEventId = LIN_PID_ERROR;
                    /* Clear bus busy flag */
                    linCurrentState->isBusBusy = false;
                    /* Change node's current state to IDLE */
                    linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;
                    /* Callback function to handle event MASTER SENT PID ERROR */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                }
            }
            /* If the node is SLAVE */
            else
            {
                /* Check the received PID */
                linCurrentState->currentId = LIN_ProcessParity(tmpByte, LIN_CHECK_PARITY);
                linCurrentState->currentPid = tmpByte;
                if (linCurrentState->currentId != 0xFFU)
                {
                    /* Set current event PID correct */
                    linCurrentState->currentEventId = LIN_PID_OK;

                    /* Clear Bus bus flag */
                    linCurrentState->isBusBusy = false;

                    /* Callback function to handle correct PID */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                }
                else
                {
                    /* Set current event PID incorrect */
                    linCurrentState->currentEventId = LIN_PID_ERROR;

                    /* Callback function to handle correct PID */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }

                    /* Clear Bus bus flag */
                    linCurrentState->isBusBusy = false;

                    /* Change node's current state to IDLE */
                    (void)LIN_LPUART_GotoIdleState(base);
                }
            }
            break;
        default:
            /* Other node state */
            break;
    }
}

static void LIN_LPUART_ProcessReceiveFrameData(uint32_t instance, uint8_t tmpByte)
{
    LPUART_Type *base = g_linLpuartBase[instance];
    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    if (linCurrentState->rxSize > (linCurrentState->cntByte + 1U))
    {
        *(linCurrentState->rxBuff) = tmpByte;
        linCurrentState->rxBuff++;
    }
    else
    {
        if ((linCurrentState->rxSize - linCurrentState->cntByte) == 1U)
        {
            linCurrentState->checkSum = tmpByte;
        }
    }

    linCurrentState->cntByte++;
    if (linCurrentState->cntByte == linCurrentState->rxSize)
    {
        /* Restore rxBuffer pointer */
        linCurrentState->rxBuff -= linCurrentState->rxSize - 1U;
        if (LIN_MakeChecksumByte(linCurrentState->rxBuff, linCurrentState->rxSize - 1U, linCurrentState->currentPid) ==
            linCurrentState->checkSum)
        {
            linCurrentState->currentEventId = LIN_RX_COMPLETED;
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA_COMPLETED;

            /* callback function to handle RX COMPLETED */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }
            if (linCurrentState->isRxBlocking == true)
            {
                /* Post Semaphore to signal Rx Completed*/
                linCurrentState->rxCompleted = true;
            }
            /* Clear Bus bus flag */
            linCurrentState->isBusBusy = false;
            linCurrentState->isRxBusy = false;

            /* In case of receiving a go to sleep request, after callback, node is in SLEEP MODE */
            /* In this case, node is in SLEEP MODE state */
            if (linCurrentState->currentNodeState != LIN_NODE_STATE_SLEEP_MODE)
            {
                (void)LIN_LPUART_GotoIdleState(base);
            }
        }
        else
        {
            linCurrentState->currentEventId = LIN_CHECKSUM_ERROR;

            /* callback function to handle checksum error */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }
            /* Clear Bus bus flag */
            linCurrentState->isBusBusy = false;
            linCurrentState->isRxBusy = false;
            /* Change node's current state to IDLE */
            (void)LIN_LPUART_GotoIdleState(base);
        }
    }
}

static void LIN_LPUART_ProcessSendFrameData(uint32_t instance, uint8_t tmpByte)
{
    bool sendFlag = true;
    uint8_t tmpSize;
    bool tmpCheckSumAndSize;
    bool tmpBuffAndSize;

    LPUART_Type *base = g_linLpuartBase[instance];

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    if ((bool)(LIN_LPUART_GetStatusFlags(base) & kLPUART_TxDataRegEmptyFlag) == false)
    {
        linCurrentState->currentEventId = LIN_READBACK_ERROR;

        /* callback function to handle Readback error */
        if (linCurrentState->Callback != NULL)
        {
            linCurrentState->Callback(instance, linCurrentState);
        }
        if (linCurrentState->isTxBlocking == false)
        {
            /* Clear Bus bus flag */
            linCurrentState->isBusBusy = false;
            linCurrentState->isTxBusy = false;
            /* Change node's current state to IDLE */
            (void)LIN_LPUART_GotoIdleState(base);
        }
        sendFlag = false;
    }
    else
    {
        tmpSize = linCurrentState->txSize - linCurrentState->cntByte;
        tmpCheckSumAndSize = (tmpSize == 1U) && (linCurrentState->checkSum != tmpByte);
        tmpBuffAndSize = (*linCurrentState->txBuff != tmpByte) && (tmpSize != 1U);
        if (tmpBuffAndSize || tmpCheckSumAndSize)
        {
            linCurrentState->currentEventId = LIN_READBACK_ERROR;

            /* callback function to handle Readback error */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }
            if (linCurrentState->isTxBlocking == false)
            {
                /* Clear Bus bus flag */
                linCurrentState->isBusBusy = false;
                linCurrentState->isTxBusy = false;
                /* Change node's current state to IDLE */
                (void)LIN_LPUART_GotoIdleState(base);
            }
            sendFlag = false;
        }
        else
        {
            linCurrentState->txBuff++;
            linCurrentState->cntByte++;
        }
    }

    if (sendFlag == true)
    {
        if (linCurrentState->cntByte < linCurrentState->txSize)
        {
            /* Send checksum byte */
            if ((linCurrentState->txSize - linCurrentState->cntByte) == 1U)
            {
                LIN_LPUART_WriteByte(base, linCurrentState->checkSum);
            }
            /* Send data bytes */
            else
            {
                LIN_LPUART_WriteByte(base, *linCurrentState->txBuff);
            }
        }
        else
        {
            linCurrentState->currentEventId = LIN_TX_COMPLETED;
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA_COMPLETED;

            LIN_LPUART_DisableInterrupts(base, kLPUART_RxDataRegFullFlag);
            /* callback function to handle event TX COMPLETED */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }
            /* Check if the transmission is non-blocking */
            if (linCurrentState->isTxBlocking == true)
            {
                linCurrentState->txCompleted = true;
            }
            /* Clear Bus bus flag */
            linCurrentState->isBusBusy = false;
            linCurrentState->isTxBusy = false;
            /* Change node's current state to IDLE */
            (void)LIN_LPUART_GotoIdleState(base);
        }
    }
}

lin_status_t LIN_LPUART_Init(LPUART_Type *base,
                             lin_user_config_t *linUserConfig,
                             lin_state_t *linCurrentState,
                             uint32_t linSourceClockFreq)
{
    uint32_t temp;
    uint32_t osrValue;
    uint16_t sbrValue;
    lin_status_t retVal = LIN_INITIALIZED;
    /* Assert parameters. */

    assert(linUserConfig);
    assert(linUserConfig->baudRate);

    uint32_t instance = LIN_LPUART_GetInstance(base);

    if (linSourceClockFreq == 0U)
    {
        retVal = LIN_LPUART_STAT_CLOCK_GATED_OFF;
    }
    else
    {
        /* if autobaud is enabled */
        if ((linUserConfig->autobaudEnable) && (linUserConfig->nodeFunction == (bool)LIN_SLAVE))
        {
            /* Setting Slave's baudrate to 19200 will help Slave node always detect LIN Break from Master */
            linUserConfig->baudRate = 19200U;
            linCurrentState->fallingEdgeInterruptCount = 0U;
            linCurrentState->baudrateEvalEnable = true;
            s_previousTwoBitTimeLength[instance] = 0U;
            s_countMeasure[instance] = 0U;
            s_timeMeasure[instance] = 0U;
        }
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
        /* LPUART init */
        CLOCK_EnableClock(s_lpuartClock[instance]);
#if defined(LPUART_PERIPH_CLOCKS)
        CLOCK_EnableClock(s_lpuartPeriphClocks[instance]);
#endif
#endif
        /* Reset values */
        base->CTRL = 0x00000000;
        /* Reset match addresses */
        base->MATCH = 0x00000000;
#if FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
        /* Reset IrDA modem features */
        base->MODIR = 0x00000000;
#endif
#if FSL_FEATURE_LPUART_FIFO_SIZEn(instance) > 0U
        /* Reset FIFO feature */
        base->FIFO = (LPUART_FIFO_RXUF_MASK | LPUART_FIFO_TXOF_MASK);
        /* Reset FIFO Watermark values */
        base->WATER = 0x00000000;
#endif

        if (LIN_LPUART_CalculateBaudRate(base, linUserConfig->baudRate, linSourceClockFreq, &osrValue, &sbrValue) ==
            kStatus_Success)
        {
            /* Set baudrate to User's value */
            LIN_LPUART_SetBaudRate(base, &osrValue, &sbrValue);

            /* config 8-bit (M=0) */
            LIN_LPUART_SetBitCountPerChar(base, LPUART_8_BITS_PER_CHAR);

            /* Set no parity mode */
            base->CTRL &= ~LPUART_CTRL_PE_MASK;
            base->CTRL &= ~LPUART_CTRL_PT_MASK;

            /* config 1 stop bit  */
            temp = base->BAUD & ~LPUART_BAUD_SBNS_MASK;
            base->BAUD = temp | LPUART_BAUD_SBNS(kLPUART_OneStopBit);

            /* Set Break char transmit length as 13 bits minimum */
            if (linUserConfig->nodeFunction == (bool)LIN_MASTER)
            {
                base->STAT |= LPUART_STAT_BRK13_MASK;
            }

            DisableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
            DisableIRQ(g_linLpuartRxTxIrqId[instance]);
#endif
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
            /* Set Break char detect length as 13 bits minimum and interrupt */
            base->STAT |= LPUART_STAT_LBKDE_MASK;
            LIN_LPUART_EnableInterrupts(base, kLPUART_LinBreakInterruptEnable);
#endif

            LIN_LPUART_EnableInterrupts(base,
                                        (kLPUART_FramingErrorInterruptEnable | kLPUART_RxDataRegFullInterruptEnable));
            EnableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
            EnableIRQ(g_linLpuartErrIrqId[instance]);
#endif

            /* Change node's current state to IDLE */
            linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;
            linCurrentState->linSourceClockFreq = linSourceClockFreq;
            /* Clear flags in current LIN state structure */
            linCurrentState->isTxBusy = false;
            linCurrentState->isRxBusy = false;
            linCurrentState->isBusBusy = false;
            linCurrentState->isRxBlocking = false;
            linCurrentState->isTxBlocking = false;
            linCurrentState->timeoutCounterFlag = false;
            linCurrentState->timeoutCounter = 0U;
            linCurrentState->txCompleted = false;
            linCurrentState->rxCompleted = false;
            linCurrentState->frame_index = 0U;

            /* Assign wakeup signal to satisfy LIN Specifications specifies that
             * wakeup signal shall be in range from 250us to 5 ms.
             */
            if (linUserConfig->baudRate > 10000U)
            {
                /* Wakeup signal will be range from 400us to 800us depend on baudrate */
                s_wakeupSignal[instance] = 0x80U;
            }
            else
            {
                /* Wakeup signal will be range from 400us to 4ms depend on baudrate */
                s_wakeupSignal[instance] = 0xF8U;
            }
            if (!((linUserConfig->autobaudEnable) && (linUserConfig->nodeFunction == (bool)LIN_SLAVE)))
            {
                /* Enable the LPUART transmitter and receiver */
                base->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
            }
        }
        else
        {
            retVal = LIN_ERROR;
        }
    } /* End of if (linSourceClockFreq == 0U) */

    return retVal;
}

lin_status_t LIN_LPUART_Deinit(LPUART_Type *base)
{
    lin_status_t retVal = LIN_SUCCESS;
    uint32_t instance = LIN_LPUART_GetInstance(base);
    lin_state_t *linCurrentState = g_linStatePtr[instance];
    if ((linCurrentState == NULL) || (linCurrentState->linSourceClockFreq == 0U))
    {
        retVal = LIN_ERROR;
    }
    else
    {
        /* Wait until the data is completely shifted out of shift register */
        while (!(LIN_LPUART_GetStatusFlags(base) & kLPUART_TransmissionCompleteFlag))
        {
            /* Do nothing */
        }

        /* Disable the LPUART transmitter and receiver */
        base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

        /* Disable LPUART interrupt. */
        DisableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
        DisableIRQ(g_linLpuartErrIrqId[instance]);
#endif

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
        /* Disable LPUART clock */
        CLOCK_DisableClock(s_lpuartClock[instance]);
#if defined(LPUART_PERIPH_CLOCKS)
        CLOCK_DisableClock(s_lpuartPeriphClocks[instance]);
#endif
#endif
        /* Change node's current state to UNINIT */
        linCurrentState->currentNodeState = LIN_NODE_STATE_UNINIT;

        /* Clear our saved pointer to the LIN state structure */
        g_linStatePtr[instance] = NULL;
    }

    return retVal;
}

lin_status_t LIN_LPUART_SendFrameDataBlocking(LPUART_Type *base,
                                              const uint8_t *txBuff,
                                              uint8_t txSize,
                                              uint32_t timeoutMSec)
{
    /* Assert parameters. */
    assert(txBuff);

    uint32_t instance = LIN_LPUART_GetInstance(base);
    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];
    lin_status_t retVal = LIN_SUCCESS;
    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if txSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < txSize) || (0U == txSize) || checkSleepMode)
    {
        retVal = LIN_ERROR;
    }
    else
    {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = LIN_BUS_BUSY;
        }
        else
        {
            /* Make the checksum byte. */
            linCurrentState->checkSum = LIN_MakeChecksumByte(txBuff, txSize, linCurrentState->currentPid);

            /* Update the LIN state structure. */
            linCurrentState->txBuff = txBuff;
            /* Add a place for checksum byte */
            linCurrentState->txSize = txSize + 1U;
            linCurrentState->cntByte = 0U;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;
            linCurrentState->isTxBusy = true;
            linCurrentState->isTxBlocking = true;
            linCurrentState->txCompleted = false;

#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
            /* Set Break char detect length as 10 bits minimum */
            base->STAT &= ~LPUART_STAT_LBKDE_MASK;
#endif

            LIN_SetTimeoutCounter(instance, timeoutMSec);
            /* Set node's current state to SEND_DATA */
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA;

            /* Start sending data */
            LIN_LPUART_WriteByte(base, *linCurrentState->txBuff);
        }
    }
    return retVal;
}

lin_status_t LIN_LPUART_SendFrameData(LPUART_Type *base, const uint8_t *txBuff, uint8_t txSize)
{
    /* Assert parameters. */
    assert(txBuff);

    uint32_t instance = LIN_LPUART_GetInstance(base);
    lin_state_t *linCurrentState = g_linStatePtr[instance];
    lin_status_t retVal = LIN_SUCCESS;
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if txSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < txSize) || (0U == txSize) || checkSleepMode)
    {
        retVal = LIN_ERROR;
    }
    /* If txSixe is in range from 1 to 8 */
    else
    {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = LIN_BUS_BUSY;
        }
        else
        {
            /* Make the checksum byte. */
            linCurrentState->checkSum = LIN_MakeChecksumByte(txBuff, txSize, linCurrentState->currentPid);

            /* Update the LIN state structure. */
            linCurrentState->txBuff = txBuff;
            /* Add a place for checksum byte */
            linCurrentState->txSize = txSize + 1U;
            linCurrentState->cntByte = 0U;
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;
            linCurrentState->isTxBusy = true;

/* Set Break char detect length as 10 bits minimum */
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
            /* Set Break char detect length as 10 bits minimum */
            base->STAT &= ~LPUART_STAT_LBKDE_MASK;
#endif
            /* Start sending data */
            LIN_LPUART_WriteByte(base, *linCurrentState->txBuff);
        }
    }
    return retVal;
}

lin_status_t LIN_LPUART_GetTransmitStatus(LPUART_Type *base, uint8_t *bytesRemaining)
{
    lin_status_t retVal = LIN_SUCCESS;

    uint32_t instance = LIN_LPUART_GetInstance(base);
    /* Get the current LIN state of this LPUART instance. */
    const lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Get the number of bytes that is still needed to transmit */
    *bytesRemaining = linCurrentState->txSize - linCurrentState->cntByte;

    /* Return status of the on-going transmission */
    if ((linCurrentState->currentEventId == LIN_NO_EVENT) && (*bytesRemaining != 0U))
    {
        if (linCurrentState->timeoutCounterFlag == false)
        {
            retVal = LIN_TX_BUSY;
        }
        else
        {
            retVal = LIN_TIMEOUT;
        }
    }

    return retVal;
}

lin_status_t LIN_LPUART_RecvFrmDataBlocking(LPUART_Type *base, uint8_t *rxBuff, uint8_t rxSize, uint32_t timeoutMSec)
{
    assert(rxBuff);

    uint32_t instance = LIN_LPUART_GetInstance(base);

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];
    lin_status_t retVal = LIN_SUCCESS;
    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if rxSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < rxSize) || (0U == rxSize) || checkSleepMode)
    {
        retVal = LIN_ERROR;
    }
    /* if rxSize is from 1 to 8 */
    else
    {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = LIN_BUS_BUSY;
        }
        else
        {
            /* Update the LIN state structure. */
            linCurrentState->rxBuff = rxBuff;
            /* Add a place for checksum byte */
            linCurrentState->rxSize = rxSize + 1U;
            linCurrentState->cntByte = 0U;

            /* Start receiving data */
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;
            linCurrentState->isRxBusy = true;
            linCurrentState->isRxBlocking = true;
            linCurrentState->rxCompleted = false;

#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
            /* Set Break char detect length as 10 bits minimum */
            base->STAT &= ~LPUART_STAT_LBKDE_MASK;
#endif

            LIN_SetTimeoutCounter(instance, timeoutMSec);
        }
    }

    return retVal;
}

lin_status_t LIN_LPUART_RecvFrmData(LPUART_Type *base, uint8_t *rxBuff, uint8_t rxSize)
{
    assert(rxBuff);

    lin_status_t retVal = LIN_SUCCESS;

    uint32_t instance = LIN_LPUART_GetInstance(base);

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if rxSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < rxSize) || (0U == rxSize) || checkSleepMode)
    {
        retVal = LIN_ERROR;
    }
    /* if rxSize is from 1 to 8 */
    else
    {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = LIN_BUS_BUSY;
        }
        else
        {
            /* Update the LIN state structure. */
            linCurrentState->rxBuff = rxBuff;
            /* Add a place for checksum byte */
            linCurrentState->rxSize = rxSize + 1U;
            linCurrentState->cntByte = 0U;

            /* Start receiving data */
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;
            linCurrentState->isRxBusy = true;
            linCurrentState->isRxBlocking = false;

#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
            /* Set Break char detect length as 10 bits minimum */
            base->STAT &= ~LPUART_STAT_LBKDE_MASK;
#endif
        }
    }

    return retVal;
}

lin_status_t LIN_LPUART_AbortTransferData(LPUART_Type *base)
{
    lin_status_t retVal = LIN_SUCCESS;

    uint32_t instance = LIN_LPUART_GetInstance(base);
    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Check if no transfer is running. */
    if (linCurrentState->isBusBusy)
    {
        /* Change node's current state to IDLE */
        (void)LIN_LPUART_GotoIdleState(base);

        /* Clear LIN Bus Busy flag */
        linCurrentState->isTxBusy = false;
        linCurrentState->isRxBusy = false;
        linCurrentState->isBusBusy = false;
    }

    return retVal;
}

lin_status_t LIN_LPUART_GetReceiveStatus(LPUART_Type *base, uint8_t *bytesRemaining)
{
    lin_status_t retVal = LIN_SUCCESS;

    uint32_t instance = LIN_LPUART_GetInstance(base);
    /* Get the current LIN state of this LPUART instance. */
    const lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Get the number of bytes that is still needed to receive */
    *bytesRemaining = linCurrentState->rxSize - linCurrentState->cntByte;

    /* Return status of the on-going reception */
    if ((linCurrentState->currentEventId == LIN_NO_EVENT) && (*bytesRemaining != 0U))
    {
        if (linCurrentState->timeoutCounterFlag == false)
        {
            retVal = LIN_RX_BUSY;
        }
        else
        {
            retVal = LIN_TIMEOUT;
        }
    }

    return retVal;
}

lin_status_t LIN_LPUART_GoToSleepMode(LPUART_Type *base)
{
    lin_status_t retVal = LIN_SUCCESS;

    uint32_t instance = LIN_LPUART_GetInstance(base);

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Update node's current state to SLEEP_MODE. */
    linCurrentState->currentNodeState = LIN_NODE_STATE_SLEEP_MODE;

    /* Set Receive data not inverted */
    LIN_LPUART_SetRxDataPolarity(base, false);

    DisableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
    DisableIRQ(g_linLpuartErrIrqId[instance]);
#endif
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
    LIN_LPUART_DisableInterrupts(base, kLPUART_LinBreakInterruptEnable);
#endif

    LIN_LPUART_DisableInterrupts(base, (kLPUART_RxDataRegFullInterruptEnable | kLPUART_FramingErrorInterruptEnable));
    LIN_LPUART_EnableInterrupts(base, kLPUART_RxActiveEdgeInterruptEnable);
    EnableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
    EnableIRQ(g_linLpuartErrIrqId[instance]);
#endif

    return retVal;
}

lin_status_t LIN_LPUART_GotoIdleState(LPUART_Type *base)
{
    uint32_t instance = LIN_LPUART_GetInstance(base);
    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Set Receive data not inverted */
    LIN_LPUART_SetRxDataPolarity(base, false);

    DisableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
    DisableIRQ(g_linLpuartErrIrqId[instance]);
#endif
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
    /* Set Break char detect length as 13 bits minimum */
    base->STAT |= LPUART_STAT_LBKDE_MASK;
    LIN_LPUART_EnableInterrupts(base, kLPUART_LinBreakInterruptEnable);
#endif

    LIN_LPUART_EnableInterrupts(base, (kLPUART_RxDataRegFullInterruptEnable | kLPUART_FramingErrorInterruptEnable));

    LIN_LPUART_DisableInterrupts(base, kLPUART_RxActiveEdgeInterruptEnable);

    EnableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
    EnableIRQ(g_linLpuartErrIrqId[instance]);
#endif
    /* Change node's current state to IDLE */
    linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;

    return LIN_SUCCESS;
}

lin_status_t LIN_LPUART_SendWakeupSignal(LPUART_Type *base)
{
    uint32_t instance = LIN_LPUART_GetInstance(base);

    /* Get the current LIN state of this LPUART instance. */
    const lin_state_t *linCurrentState = g_linStatePtr[instance];
    lin_status_t retVal = LIN_SUCCESS;

    if (linCurrentState->isBusBusy == false)
    {
        /* Send a wakeup signal */
        LIN_LPUART_WriteByte(base, s_wakeupSignal[instance]);
    }
    else
    {
        retVal = LIN_BUS_BUSY;
    }

    return retVal;
}

lin_status_t LIN_LPUART_MasterSendHeader(LPUART_Type *base, uint8_t id)
{
    lin_status_t retVal = LIN_SUCCESS;
    bool checkSleepMode = false;

    uint32_t instance = LIN_LPUART_GetInstance(base);
    /* Get the current LIN user config structure of this LPUART instance. */
    const lin_user_config_t *linUserConfig = g_linUserconfigPtr[instance];

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Check whether current mode is sleep mode */
    checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);
    /* Check if the current node is slave */
    if ((linUserConfig->nodeFunction == (bool)LIN_SLAVE) || (0x3FU < id) || checkSleepMode)
    {
        retVal = LIN_ERROR;
    }
    else
    {
        /* Check if the LIN bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = LIN_BUS_BUSY;
        }
        else
        {
            linCurrentState->currentId = id;

            /* Make parity for the current ID */
            linCurrentState->currentPid = LIN_ProcessParity(id, LIN_MAKE_PARITY);

            /* Send Break field */
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_BREAK_FIELD;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;

#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
            /* Set Break char detect length as 13 bits minimum */
            base->STAT |= LPUART_STAT_LBKDE_MASK;
            LIN_LPUART_EnableInterrupts(base, kLPUART_LinBreakInterruptEnable);
#endif
            LIN_LPUART_QueueBreakChar(base);
        }
    }

    return retVal;
}

lin_status_t LIN_LPUART_EnableIRQ(LPUART_Type *base)
{
    lin_status_t retVal = LIN_SUCCESS;
    uint32_t instance = LIN_LPUART_GetInstance(base);
    /* Get the current LIN state of this LPUART instance. */
    const lin_state_t *linCurrentState = g_linStatePtr[instance];

    DisableIRQ(g_linLpuartRxTxIrqId[instance]);

    if (linCurrentState->currentNodeState == LIN_NODE_STATE_SLEEP_MODE)
    {
        /* Enable RX Input Active Edge interrupt */
        LIN_LPUART_EnableInterrupts(base, kLPUART_RxActiveEdgeInterruptEnable);
    }
    else
    {
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
        LIN_LPUART_EnableInterrupts(base, kLPUART_LinBreakInterruptEnable);
#endif
        LIN_LPUART_EnableInterrupts(base, (kLPUART_RxDataRegFullInterruptEnable | kLPUART_FramingErrorInterruptEnable));
        EnableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
        EnableIRQ(g_linLpuartErrIrqId[instance]);
#endif
    }
    return retVal;
}

lin_status_t LIN_LPUART_DisableIRQ(LPUART_Type *base)
{
    lin_status_t retVal = LIN_SUCCESS;

    uint32_t instance = LIN_LPUART_GetInstance(base);

    /* Get the current LIN state of this LPUART instance. */
    const lin_state_t *linCurrentState = g_linStatePtr[instance];

    if (linCurrentState->currentNodeState == LIN_NODE_STATE_SLEEP_MODE)
    {
        /* Disable RX Input Active Edge interrupt */
        LIN_LPUART_DisableInterrupts(base, kLPUART_RxActiveEdgeInterruptEnable);
    }
    else
    {
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
        LIN_LPUART_DisableInterrupts(base, kLPUART_LinBreakInterruptEnable);
#endif
        LIN_LPUART_DisableInterrupts(base,
                                     (kLPUART_RxDataRegFullInterruptEnable | kLPUART_FramingErrorInterruptEnable));

        DisableIRQ(g_linLpuartRxTxIrqId[instance]);
#ifdef LPUART_ERR_IRQS
        DisableIRQ(g_linLpuartErrIrqId[instance]);
#endif
    }
    /* Disable LPUART interrupt. */
    return retVal;
}

lin_status_t LIN_LPUART_AutoBaudCapture(uint32_t instance)
{
    /* Get base address of the LPUART instance. */
    LPUART_Type *base = g_linLpuartBase[instance];
    /* Get the current LIN user config structure of this LPUART instance. */
    const lin_user_config_t *linUserConfig = g_linUserconfigPtr[instance];
    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];
    lin_status_t retVal = LIN_BUS_BUSY;
    uint32_t tmpTime = 0U;

    if (linCurrentState->baudrateEvalEnable)
    {
        /* Calculate time between two bit (for service autobaud) */
        (void)linUserConfig->timerGetTimeIntervalCallback(&tmpTime);

        /* Get two bits time length */
        s_timeMeasure[instance] += tmpTime;
        s_countMeasure[instance]++;
        if ((s_countMeasure[instance] > 1U))
        {
            switch (linCurrentState->currentNodeState)
            {
                /* If current state is SLEEP MODE */
                case LIN_NODE_STATE_SLEEP_MODE:
                    /* If length of the dominant signal is from 150us to 5ms, it is a wakeup signal */
                    if ((tmpTime >= 150000U) && (tmpTime <= 5000000U))
                    {
                        linCurrentState->currentEventId = LIN_WAKEUP_SIGNAL;

                        /* Callback to handle event: Received a wakeup signal */
                        if (linCurrentState->Callback != NULL)
                        {
                            linCurrentState->Callback(instance, linCurrentState);
                        }

                        /* Change node's state to IDLE */
                        (void)LIN_LPUART_GotoIdleState(base);
                    }
                    else
                    {
                        retVal = LIN_ERROR;
                    }
                    s_countMeasure[instance] = 0U;

                    break;
                /* If current state is IDLE */
                case LIN_NODE_STATE_IDLE:
                    /* Check break time minimum */
                    if (tmpTime >= AUTOBAUD_BREAK_TIME_MIN)
                    {
#if defined(FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT
                        /* Set Break char detect length as 13 bits minimum and interrupt */
                        base->STAT &= ~LPUART_STAT_LBKDE_MASK;
                        LIN_LPUART_DisableInterrupts(base, kLPUART_LinBreakInterruptEnable);
#endif

                        /* Set flag LIN bus busy */
                        linCurrentState->isBusBusy = true;

                        /* Change the node's current state to RECEIVED BREAK FIELD */
                        linCurrentState->currentEventId = LIN_RECV_BREAK_FIELD_OK;

                        /* Callback function */
                        if (linCurrentState->Callback != NULL)
                        {
                            linCurrentState->Callback(instance, linCurrentState);
                        }

                        /* Change the node's current state to RECEIVING SYNC FIELD */
                        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_SYNC;

                        /* Start Autobaud Count(initialize number of measurements in sync byte) */
                        linCurrentState->fallingEdgeInterruptCount = 0U;

                        s_countMeasure[instance] = 1U;
                    }
                    else
                    {
                        s_countMeasure[instance] = 0U;
                        retVal = LIN_ERROR;
                    }

                    break;
                /* If current state is RECEIVE SYNC */
                default:
                    /* Calculate baudrate */
                    LIN_LPUART_AutobaudTimerValEval(instance, s_timeMeasure[instance]);

                    /* Reset to measure in next times */
                    s_countMeasure[instance] = 0U;
                    s_timeMeasure[instance] = 0U;

                    if (linCurrentState->currentNodeState == LIN_NODE_STATE_IDLE)
                    {
                        retVal = LIN_ERROR;
                    }

                    break;
            }
        }
    }
    else
    {
        if (linCurrentState->fallingEdgeInterruptCount > 4U)
        {
            /* Enable the LPUART transmitter and receiver */
            base->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

            linCurrentState->fallingEdgeInterruptCount = 0U;
        }

        retVal = LIN_SUCCESS;
    }

    return retVal;
}

void LIN_LPUART_IRQHandler(LPUART_Type *base)
{
    uint8_t tmpByte = 0xFFU;

    uint32_t instance = LIN_LPUART_GetInstance(base);

    /* Get the current LIN state of this LPUART instance. */
    lin_state_t *linCurrentState = g_linStatePtr[instance];

    /* Check RX Input Active Edge interrupt enable */
    bool activeEdgeIntState = (bool)(base->BAUD & (1 << LPUART_BAUD_RXEDGIE_SHIFT));

    /* If LIN break character has been detected. */
    if (LIN_LPUART_GetStatusFlags(base) & kLPUART_LinBreakFlag)
    {
        LIN_LPUART_ProcessBreakDetect(instance);
    }
    else
    {
        /* If LPUART_RX Pin Active Edge has been detected. */
        if ((LIN_LPUART_GetStatusFlags(base) & kLPUART_RxActiveEdgeFlag) && activeEdgeIntState)
        {
            /* Clear LPUART_RX Pin Active Edge Interrupt Flag. Write 1 to clear */
            LIN_LPUART_ClearStatusFlags(base, kLPUART_RxActiveEdgeFlag);

            /* Check if the node is in SLEEP MODE */
            /* If yes, then check if a wakeup signal has been received */
            if (linCurrentState->currentNodeState == LIN_NODE_STATE_SLEEP_MODE)
            {
                LIN_LPUART_CheckWakeupSignal(instance);
            }
        }
        else
        {
            /* If Framing Error has been detected Write 1 to clear */
            if (LIN_LPUART_GetStatusFlags(base) & kLPUART_FramingErrorFlag)
            {
                /* Clear Framing Error Interrupt Flag */
                LIN_LPUART_ClearStatusFlags(base, kLPUART_FramingErrorFlag);

                /* Read dummy to clear LPUART_RX_DATA_REG_FULL flag */
                LIN_LPUART_ReadByte(base, &tmpByte);
                /* Set current event id to LIN_FRAME_ERROR */
                linCurrentState->currentEventId = LIN_FRAME_ERROR;

                if (linCurrentState->currentNodeState == LIN_NODE_STATE_SEND_DATA)
                {
                    /* Callback function to handle Framing Error Event */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                }

                if (linCurrentState->isRxBlocking == false)
                {
                    if (linCurrentState->currentNodeState == LIN_NODE_STATE_RECV_DATA)
                    {
                        /* Callback function to handle Framing Error Event */
                        if (linCurrentState->Callback != NULL)
                        {
                            linCurrentState->Callback(instance, linCurrentState);
                        }
                    }
                }

                /* Clear Bus busy Flag */
                linCurrentState->isBusBusy = false;
                /* Change node's state to IDLE */
                (void)LIN_LPUART_GotoIdleState(base);
            }
            else
            {
                if (LIN_LPUART_GetStatusFlags(base) & kLPUART_RxDataRegFullFlag)
                {
                    LIN_LPUART_ClearStatusFlags(base, kLPUART_RxDataRegFullFlag);
                    /* Get data from Data Register & Clear LPUART_RX_DATA_REG_FULL flag */
                    LIN_LPUART_ReadByte(base, &tmpByte);
                    /* Process data in Data Register while receive, send data */
                    LIN_LPUART_ProcessFrame(instance, tmpByte);
                }
            } /* End else: if (LIN_LPUART_GetStatusFlags(base) & kLPUART_FramingErrorFlag) */
        }     /* End else: if ((LIN_LPUART_GetStatusFlags(base) & kLPUART_RxActiveEdgeFlag) && activeEdgeIntState) */
    }         /* End else: if (LIN_LPUART_GetStatusFlags(base) & kLPUART_LinBreakFlag) */

    /* Get status RX overrun flag */
    if (LIN_LPUART_GetStatusFlags(base) & kLPUART_RxOverrunFlag)
    {
        /* Clear overrun flag */
        LIN_LPUART_ClearStatusFlags(base, kLPUART_RxOverrunFlag);
    }
} /* End void LIN_LPUART_DRV_IRQHandler(uint32_t instance) */
