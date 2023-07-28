/* ------------------------------------------------------------------------- */
/*  @file:    startup_MIMXRT1021.s                                           */
/*  @purpose: CMSIS Cortex-M7 Core Device Startup File                       */
/*            MIMXRT1021                                                     */
/*  @version: 0.1                                                            */
/*  @date:    2017-11-6                                                      */
/*  @build:   b180204                                                        */
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

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                                      /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   MemManage_Handler                               /* MPU Fault Handler*/
    .long   BusFault_Handler                                /* Bus Fault Handler*/
    .long   UsageFault_Handler                              /* Usage Fault Handler*/
    .long   g_reliableUpdateAPI                             /* Bootloader API Tree root */
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   DebugMon_Handler                                /* Debug Monitor Handler*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   Reserved16_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved17_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved18_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved19_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved20_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved21_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved22_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved23_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved24_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved25_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved26_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved27_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved28_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved29_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved30_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved31_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved32_IRQHandler                           /* Reserved interrupt*/
    .long   CTI0_ERROR_IRQHandler                           /* CTI0_Error*/
    .long   CTI1_ERROR_IRQHandler                           /* CTI1_Error*/
    .long   CORE_IRQHandler                                 /* CorePlatform exception IRQ*/
    .long   LPUART1_IRQHandler                              /* LPUART1 status and error*/
    .long   LPUART2_IRQHandler                              /* LPUART2 status and error*/
    .long   LPUART3_IRQHandler                              /* LPUART3 status and error*/
    .long   LPUART4_IRQHandler                              /* LPUART4 status and error*/
    .long   LPUART5_IRQHandler                              /* LPUART5 status and error*/
    .long   LPUART6_IRQHandler                              /* LPUART6 status and error*/
    .long   LPUART7_IRQHandler                              /* LPUART7 status and error*/
    .long   LPUART8_IRQHandler                              /* LPUART8 status and error*/
    .long   LPI2C1_IRQHandler                               /* LPI2C1 interrupt*/
    .long   LPI2C2_IRQHandler                               /* LPI2C2 interrupt*/
    .long   LPI2C3_IRQHandler                               /* LPI2C3 interrupt*/
    .long   LPI2C4_IRQHandler                               /* LPI2C4 interrupt*/
    .long   LPSPI1_IRQHandler                               /* LPSPI1 single interrupt vector for all sources*/
    .long   LPSPI2_IRQHandler                               /* LPSPI2 single interrupt vector for all sources*/
    .long   LPSPI3_IRQHandler                               /* LPSPI3 single interrupt vector for all sources*/
    .long   LPSPI4_IRQHandler                               /* LPSPI4  single interrupt vector for all sources*/
    .long   CAN1_IRQHandler                                 /* CAN1 interrupt*/
    .long   CAN2_IRQHandler                                 /* CAN2 interrupt*/
    .long   FLEXRAM_IRQHandler                              /* FlexRAM address out of range Or access hit IRQ*/
    .long   KPP_IRQHandler                                  /* Keypad nterrupt*/
    .long   TSC_DIG_IRQHandler                              /* TSC interrupt*/
    .long   GPR_IRQ_IRQHandler                              /* GPR interrupt*/
    .long   Reserved58_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved59_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved60_IRQHandler                           /* Reserved interrupt*/
    .long   WDOG2_IRQHandler                                /* WDOG2 interrupt*/
    .long   SNVS_HP_WRAPPER_IRQHandler                      /* SRTC Consolidated Interrupt. Non TZ*/
    .long   SNVS_HP_WRAPPER_TZ_IRQHandler                   /* SRTC Security Interrupt. TZ*/
    .long   SNVS_LP_WRAPPER_IRQHandler                      /* ON-OFF button press shorter than 5 secs (pulse event)*/
    .long   Reserved65_IRQHandler                           /* Reserved interrupt*/
    .long   DCP_IRQHandler                                  /* DCP_IRQ interrupt*/
    .long   DCP_VMI_IRQHandler                              /* DCP_VMI_IRQ interrupt*/
    .long   DCP_SECURE_IRQHandler                           /* DCP_SECURE_IRQ interrupt*/
    .long   TRNG_IRQHandler                                 /* TRNG interrupt*/
    .long   Reserved70_IRQHandler                           /* Reserved interrupt*/
    .long   BEE_IRQHandler                                  /* BEE interrupt*/
    .long   Reserved72_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved73_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved74_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved75_IRQHandler                           /* Reserved interrupt*/
    .long   SPDIF_IRQHandler                                /* SPDIF interrupt*/
    .long   Reserved77_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved78_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved79_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved80_IRQHandler                           /* Reserved interrupt*/
    .long   USB_PHY1_IRQHandler                             /* USBPHY (UTMI0), Interrupt*/
    .long   USB_PHY2_IRQHandler                             /* USBPHY (UTMI0), Interrupt*/
    .long   ADC1_IRQHandler                                 /* ADC1 interrupt*/
    .long   ADC_5HC_IRQHandler                              /* ADC_5HC interrupt*/
    .long   DCDC_IRQHandler                                 /* DCDC interrupt*/
    .long   Reserved86_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved87_IRQHandler                           /* Reserved interrupt*/
    .long   GPIO1_INT0_IRQHandler                           /* Active HIGH Interrupt from INT0 from GPIO*/
    .long   GPIO1_INT1_IRQHandler                           /* Active HIGH Interrupt from INT1 from GPIO*/
    .long   GPIO1_INT2_IRQHandler                           /* Active HIGH Interrupt from INT2 from GPIO*/
    .long   GPIO1_INT3_IRQHandler                           /* Active HIGH Interrupt from INT3 from GPIO*/
    .long   GPIO1_INT4_IRQHandler                           /* Active HIGH Interrupt from INT4 from GPIO*/
    .long   GPIO1_INT5_IRQHandler                           /* Active HIGH Interrupt from INT5 from GPIO*/
    .long   GPIO1_INT6_IRQHandler                           /* Active HIGH Interrupt from INT6 from GPIO*/
    .long   GPIO1_INT7_IRQHandler                           /* Active HIGH Interrupt from INT7 from GPIO*/
    .long   GPIO1_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO1 signal 0 throughout 15*/
    .long   GPIO1_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO1 signal 16 throughout 31*/
    .long   GPIO2_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO2 signal 0 throughout 15*/
    .long   GPIO2_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO2 signal 16 throughout 31*/
    .long   GPIO3_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO3 signal 0 throughout 15*/
    .long   GPIO3_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO3 signal 16 throughout 31*/
    .long   GPIO4_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO4 signal 0 throughout 15*/
    .long   GPIO4_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO4 signal 16 throughout 31*/
    .long   GPIO5_Combined_0_15_IRQHandler                  /* Combined interrupt indication for GPIO5 signal 0 throughout 15*/
    .long   GPIO5_Combined_16_31_IRQHandler                 /* Combined interrupt indication for GPIO5 signal 16 throughout 31*/
    .long   FLEXIO1_IRQHandler                              /* FLEXIO1 interrupt*/
    .long   FLEXIO2_IRQHandler                              /* FLEXIO2 interrupt*/
    .long   WDOG1_IRQHandler                                /* WDOG1 interrupt*/
    .long   RTWDOG_IRQHandler                               /* RTWDOG interrupt*/
    .long   EWM_IRQHandler                                  /* EWM interrupt*/
    .long   CCM_1_IRQHandler                                /* CCM IRQ1 interrupt*/
    .long   CCM_2_IRQHandler                                /* CCM IRQ2 interrupt*/
    .long   GPC_IRQHandler                                  /* GPC interrupt*/
    .long   SRC_IRQHandler                                  /* SRC interrupt*/
    .long   Reserved115_IRQHandler                          /* Reserved interrupt*/
    .long   GPT1_IRQHandler                                 /* GPT1 interrupt*/
    .long   GPT2_IRQHandler                                 /* GPT2 interrupt*/
    .long   PWM1_0_IRQHandler                               /* PWM1 capture 0, compare 0, or reload 0 interrupt*/
    .long   PWM1_1_IRQHandler                               /* PWM1 capture 1, compare 1, or reload 0 interrupt*/
    .long   PWM1_2_IRQHandler                               /* PWM1 capture 2, compare 2, or reload 0 interrupt*/
    .long   PWM1_3_IRQHandler                               /* PWM1 capture 3, compare 3, or reload 0 interrupt*/
    .long   PWM1_FAULT_IRQHandler                           /* PWM1 fault or reload error interrupt*/
    .long   Reserved123_IRQHandler                          /* Reserved interrupt*/
    .long   FLEXSPI_IRQHandler                              /* FlexSPI0 interrupt*/
    .long   SEMC_IRQHandler                                 /* Reserved interrupt*/
    .long   USDHC1_IRQHandler                               /* USDHC1 interrupt*/
    .long   USDHC2_IRQHandler                               /* USDHC2 interrupt*/
    .long   USB_OTG2_IRQHandler                             /* USBO2 USB OTG2*/
    .long   USB_OTG1_IRQHandler                             /* USBO2 USB OTG1*/
    .long   ENET_IRQHandler                                 /* ENET interrupt*/
    .long   ENET_1588_Timer_IRQHandler                      /* ENET_1588_Timer interrupt*/
    .long   Reserved132_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved133_IRQHandler                          /* Reserved interrupt*/
    .long   ADC_ETC_IRQ0_IRQHandler                         /* ADCETC IRQ0 interrupt*/
    .long   ADC_ETC_IRQ1_IRQHandler                         /* ADCETC IRQ1 interrupt*/
    .long   ADC_ETC_IRQ2_IRQHandler                         /* ADCETC IRQ2 interrupt*/
    .long   ADC_ETC_ERROR_IRQ_IRQHandler                    /* ADCETC Error IRQ interrupt*/
    .long   PIT_IRQHandler                                  /* PIT interrupt*/
    .long   ACMP_IRQ1_IRQHandler                            /* ACMP interrupt*/
    .long   ACMP_IRQ2_IRQHandler                            /* ACMP interrupt*/
    .long   ACMP_IRQ3_IRQHandler                            /* ACMP interrupt*/
    .long   ACMP_IRQ4_IRQHandler                            /* ACMP interrupt*/
    .long   Reserved143_IRQHandler                          /* Reserved interrupt*/
    .long   Reserved144_IRQHandler                          /* Reserved interrupt*/
    .long   ENC1_IRQHandler                                 /* ENC1 interrupt*/
    .long   ENC2_IRQHandler                                 /* ENC2 interrupt*/
    .long   ENC3_IRQHandler                                 /* ENC3 interrupt*/
    .long   ENC4_IRQHandler                                 /* ENC4 interrupt*/
    .long   TMR1_IRQHandler                                 /* TMR1 interrupt*/
    .long   TMR2_IRQHandler                                 /* TMR2 interrupt*/
    .long   TMR3_IRQHandler                                 /* TMR3 interrupt*/
    .long   TMR4_IRQHandler                                 /* TMR4 interrupt*/
    .long   PWM2_0_IRQHandler                               /* PWM2 capture 0, compare 0, or reload 0 interrupt*/
    .long   PWM2_1_IRQHandler                               /* PWM2 capture 1, compare 1, or reload 0 interrupt*/
    .long   PWM2_2_IRQHandler                               /* PWM2 capture 2, compare 2, or reload 0 interrupt*/
    .long   PWM2_3_IRQHandler                               /* PWM2 capture 3, compare 3, or reload 0 interrupt*/
    .long   PWM2_FAULT_IRQHandler                           /* PWM2 fault or reload error interrupt*/
    .long   PWM3_0_IRQHandler                               /* PWM3 capture 0, compare 0, or reload 0 interrupt*/
    .long   PWM3_1_IRQHandler                               /* PWM3 capture 1, compare 1, or reload 0 interrupt*/
    .long   PWM3_2_IRQHandler                               /* PWM3 capture 2, compare 2, or reload 0 interrupt*/
    .long   PWM3_3_IRQHandler                               /* PWM3 capture 3, compare 3, or reload 0 interrupt*/
    .long   PWM3_FAULT_IRQHandler                           /* PWM3 fault or reload error interrupt*/
    .long   PWM4_0_IRQHandler                               /* PWM4 capture 0, compare 0, or reload 0 interrupt*/
    .long   PWM4_1_IRQHandler                               /* PWM4 capture 1, compare 1, or reload 0 interrupt*/
    .long   PWM4_2_IRQHandler                               /* PWM4 capture 2, compare 2, or reload 0 interrupt*/
    .long   PWM4_3_IRQHandler                               /* PWM4 capture 3, compare 3, or reload 0 interrupt*/
    .long   PWM4_FAULT_IRQHandler                           /* PWM4 fault or reload error interrupt*/
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
    .long   DefaultISR                                      /* 240*/
    .long   DefaultISR                                      /* 241*/
    .long   DefaultISR                                      /* 242*/
    .long   DefaultISR                                      /* 243*/
    .long   DefaultISR                                      /* 244*/
    .long   DefaultISR                                      /* 245*/
    .long   DefaultISR                                      /* 246*/
    .long   DefaultISR                                      /* 247*/
    .long   DefaultISR                                      /* 248*/
    .long   DefaultISR                                      /* 249*/
    .long   DefaultISR                                      /* 250*/
    .long   DefaultISR                                      /* 251*/
    .long   DefaultISR                                      /* 252*/
    .long   DefaultISR                                      /* 253*/
    .long   DefaultISR                                      /* 254*/
    .long   0xFFFFFFFF                                      /*  Reserved for user TRIM value*/

    .size    __isr_vector, . - __isr_vector

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
    msr     msp, r2
