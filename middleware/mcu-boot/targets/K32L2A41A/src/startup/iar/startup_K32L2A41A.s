; -------------------------------------------------------------------------
;  @file:    startup_K32L2A41A.s
;  @purpose: CMSIS Cortex-M0P Core Device Startup File
;            K32L2A41A
;  @version: 1.0
;  @date:    2019-10-30
;  @build:   b191118
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
        DCD     DMA0_04_IRQHandler                            ;DMA0 channel 0/4 transfer complete
        DCD     DMA0_15_IRQHandler                            ;DMA0 channel 1/5 transfer complete
        DCD     DMA0_26_IRQHandler                            ;DMA0 channel 2/6 transfer complete
        DCD     DMA0_37_IRQHandler                            ;DMA0 channel 3/7 transfer complete
        DCD     CTI0_DMA0_Error_IRQHandler                    ;CTI0 or DMA0 error
        DCD     FLEXIO0_IRQHandler                            ;FLEXIO0
        DCD     TPM0_IRQHandler                               ;TPM0 single interrupt vector for all sources
        DCD     TPM1_IRQHandler                               ;TPM1 single interrupt vector for all sources
        DCD     TPM2_IRQHandler                               ;TPM2 single interrupt vector for all sources
        DCD     LPIT0_IRQHandler                              ;LPIT0 interrupt
        DCD     LPSPI0_IRQHandler                             ;LPSPI0 single interrupt vector for all sources
        DCD     LPSPI1_IRQHandler                             ;LPSPI1 single interrupt vector for all sources
        DCD     LPUART0_IRQHandler                            ;LPUART0 status and error
        DCD     LPUART1_IRQHandler                            ;LPUART1 status and error
        DCD     LPI2C0_IRQHandler                             ;LPI2C0 interrupt
        DCD     LPI2C1_IRQHandler                             ;LPI2C1 interrupt
        DCD     Reserved32_IRQHandler                         ;Reserved interrupt
        DCD     PORTA_IRQHandler                              ;PORTA Pin detect
        DCD     PORTB_IRQHandler                              ;PORTB Pin detect
        DCD     PORTC_IRQHandler                              ;PORTC Pin detect
        DCD     PORTD_IRQHandler                              ;PORTD Pin detect
        DCD     PORTE_IRQHandler                              ;PORTE Pin detect
        DCD     LLWU0_IRQHandler                              ;Low leakage wakeup 0
        DCD     Reserved39_IRQHandler                         ;Reserved interrupt
        DCD     USB0_IRQHandler                               ;USB0 interrupt
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     LPTMR0_IRQHandler                             ;LPTMR0 interrupt
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds
        DCD     INTMUX0_0_IRQHandler                          ;INTMUX0 channel 0 interrupt
        DCD     INTMUX0_1_IRQHandler                          ;INTMUX0 channel 1 interrupt
        DCD     INTMUX0_2_IRQHandler                          ;INTMUX0 channel 2 interrupt
        DCD     INTMUX0_3_IRQHandler                          ;INTMUX0 channel 3 interrupt
        DCD     LPTMR1_IRQHandler                             ;LPTMR1 interrupt  (INTMUX source IRQ0)
        DCD     Reserved49_IRQHandler                         ;Reserved interrupt
        DCD     Reserved50_IRQHandler                         ;Reserved interrupt
        DCD     Reserved51_IRQHandler                         ;Reserved interrupt
        DCD     LPSPI2_IRQHandler                             ;LPSPI2 single interrupt vector for all sources (INTMUX source IRQ4)
        DCD     LPUART2_IRQHandler                            ;LPUART2 status and error (INTMUX source IRQ5)
        DCD     EMVSIM0_IRQHandler                            ;EMVSIM0 interrupt (INTMUX source IRQ6)
        DCD     LPI2C2_IRQHandler                             ;LPI2C2 interrupt (INTMUX source IRQ7)
        DCD     TSI0_IRQHandler                               ;TSI0 interrupt (INTMUX source IRQ8)
        DCD     PMC_IRQHandler                                ;PMC interrupt (INTMUX source IRQ9)
        DCD     FTFA_IRQHandler                               ;FTFA interrupt (INTMUX source IRQ10)
        DCD     SCG_IRQHandler                                ;SCG interrupt (INTMUX source IRQ11)
        DCD     WDOG0_IRQHandler                              ;WDOG0 interrupt (INTMUX source IRQ12)
        DCD     DAC0_IRQHandler                               ;DAC0 interrupt (INTMUX source IRQ13)
        DCD     TRNG_IRQHandler                               ;TRNG interrupt (INTMUX source IRQ14)
        DCD     RCM_IRQHandler                                ;RCM interrupt (INTMUX source IRQ15)
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt (INTMUX source IRQ16)
        DCD     CMP1_IRQHandler                               ;CMP1 interrupt (INTMUX source IRQ17)
        DCD     RTC_IRQHandler                                ;RTC Alarm interrupt (INTMUX source IRQ18)
        DCD     Reserved67_IRQHandler                         ;Reserved interrupt
        DCD     Reserved68_IRQHandler                         ;Reserved interrupt
        DCD     Reserved69_IRQHandler                         ;Reserved interrupt
        DCD     Reserved70_IRQHandler                         ;Reserved interrupt
        DCD     Reserved71_IRQHandler                         ;Reserved interrupt
        DCD     Reserved72_IRQHandler                         ;Reserved interrupt
        DCD     Reserved73_IRQHandler                         ;Reserved interrupt
        DCD     Reserved74_IRQHandler                         ;Reserved interrupt
        DCD     Reserved75_IRQHandler                         ;Reserved interrupt
        DCD     Reserved76_IRQHandler                         ;Reserved interrupt
        DCD     Reserved77_IRQHandler                         ;Reserved interrupt
        DCD     Reserved78_IRQHandler                         ;Reserved interrupt
        DCD     Reserved79_IRQHandler                         ;Reserved interrupt
        DCD     DefaultISR                                    ;80
        DCD     DefaultISR                                    ;81
        DCD     DefaultISR                                    ;82
        DCD     DefaultISR                                    ;83
        DCD     DefaultISR                                    ;84
        DCD     DefaultISR                                    ;85
        DCD     DefaultISR                                    ;86
        DCD     DefaultISR                                    ;87
        DCD     DefaultISR                                    ;88
        DCD     DefaultISR                                    ;89
        DCD     DefaultISR                                    ;90
        DCD     DefaultISR                                    ;91
        DCD     DefaultISR                                    ;92
        DCD     DefaultISR                                    ;93
        DCD     DefaultISR                                    ;94
        DCD     DefaultISR                                    ;95
        DCD     DefaultISR                                    ;96
        DCD     DefaultISR                                    ;97
        DCD     DefaultISR                                    ;98
        DCD     DefaultISR                                    ;99
        DCD     DefaultISR                                    ;100
        DCD     DefaultISR                                    ;101
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
        LDR     R2, [R1]
        MSR     MSP, R2
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

        PUBWEAK DMA0_04_IRQHandler
        PUBWEAK DMA0_04_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_04_IRQHandler
        LDR     R0, =DMA0_04_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_15_IRQHandler
        PUBWEAK DMA0_15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_15_IRQHandler
        LDR     R0, =DMA0_15_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_26_IRQHandler
        PUBWEAK DMA0_26_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_26_IRQHandler
        LDR     R0, =DMA0_26_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_37_IRQHandler
        PUBWEAK DMA0_37_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_37_IRQHandler
        LDR     R0, =DMA0_37_DriverIRQHandler
        BX      R0

        PUBWEAK CTI0_DMA0_Error_IRQHandler
        PUBWEAK CTI0_DMA0_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTI0_DMA0_Error_IRQHandler
        LDR     R0, =CTI0_DMA0_Error_DriverIRQHandler
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
        PUBWEAK LPIT0_IRQHandler
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

        PUBWEAK Reserved32_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_IRQHandler
        PUBWEAK PORTC_IRQHandler
        PUBWEAK PORTD_IRQHandler
        PUBWEAK PORTE_IRQHandler
        PUBWEAK LLWU0_IRQHandler
        PUBWEAK Reserved39_IRQHandler
        PUBWEAK USB0_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK INTMUX0_0_IRQHandler
        PUBWEAK INTMUX0_0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
