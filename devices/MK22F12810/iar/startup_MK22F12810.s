; -------------------------------------------------------------------------
;  @file:    startup_MK22F12810.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MK22F12810
;  @version: 1.6
;  @date:    2015-2-19
;  @build:   b180801
; -------------------------------------------------------------------------
;
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2018 NXP
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
        DCD     DMA0_IRQHandler                               ;DMA Channel 0 Transfer Complete
        DCD     DMA1_IRQHandler                               ;DMA Channel 1 Transfer Complete
        DCD     DMA2_IRQHandler                               ;DMA Channel 2 Transfer Complete
        DCD     DMA3_IRQHandler                               ;DMA Channel 3 Transfer Complete
        DCD     Reserved20_IRQHandler                         ;Reserved interrupt 20
        DCD     Reserved21_IRQHandler                         ;Reserved interrupt 21
        DCD     Reserved22_IRQHandler                         ;Reserved interrupt 22
        DCD     Reserved23_IRQHandler                         ;Reserved interrupt 23
        DCD     Reserved24_IRQHandler                         ;Reserved interrupt 24
        DCD     Reserved25_IRQHandler                         ;Reserved interrupt 25
        DCD     Reserved26_IRQHandler                         ;Reserved interrupt 26
        DCD     Reserved27_IRQHandler                         ;Reserved interrupt 27
        DCD     Reserved28_IRQHandler                         ;Reserved interrupt 28
        DCD     Reserved29_IRQHandler                         ;Reserved interrupt 29
        DCD     Reserved30_IRQHandler                         ;Reserved interrupt 30
        DCD     Reserved31_IRQHandler                         ;Reserved interrupt 31
        DCD     DMA_Error_IRQHandler                          ;DMA Error Interrupt
        DCD     MCM_IRQHandler                                ;Normal Interrupt
        DCD     FTF_IRQHandler                                ;FTFA Command complete interrupt
        DCD     Read_Collision_IRQHandler                     ;Read Collision Interrupt
        DCD     LVD_LVW_IRQHandler                            ;Low Voltage Detect, Low Voltage Warning
        DCD     LLWU_IRQHandler                               ;Low Leakage Wakeup Unit
        DCD     WDOG_EWM_IRQHandler                           ;WDOG Interrupt
        DCD     Reserved39_IRQHandler                         ;Reserved Interrupt 39
        DCD     I2C0_IRQHandler                               ;I2C0 interrupt
        DCD     I2C1_IRQHandler                               ;I2C1 interrupt
        DCD     SPI0_IRQHandler                               ;SPI0 Interrupt
        DCD     SPI1_IRQHandler                               ;SPI1 Interrupt
        DCD     I2S0_Tx_IRQHandler                            ;I2S0 transmit interrupt
        DCD     I2S0_Rx_IRQHandler                            ;I2S0 receive interrupt
        DCD     LPUART0_IRQHandler                            ;LPUART0 status/error interrupt
        DCD     UART0_RX_TX_IRQHandler                        ;UART0 Receive/Transmit interrupt
        DCD     UART0_ERR_IRQHandler                          ;UART0 Error interrupt
        DCD     UART1_RX_TX_IRQHandler                        ;UART1 Receive/Transmit interrupt
        DCD     UART1_ERR_IRQHandler                          ;UART1 Error interrupt
        DCD     UART2_RX_TX_IRQHandler                        ;UART2 Receive/Transmit interrupt
        DCD     UART2_ERR_IRQHandler                          ;UART2 Error interrupt
        DCD     Reserved53_IRQHandler                         ;Reserved interrupt 53
        DCD     Reserved54_IRQHandler                         ;Reserved interrupt 54
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt
        DCD     CMP1_IRQHandler                               ;CMP1 interrupt
        DCD     FTM0_IRQHandler                               ;FTM0 fault, overflow and channels interrupt
        DCD     FTM1_IRQHandler                               ;FTM1 fault, overflow and channels interrupt
        DCD     FTM2_IRQHandler                               ;FTM2 fault, overflow and channels interrupt
        DCD     Reserved61_IRQHandler                         ;Reserved interrupt 61
        DCD     RTC_IRQHandler                                ;RTC interrupt
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds interrupt
        DCD     PIT0_IRQHandler                               ;PIT timer channel 0 interrupt
        DCD     PIT1_IRQHandler                               ;PIT timer channel 1 interrupt
        DCD     PIT2_IRQHandler                               ;PIT timer channel 2 interrupt
        DCD     PIT3_IRQHandler                               ;PIT timer channel 3 interrupt
        DCD     PDB0_IRQHandler                               ;PDB0 Interrupt
        DCD     USB0_IRQHandler                               ;USB0 interrupt
        DCD     Reserved70_IRQHandler                         ;Reserved interrupt 70
        DCD     Reserved71_IRQHandler                         ;Reserved interrupt 71
        DCD     DAC0_IRQHandler                               ;DAC0 interrupt
        DCD     MCG_IRQHandler                                ;MCG Interrupt
        DCD     LPTMR0_IRQHandler                             ;LPTimer interrupt
        DCD     PORTA_IRQHandler                              ;Port A interrupt
        DCD     PORTB_IRQHandler                              ;Port B interrupt
        DCD     PORTC_IRQHandler                              ;Port C interrupt
        DCD     PORTD_IRQHandler                              ;Port D interrupt
        DCD     PORTE_IRQHandler                              ;Port E interrupt
        DCD     SWI_IRQHandler                                ;Software interrupt
        DCD     Reserved81_IRQHandler                         ;Reserved interrupt 81
        DCD     Reserved82_IRQHandler                         ;Reserved interrupt 82
        DCD     Reserved83_IRQHandler                         ;Reserved interrupt 83
        DCD     Reserved84_IRQHandler                         ;Reserved interrupt 84
        DCD     Reserved85_IRQHandler                         ;Reserved interrupt 85
        DCD     Reserved86_IRQHandler                         ;Reserved interrupt 86
        DCD     Reserved87_IRQHandler                         ;Reserved interrupt 87
        DCD     Reserved88_IRQHandler                         ;Reserved interrupt 88
        DCD     ADC1_IRQHandler                               ;ADC1 interrupt
        DCD     Reserved90_IRQHandler                         ;Reserved Interrupt 90
        DCD     Reserved91_IRQHandler                         ;Reserved Interrupt 91
        DCD     Reserved92_IRQHandler                         ;Reserved Interrupt 92
        DCD     Reserved93_IRQHandler                         ;Reserved Interrupt 93
        DCD     Reserved94_IRQHandler                         ;Reserved Interrupt 94
        DCD     Reserved95_IRQHandler                         ;Reserved Interrupt 95
        DCD     Reserved96_IRQHandler                         ;Reserved Interrupt 96
        DCD     Reserved97_IRQHandler                         ;Reserved Interrupt 97
        DCD     Reserved98_IRQHandler                         ;Reserved Interrupt 98
        DCD     Reserved99_IRQHandler                         ;Reserved Interrupt 99
        DCD     Reserved100_IRQHandler                        ;Reserved Interrupt 100
        DCD     Reserved101_IRQHandler                        ;Reserved Interrupt 101
        DCD     DefaultISR                                    ;102
        DCD     DefaultISR                                    ;103
        DCD     DefaultISR                                    ;104
        DCD     DefaultISR                                    ;105
        DCD     DefaultISR                                    ;106
        DCD     DefaultISR                                    ;107
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
        PUBWEAK DMA_Error_IRQHandler
        PUBWEAK DMA_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_Error_IRQHandler
        LDR     R0, =DMA_Error_DriverIRQHandler
        BX      R0

        PUBWEAK MCM_IRQHandler
        PUBWEAK FTF_IRQHandler
        PUBWEAK Read_Collision_IRQHandler
        PUBWEAK LVD_LVW_IRQHandler
        PUBWEAK LLWU_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK Reserved39_IRQHandler
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

        PUBWEAK SPI1_IRQHandler
        PUBWEAK SPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI1_IRQHandler
        LDR     R0, =SPI1_DriverIRQHandler
        BX      R0

        PUBWEAK I2S0_Tx_IRQHandler
        PUBWEAK I2S0_Tx_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2S0_Tx_IRQHandler
        LDR     R0, =I2S0_Tx_DriverIRQHandler
        BX      R0

        PUBWEAK I2S0_Rx_IRQHandler
        PUBWEAK I2S0_Rx_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2S0_Rx_IRQHandler
        LDR     R0, =I2S0_Rx_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART0_IRQHandler
        PUBWEAK LPUART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_IRQHandler
        LDR     R0, =LPUART0_DriverIRQHandler
        BX      R0

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

        PUBWEAK UART2_RX_TX_IRQHandler
        PUBWEAK UART2_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART2_RX_TX_IRQHandler
        LDR     R0, =UART2_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART2_ERR_IRQHandler
        PUBWEAK UART2_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART2_ERR_IRQHandler
        LDR     R0, =UART2_ERR_DriverIRQHandler
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
        PUBWEAK PIT0_IRQHandler
        PUBWEAK PIT1_IRQHandler
        PUBWEAK PIT2_IRQHandler
        PUBWEAK PIT3_IRQHandler
        PUBWEAK PDB0_IRQHandler
        PUBWEAK USB0_IRQHandler
        PUBWEAK Reserved70_IRQHandler
        PUBWEAK Reserved71_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK MCG_IRQHandler
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
        PUBWEAK Reserved84_IRQHandler
        PUBWEAK Reserved85_IRQHandler
        PUBWEAK Reserved86_IRQHandler
        PUBWEAK Reserved87_IRQHandler
        PUBWEAK Reserved88_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK Reserved90_IRQHandler
        PUBWEAK Reserved91_IRQHandler
        PUBWEAK Reserved92_IRQHandler
        PUBWEAK Reserved93_IRQHandler
        PUBWEAK Reserved94_IRQHandler
        PUBWEAK Reserved95_IRQHandler
        PUBWEAK Reserved96_IRQHandler
        PUBWEAK Reserved97_IRQHandler
        PUBWEAK Reserved98_IRQHandler
        PUBWEAK Reserved99_IRQHandler
        PUBWEAK Reserved100_IRQHandler
        PUBWEAK Reserved101_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA0_DriverIRQHandler
