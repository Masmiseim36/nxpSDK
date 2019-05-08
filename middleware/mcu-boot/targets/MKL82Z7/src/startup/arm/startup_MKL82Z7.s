; * -------------------------------------------------------------------------
; *  @file:    startup_MKL82Z7.s
; *  @purpose: CMSIS Cortex-M0P Core Device Startup File
; *            MKL82Z7
; *  @version: 1.5
; *  @date:    2015-9-24
; *  @build:   b171205
; * -------------------------------------------------------------------------
; *
; * Copyright (c) 1997-2015 Freescale Semiconductor, Inc.
; * Copyright 2016-2018 NXP
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
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     SVC_Handler                         ;SVCall Handler
                DCD     0                                   ;Reserved
                DCD     0                                   ;Reserved
                DCD     PendSV_Handler                      ;PendSV Handler
                DCD     SysTick_Handler                     ;SysTick Handler

                                                            ;External Interrupts
                DCD     DMA0_DMA4_IRQHandler                ;DMA channel 0, 4 transfer complete
                DCD     DMA1_DMA5_IRQHandler                ;DMA channel 1, 5 transfer complete
                DCD     DMA2_DMA6_IRQHandler                ;DMA channel 2, 6 transfer complete
                DCD     DMA3_DMA7_IRQHandler                ;DMA channel 3, 7 transfer complete
                DCD     DMA_Error_IRQHandler                ;DMA channel 0 - 7 error
                DCD     FLEXIO0_IRQHandler                  ;Flexible IO
                DCD     TPM0_IRQHandler                     ;Timer/PWM module 0
                DCD     TPM1_IRQHandler                     ;Timer/PWM module 1
                DCD     TPM2_IRQHandler                     ;Timer/PWM module 2
                DCD     PIT0_IRQHandler                     ;Periodic Interrupt Timer 0
                DCD     SPI0_IRQHandler                     ;Serial Peripheral Interface 0
                DCD     EMVSIM0_IRQHandler                  ;EMVSIM0 common interrupt
                DCD     LPUART0_IRQHandler                  ;LPUART0 status and error
                DCD     LPUART1_IRQHandler                  ;LPUART1 status and error
                DCD     I2C0_IRQHandler                     ;Inter-Integrated Circuit 0
                DCD     QSPI0_IRQHandler                    ;QuadSPI0 interrupt
                DCD     Reserved32_IRQHandler               ;DryIce tamper detect
                DCD     PORTA_IRQHandler                    ;Pin detect Port A
                DCD     PORTB_IRQHandler                    ;Pin detect Port B
                DCD     PORTC_IRQHandler                    ;Pin detect Port C
                DCD     PORTD_IRQHandler                    ;Pin detect Port D
                DCD     PORTE_IRQHandler                    ;Pin detect Port E
                DCD     LLWU_IRQHandler                     ;Low Leakage Wakeup
                DCD     LTC0_IRQHandler                     ;Low power trusted cryptographic
                DCD     USB0_IRQHandler                     ;USB OTG interrupt
                DCD     ADC0_IRQHandler                     ;Analog-to-Digital Converter 0
                DCD     LPTMR0_IRQHandler                   ;Low-Power Timer 0
                DCD     RTC_Seconds_IRQHandler              ;RTC seconds
                DCD     INTMUX0_0_IRQHandler                ;Selectable peripheral interrupt INTMUX0-0
                DCD     INTMUX0_1_IRQHandler                ;Selectable peripheral interrupt INTMUX0-1
                DCD     INTMUX0_2_IRQHandler                ;Selectable peripheral interrupt INTMUX0-2
                DCD     INTMUX0_3_IRQHandler                ;Selectable peripheral interrupt INTMUX0-3
                DCD     LPTMR1_IRQHandler                   ;Low-Power Timer 1 (INTMUX source IRQ0)
                DCD     Reserved49_IRQHandler               ;Reserved interrupt (INTMUX source IRQ1)
                DCD     Reserved50_IRQHandler               ;Reserved interrupt (INTMUX source IRQ2)
                DCD     Reserved51_IRQHandler               ;Reserved interrupt (INTMUX source IRQ3)
                DCD     SPI1_IRQHandler                     ;Serial Peripheral Interface 1 (INTMUX source IRQ4)
                DCD     LPUART2_IRQHandler                  ;LPUART2 status and error (INTMUX source IRQ5)
                DCD     EMVSIM1_IRQHandler                  ;EMVSIM1 common interrupt (INTMUX source IRQ6)
                DCD     I2C1_IRQHandler                     ;Inter-Integrated Circuit 1 (INTMUX source IRQ7)
                DCD     TSI0_IRQHandler                     ;Touch Sensing Input 0 (INTMUX source IRQ8)
                DCD     PMC_IRQHandler                      ;PMC controller low-voltage detect, low-voltage warning (INTMUX source IRQ9)
                DCD     FTFA_IRQHandler                     ;FTFA command complete/read collision (INTMUX source IRQ10)
                DCD     MCG_IRQHandler                      ;Multipurpose clock generator (INTMUX source IRQ11)
                DCD     WDOG_EWM_IRQHandler                 ;Single interrupt vector for  WDOG and EWM (INTMUX source IRQ12)
                DCD     DAC0_IRQHandler                     ;Digital-to-analog converter 0 (INTMUX source IRQ13)
                DCD     TRNG0_IRQHandler                    ;True randon number generator (INTMUX source IRQ14)
                DCD     Reserved63_IRQHandler               ;Reserved interrupt (INTMUX source IRQ15)
                DCD     CMP0_IRQHandler                     ;Comparator 0 (INTMUX source IRQ16)
                DCD     Reserved65_IRQHandler               ;Reserved interrupt (INTMUX source IRQ17)
                DCD     RTC_Alarm_IRQHandler                ;Real time clock (INTMUX source IRQ18)
                DCD     Reserved67_IRQHandler               ;Reserved interrupt (INTMUX source IRQ19)
                DCD     Reserved68_IRQHandler               ;Reserved interrupt (INTMUX source IRQ20)
                DCD     Reserved69_IRQHandler               ;Reserved interrupt (INTMUX source IRQ21)
                DCD     Reserved70_IRQHandler               ;Reserved interrupt (INTMUX source IRQ22)
                DCD     Reserved71_IRQHandler               ;Reserved interrupt (INTMUX source IRQ23)
                DCD     DMA4_IRQHandler                     ;DMA channel 4 transfer complete (INTMUX source IRQ24)
                DCD     DMA5_IRQHandler                     ;DMA channel 5 transfer complete (INTMUX source IRQ25)
                DCD     DMA6_IRQHandler                     ;DMA channel 6 transfer complete (INTMUX source IRQ26)
                DCD     DMA7_IRQHandler                     ;DMA channel 7 transfer complete (INTMUX source IRQ27)
                DCD     Reserved76_IRQHandler               ;Reserved interrupt (INTMUX source IRQ28)
                DCD     Reserved77_IRQHandler               ;Reserved interrupt (INTMUX source IRQ29)
                DCD     Reserved78_IRQHandler               ;Reserved interrupt (INTMUX source IRQ30)
                DCD     Reserved79_IRQHandler               ;Reserved interrupt (INTMUX source IRQ31)
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

    AREA    BootloaderConfig, DATA, READONLY
    IF        BL_HAS_BOOTLOADER_CONFIG == 1
        ;__bootloaderConfigurationArea ; 0x3c0
        DCD     0x6766636b    ; [00:03] tag - 'kcfg' Tag value used to validate the bootloader configuration data. Must be set to 'kcfg'.
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
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF            
        DCD     0xFFFFFFFF        ; Reserved for user TRIM value
    ELSE      
        ; Fill to align with flash configuration field. 
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF
        DCD     0xFFFFFFFF                 
        DCD     0xFFFFFFFF      ; Reserved for user TRIM value
    ENDIF // BL_HAS_BOOTLOADER_CONFIG

