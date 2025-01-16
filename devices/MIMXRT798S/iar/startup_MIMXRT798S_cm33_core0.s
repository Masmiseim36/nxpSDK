; -------------------------------------------------------------------------
;  @file:    startup_MIMXRT798S_cm33_core0.s
;  @purpose: CMSIS Cortex-M33 Core Device Startup File
;            MIMXRT798S_cm33_core0
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
        DCD     UTICK0_IRQHandler                             ;Micro-tick Timer
        DCD     MRT0_IRQHandler                               ;MRT: Ored Interrupt request
        DCD     CTIMER0_IRQHandler                            ;CTIMER: Interrupt request
        DCD     CTIMER1_IRQHandler                            ;CTIMER: Interrupt request
        DCD     SCT0_IRQHandler                               ;SCT: Interrupt request
        DCD     CTIMER3_IRQHandler                            ;CTIMER: Interrupt request
        DCD     LP_FLEXCOMM0_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM1_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM2_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM3_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM4_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM5_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LPSPI14_IRQHandler                            ;LPSPI: Interrupt request
        DCD     LPI2C15_IRQHandler                            ;LPI2C: Interrupt request
        DCD     ADC_IRQHandler                                ;ADC0: Interrupt request
        DCD     SDADC_IRQHandler                              ;SDADC0: Interrupt request
        DCD     ACMP_IRQHandler                               ;ACMP: interrupt request
        DCD     PDM_EVENT_IRQHandler                          ;MIC: Interrupt request for read data or Error
        DCD     PDM_HWVAD_EVENT_IRQHandler                    ;MIC: Hardware Voice Activity Detector interrupt or error interrupt
        DCD     HYPERVISOR_IRQHandler                         ;SECURE: Compute domain P-Bus Hypervisor interrupt
        DCD     SECURE_VIOLATION_IRQHandler                   ;SECURE: Compute domain P-Bus Secure violation interrupt
        DCD     SENSE_HYPERVISOR_IRQHandler                   ;Sense domain Hypervisor interrupt
        DCD     SENSE_SECURE_VIOLATION_IRQHandler             ;Sense domain Secure violation interrupt
        DCD     MEDIA_HYPERVISOR_IRQHandler                   ;Media domain Hypervisor interrupt
        DCD     MEDIA_SECURE_VIOLATION_IRQHandler             ;Media domain Secure violation interrupt
        DCD     RTC0_ALARM_IRQHandler                         ;RTC: Alarm interrupt
        DCD     RTC0_IRQHandler                               ;RTC: wakeup interrupt to Compute domain
        DCD     HIFI4_IRQHandler                              ;HIFI4: Interrupt request
        DCD     MU0_A_IRQHandler                              ;MU0: MUA, CPU0 to HiFi1
        DCD     MU1_A_IRQHandler                              ;MU1: MUA, CPU0 to CPU1
        DCD     MU4_A_IRQHandler                              ;MU4: MUA, CPU0 to HiFi4
        DCD     CTIMER2_IRQHandler                            ;CTIMER: Interrupt request
        DCD     CTIMER4_IRQHandler                            ;CTIMER: Interrupt request
        DCD     OS_EVENT_IRQHandler                           ;OSEVENT: Event timer CPU0 Wakeup/interrupt
        DCD     LP_FLEXCOMM6_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM7_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     USDHC0_IRQHandler                             ;USDHC: Interrupt request
        DCD     USDHC1_IRQHandler                             ;USDHC: Interrupt request
        DCD     I3C0_IRQHandler                               ;I3C: Interrupt Request
        DCD     USB0_IRQHandler                               ;USB: HSUSB Interrup request
        DCD     USB1_IRQHandler                               ;USB: eUSB Interrup request
        DCD     WDT0_IRQHandler                               ;WDT: Interrupt request
        DCD     WDT1_IRQHandler                               ;WDT: Interrupt request
        DCD     USBPHY0_IRQHandler                            ;HSUSBPHY: UTM interrupt request
        DCD     PUF_IRQHandler                                ;PUF: Interrupt Request
        DCD     Reserved62_IRQHandler                         ;xxx Interrupt 62
        DCD     LP_FLEXCOMM8_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM9_IRQHandler                       ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM10_IRQHandler                      ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM11_IRQHandler                      ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM12_IRQHandler                      ;FLEXCOMM: Interrupt request
        DCD     LP_FLEXCOMM13_IRQHandler                      ;FLEXCOMM: Interrupt request
        DCD     LPSPI16_IRQHandler                            ;LPSPI: Interrupt request
        DCD     I3C1_IRQHandler                               ;I3C: Interrupt Request
        DCD     FLEXIO_IRQHandler                             ;FLEXIO: Interrupt request
        DCD     LCDIF_IRQHandler                              ;LCDIF: Interrupt request
        DCD     VGPU_IRQHandler                               ;VGPU: Interrupt from graphics core (Domain HCLK)
        DCD     MIPI_IRQHandler                               ;DSI: Interrupt request
        DCD     EDMA0_CH0_IRQHandler                          ;EDMA: Channel 0 interrupt
        DCD     EDMA0_CH1_IRQHandler                          ;EDMA: Channel 1 interrupt
        DCD     EDMA0_CH2_IRQHandler                          ;EDMA: Channel 2 interrupt
        DCD     EDMA0_CH3_IRQHandler                          ;EDMA: Channel 3 interrupt
        DCD     EDMA0_CH4_IRQHandler                          ;EDMA: Channel 4 interrupt
        DCD     EDMA0_CH5_IRQHandler                          ;EDMA: Channel 5 interrupt
        DCD     EDMA0_CH6_IRQHandler                          ;EDMA: Channel 6 interrupt
        DCD     EDMA0_CH7_IRQHandler                          ;EDMA: Channel 7 interrupt
        DCD     EDMA0_CH8_IRQHandler                          ;EDMA: Channel 8 interrupt
        DCD     EDMA0_CH9_IRQHandler                          ;EDMA: Channel 9 interrupt
        DCD     EDMA0_CH10_IRQHandler                         ;EDMA: Channel 10 interrupt
        DCD     EDMA0_CH11_IRQHandler                         ;EDMA: Channel 11 interrupt
        DCD     EDMA0_CH12_IRQHandler                         ;EDMA: Channel 12 interrupt
        DCD     EDMA0_CH13_IRQHandler                         ;EDMA: Channel 13 interrupt
        DCD     EDMA0_CH14_IRQHandler                         ;EDMA: Channel 14 interrupt
        DCD     EDMA0_CH15_IRQHandler                         ;EDMA: Channel 15 interrupt
        DCD     EDMA1_CH0_IRQHandler                          ;EDMA: Channel 0 interrupt
        DCD     EDMA1_CH1_IRQHandler                          ;EDMA: Channel 1 interrupt
        DCD     EDMA1_CH2_IRQHandler                          ;EDMA: Channel 2 interrupt
        DCD     EDMA1_CH3_IRQHandler                          ;EDMA: Channel 3 interrupt
        DCD     EDMA1_CH4_IRQHandler                          ;EDMA: Channel 4 interrupt
        DCD     EDMA1_CH5_IRQHandler                          ;EDMA: Channel 5 interrupt
        DCD     EDMA1_CH6_IRQHandler                          ;EDMA: Channel 6 interrupt
        DCD     EDMA1_CH7_IRQHandler                          ;EDMA: Channel 7 interrupt
        DCD     EDMA1_CH8_IRQHandler                          ;EDMA: Channel 8 interrupt
        DCD     EDMA1_CH9_IRQHandler                          ;EDMA: Channel 9 interrupt
        DCD     EDMA1_CH10_IRQHandler                         ;EDMA: Channel 10 interrupt
        DCD     EDMA1_CH11_IRQHandler                         ;EDMA: Channel 11 interrupt
        DCD     EDMA1_CH12_IRQHandler                         ;EDMA: Channel 12 interrupt
        DCD     EDMA1_CH13_IRQHandler                         ;EDMA: Channel 13 interrupt
        DCD     EDMA1_CH14_IRQHandler                         ;EDMA: Channel 14 interrupt
        DCD     EDMA1_CH15_IRQHandler                         ;EDMA: Channel 15 interrupt
        DCD     GPIO00_IRQHandler                             ;GPIO: Interupt request, channel 0
        DCD     GPIO01_IRQHandler                             ;GPIO: Interupt request, channel 1
        DCD     GPIO10_IRQHandler                             ;GPIO: Interupt request, channel 0
        DCD     GPIO11_IRQHandler                             ;GPIO: Interupt request, channel 1
        DCD     GPIO20_IRQHandler                             ;GPIO: Interupt request, channel 0
        DCD     GPIO21_IRQHandler                             ;GPIO: Interupt request, channel 1
        DCD     GPIO30_IRQHandler                             ;GPIO: Interupt request, channel 0
        DCD     GPIO31_IRQHandler                             ;GPIO: Interupt request, channel 1
        DCD     GPIO40_IRQHandler                             ;GPIO: Interupt request, channel 0
        DCD     GPIO41_IRQHandler                             ;GPIO: Interupt request, channel 1
        DCD     GPIO50_IRQHandler                             ;GPIO: Interupt request, channel 0
        DCD     GPIO51_IRQHandler                             ;GPIO: Interupt request, channel 1
        DCD     GPIO60_IRQHandler                             ;GPIO: Interupt request, channel 0
        DCD     GPIO61_IRQHandler                             ;GPIO: Interupt request, channel 1
        DCD     GPIO70_IRQHandler                             ;GPIO: Interupt request, channel 0
        DCD     GPIO71_IRQHandler                             ;GPIO: Interupt request, channel 1
        DCD     PIN_INT0_IRQHandler                           ;PINT: Interupt request 0
        DCD     PIN_INT1_IRQHandler                           ;PINT: Interupt request 1
        DCD     PIN_INT2_IRQHandler                           ;PINT: Interupt request 2
        DCD     PIN_INT3_IRQHandler                           ;PINT: Interupt request 3
        DCD     PIN_INT4_IRQHandler                           ;PINT: Interupt request 4
        DCD     PIN_INT5_IRQHandler                           ;PINT: Interupt request 5
        DCD     PIN_INT6_IRQHandler                           ;PINT: Interupt request 6
        DCD     PIN_INT7_IRQHandler                           ;PINT: Interupt request 7
        DCD     SAI0_IRQHandler                               ;SAI: TX/RX interrupt
        DCD     SAI1_IRQHandler                               ;SAI: TX/RX interrupt
        DCD     SAI2_IRQHandler                               ;SAI: TX/RX interrupt
        DCD     XSPI0_IRQHandler                              ;xSPI: Ored interrupt
        DCD     XSPI1_IRQHandler                              ;xSPI: Ored interrupt
        DCD     XSPI2_IRQHandler                              ;xSPI: Ored interrupt
        DCD     MMU0_IRQHandler                               ;MMU: Interrupt request
        DCD     MMU1_IRQHandler                               ;MMU: Interrupt request
        DCD     MMU2_IRQHandler                               ;MMU: Interrupt request
        DCD     Freqme_IRQHandler                             ;FREQME: Interrupt request
        DCD     GDET0_IRQHandler                              ;GDET0: Interrupt request
        DCD     Reserved142_IRQHandler                        ;Reserved interrupt
        DCD     Reserved143_IRQHandler                        ;Reserved interrupt
        DCD     GDET3_IRQHandler                              ;GDET3: Interrupt request
        DCD     CDOG0_IRQHandler                              ;CDOG: Interrupt request
        DCD     CDOG1_IRQHandler                              ;CDOG: Interrupt request
        DCD     CDOG2_IRQHandler                              ;CDOG: Interrupt request
        DCD     ITRC0_IRQHandler                              ;ITRC: Interrupt, cleared by software
        DCD     ELS_IRQHandler                                ;ELS Interrupt request
        DCD     PKC_IRQHandler                                ;PKC: Public Key Cryptography Interrutp
        DCD     OSCCA_IRQHandler                              ;OSCCA: OSCCA Interrupt request
        DCD     SLEEPCON0_IRQHandler                          ;WAKEUP: CPU0 wakeup event
        DCD     CPU0_IRQHandler                               ;CTI0: interrupt request
        DCD     PMC_IRQHandler                                ;PMC: Compute domain Interrupt
        DCD     JPEGDEC_IRQHandler                            ;JPEGDEC: Ored Context 0-3 interrupt request
        DCD     PNGDEC_IRQHandler                             ;PGNDEC: Interrut request
        DCD     NPU_IRQHandler                                ;NPU: Interrupt request
        DCD     TRNG_IRQHandler                               ;TRNG: interrupt
        DCD     TEMPDET0_IRQHandler                           ;TEMPDET0: Interrupt request 0
        DCD     TEMPDET1_IRQHandler                           ;TEMPDET1: Interrupt request 1
        DCD     EZHV_IRQHandler                               ;EZH-V: Interrupt request
        DCD     GLIKEY0_IRQHandler                            ;GLIKEY: Interrupt
        DCD     GLIKEY2_IRQHandler                            ;GLIKEY: Interrupt
        DCD     GLIKEY3_IRQHandler                            ;GLIKEY: Interrupt
        DCD     GLIKEY5_IRQHandler                            ;GLIKEY: Interrupt
        DCD     PVTS0_CPU0_IRQHandler                         ;PVTS0 CPU0 interrupt
        DCD     Reserved167_IRQHandler                        ;Reserved interrupt 167
        DCD     PVTS0_HIFI4_IRQHandler                        ;PVTS0 HIFI4 interrupt
        DCD     Reserved169_IRQHandler                        ;Reserved interrupt 169
        DCD     FRO0_IRQHandler                               ;FRO: 300MHz FRO in VDD2 domain Async interrupt
        DCD     FRO1_IRQHandler                               ;FRO: 192MHz FRO in VDD2 domain Async interrupt
        DCD     FRO2_IRQHandler                               ;FRO: 300MHz FRO in VDD1 domain Async interrupt
        DCD     OCOTP_IRQHandler                              ;OCOTP warning and integrity error interrupt
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

        PUBWEAK UTICK0_IRQHandler
        PUBWEAK UTICK0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UTICK0_IRQHandler
        LDR     R0, =UTICK0_DriverIRQHandler
        BX      R0

        PUBWEAK MRT0_IRQHandler
        PUBWEAK MRT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MRT0_IRQHandler
        LDR     R0, =MRT0_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER0_IRQHandler
        PUBWEAK CTIMER0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER0_IRQHandler
        LDR     R0, =CTIMER0_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER1_IRQHandler
        PUBWEAK CTIMER1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER1_IRQHandler
        LDR     R0, =CTIMER1_DriverIRQHandler
        BX      R0

        PUBWEAK SCT0_IRQHandler
        PUBWEAK SCT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SCT0_IRQHandler
        LDR     R0, =SCT0_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER3_IRQHandler
        PUBWEAK CTIMER3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER3_IRQHandler
        LDR     R0, =CTIMER3_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM0_IRQHandler
        PUBWEAK LP_FLEXCOMM0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM0_IRQHandler
        LDR     R0, =LP_FLEXCOMM0_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM1_IRQHandler
        PUBWEAK LP_FLEXCOMM1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM1_IRQHandler
        LDR     R0, =LP_FLEXCOMM1_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM2_IRQHandler
        PUBWEAK LP_FLEXCOMM2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM2_IRQHandler
        LDR     R0, =LP_FLEXCOMM2_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM3_IRQHandler
        PUBWEAK LP_FLEXCOMM3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM3_IRQHandler
        LDR     R0, =LP_FLEXCOMM3_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM4_IRQHandler
        PUBWEAK LP_FLEXCOMM4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM4_IRQHandler
        LDR     R0, =LP_FLEXCOMM4_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM5_IRQHandler
        PUBWEAK LP_FLEXCOMM5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM5_IRQHandler
        LDR     R0, =LP_FLEXCOMM5_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI14_IRQHandler
        PUBWEAK LPSPI14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI14_IRQHandler
        LDR     R0, =LPSPI14_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C15_IRQHandler
        PUBWEAK LPI2C15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C15_IRQHandler
        LDR     R0, =LPI2C15_DriverIRQHandler
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

        PUBWEAK PDM_HWVAD_EVENT_IRQHandler
        PUBWEAK PDM_HWVAD_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PDM_HWVAD_EVENT_IRQHandler
        LDR     R0, =PDM_HWVAD_EVENT_DriverIRQHandler
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

        PUBWEAK SENSE_HYPERVISOR_IRQHandler
        PUBWEAK SENSE_HYPERVISOR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SENSE_HYPERVISOR_IRQHandler
        LDR     R0, =SENSE_HYPERVISOR_DriverIRQHandler
        BX      R0

        PUBWEAK SENSE_SECURE_VIOLATION_IRQHandler
        PUBWEAK SENSE_SECURE_VIOLATION_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SENSE_SECURE_VIOLATION_IRQHandler
        LDR     R0, =SENSE_SECURE_VIOLATION_DriverIRQHandler
        BX      R0

        PUBWEAK MEDIA_HYPERVISOR_IRQHandler
        PUBWEAK MEDIA_HYPERVISOR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MEDIA_HYPERVISOR_IRQHandler
        LDR     R0, =MEDIA_HYPERVISOR_DriverIRQHandler
        BX      R0

        PUBWEAK MEDIA_SECURE_VIOLATION_IRQHandler
        PUBWEAK MEDIA_SECURE_VIOLATION_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MEDIA_SECURE_VIOLATION_IRQHandler
        LDR     R0, =MEDIA_SECURE_VIOLATION_DriverIRQHandler
        BX      R0

        PUBWEAK RTC0_ALARM_IRQHandler
        PUBWEAK RTC0_ALARM_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RTC0_ALARM_IRQHandler
        LDR     R0, =RTC0_ALARM_DriverIRQHandler
        BX      R0

        PUBWEAK RTC0_IRQHandler
        PUBWEAK RTC0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RTC0_IRQHandler
        LDR     R0, =RTC0_DriverIRQHandler
        BX      R0

        PUBWEAK HIFI4_IRQHandler
        PUBWEAK HIFI4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
