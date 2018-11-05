; -------------------------------------------------------------------------
;  @file:    startup_MKV58F24.s
;  @purpose: CMSIS Cortex-M7 Core Device Startup File
;            MKV58F24
;  @version: 0.3
;  @date:    2016-2-29
;  @build:   b171205
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
        EXTERN  init_data_bss
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
        DCD     DMA0_DMA16_IRQHandler                         ;DMA channel 0/16 transfer complete
        DCD     DMA1_DMA17_IRQHandler                         ;DMA channel 1/17 transfer complete
        DCD     DMA2_DMA18_IRQHandler                         ;DMA channel 2/18 transfer complete
        DCD     DMA3_DMA19_IRQHandler                         ;DMA channel 3/19 transfer complete
        DCD     DMA4_DMA20_IRQHandler                         ;DMA channel 4/20 transfer complete
        DCD     DMA5_DMA21_IRQHandler                         ;DMA channel 5/21 transfer complete
        DCD     DMA6_DMA22_IRQHandler                         ;DMA channel 6/22 transfer complete
        DCD     DMA7_DMA23_IRQHandler                         ;DMA channel 7/23 transfer complete
        DCD     DMA8_DMA24_IRQHandler                         ;DMA channel 8/24 transfer complete
        DCD     DMA9_DMA25_IRQHandler                         ;DMA channel 9/25 transfer complete
        DCD     DMA10_DMA26_IRQHandler                        ;DMA channel 10/26 transfer complete
        DCD     DMA11_DMA27_IRQHandler                        ;DMA channel 11/27 transfer complete
        DCD     DMA12_DMA28_IRQHandler                        ;DMA channel 12/28 transfer complete
        DCD     DMA13_DMA29_IRQHandler                        ;DMA channel 13/29 transfer complete
        DCD     DMA14_DMA30_IRQHandler                        ;DMA channel 14/30 transfer complete
        DCD     DMA15_DMA31_IRQHandler                        ;DMA channel 15/31 transfer complete
        DCD     DMA_Error_IRQHandler                          ;DMA error interrupt channels 0-31
        DCD     MCM_IRQHandler                                ;MCM normal interrupt
        DCD     FTFE_IRQHandler                               ;FTFL command complete
        DCD     Read_Collision_IRQHandler                     ;FTFL read collision
        DCD     PMC_IRQHandler                                ;PMC controller low-voltage detect, low-voltage warning
        DCD     LLWU_IRQHandler                               ;Low leakage wakeup
        DCD     WDOG_EWM_IRQHandler                           ;Single interrupt vector for  WDOG and EWM
        DCD     TRNG0_IRQHandler                              ;True randon number generator
        DCD     I2C0_IRQHandler                               ;Inter-integrated circuit 0
        DCD     I2C1_IRQHandler                               ;Inter-integrated circuit 1
        DCD     SPI0_IRQHandler                               ;Serial peripheral Interface 0
        DCD     SPI1_IRQHandler                               ;Serial peripheral Interface 1
        DCD     UART5_RX_TX_IRQHandler                        ;UART5 receive/transmit interrupt
        DCD     UART5_ERR_IRQHandler                          ;UART5 error interrupt
        DCD     UART0_LON_IRQHandler                          ;UART0 LON
        DCD     UART0_RX_TX_IRQHandler                        ;UART0 receive/transmit interrupt
        DCD     UART0_ERR_IRQHandler                          ;UART0 error interrupt
        DCD     UART1_RX_TX_IRQHandler                        ;UART1 receive/transmit interrupt
        DCD     UART1_ERR_IRQHandler                          ;UART1 error interrupt
        DCD     UART2_RX_TX_IRQHandler                        ;UART2 receive/transmit interrupt
        DCD     UART2_ERR_IRQHandler                          ;UART2 error interrupt
        DCD     ADC0_IRQHandler                               ;Analog-to-digital converter 0
        DCD     HSADC_ERR_IRQHandler                          ;High speed analog-to-digital converter zero cross
        DCD     HSADC0_CCA_IRQHandler                         ;High speed analog-to-digital converter 0 submodule A scan complete
        DCD     CMP0_IRQHandler                               ;Comparator 0
        DCD     CMP1_IRQHandler                               ;Comparator 1
        DCD     FTM0_IRQHandler                               ;FlexTimer module 0 fault, overflow and channels interrupt
        DCD     FTM1_IRQHandler                               ;FlexTimer module 1 fault, overflow and channels interrupt
        DCD     UART3_RX_TX_IRQHandler                        ;UART3 receive/transmit interrupt
        DCD     UART3_ERR_IRQHandler                          ;UART3 error interrupt
        DCD     UART4_RX_TX_IRQHandler                        ;UART4 receive/transmit interrupt
        DCD     UART4_ERR_IRQHandler                          ;UART4 error interrupt
        DCD     PIT0_IRQHandler                               ;Periodic interrupt timer channel 0
        DCD     PIT1_IRQHandler                               ;Periodic interrupt timer channel 1
        DCD     PIT2_IRQHandler                               ;Periodic interrupt timer channel 2
        DCD     PIT3_IRQHandler                               ;Periodic interrupt timer channel 3
        DCD     PDB0_IRQHandler                               ;Programmable delay block 0
        DCD     FTM2_IRQHandler                               ;FlexTimer module 2 fault, overflow and channels interrupt
        DCD     XBARA_IRQHandler                              ;Inter-peripheral crossbar switch A
        DCD     PDB1_IRQHandler                               ;Programmable delay block 1
        DCD     DAC0_IRQHandler                               ;Digital-to-analog converter 0
        DCD     MCG_IRQHandler                                ;Multipurpose clock generator
        DCD     LPTMR0_IRQHandler                             ;Low power timer interrupt
        DCD     PORTA_IRQHandler                              ;Port A interrupt
        DCD     PORTB_IRQHandler                              ;Port B interrupt
        DCD     PORTC_IRQHandler                              ;Port C interrupt
        DCD     PORTD_IRQHandler                              ;Port D interrupt
        DCD     PORTE_IRQHandler                              ;Port E interrupt
        DCD     SWI_IRQHandler                                ;Software interrupt
        DCD     SPI2_IRQHandler                               ;Serial peripheral Interface 2
        DCD     ENC_COMPARE_IRQHandler                        ;ENC Compare
        DCD     ENC_HOME_IRQHandler                           ;ENC Home
        DCD     ENC_WDOG_SAB_IRQHandler                       ;ENC Wdog/SAB
        DCD     ENC_INDEX_IRQHandler                          ;ENC Index/Roll over/Roll Under
        DCD     CMP2_IRQHandler                               ;Comparator 2
        DCD     FTM3_IRQHandler                               ;FlexTimer module 3 fault, overflow and channels
        DCD     Reserved88_IRQHandler                         ;Reserved interrupt
        DCD     HSADC0_CCB_IRQHandler                         ;High speed analog-to-digital converter 0 submodule B scan complete
        DCD     HSADC1_CCA_IRQHandler                         ;High speed analog-to-digital converter 1 submodule A scan complete
        DCD     CAN0_ORed_Message_buffer_IRQHandler           ;Flex controller area network 0 message buffer
        DCD     CAN0_Bus_Off_IRQHandler                       ;Flex controller area network 0 bus off
        DCD     CAN0_Error_IRQHandler                         ;Flex controller area network 0 error
        DCD     CAN0_Tx_Warning_IRQHandler                    ;Flex controller area network 0 transmit
        DCD     CAN0_Rx_Warning_IRQHandler                    ;Flex controller area network 0 receive
        DCD     CAN0_Wake_Up_IRQHandler                       ;Flex controller area network 0 wake up
        DCD     PWM0_CMP0_IRQHandler                          ;Pulse width modulator 0 channel 0 compare
        DCD     PWM0_RELOAD0_IRQHandler                       ;Pulse width modulator 0 channel 0 reload
        DCD     PWM0_CMP1_IRQHandler                          ;Pulse width modulator 0 channel 1 compare
        DCD     PWM0_RELOAD1_IRQHandler                       ;Pulse width modulator 0 channel 1 reload
        DCD     PWM0_CMP2_IRQHandler                          ;Pulse width modulator 0 channel 2 compare
        DCD     PWM0_RELOAD2_IRQHandler                       ;Pulse width modulator 0 channel 2 reload
        DCD     PWM0_CMP3_IRQHandler                          ;Pulse width modulator 0 channel 3 compare
        DCD     PWM0_RELOAD3_IRQHandler                       ;Pulse width modulator 0 channel 3 reload
        DCD     PWM0_CAP_IRQHandler                           ;Pulse width modulator 0 capture
        DCD     PWM0_RERR_IRQHandler                          ;Pulse width modulator 0 reload error
        DCD     PWM0_FAULT_IRQHandler                         ;Pulse width modulator 0 fault
        DCD     CMP3_IRQHandler                               ;Comparator 3
        DCD     HSADC1_CCB_IRQHandler                         ;High speed analog-to-digital converter 1 submodule B scan complete
        DCD     CAN1_ORed_Message_buffer_IRQHandler           ;Flex controller area network 1 message buffer
        DCD     CAN1_Bus_Off_IRQHandler                       ;Flex controller area network 1 bus off
        DCD     CAN1_Error_IRQHandler                         ;Flex controller area network 1 error
        DCD     CAN1_Tx_Warning_IRQHandler                    ;Flex controller area network 1 transmit
        DCD     CAN1_Rx_Warning_IRQHandler                    ;Flex controller area network 1 receive
        DCD     CAN1_Wake_Up_IRQHandler                       ;Flex controller area network 1 wake up
        DCD     ENET_1588_Timer_IRQHandler                    ;Ethernet MAC IEEE 1588 timer
        DCD     ENET_Transmit_IRQHandler                      ;Ethernet MAC transmit
        DCD     ENET_Receive_IRQHandler                       ;Ethernet MAC receive
        DCD     ENET_Error_IRQHandler                         ;Ethernet MAC error and miscelaneous
        DCD     PWM1_CMP0_IRQHandler                          ;Pulse width modulator 1 channel 0 compare
        DCD     PWM1_RELOAD0_IRQHandler                       ;Pulse width modulator 1 channel 0 reload
        DCD     PWM1_CMP1_IRQHandler                          ;Pulse width modulator 1 channel 1 compare
        DCD     PWM1_RELOAD1_IRQHandler                       ;Pulse width modulator 1 channel 1 reload
        DCD     PWM1_CMP2_IRQHandler                          ;Pulse width modulator 1 channel 2 compare
        DCD     PWM1_RELOAD2_IRQHandler                       ;Pulse width modulator 1 channel 2 reload
        DCD     PWM1_CMP3_IRQHandler                          ;Pulse width modulator 1 channel 3 compare
        DCD     PWM1_RELOAD3_IRQHandler                       ;Pulse width modulator 1 channel 3 reload
        DCD     PWM1_CAP_IRQHandler                           ;Pulse width modulator 1 capture
        DCD     PWM1_RERR_IRQHandler                          ;Pulse width modulator 1 reload error
        DCD     PWM1_FAULT_IRQHandler                         ;Pulse width modulator 1 fault
        DCD     CAN2_ORed_Message_buffer_IRQHandler           ;Flex controller area network 2 message buffer
        DCD     CAN2_Bus_Off_IRQHandler                       ;Flex controller area network 2 bus off
        DCD     CAN2_Error_IRQHandler                         ;Flex controller area network 2 error
        DCD     CAN2_Tx_Warning_IRQHandler                    ;Flex controller area network 2 transmit
        DCD     CAN2_Rx_Warning_IRQHandler                    ;Flex controller area network 2 receive
        DCD     CAN2_Wake_Up_IRQHandler                       ;Flex controller area network 2 wake up
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
__Vectors_End

