/*
** ###################################################################
**     Version:             rev. 1.0, 2018-06-25
**     Build:               b191031
**
**     Abstract:
**         Chip specific module features.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2019 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2018-06-25)
**         Initial version.
**
** ###################################################################
*/

#ifndef _MKW38A4_FEATURES_H_
#define _MKW38A4_FEATURES_H_

/* SOC module features */

/* @brief ADC16 availability on the SoC. */
#define FSL_FEATURE_SOC_ADC16_COUNT (1)
/* @brief AIPS availability on the SoC. */
#define FSL_FEATURE_SOC_AIPS_COUNT (1)
/* @brief AXBS availability on the SoC. */
#define FSL_FEATURE_SOC_AXBS_COUNT (1)
/* @brief CMP availability on the SoC. */
#define FSL_FEATURE_SOC_CMP_COUNT (1)
/* @brief CMT availability on the SoC. */
#define FSL_FEATURE_SOC_CMT_COUNT (1)
/* @brief COP availability on the SoC. */
#define FSL_FEATURE_SOC_COP_COUNT (1)
/* @brief DCDC availability on the SoC. */
#define FSL_FEATURE_SOC_DCDC_COUNT (1)
/* @brief DMAMUX availability on the SoC. */
#define FSL_FEATURE_SOC_DMAMUX_COUNT (1)
/* @brief DSPI availability on the SoC. */
#define FSL_FEATURE_SOC_DSPI_COUNT (2)
/* @brief EDMA availability on the SoC. */
#define FSL_FEATURE_SOC_EDMA_COUNT (1)
/* @brief FGPIO availability on the SoC. */
#define FSL_FEATURE_SOC_FGPIO_COUNT (3)
/* @brief FLEXCAN availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXCAN_COUNT (1)
/* @brief FTFE availability on the SoC. */
#define FSL_FEATURE_SOC_FTFE_COUNT (1)
/* @brief GPIO availability on the SoC. */
#define FSL_FEATURE_SOC_GPIO_COUNT (3)
/* @brief I2C availability on the SoC. */
#define FSL_FEATURE_SOC_I2C_COUNT (2)
/* @brief LLWU availability on the SoC. */
#define FSL_FEATURE_SOC_LLWU_COUNT (1)
/* @brief LPTMR availability on the SoC. */
#define FSL_FEATURE_SOC_LPTMR_COUNT (1)
/* @brief LPUART availability on the SoC. */
#define FSL_FEATURE_SOC_LPUART_COUNT (2)
/* @brief LTC availability on the SoC. */
#define FSL_FEATURE_SOC_LTC_COUNT (1)
/* @brief MCG availability on the SoC. */
#define FSL_FEATURE_SOC_MCG_COUNT (1)
/* @brief MCM availability on the SoC. */
#define FSL_FEATURE_SOC_MCM_COUNT (1)
/* @brief MTB availability on the SoC. */
#define FSL_FEATURE_SOC_MTB_COUNT (1)
/* @brief MTBDWT availability on the SoC. */
#define FSL_FEATURE_SOC_MTBDWT_COUNT (1)
/* @brief PIT availability on the SoC. */
#define FSL_FEATURE_SOC_PIT_COUNT (1)
/* @brief PMC availability on the SoC. */
#define FSL_FEATURE_SOC_PMC_COUNT (1)
/* @brief PORT availability on the SoC. */
#define FSL_FEATURE_SOC_PORT_COUNT (3)
/* @brief RCM availability on the SoC. */
#define FSL_FEATURE_SOC_RCM_COUNT (1)
/* @brief RFSYS availability on the SoC. */
#define FSL_FEATURE_SOC_RFSYS_COUNT (1)
/* @brief ROM availability on the SoC. */
#define FSL_FEATURE_SOC_ROM_COUNT (1)
/* @brief RSIM availability on the SoC. */
#define FSL_FEATURE_SOC_RSIM_COUNT (1)
/* @brief RTC availability on the SoC. */
#define FSL_FEATURE_SOC_RTC_COUNT (1)
/* @brief SIM availability on the SoC. */
#define FSL_FEATURE_SOC_SIM_COUNT (1)
/* @brief SMC availability on the SoC. */
#define FSL_FEATURE_SOC_SMC_COUNT (1)
/* @brief TPM availability on the SoC. */
#define FSL_FEATURE_SOC_TPM_COUNT (3)
/* @brief TRNG availability on the SoC. */
#define FSL_FEATURE_SOC_TRNG_COUNT (1)
/* @brief VREF availability on the SoC. */
#define FSL_FEATURE_SOC_VREF_COUNT (1)

/* ADC16 module features */

/* @brief Has Programmable Gain Amplifier (PGA) in ADC (register PGA). */
#define FSL_FEATURE_ADC16_HAS_PGA (0)
/* @brief Has PGA chopping control in ADC (bit PGA[PGACHPb] or PGA[PGACHP]). */
#define FSL_FEATURE_ADC16_HAS_PGA_CHOPPING (0)
/* @brief Has PGA offset measurement mode in ADC (bit PGA[PGAOFSM]). */
#define FSL_FEATURE_ADC16_HAS_PGA_OFFSET_MEASUREMENT (0)
/* @brief Has DMA support (bit SC2[DMAEN] or SC4[DMAEN]). */
#define FSL_FEATURE_ADC16_HAS_DMA (1)
/* @brief Has differential mode (bitfield SC1x[DIFF]). */
#define FSL_FEATURE_ADC16_HAS_DIFF_MODE (1)
/* @brief Has FIFO (bit SC4[AFDEP]). */
#define FSL_FEATURE_ADC16_HAS_FIFO (0)
/* @brief FIFO size if available (bitfield SC4[AFDEP]). */
#define FSL_FEATURE_ADC16_FIFO_SIZE (0)
/* @brief Has channel set a/b multiplexor (bitfield CFG2[MUXSEL]). */
#define FSL_FEATURE_ADC16_HAS_MUX_SELECT (1)
/* @brief Has HW trigger masking (bitfield SC5[HTRGMASKE]. */
#define FSL_FEATURE_ADC16_HAS_HW_TRIGGER_MASK (0)
/* @brief Has calibration feature (bit SC3[CAL] and registers CLPx, CLMx). */
#define FSL_FEATURE_ADC16_HAS_CALIBRATION (1)
/* @brief Has HW averaging (bit SC3[AVGE]). */
#define FSL_FEATURE_ADC16_HAS_HW_AVERAGE (1)
/* @brief Has offset correction (register OFS). */
#define FSL_FEATURE_ADC16_HAS_OFFSET_CORRECTION (1)
/* @brief Maximum ADC resolution. */
#define FSL_FEATURE_ADC16_MAX_RESOLUTION (16)
/* @brief Number of SC1x and Rx register pairs (conversion control and result registers). */
#define FSL_FEATURE_ADC16_CONVERSION_CONTROL_COUNT (2)

/* AXBS module features */

/* No feature definitions */

/* BTLE_RF module features */

/* No feature definitions */

/* FLEXCAN module features */

