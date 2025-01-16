; -------------------------------------------------------------------------
;  @file:    startup_MIMXRT758S_cm33_core1.s
;  @purpose: CMSIS Cortex-M33 Core Device Startup File
;            MIMXRT758S_cm33_core1
;  @version: 2.0
;  @date:    2024-5-28
;  @build:   b241121
; -------------------------------------------------------------------------
;
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2024 NXP
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
        SECTION RO:CODE:NOROOT(2)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

;;The vector table is not needed for initialization.
__iar_init$$done

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     MemManage_Handler                             ;MPU Fault Handler
        DCD     BusFault_Handler                              ;Bus Fault Handler
        DCD     UsageFault_Handler                            ;Usage Fault Handler
__vector_table_0x1c
        DCD     SecureFault_Handler                           ;Secure Fault Handler
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
        DCD     0x180000 ;Image length
#else
        DCD     sfe(RO) - __vector_table ;Image length
#endif
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     DebugMon_Handler                              ;Debug Monitor Handler
        DCD     __vector_table                                ;Image load address
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     Reserved16_IRQHandler                         ;Reserved interrupt 16
        DCD     Reserved17_IRQHandler                         ;Reserved interrupt 17
        DCD     Reserved18_IRQHandler                         ;Reserved interrupt 18
        DCD     FRO2_IRQHandler                               ;FRO: 300MHz FRO in VDD1 domain Async interrupt
        DCD     GLIKEY4_IRQHandler                            ;GLIKEY: Interrupt
        DCD     UTICK1_IRQHandler                             ;Micro-tick Timer
        DCD     MRT1_IRQHandler                               ;MRT: Ored Interrupt request
        DCD     CTIMER5_IRQHandler                            ;CTIMER: Interrupt request
        DCD     CTIMER6_IRQHandler                            ;CTIMER: Interrupt request
        DCD     CTIMER7_IRQHandler                            ;CTIMER: Interrupt request
        DCD     LPI2C15_IRQHandler                            ;LPI2C: Interrupt request
        DCD     LP_FLEXCOMM17_IRQHandler                      ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM18_IRQHandler                      ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM19_IRQHandler                      ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM20_IRQHandler                      ;FLEXCOMM: Interrupt request
        DCD     ADC_IRQHandler                                ;ADC0: Interrupt request
        DCD     SDADC_IRQHandler                              ;SDADC0: Interrupt request
        DCD     GLIKEY1_IRQHandler                            ;GLIKEY: Interrupt
        DCD     ACMP_IRQHandler                               ;ACMP: interrupt request
        DCD     PDM_EVENT_IRQHandler                          ;MIC: Interrupt request for read data or Error
        DCD     HYPERVISOR_IRQHandler                         ;SECURE: Sense domain Hypervisor interrupt
        DCD     SECURE_VIOLATION_IRQHandler                   ;SECURE: Sense domain Secure violation interrupt
        DCD     PDM_HWVAD_EVENT_IRQHandler                    ;MIC: Hardware Voice Activity Detector interrupt or error interrupt
        DCD     RTC1_ALARM_IRQHandler                         ;RTC: Alarm interrupt
        DCD     RTC1_IRQHandler                               ;RTC: wakeup interrupt to Sense domain
        DCD     HIFI1_IRQHandler                              ;HIFI1: Interrupt request
        DCD     MU1_B_IRQHandler                              ;MU1: MUB, CPU1 to CPU0
        DCD     Reserved43_IRQHandler                         ;Reserved interrupt
        DCD     MU3_A_IRQHandler                              ;MU3: MUA, CPU1 to HiFi1
        DCD     PMC_IRQHandler                                ;PMC: Sense domain Interrupt
        DCD     OS_EVENT_IRQHandler                           ;OSEVENT: Event timer CPU1 Wakeup/interrupt
        DCD     USDHC0_IRQHandler                             ;USDHC: Interrupt request
        DCD     USDHC1_IRQHandler                             ;USDHC: Interrupt request
        DCD     I3C2_IRQHandler                               ;I3C: Interrupt Request
        DCD     USB0_IRQHandler                               ;USB: HSUSB Interrup request
        DCD     USB1_IRQHandler                               ;USB: eUSB Interrup request
        DCD     WDT2_IRQHandler                               ;WDT: Interrupt request
        DCD     WDT3_IRQHandler                               ;WDT: Interrupt request
        DCD     USBPHY0_IRQHandler                            ;HSUSBPHY: UTM interrupt request
        DCD     Reserved55_IRQHandler                         ;xxx Interrupt 55
        DCD     I3C3_IRQHandler                               ;I3C: Interrupt Request
        DCD     FLEXIO_IRQHandler                             ;flexio: Interrupt request
        DCD     LCDIF_IRQHandler                              ;dcn: Interrupt request
        DCD     VGPU_IRQHandler                               ;VGPU interrupt from graphics core
        DCD     MIPI_IRQHandler                               ;DSI: Interrupt request
        DCD     EDMA2_CH0_IRQHandler                          ;edma2: Channel 0 interrupt
        DCD     EDMA2_CH1_IRQHandler                          ;edma2: Channel 1 interrupt
        DCD     EDMA2_CH2_IRQHandler                          ;edma2: Channel 2 interrupt
        DCD     EDMA2_CH3_IRQHandler                          ;edma2: Channel 3 interrupt
        DCD     EDMA2_CH4_IRQHandler                          ;edma2: Channel 4 interrupt
        DCD     EDMA2_CH5_IRQHandler                          ;edma2: Channel 5 interrupt
        DCD     EDMA2_CH6_IRQHandler                          ;edma2: Channel 6 interrupt
        DCD     EDMA2_CH7_IRQHandler                          ;edma2: Channel 7 interrupt
        DCD     EDMA3_CH0_IRQHandler                          ;edma3: Channel 0 interrupt
        DCD     EDMA3_CH1_IRQHandler                          ;edma3: Channel 1 interrupt
        DCD     EDMA3_CH2_IRQHandler                          ;edma3: Channel 2 interrupt
        DCD     EDMA3_CH3_IRQHandler                          ;edma3: Channel 3 interrupt
        DCD     EDMA3_CH4_IRQHandler                          ;edma3: Channel 4 interrupt
        DCD     EDMA3_CH5_IRQHandler                          ;edma3: Channel 5 interrupt
        DCD     EDMA3_CH6_IRQHandler                          ;edma3: Channel 6 interrupt
        DCD     EDMA3_CH7_IRQHandler                          ;edma3: Channel 7 interrupt
        DCD     GPIO80_IRQHandler                             ;rgpio8: Interupt request, channel 0
        DCD     GPIO81_IRQHandler                             ;rgpio8: Interupt request, channel 1
        DCD     GPIO90_IRQHandler                             ;rgpio9: Interupt request, channel 0
        DCD     GPIO91_IRQHandler                             ;rgpio9: Interupt request, channel 1
        DCD     GPIO100_IRQHandler                            ;rgpio10: Interupt request, channel 0
        DCD     GPIO101_IRQHandler                            ;rgpio10: Interupt request, channel 1
        DCD     Reserved83_IRQHandler                         ;Reserved interrupt 83
        DCD     Reserved84_IRQHandler                         ;Reserved interrupt 84
        DCD     PIN_INT0_IRQHandler                           ;pint1: Interupt request 0
        DCD     PIN_INT1_IRQHandler                           ;pint1: Interupt request 1
        DCD     PIN_INT2_IRQHandler                           ;pint1: Interupt request 2
        DCD     PIN_INT3_IRQHandler                           ;pint1: Interupt request 3
        DCD     SAI3_IRQHandler                               ;sai3: TX/RX interrupt
        DCD     XSPI2_IRQHandler                              ;xspi2: Ored interrupt
        DCD     MMU2_IRQHandler                               ;mmu2: Interrupt request
        DCD     Reserved92_IRQHandler                         ;Reserved interrupt
        DCD     GDET3_IRQHandler                              ;GDET3: Interrupt request
        DCD     CDOG3_IRQHandler                              ;cdog3: Interrupt request
        DCD     CDOG4_IRQHandler                              ;cdog4: Interrupt request
        DCD     Reserved96_IRQHandler                         ;Reserved interrupt 96
        DCD     CPU1_IRQHandler                               ;CTI interrupt request
        DCD     JPEGDEC_IRQHandler                            ;JPEGDEC Ored context 0-3 interrupt request
        DCD     PNGDEC_IRQHandler                             ;PNGDEC interrupt request
        DCD     Reserved100_IRQHandler                        ;Reserved interrupt 100
        DCD     TEMPDET0_IRQHandler                           ;TEMPDET0: Temperature Detect Interrupt request 0
        DCD     TEMPDET1_IRQHandler                           ;TEMPDET1: Temperature Detect Interrupt request 1
        DCD     EZHV_IRQHandler                               ;ezhv: Interrupt request
        DCD     SLEEPCON1_IRQHandler                          ;SLEEPCON_SENSE CPU1 wakeup event
        DCD     PVTS1_CPU1_IRQHandler                         ;PVTS1 CPU1 interrupt
        DCD     Reserved106_IRQHandler                        ;Reserved interrupt 106
        DCD     PVTS1_HIFI1_IRQHandler                        ;PVTS1 HIFI1 interrupt
        DCD     Reserved108_IRQHandler                        ;Reserved interrupt 108
