; * -------------------------------------------------------------------------
; *  @file:    startup_K32L3A60_cm4.s
; *  @purpose: CMSIS Cortex-M4 Core Device Startup File
; *            K32L3A60_cm4
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
                DCD     CTI0_MCM0_IRQHandler                ;Cross Trigger Interface 0 / Miscellaneous Control Module
                DCD     DMA0_IRQHandler                     ;DMA0 channel 0 transfer complete
                DCD     DMA1_IRQHandler                     ;DMA0 channel 1 transfer complete
                DCD     DMA2_IRQHandler                     ;DMA0 channel 2 transfer complete
                DCD     DMA3_IRQHandler                     ;DMA0 channel 3 transfer complete
                DCD     DMA4_IRQHandler                     ;DMA0 channel 4 transfer complete
                DCD     DMA5_IRQHandler                     ;DMA0 channel 5 transfer complete
                DCD     DMA6_IRQHandler                     ;DMA0 channel 6 transfer complete
                DCD     DMA7_IRQHandler                     ;DMA0 channel 7 transfer complete
                DCD     DMA8_IRQHandler                     ;DMA0 channel 8 transfer complete
                DCD     DMA9_IRQHandler                     ;DMA0 channel 9 transfer complete
                DCD     DMA10_IRQHandler                    ;DMA0 channel 10 transfer complete
                DCD     DMA11_IRQHandler                    ;DMA0 channel 11 transfer complete
                DCD     DMA12_IRQHandler                    ;DMA0 channel 12 transfer complete
                DCD     DMA13_IRQHandler                    ;DMA0 channel 13 transfer complete
                DCD     DMA14_IRQHandler                    ;DMA0 channel 14 transfer complete
                DCD     DMA15_IRQHandler                    ;DMA0 channel 15 transfer complete
                DCD     DMA0_Error_IRQHandler               ;DMA0 channel 0-15 error interrupt
                DCD     MSMC_IRQHandler                     ;MSMC (SMC0) interrupt
                DCD     EWM_IRQHandler                      ;EWM interrupt
                DCD     FTFE_Command_Complete_IRQHandler    ;FTFE interrupt
                DCD     FTFE_Read_Collision_IRQHandler      ;FTFE interrupt
                DCD     LLWU0_IRQHandler                    ;Low leakage wakeup 0
                DCD     MUA_IRQHandler                      ;MU Side A interrupt
                DCD     SPM_IRQHandler                      ;SPM
                DCD     WDOG0_IRQHandler                    ;WDOG0 interrupt
                DCD     SCG_IRQHandler                      ;SCG interrupt
                DCD     LPIT0_IRQHandler                    ;LPIT0 interrupt
                DCD     RTC_IRQHandler                      ;RTC Alarm interrupt
                DCD     LPTMR0_IRQHandler                   ;LPTMR0 interrupt
                DCD     LPTMR1_IRQHandler                   ;LPTMR1 interrupt
                DCD     TPM0_IRQHandler                     ;TPM0 single interrupt vector for all sources
                DCD     TPM1_IRQHandler                     ;TPM1 single interrupt vector for all sources
                DCD     TPM2_IRQHandler                     ;TPM2 single interrupt vector for all sources
                DCD     EMVSIM0_IRQHandler                  ;EMVSIM0 interrupt
                DCD     FLEXIO0_IRQHandler                  ;FLEXIO0
                DCD     LPI2C0_IRQHandler                   ;LPI2C0 interrupt
                DCD     LPI2C1_IRQHandler                   ;LPI2C1 interrupt
                DCD     LPI2C2_IRQHandler                   ;LPI2C2 interrupt
                DCD     I2S0_IRQHandler                     ;I2S0 interrupt
                DCD     USDHC0_IRQHandler                   ;SDHC0 interrupt
                DCD     LPSPI0_IRQHandler                   ;LPSPI0 single interrupt vector for all sources
                DCD     LPSPI1_IRQHandler                   ;LPSPI1 single interrupt vector for all sources
                DCD     LPSPI2_IRQHandler                   ;LPSPI2 single interrupt vector for all sources
                DCD     LPUART0_IRQHandler                  ;LPUART0 status and error
                DCD     LPUART1_IRQHandler                  ;LPUART1 status and error
                DCD     LPUART2_IRQHandler                  ;LPUART2 status and error
                DCD     USB0_IRQHandler                     ;USB0 interrupt
                DCD     PORTA_IRQHandler                    ;PORTA Pin detect
                DCD     PORTB_IRQHandler                    ;PORTB Pin detect
                DCD     PORTC_IRQHandler                    ;PORTC Pin detect
                DCD     PORTD_IRQHandler                    ;PORTD Pin detect
                DCD     LPADC0_IRQHandler                   ;LPADC0 interrupt
                DCD     LPCMP0_IRQHandler                   ;LPCMP0 interrupt
                DCD     LPDAC0_IRQHandler                   ;LPDAC0 interrupt
                DCD     CAU3_Task_Complete_IRQHandler       ;Cryptographic Acceleration Unit version 3 Task Complete
                DCD     CAU3_Security_Violation_IRQHandler  ;Cryptographic Acceleration Unit version 3 Security Violation
                DCD     TRNG_IRQHandler                     ;TRNG interrupt
                DCD     LPIT1_IRQHandler                    ;LPIT1 interrupt
                DCD     LPTMR2_IRQHandler                   ;LPTMR2 interrupt
                DCD     TPM3_IRQHandler                     ;TPM3 single interrupt vector for all sources
                DCD     LPI2C3_IRQHandler                   ;LPI2C3 interrupt
                DCD     LPSPI3_IRQHandler                   ;LPSPI3 single interrupt vector for all sources
                DCD     LPUART3_IRQHandler                  ;LPUART3 status and error
                DCD     PORTE_IRQHandler                    ;PORTE Pin detect
                DCD     LPCMP1_IRQHandler                   ;LPCMP1 interrupt
                DCD     DefaultISR                          ;82
                DCD     DefaultISR                          ;83
                DCD     DefaultISR                          ;84
                DCD     DefaultISR                          ;85
                DCD     DefaultISR                          ;86
                DCD     DefaultISR                          ;87
                DCD     DefaultISR                          ;88
                DCD     DefaultISR                          ;89
                DCD     DefaultISR                          ;90
                DCD     DefaultISR                          ;91
                DCD     DefaultISR                          ;92
                DCD     DefaultISR                          ;93
                DCD     DefaultISR                          ;94
                DCD     DefaultISR                          ;95
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
                DCD     DefaultISR                          ;127
                DCD     DefaultISR                          ;128
                DCD     DefaultISR                          ;129
                DCD     DefaultISR                          ;130
                DCD     DefaultISR                          ;131
                DCD     DefaultISR                          ;132
                DCD     DefaultISR                          ;133
                DCD     DefaultISR                          ;134
                DCD     DefaultISR                          ;135
                DCD     DefaultISR                          ;136
                DCD     DefaultISR                          ;137
                DCD     DefaultISR                          ;138
                DCD     DefaultISR                          ;139
                DCD     DefaultISR                          ;140
                DCD     DefaultISR                          ;141
                DCD     DefaultISR                          ;142
                DCD     DefaultISR                          ;143
                DCD     DefaultISR                          ;144
                DCD     DefaultISR                          ;145
                DCD     DefaultISR                          ;146
                DCD     DefaultISR                          ;147
                DCD     DefaultISR                          ;148
                DCD     DefaultISR                          ;149
                DCD     DefaultISR                          ;150
                DCD     DefaultISR                          ;151
                DCD     DefaultISR                          ;152
                DCD     DefaultISR                          ;153
                DCD     DefaultISR                          ;154
                DCD     DefaultISR                          ;155
                DCD     DefaultISR                          ;156
                DCD     DefaultISR                          ;157
                DCD     DefaultISR                          ;158
                DCD     DefaultISR                          ;159
                DCD     DefaultISR                          ;160
                DCD     DefaultISR                          ;161
                DCD     DefaultISR                          ;162
                DCD     DefaultISR                          ;163
                DCD     DefaultISR                          ;164
                DCD     DefaultISR                          ;165
                DCD     DefaultISR                          ;166
                DCD     DefaultISR                          ;167
                DCD     DefaultISR                          ;168
                DCD     DefaultISR                          ;169
                DCD     DefaultISR                          ;170
                DCD     DefaultISR                          ;171
                DCD     DefaultISR                          ;172
                DCD     DefaultISR                          ;173
                DCD     DefaultISR                          ;174
                DCD     DefaultISR                          ;175
                DCD     DefaultISR                          ;176
                DCD     DefaultISR                          ;177
                DCD     DefaultISR                          ;178
                DCD     DefaultISR                          ;179
                DCD     DefaultISR                          ;180
                DCD     DefaultISR                          ;181
                DCD     DefaultISR                          ;182
                DCD     DefaultISR                          ;183
                DCD     DefaultISR                          ;184
                DCD     DefaultISR                          ;185
                DCD     DefaultISR                          ;186
                DCD     DefaultISR                          ;187
                DCD     DefaultISR                          ;188
                DCD     DefaultISR                          ;189
                DCD     DefaultISR                          ;190
                DCD     DefaultISR                          ;191
                DCD     DefaultISR                          ;192
                DCD     DefaultISR                          ;193
                DCD     DefaultISR                          ;194
                DCD     DefaultISR                          ;195
                DCD     DefaultISR                          ;196
                DCD     DefaultISR                          ;197
                DCD     DefaultISR                          ;198
                DCD     DefaultISR                          ;199
                DCD     DefaultISR                          ;200
                DCD     DefaultISR                          ;201
                DCD     DefaultISR                          ;202
                DCD     DefaultISR                          ;203
                DCD     DefaultISR                          ;204
                DCD     DefaultISR                          ;205
                DCD     DefaultISR                          ;206
                DCD     DefaultISR                          ;207
                DCD     DefaultISR                          ;208
                DCD     DefaultISR                          ;209
                DCD     DefaultISR                          ;210
                DCD     DefaultISR                          ;211
                DCD     DefaultISR                          ;212
                DCD     DefaultISR                          ;213
                DCD     DefaultISR                          ;214
                DCD     DefaultISR                          ;215
                DCD     DefaultISR                          ;216
                DCD     DefaultISR                          ;217
                DCD     DefaultISR                          ;218
                DCD     DefaultISR                          ;219
                DCD     DefaultISR                          ;220
                DCD     DefaultISR                          ;221
                DCD     DefaultISR                          ;222
                DCD     DefaultISR                          ;223
                DCD     DefaultISR                          ;224
                DCD     DefaultISR                          ;225
                DCD     DefaultISR                          ;226
                DCD     DefaultISR                          ;227
                DCD     DefaultISR                          ;228
                DCD     DefaultISR                          ;229
                DCD     DefaultISR                          ;230
                DCD     DefaultISR                          ;231
                DCD     DefaultISR                          ;232
                DCD     DefaultISR                          ;233
                DCD     DefaultISR                          ;234
                DCD     DefaultISR                          ;235
                DCD     DefaultISR                          ;236
                DCD     DefaultISR                          ;237
                DCD     DefaultISR                          ;238
                DCD     DefaultISR                          ;239
                DCD     DefaultISR                          ;240
                DCD     DefaultISR                          ;241
                DCD     DefaultISR                          ;242
                DCD     DefaultISR                          ;243
                DCD     DefaultISR                          ;244
                DCD     DefaultISR                          ;245
                DCD     DefaultISR                          ;246
                DCD     DefaultISR                          ;247
                DCD     DefaultISR                          ;248
                DCD     DefaultISR                          ;249
                DCD     DefaultISR                          ;250
                DCD     DefaultISR                          ;251
                DCD     DefaultISR                          ;252
                DCD     DefaultISR                          ;253
                DCD     DefaultISR                          ;254
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
DMA0_IRQHandler\
                PROC
                EXPORT  DMA0_IRQHandler         [WEAK]
                LDR     R0, =DMA0_DriverIRQHandler
                BX      R0
                ENDP