HIFI4_IRQHandler
        LDR     R0, =HIFI4_DriverIRQHandler
        BX      R0

        PUBWEAK MU0_A_IRQHandler
        PUBWEAK MU0_A_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MU0_A_IRQHandler
        LDR     R0, =MU0_A_DriverIRQHandler
        BX      R0

        PUBWEAK MU1_A_IRQHandler
        PUBWEAK MU1_A_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MU1_A_IRQHandler
        LDR     R0, =MU1_A_DriverIRQHandler
        BX      R0

        PUBWEAK MU4_A_IRQHandler
        PUBWEAK MU4_A_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MU4_A_IRQHandler
        LDR     R0, =MU4_A_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER2_IRQHandler
        PUBWEAK CTIMER2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER2_IRQHandler
        LDR     R0, =CTIMER2_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER4_IRQHandler
        PUBWEAK CTIMER4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER4_IRQHandler
        LDR     R0, =CTIMER4_DriverIRQHandler
        BX      R0

        PUBWEAK OS_EVENT_IRQHandler
        PUBWEAK OS_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
OS_EVENT_IRQHandler
        LDR     R0, =OS_EVENT_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM6_IRQHandler
        PUBWEAK LP_FLEXCOMM6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM6_IRQHandler
        LDR     R0, =LP_FLEXCOMM6_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM7_IRQHandler
        PUBWEAK LP_FLEXCOMM7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM7_IRQHandler
        LDR     R0, =LP_FLEXCOMM7_DriverIRQHandler
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

        PUBWEAK I3C0_IRQHandler
        PUBWEAK I3C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I3C0_IRQHandler
        LDR     R0, =I3C0_DriverIRQHandler
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

        PUBWEAK WDT0_IRQHandler
        PUBWEAK WDT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WDT0_IRQHandler
        LDR     R0, =WDT0_DriverIRQHandler
        BX      R0

        PUBWEAK WDT1_IRQHandler
        PUBWEAK WDT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WDT1_IRQHandler
        LDR     R0, =WDT1_DriverIRQHandler
        BX      R0

        PUBWEAK USBPHY0_IRQHandler
        PUBWEAK USBPHY0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USBPHY0_IRQHandler
        LDR     R0, =USBPHY0_DriverIRQHandler
        BX      R0

        PUBWEAK PUF_IRQHandler
        PUBWEAK PUF_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PUF_IRQHandler
        LDR     R0, =PUF_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved62_IRQHandler
        PUBWEAK Reserved62_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved62_IRQHandler
        LDR     R0, =Reserved62_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM8_IRQHandler
        PUBWEAK LP_FLEXCOMM8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM8_IRQHandler
        LDR     R0, =LP_FLEXCOMM8_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM9_IRQHandler
        PUBWEAK LP_FLEXCOMM9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM9_IRQHandler
        LDR     R0, =LP_FLEXCOMM9_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM10_IRQHandler
        PUBWEAK LP_FLEXCOMM10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM10_IRQHandler
        LDR     R0, =LP_FLEXCOMM10_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM11_IRQHandler
        PUBWEAK LP_FLEXCOMM11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM11_IRQHandler
        LDR     R0, =LP_FLEXCOMM11_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM12_IRQHandler
        PUBWEAK LP_FLEXCOMM12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM12_IRQHandler
        LDR     R0, =LP_FLEXCOMM12_DriverIRQHandler
        BX      R0

        PUBWEAK LP_FLEXCOMM13_IRQHandler
        PUBWEAK LP_FLEXCOMM13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LP_FLEXCOMM13_IRQHandler
        LDR     R0, =LP_FLEXCOMM13_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI16_IRQHandler
        PUBWEAK LPSPI16_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI16_IRQHandler
        LDR     R0, =LPSPI16_DriverIRQHandler
        BX      R0

        PUBWEAK I3C1_IRQHandler
        PUBWEAK I3C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I3C1_IRQHandler
        LDR     R0, =I3C1_DriverIRQHandler
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

        PUBWEAK EDMA0_CH0_IRQHandler
        PUBWEAK EDMA0_CH0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH0_IRQHandler
        LDR     R0, =EDMA0_CH0_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH1_IRQHandler
        PUBWEAK EDMA0_CH1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH1_IRQHandler
        LDR     R0, =EDMA0_CH1_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH2_IRQHandler
        PUBWEAK EDMA0_CH2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH2_IRQHandler
        LDR     R0, =EDMA0_CH2_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH3_IRQHandler
        PUBWEAK EDMA0_CH3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH3_IRQHandler
        LDR     R0, =EDMA0_CH3_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH4_IRQHandler
        PUBWEAK EDMA0_CH4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH4_IRQHandler
        LDR     R0, =EDMA0_CH4_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH5_IRQHandler
        PUBWEAK EDMA0_CH5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH5_IRQHandler
        LDR     R0, =EDMA0_CH5_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH6_IRQHandler
        PUBWEAK EDMA0_CH6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH6_IRQHandler
        LDR     R0, =EDMA0_CH6_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH7_IRQHandler
        PUBWEAK EDMA0_CH7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH7_IRQHandler
        LDR     R0, =EDMA0_CH7_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH8_IRQHandler
        PUBWEAK EDMA0_CH8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH8_IRQHandler
        LDR     R0, =EDMA0_CH8_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH9_IRQHandler
        PUBWEAK EDMA0_CH9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH9_IRQHandler
        LDR     R0, =EDMA0_CH9_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH10_IRQHandler
        PUBWEAK EDMA0_CH10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH10_IRQHandler
        LDR     R0, =EDMA0_CH10_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH11_IRQHandler
        PUBWEAK EDMA0_CH11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH11_IRQHandler
        LDR     R0, =EDMA0_CH11_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH12_IRQHandler
        PUBWEAK EDMA0_CH12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH12_IRQHandler
        LDR     R0, =EDMA0_CH12_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH13_IRQHandler
        PUBWEAK EDMA0_CH13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH13_IRQHandler
        LDR     R0, =EDMA0_CH13_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH14_IRQHandler
        PUBWEAK EDMA0_CH14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH14_IRQHandler
        LDR     R0, =EDMA0_CH14_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA0_CH15_IRQHandler
        PUBWEAK EDMA0_CH15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA0_CH15_IRQHandler
        LDR     R0, =EDMA0_CH15_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH0_IRQHandler
        PUBWEAK EDMA1_CH0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH0_IRQHandler
        LDR     R0, =EDMA1_CH0_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH1_IRQHandler
        PUBWEAK EDMA1_CH1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH1_IRQHandler
        LDR     R0, =EDMA1_CH1_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH2_IRQHandler
        PUBWEAK EDMA1_CH2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH2_IRQHandler
        LDR     R0, =EDMA1_CH2_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH3_IRQHandler
        PUBWEAK EDMA1_CH3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH3_IRQHandler
        LDR     R0, =EDMA1_CH3_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH4_IRQHandler
        PUBWEAK EDMA1_CH4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH4_IRQHandler
        LDR     R0, =EDMA1_CH4_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH5_IRQHandler
        PUBWEAK EDMA1_CH5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH5_IRQHandler
        LDR     R0, =EDMA1_CH5_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH6_IRQHandler
        PUBWEAK EDMA1_CH6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH6_IRQHandler
        LDR     R0, =EDMA1_CH6_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH7_IRQHandler
        PUBWEAK EDMA1_CH7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH7_IRQHandler
        LDR     R0, =EDMA1_CH7_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH8_IRQHandler
        PUBWEAK EDMA1_CH8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH8_IRQHandler
        LDR     R0, =EDMA1_CH8_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH9_IRQHandler
        PUBWEAK EDMA1_CH9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH9_IRQHandler
        LDR     R0, =EDMA1_CH9_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH10_IRQHandler
        PUBWEAK EDMA1_CH10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH10_IRQHandler
        LDR     R0, =EDMA1_CH10_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH11_IRQHandler
        PUBWEAK EDMA1_CH11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH11_IRQHandler
        LDR     R0, =EDMA1_CH11_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH12_IRQHandler
        PUBWEAK EDMA1_CH12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH12_IRQHandler
        LDR     R0, =EDMA1_CH12_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH13_IRQHandler
        PUBWEAK EDMA1_CH13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH13_IRQHandler
        LDR     R0, =EDMA1_CH13_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH14_IRQHandler
        PUBWEAK EDMA1_CH14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH14_IRQHandler
        LDR     R0, =EDMA1_CH14_DriverIRQHandler
        BX      R0

        PUBWEAK EDMA1_CH15_IRQHandler
        PUBWEAK EDMA1_CH15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EDMA1_CH15_IRQHandler
        LDR     R0, =EDMA1_CH15_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO00_IRQHandler
        PUBWEAK GPIO00_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO00_IRQHandler
        LDR     R0, =GPIO00_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO01_IRQHandler
        PUBWEAK GPIO01_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO01_IRQHandler
        LDR     R0, =GPIO01_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO10_IRQHandler
        PUBWEAK GPIO10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO10_IRQHandler
        LDR     R0, =GPIO10_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO11_IRQHandler
        PUBWEAK GPIO11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO11_IRQHandler
        LDR     R0, =GPIO11_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO20_IRQHandler
        PUBWEAK GPIO20_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO20_IRQHandler
        LDR     R0, =GPIO20_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO21_IRQHandler
        PUBWEAK GPIO21_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO21_IRQHandler
        LDR     R0, =GPIO21_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO30_IRQHandler
        PUBWEAK GPIO30_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO30_IRQHandler
        LDR     R0, =GPIO30_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO31_IRQHandler
        PUBWEAK GPIO31_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO31_IRQHandler
        LDR     R0, =GPIO31_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO40_IRQHandler
        PUBWEAK GPIO40_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO40_IRQHandler
        LDR     R0, =GPIO40_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO41_IRQHandler
        PUBWEAK GPIO41_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO41_IRQHandler
        LDR     R0, =GPIO41_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO50_IRQHandler
        PUBWEAK GPIO50_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO50_IRQHandler
        LDR     R0, =GPIO50_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO51_IRQHandler
        PUBWEAK GPIO51_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO51_IRQHandler
        LDR     R0, =GPIO51_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO60_IRQHandler
        PUBWEAK GPIO60_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO60_IRQHandler
        LDR     R0, =GPIO60_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO61_IRQHandler
        PUBWEAK GPIO61_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO61_IRQHandler
        LDR     R0, =GPIO61_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO70_IRQHandler
        PUBWEAK GPIO70_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO70_IRQHandler
        LDR     R0, =GPIO70_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO71_IRQHandler
        PUBWEAK GPIO71_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO71_IRQHandler
        LDR     R0, =GPIO71_DriverIRQHandler
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

        PUBWEAK PIN_INT4_IRQHandler
        PUBWEAK PIN_INT4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT4_IRQHandler
        LDR     R0, =PIN_INT4_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT5_IRQHandler
        PUBWEAK PIN_INT5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT5_IRQHandler
        LDR     R0, =PIN_INT5_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT6_IRQHandler
        PUBWEAK PIN_INT6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT6_IRQHandler
        LDR     R0, =PIN_INT6_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT7_IRQHandler
        PUBWEAK PIN_INT7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT7_IRQHandler
        LDR     R0, =PIN_INT7_DriverIRQHandler
        BX      R0

        PUBWEAK SAI0_IRQHandler
        PUBWEAK SAI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SAI0_IRQHandler
        LDR     R0, =SAI0_DriverIRQHandler
        BX      R0

        PUBWEAK SAI1_IRQHandler
        PUBWEAK SAI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SAI1_IRQHandler
        LDR     R0, =SAI1_DriverIRQHandler
        BX      R0

        PUBWEAK SAI2_IRQHandler
        PUBWEAK SAI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SAI2_IRQHandler
        LDR     R0, =SAI2_DriverIRQHandler
        BX      R0

        PUBWEAK XSPI0_IRQHandler
        PUBWEAK XSPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
