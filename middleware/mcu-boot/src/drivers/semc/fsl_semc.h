/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FSL_SEMC_H__
#define __FSL_SEMC_H__

#include "fsl_common.h"
#include "bootloader_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief SEMC Status Group
#define kStatusGroup_SEMC (210)

//! @brief Constructs the four character code for tag
#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

//! @brief Constructs the version number
#if !defined(MAKE_VERSION)
#define MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))
#endif

//! @brief SEMC driver config block tag
enum __semc_config_block_tags
{
    //! @brief Tag value used to validate the SEMC config block.
    kSemcConfigBlockTag = FOUR_CHAR_CODE('S', 'E', 'M', 'C'),
};

//! @brief SEMC driver version
#define SEMC_DRIVER_VERSION (MAKE_VERSION(1, 0, 0)) /*!< Version 1.0.0. */

// Allocated memory region of SEMC
#define SEMC_MEM0_BASE (0x80000000u)
#define SEMC_MEM1_BASE (0x90000000u)
#define SEMC_MEM2_BASE (0xA0000000u)
#define SEMC_MEM3_BASE (0xC0000000u)
#define SEMC_MEM_NOR_AXI_BASE SEMC_MEM1_BASE
#define SEMC_MEM_NAND_AXI_BASE (SEMC_MEM1_BASE + 0xE000000u)
#define SEMC_MEM_NAND_AXI_SIZE (0x2000000u)
#define SEMC_MEM_NAND_IPG_BASE (0U)
#define SEMC_MEM_NAND_IPG_SIZE (2U * 1024 * 1024 * 1024) // 16Gb

//! @brief SEMC IP command defintions
#define SEMC_IPCMD_NOR_AXI_READ (0x000Ou)        // AXI CMD Read
#define SEMC_IPCMD_NOR_AXI_WRITE (0x0001u)       // AXI CMD Write
#define SEMC_IPCMD_NOR_IPG_READ_ARRAY (0x0002u)  // IP CMD Read Array
#define SEMC_IPCMD_NOR_IPG_WRITE_ARRAY (0x0003u) // IP CMD Write Array
#define SEMC_IPCMD_NOR_IPG_READ_REG (0x0004u)    // IP CMD Read Reg
#define SEMC_IPCMD_NOR_IPG_WRITE_REG (0x0005u)   // IP CMD Write Reg

#define SEMC_IPCMD_NAND_AM_COL_ROW_ADDR (0x0u)   // X Byte determined by NANDCR3[OPT]
#define SEMC_IPCMD_NAND_AM_COL_ADDR_1BYTE (0x1u) // 1 Byte-CA0
#define SEMC_IPCMD_NAND_AM_COL_ADDR_2BYTE (0x2u) // 2 Byte-CA0/CA1
#define SEMC_IPCMD_NAND_AM_ROW_ADDR_1BYTE (0x3u) // 1 Byte-RA0
#define SEMC_IPCMD_NAND_AM_ROW_ADDR_2BYTE (0x4u) // 2 Byte-RA0/RA1
#define SEMC_IPCMD_NAND_AM_ROW_ADDR_3BYTE (0x5u) // 3 Byte-RA0/RA1/RA2

#define SEMC_IPCMD_NAND_CM_AXI_CMD_ADDR_CMD_READ (0x0u) // CMD(0x05) + ADDR(CA0/1)) + CMD(0xE0) + READ
#define SEMC_IPCMD_NAND_CM_AXI_CMD_ADDR_WRITE (0x1u)    // CMD(0x85) + ADDR(CA0/1)) + WRITE + CMD(0x15)
#define SEMC_IPCMD_NAND_CM_IPG_CMD_ONLY (0x2u)          // CMD Only
#define SEMC_IPCMD_NAND_CM_IPG_CMD_HOLD (0x3u)          // CMD + HOLD(tWB)
#define SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR (0x4u)
#define SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR_HOLD (0x5u) // CMD + ADDR + HOLD(tWB)
#define SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR_READ (0x6u)
#define SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR_WRITE (0x7u)
#define SEMC_IPCMD_NAND_CM_IPG_CMD_READ (0x8u)
#define SEMC_IPCMD_NAND_CM_IPG_CMD_WRITE (0x9u)
#define SEMC_IPCMD_NAND_CM_IPG_READ_ONLY (0xAu)  // Read Only
#define SEMC_IPCMD_NAND_CM_IPG_WRITE_ONLY (0xBu) // Write Only