/* @brief Message buffer size */
#define FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(x) (32)
/* @brief Has doze mode support (register bit field MCR[DOZE]). */
#define FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT (1)
/* @brief Insatnce has doze mode support (register bit field MCR[DOZE]). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_DOZE_MODE_SUPPORTn(x) (1)
/* @brief Has a glitch filter on the receive pin (register bit field MCR[WAKSRC]). */
#define FSL_FEATURE_FLEXCAN_HAS_GLITCH_FILTER (1)
/* @brief Has extended interrupt mask and flag register (register IMASK2, IFLAG2). */
#define FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER (0)
/* @brief Instance has extended bit timing register (register CBT). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_EXTENDED_TIMING_REGISTERn(x) (1)
/* @brief Has a receive FIFO DMA feature (register bit field MCR[DMA]). */
#define FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA (1)
/* @brief Instance has a receive FIFO DMA feature (register bit field MCR[DMA]). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_RX_FIFO_DMAn(x) (1)
/* @brief Remove CAN Engine Clock Source Selection from unsupported part. */
#define FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE (0)
/* @brief Instance remove CAN Engine Clock Source Selection from unsupported part. */
#define FSL_FEATURE_FLEXCAN_INSTANCE_SUPPORT_ENGINE_CLK_SEL_REMOVEn(x) (0)
/* @brief Is affected by errata with ID 5641 (Module does not transmit a message that is enabled to be transmitted at a specific moment during the arbitration process). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_5641 (0)
/* @brief Is affected by errata with ID 5829 (FlexCAN: FlexCAN does not transmit a message that is enabled to be transmitted in a specific moment during the arbitration process). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829 (0)
/* @brief Is affected by errata with ID 6032 (FlexCAN: A frame with wrong ID or payload is transmitted into the CAN bus when the Message Buffer under transmission is either aborted or deactivated while the CAN bus is in the Bus Idle state). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_6032 (0)
/* @brief Is affected by errata with ID 9595 (FlexCAN: Corrupt frame possible if the Freeze Mode or the Low-Power Mode are entered during a Bus-Off state). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_9595 (0)
/* @brief Has CAN with Flexible Data rate (CAN FD) protocol. */
#define FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE (1)
/* @brief CAN instance support Flexible Data rate (CAN FD) protocol. */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_FLEXIBLE_DATA_RATEn(x) (1)
/* @brief Has extra MB interrupt or common one. */
#define FSL_FEATURE_FLEXCAN_HAS_EXTRA_MB_INT (1)
/* @brief Has memory error control (register MECR). */
#define FSL_FEATURE_FLEXCAN_HAS_MEMORY_ERROR_CONTROL (0)

/* CMP module features */

/* @brief Has Trigger mode in CMP (register bit field CR1[TRIGM]). */
#define FSL_FEATURE_CMP_HAS_TRIGGER_MODE (1)
/* @brief Has Window mode in CMP (register bit field CR1[WE]). */
#define FSL_FEATURE_CMP_HAS_WINDOW_MODE (0)
/* @brief Has External sample supported in CMP (register bit field CR1[SE]). */
#define FSL_FEATURE_CMP_HAS_EXTERNAL_SAMPLE_SUPPORT (0)
/* @brief Has DMA support in CMP (register bit field SCR[DMAEN]). */
#define FSL_FEATURE_CMP_HAS_DMA (1)
/* @brief Has Pass Through mode in CMP (register bit field MUXCR[PSTM]). */
#define FSL_FEATURE_CMP_HAS_PASS_THROUGH_MODE (0)
/* @brief Has DAC Test function in CMP (register DACTEST). */
#define FSL_FEATURE_CMP_HAS_DAC_TEST (0)

/* COP module features */

/* @brief Has the COP Debug Enable bit (COPC[COPDBGEN]) */
#define FSL_FEATURE_COP_HAS_DEBUG_ENABLE (1)
/* @brief Has the COP Stop mode Enable bit (COPC[COPSTPEN]) */
#define FSL_FEATURE_COP_HAS_STOP_ENABLE (1)
/* @brief Has more clock sources like MCGIRC */
#define FSL_FEATURE_COP_HAS_MORE_CLKSRC (1)
/* @brief Has the timeout long and short mode bit (COPC[COPCLKS]) */
#define FSL_FEATURE_COP_HAS_LONGTIME_MODE (1)

/* DCDC module features */

/* @brief Has VDD1P5 bits in DCDC REG3. */
#define FSL_FEATURE_DCDC_REG3_HAS_VDD1P5_BITS (1)
/* @brief Has VDD1P45 bits in DCDC REG3. */
#define FSL_FEATURE_DCDC_REG3_HAS_VDD1P45_BITS (0)
/* @brief Has BOOST mode: at least one of the bits DCDC_REG1[POSLIMIT_BOOST_IN], DCDC_REG3[DCDC_VDD1P45CTRL_TRG_BOOST] or DCDC_REG3[DCDC_VDD1P5CTRL_TRG_BOOST]. */
#define FSL_FEATURE_DCDC_HAS_BOOST_MODE (0)

/* EDMA module features */

/* @brief Number of DMA channels (related to number of registers TCD, DCHPRI, bit fields ERQ[ERQn], EEI[EEIn], INT[INTn], ERR[ERRn], HRS[HRSn] and bit field widths ES[ERRCHN], CEEI[CEEI], SEEI[SEEI], CERQ[CERQ], SERQ[SERQ], CDNE[CDNE], SSRT[SSRT], CERR[CERR], CINT[CINT], TCDn_CITER_ELINKYES[LINKCH], TCDn_CSR[MAJORLINKCH], TCDn_BITER_ELINKYES[LINKCH]). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_MODULE_CHANNEL (4)
/* @brief Total number of DMA channels on all modules. */
#define FSL_FEATURE_EDMA_DMAMUX_CHANNELS (4)
/* @brief Number of DMA channel groups (register bit fields CR[ERGA], CR[GRPnPRI], ES[GPE], DCHPRIn[GRPPRI]). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT (1)
/* @brief Has DMA_Error interrupt vector. */
#define FSL_FEATURE_EDMA_HAS_ERROR_IRQ (0)
/* @brief Number of DMA channels with asynchronous request capability (register EARS). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT (4)
/* @brief Channel IRQ entry shared offset. */
#define FSL_FEATURE_EDMA_MODULE_CHANNEL_IRQ_ENTRY_SHARED_OFFSET (0)
/* @brief If 8 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_8_BYTES_TRANSFER (0)
/* @brief If 16 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_16_BYTES_TRANSFER (1)

/* DMAMUX module features */

/* @brief Number of DMA channels (related to number of register CHCFGn). */
#define FSL_FEATURE_DMAMUX_MODULE_CHANNEL (4)
/* @brief Total number of DMA channels on all modules. */
#define FSL_FEATURE_DMAMUX_DMAMUX_CHANNELS (FSL_FEATURE_SOC_DMAMUX_COUNT * 4)
/* @brief Has the periodic trigger capability for the triggered DMA channel (register bit CHCFG0[TRIG]). */
#define FSL_FEATURE_DMAMUX_HAS_TRIG (1)
/* @brief Has DMA Channel Always ON function (register bit CHCFG0[A_ON]). */
#define FSL_FEATURE_DMAMUX_HAS_A_ON (0)

/* FLASH module features */