#ifndef __NO_SYSTEM_INIT
    ldr   r0,=SystemInit
    blx   r0
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

    cpsie   i               /* Unmask interrupts */
#ifndef __START
#define __START main
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
    b DefaultISR
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
    .weak LPUART1_IRQHandler
    .type LPUART1_IRQHandler, %function
LPUART1_IRQHandler:
    ldr   r0,=LPUART1_DriverIRQHandler
    bx    r0
    .size LPUART1_IRQHandler, . - LPUART1_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART2_IRQHandler
    .type LPUART2_IRQHandler, %function
LPUART2_IRQHandler:
    ldr   r0,=LPUART2_DriverIRQHandler
    bx    r0
    .size LPUART2_IRQHandler, . - LPUART2_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART3_IRQHandler
    .type LPUART3_IRQHandler, %function
LPUART3_IRQHandler:
    ldr   r0,=LPUART3_DriverIRQHandler
    bx    r0
    .size LPUART3_IRQHandler, . - LPUART3_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART4_IRQHandler
    .type LPUART4_IRQHandler, %function
LPUART4_IRQHandler:
    ldr   r0,=LPUART4_DriverIRQHandler
    bx    r0
    .size LPUART4_IRQHandler, . - LPUART4_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART5_IRQHandler
    .type LPUART5_IRQHandler, %function
