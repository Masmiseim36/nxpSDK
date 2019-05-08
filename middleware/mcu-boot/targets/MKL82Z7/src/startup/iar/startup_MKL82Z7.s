; -------------------------------------------------------------------------
;  @file:    startup_MKL82Z7.s
;  @purpose: CMSIS Cortex-M0P Core Device Startup File
;            MKL82Z7
;  @version: 1.5
;  @date:    2015-9-24
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
        DCD     DMA0_DMA4_IRQHandler                          ;DMA channel 0, 4 transfer complete
        DCD     DMA1_DMA5_IRQHandler                          ;DMA channel 1, 5 transfer complete
        DCD     DMA2_DMA6_IRQHandler                          ;DMA channel 2, 6 transfer complete
        DCD     DMA3_DMA7_IRQHandler                          ;DMA channel 3, 7 transfer complete
        DCD     DMA_Error_IRQHandler                          ;DMA channel 0 - 7 error
        DCD     FLEXIO0_IRQHandler                            ;Flexible IO
        DCD     TPM0_IRQHandler                               ;Timer/PWM module 0
        DCD     TPM1_IRQHandler                               ;Timer/PWM module 1
        DCD     TPM2_IRQHandler                               ;Timer/PWM module 2
        DCD     PIT0_IRQHandler                               ;Periodic Interrupt Timer 0
        DCD     SPI0_IRQHandler                               ;Serial Peripheral Interface 0
        DCD     EMVSIM0_IRQHandler                            ;EMVSIM0 common interrupt
        DCD     LPUART0_IRQHandler                            ;LPUART0 status and error
        DCD     LPUART1_IRQHandler                            ;LPUART1 status and error
        DCD     I2C0_IRQHandler                               ;Inter-Integrated Circuit 0
        DCD     QSPI0_IRQHandler                              ;QuadSPI0 interrupt
        DCD     Reserved32_IRQHandler                         ;DryIce tamper detect
        DCD     PORTA_IRQHandler                              ;Pin detect Port A
        DCD     PORTB_IRQHandler                              ;Pin detect Port B
        DCD     PORTC_IRQHandler                              ;Pin detect Port C
        DCD     PORTD_IRQHandler                              ;Pin detect Port D
        DCD     PORTE_IRQHandler                              ;Pin detect Port E
        DCD     LLWU_IRQHandler                               ;Low Leakage Wakeup
        DCD     LTC0_IRQHandler                               ;Low power trusted cryptographic
        DCD     USB0_IRQHandler                               ;USB OTG interrupt
        DCD     ADC0_IRQHandler                               ;Analog-to-Digital Converter 0
        DCD     LPTMR0_IRQHandler                             ;Low-Power Timer 0
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds
        DCD     INTMUX0_0_IRQHandler                          ;Selectable peripheral interrupt INTMUX0-0
        DCD     INTMUX0_1_IRQHandler                          ;Selectable peripheral interrupt INTMUX0-1
        DCD     INTMUX0_2_IRQHandler                          ;Selectable peripheral interrupt INTMUX0-2
        DCD     INTMUX0_3_IRQHandler                          ;Selectable peripheral interrupt INTMUX0-3
        DCD     LPTMR1_IRQHandler                             ;Low-Power Timer 1 (INTMUX source IRQ0)
        DCD     Reserved49_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ1)
        DCD     Reserved50_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ2)
        DCD     Reserved51_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ3)
        DCD     SPI1_IRQHandler                               ;Serial Peripheral Interface 1 (INTMUX source IRQ4)
        DCD     LPUART2_IRQHandler                            ;LPUART2 status and error (INTMUX source IRQ5)
        DCD     EMVSIM1_IRQHandler                            ;EMVSIM1 common interrupt (INTMUX source IRQ6)
        DCD     I2C1_IRQHandler                               ;Inter-Integrated Circuit 1 (INTMUX source IRQ7)
        DCD     TSI0_IRQHandler                               ;Touch Sensing Input 0 (INTMUX source IRQ8)
        DCD     PMC_IRQHandler                                ;PMC controller low-voltage detect, low-voltage warning (INTMUX source IRQ9)
        DCD     FTFA_IRQHandler                               ;FTFA command complete/read collision (INTMUX source IRQ10)
        DCD     MCG_IRQHandler                                ;Multipurpose clock generator (INTMUX source IRQ11)
        DCD     WDOG_EWM_IRQHandler                           ;Single interrupt vector for  WDOG and EWM (INTMUX source IRQ12)
        DCD     DAC0_IRQHandler                               ;Digital-to-analog converter 0 (INTMUX source IRQ13)
        DCD     TRNG0_IRQHandler                              ;True randon number generator (INTMUX source IRQ14)
        DCD     Reserved63_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ15)
        DCD     CMP0_IRQHandler                               ;Comparator 0 (INTMUX source IRQ16)
        DCD     Reserved65_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ17)
        DCD     RTC_Alarm_IRQHandler                          ;Real time clock (INTMUX source IRQ18)
        DCD     Reserved67_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ19)
        DCD     Reserved68_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ20)
        DCD     Reserved69_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ21)
        DCD     Reserved70_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ22)
        DCD     Reserved71_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ23)
        DCD     DMA4_IRQHandler                               ;DMA channel 4 transfer complete (INTMUX source IRQ24)
        DCD     DMA5_IRQHandler                               ;DMA channel 5 transfer complete (INTMUX source IRQ25)
        DCD     DMA6_IRQHandler                               ;DMA channel 6 transfer complete (INTMUX source IRQ26)
        DCD     DMA7_IRQHandler                               ;DMA channel 7 transfer complete (INTMUX source IRQ27)
        DCD     Reserved76_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ28)
        DCD     Reserved77_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ29)
        DCD     Reserved78_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ30)
        DCD     Reserved79_IRQHandler                         ;Reserved interrupt (INTMUX source IRQ31)
        DCD     0xFFFFFFFF                                    ;80
        DCD     0xFFFFFFFF                                    ;81
        DCD     0xFFFFFFFF                                    ;82
        DCD     0xFFFFFFFF                                    ;83
        DCD     0xFFFFFFFF                                    ;84
        DCD     0xFFFFFFFF                                    ;85
        DCD     0xFFFFFFFF                                    ;86
        DCD     0xFFFFFFFF                                    ;87
        DCD     0xFFFFFFFF                                    ;88
        DCD     0xFFFFFFFF                                    ;89
        DCD     0xFFFFFFFF                                    ;90
        DCD     0xFFFFFFFF                                    ;91
        DCD     0xFFFFFFFF                                    ;92
        DCD     0xFFFFFFFF                                    ;93
        DCD     0xFFFFFFFF                                    ;94
        DCD     0xFFFFFFFF                                    ;95
        DCD     0xFFFFFFFF                                    ;96
        DCD     0xFFFFFFFF                                    ;97
        DCD     0xFFFFFFFF                                    ;98
        DCD     0xFFFFFFFF                                    ;99    
        DCD     0xFFFFFFFF                                    ;100
        DCD     0xFFFFFFFF                                    ;101
        DCD     0xFFFFFFFF                                    ;102
        DCD     0xFFFFFFFF                                    ;103
        DCD     0xFFFFFFFF                                    ;104
        DCD     0xFFFFFFFF                                    ;105
        DCD     0xFFFFFFFF                                    ;106
        DCD     0xFFFFFFFF                                    ;107
        DCD     0xFFFFFFFF                                    ;108
        DCD     0xFFFFFFFF                                    ;109
        DCD     0xFFFFFFFF                                    ;110
        DCD     0xFFFFFFFF                                    ;111
        DCD     0xFFFFFFFF                                    ;112
        DCD     0xFFFFFFFF                                    ;113
        DCD     0xFFFFFFFF                                    ;114
        DCD     0xFFFFFFFF                                    ;115
        DCD     0xFFFFFFFF                                    ;116
        DCD     0xFFFFFFFF                                    ;117
        DCD     0xFFFFFFFF                                    ;118
        DCD     0xFFFFFFFF                                    ;119   
        DCD     0xFFFFFFFF                                    ;120
        DCD     0xFFFFFFFF                                    ;121
        DCD     0xFFFFFFFF                                    ;122         
        DCD     0xFFFFFFFF                                    ;123
        DCD     0xFFFFFFFF                                    ;124
        DCD     0xFFFFFFFF                                    ;125
        DCD     0xFFFFFFFF                                    ;126
        DCD     0xFFFFFFFF                                    ;127
        DCD     0xFFFFFFFF                                    ;128
        DCD     0xFFFFFFFF                                    ;129
        DCD     0xFFFFFFFF                                    ;130
        DCD     0xFFFFFFFF                                    ;131
        DCD     0xFFFFFFFF                                    ;132
        DCD     0xFFFFFFFF                                    ;133
        DCD     0xFFFFFFFF                                    ;134
        DCD     0xFFFFFFFF                                    ;135
        DCD     0xFFFFFFFF                                    ;136
        DCD     0xFFFFFFFF                                    ;137
        DCD     0xFFFFFFFF                                    ;138
        DCD     0xFFFFFFFF                                    ;139
        DCD     0xFFFFFFFF                                    ;140
        DCD     0xFFFFFFFF                                    ;141
        DCD     0xFFFFFFFF                                    ;142
        DCD     0xFFFFFFFF                                    ;143
        DCD     0xFFFFFFFF                                    ;144
        DCD     0xFFFFFFFF                                    ;145
        DCD     0xFFFFFFFF                                    ;146
        DCD     0xFFFFFFFF                                    ;147
        DCD     0xFFFFFFFF                                    ;148
        DCD     0xFFFFFFFF                                    ;149
        DCD     0xFFFFFFFF                                    ;150
        DCD     0xFFFFFFFF                                    ;151
        DCD     0xFFFFFFFF                                    ;152
        DCD     0xFFFFFFFF                                    ;153
        DCD     0xFFFFFFFF                                    ;154
        DCD     0xFFFFFFFF                                    ;155
        DCD     0xFFFFFFFF                                    ;156
        DCD     0xFFFFFFFF                                    ;157
        DCD     0xFFFFFFFF                                    ;158
        DCD     0xFFFFFFFF                                    ;159
        DCD     0xFFFFFFFF                                    ;160
        DCD     0xFFFFFFFF                                    ;161
        DCD     0xFFFFFFFF                                    ;162
        DCD     0xFFFFFFFF                                    ;163
        DCD     0xFFFFFFFF                                    ;164
        DCD     0xFFFFFFFF                                    ;165
        DCD     0xFFFFFFFF                                    ;166
        DCD     0xFFFFFFFF                                    ;167
        DCD     0xFFFFFFFF                                    ;168
        DCD     0xFFFFFFFF                                    ;169
        DCD     0xFFFFFFFF                                    ;170
        DCD     0xFFFFFFFF                                    ;171
        DCD     0xFFFFFFFF                                    ;172
        DCD     0xFFFFFFFF                                    ;173
        DCD     0xFFFFFFFF                                    ;174
        DCD     0xFFFFFFFF                                    ;175
        DCD     0xFFFFFFFF                                    ;176
        DCD     0xFFFFFFFF                                    ;177
        DCD     0xFFFFFFFF                                    ;178
        DCD     0xFFFFFFFF                                    ;179
        DCD     0xFFFFFFFF                                    ;180
        DCD     0xFFFFFFFF                                    ;181
        DCD     0xFFFFFFFF                                    ;182
        DCD     0xFFFFFFFF                                    ;183
        DCD     0xFFFFFFFF                                    ;184
        DCD     0xFFFFFFFF                                    ;185
        DCD     0xFFFFFFFF                                    ;186
        DCD     0xFFFFFFFF                                    ;187
        DCD     0xFFFFFFFF                                    ;188
        DCD     0xFFFFFFFF                                    ;189
        DCD     0xFFFFFFFF                                    ;190
        DCD     0xFFFFFFFF                                    ;191
        DCD     0xFFFFFFFF                                    ;192
        DCD     0xFFFFFFFF                                    ;193
        DCD     0xFFFFFFFF                                    ;194
        DCD     0xFFFFFFFF                                    ;195
        DCD     0xFFFFFFFF                                    ;196
        DCD     0xFFFFFFFF                                    ;197
        DCD     0xFFFFFFFF                                    ;198
        DCD     0xFFFFFFFF                                    ;199
        DCD     0xFFFFFFFF                                    ;200
        DCD     0xFFFFFFFF                                    ;201
        DCD     0xFFFFFFFF                                    ;202
        DCD     0xFFFFFFFF                                    ;203
        DCD     0xFFFFFFFF                                    ;204
        DCD     0xFFFFFFFF                                    ;205
        DCD     0xFFFFFFFF                                    ;206
        DCD     0xFFFFFFFF                                    ;207
        DCD     0xFFFFFFFF                                    ;208
        DCD     0xFFFFFFFF                                    ;209
        DCD     0xFFFFFFFF                                    ;210
        DCD     0xFFFFFFFF                                    ;211
        DCD     0xFFFFFFFF                                    ;212
        DCD     0xFFFFFFFF                                    ;213
        DCD     0xFFFFFFFF                                    ;214
        DCD     0xFFFFFFFF                                    ;215
        DCD     0xFFFFFFFF                                    ;216
        DCD     0xFFFFFFFF                                    ;217
        DCD     0xFFFFFFFF                                    ;218
        DCD     0xFFFFFFFF                                    ;219
        DCD     0xFFFFFFFF                                    ;220
        DCD     0xFFFFFFFF                                    ;221
        DCD     0xFFFFFFFF                                    ;222
        DCD     0xFFFFFFFF                                    ;223
        DCD     0xFFFFFFFF                                    ;224
        DCD     0xFFFFFFFF                                    ;225
        DCD     0xFFFFFFFF                                    ;226
        DCD     0xFFFFFFFF                                    ;227
        DCD     0xFFFFFFFF                                    ;228
        DCD     0xFFFFFFFF                                    ;229
        DCD     0xFFFFFFFF                                    ;230
        DCD     0xFFFFFFFF                                    ;231
        DCD     0xFFFFFFFF                                    ;232
        DCD     0xFFFFFFFF                                    ;233
        DCD     0xFFFFFFFF                                    ;234
        DCD     0xFFFFFFFF                                    ;235
        DCD     0xFFFFFFFF                                    ;236
        DCD     0xFFFFFFFF                                    ;237
        DCD     0xFFFFFFFF                                    ;238
        DCD     0xFFFFFFFF                                    ;239
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
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFF3DFE
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

        PUBWEAK DMA0_DMA4_IRQHandler
        PUBWEAK DMA0_DMA4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DMA4_IRQHandler
        LDR     R0, =DMA0_DMA4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_DMA5_IRQHandler
        PUBWEAK DMA1_DMA5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_DMA5_IRQHandler
        LDR     R0, =DMA1_DMA5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA2_DMA6_IRQHandler
        PUBWEAK DMA2_DMA6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA2_DMA6_IRQHandler
        LDR     R0, =DMA2_DMA6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_DMA7_IRQHandler
        PUBWEAK DMA3_DMA7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_DMA7_IRQHandler
        LDR     R0, =DMA3_DMA7_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_Error_IRQHandler
        PUBWEAK DMA_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_Error_IRQHandler
        LDR     R0, =DMA_Error_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXIO0_IRQHandler
        PUBWEAK FLEXIO0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO0_IRQHandler
        LDR     R0, =FLEXIO0_DriverIRQHandler
        BX      R0

        PUBWEAK TPM0_IRQHandler
        PUBWEAK TPM1_IRQHandler
        PUBWEAK TPM2_IRQHandler
        PUBWEAK PIT0_IRQHandler
        PUBWEAK SPI0_IRQHandler
        PUBWEAK SPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI0_IRQHandler
        LDR     R0, =SPI0_DriverIRQHandler
        BX      R0

        PUBWEAK EMVSIM0_IRQHandler
        PUBWEAK LPUART0_IRQHandler
        PUBWEAK LPUART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_IRQHandler
        LDR     R0, =LPUART0_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART1_IRQHandler
        PUBWEAK LPUART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_IRQHandler
        LDR     R0, =LPUART1_DriverIRQHandler
        BX      R0

        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C0_IRQHandler
        LDR     R0, =I2C0_DriverIRQHandler
        BX      R0

        PUBWEAK QSPI0_IRQHandler
        PUBWEAK QSPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