/* @brief Is of type FTFA. */
#define FSL_FEATURE_FLASH_IS_FTFA (0)
/* @brief Is of type FTFE. */
#define FSL_FEATURE_FLASH_IS_FTFE (1)
/* @brief Is of type FTFL. */
#define FSL_FEATURE_FLASH_IS_FTFL (0)
/* @brief Has flags indicating the status of the FlexRAM (register bits FCNFG[EEERDY], FCNFG[RAMRDY] and FCNFG[PFLSH]). */
#define FSL_FEATURE_FLASH_HAS_FLEX_RAM_FLAGS (1)
/* @brief Has program flash swapping status flag (register bit FCNFG[SWAP]). */
#define FSL_FEATURE_FLASH_HAS_PFLASH_SWAPPING_STATUS_FLAG (1)
/* @brief Has EEPROM region protection (register FEPROT). */
#define FSL_FEATURE_FLASH_HAS_EEROM_REGION_PROTECTION (1)
/* @brief Has data flash region protection (register FDPROT). */
#define FSL_FEATURE_FLASH_HAS_DATA_FLASH_REGION_PROTECTION (1)
/* @brief Has flash access control (registers XACCHn, SACCHn, where n is a number, FACSS and FACSN). */
#define FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL (0)
/* @brief Has flash cache control in FMC module. */
#define FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS (0)
/* @brief Has flash cache control in MCM module. */
#define FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS (1)
/* @brief Has flash cache control in MSCM module. */
#define FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CACHE_CONTROLS (0)
/* @brief Has prefetch speculation control in flash, such as kv5x. */
#define FSL_FEATURE_FLASH_PREFETCH_SPECULATION_CONTROL_IN_FLASH (0)
/* @brief P-Flash flash size coding rule version, value 0 for K1 and K2, value 1 for K3. */
#define FSL_FEATURE_FLASH_SIZE_ENCODING_RULE_VERSION (0)
/* @brief P-Flash start address. */
#define FSL_FEATURE_FLASH_PFLASH_START_ADDRESS (0x00000000)
/* @brief P-Flash block count. */
#define FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT (1)
/* @brief P-Flash block size. */
#define FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE (262144)
/* @brief P-Flash sector size. */
#define FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE (2048)
/* @brief P-Flash write unit size. */
#define FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE (8)
/* @brief P-Flash data path width. */
#define FSL_FEATURE_FLASH_PFLASH_BLOCK_DATA_PATH_WIDTH (8)
/* @brief P-Flash block swap feature. */
#define FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP (0)
/* @brief P-Flash protection region count. */
#define FSL_FEATURE_FLASH_PFLASH_PROTECTION_REGION_COUNT (32)
/* @brief Has FlexNVM memory. */
#define FSL_FEATURE_FLASH_HAS_FLEX_NVM (1)
/* @brief Has FlexNVM alias. */
#define FSL_FEATURE_FLASH_HAS_FLEX_NVM_ALIAS (1)
/* @brief FlexNVM start address. (Valid only if FlexNVM is available.) */
#define FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS (0x10000000)
/* @brief FlexNVM alias start address. (Valid only if FlexNVM alias is available.) */
#define FSL_FEATURE_FLASH_FLEX_NVM_ALIAS_START_ADDRESS (0x00040000)
/* @brief FlexNVM block count. */
#define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT (1)
/* @brief FlexNVM block size. */
#define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE (262144)
/* @brief FlexNVM sector size. */
#define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE (2048)
/* @brief FlexNVM write unit size. */
#define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE (8)
/* @brief FlexNVM data path width. */
#define FSL_FEATURE_FLASH_FLEX_BLOCK_DATA_PATH_WIDTH (8)
/* @brief Has FlexRAM memory. */
#define FSL_FEATURE_FLASH_HAS_FLEX_RAM (1)
/* @brief FlexRAM start address. (Valid only if FlexRAM is available.) */
#define FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS (0x14000000)
/* @brief FlexRAM size. */
#define FSL_FEATURE_FLASH_FLEX_RAM_SIZE (8192)
/* @brief Has 0x00 Read 1s Block command. */
#define FSL_FEATURE_FLASH_HAS_READ_1S_BLOCK_CMD (1)
/* @brief Has 0x01 Read 1s Section command. */
#define FSL_FEATURE_FLASH_HAS_READ_1S_SECTION_CMD (1)
/* @brief Has 0x02 Program Check command. */
#define FSL_FEATURE_FLASH_HAS_PROGRAM_CHECK_CMD (1)
/* @brief Has 0x03 Read Resource command. */
#define FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD (1)
/* @brief Has 0x06 Program Longword command. */
#define FSL_FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD (0)
/* @brief Has 0x07 Program Phrase command. */
#define FSL_FEATURE_FLASH_HAS_PROGRAM_PHRASE_CMD (1)
/* @brief Has 0x08 Erase Flash Block command. */
#define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_BLOCK_CMD (1)
/* @brief Has 0x09 Erase Flash Sector command. */
#define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_SECTOR_CMD (1)
/* @brief Has 0x0B Program Section command. */
#define FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD (1)
/* @brief Has 0x40 Read 1s All Blocks command. */
#define FSL_FEATURE_FLASH_HAS_READ_1S_ALL_BLOCKS_CMD (1)
/* @brief Has 0x41 Read Once command. */
#define FSL_FEATURE_FLASH_HAS_READ_ONCE_CMD (1)
/* @brief Has 0x43 Program Once command. */
#define FSL_FEATURE_FLASH_HAS_PROGRAM_ONCE_CMD (1)
/* @brief Has 0x44 Erase All Blocks command. */
#define FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_CMD (1)
/* @brief Has 0x45 Verify Backdoor Access Key command. */
#define FSL_FEATURE_FLASH_HAS_VERIFY_BACKDOOR_ACCESS_KEY_CMD (1)
/* @brief Has 0x46 Swap Control command. */
#define FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD (0)
/* @brief Has 0x49 Erase All Blocks Unsecure command. */
#define FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD (1)
/* @brief Has 0x4A Read 1s All Execute-only Segments command. */
#define FSL_FEATURE_FLASH_HAS_READ_1S_ALL_EXECUTE_ONLY_SEGMENTS_CMD (1)
/* @brief Has 0x4B Erase All Execute-only Segments command. */
#define FSL_FEATURE_FLASH_HAS_ERASE_ALL_EXECUTE_ONLY_SEGMENTS_CMD (1)
/* @brief Has 0x80 Program Partition command. */
#define FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD (1)
/* @brief Has 0x81 Set FlexRAM Function command. */
#define FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD (1)
/* @brief P-Flash Erase/Read 1st all block command address alignment. */
#define FSL_FEATURE_FLASH_PFLASH_BLOCK_CMD_ADDRESS_ALIGMENT (8)
/* @brief P-Flash Erase sector command address alignment. */
#define FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT (8)
/* @brief P-Flash Rrogram/Verify section command address alignment. */
#define FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT (8)
/* @brief P-Flash Read resource command address alignment. */
#define FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT (8)
/* @brief P-Flash Program check command address alignment. */
#define FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT (4)
/* @brief P-Flash Program check command address alignment. */
#define FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT (0)
/* @brief FlexNVM Erase/Read 1st all block command address alignment. */
#define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_CMD_ADDRESS_ALIGMENT (8)
/* @brief FlexNVM Erase sector command address alignment. */
#define FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT (8)
/* @brief FlexNVM Rrogram/Verify section command address alignment. */
#define FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT (8)
/* @brief FlexNVM Read resource command address alignment. */
#define FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT (8)
/* @brief FlexNVM Program check command address alignment. */
#define FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT (4)
/* @brief FlexNVM partition code 0000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 (0x00040000U)
/* @brief FlexNVM partition code 0001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 (0xFFFFFFFFU)
/* @brief FlexNVM partition code 0010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 (0xFFFFFFFFU)
/* @brief FlexNVM partition code 0011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 (0x00038000U)
/* @brief FlexNVM partition code 0100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 (0x00030000U)
/* @brief FlexNVM partition code 0101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 (0x00020000U)
/* @brief FlexNVM partition code 0110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 (0x00000000U)
/* @brief FlexNVM partition code 0111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 (0xFFFFFFFFU)
/* @brief FlexNVM partition code 1000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 (0x00000000U)
/* @brief FlexNVM partition code 1001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 (0xFFFFFFFFU)
/* @brief FlexNVM partition code 1010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 (0xFFFFFFFFU)
/* @brief FlexNVM partition code 1011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 (0x00008000U)
/* @brief FlexNVM partition code 1100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 (0x00010000U)
/* @brief FlexNVM partition code 1101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 (0x00020000U)
/* @brief FlexNVM partition code 1110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 (0x00040000U)
/* @brief FlexNVM partition code 1111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 (0x00040000U)
/* @brief Emulated eeprom size code 0000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0000 (0xFFFF)
/* @brief Emulated eeprom size code 0001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0001 (0x2000)
/* @brief Emulated eeprom size code 0010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0010 (0x1000)
/* @brief Emulated eeprom size code 0011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0011 (0x0800)
/* @brief Emulated eeprom size code 0100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0100 (0x0400)
/* @brief Emulated eeprom size code 0101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0101 (0x0200)
/* @brief Emulated eeprom size code 0110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0110 (0x0100)
/* @brief Emulated eeprom size code 0111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0111 (0x0080)
/* @brief Emulated eeprom size code 1000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1000 (0x0040)
/* @brief Emulated eeprom size code 1001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1001 (0x0020)
/* @brief Emulated eeprom size code 1010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1010 (0xFFFF)
/* @brief Emulated eeprom size code 1011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1011 (0xFFFF)
/* @brief Emulated eeprom size code 1100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1100 (0xFFFF)
/* @brief Emulated eeprom size code 1101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1101 (0xFFFF)
/* @brief Emulated eeprom size code 1110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1110 (0xFFFF)
/* @brief Emulated eeprom size code 1111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
#define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1111 (0x0000)

/* GENFSK module features */

