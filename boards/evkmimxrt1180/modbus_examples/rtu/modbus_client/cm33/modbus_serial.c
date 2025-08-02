/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "fsl_lpuart.h"
#include "clock_config.h"
#include "app.h"
#include "port.h"

/* ----------------------- static functions ---------------------------------*/

static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

static bool RxIsrFlag = 0;
static bool TxIsrFlag = 0;

void DEMO_LPUART_IRQHandler(void)
{
    if (RxIsrFlag) {
        prvvUARTRxISR();
        LPUART_ClearStatusFlags(DEMO_LPUART, kLPUART_RxDataRegFullFlag);
    }

    if (TxIsrFlag) {
        prvvUARTTxReadyISR();
        LPUART_ClearStatusFlags(DEMO_LPUART, kLPUART_TransmissionCompleteFlag);
    }

    SDK_ISR_EXIT_BARRIER;
}

static void UartInit(uint32_t ulBaudRate)
{
    lpuart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = ulBaudRate;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(DEMO_LPUART, &config, DEMO_LPUART_CLK_FREQ);

    /* Enable RX interrupt. */
    EnableIRQ(DEMO_LPUART_IRQn);
}

/* ----------------------- Start implementation -----------------------------*/
void
vMBMasterPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. */
    if (xRxEnable) {
        /* Enable RX interrupt. */
        RxIsrFlag = 1;
        LPUART_EnableInterrupts(DEMO_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    } else {
        /* Disable RX interrupt. */
        RxIsrFlag = 0;
        LPUART_DisableInterrupts(DEMO_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    }
    LPUART_EnableRx(DEMO_LPUART, xRxEnable);

    /* If xTxENable enable transmitter empty interrupts. */
    if (xTxEnable) {
        /* Enable TX interrupt. */
        TxIsrFlag = 1;
        LPUART_EnableInterrupts(DEMO_LPUART, kLPUART_TransmissionCompleteInterruptEnable);
    } else {
        /* Disable TX interrupt. */
        TxIsrFlag = 0;
        LPUART_DisableInterrupts(DEMO_LPUART, kLPUART_TransmissionCompleteInterruptEnable);
    }
    LPUART_EnableTx(DEMO_LPUART, xTxEnable);
}

BOOL
xMBMasterPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    (void)ucPORT;
    (void)ucDataBits;
	(void)eParity;
    UartInit(ulBaudRate);
    return 1;
}

void vMBMasterPortClose(void)
{
    
}

BOOL
xMBMasterPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    LPUART_WriteByte(DEMO_LPUART, ucByte);
    return 1;
}

BOOL
xMBMasterPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    *pucByte = LPUART_ReadByte(DEMO_LPUART);
    return 1;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
  pxMBMasterFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
  pxMBMasterFrameCBByteReceived(  );
}
