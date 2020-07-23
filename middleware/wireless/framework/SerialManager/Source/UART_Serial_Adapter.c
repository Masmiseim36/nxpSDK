/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_os_abstraction.h"
#include "UART_Serial_Adapter.h"
#include "pin_mux.h"
#include "board.h"
#include "SerialManager.h"

#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
#include "fsl_uart.h"
#endif

#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
#include "fsl_lpuart.h"
#endif

#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
#include "fsl_lpsci.h"
#endif

#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
#include "fsl_usart.h"
#endif

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
static LPUART_Type * mLpuartBase[] = LPUART_BASE_PTRS;
static uartState_t * pLpuartStates[FSL_FEATURE_SOC_LPUART_COUNT];
static void LPUART_Common_ISR(uint32_t instance);
#endif

#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
static UART_Type * mUartBase[] = UART_BASE_PTRS;
static uartState_t * pUartStates[FSL_FEATURE_SOC_UART_COUNT];
static void UART_Common_ISR(uint32_t instance);
#endif

#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
static UART0_Type * mLpsciBase[] = UART0_BASE_PTRS;
static uartState_t * pLpsciStates[FSL_FEATURE_SOC_LPSCI_COUNT];
static void LPSCI_Common_ISR(uint32_t instance);
#endif

#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
static USART_Type  *mUsartBase[]    = USART_BASE_PTRS;
static IRQn_Type    mUsartIrqs[]    = USART_IRQS;
static uartState_t *pUsartStates[FSL_FEATURE_SOC_USART_COUNT];
static void USART_ISR(void);
#endif

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/************************************************************************************/
/*                                     LPUART                                       */
/************************************************************************************/
#if defined(gSerialCyclesToWaitAfterInitialize) && ( gSerialCyclesToWaitAfterInitialize != 0)
static uint32_t LPUART_ClearRxByte(uint32_t instance)
{
    LPUART_Type * base;
    uint32_t status;

    base   = mLpuartBase[instance];
    status = LPUART_GetStatusFlags(base);

    SERIAL_DBG_LOG("[%x] sts=%x", instance, status);

#if 0
    while ( ((uint32_t)(kLPUART_RxDataRegFullFlag) & status ) == 0)
    {
       status = LPUART_GetStatusFlags(base);
    }
#endif

    /* Check if data was received */
    if( (bool)((uint32_t)(kLPUART_RxDataRegFullFlag) & status ) )
    {
        uint8_t data = LPUART_ReadByte(base);
        status = LPUART_GetStatusFlags(base);
        SERIAL_DBG_LOG("data=%x, sts=%x", data, status);
    }

    return status;
}
#endif