LPUART5_IRQHandler:
    ldr   r0,=LPUART5_DriverIRQHandler
    bx    r0
    .size LPUART5_IRQHandler, . - LPUART5_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART6_IRQHandler
    .type LPUART6_IRQHandler, %function
LPUART6_IRQHandler:
    ldr   r0,=LPUART6_DriverIRQHandler
    bx    r0
    .size LPUART6_IRQHandler, . - LPUART6_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART7_IRQHandler
    .type LPUART7_IRQHandler, %function
LPUART7_IRQHandler:
    ldr   r0,=LPUART7_DriverIRQHandler
    bx    r0
    .size LPUART7_IRQHandler, . - LPUART7_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART8_IRQHandler
    .type LPUART8_IRQHandler, %function
LPUART8_IRQHandler:
    ldr   r0,=LPUART8_DriverIRQHandler
    bx    r0
    .size LPUART8_IRQHandler, . - LPUART8_IRQHandler

    .align 1
    .thumb_func
    .weak LPI2C1_IRQHandler
    .type LPI2C1_IRQHandler, %function
LPI2C1_IRQHandler:
    ldr   r0,=LPI2C1_DriverIRQHandler
    bx    r0
    .size LPI2C1_IRQHandler, . - LPI2C1_IRQHandler

    .align 1
    .thumb_func
    .weak LPI2C2_IRQHandler
    .type LPI2C2_IRQHandler, %function
