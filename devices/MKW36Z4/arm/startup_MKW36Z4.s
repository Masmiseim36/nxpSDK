; * -------------------------------------------------------------------------
; *  @file:    startup_MKW36Z4.s
; *  @purpose: CMSIS Cortex-M0P Core Device Startup File
; *            MKW36Z4
; *  @version: 1.4
; *  @date:    2019-1-25
; *  @build:   b190529
; * -------------------------------------------------------------------------
; *
; * Copyright 1997-2016 Freescale Semiconductor, Inc.
; * Copyright 2016-2019 NXP
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
                DCD     DMA0_IRQHandler                     ;DMA channel 0 transfer complete
                DCD     DMA1_IRQHandler                     ;DMA channel 1 transfer complete
                DCD     DMA2_IRQHandler                     ;DMA channel 2 transfer complete
                DCD     DMA3_IRQHandler                     ;DMA channel 3 transfer complete
                DCD     Reserved20_IRQHandler               ;Reserved interrupt
                DCD     FTFE_IRQHandler                     ;Command complete and read collision, Flash ECC Errors
                DCD     LVD_LVW_DCDC_IRQHandler             ;Low-voltage detect, low-voltage warning, DCDC
                DCD     LLWU_IRQHandler                     ;Low leakage wakeup Unit
                DCD     I2C0_IRQHandler                     ;I2C0 interrupt
                DCD     I2C1_IRQHandler                     ;I2C1 interrupt
                DCD     SPI0_IRQHandler                     ;SPI0 single interrupt vector for all sources
                DCD     CAN0_IRQHandler                     ;FlexCAN0 ORed Error, Bus off, Transmit/Receive Warning, Wake up
                DCD     LPUART0_LPUART1_IRQHandler          ;LPUART0 or LPUART1 status and error
                DCD     TRNG0_IRQHandler                    ;TRNG0 interrupt
                DCD     CMT_IRQHandler                      ;CMT interrupt
                DCD     ADC0_IRQHandler                     ;ADC0 interrupt
                DCD     CMP0_IRQHandler                     ;CMP0 interrupt
                DCD     TPM0_IRQHandler                     ;TPM0 single interrupt vector for all sources
                DCD     TPM1_IRQHandler                     ;TPM1 single interrupt vector for all sources
                DCD     TPM2_IRQHandler                     ;TPM2 single interrupt vector for all sources
                DCD     RTC_IRQHandler                      ;RTC alarm
                DCD     RTC_Seconds_IRQHandler              ;RTC seconds
                DCD     PIT_IRQHandler                      ;PIT interrupt
                DCD     LTC0_IRQHandler                     ;LTC0 interrupt
                DCD     Radio_0_IRQHandler                  ;BTLE, ZIGBEE, ANT, GENFSK interrupt 0
                DCD     CAN0_MB_IRQHandler                  ;FlexCAN0 ORed Message Buffer (MB0-31) interrupt
                DCD     Radio_1_IRQHandler                  ;BTLE, ZIGBEE, ANT, GENFSK interrupt 1
                DCD     MCG_IRQHandler                      ;MCG interrupt
                DCD     LPTMR0_IRQHandler                   ;LPTMR0 interrupt
                DCD     SPI1_IRQHandler                     ;SPI1 single interrupt vector for all sources
                DCD     PORTA_IRQHandler                    ;PORTA Pin detect
                DCD     PORTB_PORTC_IRQHandler              ;PORTB and PORTC Pin detect
