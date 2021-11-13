; -------------------------------------------------------------------------
;  @file:    startup_MKE18F16.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MKE18F16
;  @version: 4.0
;  @date:    2016-9-20
;  @build:   b200320
; -------------------------------------------------------------------------
;
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2020 NXP
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

__iar_init$$done:              ; The vector table is not needed
                      ; until after copy initialization is done

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
        DCD     DMA0_IRQHandler                               ;DMA channel 0 transfer complete
        DCD     DMA1_IRQHandler                               ;DMA channel 1 transfer complete
        DCD     DMA2_IRQHandler                               ;DMA channel 2 transfer complete
        DCD     DMA3_IRQHandler                               ;DMA channel 3 transfer complete
        DCD     DMA4_IRQHandler                               ;DMA channel 4 transfer complete
        DCD     DMA5_IRQHandler                               ;DMA channel 5 transfer complete
        DCD     DMA6_IRQHandler                               ;DMA channel 6 transfer complete
        DCD     DMA7_IRQHandler                               ;DMA channel 7 transfer complete
        DCD     DMA8_IRQHandler                               ;DMA channel 8 transfer complete
        DCD     DMA9_IRQHandler                               ;DMA channel 9 transfer complete
        DCD     DMA10_IRQHandler                              ;DMA channel 10 transfer complete
        DCD     DMA11_IRQHandler                              ;DMA channel 11 transfer complete
        DCD     DMA12_IRQHandler                              ;DMA channel 12 transfer complete
        DCD     DMA13_IRQHandler                              ;DMA channel 13 transfer complete
        DCD     DMA14_IRQHandler                              ;DMA channel 14 transfer complete
        DCD     DMA15_IRQHandler                              ;DMA channel 15 transfer complete
        DCD     DMA_Error_IRQHandler                          ;DMA error interrupt channels 0-15
        DCD     MCM_IRQHandler                                ;FPU sources
        DCD     FTFE_IRQHandler                               ;FTFE command complete
        DCD     Read_Collision_IRQHandler                     ;FTFE read collision
        DCD     LVD_LVW_IRQHandler                            ;PMC controller low-voltage detect, low-voltage warning
        DCD     Doublebit_Fault_IRQHandler                    ;FTFE double bit fault detect
        DCD     WDOG_EWM_IRQHandler                           ;Single interrupt vector for WDOG and EWM
        DCD     Reserved39_IRQHandler                         ;Reserved interrupt
        DCD     LPI2C0_IRQHandler                             ;Inter-integrated circuit 0 interrupt
        DCD     LPI2C1_IRQHandler                             ;Inter-integrated circuit 1 interrupt
        DCD     LPSPI0_IRQHandler                             ;Serial peripheral Interface 0 interrupt
        DCD     LPSPI1_IRQHandler                             ;Serial peripheral Interface 1 interrupt
        DCD     Reserved44_IRQHandler                         ;Reserved interrupt
        DCD     PWT_IRQHandler                                ;PWT interrupt
        DCD     Reserved46_IRQHandler                         ;Reserved interrupt
        DCD     LPUART0_TX_IRQHandler                         ;LPUART0 transmit interrupt
        DCD     LPUART0_RX_IRQHandler                         ;LPUART0 receive interrupt
        DCD     LPUART1_TX_IRQHandler                         ;LPUART1 transmit interrupt
        DCD     LPUART1_RX_IRQHandler                         ;LPUART1 receive interrupt
        DCD     LPUART2_TX_IRQHandler                         ;LPUART2 transmit interrupt
        DCD     LPUART2_RX_IRQHandler                         ;LPUART2 receive interrupt
        DCD     Reserved53_IRQHandler                         ;Reserved interrupt
        DCD     Reserved54_IRQHandler                         ;Reserved interrupt
        DCD     ADC0_IRQHandler                               ;ADC conversion complete interrupt
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt
        DCD     CMP1_IRQHandler                               ;CMP1 interrupt
        DCD     FTM0_IRQHandler                               ;FTM0 single interrupt vector for all sources
        DCD     FTM1_IRQHandler                               ;FTM1 single interrupt vector for all sources
        DCD     FTM2_IRQHandler                               ;FTM2 single interrupt vector for all sources
        DCD     Reserved61_IRQHandler                         ;Reserved interrupt
        DCD     RTC_IRQHandler                                ;RTC alarm interrupt
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds interrupt
        DCD     LPIT0_Ch0_IRQHandler                          ;LPIT channel 0 overflow interrupt
        DCD     LPIT0_Ch1_IRQHandler                          ;LPIT channel 1 overflow interrupt
        DCD     LPIT0_Ch2_IRQHandler                          ;LPIT channel 2 overflow interrupt
        DCD     LPIT0_Ch3_IRQHandler                          ;LPIT channel 3 overflow interrupt
        DCD     PDB0_IRQHandler                               ;Programmable delay block 0 interrupt
        DCD     Reserved69_IRQHandler                         ;Reserved interrupt
        DCD     Reserved70_IRQHandler                         ;Reserved interrupt
        DCD     Reserved71_IRQHandler                         ;Reserved interrupt
        DCD     DAC0_IRQHandler                               ;Digital-to-analog converter 0 interrupt
        DCD     SCG_RCM_IRQHandler                            ;SCG_RCM interrupt
        DCD     LPTMR0_IRQHandler                             ;Single interrupt vector for  Low Power Timer 0 interrupt
        DCD     PORTA_IRQHandler                              ;Port A pin detect interrupt
        DCD     PORTB_IRQHandler                              ;Port B pin detect interrupt
        DCD     PORTC_IRQHandler                              ;Port C pin detect interrupt
        DCD     PORTD_IRQHandler                              ;Port D pin detect interrupt
        DCD     PORTE_IRQHandler                              ;Port E pin detect interrupt
        DCD     SWI_IRQHandler                                ;Software interrupt
        DCD     Reserved81_IRQHandler                         ;Reserved interrupt
        DCD     Reserved82_IRQHandler                         ;Reserved interrupt
        DCD     Reserved83_IRQHandler                         ;Reserved interrupt
        DCD     PDB1_IRQHandler                               ;Programmable delay block 1 interrupt
        DCD     FLEXIO_IRQHandler                             ;FLEXIO interrupt
        DCD     CMP2_IRQHandler                               ;CMP2 interrupt
        DCD     FTM3_IRQHandler                               ;FlexTimer module 3 fault, overflow and channels interrupt
        DCD     Reserved88_IRQHandler                         ;Reserved interrupt
        DCD     ADC1_IRQHandler                               ;ADC conversion complete interrupt
        DCD     ADC2_IRQHandler                               ;ADC conversion complete interrupt
        DCD     Reserved91_IRQHandler                         ;Reserved interrupt
        DCD     Reserved92_IRQHandler                         ;Reserved interrupt
        DCD     PDB2_IRQHandler                               ;Programmable delay block 2 interrupt
        DCD     CAN0_ORed_IRQHandler                          ;can
        DCD     CAN0_Error_IRQHandler                         ;can
        DCD     CAN0_Wake_Up_IRQHandler                       ;can
        DCD     CAN0_ORed_Message_buffer_IRQHandler           ;can
        DCD     CAN0_Reserved1_IRQHandler                     ;can
        DCD     CAN0_Reserved2_IRQHandler                     ;can
        DCD     CAN0_Reserved3_IRQHandler                     ;can
        DCD     CAN1_ORed_IRQHandler                          ;can
        DCD     CAN1_Error_IRQHandler                         ;can
        DCD     CAN1_Wake_Up_IRQHandler                       ;can
        DCD     CAN1_ORed_Message_buffer_IRQHandler           ;can
        DCD     CAN1_Reserved1_IRQHandler                     ;can
        DCD     CAN1_Reserved2_IRQHandler                     ;can
        DCD     CAN1_Reserved3_IRQHandler                     ;can
        DCD     DefaultISR                                    ;108
        DCD     DefaultISR                                    ;109
        DCD     DefaultISR                                    ;110
        DCD     DefaultISR                                    ;111
        DCD     DefaultISR                                    ;112
        DCD     DefaultISR                                    ;113
        DCD     DefaultISR                                    ;114
        DCD     DefaultISR                                    ;115
        DCD     DefaultISR                                    ;116
        DCD     DefaultISR                                    ;117
        DCD     DefaultISR                                    ;118
        DCD     DefaultISR                                    ;119
        DCD     DefaultISR                                    ;120
        DCD     DefaultISR                                    ;121
        DCD     DefaultISR                                    ;122
        DCD     DefaultISR                                    ;123
        DCD     DefaultISR                                    ;124
        DCD     DefaultISR                                    ;125
        DCD     DefaultISR                                    ;126
        DCD     DefaultISR                                    ;127
        DCD     DefaultISR                                    ;128
        DCD     DefaultISR                                    ;129
        DCD     DefaultISR                                    ;130
        DCD     DefaultISR                                    ;131
        DCD     DefaultISR                                    ;132
        DCD     DefaultISR                                    ;133
        DCD     DefaultISR                                    ;134
        DCD     DefaultISR                                    ;135
        DCD     DefaultISR                                    ;136
        DCD     DefaultISR                                    ;137
        DCD     DefaultISR                                    ;138
        DCD     DefaultISR                                    ;139
        DCD     DefaultISR                                    ;140
        DCD     DefaultISR                                    ;141
        DCD     DefaultISR                                    ;142
        DCD     DefaultISR                                    ;143
        DCD     DefaultISR                                    ;144
        DCD     DefaultISR                                    ;145
        DCD     DefaultISR                                    ;146
        DCD     DefaultISR                                    ;147
        DCD     DefaultISR                                    ;148
        DCD     DefaultISR                                    ;149
        DCD     DefaultISR                                    ;150
        DCD     DefaultISR                                    ;151
        DCD     DefaultISR                                    ;152
        DCD     DefaultISR                                    ;153
        DCD     DefaultISR                                    ;154
        DCD     DefaultISR                                    ;155
        DCD     DefaultISR                                    ;156
        DCD     DefaultISR                                    ;157
        DCD     DefaultISR                                    ;158
        DCD     DefaultISR                                    ;159
        DCD     DefaultISR                                    ;160
        DCD     DefaultISR                                    ;161
        DCD     DefaultISR                                    ;162
        DCD     DefaultISR                                    ;163
        DCD     DefaultISR                                    ;164
        DCD     DefaultISR                                    ;165
        DCD     DefaultISR                                    ;166
        DCD     DefaultISR                                    ;167
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

        SECTION FlashConfig:CODE