#define SMEC_MAX_DEVICE_MEMORY_SIZE_IN_BYTES (0x80000000u)

//! @brief Error codes of SEMC driver
typedef enum _semc_status
{
    kStatus_SEMC_InvalidDeviceType = MAKE_STATUS(kStatusGroup_SEMC, 0),
    kStatus_SEMC_IpCommandExecutionError = MAKE_STATUS(kStatusGroup_SEMC, 1),
    kStatus_SEMC_AxiCommandExecutionError = MAKE_STATUS(kStatusGroup_SEMC, 2),
    kStatus_SEMC_InvalidFlashMemorySize = MAKE_STATUS(kStatusGroup_SEMC, 3),
    kStatus_SEMC_InvalidIpcmdDataSize = MAKE_STATUS(kStatusGroup_SEMC, 4),
    kStatus_SEMC_InvalidAddressPortWidth = MAKE_STATUS(kStatusGroup_SEMC, 5),
    kStatus_SEMC_InvalidDataPortWidth = MAKE_STATUS(kStatusGroup_SEMC, 6),
    kStatus_SEMC_InvalidSwPinmuxSelection = MAKE_STATUS(kStatusGroup_SEMC, 7),
    kStatus_SEMC_InvalidBurstLength = MAKE_STATUS(kStatusGroup_SEMC, 8),
    kStatus_SEMC_InvalidColumnAddressBitWidth = MAKE_STATUS(kStatusGroup_SEMC, 9),
    kStatus_SEMC_InvalidIpClockConfiguration = MAKE_STATUS(kStatusGroup_SEMC, 10),
} semc_status_t;

//! @brief SEMC Misc Property
enum __semc_misc_property
{
    kSemcMiscProperty_MinRxfifoPopDelayCycle = 3U,
    kSemcMiscProperty_MaxRxfifoPopDelayCycle = 6U,
    kSemcMiscProperty_MinIpCommandDataSizeInBytes = 1U,
    kSemcMiscProperty_MaxIpCommandDataSizeInBytes = 4U,
    kSemcMiscProperty_NOR_MaxFlashAddressPortWidth = 28U,
    kSemcMiscProperty_NOR_BaseFlashAddressPortWidth = 24U,
    kSemcMiscProperty_NOR_MaxColumnAddressBitWidth = 12U,
    kSemcMiscProperty_NOR_MinColumnAddressBitWidth = 2U,
    kSemcMiscProperty_NOR_AcParameterBitWidth4b = 4U,
    kSemcMiscProperty_NAND_MaxColumnAddressBitWidth = 16U,
    kSemcMiscProperty_NAND_MinColumnAddressBitWidth = 9U,
    kSemcMiscProperty_NAND_CommandModeBitMask = 0x000FU,
    kSemcMiscProperty_NAND_CommandModeBitShift = 0,
    kSemcMiscProperty_NAND_AddressModeBitMask = 0x00F0U,
    kSemcMiscProperty_NAND_AddressModeBitShift = 4U,
    kSemcMiscProperty_NAND_AcParameterBitWidth4b = 4U,
    kSemcMiscProperty_NAND_AcParameterBitWidth6b = 6U,
    kSemcMiscProperty_NORNAND_MinBurstLengthInBytes = 1U,
    kSemcMiscProperty_NORNAND_MaxBurstLengthInBytes = 64U,
};

//!@brief SEMC Clock Frequency
typedef enum _semc_clock_freq
{
    kSemcClkFreq_33MHz = 0x00U,
    kSemcClkFreq_40MHz = 0x01U,
    kSemcClkFreq_50MHz = 0x02U,
    kSemcClkFreq_66MHz = 0x03U,
    kSemcClkFreq_108MHz = 0x04U,
    kSemcClkFreq_133MHz = 0x05U,
    kSemcClkFreq_166MHz = 0x06U,
} semc_clk_freq_t;

#if defined(MIMXRT1062_SERIES) || defined(MIMXRT1051_SERIES) || defined(MIMXRT1052_SERIES)
// i.MX RT1050 or i.MX RT1060
#define SEMC_MAX_CLK_FREQ kSemcClkFreq_166MHz
#define SEMC_2ND_MAX_CLK_FREQ kSemcClkFreq_133MHz
#elif defined(MIMXRT1021_SERIES)
// i.MX RT1020
#define SEMC_MAX_CLK_FREQ kSemcClkFreq_133MHz
#define SEMC_2ND_MAX_CLK_FREQ kSemcClkFreq_108MHz
#endif

