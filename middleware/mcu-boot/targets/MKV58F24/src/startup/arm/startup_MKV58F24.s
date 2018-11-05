; * -------------------------------------------------------------------------
; *  @file:    startup_MKV58F24.s
; *  @purpose: CMSIS Cortex-M7 Core Device Startup File
; *            MKV58F24
; *  @version: 0.3
; *  @date:    2016-2-29
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
                DCD     DMA0_DMA16_IRQHandler               ;DMA channel 0/16 transfer complete
                DCD     DMA1_DMA17_IRQHandler               ;DMA channel 1/17 transfer complete
                DCD     DMA2_DMA18_IRQHandler               ;DMA channel 2/18 transfer complete
                DCD     DMA3_DMA19_IRQHandler               ;DMA channel 3/19 transfer complete
                DCD     DMA4_DMA20_IRQHandler               ;DMA channel 4/20 transfer complete
                DCD     DMA5_DMA21_IRQHandler               ;DMA channel 5/21 transfer complete
                DCD     DMA6_DMA22_IRQHandler               ;DMA channel 6/22 transfer complete
                DCD     DMA7_DMA23_IRQHandler               ;DMA channel 7/23 transfer complete
                DCD     DMA8_DMA24_IRQHandler               ;DMA channel 8/24 transfer complete
                DCD     DMA9_DMA25_IRQHandler               ;DMA channel 9/25 transfer complete
                DCD     DMA10_DMA26_IRQHandler              ;DMA channel 10/26 transfer complete
                DCD     DMA11_DMA27_IRQHandler              ;DMA channel 11/27 transfer complete
                DCD     DMA12_DMA28_IRQHandler              ;DMA channel 12/28 transfer complete
                DCD     DMA13_DMA29_IRQHandler              ;DMA channel 13/29 transfer complete
                DCD     DMA14_DMA30_IRQHandler              ;DMA channel 14/30 transfer complete
                DCD     DMA15_DMA31_IRQHandler              ;DMA channel 15/31 transfer complete
                DCD     DMA_Error_IRQHandler                ;DMA error interrupt channels 0-31
                DCD     MCM_IRQHandler                      ;MCM normal interrupt
                DCD     FTFE_IRQHandler                     ;FTFL command complete
                DCD     Read_Collision_IRQHandler           ;FTFL read collision
                DCD     PMC_IRQHandler                      ;PMC controller low-voltage detect, low-voltage warning
                DCD     LLWU_IRQHandler                     ;Low leakage wakeup
                DCD     WDOG_EWM_IRQHandler                 ;Single interrupt vector for  WDOG and EWM
                DCD     TRNG_IRQHandler                     ;True randon number generator
                DCD     I2C0_IRQHandler                     ;Inter-integrated circuit 0
                DCD     I2C1_IRQHandler                     ;Inter-integrated circuit 1
                DCD     SPI0_IRQHandler                     ;Serial peripheral Interface 0
                DCD     SPI1_IRQHandler                     ;Serial peripheral Interface 1
                DCD     UART5_RX_TX_IRQHandler              ;UART5 receive/transmit interrupt
                DCD     UART5_ERR_IRQHandler                ;UART5 error interrupt
                DCD     UART0_LON_IRQHandler                ;UART0 LON
                DCD     UART0_RX_TX_IRQHandler              ;UART0 receive/transmit interrupt
                DCD     UART0_ERR_IRQHandler                ;UART0 error interrupt
                DCD     UART1_RX_TX_IRQHandler              ;UART1 receive/transmit interrupt
                DCD     UART1_ERR_IRQHandler                ;UART1 error interrupt
                DCD     UART2_RX_TX_IRQHandler              ;UART2 receive/transmit interrupt
                DCD     UART2_ERR_IRQHandler                ;UART2 error interrupt
                DCD     ADC0_IRQHandler                     ;Analog-to-digital converter 0
                DCD     HSADC_ERR_IRQHandler                ;High speed analog-to-digital converter zero cross
                DCD     HSADC0_CCA_IRQHandler               ;High speed analog-to-digital converter 0 submodule A scan complete
                DCD     CMP0_IRQHandler                     ;Comparator 0
                DCD     CMP1_IRQHandler                     ;Comparator 1
                DCD     FTM0_IRQHandler                     ;FlexTimer module 0 fault, overflow and channels interrupt
                DCD     FTM1_IRQHandler                     ;FlexTimer module 1 fault, overflow and channels interrupt
                DCD     UART3_RX_TX_IRQHandler              ;UART3 receive/transmit interrupt
                DCD     UART3_ERR_IRQHandler                ;UART3 error interrupt
                DCD     UART4_RX_TX_IRQHandler              ;UART4 receive/transmit interrupt
                DCD     UART4_ERR_IRQHandler                ;UART4 error interrupt
                DCD     PIT0_IRQHandler                     ;Periodic interrupt timer channel 0
                DCD     PIT1_IRQHandler                     ;Periodic interrupt timer channel 1
                DCD     PIT2_IRQHandler                     ;Periodic interrupt timer channel 2
                DCD     PIT3_IRQHandler                     ;Periodic interrupt timer channel 3
                DCD     PDB0_IRQHandler                     ;Programmable delay block 0
                DCD     FTM2_IRQHandler                     ;FlexTimer module 2 fault, overflow and channels interrupt
                DCD     XBARA_IRQHandler                    ;Inter-peripheral crossbar switch A
                DCD     PDB1_IRQHandler                     ;Programmable delay block 1
                DCD     DAC0_IRQHandler                     ;Digital-to-analog converter 0
                DCD     MCG_IRQHandler                      ;Multipurpose clock generator
                DCD     LPTMR0_IRQHandler                   ;Low power timer interrupt
                DCD     PORTA_IRQHandler                    ;Port A interrupt
                DCD     PORTB_IRQHandler                    ;Port B interrupt
                DCD     PORTC_IRQHandler                    ;Port C interrupt
                DCD     PORTD_IRQHandler                    ;Port D interrupt
                DCD     PORTE_IRQHandler                    ;Port E interrupt
                DCD     SWI_IRQHandler                      ;Software interrupt
                DCD     SPI2_IRQHandler                     ;Serial peripheral Interface 2
                DCD     ENC_COMPARE_IRQHandler              ;ENC Compare
                DCD     ENC_HOME_IRQHandler                 ;ENC Home
                DCD     ENC_WDOG_SAB_IRQHandler             ;ENC Wdog/SAB
                DCD     ENC_INDEX_IRQHandler                ;ENC Index/Roll over/Roll Under
                DCD     CMP2_IRQHandler                     ;Comparator 2
                DCD     FTM3_IRQHandler                     ;FlexTimer module 3 fault, overflow and channels
                DCD     Reserved88_IRQHandler               ;Reserved interrupt
                DCD     HSADC0_CCB_IRQHandler               ;High speed analog-to-digital converter 0 submodule B scan complete
                DCD     HSADC1_CCA_IRQHandler               ;High speed analog-to-digital converter 1 submodule A scan complete
                DCD     CAN0_ORed_Message_buffer_IRQHandler ;Flex controller area network 0 message buffer
                DCD     CAN0_Bus_Off_IRQHandler             ;Flex controller area network 0 bus off
                DCD     CAN0_Error_IRQHandler               ;Flex controller area network 0 error
                DCD     CAN0_Tx_Warning_IRQHandler          ;Flex controller area network 0 transmit
                DCD     CAN0_Rx_Warning_IRQHandler          ;Flex controller area network 0 receive
                DCD     CAN0_Wake_Up_IRQHandler             ;Flex controller area network 0 wake up
                DCD     PWMA_CMP0_IRQHandler                ;Pulse width modulator A channel 0 compare
                DCD     PWMA_RELOAD0_IRQHandler             ;Pulse width modulator A channel 0 reload
                DCD     PWMA_CMP1_IRQHandler                ;Pulse width modulator A channel 1 compare
                DCD     PWMA_RELOAD1_IRQHandler             ;Pulse width modulator A channel 1 reload
                DCD     PWMA_CMP2_IRQHandler                ;Pulse width modulator A channel 2 compare
                DCD     PWMA_RELOAD2_IRQHandler             ;Pulse width modulator A channel 2 reload
                DCD     PWMA_CMP3_IRQHandler                ;Pulse width modulator A channel 3 compare
                DCD     PWMA_RELOAD3_IRQHandler             ;Pulse width modulator A channel 3 reload
                DCD     PWMA_CAP_IRQHandler                 ;Pulse width modulator A capture
                DCD     PWMA_RERR_IRQHandler                ;Pulse width modulator A reload error
                DCD     PWMA_FAULT_IRQHandler               ;Pulse width modulator A fault
                DCD     CMP3_IRQHandler                     ;Comparator 3
                DCD     HSADC1_CCB_IRQHandler               ;High speed analog-to-digital converter 1 submodule B scan complete
                DCD     CAN1_ORed_Message_buffer_IRQHandler ;Flex controller area network 1 message buffer
                DCD     CAN1_Bus_Off_IRQHandler             ;Flex controller area network 1 bus off
                DCD     CAN1_Error_IRQHandler               ;Flex controller area network 1 error
                DCD     CAN1_Tx_Warning_IRQHandler          ;Flex controller area network 1 transmit
                DCD     CAN1_Rx_Warning_IRQHandler          ;Flex controller area network 1 receive
                DCD     CAN1_Wake_Up_IRQHandler             ;Flex controller area network 1 wake up
                DCD     ENET_1588_Timer_IRQHandler          ;Ethernet MAC IEEE 1588 timer
                DCD     ENET_Transmit_IRQHandler            ;Ethernet MAC transmit
                DCD     ENET_Receive_IRQHandler             ;Ethernet MAC receive
                DCD     ENET_Error_IRQHandler               ;Ethernet MAC error and miscelaneous
                DCD     PWMB_CMP0_IRQHandler                ;Pulse width modulator B channel 0 compare
                DCD     PWMB_RELOAD0_IRQHandler             ;Pulse width modulator B channel 0 reload
                DCD     PWMB_CMP1_IRQHandler                ;Pulse width modulator B channel 1 compare
                DCD     PWMB_RELOAD1_IRQHandler             ;Pulse width modulator B channel 1 reload
                DCD     PWMB_CMP2_IRQHandler                ;Pulse width modulator B channel 2 compare
                DCD     PWMB_RELOAD2_IRQHandler             ;Pulse width modulator B channel 2 reload
                DCD     PWMB_CMP3_IRQHandler                ;Pulse width modulator B channel 3 compare
                DCD     PWMB_RELOAD3_IRQHandler             ;Pulse width modulator B channel 3 reload
                DCD     PWMB_CAP_IRQHandler                 ;Pulse width modulator B capture
                DCD     PWMB_RERR_IRQHandler                ;Pulse width modulator B reload error
                DCD     PWMB_FAULT_IRQHandler               ;Pulse width modulator B fault
                DCD     CAN2_ORed_Message_buffer_IRQHandler ;Flex controller area network 2 message buffer
                DCD     CAN2_Bus_Off_IRQHandler             ;Flex controller area network 2 bus off
                DCD     CAN2_Error_IRQHandler               ;Flex controller area network 2 error
                DCD     CAN2_Tx_Warning_IRQHandler          ;Flex controller area network 2 transmit
                DCD     CAN2_Rx_Warning_IRQHandler          ;Flex controller area network 2 receive
                DCD     CAN2_Wake_Up_IRQHandler             ;Flex controller area network 2 wake up
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
__Vectors_End

	IF		BL_HAS_BOOTLOADER_CONFIG == 1
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