__Vectors_End

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
;     <o.0> LPBOOT0
;       <0=> Core and system clock divider (OUTDIV1) is 0x7 (divide by 8) when LPBOOT1=0 or 0x1 (divide by 2) when LPBOOT1=1.
;       <1=> Core and system clock divider (OUTDIV1) is 0x3 (divide by 4) when LPBOOT1=0 or 0x0 (divide by 1) when LPBOOT1=1.
;     <o.2> NMI_DIS
;       <0=> NMI interrupts are always blocked
;       <1=> NMI_b pin/interrupts reset default to enabled
;     <o.3> RESET_PIN_CFG
;       <0=> RESET pin is disabled following a POR and cannot be enabled as reset function
;       <1=> RESET_b pin is dedicated
;     <o.4> LPBOOT1
;       <0=> Core and system clock divider (OUTDIV1) is 0x7 (divide by 8) when LPBOOT0=0 or 0x3 (divide by 4) when LPBOOT0=1.
;       <1=> Core and system clock divider (OUTDIV1) is 0x1 (divide by 2) when LPBOOT0=0 or 0x0 (divide by 1) when LPBOOT0=1.
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
                IMPORT  __main
                IF      :DEF:SUPPORT_WARMBOOT
                IMPORT  WARMBOOT_SEQUENCE
                IMPORT  |Image$$WARMBOOT_STACK$$ZI$$Limit|
                IMPORT  |Image$$RW_m_warmboot_config$$ZI$$Base|
                ENDIF

                IF      :LNOT::DEF:RAM_TARGET
                REQUIRE FlashConfig
                ENDIF

                CPSID   I               ; Mask interrupts
                LDR     R0, =0xE000ED08
                LDR     R1, =__Vectors
                STR     R1, [R0]
                LDR     R2, [R1]
                IF      :DEF:SUPPORT_WARMBOOT
;if vlls is not the only wakeup source, jump to NORMAL_BOOT
                LDR     R0, =0x4007F000
                LDR     R0, [R0]
                CMP     R0, #1
                BNE.N   NORMAL_BOOT
;if SMC_PMCTRL_STOPM != 4(VLLSx), jump to NORMAL_BOOT
                MOVS    R1, #7
                LDR     R0, =0x4007E001
                LDRB    R0, [R0]
                ANDS    R0, R0, R1
                CMP     R0, #4
                BNE.N   NORMAL_BOOT
;if SMC_STOPCTRL_LLSM != 2(VLLS2) or 3(VLLS3), jump to NORMAL_BOOT
                LDR     R0, =0x4007E002
                LDRB    R0, [R0]
                ANDS    R0, R0, R1
                LSRS    R0, R0, #1
                CMP     R0, #1
                BNE.N   NORMAL_BOOT
;if WARMBOOT_SEQUENCE do not match what WARMBOOT_CONFIG stored, jump to NORMAL_BOOT
                LDR     R1, =|Image$$RW_m_warmboot_config$$ZI$$Base|
                LDR     R3, [R1]
                LDR     R4, =WARMBOOT_SEQUENCE
                LDR     R4, [R4]
                CMP     R3, R4
                BNE.N   NORMAL_BOOT