__Vectors_End

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
        LDR     R0, =sfb(CSTACK)
        MSR     MSPLIM, R0
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =SystemInit
        BLX     R0
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

        PUBWEAK SecureFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SecureFault_Handler
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

        PUBWEAK Reserved16_IRQHandler
        PUBWEAK Reserved16_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved16_IRQHandler
        LDR     R0, =Reserved16_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved17_IRQHandler
        PUBWEAK Reserved17_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved17_IRQHandler
        LDR     R0, =Reserved17_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved18_IRQHandler
        PUBWEAK Reserved18_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved18_IRQHandler
        LDR     R0, =Reserved18_DriverIRQHandler
        BX      R0

        PUBWEAK FRO2_IRQHandler
        PUBWEAK FRO2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FRO2_IRQHandler
        LDR     R0, =FRO2_DriverIRQHandler
        BX      R0

        PUBWEAK GLIKEY4_IRQHandler
        PUBWEAK GLIKEY4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GLIKEY4_IRQHandler
        LDR     R0, =GLIKEY4_DriverIRQHandler
        BX      R0

        PUBWEAK UTICK1_IRQHandler
        PUBWEAK UTICK1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UTICK1_IRQHandler
        LDR     R0, =UTICK1_DriverIRQHandler
        BX      R0

        PUBWEAK MRT1_IRQHandler
        PUBWEAK MRT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MRT1_IRQHandler
        LDR     R0, =MRT1_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER5_IRQHandler
        PUBWEAK CTIMER5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER5_IRQHandler
        LDR     R0, =CTIMER5_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER6_IRQHandler
        PUBWEAK CTIMER6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER6_IRQHandler
        LDR     R0, =CTIMER6_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER7_IRQHandler
        PUBWEAK CTIMER7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER7_IRQHandler
        LDR     R0, =CTIMER7_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C15_IRQHandler
        PUBWEAK LPI2C15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C15_IRQHandler
        LDR     R0, =LPI2C15_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM17_IRQHandler
        PUBWEAK LP_FLEXCOMM17_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM17_IRQHandler
        LDR     R0, =LP_FLEXCOMM17_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM18_IRQHandler
        PUBWEAK LP_FLEXCOMM18_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM18_IRQHandler
        LDR     R0, =LP_FLEXCOMM18_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM19_IRQHandler
        PUBWEAK LP_FLEXCOMM19_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM19_IRQHandler
        LDR     R0, =LP_FLEXCOMM19_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM20_IRQHandler
        PUBWEAK LP_FLEXCOMM20_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM20_IRQHandler
        LDR     R0, =LP_FLEXCOMM20_DriverIRQHandler
        BX      R0

        PUBWEAK ADC_IRQHandler
        PUBWEAK ADC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADC_IRQHandler
        LDR     R0, =ADC_DriverIRQHandler
        BX      R0

        PUBWEAK SDADC_IRQHandler
        PUBWEAK SDADC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SDADC_IRQHandler
        LDR     R0, =SDADC_DriverIRQHandler
        BX      R0

        PUBWEAK GLIKEY1_IRQHandler
        PUBWEAK GLIKEY1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GLIKEY1_IRQHandler
        LDR     R0, =GLIKEY1_DriverIRQHandler
        BX      R0

        PUBWEAK ACMP_IRQHandler
        PUBWEAK ACMP_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ACMP_IRQHandler
        LDR     R0, =ACMP_DriverIRQHandler
        BX      R0

        PUBWEAK PDM_EVENT_IRQHandler
        PUBWEAK PDM_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PDM_EVENT_IRQHandler
        LDR     R0, =PDM_EVENT_DriverIRQHandler
        BX      R0

        PUBWEAK HYPERVISOR_IRQHandler
        PUBWEAK HYPERVISOR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
