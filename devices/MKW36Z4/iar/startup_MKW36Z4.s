; -------------------------------------------------------------------------
;  @file:    startup_MKW36Z4.s
;  @purpose: CMSIS Cortex-M0P Core Device Startup File
;            MKW36Z4
;  @version: 1.4
;  @date:    2019-1-25
;  @build:   b190529
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
#ifdef SUPPORT_WARMBOOT
        SECTION WARMBOOT_STACK:DATA:NOROOT(3)
        SECTION WARMBOOT_CONFIG:DATA:NOROOT(2)
#endif

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
#ifdef SUPPORT_WARMBOOT
        EXTERN  WARMBOOT_SEQUENCE
#endif
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
        DCD     DMA0_IRQHandler                               ;DMA channel 0 transfer complete
        DCD     DMA1_IRQHandler                               ;DMA channel 1 transfer complete
        DCD     DMA2_IRQHandler                               ;DMA channel 2 transfer complete
        DCD     DMA3_IRQHandler                               ;DMA channel 3 transfer complete
        DCD     Reserved20_IRQHandler                         ;Reserved interrupt
        DCD     FTFE_IRQHandler                               ;Command complete and read collision, Flash ECC Errors
        DCD     LVD_LVW_DCDC_IRQHandler                       ;Low-voltage detect, low-voltage warning, DCDC
        DCD     LLWU_IRQHandler                               ;Low leakage wakeup Unit
        DCD     I2C0_IRQHandler                               ;I2C0 interrupt
        DCD     I2C1_IRQHandler                               ;I2C1 interrupt
        DCD     SPI0_IRQHandler                               ;SPI0 single interrupt vector for all sources
        DCD     CAN0_IRQHandler                               ;FlexCAN0 ORed Error, Bus off, Transmit/Receive Warning, Wake up
        DCD     LPUART0_LPUART1_IRQHandler                    ;LPUART0 or LPUART1 status and error
        DCD     TRNG0_IRQHandler                              ;TRNG0 interrupt
        DCD     CMT_IRQHandler                                ;CMT interrupt
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt
        DCD     TPM0_IRQHandler                               ;TPM0 single interrupt vector for all sources
        DCD     TPM1_IRQHandler                               ;TPM1 single interrupt vector for all sources
        DCD     TPM2_IRQHandler                               ;TPM2 single interrupt vector for all sources
        DCD     RTC_IRQHandler                                ;RTC alarm
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds
        DCD     PIT_IRQHandler                                ;PIT interrupt
        DCD     LTC0_IRQHandler                               ;LTC0 interrupt
        DCD     Radio_0_IRQHandler                            ;BTLE, ZIGBEE, ANT, GENFSK interrupt 0
        DCD     CAN0_MB_IRQHandler                            ;FlexCAN0 ORed Message Buffer (MB0-31) interrupt
        DCD     Radio_1_IRQHandler                            ;BTLE, ZIGBEE, ANT, GENFSK interrupt 1
        DCD     MCG_IRQHandler                                ;MCG interrupt
        DCD     LPTMR0_IRQHandler                             ;LPTMR0 interrupt
        DCD     SPI1_IRQHandler                               ;SPI1 single interrupt vector for all sources
        DCD     PORTA_IRQHandler                              ;PORTA Pin detect
        DCD     PORTB_PORTC_IRQHandler                        ;PORTB and PORTC Pin detect
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
#ifdef SUPPORT_WARMBOOT
;if vlls is not the only wakeup source, jump to .NORMAL_BOOT
        LDR     R0, =0x4007F000
        LDR     R0, [R0]
        CMP     R0, #1
        BNE.N   .NORMAL_BOOT
;if SMC_PMCTRL_STOPM != 4(VLLSx), jump to .NORMAL_BOOT
        MOVS    R1, #7
        LDR     R0, =0x4007E001
        LDRB    R0, [R0]
        ANDS    R0, R0, R1
        CMP     R0, #4
        BNE.N   .NORMAL_BOOT
;if SMC_STOPCTRL_LLSM != 2(VLLS2) or 3(VLLS3), jump to .NORMAL_BOOT
        LDR     R0, =0x4007E002
        LDRB    R0, [R0]
        ANDS    R0, R0, R1
        LSRS    R0, R0, #1
        CMP     R0, #1
        BNE.N   .NORMAL_BOOT