__FlashConfig
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFF7DFE
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

        PUBWEAK DMA4_IRQHandler
        PUBWEAK DMA4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_IRQHandler
        LDR     R0, =DMA4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA5_IRQHandler
        PUBWEAK DMA5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA5_IRQHandler
        LDR     R0, =DMA5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA6_IRQHandler
        PUBWEAK DMA6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA6_IRQHandler
        LDR     R0, =DMA6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA7_IRQHandler
        PUBWEAK DMA7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA7_IRQHandler
        LDR     R0, =DMA7_DriverIRQHandler
        BX      R0

        PUBWEAK DMA8_IRQHandler
        PUBWEAK DMA8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA8_IRQHandler
        LDR     R0, =DMA8_DriverIRQHandler
        BX      R0

        PUBWEAK DMA9_IRQHandler
        PUBWEAK DMA9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA9_IRQHandler
        LDR     R0, =DMA9_DriverIRQHandler
        BX      R0

        PUBWEAK DMA10_IRQHandler
        PUBWEAK DMA10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA10_IRQHandler
        LDR     R0, =DMA10_DriverIRQHandler
        BX      R0

        PUBWEAK DMA11_IRQHandler
        PUBWEAK DMA11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA11_IRQHandler
        LDR     R0, =DMA11_DriverIRQHandler
        BX      R0

        PUBWEAK DMA12_IRQHandler
        PUBWEAK DMA12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA12_IRQHandler
        LDR     R0, =DMA12_DriverIRQHandler
        BX      R0

        PUBWEAK DMA13_IRQHandler
        PUBWEAK DMA13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA13_IRQHandler
        LDR     R0, =DMA13_DriverIRQHandler
        BX      R0

        PUBWEAK DMA14_IRQHandler
        PUBWEAK DMA14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA14_IRQHandler
        LDR     R0, =DMA14_DriverIRQHandler
        BX      R0

        PUBWEAK DMA15_IRQHandler
        PUBWEAK DMA15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA15_IRQHandler
        LDR     R0, =DMA15_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_Error_IRQHandler
        PUBWEAK DMA_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_Error_IRQHandler
        LDR     R0, =DMA_Error_DriverIRQHandler
        BX      R0

        PUBWEAK MCM_IRQHandler
        PUBWEAK FTFE_IRQHandler
        PUBWEAK Read_Collision_IRQHandler
        PUBWEAK LVD_LVW_IRQHandler
        PUBWEAK Doublebit_Fault_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK Reserved39_IRQHandler
        PUBWEAK LPI2C0_IRQHandler
        PUBWEAK LPI2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C0_IRQHandler
        LDR     R0, =LPI2C0_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C1_IRQHandler
        PUBWEAK LPI2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C1_IRQHandler
        LDR     R0, =LPI2C1_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI0_IRQHandler
        PUBWEAK LPSPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI0_IRQHandler
        LDR     R0, =LPSPI0_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI1_IRQHandler
        PUBWEAK LPSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI1_IRQHandler
        LDR     R0, =LPSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved44_IRQHandler
        PUBWEAK PWT_IRQHandler
        PUBWEAK Reserved46_IRQHandler
        PUBWEAK LPUART0_TX_IRQHandler
        PUBWEAK LPUART0_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_TX_IRQHandler
        LDR     R0, =LPUART0_TX_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART0_RX_IRQHandler
        PUBWEAK LPUART0_RX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_RX_IRQHandler
        LDR     R0, =LPUART0_RX_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART1_TX_IRQHandler
        PUBWEAK LPUART1_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_TX_IRQHandler
        LDR     R0, =LPUART1_TX_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART1_RX_IRQHandler
        PUBWEAK LPUART1_RX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_RX_IRQHandler
        LDR     R0, =LPUART1_RX_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_TX_IRQHandler
        PUBWEAK LPUART2_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_TX_IRQHandler
        LDR     R0, =LPUART2_TX_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_RX_IRQHandler
        PUBWEAK LPUART2_RX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_RX_IRQHandler
        LDR     R0, =LPUART2_RX_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved53_IRQHandler
        PUBWEAK Reserved54_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK FTM0_IRQHandler
        PUBWEAK FTM1_IRQHandler
        PUBWEAK FTM2_IRQHandler
        PUBWEAK Reserved61_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK LPIT0_Ch0_IRQHandler
        PUBWEAK LPIT0_Ch1_IRQHandler
        PUBWEAK LPIT0_Ch2_IRQHandler
        PUBWEAK LPIT0_Ch3_IRQHandler
        PUBWEAK PDB0_IRQHandler
        PUBWEAK Reserved69_IRQHandler
        PUBWEAK Reserved70_IRQHandler
        PUBWEAK Reserved71_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK SCG_RCM_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_IRQHandler
        PUBWEAK PORTC_IRQHandler
        PUBWEAK PORTD_IRQHandler
        PUBWEAK PORTE_IRQHandler
        PUBWEAK SWI_IRQHandler
        PUBWEAK Reserved81_IRQHandler
        PUBWEAK Reserved82_IRQHandler
        PUBWEAK Reserved83_IRQHandler
        PUBWEAK PDB1_IRQHandler
        PUBWEAK FLEXIO_IRQHandler
        PUBWEAK FLEXIO_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO_IRQHandler
        LDR     R0, =FLEXIO_DriverIRQHandler
        BX      R0

        PUBWEAK CMP2_IRQHandler
        PUBWEAK FTM3_IRQHandler
        PUBWEAK Reserved88_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK ADC2_IRQHandler
        PUBWEAK Reserved91_IRQHandler
        PUBWEAK Reserved92_IRQHandler
        PUBWEAK PDB2_IRQHandler
        PUBWEAK CAN0_ORed_IRQHandler
        PUBWEAK CAN0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_ORed_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Error_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Error_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Wake_Up_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Wake_Up_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_ORed_Message_buffer_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_ORed_Message_buffer_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Reserved1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Reserved1_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Reserved2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Reserved2_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Reserved3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Reserved3_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_ORed_IRQHandler
        PUBWEAK CAN1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_ORed_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Error_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Error_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Wake_Up_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Wake_Up_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_ORed_Message_buffer_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_ORed_Message_buffer_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Reserved1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Reserved1_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Reserved2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Reserved2_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Reserved3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Reserved3_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
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
DMA_Error_DriverIRQHandler
MCM_IRQHandler
FTFE_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
Doublebit_Fault_IRQHandler
WDOG_EWM_IRQHandler
Reserved39_IRQHandler
LPI2C0_DriverIRQHandler
LPI2C1_DriverIRQHandler
LPSPI0_DriverIRQHandler
LPSPI1_DriverIRQHandler
Reserved44_IRQHandler
PWT_IRQHandler
Reserved46_IRQHandler
LPUART0_TX_DriverIRQHandler
LPUART0_RX_DriverIRQHandler
LPUART1_TX_DriverIRQHandler
LPUART1_RX_DriverIRQHandler
LPUART2_TX_DriverIRQHandler
LPUART2_RX_DriverIRQHandler
Reserved53_IRQHandler
Reserved54_IRQHandler
ADC0_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
Reserved61_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
LPIT0_Ch0_IRQHandler
LPIT0_Ch1_IRQHandler
LPIT0_Ch2_IRQHandler
LPIT0_Ch3_IRQHandler
PDB0_IRQHandler
Reserved69_IRQHandler
Reserved70_IRQHandler
Reserved71_IRQHandler
DAC0_IRQHandler
SCG_RCM_IRQHandler
LPTMR0_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
SWI_IRQHandler
Reserved81_IRQHandler
Reserved82_IRQHandler
Reserved83_IRQHandler
PDB1_IRQHandler
FLEXIO_DriverIRQHandler
CMP2_IRQHandler
FTM3_IRQHandler
Reserved88_IRQHandler
ADC1_IRQHandler
ADC2_IRQHandler
Reserved91_IRQHandler
Reserved92_IRQHandler
PDB2_IRQHandler
CAN0_DriverIRQHandler
CAN1_DriverIRQHandler
DefaultISR
        B DefaultISR

        END