__Vectors_Size  EQU     __Vectors_End - __Vectors

; <h> Flash Configuration
;   <i> 16-byte flash configuration field that stores default protection settings (loaded on reset)
;   <i> and security information that allows the MCU to restrict access to the FTFL module.
;   <h> Backdoor Comparison Key
;     <o0>  Backdoor Comparison Key 0.  <0x0-0xFF:2>
;     <o1>  Backdoor Comparison Key 1.  <0x0-0xFF:2>
;     <o2>  Backdoor Comparison Key 2.  <0x0-0xFF:2>
;     <o3>  Backdoor Comparison Key 3.  <0x0-0xFF:2>
;     <o4>  Backdoor Comparison Key 4.  <0x0-0xFF:2>
;     <o5>  Backdoor Comparison Key 5.  <0x0-0xFF:2>
;     <o6>  Backdoor Comparison Key 6.  <0x0-0xFF:2>
;     <o7>  Backdoor Comparison Key 7.  <0x0-0xFF:2>
BackDoorK0      EQU     0xFF
BackDoorK1      EQU     0xFF
BackDoorK2      EQU     0xFF
BackDoorK3      EQU     0xFF
BackDoorK4      EQU     0xFF
BackDoorK5      EQU     0xFF
BackDoorK6      EQU     0xFF
BackDoorK7      EQU     0xFF
;   </h>
;   <h> Program flash protection bytes (FPROT)
;     <i> Each program flash region can be protected from program and erase operation by setting the associated PROT bit.
;     <i> Each bit protects a 1/32 region of the program flash memory.
;     <h> FPROT0
;       <i> Program Flash Region Protect Register 0
;       <i> 1/32 - 8/32 region
;       <o.0>   FPROT0.0
;       <o.1>   FPROT0.1
;       <o.2>   FPROT0.2
;       <o.3>   FPROT0.3
;       <o.4>   FPROT0.4
;       <o.5>   FPROT0.5
;       <o.6>   FPROT0.6
;       <o.7>   FPROT0.7
nFPROT0         EQU     0x00
FPROT0          EQU     nFPROT0:EOR:0xFF
;     </h>
;     <h> FPROT1
;       <i> Program Flash Region Protect Register 1
;       <i> 9/32 - 16/32 region
;       <o.0>   FPROT1.0
;       <o.1>   FPROT1.1
;       <o.2>   FPROT1.2
;       <o.3>   FPROT1.3
;       <o.4>   FPROT1.4
;       <o.5>   FPROT1.5
;       <o.6>   FPROT1.6
;       <o.7>   FPROT1.7
nFPROT1         EQU     0x00
FPROT1          EQU     nFPROT1:EOR:0xFF
;     </h>
;     <h> FPROT2
;       <i> Program Flash Region Protect Register 2
;       <i> 17/32 - 24/32 region
;       <o.0>   FPROT2.0
;       <o.1>   FPROT2.1
;       <o.2>   FPROT2.2
;       <o.3>   FPROT2.3
;       <o.4>   FPROT2.4
;       <o.5>   FPROT2.5
;       <o.6>   FPROT2.6
;       <o.7>   FPROT2.7
nFPROT2         EQU     0x00
FPROT2          EQU     nFPROT2:EOR:0xFF
;     </h>
;     <h> FPROT3
;       <i> Program Flash Region Protect Register 3
;       <i> 25/32 - 32/32 region
;       <o.0>   FPROT3.0
;       <o.1>   FPROT3.1
;       <o.2>   FPROT3.2
;       <o.3>   FPROT3.3
;       <o.4>   FPROT3.4
;       <o.5>   FPROT3.5
;       <o.6>   FPROT3.6
;       <o.7>   FPROT3.7
nFPROT3         EQU     0x00
FPROT3          EQU     nFPROT3:EOR:0xFF
;     </h>
;   </h>
;   <h> Flash nonvolatile option byte (FOPT)
;     <i> Allows the user to customize the operation of the MCU at boot time.
;     <o.0> LPBOOT
;       <0=> Low-power boot
;       <1=> Normal boot
;     <o.1> BOOTPIN_OPT
;       <0=> Force Boot from ROM if BOOTCFG0 asserted, where BOOTCFG0 is the boot config function which is muxed with NMI pin
;       <1=> Boot source configured by FOPT (BOOTSRC_SEL) bits
;     <o.2> NMI_DIS
;       <0=> NMI interrupts are always blocked
;       <1=> NMI_b pin/interrupts reset default to enabled
;     <o.5> FAST_INIT
;       <0=> Slower initialization
;       <1=> Fast Initialization
;     <o.6..7> BOOTSRC_SEL
;       <0=> Boot from Flash
;       <2=> Boot from ROM, configure QSPI0, and enter boot loader mode.
;       <3=> Boot from ROM and enter boot loader mode.
;         <i> Boot source selection
FOPT          EQU     0x3D
;   </h>
;   <h> Flash security byte (FSEC)
;     <i> WARNING: If SEC field is configured as "MCU security status is secure" and MEEN field is configured as "Mass erase is disabled",
;     <i> MCU's security status cannot be set back to unsecure state since Mass erase via the debugger is blocked !!!
;     <o.0..1> SEC
;       <2=> MCU security status is unsecure
;       <3=> MCU security status is secure
;         <i> Flash Security
;     <o.2..3> FSLACC
;       <2=> Freescale factory access denied
;       <3=> Freescale factory access granted
;         <i> Freescale Failure Analysis Access Code
;     <o.4..5> MEEN
;       <2=> Mass erase is disabled
;       <3=> Mass erase is enabled
;     <o.6..7> KEYEN
;       <2=> Backdoor key access enabled
;       <3=> Backdoor key access disabled
;         <i> Backdoor Key Security Enable
FSEC          EQU     0xFE
;   </h>
; </h>
                IF      :LNOT::DEF:RAM_TARGET
                AREA    FlashConfig, DATA, READONLY