/* No feature definitions */

/* GPIO module features */

/* @brief Has fast (single cycle) access capability via a dedicated memory region. */
#define FSL_FEATURE_GPIO_HAS_FAST_GPIO (1)
/* @brief Has port input disable register (PIDR). */
#define FSL_FEATURE_GPIO_HAS_INPUT_DISABLE (0)
/* @brief Has dedicated interrupt vector. */
#define FSL_FEATURE_GPIO_HAS_PORT_INTERRUPT_VECTOR (1)

/* I2C module features */

/* @brief Has System Management Bus support (registers SMB, A2, SLTL and SLTH). */
#define FSL_FEATURE_I2C_HAS_SMBUS (1)
/* @brief Maximum supported baud rate in kilobit per second. */
#define FSL_FEATURE_I2C_MAX_BAUD_KBPS (400)
/* @brief Is affected by errata with ID 6070 (repeat start cannot be generated if the F[MULT] bit field is set to a non-zero value). */
#define FSL_FEATURE_I2C_HAS_ERRATA_6070 (0)
/* @brief Has DMA support (register bit C1[DMAEN]). */
#define FSL_FEATURE_I2C_HAS_DMA_SUPPORT (1)
/* @brief Has I2C bus start and stop detection (register bits FLT[SSIE], FLT[STARTF] and FLT[STOPF]). */
#define FSL_FEATURE_I2C_HAS_START_STOP_DETECT (1)
/* @brief Has I2C bus stop detection (register bits FLT[STOPIE] and FLT[STOPF]). */
#define FSL_FEATURE_I2C_HAS_STOP_DETECT (0)
/* @brief Has I2C bus stop hold off (register bit FLT[SHEN]). */
#define FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF (1)
/* @brief Maximum width of the glitch filter in number of bus clocks. */
#define FSL_FEATURE_I2C_MAX_GLITCH_FILTER_WIDTH (15)
/* @brief Has control of the drive capability of the I2C pins. */
#define FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION (1)
/* @brief Has double buffering support (register S2). */
#define FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING (1)
/* @brief Has double buffer enable. */
#define FSL_FEATURE_I2C_HAS_DOUBLE_BUFFER_ENABLE (1)

/* LLWU module features */

/* @brief Maximum number of pins (maximal index plus one) connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN (16)
/* @brief Has pins 8-15 connected to LLWU device. */
#define FSL_FEATURE_LLWU_EXTERNAL_PIN_GROUP2 (1)
/* @brief Maximum number of internal modules connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE (8)
/* @brief Number of digital filters. */
#define FSL_FEATURE_LLWU_HAS_PIN_FILTER (2)
/* @brief Has MF register. */
#define FSL_FEATURE_LLWU_HAS_MF (0)
/* @brief Has PF register. */
#define FSL_FEATURE_LLWU_HAS_PF (0)
/* @brief Has possibility to enable reset in low leakage power mode and enable digital filter for RESET pin (register LLWU_RST). */
#define FSL_FEATURE_LLWU_HAS_RESET_ENABLE (0)
/* @brief Has no internal module wakeup flag register. */
#define FSL_FEATURE_LLWU_HAS_NO_INTERNAL_MODULE_WAKEUP_FLAG_REG (0)
/* @brief Has external pin 0 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN0 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN0_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN0_GPIO_PIN (16)
/* @brief Has external pin 1 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN1 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN1_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN1_GPIO_PIN (17)
/* @brief Has external pin 2 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN2 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN2_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN2_GPIO_PIN (18)
/* @brief Has external pin 3 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN3 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN3_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN3_GPIO_PIN (19)
/* @brief Has external pin 4 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN4 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN4_GPIO_IDX (GPIOA_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN4_GPIO_PIN (16)
/* @brief Has external pin 5 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN5 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN5_GPIO_IDX (GPIOA_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN5_GPIO_PIN (17)
/* @brief Has external pin 6 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN6 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN6_GPIO_IDX (GPIOA_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN6_GPIO_PIN (18)
/* @brief Has external pin 7 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN7 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN7_GPIO_IDX (GPIOA_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN7_GPIO_PIN (19)
/* @brief Has external pin 8 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN8 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN8_GPIO_IDX (GPIOB_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN8_GPIO_PIN (0)
/* @brief Has external pin 9 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN9 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN9_GPIO_IDX (GPIOB_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN9_GPIO_PIN (2)
/* @brief Has external pin 10 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN10 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN10_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN10_GPIO_PIN (2)
/* @brief Has external pin 11 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN11 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN11_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN11_GPIO_PIN (3)
/* @brief Has external pin 12 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN12 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN12_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN12_GPIO_PIN (4)
/* @brief Has external pin 13 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN13 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN13_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN13_GPIO_PIN (5)
/* @brief Has external pin 14 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN14 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN14_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN14_GPIO_PIN (6)
/* @brief Has external pin 15 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN15 (1)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN15_GPIO_IDX (GPIOC_IDX)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN15_GPIO_PIN (7)
/* @brief Has external pin 16 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN16 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN16_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN16_GPIO_PIN (0)
/* @brief Has external pin 17 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN17 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN17_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN17_GPIO_PIN (0)
/* @brief Has external pin 18 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN18 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN18_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN18_GPIO_PIN (0)
/* @brief Has external pin 19 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN19 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN19_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN19_GPIO_PIN (0)
/* @brief Has external pin 20 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN20 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN20_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN20_GPIO_PIN (0)
/* @brief Has external pin 21 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN21 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN21_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN21_GPIO_PIN (0)
/* @brief Has external pin 22 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN22 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN22_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN22_GPIO_PIN (0)
/* @brief Has external pin 23 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN23 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN23_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN23_GPIO_PIN (0)
/* @brief Has external pin 24 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN24 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN24_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN24_GPIO_PIN (0)
/* @brief Has external pin 25 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN25 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN25_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN25_GPIO_PIN (0)
/* @brief Has external pin 26 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN26 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN26_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN26_GPIO_PIN (0)
/* @brief Has external pin 27 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN27 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN27_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN27_GPIO_PIN (0)
/* @brief Has external pin 28 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN28 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN28_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN28_GPIO_PIN (0)
/* @brief Has external pin 29 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN29 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN29_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN29_GPIO_PIN (0)
/* @brief Has external pin 30 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN30 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN30_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN30_GPIO_PIN (0)
/* @brief Has external pin 31 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN31 (0)
/* @brief Index of port of external pin. */
#define FSL_FEATURE_LLWU_PIN31_GPIO_IDX (0)
/* @brief Number of external pin port on specified port. */
#define FSL_FEATURE_LLWU_PIN31_GPIO_PIN (0)
/* @brief Has internal module 0 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0 (1)
/* @brief Has internal module 1 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE1 (1)
/* @brief Has internal module 2 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE2 (1)
/* @brief Has internal module 3 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE3 (1)
/* @brief Has internal module 4 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE4 (0)
/* @brief Has internal module 5 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE5 (1)
/* @brief Has internal module 6 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE6 (0)
/* @brief Has internal module 7 connected to LLWU device. */
#define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE7 (1)
/* @brief Has Version ID Register (LLWU_VERID). */
#define FSL_FEATURE_LLWU_HAS_VERID (0)
/* @brief Has Parameter Register (LLWU_PARAM). */
#define FSL_FEATURE_LLWU_HAS_PARAM (0)
/* @brief Width of registers of the LLWU. */
#define FSL_FEATURE_LLWU_REG_BITWIDTH (8)
/* @brief Has DMA Enable register (LLWU_DE). */
#define FSL_FEATURE_LLWU_HAS_DMA_ENABLE_REG (0)