LPI2C2_IRQHandler:
    ldr   r0,=LPI2C2_DriverIRQHandler
    bx    r0
    .size LPI2C2_IRQHandler, . - LPI2C2_IRQHandler

    .align 1
    .thumb_func
    .weak LPI2C3_IRQHandler
    .type LPI2C3_IRQHandler, %function
LPI2C3_IRQHandler:
    ldr   r0,=LPI2C3_DriverIRQHandler
    bx    r0
    .size LPI2C3_IRQHandler, . - LPI2C3_IRQHandler

    .align 1
    .thumb_func
    .weak LPI2C4_IRQHandler
    .type LPI2C4_IRQHandler, %function
LPI2C4_IRQHandler:
    ldr   r0,=LPI2C4_DriverIRQHandler
    bx    r0
    .size LPI2C4_IRQHandler, . - LPI2C4_IRQHandler

    .align 1
    .thumb_func
    .weak LPSPI1_IRQHandler
    .type LPSPI1_IRQHandler, %function
LPSPI1_IRQHandler:
    ldr   r0,=LPSPI1_DriverIRQHandler
    bx    r0
    .size LPSPI1_IRQHandler, . - LPSPI1_IRQHandler

    .align 1
    .thumb_func
    .weak LPSPI2_IRQHandler
    .type LPSPI2_IRQHandler, %function
