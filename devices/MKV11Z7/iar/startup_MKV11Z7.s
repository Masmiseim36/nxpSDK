; -------------------------------------------------------------------------
;  @file:    startup_MKV11Z7.s
;  @purpose: CMSIS Cortex-M0P Core Device Startup File
;            MKV11Z7
;  @version: 1.1
;  @date:    2017-3-22
;  @build:   b190124
; -------------------------------------------------------------------------
;
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2019 NXP
; All rights reserved.
;
; SPDX-License-Identifier: BSD-3-Clause
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
__vector_table_0x1c
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     DMA0_DMA4_IRQHandler                          ;DMA channel 0 and 4 transfer complete
        DCD     DMA1_DMA5_IRQHandler                          ;DMA channel 1 and 5 transfer complete
        DCD     DMA2_DMA6_IRQHandler                          ;DMA channel 2 and 6 transfer complete
        DCD     DMA3_DMA7_IRQHandler                          ;DMA channel 2 and 7 transfer complete
        DCD     DMA_Error_IRQHandler                          ;DMA error interrupt channels 0-7
        DCD     FTFA_IRQHandler                               ;FTFA command complete and read collision
        DCD     LVD_LVW_IRQHandler                            ;Low-voltage detect, low-voltage warning
        DCD     LLWU_IRQHandler                               ;Low Leakage Wakeup
        DCD     I2C0_IRQHandler                               ;I2C0 interrupt
        DCD     Reserved25_IRQHandler                         ;Reserved interrupt
        DCD     SPI0_IRQHandler                               ;SPI0 single interrupt vector for all sources
        DCD     Reserved27_IRQHandler                         ;Reserved interrupt
        DCD     UART0_IRQHandler                              ;UART0 status and error
        DCD     UART1_IRQHandler                              ;UART1 status and error
        DCD     CAN0_IRQHandler                               ;CAN0 interrupt
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     ADC1_IRQHandler                               ;ADC1 interrupt
        DCD     FTM0_IRQHandler                               ;FTM0 single interrupt vector for all sources
        DCD     FTM1_IRQHandler                               ;FTM1 single interrupt vector for all sources
        DCD     FTM2_IRQHandler                               ;FTM2 single interrupt vector for all sources
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt
        DCD     CMP1_IRQHandler                               ;CMP1 interrupt
        DCD     FTM3_IRQHandler                               ;FTM3 single interrupt vector for all sources
        DCD     WDOG_EWM_IRQHandler                           ;Single interrupt vector for  WDOG and EWM
        DCD     FTM4_IRQHandler                               ;FTM4 single interrupt vector for all sources
        DCD     DAC0_IRQHandler                               ;DAC0 interrupt
        DCD     FTM5_IRQHandler                               ;FTM5 single interrupt vector for all sources
        DCD     MCG_IRQHandler                                ;MCG interrupt
        DCD     LPTMR0_IRQHandler                             ;LPTMR0 interrupt
        DCD     PDB0_PDB1_IRQHandler                          ;Single interrupt vector for  PDB0 and PDB1
        DCD     PORTA_IRQHandler                              ;PORTA pin detect
        DCD     PORTB_PORTC_PORTD_PORTE_IRQHandler            ;Single interrupt vector for PORTB, PORTC, PORTD and PORTE pin detect
__Vectors_End

        SECTION FlashConfig:CODE
__FlashConfig
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFE
__FlashConfig_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        CPSID   I               ; Mask interrupts
        LDR     R0, =0xE000ED08
        LDR     R1, =__vector_table
        STR     R1, [R0]
        LDR     R2, [R1]
        MSR     MSP, R2
        LDR     R0, =SystemInit
        BLX     R0
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK DMA0_DMA4_IRQHandler
        PUBWEAK DMA0_DMA4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DMA4_IRQHandler
        LDR     R0, =DMA0_DMA4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_DMA5_IRQHandler
        PUBWEAK DMA1_DMA5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_DMA5_IRQHandler
        LDR     R0, =DMA1_DMA5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA2_DMA6_IRQHandler
        PUBWEAK DMA2_DMA6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA2_DMA6_IRQHandler
        LDR     R0, =DMA2_DMA6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_DMA7_IRQHandler
        PUBWEAK DMA3_DMA7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_DMA7_IRQHandler
        LDR     R0, =DMA3_DMA7_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_Error_IRQHandler
        PUBWEAK DMA_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_Error_IRQHandler
        LDR     R0, =DMA_Error_DriverIRQHandler
        BX      R0

        PUBWEAK FTFA_IRQHandler
        PUBWEAK LVD_LVW_IRQHandler
        PUBWEAK LLWU_IRQHandler
        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C0_IRQHandler
        LDR     R0, =I2C0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved25_IRQHandler
        PUBWEAK SPI0_IRQHandler
        PUBWEAK SPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI0_IRQHandler
        LDR     R0, =SPI0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved27_IRQHandler
        PUBWEAK UART0_IRQHandler
        PUBWEAK UART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_IRQHandler
        LDR     R0, =UART0_DriverIRQHandler
        BX      R0

        PUBWEAK UART1_IRQHandler
        PUBWEAK UART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_IRQHandler
        LDR     R0, =UART1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_IRQHandler
        PUBWEAK CAN0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK ADC0_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK FTM0_IRQHandler
        PUBWEAK FTM1_IRQHandler
        PUBWEAK FTM2_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK FTM3_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK FTM4_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK FTM5_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK PDB0_PDB1_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_PORTC_PORTD_PORTE_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DMA4_DriverIRQHandler
DMA1_DMA5_DriverIRQHandler
DMA2_DMA6_DriverIRQHandler
DMA3_DMA7_DriverIRQHandler
DMA_Error_DriverIRQHandler
FTFA_IRQHandler
LVD_LVW_IRQHandler
LLWU_IRQHandler
I2C0_DriverIRQHandler
Reserved25_IRQHandler
SPI0_DriverIRQHandler
Reserved27_IRQHandler
UART0_DriverIRQHandler
UART1_DriverIRQHandler
CAN0_DriverIRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
FTM3_IRQHandler
WDOG_EWM_IRQHandler
FTM4_IRQHandler
DAC0_IRQHandler
FTM5_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
PDB0_PDB1_IRQHandler
PORTA_IRQHandler
PORTB_PORTC_PORTD_PORTE_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

        END
