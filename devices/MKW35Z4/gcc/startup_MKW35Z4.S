/* ------------------------------------------------------------------------- */
/*  @file:    startup_MKW35Z4.s                                              */
/*  @purpose: CMSIS Cortex-M0P Core Device Startup File                      */
/*            MKW35Z4                                                        */
/*  @version: 1.4                                                            */
/*  @date:    2019-1-25                                                      */
/*  @build:   b190529                                                        */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Copyright 1997-2016 Freescale Semiconductor, Inc.                         */
/* Copyright 2016-2019 NXP                                                   */
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
    .long   DMA0_IRQHandler                                 /* DMA channel 0 transfer complete*/
    .long   DMA1_IRQHandler                                 /* DMA channel 1 transfer complete*/
    .long   DMA2_IRQHandler                                 /* DMA channel 2 transfer complete*/
    .long   DMA3_IRQHandler                                 /* DMA channel 3 transfer complete*/
    .long   Reserved20_IRQHandler                           /* Reserved interrupt*/
    .long   FTFE_IRQHandler                                 /* Command complete and read collision, Flash ECC Errors*/
    .long   LVD_LVW_DCDC_IRQHandler                         /* Low-voltage detect, low-voltage warning, DCDC*/
    .long   LLWU_IRQHandler                                 /* Low leakage wakeup Unit*/
    .long   I2C0_IRQHandler                                 /* I2C0 interrupt*/
    .long   I2C1_IRQHandler                                 /* I2C1 interrupt*/
    .long   SPI0_IRQHandler                                 /* SPI0 single interrupt vector for all sources*/
    .long   Reserved27_IRQHandler                           /* Reserved interrupt*/
    .long   LPUART0_IRQHandler                              /* LPUART0 status and error*/
    .long   TRNG0_IRQHandler                                /* TRNG0 interrupt*/
    .long   CMT_IRQHandler                                  /* CMT interrupt*/
    .long   ADC0_IRQHandler                                 /* ADC0 interrupt*/
    .long   CMP0_IRQHandler                                 /* CMP0 interrupt*/
    .long   TPM0_IRQHandler                                 /* TPM0 single interrupt vector for all sources*/
    .long   TPM1_IRQHandler                                 /* TPM1 single interrupt vector for all sources*/
    .long   TPM2_IRQHandler                                 /* TPM2 single interrupt vector for all sources*/
    .long   RTC_IRQHandler                                  /* RTC alarm*/
    .long   RTC_Seconds_IRQHandler                          /* RTC seconds*/
    .long   PIT_IRQHandler                                  /* PIT interrupt*/
    .long   LTC0_IRQHandler                                 /* LTC0 interrupt*/
    .long   Radio_0_IRQHandler                              /* BTLE, ZIGBEE, ANT, GENFSK interrupt 0*/
    .long   Reserved41_IRQHandler                           /* Reserved interrupt*/
    .long   Radio_1_IRQHandler                              /* BTLE, ZIGBEE, ANT, GENFSK interrupt 1*/
    .long   MCG_IRQHandler                                  /* MCG interrupt*/
    .long   LPTMR0_IRQHandler                               /* LPTMR0 interrupt*/
    .long   SPI1_IRQHandler                                 /* SPI1 single interrupt vector for all sources*/
    .long   PORTA_IRQHandler                                /* PORTA Pin detect*/
    .long   PORTB_PORTC_IRQHandler                          /* PORTB and PORTC Pin detect*/

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFE

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
    .equ    VTOR, 0xE000ED08
    ldr     r0, =VTOR
    ldr     r1, =__isr_vector
    str     r1, [r0]
    ldr     r2, [r1]
#ifdef SUPPORT_WARMBOOT
/* if vlls is not the only wakeup source, jump to .NORMAL_BOOT */
    ldr     r0, =0x4007F000
    ldr     r0, [r0]
    cmp     r0, #1
    bne.n   .NORMAL_BOOT
/* if SMC_PMCTRL_STOPM != 4(VLLSx), jump to .NORMAL_BOOT */
    movs    r1, #7
    ldr     r0, =0x4007E001
    ldrb    r0, [r0]
    ands    r0, r0, r1
    cmp     r0, #4
    bne.n   .NORMAL_BOOT
