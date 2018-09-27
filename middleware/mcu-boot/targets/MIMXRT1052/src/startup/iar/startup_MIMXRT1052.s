; -------------------------------------------------------------------------
;  @file:    startup_MIMXRT1052.s
;  @purpose: CMSIS Cortex-M7 Core Device Startup File
;            MIMXRT1052
;  @version: 0.1
;  @date:    2017-1-10
;  @build:   b180509
; -------------------------------------------------------------------------
;
; Copyright (c) 1997-2015 Freescale Semiconductor, Inc.
; Copyright 2016-2018 NXP
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
        DCD     MemManage_Handler                             ;MPU Fault Handler
        DCD     BusFault_Handler                              ;Bus Fault Handler
        DCD     UsageFault_Handler                            ;Usage Fault Handler
__vector_table_0x1c
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     DebugMon_Handler                              ;Debug Monitor Handler
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     Reserved16_IRQHandler                         ;Reserved interrupt
        DCD     Reserved17_IRQHandler                         ;Reserved interrupt
        DCD     Reserved18_IRQHandler                         ;Reserved interrupt
        DCD     Reserved19_IRQHandler                         ;Reserved interrupt
        DCD     Reserved20_IRQHandler                         ;Reserved interrupt
        DCD     Reserved21_IRQHandler                         ;Reserved interrupt
        DCD     Reserved22_IRQHandler                         ;Reserved interrupt
        DCD     Reserved23_IRQHandler                         ;Reserved interrupt
        DCD     Reserved24_IRQHandler                         ;Reserved interrupt
        DCD     Reserved25_IRQHandler                         ;Reserved interrupt
        DCD     Reserved26_IRQHandler                         ;Reserved interrupt
        DCD     Reserved27_IRQHandler                         ;Reserved interrupt
        DCD     Reserved28_IRQHandler                         ;Reserved interrupt
        DCD     Reserved29_IRQHandler                         ;Reserved interrupt
        DCD     Reserved30_IRQHandler                         ;Reserved interrupt
        DCD     Reserved31_IRQHandler                         ;Reserved interrupt
        DCD     Reserved32_IRQHandler                         ;Reserved interrupt
        DCD     CTI0_ERROR_IRQHandler                         ;CTI0_Error
        DCD     CTI1_ERROR_IRQHandler                         ;CTI1_Error
        DCD     CORE_IRQHandler                               ;CorePlatform exception IRQ
        DCD     LPUART1_IRQHandler                            ;LPUART1 status and error
        DCD     LPUART2_IRQHandler                            ;LPUART2 status and error
        DCD     LPUART3_IRQHandler                            ;LPUART3 status and error
        DCD     LPUART4_IRQHandler                            ;LPUART4 status and error
        DCD     LPUART5_IRQHandler                            ;LPUART5 status and error
        DCD     LPUART6_IRQHandler                            ;LPUART6 status and error
        DCD     LPUART7_IRQHandler                            ;LPUART7 status and error
        DCD     LPUART8_IRQHandler                            ;LPUART8 status and error
        DCD     LPI2C1_IRQHandler                             ;LPI2C1 interrupt
        DCD     LPI2C2_IRQHandler                             ;LPI2C2 interrupt
        DCD     LPI2C3_IRQHandler                             ;LPI2C3 interrupt
        DCD     LPI2C4_IRQHandler                             ;LPI2C4 interrupt
        DCD     LPSPI1_IRQHandler                             ;LPSPI1 single interrupt vector for all sources
        DCD     LPSPI2_IRQHandler                             ;LPSPI2 single interrupt vector for all sources
        DCD     LPSPI3_IRQHandler                             ;LPSPI3 single interrupt vector for all sources
        DCD     LPSPI4_IRQHandler                             ;LPSPI4  single interrupt vector for all sources
        DCD     CAN1_IRQHandler                               ;CAN1 interrupt
        DCD     CAN2_IRQHandler                               ;CAN2 interrupt
        DCD     FLEXRAM_IRQHandler                            ;FlexRAM address out of range Or access hit IRQ
        DCD     KPP_IRQHandler                                ;Keypad nterrupt
        DCD     TSC_DIG_IRQHandler                            ;TSC interrupt
        DCD     GPR_IRQ_IRQHandler                            ;GPR interrupt
        DCD     Reserved58_IRQHandler                         ;Reserved interrupt
        DCD     Reserved59_IRQHandler                         ;Reserved interrupt
        DCD     Reserved60_IRQHandler                         ;Reserved interrupt
        DCD     WDOG2_IRQHandler                              ;WDOG2 interrupt
        DCD     SNVS_HP_WRAPPER_IRQHandler                    ;SRTC Consolidated Interrupt. Non TZ
        DCD     SNVS_HP_WRAPPER_TZ_IRQHandler                 ;SRTC Security Interrupt. TZ
        DCD     SNVS_LP_WRAPPER_IRQHandler                    ;ON-OFF button press shorter than 5 secs (pulse event)
        DCD     Reserved65_IRQHandler                         ;Reserved interrupt
        DCD     DCP_IRQHandler                                ;DCP_IRQ interrupt
        DCD     DCP_VMI_IRQHandler                            ;DCP_VMI_IRQ interrupt
        DCD     DCP_SECURE_IRQHandler                         ;DCP_SECURE_IRQ interrupt
        DCD     Reserved69_IRQHandler                         ;Reserved interrupt
        DCD     Reserved70_IRQHandler                         ;Reserved interrupt
        DCD     BEE_IRQHandler                                ;BEE interrupt
        DCD     Reserved72_IRQHandler                         ;Reserved interrupt
        DCD     Reserved73_IRQHandler                         ;Reserved interrupt
        DCD     Reserved74_IRQHandler                         ;Reserved interrupt
        DCD     Reserved75_IRQHandler                         ;Reserved interrupt
        DCD     SPDIF_IRQHandler                              ;SPDIF interrupt
        DCD     Reserved77_IRQHandler                         ;Reserved interrupt
        DCD     Reserved78_IRQHandler                         ;Reserved interrupt
        DCD     Reserved79_IRQHandler                         ;Reserved interrupt
        DCD     Reserved80_IRQHandler                         ;Reserved interrupt
        DCD     USB_PHY1_IRQHandler                           ;USBPHY (UTMI0), Interrupt
        DCD     USB_PHY2_IRQHandler                           ;USBPHY (UTMI0), Interrupt
        DCD     ADC1_IRQHandler                               ;ADC1 interrupt
        DCD     ADC_5HC_IRQHandler                            ;ADC_5HC interrupt
        DCD     DCDC_IRQHandler                               ;DCDC interrupt
        DCD     Reserved86_IRQHandler                         ;Reserved interrupt
        DCD     Reserved87_IRQHandler                         ;Reserved interrupt
        DCD     GPIO1_INT0_IRQHandler                         ;Active HIGH Interrupt from INT0 from GPIO
        DCD     GPIO1_INT1_IRQHandler                         ;Active HIGH Interrupt from INT1 from GPIO
        DCD     GPIO1_INT2_IRQHandler                         ;Active HIGH Interrupt from INT2 from GPIO
        DCD     GPIO1_INT3_IRQHandler                         ;Active HIGH Interrupt from INT3 from GPIO
        DCD     GPIO1_INT4_IRQHandler                         ;Active HIGH Interrupt from INT4 from GPIO
        DCD     GPIO1_INT5_IRQHandler                         ;Active HIGH Interrupt from INT5 from GPIO
        DCD     GPIO1_INT6_IRQHandler                         ;Active HIGH Interrupt from INT6 from GPIO
        DCD     GPIO1_INT7_IRQHandler                         ;Active HIGH Interrupt from INT7 from GPIO
        DCD     GPIO1_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO1 signal 0 throughout 15
        DCD     GPIO1_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO1 signal 16 throughout 31
        DCD     GPIO2_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO2 signal 0 throughout 15
        DCD     GPIO2_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO2 signal 16 throughout 31
        DCD     GPIO3_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO3 signal 0 throughout 15
        DCD     GPIO3_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO3 signal 16 throughout 31
        DCD     GPIO4_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO4 signal 0 throughout 15
        DCD     GPIO4_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO4 signal 16 throughout 31
        DCD     GPIO5_Combined_0_15_IRQHandler                ;Combined interrupt indication for GPIO5 signal 0 throughout 15
        DCD     GPIO5_Combined_16_31_IRQHandler               ;Combined interrupt indication for GPIO5 signal 16 throughout 31
        DCD     FLEXIO1_IRQHandler                            ;FLEXIO1 interrupt
        DCD     FLEXIO2_IRQHandler                            ;FLEXIO2 interrupt
        DCD     WDOG1_IRQHandler                              ;WDOG1 interrupt
        DCD     RTWDOG_IRQHandler                             ;RTWDOG interrupt
        DCD     EWM_IRQHandler                                ;EWM interrupt
        DCD     CCM_1_IRQHandler                              ;CCM IRQ1 interrupt
        DCD     CCM_2_IRQHandler                              ;CCM IRQ2 interrupt
        DCD     GPC_IRQHandler                                ;GPC interrupt
        DCD     SRC_IRQHandler                                ;SRC interrupt
        DCD     Reserved115_IRQHandler                        ;Reserved interrupt
        DCD     GPT1_IRQHandler                               ;GPT1 interrupt
        DCD     GPT2_IRQHandler                               ;GPT2 interrupt
        DCD     PWM1_0_IRQHandler                             ;PWM1 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM1_1_IRQHandler                             ;PWM1 capture 1, compare 1, or reload 0 interrupt
        DCD     PWM1_2_IRQHandler                             ;PWM1 capture 2, compare 2, or reload 0 interrupt
        DCD     PWM1_3_IRQHandler                             ;PWM1 capture 3, compare 3, or reload 0 interrupt
        DCD     PWM1_FAULT_IRQHandler                         ;PWM1 fault or reload error interrupt
        DCD     Reserved123_IRQHandler                        ;Reserved interrupt
        DCD     FLEXSPI0_IRQHandler                           ;FlexSPI0 interrupt
        DCD     SEMC_IRQHandler                               ;Reserved interrupt
        DCD     USDHC1_IRQHandler                             ;USDHC1 interrupt
        DCD     USDHC2_IRQHandler                             ;USDHC2 interrupt
        DCD     USB_OTG2_IRQHandler                           ;USBO2 USB OTG2
        DCD     USB_OTG1_IRQHandler                           ;USBO2 USB OTG1
        DCD     ENET_IRQHandler                               ;ENET interrupt
        DCD     ENET_1588_Timer_IRQHandler                    ;ENET_1588_Timer interrupt
        DCD     Reserved132_IRQHandler                        ;Reserved interrupt
        DCD     Reserved133_IRQHandler                        ;Reserved interrupt
        DCD     ADC_ETC_IRQ0_IRQHandler                       ;ADCETC IRQ0 interrupt
        DCD     ADC_ETC_IRQ1_IRQHandler                       ;ADCETC IRQ1 interrupt
        DCD     ADC_ETC_IRQ2_IRQHandler                       ;ADCETC IRQ2 interrupt
        DCD     ADC_ETC_ERROR_IRQ_IRQHandler                  ;ADCETC Error IRQ interrupt
        DCD     PIT_IRQHandler                                ;PIT interrupt
        DCD     ACMP_IRQ1_IRQHandler                          ;ACMP interrupt
        DCD     ACMP_IRQ2_IRQHandler                          ;ACMP interrupt
        DCD     ACMP_IRQ3_IRQHandler                          ;ACMP interrupt
        DCD     ACMP_IRQ4_IRQHandler                          ;ACMP interrupt
        DCD     Reserved143_IRQHandler                        ;Reserved interrupt
        DCD     Reserved144_IRQHandler                        ;Reserved interrupt
        DCD     ENC1_IRQHandler                               ;ENC1 interrupt
        DCD     ENC2_IRQHandler                               ;ENC2 interrupt
        DCD     ENC3_IRQHandler                               ;ENC3 interrupt
        DCD     ENC4_IRQHandler                               ;ENC4 interrupt
        DCD     TMR1_IRQHandler                               ;TMR1 interrupt
        DCD     TMR2_IRQHandler                               ;TMR2 interrupt
        DCD     TMR3_IRQHandler                               ;TMR3 interrupt
        DCD     TMR4_IRQHandler                               ;TMR4 interrupt
        DCD     PWM2_0_IRQHandler                             ;PWM2 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM2_1_IRQHandler                             ;PWM2 capture 1, compare 1, or reload 0 interrupt
        DCD     PWM2_2_IRQHandler                             ;PWM2 capture 2, compare 2, or reload 0 interrupt
        DCD     PWM2_3_IRQHandler                             ;PWM2 capture 3, compare 3, or reload 0 interrupt
        DCD     PWM2_FAULT_IRQHandler                         ;PWM2 fault or reload error interrupt
        DCD     PWM3_0_IRQHandler                             ;PWM3 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM3_1_IRQHandler                             ;PWM3 capture 1, compare 1, or reload 0 interrupt
        DCD     PWM3_2_IRQHandler                             ;PWM3 capture 2, compare 2, or reload 0 interrupt
        DCD     PWM3_3_IRQHandler                             ;PWM3 capture 3, compare 3, or reload 0 interrupt
        DCD     PWM3_FAULT_IRQHandler                         ;PWM3 fault or reload error interrupt
        DCD     PWM4_0_IRQHandler                             ;PWM4 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM4_1_IRQHandler                             ;PWM4 capture 1, compare 1, or reload 0 interrupt
        DCD     PWM4_2_IRQHandler                             ;PWM4 capture 2, compare 2, or reload 0 interrupt
        DCD     PWM4_3_IRQHandler                             ;PWM4 capture 3, compare 3, or reload 0 interrupt
        DCD     PWM4_FAULT_IRQHandler                         ;PWM4 fault or reload error interrupt
        DCD     DefaultISR                                    ;168
        DCD     DefaultISR                                    ;169
        DCD     DefaultISR                                    ;170
        DCD     DefaultISR                                    ;171
        DCD     DefaultISR                                    ;172
        DCD     DefaultISR                                    ;173
        DCD     DefaultISR                                    ;174
        DCD     DefaultISR                                    ;175
        DCD     DefaultISR                                    ;176
        DCD     DefaultISR                                    ;177
        DCD     DefaultISR                                    ;178
        DCD     DefaultISR                                    ;179
        DCD     DefaultISR                                    ;180
        DCD     DefaultISR                                    ;181
        DCD     DefaultISR                                    ;182
        DCD     DefaultISR                                    ;183
        DCD     DefaultISR                                    ;184
        DCD     DefaultISR                                    ;185
        DCD     DefaultISR                                    ;186
        DCD     DefaultISR                                    ;187
        DCD     DefaultISR                                    ;188
        DCD     DefaultISR                                    ;189
        DCD     DefaultISR                                    ;190
        DCD     DefaultISR                                    ;191
        DCD     DefaultISR                                    ;192
        DCD     DefaultISR                                    ;193
        DCD     DefaultISR                                    ;194
        DCD     DefaultISR                                    ;195
        DCD     DefaultISR                                    ;196
        DCD     DefaultISR                                    ;197
        DCD     DefaultISR                                    ;198
        DCD     DefaultISR                                    ;199
        DCD     DefaultISR                                    ;200
        DCD     DefaultISR                                    ;201
        DCD     DefaultISR                                    ;202
        DCD     DefaultISR                                    ;203
        DCD     DefaultISR                                    ;204
        DCD     DefaultISR                                    ;205
        DCD     DefaultISR                                    ;206
        DCD     DefaultISR                                    ;207
        DCD     DefaultISR                                    ;208
        DCD     DefaultISR                                    ;209
        DCD     DefaultISR                                    ;210
        DCD     DefaultISR                                    ;211
        DCD     DefaultISR                                    ;212
        DCD     DefaultISR                                    ;213
        DCD     DefaultISR                                    ;214
        DCD     DefaultISR                                    ;215
        DCD     DefaultISR                                    ;216
        DCD     DefaultISR                                    ;217
        DCD     DefaultISR                                    ;218
        DCD     DefaultISR                                    ;219
        DCD     DefaultISR                                    ;220
        DCD     DefaultISR                                    ;221
        DCD     DefaultISR                                    ;222
        DCD     DefaultISR                                    ;223
        DCD     DefaultISR                                    ;224
        DCD     DefaultISR                                    ;225
        DCD     DefaultISR                                    ;226
        DCD     DefaultISR                                    ;227
        DCD     DefaultISR                                    ;228
        DCD     DefaultISR                                    ;229
        DCD     DefaultISR                                    ;230
        DCD     DefaultISR                                    ;231
        DCD     DefaultISR                                    ;232
        DCD     DefaultISR                                    ;233
        DCD     DefaultISR                                    ;234
        DCD     DefaultISR                                    ;235
        DCD     DefaultISR                                    ;236
        DCD     DefaultISR                                    ;237
        DCD     DefaultISR                                    ;238
        DCD     DefaultISR                                    ;239
        DCD     DefaultISR                                    ;240
        DCD     DefaultISR                                    ;241
        DCD     DefaultISR                                    ;242
        DCD     DefaultISR                                    ;243
        DCD     DefaultISR                                    ;244
        DCD     DefaultISR                                    ;245
        DCD     DefaultISR                                    ;246
        DCD     DefaultISR                                    ;247
        DCD     DefaultISR                                    ;248
        DCD     DefaultISR                                    ;249
        DCD     DefaultISR                                    ;250
        DCD     DefaultISR                                    ;251
        DCD     DefaultISR                                    ;252
        DCD     DefaultISR                                    ;253
        DCD     DefaultISR                                    ;254
        DCD     0xFFFFFFFF                                    ; Reserved for user TRIM value
