/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause

**************************************************************************
*
* $FileName: interrupt_vector_numbers_MK65F18.h$
* $Version :
* $Date    :
*
* Comments:
*
*
*
*END************************************************************************/

#ifndef ____INTERRUPT_VECTOR_NUMBERS_MK65F18_H__
#define ____INTERRUPT_VECTOR_NUMBERS_MK65F18_H__

/* ----------------------------------------------------------------------------
   -- Interrupt vector numbers
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Interrupt_vector_numbers Interrupt vector numbers
 * @{
 */

/** Interrupt Number Definitions */
typedef enum
{
    INT_Initial_Stack_Pointer = 0,      /**< Initial Stack Pointer */
    INT_Initial_Program_Counter = 1,    /**< Initial Program Counter */
    INT_NMI = 2,                        /**< Non-maskable Interrupt (NMI) */
    INT_Hard_Fault = 3,                 /**< Hard Fault */
    INT_Mem_Manage_Fault = 4,           /**< MemManage Fault */
    INT_Bus_Fault = 5,                  /**< Bus Fault */
    INT_Usage_Fault = 6,                /**< Usage Fault */
    INT_Reserved7 = 7,                  /**< Reserved interrupt 7 */
    INT_Reserved8 = 8,                  /**< Reserved interrupt 8 */
    INT_Reserved9 = 9,                  /**< Reserved interrupt 9 */
    INT_Reserved10 = 10,                /**< Reserved interrupt 10 */
    INT_SVCall = 11,                    /**< Supervisor call (SVCall) */
    INT_DebugMonitor = 12,              /**< Debug Monitor */
    INT_Reserved13 = 13,                /**< Reserved interrupt 13 */
    INT_PendableSrvReq = 14,            /**< Pendable request for system service (PendableSrvReq) */
    INT_SysTick = 15,                   /**< SysTick Interrupt */
    INT_DMA0_DMA16 = 16,                /**< DMA Channel 0, 16 Transfer Complete */
    INT_DMA1_DMA17 = 17,                /**< DMA Channel 1, 17 Transfer Complete */
    INT_DMA2_DMA18 = 18,                /**< DMA Channel 2, 18 Transfer Complete */
    INT_DMA3_DMA19 = 19,                /**< DMA Channel 3, 19 Transfer Complete */
    INT_DMA4_DMA20 = 20,                /**< DMA Channel 4, 20 Transfer Complete */
    INT_DMA5_DMA21 = 21,                /**< DMA Channel 5, 21 Transfer Complete */
    INT_DMA6_DMA22 = 22,                /**< DMA Channel 6, 22 Transfer Complete */
    INT_DMA7_DMA23 = 23,                /**< DMA Channel 7, 23 Transfer Complete */
    INT_DMA8_DMA24 = 24,                /**< DMA Channel 8, 24 Transfer Complete */
    INT_DMA9_DMA25 = 25,                /**< DMA Channel 9, 25 Transfer Complete */
    INT_DMA10_DMA26 = 26,               /**< DMA Channel 10, 26 Transfer Complete */
    INT_DMA11_DMA27 = 27,               /**< DMA Channel 11, 27 Transfer Complete */
    INT_DMA12_DMA28 = 28,               /**< DMA Channel 12, 28 Transfer Complete */
    INT_DMA13_DMA29 = 29,               /**< DMA Channel 13, 29 Transfer Complete */
    INT_DMA14_DMA30 = 30,               /**< DMA Channel 14, 30 Transfer Complete */
    INT_DMA15_DMA31 = 31,               /**< DMA Channel 15, 31 Transfer Complete */
    INT_DMA_Error = 32,                 /**< DMA Error Interrupt */
    INT_MCM = 33,                       /**< Normal Interrupt */
    INT_FTFE = 34,                      /**< FTFE Command complete interrupt */
    INT_Read_Collision = 35,            /**< Read Collision Interrupt */
    INT_LVD_LVW = 36,                   /**< Low Voltage Detect, Low Voltage Warning */
    INT_LLW = 37,                       /**< Low Leakage Wakeup */
    INT_Watchdog = 38,                  /**< WDOG Interrupt */
    INT_RNG = 39,                       /**< RNG Interrupt */
    INT_I2C0 = 40,                      /**< I2C0 interrupt */
    INT_I2C1 = 41,                      /**< I2C1 interrupt */
    INT_SPI0 = 42,                      /**< SPI0 Interrupt */
    INT_SPI1 = 43,                      /**< SPI1 Interrupt */
    INT_I2S0_Tx = 44,                   /**< I2S0 transmit interrupt */
    INT_I2S0_Rx = 45,                   /**< I2S0 receive interrupt */
    INT_UART0_LON = 46,                 /**< UART0 LON interrupt */
    INT_UART0_RX_TX = 47,               /**< UART0 Receive/Transmit interrupt */
    INT_UART0_ERR = 48,                 /**< UART0 Error interrupt */
    INT_UART1_RX_TX = 49,               /**< UART1 Receive/Transmit interrupt */
    INT_UART1_ERR = 50,                 /**< UART1 Error interrupt */
    INT_UART2_RX_TX = 51,               /**< UART2 Receive/Transmit interrupt */
    INT_UART2_ERR = 52,                 /**< UART2 Error interrupt */
    INT_UART3_RX_TX = 53,               /**< UART3 Receive/Transmit interrupt */
    INT_UART3_ERR = 54,                 /**< UART3 Error interrupt */
    INT_ADC0 = 55,                      /**< ADC0 interrupt */
    INT_CMP0 = 56,                      /**< CMP0 interrupt */
    INT_CMP1 = 57,                      /**< CMP1 interrupt */
    INT_FTM0 = 58,                      /**< FTM0 fault, overflow and channels interrupt */
    INT_FTM1 = 59,                      /**< FTM1 fault, overflow and channels interrupt */
    INT_FTM2 = 60,                      /**< FTM2 fault, overflow and channels interrupt */
    INT_CMT = 61,                       /**< CMT interrupt */
    INT_RTC = 62,                       /**< RTC interrupt */
    INT_RTC_Seconds = 63,               /**< RTC seconds interrupt */
    INT_PIT0 = 64,                      /**< PIT timer channel 0 interrupt */
    INT_PIT1 = 65,                      /**< PIT timer channel 1 interrupt */
    INT_PIT2 = 66,                      /**< PIT timer channel 2 interrupt */
    INT_PIT3 = 67,                      /**< PIT timer channel 3 interrupt */
    INT_PDB0 = 68,                      /**< PDB0 Interrupt */
    INT_USB0 = 69,                      /**< USB0 interrupt */
    INT_USBDCD = 70,                    /**< USBDCD Interrupt */
    INT_Reserved71 = 71,                /**< Reserved interrupt 71 */
    INT_DAC0 = 72,                      /**< DAC0 interrupt */
    INT_MCG = 73,                       /**< MCG Interrupt */
    INT_LPTimer = 74,                   /**< LPTimer interrupt */
    INT_PORTA = 75,                     /**< Port A interrupt */
    INT_PORTB = 76,                     /**< Port B interrupt */
    INT_PORTC = 77,                     /**< Port C interrupt */
    INT_PORTD = 78,                     /**< Port D interrupt */
    INT_PORTE = 79,                     /**< Port E interrupt */
    INT_SWI = 80,                       /**< Software interrupt */
    INT_SPI2 = 81,                      /**< SPI2 Interrupt */
    INT_UART4_RX_TX = 82,               /**< UART4 Receive/Transmit interrupt */
    INT_UART4_ERR = 83,                 /**< UART4 Error interrupt */
    INT_Reserved84 = 84,                /**< Reserved interrupt 84 */
    INT_Reserved85 = 85,                /**< Reserved interrupt 85 */
    INT_CMP2 = 86,                      /**< CMP2 interrupt */
    INT_FTM3 = 87,                      /**< FTM3 fault, overflow and channels interrupt */
    INT_DAC1 = 88,                      /**< DAC1 interrupt */
    INT_ADC1 = 89,                      /**< ADC1 interrupt */
    INT_I2C2 = 90,                      /**< I2C2 interrupt */
    INT_CAN0_ORed_Message_buffer = 91,  /**< CAN0 OR'd message buffers interrupt */
    INT_CAN0_Bus_Off = 92,              /**< CAN0 bus off interrupt */
    INT_CAN0_Error = 93,                /**< CAN0 error interrupt */
    INT_CAN0_Tx_Warning = 94,           /**< CAN0 Tx warning interrupt */
    INT_CAN0_Rx_Warning = 95,           /**< CAN0 Rx warning interrupt */
    INT_CAN0_Wake_Up = 96,              /**< CAN0 wake up interrupt */
    INT_SDHC = 97,                      /**< SDHC interrupt */
    INT_ENET_1588_Timer = 98,           /**< Ethernet MAC IEEE 1588 Timer Interrupt */
    INT_ENET_Transmit = 99,             /**< Ethernet MAC Transmit Interrupt */
    INT_ENET_Receive = 100,             /**< Ethernet MAC Receive Interrupt */
    INT_ENET_Error = 101,               /**< Ethernet MAC Error and miscelaneous Interrupt */
    INT_LPUART0 = 102,                  /**< LPUART0 status/error interrupt */
    INT_TSI0 = 103,                     /**< TSI0 interrupt */
    INT_TPM1 = 104,                     /**< TPM1 fault, overflow and channels interrupt */
    INT_TPM2 = 105,                     /**< TPM2 fault, overflow and channels interrupt */
    INT_USBHSDCD = 106,                 /**< USBHSDCD, USBHS Phy Interrupt */
    INT_I2C3 = 107,                     /**< I2C3 interrupt */
    INT_CMP3 = 108,                     /**< CMP3 interrupt */
    INT_USBHS = 109,                    /**< USB high speed OTG interrupt */
    INT_CAN1_ORed_Message_buffer = 110, /**< CAN1 OR'd message buffers interrupt */
    INT_CAN1_Bus_Off = 111,             /**< CAN1 bus off interrupt */
    INT_CAN1_Error = 112,               /**< CAN1 error interrupt */
    INT_CAN1_Tx_Warning = 113,          /**< CAN1 Tx warning interrupt */
    INT_CAN1_Rx_Warning = 114,          /**< CAN1 Rx warning interrupt */
    INT_CAN1_Wake_Up = 115              /**< CAN1 wake up interrupt */
} IRQInterruptIndex;

/* ----------------------------------------------------------------------------
   -- Interrupt vector numbers
   ---------------------------------------------------------------------------- */

#endif