uint32_t LPUART_Initialize(uint32_t instance, uartState_t *pState)
{
    static bool initialized = FALSE;
    uint32_t status = (uint32_t)gUartSuccess_c;

#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    LPUART_Type * base;
    lpuart_config_t config;
    IRQn_Type irqs[] = LPUART_RX_TX_IRQS;

    if( (instance >= (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT) || (NULL == pState) )
    {
        status = (uint32_t)gUartInvalidParameter_c;
    }
   else
   {
       base = mLpuartBase[instance];
       pLpuartStates[instance] = pState;
       /* Check if LPUART is already initialized */
       if( !initialized )
       {
           pState->rxCbParam = 0;
           pState->txCbParam = 0;
           pState->pRxData = NULL;
           pState->pTxData = NULL;
           pState->rxSize = 0;
           pState->txSize = 0;
           initialized = TRUE;
       }
       BOARD_InitLPUART();
       LPUART_GetDefaultConfig(&config);
       config.enableRx = TRUE;
       config.enableTx = TRUE;
       (void)LPUART_Init(base, &config, BOARD_GetLpuartClock(instance));

       LPUART_EnableInterrupts(base, (uint32_t)kLPUART_RxDataRegFullInterruptEnable);

       //OSA_InstallIntHandler(irqs[instance], LPUART_ISR);
#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
       if( irqs[instance] < FSL_FEATURE_INTMUX_IRQ_START_INDEX )
#endif
       {
           NVIC_SetPriority(irqs[instance], (uint32_t)gUartIsrPrio_c >> (8 - __NVIC_PRIO_BITS));
           NVIC_EnableIRQ(irqs[instance]);
       }

       /* Need to wait for roughly 80us so first bad detected byte is received  before clearing the
              RX buffer, a for loop of 10000 @48Mhz is roughly 100us  */
#if defined(gSerialCyclesToWaitAfterInitialize) && ( gSerialCyclesToWaitAfterInitialize != 0)
       for (int i=gSerialCyclesToWaitAfterInitialize; i>0; i--) {}
       LPUART_ClearRxByte(instance);
#endif

       NVIC_ClearPendingIRQ(irqs[instance]);
   }
#endif
    return status;
}


/************************************************************************************/
uint32_t LPUART_SetBaudrate(uint32_t instance, uint32_t baudrate)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    if( instance >= (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT )
    {
        status = (uint32_t)gUartInvalidParameter_c;
    }
    else
    {
        status = (uint32_t)LPUART_SetBaudRate(mLpuartBase[instance], baudrate, BOARD_GetLpuartClock(instance));
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPUART_SendData(uint32_t instance, uint8_t* pData, uint32_t size)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    LPUART_Type * base;

    if( (instance >= (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT) || (0u == size) || (NULL == pData) )
    {
        status = (uint32_t)gUartInvalidParameter_c;
    }
    else
    {
        base = mLpuartBase[instance];

        OSA_InterruptDisable();
        if( (bool)pLpuartStates[instance]->txSize )
        {
            OSA_InterruptEnable();
            status = (uint32_t)gUartBusy_c;
        }
        else
        {
            while( !(bool)((uint32_t)kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(base)) ) {}

            LPUART_WriteByte(base, *pData);
            pLpuartStates[instance]->pTxData = pData+1;
            pLpuartStates[instance]->txSize = size-1u;

            (void)LPUART_ClearStatusFlags(base, (uint32_t)kLPUART_TxDataRegEmptyFlag);
            LPUART_EnableInterrupts(base, (uint32_t)kLPUART_TxDataRegEmptyInterruptEnable);

            OSA_InterruptEnable();
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPUART_ReceiveData(uint32_t instance, uint8_t* pData, uint32_t size)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    if( (instance >= (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT)  || (0u == size) || (NULL == pData) )
    {
        status = (uint32_t)gUartInvalidParameter_c;
    }
    else
    {
        OSA_InterruptDisable();
        if( (bool)pLpuartStates[instance]->rxSize )
        {
            status = (uint32_t)gUartBusy_c;
        }
        else
        {
            pLpuartStates[instance]->rxSize = size;
            pLpuartStates[instance]->pRxData = pData;
        }
        OSA_InterruptEnable();
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t  LPUART_InstallRxCalback(uint32_t instance, uartCallback_t cb, uint32_t cbParam)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    if( instance >= (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT )
    {
        status = (uint32_t)gUartInvalidParameter_c;
    }
    else
    {
        pLpuartStates[instance]->rxCb = cb;
        pLpuartStates[instance]->rxCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t  LPUART_InstallTxCalback(uint32_t instance, uartCallback_t cb, uint32_t cbParam)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    if( instance >= (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT )
    {
        status = (uint32_t)gUartInvalidParameter_c;
    }
    else
    {
        pLpuartStates[instance]->txCb = cb;
        pLpuartStates[instance]->txCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPUART_IsTxActive(uint32_t instance)
{
    uint32_t status = 0;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    if( instance < (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT )
    {
        if( (bool)pLpuartStates[instance]->txSize )
        {
            status = 1;
        }
        else
        {
            status = (uint32_t)!(bool)(uint32_t)(LPUART_GetStatusFlags(mLpuartBase[instance]) & (uint32_t)kLPUART_TransmissionCompleteFlag);
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPUART_EnableLowPowerWakeup(uint32_t instance)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    status = LPUART_DisableLowPowerWakeup(instance);
    if( (uint32_t)gUartSuccess_c == status )
    {
        LPUART_EnableInterrupts(mLpuartBase[instance], (uint32_t)kLPUART_RxActiveEdgeInterruptEnable);
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPUART_DisableLowPowerWakeup(uint32_t instance)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    LPUART_Type * base;

    if( instance >= (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT )
    {
        status = (uint32_t)gUartInvalidParameter_c;
    }
    else
    {
        base = mLpuartBase[instance];
        LPUART_DisableInterrupts(base, (uint32_t)kLPUART_RxActiveEdgeInterruptEnable);
        status = (uint32_t)LPUART_ClearStatusFlags(base, (uint32_t)kLPUART_RxActiveEdgeFlag);
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPUART_IsWakeupSource(uint32_t instance)
{
    uint32_t status = 0;
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
    if( instance < (uint32_t)FSL_FEATURE_SOC_LPUART_COUNT )
    {
        status = (uint32_t)!!(bool)(uint32_t)(LPUART_GetStatusFlags(mLpuartBase[instance]) & (uint32_t)kLPUART_RxActiveEdgeFlag);
    }
#endif
    return status;
}

/************************************************************************************/
/*                                      UART                                        */
/************************************************************************************/
uint32_t UART_Initialize(uint32_t instance, uartState_t *pState)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    UART_Type * base;
    uart_config_t config;
    IRQn_Type irqs[] = UART_RX_TX_IRQS;

    if( (instance >= FSL_FEATURE_SOC_UART_COUNT) || (NULL == pState) )
    {
        status = gUartInvalidParameter_c;
    }
   else
   {
       base = mUartBase[instance];
       pUartStates[instance] = pState;
       pState->rxCbParam = 0;
       pState->txCbParam = 0;
       pState->pRxData = NULL;
       pState->pTxData = NULL;
       pState->rxSize = 0;
       pState->txSize = 0;

       BOARD_InitUART();
       UART_GetDefaultConfig(&config);
       config.enableRx = 1;
       config.enableTx = 1;
       UART_Init(base, &config, BOARD_GetUartClock(instance));
       UART_EnableInterrupts(base, kUART_RxDataRegFullInterruptEnable);
//       OSA_InstallIntHandler(irqs[instance], UART_ISR);
#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
       if( irqs[instance] < FSL_FEATURE_INTMUX_IRQ_START_INDEX )
#endif
       {
           NVIC_SetPriority(irqs[instance], gUartIsrPrio_c >> (8 - __NVIC_PRIO_BITS));
           NVIC_EnableIRQ(irqs[instance]);
       }
   }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_SetBaudrate(uint32_t instance, uint32_t baudrate)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    if( instance >= FSL_FEATURE_SOC_UART_COUNT )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        UART_SetBaudRate(mUartBase[instance], baudrate, BOARD_GetUartClock(instance));
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_SendData(uint32_t instance, uint8_t* pData, uint32_t size)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    UART_Type * base;

    if( instance >= FSL_FEATURE_SOC_UART_COUNT || !size || !pData )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        base = mUartBase[instance];

        OSA_InterruptDisable();
        if( pUartStates[instance]->txSize )
        {
            OSA_InterruptEnable();
            status = gUartBusy_c;
        }
        else
        {
            while( !(kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(base)) ) {}

            pUartStates[instance]->txSize = size-1;
            pUartStates[instance]->pTxData = pData+1;
            UART_WriteByte(base, *pData);

            UART_ClearStatusFlags(base, kUART_TxDataRegEmptyFlag);
            UART_EnableInterrupts(base, kUART_TxDataRegEmptyInterruptEnable);

            OSA_InterruptEnable();
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_ReceiveData(uint32_t instance, uint8_t* pData, uint32_t size)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    if( instance >= FSL_FEATURE_SOC_UART_COUNT  || !size || !pData )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        OSA_InterruptDisable();
        if( pUartStates[instance]->rxSize )
        {
            status = gUartBusy_c;
        }
        else
        {
            pUartStates[instance]->rxSize = size;
            pUartStates[instance]->pRxData = pData;
        }
        OSA_InterruptEnable();
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_InstallRxCalback(uint32_t instance, uartCallback_t cb, uint32_t cbParam)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    if( instance >= FSL_FEATURE_SOC_UART_COUNT )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        pUartStates[instance]->rxCb = cb;
        pUartStates[instance]->rxCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_InstallTxCalback(uint32_t instance, uartCallback_t cb, uint32_t cbParam)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    if( instance >= FSL_FEATURE_SOC_UART_COUNT )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        pUartStates[instance]->txCb = cb;
        pUartStates[instance]->txCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_IsTxActive(uint32_t instance)
{
    uint32_t status = 0;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    if( instance < FSL_FEATURE_SOC_UART_COUNT )
    {
        if( pUartStates[instance]->txSize )
        {
            status = 1;
        }
        else
        {
            status = !(UART_GetStatusFlags(mUartBase[instance]) & kUART_TransmissionCompleteFlag);
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_EnableLowPowerWakeup(uint32_t instance)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    status = UART_DisableLowPowerWakeup(instance);
    if( gUartSuccess_c == status )
    {
        UART_EnableInterrupts(mUartBase[instance], kUART_RxActiveEdgeInterruptEnable);
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_DisableLowPowerWakeup(uint32_t instance)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    UART_Type * base;

    if( instance >= FSL_FEATURE_SOC_UART_COUNT )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        base = mUartBase[instance];
        UART_DisableInterrupts(base, kUART_RxActiveEdgeInterruptEnable);
        UART_ClearStatusFlags(base, kUART_RxActiveEdgeFlag);
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t UART_IsWakeupSource(uint32_t instance)
{
    uint32_t status = 0;
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
    if( instance < FSL_FEATURE_SOC_UART_COUNT )
    {
        status = !!(UART_GetStatusFlags(mUartBase[instance]) & kUART_RxActiveEdgeFlag);
    }
#endif
    return status;
}


/************************************************************************************/
/*                                      LPSCI                                       */
/************************************************************************************/
uint32_t LPSCI_Initialize(uint32_t instance, uartState_t *pState)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    UART0_Type * base;
    lpsci_config_t config;
    IRQn_Type irqs[] = UART0_RX_TX_IRQS;

    if( (instance >= FSL_FEATURE_SOC_LPSCI_COUNT) || (NULL == pState) )
    {
        status = gUartInvalidParameter_c;
    }
   else
   {
       base = mLpsciBase[instance];
       pLpsciStates[instance] = pState;
       pState->rxCbParam = 0;
       pState->txCbParam = 0;
       pState->pRxData = NULL;
       pState->pTxData = NULL;
       pState->rxSize = 0;
       pState->txSize = 0;

       BOARD_InitLPSCI();
       LPSCI_GetDefaultConfig(&config);
       config.enableRx = 1;
       config.enableTx = 1;
       LPSCI_Init(base, &config, BOARD_GetLpsciClock(instance));
       LPSCI_EnableInterrupts(base, kLPSCI_RxDataRegFullInterruptEnable);
//       OSA_InstallIntHandler(irqs[instance], LPSCI_ISR);
#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
       if( irqs[instance] < FSL_FEATURE_INTMUX_IRQ_START_INDEX )
#endif
       {
           NVIC_SetPriority(irqs[instance], gUartIsrPrio_c >> (8 - __NVIC_PRIO_BITS));
           NVIC_EnableIRQ(irqs[instance]);
       }
   }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_SetBaudrate(uint32_t instance, uint32_t baudrate)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    if( instance >= FSL_FEATURE_SOC_LPSCI_COUNT )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        LPSCI_SetBaudRate(mLpsciBase[instance], baudrate, BOARD_GetLpsciClock(instance));
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_SendData(uint32_t instance, uint8_t* pData, uint32_t size)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    UART0_Type * base;

    if( instance >= FSL_FEATURE_SOC_LPSCI_COUNT || !size || !pData )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        base = mLpsciBase[instance];

        OSA_InterruptDisable();
        if( pLpsciStates[instance]->txSize )
        {
            OSA_InterruptEnable();
            status = gUartBusy_c;
        }
        else
        {
            while( !(kLPSCI_TxDataRegEmptyFlag & LPSCI_GetStatusFlags(base)) ) {}

            pLpsciStates[instance]->txSize = size-1;
            pLpsciStates[instance]->pTxData = pData+1;
            LPSCI_WriteByte(base, *pData);

            LPSCI_ClearStatusFlags(base, kLPSCI_TxDataRegEmptyFlag);
            LPSCI_EnableInterrupts(base, kLPSCI_TxDataRegEmptyInterruptEnable);

            OSA_InterruptEnable();
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_ReceiveData(uint32_t instance, uint8_t* pData, uint32_t size)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    if( instance >= FSL_FEATURE_SOC_LPSCI_COUNT  || !size || !pData )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        OSA_InterruptDisable();
        if( pLpsciStates[instance]->rxSize )
        {
            status = gUartBusy_c;
        }
        else
        {
            pLpsciStates[instance]->rxSize = size;
            pLpsciStates[instance]->pRxData = pData;
        }
        OSA_InterruptEnable();
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_InstallRxCalback(uint32_t instance, uartCallback_t cb, uint32_t cbParam)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    if( instance >= FSL_FEATURE_SOC_LPSCI_COUNT )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        pLpsciStates[instance]->rxCb = cb;
        pLpsciStates[instance]->rxCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_InstallTxCalback(uint32_t instance, uartCallback_t cb, uint32_t cbParam)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    if( instance >= FSL_FEATURE_SOC_LPSCI_COUNT )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        pLpsciStates[instance]->txCb = cb;
        pLpsciStates[instance]->txCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_IsTxActive(uint32_t instance)
{
    uint32_t status = 0;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    if( instance < FSL_FEATURE_SOC_LPSCI_COUNT )
    {
        if( pLpsciStates[instance]->txSize )
        {
            status = 1;
        }
        else
        {
            status = !(LPSCI_GetStatusFlags(mLpsciBase[instance]) & kLPSCI_TransmissionCompleteFlag);
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_EnableLowPowerWakeup(uint32_t instance)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    status = LPSCI_DisableLowPowerWakeup(instance);
    if( gUartSuccess_c == status )
    {
        LPSCI_EnableInterrupts(mLpsciBase[instance], kLPSCI_RxActiveEdgeInterruptEnable);
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_DisableLowPowerWakeup(uint32_t instance)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    UART0_Type * base;

    if( instance >= FSL_FEATURE_SOC_LPSCI_COUNT )
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        base = mLpsciBase[instance];
        LPSCI_DisableInterrupts(base, kLPSCI_RxActiveEdgeInterruptEnable);
        LPSCI_ClearStatusFlags(base, kLPSCI_RxActiveEdgeFlag);
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t LPSCI_IsWakeupSource(uint32_t instance)
{
    uint32_t status = 0;
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
    if( instance < FSL_FEATURE_SOC_LPSCI_COUNT )
    {
        status = !!(LPSCI_GetStatusFlags(mLpsciBase[instance]) & kLPSCI_RxActiveEdgeFlag);
    }
#endif
    return status;
}

/************************************************************************************/
/*                                      USART                                        */
/************************************************************************************/
uint32_t USART_Initialize(uint32_t instance, uartState_t *pState)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    USART_Type *base;
    usart_config_t config;

    if ((instance >= FSL_FEATURE_SOC_USART_COUNT) || (NULL == pState))
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        base = mUsartBase[instance];
        pUsartStates[instance] = pState;
        pState->rxCbParam = 0;
        pState->txCbParam = 0;
        pState->pRxData = NULL;
        pState->pRxData = NULL;
        pState->rxSize = 0;
        pState->txSize = 0;

        USART_GetDefaultConfig(&config);
        config.enableRx = 1;
        config.enableTx = 1;
        USART_Init(base, &config, BOARD_GetUsartClock(instance));
        USART_EnableInterrupts(base, kUSART_RxLevelInterruptEnable);
        OSA_InstallIntHandler(mUsartIrqs[instance], USART_ISR);
        NVIC_SetPriority(mUsartIrqs[instance], gUartIsrPrio_c >> (8 - __NVIC_PRIO_BITS));
        NVIC_EnableIRQ(mUsartIrqs[instance]);
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_SetBaudrate(uint32_t instance, uint32_t baudrate)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    if (instance >= FSL_FEATURE_SOC_USART_COUNT)
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        status = USART_SetBaudRate(mUsartBase[instance], baudrate, BOARD_GetUsartClock(instance));
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_SendData(uint32_t instance, uint8_t *pData, uint32_t size)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    USART_Type *base;

    if (instance >= FSL_FEATURE_SOC_USART_COUNT || !size || !pData)
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        base = mUsartBase[instance];

        OSA_InterruptDisable();
        if (pUsartStates[instance]->txSize)
        {
            OSA_InterruptEnable();
            status = gUartBusy_c;
        }
        else
        {
            while (!(kUSART_TxFifoEmptyFlag & USART_GetStatusFlags(base)))
            {
            }

            pUsartStates[instance]->txSize = size - 1;
            pUsartStates[instance]->pTxData = pData + 1;
            USART_WriteByte(base, *pData);
            OSA_InterruptEnable();

            USART_ClearStatusFlags(base, kUSART_TxFifoEmptyFlag);
            USART_EnableInterrupts(base, kUSART_TxLevelInterruptEnable);
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_ReceiveData(uint32_t instance, uint8_t *pData, uint32_t size)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    if (instance >= FSL_FEATURE_SOC_USART_COUNT || !size || !pData)
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        OSA_InterruptDisable();
        if (pUsartStates[instance]->rxSize)
        {
            status = gUartBusy_c;
        }
        else
        {
            pUsartStates[instance]->rxSize = size;
            pUsartStates[instance]->pRxData = pData;
        }
        OSA_InterruptEnable();
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_InstallRxCalback(uint32_t instance, uartCallback_t cb, uint32_t cbParam)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    if (instance >= FSL_FEATURE_SOC_USART_COUNT)
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        pUsartStates[instance]->rxCb = cb;
        pUsartStates[instance]->rxCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_InstallTxCalback(uint32_t instance, uartCallback_t cb, uint32_t cbParam)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    if (instance >= FSL_FEATURE_SOC_USART_COUNT)
    {
        status = gUartInvalidParameter_c;
    }
    else
    {
        pUsartStates[instance]->txCb = cb;
        pUsartStates[instance]->txCbParam = cbParam;
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_IsTxActive(uint32_t instance)
{
    uint32_t status = 0;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    if (instance < FSL_FEATURE_SOC_USART_COUNT)
    {
        if (pUsartStates[instance]->txSize)
        {
            status = 1;
        }
        else
        {
            status = !(USART_GetStatusFlags(mUsartBase[instance]) & kUSART_TxFifoEmptyFlag);
        }
    }
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_EnableLowPowerWakeup(uint32_t instance)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    // TODO
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_DisableLowPowerWakeup(uint32_t instance)
{
    uint32_t status = (uint32_t)gUartSuccess_c;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    // TODO
#endif
    return status;
}

/************************************************************************************/
uint32_t USART_IsWakeupSource(uint32_t instance)
{
    uint32_t status = 0;
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
    if( instance < FSL_FEATURE_SOC_USART_COUNT )
    {
        status = ((mUsartBase[instance]->STAT & USART_STAT_START_MASK) >> USART_STAT_START_SHIFT);
    }
#endif
    return status;
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************* */
#if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0))
static void LPUART_Common_ISR(uint32_t instance)
{
    uint32_t interrupts;
    LPUART_Type * base;
    uartState_t * pState;
    bool frameError = false;

    base = mLpuartBase[instance];
    pState = pLpuartStates[instance];
    interrupts = LPUART_GetEnabledInterrupts(base);

    uint32_t status = LPUART_GetStatusFlags(base);

//    SERIAL_DBG_LOG("[%d] int=%x sts=%x", instance , interrupts, status);

    /*check noise error*/
    if(0u != ((uint32_t)kLPUART_NoiseErrorFlag & status))
    {
        (void)LPUART_ClearStatusFlags(base, (uint32_t)kLPUART_NoiseErrorFlag);
        frameError = true;
    }

    /*check framing error*/
    if(0u != ((uint32_t)kLPUART_FramingErrorFlag & status))
    {
        (void)LPUART_ClearStatusFlags(base, (uint32_t)kLPUART_FramingErrorFlag);
        frameError = true;
    }

    /*check parity error*/
    if(0u != ((uint32_t)kLPUART_ParityErrorFlag & status))
    {
        (void)LPUART_ClearStatusFlags(base, (uint32_t)kLPUART_ParityErrorFlag);
        frameError = true;
    }

    /* Check if data was received */
    if( (bool)((uint32_t)(kLPUART_RxDataRegFullFlag) & status) )
    {
        uint8_t data = LPUART_ReadByte(base);

        if(false == frameError)
        {
            SERIAL_DBG_LOG("[%d] %x data=%x size=%d", instance , data, pState->rxSize);

            if( (bool)pState->rxSize )
            {
                pState->rxSize--;
            }
            if( pState->pRxData != NULL )
            {
                *(pState->pRxData) = data;
                pState->pRxData++;
            }

            if( (0u == pState->rxSize) && (NULL != pState->rxCb) )
            {
                pState->rxCb(pState);
            }

            //SERIAL_DBG_LOG("sts=%x", status);
        }
    }

    /* Check if data Tx has end */
    if( (bool)((uint32_t)kLPUART_TxDataRegEmptyFlag & status) &&
       (bool)((uint32_t)kLPUART_TxDataRegEmptyInterruptEnable & interrupts) )
    {
        if( (bool)pState->txSize )
        {
            pState->txSize--;

            LPUART_WriteByte(base, *(pState->pTxData++));
        }
        else
        {
            LPUART_DisableInterrupts(base, (uint32_t)kLPUART_TxDataRegEmptyInterruptEnable);

            if( NULL != pState->txCb )
            {
                pState->txCb(pState);
            }
        }
    }

    if( (bool)((uint32_t)kLPUART_RxOverrunFlag & status) )
    {
        (void)LPUART_ClearStatusFlags(base, (uint32_t)kLPUART_RxOverrunFlag);
    }
    if( (bool)((uint32_t)kLPUART_RxActiveEdgeFlag & status) )
    {
        (void)LPUART_ClearStatusFlags(base, (uint32_t)kLPUART_RxActiveEdgeFlag);
    }
}

#if FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1
void LPUART0_LPUART1_IRQHandler(void)
{
    const clock_ip_name_t lpuartClock[] = LPUART_CLOCKS;
    const LPUART_Type* lpuartBase[] = LPUART_BASE_PTRS;

    for (uint32_t i = 0; i < (sizeof(lpuartClock) / sizeof(clock_ip_name_t)); i++)
    {
        if (CLOCK_isEnabledClock(lpuartClock[i]))
        {
            if ((bool)(LPUART_STAT_OR_MASK & lpuartBase[i]->STAT) ||
                ((bool)(LPUART_STAT_RDRF_MASK & lpuartBase[i]->STAT) && (bool)(LPUART_CTRL_RIE_MASK & lpuartBase[i]->CTRL)) ||
                ((bool)(lpuartBase[i]->STAT & LPUART_STAT_TDRE_MASK) && (bool)(lpuartBase[i]->CTRL & LPUART_CTRL_TIE_MASK)))
            {
                 LPUART_Common_ISR(i);
            }
        }
    }
}
#else

#if defined(LPUART0)
void LPUART0_IRQHandler(void)
{
    LPUART_Common_ISR(0);
}
#endif

#if defined(LPUART1)
void LPUART1_IRQHandler(void)
{
    LPUART_Common_ISR(1);
}
#endif

#endif /* #if FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1 */

#if defined(LPUART2)
void LPUART2_IRQHandler(void)
{
    LPUART_Common_ISR(2);
}
#endif

#if defined(LPUART3)
void LPUART3_IRQHandler(void)
{
    LPUART_Common_ISR(3);
}
#endif

#if defined(LPUART4)
void LPUART4_IRQHandler(void)
{
    LPUART_Common_ISR(4);
}
#endif

#if defined(LPUART5)
void LPUART5_IRQHandler(void)
{
    LPUART_Common_ISR(5);
}
#endif
#endif /* #if (defined(FSL_FEATURE_SOC_LPUART_COUNT)&&(FSL_FEATURE_SOC_LPUART_COUNT > 0)) */

/************************************************************************************/
#if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0))
static void UART_Common_ISR(uint32_t instance)
{
    UART_Type * base;
    uartState_t * pState;

    base = mUartBase[instance];
    pState = pUartStates[instance];
    /* Check if data was received */
    if( (kUART_RxDataRegFullFlag) & UART_GetStatusFlags(base) )
    {
        uint8_t data = UART_ReadByte(base);
        UART_ClearStatusFlags(base, kUART_RxDataRegFullFlag);

        if( pState->rxSize )
        {
            pState->rxSize--;
        }

        if( pState->pRxData )
        {
            *(pState->pRxData) = data;
            pState->pRxData++;
        }

        if( (0 == pState->rxSize) && (NULL != pState->rxCb) )
        {
            pState->rxCb(pState);
        }
    }
    /* Check if data Tx has end */
    if( (kUART_TxDataRegEmptyFlag) & UART_GetStatusFlags(base) &&
       (kUART_TxDataRegEmptyInterruptEnable) & UART_GetEnabledInterrupts(base) )
    {
        if( pState->txSize )
        {
            pState->txSize--;

            UART_WriteByte(base, *(pState->pTxData++));
        }
        else if( 0 == pState->txSize )
        {
            UART_DisableInterrupts(base, kUART_TxDataRegEmptyInterruptEnable);

            if( NULL != pState->txCb )
            {
                pState->txCb(pState);
            }
        }
    }

    if( kUART_RxOverrunFlag & UART_GetStatusFlags(base) )
    {
        UART_ClearStatusFlags(base, kUART_RxOverrunFlag);
    }
}

#if defined(UART0)
void UART0_RX_TX_IRQHandler(void)
{
    UART_Common_ISR(0);
}
#endif

#if defined(UART1)
void UART1_RX_TX_IRQHandler(void)
{
    UART_Common_ISR(1);
}
#endif

#if defined(UART2)
void UART2_RX_TX_IRQHandler(void)
{
    UART_Common_ISR(2);
}
#endif

#if defined(UART3)
void UART3_RX_TX_IRQHandler(void)
{
    UART_Common_ISR(3);
}
#endif

#if defined(UART4)
void UART4_RX_TX_IRQHandler(void)
{
    UART_Common_ISR(4);
}
#endif

#if defined(UART5)
void UART5_RX_TX_IRQHandler(void)
{
    UART_Common_ISR(5);
}
#endif
#endif /* #if (defined(FSL_FEATURE_SOC_UART_COUNT)&&(FSL_FEATURE_SOC_UART_COUNT > 0)) */

/************************************************************************************/
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT)&&(FSL_FEATURE_SOC_LPSCI_COUNT > 0))
static void LPSCI_Common_ISR(uint32_t instance)
{
    UART0_Type * base;
    uartState_t * pState;

    base = mLpsciBase[instance];
    pState = pLpsciStates[instance];
    /* Check if data was received */
    if( (kLPSCI_RxDataRegFullFlag) & LPSCI_GetStatusFlags(base) )
    {
        uint8_t data = LPSCI_ReadByte(base);
        LPSCI_ClearStatusFlags(base, kLPSCI_RxDataRegFullFlag);

        if( pState->rxSize )
        {
            pState->rxSize--;
        }

        if( pState->pRxData )
        {
            *(pState->pRxData) = data;
            pState->pRxData++;
        }

        if( (0 == pState->rxSize) && (NULL != pState->rxCb) )
        {
            pState->rxCb(pState);
        }
    }
    /* Check if data Tx has end */
    if( (kLPSCI_TxDataRegEmptyFlag) & LPSCI_GetStatusFlags(base) &&
       (kLPSCI_TxDataRegEmptyInterruptEnable) & LPSCI_GetEnabledInterrupts(base) )
    {
        if( pState->txSize )
        {
            pState->txSize--;

            LPSCI_WriteByte(base, *(pState->pTxData++));
        }
        else if( 0 == pState->txSize )
        {
            LPSCI_DisableInterrupts(base, kLPSCI_TxDataRegEmptyInterruptEnable);

            if( NULL != pState->txCb )
            {
                pState->txCb(pState);
            }
        }
    }

    if( kLPSCI_RxOverrunFlag & LPSCI_GetStatusFlags(base) )
    {
        LPSCI_ClearStatusFlags(base, kLPSCI_RxOverrunFlag);
    }
}

void UART0_IRQHandler(void)
{
    LPSCI_Common_ISR(0);
}
#endif

/************************************************************************************/
#if (defined(FSL_FEATURE_SOC_USART_COUNT)&&(FSL_FEATURE_SOC_USART_COUNT > 0))
static void USART_ISR(void)
{
    uint32_t irq = __get_IPSR() - 16;
    uint32_t instance;
    USART_Type *base;
    uartState_t *pState;

    /* Get instance */
    for (instance = 0; instance < FSL_FEATURE_SOC_USART_COUNT; instance++)
    {
        if (irq == mUsartIrqs[instance])
        {
            base = mUsartBase[instance];
            pState = pUsartStates[instance];

            /* Check if data was received */
            if ((kUSART_RxFifoNotEmptyFlag)& USART_GetStatusFlags(base))
            {
                uint8_t data = USART_ReadByte(base);

                if (pState->rxSize)
                {
                    pState->rxSize--;
                }

                if (pState->pRxData)
                {
                    *(pState->pRxData) = data;
                    pState->pRxData++;
                }

                if ((0 == pState->rxSize) && (NULL != pState->rxCb))
                {
                    pState->rxCb(pState);
                }
            }
            /* Check if data Tx has end */
            if ((kUSART_TxFifoEmptyFlag)&USART_GetStatusFlags(base) &&
                (kUSART_TxLevelInterruptEnable) & USART_GetEnabledInterrupts(base))
            {
                if (pState->txSize)
                {
                    pState->txSize--;

                    USART_WriteByte(base, *(pState->pTxData++));
                }
                else if (0 == pState->txSize)
                {
                    USART_DisableInterrupts(base, kUSART_TxLevelInterruptEnable);

                    if (NULL != pState->txCb)
                    {
                        pState->txCb(pState);
                    }
                }
            }

            if (kUSART_RxError & USART_GetStatusFlags(base))
            {
                USART_ClearStatusFlags(base, kUSART_RxError);
            }
            break;
        }
    }
}
#endif
