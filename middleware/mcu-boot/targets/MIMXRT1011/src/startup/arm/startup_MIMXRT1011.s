; * -------------------------------------------------------------------------
; *  @file:    startup_MIMXRT1011.s
; *  @purpose: CMSIS Cortex-M7 Core Device Startup File
; *            MIMXRT1011
; *  @version: 1.0
; *  @date:    2019-8-1
; *  @build:   b190801
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
                DCD     MemManage_Handler                   ;MPU Fault Handler
                DCD     BusFault_Handler                    ;Bus Fault Handler
                DCD     UsageFault_Handler                  ;Usage Fault Handler
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     SVC_Handler                         ;SVCall Handler
                DCD     DebugMon_Handler                    ;Debug Monitor Handler
                DCD     0                                   ;Reserved
                DCD     PendSV_Handler                      ;PendSV Handler
                DCD     SysTick_Handler                     ;SysTick Handler

                                                            ;External Interrupts
                DCD     DMA0_IRQHandler                     ;DMA channel 0 transfer complete
                DCD     DMA1_IRQHandler                     ;DMA channel 1 transfer complete
                DCD     DMA2_IRQHandler                     ;DMA channel 2 transfer complete
                DCD     DMA3_IRQHandler                     ;DMA channel 3 transfer complete
                DCD     DMA4_IRQHandler                     ;DMA channel 4 transfer complete
                DCD     DMA5_IRQHandler                     ;DMA channel 5 transfer complete
                DCD     DMA6_IRQHandler                     ;DMA channel 6 transfer complete
                DCD     DMA7_IRQHandler                     ;DMA channel 7 transfer complete
                DCD     DMA8_IRQHandler                     ;DMA channel 8 transfer complete
                DCD     DMA9_IRQHandler                     ;DMA channel 9 transfer complete
                DCD     DMA10_IRQHandler                    ;DMA channel 10 transfer complete
                DCD     DMA11_IRQHandler                    ;DMA channel 11 transfer complete
                DCD     DMA12_IRQHandler                    ;DMA channel 12 transfer complete
                DCD     DMA13_IRQHandler                    ;DMA channel 13 transfer complete
                DCD     DMA14_IRQHandler                    ;DMA channel 14 transfer complete
                DCD     DMA15_IRQHandler                    ;DMA channel 15 transfer complete
                DCD     DMA_ERROR_IRQHandler                ;DMA error interrupt channels 0-15
                DCD     CTI0_ERROR_IRQHandler               ;CTI trigger outputs
                DCD     CTI1_ERROR_IRQHandler               ;CTI trigger outputs
                DCD     CORE_IRQHandler                     ;CorePlatform exception IRQ
                DCD     LPUART1_IRQHandler                  ;LPUART1 TX interrupt and RX interrupt
                DCD     LPUART2_IRQHandler                  ;LPUART2 TX interrupt and RX interrupt
                DCD     LPUART3_IRQHandler                  ;LPUART3 TX interrupt and RX interrupt
                DCD     LPUART4_IRQHandler                  ;LPUART4 TX interrupt and RX interrupt
                DCD     PIT_IRQHandler                      ;PIT interrupt
                DCD     USB_OTG1_IRQHandler                 ;USBO2 USB OTG1
                DCD     FLEXSPI_IRQHandler                  ;FlexSPI0 interrupt
                DCD     FLEXRAM_IRQHandler                  ;FlexRAM address out of range Or access hit IRQ
                DCD     LPI2C1_IRQHandler                   ;LPI2C1 interrupt
                DCD     LPI2C2_IRQHandler                   ;LPI2C2 interrupt
                DCD     GPT1_IRQHandler                     ;GPT1 interrupt
                DCD     GPT2_IRQHandler                     ;GPT2 interrupt
                DCD     LPSPI1_IRQHandler                   ;LPSPI1 single interrupt vector for all sources
                DCD     LPSPI2_IRQHandler                   ;LPSPI2 single interrupt vector for all sources
                DCD     PWM1_0_IRQHandler                   ;PWM1 capture 0, compare 0, or reload 0 interrupt
                DCD     PWM1_1_IRQHandler                   ;PWM1 capture 1, compare 1, or reload 0 interrupt
                DCD     PWM1_2_IRQHandler                   ;PWM1 capture 2, compare 2, or reload 0 interrupt
                DCD     PWM1_3_IRQHandler                   ;PWM1 capture 3, compare 3, or reload 0 interrupt
                DCD     PWM1_FAULT_IRQHandler               ;PWM1 fault or reload error interrupt
                DCD     KPP_IRQHandler                      ;Keypad nterrupt
                DCD     SRC_IRQHandler                      ;SRC interrupt
                DCD     GPR_IRQ_IRQHandler                  ;Used to notify cores on exception condition while boot
                DCD     CCM_1_IRQHandler                    ;CCM IRQ1 interrupt
                DCD     CCM_2_IRQHandler                    ;CCM IRQ2 interrupt
                DCD     EWM_IRQHandler                      ;EWM interrupt
                DCD     WDOG2_IRQHandler                    ;WDOG2 interrupt
                DCD     SNVS_HP_WRAPPER_IRQHandler          ;SNVS Functional Interrupt
                DCD     SNVS_HP_WRAPPER_TZ_IRQHandler       ;SNVS Security Interrupt
                DCD     SNVS_LP_WRAPPER_IRQHandler          ;ON-OFF button press shorter than 5 secs (pulse event)
                DCD     CSU_IRQHandler                      ;CSU interrupt
                DCD     DCP_IRQHandler                      ;Combined DCP channel interrupts(except channel 0) and CRC interrupt
                DCD     DCP_VMI_IRQHandler                  ;IRQ of DCP channel 0
                DCD     Reserved68_IRQHandler               ;Reserved interrupt
                DCD     TRNG_IRQHandler                     ;TRNG interrupt
                DCD     Reserved70_IRQHandler               ;Reserved interrupt
                DCD     Reserved71_IRQHandler               ;Reserved interrupt
                DCD     SAI1_IRQHandler                     ;SAI1 interrupt
                DCD     RTWDOG_IRQHandler                   ;RTWDOG interrupt
                DCD     SAI3_RX_IRQHandler                  ;SAI3 interrupt
                DCD     SAI3_TX_IRQHandler                  ;SAI3 interrupt
                DCD     SPDIF_IRQHandler                    ;SPDIF interrupt
                DCD     PMU_IRQHandler                      ;PMU interrupt
                DCD     XBAR1_IRQ_0_1_2_3_IRQHandler        ;XBAR1 interrupt
                DCD     TEMP_LOW_HIGH_IRQHandler            ;TEMPMON interrupt
                DCD     TEMP_PANIC_IRQHandler               ;TEMPMON interrupt
                DCD     USB_PHY_IRQHandler                  ;USBPHY (OTG1 UTMI), Interrupt
                DCD     GPC_IRQHandler                      ;GPC interrupt
                DCD     ADC1_IRQHandler                     ;ADC1 interrupt
                DCD     FLEXIO1_IRQHandler                  ;FLEXIO1 interrupt
                DCD     DCDC_IRQHandler                     ;DCDC interrupt
                DCD     GPIO1_Combined_0_15_IRQHandler      ;Combined interrupt indication for GPIO1 signal 0 throughout 15
                DCD     GPIO1_Combined_16_31_IRQHandler     ;Combined interrupt indication for GPIO1 signal 16 throughout 31
                DCD     GPIO2_Combined_0_15_IRQHandler      ;Combined interrupt indication for GPIO2 signal 0 throughout 15
                DCD     GPIO5_Combined_0_15_IRQHandler      ;Combined interrupt indication for GPIO5 signal 0 throughout 15
                DCD     WDOG1_IRQHandler                    ;WDOG1 interrupt
                DCD     ADC_ETC_IRQ0_IRQHandler             ;ADCETC IRQ0 interrupt
                DCD     ADC_ETC_IRQ1_IRQHandler             ;ADCETC IRQ1 interrupt
                DCD     ADC_ETC_IRQ2_IRQHandler             ;ADCETC IRQ2 interrupt
                DCD     ADC_ETC_IRQ3_IRQHandler             ;ADCETC IRQ3 interrupt
                DCD     ADC_ETC_ERROR_IRQ_IRQHandler        ;ADCETC Error IRQ interrupt
                DCD     DefaultISR                          ;96
                DCD     DefaultISR                          ;97
                DCD     DefaultISR                          ;98
                DCD     DefaultISR                          ;99
                DCD     DefaultISR                          ;100
                DCD     DefaultISR                          ;101
                DCD     DefaultISR                          ;102
                DCD     DefaultISR                          ;103
                DCD     DefaultISR                          ;104
                DCD     DefaultISR                          ;105
                DCD     DefaultISR                          ;106
                DCD     DefaultISR                          ;107
                DCD     DefaultISR                          ;108
                DCD     DefaultISR                          ;109
                DCD     DefaultISR                          ;110
                DCD     DefaultISR                          ;111
                DCD     DefaultISR                          ;112
                DCD     DefaultISR                          ;113
                DCD     DefaultISR                          ;114
                DCD     DefaultISR                          ;115
                DCD     DefaultISR                          ;116
                DCD     DefaultISR                          ;117
                DCD     DefaultISR                          ;118
                DCD     DefaultISR                          ;119
                DCD     DefaultISR                          ;120
                DCD     DefaultISR                          ;121
                DCD     DefaultISR                          ;122
                DCD     DefaultISR                          ;123
                DCD     DefaultISR                          ;124
                DCD     DefaultISR                          ;125
                DCD     DefaultISR                          ;126
                DCD     0xFFFFFFFF                          ; Reserved for user TRIM value
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
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler         [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler\
                PROC
                EXPORT  SVC_Handler         [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler         [WEAK]
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

LPUART1_IRQHandler\
                PROC
                EXPORT  LPUART1_IRQHandler         [WEAK]
                LDR     R0, =LPUART1_DriverIRQHandler
                BX      R0
                ENDP

FLEXSPI_IRQHandler\
                PROC
                EXPORT  FLEXSPI_IRQHandler         [WEAK]
                LDR     R0, =FLEXSPI_DriverIRQHandler
                BX      R0
                ENDP

FLEXIO1_IRQHandler\
                PROC
                EXPORT  FLEXIO1_IRQHandler         [WEAK]
                LDR     R0, =FLEXIO1_DriverIRQHandler
                BX      R0
                ENDP

Default_Handler\
                PROC
                EXPORT  DMA0_DriverIRQHandler         [WEAK]
                EXPORT  DMA1_DriverIRQHandler         [WEAK]
                EXPORT  DMA2_DriverIRQHandler         [WEAK]
                EXPORT  DMA3_DriverIRQHandler         [WEAK]
                EXPORT  DMA4_DriverIRQHandler         [WEAK]
                EXPORT  DMA5_DriverIRQHandler         [WEAK]
                EXPORT  DMA6_DriverIRQHandler         [WEAK]
                EXPORT  DMA7_DriverIRQHandler         [WEAK]
                EXPORT  DMA8_DriverIRQHandler         [WEAK]
                EXPORT  DMA9_DriverIRQHandler         [WEAK]
                EXPORT  DMA10_DriverIRQHandler         [WEAK]
                EXPORT  DMA11_DriverIRQHandler         [WEAK]
                EXPORT  DMA12_DriverIRQHandler         [WEAK]
                EXPORT  DMA13_DriverIRQHandler         [WEAK]
                EXPORT  DMA14_DriverIRQHandler         [WEAK]
                EXPORT  DMA15_DriverIRQHandler         [WEAK]
                EXPORT  DMA_ERROR_DriverIRQHandler         [WEAK]
                EXPORT  CTI0_ERROR_IRQHandler         [WEAK]
                EXPORT  CTI1_ERROR_IRQHandler         [WEAK]
                EXPORT  CORE_IRQHandler         [WEAK]
                EXPORT  LPUART1_DriverIRQHandler         [WEAK]
                EXPORT  LPUART2_DriverIRQHandler         [WEAK]
                EXPORT  LPUART3_DriverIRQHandler         [WEAK]
                EXPORT  LPUART4_DriverIRQHandler         [WEAK]
                EXPORT  PIT_IRQHandler         [WEAK]
                EXPORT  USB_OTG1_IRQHandler         [WEAK]
                EXPORT  FLEXSPI_DriverIRQHandler         [WEAK]
                EXPORT  FLEXRAM_IRQHandler         [WEAK]
                EXPORT  LPI2C1_DriverIRQHandler         [WEAK]
                EXPORT  LPI2C2_DriverIRQHandler         [WEAK]
                EXPORT  GPT1_IRQHandler         [WEAK]
                EXPORT  GPT2_IRQHandler         [WEAK]
                EXPORT  LPSPI1_DriverIRQHandler         [WEAK]
                EXPORT  LPSPI2_DriverIRQHandler         [WEAK]
                EXPORT  PWM1_0_IRQHandler         [WEAK]
                EXPORT  PWM1_1_IRQHandler         [WEAK]
                EXPORT  PWM1_2_IRQHandler         [WEAK]
                EXPORT  PWM1_3_IRQHandler         [WEAK]
                EXPORT  PWM1_FAULT_IRQHandler         [WEAK]
                EXPORT  KPP_IRQHandler         [WEAK]
                EXPORT  SRC_IRQHandler         [WEAK]
                EXPORT  GPR_IRQ_IRQHandler         [WEAK]
                EXPORT  CCM_1_IRQHandler         [WEAK]
                EXPORT  CCM_2_IRQHandler         [WEAK]
                EXPORT  EWM_IRQHandler         [WEAK]
                EXPORT  WDOG2_IRQHandler         [WEAK]
                EXPORT  SNVS_HP_WRAPPER_IRQHandler         [WEAK]
                EXPORT  SNVS_HP_WRAPPER_TZ_IRQHandler         [WEAK]
                EXPORT  SNVS_LP_WRAPPER_IRQHandler         [WEAK]
                EXPORT  CSU_IRQHandler         [WEAK]
                EXPORT  DCP_IRQHandler         [WEAK]
                EXPORT  DCP_VMI_IRQHandler         [WEAK]
                EXPORT  Reserved68_IRQHandler         [WEAK]
                EXPORT  TRNG_IRQHandler         [WEAK]
                EXPORT  Reserved70_IRQHandler         [WEAK]
                EXPORT  Reserved71_IRQHandler         [WEAK]
                EXPORT  SAI1_DriverIRQHandler         [WEAK]
                EXPORT  RTWDOG_IRQHandler         [WEAK]
                EXPORT  SAI3_RX_DriverIRQHandler         [WEAK]
                EXPORT  SAI3_TX_DriverIRQHandler         [WEAK]
                EXPORT  SPDIF_DriverIRQHandler         [WEAK]
                EXPORT  PMU_IRQHandler         [WEAK]
                EXPORT  XBAR1_IRQ_0_1_2_3_IRQHandler         [WEAK]
                EXPORT  TEMP_LOW_HIGH_IRQHandler         [WEAK]
                EXPORT  TEMP_PANIC_IRQHandler         [WEAK]
                EXPORT  USB_PHY_IRQHandler         [WEAK]
                EXPORT  GPC_IRQHandler         [WEAK]
                EXPORT  ADC1_IRQHandler         [WEAK]
                EXPORT  FLEXIO1_DriverIRQHandler         [WEAK]
                EXPORT  DCDC_IRQHandler         [WEAK]
                EXPORT  GPIO1_Combined_0_15_IRQHandler         [WEAK]
                EXPORT  GPIO1_Combined_16_31_IRQHandler         [WEAK]
                EXPORT  GPIO2_Combined_0_15_IRQHandler         [WEAK]
                EXPORT  GPIO5_Combined_0_15_IRQHandler         [WEAK]
                EXPORT  WDOG1_IRQHandler         [WEAK]
                EXPORT  ADC_ETC_IRQ0_IRQHandler         [WEAK]
                EXPORT  ADC_ETC_IRQ1_IRQHandler         [WEAK]
                EXPORT  ADC_ETC_IRQ2_IRQHandler         [WEAK]
                EXPORT  ADC_ETC_IRQ3_IRQHandler         [WEAK]
                EXPORT  ADC_ETC_ERROR_IRQ_IRQHandler         [WEAK]
                EXPORT  DefaultISR         [WEAK]
                EXPORT  DMA0_IRQHandler      [WEAK]
                EXPORT  DMA1_IRQHandler      [WEAK]
                EXPORT  DMA2_IRQHandler      [WEAK]
                EXPORT  DMA3_IRQHandler      [WEAK]
                EXPORT  DMA4_IRQHandler      [WEAK]
                EXPORT  DMA5_IRQHandler      [WEAK]
                EXPORT  DMA6_IRQHandler      [WEAK]
                EXPORT  DMA7_IRQHandler      [WEAK]
                EXPORT  DMA8_IRQHandler      [WEAK]
                EXPORT  DMA9_IRQHandler      [WEAK]
                EXPORT  DMA10_IRQHandler     [WEAK]
                EXPORT  DMA11_IRQHandler     [WEAK]
                EXPORT  DMA12_IRQHandler     [WEAK]
                EXPORT  DMA13_IRQHandler     [WEAK]
                EXPORT  DMA14_IRQHandler     [WEAK]
                EXPORT  DMA15_IRQHandler     [WEAK]
                EXPORT  DMA_ERROR_IRQHandler [WEAK]
                EXPORT  LPUART2_IRQHandler   [WEAK]
                EXPORT  LPUART3_IRQHandler   [WEAK]
                EXPORT  LPUART4_IRQHandler   [WEAK]
                EXPORT  LPI2C1_IRQHandler    [WEAK]
                EXPORT  LPI2C2_IRQHandler    [WEAK]
                EXPORT  LPSPI1_IRQHandler    [WEAK]
                EXPORT  LPSPI2_IRQHandler    [WEAK]
                EXPORT  SAI1_IRQHandler      [WEAK]
                EXPORT  SAI3_RX_IRQHandler   [WEAK]
                EXPORT  SAI3_TX_IRQHandler   [WEAK]
                EXPORT  SPDIF_IRQHandler     [WEAK]
DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
DMA8_IRQHandler
DMA9_IRQHandler
DMA10_IRQHandler
DMA11_IRQHandler
DMA12_IRQHandler
DMA13_IRQHandler
DMA14_IRQHandler
DMA15_IRQHandler
DMA_ERROR_IRQHandler
LPUART2_IRQHandler
LPUART3_IRQHandler
LPUART4_IRQHandler
LPI2C1_IRQHandler
LPI2C2_IRQHandler
LPSPI1_IRQHandler
LPSPI2_IRQHandler
SAI1_IRQHandler
SAI3_RX_IRQHandler
SAI3_TX_IRQHandler
SPDIF_IRQHandler
DMA0_DriverIRQHandler
DMA1_DriverIRQHandler
DMA2_DriverIRQHandler
DMA3_DriverIRQHandler
DMA4_DriverIRQHandler
DMA5_DriverIRQHandler
DMA6_DriverIRQHandler
DMA7_DriverIRQHandler
DMA8_DriverIRQHandler
DMA9_DriverIRQHandler
DMA10_DriverIRQHandler
DMA11_DriverIRQHandler
DMA12_DriverIRQHandler
DMA13_DriverIRQHandler
DMA14_DriverIRQHandler
DMA15_DriverIRQHandler
DMA_ERROR_DriverIRQHandler
CTI0_ERROR_IRQHandler
CTI1_ERROR_IRQHandler
CORE_IRQHandler
LPUART1_DriverIRQHandler
LPUART2_DriverIRQHandler
LPUART3_DriverIRQHandler
LPUART4_DriverIRQHandler
PIT_IRQHandler
USB_OTG1_IRQHandler
FLEXSPI_DriverIRQHandler
FLEXRAM_IRQHandler
LPI2C1_DriverIRQHandler
LPI2C2_DriverIRQHandler
GPT1_IRQHandler
GPT2_IRQHandler
LPSPI1_DriverIRQHandler
LPSPI2_DriverIRQHandler
PWM1_0_IRQHandler
PWM1_1_IRQHandler
PWM1_2_IRQHandler
PWM1_3_IRQHandler
PWM1_FAULT_IRQHandler
KPP_IRQHandler
SRC_IRQHandler
GPR_IRQ_IRQHandler
CCM_1_IRQHandler
CCM_2_IRQHandler
EWM_IRQHandler
WDOG2_IRQHandler
SNVS_HP_WRAPPER_IRQHandler
SNVS_HP_WRAPPER_TZ_IRQHandler
SNVS_LP_WRAPPER_IRQHandler
CSU_IRQHandler
DCP_IRQHandler
DCP_VMI_IRQHandler
Reserved68_IRQHandler
TRNG_IRQHandler
Reserved70_IRQHandler
Reserved71_IRQHandler
SAI1_DriverIRQHandler
RTWDOG_IRQHandler
SAI3_RX_DriverIRQHandler
SAI3_TX_DriverIRQHandler
SPDIF_DriverIRQHandler
PMU_IRQHandler
XBAR1_IRQ_0_1_2_3_IRQHandler
TEMP_LOW_HIGH_IRQHandler
TEMP_PANIC_IRQHandler
USB_PHY_IRQHandler
GPC_IRQHandler
ADC1_IRQHandler
FLEXIO1_DriverIRQHandler
DCDC_IRQHandler
GPIO1_Combined_0_15_IRQHandler
GPIO1_Combined_16_31_IRQHandler
GPIO2_Combined_0_15_IRQHandler
GPIO5_Combined_0_15_IRQHandler
WDOG1_IRQHandler
ADC_ETC_IRQ0_IRQHandler
ADC_ETC_IRQ1_IRQHandler
ADC_ETC_IRQ2_IRQHandler
ADC_ETC_IRQ3_IRQHandler
ADC_ETC_ERROR_IRQ_IRQHandler
DefaultISR
                LDR    R0, =DefaultISR
                BX     R0
                ENDP
                  ALIGN


                END