__FlashConfig
                DCB     BackDoorK0, BackDoorK1, BackDoorK2, BackDoorK3
                DCB     BackDoorK4, BackDoorK5, BackDoorK6, BackDoorK7
                DCB     FPROT0    , FPROT1    , FPROT2    , FPROT3
                DCB     FSEC      , FOPT      , 0xFF      , 0xFF
                ENDIF


                AREA    |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                IF      :LNOT::DEF:RAM_TARGET
                REQUIRE FlashConfig
                REQUIRE BootloaderConfig
                ENDIF

                CPSID   I               ; Mask interrupts
                LDR     R0, =0xE000ED08
                LDR     R1, =__Vectors
                STR     R1, [R0]
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
SVC_Handler\
                PROC
                EXPORT  SVC_Handler         [WEAK]
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
DMA0_DMA4_IRQHandler\
                PROC
                EXPORT  DMA0_DMA4_IRQHandler         [WEAK]
                LDR     R0, =DMA0_DMA4_DriverIRQHandler
                BX      R0
                ENDP

DMA1_DMA5_IRQHandler\
                PROC
                EXPORT  DMA1_DMA5_IRQHandler         [WEAK]
                LDR     R0, =DMA1_DMA5_DriverIRQHandler
                BX      R0
                ENDP

