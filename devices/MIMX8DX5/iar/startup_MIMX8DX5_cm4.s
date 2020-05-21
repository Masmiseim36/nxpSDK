; -------------------------------------------------------------------------
;  @file:    startup_MIMX8DX5_cm4.s
;  @purpose: CMSIS Cortex-M4 Core Device Startup File
;            MIMX8DX5_cm4
;  @version: 4.0
;  @date:    2018-8-22
;  @build:   b191202
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
        SECTION HEAP:DATA:NOROOT(3)
        SECTION RW:DATA:NOROOT(2)
        SECTION QACCESS_CODE_VAR:DATA:NOROOT(3)
        SECTION QACCESS_DATA_VAR:DATA:NOROOT(3)

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
        DCD     M4_MCM_IRQHandler                             ;MCM IRQ
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
        DCD     M4_TPM_IRQHandler                             ;Timer PWM Module
        DCD     Reserved36_IRQHandler                         ;Reserved
        DCD     Reserved37_IRQHandler                         ;Reserved
        DCD     M4_LPIT_IRQHandler                            ;Low-Power Periodic Interrupt Timer
        DCD     Reserved39_IRQHandler                         ;Reserved
        DCD     Reserved40_IRQHandler                         ;Reserved
        DCD     M4_LPUART_IRQHandler                          ;Low Power UART
        DCD     Reserved42_IRQHandler                         ;Reserved
        DCD     M4_LPI2C_IRQHandler                           ;Low-Power I2C - Logical OR of master and slave interrupts
        DCD     Reserved44_IRQHandler                         ;Reserved
        DCD     M4_MU0_B0_IRQHandler                          ;Messaging Unit 0 (IPC with other subsystems) - Side B (local), Port 0, Logical OR of all general-purpose, TX, and RX interrupts
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
        DCD     M4_MU0_B1_IRQHandler                          ;Messaging Unit 0 (IPC with other subsystems) - Side B (local), Port 1, Logical OR of all general-purpose, TX, and RX interrupts
        DCD     M4_MU0_B2_IRQHandler                          ;Messaging Unit 0 (IPC with other subsystems) - Side B (local), Port 2, Logical OR of all general-purpose, TX, and RX interrupts
        DCD     M4_MU0_B3_IRQHandler                          ;Messaging Unit 0 (IPC with other subsystems) - Side B (local), Port 3, Logical OR of all general-purpose, TX, and RX interrupts
        DCD     Reserved63_IRQHandler                         ;Reserved
        DCD     Reserved64_IRQHandler                         ;Reserved
        DCD     M4_MU1_A_IRQHandler                           ;Messaging Unit 1 (IPC with System Controller) - Side A (MCU), Logical OR of all general-purpose, TX, and RX interrupts
        DCD     M4_SW_IRQHandler                              ;Software interrupt (asserted/cleared via NVIC registers, INTISR[50] input tied low)
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
        DCD     A35_NEXTERRIRQ_IRQHandler                     ;Shared Int Source nEXTERRIRQ from A35 Sub-System
        DCD     A35_NINTERRIRQ_IRQHandler                     ;Shared Int Source nINTERRIRQ from A35 Sub-System
        DCD     Reserved101_IRQHandler                        ;xxx Interrupt 101
        DCD     Reserved102_IRQHandler                        ;xxx Interrupt 102
        DCD     Reserved103_IRQHandler                        ;xxx Interrupt 103
        DCD     Reserved104_IRQHandler                        ;xxx Interrupt 104
        DCD     Reserved105_IRQHandler                        ;xxx Interrupt 105
        DCD     Reserved106_IRQHandler                        ;xxx Interrupt 106
        DCD     Reserved107_IRQHandler                        ;xxx Interrupt 107
        DCD     Reserved108_IRQHandler                        ;xxx Interrupt 108
        DCD     Reserved109_IRQHandler                        ;xxx Interrupt 109
        DCD     Reserved110_IRQHandler                        ;xxx Interrupt 110
        DCD     Reserved111_IRQHandler                        ;xxx Interrupt 111
        DCD     Reserved112_IRQHandler                        ;xxx Interrupt 112
        DCD     Reserved113_IRQHandler                        ;xxx Interrupt 113
        DCD     Reserved114_IRQHandler                        ;xxx Interrupt 114
        DCD     M4_INT_OUT0_IRQHandler                        ;Shared Int Source INT_OUT[0] from M4 Sub-System
        DCD     M4_INT_OUT1_IRQHandler                        ;Shared Int Source INT_OUT[1] from M4 Sub-System
        DCD     M4_INT_OUT2_IRQHandler                        ;Shared Int Source INT_OUT[2] from M4 Sub-System
        DCD     M4_INT_OUT3_IRQHandler                        ;Shared Int Source INT_OUT[3] from M4 Sub-System
        DCD     M4_INT_OUT4_IRQHandler                        ;Shared Int Source INT_OUT[4] from M4 Sub-System
        DCD     M4_INT_OUT5_IRQHandler                        ;Shared Int Source INT_OUT[5] from M4 Sub-System
        DCD     M4_INT_OUT6_IRQHandler                        ;Shared Int Source INT_OUT[6] from M4 Sub-System
        DCD     M4_INT_OUT7_IRQHandler                        ;Shared Int Source INT_OUT[7] from M4 Sub-System
        DCD     Reserved123_IRQHandler                        ;xxx Interrupt 123
        DCD     Reserved124_IRQHandler                        ;xxx Interrupt 124
        DCD     Reserved125_IRQHandler                        ;xxx Interrupt 125
        DCD     Reserved126_IRQHandler                        ;xxx Interrupt 126
        DCD     Reserved127_IRQHandler                        ;xxx Interrupt 127
        DCD     Reserved128_IRQHandler                        ;xxx Interrupt 128
        DCD     Reserved129_IRQHandler                        ;xxx Interrupt 129
        DCD     Reserved130_IRQHandler                        ;xxx Interrupt 130
        DCD     Reserved131_IRQHandler                        ;xxx Interrupt 131
        DCD     Reserved132_IRQHandler                        ;xxx Interrupt 132
        DCD     Reserved133_IRQHandler                        ;xxx Interrupt 133
        DCD     Reserved134_IRQHandler                        ;xxx Interrupt 134
        DCD     Reserved135_IRQHandler                        ;xxx Interrupt 135
        DCD     Reserved136_IRQHandler                        ;xxx Interrupt 136
        DCD     Reserved137_IRQHandler                        ;xxx Interrupt 137
        DCD     Reserved138_IRQHandler                        ;xxx Interrupt 138
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
        DCD     Reserved156_IRQHandler                        ;xxx Interrupt 156
        DCD     Reserved157_IRQHandler                        ;xxx Interrupt 157
        DCD     MIPI_DSI0_INT_OUT_IRQHandler                  ;Shared Int Source INT_OUT from MIPI_DSI0 Sub-System
        DCD     MIPI_DSI1_INT_OUT_IRQHandler                  ;Shared Int Source INT_OUT from MIPI_DSI1 Sub-System
        DCD     Reserved160_IRQHandler                        ;xxx Interrupt 160
        DCD     LCD_MOD_INT_IRQHandler                        ;Shared Int Source INT_OUT from ADMA Sub-System
        DCD     LCD_PWM_INT_IRQHandler                        ;Shared Int Source INT_OUT from ADMA Sub-System
        DCD     GPU0_XAQ2_INTR_IRQHandler                     ;Shared Int Source xaq2_intr from GPU0 Sub-System
        DCD     Reserved164_IRQHandler                        ;xxx Interrupt 164
        DCD     ADMA_EDMA2_INT_IRQHandler                     ;Shared Int Source eDMA2_INT from ADMA Sub-System
        DCD     ADMA_EDMA2_ERR_INT_IRQHandler                 ;Shared Int Source eDMA2_ERR_INT from ADMA Sub-System
        DCD     ADMA_EDMA3_INT_IRQHandler                     ;Shared Int Source eDMA3_INT from ADMA Sub-System
        DCD     ADMA_EDMA3_ERR_INT_IRQHandler                 ;Shared Int Source eDMA3_ERR_INT from ADMA Sub-System
        DCD     Reserved169_IRQHandler                        ;xxx Interrupt 169
        DCD     Reserved170_IRQHandler                        ;xxx Interrupt 170
        DCD     Reserved171_IRQHandler                        ;xxx Interrupt 171
        DCD     Reserved172_IRQHandler                        ;xxx Interrupt 172
        DCD     Reserved173_IRQHandler                        ;xxx Interrupt 173
        DCD     Reserved174_IRQHandler                        ;xxx Interrupt 174
        DCD     Reserved175_IRQHandler                        ;xxx Interrupt 175
        DCD     Reserved176_IRQHandler                        ;xxx Interrupt 176
        DCD     Reserved177_IRQHandler                        ;xxx Interrupt 177
        DCD     Reserved178_IRQHandler                        ;xxx Interrupt 178
        DCD     LSIO_GPT0_INT_IRQHandler                      ;Shared Int Source GPT0_INT from LSIO Sub-System
        DCD     LSIO_GPT1_INT_IRQHandler                      ;Shared Int Source GPT1_INT from LSIO Sub-System
        DCD     LSIO_GPT2_INT_IRQHandler                      ;Shared Int Source GPT2_INT from LSIO Sub-System
        DCD     LSIO_GPT3_INT_IRQHandler                      ;Shared Int Source GPT3_INT from LSIO Sub-System
        DCD     LSIO_GPT4_INT_IRQHandler                      ;Shared Int Source GPT4_INT from LSIO Sub-System
        DCD     LSIO_KPP_INT_IRQHandler                       ;Shared Int Source KPP_INT from LSIO Sub-System
        DCD     Reserved185_IRQHandler                        ;xxx Interrupt 185
        DCD     Reserved186_IRQHandler                        ;xxx Interrupt 186
        DCD     Reserved187_IRQHandler                        ;xxx Interrupt 187
        DCD     Reserved188_IRQHandler                        ;xxx Interrupt 188
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
        DCD     SCU_SYS_COUNT_INT0_IRQHandler                 ;Shared Int Source SYS_COUNT_INT0 from SCU Sub-System
        DCD     SCU_SYS_COUNT_INT1_IRQHandler                 ;Shared Int Source SYS_COUNT_INT1 from SCU Sub-System
        DCD     SCU_SYS_COUNT_INT2_IRQHandler                 ;Shared Int Source SYS_COUNT_INT2 from SCU Sub-System
        DCD     SCU_SYS_COUNT_INT3_IRQHandler                 ;Shared Int Source SYS_COUNT_INT3 from SCU Sub-System
        DCD     Reserved223_IRQHandler                        ;xxx Interrupt 223
        DCD     Reserved224_IRQHandler                        ;xxx Interrupt 224
        DCD     Reserved225_IRQHandler                        ;xxx Interrupt 225
        DCD     Reserved226_IRQHandler                        ;xxx Interrupt 226
        DCD     DRC_ECC_CORRECT_INT_IRQHandler                ;Shared Int Source ECC_CORRECT_INT from DRC Sub-System
        DCD     DRC_ECC_NCORRECT_INT_IRQHandler               ;Shared Int Source ECC_NCORRECT_INT from DRC Sub-System
        DCD     DRC_SBR_DONE_INT_IRQHandler                   ;Shared Int Source SBR_DONE_INT from DRC Sub-System
        DCD     DRC_PERF_CNT_INT_IRQHandler                   ;Shared Int Source PERF_CNT_INT from DRC Sub-System
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
        DCD     Reserved251_IRQHandler                        ;xxx Interrupt 251
        DCD     Reserved252_IRQHandler                        ;xxx Interrupt 252
        DCD     Reserved253_IRQHandler                        ;xxx Interrupt 253
        DCD     Reserved254_IRQHandler                        ;xxx Interrupt 254
        DCD     Reserved255_IRQHandler                        ;xxx Interrupt 255
        DCD     Reserved256_IRQHandler                        ;xxx Interrupt 256
        DCD     Reserved257_IRQHandler                        ;xxx Interrupt 257
        DCD     Reserved258_IRQHandler                        ;xxx Interrupt 258
        DCD     Reserved259_IRQHandler                        ;xxx Interrupt 259
        DCD     Reserved260_IRQHandler                        ;xxx Interrupt 260
        DCD     Reserved261_IRQHandler                        ;xxx Interrupt 261
        DCD     Reserved262_IRQHandler                        ;xxx Interrupt 262
        DCD     Reserved263_IRQHandler                        ;xxx Interrupt 263
        DCD     Reserved264_IRQHandler                        ;xxx Interrupt 264
        DCD     Reserved265_IRQHandler                        ;xxx Interrupt 265
        DCD     Reserved266_IRQHandler                        ;xxx Interrupt 266
        DCD     Reserved267_IRQHandler                        ;xxx Interrupt 267
        DCD     Reserved268_IRQHandler                        ;xxx Interrupt 268
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
        DCD     ADMA_SPI0_INT_IRQHandler                      ;Shared Int Source SPI0_INT from ADMA Sub-System
        DCD     ADMA_SPI1_INT_IRQHandler                      ;Shared Int Source SPI1_INT from ADMA Sub-System
        DCD     ADMA_SPI2_INT_IRQHandler                      ;Shared Int Source SPI2_INT from ADMA Sub-System
        DCD     ADMA_SPI3_INT_IRQHandler                      ;Shared Int Source SPI3_INT from ADMA Sub-System
        DCD     ADMA_I2C0_INT_IRQHandler                      ;Shared Int Source I2C0_INT from ADMA Sub-System
        DCD     ADMA_I2C1_INT_IRQHandler                      ;Shared Int Source I2C1_INT from ADMA Sub-System
        DCD     ADMA_I2C2_INT_IRQHandler                      ;Shared Int Source I2C2_INT from ADMA Sub-System
        DCD     ADMA_I2C3_INT_IRQHandler                      ;Shared Int Source I2C3_INT from ADMA Sub-System
        DCD     ADMA_I2C4_INT_IRQHandler                      ;Shared Int Source I2C4_INT from ADMA Sub-System
        DCD     ADMA_UART0_INT_IRQHandler                     ;Shared Int Source UART0_INT from ADMA Sub-System
        DCD     ADMA_UART1_INT_IRQHandler                     ;Shared Int Source UART1_INT from ADMA Sub-System
        DCD     ADMA_UART2_INT_IRQHandler                     ;Shared Int Source UART2_INT from ADMA Sub-System
        DCD     ADMA_UART3_INT_IRQHandler                     ;Shared Int Source UART3_INT from ADMA Sub-System
        DCD     Reserved328_IRQHandler                        ;xxx Interrupt 328
        DCD     Reserved329_IRQHandler                        ;xxx Interrupt 329
        DCD     Reserved330_IRQHandler                        ;xxx Interrupt 330
        DCD     CONNECTIVITY_USDHC0_INT_IRQHandler            ;Shared Int Source uSDHC0_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_USDHC1_INT_IRQHandler            ;Shared Int Source uSDHC1_INT from Connectivity Sub-System
        DCD     CONNECTIVITY_USDHC2_INT_IRQHandler            ;Shared Int Source uSDHC2_INT from Connectivity Sub-System
        DCD     ADMA_FLEXCAN0_INT_IRQHandler                  ;Shared Int Source FlexCAN0_INT from ADMA Sub-System
        DCD     ADMA_FLEXCAN1_INT_IRQHandler                  ;Shared Int Source FlexCAN1_INT from ADMA Sub-System
        DCD     ADMA_FLEXCAN2_INT_IRQHandler                  ;Shared Int Source FlexCAN2_INT from ADMA Sub-System
        DCD     ADMA_FTM0_INT_IRQHandler                      ;Shared Int Source FTM0_INT from ADMA Sub-System
        DCD     ADMA_FTM1_INT_IRQHandler                      ;Shared Int Source FTM1_INT from ADMA Sub-System
        DCD     ADMA_ADC0_INT_IRQHandler                      ;Shared Int Source ADC0_INT from ADMA Sub-System
        DCD     Reserved340_IRQHandler                        ;xxx Interrupt 340
        DCD     ADMA_EXTERNAL_DMA_INT_0_IRQHandler            ;Shared Int Source EXTERNAL_DMA_INT_0 from ADMA Sub-System
        DCD     ADMA_EXTERNAL_DMA_INT_1_IRQHandler            ;Shared Int Source EXTERNAL_DMA_INT_1 from ADMA Sub-System
        DCD     ADMA_EXTERNAL_DMA_INT_2_IRQHandler            ;Shared Int Source EXTERNAL_DMA_INT_2 from ADMA Sub-System
        DCD     ADMA_EXTERNAL_DMA_INT_3_IRQHandler            ;Shared Int Source EXTERNAL_DMA_INT_3 from ADMA Sub-System
        DCD     ADMA_EXTERNAL_DMA_INT_4_IRQHandler            ;Shared Int Source EXTERNAL_DMA_INT_4 from ADMA Sub-System
        DCD     ADMA_EXTERNAL_DMA_INT_5_IRQHandler            ;Shared Int Source EXTERNAL_DMA_INT_5 from ADMA Sub-System
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
        DCD     ADMA_SAI0_MOD_INT_IRQHandler                  ;Shared Int Source SAI0_MOD_INT from ADMA Sub-System
        DCD     ADMA_SAI0_DMA_INT_IRQHandler                  ;Shared Int Source SAI0_DMA_INT from ADMA Sub-System
        DCD     ADMA_SAI1_MOD_INT_IRQHandler                  ;Shared Int Source SAI1_MOD_INT from ADMA Sub-System
        DCD     ADMA_SAI1_DMA_INT_IRQHandler                  ;Shared Int Source SAI1_DMA_INT from ADMA Sub-System
        DCD     ADMA_SAI2_MOD_INT_IRQHandler                  ;Shared Int Source SAI2_MOD_INT from ADMA Sub-System
        DCD     ADMA_SAI2_DMA_INT_IRQHandler                  ;Shared Int Source SAI2_DMA_INT from ADMA Sub-System
        DCD     MIPI_CSI0_OUT_INT_IRQHandler                  ;Shared Int Source OUT_INT from MIPI_CSI0 Sub-System
        DCD     Reserved420_IRQHandler                        ;xxx Interrupt 420
        DCD     Reserved421_IRQHandler                        ;xxx Interrupt 421
        DCD     ADMA_SAI3_MOD_INT_IRQHandler                  ;Shared Int Source SAI3_MOD_INT from ADMA Sub-System
        DCD     ADMA_SAI3_DMA_INT_IRQHandler                  ;Shared Int Source SAI3_DMA_INT from ADMA Sub-System
        DCD     Reserved424_IRQHandler                        ;xxx Interrupt 424
        DCD     Reserved425_IRQHandler                        ;xxx Interrupt 425
        DCD     Reserved426_IRQHandler                        ;xxx Interrupt 426
        DCD     Reserved427_IRQHandler                        ;xxx Interrupt 427
        DCD     ADMA_SAI4_MOD_INT_IRQHandler                  ;Shared Int Source SAI4_MOD_INT from ADMA Sub-System
        DCD     ADMA_SAI4_DMA_INT_IRQHandler                  ;Shared Int Source SAI4_DMA_INT from ADMA Sub-System
        DCD     ADMA_SAI5_MOD_INT_IRQHandler                  ;Shared Int Source SAI5_MOD_INT from ADMA Sub-System
        DCD     ADMA_SAI5_DMA_INT_IRQHandler                  ;Shared Int Source SAI5_DMA_INT from ADMA Sub-System
        DCD     Reserved432_IRQHandler                        ;xxx Interrupt 432
        DCD     Reserved433_IRQHandler                        ;xxx Interrupt 433
        DCD     Reserved434_IRQHandler                        ;xxx Interrupt 434
        DCD     ADMA_SPI0_MOD_INT_IRQHandler                  ;Shared Int Source SPI0_MOD_INT from ADMA Sub-System
        DCD     ADMA_SPI1_MOD_INT_IRQHandler                  ;Shared Int Source SPI1_MOD_INT from ADMA Sub-System
        DCD     ADMA_SPI2_MOD_INT_IRQHandler                  ;Shared Int Source SPI2_MOD_INT from ADMA Sub-System
        DCD     ADMA_SPI3_MOD_INT_IRQHandler                  ;Shared Int Source SPI3_MOD_INT from ADMA Sub-System
        DCD     ADMA_I2C0_MOD_INT_IRQHandler                  ;Shared Int Source I2C0_MOD_INT from ADMA Sub-System
        DCD     ADMA_I2C1_MOD_INT_IRQHandler                  ;Shared Int Source I2C1_MOD_INT from ADMA Sub-System
        DCD     ADMA_I2C2_MOD_INT_IRQHandler                  ;Shared Int Source I2C2_MOD_INT from ADMA Sub-System
        DCD     ADMA_I2C3_MOD_INT_IRQHandler                  ;Shared Int Source I2C3_MOD_INT from ADMA Sub-System
        DCD     Reserved443_IRQHandler                        ;xxx Interrupt 443
        DCD     ADMA_UART0_MOD_INT_IRQHandler                 ;Shared Int Source UART0_MOD_INT from ADMA Sub-System
        DCD     ADMA_UART1_MOD_INT_IRQHandler                 ;Shared Int Source UART1_MOD_INT from ADMA Sub-System
        DCD     ADMA_UART2_MOD_INT_IRQHandler                 ;Shared Int Source UART2_MOD_INT from ADMA Sub-System
        DCD     ADMA_UART3_MOD_INT_IRQHandler                 ;Shared Int Source UART3_MOD_INT from ADMA Sub-System
        DCD     Reserved448_IRQHandler                        ;xxx Interrupt 448
        DCD     Reserved449_IRQHandler                        ;xxx Interrupt 449
        DCD     Reserved450_IRQHandler                        ;xxx Interrupt 450
        DCD     ADMA_FLEXCAN0_MOD_INT_IRQHandler              ;Shared Int Source FLEXCAN0_MOD_INT from ADMA Sub-System
        DCD     ADMA_FLEXCAN1_MOD_INT_IRQHandler              ;Shared Int Source FLEXCAN1_MOD_INT from ADMA Sub-System
        DCD     ADMA_FLEXCAN2_MOD_INT_IRQHandler              ;Shared Int Source FLEXCAN2_MOD_INT from ADMA Sub-System
        DCD     ADMA_FTM0_MOD_INT_IRQHandler                  ;Shared Int Source FTM0_MOD_INT from ADMA Sub-System
        DCD     ADMA_FTM1_MOD_INT_IRQHandler                  ;Shared Int Source FTM1_MOD_INT from ADMA Sub-System
        DCD     ADMA_ADC0_MOD_INT_IRQHandler                  ;Shared Int Source ADC0_MOD_INT from ADMA Sub-System
        DCD     Reserved457_IRQHandler                        ;xxx Interrupt 457
        DCD     ADMA_FLEXCAN0_DMA_INT_IRQHandler              ;Shared Int Source FLEXCAN0_DMA_INT from ADMA Sub-System
        DCD     ADMA_FLEXCAN1_DMA_INT_IRQHandler              ;Shared Int Source FLEXCAN1_DMA_INT from ADMA Sub-System
        DCD     ADMA_FLEXCAN2_DMA_INT_IRQHandler              ;Shared Int Source FLEXCAN2_DMA_INT from ADMA Sub-System
        DCD     ADMA_FTM0_DMA_INT_IRQHandler                  ;Shared Int Source FTM0_DMA_INT from ADMA Sub-System
        DCD     ADMA_FTM1_DMA_INT_IRQHandler                  ;Shared Int Source FTM1_DMA_INT from ADMA Sub-System
        DCD     ADMA_ADC0_DMA_INT_IRQHandler                  ;Shared Int Source ADC0_DMA_INT from ADMA Sub-System
        DCD     ADMA_ADC1_DMA_INT_IRQHandler                  ;Shared Int Source ADC1_DMA_INT from ADMA Sub-System
        DCD     Reserved465_IRQHandler                        ;xxx Interrupt 465
        DCD     Reserved466_IRQHandler                        ;xxx Interrupt 466
        DCD     ADMA_EDMA0_INT_IRQHandler                     ;Shared Int Source eDMA0_INT from ADMA Sub-System
        DCD     ADMA_EDMA0_ERR_INT_IRQHandler                 ;Shared Int Source eDMA0_ERR_INT from ADMA Sub-System
        DCD     ADMA_EDMA1_INT_IRQHandler                     ;Shared Int Source eDMA1_INT from ADMA Sub-System
        DCD     ADMA_EDMA1_ERR_INT_IRQHandler                 ;Shared Int Source eDMA1_ERR_INT from ADMA Sub-System
        DCD     ADMA_ASRC0_INT1_IRQHandler                    ;Shared Int Source ASRC0_INT1 from ADMA Sub-System
        DCD     ADMA_ASRC0_INT2_IRQHandler                    ;Shared Int Source ASRC0_INT2 from ADMA Sub-System
        DCD     ADMA_DMA0_CH0_INT_IRQHandler                  ;Shared Int Source DMA0_CH0_INT from ADMA Sub-System
        DCD     ADMA_DMA0_CH1_INT_IRQHandler                  ;Shared Int Source DMA0_CH1_INT from ADMA Sub-System
        DCD     ADMA_DMA0_CH2_INT_IRQHandler                  ;Shared Int Source DMA0_CH2_INT from ADMA Sub-System
        DCD     ADMA_DMA0_CH3_INT_IRQHandler                  ;Shared Int Source DMA0_CH3_INT from ADMA Sub-System
        DCD     ADMA_DMA0_CH4_INT_IRQHandler                  ;Shared Int Source DMA0_CH4_INT from ADMA Sub-System
        DCD     ADMA_DMA0_CH5_INT_IRQHandler                  ;Shared Int Source DMA0_CH5_INT from ADMA Sub-System
        DCD     ADMA_ASRC1_INT1_IRQHandler                    ;Shared Int Source ASRC1_INT1 from ADMA Sub-System
        DCD     ADMA_ASRC1_INT2_IRQHandler                    ;Shared Int Source ASRC1_INT2 from ADMA Sub-System
        DCD     ADMA_DMA1_CH0_INT_IRQHandler                  ;Shared Int Source DMA1_CH0_INT from ADMA Sub-System
        DCD     ADMA_DMA1_CH1_INT_IRQHandler                  ;Shared Int Source DMA1_CH1_INT from ADMA Sub-System
        DCD     ADMA_DMA1_CH2_INT_IRQHandler                  ;Shared Int Source DMA1_CH2_INT from ADMA Sub-System
        DCD     ADMA_DMA1_CH3_INT_IRQHandler                  ;Shared Int Source DMA1_CH3_INT from ADMA Sub-System
        DCD     ADMA_DMA1_CH4_INT_IRQHandler                  ;Shared Int Source DMA1_CH4_INT from ADMA Sub-System
        DCD     ADMA_DMA1_CH5_INT_IRQHandler                  ;Shared Int Source DMA1_CH5_INT from ADMA Sub-System
        DCD     ADMA_ESAI0_INT_IRQHandler                     ;Shared Int Source ESAI0_INT from ADMA Sub-System
        DCD     Reserved488_IRQHandler                        ;xxx Interrupt 488
        DCD     Reserved489_IRQHandler                        ;xxx Interrupt 489
        DCD     ADMA_GPT0_INT_IRQHandler                      ;Shared Int Source GPT0_INT from ADMA Sub-System
        DCD     ADMA_GPT1_INT_IRQHandler                      ;Shared Int Source GPT1_INT from ADMA Sub-System
        DCD     ADMA_GPT2_INT_IRQHandler                      ;Shared Int Source GPT2_INT from ADMA Sub-System
        DCD     ADMA_GPT3_INT_IRQHandler                      ;Shared Int Source GPT3_INT from ADMA Sub-System
        DCD     ADMA_GPT4_INT_IRQHandler                      ;Shared Int Source GPT4_INT from ADMA Sub-System
        DCD     ADMA_GPT5_INT_IRQHandler                      ;Shared Int Source GPT5_INT from ADMA Sub-System
        DCD     ADMA_SAI0_INT_IRQHandler                      ;Shared Int Source SAI0_INT from ADMA Sub-System
        DCD     ADMA_SAI1_INT_IRQHandler                      ;Shared Int Source SAI1_INT from ADMA Sub-System
        DCD     ADMA_SAI2_INT_IRQHandler                      ;Shared Int Source SAI2_INT from ADMA Sub-System
        DCD     ADMA_SAI3_INT_IRQHandler                      ;Shared Int Source SAI3_INT from ADMA Sub-System
        DCD     Reserved500_IRQHandler                        ;xxx Interrupt 500
        DCD     Reserved501_IRQHandler                        ;xxx Interrupt 501
        DCD     ADMA_SAI4_INT_IRQHandler                      ;Shared Int Source SAI4_INT from ADMA Sub-System
        DCD     ADMA_SAI5_INT_IRQHandler                      ;Shared Int Source SAI5_INT from ADMA Sub-System
        DCD     ADMA_SPDIF0_RX_INT_IRQHandler                 ;Shared Int Source SPDIF0_RX_INT from ADMA Sub-System
        DCD     ADMA_SPDIF0_TX_INT_IRQHandler                 ;Shared Int Source SPDIF0_TX_INT from ADMA Sub-System
        DCD     Reserved506_IRQHandler                        ;xxx Interrupt 506
        DCD     Reserved507_IRQHandler                        ;xxx Interrupt 507
        DCD     ADMA_ESAI0_MOD_INT_IRQHandler                 ;Shared Int Source ESAI0_MOD_INT from ADMA Sub-System
        DCD     ADMA_ESAI0_DMA_INT_IRQHandler                 ;Shared Int Source ESAI0_DMA_INT from ADMA Sub-System
        DCD     Reserved510_IRQHandler                        ;xxx Interrupt 510
        DCD     Reserved511_IRQHandler                        ;xxx Interrupt 511
        DCD     Reserved512_IRQHandler                        ;xxx Interrupt 512
        DCD     Reserved513_IRQHandler                        ;xxx Interrupt 513
        DCD     Reserved514_IRQHandler                        ;xxx Interrupt 514
        DCD     ADMA_SPI0_DMA_RX_INT_IRQHandler               ;Shared Int Source SPI0_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_SPI0_DMA_TX_INT_IRQHandler               ;Shared Int Source SPI0_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_SPI1_DMA_RX_INT_IRQHandler               ;Shared Int Source SPI1_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_SPI1_DMA_TX_INT_IRQHandler               ;Shared Int Source SPI1_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_SPI2_DMA_RX_INT_IRQHandler               ;Shared Int Source SPI2_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_SPI2_DMA_TX_INT_IRQHandler               ;Shared Int Source SPI2_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_SPI3_DMA_RX_INT_IRQHandler               ;Shared Int Source SPI3_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_SPI3_DMA_TX_INT_IRQHandler               ;Shared Int Source SPI3_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_I2C0_DMA_RX_INT_IRQHandler               ;Shared Int Source I2C0_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_I2C0_DMA_TX_INT_IRQHandler               ;Shared Int Source I2C0_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_I2C1_DMA_RX_INT_IRQHandler               ;Shared Int Source I2C1_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_I2C1_DMA_TX_INT_IRQHandler               ;Shared Int Source I2C1_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_I2C2_DMA_RX_INT_IRQHandler               ;Shared Int Source I2C2_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_I2C2_DMA_TX_INT_IRQHandler               ;Shared Int Source I2C2_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_I2C3_DMA_RX_INT_IRQHandler               ;Shared Int Source I2C3_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_I2C3_DMA_TX_INT_IRQHandler               ;Shared Int Source I2C3_DMA_TX_INT from ADMA Sub-System
        DCD     Reserved531_IRQHandler                        ;xxx Interrupt 531
        DCD     Reserved532_IRQHandler                        ;xxx Interrupt 532
        DCD     ADMA_UART0_DMA_RX_INT_IRQHandler              ;Shared Int Source UART0_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_UART0_DMA_TX_INT_IRQHandler              ;Shared Int Source UART0_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_UART1_DMA_RX_INT_IRQHandler              ;Shared Int Source UART1_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_UART1_DMA_TX_INT_IRQHandler              ;Shared Int Source UART1_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_UART2_DMA_RX_INT_IRQHandler              ;Shared Int Source UART2_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_UART2_DMA_TX_INT_IRQHandler              ;Shared Int Source UART2_DMA_TX_INT from ADMA Sub-System
        DCD     ADMA_UART3_DMA_RX_INT_IRQHandler              ;Shared Int Source UART3_DMA_RX_INT from ADMA Sub-System
        DCD     ADMA_UART3_DMA_TX_INT_IRQHandler              ;Shared Int Source UART3_DMA_TX_INT from ADMA Sub-System
        DCD     Reserved541_IRQHandler                        ;xxx Interrupt 541
        DCD     Reserved542_IRQHandler                        ;xxx Interrupt 542
        DCD     Reserved543_IRQHandler                        ;xxx Interrupt 543
        DCD     Reserved544_IRQHandler                        ;xxx Interrupt 544
        DCD     Reserved545_IRQHandler                        ;xxx Interrupt 545
        DCD     Reserved546_IRQHandler                        ;xxx Interrupt 546
        DCD     SECURITY_MU1_A_INT_IRQHandler                 ;Shared Int Source MU1_A_INT from Security Sub-System
        DCD     SECURITY_MU2_A_INT_IRQHandler                 ;Shared Int Source MU2_A_INT from Security Sub-System
        DCD     SECURITY_MU3_A_INT_IRQHandler                 ;Shared Int Source MU3_A_INT from Security Sub-System
        DCD     SECURITY_CAAM_INT0_IRQHandler                 ;Shared Int Source CAAM_INT0 from Security Sub-System
        DCD     SECURITY_CAAM_INT1_IRQHandler                 ;Shared Int Source CAAM_INT1 from Security Sub-System
        DCD     SECURITY_CAAM_INT2_IRQHandler                 ;Shared Int Source CAAM_INT2 from Security Sub-System
        DCD     SECURITY_CAAM_INT3_IRQHandler                 ;Shared Int Source CAAM_INT3 from Security Sub-System
        DCD     SECURITY_CAAM_RTIC_INT_IRQHandler             ;Shared Int Source CAAM_RTIC_INT from Security Sub-System
        DCD     ADMA_SPDIF0_RX_MOD_INT_IRQHandler             ;Shared Int Source SPDIF0_RX_MOD_INT from ADMA Sub-System
        DCD     ADMA_SPDIF0_RX_DMA_INT_IRQHandler             ;Shared Int Source SPDIF0_RX_DMA_INT from ADMA Sub-System
        DCD     ADMA_SPDIF0_TX_MOD_INT_IRQHandler             ;Shared Int Source SPDIF0_TX_MOD_INT from ADMA Sub-System
        DCD     ADMA_SPDIF0_TX_DMA_INT_IRQHandler             ;Shared Int Source SPDIF0_TX_DMA_INT from ADMA Sub-System
        DCD     Reserved559_IRQHandler                        ;xxx Interrupt 559
        DCD     Reserved560_IRQHandler                        ;xxx Interrupt 560
        DCD     Reserved561_IRQHandler                        ;xxx Interrupt 561
        DCD     Reserved562_IRQHandler                        ;xxx Interrupt 562
        DCD     VPU_VPU_INT_0_IRQHandler                      ;Shared Int Source VPU_INT_0 from VPU Sub-System
        DCD     VPU_VPU_INT_1_IRQHandler                      ;Shared Int Source VPU_INT_1 from VPU Sub-System
        DCD     VPU_VPU_INT_2_IRQHandler                      ;Shared Int Source VPU_INT_2 from VPU Sub-System
        DCD     VPU_VPU_INT_3_IRQHandler                      ;Shared Int Source VPU_INT_3 from VPU Sub-System
        DCD     VPU_VPU_INT_4_IRQHandler                      ;Shared Int Source VPU_INT_4 from VPU Sub-System
        DCD     Reserved568_IRQHandler                        ;xxx Interrupt 568
        DCD     Reserved569_IRQHandler                        ;xxx Interrupt 569
        DCD     Reserved570_IRQHandler                        ;xxx Interrupt 570
        DCD     Reserved571_IRQHandler                        ;xxx Interrupt 571
        DCD     Reserved572_IRQHandler                        ;xxx Interrupt 572
        DCD     Reserved573_IRQHandler                        ;xxx Interrupt 573
        DCD     Reserved574_IRQHandler                        ;xxx Interrupt 574
        DCD     Reserved575_IRQHandler                        ;xxx Interrupt 575
        DCD     Reserved576_IRQHandler                        ;xxx Interrupt 576
        DCD     Reserved577_IRQHandler                        ;xxx Interrupt 577
        DCD     Reserved578_IRQHandler                        ;xxx Interrupt 578
        DCD     Reserved579_IRQHandler                        ;xxx Interrupt 579
        DCD     M4_INTMUX_SOURCE_TPM_IRQHandler               ;INTMUX Input source: TPM Interrupt
        DCD     Reserved581_IRQHandler                        ;xxx Interrupt 581
        DCD     Reserved582_IRQHandler                        ;xxx Interrupt 582
        DCD     M4_INTMUX_SOURCE_LPIT_IRQHandler              ;INTMUX Input source: LPIT Interrupt
        DCD     Reserved584_IRQHandler                        ;xxx Interrupt 584
        DCD     Reserved585_IRQHandler                        ;xxx Interrupt 585
        DCD     M4_INTMUX_SOURCE_LPUART_IRQHandler            ;INTMUX Input source: LPUART Interrupt
        DCD     Reserved587_IRQHandler                        ;xxx Interrupt 587
        DCD     M4_INTMUX_SOURCE_LPI2C_IRQHandler             ;INTMUX Input source: LPI2C Interrupt
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
        DCD     M4_INTMUX_SOURCE_MU0_A3_IRQHandler            ;INTMUX Input source: MU0_A3 Interrupt
        DCD     M4_INTMUX_SOURCE_MU0_A2_IRQHandler            ;INTMUX Input source: MU0_A2 Interrupt
        DCD     M4_INTMUX_SOURCE_MU0_A1_IRQHandler            ;INTMUX Input source: MU0_A1 Interrupt
        DCD     M4_INTMUX_SOURCE_MU0_A0_IRQHandler            ;INTMUX Input source: MU0_A0 Interrupt
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

