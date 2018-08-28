; -------------------------------------------------------------------------
;  @file:    startup_MK66F18.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MK66F18
;  @version: 3.0
;  @date:    2015-3-25
;  @build:   b171205
; -------------------------------------------------------------------------
;
; The Clear BSD License
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2017 NXP
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted (subject to the limitations in the
; disclaimer below) provided that the following conditions are met:
;
; * Redistributions of source code must retain the above copyright
;   notice, this list of conditions and the following disclaimer.
;
; * Redistributions in binary form must reproduce the above copyright
;   notice, this list of conditions and the following disclaimer in the
;   documentation and/or other materials provided with the distribution.
;
; * Neither the name of the copyright holder nor the names of its
;   contributors may be used to endorse or promote products derived from
;   this software without specific prior written permission.
;
; NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
; GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
; HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
; WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
; BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
; WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
; OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
; IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
        DCD     DMA0_DMA16_IRQHandler                         ;DMA Channel 0, 16 Transfer Complete
        DCD     DMA1_DMA17_IRQHandler                         ;DMA Channel 1, 17 Transfer Complete
        DCD     DMA2_DMA18_IRQHandler                         ;DMA Channel 2, 18 Transfer Complete
        DCD     DMA3_DMA19_IRQHandler                         ;DMA Channel 3, 19 Transfer Complete
        DCD     DMA4_DMA20_IRQHandler                         ;DMA Channel 4, 20 Transfer Complete
        DCD     DMA5_DMA21_IRQHandler                         ;DMA Channel 5, 21 Transfer Complete
        DCD     DMA6_DMA22_IRQHandler                         ;DMA Channel 6, 22 Transfer Complete
        DCD     DMA7_DMA23_IRQHandler                         ;DMA Channel 7, 23 Transfer Complete
        DCD     DMA8_DMA24_IRQHandler                         ;DMA Channel 8, 24 Transfer Complete
        DCD     DMA9_DMA25_IRQHandler                         ;DMA Channel 9, 25 Transfer Complete
        DCD     DMA10_DMA26_IRQHandler                        ;DMA Channel 10, 26 Transfer Complete
        DCD     DMA11_DMA27_IRQHandler                        ;DMA Channel 11, 27 Transfer Complete
        DCD     DMA12_DMA28_IRQHandler                        ;DMA Channel 12, 28 Transfer Complete
        DCD     DMA13_DMA29_IRQHandler                        ;DMA Channel 13, 29 Transfer Complete
        DCD     DMA14_DMA30_IRQHandler                        ;DMA Channel 14, 30 Transfer Complete
        DCD     DMA15_DMA31_IRQHandler                        ;DMA Channel 15, 31 Transfer Complete
        DCD     DMA_Error_IRQHandler                          ;DMA Error Interrupt
        DCD     MCM_IRQHandler                                ;Normal Interrupt
        DCD     FTFE_IRQHandler                               ;FTFE Command complete interrupt
        DCD     Read_Collision_IRQHandler                     ;Read Collision Interrupt
        DCD     LVD_LVW_IRQHandler                            ;Low Voltage Detect, Low Voltage Warning
        DCD     LLWU_IRQHandler                               ;Low Leakage Wakeup Unit
        DCD     WDOG_EWM_IRQHandler                           ;WDOG Interrupt
        DCD     RNG_IRQHandler                                ;RNG Interrupt
        DCD     I2C0_IRQHandler                               ;I2C0 interrupt
        DCD     I2C1_IRQHandler                               ;I2C1 interrupt
        DCD     SPI0_IRQHandler                               ;SPI0 Interrupt
        DCD     SPI1_IRQHandler                               ;SPI1 Interrupt
        DCD     I2S0_Tx_IRQHandler                            ;I2S0 transmit interrupt
        DCD     I2S0_Rx_IRQHandler                            ;I2S0 receive interrupt
        DCD     Reserved46_IRQHandler                         ;Reserved interrupt 46
        DCD     UART0_RX_TX_IRQHandler                        ;UART0 Receive/Transmit interrupt
        DCD     UART0_ERR_IRQHandler                          ;UART0 Error interrupt
        DCD     UART1_RX_TX_IRQHandler                        ;UART1 Receive/Transmit interrupt
        DCD     UART1_ERR_IRQHandler                          ;UART1 Error interrupt
        DCD     UART2_RX_TX_IRQHandler                        ;UART2 Receive/Transmit interrupt
        DCD     UART2_ERR_IRQHandler                          ;UART2 Error interrupt
        DCD     UART3_RX_TX_IRQHandler                        ;UART3 Receive/Transmit interrupt
        DCD     UART3_ERR_IRQHandler                          ;UART3 Error interrupt
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt
        DCD     CMP1_IRQHandler                               ;CMP1 interrupt
        DCD     FTM0_IRQHandler                               ;FTM0 fault, overflow and channels interrupt
        DCD     FTM1_IRQHandler                               ;FTM1 fault, overflow and channels interrupt
        DCD     FTM2_IRQHandler                               ;FTM2 fault, overflow and channels interrupt
        DCD     CMT_IRQHandler                                ;CMT interrupt
        DCD     RTC_IRQHandler                                ;RTC interrupt
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds interrupt
        DCD     PIT0_IRQHandler                               ;PIT timer channel 0 interrupt
        DCD     PIT1_IRQHandler                               ;PIT timer channel 1 interrupt
        DCD     PIT2_IRQHandler                               ;PIT timer channel 2 interrupt
        DCD     PIT3_IRQHandler                               ;PIT timer channel 3 interrupt
        DCD     PDB0_IRQHandler                               ;PDB0 Interrupt
        DCD     USB0_IRQHandler                               ;USB0 interrupt
        DCD     USBDCD_IRQHandler                             ;USBDCD Interrupt
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
        DCD     SPI2_IRQHandler                               ;SPI2 Interrupt
        DCD     UART4_RX_TX_IRQHandler                        ;UART4 Receive/Transmit interrupt
        DCD     UART4_ERR_IRQHandler                          ;UART4 Error interrupt
        DCD     Reserved84_IRQHandler                         ;Reserved interrupt 84
        DCD     Reserved85_IRQHandler                         ;Reserved interrupt 85
        DCD     CMP2_IRQHandler                               ;CMP2 interrupt
        DCD     FTM3_IRQHandler                               ;FTM3 fault, overflow and channels interrupt
        DCD     DAC1_IRQHandler                               ;DAC1 interrupt
        DCD     ADC1_IRQHandler                               ;ADC1 interrupt
        DCD     I2C2_IRQHandler                               ;I2C2 interrupt
        DCD     CAN0_ORed_Message_buffer_IRQHandler           ;CAN0 OR'd message buffers interrupt
        DCD     CAN0_Bus_Off_IRQHandler                       ;CAN0 bus off interrupt
        DCD     CAN0_Error_IRQHandler                         ;CAN0 error interrupt
        DCD     CAN0_Tx_Warning_IRQHandler                    ;CAN0 Tx warning interrupt
        DCD     CAN0_Rx_Warning_IRQHandler                    ;CAN0 Rx warning interrupt
        DCD     CAN0_Wake_Up_IRQHandler                       ;CAN0 wake up interrupt
        DCD     SDHC_IRQHandler                               ;SDHC interrupt
        DCD     ENET_1588_Timer_IRQHandler                    ;Ethernet MAC IEEE 1588 Timer Interrupt
        DCD     ENET_Transmit_IRQHandler                      ;Ethernet MAC Transmit Interrupt
        DCD     ENET_Receive_IRQHandler                       ;Ethernet MAC Receive Interrupt
        DCD     ENET_Error_IRQHandler                         ;Ethernet MAC Error and miscelaneous Interrupt
        DCD     LPUART0_IRQHandler                            ;LPUART0 status/error interrupt
        DCD     TSI0_IRQHandler                               ;TSI0 interrupt
        DCD     TPM1_IRQHandler                               ;TPM1 fault, overflow and channels interrupt
        DCD     TPM2_IRQHandler                               ;TPM2 fault, overflow and channels interrupt
        DCD     USBHSDCD_IRQHandler                           ;USBHSDCD, USBHS Phy Interrupt
        DCD     I2C3_IRQHandler                               ;I2C3 interrupt
        DCD     CMP3_IRQHandler                               ;CMP3 interrupt
        DCD     USBHS_IRQHandler                              ;USB high speed OTG interrupt
        DCD     CAN1_ORed_Message_buffer_IRQHandler           ;CAN1 OR'd message buffers interrupt
        DCD     CAN1_Bus_Off_IRQHandler                       ;CAN1 bus off interrupt
        DCD     CAN1_Error_IRQHandler                         ;CAN1 error interrupt
        DCD     CAN1_Tx_Warning_IRQHandler                    ;CAN1 Tx warning interrupt
        DCD     CAN1_Rx_Warning_IRQHandler                    ;CAN1 Rx warning interrupt
        DCD     CAN1_Wake_Up_IRQHandler                       ;CAN1 wake up interrupt
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

        PUBWEAK DMA0_DMA16_IRQHandler
        PUBWEAK DMA0_DMA16_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DMA16_IRQHandler
        LDR     R0, =DMA0_DMA16_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_DMA17_IRQHandler
        PUBWEAK DMA1_DMA17_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_DMA17_IRQHandler
        LDR     R0, =DMA1_DMA17_DriverIRQHandler
        BX      R0

        PUBWEAK DMA2_DMA18_IRQHandler
        PUBWEAK DMA2_DMA18_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA2_DMA18_IRQHandler
        LDR     R0, =DMA2_DMA18_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_DMA19_IRQHandler
        PUBWEAK DMA3_DMA19_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_DMA19_IRQHandler
        LDR     R0, =DMA3_DMA19_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_DMA20_IRQHandler
        PUBWEAK DMA4_DMA20_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_DMA20_IRQHandler
        LDR     R0, =DMA4_DMA20_DriverIRQHandler
        BX      R0

        PUBWEAK DMA5_DMA21_IRQHandler
        PUBWEAK DMA5_DMA21_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA5_DMA21_IRQHandler
        LDR     R0, =DMA5_DMA21_DriverIRQHandler
        BX      R0

        PUBWEAK DMA6_DMA22_IRQHandler
        PUBWEAK DMA6_DMA22_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA6_DMA22_IRQHandler
        LDR     R0, =DMA6_DMA22_DriverIRQHandler
        BX      R0

        PUBWEAK DMA7_DMA23_IRQHandler
        PUBWEAK DMA7_DMA23_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA7_DMA23_IRQHandler
        LDR     R0, =DMA7_DMA23_DriverIRQHandler
        BX      R0

        PUBWEAK DMA8_DMA24_IRQHandler
        PUBWEAK DMA8_DMA24_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA8_DMA24_IRQHandler
        LDR     R0, =DMA8_DMA24_DriverIRQHandler
        BX      R0

        PUBWEAK DMA9_DMA25_IRQHandler
        PUBWEAK DMA9_DMA25_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA9_DMA25_IRQHandler
        LDR     R0, =DMA9_DMA25_DriverIRQHandler
        BX      R0

        PUBWEAK DMA10_DMA26_IRQHandler
        PUBWEAK DMA10_DMA26_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA10_DMA26_IRQHandler
        LDR     R0, =DMA10_DMA26_DriverIRQHandler
        BX      R0

        PUBWEAK DMA11_DMA27_IRQHandler
        PUBWEAK DMA11_DMA27_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA11_DMA27_IRQHandler
        LDR     R0, =DMA11_DMA27_DriverIRQHandler
        BX      R0

        PUBWEAK DMA12_DMA28_IRQHandler
        PUBWEAK DMA12_DMA28_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA12_DMA28_IRQHandler
        LDR     R0, =DMA12_DMA28_DriverIRQHandler
        BX      R0

        PUBWEAK DMA13_DMA29_IRQHandler
        PUBWEAK DMA13_DMA29_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA13_DMA29_IRQHandler
        LDR     R0, =DMA13_DMA29_DriverIRQHandler
        BX      R0

        PUBWEAK DMA14_DMA30_IRQHandler
        PUBWEAK DMA14_DMA30_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA14_DMA30_IRQHandler
        LDR     R0, =DMA14_DMA30_DriverIRQHandler
        BX      R0

        PUBWEAK DMA15_DMA31_IRQHandler
        PUBWEAK DMA15_DMA31_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA15_DMA31_IRQHandler
        LDR     R0, =DMA15_DMA31_DriverIRQHandler
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
        PUBWEAK LLWU_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK RNG_IRQHandler
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

        PUBWEAK UART3_RX_TX_IRQHandler
        PUBWEAK UART3_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART3_RX_TX_IRQHandler
        LDR     R0, =UART3_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART3_ERR_IRQHandler
        PUBWEAK UART3_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART3_ERR_IRQHandler
        LDR     R0, =UART3_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK ADC0_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK FTM0_IRQHandler
        PUBWEAK FTM1_IRQHandler
        PUBWEAK FTM2_IRQHandler
        PUBWEAK CMT_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK PIT0_IRQHandler
        PUBWEAK PIT1_IRQHandler
        PUBWEAK PIT2_IRQHandler
        PUBWEAK PIT3_IRQHandler
        PUBWEAK PDB0_IRQHandler
        PUBWEAK USB0_IRQHandler
        PUBWEAK USBDCD_IRQHandler
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
        PUBWEAK SPI2_IRQHandler
        PUBWEAK SPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI2_IRQHandler
        LDR     R0, =SPI2_DriverIRQHandler
        BX      R0

        PUBWEAK UART4_RX_TX_IRQHandler
        PUBWEAK UART4_RX_TX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART4_RX_TX_IRQHandler
        LDR     R0, =UART4_RX_TX_DriverIRQHandler
        BX      R0

        PUBWEAK UART4_ERR_IRQHandler
        PUBWEAK UART4_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART4_ERR_IRQHandler
        LDR     R0, =UART4_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved84_IRQHandler
        PUBWEAK Reserved85_IRQHandler
        PUBWEAK CMP2_IRQHandler
        PUBWEAK FTM3_IRQHandler
        PUBWEAK DAC1_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK I2C2_IRQHandler
        PUBWEAK I2C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C2_IRQHandler
        LDR     R0, =I2C2_DriverIRQHandler
        BX      R0

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

        PUBWEAK SDHC_IRQHandler
        PUBWEAK SDHC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SDHC_IRQHandler
        LDR     R0, =SDHC_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_1588_Timer_IRQHandler
        PUBWEAK ENET_1588_Timer_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_1588_Timer_IRQHandler
        LDR     R0, =ENET_1588_Timer_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_Transmit_IRQHandler
        PUBWEAK ENET_Transmit_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_Transmit_IRQHandler
        LDR     R0, =ENET_Transmit_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_Receive_IRQHandler
        PUBWEAK ENET_Receive_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_Receive_IRQHandler
        LDR     R0, =ENET_Receive_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_Error_IRQHandler
        PUBWEAK ENET_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_Error_IRQHandler
        LDR     R0, =ENET_Error_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART0_IRQHandler
        PUBWEAK LPUART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_IRQHandler
        LDR     R0, =LPUART0_DriverIRQHandler
        BX      R0

        PUBWEAK TSI0_IRQHandler
        PUBWEAK TPM1_IRQHandler
        PUBWEAK TPM2_IRQHandler
        PUBWEAK USBHSDCD_IRQHandler
        PUBWEAK I2C3_IRQHandler
        PUBWEAK I2C3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C3_IRQHandler
        LDR     R0, =I2C3_DriverIRQHandler
        BX      R0

        PUBWEAK CMP3_IRQHandler
        PUBWEAK USBHS_IRQHandler
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
DMA0_DMA16_DriverIRQHandler
DMA1_DMA17_DriverIRQHandler
DMA2_DMA18_DriverIRQHandler
DMA3_DMA19_DriverIRQHandler
DMA4_DMA20_DriverIRQHandler
DMA5_DMA21_DriverIRQHandler
DMA6_DMA22_DriverIRQHandler
DMA7_DMA23_DriverIRQHandler
DMA8_DMA24_DriverIRQHandler
DMA9_DMA25_DriverIRQHandler
DMA10_DMA26_DriverIRQHandler
DMA11_DMA27_DriverIRQHandler
DMA12_DMA28_DriverIRQHandler
DMA13_DMA29_DriverIRQHandler
DMA14_DMA30_DriverIRQHandler
DMA15_DMA31_DriverIRQHandler
DMA_Error_DriverIRQHandler
MCM_IRQHandler
FTFE_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
LLWU_IRQHandler
WDOG_EWM_IRQHandler
RNG_IRQHandler
I2C0_DriverIRQHandler
I2C1_DriverIRQHandler
SPI0_DriverIRQHandler
SPI1_DriverIRQHandler
I2S0_Tx_DriverIRQHandler
I2S0_Rx_DriverIRQHandler
Reserved46_IRQHandler
UART0_RX_TX_DriverIRQHandler
UART0_ERR_DriverIRQHandler
UART1_RX_TX_DriverIRQHandler
UART1_ERR_DriverIRQHandler
UART2_RX_TX_DriverIRQHandler
UART2_ERR_DriverIRQHandler
UART3_RX_TX_DriverIRQHandler
UART3_ERR_DriverIRQHandler
ADC0_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB0_IRQHandler
USB0_IRQHandler
USBDCD_IRQHandler
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
SPI2_DriverIRQHandler
UART4_RX_TX_DriverIRQHandler
UART4_ERR_DriverIRQHandler
Reserved84_IRQHandler
Reserved85_IRQHandler
CMP2_IRQHandler
FTM3_IRQHandler
DAC1_IRQHandler
ADC1_IRQHandler
I2C2_DriverIRQHandler
CAN0_DriverIRQHandler
SDHC_DriverIRQHandler
ENET_1588_Timer_DriverIRQHandler
ENET_Transmit_DriverIRQHandler
ENET_Receive_DriverIRQHandler
ENET_Error_DriverIRQHandler
LPUART0_DriverIRQHandler
TSI0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
USBHSDCD_IRQHandler
I2C3_DriverIRQHandler
CMP3_IRQHandler
USBHS_IRQHandler
CAN1_DriverIRQHandler
DefaultISR
        B DefaultISR

        END