DMA1_DriverIRQHandler
DMA2_DriverIRQHandler
DMA3_DriverIRQHandler
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
DMA_Error_DriverIRQHandler
MCM_IRQHandler
FTF_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
LLWU_IRQHandler
WDOG_EWM_IRQHandler
Reserved39_IRQHandler
I2C0_DriverIRQHandler
I2C1_DriverIRQHandler
SPI0_DriverIRQHandler
SPI1_DriverIRQHandler
I2S0_Tx_DriverIRQHandler
I2S0_Rx_DriverIRQHandler
LPUART0_DriverIRQHandler
UART0_RX_TX_DriverIRQHandler
UART0_ERR_DriverIRQHandler
UART1_RX_TX_DriverIRQHandler
UART1_ERR_DriverIRQHandler
UART2_RX_TX_DriverIRQHandler
UART2_ERR_DriverIRQHandler
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
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB0_IRQHandler
USB0_IRQHandler
Reserved70_IRQHandler
Reserved71_IRQHandler
DAC0_IRQHandler
MCG_IRQHandler
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
Reserved84_IRQHandler
Reserved85_IRQHandler
Reserved86_IRQHandler
Reserved87_IRQHandler
Reserved88_IRQHandler
ADC1_IRQHandler
Reserved90_IRQHandler
Reserved91_IRQHandler
Reserved92_IRQHandler
Reserved93_IRQHandler
Reserved94_IRQHandler
Reserved95_IRQHandler
Reserved96_IRQHandler
Reserved97_IRQHandler
Reserved98_IRQHandler
Reserved99_IRQHandler
Reserved100_IRQHandler
Reserved101_IRQHandler
DefaultISR
        B DefaultISR

        END