#if defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE) && FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE
        LDR     R1, =SFB(QACCESS_CODE_VAR)
        LDR     R2, =SFE(QACCESS_CODE_VAR)
.LC5:
        CMP     R1, R2
        ITT     LT
        STRLT   R0, [R1], #4
        BLT     .LC5

        LDR     R1, =SFB(QACCESS_DATA_VAR)
        LDR     R2, =SFE(QACCESS_DATA_VAR)
.LC6:
        CMP     R1, R2
        ITT     LT
        STRLT   R0, [R1], #4
        BLT     .LC6
#endif

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
        PUBWEAK M4_MCM_IRQHandler
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
        PUBWEAK M4_TPM_IRQHandler
        PUBWEAK Reserved36_IRQHandler
        PUBWEAK Reserved37_IRQHandler
        PUBWEAK M4_LPIT_IRQHandler
        PUBWEAK Reserved39_IRQHandler
        PUBWEAK Reserved40_IRQHandler
        PUBWEAK M4_LPUART_IRQHandler
        PUBWEAK M4_LPUART_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_LPUART_IRQHandler
        LDR     R0, =M4_LPUART_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved42_IRQHandler
        PUBWEAK M4_LPI2C_IRQHandler
        PUBWEAK M4_LPI2C_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_LPI2C_IRQHandler
        LDR     R0, =M4_LPI2C_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved44_IRQHandler
        PUBWEAK M4_MU0_B0_IRQHandler
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
        PUBWEAK M4_MU0_B1_IRQHandler
        PUBWEAK M4_MU0_B2_IRQHandler
        PUBWEAK M4_MU0_B3_IRQHandler
        PUBWEAK Reserved63_IRQHandler
        PUBWEAK Reserved64_IRQHandler
        PUBWEAK M4_MU1_A_IRQHandler
        PUBWEAK M4_SW_IRQHandler
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
        PUBWEAK A35_NEXTERRIRQ_IRQHandler
        PUBWEAK A35_NINTERRIRQ_IRQHandler
        PUBWEAK Reserved101_IRQHandler
        PUBWEAK Reserved102_IRQHandler
        PUBWEAK Reserved103_IRQHandler
        PUBWEAK Reserved104_IRQHandler
        PUBWEAK Reserved105_IRQHandler
        PUBWEAK Reserved106_IRQHandler
        PUBWEAK Reserved107_IRQHandler
        PUBWEAK Reserved108_IRQHandler
        PUBWEAK Reserved109_IRQHandler
        PUBWEAK Reserved110_IRQHandler
        PUBWEAK Reserved111_IRQHandler
        PUBWEAK Reserved112_IRQHandler
        PUBWEAK Reserved113_IRQHandler
        PUBWEAK Reserved114_IRQHandler
        PUBWEAK M4_INT_OUT0_IRQHandler
        PUBWEAK M4_INT_OUT1_IRQHandler
        PUBWEAK M4_INT_OUT2_IRQHandler
        PUBWEAK M4_INT_OUT3_IRQHandler
        PUBWEAK M4_INT_OUT4_IRQHandler
        PUBWEAK M4_INT_OUT5_IRQHandler
        PUBWEAK M4_INT_OUT6_IRQHandler
        PUBWEAK M4_INT_OUT7_IRQHandler
        PUBWEAK Reserved123_IRQHandler
        PUBWEAK Reserved124_IRQHandler
        PUBWEAK Reserved125_IRQHandler
        PUBWEAK Reserved126_IRQHandler
        PUBWEAK Reserved127_IRQHandler
        PUBWEAK Reserved128_IRQHandler
        PUBWEAK Reserved129_IRQHandler
        PUBWEAK Reserved130_IRQHandler
        PUBWEAK Reserved131_IRQHandler
        PUBWEAK Reserved132_IRQHandler
        PUBWEAK Reserved133_IRQHandler
        PUBWEAK Reserved134_IRQHandler
        PUBWEAK Reserved135_IRQHandler
        PUBWEAK Reserved136_IRQHandler
        PUBWEAK Reserved137_IRQHandler
        PUBWEAK Reserved138_IRQHandler
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
        PUBWEAK Reserved156_IRQHandler
        PUBWEAK Reserved157_IRQHandler
        PUBWEAK MIPI_DSI0_INT_OUT_IRQHandler
        PUBWEAK MIPI_DSI1_INT_OUT_IRQHandler
        PUBWEAK Reserved160_IRQHandler
        PUBWEAK LCD_MOD_INT_IRQHandler
        PUBWEAK LCD_PWM_INT_IRQHandler
        PUBWEAK GPU0_XAQ2_INTR_IRQHandler
        PUBWEAK Reserved164_IRQHandler
        PUBWEAK ADMA_EDMA2_INT_IRQHandler
        PUBWEAK ADMA_EDMA2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EDMA2_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EDMA2_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EDMA2_ERR_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EDMA3_INT_IRQHandler
        PUBWEAK ADMA_EDMA3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EDMA3_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EDMA3_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EDMA3_ERR_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved169_IRQHandler
        PUBWEAK Reserved170_IRQHandler
        PUBWEAK Reserved171_IRQHandler
        PUBWEAK Reserved172_IRQHandler
        PUBWEAK Reserved173_IRQHandler
        PUBWEAK Reserved174_IRQHandler
        PUBWEAK Reserved175_IRQHandler
        PUBWEAK Reserved176_IRQHandler
        PUBWEAK Reserved177_IRQHandler
        PUBWEAK Reserved178_IRQHandler
        PUBWEAK LSIO_GPT0_INT_IRQHandler
        PUBWEAK LSIO_GPT1_INT_IRQHandler
        PUBWEAK LSIO_GPT2_INT_IRQHandler
        PUBWEAK LSIO_GPT3_INT_IRQHandler
        PUBWEAK LSIO_GPT4_INT_IRQHandler
        PUBWEAK LSIO_KPP_INT_IRQHandler
        PUBWEAK Reserved185_IRQHandler
        PUBWEAK Reserved186_IRQHandler
        PUBWEAK Reserved187_IRQHandler
        PUBWEAK Reserved188_IRQHandler
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
        PUBWEAK DRC_ECC_CORRECT_INT_IRQHandler
        PUBWEAK DRC_ECC_NCORRECT_INT_IRQHandler
        PUBWEAK DRC_SBR_DONE_INT_IRQHandler
        PUBWEAK DRC_PERF_CNT_INT_IRQHandler
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
        PUBWEAK Reserved251_IRQHandler
        PUBWEAK Reserved252_IRQHandler
        PUBWEAK Reserved253_IRQHandler
        PUBWEAK Reserved254_IRQHandler
        PUBWEAK Reserved255_IRQHandler
        PUBWEAK Reserved256_IRQHandler
        PUBWEAK Reserved257_IRQHandler
        PUBWEAK Reserved258_IRQHandler
        PUBWEAK Reserved259_IRQHandler
        PUBWEAK Reserved260_IRQHandler
        PUBWEAK Reserved261_IRQHandler
        PUBWEAK Reserved262_IRQHandler
        PUBWEAK Reserved263_IRQHandler
        PUBWEAK Reserved264_IRQHandler
        PUBWEAK Reserved265_IRQHandler
        PUBWEAK Reserved266_IRQHandler
        PUBWEAK Reserved267_IRQHandler
        PUBWEAK Reserved268_IRQHandler
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
        PUBWEAK ADMA_SPI0_INT_IRQHandler
        PUBWEAK ADMA_SPI0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI0_INT_IRQHandler
        LDR     R0, =ADMA_SPI0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI1_INT_IRQHandler
        PUBWEAK ADMA_SPI1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI1_INT_IRQHandler
        LDR     R0, =ADMA_SPI1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI2_INT_IRQHandler
        PUBWEAK ADMA_SPI2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI2_INT_IRQHandler
        LDR     R0, =ADMA_SPI2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI3_INT_IRQHandler
        PUBWEAK ADMA_SPI3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI3_INT_IRQHandler
        LDR     R0, =ADMA_SPI3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C0_INT_IRQHandler
        PUBWEAK ADMA_I2C0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C0_INT_IRQHandler
        LDR     R0, =ADMA_I2C0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C1_INT_IRQHandler
        PUBWEAK ADMA_I2C1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C1_INT_IRQHandler
        LDR     R0, =ADMA_I2C1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C2_INT_IRQHandler
        PUBWEAK ADMA_I2C2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C2_INT_IRQHandler
        LDR     R0, =ADMA_I2C2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C3_INT_IRQHandler
        PUBWEAK ADMA_I2C3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C3_INT_IRQHandler
        LDR     R0, =ADMA_I2C3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C4_INT_IRQHandler
        PUBWEAK ADMA_I2C4_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C4_INT_IRQHandler
        LDR     R0, =ADMA_I2C4_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART0_INT_IRQHandler
        PUBWEAK ADMA_UART0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART0_INT_IRQHandler
        LDR     R0, =ADMA_UART0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART1_INT_IRQHandler
        PUBWEAK ADMA_UART1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART1_INT_IRQHandler
        LDR     R0, =ADMA_UART1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART2_INT_IRQHandler
        PUBWEAK ADMA_UART2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART2_INT_IRQHandler
        LDR     R0, =ADMA_UART2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART3_INT_IRQHandler
        PUBWEAK ADMA_UART3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART3_INT_IRQHandler
        LDR     R0, =ADMA_UART3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved328_IRQHandler
        PUBWEAK Reserved329_IRQHandler
        PUBWEAK Reserved330_IRQHandler
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

        PUBWEAK ADMA_FLEXCAN0_INT_IRQHandler
        PUBWEAK ADMA_FLEXCAN0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN0_INT_IRQHandler
        LDR     R0, =ADMA_FLEXCAN0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FLEXCAN1_INT_IRQHandler
        PUBWEAK ADMA_FLEXCAN1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN1_INT_IRQHandler
        LDR     R0, =ADMA_FLEXCAN1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FLEXCAN2_INT_IRQHandler
        PUBWEAK ADMA_FLEXCAN2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN2_INT_IRQHandler
        LDR     R0, =ADMA_FLEXCAN2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FTM0_INT_IRQHandler
        PUBWEAK ADMA_FTM0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FTM0_INT_IRQHandler
        LDR     R0, =ADMA_FTM0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FTM1_INT_IRQHandler
        PUBWEAK ADMA_FTM1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FTM1_INT_IRQHandler
        LDR     R0, =ADMA_FTM1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ADC0_INT_IRQHandler
        PUBWEAK ADMA_ADC0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ADC0_INT_IRQHandler
        LDR     R0, =ADMA_ADC0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved340_IRQHandler
        PUBWEAK ADMA_EXTERNAL_DMA_INT_0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EXTERNAL_DMA_INT_0_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EXTERNAL_DMA_INT_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EXTERNAL_DMA_INT_1_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EXTERNAL_DMA_INT_2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EXTERNAL_DMA_INT_2_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EXTERNAL_DMA_INT_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EXTERNAL_DMA_INT_3_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EXTERNAL_DMA_INT_4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EXTERNAL_DMA_INT_4_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EXTERNAL_DMA_INT_5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EXTERNAL_DMA_INT_5_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
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
        PUBWEAK ADMA_SAI0_MOD_INT_IRQHandler
        PUBWEAK ADMA_SAI0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI0_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SAI0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI0_DMA_INT_IRQHandler
        PUBWEAK ADMA_EDMA0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI0_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI1_MOD_INT_IRQHandler
        PUBWEAK ADMA_SAI1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI1_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SAI1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI1_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI2_MOD_INT_IRQHandler
        PUBWEAK ADMA_SAI2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI2_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SAI2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI2_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI2_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK MIPI_CSI0_OUT_INT_IRQHandler
        PUBWEAK Reserved420_IRQHandler
        PUBWEAK Reserved421_IRQHandler
        PUBWEAK ADMA_SAI3_MOD_INT_IRQHandler
        PUBWEAK ADMA_SAI3_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI3_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SAI3_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI3_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI3_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved424_IRQHandler
        PUBWEAK Reserved425_IRQHandler
        PUBWEAK Reserved426_IRQHandler
        PUBWEAK Reserved427_IRQHandler
        PUBWEAK ADMA_SAI4_MOD_INT_IRQHandler
        PUBWEAK ADMA_SAI4_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI4_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SAI4_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI4_DMA_INT_IRQHandler
        PUBWEAK ADMA_EDMA1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI4_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI5_MOD_INT_IRQHandler
        PUBWEAK ADMA_SAI5_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI5_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SAI5_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI5_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI5_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved432_IRQHandler
        PUBWEAK Reserved433_IRQHandler
        PUBWEAK Reserved434_IRQHandler
        PUBWEAK ADMA_SPI0_MOD_INT_IRQHandler
        PUBWEAK ADMA_SPI0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI0_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SPI0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI1_MOD_INT_IRQHandler
        PUBWEAK ADMA_SPI1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI1_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SPI1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI2_MOD_INT_IRQHandler
        PUBWEAK ADMA_SPI2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI2_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SPI2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI3_MOD_INT_IRQHandler
        PUBWEAK ADMA_SPI3_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI3_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SPI3_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C0_MOD_INT_IRQHandler
        PUBWEAK ADMA_I2C0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C0_MOD_INT_IRQHandler
        LDR     R0, =ADMA_I2C0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C1_MOD_INT_IRQHandler
        PUBWEAK ADMA_I2C1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C1_MOD_INT_IRQHandler
        LDR     R0, =ADMA_I2C1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C2_MOD_INT_IRQHandler
        PUBWEAK ADMA_I2C2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C2_MOD_INT_IRQHandler
        LDR     R0, =ADMA_I2C2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C3_MOD_INT_IRQHandler
        PUBWEAK ADMA_I2C3_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C3_MOD_INT_IRQHandler
        LDR     R0, =ADMA_I2C3_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved443_IRQHandler
        PUBWEAK ADMA_UART0_MOD_INT_IRQHandler
        PUBWEAK ADMA_UART0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART0_MOD_INT_IRQHandler
        LDR     R0, =ADMA_UART0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART1_MOD_INT_IRQHandler
        PUBWEAK ADMA_UART1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART1_MOD_INT_IRQHandler
        LDR     R0, =ADMA_UART1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART2_MOD_INT_IRQHandler
        PUBWEAK ADMA_UART2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART2_MOD_INT_IRQHandler
        LDR     R0, =ADMA_UART2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART3_MOD_INT_IRQHandler
        PUBWEAK ADMA_UART3_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART3_MOD_INT_IRQHandler
        LDR     R0, =ADMA_UART3_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved448_IRQHandler
        PUBWEAK Reserved449_IRQHandler
        PUBWEAK Reserved450_IRQHandler
        PUBWEAK ADMA_FLEXCAN0_MOD_INT_IRQHandler
        PUBWEAK ADMA_FLEXCAN0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN0_MOD_INT_IRQHandler
        LDR     R0, =ADMA_FLEXCAN0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FLEXCAN1_MOD_INT_IRQHandler
        PUBWEAK ADMA_FLEXCAN1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN1_MOD_INT_IRQHandler
        LDR     R0, =ADMA_FLEXCAN1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FLEXCAN2_MOD_INT_IRQHandler
        PUBWEAK ADMA_FLEXCAN2_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN2_MOD_INT_IRQHandler
        LDR     R0, =ADMA_FLEXCAN2_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FTM0_MOD_INT_IRQHandler
        PUBWEAK ADMA_FTM0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FTM0_MOD_INT_IRQHandler
        LDR     R0, =ADMA_FTM0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FTM1_MOD_INT_IRQHandler
        PUBWEAK ADMA_FTM1_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FTM1_MOD_INT_IRQHandler
        LDR     R0, =ADMA_FTM1_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ADC0_MOD_INT_IRQHandler
        PUBWEAK ADMA_ADC0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ADC0_MOD_INT_IRQHandler
        LDR     R0, =ADMA_ADC0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved457_IRQHandler
        PUBWEAK ADMA_FLEXCAN0_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN0_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FLEXCAN1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN1_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FLEXCAN2_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FLEXCAN2_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FTM0_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FTM0_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_FTM1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_FTM1_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ADC0_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ADC0_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ADC1_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ADC1_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved465_IRQHandler
        PUBWEAK Reserved466_IRQHandler
        PUBWEAK ADMA_EDMA0_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EDMA0_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EDMA0_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EDMA0_ERR_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EDMA1_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EDMA1_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_EDMA1_ERR_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_EDMA1_ERR_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ASRC0_INT1_IRQHandler
        PUBWEAK ADMA_ASRC0_INT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ASRC0_INT1_IRQHandler
        LDR     R0, =ADMA_ASRC0_INT1_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ASRC0_INT2_IRQHandler
        PUBWEAK ADMA_ASRC0_INT2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ASRC0_INT2_IRQHandler
        LDR     R0, =ADMA_ASRC0_INT2_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA0_CH0_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA0_CH0_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA0_CH1_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA0_CH1_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA0_CH2_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA0_CH2_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA0_CH3_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA0_CH3_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA0_CH4_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA0_CH4_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA0_CH5_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA0_CH5_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ASRC1_INT1_IRQHandler
        PUBWEAK ADMA_ASRC1_INT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ASRC1_INT1_IRQHandler
        LDR     R0, =ADMA_ASRC1_INT1_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ASRC1_INT2_IRQHandler
        PUBWEAK ADMA_ASRC1_INT2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ASRC1_INT2_IRQHandler
        LDR     R0, =ADMA_ASRC1_INT2_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA1_CH0_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA1_CH0_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA1_CH1_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA1_CH1_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA1_CH2_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA1_CH2_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA1_CH3_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA1_CH3_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA1_CH4_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA1_CH4_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_DMA1_CH5_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_DMA1_CH5_INT_IRQHandler
        LDR     R0, =ADMA_EDMA1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ESAI0_INT_IRQHandler
        PUBWEAK ADMA_ESAI0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ESAI0_INT_IRQHandler
        LDR     R0, =ADMA_ESAI0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved488_IRQHandler
        PUBWEAK Reserved489_IRQHandler
        PUBWEAK ADMA_GPT0_INT_IRQHandler
        PUBWEAK ADMA_GPT0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_GPT0_INT_IRQHandler
        LDR     R0, =ADMA_GPT0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_GPT1_INT_IRQHandler
        PUBWEAK ADMA_GPT1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_GPT1_INT_IRQHandler
        LDR     R0, =ADMA_GPT1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_GPT2_INT_IRQHandler
        PUBWEAK ADMA_GPT2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_GPT2_INT_IRQHandler
        LDR     R0, =ADMA_GPT2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_GPT3_INT_IRQHandler
        PUBWEAK ADMA_GPT3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_GPT3_INT_IRQHandler
        LDR     R0, =ADMA_GPT3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_GPT4_INT_IRQHandler
        PUBWEAK ADMA_GPT4_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_GPT4_INT_IRQHandler
        LDR     R0, =ADMA_GPT4_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_GPT5_INT_IRQHandler
        PUBWEAK ADMA_GPT5_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_GPT5_INT_IRQHandler
        LDR     R0, =ADMA_GPT5_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI0_INT_IRQHandler
        PUBWEAK ADMA_SAI0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI0_INT_IRQHandler
        LDR     R0, =ADMA_SAI0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI1_INT_IRQHandler
        PUBWEAK ADMA_SAI1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI1_INT_IRQHandler
        LDR     R0, =ADMA_SAI1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI2_INT_IRQHandler
        PUBWEAK ADMA_SAI2_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI2_INT_IRQHandler
        LDR     R0, =ADMA_SAI2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI3_INT_IRQHandler
        PUBWEAK ADMA_SAI3_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI3_INT_IRQHandler
        LDR     R0, =ADMA_SAI3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved500_IRQHandler
        PUBWEAK Reserved501_IRQHandler
        PUBWEAK ADMA_SAI4_INT_IRQHandler
        PUBWEAK ADMA_SAI4_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI4_INT_IRQHandler
        LDR     R0, =ADMA_SAI4_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SAI5_INT_IRQHandler
        PUBWEAK ADMA_SAI5_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SAI5_INT_IRQHandler
        LDR     R0, =ADMA_SAI5_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPDIF0_RX_INT_IRQHandler
        PUBWEAK ADMA_SPDIF0_RX_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPDIF0_RX_INT_IRQHandler
        LDR     R0, =ADMA_SPDIF0_RX_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPDIF0_TX_INT_IRQHandler
        PUBWEAK ADMA_SPDIF0_TX_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPDIF0_TX_INT_IRQHandler
        LDR     R0, =ADMA_SPDIF0_TX_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved506_IRQHandler
        PUBWEAK Reserved507_IRQHandler
        PUBWEAK ADMA_ESAI0_MOD_INT_IRQHandler
        PUBWEAK ADMA_ESAI0_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ESAI0_MOD_INT_IRQHandler
        LDR     R0, =ADMA_ESAI0_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_ESAI0_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_ESAI0_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved510_IRQHandler
        PUBWEAK Reserved511_IRQHandler
        PUBWEAK Reserved512_IRQHandler
        PUBWEAK Reserved513_IRQHandler
        PUBWEAK Reserved514_IRQHandler
        PUBWEAK ADMA_SPI0_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI0_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI0_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI0_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI1_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI1_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI1_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI1_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI2_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI2_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI2_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI2_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI3_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI3_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPI3_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPI3_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C0_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C0_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C0_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C0_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C1_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C1_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C1_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C1_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C2_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C2_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C2_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C2_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C3_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C3_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_I2C3_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_I2C3_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA3_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved531_IRQHandler
        PUBWEAK Reserved532_IRQHandler
        PUBWEAK ADMA_UART0_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART0_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART0_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART0_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART1_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART1_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART1_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART1_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART2_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART2_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART2_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART2_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART3_DMA_RX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART3_DMA_RX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_UART3_DMA_TX_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_UART3_DMA_TX_INT_IRQHandler
        LDR     R0, =ADMA_EDMA2_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved541_IRQHandler
        PUBWEAK Reserved542_IRQHandler
        PUBWEAK Reserved543_IRQHandler
        PUBWEAK Reserved544_IRQHandler
        PUBWEAK Reserved545_IRQHandler
        PUBWEAK Reserved546_IRQHandler
        PUBWEAK SECURITY_MU1_A_INT_IRQHandler
        PUBWEAK SECURITY_MU2_A_INT_IRQHandler
        PUBWEAK SECURITY_MU3_A_INT_IRQHandler
        PUBWEAK SECURITY_CAAM_INT0_IRQHandler
        PUBWEAK SECURITY_CAAM_INT1_IRQHandler
        PUBWEAK SECURITY_CAAM_INT2_IRQHandler
        PUBWEAK SECURITY_CAAM_INT3_IRQHandler
        PUBWEAK SECURITY_CAAM_RTIC_INT_IRQHandler
        PUBWEAK ADMA_SPDIF0_RX_MOD_INT_IRQHandler
        PUBWEAK ADMA_SPDIF0_RX_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPDIF0_RX_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SPDIF0_RX_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPDIF0_RX_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPDIF0_RX_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPDIF0_TX_MOD_INT_IRQHandler
        PUBWEAK ADMA_SPDIF0_TX_MOD_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPDIF0_TX_MOD_INT_IRQHandler
        LDR     R0, =ADMA_SPDIF0_TX_MOD_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ADMA_SPDIF0_TX_DMA_INT_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADMA_SPDIF0_TX_DMA_INT_IRQHandler
        LDR     R0, =ADMA_EDMA0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved559_IRQHandler
        PUBWEAK Reserved560_IRQHandler
        PUBWEAK Reserved561_IRQHandler
        PUBWEAK Reserved562_IRQHandler
        PUBWEAK VPU_VPU_INT_0_IRQHandler
        PUBWEAK VPU_VPU_INT_1_IRQHandler
        PUBWEAK VPU_VPU_INT_2_IRQHandler
        PUBWEAK VPU_VPU_INT_3_IRQHandler
        PUBWEAK VPU_VPU_INT_4_IRQHandler
        PUBWEAK Reserved568_IRQHandler
        PUBWEAK Reserved569_IRQHandler
        PUBWEAK Reserved570_IRQHandler
        PUBWEAK Reserved571_IRQHandler
        PUBWEAK Reserved572_IRQHandler
        PUBWEAK Reserved573_IRQHandler
        PUBWEAK Reserved574_IRQHandler
        PUBWEAK Reserved575_IRQHandler
        PUBWEAK Reserved576_IRQHandler
        PUBWEAK Reserved577_IRQHandler
        PUBWEAK Reserved578_IRQHandler
        PUBWEAK Reserved579_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_TPM_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_TPM_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_INTMUX_SOURCE_TPM_IRQHandler
        LDR     R0, =M4_INTMUX_SOURCE_TPM_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved581_IRQHandler
        PUBWEAK Reserved582_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_LPIT_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_LPIT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_INTMUX_SOURCE_LPIT_IRQHandler
        LDR     R0, =M4_INTMUX_SOURCE_LPIT_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved584_IRQHandler
        PUBWEAK Reserved585_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_LPUART_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_LPUART_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_INTMUX_SOURCE_LPUART_IRQHandler
        LDR     R0, =M4_INTMUX_SOURCE_LPUART_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved587_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_LPI2C_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_LPI2C_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_INTMUX_SOURCE_LPI2C_IRQHandler
        LDR     R0, =M4_INTMUX_SOURCE_LPI2C_DriverIRQHandler
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
        PUBWEAK M4_INTMUX_SOURCE_MU0_A3_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_MU0_A3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_INTMUX_SOURCE_MU0_A3_IRQHandler
        LDR     R0, =M4_INTMUX_SOURCE_MU0_A3_DriverIRQHandler
        BX      R0

        PUBWEAK M4_INTMUX_SOURCE_MU0_A2_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_MU0_A2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_INTMUX_SOURCE_MU0_A2_IRQHandler
        LDR     R0, =M4_INTMUX_SOURCE_MU0_A2_DriverIRQHandler
        BX      R0

        PUBWEAK M4_INTMUX_SOURCE_MU0_A1_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_MU0_A1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_INTMUX_SOURCE_MU0_A1_IRQHandler
        LDR     R0, =M4_INTMUX_SOURCE_MU0_A1_DriverIRQHandler
        BX      R0

        PUBWEAK M4_INTMUX_SOURCE_MU0_A0_IRQHandler
        PUBWEAK M4_INTMUX_SOURCE_MU0_A0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