LPSPI2_IRQHandler:
    ldr   r0,=LPSPI2_DriverIRQHandler
    bx    r0
    .size LPSPI2_IRQHandler, . - LPSPI2_IRQHandler

    .align 1
    .thumb_func
    .weak LPSPI3_IRQHandler
    .type LPSPI3_IRQHandler, %function
LPSPI3_IRQHandler:
    ldr   r0,=LPSPI3_DriverIRQHandler
    bx    r0
    .size LPSPI3_IRQHandler, . - LPSPI3_IRQHandler

    .align 1
    .thumb_func
    .weak LPSPI4_IRQHandler
    .type LPSPI4_IRQHandler, %function
LPSPI4_IRQHandler:
    ldr   r0,=LPSPI4_DriverIRQHandler
    bx    r0
    .size LPSPI4_IRQHandler, . - LPSPI4_IRQHandler

    .align 1
    .thumb_func
    .weak CAN1_IRQHandler
    .type CAN1_IRQHandler, %function
CAN1_IRQHandler:
    ldr   r0,=CAN1_DriverIRQHandler
    bx    r0
    .size CAN1_IRQHandler, . - CAN1_IRQHandler

    .align 1
    .thumb_func
    .weak CAN2_IRQHandler
    .type CAN2_IRQHandler, %function
CAN2_IRQHandler:
    ldr   r0,=CAN2_DriverIRQHandler
    bx    r0
    .size CAN2_IRQHandler, . - CAN2_IRQHandler

    .align 1
    .thumb_func
    .weak SPDIF_IRQHandler
    .type SPDIF_IRQHandler, %function
