/* ------------------------------------------------------------------------- */
/*  @file:    startup_MKV11Z7.s                                              */
/*  @purpose: CMSIS Cortex-M0P Core Device Startup File                      */
/*            MKV11Z7                                                        */
/*  @version: 1.1                                                            */
/*  @date:    2017-3-22                                                      */
/*  @build:   b190916                                                        */
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
    .eabi_attribute Tag_ABI_align_preserved, 1 /*8-byte alignment */

    .section .isr_vector, "a"
    .align 2
    .globl __Vectors
__Vectors:
    .long   Image$$ARM_LIB_STACK$$ZI$$Limit                 /* Top of Stack */
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
    .long   DMA0_DMA4_IRQHandler                            /* DMA channel 0 and 4 transfer complete*/
    .long   DMA1_DMA5_IRQHandler                            /* DMA channel 1 and 5 transfer complete*/
    .long   DMA2_DMA6_IRQHandler                            /* DMA channel 2 and 6 transfer complete*/
    .long   DMA3_DMA7_IRQHandler                            /* DMA channel 2 and 7 transfer complete*/
    .long   DMA_Error_IRQHandler                            /* DMA error interrupt channels 0-7*/
    .long   FTFA_IRQHandler                                 /* FTFA command complete and read collision*/
    .long   LVD_LVW_IRQHandler                              /* Low-voltage detect, low-voltage warning*/
    .long   LLWU_IRQHandler                                 /* Low Leakage Wakeup*/
    .long   I2C0_IRQHandler                                 /* I2C0 interrupt*/
    .long   Reserved25_IRQHandler                           /* Reserved interrupt*/
    .long   SPI0_IRQHandler                                 /* SPI0 single interrupt vector for all sources*/
    .long   Reserved27_IRQHandler                           /* Reserved interrupt*/
    .long   UART0_IRQHandler                                /* UART0 status and error*/
    .long   UART1_IRQHandler                                /* UART1 status and error*/
    .long   CAN0_IRQHandler                                 /* CAN0 interrupt*/
    .long   ADC0_IRQHandler                                 /* ADC0 interrupt*/
    .long   ADC1_IRQHandler                                 /* ADC1 interrupt*/
    .long   FTM0_IRQHandler                                 /* FTM0 single interrupt vector for all sources*/
    .long   FTM1_IRQHandler                                 /* FTM1 single interrupt vector for all sources*/
    .long   FTM2_IRQHandler                                 /* FTM2 single interrupt vector for all sources*/
    .long   CMP0_IRQHandler                                 /* CMP0 interrupt*/
    .long   CMP1_IRQHandler                                 /* CMP1 interrupt*/
    .long   FTM3_IRQHandler                                 /* FTM3 single interrupt vector for all sources*/
    .long   WDOG_EWM_IRQHandler                             /* Single interrupt vector for  WDOG and EWM*/
    .long   FTM4_IRQHandler                                 /* FTM4 single interrupt vector for all sources*/
    .long   DAC0_IRQHandler                                 /* DAC0 interrupt*/
    .long   FTM5_IRQHandler                                 /* FTM5 single interrupt vector for all sources*/
    .long   MCG_IRQHandler                                  /* MCG interrupt*/
    .long   LPTMR0_IRQHandler                               /* LPTMR0 interrupt*/
    .long   PDB0_PDB1_IRQHandler                            /* Single interrupt vector for  PDB0 and PDB1*/
    .long   PORTA_IRQHandler                                /* PORTA pin detect*/
    .long   PORTB_PORTC_PORTD_PORTE_IRQHandler              /* Single interrupt vector for PORTB, PORTC, PORTD and PORTE pin detect*/
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

    .size    __Vectors, . - __Vectors

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
    ldr     r1, =__Vectors
    str     r1, [r0]
    ldr     r2, [r1]
    msr     msp, r2
    ldr   r0,=SystemInit
    blx   r0
    ldr   r0,=init_data_bss
    blx   r0
    cpsie   i               /* Unmask interrupts */
    ldr   r0,=__main
    bx    r0

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
    .weak I2C0_IRQHandler
    .type I2C0_IRQHandler, %function
I2C0_IRQHandler:
    ldr   r0,=I2C0_DriverIRQHandler
    bx    r0
    .size I2C0_IRQHandler, . - I2C0_IRQHandler

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
    .weak UART0_IRQHandler
    .type UART0_IRQHandler, %function
UART0_IRQHandler:
    ldr   r0,=UART0_DriverIRQHandler
    bx    r0
    .size UART0_IRQHandler, . - UART0_IRQHandler

    .align 1
    .thumb_func
    .weak UART1_IRQHandler
    .type UART1_IRQHandler, %function
UART1_IRQHandler:
    ldr   r0,=UART1_DriverIRQHandler
    bx    r0
    .size UART1_IRQHandler, . - UART1_IRQHandler

    .align 1
    .thumb_func
    .weak CAN0_IRQHandler
    .type CAN0_IRQHandler, %function
CAN0_IRQHandler:
    ldr   r0,=CAN0_DriverIRQHandler
    bx    r0
    .size CAN0_IRQHandler, . - CAN0_IRQHandler


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    DMA0_DMA4_DriverIRQHandler
    def_irq_handler    DMA1_DMA5_DriverIRQHandler
    def_irq_handler    DMA2_DMA6_DriverIRQHandler
    def_irq_handler    DMA3_DMA7_DriverIRQHandler
    def_irq_handler    DMA_Error_DriverIRQHandler
    def_irq_handler    FTFA_IRQHandler
    def_irq_handler    LVD_LVW_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    I2C0_DriverIRQHandler
    def_irq_handler    Reserved25_IRQHandler
    def_irq_handler    SPI0_DriverIRQHandler
    def_irq_handler    Reserved27_IRQHandler
    def_irq_handler    UART0_DriverIRQHandler
    def_irq_handler    UART1_DriverIRQHandler
    def_irq_handler    CAN0_DriverIRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    ADC1_IRQHandler
    def_irq_handler    FTM0_IRQHandler
    def_irq_handler    FTM1_IRQHandler
    def_irq_handler    FTM2_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    CMP1_IRQHandler
    def_irq_handler    FTM3_IRQHandler
    def_irq_handler    WDOG_EWM_IRQHandler
    def_irq_handler    FTM4_IRQHandler
    def_irq_handler    DAC0_IRQHandler
    def_irq_handler    FTM5_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    LPTMR0_IRQHandler
    def_irq_handler    PDB0_PDB1_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_PORTC_PORTD_PORTE_IRQHandler

    .end