INTMUX0_0_IRQHandler
        LDR     R0, =INTMUX0_0_DriverIRQHandler
        BX      R0

        PUBWEAK INTMUX0_1_IRQHandler
        PUBWEAK INTMUX0_1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
INTMUX0_1_IRQHandler
        LDR     R0, =INTMUX0_1_DriverIRQHandler
        BX      R0

        PUBWEAK INTMUX0_2_IRQHandler
        PUBWEAK INTMUX0_2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
INTMUX0_2_IRQHandler
        LDR     R0, =INTMUX0_2_DriverIRQHandler
        BX      R0

        PUBWEAK INTMUX0_3_IRQHandler
        PUBWEAK INTMUX0_3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
INTMUX0_3_IRQHandler
        LDR     R0, =INTMUX0_3_DriverIRQHandler
        BX      R0

        PUBWEAK LPTMR1_IRQHandler
        PUBWEAK Reserved49_IRQHandler
        PUBWEAK Reserved50_IRQHandler
        PUBWEAK Reserved51_IRQHandler
        PUBWEAK LPSPI2_IRQHandler
        PUBWEAK LPSPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI2_IRQHandler
        LDR     R0, =LPSPI2_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_IRQHandler
        LDR     R0, =LPUART2_DriverIRQHandler
        BX      R0

        PUBWEAK EMVSIM0_IRQHandler
        PUBWEAK LPI2C2_IRQHandler
        PUBWEAK LPI2C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C2_IRQHandler
        LDR     R0, =LPI2C2_DriverIRQHandler
        BX      R0

        PUBWEAK TSI0_IRQHandler
        PUBWEAK PMC_IRQHandler
        PUBWEAK FTFA_IRQHandler
        PUBWEAK SCG_IRQHandler
        PUBWEAK WDOG0_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK TRNG_IRQHandler
        PUBWEAK RCM_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK Reserved67_IRQHandler
        PUBWEAK Reserved68_IRQHandler
        PUBWEAK Reserved69_IRQHandler
        PUBWEAK Reserved70_IRQHandler
        PUBWEAK Reserved71_IRQHandler
        PUBWEAK Reserved72_IRQHandler
        PUBWEAK Reserved73_IRQHandler
        PUBWEAK Reserved74_IRQHandler
        PUBWEAK Reserved75_IRQHandler
        PUBWEAK Reserved76_IRQHandler
        PUBWEAK Reserved77_IRQHandler
        PUBWEAK Reserved78_IRQHandler
        PUBWEAK Reserved79_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_04_DriverIRQHandler
