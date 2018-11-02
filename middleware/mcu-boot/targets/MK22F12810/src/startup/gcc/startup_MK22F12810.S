/* ------------------------------------------------------------------------- */
/*  @file:    startup_MK22F12810.s                                           */
/*  @purpose: CMSIS Cortex-M4 Core Device Startup File                       */
/*            MK22F12810                                                     */
/*  @version: 1.6                                                            */
/*  @date:    2015-2-19                                                      */
/*  @build:   b171205                                                        */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Copyright (c) 1997-2015 Freescale Semiconductor, Inc.                     */
/* Copyright 2016-2018 NXP                                                   */
/* All rights reserved.                                                      */
/*                                                                           */
/* SPDX-License-Identifier: BSD-3-Clause                                     */
/*****************************************************************************/
/* Version: GCC for ARM Embedded Processors                                  */
/*****************************************************************************/

    .syntax unified
    .arch armv7-m

/*
    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x00000400
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x00000C00
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .if    Heap_Size
    .space    Heap_Size
    .endif
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit
*/

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                  /* Top of Stack */
    .long   Reset_Handler               /* Reset Handler */
    .long   NMI_Handler                 /* NMI Handler                  */
    .long   HardFault_Handler           /* Hard Fault Handler           */
    .long   MemManage_Handler           /* MPU Fault Handler            */
    .long   BusFault_Handler            /* Bus Fault Handler            */
    .long   UsageFault_Handler          /* Usage Fault Handler          */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   SVC_Handler                 /* SVCall Handler               */
    .long   DebugMon_Handler            /* Debug Monitor Handler        */
    .long   0                           /* Reserved                     */
    .long   PendSV_Handler              /* PendSV Handler               */
    .long   SysTick_Handler             /* SysTick Handler              */

    /* External Interrupts */
    .long   DMA0_IRQHandler  /* DMA Channel 0 Transfer Complete */
    .long   DMA1_IRQHandler  /* DMA Channel 1 Transfer Complete */
    .long   DMA2_IRQHandler  /* DMA Channel 2 Transfer Complete */
    .long   DMA3_IRQHandler  /* DMA Channel 3 Transfer Complete */
    .long   Reserved20_IRQHandler  /* Reserved interrupt 20 */
    .long   Reserved21_IRQHandler  /* Reserved interrupt 21 */
    .long   Reserved22_IRQHandler  /* Reserved interrupt 22 */
    .long   Reserved23_IRQHandler  /* Reserved interrupt 23 */
    .long   Reserved24_IRQHandler  /* Reserved interrupt 24 */
    .long   Reserved25_IRQHandler  /* Reserved interrupt 25 */
    .long   Reserved26_IRQHandler  /* Reserved interrupt 26 */
    .long   Reserved27_IRQHandler  /* Reserved interrupt 27 */
    .long   Reserved28_IRQHandler  /* Reserved interrupt 28 */
    .long   Reserved29_IRQHandler  /* Reserved interrupt 29 */
    .long   Reserved30_IRQHandler  /* Reserved interrupt 30 */
    .long   Reserved31_IRQHandler  /* Reserved interrupt 31 */
    .long   DMA_Error_IRQHandler  /* DMA Error Interrupt */
    .long   MCM_IRQHandler  /* Normal Interrupt */
    .long   FTF_IRQHandler  /* FTFA Command complete interrupt */
    .long   Read_Collision_IRQHandler  /* Read Collision Interrupt */
    .long   LVD_LVW_IRQHandler  /* Low Voltage Detect, Low Voltage Warning */
    .long   LLW_IRQHandler  /* Low Leakage Wakeup */
    .long   Watchdog_IRQHandler  /* WDOG Interrupt */
    .long   Reserved39_IRQHandler  /* Reserved Interrupt 39 */
    .long   I2C0_IRQHandler  /* I2C0 interrupt */
    .long   I2C1_IRQHandler  /* I2C1 interrupt */
    .long   SPI0_IRQHandler  /* SPI0 Interrupt */
    .long   SPI1_IRQHandler  /* SPI1 Interrupt */
    .long   I2S0_Tx_IRQHandler  /* I2S0 transmit interrupt */
    .long   I2S0_Rx_IRQHandler  /* I2S0 receive interrupt */
    .long   LPUART0_IRQHandler  /* LPUART0 status/error interrupt */
    .long   UART0_RX_TX_IRQHandler  /* UART0 Receive/Transmit interrupt */
    .long   UART0_ERR_IRQHandler  /* UART0 Error interrupt */
    .long   UART1_RX_TX_IRQHandler  /* UART1 Receive/Transmit interrupt */
    .long   UART1_ERR_IRQHandler  /* UART1 Error interrupt */
    .long   UART2_RX_TX_IRQHandler  /* UART2 Receive/Transmit interrupt */
    .long   UART2_ERR_IRQHandler  /* UART2 Error interrupt */
    .long   Reserved53_IRQHandler  /* Reserved interrupt 53 */
    .long   Reserved54_IRQHandler  /* Reserved interrupt 54 */
    .long   ADC0_IRQHandler  /* ADC0 interrupt */
    .long   CMP0_IRQHandler  /* CMP0 interrupt */
    .long   CMP1_IRQHandler  /* CMP1 interrupt */
    .long   FTM0_IRQHandler  /* FTM0 fault, overflow and channels interrupt */
    .long   FTM1_IRQHandler  /* FTM1 fault, overflow and channels interrupt */
    .long   FTM2_IRQHandler  /* FTM2 fault, overflow and channels interrupt */
    .long   Reserved61_IRQHandler  /* Reserved interrupt 61 */
    .long   RTC_IRQHandler  /* RTC interrupt */
    .long   RTC_Seconds_IRQHandler  /* RTC seconds interrupt */
    .long   PIT0_IRQHandler  /* PIT timer channel 0 interrupt */
    .long   PIT1_IRQHandler  /* PIT timer channel 1 interrupt */
    .long   PIT2_IRQHandler  /* PIT timer channel 2 interrupt */
    .long   PIT3_IRQHandler  /* PIT timer channel 3 interrupt */
    .long   PDB0_IRQHandler  /* PDB0 Interrupt */
    .long   USB0_IRQHandler  /* USB0 interrupt */
    .long   Reserved70_IRQHandler  /* Reserved interrupt 70 */
    .long   Reserved71_IRQHandler  /* Reserved interrupt 71 */
    .long   DAC0_IRQHandler  /* DAC0 interrupt */
    .long   MCG_IRQHandler  /* MCG Interrupt */
    .long   LPTimer_IRQHandler  /* LPTimer interrupt */
    .long   PORTA_IRQHandler  /* Port A interrupt */
    .long   PORTB_IRQHandler  /* Port B interrupt */
    .long   PORTC_IRQHandler  /* Port C interrupt */
    .long   PORTD_IRQHandler  /* Port D interrupt */
    .long   PORTE_IRQHandler  /* Port E interrupt */
    .long   SWI_IRQHandler  /* Software interrupt */
    .long   Reserved81_IRQHandler  /* Reserved interrupt 81 */
    .long   Reserved82_IRQHandler  /* Reserved interrupt 82 */
    .long   Reserved83_IRQHandler  /* Reserved interrupt 83 */
    .long   Reserved84_IRQHandler  /* Reserved interrupt 84 */
    .long   Reserved85_IRQHandler  /* Reserved interrupt 85 */
    .long   Reserved86_IRQHandler  /* Reserved interrupt 86 */
    .long   Reserved87_IRQHandler  /* Reserved interrupt 87 */
    .long   Reserved88_IRQHandler  /* Reserved interrupt 88 */
    .long   ADC1_IRQHandler  /* ADC1 interrupt */
    .long   Reserved90_IRQHandler  /* Reserved Interrupt 90 */
    .long   Reserved91_IRQHandler  /* Reserved Interrupt 91 */
    .long   Reserved92_IRQHandler  /* Reserved Interrupt 92 */
    .long   Reserved93_IRQHandler  /* Reserved Interrupt 93 */
    .long   Reserved94_IRQHandler  /* Reserved Interrupt 94 */
    .long   Reserved95_IRQHandler  /* Reserved Interrupt 95 */
    .long   Reserved96_IRQHandler  /* Reserved Interrupt 96 */
    .long   Reserved97_IRQHandler  /* Reserved Interrupt 97 */
    .long   Reserved98_IRQHandler  /* Reserved Interrupt 98 */
    .long   Reserved99_IRQHandler  /* Reserved Interrupt 99 */
    .long   Reserved100_IRQHandler  /* Reserved Interrupt 100 */
    .long   Reserved101_IRQHandler  /* Reserved Interrupt 101 */
    .long   DefaultISR  /* 102 */
    .long   DefaultISR  /* 103 */
    .long   DefaultISR  /* 104 */
    .long   DefaultISR  /* 105 */
    .long   DefaultISR  /* 106 */
    .long   DefaultISR  /* 107 */
    .long   DefaultISR  /* 108 */
    .long   DefaultISR  /* 109 */
    .long   DefaultISR  /* 110 */
    .long   DefaultISR  /* 111 */
    .long   DefaultISR  /* 112 */
    .long   DefaultISR  /* 113 */
    .long   DefaultISR  /* 114 */
    .long   DefaultISR  /* 115 */
    .long   DefaultISR  /* 116 */
    .long   DefaultISR  /* 117 */
    .long   DefaultISR  /* 118 */
    .long   DefaultISR  /* 119 */
    .long   DefaultISR  /* 120 */
    .long   DefaultISR  /* 121 */
    .long   DefaultISR  /* 122 */
    .long   DefaultISR  /* 123 */
    .long   DefaultISR  /* 124 */
    .long   DefaultISR  /* 125 */
    .long   DefaultISR  /* 126 */
    .long   DefaultISR  /* 127 */
    .long   DefaultISR  /* 128 */
    .long   DefaultISR  /* 129 */
    .long   DefaultISR  /* 130 */
    .long   DefaultISR  /* 131 */
    .long   DefaultISR  /* 132 */
    .long   DefaultISR  /* 133 */
    .long   DefaultISR  /* 134 */
    .long   DefaultISR  /* 135 */
    .long   DefaultISR  /* 136 */
    .long   DefaultISR  /* 137 */
    .long   DefaultISR  /* 138 */
    .long   DefaultISR  /* 139 */
    .long   DefaultISR  /* 140 */
    .long   DefaultISR  /* 141 */
    .long   DefaultISR  /* 142 */
    .long   DefaultISR  /* 143 */
    .long   DefaultISR  /* 144 */
    .long   DefaultISR  /* 145 */
    .long   DefaultISR  /* 146 */
    .long   DefaultISR  /* 147 */
    .long   DefaultISR  /* 148 */
    .long   DefaultISR  /* 149 */
    .long   DefaultISR  /* 150 */
    .long   DefaultISR  /* 151 */
    .long   DefaultISR  /* 152 */
    .long   DefaultISR  /* 153 */
    .long   DefaultISR  /* 154 */
    .long   DefaultISR  /* 155 */
    .long   DefaultISR  /* 156 */
    .long   DefaultISR  /* 157 */
    .long   DefaultISR  /* 158 */
    .long   DefaultISR  /* 159 */
    .long   DefaultISR  /* 160 */
    .long   DefaultISR  /* 161 */
    .long   DefaultISR  /* 162 */
    .long   DefaultISR  /* 163 */
    .long   DefaultISR  /* 164 */
    .long   DefaultISR  /* 165 */
    .long   DefaultISR  /* 166 */
    .long   DefaultISR  /* 167 */
    .long   DefaultISR  /* 168 */
    .long   DefaultISR  /* 169 */
    .long   DefaultISR  /* 170 */
    .long   DefaultISR  /* 171 */
    .long   DefaultISR  /* 172 */
    .long   DefaultISR  /* 173 */
    .long   DefaultISR  /* 174 */
    .long   DefaultISR  /* 175 */
    .long   DefaultISR  /* 176 */
    .long   DefaultISR  /* 177 */
    .long   DefaultISR  /* 178 */
    .long   DefaultISR  /* 179 */
    .long   DefaultISR  /* 180 */
    .long   DefaultISR  /* 181 */
    .long   DefaultISR  /* 182 */
    .long   DefaultISR  /* 183 */
    .long   DefaultISR  /* 184 */
    .long   DefaultISR  /* 185 */
    .long   DefaultISR  /* 186 */
    .long   DefaultISR  /* 187 */
    .long   DefaultISR  /* 188 */
    .long   DefaultISR  /* 189 */
    .long   DefaultISR  /* 190 */
    .long   DefaultISR  /* 191 */
    .long   DefaultISR  /* 192 */
    .long   DefaultISR  /* 193 */
    .long   DefaultISR  /* 194 */
    .long   DefaultISR  /* 195 */
    .long   DefaultISR  /* 196 */
    .long   DefaultISR  /* 197 */
    .long   DefaultISR  /* 198 */
    .long   DefaultISR  /* 199 */
    .long   DefaultISR  /* 200 */
    .long   DefaultISR  /* 201 */
    .long   DefaultISR  /* 202 */
    .long   DefaultISR  /* 203 */
    .long   DefaultISR  /* 204 */
    .long   DefaultISR  /* 205 */
    .long   DefaultISR  /* 206 */
    .long   DefaultISR  /* 207 */
    .long   DefaultISR  /* 208 */
    .long   DefaultISR  /* 209 */
    .long   DefaultISR  /* 210 */
    .long   DefaultISR  /* 211 */
    .long   DefaultISR  /* 212 */
    .long   DefaultISR  /* 213 */
    .long   DefaultISR  /* 214 */
    .long   DefaultISR  /* 215 */
    .long   DefaultISR  /* 216 */
    .long   DefaultISR  /* 217 */
    .long   DefaultISR  /* 218 */
    .long   DefaultISR  /* 219 */
    .long   DefaultISR  /* 220 */
    .long   DefaultISR  /* 221 */
    .long   DefaultISR  /* 222 */
    .long   DefaultISR  /* 223 */
    .long   DefaultISR  /* 224 */
    .long   DefaultISR  /* 225 */
    .long   DefaultISR  /* 226 */
    .long   DefaultISR  /* 227 */
    .long   DefaultISR  /* 228 */
    .long   DefaultISR  /* 229 */
    .long   DefaultISR  /* 230 */
    .long   DefaultISR  /* 231 */
    .long   DefaultISR  /* 232 */
    .long   DefaultISR  /* 233 */
    .long   DefaultISR  /* 234 */
    .long   DefaultISR  /* 235 */
    .long   DefaultISR  /* 236 */
    .long   DefaultISR  /* 237 */
    .long   DefaultISR  /* 238 */
    .long   DefaultISR  /* 239 */