M4_INTMUX_SOURCE_MU0_A0_IRQHandler
        LDR     R0, =M4_INTMUX_SOURCE_MU0_A0_DriverIRQHandler
        BX      R0

        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved16_IRQHandler
Reserved17_IRQHandler
Reserved18_IRQHandler
Reserved19_IRQHandler
Reserved20_IRQHandler
M4_MCM_IRQHandler
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
M4_TPM_IRQHandler
Reserved36_IRQHandler
Reserved37_IRQHandler
M4_LPIT_IRQHandler
Reserved39_IRQHandler
Reserved40_IRQHandler
M4_LPUART_DriverIRQHandler
Reserved42_IRQHandler
M4_LPI2C_DriverIRQHandler
Reserved44_IRQHandler
M4_MU0_B0_IRQHandler
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
M4_MU0_B1_IRQHandler
M4_MU0_B2_IRQHandler
M4_MU0_B3_IRQHandler
Reserved63_IRQHandler
Reserved64_IRQHandler
M4_MU1_A_IRQHandler
M4_SW_IRQHandler
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
A35_NEXTERRIRQ_IRQHandler
A35_NINTERRIRQ_IRQHandler
Reserved101_IRQHandler
Reserved102_IRQHandler
Reserved103_IRQHandler
Reserved104_IRQHandler
Reserved105_IRQHandler
Reserved106_IRQHandler
Reserved107_IRQHandler
Reserved108_IRQHandler
Reserved109_IRQHandler
Reserved110_IRQHandler
Reserved111_IRQHandler
Reserved112_IRQHandler
Reserved113_IRQHandler
Reserved114_IRQHandler
M4_INT_OUT0_IRQHandler
M4_INT_OUT1_IRQHandler
M4_INT_OUT2_IRQHandler
M4_INT_OUT3_IRQHandler
M4_INT_OUT4_IRQHandler
M4_INT_OUT5_IRQHandler
M4_INT_OUT6_IRQHandler
M4_INT_OUT7_IRQHandler
Reserved123_IRQHandler
Reserved124_IRQHandler
Reserved125_IRQHandler
Reserved126_IRQHandler
Reserved127_IRQHandler
Reserved128_IRQHandler
Reserved129_IRQHandler
Reserved130_IRQHandler
Reserved131_IRQHandler
Reserved132_IRQHandler
Reserved133_IRQHandler
Reserved134_IRQHandler
Reserved135_IRQHandler
Reserved136_IRQHandler
Reserved137_IRQHandler
Reserved138_IRQHandler
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
Reserved156_IRQHandler
Reserved157_IRQHandler
MIPI_DSI0_INT_OUT_IRQHandler
MIPI_DSI1_INT_OUT_IRQHandler
Reserved160_IRQHandler
LCD_MOD_INT_IRQHandler
LCD_PWM_INT_IRQHandler
GPU0_XAQ2_INTR_IRQHandler
Reserved164_IRQHandler
ADMA_EDMA2_INT_DriverIRQHandler
ADMA_EDMA3_INT_DriverIRQHandler
Reserved169_IRQHandler
Reserved170_IRQHandler
Reserved171_IRQHandler
Reserved172_IRQHandler
Reserved173_IRQHandler
Reserved174_IRQHandler
Reserved175_IRQHandler
Reserved176_IRQHandler
Reserved177_IRQHandler
Reserved178_IRQHandler
LSIO_GPT0_INT_IRQHandler
LSIO_GPT1_INT_IRQHandler
LSIO_GPT2_INT_IRQHandler
LSIO_GPT3_INT_IRQHandler
LSIO_GPT4_INT_IRQHandler
LSIO_KPP_INT_IRQHandler
Reserved185_IRQHandler
Reserved186_IRQHandler
Reserved187_IRQHandler
Reserved188_IRQHandler
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
DRC_ECC_CORRECT_INT_IRQHandler
DRC_ECC_NCORRECT_INT_IRQHandler
DRC_SBR_DONE_INT_IRQHandler
DRC_PERF_CNT_INT_IRQHandler
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
Reserved251_IRQHandler
Reserved252_IRQHandler
Reserved253_IRQHandler
Reserved254_IRQHandler
Reserved255_IRQHandler
Reserved256_IRQHandler
Reserved257_IRQHandler
Reserved258_IRQHandler
Reserved259_IRQHandler
Reserved260_IRQHandler
Reserved261_IRQHandler
Reserved262_IRQHandler
Reserved263_IRQHandler
Reserved264_IRQHandler
Reserved265_IRQHandler
Reserved266_IRQHandler
Reserved267_IRQHandler
Reserved268_IRQHandler
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
ADMA_SPI0_INT_DriverIRQHandler
ADMA_SPI1_INT_DriverIRQHandler
ADMA_SPI2_INT_DriverIRQHandler
ADMA_SPI3_INT_DriverIRQHandler
ADMA_I2C0_INT_DriverIRQHandler
ADMA_I2C1_INT_DriverIRQHandler
ADMA_I2C2_INT_DriverIRQHandler
ADMA_I2C3_INT_DriverIRQHandler
ADMA_I2C4_INT_DriverIRQHandler
ADMA_UART0_INT_DriverIRQHandler
ADMA_UART1_INT_DriverIRQHandler
ADMA_UART2_INT_DriverIRQHandler
ADMA_UART3_INT_DriverIRQHandler
Reserved328_IRQHandler
Reserved329_IRQHandler
Reserved330_IRQHandler
CONNECTIVITY_USDHC0_INT_DriverIRQHandler
CONNECTIVITY_USDHC1_INT_DriverIRQHandler
CONNECTIVITY_USDHC2_INT_DriverIRQHandler
ADMA_FLEXCAN0_INT_DriverIRQHandler
ADMA_FLEXCAN1_INT_DriverIRQHandler
ADMA_FLEXCAN2_INT_DriverIRQHandler
ADMA_FTM0_INT_DriverIRQHandler
ADMA_FTM1_INT_DriverIRQHandler
ADMA_ADC0_INT_DriverIRQHandler
Reserved340_IRQHandler
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
ADMA_SAI0_MOD_INT_DriverIRQHandler
ADMA_EDMA0_INT_DriverIRQHandler
ADMA_SAI1_MOD_INT_DriverIRQHandler
ADMA_SAI2_MOD_INT_DriverIRQHandler
MIPI_CSI0_OUT_INT_IRQHandler
Reserved420_IRQHandler
Reserved421_IRQHandler
ADMA_SAI3_MOD_INT_DriverIRQHandler
Reserved424_IRQHandler
Reserved425_IRQHandler
Reserved426_IRQHandler
Reserved427_IRQHandler
ADMA_SAI4_MOD_INT_DriverIRQHandler
ADMA_EDMA1_INT_DriverIRQHandler
ADMA_SAI5_MOD_INT_DriverIRQHandler
Reserved432_IRQHandler
Reserved433_IRQHandler
Reserved434_IRQHandler
ADMA_SPI0_MOD_INT_DriverIRQHandler
ADMA_SPI1_MOD_INT_DriverIRQHandler
ADMA_SPI2_MOD_INT_DriverIRQHandler
ADMA_SPI3_MOD_INT_DriverIRQHandler
ADMA_I2C0_MOD_INT_DriverIRQHandler
ADMA_I2C1_MOD_INT_DriverIRQHandler
ADMA_I2C2_MOD_INT_DriverIRQHandler
ADMA_I2C3_MOD_INT_DriverIRQHandler
Reserved443_IRQHandler
ADMA_UART0_MOD_INT_DriverIRQHandler
ADMA_UART1_MOD_INT_DriverIRQHandler
ADMA_UART2_MOD_INT_DriverIRQHandler
ADMA_UART3_MOD_INT_DriverIRQHandler
Reserved448_IRQHandler
Reserved449_IRQHandler
Reserved450_IRQHandler
ADMA_FLEXCAN0_MOD_INT_DriverIRQHandler
ADMA_FLEXCAN1_MOD_INT_DriverIRQHandler
ADMA_FLEXCAN2_MOD_INT_DriverIRQHandler
ADMA_FTM0_MOD_INT_DriverIRQHandler
ADMA_FTM1_MOD_INT_DriverIRQHandler
ADMA_ADC0_MOD_INT_DriverIRQHandler
Reserved457_IRQHandler
Reserved465_IRQHandler
Reserved466_IRQHandler
ADMA_ASRC0_INT1_DriverIRQHandler
ADMA_ASRC0_INT2_DriverIRQHandler
ADMA_ASRC1_INT1_DriverIRQHandler
ADMA_ASRC1_INT2_DriverIRQHandler
ADMA_ESAI0_INT_DriverIRQHandler
Reserved488_IRQHandler
Reserved489_IRQHandler
ADMA_GPT0_INT_DriverIRQHandler
ADMA_GPT1_INT_DriverIRQHandler
ADMA_GPT2_INT_DriverIRQHandler
ADMA_GPT3_INT_DriverIRQHandler
ADMA_GPT4_INT_DriverIRQHandler
ADMA_GPT5_INT_DriverIRQHandler
ADMA_SAI0_INT_DriverIRQHandler
ADMA_SAI1_INT_DriverIRQHandler
ADMA_SAI2_INT_DriverIRQHandler
ADMA_SAI3_INT_DriverIRQHandler
Reserved500_IRQHandler
Reserved501_IRQHandler
ADMA_SAI4_INT_DriverIRQHandler
ADMA_SAI5_INT_DriverIRQHandler
ADMA_SPDIF0_RX_INT_DriverIRQHandler
ADMA_SPDIF0_TX_INT_DriverIRQHandler
Reserved506_IRQHandler
Reserved507_IRQHandler
ADMA_ESAI0_MOD_INT_DriverIRQHandler
Reserved510_IRQHandler
Reserved511_IRQHandler
Reserved512_IRQHandler
Reserved513_IRQHandler
Reserved514_IRQHandler
Reserved531_IRQHandler
Reserved532_IRQHandler
Reserved541_IRQHandler
Reserved542_IRQHandler
Reserved543_IRQHandler
Reserved544_IRQHandler
Reserved545_IRQHandler
Reserved546_IRQHandler
SECURITY_MU1_A_INT_IRQHandler
SECURITY_MU2_A_INT_IRQHandler
SECURITY_MU3_A_INT_IRQHandler
SECURITY_CAAM_INT0_IRQHandler
SECURITY_CAAM_INT1_IRQHandler
SECURITY_CAAM_INT2_IRQHandler
SECURITY_CAAM_INT3_IRQHandler
SECURITY_CAAM_RTIC_INT_IRQHandler
ADMA_SPDIF0_RX_MOD_INT_DriverIRQHandler
ADMA_SPDIF0_TX_MOD_INT_DriverIRQHandler
Reserved559_IRQHandler
Reserved560_IRQHandler
Reserved561_IRQHandler
Reserved562_IRQHandler
VPU_VPU_INT_0_IRQHandler
VPU_VPU_INT_1_IRQHandler
VPU_VPU_INT_2_IRQHandler
VPU_VPU_INT_3_IRQHandler
VPU_VPU_INT_4_IRQHandler
Reserved568_IRQHandler
Reserved569_IRQHandler
Reserved570_IRQHandler
Reserved571_IRQHandler
Reserved572_IRQHandler
Reserved573_IRQHandler
Reserved574_IRQHandler
Reserved575_IRQHandler
Reserved576_IRQHandler
Reserved577_IRQHandler
Reserved578_IRQHandler
Reserved579_IRQHandler
M4_INTMUX_SOURCE_TPM_DriverIRQHandler
Reserved581_IRQHandler
Reserved582_IRQHandler
M4_INTMUX_SOURCE_LPIT_DriverIRQHandler
Reserved584_IRQHandler
Reserved585_IRQHandler
M4_INTMUX_SOURCE_LPUART_DriverIRQHandler
Reserved587_IRQHandler
M4_INTMUX_SOURCE_LPI2C_DriverIRQHandler
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
M4_INTMUX_SOURCE_MU0_A3_DriverIRQHandler
M4_INTMUX_SOURCE_MU0_A2_DriverIRQHandler
M4_INTMUX_SOURCE_MU0_A1_DriverIRQHandler
M4_INTMUX_SOURCE_MU0_A0_DriverIRQHandler
DefaultISR
        B DefaultISR

        END