QSPI0_IRQHandler
        LDR     R0, =QSPI0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved32_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_IRQHandler
        PUBWEAK PORTC_IRQHandler
        PUBWEAK PORTD_IRQHandler
        PUBWEAK PORTE_IRQHandler
        PUBWEAK LLWU_IRQHandler
        PUBWEAK LTC0_IRQHandler
        PUBWEAK USB0_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK INTMUX0_0_IRQHandler
        PUBWEAK INTMUX0_1_IRQHandler
        PUBWEAK INTMUX0_2_IRQHandler
        PUBWEAK INTMUX0_3_IRQHandler
        PUBWEAK LPTMR1_IRQHandler
        PUBWEAK Reserved49_IRQHandler
        PUBWEAK Reserved50_IRQHandler
        PUBWEAK Reserved51_IRQHandler
        PUBWEAK SPI1_IRQHandler
        PUBWEAK SPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPI1_IRQHandler
        LDR     R0, =SPI1_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_IRQHandler
        LDR     R0, =LPUART2_DriverIRQHandler
        BX      R0

        PUBWEAK EMVSIM1_IRQHandler
        PUBWEAK I2C1_IRQHandler
        PUBWEAK I2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2C1_IRQHandler
        LDR     R0, =I2C1_DriverIRQHandler
        BX      R0

        PUBWEAK TSI0_IRQHandler
        PUBWEAK PMC_IRQHandler
        PUBWEAK FTFA_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK TRNG0_IRQHandler
        PUBWEAK Reserved63_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK Reserved65_IRQHandler
        PUBWEAK RTC_Alarm_IRQHandler
        PUBWEAK Reserved67_IRQHandler
        PUBWEAK Reserved68_IRQHandler
        PUBWEAK Reserved69_IRQHandler
        PUBWEAK Reserved70_IRQHandler
        PUBWEAK Reserved71_IRQHandler
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

        PUBWEAK Reserved76_IRQHandler
        PUBWEAK Reserved77_IRQHandler
        PUBWEAK Reserved78_IRQHandler
        PUBWEAK Reserved79_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DMA4_DriverIRQHandler
