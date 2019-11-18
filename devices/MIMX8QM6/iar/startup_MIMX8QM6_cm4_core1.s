; -------------------------------------------------------------------------
;  @file:    startup_MIMX8QM6_cm4_core1.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MIMX8QM6_cm4_core1
;  @version: 4.0
;  @date:    2018-8-30
;  @build:   b180831
; -------------------------------------------------------------------------
;
; Copyright 1997-2016 Freescale Semiconductor, Inc.
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
        SECTION HEAP:DATA:NOROOT(3)
        SECTION RW:DATA:NOROOT(2)

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
        DCD     MemManage_Handler                             ;MPU Fault Handler
        DCD     BusFault_Handler                              ;Bus Fault Handler
        DCD     UsageFault_Handler                            ;Usage Fault Handler
__vector_table_0x1c
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     DebugMon_Handler                              ;Debug Monitor Handler
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     Reserved16_IRQHandler                         ;Reserved
        DCD     Reserved17_IRQHandler                         ;Reserved
        DCD     Reserved18_IRQHandler                         ;Reserved
        DCD     Reserved19_IRQHandler                         ;Reserved
        DCD     Reserved20_IRQHandler                         ;Reserved
        DCD     M4_1_MCM_IRQHandler                           ;MCM IRQ
        DCD     Reserved22_IRQHandler                         ;Reserved
        DCD     Reserved23_IRQHandler                         ;Reserved
        DCD     Reserved24_IRQHandler                         ;Reserved
        DCD     Reserved25_IRQHandler                         ;Reserved
        DCD     Reserved26_IRQHandler                         ;Reserved
        DCD     Reserved27_IRQHandler                         ;Reserved
        DCD     Reserved28_IRQHandler                         ;Reserved
        DCD     Reserved29_IRQHandler                         ;Reserved
        DCD     Reserved30_IRQHandler                         ;Reserved
        DCD     Reserved31_IRQHandler                         ;Reserved
        DCD     Reserved32_IRQHandler                         ;Reserved
        DCD     Reserved33_IRQHandler                         ;Reserved
        DCD     Reserved34_IRQHandler                         ;Reserved
        DCD     M4_1_TPM_IRQHandler                           ;Timer PWM Module
        DCD     Reserved36_IRQHandler                         ;Reserved
        DCD     Reserved37_IRQHandler                         ;Reserved
        DCD     M4_1_LPIT_IRQHandler                          ;Low-Power Periodic Interrupt Timer
        DCD     Reserved39_IRQHandler                         ;Reserved
        DCD     Reserved40_IRQHandler                         ;Reserved
        DCD     M4_1_LPUART_IRQHandler                        ;Low Power UART
        DCD     Reserved42_IRQHandler                         ;Reserved
        DCD     M4_1_LPI2C_IRQHandler                         ;Low-Power I2C - Logical OR of master and slave interrupts
        DCD     Reserved44_IRQHandler                         ;Reserved
        DCD     M4_1_MU0_B0_IRQHandler                        ;Messaging Unit 0 (IPC with other subsystems) - Side B (local), Port 0, Logical OR of all general-purpose, TX, and RX interrupts
        DCD     Reserved46_IRQHandler                         ;Reserved
        DCD     Reserved47_IRQHandler                         ;Reserved
        DCD     IRQSTEER_0_IRQHandler                         ;External interrupt 0
        DCD     IRQSTEER_1_IRQHandler                         ;External interrupt 1
        DCD     IRQSTEER_2_IRQHandler                         ;External interrupt 2
        DCD     IRQSTEER_3_IRQHandler                         ;External interrupt 3
        DCD     IRQSTEER_4_IRQHandler                         ;External interrupt 4
        DCD     IRQSTEER_5_IRQHandler                         ;External interrupt 5
        DCD     IRQSTEER_6_IRQHandler                         ;External interrupt 6
        DCD     IRQSTEER_7_IRQHandler                         ;External interrupt 7
        DCD     Reserved56_IRQHandler                         ;Reserved
        DCD     Reserved57_IRQHandler                         ;Reserved
        DCD     Reserved58_IRQHandler                         ;Reserved
        DCD     Reserved59_IRQHandler                         ;Reserved
        DCD     M4_1_MU0_B1_IRQHandler                        ;Messaging Unit 0 (IPC with other subsystems) - Side B (local), Port 1, Logical OR of all general-purpose, TX, and RX interrupts
        DCD     M4_1_MU0_B2_IRQHandler                        ;Messaging Unit 0 (IPC with other subsystems) - Side B (local), Port 2, Logical OR of all general-purpose, TX, and RX interrupts
        DCD     M4_1_MU0_B3_IRQHandler                        ;Messaging Unit 0 (IPC with other subsystems) - Side B (local), Port 3, Logical OR of all general-purpose, TX, and RX interrupts
        DCD     Reserved63_IRQHandler                         ;Reserved
        DCD     Reserved64_IRQHandler                         ;Reserved
        DCD     M4_1_MU1_A_IRQHandler                         ;Messaging Unit 1 (IPC with System Controller) - Side A (MCU), Logical OR of all general-purpose, TX, and RX interrupts
        DCD     M4_1_SW_IRQHandler                            ;Software interrupt (asserted/cleared via NVIC registers, INTISR[50] input tied low)
        DCD     Reserved67_IRQHandler                         ;xxx Interrupt 67
        DCD     Reserved68_IRQHandler                         ;xxx Interrupt 68
        DCD     Reserved69_IRQHandler                         ;xxx Interrupt 69
        DCD     Reserved70_IRQHandler                         ;xxx Interrupt 70
        DCD     Reserved71_IRQHandler                         ;xxx Interrupt 71
        DCD     Reserved72_IRQHandler                         ;xxx Interrupt 72
        DCD     Reserved73_IRQHandler                         ;xxx Interrupt 73
        DCD     Reserved74_IRQHandler                         ;xxx Interrupt 74
        DCD     Reserved75_IRQHandler                         ;xxx Interrupt 75
        DCD     Reserved76_IRQHandler                         ;xxx Interrupt 76
        DCD     Reserved77_IRQHandler                         ;xxx Interrupt 77
        DCD     Reserved78_IRQHandler                         ;xxx Interrupt 78
        DCD     Reserved79_IRQHandler                         ;xxx Interrupt 79
        DCD     Reserved80_IRQHandler                         ;xxx Interrupt 80
        DCD     Reserved81_IRQHandler                         ;xxx Interrupt 81
        DCD     Reserved82_IRQHandler                         ;xxx Interrupt 82
        DCD     Reserved83_IRQHandler                         ;xxx Interrupt 83
        DCD     Reserved84_IRQHandler                         ;xxx Interrupt 84
        DCD     Reserved85_IRQHandler                         ;xxx Interrupt 85
        DCD     Reserved86_IRQHandler                         ;xxx Interrupt 86
        DCD     Reserved87_IRQHandler                         ;xxx Interrupt 87
        DCD     Reserved88_IRQHandler                         ;xxx Interrupt 88
        DCD     Reserved89_IRQHandler                         ;xxx Interrupt 89
        DCD     Reserved90_IRQHandler                         ;xxx Interrupt 90
        DCD     Reserved91_IRQHandler                         ;xxx Interrupt 91
        DCD     Reserved92_IRQHandler                         ;xxx Interrupt 92
        DCD     Reserved93_IRQHandler                         ;xxx Interrupt 93
        DCD     Reserved94_IRQHandler                         ;xxx Interrupt 94
        DCD     Reserved95_IRQHandler                         ;xxx Interrupt 95
        DCD     Reserved96_IRQHandler                         ;xxx Interrupt 96
        DCD     Reserved97_IRQHandler                         ;xxx Interrupt 97
        DCD     Reserved98_IRQHandler                         ;xxx Interrupt 98
        DCD     A53_NEXTERRIRQ_IRQHandler                     ;Shared Int Source nEXTERRIRQ from A53 Sub-System
        DCD     A53_NINTERRIRQ_IRQHandler                     ;Shared Int Source nINTERRIRQ from A53 Sub-System
        DCD     A72_NEXTERRIRQ_IRQHandler                     ;Shared Int Source nEXTERRIRQ from A72 Sub-System
        DCD     A72_NINTERRIRQ_IRQHandler                     ;Shared Int Source nINTERRIRQ from A72 Sub-System
        DCD     VPU_NEXTERRIRQ_IRQHandler                     ;Shared Int Source nEXTERRIRQ from VPU Sub-System
        DCD     Reserved104_IRQHandler                        ;xxx Interrupt 104
        DCD     Reserved105_IRQHandler                        ;xxx Interrupt 105
        DCD     Reserved106_IRQHandler                        ;xxx Interrupt 106
        DCD     CCI_NERRORIRQ_IRQHandler                      ;Shared Int Source nERRORIRQ from CCI Sub-System
        DCD     CCI_NEVNTCNTOVERFLOW0_IRQHandler              ;Shared Int Source nEVNTCNTOVERFLOW[0] from CCI Sub-System
        DCD     CCI_NEVNTCNTOVERFLOW1_IRQHandler              ;Shared Int Source nEVNTCNTOVERFLOW[1] from CCI Sub-System
        DCD     CCI_NEVNTCNTOVERFLOW2_IRQHandler              ;Shared Int Source nEVNTCNTOVERFLOW[2] from CCI Sub-System
        DCD     CCI_NEVNTCNTOVERFLOW3_IRQHandler              ;Shared Int Source nEVNTCNTOVERFLOW[3] from CCI Sub-System
        DCD     CCI_NEVNTCNTOVERFLOW4_IRQHandler              ;Shared Int Source nEVNTCNTOVERFLOW[4] from CCI Sub-System
        DCD     Reserved113_IRQHandler                        ;xxx Interrupt 113
        DCD     Reserved114_IRQHandler                        ;xxx Interrupt 114
        DCD     M4_0_INT_OUT0_IRQHandler                      ;Shared Int Source INT_OUT[0] from M4_0 Sub-System
        DCD     M4_0_INT_OUT1_IRQHandler                      ;Shared Int Source INT_OUT[1] from M4_0 Sub-System
        DCD     M4_0_INT_OUT2_IRQHandler                      ;Shared Int Source INT_OUT[2] from M4_0 Sub-System
        DCD     M4_0_INT_OUT3_IRQHandler                      ;Shared Int Source INT_OUT[3] from M4_0 Sub-System
        DCD     M4_0_INT_OUT4_IRQHandler                      ;Shared Int Source INT_OUT[4] from M4_0 Sub-System
        DCD     M4_0_INT_OUT5_IRQHandler                      ;Shared Int Source INT_OUT[5] from M4_0 Sub-System
        DCD     M4_0_INT_OUT6_IRQHandler                      ;Shared Int Source INT_OUT[6] from M4_0 Sub-System
        DCD     M4_0_INT_OUT7_IRQHandler                      ;Shared Int Source INT_OUT[7] from M4_0 Sub-System
        DCD     M4_1_INT_OUT0_IRQHandler                      ;Shared Int Source INT_OUT[0] from M4_1 Sub-System
        DCD     M4_1_INT_OUT1_IRQHandler                      ;Shared Int Source INT_OUT[1] from M4_1 Sub-System
        DCD     M4_1_INT_OUT2_IRQHandler                      ;Shared Int Source INT_OUT[2] from M4_1 Sub-System
        DCD     M4_1_INT_OUT3_IRQHandler                      ;Shared Int Source INT_OUT[3] from M4_1 Sub-System
        DCD     M4_1_INT_OUT4_IRQHandler                      ;Shared Int Source INT_OUT[4] from M4_1 Sub-System
        DCD     M4_1_INT_OUT5_IRQHandler                      ;Shared Int Source INT_OUT[5] from M4_1 Sub-System
        DCD     M4_1_INT_OUT6_IRQHandler                      ;Shared Int Source INT_OUT[6] from M4_1 Sub-System
        DCD     M4_1_INT_OUT7_IRQHandler                      ;Shared Int Source INT_OUT[7] from M4_1 Sub-System
        DCD     DBLOG_COMB_IRPT_NS_IRQHandler                 ;Shared Int Source comb_irpt_ns from DBLog Sub-System
        DCD     DBLOG_COMB_IRPT_S_IRQHandler                  ;Shared Int Source comb_irpt_s from DBLog Sub-System
        DCD     DBLOG_GBL_FLT_IRPT_NS_IRQHandler              ;Shared Int Source gbl_flt_irpt_ns from DBLog Sub-System
        DCD     DBLOG_GBL_FLT_IRPT_S_IRQHandler               ;Shared Int Source gbl_flt_irpt_s from DBLog Sub-System
        DCD     DBLOG_PERF_IRPT_IMX8_0_IRQHandler             ;Shared Int Source perf_irpt_imx8_0 from DBLog Sub-System
        DCD     DBLOG_PERF_IRPT_IMX8_1_IRQHandler             ;Shared Int Source perf_irpt_imx8_1 from DBLog Sub-System
        DCD     DBLOG_PERF_IRPT_IMX8_2_IRQHandler             ;Shared Int Source perf_irpt_imx8_2 from DBLog Sub-System
        DCD     DBLOG_PERF_IRPT_IMX8_3_IRQHandler             ;Shared Int Source perf_irpt_imx8_3 from DBLog Sub-System
        DCD     DISPLAY0_INT_OUT0_IRQHandler                  ;Shared Int Source INT_OUT[0] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT1_IRQHandler                  ;Shared Int Source INT_OUT[1] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT2_IRQHandler                  ;Shared Int Source INT_OUT[2] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT3_IRQHandler                  ;Shared Int Source INT_OUT[3] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT4_IRQHandler                  ;Shared Int Source INT_OUT[4] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT5_IRQHandler                  ;Shared Int Source INT_OUT[5] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT6_IRQHandler                  ;Shared Int Source INT_OUT[6] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT7_IRQHandler                  ;Shared Int Source INT_OUT[7] from Display0 Sub-System
        DCD     DISPLAY0_RESERVED_IRQHandler                  ;Shared Int Source Reserved from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT9_IRQHandler                  ;Shared Int Source INT_OUT[9] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT10_IRQHandler                 ;Shared Int Source INT_OUT[10] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT11_IRQHandler                 ;Shared Int Source INT_OUT[11] from Display0 Sub-System
        DCD     DISPLAY0_INT_OUT12_IRQHandler                 ;Shared Int Source INT_OUT[12] from Display0 Sub-System
        DCD     Reserved152_IRQHandler                        ;xxx Interrupt 152
        DCD     Reserved153_IRQHandler                        ;xxx Interrupt 153
        DCD     Reserved154_IRQHandler                        ;xxx Interrupt 154
        DCD     Reserved155_IRQHandler                        ;xxx Interrupt 155
        DCD     LVDS0_INT_OUT_IRQHandler                      ;Shared Int Source INT_OUT from LVDS0 Sub-System
        DCD     LVDS1_INT_OUT_IRQHandler                      ;Shared Int Source INT_OUT from LVDS1 Sub-System
        DCD     MIPI_DSI0_INT_OUT_IRQHandler                  ;Shared Int Source INT_OUT from MIPI_DSI0 Sub-System
        DCD     MIPI_DSI1_INT_OUT_IRQHandler                  ;Shared Int Source INT_OUT from MIPI_DSI1 Sub-System
        DCD     HDMI_TX_INT_OUT_IRQHandler                    ;Shared Int Source INT_OUT from HDMI_TX Sub-System
        DCD     Reserved161_IRQHandler                        ;xxx Interrupt 161
        DCD     Reserved162_IRQHandler                        ;xxx Interrupt 162
        DCD     GPU0_XAQ2_INTR_IRQHandler                     ;Shared Int Source xaq2_intr from GPU0 Sub-System
        DCD     GPU1_XAQ2_INTR_IRQHandler                     ;Shared Int Source xaq2_intr from GPU1 Sub-System
        DCD     DMA_EDMA0_INT_IRQHandler                      ;Shared Int Source eDMA0_INT from DMA Sub-System
        DCD     DMA_EDMA0_ERR_INT_IRQHandler                  ;Shared Int Source eDMA0_ERR_INT from DMA Sub-System
        DCD     DMA_EDMA1_INT_IRQHandler                      ;Shared Int Source eDMA1_INT from DMA Sub-System
        DCD     DMA_EDMA1_ERR_INT_IRQHandler                  ;Shared Int Source eDMA1_ERR_INT from DMA Sub-System
        DCD     HSIO_PCIEA_MSI_CTRL_INT_IRQHandler            ;Shared Int Source PCIeA_MSI_CTRL_INT from HSIO Sub-System
        DCD     HSIO_PCIEA_CLK_REQ_INT_IRQHandler             ;Shared Int Source PCIeA_CLK_REQ_INT from HSIO Sub-System
        DCD     HSIO_PCIEA_DMA_INT_IRQHandler                 ;Shared Int Source PCIeA_DMA_INT from HSIO Sub-System
        DCD     HSIO_PCIEA_INT_D_IRQHandler                   ;Shared Int Source PCIeA_INT_D from HSIO Sub-System
        DCD     HSIO_PCIEA_INT_C_IRQHandler                   ;Shared Int Source PCIeA_INT_C from HSIO Sub-System
        DCD     HSIO_PCIEA_INT_B_IRQHandler                   ;Shared Int Source PCIeA_INT_B from HSIO Sub-System
        DCD     HSIO_PCIEA_INT_A_IRQHandler                   ;Shared Int Source PCIeA_INT_A from HSIO Sub-System
        DCD     HSIO_PCIEA_SMLH_REQ_RST_IRQHandler            ;Shared Int Source PCIeA_SMLH_REQ_RST from HSIO Sub-System
        DCD     HSIO_PCIEA_GPIO_WAKEUP0_IRQHandler            ;Shared Int Source PCIeA_GPIO_WAKEUP[0] from HSIO Sub-System
        DCD     HSIO_PCIEA_GPIO_WAKEUP1_IRQHandler            ;Shared Int Source PCIeA_GPIO_WAKEUP[1] from HSIO Sub-System
        DCD     LSIO_GPT0_INT_IRQHandler                      ;Shared Int Source GPT0_INT from LSIO Sub-System
        DCD     LSIO_GPT1_INT_IRQHandler                      ;Shared Int Source GPT1_INT from LSIO Sub-System
        DCD     LSIO_GPT2_INT_IRQHandler                      ;Shared Int Source GPT2_INT from LSIO Sub-System
        DCD     LSIO_GPT3_INT_IRQHandler                      ;Shared Int Source GPT3_INT from LSIO Sub-System
        DCD     LSIO_GPT4_INT_IRQHandler                      ;Shared Int Source GPT4_INT from LSIO Sub-System
        DCD     LSIO_KPP_INT_IRQHandler                       ;Shared Int Source KPP_INT from LSIO Sub-System
        DCD     Reserved185_IRQHandler                        ;xxx Interrupt 185
        DCD     Reserved186_IRQHandler                        ;xxx Interrupt 186
        DCD     HSIO_SATA_INT0_IRQHandler                     ;Shared Int Source SATA_INT[0] from HSIO Sub-System
        DCD     HSIO_SATA_INT2_IRQHandler                     ;Shared Int Source SATA_INT[2] from HSIO Sub-System
        DCD     Reserved189_IRQHandler                        ;xxx Interrupt 189
        DCD     Reserved190_IRQHandler                        ;xxx Interrupt 190
        DCD     LSIO_OCTASPI0_INT_IRQHandler                  ;Shared Int Source OctaSPI0_INT from LSIO Sub-System
        DCD     LSIO_OCTASPI1_INT_IRQHandler                  ;Shared Int Source OctaSPI1_INT from LSIO Sub-System
        DCD     LSIO_PWM0_INT_IRQHandler                      ;Shared Int Source PWM0_INT from LSIO Sub-System
        DCD     LSIO_PWM1_INT_IRQHandler                      ;Shared Int Source PWM1_INT from LSIO Sub-System
        DCD     LSIO_PWM2_INT_IRQHandler                      ;Shared Int Source PWM2_INT from LSIO Sub-System
        DCD     LSIO_PWM3_INT_IRQHandler                      ;Shared Int Source PWM3_INT from LSIO Sub-System
        DCD     LSIO_PWM4_INT_IRQHandler                      ;Shared Int Source PWM4_INT from LSIO Sub-System
        DCD     LSIO_PWM5_INT_IRQHandler                      ;Shared Int Source PWM5_INT from LSIO Sub-System
        DCD     LSIO_PWM6_INT_IRQHandler                      ;Shared Int Source PWM6_INT from LSIO Sub-System
        DCD     LSIO_PWM7_INT_IRQHandler                      ;Shared Int Source PWM7_INT from LSIO Sub-System
        DCD     HSIO_PCIEB_MSI_CTRL_INT_IRQHandler            ;Shared Int Source PCIeB_MSI_CTRL_INT from HSIO Sub-System
        DCD     HSIO_PCIEB_CLK_REQ_INT_IRQHandler             ;Shared Int Source PCIeB_CLK_REQ_INT from HSIO Sub-System
        DCD     HSIO_PCIEB_DMA_INT_IRQHandler                 ;Shared Int Source PCIeB_DMA_INT from HSIO Sub-System
        DCD     HSIO_PCIEB_INT_D_IRQHandler                   ;Shared Int Source PCIeB_INT_D from HSIO Sub-System
        DCD     HSIO_PCIEB_INT_C_IRQHandler                   ;Shared Int Source PCIeB_INT_C from HSIO Sub-System
        DCD     HSIO_PCIEB_INT_B_IRQHandler                   ;Shared Int Source PCIeB_INT_B from HSIO Sub-System
        DCD     HSIO_PCIEB_INT_A_IRQHandler                   ;Shared Int Source PCIeB_INT_A from HSIO Sub-System
        DCD     HSIO_PCIEB_SMLH_REQ_RST_IRQHandler            ;Shared Int Source PCIeB_SMLH_REQ_RST from HSIO Sub-System
        DCD     HSIO_PCIEB_GPIO_WAKEUP0_IRQHandler            ;Shared Int Source PCIeB_GPIO_WAKEUP[0] from HSIO Sub-System
        DCD     HSIO_PCIEB_GPIO_WAKEUP1_IRQHandler            ;Shared Int Source PCIeB_GPIO_WAKEUP[1] from HSIO Sub-System
        DCD     SCU_INT_OUT0_IRQHandler                       ;Shared Int Source INT_OUT[0] from SCU Sub-System
        DCD     SCU_INT_OUT1_IRQHandler                       ;Shared Int Source INT_OUT[1] from SCU Sub-System
        DCD     SCU_INT_OUT2_IRQHandler                       ;Shared Int Source INT_OUT[2] from SCU Sub-System
        DCD     SCU_INT_OUT3_IRQHandler                       ;Shared Int Source INT_OUT[3] from SCU Sub-System
        DCD     SCU_INT_OUT4_IRQHandler                       ;Shared Int Source INT_OUT[4] from SCU Sub-System
        DCD     SCU_INT_OUT5_IRQHandler                       ;Shared Int Source INT_OUT[5] from SCU Sub-System
        DCD     SCU_INT_OUT6_IRQHandler                       ;Shared Int Source INT_OUT[6] from SCU Sub-System
        DCD     SCU_INT_OUT7_IRQHandler                       ;Shared Int Source INT_OUT[7] from SCU Sub-System
        DCD     SCU_SYS_COUNT_INT0_IRQHandler                 ;Shared Int Source SYS_COUNT_INT[0] from SCU Sub-System
        DCD     SCU_SYS_COUNT_INT1_IRQHandler                 ;Shared Int Source SYS_COUNT_INT[1] from SCU Sub-System
        DCD     SCU_SYS_COUNT_INT2_IRQHandler                 ;Shared Int Source SYS_COUNT_INT[2] from SCU Sub-System
        DCD     SCU_SYS_COUNT_INT3_IRQHandler                 ;Shared Int Source SYS_COUNT_INT[3] from SCU Sub-System
        DCD     Reserved223_IRQHandler                        ;xxx Interrupt 223
        DCD     Reserved224_IRQHandler                        ;xxx Interrupt 224
        DCD     Reserved225_IRQHandler                        ;xxx Interrupt 225
        DCD     Reserved226_IRQHandler                        ;xxx Interrupt 226
        DCD     DRC0_DFI_ALERT_ERR_IRQHandler                 ;Shared Int Source DFI_ALERT_ERR from DRC0 Sub-System
        DCD     DRC1_DFI_ALERT_ERR_IRQHandler                 ;Shared Int Source DFI_ALERT_ERR from DRC1 Sub-System
        DCD     DRC0_PERF_CNT_FULL_IRQHandler                 ;Shared Int Source PERF_CNT_FULL from DRC0 Sub-System
        DCD     DRC1_PERF_CNT_FULL_IRQHandler                 ;Shared Int Source PERF_CNT_FULL from DRC1 Sub-System
        DCD     Reserved231_IRQHandler                        ;xxx Interrupt 231
        DCD     Reserved232_IRQHandler                        ;xxx Interrupt 232
        DCD     Reserved233_IRQHandler                        ;xxx Interrupt 233
        DCD     Reserved234_IRQHandler                        ;xxx Interrupt 234
        DCD     LSIO_GPIO_INT0_IRQHandler                     ;Shared Int Source GPIO_INT[0] from LSIO Sub-System
        DCD     LSIO_GPIO_INT1_IRQHandler                     ;Shared Int Source GPIO_INT[1] from LSIO Sub-System
        DCD     LSIO_GPIO_INT2_IRQHandler                     ;Shared Int Source GPIO_INT[2] from LSIO Sub-System
        DCD     LSIO_GPIO_INT3_IRQHandler                     ;Shared Int Source GPIO_INT[3] from LSIO Sub-System
        DCD     LSIO_GPIO_INT4_IRQHandler                     ;Shared Int Source GPIO_INT[4] from LSIO Sub-System
        DCD     LSIO_GPIO_INT5_IRQHandler                     ;Shared Int Source GPIO_INT[5] from LSIO Sub-System
        DCD     LSIO_GPIO_INT6_IRQHandler                     ;Shared Int Source GPIO_INT[6] from LSIO Sub-System
        DCD     LSIO_GPIO_INT7_IRQHandler                     ;Shared Int Source GPIO_INT[7] from LSIO Sub-System
        DCD     Reserved243_IRQHandler                        ;xxx Interrupt 243
        DCD     Reserved244_IRQHandler                        ;xxx Interrupt 244
        DCD     Reserved245_IRQHandler                        ;xxx Interrupt 245
        DCD     Reserved246_IRQHandler                        ;xxx Interrupt 246
        DCD     Reserved247_IRQHandler                        ;xxx Interrupt 247
        DCD     Reserved248_IRQHandler                        ;xxx Interrupt 248
        DCD     Reserved249_IRQHandler                        ;xxx Interrupt 249
        DCD     Reserved250_IRQHandler                        ;xxx Interrupt 250
        DCD     DISPLAY1_INT_OUT0_IRQHandler                  ;Shared Int Source INT_OUT[0] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT1_IRQHandler                  ;Shared Int Source INT_OUT[1] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT2_IRQHandler                  ;Shared Int Source INT_OUT[2] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT3_IRQHandler                  ;Shared Int Source INT_OUT[3] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT4_IRQHandler                  ;Shared Int Source INT_OUT[4] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT5_IRQHandler                  ;Shared Int Source INT_OUT[5] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT6_IRQHandler                  ;Shared Int Source INT_OUT[6] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT7_IRQHandler                  ;Shared Int Source INT_OUT[7] from Display1 Sub-System
        DCD     DISPLAY1_RESERVED_IRQHandler                  ;Shared Int Source Reserved from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT9_IRQHandler                  ;Shared Int Source INT_OUT[9] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT10_IRQHandler                 ;Shared Int Source INT_OUT[10] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT11_IRQHandler                 ;Shared Int Source INT_OUT[11] from Display1 Sub-System
        DCD     DISPLAY1_INT_OUT12_IRQHandler                 ;Shared Int Source INT_OUT[12] from Display1 Sub-System
        DCD     Reserved264_IRQHandler                        ;xxx Interrupt 264
        DCD     Reserved265_IRQHandler                        ;xxx Interrupt 265
        DCD     Reserved266_IRQHandler                        ;xxx Interrupt 266
        DCD     VPU_SYS_INT0_IRQHandler                       ;Shared Int Source SYS_INT[0] from VPU Sub-System
        DCD     VPU_SYS_INT1_IRQHandler                       ;Shared Int Source SYS_INT[1] from VPU Sub-System
        DCD     Reserved269_IRQHandler                        ;xxx Interrupt 269
        DCD     Reserved270_IRQHandler                        ;xxx Interrupt 270
        DCD     Reserved271_IRQHandler                        ;xxx Interrupt 271
        DCD     Reserved272_IRQHandler                        ;xxx Interrupt 272
        DCD     Reserved273_IRQHandler                        ;xxx Interrupt 273
        DCD     Reserved274_IRQHandler                        ;xxx Interrupt 274
        DCD     LSIO_MU0_INT_IRQHandler                       ;Shared Int Source MU0_INT from LSIO Sub-System
        DCD     LSIO_MU1_INT_IRQHandler                       ;Shared Int Source MU1_INT from LSIO Sub-System
        DCD     LSIO_MU2_INT_IRQHandler                       ;Shared Int Source MU2_INT from LSIO Sub-System
        DCD     LSIO_MU3_INT_IRQHandler                       ;Shared Int Source MU3_INT from LSIO Sub-System
        DCD     LSIO_MU4_INT_IRQHandler                       ;Shared Int Source MU4_INT from LSIO Sub-System
        DCD     Reserved280_IRQHandler                        ;xxx Interrupt 280
        DCD     Reserved281_IRQHandler                        ;xxx Interrupt 281
        DCD     Reserved282_IRQHandler                        ;xxx Interrupt 282
        DCD     LSIO_MU5_INT_A_IRQHandler                     ;Shared Int Source MU5_INT_A from LSIO Sub-System
        DCD     LSIO_MU6_INT_A_IRQHandler                     ;Shared Int Source MU6_INT_A from LSIO Sub-System
        DCD     LSIO_MU7_INT_A_IRQHandler                     ;Shared Int Source MU7_INT_A from LSIO Sub-System
        DCD     LSIO_MU8_INT_A_IRQHandler                     ;Shared Int Source MU8_INT_A from LSIO Sub-System
        DCD     LSIO_MU9_INT_A_IRQHandler                     ;Shared Int Source MU9_INT_A from LSIO Sub-System
        DCD     LSIO_MU10_INT_A_IRQHandler                    ;Shared Int Source MU10_INT_A from LSIO Sub-System
        DCD     LSIO_MU11_INT_A_IRQHandler                    ;Shared Int Source MU11_INT_A from LSIO Sub-System
        DCD     LSIO_MU12_INT_A_IRQHandler                    ;Shared Int Source MU12_INT_A from LSIO Sub-System
        DCD     LSIO_MU13_INT_A_IRQHandler                    ;Shared Int Source MU13_INT_A from LSIO Sub-System
        DCD     Reserved292_IRQHandler                        ;xxx Interrupt 292
        DCD     Reserved293_IRQHandler                        ;xxx Interrupt 293
        DCD     Reserved294_IRQHandler                        ;xxx Interrupt 294
        DCD     Reserved295_IRQHandler                        ;xxx Interrupt 295
        DCD     Reserved296_IRQHandler                        ;xxx Interrupt 296
        DCD     Reserved297_IRQHandler                        ;xxx Interrupt 297
        DCD     Reserved298_IRQHandler                        ;xxx Interrupt 298
        DCD     LSIO_MU5_INT_B_IRQHandler                     ;Shared Int Source MU5_INT_B from LSIO Sub-System
        DCD     LSIO_MU6_INT_B_IRQHandler                     ;Shared Int Source MU6_INT_B from LSIO Sub-System
        DCD     LSIO_MU7_INT_B_IRQHandler                     ;Shared Int Source MU7_INT_B from LSIO Sub-System
        DCD     LSIO_MU8_INT_B_IRQHandler                     ;Shared Int Source MU8_INT_B from LSIO Sub-System
        DCD     LSIO_MU9_INT_B_IRQHandler                     ;Shared Int Source MU9_INT_B from LSIO Sub-System
        DCD     LSIO_MU10_INT_B_IRQHandler                    ;Shared Int Source MU10_INT_B from LSIO Sub-System
        DCD     LSIO_MU11_INT_B_IRQHandler                    ;Shared Int Source MU11_INT_B from LSIO Sub-System
        DCD     LSIO_MU12_INT_B_IRQHandler                    ;Shared Int Source MU12_INT_B from LSIO Sub-System
        DCD     LSIO_MU13_INT_B_IRQHandler                    ;Shared Int Source MU13_INT_B from LSIO Sub-System
        DCD     Reserved308_IRQHandler                        ;xxx Interrupt 308
        DCD     Reserved309_IRQHandler                        ;xxx Interrupt 309
        DCD     Reserved310_IRQHandler                        ;xxx Interrupt 310
        DCD     Reserved311_IRQHandler                        ;xxx Interrupt 311
        DCD     Reserved312_IRQHandler                        ;xxx Interrupt 312
        DCD     Reserved313_IRQHandler                        ;xxx Interrupt 313
        DCD     Reserved314_IRQHandler                        ;xxx Interrupt 314
        DCD     DMA_SPI0_INT_IRQHandler                       ;Shared Int Source SPI0_INT from DMA Sub-System
        DCD     DMA_SPI1_INT_IRQHandler                       ;Shared Int Source SPI1_INT from DMA Sub-System
        DCD     DMA_SPI2_INT_IRQHandler                       ;Shared Int Source SPI2_INT from DMA Sub-System
        DCD     DMA_SPI3_INT_IRQHandler                       ;Shared Int Source SPI3_INT from DMA Sub-System
        DCD     DMA_I2C0_INT_IRQHandler                       ;Shared Int Source I2C0_INT from DMA Sub-System
        DCD     DMA_I2C1_INT_IRQHandler                       ;Shared Int Source I2C1_INT from DMA Sub-System
        DCD     DMA_I2C2_INT_IRQHandler                       ;Shared Int Source I2C2_INT from DMA Sub-System
        DCD     DMA_I2C3_INT_IRQHandler                       ;Shared Int Source I2C3_INT from DMA Sub-System
        DCD     DMA_I2C4_INT_IRQHandler                       ;Shared Int Source I2C4_INT from DMA Sub-System
        DCD     DMA_UART0_INT_IRQHandler                      ;Shared Int Source UART0_INT from DMA Sub-System
        DCD     DMA_UART1_INT_IRQHandler                      ;Shared Int Source UART1_INT from DMA Sub-System
        DCD     DMA_UART2_INT_IRQHandler                      ;Shared Int Source UART2_INT from DMA Sub-System
        DCD     DMA_UART3_INT_IRQHandler                      ;Shared Int Source UART3_INT from DMA Sub-System
        DCD     DMA_UART4_INT_IRQHandler                      ;Shared Int Source UART4_INT from DMA Sub-System
        DCD     DMA_SIM0_INT_IRQHandler                       ;Shared Int Source SIM0_INT from DMA Sub-System
        DCD     DMA_SIM1_INT_IRQHandler                       ;Shared Int Source SIM1_INT from DMA Sub-System
        DCD     CONNECTIVITY_USDHC0_INT_IRQHandler            ;Shared Int Source uSDHC0_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_USDHC1_INT_IRQHandler            ;Shared Int Source uSDHC1_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_USDHC2_INT_IRQHandler            ;Shared Int Source uSDHC2_INT from Connectivity Sub-System
        DCD     DMA_FLEXCAN0_INT_IRQHandler                   ;Shared Int Source FlexCAN0_INT from DMA Sub-System
        DCD     DMA_FLEXCAN1_INT_IRQHandler                   ;Shared Int Source FlexCAN1_INT from DMA Sub-System
        DCD     DMA_FLEXCAN2_INT_IRQHandler                   ;Shared Int Source FlexCAN2_INT from DMA Sub-System
        DCD     DMA_FTM0_INT_IRQHandler                       ;Shared Int Source FTM0_INT from DMA Sub-System
        DCD     DMA_FTM1_INT_IRQHandler                       ;Shared Int Source FTM1_INT from DMA Sub-System
        DCD     DMA_ADC0_INT_IRQHandler                       ;Shared Int Source ADC0_INT from DMA Sub-System
        DCD     DMA_ADC1_INT_IRQHandler                       ;Shared Int Source ADC1_INT from DMA Sub-System
        DCD     DMA_EXTERNAL_DMA_INT_0_IRQHandler             ;Shared Int Source EXTERNAL_DMA_INT_0 from DMA Sub-System
        DCD     DMA_EXTERNAL_DMA_INT_1_IRQHandler             ;Shared Int Source EXTERNAL_DMA_INT_1 from DMA Sub-System
        DCD     DMA_EXTERNAL_DMA_INT_2_IRQHandler             ;Shared Int Source EXTERNAL_DMA_INT_2 from DMA Sub-System
        DCD     DMA_EXTERNAL_DMA_INT_3_IRQHandler             ;Shared Int Source EXTERNAL_DMA_INT_3 from DMA Sub-System
        DCD     DMA_EXTERNAL_DMA_INT_4_IRQHandler             ;Shared Int Source EXTERNAL_DMA_INT_4 from DMA Sub-System
        DCD     DMA_EXTERNAL_DMA_INT_5_IRQHandler             ;Shared Int Source EXTERNAL_DMA_INT_5 from DMA Sub-System
        DCD     Reserved347_IRQHandler                        ;xxx Interrupt 347
        DCD     Reserved348_IRQHandler                        ;xxx Interrupt 348
        DCD     Reserved349_IRQHandler                        ;xxx Interrupt 349
        DCD     Reserved350_IRQHandler                        ;xxx Interrupt 350
        DCD     Reserved351_IRQHandler                        ;xxx Interrupt 351
        DCD     Reserved352_IRQHandler                        ;xxx Interrupt 352
        DCD     Reserved353_IRQHandler                        ;xxx Interrupt 353
        DCD     Reserved354_IRQHandler                        ;xxx Interrupt 354
        DCD     CONNECTIVITY_ENET0_FRAME1_INT_IRQHandler      ;Shared Int Source ENET0_FRAME1_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ENET0_FRAME2_INT_IRQHandler      ;Shared Int Source ENET0_FRAME2_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ENET0_FRAME0_EVENT_INT_IRQHandler    ;Shared Int Source ENET0_FRAME0_EVENT_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ENET0_TIMER_INT_IRQHandler       ;Shared Int Source ENET0_TIMER_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ENET1_FRAME1_INT_IRQHandler      ;Shared Int Source ENET1_FRAME1_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ENET1_FRAME2_INT_IRQHandler      ;Shared Int Source ENET1_FRAME2_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ENET1_FRAME0_EVENT_INT_IRQHandler    ;Shared Int Source ENET1_FRAME0_EVENT_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ENET1_TIMER_INT_IRQHandler       ;Shared Int Source ENET1_TIMER_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_DTCP_INT_IRQHandler              ;Shared Int Source DTCP_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_MLB_INT_IRQHandler               ;Shared Int Source MLB_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_MLB_AHB_INT_IRQHandler           ;Shared Int Source MLB_AHB_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_USB_OTG_INT_IRQHandler           ;Shared Int Source USB_OTG_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_USB_HOST_INT_IRQHandler          ;Shared Int Source USB_HOST_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_UTMI_INT_IRQHandler              ;Shared Int Source UTMI_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_WAKEUP_INT_IRQHandler            ;Shared Int Source WAKEUP_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_USB3_INT_IRQHandler              ;Shared Int Source USB3_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ND_FLASH_BCH_INT_IRQHandler      ;Shared Int Source ND_FLASH_BCH_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_ND_FLASH_GPMI_INT_IRQHandler     ;Shared Int Source ND_FLASH_GPMI_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_APBHDMA_IRQHandler               ;Shared Int Source APBHDMA from Connectivity Sub-System
        DCD     CONNECTIVITY_DMA_INT_IRQHandler               ;Shared Int Source DMA_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_DMA_ERR_INT_IRQHandler           ;Shared Int Source DMA_ERR_INT from Connectivity Sub-System
        DCD     Reserved376_IRQHandler                        ;xxx Interrupt 376
        DCD     Reserved377_IRQHandler                        ;xxx Interrupt 377
        DCD     Reserved378_IRQHandler                        ;xxx Interrupt 378
        DCD     Reserved379_IRQHandler                        ;xxx Interrupt 379
        DCD     Reserved380_IRQHandler                        ;xxx Interrupt 380
        DCD     Reserved381_IRQHandler                        ;xxx Interrupt 381
        DCD     Reserved382_IRQHandler                        ;xxx Interrupt 382
        DCD     Reserved383_IRQHandler                        ;xxx Interrupt 383
        DCD     Reserved384_IRQHandler                        ;xxx Interrupt 384
        DCD     Reserved385_IRQHandler                        ;xxx Interrupt 385
        DCD     Reserved386_IRQHandler                        ;xxx Interrupt 386
        DCD     IMAGING_MSI_INT_IRQHandler                    ;Shared Int Source MSI_INT from Imaging Sub-System
        DCD     Reserved388_IRQHandler                        ;xxx Interrupt 388
        DCD     Reserved389_IRQHandler                        ;xxx Interrupt 389
        DCD     Reserved390_IRQHandler                        ;xxx Interrupt 390
        DCD     Reserved391_IRQHandler                        ;xxx Interrupt 391
        DCD     Reserved392_IRQHandler                        ;xxx Interrupt 392
        DCD     Reserved393_IRQHandler                        ;xxx Interrupt 393
        DCD     Reserved394_IRQHandler                        ;xxx Interrupt 394
        DCD     Reserved395_IRQHandler                        ;xxx Interrupt 395
        DCD     IMAGING_PDMA_STREAM0_INT_IRQHandler           ;Shared Int Source PDMA_STREAM0_INT from Imaging Sub-System
        DCD     IMAGING_PDMA_STREAM1_INT_IRQHandler           ;Shared Int Source PDMA_STREAM1_INT from Imaging Sub-System
        DCD     IMAGING_PDMA_STREAM2_INT_IRQHandler           ;Shared Int Source PDMA_STREAM2_INT from Imaging Sub-System
        DCD     IMAGING_PDMA_STREAM3_INT_IRQHandler           ;Shared Int Source PDMA_STREAM3_INT from Imaging Sub-System
        DCD     IMAGING_PDMA_STREAM4_INT_IRQHandler           ;Shared Int Source PDMA_STREAM4_INT from Imaging Sub-System
        DCD     IMAGING_PDMA_STREAM5_INT_IRQHandler           ;Shared Int Source PDMA_STREAM5_INT from Imaging Sub-System
        DCD     IMAGING_PDMA_STREAM6_INT_IRQHandler           ;Shared Int Source PDMA_STREAM6_INT from Imaging Sub-System
        DCD     IMAGING_PDMA_STREAM7_INT_IRQHandler           ;Shared Int Source PDMA_STREAM7_INT from Imaging Sub-System
        DCD     IMAGING_MJPEG_ENC0_INT_IRQHandler             ;Shared Int Source MJPEG_ENC0_INT from Imaging Sub-System
        DCD     IMAGING_MJPEG_ENC1_INT_IRQHandler             ;Shared Int Source MJPEG_ENC1_INT from Imaging Sub-System
        DCD     IMAGING_MJPEG_ENC2_INT_IRQHandler             ;Shared Int Source MJPEG_ENC2_INT from Imaging Sub-System
        DCD     IMAGING_MJPEG_ENC3_INT_IRQHandler             ;Shared Int Source MJPEG_ENC3_INT from Imaging Sub-System
        DCD     IMAGING_MJPEG_DEC0_INT_IRQHandler             ;Shared Int Source MJPEG_DEC0_INT from Imaging Sub-System
        DCD     IMAGING_MJPEG_DEC1_INT_IRQHandler             ;Shared Int Source MJPEG_DEC1_INT from Imaging Sub-System
        DCD     IMAGING_MJPEG_DEC2_INT_IRQHandler             ;Shared Int Source MJPEG_DEC2_INT from Imaging Sub-System
        DCD     IMAGING_MJPEG_DEC3_INT_IRQHandler             ;Shared Int Source MJPEG_DEC3_INT from Imaging Sub-System
        DCD     Reserved412_IRQHandler                        ;xxx Interrupt 412
        DCD     AUDIO_SAI0_MOD_INT_IRQHandler                 ;Shared Int Source SAI0_MOD_INT from Audio Sub-System
        DCD     AUDIO_SAI0_DMA_INT_IRQHandler                 ;Shared Int Source SAI0_DMA_INT from Audio Sub-System
        DCD     AUDIO_SAI1_MOD_INT_IRQHandler                 ;Shared Int Source SAI1_MOD_INT from Audio Sub-System
        DCD     AUDIO_SAI1_DMA_INT_IRQHandler                 ;Shared Int Source SAI1_DMA_INT from Audio Sub-System
        DCD     AUDIO_SAI2_MOD_INT_IRQHandler                 ;Shared Int Source SAI2_MOD_INT from Audio Sub-System
        DCD     AUDIO_SAI2_DMA_INT_IRQHandler                 ;Shared Int Source SAI2_DMA_INT from Audio Sub-System
        DCD     MIPI_CSI0_OUT_INT_IRQHandler                  ;Shared Int Source OUT_INT from MIPI_CSI0 Sub-System
        DCD     MIPI_CSI1_OUT_INT_IRQHandler                  ;Shared Int Source OUT_INT from MIPI_CSI1 Sub-System
        DCD     HDMI_RX_OUT_INT_IRQHandler                    ;Shared Int Source OUT_INT from HDMI_RX Sub-System
        DCD     AUDIO_SAI3_MOD_INT_IRQHandler                 ;Shared Int Source SAI3_MOD_INT from Audio Sub-System
        DCD     AUDIO_SAI3_DMA_INT_IRQHandler                 ;Shared Int Source SAI3_DMA_INT from Audio Sub-System
        DCD     AUDIO_SAI_HDMI_RX_MOD_INT_IRQHandler          ;Shared Int Source SAI_HDMI_RX_MOD_INT from Audio Sub-System
        DCD     AUDIO_SAI_HDMI_RX_DMA_INT_IRQHandler          ;Shared Int Source SAI_HDMI_RX_DMA_INT from Audio Sub-System
        DCD     AUDIO_SAI_HDMI_TX_MOD_INT_IRQHandler          ;Shared Int Source SAI_HDMI_TX_MOD_INT from Audio Sub-System
        DCD     AUDIO_SAI_HDMI_TX_DMA_INT_IRQHandler          ;Shared Int Source SAI_HDMI_TX_DMA_INT from Audio Sub-System
        DCD     AUDIO_SAI6_MOD_INT_IRQHandler                 ;Shared Int Source SAI6_MOD_INT from Audio Sub-System
        DCD     AUDIO_SAI6_DMA_INT_IRQHandler                 ;Shared Int Source SAI6_DMA_INT from Audio Sub-System
        DCD     AUDIO_SAI7_MOD_INT_IRQHandler                 ;Shared Int Source SAI7_MOD_INT from Audio Sub-System
        DCD     AUDIO_SAI7_DMA_INT_IRQHandler                 ;Shared Int Source SAI7_DMA_INT from Audio Sub-System
        DCD     Reserved432_IRQHandler                        ;xxx Interrupt 432
        DCD     Reserved433_IRQHandler                        ;xxx Interrupt 433
        DCD     Reserved434_IRQHandler                        ;xxx Interrupt 434
        DCD     DMA_SPI0_MOD_INT_IRQHandler                   ;Shared Int Source SPI0_MOD_INT from DMA Sub-System
        DCD     DMA_SPI1_MOD_INT_IRQHandler                   ;Shared Int Source SPI1_MOD_INT from DMA Sub-System
        DCD     DMA_SPI2_MOD_INT_IRQHandler                   ;Shared Int Source SPI2_MOD_INT from DMA Sub-System
        DCD     DMA_SPI3_MOD_INT_IRQHandler                   ;Shared Int Source SPI3_MOD_INT from DMA Sub-System
        DCD     DMA_I2C0_MOD_INT_IRQHandler                   ;Shared Int Source I2C0_MOD_INT from DMA Sub-System
        DCD     DMA_I2C1_MOD_INT_IRQHandler                   ;Shared Int Source I2C1_MOD_INT from DMA Sub-System
        DCD     DMA_I2C2_MOD_INT_IRQHandler                   ;Shared Int Source I2C2_MOD_INT from DMA Sub-System
        DCD     DMA_I2C3_MOD_INT_IRQHandler                   ;Shared Int Source I2C3_MOD_INT from DMA Sub-System
        DCD     DMA_I2C4_MOD_INT_IRQHandler                   ;Shared Int Source I2C4_MOD_INT from DMA Sub-System
        DCD     DMA_UART0_MOD_INT_IRQHandler                  ;Shared Int Source UART0_MOD_INT from DMA Sub-System
        DCD     DMA_UART1_MOD_INT_IRQHandler                  ;Shared Int Source UART1_MOD_INT from DMA Sub-System
        DCD     DMA_UART2_MOD_INT_IRQHandler                  ;Shared Int Source UART2_MOD_INT from DMA Sub-System
        DCD     DMA_UART3_MOD_INT_IRQHandler                  ;Shared Int Source UART3_MOD_INT from DMA Sub-System
        DCD     DMA_UART4_MOD_INT_IRQHandler                  ;Shared Int Source UART4_MOD_INT from DMA Sub-System
        DCD     DMA_SIM0_MOD_INT_IRQHandler                   ;Shared Int Source SIM0_MOD_INT from DMA Sub-System
        DCD     DMA_SIM1_MOD_INT_IRQHandler                   ;Shared Int Source SIM1_MOD_INT from DMA Sub-System
        DCD     DMA_FLEXCAN0_MOD_INT_IRQHandler               ;Shared Int Source FLEXCAN0_MOD_INT from DMA Sub-System
        DCD     DMA_FLEXCAN1_MOD_INT_IRQHandler               ;Shared Int Source FLEXCAN1_MOD_INT from DMA Sub-System
        DCD     DMA_FLEXCAN2_MOD_INT_IRQHandler               ;Shared Int Source FLEXCAN2_MOD_INT from DMA Sub-System
        DCD     DMA_FTM0_MOD_INT_IRQHandler                   ;Shared Int Source FTM0_MOD_INT from DMA Sub-System
        DCD     DMA_FTM1_MOD_INT_IRQHandler                   ;Shared Int Source FTM1_MOD_INT from DMA Sub-System
        DCD     DMA_ADC0_MOD_INT_IRQHandler                   ;Shared Int Source ADC0_MOD_INT from DMA Sub-System
        DCD     DMA_ADC1_MOD_INT_IRQHandler                   ;Shared Int Source ADC1_MOD_INT from DMA Sub-System
        DCD     DMA_FLEXCAN0_DMA_INT_IRQHandler               ;Shared Int Source FLEXCAN0_DMA_INT from DMA Sub-System
        DCD     DMA_FLEXCAN1_DMA_INT_IRQHandler               ;Shared Int Source FLEXCAN1_DMA_INT from DMA Sub-System
        DCD     DMA_FLEXCAN2_DMA_INT_IRQHandler               ;Shared Int Source FLEXCAN2_DMA_INT from DMA Sub-System
        DCD     DMA_FTM0_DMA_INT_IRQHandler                   ;Shared Int Source FTM0_DMA_INT from DMA Sub-System
        DCD     DMA_FTM1_DMA_INT_IRQHandler                   ;Shared Int Source FTM1_DMA_INT from DMA Sub-System
        DCD     DMA_ADC0_DMA_INT_IRQHandler                   ;Shared Int Source ADC0_DMA_INT from DMA Sub-System
        DCD     DMA_ADC1_DMA_INT_IRQHandler                   ;Shared Int Source ADC1_DMA_INT from DMA Sub-System
        DCD     Reserved465_IRQHandler                        ;xxx Interrupt 465
        DCD     Reserved466_IRQHandler                        ;xxx Interrupt 466
        DCD     AUDIO_EDMA0_INT_IRQHandler                    ;Shared Int Source eDMA0_INT from Audio Sub-System
        DCD     AUDIO_EDMA0_ERR_INT_IRQHandler                ;Shared Int Source eDMA0_ERR_INT from Audio Sub-System
        DCD     AUDIO_EDMA1_INT_IRQHandler                    ;Shared Int Source eDMA1_INT from Audio Sub-System
        DCD     AUDIO_EDMA1_ERR_INT_IRQHandler                ;Shared Int Source eDMA1_ERR_INT from Audio Sub-System
        DCD     AUDIO_ASRC0_INT1_IRQHandler                   ;Shared Int Source ASRC0_INT1 from Audio Sub-System
        DCD     AUDIO_ASRC0_INT2_IRQHandler                   ;Shared Int Source ASRC0_INT2 from Audio Sub-System
        DCD     AUDIO_DMA0_CH0_INT_IRQHandler                 ;Shared Int Source DMA0_CH0_INT from Audio Sub-System
        DCD     AUDIO_DMA0_CH1_INT_IRQHandler                 ;Shared Int Source DMA0_CH1_INT from Audio Sub-System
        DCD     AUDIO_DMA0_CH2_INT_IRQHandler                 ;Shared Int Source DMA0_CH2_INT from Audio Sub-System
        DCD     AUDIO_DMA0_CH3_INT_IRQHandler                 ;Shared Int Source DMA0_CH3_INT from Audio Sub-System
        DCD     AUDIO_DMA0_CH4_INT_IRQHandler                 ;Shared Int Source DMA0_CH4_INT from Audio Sub-System
        DCD     AUDIO_DMA0_CH5_INT_IRQHandler                 ;Shared Int Source DMA0_CH5_INT from Audio Sub-System
        DCD     AUDIO_ASRC1_INT1_IRQHandler                   ;Shared Int Source ASRC1_INT1 from Audio Sub-System
        DCD     AUDIO_ASRC1_INT2_IRQHandler                   ;Shared Int Source ASRC1_INT2 from Audio Sub-System
        DCD     AUDIO_DMA1_CH0_INT_IRQHandler                 ;Shared Int Source DMA1_CH0_INT from Audio Sub-System
        DCD     AUDIO_DMA1_CH1_INT_IRQHandler                 ;Shared Int Source DMA1_CH1_INT from Audio Sub-System
        DCD     AUDIO_DMA1_CH2_INT_IRQHandler                 ;Shared Int Source DMA1_CH2_INT from Audio Sub-System
        DCD     AUDIO_DMA1_CH3_INT_IRQHandler                 ;Shared Int Source DMA1_CH3_INT from Audio Sub-System
        DCD     AUDIO_DMA1_CH4_INT_IRQHandler                 ;Shared Int Source DMA1_CH4_INT from Audio Sub-System
        DCD     AUDIO_DMA1_CH5_INT_IRQHandler                 ;Shared Int Source DMA1_CH5_INT from Audio Sub-System
        DCD     AUDIO_ESAI0_INT_IRQHandler                    ;Shared Int Source ESAI0_INT from Audio Sub-System
        DCD     AUDIO_ESAI1_INT_IRQHandler                    ;Shared Int Source ESAI1_INT  from Audio Sub-System
        DCD     AUDIO_UNUSED_IRQHandler                       ;Shared Int Source Unused from Audio Sub-System
        DCD     AUDIO_GPT0_INT_IRQHandler                     ;Shared Int Source GPT0_INT from Audio Sub-System
        DCD     AUDIO_GPT1_INT_IRQHandler                     ;Shared Int Source GPT1_INT from Audio Sub-System
        DCD     AUDIO_GPT2_INT_IRQHandler                     ;Shared Int Source GPT2_INT from Audio Sub-System
        DCD     AUDIO_GPT3_INT_IRQHandler                     ;Shared Int Source GPT3_INT from Audio Sub-System
        DCD     AUDIO_GPT4_INT_IRQHandler                     ;Shared Int Source GPT4_INT from Audio Sub-System
        DCD     AUDIO_GPT5_INT_IRQHandler                     ;Shared Int Source GPT5_INT from Audio Sub-System
        DCD     AUDIO_SAI0_INT_IRQHandler                     ;Shared Int Source SAI0_INT from Audio Sub-System
        DCD     AUDIO_SAI1_INT_IRQHandler                     ;Shared Int Source SAI1_INT from Audio Sub-System
        DCD     AUDIO_SAI2_INT_IRQHandler                     ;Shared Int Source SAI2_INT from Audio Sub-System
        DCD     AUDIO_SAI3_INT_IRQHandler                     ;Shared Int Source SAI3_INT from Audio Sub-System
        DCD     AUDIO_SAI_HDMI_RX_INT_IRQHandler              ;Shared Int Source SAI_HDMI_RX_INT from Audio Sub-System
        DCD     AUDIO_SAI_HDMI_TX_INT_IRQHandler              ;Shared Int Source SAI_HDMI_TX_INT from Audio Sub-System
        DCD     AUDIO_SAI6_INT_IRQHandler                     ;Shared Int Source SAI6_INT from Audio Sub-System
        DCD     AUDIO_SAI7_INT_IRQHandler                     ;Shared Int Source SAI7_INT from Audio Sub-System
        DCD     AUDIO_SPDIF0_RX_INT_IRQHandler                ;Shared Int Source SPDIF0_RX_INT from Audio Sub-System
        DCD     AUDIO_SPDIF0_TX_INT_IRQHandler                ;Shared Int Source SPDIF0_TX_INT from Audio Sub-System
        DCD     AUDIO_SPDIF1_RX_INT_IRQHandler                ;Shared Int Source SPDIF1_RX_INT from Audio Sub-System
        DCD     AUDIO_SPDIF1_TX_INT_IRQHandler                ;Shared Int Source SPDIF1_TX_INT from Audio Sub-System
        DCD     AUDIO_ESAI0_MOD_INT_IRQHandler                ;Shared Int Source ESAI0_MOD_INT from Audio Sub-System
        DCD     AUDIO_ESAI0_DMA_INT_IRQHandler                ;Shared Int Source ESAI0_DMA_INT from Audio Sub-System
        DCD     AUDIO_ESAI1_MOD_INT_IRQHandler                ;Shared Int Source ESAI1_MOD_INT from Audio Sub-System
        DCD     AUDIO_ESAI1_DMA_INT_IRQHandler                ;Shared Int Source ESAI1_DMA_INT from Audio Sub-System
        DCD     Reserved512_IRQHandler                        ;xxx Interrupt 512
        DCD     Reserved513_IRQHandler                        ;xxx Interrupt 513
        DCD     Reserved514_IRQHandler                        ;xxx Interrupt 514
        DCD     DMA_SPI0_DMA_RX_INT_IRQHandler                ;Shared Int Source SPI0_DMA_RX_INT from DMA Sub-System
        DCD     DMA_SPI0_DMA_TX_INT_IRQHandler                ;Shared Int Source SPI0_DMA_TX_INT from DMA Sub-System
        DCD     DMA_SPI1_DMA_RX_INT_IRQHandler                ;Shared Int Source SPI1_DMA_RX_INT from DMA Sub-System
        DCD     DMA_SPI1_DMA_TX_INT_IRQHandler                ;Shared Int Source SPI1_DMA_TX_INT from DMA Sub-System
        DCD     DMA_SPI2_DMA_RX_INT_IRQHandler                ;Shared Int Source SPI2_DMA_RX_INT from DMA Sub-System
        DCD     DMA_SPI2_DMA_TX_INT_IRQHandler                ;Shared Int Source SPI2_DMA_TX_INT from DMA Sub-System
        DCD     DMA_SPI3_DMA_RX_INT_IRQHandler                ;Shared Int Source SPI3_DMA_RX_INT from DMA Sub-System
        DCD     DMA_SPI3_DMA_TX_INT_IRQHandler                ;Shared Int Source SPI3_DMA_TX_INT from DMA Sub-System
        DCD     DMA_I2C0_DMA_RX_INT_IRQHandler                ;Shared Int Source I2C0_DMA_RX_INT from DMA Sub-System
        DCD     DMA_I2C0_DMA_TX_INT_IRQHandler                ;Shared Int Source I2C0_DMA_TX_INT from DMA Sub-System
        DCD     DMA_I2C1_DMA_RX_INT_IRQHandler                ;Shared Int Source I2C1_DMA_RX_INT from DMA Sub-System
        DCD     DMA_I2C1_DMA_TX_INT_IRQHandler                ;Shared Int Source I2C1_DMA_TX_INT from DMA Sub-System
        DCD     DMA_I2C2_DMA_RX_INT_IRQHandler                ;Shared Int Source I2C2_DMA_RX_INT from DMA Sub-System
        DCD     DMA_I2C2_DMA_TX_INT_IRQHandler                ;Shared Int Source I2C2_DMA_TX_INT from DMA Sub-System
        DCD     DMA_I2C3_DMA_RX_INT_IRQHandler                ;Shared Int Source I2C3_DMA_RX_INT from DMA Sub-System
        DCD     DMA_I2C3_DMA_TX_INT_IRQHandler                ;Shared Int Source I2C3_DMA_TX_INT from DMA Sub-System
        DCD     DMA_I2C4_DMA_RX_INT_IRQHandler                ;Shared Int Source I2C4_DMA_RX_INT from DMA Sub-System
        DCD     DMA_I2C4_DMA_TX_INT_IRQHandler                ;Shared Int Source I2C4_DMA_TX_INT from DMA Sub-System
        DCD     DMA_UART0_DMA_RX_INT_IRQHandler               ;Shared Int Source UART0_DMA_RX_INT from DMA Sub-System
        DCD     DMA_UART0_DMA_TX_INT_IRQHandler               ;Shared Int Source UART0_DMA_TX_INT from DMA Sub-System
        DCD     DMA_UART1_DMA_RX_INT_IRQHandler               ;Shared Int Source UART1_DMA_RX_INT from DMA Sub-System
        DCD     DMA_UART1_DMA_TX_INT_IRQHandler               ;Shared Int Source UART1_DMA_TX_INT from DMA Sub-System
        DCD     DMA_UART2_DMA_RX_INT_IRQHandler               ;Shared Int Source UART2_DMA_RX_INT from DMA Sub-System
        DCD     DMA_UART2_DMA_TX_INT_IRQHandler               ;Shared Int Source UART2_DMA_TX_INT from DMA Sub-System
        DCD     DMA_UART3_DMA_RX_INT_IRQHandler               ;Shared Int Source UART3_DMA_RX_INT from DMA Sub-System
        DCD     DMA_UART3_DMA_TX_INT_IRQHandler               ;Shared Int Source UART3_DMA_TX_INT from DMA Sub-System
        DCD     DMA_UART4_DMA_RX_INT_IRQHandler               ;Shared Int Source UART4_DMA_RX_INT from DMA Sub-System
        DCD     DMA_UART4_DMA_TX_INT_IRQHandler               ;Shared Int Source UART4_DMA_TX_INT from DMA Sub-System
        DCD     DMA_SIM0_DMA_RX_INT_IRQHandler                ;Shared Int Source SIM0_DMA_RX_INT from DMA Sub-System
        DCD     DMA_SIM0_DMA_TX_INT_IRQHandler                ;Shared Int Source SIM0_DMA_TX_INT from DMA Sub-System
        DCD     DMA_SIM1_DMA_RX_INT_IRQHandler                ;Shared Int Source SIM1_DMA_RX_INT from DMA Sub-System
        DCD     DMA_SIM1_DMA_TX_INT_IRQHandler                ;Shared Int Source SIM1_DMA_TX_INT from DMA Sub-System
        DCD     SECURITY_MU1_A_INT_IRQHandler                 ;Shared Int Source MU1_A_INT from Security Sub-System
        DCD     SECURITY_MU2_A_INT_IRQHandler                 ;Shared Int Source MU2_A_INT from Security Sub-System
        DCD     SECURITY_MU3_A_INT_IRQHandler                 ;Shared Int Source MU3_A_INT from Security Sub-System
        DCD     SECURITY_CAAM_INT0_IRQHandler                 ;Shared Int Source CAAM_INT0 from Security Sub-System
        DCD     SECURITY_CAAM_INT1_IRQHandler                 ;Shared Int Source CAAM_INT1 from Security Sub-System
        DCD     SECURITY_CAAM_INT2_IRQHandler                 ;Shared Int Source CAAM_INT2 from Security Sub-System
        DCD     SECURITY_CAAM_INT3_IRQHandler                 ;Shared Int Source CAAM_INT3 from Security Sub-System
        DCD     SECURITY_CAAM_RTIC_INT_IRQHandler             ;Shared Int Source CAAM_RTIC_INT from Security Sub-System
        DCD     AUDIO_SPDIF0_RX_MOD_INT_IRQHandler            ;Shared Int Source SPDIF0_RX_MOD_INT from Audio Sub-System
        DCD     AUDIO_SPDIF0_RX_DMA_INT_IRQHandler            ;Shared Int Source SPDIF0_RX_DMA_INT from Audio Sub-System
        DCD     AUDIO_SPDIF0_TX_MOD_INT_IRQHandler            ;Shared Int Source SPDIF0_TX_MOD_INT from Audio Sub-System
        DCD     AUDIO_SPDIF0_TX_DMA_INT_IRQHandler            ;Shared Int Source SPDIF0_TX_DMA_INT from Audio Sub-System
        DCD     AUDIO_SPDIF1_RX_MOD_INT_IRQHandler            ;Shared Int Source SPDIF1_RX_MOD_INT from Audio Sub-System
        DCD     AUDIO_SPDIF1_RX_DMA_INT_IRQHandler            ;Shared Int Source SPDIF1_RX_DMA_INT from Audio Sub-System
        DCD     AUDIO_SPDIF1_TX_MOD_INT_IRQHandler            ;Shared Int Source SPDIF1_TX_MOD_INT from Audio Sub-System
        DCD     AUDIO_SPDIF1_TX_DMA_INT_IRQHandler            ;Shared Int Source SPDIF1_TX_DMA_INT from Audio Sub-System
        DCD     VPU_VPU_INT_0_IRQHandler                      ;Shared Int Source VPU_INT_0 from VPU Sub-System
        DCD     VPU_VPU_INT_1_IRQHandler                      ;Shared Int Source VPU_INT_1 from VPU Sub-System
        DCD     VPU_VPU_INT_2_IRQHandler                      ;Shared Int Source VPU_INT_2 from VPU Sub-System
        DCD     VPU_VPU_INT_3_IRQHandler                      ;Shared Int Source VPU_INT_3 from VPU Sub-System
        DCD     VPU_VPU_INT_4_IRQHandler                      ;Shared Int Source VPU_INT_4 from VPU Sub-System
        DCD     VPU_VPU_INT_5_IRQHandler                      ;Shared Int Source VPU_INT_5 from VPU Sub-System
        DCD     VPU_VPU_INT_6_IRQHandler                      ;Shared Int Source VPU_INT_6 from VPU Sub-System
        DCD     VPU_VPU_INT_7_IRQHandler                      ;Shared Int Source VPU_INT_7 from VPU Sub-System
        DCD     Reserved571_IRQHandler                        ;xxx Interrupt 571
        DCD     Reserved572_IRQHandler                        ;xxx Interrupt 572
        DCD     Reserved573_IRQHandler                        ;xxx Interrupt 573
        DCD     Reserved574_IRQHandler                        ;xxx Interrupt 574
        DCD     Reserved575_IRQHandler                        ;xxx Interrupt 575
        DCD     Reserved576_IRQHandler                        ;xxx Interrupt 576
        DCD     Reserved577_IRQHandler                        ;xxx Interrupt 577
        DCD     Reserved578_IRQHandler                        ;xxx Interrupt 578
        DCD     Reserved579_IRQHandler                        ;xxx Interrupt 579
        DCD     M4_1_INTMUX_SOURCE_TPM_IRQHandler             ;INTMUX Input source: TPM Interrupt
        DCD     Reserved581_IRQHandler                        ;xxx Interrupt 581
        DCD     Reserved582_IRQHandler                        ;xxx Interrupt 582
        DCD     M4_1_INTMUX_SOURCE_LPIT_IRQHandler            ;INTMUX Input source: LPIT Interrupt
        DCD     Reserved584_IRQHandler                        ;xxx Interrupt 584
        DCD     Reserved585_IRQHandler                        ;xxx Interrupt 585
        DCD     M4_1_INTMUX_SOURCE_LPUART_IRQHandler          ;INTMUX Input source: LPUART Interrupt
        DCD     Reserved587_IRQHandler                        ;xxx Interrupt 587
        DCD     M4_1_INTMUX_SOURCE_LPI2C_IRQHandler           ;INTMUX Input source: LPI2C Interrupt
        DCD     Reserved589_IRQHandler                        ;xxx Interrupt 589
        DCD     Reserved590_IRQHandler                        ;xxx Interrupt 590
        DCD     Reserved591_IRQHandler                        ;xxx Interrupt 591
        DCD     Reserved592_IRQHandler                        ;xxx Interrupt 592
        DCD     Reserved593_IRQHandler                        ;xxx Interrupt 593
        DCD     Reserved594_IRQHandler                        ;xxx Interrupt 594
        DCD     Reserved595_IRQHandler                        ;xxx Interrupt 595
        DCD     Reserved596_IRQHandler                        ;xxx Interrupt 596
        DCD     Reserved597_IRQHandler                        ;xxx Interrupt 597
        DCD     Reserved598_IRQHandler                        ;xxx Interrupt 598
        DCD     Reserved599_IRQHandler                        ;xxx Interrupt 599
        DCD     Reserved600_IRQHandler                        ;xxx Interrupt 600
        DCD     Reserved601_IRQHandler                        ;xxx Interrupt 601
        DCD     Reserved602_IRQHandler                        ;xxx Interrupt 602
        DCD     Reserved603_IRQHandler                        ;xxx Interrupt 603
        DCD     Reserved604_IRQHandler                        ;xxx Interrupt 604
        DCD     Reserved605_IRQHandler                        ;xxx Interrupt 605
        DCD     Reserved606_IRQHandler                        ;xxx Interrupt 606
        DCD     M4_1_INTMUX_SOURCE_MU0_A3_IRQHandler          ;INTMUX Input source: MU0_A3 Interrupt
        DCD     M4_1_INTMUX_SOURCE_MU0_A2_IRQHandler          ;INTMUX Input source: MU0_A2 Interrupt
        DCD     M4_1_INTMUX_SOURCE_MU0_A1_IRQHandler          ;INTMUX Input source: MU0_A1 Interrupt
        DCD     M4_1_INTMUX_SOURCE_MU0_A0_IRQHandler          ;INTMUX Input source: MU0_A0 Interrupt
        DCD     DefaultISR                                    ;611
        DCD     DefaultISR                                    ;612
        DCD     DefaultISR                                    ;613
        DCD     DefaultISR                                    ;614
        DCD     DefaultISR                                    ;615
        DCD     DefaultISR                                    ;616
        DCD     DefaultISR                                    ;617
        DCD     DefaultISR                                    ;618
        DCD     DefaultISR                                    ;619
        DCD     DefaultISR                                    ;620
        DCD     DefaultISR                                    ;621
        DCD     DefaultISR                                    ;622
        DCD     DefaultISR                                    ;623
        DCD     DefaultISR                                    ;624
        DCD     DefaultISR                                    ;625
        DCD     DefaultISR                                    ;626
        DCD     DefaultISR                                    ;627
        DCD     DefaultISR                                    ;628
        DCD     DefaultISR                                    ;629
        DCD     DefaultISR                                    ;630
        DCD     DefaultISR                                    ;631
        DCD     DefaultISR                                    ;632
        DCD     DefaultISR                                    ;633
        DCD     DefaultISR                                    ;634
        DCD     DefaultISR                                    ;635
        DCD     DefaultISR                                    ;636
        DCD     DefaultISR                                    ;637
        DCD     DefaultISR                                    ;638
        DCD     DefaultISR                                    ;639
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
        LDR     R0, =SystemInit
        BLX     R0