DMA0_15_DriverIRQHandler
DMA0_26_DriverIRQHandler
DMA0_37_DriverIRQHandler
CTI0_DMA0_Error_DriverIRQHandler
FLEXIO0_DriverIRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
LPIT0_IRQHandler
LPSPI0_DriverIRQHandler
LPSPI1_DriverIRQHandler
LPUART0_DriverIRQHandler
LPUART1_DriverIRQHandler
LPI2C0_DriverIRQHandler
LPI2C1_DriverIRQHandler
Reserved32_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
LLWU0_IRQHandler
Reserved39_IRQHandler
USB0_IRQHandler
ADC0_IRQHandler
LPTMR0_IRQHandler
RTC_Seconds_IRQHandler
INTMUX0_0_DriverIRQHandler
INTMUX0_1_DriverIRQHandler
INTMUX0_2_DriverIRQHandler
INTMUX0_3_DriverIRQHandler
LPTMR1_IRQHandler
Reserved49_IRQHandler
Reserved50_IRQHandler
Reserved51_IRQHandler
LPSPI2_DriverIRQHandler
LPUART2_DriverIRQHandler
EMVSIM0_IRQHandler
LPI2C2_DriverIRQHandler
TSI0_IRQHandler
PMC_IRQHandler
FTFA_IRQHandler
SCG_IRQHandler
WDOG0_IRQHandler
DAC0_IRQHandler
TRNG_IRQHandler
RCM_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
RTC_IRQHandler
Reserved67_IRQHandler
Reserved68_IRQHandler
Reserved69_IRQHandler
Reserved70_IRQHandler
Reserved71_IRQHandler
Reserved72_IRQHandler
Reserved73_IRQHandler
Reserved74_IRQHandler
Reserved75_IRQHandler
Reserved76_IRQHandler
Reserved77_IRQHandler
Reserved78_IRQHandler
Reserved79_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

        END
