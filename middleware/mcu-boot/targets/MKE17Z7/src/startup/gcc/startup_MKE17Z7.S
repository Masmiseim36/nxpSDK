/* ------------------------------------------------------------------------- */
/*  @file:    startup_MKE17Z7.s                                              */
/*  @purpose: CMSIS Cortex-M0P Core Device Startup File                      */
/*            MKE17Z7                                                        */
/*  @version: 1.0                                                            */
/*  @date:    2020-12-10                                                     */
/*  @build:   b201210                                                        */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Copyright 1997-2016 Freescale Semiconductor, Inc.                         */
/* Copyright 2016-2020 NXP                                                   */
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
    .long   DMA0_04_IRQHandler                              /* DMA channel 0 or 4 transfer complete*/
    .long   DMA0_15_IRQHandler                              /* DMA channel 1 or 5 transfer complete*/
    .long   DMA0_26_IRQHandler                              /* DMA channel 2 or 6 transfer complete*/
    .long   DMA0_37_IRQHandler                              /* DMA channel 3 or 7 transfer complete*/
    .long   DMA_Error_IRQHandler                            /* DMA error interrupt channels 0-7*/
    .long   FTFA_IRQHandler                                 /* Single interrupt vector for all sources*/
    .long   LVD_LVW_IRQHandler                              /* Low-voltage detect, low-voltage warning*/
    .long   PORTAE_IRQHandler                               /* Pin detect (Port A, E)*/
    .long   LPI2C0_IRQHandler                               /* Inter-integrated circuit 0 interrupt*/
    .long   Reserved25_IRQHandler                           /* Reserved interrupt*/
    .long   LPSPI0_IRQHandler                               /* Serial peripheral Interface 0 interrupt*/
    .long   Reserved27_IRQHandler                           /* Reserved interrupt*/
    .long   LPUART0_IRQHandler                              /* Single interrupt vector for all sources*/
    .long   LPUART1_IRQHandler                              /* Single interrupt vector for all sources*/
    .long   LPUART2_IRQHandler                              /* Single interrupt vector for all sources*/
    .long   ADC0_IRQHandler                                 /* ADC0 conversion complete interrupt*/
    .long   CMP0_IRQHandler                                 /* CMP0 interrupt*/
    .long   FTM0_IRQHandler                                 /* FTM0 single interrupt vector for all sources*/
    .long   FTM1_IRQHandler                                 /* FTM1 single interrupt vector for all sources*/
    .long   FTM2_IRQHandler                                 /* FTM2 single interrupt vector for all sources*/
    .long   Reserved36_IRQHandler                           /* Reserved interrupt*/
    .long   Reserved37_IRQHandler                           /* Reserved interrupt*/
    .long   LPIT0_IRQHandler                                /* LPIT interrupt*/
    .long   FLEXIO_IRQHandler                               /* FLEXIO interrupt*/
    .long   TSI0_IRQHandler                                 /* TSI0 interrupt*/
    .long   TSI1_IRQHandler                                 /* TSI1 interrupt*/
    .long   PORTBCD_IRQHandler                              /* Pin detect (Port B, C, D)*/
    .long   SCG_IRQHandler                                  /* System clock generator interrupt*/
    .long   WDOG_EWM_IRQHandler                             /* Single interrupt vector for WDOG and EWM*/
    .long   PWT_LPTMR0_IRQHandler                           /* Single interrupt vector for PWT and LPTMR0*/
    .long   Reserved46_IRQHandler                           /* Reserved interrupt*/
    .long   RCM_IRQHandler                                  /* RCM interrupt*/
    .long   DefaultISR                                      /* 48*/
    .long   DefaultISR                                      /* 49*/
    .long   DefaultISR                                      /* 50*/
    .long   DefaultISR                                      /* 51*/
    .long   DefaultISR                                      /* 52*/
    .long   DefaultISR                                      /* 53*/
    .long   DefaultISR                                      /* 54*/
    .long   DefaultISR                                      /* 55*/
    .long   DefaultISR                                      /* 56*/
    .long   DefaultISR                                      /* 57*/
    .long   DefaultISR                                      /* 58*/
    .long   DefaultISR                                      /* 59*/
    .long   DefaultISR                                      /* 60*/
    .long   DefaultISR                                      /* 61*/
    .long   DefaultISR                                      /* 62*/
    .long   DefaultISR                                      /* 63*/
    .long   DefaultISR                                      /* 64*/
    .long   DefaultISR                                      /* 65*/
    .long   DefaultISR                                      /* 66*/
    .long   DefaultISR                                      /* 67*/
    .long   DefaultISR                                      /* 68*/
    .long   DefaultISR                                      /* 69*/
    .long   DefaultISR                                      /* 70*/
    .long   DefaultISR                                      /* 71*/
    .long   DefaultISR                                      /* 72*/
    .long   DefaultISR                                      /* 73*/
    .long   DefaultISR                                      /* 74*/
    .long   DefaultISR                                      /* 75*/
    .long   DefaultISR                                      /* 76*/
    .long   DefaultISR                                      /* 77*/
    .long   DefaultISR                                      /* 78*/
    .long   DefaultISR                                      /* 79*/
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
    .weak DMA0_04_IRQHandler
    .type DMA0_04_IRQHandler, %function
