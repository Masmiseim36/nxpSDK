; -------------------------------------------------------------------------
;  @file:    startup_MKV11Z7.s
;  @purpose: CMSIS Cortex-M0P Core Device Startup File
;            MKV11Z7
;  @version: 1.1
;  @date:    2017-3-22
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
        DCD     DMA0_DMA4_IRQHandler                          ;DMA channel 0 and 4 transfer complete
        DCD     DMA1_DMA5_IRQHandler                          ;DMA channel 1 and 5 transfer complete
        DCD     DMA2_DMA6_IRQHandler                          ;DMA channel 2 and 6 transfer complete
        DCD     DMA3_DMA7_IRQHandler                          ;DMA channel 2 and 7 transfer complete
        DCD     DMA_Error_IRQHandler                          ;DMA error interrupt channels 0-7
        DCD     FTFA_IRQHandler                               ;FTFA command complete and read collision
        DCD     LVD_LVW_IRQHandler                            ;Low-voltage detect, low-voltage warning
        DCD     LLW_IRQHandler                                ;Low Leakage Wakeup
        DCD     I2C0_IRQHandler                               ;I2C0 interrupt
        DCD     Reserved25_IRQHandler                         ;Reserved interrupt
        DCD     SPI0_IRQHandler                               ;SPI0 single interrupt vector for all sources
        DCD     Reserved27_IRQHandler                         ;Reserved interrupt
        DCD     UART0_IRQHandler                              ;UART0 status and error
        DCD     UART1_IRQHandler                              ;UART1 status and error
        DCD     CAN0_IRQHandler                               ;CAN0 interrupt
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     ADC1_IRQHandler                               ;ADC1 interrupt
        DCD     FTM0_IRQHandler                               ;FTM0 single interrupt vector for all sources
        DCD     FTM1_IRQHandler                               ;FTM1 single interrupt vector for all sources
        DCD     FTM2_IRQHandler                               ;FTM2 single interrupt vector for all sources
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt
        DCD     CMP1_IRQHandler                               ;CMP1 interrupt
        DCD     FTM3_IRQHandler                               ;FTM3 single interrupt vector for all sources
        DCD     WDOG_EWM_IRQHandler                           ;Single interrupt vector for  WDOG and EWM
        DCD     FTM4_IRQHandler                               ;FTM4 single interrupt vector for all sources
        DCD     DAC0_IRQHandler                               ;DAC0 interrupt
        DCD     FTM5_IRQHandler                               ;FTM5 single interrupt vector for all sources
        DCD     MCG_IRQHandler                                ;MCG interrupt
        DCD     LPTMR0_IRQHandler                             ;LPTMR0 interrupt
        DCD     PDB0_PDB1_IRQHandler                          ;Single interrupt vector for  PDB0 and PDB1
        DCD     PORTA_IRQHandler                              ;PORTA pin detect
        DCD     PORTB_PORTC_PORTD_PORTE_IRQHandler            ;Single interrupt vector for PORTB, PORTC, PORTD and PORTE pin detect
        DCD     DefaultISR                                    ;48
        DCD     DefaultISR                                    ;49
        DCD     DefaultISR                                    ;50
        DCD     DefaultISR                                    ;51
        DCD     DefaultISR                                    ;52
        DCD     DefaultISR                                    ;53
        DCD     DefaultISR                                    ;54
        DCD     DefaultISR                                    ;55
        DCD     DefaultISR                                    ;56
        DCD     DefaultISR                                    ;57
        DCD     DefaultISR                                    ;58
        DCD     DefaultISR                                    ;59
        DCD     DefaultISR                                    ;60
        DCD     DefaultISR                                    ;61
        DCD     DefaultISR                                    ;62
        DCD     DefaultISR                                    ;63
        DCD     DefaultISR                                    ;64
        DCD     DefaultISR                                    ;65
        DCD     DefaultISR                                    ;66
        DCD     DefaultISR                                    ;67
        DCD     DefaultISR                                    ;68
        DCD     DefaultISR                                    ;69
        DCD     DefaultISR                                    ;70
        DCD     DefaultISR                                    ;71
        DCD     DefaultISR                                    ;72
        DCD     DefaultISR                                    ;73
        DCD     DefaultISR                                    ;74
        DCD     DefaultISR                                    ;75
        DCD     DefaultISR                                    ;76
        DCD     DefaultISR                                    ;77
        DCD     DefaultISR                                    ;78
        DCD     DefaultISR                                    ;79
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
        DCD     DefaultISR                                    ;125
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
        PUBWEAK DMA1_DMA5_IRQHandler
        PUBWEAK DMA2_DMA6_IRQHandler
        PUBWEAK DMA3_DMA7_IRQHandler
        PUBWEAK DMA_Error_IRQHandler
        PUBWEAK FTFA_IRQHandler
        PUBWEAK LVD_LVW_IRQHandler
        PUBWEAK LLW_IRQHandler
        PUBWEAK I2C0_IRQHandler
        PUBWEAK Reserved25_IRQHandler
        PUBWEAK SPI0_IRQHandler
        PUBWEAK Reserved27_IRQHandler
        PUBWEAK UART0_IRQHandler
        PUBWEAK UART1_IRQHandler
        PUBWEAK CAN0_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK FTM0_IRQHandler
        PUBWEAK FTM1_IRQHandler
        PUBWEAK FTM2_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK FTM3_IRQHandler
        PUBWEAK WDOG_EWM_IRQHandler
        PUBWEAK FTM4_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK FTM5_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK PDB0_PDB1_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_PORTC_PORTD_PORTE_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_DMA4_IRQHandler
DMA1_DMA5_IRQHandler
DMA2_DMA6_IRQHandler
DMA3_DMA7_IRQHandler
DMA_Error_IRQHandler
FTFA_IRQHandler
LVD_LVW_IRQHandler
LLW_IRQHandler
I2C0_IRQHandler
Reserved25_IRQHandler
SPI0_IRQHandler
Reserved27_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
CAN0_IRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
FTM3_IRQHandler
WDOG_EWM_IRQHandler
FTM4_IRQHandler
DAC0_IRQHandler
FTM5_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
PDB0_PDB1_IRQHandler
PORTA_IRQHandler
PORTB_PORTC_PORTD_PORTE_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

        END