/* LPTMR module features */

/* @brief Has shared interrupt handler with another LPTMR module. */
#define FSL_FEATURE_LPTMR_HAS_SHARED_IRQ_HANDLER (0)
/* @brief Whether LPTMR counter is 32 bits width. */
#define FSL_FEATURE_LPTMR_CNR_WIDTH_IS_32B (0)
/* @brief Has timer DMA request enable (register bit CSR[TDRE]). */
#define FSL_FEATURE_LPTMR_HAS_CSR_TDRE (0)

/* LPUART module features */

/* @brief LPUART0 and LPUART1 has shared interrupt vector. */
#define FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1 (1)
/* @brief Has receive FIFO overflow detection (bit field CFIFO[RXOFE]). */
#define FSL_FEATURE_LPUART_HAS_IRQ_EXTENDED_FUNCTIONS (0)
/* @brief Has low power features (can be enabled in wait mode via register bit C1[DOZEEN] or CTRL[DOZEEN] if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_LOW_POWER_UART_SUPPORT (1)
/* @brief Has extended data register ED (or extra flags in the DATA register if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_EXTENDED_DATA_REGISTER_FLAGS (1)
/* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
#define FSL_FEATURE_LPUART_HAS_FIFO (1)
/* @brief Has 32-bit register MODIR */
#define FSL_FEATURE_LPUART_HAS_MODIR (1)
/* @brief Hardware flow control (RTS, CTS) is supported. */
#define FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT (1)
/* @brief Infrared (modulation) is supported. */
#define FSL_FEATURE_LPUART_HAS_IR_SUPPORT (1)
/* @brief 2 bits long stop bit is available. */
#define FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT (1)
/* @brief If 10-bit mode is supported. */
#define FSL_FEATURE_LPUART_HAS_10BIT_DATA_SUPPORT (1)
/* @brief If 7-bit mode is supported. */
#define FSL_FEATURE_LPUART_HAS_7BIT_DATA_SUPPORT (0)
/* @brief Baud rate fine adjustment is available. */
#define FSL_FEATURE_LPUART_HAS_BAUD_RATE_FINE_ADJUST_SUPPORT (0)
/* @brief Baud rate oversampling is available (has bit fields C4[OSR], C5[BOTHEDGE], C5[RESYNCDIS] or BAUD[OSR], BAUD[BOTHEDGE], BAUD[RESYNCDIS] if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_BAUD_RATE_OVER_SAMPLING_SUPPORT (1)
/* @brief Baud rate oversampling is available. */
#define FSL_FEATURE_LPUART_HAS_RX_RESYNC_SUPPORT (1)
/* @brief Baud rate oversampling is available. */
#define FSL_FEATURE_LPUART_HAS_BOTH_EDGE_SAMPLING_SUPPORT (1)
/* @brief Peripheral type. */
#define FSL_FEATURE_LPUART_IS_SCI (1)
/* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
#define FSL_FEATURE_LPUART_FIFO_SIZEn(x) (8)
/* @brief Maximal data width without parity bit. */
#define FSL_FEATURE_LPUART_MAX_DATA_WIDTH_WITH_NO_PARITY (10)
/* @brief Maximal data width with parity bit. */
#define FSL_FEATURE_LPUART_MAX_DATA_WIDTH_WITH_PARITY (9)
/* @brief Supports two match addresses to filter incoming frames. */
#define FSL_FEATURE_LPUART_HAS_ADDRESS_MATCHING (1)
/* @brief Has transmitter/receiver DMA enable bits C5[TDMAE]/C5[RDMAE] (or BAUD[TDMAE]/BAUD[RDMAE] if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_DMA_ENABLE (1)
/* @brief Has transmitter/receiver DMA select bits C4[TDMAS]/C4[RDMAS], resp. C5[TDMAS]/C5[RDMAS] if IS_SCI = 0. */
#define FSL_FEATURE_LPUART_HAS_DMA_SELECT (0)
/* @brief Data character bit order selection is supported (bit field S2[MSBF] or STAT[MSBF] if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_BIT_ORDER_SELECT (1)
/* @brief Has smart card (ISO7816 protocol) support and no improved smart card support. */
#define FSL_FEATURE_LPUART_HAS_SMART_CARD_SUPPORT (0)
/* @brief Has improved smart card (ISO7816 protocol) support. */
#define FSL_FEATURE_LPUART_HAS_IMPROVED_SMART_CARD_SUPPORT (0)
/* @brief Has local operation network (CEA709.1-B protocol) support. */
#define FSL_FEATURE_LPUART_HAS_LOCAL_OPERATION_NETWORK_SUPPORT (0)
/* @brief Has 32-bit registers (BAUD, STAT, CTRL, DATA, MATCH, MODIR) instead of 8-bit (BDH, BDL, C1, S1, D, etc.). */
#define FSL_FEATURE_LPUART_HAS_32BIT_REGISTERS (1)
/* @brief Lin break detect available (has bit BAUD[LBKDIE]). */
#define FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT (1)
/* @brief UART stops in Wait mode available (has bit C1[UARTSWAI]). */
#define FSL_FEATURE_LPUART_HAS_WAIT_MODE_OPERATION (0)
/* @brief Has separate DMA RX and TX requests. */
#define FSL_FEATURE_LPUART_HAS_SEPARATE_DMA_RX_TX_REQn(x) (0)
/* @brief Has separate RX and TX interrupts. */
#define FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ (0)
/* @brief Has LPAURT_PARAM. */
#define FSL_FEATURE_LPUART_HAS_PARAM (0)
/* @brief Has LPUART_VERID. */
#define FSL_FEATURE_LPUART_HAS_VERID (0)
/* @brief Has LPUART_GLOBAL. */
#define FSL_FEATURE_LPUART_HAS_GLOBAL (0)
/* @brief Has LPUART_PINCFG. */
#define FSL_FEATURE_LPUART_HAS_PINCFG (0)

/* LTC module features */

/* @brief LTC module supports DES algorithm. */
#define FSL_FEATURE_LTC_HAS_DES (0)
/* @brief LTC module supports PKHA algorithm. */
#define FSL_FEATURE_LTC_HAS_PKHA (0)
/* @brief LTC module supports SHA algorithm. */
#define FSL_FEATURE_LTC_HAS_SHA (0)
/* @brief LTC module supports AES GCM mode. */
#define FSL_FEATURE_LTC_HAS_GCM (0)
/* @brief LTC module supports DPAMS registers. */
#define FSL_FEATURE_LTC_HAS_DPAMS (0)
/* @brief LTC module supports AES with 24 bytes key. */
#define FSL_FEATURE_LTC_HAS_AES192 (0)
/* @brief LTC module supports AES with 32 bytes key. */
#define FSL_FEATURE_LTC_HAS_AES256 (0)

/* MCG module features */