#ifdef BL_HAS_BOOTLOADER_CONFIG

//__bootloaderConfigurationArea ; 0x3c0
//    .long     'kcfg'        // [00:03] tag - Tag value used to validate the bootloader configuration data. Must be set to 'kcfg'
        .long     0x6766636b
        .long     0xFFFFFFFF    // [04:07] crcStartAddress
        .long     0xFFFFFFFF    // [08:0b] crcByteCount
        .long     0xFFFFFFFF    // [0c:0f] crcExpectedValue
        .byte     0xFF          // [10:10] enabledPeripherals
        .byte     0xFF          // [11:11] i2cSlaveAddress
        .short    5000          // [12:13] peripheralDetectionTimeoutMs - Timeout in milliseconds for peripheral detection before jumping to application code
        .short     0xFFFF       // [14:15] usbVid
        .short     0xFFFF       // [16:17] usbPid
        .long     0xFFFFFFFF    // [18:1b] usbStringsPointer
        .byte     0xFF          // [1c:1c] clockFlags - High Speed and other clock options
        .byte     0xFF          // [1d:1d] clockDivider - One's complement of clock divider, zero divider is divide by 1
        .short     0xFFFF        // [1e:1f] reserved
        // Fill to align with flash configuration field.
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF        // Reserved for user TRIM value
#else        
         //Fill to align with flash configuration field.
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF
        .long     0xFFFFFFFF        
        .long     0xFFFFFFFF      // Reserved for user TRIM value