;
; Add RW / stack / heap initializaiton
; TCM controller must perform a read-modify-write for any access < 32-bit to keep the ECC updated.  
; The Software must ensure the TCM is ECC clean by initializing all memories that have the potential to be accessed as < 32-bit.
        MOV    R0, #0
        LDR    R1, =SFB(RW)
        LDR    R2, =SFE(RW)
.LC2:
        CMP    R1, R2
        ITT    LT
        STRLT  R0, [R1], #4
        BLT    .LC2

        MOV    R0, #0
        LDR    R1, =SFB(HEAP)
        LDR    R2, =SFE(HEAP)
.LC3:
        CMP    R1, R2
        ITT    LT
        STRLT  R0, [R1], #4
        BLT    .LC3

        LDR     R1, =SFB(CSTACK)
        LDR     R2, =SFE(CSTACK)
.LC4:
        CMP     R1, R2
        ITT     LT
        STRLT   R0, [R1], #4
        BLT     .LC4
; End RW / stack / heap initialization
;
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
        PUBWEAK Reserved17_IRQHandler
        PUBWEAK Reserved18_IRQHandler
        PUBWEAK Reserved19_IRQHandler
        PUBWEAK Reserved20_IRQHandler
        PUBWEAK M4_1_MCM_IRQHandler
        PUBWEAK Reserved22_IRQHandler
        PUBWEAK Reserved23_IRQHandler
        PUBWEAK Reserved24_IRQHandler
        PUBWEAK Reserved25_IRQHandler
        PUBWEAK Reserved26_IRQHandler
        PUBWEAK Reserved27_IRQHandler
        PUBWEAK Reserved28_IRQHandler
        PUBWEAK Reserved29_IRQHandler
        PUBWEAK Reserved30_IRQHandler
        PUBWEAK Reserved31_IRQHandler
        PUBWEAK Reserved32_IRQHandler
        PUBWEAK Reserved33_IRQHandler
        PUBWEAK Reserved34_IRQHandler
        PUBWEAK M4_1_TPM_IRQHandler
        PUBWEAK Reserved36_IRQHandler
        PUBWEAK Reserved37_IRQHandler
        PUBWEAK M4_1_LPIT_IRQHandler
        PUBWEAK Reserved39_IRQHandler
        PUBWEAK Reserved40_IRQHandler
        PUBWEAK M4_1_LPUART_IRQHandler
        PUBWEAK M4_1_LPUART_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_LPUART_IRQHandler
        LDR     R0, =M4_1_LPUART_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved42_IRQHandler
        PUBWEAK M4_1_LPI2C_IRQHandler
        PUBWEAK M4_1_LPI2C_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_LPI2C_IRQHandler
        LDR     R0, =M4_1_LPI2C_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved44_IRQHandler
        PUBWEAK M4_1_MU0_B0_IRQHandler
        PUBWEAK Reserved46_IRQHandler
        PUBWEAK Reserved47_IRQHandler
        PUBWEAK IRQSTEER_0_IRQHandler
        PUBWEAK IRQSTEER_0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IRQSTEER_0_IRQHandler
        LDR     R0, =IRQSTEER_0_DriverIRQHandler
        BX      R0

        PUBWEAK IRQSTEER_1_IRQHandler
        PUBWEAK IRQSTEER_1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IRQSTEER_1_IRQHandler
        LDR     R0, =IRQSTEER_1_DriverIRQHandler
        BX      R0

        PUBWEAK IRQSTEER_2_IRQHandler
        PUBWEAK IRQSTEER_2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IRQSTEER_2_IRQHandler
        LDR     R0, =IRQSTEER_2_DriverIRQHandler
        BX      R0

        PUBWEAK IRQSTEER_3_IRQHandler
        PUBWEAK IRQSTEER_3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IRQSTEER_3_IRQHandler
        LDR     R0, =IRQSTEER_3_DriverIRQHandler
        BX      R0

        PUBWEAK IRQSTEER_4_IRQHandler
        PUBWEAK IRQSTEER_4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IRQSTEER_4_IRQHandler
        LDR     R0, =IRQSTEER_4_DriverIRQHandler
        BX      R0

        PUBWEAK IRQSTEER_5_IRQHandler
        PUBWEAK IRQSTEER_5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IRQSTEER_5_IRQHandler
        LDR     R0, =IRQSTEER_5_DriverIRQHandler
        BX      R0

        PUBWEAK IRQSTEER_6_IRQHandler
        PUBWEAK IRQSTEER_6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IRQSTEER_6_IRQHandler
        LDR     R0, =IRQSTEER_6_DriverIRQHandler
        BX      R0

        PUBWEAK IRQSTEER_7_IRQHandler
        PUBWEAK IRQSTEER_7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IRQSTEER_7_IRQHandler
        LDR     R0, =IRQSTEER_7_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved56_IRQHandler
        PUBWEAK Reserved57_IRQHandler
        PUBWEAK Reserved58_IRQHandler
        PUBWEAK Reserved59_IRQHandler
        PUBWEAK M4_1_MU0_B1_IRQHandler
        PUBWEAK M4_1_MU0_B2_IRQHandler
        PUBWEAK M4_1_MU0_B3_IRQHandler
        PUBWEAK Reserved63_IRQHandler
        PUBWEAK Reserved64_IRQHandler
        PUBWEAK M4_1_MU1_A_IRQHandler
        PUBWEAK M4_1_SW_IRQHandler
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
        PUBWEAK Reserved80_IRQHandler
        PUBWEAK Reserved81_IRQHandler
        PUBWEAK Reserved82_IRQHandler
        PUBWEAK Reserved83_IRQHandler
        PUBWEAK Reserved84_IRQHandler
        PUBWEAK Reserved85_IRQHandler
        PUBWEAK Reserved86_IRQHandler
        PUBWEAK Reserved87_IRQHandler
        PUBWEAK Reserved88_IRQHandler
        PUBWEAK Reserved89_IRQHandler
        PUBWEAK Reserved90_IRQHandler
        PUBWEAK Reserved91_IRQHandler
        PUBWEAK Reserved92_IRQHandler
        PUBWEAK Reserved93_IRQHandler
        PUBWEAK Reserved94_IRQHandler
        PUBWEAK Reserved95_IRQHandler
        PUBWEAK Reserved96_IRQHandler
        PUBWEAK Reserved97_IRQHandler
        PUBWEAK Reserved98_IRQHandler
        PUBWEAK A53_NEXTERRIRQ_IRQHandler
        PUBWEAK A53_NINTERRIRQ_IRQHandler
        PUBWEAK A72_NEXTERRIRQ_IRQHandler
        PUBWEAK A72_NINTERRIRQ_IRQHandler
        PUBWEAK VPU_NEXTERRIRQ_IRQHandler
        PUBWEAK Reserved104_IRQHandler
        PUBWEAK Reserved105_IRQHandler
        PUBWEAK Reserved106_IRQHandler
        PUBWEAK CCI_NERRORIRQ_IRQHandler
        PUBWEAK CCI_NEVNTCNTOVERFLOW0_IRQHandler
        PUBWEAK CCI_NEVNTCNTOVERFLOW1_IRQHandler
        PUBWEAK CCI_NEVNTCNTOVERFLOW2_IRQHandler
        PUBWEAK CCI_NEVNTCNTOVERFLOW3_IRQHandler
        PUBWEAK CCI_NEVNTCNTOVERFLOW4_IRQHandler
        PUBWEAK Reserved113_IRQHandler
        PUBWEAK Reserved114_IRQHandler
        PUBWEAK M4_0_INT_OUT0_IRQHandler
        PUBWEAK M4_0_INT_OUT1_IRQHandler
        PUBWEAK M4_0_INT_OUT2_IRQHandler
        PUBWEAK M4_0_INT_OUT3_IRQHandler
        PUBWEAK M4_0_INT_OUT4_IRQHandler
        PUBWEAK M4_0_INT_OUT5_IRQHandler
        PUBWEAK M4_0_INT_OUT6_IRQHandler
        PUBWEAK M4_0_INT_OUT7_IRQHandler
        PUBWEAK M4_1_INT_OUT0_IRQHandler
        PUBWEAK M4_1_INT_OUT1_IRQHandler
        PUBWEAK M4_1_INT_OUT2_IRQHandler
        PUBWEAK M4_1_INT_OUT3_IRQHandler
        PUBWEAK M4_1_INT_OUT4_IRQHandler
        PUBWEAK M4_1_INT_OUT5_IRQHandler
        PUBWEAK M4_1_INT_OUT6_IRQHandler
        PUBWEAK M4_1_INT_OUT7_IRQHandler
        PUBWEAK DBLOG_COMB_IRPT_NS_IRQHandler
        PUBWEAK DBLOG_COMB_IRPT_S_IRQHandler
        PUBWEAK DBLOG_GBL_FLT_IRPT_NS_IRQHandler
        PUBWEAK DBLOG_GBL_FLT_IRPT_S_IRQHandler
        PUBWEAK DBLOG_PERF_IRPT_IMX8_0_IRQHandler
        PUBWEAK DBLOG_PERF_IRPT_IMX8_1_IRQHandler
        PUBWEAK DBLOG_PERF_IRPT_IMX8_2_IRQHandler
        PUBWEAK DBLOG_PERF_IRPT_IMX8_3_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT0_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT1_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT2_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT3_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT4_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT5_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT6_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT7_IRQHandler
        PUBWEAK DISPLAY0_RESERVED_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT9_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT10_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT11_IRQHandler
        PUBWEAK DISPLAY0_INT_OUT12_IRQHandler
        PUBWEAK Reserved152_IRQHandler
        PUBWEAK Reserved153_IRQHandler
        PUBWEAK Reserved154_IRQHandler
        PUBWEAK Reserved155_IRQHandler
        PUBWEAK LVDS0_INT_OUT_IRQHandler
        PUBWEAK LVDS1_INT_OUT_IRQHandler
        PUBWEAK MIPI_DSI0_INT_OUT_IRQHandler
        PUBWEAK MIPI_DSI1_INT_OUT_IRQHandler
        PUBWEAK HDMI_TX_INT_OUT_IRQHandler
        PUBWEAK Reserved161_IRQHandler
        PUBWEAK Reserved162_IRQHandler
        PUBWEAK GPU0_XAQ2_INTR_IRQHandler
        PUBWEAK GPU1_XAQ2_INTR_IRQHandler
        PUBWEAK DMA_EDMA0_INT_IRQHandler
        PUBWEAK DMA_EDMA0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EDMA0_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EDMA0_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EDMA0_ERR_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EDMA1_INT_IRQHandler
        PUBWEAK DMA_EDMA1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EDMA1_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EDMA1_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EDMA1_ERR_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK HSIO_PCIEA_MSI_CTRL_INT_IRQHandler
        PUBWEAK HSIO_PCIEA_CLK_REQ_INT_IRQHandler
        PUBWEAK HSIO_PCIEA_DMA_INT_IRQHandler
        PUBWEAK HSIO_PCIEA_DMA_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