DMA1_DMA5_DriverIRQHandler
DMA2_DMA6_DriverIRQHandler
DMA3_DMA7_DriverIRQHandler
DMA_Error_DriverIRQHandler
FLEXIO0_DriverIRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
PIT0_IRQHandler
SPI0_DriverIRQHandler
EMVSIM0_IRQHandler
LPUART0_DriverIRQHandler
LPUART1_DriverIRQHandler
I2C0_DriverIRQHandler
QSPI0_DriverIRQHandler
Reserved32_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
LLWU_IRQHandler
LTC0_IRQHandler
USB0_IRQHandler
ADC0_IRQHandler
LPTMR0_IRQHandler
RTC_Seconds_IRQHandler
INTMUX0_0_IRQHandler
INTMUX0_1_IRQHandler
INTMUX0_2_IRQHandler
INTMUX0_3_IRQHandler
LPTMR1_IRQHandler
Reserved49_IRQHandler
Reserved50_IRQHandler
Reserved51_IRQHandler
SPI1_DriverIRQHandler
LPUART2_DriverIRQHandler
EMVSIM1_IRQHandler
I2C1_DriverIRQHandler
TSI0_IRQHandler
PMC_IRQHandler
FTFA_IRQHandler
MCG_IRQHandler
WDOG_EWM_IRQHandler
DAC0_IRQHandler
TRNG0_IRQHandler
Reserved63_IRQHandler
CMP0_IRQHandler
Reserved65_IRQHandler
RTC_Alarm_IRQHandler
Reserved67_IRQHandler
Reserved68_IRQHandler
Reserved69_IRQHandler
Reserved70_IRQHandler
Reserved71_IRQHandler
DMA4_DriverIRQHandler
DMA5_DriverIRQHandler
DMA6_DriverIRQHandler
DMA7_DriverIRQHandler
Reserved76_IRQHandler
Reserved77_IRQHandler
Reserved78_IRQHandler
Reserved79_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

        END