__Vectors_End

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

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B .

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B .

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK Reserved16_IRQHandler
        PUBWEAK Reserved17_IRQHandler
        PUBWEAK Reserved18_IRQHandler
        PUBWEAK Reserved19_IRQHandler
        PUBWEAK Reserved20_IRQHandler
        PUBWEAK Reserved21_IRQHandler
        PUBWEAK Reserved22_IRQHandler
        PUBWEAK Reserved23_IRQHandler
        PUBWEAK Reserved24_IRQHandler
        PUBWEAK Reserved25_IRQHandler
        PUBWEAK Reserved26_IRQHandler
        PUBWEAK Reserved27_IRQHandler
        PUBWEAK Reserved28_IRQHandler
        PUBWEAK Reserved29_IRQHandler
        PUBWEAK Reserved30_IRQHandler
        PUBWEAK Reserved31_IRQHandler
        PUBWEAK Reserved32_IRQHandler
        PUBWEAK CTI0_ERROR_IRQHandler
        PUBWEAK CTI1_ERROR_IRQHandler
        PUBWEAK CORE_IRQHandler
        PUBWEAK LPUART1_IRQHandler
        PUBWEAK LPUART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_IRQHandler
        LDR     R0, =LPUART1_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_IRQHandler
        LDR     R0, =LPUART2_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART3_IRQHandler
        PUBWEAK LPUART3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART3_IRQHandler
        LDR     R0, =LPUART3_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART4_IRQHandler
        PUBWEAK LPUART4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART4_IRQHandler
        LDR     R0, =LPUART4_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART5_IRQHandler
        PUBWEAK LPUART5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART5_IRQHandler
        LDR     R0, =LPUART5_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART6_IRQHandler
        PUBWEAK LPUART6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART6_IRQHandler
        LDR     R0, =LPUART6_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART7_IRQHandler
        PUBWEAK LPUART7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART7_IRQHandler
        LDR     R0, =LPUART7_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART8_IRQHandler
        PUBWEAK LPUART8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART8_IRQHandler
        LDR     R0, =LPUART8_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C1_IRQHandler
        PUBWEAK LPI2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C1_IRQHandler
        LDR     R0, =LPI2C1_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C2_IRQHandler
        PUBWEAK LPI2C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C2_IRQHandler
        LDR     R0, =LPI2C2_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C3_IRQHandler
        PUBWEAK LPI2C3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C3_IRQHandler
        LDR     R0, =LPI2C3_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C4_IRQHandler
        PUBWEAK LPI2C4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C4_IRQHandler
        LDR     R0, =LPI2C4_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI1_IRQHandler
        PUBWEAK LPSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI1_IRQHandler
        LDR     R0, =LPSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI2_IRQHandler
        PUBWEAK LPSPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI2_IRQHandler
        LDR     R0, =LPSPI2_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI3_IRQHandler
        PUBWEAK LPSPI3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI3_IRQHandler
        LDR     R0, =LPSPI3_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI4_IRQHandler
        PUBWEAK LPSPI4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI4_IRQHandler
        LDR     R0, =LPSPI4_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_IRQHandler
        PUBWEAK CAN1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN2_IRQHandler
        PUBWEAK CAN2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN2_IRQHandler
        LDR     R0, =CAN2_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXRAM_IRQHandler
        PUBWEAK KPP_IRQHandler
        PUBWEAK TSC_DIG_IRQHandler
        PUBWEAK GPR_IRQ_IRQHandler
        PUBWEAK Reserved58_IRQHandler
        PUBWEAK Reserved59_IRQHandler
        PUBWEAK Reserved60_IRQHandler
        PUBWEAK WDOG2_IRQHandler
        PUBWEAK SNVS_HP_WRAPPER_IRQHandler
        PUBWEAK SNVS_HP_WRAPPER_TZ_IRQHandler
        PUBWEAK SNVS_LP_WRAPPER_IRQHandler
        PUBWEAK Reserved65_IRQHandler
        PUBWEAK DCP_IRQHandler
        PUBWEAK DCP_VMI_IRQHandler
        PUBWEAK DCP_SECURE_IRQHandler
        PUBWEAK Reserved69_IRQHandler
        PUBWEAK Reserved70_IRQHandler
        PUBWEAK BEE_IRQHandler
        PUBWEAK Reserved72_IRQHandler
        PUBWEAK Reserved73_IRQHandler
        PUBWEAK Reserved74_IRQHandler
        PUBWEAK Reserved75_IRQHandler
        PUBWEAK SPDIF_IRQHandler
        PUBWEAK SPDIF_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPDIF_IRQHandler
        LDR     R0, =SPDIF_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved77_IRQHandler
        PUBWEAK Reserved78_IRQHandler
        PUBWEAK Reserved79_IRQHandler
        PUBWEAK Reserved80_IRQHandler
        PUBWEAK USB_PHY1_IRQHandler
        PUBWEAK USB_PHY2_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK ADC_5HC_IRQHandler
        PUBWEAK DCDC_IRQHandler
        PUBWEAK Reserved86_IRQHandler
        PUBWEAK Reserved87_IRQHandler
        PUBWEAK GPIO1_INT0_IRQHandler
        PUBWEAK GPIO1_INT1_IRQHandler
        PUBWEAK GPIO1_INT2_IRQHandler
        PUBWEAK GPIO1_INT3_IRQHandler
        PUBWEAK GPIO1_INT4_IRQHandler
        PUBWEAK GPIO1_INT5_IRQHandler
        PUBWEAK GPIO1_INT6_IRQHandler
        PUBWEAK GPIO1_INT7_IRQHandler
        PUBWEAK GPIO1_Combined_0_15_IRQHandler
        PUBWEAK GPIO1_Combined_16_31_IRQHandler
        PUBWEAK GPIO2_Combined_0_15_IRQHandler
        PUBWEAK GPIO2_Combined_16_31_IRQHandler
        PUBWEAK GPIO3_Combined_0_15_IRQHandler
        PUBWEAK GPIO3_Combined_16_31_IRQHandler
        PUBWEAK GPIO4_Combined_0_15_IRQHandler
        PUBWEAK GPIO4_Combined_16_31_IRQHandler
        PUBWEAK GPIO5_Combined_0_15_IRQHandler
        PUBWEAK GPIO5_Combined_16_31_IRQHandler
        PUBWEAK FLEXIO1_IRQHandler
        PUBWEAK FLEXIO1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO1_IRQHandler
        LDR     R0, =FLEXIO1_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXIO2_IRQHandler
        PUBWEAK FLEXIO2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO2_IRQHandler
        LDR     R0, =FLEXIO2_DriverIRQHandler
        BX      R0

        PUBWEAK WDOG1_IRQHandler
        PUBWEAK RTWDOG_IRQHandler
        PUBWEAK EWM_IRQHandler
        PUBWEAK CCM_1_IRQHandler
        PUBWEAK CCM_2_IRQHandler
        PUBWEAK GPC_IRQHandler
        PUBWEAK SRC_IRQHandler
        PUBWEAK Reserved115_IRQHandler
        PUBWEAK GPT1_IRQHandler
        PUBWEAK GPT2_IRQHandler
        PUBWEAK PWM1_0_IRQHandler
        PUBWEAK PWM1_1_IRQHandler
        PUBWEAK PWM1_2_IRQHandler
        PUBWEAK PWM1_3_IRQHandler
        PUBWEAK PWM1_FAULT_IRQHandler
        PUBWEAK Reserved123_IRQHandler
        PUBWEAK FLEXSPI0_IRQHandler
        PUBWEAK FLEXSPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXSPI0_IRQHandler
        LDR     R0, =FLEXSPI0_DriverIRQHandler
        BX      R0

        PUBWEAK SEMC_IRQHandler
        PUBWEAK USDHC1_IRQHandler
        PUBWEAK USDHC1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC1_IRQHandler
        LDR     R0, =USDHC1_DriverIRQHandler
        BX      R0

        PUBWEAK USDHC2_IRQHandler
        PUBWEAK USDHC2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC2_IRQHandler
        LDR     R0, =USDHC2_DriverIRQHandler
        BX      R0

        PUBWEAK USB_OTG2_IRQHandler
        PUBWEAK USB_OTG1_IRQHandler
        PUBWEAK ENET_IRQHandler
        PUBWEAK ENET_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_IRQHandler
        LDR     R0, =ENET_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_1588_Timer_IRQHandler
        PUBWEAK ENET_1588_Timer_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_1588_Timer_IRQHandler
        LDR     R0, =ENET_1588_Timer_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved132_IRQHandler
        PUBWEAK Reserved133_IRQHandler
        PUBWEAK ADC_ETC_IRQ0_IRQHandler
        PUBWEAK ADC_ETC_IRQ1_IRQHandler
        PUBWEAK ADC_ETC_IRQ2_IRQHandler
        PUBWEAK ADC_ETC_ERROR_IRQ_IRQHandler
        PUBWEAK PIT_IRQHandler
        PUBWEAK ACMP_IRQ1_IRQHandler
        PUBWEAK ACMP_IRQ2_IRQHandler
        PUBWEAK ACMP_IRQ3_IRQHandler
        PUBWEAK ACMP_IRQ4_IRQHandler
        PUBWEAK Reserved143_IRQHandler
        PUBWEAK Reserved144_IRQHandler
        PUBWEAK ENC1_IRQHandler
        PUBWEAK ENC2_IRQHandler
        PUBWEAK ENC3_IRQHandler
        PUBWEAK ENC4_IRQHandler
        PUBWEAK TMR1_IRQHandler
        PUBWEAK TMR2_IRQHandler
        PUBWEAK TMR3_IRQHandler
        PUBWEAK TMR4_IRQHandler
        PUBWEAK PWM2_0_IRQHandler
        PUBWEAK PWM2_1_IRQHandler
        PUBWEAK PWM2_2_IRQHandler
        PUBWEAK PWM2_3_IRQHandler
        PUBWEAK PWM2_FAULT_IRQHandler
        PUBWEAK PWM3_0_IRQHandler
        PUBWEAK PWM3_1_IRQHandler
        PUBWEAK PWM3_2_IRQHandler
        PUBWEAK PWM3_3_IRQHandler
        PUBWEAK PWM3_FAULT_IRQHandler
        PUBWEAK PWM4_0_IRQHandler
        PUBWEAK PWM4_1_IRQHandler
        PUBWEAK PWM4_2_IRQHandler
        PUBWEAK PWM4_3_IRQHandler
        PUBWEAK PWM4_FAULT_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved16_IRQHandler