DMA1_IRQHandler\
                PROC
                EXPORT  DMA1_IRQHandler         [WEAK]
                LDR     R0, =DMA1_DriverIRQHandler
                BX      R0
                ENDP

DMA2_IRQHandler\
                PROC
                EXPORT  DMA2_IRQHandler         [WEAK]
                LDR     R0, =DMA2_DriverIRQHandler
                BX      R0
                ENDP

DMA3_IRQHandler\
                PROC
                EXPORT  DMA3_IRQHandler         [WEAK]
                LDR     R0, =DMA3_DriverIRQHandler
                BX      R0
                ENDP

DMA4_IRQHandler\
                PROC
                EXPORT  DMA4_IRQHandler         [WEAK]
                LDR     R0, =DMA4_DriverIRQHandler
                BX      R0
                ENDP

DMA5_IRQHandler\
                PROC
                EXPORT  DMA5_IRQHandler         [WEAK]
                LDR     R0, =DMA5_DriverIRQHandler
                BX      R0
                ENDP

DMA6_IRQHandler\
                PROC
                EXPORT  DMA6_IRQHandler         [WEAK]
                LDR     R0, =DMA6_DriverIRQHandler
                BX      R0
                ENDP

DMA7_IRQHandler\
                PROC
                EXPORT  DMA7_IRQHandler         [WEAK]
                LDR     R0, =DMA7_DriverIRQHandler
                BX      R0
                ENDP