DMA0_04_IRQHandler:
    ldr   r0,=DMA0_04_DriverIRQHandler
    bx    r0
    .size DMA0_04_IRQHandler, . - DMA0_04_IRQHandler

    .align 1
    .thumb_func
    .weak DMA0_15_IRQHandler
    .type DMA0_15_IRQHandler, %function
DMA0_15_IRQHandler:
    ldr   r0,=DMA0_15_DriverIRQHandler
    bx    r0
    .size DMA0_15_IRQHandler, . - DMA0_15_IRQHandler

    .align 1
    .thumb_func
    .weak DMA0_26_IRQHandler
    .type DMA0_26_IRQHandler, %function
DMA0_26_IRQHandler:
    ldr   r0,=DMA0_26_DriverIRQHandler
    bx    r0
    .size DMA0_26_IRQHandler, . - DMA0_26_IRQHandler

    .align 1
    .thumb_func
    .weak DMA0_37_IRQHandler
    .type DMA0_37_IRQHandler, %function
DMA0_37_IRQHandler:
    ldr   r0,=DMA0_37_DriverIRQHandler
    bx    r0
    .size DMA0_37_IRQHandler, . - DMA0_37_IRQHandler

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
    .weak LPI2C0_IRQHandler
    .type LPI2C0_IRQHandler, %function
LPI2C0_IRQHandler:
    ldr   r0,=LPI2C0_DriverIRQHandler
    bx    r0
    .size LPI2C0_IRQHandler, . - LPI2C0_IRQHandler

    .align 1
    .thumb_func
    .weak LPSPI0_IRQHandler
    .type LPSPI0_IRQHandler, %function
LPSPI0_IRQHandler:
    ldr   r0,=LPSPI0_DriverIRQHandler
    bx    r0
    .size LPSPI0_IRQHandler, . - LPSPI0_IRQHandler

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
    .weak LPUART2_IRQHandler
    .type LPUART2_IRQHandler, %function
LPUART2_IRQHandler:
    ldr   r0,=LPUART2_DriverIRQHandler
    bx    r0
    .size LPUART2_IRQHandler, . - LPUART2_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXIO_IRQHandler
    .type FLEXIO_IRQHandler, %function
FLEXIO_IRQHandler:
    ldr   r0,=FLEXIO_DriverIRQHandler
    bx    r0
    .size FLEXIO_IRQHandler, . - FLEXIO_IRQHandler


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    DMA0_04_DriverIRQHandler
    def_irq_handler    DMA0_15_DriverIRQHandler
    def_irq_handler    DMA0_26_DriverIRQHandler
    def_irq_handler    DMA0_37_DriverIRQHandler
    def_irq_handler    DMA_Error_DriverIRQHandler
    def_irq_handler    FTFA_IRQHandler
    def_irq_handler    LVD_LVW_IRQHandler
    def_irq_handler    PORTAE_IRQHandler
    def_irq_handler    LPI2C0_DriverIRQHandler
    def_irq_handler    Reserved25_IRQHandler
    def_irq_handler    LPSPI0_DriverIRQHandler
    def_irq_handler    Reserved27_IRQHandler
    def_irq_handler    LPUART0_DriverIRQHandler
    def_irq_handler    LPUART1_DriverIRQHandler
    def_irq_handler    LPUART2_DriverIRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    FTM0_IRQHandler
    def_irq_handler    FTM1_IRQHandler
    def_irq_handler    FTM2_IRQHandler
    def_irq_handler    Reserved36_IRQHandler
    def_irq_handler    Reserved37_IRQHandler
    def_irq_handler    LPIT0_IRQHandler
    def_irq_handler    FLEXIO_DriverIRQHandler
    def_irq_handler    TSI0_IRQHandler
    def_irq_handler    TSI1_IRQHandler
    def_irq_handler    PORTBCD_IRQHandler
    def_irq_handler    SCG_IRQHandler
    def_irq_handler    WDOG_EWM_IRQHandler
    def_irq_handler    PWT_LPTMR0_IRQHandler
    def_irq_handler    Reserved46_IRQHandler
    def_irq_handler    RCM_IRQHandler

    .end
