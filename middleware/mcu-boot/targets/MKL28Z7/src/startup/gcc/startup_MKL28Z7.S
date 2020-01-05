/* ------------------------------------------------------------------------- */
/*  @file:    startup_MKL28Z7.s                                              */
/*  @purpose: CMSIS Cortex-M0P Core Device Startup File                      */
/*            MKL28Z7                                                        */
/*  @version: 1.12                                                           */
/*  @date:    2015-6-3                                                       */
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
    .arch armv6-m

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                                      /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   DMA0_04_IRQHandler                              /* DMA0 channel 0/4 transfer complete*/
    .long   DMA0_15_IRQHandler                              /* DMA0 channel 1/5 transfer complete*/
    .long   DMA0_26_IRQHandler                              /* DMA0 channel 2/6 transfer complete*/
    .long   DMA0_37_IRQHandler                              /* DMA0 channel 3/7 transfer complete*/
    .long   CTI0_DMA0_Error_IRQHandler                      /* CTI0 or DMA0 error*/
    .long   FLEXIO0_IRQHandler                              /* FLEXIO0*/
    .long   TPM0_IRQHandler                                 /* TPM0 single interrupt vector for all sources*/
    .long   TPM1_IRQHandler                                 /* TPM1 single interrupt vector for all sources*/
    .long   TPM2_IRQHandler                                 /* TPM2 single interrupt vector for all sources*/
    .long   LPIT0_IRQHandler                                /* LPIT0 interrupt*/
    .long   LPSPI0_IRQHandler                               /* LPSPI0 single interrupt vector for all sources*/
    .long   LPSPI1_IRQHandler                               /* LPSPI1 single interrupt vector for all sources*/
    .long   LPUART0_IRQHandler                              /* LPUART0 status and error*/
    .long   LPUART1_IRQHandler                              /* LPUART1 status and error*/
    .long   LPI2C0_IRQHandler                               /* LPI2C0 interrupt*/
    .long   LPI2C1_IRQHandler                               /* LPI2C1 interrupt*/
    .long   Reserved32_IRQHandler                           /* Reserved interrupt*/
    .long   PORTA_IRQHandler                                /* PORTA Pin detect*/
    .long   PORTB_IRQHandler                                /* PORTB Pin detect*/
    .long   PORTC_IRQHandler                                /* PORTC Pin detect*/
    .long   PORTD_IRQHandler                                /* PORTD Pin detect*/
    .long   PORTE_IRQHandler                                /* PORTE Pin detect*/
    .long   LLWU0_IRQHandler                                /* Low leakage wakeup 0*/
    .long   I2S0_IRQHandler                                 /* I2S0 interrupt*/
    .long   USB0_IRQHandler                                 /* USB0 interrupt*/
    .long   ADC0_IRQHandler                                 /* ADC0 interrupt*/
    .long   LPTMR0_IRQHandler                               /* LPTMR0 interrupt*/
    .long   RTC_Seconds_IRQHandler                          /* RTC seconds*/
    .long   INTMUX0_0_IRQHandler                            /* INTMUX0 channel 0 interrupt*/
    .long   INTMUX0_1_IRQHandler                            /* INTMUX0 channel 1 interrupt*/
    .long   INTMUX0_2_IRQHandler                            /* INTMUX0 channel 2 interrupt*/
    .long   INTMUX0_3_IRQHandler                            /* INTMUX0 channel 3 interrupt*/
    .long   LPTMR1_IRQHandler                               /* LPTMR1 interrupt  (INTMUX source IRQ0)*/
    .long   Reserved49_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved50_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved51_IRQHandler                           /* Reserved interrupt*/
    .long   LPSPI2_IRQHandler                               /* LPSPI2 single interrupt vector for all sources (INTMUX source IRQ4)*/
    .long   LPUART2_IRQHandler                              /* LPUART2 status and error (INTMUX source IRQ5)*/
    .long   EMVSIM0_IRQHandler                              /* EMVSIM0 interrupt (INTMUX source IRQ6)*/
    .long   LPI2C2_IRQHandler                               /* LPI2C2 interrupt (INTMUX source IRQ7)*/
    .long   TSI0_IRQHandler                                 /* TSI0 interrupt (INTMUX source IRQ8)*/
    .long   PMC_IRQHandler                                  /* PMC interrupt (INTMUX source IRQ9)*/
    .long   FTFA_IRQHandler                                 /* FTFA interrupt (INTMUX source IRQ10)*/
    .long   SCG_IRQHandler                                  /* SCG interrupt (INTMUX source IRQ11)*/
    .long   WDOG0_IRQHandler                                /* WDOG0 interrupt (INTMUX source IRQ12)*/
    .long   DAC0_IRQHandler                                 /* DAC0 interrupt (INTMUX source IRQ13)*/
    .long   TRNG_IRQHandler                                 /* TRNG interrupt (INTMUX source IRQ14)*/
    .long   RCM_IRQHandler                                  /* RCM interrupt (INTMUX source IRQ15)*/
    .long   CMP0_IRQHandler                                 /* CMP0 interrupt (INTMUX source IRQ16)*/
    .long   CMP1_IRQHandler                                 /* CMP1 interrupt (INTMUX source IRQ17)*/
    .long   RTC_IRQHandler                                  /* RTC Alarm interrupt (INTMUX source IRQ18)*/
    .long   Reserved67_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved68_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved69_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved70_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved71_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved72_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved73_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved74_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved75_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved76_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved77_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved78_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved79_IRQHandler                           /* Reserved interrupt*/
    .long   DefaultISR                                      /* 80*/
    .long   DefaultISR                                      /* 81*/
    .long   DefaultISR                                      /* 82*/
    .long   DefaultISR                                      /* 83*/
    .long   DefaultISR                                      /* 84*/
    .long   DefaultISR                                      /* 85*/
    .long   DefaultISR                                      /* 86*/
    .long   DefaultISR                                      /* 87*/
    .long   DefaultISR                                      /* 88*/
    .long   DefaultISR                                      /* 89*/  
    .long   DefaultISR                                      /* 90*/
    .long   DefaultISR                                      /* 91*/
    .long   DefaultISR                                      /* 92*/
    .long   DefaultISR                                      /* 93*/
    .long   DefaultISR                                      /* 94*/
    .long   DefaultISR                                      /* 95*/
    .long   DefaultISR                                      /* 96*/
    .long   DefaultISR                                      /* 97*/
    .long   DefaultISR                                      /* 98*/
    .long   DefaultISR                                      /* 99*/  
    .long   DefaultISR                                      /* 100*/
    .long   DefaultISR                                      /* 101*/     
    .long   DefaultISR                                      /* 102*/
    .long   DefaultISR                                      /* 103*/
    .long   DefaultISR                                      /* 104*/
    .long   DefaultISR                                      /* 105*/
    .long   DefaultISR                                      /* 106*/
    .long   DefaultISR                                      /* 107*/
    .long   DefaultISR                                      /* 108*/
    .long   DefaultISR                                      /* 109*/
    .long   DefaultISR                                      /* 110*/
    .long   DefaultISR                                      /* 111*/
    .long   DefaultISR                                      /* 112*/
    .long   DefaultISR                                      /* 113*/
    .long   DefaultISR                                      /* 114*/
    .long   DefaultISR                                      /* 115*/
    .long   DefaultISR                                      /* 116*/
    .long   DefaultISR                                      /* 117*/
    .long   DefaultISR                                      /* 118*/
    .long   DefaultISR                                      /* 119*/
    .long   DefaultISR                                      /* 120*/
    .long   DefaultISR                                      /* 121*/
    .long   DefaultISR                                      /* 122*/
    .long   DefaultISR                                      /* 123*/
    .long   DefaultISR                                      /* 124*/
    .long   DefaultISR                                      /* 125*/
    .long   DefaultISR                                      /* 126*/
    .long   DefaultISR                                      /* 127*/
    .long   DefaultISR                                      /* 128*/
    .long   DefaultISR                                      /* 129*/
    .long   DefaultISR                                      /* 130*/
    .long   DefaultISR                                      /* 131*/
    .long   DefaultISR                                      /* 132*/
    .long   DefaultISR                                      /* 133*/
    .long   DefaultISR                                      /* 134*/
    .long   DefaultISR                                      /* 135*/
    .long   DefaultISR                                      /* 136*/
    .long   DefaultISR                                      /* 137*/
    .long   DefaultISR                                      /* 138*/
    .long   DefaultISR                                      /* 139*/
    .long   DefaultISR                                      /* 140*/
    .long   DefaultISR                                      /* 141*/
    .long   DefaultISR                                      /* 142*/
    .long   DefaultISR                                      /* 143*/
    .long   DefaultISR                                      /* 144*/
    .long   DefaultISR                                      /* 145*/
    .long   DefaultISR                                      /* 146*/
    .long   DefaultISR                                      /* 147*/
    .long   DefaultISR                                      /* 148*/
    .long   DefaultISR                                      /* 149*/
    .long   DefaultISR                                      /* 150*/
    .long   DefaultISR                                      /* 151*/
    .long   DefaultISR                                      /* 152*/
    .long   DefaultISR                                      /* 153*/
    .long   DefaultISR                                      /* 154*/
    .long   DefaultISR                                      /* 155*/
    .long   DefaultISR                                      /* 156*/
    .long   DefaultISR                                      /* 157*/
    .long   DefaultISR                                      /* 158*/
    .long   DefaultISR                                      /* 159*/
    .long   DefaultISR                                      /* 160*/
    .long   DefaultISR                                      /* 161*/
    .long   DefaultISR                                      /* 162*/
    .long   DefaultISR                                      /* 163*/
    .long   DefaultISR                                      /* 164*/
    .long   DefaultISR                                      /* 165*/
    .long   DefaultISR                                      /* 166*/
    .long   DefaultISR                                      /* 167*/
    .long   DefaultISR                                      /* 168*/
    .long   DefaultISR                                      /* 169*/
    .long   DefaultISR                                      /* 170*/
    .long   DefaultISR                                      /* 171*/
    .long   DefaultISR                                      /* 172*/
    .long   DefaultISR                                      /* 173*/
    .long   DefaultISR                                      /* 174*/
    .long   DefaultISR                                      /* 175*/
    .long   DefaultISR                                      /* 176*/
    .long   DefaultISR                                      /* 177*/
    .long   DefaultISR                                      /* 178*/
    .long   DefaultISR                                      /* 179*/
    .long   DefaultISR                                      /* 180*/
    .long   DefaultISR                                      /* 181*/
    .long   DefaultISR                                      /* 182*/
    .long   DefaultISR                                      /* 183*/
    .long   DefaultISR                                      /* 184*/
    .long   DefaultISR                                      /* 185*/
    .long   DefaultISR                                      /* 186*/
    .long   DefaultISR                                      /* 187*/
    .long   DefaultISR                                      /* 188*/
    .long   DefaultISR                                      /* 189*/
    .long   DefaultISR                                      /* 190*/
    .long   DefaultISR                                      /* 191*/
    .long   DefaultISR                                      /* 192*/
    .long   DefaultISR                                      /* 193*/
    .long   DefaultISR                                      /* 194*/
    .long   DefaultISR                                      /* 195*/
    .long   DefaultISR                                      /* 196*/
    .long   DefaultISR                                      /* 197*/
    .long   DefaultISR                                      /* 198*/
    .long   DefaultISR                                      /* 199*/
    .long   DefaultISR                                      /* 200*/
    .long   DefaultISR                                      /* 201*/
    .long   DefaultISR                                      /* 202*/
    .long   DefaultISR                                      /* 203*/
    .long   DefaultISR                                      /* 204*/
    .long   DefaultISR                                      /* 205*/
    .long   DefaultISR                                      /* 206*/
    .long   DefaultISR                                      /* 207*/
    .long   DefaultISR                                      /* 208*/
    .long   DefaultISR                                      /* 209*/
    .long   DefaultISR                                      /* 210*/
    .long   DefaultISR                                      /* 211*/
    .long   DefaultISR                                      /* 212*/
    .long   DefaultISR                                      /* 213*/
    .long   DefaultISR                                      /* 214*/
    .long   DefaultISR                                      /* 215*/
    .long   DefaultISR                                      /* 216*/
    .long   DefaultISR                                      /* 217*/
    .long   DefaultISR                                      /* 218*/
    .long   DefaultISR                                      /* 219*/
    .long   DefaultISR                                      /* 220*/
    .long   DefaultISR                                      /* 221*/
    .long   DefaultISR                                      /* 222*/
    .long   DefaultISR                                      /* 223*/
    .long   DefaultISR                                      /* 224*/
    .long   DefaultISR                                      /* 225*/
    .long   DefaultISR                                      /* 226*/
    .long   DefaultISR                                      /* 227*/
    .long   DefaultISR                                      /* 228*/
    .long   DefaultISR                                      /* 229*/
    .long   DefaultISR                                      /* 230*/
    .long   DefaultISR                                      /* 231*/
    .long   DefaultISR                                      /* 232*/
    .long   DefaultISR                                      /* 233*/
    .long   DefaultISR                                      /* 234*/
    .long   DefaultISR                                      /* 235*/
    .long   DefaultISR                                      /* 236*/
    .long   DefaultISR                                      /* 237*/
    .long   DefaultISR                                      /* 238*/
    .long   DefaultISR                                      /* 239*/

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
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFF3DFE

    .text
    .thumb