;if useWarmbootSp = 1, set MSP to top of WARMBOOT_STACK
                LDR     R3, [R1,#4]
                CMP     R3, #0
                BEQ.N   WARM_BOOT
;load warm boot stack pointer
                LDR     R2, =|Image$$WARMBOOT_STACK$$ZI$$Limit|
WARM_BOOT
                MSR     MSP, R2
                LDR     R0, =SystemInit
                BLX     R0
                CPSIE   I               ; Unmask interrupts
;jump to warmmain
                LDR     R0, =warmmain
                BX      R0
                B       .
NORMAL_BOOT
                ENDIF
                MSR     MSP, R2
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
DMA0_IRQHandler\
                PROC
                EXPORT  DMA0_IRQHandler         [WEAK]
                LDR     R0, =DMA0_DriverIRQHandler
                BX      R0
                ENDP

DMA1_IRQHandler\
                PROC
                EXPORT  DMA1_IRQHandler         [WEAK]
                LDR     R0, =DMA1_DriverIRQHandler
                BX      R0
                ENDP

DMA2_IRQHandler\
                PROC
                EXPORT  DMA2_IRQHandler         [WEAK]
                LDR     R0, =DMA2_DriverIRQHandler
                BX      R0
                ENDP

DMA3_IRQHandler\
                PROC
                EXPORT  DMA3_IRQHandler         [WEAK]
                LDR     R0, =DMA3_DriverIRQHandler
                BX      R0
                ENDP

I2C0_IRQHandler\
                PROC
                EXPORT  I2C0_IRQHandler         [WEAK]
                LDR     R0, =I2C0_DriverIRQHandler
                BX      R0
                ENDP

I2C1_IRQHandler\
                PROC
                EXPORT  I2C1_IRQHandler         [WEAK]
                LDR     R0, =I2C1_DriverIRQHandler
                BX      R0
                ENDP

SPI0_IRQHandler\
                PROC
                EXPORT  SPI0_IRQHandler         [WEAK]
                LDR     R0, =SPI0_DriverIRQHandler
                BX      R0
                ENDP

CAN0_IRQHandler\
                PROC
                EXPORT  CAN0_IRQHandler         [WEAK]
                LDR     R0, =CAN0_DriverIRQHandler
                BX      R0
                ENDP

LPUART0_LPUART1_IRQHandler\
                PROC
                EXPORT  LPUART0_LPUART1_IRQHandler         [WEAK]
                LDR     R0, =LPUART0_LPUART1_DriverIRQHandler
                BX      R0
                ENDP

CAN0_MB_IRQHandler\
                PROC
                EXPORT  CAN0_MB_IRQHandler         [WEAK]
                LDR     R0, =CAN0_DriverIRQHandler
                BX      R0
                ENDP

SPI1_IRQHandler\
                PROC
                EXPORT  SPI1_IRQHandler         [WEAK]
                LDR     R0, =SPI1_DriverIRQHandler
                BX      R0
                ENDP

Default_Handler\
                PROC
                EXPORT  DMA0_DriverIRQHandler         [WEAK]
                EXPORT  DMA1_DriverIRQHandler         [WEAK]
                EXPORT  DMA2_DriverIRQHandler         [WEAK]
                EXPORT  DMA3_DriverIRQHandler         [WEAK]
                EXPORT  Reserved20_IRQHandler         [WEAK]
                EXPORT  FTFE_IRQHandler         [WEAK]
                EXPORT  LVD_LVW_DCDC_IRQHandler         [WEAK]
                EXPORT  LLWU_IRQHandler         [WEAK]
                EXPORT  I2C0_DriverIRQHandler         [WEAK]
                EXPORT  I2C1_DriverIRQHandler         [WEAK]
                EXPORT  SPI0_DriverIRQHandler         [WEAK]
                EXPORT  CAN0_DriverIRQHandler         [WEAK]
                EXPORT  LPUART0_LPUART1_DriverIRQHandler         [WEAK]
                EXPORT  TRNG0_IRQHandler         [WEAK]
                EXPORT  CMT_IRQHandler         [WEAK]
                EXPORT  ADC0_IRQHandler         [WEAK]
                EXPORT  CMP0_IRQHandler         [WEAK]
                EXPORT  TPM0_IRQHandler         [WEAK]
                EXPORT  TPM1_IRQHandler         [WEAK]
                EXPORT  TPM2_IRQHandler         [WEAK]
                EXPORT  RTC_IRQHandler         [WEAK]
                EXPORT  RTC_Seconds_IRQHandler         [WEAK]
                EXPORT  PIT_IRQHandler         [WEAK]
                EXPORT  LTC0_IRQHandler         [WEAK]
                EXPORT  Radio_0_IRQHandler         [WEAK]
                EXPORT  Radio_1_IRQHandler         [WEAK]
                EXPORT  MCG_IRQHandler         [WEAK]
                EXPORT  LPTMR0_IRQHandler         [WEAK]
                EXPORT  SPI1_DriverIRQHandler         [WEAK]
                EXPORT  PORTA_IRQHandler         [WEAK]
                EXPORT  PORTB_PORTC_IRQHandler         [WEAK]
                EXPORT  DefaultISR         [WEAK]
DMA0_DriverIRQHandler
DMA1_DriverIRQHandler
DMA2_DriverIRQHandler
DMA3_DriverIRQHandler
Reserved20_IRQHandler
FTFE_IRQHandler
LVD_LVW_DCDC_IRQHandler
LLWU_IRQHandler
I2C0_DriverIRQHandler
I2C1_DriverIRQHandler
SPI0_DriverIRQHandler
CAN0_DriverIRQHandler
LPUART0_LPUART1_DriverIRQHandler
TRNG0_IRQHandler
CMT_IRQHandler
ADC0_IRQHandler
CMP0_IRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT_IRQHandler
LTC0_IRQHandler
Radio_0_IRQHandler
Radio_1_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
SPI1_DriverIRQHandler
PORTA_IRQHandler
PORTB_PORTC_IRQHandler
DefaultISR
                LDR    R0, =DefaultISR
                BX     R0
                ENDP
                IF      :DEF:SUPPORT_WARMBOOT
warmmain\
                PROC
                EXPORT  warmmain         [WEAK]
                B       .
                ENDP
                ENDIF
                  ALIGN


                END
