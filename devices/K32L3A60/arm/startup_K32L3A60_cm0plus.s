; * -------------------------------------------------------------------------
; *  @file:    startup_K32L3A60_cm0plus.s
; *  @purpose: CMSIS Cortex-M0P Core Device Startup File
; *            K32L3A60_cm0plus
; *  @version: 1.0
; *  @date:    2019-4-22
; *  @build:   b190628
; * -------------------------------------------------------------------------
; *
; * Copyright 1997-2016 Freescale Semiconductor, Inc.
; * Copyright 2016-2019 NXP
; * All rights reserved.
; *
; * SPDX-License-Identifier: BSD-3-Clause
; *
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; *****************************************************************************/


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                IMPORT  |Image$$ARM_LIB_STACK$$ZI$$Limit|

__Vectors       DCD     |Image$$ARM_LIB_STACK$$ZI$$Limit| ; Top of Stack
                DCD     Reset_Handler  ; Reset Handler
                DCD     NMI_Handler                         ;NMI Handler
                DCD     HardFault_Handler                   ;Hard Fault Handler
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     SVC_Handler                         ;SVCall Handler
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     PendSV_Handler                      ;PendSV Handler
                DCD     SysTick_Handler                     ;SysTick Handler

                                                            ;External Interrupts
                DCD     CTI1_IRQHandler                     ;Cross Trigger Interface 1
                DCD     DMA1_04_IRQHandler                  ;DMA1 channel 0/4 transfer complete
                DCD     DMA1_15_IRQHandler                  ;DMA1 channel 1/5 transfer complete
                DCD     DMA1_26_IRQHandler                  ;DMA1 channel 2/6 transfer complete
                DCD     DMA1_37_IRQHandler                  ;DMA1 channel 3/7 transfer complete
                DCD     DMA1_Error_IRQHandler               ;DMA1 channel 0-7 error interrupt
                DCD     MSMC_IRQHandler                     ;MSMC (SMC1) interrupt
                DCD     LLWU1_IRQHandler                    ;Low leakage wakeup 1
                DCD     MUB_IRQHandler                      ;MU Side B interrupt
                DCD     WDOG1_IRQHandler                    ;WDOG1 interrupt
                DCD     CAU3_Task_Complete_IRQHandler       ;Cryptographic Acceleration Unit version 3 Task Complete
                DCD     CAU3_Security_Violation_IRQHandler  ;Cryptographic Acceleration Unit version 3 Security Violation
                DCD     TRNG_IRQHandler                     ;TRNG interrupt
                DCD     LPIT1_IRQHandler                    ;LPIT1 interrupt
                DCD     LPTMR2_IRQHandler                   ;LPTMR2 interrupt
                DCD     TPM3_IRQHandler                     ;TPM3 single interrupt vector for all sources
                DCD     LPI2C3_IRQHandler                   ;LPI2C3 interrupt
                DCD     Reserved33_IRQHandler               ;Reserved interrupt
                DCD     Reserved34_IRQHandler               ;Reserved interrupt
                DCD     LPSPI3_IRQHandler                   ;LPSPI3 single interrupt vector for all sources
                DCD     LPUART3_IRQHandler                  ;LPUART3 status and error
                DCD     PORTE_IRQHandler                    ;PORTE Pin detect
                DCD     LPCMP1_IRQHandler                   ;LPCMP1 interrupt
                DCD     RTC_IRQHandler                      ;RTC
                DCD     INTMUX1_0_IRQHandler                ;INTMUX1 channel0 interrupt
                DCD     INTMUX1_1_IRQHandler                ;INTMUX1 channel1 interrupt
                DCD     INTMUX1_2_IRQHandler                ;INTMUX1 channel2 interrupt
                DCD     INTMUX1_3_IRQHandler                ;INTMUX1 channel3 interrupt
                DCD     INTMUX1_4_IRQHandler                ;INTMUX1 channel4 interrupt
                DCD     INTMUX1_5_IRQHandler                ;INTMUX1 channel5 interrupt
                DCD     INTMUX1_6_IRQHandler                ;INTMUX1 channel6 interrupt
                DCD     INTMUX1_7_IRQHandler                ;INTMUX1 channel7 interrupt
                DCD     EWM_IRQHandler                      ;EWM interrupt  (INTMUX1 source IRQ0)
                DCD     FTFE_Command_Complete_IRQHandler    ;FTFE interrupt  (INTMUX1 source IRQ1)
                DCD     FTFE_Read_Collision_IRQHandler      ;FTFE interrupt  (INTMUX1 source IRQ2)
                DCD     SPM_IRQHandler                      ;SPM (INTMUX1 source IRQ3)
                DCD     SCG_IRQHandler                      ;SCG interrupt (INTMUX1 source IRQ4)
                DCD     LPIT0_IRQHandler                    ;LPIT0 interrupt (INTMUX1 source IRQ5)
                DCD     LPTMR0_IRQHandler                   ;LPTMR0 interrupt (INTMUX1 source IRQ6)
                DCD     LPTMR1_IRQHandler                   ;LPTMR1 interrupt (INTMUX1 source IRQ7)
                DCD     TPM0_IRQHandler                     ;TPM0 single interrupt vector for all sources (INTMUX1 source IRQ8)
                DCD     TPM1_IRQHandler                     ;TPM1 single interrupt vector for all sources (INTMUX1 source IRQ9)
                DCD     TPM2_IRQHandler                     ;TPM2 single interrupt vector for all sources (INTMUX1 source IRQ10)
                DCD     EMVSIM0_IRQHandler                  ;EMVSIM0 interrupt (INTMUX1 source IRQ11)
                DCD     FLEXIO0_IRQHandler                  ;FLEXIO0 (INTMUX1 source IRQ12)
                DCD     LPI2C0_IRQHandler                   ;LPI2C0 interrupt (INTMUX1 source IRQ13)
                DCD     LPI2C1_IRQHandler                   ;LPI2C1 interrupt (INTMUX1 source IRQ14)
                DCD     LPI2C2_IRQHandler                   ;LPI2C2 interrupt (INTMUX1 source IRQ15)
                DCD     I2S0_IRQHandler                     ;I2S0 interrupt (INTMUX1 source IRQ16)
                DCD     USDHC0_IRQHandler                   ;SDHC0 interrupt (INTMUX1 source IRQ17)
                DCD     LPSPI0_IRQHandler                   ;LPSPI0 single interrupt vector for all sources (INTMUX1 source IRQ18)
                DCD     LPSPI1_IRQHandler                   ;LPSPI1 single interrupt vector for all sources (INTMUX1 source IRQ19)
                DCD     LPSPI2_IRQHandler                   ;LPSPI2 single interrupt vector for all sources (INTMUX1 source IRQ20)
                DCD     LPUART0_IRQHandler                  ;LPUART0 status and error (INTMUX1 source IRQ21)
                DCD     LPUART1_IRQHandler                  ;LPUART1 status and error (INTMUX1 source IRQ22)
                DCD     LPUART2_IRQHandler                  ;LPUART2 status and error (INTMUX1 source IRQ23)
                DCD     USB0_IRQHandler                     ;USB0 interrupt (INTMUX1 source IRQ24)
                DCD     PORTA_IRQHandler                    ;PORTA Pin detect (INTMUX1 source IRQ25)
                DCD     PORTB_IRQHandler                    ;PORTB Pin detect (INTMUX1 source IRQ26)
                DCD     PORTC_IRQHandler                    ;PORTC Pin detect (INTMUX1 source IRQ27)
                DCD     PORTD_IRQHandler                    ;PORTD Pin detect (INTMUX1 source IRQ28)
                DCD     LPADC0_IRQHandler                   ;LPADC0 interrupt (INTMUX1 source IRQ29)
                DCD     LPCMP0_IRQHandler                   ;LPCMP0 interrupt (INTMUX1 source IRQ30)
                DCD     LPDAC0_IRQHandler                   ;LPDAC0 interrupt (INTMUX1 source IRQ31)
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                CPSID   I               ; Mask interrupts
                LDR     R0, =0xE000ED08
                LDR     R1, =__Vectors
                STR     R1, [R0]
                LDR     R2, [R1]
                MSR     MSP, R2
                LDR     R0, =SystemInit
                BLX     R0
                CPSIE   i               ; Unmask interrupts
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)
NMI_Handler\
                PROC
                EXPORT  NMI_Handler         [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler\
                PROC
                EXPORT  SVC_Handler         [WEAK]
                B       .
                ENDP
PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler         [WEAK]
                B       .
                ENDP
SysTick_Handler\
                PROC
                EXPORT  SysTick_Handler         [WEAK]
                B       .
                ENDP
DMA1_04_IRQHandler\
                PROC
                EXPORT  DMA1_04_IRQHandler         [WEAK]
                LDR     R0, =DMA1_04_DriverIRQHandler
                BX      R0
                ENDP

DMA1_15_IRQHandler\
                PROC
                EXPORT  DMA1_15_IRQHandler         [WEAK]
                LDR     R0, =DMA1_15_DriverIRQHandler
                BX      R0
                ENDP

DMA1_26_IRQHandler\
                PROC
                EXPORT  DMA1_26_IRQHandler         [WEAK]
                LDR     R0, =DMA1_26_DriverIRQHandler
                BX      R0
                ENDP

DMA1_37_IRQHandler\
                PROC
                EXPORT  DMA1_37_IRQHandler         [WEAK]
                LDR     R0, =DMA1_37_DriverIRQHandler
                BX      R0
                ENDP

DMA1_Error_IRQHandler\
                PROC
                EXPORT  DMA1_Error_IRQHandler         [WEAK]
                LDR     R0, =DMA1_Error_DriverIRQHandler
                BX      R0
                ENDP

LPI2C3_IRQHandler\
                PROC
                EXPORT  LPI2C3_IRQHandler         [WEAK]
                LDR     R0, =LPI2C3_DriverIRQHandler
                BX      R0
                ENDP

LPSPI3_IRQHandler\
                PROC
                EXPORT  LPSPI3_IRQHandler         [WEAK]
                LDR     R0, =LPSPI3_DriverIRQHandler
                BX      R0
                ENDP

LPUART3_IRQHandler\
                PROC
                EXPORT  LPUART3_IRQHandler         [WEAK]
                LDR     R0, =LPUART3_DriverIRQHandler
                BX      R0
                ENDP

INTMUX1_0_IRQHandler\
                PROC
                EXPORT  INTMUX1_0_IRQHandler         [WEAK]
                LDR     R0, =INTMUX1_0_DriverIRQHandler
                BX      R0
                ENDP

INTMUX1_1_IRQHandler\
                PROC
                EXPORT  INTMUX1_1_IRQHandler         [WEAK]
                LDR     R0, =INTMUX1_1_DriverIRQHandler
                BX      R0
                ENDP

INTMUX1_2_IRQHandler\
                PROC
                EXPORT  INTMUX1_2_IRQHandler         [WEAK]
                LDR     R0, =INTMUX1_2_DriverIRQHandler
                BX      R0
                ENDP

INTMUX1_3_IRQHandler\
                PROC
                EXPORT  INTMUX1_3_IRQHandler         [WEAK]
                LDR     R0, =INTMUX1_3_DriverIRQHandler
                BX      R0
                ENDP

INTMUX1_4_IRQHandler\
                PROC
                EXPORT  INTMUX1_4_IRQHandler         [WEAK]
                LDR     R0, =INTMUX1_4_DriverIRQHandler
                BX      R0
                ENDP

INTMUX1_5_IRQHandler\
                PROC
                EXPORT  INTMUX1_5_IRQHandler         [WEAK]
                LDR     R0, =INTMUX1_5_DriverIRQHandler
                BX      R0
                ENDP

INTMUX1_6_IRQHandler\
                PROC
                EXPORT  INTMUX1_6_IRQHandler         [WEAK]
                LDR     R0, =INTMUX1_6_DriverIRQHandler
                BX      R0
                ENDP

INTMUX1_7_IRQHandler\
                PROC
                EXPORT  INTMUX1_7_IRQHandler         [WEAK]
                LDR     R0, =INTMUX1_7_DriverIRQHandler
                BX      R0
                ENDP

FLEXIO0_IRQHandler\
                PROC
                EXPORT  FLEXIO0_IRQHandler         [WEAK]
                LDR     R0, =FLEXIO0_DriverIRQHandler
                BX      R0
                ENDP

LPI2C0_IRQHandler\
                PROC
                EXPORT  LPI2C0_IRQHandler         [WEAK]
                LDR     R0, =LPI2C0_DriverIRQHandler
                BX      R0
                ENDP

LPI2C1_IRQHandler\
                PROC
                EXPORT  LPI2C1_IRQHandler         [WEAK]
                LDR     R0, =LPI2C1_DriverIRQHandler
                BX      R0
                ENDP

LPI2C2_IRQHandler\
                PROC
                EXPORT  LPI2C2_IRQHandler         [WEAK]
                LDR     R0, =LPI2C2_DriverIRQHandler
                BX      R0
                ENDP

I2S0_IRQHandler\
                PROC
                EXPORT  I2S0_IRQHandler         [WEAK]
                LDR     R0, =I2S0_DriverIRQHandler
                BX      R0
                ENDP

USDHC0_IRQHandler\
                PROC
                EXPORT  USDHC0_IRQHandler         [WEAK]
                LDR     R0, =USDHC0_DriverIRQHandler
                BX      R0
                ENDP

LPSPI0_IRQHandler\
                PROC
                EXPORT  LPSPI0_IRQHandler         [WEAK]
                LDR     R0, =LPSPI0_DriverIRQHandler
                BX      R0
                ENDP

LPSPI1_IRQHandler\
                PROC
                EXPORT  LPSPI1_IRQHandler         [WEAK]
                LDR     R0, =LPSPI1_DriverIRQHandler
                BX      R0
                ENDP

LPSPI2_IRQHandler\
                PROC
                EXPORT  LPSPI2_IRQHandler         [WEAK]
                LDR     R0, =LPSPI2_DriverIRQHandler
                BX      R0
                ENDP

LPUART0_IRQHandler\
                PROC
                EXPORT  LPUART0_IRQHandler         [WEAK]
                LDR     R0, =LPUART0_DriverIRQHandler
                BX      R0
                ENDP

LPUART1_IRQHandler\
                PROC
                EXPORT  LPUART1_IRQHandler         [WEAK]
                LDR     R0, =LPUART1_DriverIRQHandler
                BX      R0
                ENDP

LPUART2_IRQHandler\
                PROC
                EXPORT  LPUART2_IRQHandler         [WEAK]
                LDR     R0, =LPUART2_DriverIRQHandler
                BX      R0
                ENDP

Default_Handler\
                PROC
                EXPORT  CTI1_IRQHandler         [WEAK]
                EXPORT  DMA1_04_DriverIRQHandler         [WEAK]
                EXPORT  DMA1_15_DriverIRQHandler         [WEAK]
                EXPORT  DMA1_26_DriverIRQHandler         [WEAK]
                EXPORT  DMA1_37_DriverIRQHandler         [WEAK]
                EXPORT  DMA1_Error_DriverIRQHandler         [WEAK]
                EXPORT  MSMC_IRQHandler         [WEAK]
                EXPORT  LLWU1_IRQHandler         [WEAK]
                EXPORT  MUB_IRQHandler         [WEAK]
                EXPORT  WDOG1_IRQHandler         [WEAK]
                EXPORT  CAU3_Task_Complete_IRQHandler         [WEAK]
                EXPORT  CAU3_Security_Violation_IRQHandler         [WEAK]
                EXPORT  TRNG_IRQHandler         [WEAK]
                EXPORT  LPIT1_IRQHandler         [WEAK]
                EXPORT  LPTMR2_IRQHandler         [WEAK]
                EXPORT  TPM3_IRQHandler         [WEAK]
                EXPORT  LPI2C3_DriverIRQHandler         [WEAK]
                EXPORT  Reserved33_IRQHandler         [WEAK]
                EXPORT  Reserved34_IRQHandler         [WEAK]
                EXPORT  LPSPI3_DriverIRQHandler         [WEAK]
                EXPORT  LPUART3_DriverIRQHandler         [WEAK]
                EXPORT  PORTE_IRQHandler         [WEAK]
                EXPORT  LPCMP1_IRQHandler         [WEAK]
                EXPORT  RTC_IRQHandler         [WEAK]
                EXPORT  INTMUX1_0_DriverIRQHandler         [WEAK]
                EXPORT  INTMUX1_1_DriverIRQHandler         [WEAK]
                EXPORT  INTMUX1_2_DriverIRQHandler         [WEAK]
                EXPORT  INTMUX1_3_DriverIRQHandler         [WEAK]
                EXPORT  INTMUX1_4_DriverIRQHandler         [WEAK]
                EXPORT  INTMUX1_5_DriverIRQHandler         [WEAK]
                EXPORT  INTMUX1_6_DriverIRQHandler         [WEAK]
                EXPORT  INTMUX1_7_DriverIRQHandler         [WEAK]
                EXPORT  EWM_IRQHandler         [WEAK]
                EXPORT  FTFE_Command_Complete_IRQHandler         [WEAK]
                EXPORT  FTFE_Read_Collision_IRQHandler         [WEAK]
                EXPORT  SPM_IRQHandler         [WEAK]
                EXPORT  SCG_IRQHandler         [WEAK]
                EXPORT  LPIT0_IRQHandler         [WEAK]
                EXPORT  LPTMR0_IRQHandler         [WEAK]
                EXPORT  LPTMR1_IRQHandler         [WEAK]
                EXPORT  TPM0_IRQHandler         [WEAK]
                EXPORT  TPM1_IRQHandler         [WEAK]
                EXPORT  TPM2_IRQHandler         [WEAK]
                EXPORT  EMVSIM0_IRQHandler         [WEAK]
                EXPORT  FLEXIO0_DriverIRQHandler         [WEAK]
                EXPORT  LPI2C0_DriverIRQHandler         [WEAK]
                EXPORT  LPI2C1_DriverIRQHandler         [WEAK]
                EXPORT  LPI2C2_DriverIRQHandler         [WEAK]
                EXPORT  I2S0_DriverIRQHandler         [WEAK]
                EXPORT  USDHC0_DriverIRQHandler         [WEAK]
                EXPORT  LPSPI0_DriverIRQHandler         [WEAK]
                EXPORT  LPSPI1_DriverIRQHandler         [WEAK]
                EXPORT  LPSPI2_DriverIRQHandler         [WEAK]
                EXPORT  LPUART0_DriverIRQHandler         [WEAK]
                EXPORT  LPUART1_DriverIRQHandler         [WEAK]
                EXPORT  LPUART2_DriverIRQHandler         [WEAK]
                EXPORT  USB0_IRQHandler         [WEAK]
                EXPORT  PORTA_IRQHandler         [WEAK]
                EXPORT  PORTB_IRQHandler         [WEAK]
                EXPORT  PORTC_IRQHandler         [WEAK]
                EXPORT  PORTD_IRQHandler         [WEAK]
                EXPORT  LPADC0_IRQHandler         [WEAK]
                EXPORT  LPCMP0_IRQHandler         [WEAK]
                EXPORT  LPDAC0_IRQHandler         [WEAK]
                EXPORT  DefaultISR         [WEAK]
CTI1_IRQHandler
DMA1_04_DriverIRQHandler
DMA1_15_DriverIRQHandler
DMA1_26_DriverIRQHandler
DMA1_37_DriverIRQHandler
DMA1_Error_DriverIRQHandler
MSMC_IRQHandler
LLWU1_IRQHandler
MUB_IRQHandler
WDOG1_IRQHandler
CAU3_Task_Complete_IRQHandler
CAU3_Security_Violation_IRQHandler
TRNG_IRQHandler
LPIT1_IRQHandler
LPTMR2_IRQHandler
TPM3_IRQHandler
LPI2C3_DriverIRQHandler
Reserved33_IRQHandler
Reserved34_IRQHandler
LPSPI3_DriverIRQHandler
LPUART3_DriverIRQHandler
PORTE_IRQHandler
LPCMP1_IRQHandler
RTC_IRQHandler
INTMUX1_0_DriverIRQHandler
INTMUX1_1_DriverIRQHandler
INTMUX1_2_DriverIRQHandler
INTMUX1_3_DriverIRQHandler
INTMUX1_4_DriverIRQHandler
INTMUX1_5_DriverIRQHandler
INTMUX1_6_DriverIRQHandler
INTMUX1_7_DriverIRQHandler
EWM_IRQHandler
FTFE_Command_Complete_IRQHandler
FTFE_Read_Collision_IRQHandler
SPM_IRQHandler
SCG_IRQHandler
LPIT0_IRQHandler
LPTMR0_IRQHandler
LPTMR1_IRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
EMVSIM0_IRQHandler
FLEXIO0_DriverIRQHandler
LPI2C0_DriverIRQHandler
LPI2C1_DriverIRQHandler
LPI2C2_DriverIRQHandler
I2S0_DriverIRQHandler
USDHC0_DriverIRQHandler
LPSPI0_DriverIRQHandler
LPSPI1_DriverIRQHandler
LPSPI2_DriverIRQHandler
LPUART0_DriverIRQHandler
LPUART1_DriverIRQHandler
LPUART2_DriverIRQHandler
USB0_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
LPADC0_IRQHandler
LPCMP0_IRQHandler
LPDAC0_IRQHandler
DefaultISR
                LDR    R0, =DefaultISR
                BX     R0
                ENDP
                  ALIGN


                END