XSPI0_IRQHandler
        LDR     R0, =XSPI0_DriverIRQHandler
        BX      R0

        PUBWEAK XSPI1_IRQHandler
        PUBWEAK XSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
XSPI1_IRQHandler
        LDR     R0, =XSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK XSPI2_IRQHandler
        PUBWEAK XSPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
XSPI2_IRQHandler
        LDR     R0, =XSPI2_DriverIRQHandler
        BX      R0

        PUBWEAK MMU0_IRQHandler
        PUBWEAK MMU0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MMU0_IRQHandler
        LDR     R0, =MMU0_DriverIRQHandler
        BX      R0

        PUBWEAK MMU1_IRQHandler
        PUBWEAK MMU1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MMU1_IRQHandler
        LDR     R0, =MMU1_DriverIRQHandler
        BX      R0

        PUBWEAK MMU2_IRQHandler
        PUBWEAK MMU2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MMU2_IRQHandler
        LDR     R0, =MMU2_DriverIRQHandler
        BX      R0

        PUBWEAK Freqme_IRQHandler
        PUBWEAK Freqme_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Freqme_IRQHandler
        LDR     R0, =Freqme_DriverIRQHandler
        BX      R0

        PUBWEAK GDET0_IRQHandler
        PUBWEAK GDET0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GDET0_IRQHandler
        LDR     R0, =GDET0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved142_IRQHandler
        PUBWEAK Reserved142_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved142_IRQHandler
        LDR     R0, =Reserved142_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved143_IRQHandler
        PUBWEAK Reserved143_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved143_IRQHandler
        LDR     R0, =Reserved143_DriverIRQHandler
        BX      R0

        PUBWEAK GDET3_IRQHandler
        PUBWEAK GDET3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GDET3_IRQHandler
        LDR     R0, =GDET3_DriverIRQHandler
        BX      R0

        PUBWEAK CDOG0_IRQHandler
        PUBWEAK CDOG0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CDOG0_IRQHandler
        LDR     R0, =CDOG0_DriverIRQHandler
        BX      R0

        PUBWEAK CDOG1_IRQHandler
        PUBWEAK CDOG1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CDOG1_IRQHandler
        LDR     R0, =CDOG1_DriverIRQHandler
        BX      R0

        PUBWEAK CDOG2_IRQHandler
        PUBWEAK CDOG2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CDOG2_IRQHandler
        LDR     R0, =CDOG2_DriverIRQHandler
        BX      R0

        PUBWEAK ITRC0_IRQHandler
        PUBWEAK ITRC0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ITRC0_IRQHandler
        LDR     R0, =ITRC0_DriverIRQHandler
        BX      R0

        PUBWEAK ELS_IRQHandler
        PUBWEAK ELS_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ELS_IRQHandler
        LDR     R0, =ELS_DriverIRQHandler
        BX      R0

        PUBWEAK PKC_IRQHandler
        PUBWEAK PKC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PKC_IRQHandler
        LDR     R0, =PKC_DriverIRQHandler
        BX      R0

        PUBWEAK OSCCA_IRQHandler
        PUBWEAK OSCCA_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