/* @brief PRDIV base value (divider of register bit field [PRDIV] zero value). */
#define FSL_FEATURE_MCG_PLL_PRDIV_BASE (0)
/* @brief Maximum PLL external reference divider value (max. value of register bit field C5[PRVDIV]). */
#define FSL_FEATURE_MCG_PLL_PRDIV_MAX (0)
/* @brief VCO divider base value (multiply factor of register bit field C6[VDIV] zero value). */
#define FSL_FEATURE_MCG_PLL_VDIV_BASE (0)
/* @brief PLL reference clock low range. OSCCLK/PLL_R. */
#define FSL_FEATURE_MCG_PLL_REF_MIN (0)
/* @brief PLL reference clock high range. OSCCLK/PLL_R. */
#define FSL_FEATURE_MCG_PLL_REF_MAX (0)
/* @brief The PLL clock is divided by 2 before VCO divider. */
#define FSL_FEATURE_MCG_HAS_PLL_INTERNAL_DIV (0)
/* @brief FRDIV supports 1280. */
#define FSL_FEATURE_MCG_FRDIV_SUPPORT_1280 (1)
/* @brief FRDIV supports 1536. */
#define FSL_FEATURE_MCG_FRDIV_SUPPORT_1536 (1)
/* @brief MCGFFCLK divider. */
#define FSL_FEATURE_MCG_FFCLK_DIV (1)
/* @brief Is PLL clock divided by 2 before MCG PLL/FLL clock selection in the SIM module. */
#define FSL_FEATURE_MCG_HAS_PLL_EXTRA_DIV (0)
/* @brief Has 32kHz RTC external reference clock (register bits C8[LOCS1], C8[CME1], C8[LOCRE1] and RTC module are present). */
#define FSL_FEATURE_MCG_HAS_RTC_32K (1)
/* @brief Has PLL1 external reference clock (registers C10, C11, C12, S2). */
#define FSL_FEATURE_MCG_HAS_PLL1 (0)
/* @brief Has 48MHz internal oscillator. */
#define FSL_FEATURE_MCG_HAS_IRC_48M (0)
/* @brief Has OSC1 external oscillator (registers C10, C11, C12, S2). */
#define FSL_FEATURE_MCG_HAS_OSC1 (0)
/* @brief Has fast internal reference clock fine trim (register bit C2[FCFTRIM]). */
#define FSL_FEATURE_MCG_HAS_FCFTRIM (1)
/* @brief Has PLL loss of lock reset (register bit C8[LOLRE]). */
#define FSL_FEATURE_MCG_HAS_LOLRE (0)
/* @brief Has MCG OSC clock selection (register bit C7[OSCSEL]). */
#define FSL_FEATURE_MCG_USE_OSCSEL (1)
/* @brief Has PLL external reference selection (register bits C5[PLLREFSEL0] and C11[PLLREFSEL1]). */
#define FSL_FEATURE_MCG_USE_PLLREFSEL (0)
/* @brief TBD */
#define FSL_FEATURE_MCG_USE_SYSTEM_CLOCK (0)
/* @brief Has phase-locked loop (PLL) (register C5 and bits C6[VDIV], C6[PLLS], C6[LOLIE0], S[PLLST], S[LOCK0], S[LOLS0]). */
#define FSL_FEATURE_MCG_HAS_PLL (0)
/* @brief Has phase-locked loop (PLL) PRDIV (register C5[PRDIV]. */
#define FSL_FEATURE_MCG_HAS_PLL_PRDIV (0)
/* @brief Has phase-locked loop (PLL) VDIV (register C6[VDIV]. */
#define FSL_FEATURE_MCG_HAS_PLL_VDIV (0)
/* @brief PLL/OSC related register bit fields have PLL/OSC index in their name. */
#define FSL_FEATURE_MCG_HAS_PLL_OSC_INDEX (0)
/* @brief Has frequency-locked loop (FLL) (register ATCVH, ATCVL and bits C1[IREFS], C1[FRDIV]). */
#define FSL_FEATURE_MCG_HAS_FLL (1)
/* @brief Has PLL external to MCG (C9[PLL_CME], C9[PLL_LOCRE], C9[EXT_PLL_LOCS]). */
#define FSL_FEATURE_MCG_HAS_EXTERNAL_PLL (0)
/* @brief Has crystal oscillator or external reference clock low power controls (register bits C2[HGO], C2[RANGE]). */
#define FSL_FEATURE_MCG_HAS_EXT_REF_LOW_POWER_CONTROL (1)
/* @brief Has PLL/FLL selection as MCG output (register bit C6[PLLS]). */
#define FSL_FEATURE_MCG_HAS_PLL_FLL_SELECTION (0)
/* @brief Has PLL output selection (PLL0/PLL1, PLL/external PLL) (register bit C11[PLLCS]). */
#define FSL_FEATURE_MCG_HAS_PLL_OUTPUT_SELECTION (0)
/* @brief Has automatic trim machine (registers ATCVH, ATCVL and bits SC[ATMF], SC[ATMS], SC[ATME]). */
#define FSL_FEATURE_MCG_HAS_AUTO_TRIM_MACHINE (1)
/* @brief Has external clock monitor (register bit C6[CME]). */
#define FSL_FEATURE_MCG_HAS_EXTERNAL_CLOCK_MONITOR (1)
/* @brief Has low frequency internal reference clock (IRC) (registers LTRIMRNG, LFRIM, LSTRIM and bit MC[LIRC_DIV2]). */
#define FSL_FEATURE_MCG_HAS_LOW_FREQ_IRC (0)
/* @brief Has high frequency internal reference clock (IRC) (registers HCTRIM, HTTRIM, HFTRIM and bit MC[HIRCEN]). */
#define FSL_FEATURE_MCG_HAS_HIGH_FREQ_IRC (0)
/* @brief Has PEI mode or PBI mode. */
#define FSL_FEATURE_MCG_HAS_PLL_INTERNAL_MODE (0)
/* @brief Reset clock mode is BLPI. */
#define FSL_FEATURE_MCG_RESET_IS_BLPI (0)

/* interrupt module features */

/* @brief Lowest interrupt request number. */
#define FSL_FEATURE_INTERRUPT_IRQ_MIN (-14)
/* @brief Highest interrupt request number. */
#define FSL_FEATURE_INTERRUPT_IRQ_MAX (31)

/* PIT module features */

/* @brief Number of channels (related to number of registers LDVALn, CVALn, TCTRLn, TFLGn). */
#define FSL_FEATURE_PIT_TIMER_COUNT (2)
/* @brief Has lifetime timer (related to existence of registers LTMR64L and LTMR64H). */
#define FSL_FEATURE_PIT_HAS_LIFETIME_TIMER (1)
/* @brief Has chain mode (related to existence of register bit field TCTRLn[CHN]). */
#define FSL_FEATURE_PIT_HAS_CHAIN_MODE (1)
/* @brief Has shared interrupt handler (has not individual interrupt handler for each channel). */
#define FSL_FEATURE_PIT_HAS_SHARED_IRQ_HANDLER (1)
/* @brief Has timer enable control. */
#define FSL_FEATURE_PIT_HAS_MDIS (1)

/* PMC module features */

/* @brief Has Bandgap Enable In VLPx Operation support. */
#define FSL_FEATURE_PMC_HAS_BGEN (1)
/* @brief Has Bandgap Buffer Enable. */
#define FSL_FEATURE_PMC_HAS_BGBE (1)
/* @brief Has Bandgap Buffer Drive Select. */
#define FSL_FEATURE_PMC_HAS_BGBDS (0)
/* @brief Has Low-Voltage Detect Voltage Select support. */
#define FSL_FEATURE_PMC_HAS_LVDV (1)
/* @brief Has Low-Voltage Warning Voltage Select support. */
#define FSL_FEATURE_PMC_HAS_LVWV (1)
/* @brief Has LPO. */
#define FSL_FEATURE_PMC_HAS_LPO (0)
/* @brief Has VLPx option PMC_REGSC[VLPO]. */
#define FSL_FEATURE_PMC_HAS_VLPO (1)
/* @brief Has acknowledge isolation support. */
#define FSL_FEATURE_PMC_HAS_ACKISO (1)
/* @brief Has Regulator In Full Performance Mode Status Bit PMC_REGSC[REGFPM]. */
#define FSL_FEATURE_PMC_HAS_REGFPM (0)
/* @brief Has Regulator In Run Regulation Status Bit PMC_REGSC[REGONS]. */
#define FSL_FEATURE_PMC_HAS_REGONS (1)
/* @brief Has PMC_HVDSC1. */
#define FSL_FEATURE_PMC_HAS_HVDSC1 (0)
/* @brief Has PMC_PARAM. */
#define FSL_FEATURE_PMC_HAS_PARAM (0)
/* @brief Has PMC_VERID. */
#define FSL_FEATURE_PMC_HAS_VERID (0)

/* PORT module features */