//!@brief SEMC Clock Type
typedef enum _semc_clock_type
{
    kSemcClkType_IpgClock = 0U, //!< IPBUS clock
    kSemcClkType_AxiClock = 1U, //!< AXI32 clock
} semc_clock_type_t;

//!@brief SEMC device memory type
typedef enum _semc_device_mem_type
{
    kSemcDeviceMemType_NOR = 0U,
    kSemcDeviceMemType_NAND = 1U,
    kSemcDeviceMemType_SDRAM = 2U,
    kSemcDeviceMemType_SRAM = 3U,
    kSemcDeviceMemType_8080 = 4U,
} semc_device_mem_type_t;

//!@brief SEMC access command type
typedef enum _semc_access_command_type
{
    kSemcAccessCommandType_IPBUSCMD = 0x00U,
    kSemcAccessCommandType_AXI32CMD = 0x01U,
} semc_access_command_type_t;

//!@brief SEMC CE# port output selection
enum _semc_ce_output_selection
{
    kSemcCeOutputSelection_MUX_CSX0 = 0U,
    kSemcCeOutputSelection_MUX_CSX1 = 1U,
    kSemcCeOutputSelection_MUX_CSX2 = 2U,
    kSemcCeOutputSelection_MUX_CSX3 = 3U,
    kSemcCeOutputSelection_MUX_A8 = 4U,
    kSemcCeOutputSelection_MUX_RDY = 5U,
};

//!@brief SEMC port polarity
typedef enum _semc_port_polarity
{
    kSemcPortPloarity_LowActive = 0U,
    kSemcPortPloarity_HighActive = 1U,
} semc_port_polarity_t;

//!@brief SEMC DQS mode
typedef enum _semc_dqs_mode
{
    kSemcDqsMode_LoopbackInternally = 0U,
    kSemcDqsMode_LoopbackFromDqsPad = 1U,
} semc_dqs_mode_t;

//!@brief SEMC NOR address mode
typedef enum _semc_nor_address_mode
{
    kSemcNorAddressMode_ADM = 0U,  /*!< Address/Data MUX mode*/
    kSemcNorAddressMode_AADM = 1U, /*!< Advanced Address/Data MUX mode*/
    kSemcNorAddressMode_ADP = 2U,  /*!< Address/Data non-MUX mode*/
} semc_nor_address_mode_t;

//!@brief SEMC NOR Memory Configuration Block
typedef struct _semc_nor_mem_config
{
    // Memory property
    uint32_t comMemBaseAddress;               //!< [0x000-0x003]
    uint32_t comMemSizeInByte;                //!< [0x004-0x007]
    uint8_t reserved0[8];                     //!< [0x008-0x00f]
    // Work mode
    uint8_t addressMode;                      //!< [0x010-0x010]
    uint8_t addressPortWidth;                 //!< [0x011-0x011]
    uint8_t dataPortWidth;                    //!< [0x012-0x012]
    uint8_t columnAddressWidth;               //!< [0x013-0x013]
    uint8_t burstLengthInBytes;               //!< [0x014-0x014]
    uint8_t reserved1[3];                     //!< [0x015-0x017]
    // Port pinmux and polarity
    uint8_t cePortOutputSelection;            //!< [0x018-0x018]
    uint8_t rdyPortPolarity;                  //!< [0x019-0x019]
    uint8_t advPortPolarity;                  //!< [0x01a-0x01a]
    uint8_t reserved2[13];                    //!< [0x01b-0x027]
    // AC Characteristics
    uint8_t ceSetupTime;                      //!< [0x028-0x028]
    uint8_t ceMinHoldTime;                    //!< [0x029-0x029]
    uint8_t ceMinIntervalTime;                //!< [0x02a-0x02a]
    uint8_t addressSetupTime;                 //!< [0x02b-0x02b]
    uint8_t addressHoldTime;                  //!< [0x02c-0x02c]
    uint8_t asyncWeLowTime;                   //!< [0x02d-0x02d]
    uint8_t asyncWeHighTime;                  //!< [0x02e-0x02e]
    uint8_t asyncOeLowTime;                   //!< [0x02f-0x02f]
    uint8_t asyncOeHighTime;                  //!< [0x030-0x030]
    uint8_t asyncTurnaroundTime;              //!< [0x031-0x031]
    uint8_t asyncAddressToDataHoldTime;       //!< [0x032-0x032]
    uint8_t syncDataSetupTime;                //!< [0x033-0x033]
    uint8_t syncDataHoldTime;                 //!< [0x034-0x034]
    uint8_t syncLatencyCount;                 //!< [0x035-0x035]
    uint8_t syncReadCycleTime;                //!< [0x036-0x036]
    uint8_t reserved3[9];                     //!< [0x037-0x03f]
} semc_nor_mem_config_t;