HYPERVISOR_IRQHandler
        LDR     R0, =HYPERVISOR_DriverIRQHandler
        BX      R0

        PUBWEAK SECURE_VIOLATION_IRQHandler
        PUBWEAK SECURE_VIOLATION_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SECURE_VIOLATION_IRQHandler
        LDR     R0, =SECURE_VIOLATION_DriverIRQHandler
        BX      R0

        PUBWEAK PDM_HWVAD_EVENT_IRQHandler
        PUBWEAK PDM_HWVAD_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PDM_HWVAD_EVENT_IRQHandler
        LDR     R0, =PDM_HWVAD_EVENT_DriverIRQHandler
        BX      R0

        PUBWEAK RTC1_ALARM_IRQHandler
        PUBWEAK RTC1_ALARM_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RTC1_ALARM_IRQHandler
        LDR     R0, =RTC1_ALARM_DriverIRQHandler
        BX      R0

        PUBWEAK RTC1_IRQHandler
        PUBWEAK RTC1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RTC1_IRQHandler
        LDR     R0, =RTC1_DriverIRQHandler
        BX      R0

        PUBWEAK HIFI1_IRQHandler
        PUBWEAK HIFI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
HIFI1_IRQHandler
        LDR     R0, =HIFI1_DriverIRQHandler
        BX      R0

        PUBWEAK MU1_B_IRQHandler
        PUBWEAK MU1_B_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MU1_B_IRQHandler
        LDR     R0, =MU1_B_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved43_IRQHandler
        PUBWEAK Reserved43_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved43_IRQHandler
        LDR     R0, =Reserved43_DriverIRQHandler
        BX      R0

        PUBWEAK MU3_A_IRQHandler
        PUBWEAK MU3_A_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MU3_A_IRQHandler
        LDR     R0, =MU3_A_DriverIRQHandler
        BX      R0

        PUBWEAK PMC_IRQHandler
        PUBWEAK PMC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PMC_IRQHandler
        LDR     R0, =PMC_DriverIRQHandler
        BX      R0

        PUBWEAK OS_EVENT_IRQHandler
        PUBWEAK OS_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