/* if SMC_STOPCTRL_LLSM != 2(VLLS2) or 3(VLLS3), jump to .NORMAL_BOOT */
    ldr     r0, =0x4007E002
    ldrb    r0, [r0]
    ands    r0, r0, r1
    lsrs    r0, r0, #1
    cmp     r0, #1
    bne.n   .NORMAL_BOOT
/* if WARMBOOT_SEQUENCE do not match what WARMBOOT_CONFIG stored, jump to .NORMAL_BOOT */
    ldr     r1, =__warmboot_config_start__
    ldr     r3, [r1]
    ldr     r4, =WARMBOOT_SEQUENCE
    ldr     r4, [r4]
    cmp     r3, r4
    bne.n   .NORMAL_BOOT
/* if useWarmbootSp = 1, set MSP to top of WARMBOOT_STACK */
    ldr     r3, [r1,#4]
    cmp     r3, #0
    beq.n   .WARM_BOOT
/* load warm boot stack pointer    */
    ldr     r2, =__WarmbootStackLimit
.WARM_BOOT:
    msr     msp, r2
    ldr     r0, =SystemInit
    blx     r0
    cpsie   i               /* Unmask interrupts */
/* jump to warmmain  */
    ldr     r0, =warmmain
    bx      r0
    b       .
.NORMAL_BOOT:
#endif
    msr     msp, r2
#ifndef __NO_SYSTEM_INIT
    ldr   r0,=SystemInit
    blx   r0
#endif
#ifdef SUPPORT_WARMBOOT
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __warmboot_config_start__/__warmboot_config_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__warmboot_config_start__
    ldr    r3, =__warmboot_config_end__

    subs    r3, r2
    ble     .WARMBOOT_CONFIG_DATA_COPY_END

.WARMBOOT_CONFIG_DATA_COPY_START:
    subs    r3, 4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .WARMBOOT_CONFIG_DATA_COPY_START
.WARMBOOT_CONFIG_DATA_COPY_END:
#endif

/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      if SUPPORT_WARMBOOT is defined, data is copied from __WARMBOOT_CONFIG_END,
 *      else data is copied from __etext.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

#ifdef SUPPORT_WARMBOOT
    ldr    r1, =__WARMBOOT_CONFIG_END
#else
    ldr    r1, =__etext
#endif
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble     .LC0

.LC1:
    subs    r3, 4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .LC1
.LC0:

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

    subs    r2, r1
    ble .LC3

    movs    r0, 0
.LC2:
    subs    r2, 4
    str r0, [r1, r2]
    bge .LC2
.LC3:
#endif
    cpsie   i               /* Unmask interrupts */
#ifndef __START
#define __START _start
#endif
#ifndef __ATOLLIC__
    ldr   r0,=__START
    blx   r0
#else
    ldr   r0,=__libc_init_array
    blx   r0
    ldr   r0,=main
    bx    r0
#endif
    .pool
    .size Reset_Handler, . - Reset_Handler

    .align  1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    ldr r0, =DefaultISR
    bx r0
    .size DefaultISR, . - DefaultISR

    .align 1
    .thumb_func
    .weak NMI_Handler
    .type NMI_Handler, %function
NMI_Handler:
    ldr   r0,=NMI_Handler
    bx    r0
    .size NMI_Handler, . - NMI_Handler

    .align 1
    .thumb_func
    .weak HardFault_Handler
    .type HardFault_Handler, %function
HardFault_Handler:
    ldr   r0,=HardFault_Handler
    bx    r0
    .size HardFault_Handler, . - HardFault_Handler

    .align 1
    .thumb_func
    .weak SVC_Handler
    .type SVC_Handler, %function
SVC_Handler:
    ldr   r0,=SVC_Handler
    bx    r0
    .size SVC_Handler, . - SVC_Handler

    .align 1
    .thumb_func
    .weak PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    ldr   r0,=PendSV_Handler
    bx    r0
    .size PendSV_Handler, . - PendSV_Handler

    .align 1
    .thumb_func
    .weak SysTick_Handler
    .type SysTick_Handler, %function
SysTick_Handler:
    ldr   r0,=SysTick_Handler
    bx    r0
    .size SysTick_Handler, . - SysTick_Handler

    .align 1
    .thumb_func
    .weak DMA0_IRQHandler
    .type DMA0_IRQHandler, %function
DMA0_IRQHandler:
    ldr   r0,=DMA0_DriverIRQHandler
    bx    r0
    .size DMA0_IRQHandler, . - DMA0_IRQHandler

    .align 1
    .thumb_func
    .weak DMA1_IRQHandler
    .type DMA1_IRQHandler, %function
DMA1_IRQHandler:
    ldr   r0,=DMA1_DriverIRQHandler
    bx    r0
    .size DMA1_IRQHandler, . - DMA1_IRQHandler

    .align 1
    .thumb_func
    .weak DMA2_IRQHandler
    .type DMA2_IRQHandler, %function
DMA2_IRQHandler:
    ldr   r0,=DMA2_DriverIRQHandler
    bx    r0
    .size DMA2_IRQHandler, . - DMA2_IRQHandler

    .align 1
    .thumb_func
    .weak DMA3_IRQHandler
    .type DMA3_IRQHandler, %function
DMA3_IRQHandler:
    ldr   r0,=DMA3_DriverIRQHandler
    bx    r0
    .size DMA3_IRQHandler, . - DMA3_IRQHandler

    .align 1
    .thumb_func
    .weak I2C0_IRQHandler
    .type I2C0_IRQHandler, %function
I2C0_IRQHandler:
    ldr   r0,=I2C0_DriverIRQHandler
    bx    r0
    .size I2C0_IRQHandler, . - I2C0_IRQHandler

    .align 1
    .thumb_func
    .weak I2C1_IRQHandler
    .type I2C1_IRQHandler, %function
I2C1_IRQHandler:
    ldr   r0,=I2C1_DriverIRQHandler
    bx    r0
    .size I2C1_IRQHandler, . - I2C1_IRQHandler

    .align 1
    .thumb_func
    .weak SPI0_IRQHandler
    .type SPI0_IRQHandler, %function
SPI0_IRQHandler:
    ldr   r0,=SPI0_DriverIRQHandler
    bx    r0
    .size SPI0_IRQHandler, . - SPI0_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART0_IRQHandler
    .type LPUART0_IRQHandler, %function
LPUART0_IRQHandler:
    ldr   r0,=LPUART0_DriverIRQHandler
    bx    r0
    .size LPUART0_IRQHandler, . - LPUART0_IRQHandler

    .align 1
    .thumb_func
    .weak SPI1_IRQHandler
    .type SPI1_IRQHandler, %function
SPI1_IRQHandler:
    ldr   r0,=SPI1_DriverIRQHandler
    bx    r0
    .size SPI1_IRQHandler, . - SPI1_IRQHandler

#ifdef SUPPORT_WARMBOOT
    .align 1
    .thumb_func
    .weak warmmain
    .type warmmain, %function
warmmain:
    ldr   r0,=warmmain
    bx    r0
    .size warmmain, . - warmmain
#endif

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    DMA0_DriverIRQHandler
    def_irq_handler    DMA1_DriverIRQHandler
    def_irq_handler    DMA2_DriverIRQHandler
    def_irq_handler    DMA3_DriverIRQHandler
    def_irq_handler    Reserved20_IRQHandler
    def_irq_handler    FTFE_IRQHandler
    def_irq_handler    LVD_LVW_DCDC_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    I2C0_DriverIRQHandler
    def_irq_handler    I2C1_DriverIRQHandler
    def_irq_handler    SPI0_DriverIRQHandler
    def_irq_handler    Reserved27_IRQHandler
    def_irq_handler    LPUART0_DriverIRQHandler
    def_irq_handler    TRNG0_IRQHandler
    def_irq_handler    CMT_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    TPM0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    RTC_Seconds_IRQHandler
    def_irq_handler    PIT_IRQHandler
    def_irq_handler    LTC0_IRQHandler
    def_irq_handler    Radio_0_IRQHandler
    def_irq_handler    Reserved41_IRQHandler
    def_irq_handler    Radio_1_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    LPTMR0_IRQHandler
    def_irq_handler    SPI1_DriverIRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_PORTC_IRQHandler

    .end