DMA8_IRQHandler\
                PROC
                EXPORT  DMA8_IRQHandler         [WEAK]
                LDR     R0, =DMA8_DriverIRQHandler
                BX      R0
                ENDP

DMA9_IRQHandler\
                PROC
                EXPORT  DMA9_IRQHandler         [WEAK]
                LDR     R0, =DMA9_DriverIRQHandler
                BX      R0
                ENDP

DMA10_IRQHandler\
                PROC
                EXPORT  DMA10_IRQHandler         [WEAK]
                LDR     R0, =DMA10_DriverIRQHandler
                BX      R0
                ENDP

DMA11_IRQHandler\
                PROC
                EXPORT  DMA11_IRQHandler         [WEAK]
                LDR     R0, =DMA11_DriverIRQHandler
                BX      R0
                ENDP

DMA12_IRQHandler\
                PROC
                EXPORT  DMA12_IRQHandler         [WEAK]
                LDR     R0, =DMA12_DriverIRQHandler
                BX      R0
                ENDP

DMA13_IRQHandler\
                PROC
                EXPORT  DMA13_IRQHandler         [WEAK]
                LDR     R0, =DMA13_DriverIRQHandler
                BX      R0
                ENDP

DMA14_IRQHandler\
                PROC
                EXPORT  DMA14_IRQHandler         [WEAK]
                LDR     R0, =DMA14_DriverIRQHandler
                BX      R0
                ENDP