#endif  // BL_HAS_BOOTLOADER_CONFIG

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
  	.long	0xFFFFFFFF
  	.long	0xFFFFFFFF
  	.long	0xFFFFFFFF
  	.long	0xFFFFFFFE

    .text
    .thumb

    .weak   init_data_bss
    .type   init_data_bss, %function
init_data_bss:
    bx     r14

/* Reset Handler */

    .text
    .thumb
    .thumb_func
    .align 2
    .globl   Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
#ifndef __NO_SYSTEM_INIT
    bl    SystemInit
#endif
#ifndef __NO_INIT_DATA_BSS
    bl    init_data_bss
#endif

/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

#if 1
/* Here are two copies of loop implemenations. First one favors code size
 * and the second one favors performance. Default uses the first one.
 * Change to "#if 0" to use the second one */
.LC0:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt    .LC0
#else
    subs    r3, r2
    ble    .LC1
.LC0:
    subs    r3, #4
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    bgt    .LC0
.LC1:
#endif

#ifdef __STARTUP_CLEAR_BSS
/*     This part of work usually is done in C library startup code. Otherwise,
 *     define this macro to enable it in this startup.
 *
 *     Loop to zero out BSS section, which uses following symbols
 *     in linker script:
 *      __bss_start__: start of BSS section. Must align to 4
 *      __bss_end__: end of BSS section. Must align to 4
 */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__

    movs    r0, 0