/* Reset Handler */

    .thumb_func
    .align 2
    .globl   Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    cpsid   i               /* Mask interrupts */
#ifndef __NO_SYSTEM_INIT
    bl SystemInit
#endif
    bl init_data_bss
    cpsie   i               /* Unmask interrupts */
#ifndef __START
#define __START main
#endif
#ifndef __ATOLLIC__
    bl    __START
#else
    bl    __libc_init_array
    bl    main
#endif
    .pool
    .size Reset_Handler, . - Reset_Handler

    .align	1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    ldr	r0, =DefaultISR
    bx r0
    .size DefaultISR, . - DefaultISR

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler	handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler
    def_irq_handler    DMA0_04_IRQHandler
    def_irq_handler    DMA0_15_IRQHandler
    def_irq_handler    DMA0_26_IRQHandler
    def_irq_handler    DMA0_37_IRQHandler
    def_irq_handler    CTI0_DMA0_Error_IRQHandler
    def_irq_handler    FLEXIO0_IRQHandler
    def_irq_handler    TPM0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    LPIT0_IRQHandler
    def_irq_handler    LPSPI0_IRQHandler
    def_irq_handler    LPSPI1_IRQHandler
    def_irq_handler    LPUART0_IRQHandler
    def_irq_handler    LPUART1_IRQHandler
    def_irq_handler    LPI2C0_IRQHandler
    def_irq_handler    LPI2C1_IRQHandler
    def_irq_handler    Reserved32_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_IRQHandler
    def_irq_handler    PORTC_IRQHandler
    def_irq_handler    PORTD_IRQHandler
    def_irq_handler    PORTE_IRQHandler
    def_irq_handler    LLWU0_IRQHandler
    def_irq_handler    I2S0_IRQHandler
    def_irq_handler    USB0_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    LPTMR0_IRQHandler
    def_irq_handler    RTC_Seconds_IRQHandler
    def_irq_handler    INTMUX0_0_IRQHandler
    def_irq_handler    INTMUX0_1_IRQHandler
    def_irq_handler    INTMUX0_2_IRQHandler
    def_irq_handler    INTMUX0_3_IRQHandler
    def_irq_handler    LPTMR1_IRQHandler
    def_irq_handler    Reserved49_IRQHandler
    def_irq_handler    Reserved50_IRQHandler
    def_irq_handler    Reserved51_IRQHandler
    def_irq_handler    LPSPI2_IRQHandler
    def_irq_handler    LPUART2_IRQHandler
    def_irq_handler    EMVSIM0_IRQHandler
    def_irq_handler    LPI2C2_IRQHandler
    def_irq_handler    TSI0_IRQHandler
    def_irq_handler    PMC_IRQHandler
    def_irq_handler    FTFA_IRQHandler
    def_irq_handler    SCG_IRQHandler
    def_irq_handler    WDOG0_IRQHandler
    def_irq_handler    DAC0_IRQHandler
    def_irq_handler    TRNG_IRQHandler
    def_irq_handler    RCM_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    CMP1_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    Reserved67_IRQHandler
    def_irq_handler    Reserved68_IRQHandler
    def_irq_handler    Reserved69_IRQHandler
    def_irq_handler    Reserved70_IRQHandler
    def_irq_handler    Reserved71_IRQHandler
    def_irq_handler    Reserved72_IRQHandler
    def_irq_handler    Reserved73_IRQHandler
    def_irq_handler    Reserved74_IRQHandler
    def_irq_handler    Reserved75_IRQHandler
    def_irq_handler    Reserved76_IRQHandler
    def_irq_handler    Reserved77_IRQHandler
    def_irq_handler    Reserved78_IRQHandler
    def_irq_handler    Reserved79_IRQHandler

    .end