OSCCA_IRQHandler
        LDR     R0, =OSCCA_DriverIRQHandler
        BX      R0

        PUBWEAK SLEEPCON0_IRQHandler
        PUBWEAK SLEEPCON0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SLEEPCON0_IRQHandler
        LDR     R0, =SLEEPCON0_DriverIRQHandler
        BX      R0

        PUBWEAK CPU0_IRQHandler
        PUBWEAK CPU0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CPU0_IRQHandler
        LDR     R0, =CPU0_DriverIRQHandler
        BX      R0

        PUBWEAK PMC_IRQHandler
        PUBWEAK PMC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PMC_IRQHandler
        LDR     R0, =PMC_DriverIRQHandler
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

        PUBWEAK NPU_IRQHandler
        PUBWEAK NPU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
NPU_IRQHandler
        LDR     R0, =NPU_DriverIRQHandler
        BX      R0

        PUBWEAK TRNG_IRQHandler
        PUBWEAK TRNG_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
TRNG_IRQHandler
        LDR     R0, =TRNG_DriverIRQHandler
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

        PUBWEAK GLIKEY0_IRQHandler
        PUBWEAK GLIKEY0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GLIKEY0_IRQHandler
        LDR     R0, =GLIKEY0_DriverIRQHandler
        BX      R0

        PUBWEAK GLIKEY2_IRQHandler
        PUBWEAK GLIKEY2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GLIKEY2_IRQHandler
        LDR     R0, =GLIKEY2_DriverIRQHandler
        BX      R0

        PUBWEAK GLIKEY3_IRQHandler
        PUBWEAK GLIKEY3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GLIKEY3_IRQHandler
        LDR     R0, =GLIKEY3_DriverIRQHandler
        BX      R0

        PUBWEAK GLIKEY5_IRQHandler
        PUBWEAK GLIKEY5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GLIKEY5_IRQHandler
        LDR     R0, =GLIKEY5_DriverIRQHandler
        BX      R0

        PUBWEAK PVTS0_CPU0_IRQHandler
        PUBWEAK PVTS0_CPU0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PVTS0_CPU0_IRQHandler
        LDR     R0, =PVTS0_CPU0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved167_IRQHandler
        PUBWEAK Reserved167_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved167_IRQHandler
        LDR     R0, =Reserved167_DriverIRQHandler
        BX      R0

        PUBWEAK PVTS0_HIFI4_IRQHandler
        PUBWEAK PVTS0_HIFI4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PVTS0_HIFI4_IRQHandler
        LDR     R0, =PVTS0_HIFI4_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved169_IRQHandler
        PUBWEAK Reserved169_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved169_IRQHandler
        LDR     R0, =Reserved169_DriverIRQHandler
        BX      R0

        PUBWEAK FRO0_IRQHandler
        PUBWEAK FRO0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FRO0_IRQHandler
        LDR     R0, =FRO0_DriverIRQHandler
        BX      R0

        PUBWEAK FRO1_IRQHandler
        PUBWEAK FRO1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FRO1_IRQHandler
        LDR     R0, =FRO1_DriverIRQHandler
        BX      R0

        PUBWEAK FRO2_IRQHandler
        PUBWEAK FRO2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FRO2_IRQHandler
        LDR     R0, =FRO2_DriverIRQHandler
        BX      R0

        PUBWEAK OCOTP_IRQHandler
        PUBWEAK OCOTP_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