Reserved17_IRQHandler
Reserved18_IRQHandler
Reserved19_IRQHandler
Reserved20_IRQHandler
Reserved21_IRQHandler
Reserved22_IRQHandler
Reserved23_IRQHandler
Reserved24_IRQHandler
Reserved25_IRQHandler
Reserved26_IRQHandler
Reserved27_IRQHandler
Reserved28_IRQHandler
Reserved29_IRQHandler
Reserved30_IRQHandler
Reserved31_IRQHandler
Reserved32_IRQHandler
CTI0_ERROR_IRQHandler
CTI1_ERROR_IRQHandler
CORE_IRQHandler
LPUART1_DriverIRQHandler
LPUART2_DriverIRQHandler
LPUART3_DriverIRQHandler
LPUART4_DriverIRQHandler
LPUART5_DriverIRQHandler
LPUART6_DriverIRQHandler
LPUART7_DriverIRQHandler
LPUART8_DriverIRQHandler
LPI2C1_DriverIRQHandler
LPI2C2_DriverIRQHandler
LPI2C3_DriverIRQHandler
LPI2C4_DriverIRQHandler
LPSPI1_DriverIRQHandler
LPSPI2_DriverIRQHandler
LPSPI3_DriverIRQHandler
LPSPI4_DriverIRQHandler
CAN1_DriverIRQHandler
CAN2_DriverIRQHandler
FLEXRAM_IRQHandler
KPP_IRQHandler
TSC_DIG_IRQHandler
GPR_IRQ_IRQHandler
Reserved58_IRQHandler
Reserved59_IRQHandler
Reserved60_IRQHandler
WDOG2_IRQHandler
SNVS_HP_WRAPPER_IRQHandler
SNVS_HP_WRAPPER_TZ_IRQHandler
SNVS_LP_WRAPPER_IRQHandler
Reserved65_IRQHandler
DCP_IRQHandler
DCP_VMI_IRQHandler
DCP_SECURE_IRQHandler
Reserved69_IRQHandler
Reserved70_IRQHandler
BEE_IRQHandler
Reserved72_IRQHandler
Reserved73_IRQHandler
Reserved74_IRQHandler
Reserved75_IRQHandler
SPDIF_DriverIRQHandler
Reserved77_IRQHandler
Reserved78_IRQHandler
Reserved79_IRQHandler
Reserved80_IRQHandler
USB_PHY1_IRQHandler
USB_PHY2_IRQHandler
ADC1_IRQHandler
ADC_5HC_IRQHandler
DCDC_IRQHandler
Reserved86_IRQHandler
Reserved87_IRQHandler
GPIO1_INT0_IRQHandler
GPIO1_INT1_IRQHandler
GPIO1_INT2_IRQHandler
GPIO1_INT3_IRQHandler
GPIO1_INT4_IRQHandler
GPIO1_INT5_IRQHandler
GPIO1_INT6_IRQHandler
GPIO1_INT7_IRQHandler
GPIO1_Combined_0_15_IRQHandler
GPIO1_Combined_16_31_IRQHandler
GPIO2_Combined_0_15_IRQHandler
GPIO2_Combined_16_31_IRQHandler
GPIO3_Combined_0_15_IRQHandler
GPIO3_Combined_16_31_IRQHandler
GPIO4_Combined_0_15_IRQHandler
GPIO4_Combined_16_31_IRQHandler
GPIO5_Combined_0_15_IRQHandler
GPIO5_Combined_16_31_IRQHandler
FLEXIO1_DriverIRQHandler
FLEXIO2_DriverIRQHandler
WDOG1_IRQHandler
RTWDOG_IRQHandler
EWM_IRQHandler
CCM_1_IRQHandler
CCM_2_IRQHandler
GPC_IRQHandler
SRC_IRQHandler
Reserved115_IRQHandler
GPT1_IRQHandler
GPT2_IRQHandler
PWM1_0_IRQHandler
PWM1_1_IRQHandler
PWM1_2_IRQHandler
PWM1_3_IRQHandler
PWM1_FAULT_IRQHandler
Reserved123_IRQHandler
FLEXSPI0_DriverIRQHandler
SEMC_IRQHandler
USDHC1_DriverIRQHandler
USDHC2_DriverIRQHandler
USB_OTG2_IRQHandler
USB_OTG1_IRQHandler
ENET_DriverIRQHandler
ENET_1588_Timer_DriverIRQHandler
Reserved132_IRQHandler
Reserved133_IRQHandler
ADC_ETC_IRQ0_IRQHandler
ADC_ETC_IRQ1_IRQHandler
ADC_ETC_IRQ2_IRQHandler
ADC_ETC_ERROR_IRQ_IRQHandler
PIT_IRQHandler
ACMP_IRQ1_IRQHandler
ACMP_IRQ2_IRQHandler
ACMP_IRQ3_IRQHandler
ACMP_IRQ4_IRQHandler
Reserved143_IRQHandler
Reserved144_IRQHandler
ENC1_IRQHandler
ENC2_IRQHandler
ENC3_IRQHandler
ENC4_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
TMR4_IRQHandler
PWM2_0_IRQHandler
PWM2_1_IRQHandler
PWM2_2_IRQHandler
PWM2_3_IRQHandler
PWM2_FAULT_IRQHandler
PWM3_0_IRQHandler
PWM3_1_IRQHandler
PWM3_2_IRQHandler
PWM3_3_IRQHandler
PWM3_FAULT_IRQHandler
PWM4_0_IRQHandler
PWM4_1_IRQHandler
PWM4_2_IRQHandler
PWM4_3_IRQHandler
PWM4_FAULT_IRQHandler
DefaultISR
        B DefaultISR

        END
