/* ------------------------------------------------------------------------- */
/*  @file:    startup_MKL82Z7.s                                              */
/*  @purpose: CMSIS Cortex-M0P Core Device Startup File                      */
/*            MKL82Z7                                                        */
/*  @version: 1.5                                                            */
/*  @date:    2015-9-24                                                      */
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

    .size    __isr_vector, . - __isr_vector


 /* Bootloader Config Area */
    .section .BootloaderConfig, "a"
#ifdef BL_HAS_BOOTLOADER_CONFIG

//__bootloaderConfigurationArea ; 0x3c0
    .long     0x6766636b    // [00:03] tag - Tag value used to validate the bootloader configuration data. Must be set to 'kcfg'
    .long     0xFFFFFFFF    // [04:07] crcStartAddress
    .long     0xFFFFFFFF    // [08:0b] crcByteCount
    .long     0xFFFFFFFF    // [0c:0f] crcExpectedValue
    .byte     0xFF          // [10:10] enabledPeripherals
    .byte     0xFF          // [11:11] i2cSlaveAddress
    .short    5000          // [12:13] peripheralDetectionTimeoutMs - Timeout in milliseconds for peripheral detection before jumping to application code
    .short    0xFFFF       // [14:15] usbVid
    .short    0xFFFF       // [16:17] usbPid
    .long     0xFFFFFFFF    // [18:1b] usbStringsPointer
    .byte     0xFF          // [1c:1c] clockFlags - High Speed and other clock options
    .byte     0xFF          // [1d:1d] clockDivider - One's complement of clock divider, zero divider is divide by 1
    .short    0xFFFF        // [1e:1f] reserved
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
    .equ    VTOR, 0xE000ED08
    ldr     r0, =VTOR
    ldr     r1, =__isr_vector
    str     r1, [r0]
#ifndef __NO_SYSTEM_INIT
    ldr   r0,=SystemInit
    blx   r0
#endif
    ldr   r0,=init_data_bss
    blx   r0

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

    .align 1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    ldr r0, =DefaultISR
    bx r0
    .size DefaultISR, . - DefaultISR

    .align 1
    .thumb_func
    .weak DMA0_DMA4_IRQHandler
    .type DMA0_DMA4_IRQHandler, %function
DMA0_DMA4_IRQHandler:
    ldr   r0,=DMA0_DMA4_DriverIRQHandler
    bx    r0
    .size DMA0_DMA4_IRQHandler, . - DMA0_DMA4_IRQHandler

    .align 1
    .thumb_func
    .weak DMA1_DMA5_IRQHandler
    .type DMA1_DMA5_IRQHandler, %function
DMA1_DMA5_IRQHandler:
    ldr   r0,=DMA1_DMA5_DriverIRQHandler
    bx    r0
    .size DMA1_DMA5_IRQHandler, . - DMA1_DMA5_IRQHandler

    .align 1
    .thumb_func
    .weak DMA2_DMA6_IRQHandler
    .type DMA2_DMA6_IRQHandler, %function
DMA2_DMA6_IRQHandler:
    ldr   r0,=DMA2_DMA6_DriverIRQHandler
    bx    r0
    .size DMA2_DMA6_IRQHandler, . - DMA2_DMA6_IRQHandler

    .align 1
    .thumb_func
    .weak DMA3_DMA7_IRQHandler
    .type DMA3_DMA7_IRQHandler, %function
DMA3_DMA7_IRQHandler:
    ldr   r0,=DMA3_DMA7_DriverIRQHandler
    bx    r0
    .size DMA3_DMA7_IRQHandler, . - DMA3_DMA7_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_Error_IRQHandler
    .type DMA_Error_IRQHandler, %function
DMA_Error_IRQHandler:
    ldr   r0,=DMA_Error_DriverIRQHandler
    bx    r0
    .size DMA_Error_IRQHandler, . - DMA_Error_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXIO0_IRQHandler
    .type FLEXIO0_IRQHandler, %function
FLEXIO0_IRQHandler:
    ldr   r0,=FLEXIO0_DriverIRQHandler
    bx    r0
    .size FLEXIO0_IRQHandler, . - FLEXIO0_IRQHandler

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
    .weak LPUART1_IRQHandler
    .type LPUART1_IRQHandler, %function
LPUART1_IRQHandler:
    ldr   r0,=LPUART1_DriverIRQHandler
    bx    r0
    .size LPUART1_IRQHandler, . - LPUART1_IRQHandler

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
    .weak QSPI0_IRQHandler
    .type QSPI0_IRQHandler, %function
QSPI0_IRQHandler:
    ldr   r0,=QSPI0_DriverIRQHandler
    bx    r0
    .size QSPI0_IRQHandler, . - QSPI0_IRQHandler


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler
    def_irq_handler    DMA0_DMA4_DriverIRQHandler
    def_irq_handler    DMA1_DMA5_DriverIRQHandler
    def_irq_handler    DMA2_DMA6_DriverIRQHandler
    def_irq_handler    DMA3_DMA7_DriverIRQHandler
    def_irq_handler    DMA_Error_DriverIRQHandler
    def_irq_handler    FLEXIO0_DriverIRQHandler
    def_irq_handler    TPM0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    PIT0_IRQHandler
    def_irq_handler    SPI0_DriverIRQHandler
    def_irq_handler    EMVSIM0_IRQHandler
    def_irq_handler    LPUART0_DriverIRQHandler
    def_irq_handler    LPUART1_DriverIRQHandler
    def_irq_handler    I2C0_DriverIRQHandler
    def_irq_handler    QSPI0_DriverIRQHandler
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