HSIO_PCIEA_DMA_INT_IRQHandler
        LDR     R0, =HSIO_PCIEA_DMA_INT_DriverIRQHandler
        BX      R0

        PUBWEAK HSIO_PCIEA_INT_D_IRQHandler
        PUBWEAK HSIO_PCIEA_INT_C_IRQHandler
        PUBWEAK HSIO_PCIEA_INT_B_IRQHandler
        PUBWEAK HSIO_PCIEA_INT_A_IRQHandler
        PUBWEAK HSIO_PCIEA_SMLH_REQ_RST_IRQHandler
        PUBWEAK HSIO_PCIEA_GPIO_WAKEUP0_IRQHandler
        PUBWEAK HSIO_PCIEA_GPIO_WAKEUP1_IRQHandler
        PUBWEAK LSIO_GPT0_INT_IRQHandler
        PUBWEAK LSIO_GPT1_INT_IRQHandler
        PUBWEAK LSIO_GPT2_INT_IRQHandler
        PUBWEAK LSIO_GPT3_INT_IRQHandler
        PUBWEAK LSIO_GPT4_INT_IRQHandler
        PUBWEAK LSIO_KPP_INT_IRQHandler
        PUBWEAK Reserved185_IRQHandler
        PUBWEAK Reserved186_IRQHandler
        PUBWEAK HSIO_SATA_INT0_IRQHandler
        PUBWEAK HSIO_SATA_INT2_IRQHandler
        PUBWEAK Reserved189_IRQHandler
        PUBWEAK Reserved190_IRQHandler
        PUBWEAK LSIO_OCTASPI0_INT_IRQHandler
        PUBWEAK LSIO_OCTASPI0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LSIO_OCTASPI0_INT_IRQHandler
        LDR     R0, =LSIO_OCTASPI0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK LSIO_OCTASPI1_INT_IRQHandler
        PUBWEAK LSIO_OCTASPI1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LSIO_OCTASPI1_INT_IRQHandler
        LDR     R0, =LSIO_OCTASPI1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK LSIO_PWM0_INT_IRQHandler
        PUBWEAK LSIO_PWM1_INT_IRQHandler
        PUBWEAK LSIO_PWM2_INT_IRQHandler
        PUBWEAK LSIO_PWM3_INT_IRQHandler
        PUBWEAK LSIO_PWM4_INT_IRQHandler
        PUBWEAK LSIO_PWM5_INT_IRQHandler
        PUBWEAK LSIO_PWM6_INT_IRQHandler
        PUBWEAK LSIO_PWM7_INT_IRQHandler
        PUBWEAK HSIO_PCIEB_MSI_CTRL_INT_IRQHandler
        PUBWEAK HSIO_PCIEB_CLK_REQ_INT_IRQHandler
        PUBWEAK HSIO_PCIEB_DMA_INT_IRQHandler
        PUBWEAK HSIO_PCIEB_DMA_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