#ifdef BL_HAS_BOOTLOADER_CONFIG

__bootloaderConfigurationArea ; 0x3c0
        DCD     'kcfg'        ; [00:03] tag - Tag value used to validate the bootloader configuration data. Must be set to 'kcfg'.
        DCD     0xFFFFFFFF    ; [04:07] crcStartAddress
        DCD     0xFFFFFFFF    ; [08:0b] crcByteCount
        DCD     0xFFFFFFFF    ; [0c:0f] crcExpectedValue
        DCB     0xFF          ; [10:10] enabledPeripherals
        DCB     0xFF          ; [11:11] i2cSlaveAddress
        DCW     5000           ; [12:13] peripheralDetectionTimeoutMs - Timeout in milliseconds for peripheral detection before jumping to application code
        DCW     0xFFFF        ; [14:15] usbVid
        DCW     0xFFFF        ; [16:17] usbPid
        DCD     0xFFFFFFFF    ; [18:1b] usbStringsPointer
        DCB     0xFF          ; [1c:1c] clockFlags - High Speed and other clock options
        DCB     0xFF          ; [1d:1d] clockDivider - One's complement of clock divider, zero divider is divide by 1
        DCW     0xFFFF        ; [1e:1f] reserved
        ; Fill to align with flash configuration field. 
        REPT    (0x400-0x3e0)/4   ; 0x3E0 - 0x3FF
        DCD     0xFFFFFFFF        ; Reserved for user TRIM value
        ENDR 