SPDIF_IRQHandler:
    ldr   r0,=SPDIF_DriverIRQHandler
    bx    r0
    .size SPDIF_IRQHandler, . - SPDIF_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXIO1_IRQHandler
    .type FLEXIO1_IRQHandler, %function
FLEXIO1_IRQHandler:
    ldr   r0,=FLEXIO1_DriverIRQHandler
    bx    r0
    .size FLEXIO1_IRQHandler, . - FLEXIO1_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXIO2_IRQHandler
    .type FLEXIO2_IRQHandler, %function
FLEXIO2_IRQHandler:
    ldr   r0,=FLEXIO2_DriverIRQHandler
    bx    r0
    .size FLEXIO2_IRQHandler, . - FLEXIO2_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXSPI_IRQHandler
    .type FLEXSPI_IRQHandler, %function
FLEXSPI_IRQHandler:
    ldr   r0,=FLEXSPI_DriverIRQHandler
    bx    r0
    .size FLEXSPI_IRQHandler, . - FLEXSPI_IRQHandler

    .align 1
    .thumb_func
    .weak USDHC1_IRQHandler
    .type USDHC1_IRQHandler, %function
USDHC1_IRQHandler:
    ldr   r0,=USDHC1_DriverIRQHandler
    bx    r0
    .size USDHC1_IRQHandler, . - USDHC1_IRQHandler

    .align 1
    .thumb_func
    .weak USDHC2_IRQHandler
    .type USDHC2_IRQHandler, %function
USDHC2_IRQHandler:
    ldr   r0,=USDHC2_DriverIRQHandler
    bx    r0
    .size USDHC2_IRQHandler, . - USDHC2_IRQHandler

    .align 1
    .thumb_func
    .weak ENET_IRQHandler
    .type ENET_IRQHandler, %function
ENET_IRQHandler:
    ldr   r0,=ENET_DriverIRQHandler
    bx    r0
    .size ENET_IRQHandler, . - ENET_IRQHandler

    .align 1
    .thumb_func
    .weak ENET_1588_Timer_IRQHandler
    .type ENET_1588_Timer_IRQHandler, %function