HSIO_PCIEB_DMA_INT_IRQHandler
        LDR     R0, =HSIO_PCIEB_DMA_INT_DriverIRQHandler
        BX      R0

        PUBWEAK HSIO_PCIEB_INT_D_IRQHandler
        PUBWEAK HSIO_PCIEB_INT_C_IRQHandler
        PUBWEAK HSIO_PCIEB_INT_B_IRQHandler
        PUBWEAK HSIO_PCIEB_INT_A_IRQHandler
        PUBWEAK HSIO_PCIEB_SMLH_REQ_RST_IRQHandler
        PUBWEAK HSIO_PCIEB_GPIO_WAKEUP0_IRQHandler
        PUBWEAK HSIO_PCIEB_GPIO_WAKEUP1_IRQHandler
        PUBWEAK SCU_INT_OUT0_IRQHandler
        PUBWEAK SCU_INT_OUT1_IRQHandler
        PUBWEAK SCU_INT_OUT2_IRQHandler
        PUBWEAK SCU_INT_OUT3_IRQHandler
        PUBWEAK SCU_INT_OUT4_IRQHandler
        PUBWEAK SCU_INT_OUT5_IRQHandler
        PUBWEAK SCU_INT_OUT6_IRQHandler
        PUBWEAK SCU_INT_OUT7_IRQHandler
        PUBWEAK SCU_SYS_COUNT_INT0_IRQHandler
        PUBWEAK SCU_SYS_COUNT_INT1_IRQHandler
        PUBWEAK SCU_SYS_COUNT_INT2_IRQHandler
        PUBWEAK SCU_SYS_COUNT_INT3_IRQHandler
        PUBWEAK Reserved223_IRQHandler
        PUBWEAK Reserved224_IRQHandler
        PUBWEAK Reserved225_IRQHandler
        PUBWEAK Reserved226_IRQHandler
        PUBWEAK DRC0_DFI_ALERT_ERR_IRQHandler
        PUBWEAK DRC1_DFI_ALERT_ERR_IRQHandler
        PUBWEAK DRC0_PERF_CNT_FULL_IRQHandler
        PUBWEAK DRC1_PERF_CNT_FULL_IRQHandler
        PUBWEAK Reserved231_IRQHandler
        PUBWEAK Reserved232_IRQHandler
        PUBWEAK Reserved233_IRQHandler
        PUBWEAK Reserved234_IRQHandler
        PUBWEAK LSIO_GPIO_INT0_IRQHandler
        PUBWEAK LSIO_GPIO_INT1_IRQHandler
        PUBWEAK LSIO_GPIO_INT2_IRQHandler
        PUBWEAK LSIO_GPIO_INT3_IRQHandler
        PUBWEAK LSIO_GPIO_INT4_IRQHandler
        PUBWEAK LSIO_GPIO_INT5_IRQHandler
        PUBWEAK LSIO_GPIO_INT6_IRQHandler
        PUBWEAK LSIO_GPIO_INT7_IRQHandler
        PUBWEAK Reserved243_IRQHandler
        PUBWEAK Reserved244_IRQHandler
        PUBWEAK Reserved245_IRQHandler
        PUBWEAK Reserved246_IRQHandler
        PUBWEAK Reserved247_IRQHandler
        PUBWEAK Reserved248_IRQHandler
        PUBWEAK Reserved249_IRQHandler
        PUBWEAK Reserved250_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT0_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT1_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT2_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT3_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT4_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT5_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT6_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT7_IRQHandler
        PUBWEAK DISPLAY1_RESERVED_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT9_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT10_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT11_IRQHandler
        PUBWEAK DISPLAY1_INT_OUT12_IRQHandler
        PUBWEAK Reserved264_IRQHandler
        PUBWEAK Reserved265_IRQHandler
        PUBWEAK Reserved266_IRQHandler
        PUBWEAK VPU_SYS_INT0_IRQHandler
        PUBWEAK VPU_SYS_INT1_IRQHandler
        PUBWEAK Reserved269_IRQHandler
        PUBWEAK Reserved270_IRQHandler
        PUBWEAK Reserved271_IRQHandler
        PUBWEAK Reserved272_IRQHandler
        PUBWEAK Reserved273_IRQHandler
        PUBWEAK Reserved274_IRQHandler
        PUBWEAK LSIO_MU0_INT_IRQHandler
        PUBWEAK LSIO_MU1_INT_IRQHandler
        PUBWEAK LSIO_MU2_INT_IRQHandler
        PUBWEAK LSIO_MU3_INT_IRQHandler
        PUBWEAK LSIO_MU4_INT_IRQHandler
        PUBWEAK Reserved280_IRQHandler
        PUBWEAK Reserved281_IRQHandler
        PUBWEAK Reserved282_IRQHandler
        PUBWEAK LSIO_MU5_INT_A_IRQHandler
        PUBWEAK LSIO_MU6_INT_A_IRQHandler
        PUBWEAK LSIO_MU7_INT_A_IRQHandler
        PUBWEAK LSIO_MU8_INT_A_IRQHandler
        PUBWEAK LSIO_MU9_INT_A_IRQHandler
        PUBWEAK LSIO_MU10_INT_A_IRQHandler
        PUBWEAK LSIO_MU11_INT_A_IRQHandler
        PUBWEAK LSIO_MU12_INT_A_IRQHandler
        PUBWEAK LSIO_MU13_INT_A_IRQHandler
        PUBWEAK Reserved292_IRQHandler
        PUBWEAK Reserved293_IRQHandler
        PUBWEAK Reserved294_IRQHandler
        PUBWEAK Reserved295_IRQHandler
        PUBWEAK Reserved296_IRQHandler
        PUBWEAK Reserved297_IRQHandler
        PUBWEAK Reserved298_IRQHandler
        PUBWEAK LSIO_MU5_INT_B_IRQHandler
        PUBWEAK LSIO_MU6_INT_B_IRQHandler
        PUBWEAK LSIO_MU7_INT_B_IRQHandler
        PUBWEAK LSIO_MU8_INT_B_IRQHandler
        PUBWEAK LSIO_MU9_INT_B_IRQHandler
        PUBWEAK LSIO_MU10_INT_B_IRQHandler
        PUBWEAK LSIO_MU11_INT_B_IRQHandler
        PUBWEAK LSIO_MU12_INT_B_IRQHandler
        PUBWEAK LSIO_MU13_INT_B_IRQHandler
        PUBWEAK Reserved308_IRQHandler
        PUBWEAK Reserved309_IRQHandler
        PUBWEAK Reserved310_IRQHandler
        PUBWEAK Reserved311_IRQHandler
        PUBWEAK Reserved312_IRQHandler
        PUBWEAK Reserved313_IRQHandler
        PUBWEAK Reserved314_IRQHandler
        PUBWEAK DMA_SPI0_INT_IRQHandler
        PUBWEAK DMA_SPI0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI0_INT_IRQHandler
        LDR     R0, =DMA_SPI0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI1_INT_IRQHandler
        PUBWEAK DMA_SPI1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI1_INT_IRQHandler
        LDR     R0, =DMA_SPI1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI2_INT_IRQHandler
        PUBWEAK DMA_SPI2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI2_INT_IRQHandler
        LDR     R0, =DMA_SPI2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI3_INT_IRQHandler
        PUBWEAK DMA_SPI3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI3_INT_IRQHandler
        LDR     R0, =DMA_SPI3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C0_INT_IRQHandler
        PUBWEAK DMA_I2C0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C0_INT_IRQHandler
        LDR     R0, =DMA_I2C0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C1_INT_IRQHandler
        PUBWEAK DMA_I2C1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C1_INT_IRQHandler
        LDR     R0, =DMA_I2C1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C2_INT_IRQHandler
        PUBWEAK DMA_I2C2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C2_INT_IRQHandler
        LDR     R0, =DMA_I2C2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C3_INT_IRQHandler
        PUBWEAK DMA_I2C3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C3_INT_IRQHandler
        LDR     R0, =DMA_I2C3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C4_INT_IRQHandler
        PUBWEAK DMA_I2C4_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C4_INT_IRQHandler
        LDR     R0, =DMA_I2C4_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART0_INT_IRQHandler
        PUBWEAK DMA_UART0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART0_INT_IRQHandler
        LDR     R0, =DMA_UART0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART1_INT_IRQHandler
        PUBWEAK DMA_UART1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART1_INT_IRQHandler
        LDR     R0, =DMA_UART1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART2_INT_IRQHandler
        PUBWEAK DMA_UART2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART2_INT_IRQHandler
        LDR     R0, =DMA_UART2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART3_INT_IRQHandler
        PUBWEAK DMA_UART3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART3_INT_IRQHandler
        LDR     R0, =DMA_UART3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART4_INT_IRQHandler
        PUBWEAK DMA_UART4_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART4_INT_IRQHandler
        LDR     R0, =DMA_UART4_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SIM0_INT_IRQHandler
        PUBWEAK DMA_SIM0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SIM0_INT_IRQHandler
        LDR     R0, =DMA_SIM0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SIM1_INT_IRQHandler
        PUBWEAK DMA_SIM1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SIM1_INT_IRQHandler
        LDR     R0, =DMA_SIM1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_USDHC0_INT_IRQHandler
        PUBWEAK CONNECTIVITY_USDHC0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_USDHC0_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_USDHC0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_USDHC1_INT_IRQHandler
        PUBWEAK CONNECTIVITY_USDHC1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_USDHC1_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_USDHC1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_USDHC2_INT_IRQHandler
        PUBWEAK CONNECTIVITY_USDHC2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_USDHC2_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_USDHC2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN0_INT_IRQHandler
        PUBWEAK DMA_FLEXCAN0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN0_INT_IRQHandler
        LDR     R0, =DMA_FLEXCAN0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN1_INT_IRQHandler
        PUBWEAK DMA_FLEXCAN1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN1_INT_IRQHandler
        LDR     R0, =DMA_FLEXCAN1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN2_INT_IRQHandler
        PUBWEAK DMA_FLEXCAN2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN2_INT_IRQHandler
        LDR     R0, =DMA_FLEXCAN2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FTM0_INT_IRQHandler
        PUBWEAK DMA_FTM0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FTM0_INT_IRQHandler
        LDR     R0, =DMA_FTM0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FTM1_INT_IRQHandler
        PUBWEAK DMA_FTM1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FTM1_INT_IRQHandler
        LDR     R0, =DMA_FTM1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_ADC0_INT_IRQHandler
        PUBWEAK DMA_ADC0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_ADC0_INT_IRQHandler
        LDR     R0, =DMA_ADC0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_ADC1_INT_IRQHandler
        PUBWEAK DMA_ADC1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_ADC1_INT_IRQHandler
        LDR     R0, =DMA_ADC1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EXTERNAL_DMA_INT_0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EXTERNAL_DMA_INT_0_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EXTERNAL_DMA_INT_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EXTERNAL_DMA_INT_1_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EXTERNAL_DMA_INT_2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EXTERNAL_DMA_INT_2_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EXTERNAL_DMA_INT_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EXTERNAL_DMA_INT_3_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EXTERNAL_DMA_INT_4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EXTERNAL_DMA_INT_4_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_EXTERNAL_DMA_INT_5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_EXTERNAL_DMA_INT_5_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved347_IRQHandler
        PUBWEAK Reserved348_IRQHandler
        PUBWEAK Reserved349_IRQHandler
        PUBWEAK Reserved350_IRQHandler
        PUBWEAK Reserved351_IRQHandler
        PUBWEAK Reserved352_IRQHandler
        PUBWEAK Reserved353_IRQHandler
        PUBWEAK Reserved354_IRQHandler
        PUBWEAK CONNECTIVITY_ENET0_FRAME1_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ENET0_FRAME1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_ENET0_FRAME1_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_ENET0_FRAME1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_ENET0_FRAME2_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ENET0_FRAME2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_ENET0_FRAME2_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_ENET0_FRAME2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_ENET0_FRAME0_EVENT_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ENET0_FRAME0_EVENT_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_ENET0_FRAME0_EVENT_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_ENET0_FRAME0_EVENT_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_ENET0_TIMER_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ENET0_TIMER_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_ENET0_TIMER_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_ENET0_TIMER_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_ENET1_FRAME1_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ENET1_FRAME1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_ENET1_FRAME1_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_ENET1_FRAME1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_ENET1_FRAME2_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ENET1_FRAME2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_ENET1_FRAME2_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_ENET1_FRAME2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_ENET1_FRAME0_EVENT_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ENET1_FRAME0_EVENT_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_ENET1_FRAME0_EVENT_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_ENET1_FRAME0_EVENT_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_ENET1_TIMER_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ENET1_TIMER_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_ENET1_TIMER_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_ENET1_TIMER_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_DTCP_INT_IRQHandler
        PUBWEAK CONNECTIVITY_MLB_INT_IRQHandler
        PUBWEAK CONNECTIVITY_MLB_AHB_INT_IRQHandler
        PUBWEAK CONNECTIVITY_USB_OTG_INT_IRQHandler
        PUBWEAK CONNECTIVITY_USB_HOST_INT_IRQHandler
        PUBWEAK CONNECTIVITY_UTMI_INT_IRQHandler
        PUBWEAK CONNECTIVITY_WAKEUP_INT_IRQHandler
        PUBWEAK CONNECTIVITY_USB3_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ND_FLASH_BCH_INT_IRQHandler
        PUBWEAK CONNECTIVITY_ND_FLASH_GPMI_INT_IRQHandler
        PUBWEAK CONNECTIVITY_APBHDMA_IRQHandler
        PUBWEAK CONNECTIVITY_APBHDMA_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_APBHDMA_IRQHandler
        LDR     R0, =CONNECTIVITY_APBHDMA_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_DMA_INT_IRQHandler
        PUBWEAK CONNECTIVITY_DMA_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_DMA_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_DMA_INT_DriverIRQHandler
        BX      R0

        PUBWEAK CONNECTIVITY_DMA_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CONNECTIVITY_DMA_ERR_INT_IRQHandler
        LDR     R0, =CONNECTIVITY_DMA_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved376_IRQHandler
        PUBWEAK Reserved377_IRQHandler
        PUBWEAK Reserved378_IRQHandler
        PUBWEAK Reserved379_IRQHandler
        PUBWEAK Reserved380_IRQHandler
        PUBWEAK Reserved381_IRQHandler
        PUBWEAK Reserved382_IRQHandler
        PUBWEAK Reserved383_IRQHandler
        PUBWEAK Reserved384_IRQHandler
        PUBWEAK Reserved385_IRQHandler
        PUBWEAK Reserved386_IRQHandler
        PUBWEAK IMAGING_MSI_INT_IRQHandler
        PUBWEAK Reserved388_IRQHandler
        PUBWEAK Reserved389_IRQHandler
        PUBWEAK Reserved390_IRQHandler
        PUBWEAK Reserved391_IRQHandler
        PUBWEAK Reserved392_IRQHandler
        PUBWEAK Reserved393_IRQHandler
        PUBWEAK Reserved394_IRQHandler
        PUBWEAK Reserved395_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM0_INT_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IMAGING_PDMA_STREAM0_INT_IRQHandler
        LDR     R0, =IMAGING_PDMA_STREAM0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK IMAGING_PDMA_STREAM1_INT_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IMAGING_PDMA_STREAM1_INT_IRQHandler
        LDR     R0, =IMAGING_PDMA_STREAM1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK IMAGING_PDMA_STREAM2_INT_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IMAGING_PDMA_STREAM2_INT_IRQHandler
        LDR     R0, =IMAGING_PDMA_STREAM2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK IMAGING_PDMA_STREAM3_INT_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IMAGING_PDMA_STREAM3_INT_IRQHandler
        LDR     R0, =IMAGING_PDMA_STREAM3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK IMAGING_PDMA_STREAM4_INT_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM4_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IMAGING_PDMA_STREAM4_INT_IRQHandler
        LDR     R0, =IMAGING_PDMA_STREAM4_INT_DriverIRQHandler
        BX      R0

        PUBWEAK IMAGING_PDMA_STREAM5_INT_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM5_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IMAGING_PDMA_STREAM5_INT_IRQHandler
        LDR     R0, =IMAGING_PDMA_STREAM5_INT_DriverIRQHandler
        BX      R0

        PUBWEAK IMAGING_PDMA_STREAM6_INT_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM6_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IMAGING_PDMA_STREAM6_INT_IRQHandler
        LDR     R0, =IMAGING_PDMA_STREAM6_INT_DriverIRQHandler
        BX      R0

        PUBWEAK IMAGING_PDMA_STREAM7_INT_IRQHandler
        PUBWEAK IMAGING_PDMA_STREAM7_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