OS_EVENT_IRQHandler
        LDR     R0, =OS_EVENT_DriverIRQHandler
        BX      R0

        PUBWEAK USDHC0_IRQHandler
        PUBWEAK USDHC0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC0_IRQHandler
        LDR     R0, =USDHC0_DriverIRQHandler
        BX      R0

        PUBWEAK USDHC1_IRQHandler
        PUBWEAK USDHC1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC1_IRQHandler
        LDR     R0, =USDHC1_DriverIRQHandler
        BX      R0

        PUBWEAK I3C2_IRQHandler
        PUBWEAK I3C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I3C2_IRQHandler
        LDR     R0, =I3C2_DriverIRQHandler
        BX      R0

        PUBWEAK USB0_IRQHandler
        PUBWEAK USB0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USB0_IRQHandler
        LDR     R0, =USB0_DriverIRQHandler
        BX      R0

        PUBWEAK USB1_IRQHandler
        PUBWEAK USB1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USB1_IRQHandler
        LDR     R0, =USB1_DriverIRQHandler
        BX      R0

        PUBWEAK WDT2_IRQHandler
        PUBWEAK WDT2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WDT2_IRQHandler
        LDR     R0, =WDT2_DriverIRQHandler
        BX      R0

        PUBWEAK WDT3_IRQHandler
        PUBWEAK WDT3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WDT3_IRQHandler
        LDR     R0, =WDT3_DriverIRQHandler
        BX      R0

        PUBWEAK USBPHY0_IRQHandler
        PUBWEAK USBPHY0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USBPHY0_IRQHandler
        LDR     R0, =USBPHY0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved55_IRQHandler
        PUBWEAK Reserved55_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved55_IRQHandler
        LDR     R0, =Reserved55_DriverIRQHandler
        BX      R0

        PUBWEAK I3C3_IRQHandler
        PUBWEAK I3C3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I3C3_IRQHandler
        LDR     R0, =I3C3_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXIO_IRQHandler
        PUBWEAK FLEXIO_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO_IRQHandler
        LDR     R0, =FLEXIO_DriverIRQHandler
        BX      R0

        PUBWEAK LCDIF_IRQHandler
        PUBWEAK LCDIF_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LCDIF_IRQHandler
        LDR     R0, =LCDIF_DriverIRQHandler
        BX      R0

        PUBWEAK VGPU_IRQHandler
        PUBWEAK VGPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