//!@brief SEMC NAND EDO mode
typedef enum _semc_nand_edo_mode
{
    kSemcNandEdoMode_Disabled = 0U,
    kSemcNandEdoMode_Enabled = 1U,
} semc_nand_edo_mode_t;

//!@brief SEMC NAND address option
typedef enum _semc_nand_address_option
{
    kSemcNandAddressOption_5byte_CA2RA3 = 0U, /*!< CA0+CA1+RA0+RA1+RA2 */
    kSemcNandAddressOption_4byte_CA2RA2 = 2U, /*!< CA0+CA1+RA0+RA1 */
    kSemcNandAddressOption_3byte_CA2RA1 = 4U, /*!< CA0+CA1+RA0 */
    kSemcNandAddressOption_4byte_CA1RA3 = 1U, /*!< CA0+RA0+RA1+RA2 */
    kSemcNandAddressOption_3byte_CA1RA2 = 3U, /*!< CA0+RA0+RA1 */
    kSemcNandAddressOption_2byte_CA1RA1 = 7U, /*!< CA0+RA0 */
} semc_nand_address_option_t;

//!@brief SEMC NAND column Address option
typedef enum _semc_nand_column_address_option
{
    kSemcNandColumnAddressOption_PageAreaAccess = 0U,
    kSemcNandColumnAddressOption_SpareAreaAccess = 1U,
} semc_nand_column_address_option_t;

//!@brief SEMC NAND Memory Configuration Block
typedef struct _semc_nand_mem_config
{
    // Memory property
    uint32_t axiMemBaseAddress;               //!< [0x000-0x003]
    uint32_t axiMemSizeInByte;                //!< [0x004-0x007]
    uint32_t ipgMemBaseAddress;               //!< [0x008-0x00b]
    uint32_t ipgMemSizeInByte;                //!< [0x00c-0x00f]
    // Work mode
    uint8_t edoMode;                          //!< [0x010-0x010]
    uint8_t ioPortWidth;                      //!< [0x011-0x011]
    uint8_t arrayAddressOption;               //!< [0x012-0x012]
    uint8_t columnAddressWidth;               //!< [0x013-0x013]
    uint8_t burstLengthInBytes;               //!< [0x014-0x014]
    uint8_t columnAddressOption;              //!< [0x015-0x015]
    uint8_t reserved0[10];                    //!< [0x016-0x01f]
    // Port pinmux and polarity
    uint8_t cePortOutputSelection;            //!< [0x020-0x020]
    uint8_t rdyPortPolarity;                  //!< [0x021-0x021]
    uint8_t reserved1[14];                    //!< [0x022-0x02f]
    // AC Characteristics
    uint8_t ceSetupTime;                      //!< [0x030-0x030]
    uint8_t ceMinHoldTime;                    //!< [0x031-0x031]
    uint8_t ceMinIntervalTime;                //!< [0x032-0x032]
    uint8_t weLowTime;                        //!< [0x033-0x033]
    uint8_t weHighTime;                       //!< [0x034-0x034]
    uint8_t reLowTime;                        //!< [0x035-0x035]
    uint8_t reHighTime;                       //!< [0x036-0x036]
    uint8_t weHighToReLowTime;                //!< [0x037-0x037]
    uint8_t reHighToWeLowTime;                //!< [0x038-0x038]
    uint8_t aleToDataStartTime;               //!< [0x039-0x039]
    uint8_t readyToReLowTime;                 //!< [0x03a-0x03a]
    uint8_t weHighToBusyTime;                 //!< [0x03b-0x03b]
    uint8_t asyncTurnaroundTime;              //!< [0x03c-0x03c]
    uint8_t reserved2[3];                     //!< [0x03d-0x03f]
} semc_nand_mem_config_t;