DMA15_IRQHandler\
                PROC
                EXPORT  DMA15_IRQHandler         [WEAK]
                LDR     R0, =DMA15_DriverIRQHandler
                BX      R0
                ENDP

DMA0_Error_IRQHandler\
                PROC
                EXPORT  DMA0_Error_IRQHandler         [WEAK]
                LDR     R0, =DMA0_Error_DriverIRQHandler
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

Default_Handler\
                PROC
                EXPORT  CTI0_MCM0_IRQHandler         [WEAK]
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
                EXPORT  DMA0_Error_DriverIRQHandler         [WEAK]
                EXPORT  MSMC_IRQHandler         [WEAK]
                EXPORT  EWM_IRQHandler         [WEAK]
                EXPORT  FTFE_Command_Complete_IRQHandler         [WEAK]
                EXPORT  FTFE_Read_Collision_IRQHandler         [WEAK]
                EXPORT  LLWU0_IRQHandler         [WEAK]
                EXPORT  MUA_IRQHandler         [WEAK]
                EXPORT  SPM_IRQHandler         [WEAK]
                EXPORT  WDOG0_IRQHandler         [WEAK]
                EXPORT  SCG_IRQHandler         [WEAK]
                EXPORT  LPIT0_IRQHandler         [WEAK]
                EXPORT  RTC_IRQHandler         [WEAK]
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
                EXPORT  CAU3_Task_Complete_IRQHandler         [WEAK]
                EXPORT  CAU3_Security_Violation_IRQHandler         [WEAK]
                EXPORT  TRNG_IRQHandler         [WEAK]
                EXPORT  LPIT1_IRQHandler         [WEAK]
                EXPORT  LPTMR2_IRQHandler         [WEAK]
                EXPORT  TPM3_IRQHandler         [WEAK]
                EXPORT  LPI2C3_DriverIRQHandler         [WEAK]
                EXPORT  LPSPI3_DriverIRQHandler         [WEAK]
                EXPORT  LPUART3_DriverIRQHandler         [WEAK]
                EXPORT  PORTE_IRQHandler         [WEAK]
                EXPORT  LPCMP1_IRQHandler         [WEAK]
                EXPORT  DefaultISR         [WEAK]
CTI0_MCM0_IRQHandler
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
DMA0_Error_DriverIRQHandler
MSMC_IRQHandler
EWM_IRQHandler
FTFE_Command_Complete_IRQHandler
FTFE_Read_Collision_IRQHandler
LLWU0_IRQHandler
MUA_IRQHandler
SPM_IRQHandler
WDOG0_IRQHandler
SCG_IRQHandler
LPIT0_IRQHandler
RTC_IRQHandler
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
CAU3_Task_Complete_IRQHandler
CAU3_Security_Violation_IRQHandler
TRNG_IRQHandler
LPIT1_IRQHandler
LPTMR2_IRQHandler
TPM3_IRQHandler
LPI2C3_DriverIRQHandler
LPSPI3_DriverIRQHandler
LPUART3_DriverIRQHandler
PORTE_IRQHandler
LPCMP1_IRQHandler
DefaultISR
                LDR    R0, =DefaultISR
                BX     R0
                ENDP
                  ALIGN


                END