IMAGING_PDMA_STREAM7_INT_IRQHandler
        LDR     R0, =IMAGING_PDMA_STREAM7_INT_DriverIRQHandler
        BX      R0

        PUBWEAK IMAGING_MJPEG_ENC0_INT_IRQHandler
        PUBWEAK IMAGING_MJPEG_ENC1_INT_IRQHandler
        PUBWEAK IMAGING_MJPEG_ENC2_INT_IRQHandler
        PUBWEAK IMAGING_MJPEG_ENC3_INT_IRQHandler
        PUBWEAK IMAGING_MJPEG_DEC0_INT_IRQHandler
        PUBWEAK IMAGING_MJPEG_DEC1_INT_IRQHandler
        PUBWEAK IMAGING_MJPEG_DEC2_INT_IRQHandler
        PUBWEAK IMAGING_MJPEG_DEC3_INT_IRQHandler
        PUBWEAK Reserved412_IRQHandler
        PUBWEAK AUDIO_SAI0_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SAI0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI0_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SAI0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI0_DMA_INT_IRQHandler
        PUBWEAK AUDIO_EDMA0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI0_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI1_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SAI1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI1_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SAI1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI1_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI2_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SAI2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI2_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SAI2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI2_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI2_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK MIPI_CSI0_OUT_INT_IRQHandler
        PUBWEAK MIPI_CSI1_OUT_INT_IRQHandler
        PUBWEAK HDMI_RX_OUT_INT_IRQHandler
        PUBWEAK AUDIO_SAI3_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SAI3_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI3_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SAI3_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI3_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI3_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI_HDMI_RX_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SAI_HDMI_RX_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI_HDMI_RX_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SAI_HDMI_RX_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI_HDMI_RX_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI_HDMI_RX_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI_HDMI_TX_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SAI_HDMI_TX_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI_HDMI_TX_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SAI_HDMI_TX_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI_HDMI_TX_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI_HDMI_TX_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI6_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SAI6_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI6_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SAI6_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI6_DMA_INT_IRQHandler
        PUBWEAK AUDIO_EDMA1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI6_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI7_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SAI7_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI7_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SAI7_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI7_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI7_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved432_IRQHandler
        PUBWEAK Reserved433_IRQHandler
        PUBWEAK Reserved434_IRQHandler
        PUBWEAK DMA_SPI0_MOD_INT_IRQHandler
        PUBWEAK DMA_SPI0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI0_MOD_INT_IRQHandler
        LDR     R0, =DMA_SPI0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI1_MOD_INT_IRQHandler
        PUBWEAK DMA_SPI1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI1_MOD_INT_IRQHandler
        LDR     R0, =DMA_SPI1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI2_MOD_INT_IRQHandler
        PUBWEAK DMA_SPI2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI2_MOD_INT_IRQHandler
        LDR     R0, =DMA_SPI2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI3_MOD_INT_IRQHandler
        PUBWEAK DMA_SPI3_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI3_MOD_INT_IRQHandler
        LDR     R0, =DMA_SPI3_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C0_MOD_INT_IRQHandler
        PUBWEAK DMA_I2C0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C0_MOD_INT_IRQHandler
        LDR     R0, =DMA_I2C0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C1_MOD_INT_IRQHandler
        PUBWEAK DMA_I2C1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C1_MOD_INT_IRQHandler
        LDR     R0, =DMA_I2C1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C2_MOD_INT_IRQHandler
        PUBWEAK DMA_I2C2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C2_MOD_INT_IRQHandler
        LDR     R0, =DMA_I2C2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C3_MOD_INT_IRQHandler
        PUBWEAK DMA_I2C3_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C3_MOD_INT_IRQHandler
        LDR     R0, =DMA_I2C3_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C4_MOD_INT_IRQHandler
        PUBWEAK DMA_I2C4_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C4_MOD_INT_IRQHandler
        LDR     R0, =DMA_I2C4_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART0_MOD_INT_IRQHandler
        PUBWEAK DMA_UART0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART0_MOD_INT_IRQHandler
        LDR     R0, =DMA_UART0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART1_MOD_INT_IRQHandler
        PUBWEAK DMA_UART1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART1_MOD_INT_IRQHandler
        LDR     R0, =DMA_UART1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART2_MOD_INT_IRQHandler
        PUBWEAK DMA_UART2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART2_MOD_INT_IRQHandler
        LDR     R0, =DMA_UART2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART3_MOD_INT_IRQHandler
        PUBWEAK DMA_UART3_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART3_MOD_INT_IRQHandler
        LDR     R0, =DMA_UART3_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART4_MOD_INT_IRQHandler
        PUBWEAK DMA_UART4_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART4_MOD_INT_IRQHandler
        LDR     R0, =DMA_UART4_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SIM0_MOD_INT_IRQHandler
        PUBWEAK DMA_SIM0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SIM0_MOD_INT_IRQHandler
        LDR     R0, =DMA_SIM0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SIM1_MOD_INT_IRQHandler
        PUBWEAK DMA_SIM1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SIM1_MOD_INT_IRQHandler
        LDR     R0, =DMA_SIM1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN0_MOD_INT_IRQHandler
        PUBWEAK DMA_FLEXCAN0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN0_MOD_INT_IRQHandler
        LDR     R0, =DMA_FLEXCAN0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN1_MOD_INT_IRQHandler
        PUBWEAK DMA_FLEXCAN1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN1_MOD_INT_IRQHandler
        LDR     R0, =DMA_FLEXCAN1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN2_MOD_INT_IRQHandler
        PUBWEAK DMA_FLEXCAN2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN2_MOD_INT_IRQHandler
        LDR     R0, =DMA_FLEXCAN2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FTM0_MOD_INT_IRQHandler
        PUBWEAK DMA_FTM0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FTM0_MOD_INT_IRQHandler
        LDR     R0, =DMA_FTM0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FTM1_MOD_INT_IRQHandler
        PUBWEAK DMA_FTM1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FTM1_MOD_INT_IRQHandler
        LDR     R0, =DMA_FTM1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_ADC0_MOD_INT_IRQHandler
        PUBWEAK DMA_ADC0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_ADC0_MOD_INT_IRQHandler
        LDR     R0, =DMA_ADC0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_ADC1_MOD_INT_IRQHandler
        PUBWEAK DMA_ADC1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_ADC1_MOD_INT_IRQHandler
        LDR     R0, =DMA_ADC1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN0_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN0_DMA_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN1_DMA_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FLEXCAN2_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FLEXCAN2_DMA_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FTM0_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FTM0_DMA_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_FTM1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_FTM1_DMA_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_ADC0_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_ADC0_DMA_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_ADC1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_ADC1_DMA_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved465_IRQHandler
        PUBWEAK Reserved466_IRQHandler
        PUBWEAK AUDIO_EDMA0_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_EDMA0_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_EDMA0_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_EDMA0_ERR_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_EDMA1_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_EDMA1_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_EDMA1_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_EDMA1_ERR_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_ASRC0_INT1_IRQHandler
        PUBWEAK AUDIO_ASRC0_INT2_IRQHandler
        PUBWEAK AUDIO_DMA0_CH0_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA0_CH0_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA0_CH1_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA0_CH1_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA0_CH2_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA0_CH2_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA0_CH3_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA0_CH3_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA0_CH4_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA0_CH4_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA0_CH5_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA0_CH5_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_ASRC1_INT1_IRQHandler
        PUBWEAK AUDIO_ASRC1_INT2_IRQHandler
        PUBWEAK AUDIO_DMA1_CH0_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA1_CH0_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA1_CH1_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA1_CH1_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA1_CH2_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA1_CH2_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA1_CH3_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA1_CH3_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA1_CH4_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA1_CH4_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_DMA1_CH5_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_DMA1_CH5_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_ESAI0_INT_IRQHandler
        PUBWEAK AUDIO_ESAI0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_ESAI0_INT_IRQHandler
        LDR     R0, =AUDIO_ESAI0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_ESAI1_INT_IRQHandler
        PUBWEAK AUDIO_ESAI1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_ESAI1_INT_IRQHandler
        LDR     R0, =AUDIO_ESAI1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_UNUSED_IRQHandler
        PUBWEAK AUDIO_GPT0_INT_IRQHandler
        PUBWEAK AUDIO_GPT1_INT_IRQHandler
        PUBWEAK AUDIO_GPT2_INT_IRQHandler
        PUBWEAK AUDIO_GPT3_INT_IRQHandler
        PUBWEAK AUDIO_GPT4_INT_IRQHandler
        PUBWEAK AUDIO_GPT5_INT_IRQHandler
        PUBWEAK AUDIO_SAI0_INT_IRQHandler
        PUBWEAK AUDIO_SAI0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI0_INT_IRQHandler
        LDR     R0, =AUDIO_SAI0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI1_INT_IRQHandler
        PUBWEAK AUDIO_SAI1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI1_INT_IRQHandler
        LDR     R0, =AUDIO_SAI1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI2_INT_IRQHandler
        PUBWEAK AUDIO_SAI2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI2_INT_IRQHandler
        LDR     R0, =AUDIO_SAI2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI3_INT_IRQHandler
        PUBWEAK AUDIO_SAI3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI3_INT_IRQHandler
        LDR     R0, =AUDIO_SAI3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI_HDMI_RX_INT_IRQHandler
        PUBWEAK AUDIO_SAI_HDMI_RX_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI_HDMI_RX_INT_IRQHandler
        LDR     R0, =AUDIO_SAI_HDMI_RX_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI_HDMI_TX_INT_IRQHandler
        PUBWEAK AUDIO_SAI_HDMI_TX_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI_HDMI_TX_INT_IRQHandler
        LDR     R0, =AUDIO_SAI_HDMI_TX_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI6_INT_IRQHandler
        PUBWEAK AUDIO_SAI6_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI6_INT_IRQHandler
        LDR     R0, =AUDIO_SAI6_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SAI7_INT_IRQHandler
        PUBWEAK AUDIO_SAI7_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SAI7_INT_IRQHandler
        LDR     R0, =AUDIO_SAI7_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF0_RX_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF0_RX_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF0_RX_INT_IRQHandler
        LDR     R0, =AUDIO_SPDIF0_RX_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF0_TX_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF0_TX_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF0_TX_INT_IRQHandler
        LDR     R0, =AUDIO_SPDIF0_TX_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF1_RX_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF1_RX_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF1_RX_INT_IRQHandler
        LDR     R0, =AUDIO_SPDIF1_RX_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF1_TX_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF1_TX_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF1_TX_INT_IRQHandler
        LDR     R0, =AUDIO_SPDIF1_TX_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_ESAI0_MOD_INT_IRQHandler
        PUBWEAK AUDIO_ESAI0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_ESAI0_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_ESAI0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_ESAI0_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_ESAI0_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_ESAI1_MOD_INT_IRQHandler
        PUBWEAK AUDIO_ESAI1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_ESAI1_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_ESAI1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_ESAI1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_ESAI1_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved512_IRQHandler
        PUBWEAK Reserved513_IRQHandler
        PUBWEAK Reserved514_IRQHandler
        PUBWEAK DMA_SPI0_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI0_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI0_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI0_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI1_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI1_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI1_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI1_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI2_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI2_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI2_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI2_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI3_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI3_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SPI3_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SPI3_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C0_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C0_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C0_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C0_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C1_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C1_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C1_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C1_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C2_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C2_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C2_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C2_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C3_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C3_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C3_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C3_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C4_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C4_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_I2C4_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_I2C4_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART0_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART0_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART0_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART0_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART1_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART1_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART1_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART1_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART2_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART2_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART2_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART2_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART3_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART3_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART3_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART3_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART4_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART4_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_UART4_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_UART4_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SIM0_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SIM0_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SIM0_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SIM0_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SIM1_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SIM1_DMA_RX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_SIM1_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_SIM1_DMA_TX_INT_IRQHandler
        LDR     R0, =DMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK SECURITY_MU1_A_INT_IRQHandler
        PUBWEAK SECURITY_MU2_A_INT_IRQHandler
        PUBWEAK SECURITY_MU3_A_INT_IRQHandler
        PUBWEAK SECURITY_CAAM_INT0_IRQHandler
        PUBWEAK SECURITY_CAAM_INT1_IRQHandler
        PUBWEAK SECURITY_CAAM_INT2_IRQHandler
        PUBWEAK SECURITY_CAAM_INT3_IRQHandler
        PUBWEAK SECURITY_CAAM_RTIC_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF0_RX_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF0_RX_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF0_RX_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SPDIF0_RX_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF0_RX_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF0_RX_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF0_TX_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF0_TX_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF0_TX_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SPDIF0_TX_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF0_TX_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF0_TX_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF1_RX_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF1_RX_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF1_RX_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SPDIF1_RX_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF1_RX_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF1_RX_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF1_TX_MOD_INT_IRQHandler
        PUBWEAK AUDIO_SPDIF1_TX_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF1_TX_MOD_INT_IRQHandler
        LDR     R0, =AUDIO_SPDIF1_TX_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK AUDIO_SPDIF1_TX_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AUDIO_SPDIF1_TX_DMA_INT_IRQHandler
        LDR     R0, =AUDIO_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK VPU_VPU_INT_0_IRQHandler
        PUBWEAK VPU_VPU_INT_1_IRQHandler
        PUBWEAK VPU_VPU_INT_2_IRQHandler
        PUBWEAK VPU_VPU_INT_3_IRQHandler
        PUBWEAK VPU_VPU_INT_4_IRQHandler
        PUBWEAK VPU_VPU_INT_5_IRQHandler
        PUBWEAK VPU_VPU_INT_6_IRQHandler
        PUBWEAK VPU_VPU_INT_7_IRQHandler
        PUBWEAK Reserved571_IRQHandler
        PUBWEAK Reserved572_IRQHandler
        PUBWEAK Reserved573_IRQHandler
        PUBWEAK Reserved574_IRQHandler
        PUBWEAK Reserved575_IRQHandler
        PUBWEAK Reserved576_IRQHandler
        PUBWEAK Reserved577_IRQHandler
        PUBWEAK Reserved578_IRQHandler
        PUBWEAK Reserved579_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_TPM_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_TPM_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_INTMUX_SOURCE_TPM_IRQHandler
        LDR     R0, =M4_1_INTMUX_SOURCE_TPM_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved581_IRQHandler
        PUBWEAK Reserved582_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_LPIT_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_LPIT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_INTMUX_SOURCE_LPIT_IRQHandler
        LDR     R0, =M4_1_INTMUX_SOURCE_LPIT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved584_IRQHandler
        PUBWEAK Reserved585_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_LPUART_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_LPUART_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_INTMUX_SOURCE_LPUART_IRQHandler
        LDR     R0, =M4_1_INTMUX_SOURCE_LPUART_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved587_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_LPI2C_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_LPI2C_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_INTMUX_SOURCE_LPI2C_IRQHandler
        LDR     R0, =M4_1_INTMUX_SOURCE_LPI2C_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved589_IRQHandler
        PUBWEAK Reserved590_IRQHandler
        PUBWEAK Reserved591_IRQHandler
        PUBWEAK Reserved592_IRQHandler
        PUBWEAK Reserved593_IRQHandler
        PUBWEAK Reserved594_IRQHandler
        PUBWEAK Reserved595_IRQHandler
        PUBWEAK Reserved596_IRQHandler
        PUBWEAK Reserved597_IRQHandler
        PUBWEAK Reserved598_IRQHandler
        PUBWEAK Reserved599_IRQHandler
        PUBWEAK Reserved600_IRQHandler
        PUBWEAK Reserved601_IRQHandler
        PUBWEAK Reserved602_IRQHandler
        PUBWEAK Reserved603_IRQHandler
        PUBWEAK Reserved604_IRQHandler
        PUBWEAK Reserved605_IRQHandler
        PUBWEAK Reserved606_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_MU0_A3_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_MU0_A3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_INTMUX_SOURCE_MU0_A3_IRQHandler
        LDR     R0, =M4_1_INTMUX_SOURCE_MU0_A3_DriverIRQHandler
        BX      R0

        PUBWEAK M4_1_INTMUX_SOURCE_MU0_A2_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_MU0_A2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_INTMUX_SOURCE_MU0_A2_IRQHandler
        LDR     R0, =M4_1_INTMUX_SOURCE_MU0_A2_DriverIRQHandler
        BX      R0

        PUBWEAK M4_1_INTMUX_SOURCE_MU0_A1_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_MU0_A1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_INTMUX_SOURCE_MU0_A1_IRQHandler
        LDR     R0, =M4_1_INTMUX_SOURCE_MU0_A1_DriverIRQHandler
        BX      R0

        PUBWEAK M4_1_INTMUX_SOURCE_MU0_A0_IRQHandler
        PUBWEAK M4_1_INTMUX_SOURCE_MU0_A0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_1_INTMUX_SOURCE_MU0_A0_IRQHandler
        LDR     R0, =M4_1_INTMUX_SOURCE_MU0_A0_DriverIRQHandler
        BX      R0

        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved16_IRQHandler