//!@brief SEMC Memory Configuration Block
typedef struct _semc_mem_config
{
    uint32_t tag;                             //!< [0x000-0x003]
    uint32_t version;                         //!< [0x004-0x007]
    uint8_t deviceMemType;                    //!< [0x008-0x008]
    uint8_t accessCommandType;                //!< [0x009-0x009]
    uint8_t reserved0[2];                     //!< [0x00a-0x00b]
    uint8_t asyncClkFreq;                     //!< [0x00c-0x00c]
    uint8_t busTimeoutCycles;                 //!< [0x00d-0x00d]
    uint8_t commandExecutionTimeoutCycles;    //!< [0x00e-0x00e]
    uint8_t readStrobeMode;                   //!< [0x00f-0x00f]
    union
    {
        semc_nor_mem_config_t norMemConfig;   //!< [0x010-0x04f]
        semc_nand_mem_config_t nandMemConfig; //!< [0x010-0x04f]
    };
} semc_mem_config_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

//!@brief Initialize SEMC module
status_t semc_init(semc_mem_config_t *config);

//!@brief Wait the SEMC module to be in IDLE state
status_t semc_wait_for_idle(void);

//!@brief Set data size of SEMC IP command
status_t semc_ipg_command_set_data_size(uint8_t sizeInBytes);

//!@brief Get data size of SEMC IP command
uint8_t semc_ipg_command_get_data_size(void);

//!@brief Write device data(max 4 bytes) via SEMC IP commmand
status_t semc_ipg_command_device_write(uint32_t slaveAddress,
                                       uint16_t commandCode,
                                       uint32_t writeData,
                                       uint8_t lengthInBytes);

//!@brief Read device data(max 4 bytes) via SEMC IP commmand
status_t semc_ipg_command_device_read(uint32_t slaveAddress,
                                      uint16_t commandCode,
                                      uint8_t *readoutData,
                                      uint8_t lengthInBytes);

//!@brief Read memory data via SEMC IPG commmand
status_t semc_ipg_memory_read(semc_mem_config_t *config, uint8_t *readoutData, uint32_t lengthInBytes);

//!@brief Write memory data via SEMC IPG commmand
status_t semc_ipg_memory_write(semc_mem_config_t *config, uint8_t *writeData, uint32_t lengthInBytes);

//!@brief Read memory data via SEMC AXI commmand
status_t semc_axi_memory_read(uint32_t axiAddress, uint8_t *readoutData, uint32_t lengthInBytes);

//!@brief Write memory data via SEMC AXI commmand
status_t semc_axi_memory_write(uint32_t axiAddress, uint8_t *writeData, uint32_t lengthInBytes);

//!@brief Check ready status of the NAND device
bool semc_is_nand_ready(void);

//!@brief Get NAND command code for SEMC IP commmand
uint16_t semc_ipg_command_get_nand_code(uint8_t userCommandCode, uint8_t addressMode, uint8_t commandMode);

//!@brief Convert NAND salve address for SEMC IP commmand
uint32_t semc_ipg_command_convert_nand_address(semc_mem_config_t *config, uint32_t slaveAddress);

//!@brief Access NAND via SEMC IP commmand
status_t semc_ipg_command_nand_access(uint32_t slaveAddress,
                                      uint16_t commandCode,
                                      uint32_t writeData,
                                      uint32_t *readoutData);

//!@brief Convert NOR salve address for SEMC IP commmand
uint32_t semc_ipg_command_convert_nor_address(semc_mem_config_t *config, uint32_t slaveAddress);

//!@brief Enable clock gate of SEMC
extern void semc_clock_gate_enable(void);

//!@brief Disable clock gate of SEMC
extern void semc_clock_gate_disable(void);

//!@brief Configure IOMUX for SEMC
extern void semc_iomux_config(semc_mem_config_t *config);

//!@brief Get SEMC Clock frequency
extern status_t semc_get_clock(semc_clock_type_t type, uint32_t *freq);

//!@brief Configure AXI Clock for SEMC
extern void semc_axi_clock_config(semc_clk_freq_t freq);

#ifdef __cplusplus
}
#endif

#endif // __FSL_SEMC_H__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