;if WARMBOOT_SEQUENCE do not match what WARMBOOT_CONFIG stored, jump to .NORMAL_BOOT
        LDR     R1, =sfb(WARMBOOT_CONFIG)
        LDR     R3, [R1]
        LDR     R4, =WARMBOOT_SEQUENCE
        LDR     R4, [R4]
        CMP     R3, R4
        BNE.N   .NORMAL_BOOT
;if useWarmbootSp = 1, set MSP to top of WARMBOOT_STACK
        LDR     R3, [R1,#4]
        CMP     R3, #0
        BEQ.N   .WARM_BOOT
;load warm boot stack pointer
        LDR     R2, =sfe(WARMBOOT_STACK)
.WARM_BOOT
        MSR     MSP, R2
        LDR     R0, =SystemInit
        BLX     R0
        CPSIE   I               ; Unmask interrupts
;jump to warmmain
        LDR     R0, =warmmain
        BX      R0
        B       .
.NORMAL_BOOT
#endif
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

        PUBWEAK DMA0_IRQHandler
        PUBWEAK DMA0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_IRQHandler
        LDR     R0, =DMA0_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_IRQHandler
        PUBWEAK DMA1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_IRQHandler
        LDR     R0, =DMA1_DriverIRQHandler
        BX      R0

        PUBWEAK DMA2_IRQHandler
        PUBWEAK DMA2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA2_IRQHandler
        LDR     R0, =DMA2_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_IRQHandler
        PUBWEAK DMA3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_IRQHandler
        LDR     R0, =DMA3_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved20_IRQHandler
        PUBWEAK FTFE_IRQHandler
        PUBWEAK LVD_LVW_DCDC_IRQHandler
        PUBWEAK LLWU_IRQHandler
        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C0_IRQHandler
        LDR     R0, =I2C0_DriverIRQHandler
        BX      R0

        PUBWEAK I2C1_IRQHandler
        PUBWEAK I2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C1_IRQHandler
        LDR     R0, =I2C1_DriverIRQHandler
        BX      R0

        PUBWEAK SPI0_IRQHandler
        PUBWEAK SPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI0_IRQHandler
        LDR     R0, =SPI0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_IRQHandler
        PUBWEAK CAN0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART0_LPUART1_IRQHandler
        PUBWEAK LPUART0_LPUART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_LPUART1_IRQHandler
        LDR     R0, =LPUART0_LPUART1_DriverIRQHandler
        BX      R0

        PUBWEAK TRNG0_IRQHandler
        PUBWEAK CMT_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK TPM0_IRQHandler
        PUBWEAK TPM1_IRQHandler
        PUBWEAK TPM2_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK PIT_IRQHandler
        PUBWEAK LTC0_IRQHandler
        PUBWEAK Radio_0_IRQHandler
        PUBWEAK CAN0_MB_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_MB_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK Radio_1_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK SPI1_IRQHandler
        PUBWEAK SPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI1_IRQHandler
        LDR     R0, =SPI1_DriverIRQHandler
        BX      R0

        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_PORTC_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DriverIRQHandler
DMA1_DriverIRQHandler
DMA2_DriverIRQHandler
DMA3_DriverIRQHandler
Reserved20_IRQHandler
FTFE_IRQHandler
LVD_LVW_DCDC_IRQHandler
LLWU_IRQHandler
I2C0_DriverIRQHandler
I2C1_DriverIRQHandler
SPI0_DriverIRQHandler
CAN0_DriverIRQHandler
LPUART0_LPUART1_DriverIRQHandler
TRNG0_IRQHandler
CMT_IRQHandler
ADC0_IRQHandler
CMP0_IRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT_IRQHandler
LTC0_IRQHandler
Radio_0_IRQHandler
Radio_1_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
SPI1_DriverIRQHandler
PORTA_IRQHandler
PORTB_PORTC_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

#ifdef SUPPORT_WARMBOOT
        PUBWEAK warmmain
        SECTION .text:CODE:REORDER:NOROOT(1)
warmmain
        B .
#endif

        END