OCOTP_IRQHandler
        LDR     R0, =OCOTP_DriverIRQHandler
        BX      R0

Reserved16_DriverIRQHandler
UTICK0_DriverIRQHandler
MRT0_DriverIRQHandler
CTIMER0_DriverIRQHandler
CTIMER1_DriverIRQHandler
SCT0_DriverIRQHandler
CTIMER3_DriverIRQHandler
LP_FLEXCOMM0_DriverIRQHandler
LP_FLEXCOMM1_DriverIRQHandler
LP_FLEXCOMM2_DriverIRQHandler
LP_FLEXCOMM3_DriverIRQHandler
LP_FLEXCOMM4_DriverIRQHandler
LP_FLEXCOMM5_DriverIRQHandler
LPSPI14_DriverIRQHandler
LPI2C15_DriverIRQHandler
ADC_DriverIRQHandler
SDADC_DriverIRQHandler
ACMP_DriverIRQHandler
PDM_EVENT_DriverIRQHandler
PDM_HWVAD_EVENT_DriverIRQHandler
HYPERVISOR_DriverIRQHandler
SECURE_VIOLATION_DriverIRQHandler
SENSE_HYPERVISOR_DriverIRQHandler
SENSE_SECURE_VIOLATION_DriverIRQHandler
MEDIA_HYPERVISOR_DriverIRQHandler
MEDIA_SECURE_VIOLATION_DriverIRQHandler
RTC0_ALARM_DriverIRQHandler
RTC0_DriverIRQHandler
HIFI4_DriverIRQHandler
MU0_A_DriverIRQHandler
MU1_A_DriverIRQHandler
MU4_A_DriverIRQHandler
CTIMER2_DriverIRQHandler
CTIMER4_DriverIRQHandler
OS_EVENT_DriverIRQHandler
LP_FLEXCOMM6_DriverIRQHandler
LP_FLEXCOMM7_DriverIRQHandler
USDHC0_DriverIRQHandler
USDHC1_DriverIRQHandler
I3C0_DriverIRQHandler
USB0_DriverIRQHandler
USB1_DriverIRQHandler
WDT0_DriverIRQHandler
WDT1_DriverIRQHandler
USBPHY0_DriverIRQHandler
PUF_DriverIRQHandler
Reserved62_DriverIRQHandler
LP_FLEXCOMM8_DriverIRQHandler
LP_FLEXCOMM9_DriverIRQHandler
LP_FLEXCOMM10_DriverIRQHandler
LP_FLEXCOMM11_DriverIRQHandler
LP_FLEXCOMM12_DriverIRQHandler
LP_FLEXCOMM13_DriverIRQHandler
LPSPI16_DriverIRQHandler
I3C1_DriverIRQHandler
FLEXIO_DriverIRQHandler
LCDIF_DriverIRQHandler
VGPU_DriverIRQHandler
MIPI_DriverIRQHandler
EDMA0_CH0_DriverIRQHandler
EDMA0_CH1_DriverIRQHandler
EDMA0_CH2_DriverIRQHandler
EDMA0_CH3_DriverIRQHandler
EDMA0_CH4_DriverIRQHandler
EDMA0_CH5_DriverIRQHandler
EDMA0_CH6_DriverIRQHandler
EDMA0_CH7_DriverIRQHandler
EDMA0_CH8_DriverIRQHandler
EDMA0_CH9_DriverIRQHandler
EDMA0_CH10_DriverIRQHandler
EDMA0_CH11_DriverIRQHandler
EDMA0_CH12_DriverIRQHandler
EDMA0_CH13_DriverIRQHandler
EDMA0_CH14_DriverIRQHandler
EDMA0_CH15_DriverIRQHandler
EDMA1_CH0_DriverIRQHandler
EDMA1_CH1_DriverIRQHandler
EDMA1_CH2_DriverIRQHandler
EDMA1_CH3_DriverIRQHandler
EDMA1_CH4_DriverIRQHandler
EDMA1_CH5_DriverIRQHandler
EDMA1_CH6_DriverIRQHandler
EDMA1_CH7_DriverIRQHandler
EDMA1_CH8_DriverIRQHandler
EDMA1_CH9_DriverIRQHandler
EDMA1_CH10_DriverIRQHandler
EDMA1_CH11_DriverIRQHandler
EDMA1_CH12_DriverIRQHandler
EDMA1_CH13_DriverIRQHandler
EDMA1_CH14_DriverIRQHandler
EDMA1_CH15_DriverIRQHandler
GPIO00_DriverIRQHandler
GPIO01_DriverIRQHandler
GPIO10_DriverIRQHandler
GPIO11_DriverIRQHandler
GPIO20_DriverIRQHandler
GPIO21_DriverIRQHandler
GPIO30_DriverIRQHandler
GPIO31_DriverIRQHandler
GPIO40_DriverIRQHandler
GPIO41_DriverIRQHandler
GPIO50_DriverIRQHandler
GPIO51_DriverIRQHandler
GPIO60_DriverIRQHandler
GPIO61_DriverIRQHandler
GPIO70_DriverIRQHandler
GPIO71_DriverIRQHandler
PIN_INT0_DriverIRQHandler
PIN_INT1_DriverIRQHandler
PIN_INT2_DriverIRQHandler
PIN_INT3_DriverIRQHandler
PIN_INT4_DriverIRQHandler
PIN_INT5_DriverIRQHandler
PIN_INT6_DriverIRQHandler
PIN_INT7_DriverIRQHandler
SAI0_DriverIRQHandler
SAI1_DriverIRQHandler
SAI2_DriverIRQHandler
XSPI0_DriverIRQHandler
XSPI1_DriverIRQHandler
XSPI2_DriverIRQHandler
MMU0_DriverIRQHandler
MMU1_DriverIRQHandler
MMU2_DriverIRQHandler
Freqme_DriverIRQHandler
GDET0_DriverIRQHandler
Reserved142_DriverIRQHandler
Reserved143_DriverIRQHandler
GDET3_DriverIRQHandler
CDOG0_DriverIRQHandler
CDOG1_DriverIRQHandler
CDOG2_DriverIRQHandler
ITRC0_DriverIRQHandler
ELS_DriverIRQHandler
PKC_DriverIRQHandler
OSCCA_DriverIRQHandler
SLEEPCON0_DriverIRQHandler
CPU0_DriverIRQHandler
PMC_DriverIRQHandler
JPEGDEC_DriverIRQHandler
PNGDEC_DriverIRQHandler
NPU_DriverIRQHandler
TRNG_DriverIRQHandler
TEMPDET0_DriverIRQHandler
TEMPDET1_DriverIRQHandler
EZHV_DriverIRQHandler
GLIKEY0_DriverIRQHandler
GLIKEY2_DriverIRQHandler
GLIKEY3_DriverIRQHandler
GLIKEY5_DriverIRQHandler
PVTS0_CPU0_DriverIRQHandler
Reserved167_DriverIRQHandler
PVTS0_HIFI4_DriverIRQHandler
Reserved169_DriverIRQHandler
FRO0_DriverIRQHandler
FRO1_DriverIRQHandler
FRO2_DriverIRQHandler
OCOTP_DriverIRQHandler
DefaultISR
        B .

        END
