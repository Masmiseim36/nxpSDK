; ---------------------------------------------------------------------------------------
;  @file:    startup_MKL02Z4.s
;  @purpose: CMSIS Cortex-M0P Core Device Startup File
;            MKL02Z4
;  @version: 1.6
;  @date:    2015-7-29
;  @build:   b170713
; ---------------------------------------------------------------------------------------
;
; The Clear BSD License
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2017 NXP
; All rights reserved.
; 
; Redistribution and use in source and binary forms, with or without modification,
; are permitted (subject to the limitations in the disclaimer below) provided
;  that the following conditions are met:
;
; 1. Redistributions of source code must retain the above copyright notice, this list
;   of conditions and the following disclaimer.
;
; 2. Redistributions in binary form must reproduce the above copyright notice, this
;   list of conditions and the following disclaimer in the documentation and/or
;   other materials provided with the distribution.
;
; 3. Neither the name of the copyright holder nor the names of its
;   contributors may be used to endorse or promote products derived from this
;   software without specific prior written permission.
;
; NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S' PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
; ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
; ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
        DCD     Reserved16_IRQHandler                         ;Reserved interrupt
        DCD     Reserved17_IRQHandler                         ;Reserved interrupt
        DCD     Reserved18_IRQHandler                         ;Reserved interrupt
        DCD     Reserved19_IRQHandler                         ;Reserved interrupt
        DCD     Reserved20_IRQHandler                         ;Reserved interrupt
        DCD     FTFA_IRQHandler                               ;FTFA command complete and read collision
        DCD     LVD_LVW_IRQHandler                            ;Low-voltage detect, low-voltage warning
        DCD     Reserved23_IRQHandler                         ;Reserved interrupt
        DCD     I2C0_IRQHandler                               ;I2C0 interrupt
        DCD     I2C1_IRQHandler                               ;I2C1 interrupt
        DCD     SPI0_IRQHandler                               ;SPI0 single interrupt vector for all sources
        DCD     Reserved27_IRQHandler                         ;Reserved interrupt
        DCD     UART0_IRQHandler                              ;UART0 status and error
        DCD     Reserved29_IRQHandler                         ;Reserved interrupt
        DCD     Reserved30_IRQHandler                         ;Reserved interrupt
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt
        DCD     TPM0_IRQHandler                               ;TPM0 single interrupt vector for all sources
        DCD     TPM1_IRQHandler                               ;TPM1 single interrupt vector for all sources
        DCD     Reserved35_IRQHandler                         ;Reserved interrupt
        DCD     Reserved36_IRQHandler                         ;Reserved interrupt
        DCD     Reserved37_IRQHandler                         ;Reserved interrupt
        DCD     Reserved38_IRQHandler                         ;Reserved interrupt
        DCD     Reserved39_IRQHandler                         ;Reserved interrupt
        DCD     Reserved40_IRQHandler                         ;Reserved interrupt
        DCD     Reserved41_IRQHandler                         ;Reserved interrupt
        DCD     Reserved42_IRQHandler                         ;Reserved interrupt
        DCD     MCG_IRQHandler                                ;MCG interrupt
        DCD     LPTMR0_IRQHandler                             ;LPTMR0 interrupt
        DCD     Reserved45_IRQHandler                         ;Reserved interrupt
        DCD     PORTA_IRQHandler                              ;PORTA pin detect
        DCD     PORTB_IRQHandler                              ;PORTB pin detect
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

        PUBWEAK Reserved16_IRQHandler
        PUBWEAK Reserved17_IRQHandler
        PUBWEAK Reserved18_IRQHandler
        PUBWEAK Reserved19_IRQHandler
        PUBWEAK Reserved20_IRQHandler
        PUBWEAK FTFA_IRQHandler
        PUBWEAK LVD_LVW_IRQHandler
        PUBWEAK Reserved23_IRQHandler
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

        PUBWEAK Reserved27_IRQHandler
        PUBWEAK UART0_IRQHandler
        PUBWEAK UART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UART0_IRQHandler
        LDR     R0, =UART0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved29_IRQHandler
        PUBWEAK Reserved30_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK TPM0_IRQHandler
        PUBWEAK TPM1_IRQHandler
        PUBWEAK Reserved35_IRQHandler
        PUBWEAK Reserved36_IRQHandler
        PUBWEAK Reserved37_IRQHandler
        PUBWEAK Reserved38_IRQHandler
        PUBWEAK Reserved39_IRQHandler
        PUBWEAK Reserved40_IRQHandler
        PUBWEAK Reserved41_IRQHandler
        PUBWEAK Reserved42_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK Reserved45_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved16_IRQHandler
Reserved17_IRQHandler
Reserved18_IRQHandler
Reserved19_IRQHandler
Reserved20_IRQHandler
FTFA_IRQHandler
LVD_LVW_IRQHandler
Reserved23_IRQHandler
I2C0_DriverIRQHandler
I2C1_DriverIRQHandler
SPI0_DriverIRQHandler
Reserved27_IRQHandler
UART0_DriverIRQHandler
Reserved29_IRQHandler
Reserved30_IRQHandler
ADC0_IRQHandler
CMP0_IRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
Reserved35_IRQHandler
Reserved36_IRQHandler
Reserved37_IRQHandler
Reserved38_IRQHandler
Reserved39_IRQHandler
Reserved40_IRQHandler
Reserved41_IRQHandler
Reserved42_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
Reserved45_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

        END