VGPU_IRQHandler
        LDR     R0, =VGPU_DriverIRQHandler
        BX      R0

        PUBWEAK MIPI_IRQHandler
        PUBWEAK MIPI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MIPI_IRQHandler
        LDR     R0, =MIPI_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA2_CH0_IRQHandler
        PUBWEAK EDMA2_CH0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA2_CH0_IRQHandler
        LDR     R0, =EDMA2_CH0_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA2_CH1_IRQHandler
        PUBWEAK EDMA2_CH1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA2_CH1_IRQHandler
        LDR     R0, =EDMA2_CH1_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA2_CH2_IRQHandler
        PUBWEAK EDMA2_CH2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA2_CH2_IRQHandler
        LDR     R0, =EDMA2_CH2_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA2_CH3_IRQHandler
        PUBWEAK EDMA2_CH3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA2_CH3_IRQHandler
        LDR     R0, =EDMA2_CH3_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA2_CH4_IRQHandler
        PUBWEAK EDMA2_CH4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA2_CH4_IRQHandler
        LDR     R0, =EDMA2_CH4_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA2_CH5_IRQHandler
        PUBWEAK EDMA2_CH5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA2_CH5_IRQHandler
        LDR     R0, =EDMA2_CH5_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA2_CH6_IRQHandler
        PUBWEAK EDMA2_CH6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA2_CH6_IRQHandler
        LDR     R0, =EDMA2_CH6_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA2_CH7_IRQHandler
        PUBWEAK EDMA2_CH7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA2_CH7_IRQHandler
        LDR     R0, =EDMA2_CH7_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA3_CH0_IRQHandler
        PUBWEAK EDMA3_CH0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA3_CH0_IRQHandler
        LDR     R0, =EDMA3_CH0_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA3_CH1_IRQHandler
        PUBWEAK EDMA3_CH1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA3_CH1_IRQHandler
        LDR     R0, =EDMA3_CH1_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA3_CH2_IRQHandler
        PUBWEAK EDMA3_CH2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA3_CH2_IRQHandler
        LDR     R0, =EDMA3_CH2_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA3_CH3_IRQHandler
        PUBWEAK EDMA3_CH3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA3_CH3_IRQHandler
        LDR     R0, =EDMA3_CH3_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA3_CH4_IRQHandler
        PUBWEAK EDMA3_CH4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA3_CH4_IRQHandler
        LDR     R0, =EDMA3_CH4_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA3_CH5_IRQHandler
        PUBWEAK EDMA3_CH5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA3_CH5_IRQHandler
        LDR     R0, =EDMA3_CH5_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA3_CH6_IRQHandler
        PUBWEAK EDMA3_CH6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA3_CH6_IRQHandler
        LDR     R0, =EDMA3_CH6_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA3_CH7_IRQHandler
        PUBWEAK EDMA3_CH7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA3_CH7_IRQHandler
        LDR     R0, =EDMA3_CH7_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO80_IRQHandler
        PUBWEAK GPIO80_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO80_IRQHandler
        LDR     R0, =GPIO80_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO81_IRQHandler
        PUBWEAK GPIO81_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO81_IRQHandler
        LDR     R0, =GPIO81_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO90_IRQHandler
        PUBWEAK GPIO90_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO90_IRQHandler
        LDR     R0, =GPIO90_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO91_IRQHandler
        PUBWEAK GPIO91_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO91_IRQHandler
        LDR     R0, =GPIO91_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO100_IRQHandler
        PUBWEAK GPIO100_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO100_IRQHandler
        LDR     R0, =GPIO100_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO101_IRQHandler
        PUBWEAK GPIO101_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO101_IRQHandler
        LDR     R0, =GPIO101_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved83_IRQHandler
        PUBWEAK Reserved83_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved83_IRQHandler
        LDR     R0, =Reserved83_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved84_IRQHandler
        PUBWEAK Reserved84_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved84_IRQHandler
        LDR     R0, =Reserved84_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT0_IRQHandler
        PUBWEAK PIN_INT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT0_IRQHandler
        LDR     R0, =PIN_INT0_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT1_IRQHandler
        PUBWEAK PIN_INT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT1_IRQHandler
        LDR     R0, =PIN_INT1_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT2_IRQHandler
        PUBWEAK PIN_INT2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT2_IRQHandler
        LDR     R0, =PIN_INT2_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT3_IRQHandler
        PUBWEAK PIN_INT3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT3_IRQHandler
        LDR     R0, =PIN_INT3_DriverIRQHandler
        BX      R0

        PUBWEAK SAI3_IRQHandler
        PUBWEAK SAI3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SAI3_IRQHandler
        LDR     R0, =SAI3_DriverIRQHandler
        BX      R0

        PUBWEAK XSPI2_IRQHandler
        PUBWEAK XSPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