#else        
        ; Fill to align with flash configuration field. 
        REPT    (0x400-0x3c0)/4 ; 0x3c0:0x400
        DCD     0xFFFFFFFF      ; Reserved for user TRIM value
        ENDR
#endif // BL_HAS_BOOTLOADER_CONFIG

__FlashConfig
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFE
__FlashConfig_End

__Vectors       EQU   __vector_table
__Vectors_Size 	EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        CPSID   I               ; Mask interrupts
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =init_data_bss
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
        PUBWEAK DMA1_DMA17_IRQHandler
        PUBWEAK DMA2_DMA18_IRQHandler
        PUBWEAK DMA3_DMA19_IRQHandler
        PUBWEAK DMA4_DMA20_IRQHandler
        PUBWEAK DMA5_DMA21_IRQHandler
        PUBWEAK DMA6_DMA22_IRQHandler
        PUBWEAK DMA7_DMA23_IRQHandler
        PUBWEAK DMA8_DMA24_IRQHandler
        PUBWEAK DMA9_DMA25_IRQHandler
        PUBWEAK DMA10_DMA26_IRQHandler
        PUBWEAK DMA11_DMA27_IRQHandler
        PUBWEAK DMA12_DMA28_IRQHandler
        PUBWEAK DMA13_DMA29_IRQHandler
        PUBWEAK DMA14_DMA30_IRQHandler
        PUBWEAK DMA15_DMA31_IRQHandler
        PUBWEAK DMA_Error_IRQHandler
        PUBWEAK MCM_IRQHandler
        PUBWEAK FTFE_IRQHandler
        PUBWEAK Read_Collision_IRQHandler
        PUBWEAK PMC_IRQHandler
        PUBWEAK LLWU_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK TRNG0_IRQHandler
        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C1_IRQHandler
        PUBWEAK SPI0_IRQHandler
        PUBWEAK SPI1_IRQHandler
        PUBWEAK UART5_RX_TX_IRQHandler
        PUBWEAK UART5_ERR_IRQHandler
        PUBWEAK UART0_LON_IRQHandler
        PUBWEAK UART0_RX_TX_IRQHandler
        PUBWEAK UART0_ERR_IRQHandler
        PUBWEAK UART1_RX_TX_IRQHandler
        PUBWEAK UART1_ERR_IRQHandler
        PUBWEAK UART2_RX_TX_IRQHandler
        PUBWEAK UART2_ERR_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK HSADC_ERR_IRQHandler
        PUBWEAK HSADC0_CCA_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK FTM0_IRQHandler
        PUBWEAK FTM1_IRQHandler
        PUBWEAK UART3_RX_TX_IRQHandler
        PUBWEAK UART3_ERR_IRQHandler
        PUBWEAK UART4_RX_TX_IRQHandler
        PUBWEAK UART4_ERR_IRQHandler
        PUBWEAK PIT0_IRQHandler
        PUBWEAK PIT1_IRQHandler
        PUBWEAK PIT2_IRQHandler
        PUBWEAK PIT3_IRQHandler
        PUBWEAK PDB0_IRQHandler
        PUBWEAK FTM2_IRQHandler
        PUBWEAK XBARA_IRQHandler
        PUBWEAK PDB1_IRQHandler
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
        PUBWEAK ENC_COMPARE_IRQHandler
        PUBWEAK ENC_HOME_IRQHandler
        PUBWEAK ENC_WDOG_SAB_IRQHandler
        PUBWEAK ENC_INDEX_IRQHandler
        PUBWEAK CMP2_IRQHandler
        PUBWEAK FTM3_IRQHandler
        PUBWEAK Reserved88_IRQHandler
        PUBWEAK HSADC0_CCB_IRQHandler
        PUBWEAK HSADC1_CCA_IRQHandler
        PUBWEAK CAN0_ORed_Message_buffer_IRQHandler
        PUBWEAK CAN0_Bus_Off_IRQHandler
        PUBWEAK CAN0_Error_IRQHandler
        PUBWEAK CAN0_Tx_Warning_IRQHandler
        PUBWEAK CAN0_Rx_Warning_IRQHandler
        PUBWEAK CAN0_Wake_Up_IRQHandler
        PUBWEAK PWM0_CMP0_IRQHandler
        PUBWEAK PWM0_RELOAD0_IRQHandler
        PUBWEAK PWM0_CMP1_IRQHandler
        PUBWEAK PWM0_RELOAD1_IRQHandler
        PUBWEAK PWM0_CMP2_IRQHandler
        PUBWEAK PWM0_RELOAD2_IRQHandler
        PUBWEAK PWM0_CMP3_IRQHandler
        PUBWEAK PWM0_RELOAD3_IRQHandler
        PUBWEAK PWM0_CAP_IRQHandler
        PUBWEAK PWM0_RERR_IRQHandler
        PUBWEAK PWM0_FAULT_IRQHandler
        PUBWEAK CMP3_IRQHandler
        PUBWEAK HSADC1_CCB_IRQHandler
        PUBWEAK CAN1_ORed_Message_buffer_IRQHandler
        PUBWEAK CAN1_Bus_Off_IRQHandler
        PUBWEAK CAN1_Error_IRQHandler
        PUBWEAK CAN1_Tx_Warning_IRQHandler
        PUBWEAK CAN1_Rx_Warning_IRQHandler
        PUBWEAK CAN1_Wake_Up_IRQHandler
        PUBWEAK ENET_1588_Timer_IRQHandler
        PUBWEAK ENET_Transmit_IRQHandler
        PUBWEAK ENET_Receive_IRQHandler
        PUBWEAK ENET_Error_IRQHandler
        PUBWEAK PWM1_CMP0_IRQHandler
        PUBWEAK PWM1_RELOAD0_IRQHandler
        PUBWEAK PWM1_CMP1_IRQHandler
        PUBWEAK PWM1_RELOAD1_IRQHandler
        PUBWEAK PWM1_CMP2_IRQHandler
        PUBWEAK PWM1_RELOAD2_IRQHandler
        PUBWEAK PWM1_CMP3_IRQHandler
        PUBWEAK PWM1_RELOAD3_IRQHandler
        PUBWEAK PWM1_CAP_IRQHandler
        PUBWEAK PWM1_RERR_IRQHandler
        PUBWEAK PWM1_FAULT_IRQHandler
        PUBWEAK CAN2_ORed_Message_buffer_IRQHandler
        PUBWEAK CAN2_Bus_Off_IRQHandler
        PUBWEAK CAN2_Error_IRQHandler
        PUBWEAK CAN2_Tx_Warning_IRQHandler
        PUBWEAK CAN2_Rx_Warning_IRQHandler
        PUBWEAK CAN2_Wake_Up_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA0_DMA16_IRQHandler