__Vectors_Size 	EQU     __Vectors_End - __Vectors

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
;   <h> Data flash protection byte (FDPROT)
;     <i> Each bit protects a 1/8 region of the data flash memory.
;     <i> (Program flash only devices: Reserved)
;       <o.0>   FDPROT.0
;       <o.1>   FDPROT.1
;       <o.2>   FDPROT.2
;       <o.3>   FDPROT.3
;       <o.4>   FDPROT.4
;       <o.5>   FDPROT.5
;       <o.6>   FDPROT.6
;       <o.7>   FDPROT.7
nFDPROT         EQU     0x00
FDPROT          EQU     nFDPROT:EOR:0xFF
;   </h>
;   <h> EEPROM protection byte (FEPROT)
;     <i> FlexNVM devices: Each bit protects a 1/8 region of the EEPROM.
;     <i> (Program flash only devices: Reserved)
;       <o.0>   FEPROT.0
;       <o.1>   FEPROT.1
;       <o.2>   FEPROT.2
;       <o.3>   FEPROT.3
;       <o.4>   FEPROT.4
;       <o.5>   FEPROT.5
;       <o.6>   FEPROT.6
;       <o.7>   FEPROT.7
nFEPROT         EQU     0x00
FEPROT          EQU     nFEPROT:EOR:0xFF
;   </h>
;   <h> Flash nonvolatile option byte (FOPT)
;     <i> Allows the user to customize the operation of the MCU at boot time.
;     <o.0> LPBOOT
;       <0=> Low-power boot
;       <1=> Normal boot
;     <o.2> NMI_DIS
;       <0=> NMI interrupts are always blocked
;       <1=> NMI_b pin/interrupts reset default to enabled
;     <o.5> FAST_INIT
;       <0=> Slower initialization
;       <1=> Fast Initialization
FOPT          EQU     0xFF
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
                DCB     FSEC      , FOPT      , FEPROT    , FDPROT
                ENDIF


                AREA    |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  init_data_bss
                IMPORT  __main

                IF      :LNOT::DEF:RAM_TARGET
                LDR R0, =FlashConfig    ; dummy read, workaround for flashConfig
                ENDIF

                CPSID   I               ; Mask interrupts
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =init_data_bss
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
Default_Handler\
                PROC
                EXPORT  DMA0_DMA16_IRQHandler         [WEAK]
                EXPORT  DMA1_DMA17_IRQHandler         [WEAK]
                EXPORT  DMA2_DMA18_IRQHandler         [WEAK]
                EXPORT  DMA3_DMA19_IRQHandler         [WEAK]
                EXPORT  DMA4_DMA20_IRQHandler         [WEAK]
                EXPORT  DMA5_DMA21_IRQHandler         [WEAK]
                EXPORT  DMA6_DMA22_IRQHandler         [WEAK]
                EXPORT  DMA7_DMA23_IRQHandler         [WEAK]
                EXPORT  DMA8_DMA24_IRQHandler         [WEAK]
                EXPORT  DMA9_DMA25_IRQHandler         [WEAK]
                EXPORT  DMA10_DMA26_IRQHandler         [WEAK]
                EXPORT  DMA11_DMA27_IRQHandler         [WEAK]
                EXPORT  DMA12_DMA28_IRQHandler         [WEAK]
                EXPORT  DMA13_DMA29_IRQHandler         [WEAK]
                EXPORT  DMA14_DMA30_IRQHandler         [WEAK]
                EXPORT  DMA15_DMA31_IRQHandler         [WEAK]
                EXPORT  DMA_Error_IRQHandler         [WEAK]
                EXPORT  MCM_IRQHandler         [WEAK]
                EXPORT  FTFE_IRQHandler         [WEAK]
                EXPORT  Read_Collision_IRQHandler         [WEAK]
                EXPORT  PMC_IRQHandler         [WEAK]
                EXPORT  LLWU_IRQHandler         [WEAK]
                EXPORT  WDOG_EWM_IRQHandler         [WEAK]
                EXPORT  TRNG_IRQHandler         [WEAK]
                EXPORT  I2C0_IRQHandler         [WEAK]
                EXPORT  I2C1_IRQHandler         [WEAK]
                EXPORT  SPI0_IRQHandler         [WEAK]
                EXPORT  SPI1_IRQHandler         [WEAK]
                EXPORT  UART5_RX_TX_IRQHandler         [WEAK]
                EXPORT  UART5_ERR_IRQHandler         [WEAK]
                EXPORT  UART0_LON_IRQHandler         [WEAK]
                EXPORT  UART0_RX_TX_IRQHandler         [WEAK]
                EXPORT  UART0_ERR_IRQHandler         [WEAK]
                EXPORT  UART1_RX_TX_IRQHandler         [WEAK]
                EXPORT  UART1_ERR_IRQHandler         [WEAK]
                EXPORT  UART2_RX_TX_IRQHandler         [WEAK]
                EXPORT  UART2_ERR_IRQHandler         [WEAK]
                EXPORT  ADC0_IRQHandler         [WEAK]
                EXPORT  HSADC_ERR_IRQHandler         [WEAK]
                EXPORT  HSADC0_CCA_IRQHandler         [WEAK]
                EXPORT  CMP0_IRQHandler         [WEAK]
                EXPORT  CMP1_IRQHandler         [WEAK]
                EXPORT  FTM0_IRQHandler         [WEAK]
                EXPORT  FTM1_IRQHandler         [WEAK]
                EXPORT  UART3_RX_TX_IRQHandler         [WEAK]
                EXPORT  UART3_ERR_IRQHandler         [WEAK]
                EXPORT  UART4_RX_TX_IRQHandler         [WEAK]
                EXPORT  UART4_ERR_IRQHandler         [WEAK]
                EXPORT  PIT0_IRQHandler         [WEAK]
                EXPORT  PIT1_IRQHandler         [WEAK]
                EXPORT  PIT2_IRQHandler         [WEAK]
                EXPORT  PIT3_IRQHandler         [WEAK]
                EXPORT  PDB0_IRQHandler         [WEAK]
                EXPORT  FTM2_IRQHandler         [WEAK]
                EXPORT  XBARA_IRQHandler         [WEAK]
                EXPORT  PDB1_IRQHandler         [WEAK]
                EXPORT  DAC0_IRQHandler         [WEAK]
                EXPORT  MCG_IRQHandler         [WEAK]
                EXPORT  LPTMR0_IRQHandler         [WEAK]
                EXPORT  PORTA_IRQHandler         [WEAK]
                EXPORT  PORTB_IRQHandler         [WEAK]
                EXPORT  PORTC_IRQHandler         [WEAK]
                EXPORT  PORTD_IRQHandler         [WEAK]
                EXPORT  PORTE_IRQHandler         [WEAK]
                EXPORT  SWI_IRQHandler         [WEAK]
                EXPORT  SPI2_IRQHandler         [WEAK]
                EXPORT  ENC_COMPARE_IRQHandler         [WEAK]
                EXPORT  ENC_HOME_IRQHandler         [WEAK]
                EXPORT  ENC_WDOG_SAB_IRQHandler         [WEAK]
                EXPORT  ENC_INDEX_IRQHandler         [WEAK]
                EXPORT  CMP2_IRQHandler         [WEAK]
                EXPORT  FTM3_IRQHandler         [WEAK]
                EXPORT  Reserved88_IRQHandler         [WEAK]
                EXPORT  HSADC0_CCB_IRQHandler         [WEAK]
                EXPORT  HSADC1_CCA_IRQHandler         [WEAK]
                EXPORT  CAN0_ORed_Message_buffer_IRQHandler         [WEAK]
                EXPORT  CAN0_Bus_Off_IRQHandler         [WEAK]
                EXPORT  CAN0_Error_IRQHandler         [WEAK]
                EXPORT  CAN0_Tx_Warning_IRQHandler         [WEAK]
                EXPORT  CAN0_Rx_Warning_IRQHandler         [WEAK]
                EXPORT  CAN0_Wake_Up_IRQHandler         [WEAK]
                EXPORT  PWMA_CMP0_IRQHandler         [WEAK]
                EXPORT  PWMA_RELOAD0_IRQHandler         [WEAK]
                EXPORT  PWMA_CMP1_IRQHandler         [WEAK]
                EXPORT  PWMA_RELOAD1_IRQHandler         [WEAK]
                EXPORT  PWMA_CMP2_IRQHandler         [WEAK]
                EXPORT  PWMA_RELOAD2_IRQHandler         [WEAK]
                EXPORT  PWMA_CMP3_IRQHandler         [WEAK]
                EXPORT  PWMA_RELOAD3_IRQHandler         [WEAK]
                EXPORT  PWMA_CAP_IRQHandler         [WEAK]
                EXPORT  PWMA_RERR_IRQHandler         [WEAK]
                EXPORT  PWMA_FAULT_IRQHandler         [WEAK]
                EXPORT  CMP3_IRQHandler         [WEAK]
                EXPORT  HSADC1_CCB_IRQHandler         [WEAK]
                EXPORT  CAN1_ORed_Message_buffer_IRQHandler         [WEAK]
                EXPORT  CAN1_Bus_Off_IRQHandler         [WEAK]
                EXPORT  CAN1_Error_IRQHandler         [WEAK]
                EXPORT  CAN1_Tx_Warning_IRQHandler         [WEAK]
                EXPORT  CAN1_Rx_Warning_IRQHandler         [WEAK]
                EXPORT  CAN1_Wake_Up_IRQHandler         [WEAK]
                EXPORT  ENET_1588_Timer_IRQHandler         [WEAK]
                EXPORT  ENET_Transmit_IRQHandler         [WEAK]
                EXPORT  ENET_Receive_IRQHandler         [WEAK]
                EXPORT  ENET_Error_IRQHandler         [WEAK]
                EXPORT  PWMB_CMP0_IRQHandler         [WEAK]
                EXPORT  PWMB_RELOAD0_IRQHandler         [WEAK]
                EXPORT  PWMB_CMP1_IRQHandler         [WEAK]
                EXPORT  PWMB_RELOAD1_IRQHandler         [WEAK]
                EXPORT  PWMB_CMP2_IRQHandler         [WEAK]
                EXPORT  PWMB_RELOAD2_IRQHandler         [WEAK]
                EXPORT  PWMB_CMP3_IRQHandler         [WEAK]
                EXPORT  PWMB_RELOAD3_IRQHandler         [WEAK]
                EXPORT  PWMB_CAP_IRQHandler         [WEAK]
                EXPORT  PWMB_RERR_IRQHandler         [WEAK]
                EXPORT  PWMB_FAULT_IRQHandler         [WEAK]
                EXPORT  CAN2_ORed_Message_buffer_IRQHandler         [WEAK]
                EXPORT  CAN2_Bus_Off_IRQHandler         [WEAK]
                EXPORT  CAN2_Error_IRQHandler         [WEAK]
                EXPORT  CAN2_Tx_Warning_IRQHandler         [WEAK]
                EXPORT  CAN2_Rx_Warning_IRQHandler         [WEAK]
                EXPORT  CAN2_Wake_Up_IRQHandler         [WEAK]
                EXPORT  DefaultISR         [WEAK]
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
TRNG_IRQHandler
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
PWMA_CMP0_IRQHandler
PWMA_RELOAD0_IRQHandler
PWMA_CMP1_IRQHandler
PWMA_RELOAD1_IRQHandler
PWMA_CMP2_IRQHandler
PWMA_RELOAD2_IRQHandler
PWMA_CMP3_IRQHandler
PWMA_RELOAD3_IRQHandler
PWMA_CAP_IRQHandler
PWMA_RERR_IRQHandler
PWMA_FAULT_IRQHandler
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
PWMB_CMP0_IRQHandler
PWMB_RELOAD0_IRQHandler
PWMB_CMP1_IRQHandler
PWMB_RELOAD1_IRQHandler
PWMB_CMP2_IRQHandler
PWMB_RELOAD2_IRQHandler
PWMB_CMP3_IRQHandler
PWMB_RELOAD3_IRQHandler
PWMB_CAP_IRQHandler
PWMB_RERR_IRQHandler
PWMB_FAULT_IRQHandler
CAN2_ORed_Message_buffer_IRQHandler
CAN2_Bus_Off_IRQHandler
CAN2_Error_IRQHandler
CAN2_Tx_Warning_IRQHandler
CAN2_Rx_Warning_IRQHandler
CAN2_Wake_Up_IRQHandler
DefaultISR
                B      DefaultISR
                ENDP
                  ALIGN


                END