Reserved17_IRQHandler
Reserved18_IRQHandler
Reserved19_IRQHandler
Reserved20_IRQHandler
M4_1_MCM_IRQHandler
Reserved22_IRQHandler
Reserved23_IRQHandler
Reserved24_IRQHandler
Reserved25_IRQHandler
Reserved26_IRQHandler
Reserved27_IRQHandler
Reserved28_IRQHandler
Reserved29_IRQHandler
Reserved30_IRQHandler
Reserved31_IRQHandler
Reserved32_IRQHandler
Reserved33_IRQHandler
Reserved34_IRQHandler
M4_1_TPM_IRQHandler
Reserved36_IRQHandler
Reserved37_IRQHandler
M4_1_LPIT_IRQHandler
Reserved39_IRQHandler
Reserved40_IRQHandler
M4_1_LPUART_DriverIRQHandler
Reserved42_IRQHandler
M4_1_LPI2C_DriverIRQHandler
Reserved44_IRQHandler
M4_1_MU0_B0_IRQHandler
Reserved46_IRQHandler
Reserved47_IRQHandler
IRQSTEER_0_DriverIRQHandler
IRQSTEER_1_DriverIRQHandler
IRQSTEER_2_DriverIRQHandler
IRQSTEER_3_DriverIRQHandler
IRQSTEER_4_DriverIRQHandler
IRQSTEER_5_DriverIRQHandler
IRQSTEER_6_DriverIRQHandler
IRQSTEER_7_DriverIRQHandler
Reserved56_IRQHandler
Reserved57_IRQHandler
Reserved58_IRQHandler
Reserved59_IRQHandler
M4_1_MU0_B1_IRQHandler
M4_1_MU0_B2_IRQHandler
M4_1_MU0_B3_IRQHandler
Reserved63_IRQHandler
Reserved64_IRQHandler
M4_1_MU1_A_IRQHandler
M4_1_SW_IRQHandler
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
Reserved80_IRQHandler
Reserved81_IRQHandler
Reserved82_IRQHandler
Reserved83_IRQHandler
Reserved84_IRQHandler
Reserved85_IRQHandler
Reserved86_IRQHandler
Reserved87_IRQHandler
Reserved88_IRQHandler
Reserved89_IRQHandler
Reserved90_IRQHandler
Reserved91_IRQHandler
Reserved92_IRQHandler
Reserved93_IRQHandler
Reserved94_IRQHandler
Reserved95_IRQHandler
Reserved96_IRQHandler
Reserved97_IRQHandler
Reserved98_IRQHandler
A53_NEXTERRIRQ_IRQHandler
A53_NINTERRIRQ_IRQHandler
A72_NEXTERRIRQ_IRQHandler
A72_NINTERRIRQ_IRQHandler
VPU_NEXTERRIRQ_IRQHandler
Reserved104_IRQHandler
Reserved105_IRQHandler
Reserved106_IRQHandler
CCI_NERRORIRQ_IRQHandler
CCI_NEVNTCNTOVERFLOW0_IRQHandler
CCI_NEVNTCNTOVERFLOW1_IRQHandler
CCI_NEVNTCNTOVERFLOW2_IRQHandler
CCI_NEVNTCNTOVERFLOW3_IRQHandler
CCI_NEVNTCNTOVERFLOW4_IRQHandler
Reserved113_IRQHandler
Reserved114_IRQHandler
M4_0_INT_OUT0_IRQHandler
M4_0_INT_OUT1_IRQHandler
M4_0_INT_OUT2_IRQHandler
M4_0_INT_OUT3_IRQHandler
M4_0_INT_OUT4_IRQHandler
M4_0_INT_OUT5_IRQHandler
M4_0_INT_OUT6_IRQHandler
M4_0_INT_OUT7_IRQHandler
M4_1_INT_OUT0_IRQHandler
M4_1_INT_OUT1_IRQHandler
M4_1_INT_OUT2_IRQHandler
M4_1_INT_OUT3_IRQHandler
M4_1_INT_OUT4_IRQHandler
M4_1_INT_OUT5_IRQHandler
M4_1_INT_OUT6_IRQHandler
M4_1_INT_OUT7_IRQHandler
DBLOG_COMB_IRPT_NS_IRQHandler
DBLOG_COMB_IRPT_S_IRQHandler
DBLOG_GBL_FLT_IRPT_NS_IRQHandler
DBLOG_GBL_FLT_IRPT_S_IRQHandler
DBLOG_PERF_IRPT_IMX8_0_IRQHandler
DBLOG_PERF_IRPT_IMX8_1_IRQHandler
DBLOG_PERF_IRPT_IMX8_2_IRQHandler
DBLOG_PERF_IRPT_IMX8_3_IRQHandler
DISPLAY0_INT_OUT0_IRQHandler
DISPLAY0_INT_OUT1_IRQHandler
DISPLAY0_INT_OUT2_IRQHandler
DISPLAY0_INT_OUT3_IRQHandler
DISPLAY0_INT_OUT4_IRQHandler
DISPLAY0_INT_OUT5_IRQHandler
DISPLAY0_INT_OUT6_IRQHandler
DISPLAY0_INT_OUT7_IRQHandler
DISPLAY0_RESERVED_IRQHandler
DISPLAY0_INT_OUT9_IRQHandler
DISPLAY0_INT_OUT10_IRQHandler
DISPLAY0_INT_OUT11_IRQHandler
DISPLAY0_INT_OUT12_IRQHandler
Reserved152_IRQHandler
Reserved153_IRQHandler
Reserved154_IRQHandler
Reserved155_IRQHandler
LVDS0_INT_OUT_IRQHandler
LVDS1_INT_OUT_IRQHandler
MIPI_DSI0_INT_OUT_IRQHandler
MIPI_DSI1_INT_OUT_IRQHandler
HDMI_TX_INT_OUT_IRQHandler
Reserved161_IRQHandler
Reserved162_IRQHandler
GPU0_XAQ2_INTR_IRQHandler
GPU1_XAQ2_INTR_IRQHandler
DMA_EDMA0_INT_DriverIRQHandler
DMA_EDMA1_INT_DriverIRQHandler
HSIO_PCIEA_MSI_CTRL_INT_IRQHandler
HSIO_PCIEA_CLK_REQ_INT_IRQHandler
HSIO_PCIEA_DMA_INT_DriverIRQHandler
HSIO_PCIEA_INT_D_IRQHandler
HSIO_PCIEA_INT_C_IRQHandler
HSIO_PCIEA_INT_B_IRQHandler
HSIO_PCIEA_INT_A_IRQHandler
HSIO_PCIEA_SMLH_REQ_RST_IRQHandler
HSIO_PCIEA_GPIO_WAKEUP0_IRQHandler
HSIO_PCIEA_GPIO_WAKEUP1_IRQHandler
LSIO_GPT0_INT_IRQHandler
LSIO_GPT1_INT_IRQHandler
LSIO_GPT2_INT_IRQHandler
LSIO_GPT3_INT_IRQHandler
LSIO_GPT4_INT_IRQHandler
LSIO_KPP_INT_IRQHandler
Reserved185_IRQHandler
Reserved186_IRQHandler
HSIO_SATA_INT0_IRQHandler
HSIO_SATA_INT2_IRQHandler
Reserved189_IRQHandler
Reserved190_IRQHandler
LSIO_OCTASPI0_INT_DriverIRQHandler
LSIO_OCTASPI1_INT_DriverIRQHandler
LSIO_PWM0_INT_IRQHandler
LSIO_PWM1_INT_IRQHandler
LSIO_PWM2_INT_IRQHandler
LSIO_PWM3_INT_IRQHandler
LSIO_PWM4_INT_IRQHandler
LSIO_PWM5_INT_IRQHandler
LSIO_PWM6_INT_IRQHandler
LSIO_PWM7_INT_IRQHandler
HSIO_PCIEB_MSI_CTRL_INT_IRQHandler
HSIO_PCIEB_CLK_REQ_INT_IRQHandler
HSIO_PCIEB_DMA_INT_DriverIRQHandler
HSIO_PCIEB_INT_D_IRQHandler
HSIO_PCIEB_INT_C_IRQHandler
HSIO_PCIEB_INT_B_IRQHandler
HSIO_PCIEB_INT_A_IRQHandler
HSIO_PCIEB_SMLH_REQ_RST_IRQHandler
HSIO_PCIEB_GPIO_WAKEUP0_IRQHandler
HSIO_PCIEB_GPIO_WAKEUP1_IRQHandler
SCU_INT_OUT0_IRQHandler
SCU_INT_OUT1_IRQHandler
SCU_INT_OUT2_IRQHandler
SCU_INT_OUT3_IRQHandler
SCU_INT_OUT4_IRQHandler
SCU_INT_OUT5_IRQHandler
SCU_INT_OUT6_IRQHandler
SCU_INT_OUT7_IRQHandler
SCU_SYS_COUNT_INT0_IRQHandler
SCU_SYS_COUNT_INT1_IRQHandler
SCU_SYS_COUNT_INT2_IRQHandler
SCU_SYS_COUNT_INT3_IRQHandler
Reserved223_IRQHandler
Reserved224_IRQHandler
Reserved225_IRQHandler
Reserved226_IRQHandler
DRC0_DFI_ALERT_ERR_IRQHandler
DRC1_DFI_ALERT_ERR_IRQHandler
DRC0_PERF_CNT_FULL_IRQHandler
DRC1_PERF_CNT_FULL_IRQHandler
Reserved231_IRQHandler
Reserved232_IRQHandler
Reserved233_IRQHandler
Reserved234_IRQHandler
LSIO_GPIO_INT0_IRQHandler
LSIO_GPIO_INT1_IRQHandler
LSIO_GPIO_INT2_IRQHandler
LSIO_GPIO_INT3_IRQHandler
LSIO_GPIO_INT4_IRQHandler
LSIO_GPIO_INT5_IRQHandler
LSIO_GPIO_INT6_IRQHandler
LSIO_GPIO_INT7_IRQHandler
Reserved243_IRQHandler
Reserved244_IRQHandler
Reserved245_IRQHandler
Reserved246_IRQHandler
Reserved247_IRQHandler
Reserved248_IRQHandler
Reserved249_IRQHandler
Reserved250_IRQHandler
DISPLAY1_INT_OUT0_IRQHandler
DISPLAY1_INT_OUT1_IRQHandler
DISPLAY1_INT_OUT2_IRQHandler
DISPLAY1_INT_OUT3_IRQHandler
DISPLAY1_INT_OUT4_IRQHandler
DISPLAY1_INT_OUT5_IRQHandler
DISPLAY1_INT_OUT6_IRQHandler
DISPLAY1_INT_OUT7_IRQHandler
DISPLAY1_RESERVED_IRQHandler
DISPLAY1_INT_OUT9_IRQHandler
DISPLAY1_INT_OUT10_IRQHandler
DISPLAY1_INT_OUT11_IRQHandler
DISPLAY1_INT_OUT12_IRQHandler
Reserved264_IRQHandler
Reserved265_IRQHandler
Reserved266_IRQHandler
VPU_SYS_INT0_IRQHandler
VPU_SYS_INT1_IRQHandler
Reserved269_IRQHandler
Reserved270_IRQHandler
Reserved271_IRQHandler
Reserved272_IRQHandler
Reserved273_IRQHandler
Reserved274_IRQHandler
LSIO_MU0_INT_IRQHandler
LSIO_MU1_INT_IRQHandler
LSIO_MU2_INT_IRQHandler
LSIO_MU3_INT_IRQHandler
LSIO_MU4_INT_IRQHandler
Reserved280_IRQHandler
Reserved281_IRQHandler
Reserved282_IRQHandler
LSIO_MU5_INT_A_IRQHandler
LSIO_MU6_INT_A_IRQHandler
LSIO_MU7_INT_A_IRQHandler
LSIO_MU8_INT_A_IRQHandler
LSIO_MU9_INT_A_IRQHandler
LSIO_MU10_INT_A_IRQHandler
LSIO_MU11_INT_A_IRQHandler
LSIO_MU12_INT_A_IRQHandler
LSIO_MU13_INT_A_IRQHandler
Reserved292_IRQHandler
Reserved293_IRQHandler
Reserved294_IRQHandler
Reserved295_IRQHandler
Reserved296_IRQHandler
Reserved297_IRQHandler
Reserved298_IRQHandler
LSIO_MU5_INT_B_IRQHandler
LSIO_MU6_INT_B_IRQHandler
LSIO_MU7_INT_B_IRQHandler
LSIO_MU8_INT_B_IRQHandler
LSIO_MU9_INT_B_IRQHandler
LSIO_MU10_INT_B_IRQHandler
LSIO_MU11_INT_B_IRQHandler
LSIO_MU12_INT_B_IRQHandler
LSIO_MU13_INT_B_IRQHandler
Reserved308_IRQHandler
Reserved309_IRQHandler
Reserved310_IRQHandler
Reserved311_IRQHandler
Reserved312_IRQHandler
Reserved313_IRQHandler
Reserved314_IRQHandler
DMA_SPI0_INT_DriverIRQHandler
DMA_SPI1_INT_DriverIRQHandler
DMA_SPI2_INT_DriverIRQHandler
DMA_SPI3_INT_DriverIRQHandler
DMA_I2C0_INT_DriverIRQHandler
DMA_I2C1_INT_DriverIRQHandler
DMA_I2C2_INT_DriverIRQHandler
DMA_I2C3_INT_DriverIRQHandler
DMA_I2C4_INT_DriverIRQHandler
DMA_UART0_INT_DriverIRQHandler
DMA_UART1_INT_DriverIRQHandler
DMA_UART2_INT_DriverIRQHandler
DMA_UART3_INT_DriverIRQHandler
DMA_UART4_INT_DriverIRQHandler
DMA_SIM0_INT_DriverIRQHandler
DMA_SIM1_INT_DriverIRQHandler
CONNECTIVITY_USDHC0_INT_DriverIRQHandler
CONNECTIVITY_USDHC1_INT_DriverIRQHandler
CONNECTIVITY_USDHC2_INT_DriverIRQHandler
DMA_FLEXCAN0_INT_DriverIRQHandler
DMA_FLEXCAN1_INT_DriverIRQHandler
DMA_FLEXCAN2_INT_DriverIRQHandler
DMA_FTM0_INT_DriverIRQHandler
DMA_FTM1_INT_DriverIRQHandler
DMA_ADC0_INT_DriverIRQHandler
DMA_ADC1_INT_DriverIRQHandler
Reserved347_IRQHandler
Reserved348_IRQHandler
Reserved349_IRQHandler
Reserved350_IRQHandler
Reserved351_IRQHandler
Reserved352_IRQHandler
Reserved353_IRQHandler
Reserved354_IRQHandler
CONNECTIVITY_ENET0_FRAME1_INT_DriverIRQHandler
CONNECTIVITY_ENET0_FRAME2_INT_DriverIRQHandler
CONNECTIVITY_ENET0_FRAME0_EVENT_INT_DriverIRQHandler
CONNECTIVITY_ENET0_TIMER_INT_DriverIRQHandler
CONNECTIVITY_ENET1_FRAME1_INT_DriverIRQHandler
CONNECTIVITY_ENET1_FRAME2_INT_DriverIRQHandler
CONNECTIVITY_ENET1_FRAME0_EVENT_INT_DriverIRQHandler
CONNECTIVITY_ENET1_TIMER_INT_DriverIRQHandler
CONNECTIVITY_DTCP_INT_IRQHandler
CONNECTIVITY_MLB_INT_IRQHandler
CONNECTIVITY_MLB_AHB_INT_IRQHandler
CONNECTIVITY_USB_OTG_INT_IRQHandler
CONNECTIVITY_USB_HOST_INT_IRQHandler
CONNECTIVITY_UTMI_INT_IRQHandler
CONNECTIVITY_WAKEUP_INT_IRQHandler
CONNECTIVITY_USB3_INT_IRQHandler
CONNECTIVITY_ND_FLASH_BCH_INT_IRQHandler
CONNECTIVITY_ND_FLASH_GPMI_INT_IRQHandler
CONNECTIVITY_APBHDMA_DriverIRQHandler
CONNECTIVITY_DMA_INT_DriverIRQHandler
Reserved376_IRQHandler
Reserved377_IRQHandler
Reserved378_IRQHandler
Reserved379_IRQHandler
Reserved380_IRQHandler
Reserved381_IRQHandler
Reserved382_IRQHandler
Reserved383_IRQHandler
Reserved384_IRQHandler
Reserved385_IRQHandler
Reserved386_IRQHandler
IMAGING_MSI_INT_IRQHandler
Reserved388_IRQHandler
Reserved389_IRQHandler
Reserved390_IRQHandler
Reserved391_IRQHandler
Reserved392_IRQHandler
Reserved393_IRQHandler
Reserved394_IRQHandler
Reserved395_IRQHandler
IMAGING_PDMA_STREAM0_INT_DriverIRQHandler
IMAGING_PDMA_STREAM1_INT_DriverIRQHandler
IMAGING_PDMA_STREAM2_INT_DriverIRQHandler
IMAGING_PDMA_STREAM3_INT_DriverIRQHandler
IMAGING_PDMA_STREAM4_INT_DriverIRQHandler
IMAGING_PDMA_STREAM5_INT_DriverIRQHandler
IMAGING_PDMA_STREAM6_INT_DriverIRQHandler
IMAGING_PDMA_STREAM7_INT_DriverIRQHandler
IMAGING_MJPEG_ENC0_INT_IRQHandler
IMAGING_MJPEG_ENC1_INT_IRQHandler
IMAGING_MJPEG_ENC2_INT_IRQHandler
IMAGING_MJPEG_ENC3_INT_IRQHandler
IMAGING_MJPEG_DEC0_INT_IRQHandler
IMAGING_MJPEG_DEC1_INT_IRQHandler
IMAGING_MJPEG_DEC2_INT_IRQHandler
IMAGING_MJPEG_DEC3_INT_IRQHandler
Reserved412_IRQHandler
AUDIO_SAI0_MOD_INT_DriverIRQHandler
AUDIO_EDMA0_INT_DriverIRQHandler
AUDIO_SAI1_MOD_INT_DriverIRQHandler
AUDIO_SAI2_MOD_INT_DriverIRQHandler
MIPI_CSI0_OUT_INT_IRQHandler
MIPI_CSI1_OUT_INT_IRQHandler
HDMI_RX_OUT_INT_IRQHandler
AUDIO_SAI3_MOD_INT_DriverIRQHandler
AUDIO_SAI_HDMI_RX_MOD_INT_DriverIRQHandler
AUDIO_SAI_HDMI_TX_MOD_INT_DriverIRQHandler
AUDIO_SAI6_MOD_INT_DriverIRQHandler
AUDIO_EDMA1_INT_DriverIRQHandler
AUDIO_SAI7_MOD_INT_DriverIRQHandler
Reserved432_IRQHandler
Reserved433_IRQHandler
Reserved434_IRQHandler
DMA_SPI0_MOD_INT_DriverIRQHandler
DMA_SPI1_MOD_INT_DriverIRQHandler
DMA_SPI2_MOD_INT_DriverIRQHandler
DMA_SPI3_MOD_INT_DriverIRQHandler
DMA_I2C0_MOD_INT_DriverIRQHandler
DMA_I2C1_MOD_INT_DriverIRQHandler
DMA_I2C2_MOD_INT_DriverIRQHandler
DMA_I2C3_MOD_INT_DriverIRQHandler
DMA_I2C4_MOD_INT_DriverIRQHandler
DMA_UART0_MOD_INT_DriverIRQHandler
DMA_UART1_MOD_INT_DriverIRQHandler
DMA_UART2_MOD_INT_DriverIRQHandler
DMA_UART3_MOD_INT_DriverIRQHandler
DMA_UART4_MOD_INT_DriverIRQHandler
DMA_SIM0_MOD_INT_DriverIRQHandler
DMA_SIM1_MOD_INT_DriverIRQHandler
DMA_FLEXCAN0_MOD_INT_DriverIRQHandler
DMA_FLEXCAN1_MOD_INT_DriverIRQHandler
DMA_FLEXCAN2_MOD_INT_DriverIRQHandler
DMA_FTM0_MOD_INT_DriverIRQHandler
DMA_FTM1_MOD_INT_DriverIRQHandler
DMA_ADC0_MOD_INT_DriverIRQHandler
DMA_ADC1_MOD_INT_DriverIRQHandler
Reserved465_IRQHandler
Reserved466_IRQHandler
AUDIO_ASRC0_INT1_IRQHandler
AUDIO_ASRC0_INT2_IRQHandler
AUDIO_ASRC1_INT1_IRQHandler
AUDIO_ASRC1_INT2_IRQHandler
AUDIO_ESAI0_INT_DriverIRQHandler
AUDIO_ESAI1_INT_DriverIRQHandler
AUDIO_UNUSED_IRQHandler
AUDIO_GPT0_INT_IRQHandler
AUDIO_GPT1_INT_IRQHandler
AUDIO_GPT2_INT_IRQHandler
AUDIO_GPT3_INT_IRQHandler
AUDIO_GPT4_INT_IRQHandler
AUDIO_GPT5_INT_IRQHandler
AUDIO_SAI0_INT_DriverIRQHandler
AUDIO_SAI1_INT_DriverIRQHandler
AUDIO_SAI2_INT_DriverIRQHandler
AUDIO_SAI3_INT_DriverIRQHandler
AUDIO_SAI_HDMI_RX_INT_DriverIRQHandler
AUDIO_SAI_HDMI_TX_INT_DriverIRQHandler
AUDIO_SAI6_INT_DriverIRQHandler
AUDIO_SAI7_INT_DriverIRQHandler
AUDIO_SPDIF0_RX_INT_DriverIRQHandler
AUDIO_SPDIF0_TX_INT_DriverIRQHandler
AUDIO_SPDIF1_RX_INT_DriverIRQHandler
AUDIO_SPDIF1_TX_INT_DriverIRQHandler
AUDIO_ESAI0_MOD_INT_DriverIRQHandler
AUDIO_ESAI1_MOD_INT_DriverIRQHandler
Reserved512_IRQHandler
Reserved513_IRQHandler
Reserved514_IRQHandler
SECURITY_MU1_A_INT_IRQHandler
SECURITY_MU2_A_INT_IRQHandler
SECURITY_MU3_A_INT_IRQHandler
SECURITY_CAAM_INT0_IRQHandler
SECURITY_CAAM_INT1_IRQHandler
SECURITY_CAAM_INT2_IRQHandler
SECURITY_CAAM_INT3_IRQHandler
SECURITY_CAAM_RTIC_INT_IRQHandler
AUDIO_SPDIF0_RX_MOD_INT_DriverIRQHandler
AUDIO_SPDIF0_TX_MOD_INT_DriverIRQHandler
AUDIO_SPDIF1_RX_MOD_INT_DriverIRQHandler
AUDIO_SPDIF1_TX_MOD_INT_DriverIRQHandler
VPU_VPU_INT_0_IRQHandler
VPU_VPU_INT_1_IRQHandler
VPU_VPU_INT_2_IRQHandler
VPU_VPU_INT_3_IRQHandler
VPU_VPU_INT_4_IRQHandler
VPU_VPU_INT_5_IRQHandler
VPU_VPU_INT_6_IRQHandler
VPU_VPU_INT_7_IRQHandler
Reserved571_IRQHandler
Reserved572_IRQHandler
Reserved573_IRQHandler
Reserved574_IRQHandler
Reserved575_IRQHandler
Reserved576_IRQHandler
Reserved577_IRQHandler
Reserved578_IRQHandler
Reserved579_IRQHandler
M4_1_INTMUX_SOURCE_TPM_DriverIRQHandler
Reserved581_IRQHandler
Reserved582_IRQHandler
M4_1_INTMUX_SOURCE_LPIT_DriverIRQHandler
Reserved584_IRQHandler
Reserved585_IRQHandler
M4_1_INTMUX_SOURCE_LPUART_DriverIRQHandler
Reserved587_IRQHandler
M4_1_INTMUX_SOURCE_LPI2C_DriverIRQHandler
Reserved589_IRQHandler
Reserved590_IRQHandler
Reserved591_IRQHandler
Reserved592_IRQHandler
Reserved593_IRQHandler
Reserved594_IRQHandler
Reserved595_IRQHandler
Reserved596_IRQHandler
Reserved597_IRQHandler
Reserved598_IRQHandler
Reserved599_IRQHandler
Reserved600_IRQHandler
Reserved601_IRQHandler
Reserved602_IRQHandler
Reserved603_IRQHandler
Reserved604_IRQHandler
Reserved605_IRQHandler
Reserved606_IRQHandler
M4_1_INTMUX_SOURCE_MU0_A3_DriverIRQHandler
M4_1_INTMUX_SOURCE_MU0_A2_DriverIRQHandler
M4_1_INTMUX_SOURCE_MU0_A1_DriverIRQHandler
M4_1_INTMUX_SOURCE_MU0_A0_DriverIRQHandler
DefaultISR
        B DefaultISR

        END