XSPI2_IRQHandler
        LDR     R0, =XSPI2_DriverIRQHandler
        BX      R0

        PUBWEAK MMU2_IRQHandler
        PUBWEAK MMU2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MMU2_IRQHandler
        LDR     R0, =MMU2_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved92_IRQHandler
        PUBWEAK Reserved92_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved92_IRQHandler
        LDR     R0, =Reserved92_DriverIRQHandler
        BX      R0

        PUBWEAK GDET3_IRQHandler
        PUBWEAK GDET3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GDET3_IRQHandler
        LDR     R0, =GDET3_DriverIRQHandler
        BX      R0

        PUBWEAK CDOG3_IRQHandler
        PUBWEAK CDOG3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CDOG3_IRQHandler
        LDR     R0, =CDOG3_DriverIRQHandler
        BX      R0

        PUBWEAK CDOG4_IRQHandler
        PUBWEAK CDOG4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CDOG4_IRQHandler
        LDR     R0, =CDOG4_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved96_IRQHandler
        PUBWEAK Reserved96_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved96_IRQHandler
        LDR     R0, =Reserved96_DriverIRQHandler
        BX      R0

        PUBWEAK CPU1_IRQHandler
        PUBWEAK CPU1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CPU1_IRQHandler
        LDR     R0, =CPU1_DriverIRQHandler
        BX      R0

        PUBWEAK JPEGDEC_IRQHandler
        PUBWEAK JPEGDEC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
JPEGDEC_IRQHandler
        LDR     R0, =JPEGDEC_DriverIRQHandler
        BX      R0

        PUBWEAK PNGDEC_IRQHandler
        PUBWEAK PNGDEC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PNGDEC_IRQHandler
        LDR     R0, =PNGDEC_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved100_IRQHandler
        PUBWEAK Reserved100_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved100_IRQHandler
        LDR     R0, =Reserved100_DriverIRQHandler
        BX      R0

        PUBWEAK TEMPDET0_IRQHandler
        PUBWEAK TEMPDET0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
TEMPDET0_IRQHandler
        LDR     R0, =TEMPDET0_DriverIRQHandler
        BX      R0

        PUBWEAK TEMPDET1_IRQHandler
        PUBWEAK TEMPDET1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
TEMPDET1_IRQHandler
        LDR     R0, =TEMPDET1_DriverIRQHandler
        BX      R0

        PUBWEAK EZHV_IRQHandler
        PUBWEAK EZHV_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EZHV_IRQHandler
        LDR     R0, =EZHV_DriverIRQHandler
        BX      R0

        PUBWEAK SLEEPCON1_IRQHandler
        PUBWEAK SLEEPCON1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SLEEPCON1_IRQHandler
        LDR     R0, =SLEEPCON1_DriverIRQHandler
        BX      R0

        PUBWEAK PVTS1_CPU1_IRQHandler
        PUBWEAK PVTS1_CPU1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PVTS1_CPU1_IRQHandler
        LDR     R0, =PVTS1_CPU1_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved106_IRQHandler
        PUBWEAK Reserved106_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved106_IRQHandler
        LDR     R0, =Reserved106_DriverIRQHandler
        BX      R0

        PUBWEAK PVTS1_HIFI1_IRQHandler
        PUBWEAK PVTS1_HIFI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PVTS1_HIFI1_IRQHandler
        LDR     R0, =PVTS1_HIFI1_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved108_IRQHandler
        PUBWEAK Reserved108_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved108_IRQHandler
        LDR     R0, =Reserved108_DriverIRQHandler
        BX      R0