DMA1_DMA17_IRQHandler
DMA2_DMA18_IRQHandler
DMA3_DMA19_IRQHandler
DMA4_DMA20_IRQHandler
DMA5_DMA21_IRQHandler
DMA6_DMA22_IRQHandler
DMA7_DMA23_IRQHandler
DMA8_DMA24_IRQHandler
DMA9_DMA25_IRQHandler
DMA10_DMA26_IRQHandler
DMA11_DMA27_IRQHandler
DMA12_DMA28_IRQHandler
DMA13_DMA29_IRQHandler
DMA14_DMA30_IRQHandler
DMA15_DMA31_IRQHandler
DMA_Error_IRQHandler
MCM_IRQHandler
FTFE_IRQHandler
Read_Collision_IRQHandler
PMC_IRQHandler
LLWU_IRQHandler
WDOG_EWM_IRQHandler
TRNG0_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler
UART0_LON_IRQHandler
UART0_RX_TX_IRQHandler
UART0_ERR_IRQHandler
UART1_RX_TX_IRQHandler
UART1_ERR_IRQHandler
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler
ADC0_IRQHandler
HSADC_ERR_IRQHandler
HSADC0_CCA_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB0_IRQHandler
FTM2_IRQHandler
XBARA_IRQHandler
PDB1_IRQHandler
DAC0_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
SWI_IRQHandler
SPI2_IRQHandler
ENC_COMPARE_IRQHandler
ENC_HOME_IRQHandler
ENC_WDOG_SAB_IRQHandler
ENC_INDEX_IRQHandler
CMP2_IRQHandler
FTM3_IRQHandler
Reserved88_IRQHandler
HSADC0_CCB_IRQHandler
HSADC1_CCA_IRQHandler
CAN0_ORed_Message_buffer_IRQHandler
CAN0_Bus_Off_IRQHandler
CAN0_Error_IRQHandler
CAN0_Tx_Warning_IRQHandler
CAN0_Rx_Warning_IRQHandler
CAN0_Wake_Up_IRQHandler
PWM0_CMP0_IRQHandler
PWM0_RELOAD0_IRQHandler
PWM0_CMP1_IRQHandler
PWM0_RELOAD1_IRQHandler
PWM0_CMP2_IRQHandler
PWM0_RELOAD2_IRQHandler
PWM0_CMP3_IRQHandler
PWM0_RELOAD3_IRQHandler
PWM0_CAP_IRQHandler
PWM0_RERR_IRQHandler
PWM0_FAULT_IRQHandler
CMP3_IRQHandler
HSADC1_CCB_IRQHandler
CAN1_ORed_Message_buffer_IRQHandler
CAN1_Bus_Off_IRQHandler
CAN1_Error_IRQHandler
CAN1_Tx_Warning_IRQHandler
CAN1_Rx_Warning_IRQHandler
CAN1_Wake_Up_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler
ENET_Receive_IRQHandler
ENET_Error_IRQHandler
PWM1_CMP0_IRQHandler
PWM1_RELOAD0_IRQHandler
PWM1_CMP1_IRQHandler
PWM1_RELOAD1_IRQHandler
PWM1_CMP2_IRQHandler
PWM1_RELOAD2_IRQHandler
PWM1_CMP3_IRQHandler
PWM1_RELOAD3_IRQHandler
PWM1_CAP_IRQHandler
PWM1_RERR_IRQHandler
PWM1_FAULT_IRQHandler
CAN2_ORed_Message_buffer_IRQHandler
CAN2_Bus_Off_IRQHandler
CAN2_Error_IRQHandler
CAN2_Tx_Warning_IRQHandler
CAN2_Rx_Warning_IRQHandler
CAN2_Wake_Up_IRQHandler
DefaultISR
        B DefaultISR

        END
