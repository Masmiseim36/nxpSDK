/* ---------------------------------------------------------------------------------------*/
/*  @file:    startup_MKL82Z7.s                                                           */
/*  @purpose: CMSIS Cortex-M0P Core Device Startup File                                   */
/*            MKL82Z7                                                                     */
/*  @version: 1.4                                                                         */
/*  @date:    2015-8-28                                                                   */
/*  @build:   b150910                                                                     */
/* ---------------------------------------------------------------------------------------*/
/*                                                                                        */
/* Copyright (c) 1997-2015 Freescale Semiconductor, Inc.                                  */
/* All rights reserved.                                                                   */
/*                                                                                        */
/* SPDX-License-Identifier: BSD-3-Clause                                                  */
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
    .long   DMA0_DMA4_IRQHandler                            /* DMA channel 0, 4 transfer complete*/
    .long   DMA1_DMA5_IRQHandler                            /* DMA channel 1, 5 transfer complete*/
    .long   DMA2_DMA6_IRQHandler                            /* DMA channel 2, 6 transfer complete*/
    .long   DMA3_DMA7_IRQHandler                            /* DMA channel 3, 7 transfer complete*/
    .long   DMA_Error_IRQHandler                            /* DMA channel 0 - 7 error*/
    .long   FLEXIO0_IRQHandler                              /* Flexible IO*/
    .long   TPM0_IRQHandler                                 /* Timer/PWM module 0*/
    .long   TPM1_IRQHandler                                 /* Timer/PWM module 1*/
    .long   TPM2_IRQHandler                                 /* Timer/PWM module 2*/
    .long   PIT0_IRQHandler                                 /* Periodic Interrupt Timer 0*/
    .long   SPI0_IRQHandler                                 /* Serial Peripheral Interface 0*/
    .long   EMVSIM0_IRQHandler                              /* EMVSIM0 common interrupt*/
    .long   LPUART0_IRQHandler                              /* LPUART0 status and error*/
    .long   LPUART1_IRQHandler                              /* LPUART1 status and error*/
    .long   I2C0_IRQHandler                                 /* Inter-Integrated Circuit 0*/
    .long   QSPI0_IRQHandler                                /* QuadSPI0 interrupt*/
    .long   Reserved32_IRQHandler                           /* DryIce tamper detect*/
    .long   PORTA_IRQHandler                                /* Pin detect Port A*/
    .long   PORTB_IRQHandler                                /* Pin detect Port B*/
    .long   PORTC_IRQHandler                                /* Pin detect Port C*/
    .long   PORTD_IRQHandler                                /* Pin detect Port D*/
    .long   PORTE_IRQHandler                                /* Pin detect Port E*/
    .long   LLWU_IRQHandler                                 /* Low Leakage Wakeup*/
    .long   LTC0_IRQHandler                                 /* Low power trusted cryptographic*/
    .long   USB0_IRQHandler                                 /* USB OTG interrupt*/
    .long   ADC0_IRQHandler                                 /* Analog-to-Digital Converter 0*/
    .long   LPTMR0_IRQHandler                               /* Low-Power Timer 0*/
    .long   RTC_Seconds_IRQHandler                          /* RTC seconds*/
    .long   INTMUX0_0_IRQHandler                            /* Selectable peripheral interrupt INTMUX0-0*/
    .long   INTMUX0_1_IRQHandler                            /* Selectable peripheral interrupt INTMUX0-1*/
    .long   INTMUX0_2_IRQHandler                            /* Selectable peripheral interrupt INTMUX0-2*/
    .long   INTMUX0_3_IRQHandler                            /* Selectable peripheral interrupt INTMUX0-3*/
    .long   LPTMR1_IRQHandler                               /* Low-Power Timer 1 (INTMUX source IRQ0)*/
    .long   Reserved49_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ1)*/
    .long   Reserved50_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ2)*/
    .long   Reserved51_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ3)*/
    .long   SPI1_IRQHandler                                 /* Serial Peripheral Interface 1 (INTMUX source IRQ4)*/
    .long   LPUART2_IRQHandler                              /* LPUART2 status and error (INTMUX source IRQ5)*/
    .long   EMVSIM1_IRQHandler                              /* EMVSIM1 common interrupt (INTMUX source IRQ6)*/
    .long   I2C1_IRQHandler                                 /* Inter-Integrated Circuit 1 (INTMUX source IRQ7)*/
    .long   TSI0_IRQHandler                                 /* Touch Sensing Input 0 (INTMUX source IRQ8)*/
    .long   PMC_IRQHandler                                  /* PMC controller low-voltage detect, low-voltage warning (INTMUX source IRQ9)*/
    .long   FTFA_IRQHandler                                 /* FTFA command complete/read collision (INTMUX source IRQ10)*/
    .long   MCG_IRQHandler                                  /* Multipurpose clock generator (INTMUX source IRQ11)*/
    .long   WDOG_EWM_IRQHandler                             /* Single interrupt vector for  WDOG and EWM (INTMUX source IRQ12)*/
    .long   DAC0_IRQHandler                                 /* Digital-to-analog converter 0 (INTMUX source IRQ13)*/
    .long   TRNG0_IRQHandler                                /* True randon number generator (INTMUX source IRQ14)*/
    .long   Reserved63_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ15)*/
    .long   CMP0_IRQHandler                                 /* Comparator 0 (INTMUX source IRQ16)*/
    .long   Reserved65_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ17)*/
    .long   RTC_Alarm_IRQHandler                            /* Real time clock (INTMUX source IRQ18)*/
    .long   Reserved67_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ19)*/
    .long   Reserved68_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ20)*/
    .long   Reserved69_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ21)*/
    .long   Reserved70_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ22)*/
    .long   Reserved71_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ23)*/
    .long   DMA4_IRQHandler                                 /* DMA channel 4 transfer complete (INTMUX source IRQ24)*/
    .long   DMA5_IRQHandler                                 /* DMA channel 5 transfer complete (INTMUX source IRQ25)*/
    .long   DMA6_IRQHandler                                 /* DMA channel 6 transfer complete (INTMUX source IRQ26)*/
    .long   DMA7_IRQHandler                                 /* DMA channel 7 transfer complete (INTMUX source IRQ27)*/
    .long   Reserved76_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ28)*/
    .long   Reserved77_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ29)*/
    .long   Reserved78_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ30)*/
    .long   Reserved79_IRQHandler                           /* Reserved interrupt (INTMUX source IRQ31)*/

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFBDFE

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
    def_irq_handler    DMA0_DMA4_IRQHandler
    def_irq_handler    DMA1_DMA5_IRQHandler
    def_irq_handler    DMA2_DMA6_IRQHandler
    def_irq_handler    DMA3_DMA7_IRQHandler
    def_irq_handler    DMA_Error_IRQHandler
    def_irq_handler    FLEXIO0_IRQHandler
    def_irq_handler    TPM0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    PIT0_IRQHandler
    def_irq_handler    SPI0_IRQHandler
    def_irq_handler    EMVSIM0_IRQHandler
    def_irq_handler    LPUART0_IRQHandler
    def_irq_handler    LPUART1_IRQHandler
    def_irq_handler    I2C0_IRQHandler
    def_irq_handler    QSPI0_IRQHandler
    def_irq_handler    Reserved32_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_IRQHandler
    def_irq_handler    PORTC_IRQHandler
    def_irq_handler    PORTD_IRQHandler
    def_irq_handler    PORTE_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    LTC0_IRQHandler
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
    def_irq_handler    SPI1_IRQHandler
    def_irq_handler    LPUART2_IRQHandler
    def_irq_handler    EMVSIM1_IRQHandler
    def_irq_handler    I2C1_IRQHandler
    def_irq_handler    TSI0_IRQHandler
    def_irq_handler    PMC_IRQHandler
    def_irq_handler    FTFA_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    WDOG_EWM_IRQHandler
    def_irq_handler    DAC0_IRQHandler
    def_irq_handler    TRNG0_IRQHandler
    def_irq_handler    Reserved63_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    Reserved65_IRQHandler
    def_irq_handler    RTC_Alarm_IRQHandler
    def_irq_handler    Reserved67_IRQHandler
    def_irq_handler    Reserved68_IRQHandler
    def_irq_handler    Reserved69_IRQHandler
    def_irq_handler    Reserved70_IRQHandler
    def_irq_handler    Reserved71_IRQHandler
    def_irq_handler    DMA4_IRQHandler
    def_irq_handler    DMA5_IRQHandler
    def_irq_handler    DMA6_IRQHandler
    def_irq_handler    DMA7_IRQHandler
    def_irq_handler    Reserved76_IRQHandler
    def_irq_handler    Reserved77_IRQHandler
    def_irq_handler    Reserved78_IRQHandler
    def_irq_handler    Reserved79_IRQHandler

    .end