Reserved16_DriverIRQHandler
Reserved17_DriverIRQHandler
Reserved18_DriverIRQHandler
FRO2_DriverIRQHandler
GLIKEY4_DriverIRQHandler
UTICK1_DriverIRQHandler
MRT1_DriverIRQHandler
CTIMER5_DriverIRQHandler
CTIMER6_DriverIRQHandler
CTIMER7_DriverIRQHandler
LPI2C15_DriverIRQHandler
LP_FLEXCOMM17_DriverIRQHandler
LP_FLEXCOMM18_DriverIRQHandler
LP_FLEXCOMM19_DriverIRQHandler
LP_FLEXCOMM20_DriverIRQHandler
ADC_DriverIRQHandler
SDADC_DriverIRQHandler
GLIKEY1_DriverIRQHandler
ACMP_DriverIRQHandler
PDM_EVENT_DriverIRQHandler
HYPERVISOR_DriverIRQHandler
SECURE_VIOLATION_DriverIRQHandler
PDM_HWVAD_EVENT_DriverIRQHandler
RTC1_ALARM_DriverIRQHandler
RTC1_DriverIRQHandler
HIFI1_DriverIRQHandler
MU1_B_DriverIRQHandler
Reserved43_DriverIRQHandler
MU3_A_DriverIRQHandler
PMC_DriverIRQHandler
OS_EVENT_DriverIRQHandler
USDHC0_DriverIRQHandler
USDHC1_DriverIRQHandler
I3C2_DriverIRQHandler
USB0_DriverIRQHandler
USB1_DriverIRQHandler
WDT2_DriverIRQHandler
WDT3_DriverIRQHandler
USBPHY0_DriverIRQHandler
Reserved55_DriverIRQHandler
I3C3_DriverIRQHandler
FLEXIO_DriverIRQHandler
LCDIF_DriverIRQHandler
VGPU_DriverIRQHandler
MIPI_DriverIRQHandler
EDMA2_CH0_DriverIRQHandler
EDMA2_CH1_DriverIRQHandler
EDMA2_CH2_DriverIRQHandler
EDMA2_CH3_DriverIRQHandler
EDMA2_CH4_DriverIRQHandler
EDMA2_CH5_DriverIRQHandler
EDMA2_CH6_DriverIRQHandler
EDMA2_CH7_DriverIRQHandler
EDMA3_CH0_DriverIRQHandler
EDMA3_CH1_DriverIRQHandler
EDMA3_CH2_DriverIRQHandler
EDMA3_CH3_DriverIRQHandler
EDMA3_CH4_DriverIRQHandler
EDMA3_CH5_DriverIRQHandler
EDMA3_CH6_DriverIRQHandler
EDMA3_CH7_DriverIRQHandler
GPIO80_DriverIRQHandler
GPIO81_DriverIRQHandler
GPIO90_DriverIRQHandler
GPIO91_DriverIRQHandler
GPIO100_DriverIRQHandler
GPIO101_DriverIRQHandler
Reserved83_DriverIRQHandler
Reserved84_DriverIRQHandler
PIN_INT0_DriverIRQHandler
PIN_INT1_DriverIRQHandler
PIN_INT2_DriverIRQHandler
PIN_INT3_DriverIRQHandler
SAI3_DriverIRQHandler
XSPI2_DriverIRQHandler
MMU2_DriverIRQHandler
Reserved92_DriverIRQHandler
GDET3_DriverIRQHandler
CDOG3_DriverIRQHandler
CDOG4_DriverIRQHandler
Reserved96_DriverIRQHandler
CPU1_DriverIRQHandler
JPEGDEC_DriverIRQHandler
PNGDEC_DriverIRQHandler
Reserved100_DriverIRQHandler
TEMPDET0_DriverIRQHandler
TEMPDET1_DriverIRQHandler
EZHV_DriverIRQHandler
SLEEPCON1_DriverIRQHandler
PVTS1_CPU1_DriverIRQHandler
Reserved106_DriverIRQHandler
PVTS1_HIFI1_DriverIRQHandler
Reserved108_DriverIRQHandler
DefaultISR
        B .

        END