/* @brief Has control lock (register bit PCR[LK]). */
#define FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK (0)
/* @brief Has open drain control (register bit PCR[ODE]). */
#define FSL_FEATURE_PORT_HAS_OPEN_DRAIN (0)
/* @brief Has digital filter (registers DFER, DFCR and DFWR). */
#define FSL_FEATURE_PORT_HAS_DIGITAL_FILTER (0)
/* @brief Has DMA request (register bit field PCR[IRQC] or ICR[IRQC] values). */
#define FSL_FEATURE_PORT_HAS_DMA_REQUEST (1)
/* @brief Has pull resistor selection available. */
#define FSL_FEATURE_PORT_HAS_PULL_SELECTION (1)
/* @brief Has pull resistor enable (register bit PCR[PE]). */
#define FSL_FEATURE_PORT_HAS_PULL_ENABLE (1)
/* @brief Has slew rate control (register bit PCR[SRE]). */
#define FSL_FEATURE_PORT_HAS_SLEW_RATE (1)
/* @brief Has passive filter (register bit field PCR[PFE]). */
#define FSL_FEATURE_PORT_HAS_PASSIVE_FILTER (1)
/* @brief Has drive strength control (register bit PCR[DSE]). */
#define FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH (1)
/* @brief Has separate drive strength register (HDRVE). */
#define FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH_REGISTER (0)
/* @brief Has glitch filter (register IOFLT). */
#define FSL_FEATURE_PORT_HAS_GLITCH_FILTER (0)
/* @brief Defines width of PCR[MUX] field. */
#define FSL_FEATURE_PORT_PCR_MUX_WIDTH (4)
/* @brief Has dedicated interrupt vector. */
#define FSL_FEATURE_PORT_HAS_INTERRUPT_VECTOR (1)
/* @brief Has independent interrupt control(register ICR). */
#define FSL_FEATURE_PORT_HAS_INDEPENDENT_INTERRUPT_CONTROL (0)
/* @brief Has multiple pin IRQ configuration (register GICLR and GICHR). */
#define FSL_FEATURE_PORT_HAS_MULTIPLE_IRQ_CONFIG (1)
/* @brief Defines whether PCR[IRQC] bit-field has flag states. */
#define FSL_FEATURE_PORT_HAS_IRQC_FLAG (1)
/* @brief Defines whether PCR[IRQC] bit-field has trigger states. */
#define FSL_FEATURE_PORT_HAS_IRQC_TRIGGER (1)

/* RCM module features */

/* @brief Has Loss-of-Lock Reset support. */
#define FSL_FEATURE_RCM_HAS_LOL (0)
/* @brief Has Loss-of-Clock Reset support. */
#define FSL_FEATURE_RCM_HAS_LOC (1)
/* @brief Has JTAG generated Reset support. */
#define FSL_FEATURE_RCM_HAS_JTAG (0)
/* @brief Has EzPort generated Reset support. */
#define FSL_FEATURE_RCM_HAS_EZPORT (0)
/* @brief Has bit-field indicating EZP_MS_B pin state during last reset. */
#define FSL_FEATURE_RCM_HAS_EZPMS (0)
/* @brief Has boot ROM configuration, MR[BOOTROM], FM[FORCEROM] */
#define FSL_FEATURE_RCM_HAS_BOOTROM (0)
/* @brief Has sticky system reset status register RCM_SSRS0 and RCM_SSRS1. */
#define FSL_FEATURE_RCM_HAS_SSRS (0)
/* @brief Has Version ID Register (RCM_VERID). */
#define FSL_FEATURE_RCM_HAS_VERID (0)
/* @brief Has Parameter Register (RCM_PARAM). */
#define FSL_FEATURE_RCM_HAS_PARAM (0)
/* @brief Has Reset Interrupt Enable Register RCM_SRIE. */
#define FSL_FEATURE_RCM_HAS_SRIE (0)
/* @brief Width of registers of the RCM. */
#define FSL_FEATURE_RCM_REG_WIDTH (8)
/* @brief Has Core 1 generated Reset support RCM_SRS[CORE1] */
#define FSL_FEATURE_RCM_HAS_CORE1 (0)
/* @brief Has MDM-AP system reset support RCM_SRS1[MDM_AP] */
#define FSL_FEATURE_RCM_HAS_MDM_AP (1)
/* @brief Has wakeup reset feature. Register bit SRS[WAKEUP]. */
#define FSL_FEATURE_RCM_HAS_WAKEUP (1)

/* RSIM module features */

/* No feature definitions */

/* RTC module features */

/* @brief Has wakeup pin. */
#define FSL_FEATURE_RTC_HAS_WAKEUP_PIN (1)
/* @brief Has wakeup pin selection (bit field CR[WPS]). */
#define FSL_FEATURE_RTC_HAS_WAKEUP_PIN_SELECTION (1)
/* @brief Has low power features (registers MER, MCLR and MCHR). */
#define FSL_FEATURE_RTC_HAS_MONOTONIC (0)
/* @brief Has read/write access control (registers WAR and RAR). */
#define FSL_FEATURE_RTC_HAS_ACCESS_CONTROL (0)
/* @brief Has security features (registers TTSR, MER, MCLR and MCHR). */
#define FSL_FEATURE_RTC_HAS_SECURITY (0)
/* @brief Has RTC_CLKIN available. */
#define FSL_FEATURE_RTC_HAS_RTC_CLKIN (0)
/* @brief Has prescaler adjust for LPO. */
#define FSL_FEATURE_RTC_HAS_LPO_ADJUST (0)
/* @brief Has Clock Pin Enable field. */
#define FSL_FEATURE_RTC_HAS_CPE (0)
/* @brief Has Timer Seconds Interrupt Configuration field. */
#define FSL_FEATURE_RTC_HAS_TSIC (0)
/* @brief Has OSC capacitor setting RTC_CR[SC2P ~ SC16P] */
#define FSL_FEATURE_RTC_HAS_OSC_SCXP (1)
/* @brief Has Tamper Interrupt Register (register TIR). */
#define FSL_FEATURE_RTC_HAS_TIR (0)
/* @brief Has Tamper Pin Interrupt Enable (bitfield TIR[TPIE]). */
#define FSL_FEATURE_RTC_HAS_TIR_TPIE (0)
/* @brief Has Security Interrupt Enable (bitfield TIR[SIE]). */
#define FSL_FEATURE_RTC_HAS_TIR_SIE (0)
/* @brief Has Loss of Clock Interrupt Enable (bitfield TIR[LCIE]). */
#define FSL_FEATURE_RTC_HAS_TIR_LCIE (0)
/* @brief Has Tamper Interrupt Detect Flag (bitfield SR[TIDF]). */
#define FSL_FEATURE_RTC_HAS_SR_TIDF (0)
/* @brief Has Tamper Detect Register (register TDR). */
#define FSL_FEATURE_RTC_HAS_TDR (0)
/* @brief Has Tamper Pin Flag (bitfield TDR[TPF]). */
#define FSL_FEATURE_RTC_HAS_TDR_TPF (0)
/* @brief Has Security Tamper Flag (bitfield TDR[STF]). */
#define FSL_FEATURE_RTC_HAS_TDR_STF (0)
/* @brief Has Loss of Clock Tamper Flag (bitfield TDR[LCTF]). */
#define FSL_FEATURE_RTC_HAS_TDR_LCTF (0)
/* @brief Has Tamper Time Seconds Register (register TTSR). */
#define FSL_FEATURE_RTC_HAS_TTSR (0)
/* @brief Has Pin Configuration Register (register PCR). */
#define FSL_FEATURE_RTC_HAS_PCR (0)

/* SMC module features */