DMA2_DMA6_IRQHandler\
                PROC
                EXPORT  DMA2_DMA6_IRQHandler         [WEAK]
                LDR     R0, =DMA2_DMA6_DriverIRQHandler
                BX      R0
                ENDP

DMA3_DMA7_IRQHandler\
                PROC
                EXPORT  DMA3_DMA7_IRQHandler         [WEAK]
                LDR     R0, =DMA3_DMA7_DriverIRQHandler
                BX      R0
                ENDP

DMA_Error_IRQHandler\
                PROC
                EXPORT  DMA_Error_IRQHandler         [WEAK]
                LDR     R0, =DMA_Error_DriverIRQHandler
                BX      R0
                ENDP

FLEXIO0_IRQHandler\
                PROC
                EXPORT  FLEXIO0_IRQHandler         [WEAK]
                LDR     R0, =FLEXIO0_DriverIRQHandler
                BX      R0
                ENDP

SPI0_IRQHandler\
                PROC
                EXPORT  SPI0_IRQHandler         [WEAK]
                LDR     R0, =SPI0_DriverIRQHandler
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

I2C0_IRQHandler\
                PROC
                EXPORT  I2C0_IRQHandler         [WEAK]
                LDR     R0, =I2C0_DriverIRQHandler
                BX      R0
                ENDP

QSPI0_IRQHandler\
                PROC
                EXPORT  QSPI0_IRQHandler         [WEAK]
                LDR     R0, =QSPI0_DriverIRQHandler
                BX      R0
                ENDP

SPI1_IRQHandler\
                PROC
                EXPORT  SPI1_IRQHandler         [WEAK]
                LDR     R0, =SPI1_DriverIRQHandler
                BX      R0
                ENDP

LPUART2_IRQHandler\
                PROC
                EXPORT  LPUART2_IRQHandler         [WEAK]
                LDR     R0, =LPUART2_DriverIRQHandler
                BX      R0
                ENDP

I2C1_IRQHandler\
                PROC
                EXPORT  I2C1_IRQHandler         [WEAK]
                LDR     R0, =I2C1_DriverIRQHandler
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

