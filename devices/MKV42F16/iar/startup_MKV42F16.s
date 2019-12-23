; -------------------------------------------------------------------------
;  @file:    startup_MKV42F16.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MKV42F16
;  @version: 1.3
;  @date:    2015-7-29
;  @build:   b190918
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
        DCD     DMA0_IRQHandler                               ;DMA channel 0, 16 transfer complete
        DCD     DMA1_IRQHandler                               ;DMA channel 1, 17 transfer complete
        DCD     DMA2_IRQHandler                               ;DMA channel 2, 18 transfer complete
        DCD     DMA3_IRQHandler                               ;DMA channel 3, 19 transfer complete
        DCD     DMA4_IRQHandler                               ;DMA channel 4, 20 transfer complete
        DCD     DMA5_IRQHandler                               ;DMA channel 5, 21 transfer complete
        DCD     DMA6_IRQHandler                               ;DMA channel 6, 22 transfer complete
        DCD     DMA7_IRQHandler                               ;DMA channel 7, 23 transfer complete
        DCD     DMA8_IRQHandler                               ;DMA channel 8, 24 transfer complete
        DCD     DMA9_IRQHandler                               ;DMA channel 9, 25 transfer complete
        DCD     DMA10_IRQHandler                              ;DMA channel 10, 26 transfer complete
        DCD     DMA11_IRQHandler                              ;DMA channel 11, 27 transfer complete
        DCD     DMA12_IRQHandler                              ;DMA channel 12, 28 transfer complete
        DCD     DMA13_IRQHandler                              ;DMA channel 13, 29 transfer complete
        DCD     DMA14_IRQHandler                              ;DMA channel 14, 30 transfer complete
        DCD     DMA15_IRQHandler                              ;DMA channel 15, 31 transfer complete
        DCD     DMA_Error_IRQHandler                          ;DMA error interrupt channels 0-1531
        DCD     MCM_IRQHandler                                ;MCM interrupt
        DCD     FTFA_IRQHandler                               ;Command complete
        DCD     FTFA_Collision_IRQHandler                     ;Read collision
        DCD     PMC_IRQHandler                                ;Low-voltage detect, low-voltage warning
        DCD     LLWU_IRQHandler                               ;Low Leakage Wakeup
        DCD     WDOG_EWM_IRQHandler                           ;Both watchdog modules share this interrupt
        DCD     Reserved39_IRQHandler                         ;Reserved interrupt
        DCD     I2C0_IRQHandler                               ;I2C0
        DCD     Reserved41_IRQHandler                         ;Reserved interrupt
        DCD     SPI0_IRQHandler                               ;SPI0
        DCD     Reserved43_IRQHandler                         ;Reserved interrupt
        DCD     Reserved44_IRQHandler                         ;Reserved interrupt
        DCD     Reserved45_IRQHandler                         ;Reserved interrupt
        DCD     Reserved46_IRQHandler                         ;Reserved interrupt
        DCD     UART0_RX_TX_IRQHandler                        ;UART0 status sources
        DCD     UART0_ERR_IRQHandler                          ;UART0 error sources
        DCD     UART1_RX_TX_IRQHandler                        ;UART1 status sources
        DCD     UART1_ERR_IRQHandler                          ;UART1 error sources
        DCD     Reserved51_IRQHandler                         ;Reserved interrupt
        DCD     Reserved52_IRQHandler                         ;Reserved interrupt
        DCD     Reserved53_IRQHandler                         ;Reserved interrupt
        DCD     ADC_ERR_IRQHandler                            ;ADC_ERR A and B ( zero cross, high/low limit)
        DCD     ADCA_IRQHandler                               ;ADCA Scan complete
        DCD     CMP0_IRQHandler                               ;CMP0
        DCD     CMP1_IRQHandler                               ;CMP1
        DCD     FTM0_IRQHandler                               ;FTM0 8 channels
        DCD     FTM1_IRQHandler                               ;FTM1 2 channels
        DCD     Reserved60_IRQHandler                         ;Reserved interrupt
        DCD     Reserved61_IRQHandler                         ;Reserved interrupt
        DCD     Reserved62_IRQHandler                         ;Reserved interrupt
        DCD     Reserved63_IRQHandler                         ;Reserved interrupt
        DCD     PIT0_IRQHandler                               ;PIT Channel 0
        DCD     PIT1_IRQHandler                               ;PIT Channel 1
        DCD     PIT2_IRQHandler                               ;PIT Channel 2
        DCD     PIT3_IRQHandler                               ;PIT Channel 3
        DCD     PDB0_IRQHandler                               ;PDB0
        DCD     Reserved69_IRQHandler                         ;Reserved interrupt
        DCD     XBARA_IRQHandler                              ;XBARA
        DCD     PDB1_IRQHandler                               ;PDB1
        DCD     Reserved72_IRQHandler                         ;Reserved interrupt
        DCD     MCG_IRQHandler                                ;MCG
        DCD     LPTMR0_IRQHandler                             ;LPTMR0
        DCD     PORTA_IRQHandler                              ;Pin detect (Port A)
        DCD     PORTB_IRQHandler                              ;Pin detect (Port B)
        DCD     PORTC_IRQHandler                              ;Pin detect (Port C)
        DCD     PORTD_IRQHandler                              ;Pin detect (Port D)
        DCD     PORTE_IRQHandler                              ;Pin detect (Port E)
        DCD     SWI_IRQHandler                                ;Software
        DCD     Reserved81_IRQHandler                         ;Reserved interrupt
        DCD     ENC0_COMPARE_IRQHandler                       ;ENC0 Compare
        DCD     ENC0_HOME_IRQHandler                          ;ENC0 Home
        DCD     ENC0_WDOG_SAB_IRQHandler                      ;ENC0 Watchdog/Simultaneous A and B change
        DCD     ENC0_INDEX_IRQHandler                         ;ENC0 Index/Roll over/Roll Under
        DCD     CMP2_IRQHandler                               ;CMP2
        DCD     FTM3_IRQHandler                               ;FTM3 8 channels
        DCD     Reserved88_IRQHandler                         ;Reserved interrupt
        DCD     ADCB_IRQHandler                               ;ADCB Scan complete
        DCD     Reserved90_IRQHandler                         ;Reserved interrupt
        DCD     CAN0_ORed_Message_buffer_IRQHandler           ;FLexCAN0 OR'ed Message buffer (0-15)
        DCD     CAN0_Bus_Off_IRQHandler                       ;FLexCAN0 Bus Off
        DCD     CAN0_Error_IRQHandler                         ;FLexCAN0 Error
        DCD     CAN0_Tx_Warning_IRQHandler                    ;FLexCAN0 Transmit Warning
        DCD     CAN0_Rx_Warning_IRQHandler                    ;FLexCAN0 Receive Warning
        DCD     CAN0_Wake_Up_IRQHandler                       ;FLexCAN0 Wake Up
        DCD     Reserved97_IRQHandler                         ;Reserved interrupt
        DCD     Reserved98_IRQHandler                         ;Reserved interrupt
        DCD     Reserved99_IRQHandler                         ;Reserved interrupt
        DCD     Reserved100_IRQHandler                        ;Reserved interrupt
        DCD     Reserved101_IRQHandler                        ;Reserved interrupt
        DCD     Reserved102_IRQHandler                        ;Reserved interrupt
        DCD     Reserved103_IRQHandler                        ;Reserved interrupt
        DCD     Reserved104_IRQHandler                        ;Reserved interrupt
        DCD     Reserved105_IRQHandler                        ;Reserved interrupt
        DCD     Reserved106_IRQHandler                        ;Reserved interrupt
        DCD     Reserved107_IRQHandler                        ;Reserved interrupt
        DCD     CMP3_IRQHandler                               ;CMP3
        DCD     Reserved109_IRQHandler                        ;Reserved interrupt
        DCD     CAN1_ORed_Message_buffer_IRQHandler           ;FLexCAN1 OR'ed Message buffer (0-15)
        DCD     CAN1_Bus_Off_IRQHandler                       ;FLexCAN1 Bus Off
        DCD     CAN1_Error_IRQHandler                         ;FLexCAN1 Error
        DCD     CAN1_Tx_Warning_IRQHandler                    ;FLexCAN1 Transmit Warning
        DCD     CAN1_Rx_Warning_IRQHandler                    ;FLexCAN1 Receive Warning
        DCD     CAN1_Wake_Up_IRQHandler                       ;FLexCAN1 Wake Up
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
        PUBWEAK FTFA_IRQHandler
        PUBWEAK FTFA_Collision_IRQHandler
        PUBWEAK PMC_IRQHandler
        PUBWEAK LLWU_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK Reserved39_IRQHandler
        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C0_IRQHandler
        LDR     R0, =I2C0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved41_IRQHandler
        PUBWEAK SPI0_IRQHandler
        PUBWEAK SPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI0_IRQHandler
        LDR     R0, =SPI0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved43_IRQHandler
        PUBWEAK Reserved44_IRQHandler
        PUBWEAK Reserved45_IRQHandler
        PUBWEAK Reserved46_IRQHandler
        PUBWEAK UART0_RX_TX_IRQHandler
        PUBWEAK UART0_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_RX_TX_IRQHandler
        LDR     R0, =UART0_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART0_ERR_IRQHandler
        PUBWEAK UART0_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_ERR_IRQHandler
        LDR     R0, =UART0_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK UART1_RX_TX_IRQHandler
        PUBWEAK UART1_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_RX_TX_IRQHandler
        LDR     R0, =UART1_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART1_ERR_IRQHandler
        PUBWEAK UART1_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART1_ERR_IRQHandler
        LDR     R0, =UART1_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved51_IRQHandler
        PUBWEAK Reserved52_IRQHandler
        PUBWEAK Reserved53_IRQHandler
        PUBWEAK ADC_ERR_IRQHandler
        PUBWEAK ADCA_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK FTM0_IRQHandler
        PUBWEAK FTM1_IRQHandler
        PUBWEAK Reserved60_IRQHandler
        PUBWEAK Reserved61_IRQHandler
        PUBWEAK Reserved62_IRQHandler
        PUBWEAK Reserved63_IRQHandler
        PUBWEAK PIT0_IRQHandler
        PUBWEAK PIT1_IRQHandler
        PUBWEAK PIT2_IRQHandler
        PUBWEAK PIT3_IRQHandler
        PUBWEAK PDB0_IRQHandler
        PUBWEAK Reserved69_IRQHandler
        PUBWEAK XBARA_IRQHandler
        PUBWEAK PDB1_IRQHandler
        PUBWEAK Reserved72_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_IRQHandler
        PUBWEAK PORTC_IRQHandler
        PUBWEAK PORTD_IRQHandler
        PUBWEAK PORTE_IRQHandler
        PUBWEAK SWI_IRQHandler
        PUBWEAK Reserved81_IRQHandler
        PUBWEAK ENC0_COMPARE_IRQHandler
        PUBWEAK ENC0_HOME_IRQHandler
        PUBWEAK ENC0_WDOG_SAB_IRQHandler
        PUBWEAK ENC0_INDEX_IRQHandler
        PUBWEAK CMP2_IRQHandler
        PUBWEAK FTM3_IRQHandler
        PUBWEAK Reserved88_IRQHandler
        PUBWEAK ADCB_IRQHandler
        PUBWEAK Reserved90_IRQHandler
        PUBWEAK CAN0_ORed_Message_buffer_IRQHandler
        PUBWEAK CAN0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_ORed_Message_buffer_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Bus_Off_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Bus_Off_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Error_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Error_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Tx_Warning_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Tx_Warning_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Rx_Warning_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Rx_Warning_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_Wake_Up_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_Wake_Up_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved97_IRQHandler
        PUBWEAK Reserved98_IRQHandler
        PUBWEAK Reserved99_IRQHandler
        PUBWEAK Reserved100_IRQHandler
        PUBWEAK Reserved101_IRQHandler
        PUBWEAK Reserved102_IRQHandler
        PUBWEAK Reserved103_IRQHandler
        PUBWEAK Reserved104_IRQHandler
        PUBWEAK Reserved105_IRQHandler
        PUBWEAK Reserved106_IRQHandler
        PUBWEAK Reserved107_IRQHandler
        PUBWEAK CMP3_IRQHandler
        PUBWEAK Reserved109_IRQHandler
        PUBWEAK CAN1_ORed_Message_buffer_IRQHandler
        PUBWEAK CAN1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_ORed_Message_buffer_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Bus_Off_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Bus_Off_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Error_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Error_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Tx_Warning_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Tx_Warning_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Rx_Warning_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Rx_Warning_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_Wake_Up_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_Wake_Up_IRQHandler
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
FTFA_IRQHandler
FTFA_Collision_IRQHandler
PMC_IRQHandler
LLWU_IRQHandler
WDOG_EWM_IRQHandler
Reserved39_IRQHandler
I2C0_DriverIRQHandler
Reserved41_IRQHandler
SPI0_DriverIRQHandler
Reserved43_IRQHandler
Reserved44_IRQHandler
Reserved45_IRQHandler
Reserved46_IRQHandler
UART0_RX_TX_DriverIRQHandler
UART0_ERR_DriverIRQHandler
UART1_RX_TX_DriverIRQHandler
UART1_ERR_DriverIRQHandler
Reserved51_IRQHandler
Reserved52_IRQHandler
Reserved53_IRQHandler
ADC_ERR_IRQHandler
ADCA_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
Reserved60_IRQHandler
Reserved61_IRQHandler
Reserved62_IRQHandler
Reserved63_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB0_IRQHandler
Reserved69_IRQHandler
XBARA_IRQHandler
PDB1_IRQHandler
Reserved72_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
SWI_IRQHandler
Reserved81_IRQHandler
ENC0_COMPARE_IRQHandler
ENC0_HOME_IRQHandler
ENC0_WDOG_SAB_IRQHandler
ENC0_INDEX_IRQHandler
CMP2_IRQHandler
FTM3_IRQHandler
Reserved88_IRQHandler
ADCB_IRQHandler
Reserved90_IRQHandler
CAN0_DriverIRQHandler
Reserved97_IRQHandler
Reserved98_IRQHandler
Reserved99_IRQHandler
Reserved100_IRQHandler
Reserved101_IRQHandler
Reserved102_IRQHandler
Reserved103_IRQHandler
Reserved104_IRQHandler
Reserved105_IRQHandler
Reserved106_IRQHandler
Reserved107_IRQHandler
CMP3_IRQHandler
Reserved109_IRQHandler
CAN1_DriverIRQHandler
DefaultISR
        B DefaultISR

        END