/* @brief Has partial stop option (register bit STOPCTRL[PSTOPO]). */
#define FSL_FEATURE_SMC_HAS_PSTOPO (1)
/* @brief Has LPO power option (register bit STOPCTRL[LPOPO]). */
#define FSL_FEATURE_SMC_HAS_LPOPO (0)
/* @brief Has POR power option (register bit STOPCTRL[PORPO] or VLLSCTRL[PORPO]). */
#define FSL_FEATURE_SMC_HAS_PORPO (1)
/* @brief Has low power wakeup on interrupt (register bit PMCTRL[LPWUI]). */
#define FSL_FEATURE_SMC_HAS_LPWUI (0)
/* @brief Has LLS or VLLS mode control (register bit STOPCTRL[LLSM]). */
#define FSL_FEATURE_SMC_HAS_LLS_SUBMODE (1)
/* @brief Has VLLS mode control (register bit VLLSCTRL[VLLSM]). */
#define FSL_FEATURE_SMC_USE_VLLSCTRL_REG (0)
/* @brief Has VLLS mode control (register bit STOPCTRL[VLLSM]). */
#define FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM (0)
/* @brief Has RAM partition 2 power option (register bit STOPCTRL[RAM2PO]). */
#define FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION (1)
/* @brief Has high speed run mode (register bit PMPROT[AHSRUN]). */
#define FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE (0)
/* @brief Has low leakage stop mode (register bit PMPROT[ALLS]). */
#define FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE (1)
/* @brief Has very low leakage stop mode (register bit PMPROT[AVLLS]). */
#define FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE (1)
/* @brief Has stop submode. */
#define FSL_FEATURE_SMC_HAS_SUB_STOP_MODE (1)
/* @brief Has stop submode 0(VLLS0). */
#define FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 (1)
/* @brief Has stop submode 1(VLLS1). */
#define FSL_FEATURE_SMC_HAS_STOP_SUBMODE1 (1)
/* @brief Has stop submode 2(VLLS2). */
#define FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 (1)
/* @brief Has SMC_PARAM. */
#define FSL_FEATURE_SMC_HAS_PARAM (0)
/* @brief Has SMC_VERID. */
#define FSL_FEATURE_SMC_HAS_VERID (0)
/* @brief Has stop abort flag (register bit PMCTRL[STOPA]). */
#define FSL_FEATURE_SMC_HAS_PMCTRL_STOPA (1)
/* @brief Has tamper reset (register bit SRS[TAMPER]). */
#define FSL_FEATURE_SMC_HAS_SRS_TAMPER (0)
/* @brief Has security violation reset (register bit SRS[SECVIO]). */
#define FSL_FEATURE_SMC_HAS_SRS_SECVIO (0)

/* DSPI module features */

/* @brief Receive/transmit FIFO size in number of items. */
#define FSL_FEATURE_DSPI_FIFO_SIZEn(x) (4)
/* @brief Maximum transfer data width in bits. */
#define FSL_FEATURE_DSPI_MAX_DATA_WIDTH (16)
/* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS].) */
#define FSL_FEATURE_DSPI_MAX_CHIP_SELECT_COUNT (4)
/* @brief Number of chip select pins. */
#define FSL_FEATURE_DSPI_CHIP_SELECT_COUNT (3)
/* @brief Number of CTAR registers. */
#define FSL_FEATURE_DSPI_CTAR_COUNT (2)
/* @brief Has chip select strobe capability on the PCS5 pin. */
#define FSL_FEATURE_DSPI_HAS_CHIP_SELECT_STROBE (0)
/* @brief Has separated TXDATA and CMD FIFOs (register SREX). */
#define FSL_FEATURE_DSPI_HAS_SEPARATE_TXDATA_CMD_FIFO (0)
/* @brief Has 16-bit data transfer support. */
#define FSL_FEATURE_DSPI_16BIT_TRANSFERS (1)
/* @brief Has separate DMA RX and TX requests. */
#define FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)

/* SysTick module features */

/* @brief Systick has external reference clock. */
#define FSL_FEATURE_SYSTICK_HAS_EXT_REF (1)
/* @brief Systick external reference clock is core clock divided by this value. */
#define FSL_FEATURE_SYSTICK_EXT_REF_CORE_DIV (16)

/* TPM module features */

/* @brief Bus clock is the source clock for the module. */
#define FSL_FEATURE_TPM_BUS_CLOCK (0)
/* @brief Number of channels. */
#define FSL_FEATURE_TPM_CHANNEL_COUNTn(x) \
    (((x) == TPM0) ? (4) : \
    (((x) == TPM1) ? (2) : \
    (((x) == TPM2) ? (2) : (-1))))
/* @brief Has counter reset by the selected input capture event (register bits C0SC[ICRST], C1SC[ICRST], ...). */
#define FSL_FEATURE_TPM_HAS_COUNTER_RESET_BY_CAPTURE_EVENT (0)
/* @brief Has TPM_PARAM. */
#define FSL_FEATURE_TPM_HAS_PARAM (0)
/* @brief Has TPM_VERID. */
#define FSL_FEATURE_TPM_HAS_VERID (0)
/* @brief Has TPM_GLOBAL. */
#define FSL_FEATURE_TPM_HAS_GLOBAL (0)
/* @brief Has TPM_TRIG. */
#define FSL_FEATURE_TPM_HAS_TRIG (0)
/* @brief Has counter pause on trigger. */
#define FSL_FEATURE_TPM_HAS_PAUSE_COUNTER_ON_TRIGGER (1)
/* @brief Has external trigger selection. */
#define FSL_FEATURE_TPM_HAS_EXTERNAL_TRIGGER_SELECTION (1)
/* @brief Has TPM_COMBINE register. */
#define FSL_FEATURE_TPM_HAS_COMBINE (1)
/* @brief Whether COMBINE register has effect. */
#define FSL_FEATURE_TPM_COMBINE_HAS_EFFECTn(x) \
    (((x) == TPM0) ? (0) : \
    (((x) == TPM1) ? (1) : \
    (((x) == TPM2) ? (1) : (-1))))
/* @brief Has TPM_POL. */
#define FSL_FEATURE_TPM_HAS_POL (1)
/* @brief Has TPM_FILTER register. */
#define FSL_FEATURE_TPM_HAS_FILTER (1)
/* @brief Whether FILTER register has effect. */
#define FSL_FEATURE_TPM_FILTER_HAS_EFFECTn(x) \
    (((x) == TPM0) ? (0) : \
    (((x) == TPM1) ? (1) : \
    (((x) == TPM2) ? (1) : (-1))))
/* @brief Has TPM_QDCTRL register. */
#define FSL_FEATURE_TPM_HAS_QDCTRL (1)
/* @brief Whether QDCTRL register has effect. */
#define FSL_FEATURE_TPM_QDCTRL_HAS_EFFECTn(x) \
    (((x) == TPM0) ? (0) : \
    (((x) == TPM1) ? (1) : \
    (((x) == TPM2) ? (1) : (-1))))
/* @brief Is affected by errata with ID 050050 (Incorrect duty output when EPWM mode is set to PS=0 during write 1 to CnV register). */
#define FSL_FEATURE_TPM_HAS_ERRATA_050050 (0)

/* TRNG module features */

/* No feature definitions */

/* VREF module features */

/* @brief Has chop oscillator (bit TRM[CHOPEN]) */
#define FSL_FEATURE_VREF_HAS_CHOP_OSC (1)
/* @brief Has second order curvature compensation (bit SC[ICOMPEN]) */
#define FSL_FEATURE_VREF_HAS_COMPENSATION (1)
/* @brief If high/low buffer mode supported */
#define FSL_FEATURE_VREF_MODE_LV_TYPE (1)
/* @brief Module has also low reference (registers VREFL/VREFH) */
#define FSL_FEATURE_VREF_HAS_LOW_REFERENCE (0)
/* @brief Has VREF_TRM4. */
#define FSL_FEATURE_VREF_HAS_TRM4 (0)

/* XCVR_2P4GHZ_PHY module features */

/* No feature definitions */

/* XCVR_ANALOG module features */

/* No feature definitions */

/* XCVR_MISC module features */

/* No feature definitions */

/* XCVR_TSM module features */

/* No feature definitions */

#endif /* _MKW38A4_FEATURES_H_ */