.LC2:
    cmp     r1, r2
    itt    lt
    strlt   r0, [r1], #4
    blt    .LC2
#endif /* __STARTUP_CLEAR_BSS */

#ifndef __START
#define __START main
#endif
    bl    __START
    .pool
    .size Reset_Handler, . - Reset_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_irq_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .size    \handler_name, . - \handler_name
    .endm

/* Exception Handlers */

    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    MemManage_Handler
    def_irq_handler    BusFault_Handler
    def_irq_handler    UsageFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    DebugMon_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler
    def_irq_handler    Default_Handler

/* IRQ Handlers */
    def_irq_handler     DMA0_IRQHandler
    def_irq_handler     DMA1_IRQHandler
    def_irq_handler     DMA2_IRQHandler
    def_irq_handler     DMA3_IRQHandler
    def_irq_handler     Reserved20_IRQHandler
    def_irq_handler     Reserved21_IRQHandler
    def_irq_handler     Reserved22_IRQHandler
    def_irq_handler     Reserved23_IRQHandler
    def_irq_handler     Reserved24_IRQHandler
    def_irq_handler     Reserved25_IRQHandler
    def_irq_handler     Reserved26_IRQHandler
    def_irq_handler     Reserved27_IRQHandler
    def_irq_handler     Reserved28_IRQHandler
    def_irq_handler     Reserved29_IRQHandler
    def_irq_handler     Reserved30_IRQHandler
    def_irq_handler     Reserved31_IRQHandler
    def_irq_handler     DMA_Error_IRQHandler
    def_irq_handler     MCM_IRQHandler
    def_irq_handler     FTF_IRQHandler
    def_irq_handler     Read_Collision_IRQHandler
    def_irq_handler     LVD_LVW_IRQHandler
    def_irq_handler     LLW_IRQHandler
    def_irq_handler     Watchdog_IRQHandler
    def_irq_handler     Reserved39_IRQHandler
    def_irq_handler     I2C0_IRQHandler
    def_irq_handler     I2C1_IRQHandler
    def_irq_handler     SPI0_IRQHandler
    def_irq_handler     SPI1_IRQHandler
    def_irq_handler     I2S0_Tx_IRQHandler
    def_irq_handler     I2S0_Rx_IRQHandler
    def_irq_handler     LPUART0_IRQHandler
    def_irq_handler     UART0_RX_TX_IRQHandler
    def_irq_handler     UART0_ERR_IRQHandler
    def_irq_handler     UART1_RX_TX_IRQHandler
    def_irq_handler     UART1_ERR_IRQHandler
    def_irq_handler     UART2_RX_TX_IRQHandler
    def_irq_handler     UART2_ERR_IRQHandler
    def_irq_handler     Reserved53_IRQHandler
    def_irq_handler     Reserved54_IRQHandler
    def_irq_handler     ADC0_IRQHandler
    def_irq_handler     CMP0_IRQHandler
    def_irq_handler     CMP1_IRQHandler
    def_irq_handler     FTM0_IRQHandler
    def_irq_handler     FTM1_IRQHandler
    def_irq_handler     FTM2_IRQHandler
    def_irq_handler     Reserved61_IRQHandler
    def_irq_handler     RTC_IRQHandler
    def_irq_handler     RTC_Seconds_IRQHandler
    def_irq_handler     PIT0_IRQHandler
    def_irq_handler     PIT1_IRQHandler
    def_irq_handler     PIT2_IRQHandler
    def_irq_handler     PIT3_IRQHandler
    def_irq_handler     PDB0_IRQHandler
    def_irq_handler     USB0_IRQHandler
    def_irq_handler     Reserved70_IRQHandler
    def_irq_handler     Reserved71_IRQHandler
    def_irq_handler     DAC0_IRQHandler
    def_irq_handler     MCG_IRQHandler
    def_irq_handler     LPTimer_IRQHandler
    def_irq_handler     PORTA_IRQHandler
    def_irq_handler     PORTB_IRQHandler
    def_irq_handler     PORTC_IRQHandler
    def_irq_handler     PORTD_IRQHandler
    def_irq_handler     PORTE_IRQHandler
    def_irq_handler     SWI_IRQHandler
    def_irq_handler     Reserved81_IRQHandler
    def_irq_handler     Reserved82_IRQHandler
    def_irq_handler     Reserved83_IRQHandler
    def_irq_handler     Reserved84_IRQHandler
    def_irq_handler     Reserved85_IRQHandler
    def_irq_handler     Reserved86_IRQHandler
    def_irq_handler     Reserved87_IRQHandler
    def_irq_handler     Reserved88_IRQHandler
    def_irq_handler     ADC1_IRQHandler
    def_irq_handler     Reserved90_IRQHandler
    def_irq_handler     Reserved91_IRQHandler
    def_irq_handler     Reserved92_IRQHandler
    def_irq_handler     Reserved93_IRQHandler
    def_irq_handler     Reserved94_IRQHandler
    def_irq_handler     Reserved95_IRQHandler
    def_irq_handler     Reserved96_IRQHandler
    def_irq_handler     Reserved97_IRQHandler
    def_irq_handler     Reserved98_IRQHandler
    def_irq_handler     Reserved99_IRQHandler
    def_irq_handler     Reserved100_IRQHandler
    def_irq_handler     Reserved101_IRQHandler
    def_irq_handler     DefaultISR

    .end