ENET_1588_Timer_IRQHandler:
    ldr   r0,=ENET_1588_Timer_DriverIRQHandler
    bx    r0
    .size ENET_1588_Timer_IRQHandler, . - ENET_1588_Timer_IRQHandler


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    MemManage_Handler
    def_irq_handler    BusFault_Handler
    def_irq_handler    UsageFault_Handler
    def_irq_handler    DebugMon_Handler
    def_irq_handler    Reserved16_IRQHandler
    def_irq_handler    Reserved17_IRQHandler
    def_irq_handler    Reserved18_IRQHandler
    def_irq_handler    Reserved19_IRQHandler
    def_irq_handler    Reserved20_IRQHandler
    def_irq_handler    Reserved21_IRQHandler
    def_irq_handler    Reserved22_IRQHandler
    def_irq_handler    Reserved23_IRQHandler
    def_irq_handler    Reserved24_IRQHandler
    def_irq_handler    Reserved25_IRQHandler
    def_irq_handler    Reserved26_IRQHandler
    def_irq_handler    Reserved27_IRQHandler
    def_irq_handler    Reserved28_IRQHandler
    def_irq_handler    Reserved29_IRQHandler
    def_irq_handler    Reserved30_IRQHandler
    def_irq_handler    Reserved31_IRQHandler
    def_irq_handler    Reserved32_IRQHandler
    def_irq_handler    CTI0_ERROR_IRQHandler
    def_irq_handler    CTI1_ERROR_IRQHandler
    def_irq_handler    CORE_IRQHandler
    def_irq_handler    LPUART1_DriverIRQHandler
    def_irq_handler    LPUART2_DriverIRQHandler
    def_irq_handler    LPUART3_DriverIRQHandler
    def_irq_handler    LPUART4_DriverIRQHandler
    def_irq_handler    LPUART5_DriverIRQHandler
    def_irq_handler    LPUART6_DriverIRQHandler
    def_irq_handler    LPUART7_DriverIRQHandler
    def_irq_handler    LPUART8_DriverIRQHandler
    def_irq_handler    LPI2C1_DriverIRQHandler
    def_irq_handler    LPI2C2_DriverIRQHandler
    def_irq_handler    LPI2C3_DriverIRQHandler
    def_irq_handler    LPI2C4_DriverIRQHandler
    def_irq_handler    LPSPI1_DriverIRQHandler
    def_irq_handler    LPSPI2_DriverIRQHandler
    def_irq_handler    LPSPI3_DriverIRQHandler
    def_irq_handler    LPSPI4_DriverIRQHandler
    def_irq_handler    CAN1_DriverIRQHandler
    def_irq_handler    CAN2_DriverIRQHandler
    def_irq_handler    FLEXRAM_IRQHandler
    def_irq_handler    KPP_IRQHandler
    def_irq_handler    TSC_DIG_IRQHandler
    def_irq_handler    GPR_IRQ_IRQHandler
    def_irq_handler    Reserved58_IRQHandler
    def_irq_handler    Reserved59_IRQHandler
    def_irq_handler    Reserved60_IRQHandler
    def_irq_handler    WDOG2_IRQHandler
    def_irq_handler    SNVS_HP_WRAPPER_IRQHandler
    def_irq_handler    SNVS_HP_WRAPPER_TZ_IRQHandler
    def_irq_handler    SNVS_LP_WRAPPER_IRQHandler
    def_irq_handler    Reserved65_IRQHandler
    def_irq_handler    DCP_IRQHandler
    def_irq_handler    DCP_VMI_IRQHandler
    def_irq_handler    DCP_SECURE_IRQHandler
    def_irq_handler    TRNG_IRQHandler
    def_irq_handler    Reserved70_IRQHandler
    def_irq_handler    BEE_IRQHandler
    def_irq_handler    Reserved72_IRQHandler
    def_irq_handler    Reserved73_IRQHandler
    def_irq_handler    Reserved74_IRQHandler
    def_irq_handler    Reserved75_IRQHandler
    def_irq_handler    SPDIF_DriverIRQHandler
    def_irq_handler    Reserved77_IRQHandler
    def_irq_handler    Reserved78_IRQHandler
    def_irq_handler    Reserved79_IRQHandler
    def_irq_handler    Reserved80_IRQHandler
    def_irq_handler    USB_PHY1_IRQHandler
    def_irq_handler    USB_PHY2_IRQHandler
    def_irq_handler    ADC1_IRQHandler
    def_irq_handler    ADC_5HC_IRQHandler
    def_irq_handler    DCDC_IRQHandler
    def_irq_handler    Reserved86_IRQHandler
    def_irq_handler    Reserved87_IRQHandler
    def_irq_handler    GPIO1_INT0_IRQHandler
    def_irq_handler    GPIO1_INT1_IRQHandler
    def_irq_handler    GPIO1_INT2_IRQHandler
    def_irq_handler    GPIO1_INT3_IRQHandler
    def_irq_handler    GPIO1_INT4_IRQHandler
    def_irq_handler    GPIO1_INT5_IRQHandler
    def_irq_handler    GPIO1_INT6_IRQHandler
    def_irq_handler    GPIO1_INT7_IRQHandler
    def_irq_handler    GPIO1_Combined_0_15_IRQHandler
    def_irq_handler    GPIO1_Combined_16_31_IRQHandler
    def_irq_handler    GPIO2_Combined_0_15_IRQHandler
    def_irq_handler    GPIO2_Combined_16_31_IRQHandler
    def_irq_handler    GPIO3_Combined_0_15_IRQHandler
    def_irq_handler    GPIO3_Combined_16_31_IRQHandler
    def_irq_handler    GPIO4_Combined_0_15_IRQHandler
    def_irq_handler    GPIO4_Combined_16_31_IRQHandler
    def_irq_handler    GPIO5_Combined_0_15_IRQHandler
    def_irq_handler    GPIO5_Combined_16_31_IRQHandler
    def_irq_handler    FLEXIO1_DriverIRQHandler
    def_irq_handler    FLEXIO2_DriverIRQHandler
    def_irq_handler    WDOG1_IRQHandler
    def_irq_handler    RTWDOG_IRQHandler
    def_irq_handler    EWM_IRQHandler
    def_irq_handler    CCM_1_IRQHandler
    def_irq_handler    CCM_2_IRQHandler
    def_irq_handler    GPC_IRQHandler
    def_irq_handler    SRC_IRQHandler
    def_irq_handler    Reserved115_IRQHandler
    def_irq_handler    GPT1_IRQHandler
    def_irq_handler    GPT2_IRQHandler
    def_irq_handler    PWM1_0_IRQHandler
    def_irq_handler    PWM1_1_IRQHandler
    def_irq_handler    PWM1_2_IRQHandler
    def_irq_handler    PWM1_3_IRQHandler
    def_irq_handler    PWM1_FAULT_IRQHandler
    def_irq_handler    Reserved123_IRQHandler
    def_irq_handler    FLEXSPI_DriverIRQHandler
    def_irq_handler    SEMC_IRQHandler
    def_irq_handler    USDHC1_DriverIRQHandler
    def_irq_handler    USDHC2_DriverIRQHandler
    def_irq_handler    USB_OTG2_IRQHandler
    def_irq_handler    USB_OTG1_IRQHandler
    def_irq_handler    ENET_DriverIRQHandler
    def_irq_handler    ENET_1588_Timer_DriverIRQHandler
    def_irq_handler    Reserved132_IRQHandler
    def_irq_handler    Reserved133_IRQHandler
    def_irq_handler    ADC_ETC_IRQ0_IRQHandler
    def_irq_handler    ADC_ETC_IRQ1_IRQHandler
    def_irq_handler    ADC_ETC_IRQ2_IRQHandler
    def_irq_handler    ADC_ETC_ERROR_IRQ_IRQHandler
    def_irq_handler    PIT_IRQHandler
    def_irq_handler    ACMP_IRQ1_IRQHandler
    def_irq_handler    ACMP_IRQ2_IRQHandler
    def_irq_handler    ACMP_IRQ3_IRQHandler
    def_irq_handler    ACMP_IRQ4_IRQHandler
    def_irq_handler    Reserved143_IRQHandler
    def_irq_handler    Reserved144_IRQHandler
    def_irq_handler    ENC1_IRQHandler
    def_irq_handler    ENC2_IRQHandler
    def_irq_handler    ENC3_IRQHandler
    def_irq_handler    ENC4_IRQHandler
    def_irq_handler    TMR1_IRQHandler
    def_irq_handler    TMR2_IRQHandler
    def_irq_handler    TMR3_IRQHandler
    def_irq_handler    TMR4_IRQHandler
    def_irq_handler    PWM2_0_IRQHandler
    def_irq_handler    PWM2_1_IRQHandler
    def_irq_handler    PWM2_2_IRQHandler
    def_irq_handler    PWM2_3_IRQHandler
    def_irq_handler    PWM2_FAULT_IRQHandler
    def_irq_handler    PWM3_0_IRQHandler
    def_irq_handler    PWM3_1_IRQHandler
    def_irq_handler    PWM3_2_IRQHandler
    def_irq_handler    PWM3_3_IRQHandler
    def_irq_handler    PWM3_FAULT_IRQHandler
    def_irq_handler    PWM4_0_IRQHandler
    def_irq_handler    PWM4_1_IRQHandler
    def_irq_handler    PWM4_2_IRQHandler
    def_irq_handler    PWM4_3_IRQHandler
    def_irq_handler    PWM4_FAULT_IRQHandler

    .end
