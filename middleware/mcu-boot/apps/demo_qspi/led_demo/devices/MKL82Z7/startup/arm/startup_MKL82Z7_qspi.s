; * ---------------------------------------------------------------------------------------
; *  @file:    startup_MKL82Z7.s
; *  @purpose: CMSIS Cortex-M0P Core Device Startup File
; *            MKL82Z7
; *  @version: 1.4
; *  @date:    2015-8-28
; *  @build:   b150910
; * ---------------------------------------------------------------------------------------
; *
; * Copyright (c) 1997-2015 Freescale Semiconductor, Inc.
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
__Vectors_End

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
FOPT          EQU     0xBD
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
                IMPORT  init_data_bss
                IMPORT  __main

                IF      :LNOT::DEF:RAM_TARGET
                REQUIRE FlashConfig
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
Default_Handler\
                PROC
                EXPORT  DMA0_DMA4_IRQHandler         [WEAK]
                EXPORT  DMA1_DMA5_IRQHandler         [WEAK]
                EXPORT  DMA2_DMA6_IRQHandler         [WEAK]
                EXPORT  DMA3_DMA7_IRQHandler         [WEAK]
                EXPORT  DMA_Error_IRQHandler         [WEAK]
                EXPORT  FLEXIO0_IRQHandler         [WEAK]
                EXPORT  TPM0_IRQHandler         [WEAK]
                EXPORT  TPM1_IRQHandler         [WEAK]
                EXPORT  TPM2_IRQHandler         [WEAK]
                EXPORT  PIT0_IRQHandler         [WEAK]
                EXPORT  SPI0_IRQHandler         [WEAK]
                EXPORT  EMVSIM0_IRQHandler         [WEAK]
                EXPORT  LPUART0_IRQHandler         [WEAK]
                EXPORT  LPUART1_IRQHandler         [WEAK]
                EXPORT  I2C0_IRQHandler         [WEAK]
                EXPORT  QSPI0_IRQHandler         [WEAK]
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
                EXPORT  SPI1_IRQHandler         [WEAK]
                EXPORT  LPUART2_IRQHandler         [WEAK]
                EXPORT  EMVSIM1_IRQHandler         [WEAK]
                EXPORT  I2C1_IRQHandler         [WEAK]
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
                EXPORT  DMA4_IRQHandler         [WEAK]
                EXPORT  DMA5_IRQHandler         [WEAK]
                EXPORT  DMA6_IRQHandler         [WEAK]
                EXPORT  DMA7_IRQHandler         [WEAK]
                EXPORT  Reserved76_IRQHandler         [WEAK]
                EXPORT  Reserved77_IRQHandler         [WEAK]
                EXPORT  Reserved78_IRQHandler         [WEAK]
                EXPORT  Reserved79_IRQHandler         [WEAK]
                EXPORT  DefaultISR         [WEAK]
DMA0_DMA4_IRQHandler
DMA1_DMA5_IRQHandler
DMA2_DMA6_IRQHandler
DMA3_DMA7_IRQHandler
DMA_Error_IRQHandler
FLEXIO0_IRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
PIT0_IRQHandler
SPI0_IRQHandler
EMVSIM0_IRQHandler
LPUART0_IRQHandler
LPUART1_IRQHandler
I2C0_IRQHandler
QSPI0_IRQHandler
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
SPI1_IRQHandler
LPUART2_IRQHandler
EMVSIM1_IRQHandler
I2C1_IRQHandler
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
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
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