Default_Handler\
                PROC
                EXPORT  DMA0_DMA4_DriverIRQHandler         [WEAK]
                EXPORT  DMA1_DMA5_DriverIRQHandler         [WEAK]
                EXPORT  DMA2_DMA6_DriverIRQHandler         [WEAK]
                EXPORT  DMA3_DMA7_DriverIRQHandler         [WEAK]
                EXPORT  DMA_Error_DriverIRQHandler         [WEAK]
                EXPORT  FLEXIO0_DriverIRQHandler         [WEAK]
                EXPORT  TPM0_IRQHandler         [WEAK]
                EXPORT  TPM1_IRQHandler         [WEAK]
                EXPORT  TPM2_IRQHandler         [WEAK]
                EXPORT  PIT0_IRQHandler         [WEAK]
                EXPORT  SPI0_DriverIRQHandler         [WEAK]
                EXPORT  EMVSIM0_IRQHandler         [WEAK]
                EXPORT  LPUART0_DriverIRQHandler         [WEAK]
                EXPORT  LPUART1_DriverIRQHandler         [WEAK]
                EXPORT  I2C0_DriverIRQHandler         [WEAK]
                EXPORT  QSPI0_DriverIRQHandler         [WEAK]
                EXPORT  Reserved32_IRQHandler         [WEAK]
                EXPORT  PORTA_IRQHandler         [WEAK]
                EXPORT  PORTB_IRQHandler         [WEAK]
                EXPORT  PORTC_IRQHandler         [WEAK]
                EXPORT  PORTD_IRQHandler         [WEAK]
                EXPORT  PORTE_IRQHandler         [WEAK]
                EXPORT  LLWU_IRQHandler         [WEAK]
                EXPORT  LTC0_IRQHandler         [WEAK]
                EXPORT  USB0_IRQHandler         [WEAK]
                EXPORT  ADC0_IRQHandler         [WEAK]
                EXPORT  LPTMR0_IRQHandler         [WEAK]
                EXPORT  RTC_Seconds_IRQHandler         [WEAK]
                EXPORT  INTMUX0_0_IRQHandler         [WEAK]
                EXPORT  INTMUX0_1_IRQHandler         [WEAK]
                EXPORT  INTMUX0_2_IRQHandler         [WEAK]
                EXPORT  INTMUX0_3_IRQHandler         [WEAK]
                EXPORT  LPTMR1_IRQHandler         [WEAK]
                EXPORT  Reserved49_IRQHandler         [WEAK]
                EXPORT  Reserved50_IRQHandler         [WEAK]
                EXPORT  Reserved51_IRQHandler         [WEAK]
                EXPORT  SPI1_DriverIRQHandler         [WEAK]
                EXPORT  LPUART2_DriverIRQHandler         [WEAK]
                EXPORT  EMVSIM1_IRQHandler         [WEAK]
                EXPORT  I2C1_DriverIRQHandler         [WEAK]
                EXPORT  TSI0_IRQHandler         [WEAK]
                EXPORT  PMC_IRQHandler         [WEAK]
                EXPORT  FTFA_IRQHandler         [WEAK]
                EXPORT  MCG_IRQHandler         [WEAK]
                EXPORT  WDOG_EWM_IRQHandler         [WEAK]
                EXPORT  DAC0_IRQHandler         [WEAK]
                EXPORT  TRNG0_IRQHandler         [WEAK]
                EXPORT  Reserved63_IRQHandler         [WEAK]
                EXPORT  CMP0_IRQHandler         [WEAK]
                EXPORT  Reserved65_IRQHandler         [WEAK]
                EXPORT  RTC_Alarm_IRQHandler         [WEAK]
                EXPORT  Reserved67_IRQHandler         [WEAK]
                EXPORT  Reserved68_IRQHandler         [WEAK]
                EXPORT  Reserved69_IRQHandler         [WEAK]
                EXPORT  Reserved70_IRQHandler         [WEAK]
                EXPORT  Reserved71_IRQHandler         [WEAK]
                EXPORT  DMA4_DriverIRQHandler         [WEAK]
                EXPORT  DMA5_DriverIRQHandler         [WEAK]
                EXPORT  DMA6_DriverIRQHandler         [WEAK]
                EXPORT  DMA7_DriverIRQHandler         [WEAK]
                EXPORT  Reserved76_IRQHandler         [WEAK]
                EXPORT  Reserved77_IRQHandler         [WEAK]
                EXPORT  Reserved78_IRQHandler         [WEAK]
                EXPORT  Reserved79_IRQHandler         [WEAK]
                EXPORT  DefaultISR         [WEAK]
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
                LDR    R0, =DefaultISR
                BX     R0
                ENDP
                  ALIGN


                END
