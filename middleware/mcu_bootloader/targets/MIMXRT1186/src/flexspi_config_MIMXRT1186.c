/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "bl_flexspi.h"
#include "flexspi_nor_flash.h"
#include "fsl_clock.h"
#include "fsl_device_registers.h"
#include "fusemap.h"
#include "microseconds.h"
#include "peripherals_pinmux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FREQ_396MHz (396000000U)
#define FREQ_480MHz (480000000U)
#define FREQ_528MHz (528000000U)
#define FREQ_24MHz (24000000U)
#define FREQ_1MHz (1000000U)
#define FREQ_400MHz (400U * FREQ_1MHz)

//!@brief OTFAD related bitmask
#define IOMUXC_GPR34_OTFAD1_EN_MASK (1u << 1)
#define IOMUXC_GPR35_OTFAD2_EN_MASK (1u << 1)

#define FLEXSPI_REMAP_ADDR_UNIT_IN_BYTES (4096ul)

//!@brief Address remap related defintions
typedef struct
{
    __IO uint32_t START;
    __IO uint32_t END;
    __IO uint32_t OFFSET;
} FLEXSPI_ADDR_REMAP_Type;

enum
{
    kImageSize_SecImageOffset = 0, // Image Size = Secondary Image Offset
    kImageSize_1MB = 1,
    kImageSize_2MB = 2,
    kImageSize_3MB = 3,
    kImageSize_4MB = 4,
    kImageSiz_5MB = 5,
    kImageSiz_6MB = 6,
    kImageSiz_7MB = 7,
    kImageSiz_8MB = 8,
    kImageSiz_9MB = 9,
    kImageSiz_10MB = 10,
    kImageSiz_11MB = 11,
    kImageSiz_12MB = 12,
    kImageSize_256KB = 13,
    kImageSize_512KB = 14,
    kImageSize_768KB = 15,
};

enum
{
    kFlashSize_1KB = 1024u,
    kFlashSize_256KB = 256 * kFlashSize_1KB,
    kFlashSize_1MB = (1024 * kFlashSize_1KB),
};

enum
{
    kHoldTime_500US = 0,
    kHoldTime_1MS = 1,
    kHoldTime_3MS = 2,
    kHoldTime_10MS = 3,
};

//!@brief FLEXSPI ROOT CLOCK soruce related definitions
enum
{
    kFlexSpiClkSrc_RC48M_Div2 = 0,
    kFlexSpiClkSrc_OSC = 1,
    kFlexSpiClkSrc_RC400M = 2,
    kFlexSpiClkSrc_RC4M_16M = 3,
    kFlexSpiClkSrc_PLL_480_PFD0 = 4,
    kFlexSpiClkSrc_PLL_528 = 5,
    kFlexSpiClkSrc_PLL_528_PFD2 = 6,
    kFlexSpiClkSrc_PLL_480 = 7,
};

enum
{
    kFlexSpiClkFreqSrc_Normal = 0,
    kFlexSpiClkFreqSrc_RC400M = 1,
    kFlexSpiClkFreqSrc_PLL480M = 2,
};

enum
{
    kFlashResetLogic_Disabled = 0,
    kFlashResetLogic_ResetPin = 1,
    kFlashResetLogic_JedecHwReset = 2,
};

typedef struct _flexspi_pinmux_
{
    uint32_t muxRegister;
    uint32_t muxMode;
    uint32_t inputRegister;
    uint32_t inputDaisy;
    uint32_t configRegister;
    uint32_t configValue;
} flexspi_pinmux_t;

typedef struct _flexspi_pinmux_group_
{
    flexspi_pinmux_t A_SS0;
    flexspi_pinmux_t A_SS1;
    flexspi_pinmux_t A_SCLK;
    flexspi_pinmux_t A_SCLKB;
    flexspi_pinmux_t A_DQS;
    flexspi_pinmux_t A_DQS_DUMMY;
    flexspi_pinmux_t A_DATA[8];
    flexspi_pinmux_t B_SS0;
    flexspi_pinmux_t B_SS1;
    flexspi_pinmux_t B_SCLK;
    flexspi_pinmux_t B_SCLKB;
    flexspi_pinmux_t B_DQS;
    flexspi_pinmux_t B_DQS_DUMMY;
    flexspi_pinmux_t B_DATA[8];
} flexspi_pinmux_group_t;

enum
{
    kFlexspiGroup_FlexSpi1_Primary = 0,
    kFlexspiGroup_FlexSpi1_Secondary = 1,
    kFlexspiGroup_FlexSpi2_Primary = 2,
    kFlexspiGroup_FlexSpi2_Secondary = 3,
    kFlexspiGroup_Max,
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static bool is_flexspi_clock_enabled(uint32_t instance);

/*******************************************************************************
 * Variables
 ******************************************************************************/

typedef struct
{
    uint8_t muxPadIndex;      // Index in IOMUXC_MUX_CTL and IOMUXC_PAD_CTL , 0 means this feature is not supported
    uint8_t muxValue;         // MUX value of Pin
    uint8_t selectInputIndex; // index in IOMUXC_SELECT_INPUT, 0 means this feature is not supported
    uint8_t selectInputValue; // Selece Input value
} flexspi_pad_attribute_t;

typedef struct
{
    uint32_t IOMUXC_IDX;
    uint32_t A_DQS_DUMMY_IOMUXC_IDX;
    uint32_t B_DQS_DUMMY_IOMUXC_IDX;
    flexspi_pad_attribute_t A_SS0;
    flexspi_pad_attribute_t A_SS1;
    flexspi_pad_attribute_t A_SCLK;
    flexspi_pad_attribute_t A_SCLKB;
    flexspi_pad_attribute_t A_DQS;
    flexspi_pad_attribute_t A_DQS_DUMMY;
    flexspi_pad_attribute_t A_DATA[8];
    flexspi_pad_attribute_t B_SS0;
    flexspi_pad_attribute_t B_SS1;
    flexspi_pad_attribute_t B_SCLK;
    flexspi_pad_attribute_t B_SCLKB;
    flexspi_pad_attribute_t B_DQS;
    flexspi_pad_attribute_t B_DQS_DUMMY;
    flexspi_pad_attribute_t B_DATA[8];
    uint32_t csPadSetting;
    uint32_t sclkPadSetting;
    uint32_t dataPadSetting;
    uint32_t dqsPadSetting;
} flexspi_pad_data_t;

enum
{
    kIomuxIdx_IOMUXC,
    kIomuxIdx_IOMUXC_AON,
};

enum
{
    kFlexspiPadDataIndex_FlexSpi1_Primary = 0,
    kFlexspiPadDataIndex_FlexSpi1_Secondary = 1,
    kFlexspiPadDataIndex_FlexSpi2_Primary = 2,
    kFlexspiPadDataIndex_FlexSpi2_Secondary = 3,
    kFlexspiPadDataIndex_Max,
};

// FlexSPI NOR Pin setting list
const flexspi_pad_data_t k_flexspi_pads[kFlexspiPadDataIndex_Max] = {
    // FLEXSPI1 PRIMARY
    {
        .IOMUXC_IDX = kIomuxIdx_IOMUXC,
        .A_DQS_DUMMY_IOMUXC_IDX = kIomuxIdx_IOMUXC,
        .B_DQS_DUMMY_IOMUXC_IDX = kIomuxIdx_IOMUXC,
        .A_SS0 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_09, 7, 0, 0 },
        .A_SS1 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_01, 6, 0, 0 },
        .A_SCLK = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_08, 7, 0, 0 },
        .A_SCLKB = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_02, 6, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_SCK_FB_SELECT_INPUT, 2 },
        .A_DQS = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_07, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_DQS_FA_SELECT_INPUT, 1 },
        .A_DQS_DUMMY = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_12_DUMMY, 0, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_DQS_FA_SELECT_INPUT, 0 },
        .A_DATA =
            {
                // 0
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_10, 7, 0, 0 },
                // 1
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_11, 7, 0, 0 },
                // 2
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_12, 7, 0, 0 },
                // 3
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_13, 7, 0, 0 },
                // 4
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_03, 7, 0, 0 },
                // 5
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_04, 7, 0, 0 },
                // 6
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_05, 7, 0, 0 },
                // 7
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_06, 7, 0, 0 },
            },
        .B_SS0 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_06, 1, 0, 0 },
        .B_SS1 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_04, 1, 0, 0 },
        .B_SCLK = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_07, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_SCK_FB_SELECT_INPUT, 0 },
        .B_DQS = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_05, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_DQS_FB_SELECT_INPUT, 0 },
        .B_DQS_DUMMY = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_12_DUMMY, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_DQS_FB_SELECT_INPUT, 1 },
        .B_DATA =
            {
                // 0
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_08, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT0_SELECT_INPUT, 0 },
                // 1
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_09, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT1_SELECT_INPUT, 0 },
                // 2
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_10, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT2_SELECT_INPUT, 0 },
                // 3
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_11, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT3_SELECT_INPUT, 0 },

                // 4
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_00, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT4_SELECT_INPUT, 0 },
                // 5
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_01, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT5_SELECT_INPUT, 0 },
                // 6
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_02, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT6_SELECT_INPUT, 0 },
                // 7
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_03, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT7_SELECT_INPUT, 0 },
            },
        .csPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3),   // No-Pull
        .sclkPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3), // No-Pull
        .dataPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3), // No-Pull
        .dqsPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(2),  // Pull down
    },
    // FLEXSPI1 SECONDARY
    {
        .IOMUXC_IDX = kIomuxIdx_IOMUXC,
        .A_DQS_DUMMY_IOMUXC_IDX = kIomuxIdx_IOMUXC,
        .B_DQS_DUMMY_IOMUXC_IDX = kIomuxIdx_IOMUXC,
        .A_SS0 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_09, 7, 0, 0 }, // Primary pin group reused
        .A_SS1 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_01, 6, 0, 0 }, // Primary pin group reused
        .A_SCLK = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_08, 7, 0, 0 }, // Primary pin group reused
        .A_SCLKB = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_02, 6, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_SCK_FB_SELECT_INPUT, 2 }, // Primary pin group reused
        .A_DQS = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_07, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_DQS_FA_SELECT_INPUT, 1 }, // Primary pin group reused
        .A_DQS_DUMMY = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_12_DUMMY, 0, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_DQS_FA_SELECT_INPUT, 0 }, // Primary pin group reused
        .A_DATA = // Primary pin group reused
            {
                // 0
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_10, 7, 0, 0 },
                // 1
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_11, 7, 0, 0 },
                // 2
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_12, 7, 0, 0 },
                // 3
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_13, 7, 0, 0 },
                // 4
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_03, 7, 0, 0 },
                // 5
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_04, 7, 0, 0 },
                // 6
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_05, 7, 0, 0 },
                // 7
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B2_06, 7, 0, 0 },
            },
        .B_SS0 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_04, 7, 0, 0 },
        .B_SS1 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_02, 7, 0, 0 },
        .B_SCLK = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_05, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_SCK_FB_SELECT_INPUT, 1 },
        .B_DQS = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_03, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_DQS_FB_SELECT_INPUT, 2 },
        .B_DQS_DUMMY = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_SD_B2_12_DUMMY, 1, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_DQS_FB_SELECT_INPUT, 1 },
        .B_DATA =
            {
                // 0
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_13, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT0_SELECT_INPUT, 1 },
                // 1
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_12, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT1_SELECT_INPUT, 1 },
                // 2
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_11, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT2_SELECT_INPUT, 1 },
                // 3
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_10, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT3_SELECT_INPUT, 1 },

                // 4
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_09, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT4_SELECT_INPUT, 1 },
                // 5
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_08, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT5_SELECT_INPUT, 1 },
                // 6
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_07, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT6_SELECT_INPUT, 1 },
                // 7
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_B1_06, 7, kIOMUXC_FLEXSPI1_BUS2BIT_IPP_IND_IO_FB_BIT7_SELECT_INPUT, 1 },
            },
        .csPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3),   // No-Pull
        .sclkPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3), // No-Pull
        .dataPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3), // No-Pull
        .dqsPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(2),  // Pull down
    },
    // FLEXSPI2 PRIMARY
    {
        .IOMUXC_IDX = kIomuxIdx_IOMUXC_AON,
        .A_DQS_DUMMY_IOMUXC_IDX = kIomuxIdx_IOMUXC_AON,
        .B_DQS_DUMMY_IOMUXC_IDX = kIomuxIdx_IOMUXC_AON,
        .A_SS0 = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_22, 0, 0, 0 },
        .A_SS1 = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_20, 1, 0, 0 },
        .A_SCLK = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_23, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_SCK_FA_SELECT_INPUT, 1 },
        .A_SCLKB = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_19, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_SCK_FB_SELECT_INPUT, 1 },
        .A_DQS = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_21, 8, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_DQS_FA_SELECT_INPUT, 1 },
        .A_DQS_DUMMY = {kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_28_DUMMY, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_DQS_FA_SELECT_INPUT, 2},
        .A_DATA =
            {
                // 0
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_24, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FA_BIT0_SELECT_INPUT, 1 },
                // 1
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_25, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FA_BIT1_SELECT_INPUT, 1 },
                // 2
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_26, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FA_BIT2_SELECT_INPUT, 1 },
                // 3
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_27, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FA_BIT3_SELECT_INPUT, 1 },
                // 4
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_18, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT0_SELECT_INPUT, 2 },
                // 5
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_17, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT1_SELECT_INPUT, 2 },
                // 6
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_16, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT2_SELECT_INPUT, 2 },
                // 7
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_15, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT3_SELECT_INPUT, 2 },
            },
        .B_SS0 = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_21, 0, 0, 0 },
        // .B_SS1 not available in the same io bank
        .B_SCLK = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_19, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_SCK_FB_SELECT_INPUT, 1 },
        .B_DQS = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_20, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_DQS_FB_SELECT_INPUT, 2 },
        .B_DQS_DUMMY = {kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_28_DUMMY, 1, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_DQS_FB_SELECT_INPUT, 3},
        .B_DATA =
            {
                // 0
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_18, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT0_SELECT_INPUT, 2 },
                // 1
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_17, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT1_SELECT_INPUT, 2 },
                // 2
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_16, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT2_SELECT_INPUT, 2 },
                // 3
                { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_15, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT3_SELECT_INPUT, 2 },
            },
        .csPadSetting = IOMUXC_SW_PAD_CTL_PAD_DSE(1),
        .sclkPadSetting = IOMUXC_SW_PAD_CTL_PAD_DSE(1),
        .dataPadSetting = IOMUXC_SW_PAD_CTL_PAD_DSE(1),
        .dqsPadSetting = IOMUXC_SW_PAD_CTL_PAD_DSE(1) | IOMUXC_SW_PAD_CTL_PAD_PUE(1),  // Pull down
    },
    // FLEXSPI2 SECONDARY
    {
        .IOMUXC_IDX = kIomuxIdx_IOMUXC,
        .A_DQS_DUMMY_IOMUXC_IDX = kIomuxIdx_IOMUXC_AON,
        .B_DQS_DUMMY_IOMUXC_IDX = kIomuxIdx_IOMUXC_AON,
        .A_SS0 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_39, 3, 0, 0 },
        .A_SS1 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_26, 3, 0, 0 },
        .A_SCLK = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_41, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_SCK_FA_SELECT_INPUT, 0 },
        .A_SCLKB = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_34, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_SCK_FB_SELECT_INPUT, 0 },
        .A_DQS = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_40, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_DQS_FA_SELECT_INPUT, 0 },
        .A_DQS_DUMMY = {kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_28_DUMMY, 0, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_DQS_FA_SELECT_INPUT, 2},
        .A_DATA =
            {
                // 0
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_35, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FA_BIT0_SELECT_INPUT, 0 },
                // 1
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_36, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FA_BIT1_SELECT_INPUT, 0 },
                // 2
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_37, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FA_BIT2_SELECT_INPUT, 0 },
                // 3
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_38, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FA_BIT3_SELECT_INPUT, 0 },
                // 4
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_33, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT0_SELECT_INPUT, 1 },
                // 5
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_32, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT1_SELECT_INPUT, 1 },
                // 6
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_31, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT2_SELECT_INPUT, 1 },
                // 7
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_30, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT3_SELECT_INPUT, 1 },
            },
        .B_SS0 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_28, 3, 0, 0 },
        .B_SS1 = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_27, 3, 0, 0},
        .B_SCLK = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_34, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_SCK_FB_SELECT_INPUT, 0 },
        .B_DQS = { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_29, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_DQS_FB_SELECT_INPUT, 1 },
        .B_DQS_DUMMY = { kIOMUXC_AON_SW_MUX_CTL_PAD_GPIO_AON_28_DUMMY, 1, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_DQS_FB_SELECT_INPUT, 3 },
        .B_DATA =
            {
                // 0
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_33, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT0_SELECT_INPUT, 1 },
                // 1
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_32, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT1_SELECT_INPUT, 1 },
                // 2
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_31, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT2_SELECT_INPUT, 1 },
                // 3
                { kIOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_B1_30, 3, kIOMUXC_FLEXSPI2_BUS2BIT_IPP_IND_IO_FB_BIT3_SELECT_INPUT, 1 },
            },
        .csPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3),   // No-pull
        .sclkPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3), // No-pull
        .dataPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(3), // No-pull
        .dqsPadSetting = IOMUXC_SW_PAD_CTL_PAD_PULL(2),  // Pull down
    },
};

typedef enum
{
    kFlexSpi1ClkSrc_RC24M = 0,
    kFlexSpi1ClkSrc_RC400M = 1,
    kFlexSpi1ClkSrc_PLL_480_PFD0 = 2,
    kFlexSpi1ClkSrc_PLL_528_PFD0 = 3,

    kFlexSpi2ClkSrc_RC24M = 0,
    kFlexSpi2ClkSrc_RC400M = 1,
    kFlexSpi2ClkSrc_PLL_480_PFD2 = 2,
    kFlexSpi2ClkSrc_PLL_528_PFD1 = 3,

    kFlexSpiClkSrc_MuxOpt_Num = 4,
} flexspi_clk_src_mux_t;

typedef enum
{
    kFlexSpiClkSrc_Type_Osc,
    kFlexSpiClkSrc_Type_PllDiv, // Including the PLL itself
    kFlexSpiClkSrc_Type_PllPfd,
} flexspi_clk_src_type_t;

typedef struct
{
    flexspi_clk_src_mux_t src_mux;
    flexspi_clk_src_type_t src_typ;
    union
    {
        struct
        {
            clock_pll_t pll_idx; // Available for kFlexSpiClkSrc_Type_PllPfd, kFlexSpiClkSrc_Type_PllDiv
            union
            {
                clock_pfd_t pfd_idx; // Available for kFlexSpiClkSrc_Type_PllPfd
                uint16_t pll_div;    // Available for kFlexSpiClkSrc_Type_PllDiv
            };
        };
        uint32_t osc_frq; // Available for kFlexSpiClkSrc_Type_Osc
    };
    struct
    {
        uint8_t pfd_frc; // pfd frac
        uint8_t clk_div; // clock root div
    } clk_cfg[kFlexSpiSerialClk_200MHz];
} flexspi_clk_freq_list_t;

enum
{
    kFlexSpi_1st_Inst = kFlexspiInstance_1,
    kFlexSpi_Last_Inst = kFlexspiInstance_2,
    kFlexSpi_Inst_Num = kFlexSpi_Last_Inst - kFlexSpi_1st_Inst + 1,
};

static const flexspi_clk_freq_list_t k_flexspi_clk_list_sdr[kFlexSpi_Inst_Num][kFlexSpiClkSrc_MuxOpt_Num] = {
    /* FlexSPI1 */
    {
        /* OSC_RC_24M */
        {
            .src_mux = kFlexSpi1ClkSrc_RC24M,
            .src_typ = kFlexSpiClkSrc_Type_Osc,
            .osc_frq = FREQ_24MHz,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
            },
        },
        /* RC400M */
        {
            .src_mux = kFlexSpi1ClkSrc_RC400M,
            .src_typ = kFlexSpiClkSrc_Type_Osc,
            .osc_frq = FREQ_400MHz,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {0, 13}, /* 400MHz / 13 30.77MHz */ {0, 8}, /* 400MHz / 8 50MHz  */ {0, 6}, /* 400MHz / 6 66.67MHz */
                {0, 5 }, /* 400MHz / 5  80MHz    */ {0, 4}, /* 400MHz / 4 100MHz */ {0, 4}, /* 400MHz / 4 100MHz   */
                {0, 3 }, /* 400MHz / 3  133MHz   */ {0, 3}, /* 400MHz / 3 133MHz */ {0, 2}, /* 400MHz / 2 200MHz   */
            },
        },
        /* PLL480_PFD0 */
        {
            .src_mux = kFlexSpi1ClkSrc_PLL_480_PFD0,
            .src_typ = kFlexSpiClkSrc_Type_PllPfd,
            .pll_idx = kCLOCK_PllSys3,
            .pfd_idx = kCLOCK_Pfd0,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {16, 18}, /* 480MHz * 18 / 16 / 18 30.00MHz  */ {29, 6 }, /* 480MHz * 18 / 29 / 6  49.66MHz  */ {16, 9 }, /* 480MHz * 18 / 16 / 9  60.00MHz  */
                {18, 6 }, /* 480MHz * 18 / 18 / 6  80.00MHz  */ {29, 3 }, /* 480MHz * 18 / 29 / 3  99.31MHz  */ {18, 4 }, /* 480MHz * 18 / 18 / 4  120.00MHz */
                {13, 5 }, /* 480MHz * 18 / 13 / 5  132.92MHz */ {13, 4 }, /* 480MHz * 18 / 13 / 4  166.15MHz */ {22, 2 }, /* 480MHz * 18 / 22 / 2  196.36MHz */
            },
        },
        /* PLL528_PFD0 */
        {
            .src_mux = kFlexSpi1ClkSrc_PLL_528_PFD0,
            .src_typ = kFlexSpiClkSrc_Type_PllPfd,
            .pll_idx = kCLOCK_PllSys2,
            .pfd_idx = kCLOCK_Pfd0,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {15, 21}, /* 528MHz * 18 / 15 / 21 30.17MHz  */ {19, 10}, /* 528MHz * 18 / 19 / 10 50.02MHz  */ {16, 10}, /* 528MHz * 18 / 16 / 10 59.40MHz  */
                {17, 7 }, /* 528MHz * 18 / 17 / 7  79.87MHz  */ {19, 5 }, /* 528MHz * 18 / 19 / 5  100.04MHz */ {16, 5 }, /* 528MHz * 18 / 16 / 5  118.80MHz */
                {18, 4 }, /* 528MHz * 18 / 18 / 4  132.00MHz */ {19, 3 }, /* 528MHz * 18 / 19 / 3  166.74MHz */ {16, 3 }, /* 528MHz * 18 / 16 / 3  198.00MHz */
            },
        },
    },
    /* FlexSPI2 */
    {
        /* OSC_RC_24M */
        {
            .src_mux = kFlexSpi2ClkSrc_RC24M,
            .src_typ = kFlexSpiClkSrc_Type_Osc,
            .osc_frq = FREQ_24MHz,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
            },
        },
        /* RC400M */
        {
            .src_mux = kFlexSpi2ClkSrc_RC400M,
            .src_typ = kFlexSpiClkSrc_Type_Osc,
            .osc_frq = FREQ_400MHz,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {0, 13}, /* 400MHz / 13 30.77MHz*/ {0, 8}, /* 400MHz / 8 50MHz  */ {0, 6}, /* 400MHz / 6 66.67MHz */
                {0, 5 }, /* 400MHz / 5  80MHz   */ {0, 4}, /* 400MHz / 4 100MHz */ {0, 4}, /* 400MHz / 4 100MHz   */
                {0, 3 }, /* 400MHz / 3  133MHz  */ {0, 3}, /* 400MHz / 3 133MHz */ {0, 2}, /* 400MHz / 2 200MHz   */
            },
        },
        /* PLL480_PFD2 */
        {
            .src_mux = kFlexSpi2ClkSrc_PLL_480_PFD2,
            .src_typ = kFlexSpiClkSrc_Type_PllPfd,
            .pll_idx = kCLOCK_PllSys3,
            .pfd_idx = kCLOCK_Pfd2,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {16, 18}, /* 480MHz * 18 / 16 / 18 30.00MHz  */ {29, 6 }, /* 480MHz * 18 / 29 / 6  49.66MHz  */ {16, 9 }, /* 480MHz * 18 / 16 / 9  60.00MHz  */
                {18, 6 }, /* 480MHz * 18 / 18 / 6  80.00MHz  */ {29, 3 }, /* 480MHz * 18 / 29 / 3  99.31MHz  */ {18, 4 }, /* 480MHz * 18 / 18 / 4  120.00MHz */
                {13, 5 }, /* 480MHz * 18 / 13 / 5  132.92MHz */ {13, 4 }, /* 480MHz * 18 / 13 / 4  166.15MHz */ {22, 2 }, /* 480MHz * 18 / 22 / 2  196.36MHz */
            },
        },
        /* PLL528_PFD1 */
        {
            .src_mux = kFlexSpi2ClkSrc_PLL_528_PFD1,
            .src_typ = kFlexSpiClkSrc_Type_PllPfd,
            .pll_idx = kCLOCK_PllSys2,
            .pfd_idx = kCLOCK_Pfd1,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {15, 21}, /* 528MHz * 18 / 15 / 21 30.17MHz  */ {19, 10}, /* 528MHz * 18 / 19 / 10 50.02MHz  */ {16, 10}, /* 528MHz * 18 / 16 / 10 59.40MHz   */
                {17, 7 }, /* 528MHz * 18 / 17 / 7  79.87MHz  */ {19, 5 }, /* 528MHz * 18 / 19 / 5  100.04MHz */ {16, 5 },  /* 528MHz * 18 / 16 / 5  118.80MHz */
                {18, 4 }, /* 528MHz * 18 / 18 / 4  132.00MHz */ {19, 3 }, /* 528MHz * 18 / 19 / 3  166.74MHz */ {16, 3 },  /* 528MHz * 18 / 16 / 3  198.00MHz */
            },
        },
    },
};

// Formula: Clock(src) / div / 2
static const flexspi_clk_freq_list_t k_flexspi_clk_list_ddr[kFlexSpi_Inst_Num][kFlexSpiClkSrc_MuxOpt_Num] = {
    /* FlexSPI1 */
    {
        /* OSC_RC_24M */
        {
            .src_mux = kFlexSpi1ClkSrc_RC24M,
            .src_typ = kFlexSpiClkSrc_Type_Osc,
            .osc_frq = FREQ_24MHz,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
                {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */ {0, 1}, /* 24MHz */
            },
        },
        /* RC400M */
        {
            .src_mux = kFlexSpi1ClkSrc_RC400M,
            .src_typ = kFlexSpiClkSrc_Type_Osc,
            .osc_frq = FREQ_400MHz,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {0, 6}, /* 400MHz / 6 33.33MHz*2 */ {0, 4}, /* 400MHz / 4 50MHz*2  */ {0, 3}, /* 400MHz / 3 66.67MHz*2 */
                {0, 3}, /* 400MHz / 3 66.67MHz*2 */ {0, 2}, /* 400MHz / 2 100MHz*2 */ {0, 2}, /* 400MHz / 2 100MHz*2   */
                {0, 2}, /* 400MHz / 2 100MHz*2   */ {0, 2}, /* 400MHz / 2 100MHz*2 */ {0, 1}, /* 400MHz / 1 200MHz*2   */
            },
        },
        /* PLL480_PFD0 */
        {
            .src_mux = kFlexSpi1ClkSrc_PLL_480_PFD0,
            .src_typ = kFlexSpiClkSrc_Type_PllPfd,
            .pll_idx = kCLOCK_PllSys3,
            .pfd_idx = kCLOCK_Pfd0,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {16, 9}, /* 480MHz * 18 / 16 / 9 30.00MHz*2  */ {29, 3}, /* 480MHz * 18 / 29 / 3 49.66MHz*2  */ {18, 4}, /* 480MHz * 18 / 18 / 4 60.00MHz*2  */
                {18, 3}, /* 480MHz * 18 / 18 / 3 80.00MHz*2  */ {22, 2}, /* 480MHz * 18 / 22 / 2 98.18MHz*2  */ {18, 2}, /* 480MHz * 18 / 18 / 2 120.00MHz*2 */
                {16, 2}, /* 480MHz * 18 / 16 / 2 135.00MHz*2 */ {13, 2}, /* 480MHz * 18 / 13 / 2 166.15MHz*2 */ {22, 1}, /* 480MHz * 18 / 22 / 1 196.36MHz*2 */
            },
        },
        /* PLL_528_PFD0 */
        {
            .src_mux = kFlexSpi1ClkSrc_PLL_528_PFD0,
            .src_typ = kFlexSpiClkSrc_Type_PllPfd,
            .pll_idx = kCLOCK_PllSys2,
            .pfd_idx = kCLOCK_Pfd0,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {16, 10}, /* 528MHz * 18 / 16 / 10 29.70MHz*2  */ {19, 5}, /* 528MHz * 18 / 19 / 5  50.02MHz*2  */ {16, 5}, /* 528MHz * 18 / 16 / 5  59.40MHz*2  */
                {15, 4 }, /* 528MHz * 18 / 15 / 4  79.20MHz*2  */ {16, 3}, /* 528MHz * 18 / 16 / 3  99.00MHz*2  */ {20, 2}, /* 528MHz * 18 / 20 / 2  118.80MHz*2 */
                {18, 2 }, /* 528MHz * 18 / 18 / 2  132.00MHz*2 */ {29, 1}, /* 528MHz * 18 / 29 / 1  163.86MHz*2 */ {24, 1}, /* 528MHz * 18 / 24 / 1  198.00MHz*2 */
            },
        },
    },
    /* FlexSPI2 */
    {
        /* OSC_RC_24M */
        {
            .src_mux = kFlexSpi2ClkSrc_RC24M,
            .src_typ = kFlexSpiClkSrc_Type_Osc,
            .osc_frq = FREQ_24MHz,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {0, 1}, /* 12MHz*2 */ {0, 1}, /* 12MHz*2 */ {0, 1}, /* 12MHz*2 */
                {0, 1}, /* 12MHz*2 */ {0, 1}, /* 12MHz*2 */ {0, 1}, /* 12MHz*2 */
                {0, 1}, /* 12MHz*2 */ {0, 1}, /* 12MHz*2 */ {0, 1}, /* 12MHz*2 */
            },
        },
        /* RC400M */
        {
            .src_mux = kFlexSpi2ClkSrc_RC400M,
            .src_typ = kFlexSpiClkSrc_Type_Osc,
            .osc_frq = FREQ_400MHz,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
               {0, 6}, /* 400MHz / 6 33.33MHz*2 */ {0, 4}, /* 400MHz / 4 50MHz*2  */ {0, 3}, /* 400MHz / 3 66.67MHz*2 */
               {0, 3}, /* 400MHz / 3 66.67MHz*2 */ {0, 2}, /* 400MHz / 2 100MHz*2 */ {0, 2}, /* 400MHz / 2 100MHz*2   */
               {0, 2}, /* 400MHz / 2 100MHz*2   */ {0, 2}, /* 400MHz / 2 100MHz*2 */ {0, 1}, /* 400MHz / 1 200MHz*2   */
            },
        },
        /* PLL480_PFD2 */
        {
            .src_mux = kFlexSpi2ClkSrc_PLL_480_PFD2,
            .src_typ = kFlexSpiClkSrc_Type_PllPfd,
            .pll_idx = kCLOCK_PllSys3,
            .pfd_idx = kCLOCK_Pfd2,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {16, 9}, /* 480MHz * 18 / 16 / 9 30.00MHz*2  */ {29, 3}, /* 480MHz * 18 / 29 / 3 49.66MHz*2  */ {18, 4}, /* 480MHz * 18 / 18 / 4 60.00MHz*2  */
                {18, 3}, /* 480MHz * 18 / 18 / 3 80.00MHz*2  */ {22, 2}, /* 480MHz * 18 / 22 / 2 98.18MHz*2  */ {18, 2}, /* 480MHz * 18 / 18 / 2 120.00MHz*2 */
                {16, 2}, /* 480MHz * 18 / 16 / 2 135.00MHz*2 */ {13, 2}, /* 480MHz * 18 / 13 / 2 166.15MHz*2 */ {22, 1}, /* 480MHz * 18 / 22 / 1 196.36MHz*2 */
            },
        },
        /* PLL528_PFD1 */
        {
            .src_mux = kFlexSpi2ClkSrc_PLL_528_PFD1,
            .src_typ = kFlexSpiClkSrc_Type_PllPfd,
            .pll_idx = kCLOCK_PllSys2,
            .pfd_idx = kCLOCK_Pfd1,
            .clk_cfg = {
                /* *18/frac(13~35) | /div(1~256) */
                {16, 10}, /* 528MHz * 18 / 16 / 10 29.70MHz*2  */ {19, 5}, /* 528MHz * 18 / 19 / 5  50.02MHz*2  */ {16, 5}, /* 528MHz * 18 / 16 / 5  59.40MHz*2  */
                {15, 4 }, /* 528MHz * 18 / 15 / 4  79.20MHz*2  */ {16, 3}, /* 528MHz * 18 / 16 / 3  99.00MHz*2  */ {20, 2}, /* 528MHz * 18 / 20 / 2  118.80MHz*2 */
                {18, 2 }, /* 528MHz * 18 / 18 / 2  132.00MHz*2 */ {29, 1}, /* 528MHz * 18 / 29 / 1  163.86MHz*2 */ {24, 1}, /* 528MHz * 18 / 24 / 1  198.00MHz*2 */
            },
        },
    },
};
FLEXSPI_Type *const g_flexSpiInstances[] = FLEXSPI_BASE_PTRS;

/*******************************************************************************
 * Codes
 ******************************************************************************/
//!@brief Get Clock for FlexSPI peripheral
status_t flexspi_get_clock(uint32_t instance, flexspi_clock_type_t type, uint32_t *freq)
{
    uint32_t clockFrequency = 0;
    status_t status = kStatus_Success;

    if ((freq == NULL) || (type > kFlexSpiClock_IpgClock))
    {
        return kStatus_InvalidArgument;
    }

    if ((instance != kFlexspiInstance_1) && (instance != kFlexspiInstance_2))
    {
        return kStatus_InvalidArgument;
    }

#ifndef BL_TARGET_FPGA
    switch (type)
    {
        case kFlexSpiClock_CoreClock:
            clockFrequency = CLOCK_GetRootClockFreq(kCLOCK_Root_M33);
            break;
        case kFlexSpiClock_AhbClock:
            clockFrequency = CLOCK_GetRootClockFreq(kCLOCK_Root_Wakeup_Axi);
            break;
        case kFlexSpiClock_SerialRootClock:
        {
            uint32_t srcRootClk;
            uint32_t flexspiClkSrc = 0;
            uint32_t flexspiClkDiv = 0;
            volatile uint32_t *clkDivRegister = NULL;
            if (instance == kFlexspiInstance_1)
            {
                clkDivRegister = &CCM->CLOCK_ROOT[kCLOCK_Root_Flexspi1].CONTROL;
            }
            else if (instance == kFlexspiInstance_2)
            {
                clkDivRegister = &CCM->CLOCK_ROOT[kCLOCK_Root_Flexspi2].CONTROL;
            }
            // FLEXPI CLK SEL
            flexspiClkSrc = (*clkDivRegister & CCM_CLOCK_ROOT_CONTROL_MUX_MASK) >> CCM_CLOCK_ROOT_CONTROL_MUX_SHIFT;
            // FLEXSPI CLK DIV
            flexspiClkDiv =
                1 + ((*clkDivRegister & CCM_CLOCK_ROOT_CONTROL_DIV_MASK) >> CCM_CLOCK_ROOT_CONTROL_DIV_SHIFT);

            switch (flexspiClkSrc)
            {
                case kFlexSpiClkSrc_RC48M_Div2:
                    srcRootClk = 24000000u;
                    break;
                case kFlexSpiClkSrc_OSC:
                    srcRootClk = 24000000u;
                    break;
                case kFlexSpiClkSrc_RC400M:
                    srcRootClk = 400000000u;
                    break;
                case kFlexSpiClkSrc_RC4M_16M:
                    srcRootClk = 16000000u;
                    break;
                case kFlexSpiClkSrc_PLL_480_PFD0:
                    srcRootClk = 664620000u;
                    break;
                case kFlexSpiClkSrc_PLL_528:
                    srcRootClk = 528000000u;
                    break;
                case kFlexSpiClkSrc_PLL_528_PFD2:
                    srcRootClk = 396000000u;
                    break;
                case kFlexSpiClkSrc_PLL_480:
                    srcRootClk = 480000000u;
                    break;
                default:
                    status = kStatus_OutOfRange;
                    break;
            }

            if (status != kStatus_Success)
            {
                break;
            }

            clockFrequency = srcRootClk / flexspiClkDiv;
        }
        break;
        default:
            status = kStatus_InvalidArgument;
            break;
    }
#else
    switch (type)
    {
        case kFlexSpiClock_CoreClock:
            clockFrequency = FPGA_FREQ_CORE;
            break;
        case kFlexSpiClock_SerialRootClock:
        case kFlexSpiClock_AhbClock:
            clockFrequency = FPGA_FREQ_BUS;
            break;
        default:
            status = kStatus_InvalidArgument;
            break;
    }
#endif // #ifndef BL_TARGET_FPGA

    if (status == kStatus_Success)
    {
        *freq = clockFrequency;
    }

    return status;
}

void flexspi_set_pinmux(const flexspi_pinmux_t *pinmux, uint32_t overridePadSetting)
{
    IOMUXC_SetPinMux(pinmux->muxRegister, pinmux->muxMode, pinmux->inputRegister, pinmux->inputDaisy,
                     pinmux->configRegister, 0);
    if (overridePadSetting)
    {
        IOMUXC_SetPinConfig(pinmux->muxRegister, pinmux->muxMode, pinmux->inputRegister, pinmux->inputDaisy,
                            pinmux->configRegister, overridePadSetting);
    }
    else
    {
        IOMUXC_SetPinConfig(pinmux->muxRegister, pinmux->muxMode, pinmux->inputRegister, pinmux->inputDaisy,
                            pinmux->configRegister, pinmux->configValue);
    }
}

//!@brief Configure IOMUX for FlexSPI Peripheral
static void flexspi_iomux_config_on_iomuxc(const flexspi_pad_data_t *padData, flexspi_mem_config_t *config)
{
    uint32_t csPadCtrlValue = config->csPadSettingOverride ? config->csPadSettingOverride : padData->csPadSetting;
    uint32_t dqsPadCtrlValue = config->dqsPadSettingOverride ? config->dqsPadSettingOverride : padData->dqsPadSetting;
    uint32_t sclkPadCtrlValue =
        config->sclkPadSettingOverride ? config->sclkPadSettingOverride : padData->sclkPadSetting;
    uint32_t dataPadCtrlValue =
        config->dataPadSettingOverride ? config->dataPadSettingOverride : padData->dataPadSetting;

    if (config->sflashA1Size || config->sflashA2Size)
    {
        // Configure Chip Select Pin
        if (config->sflashA1Size)
        {
            IOMUXC->SW_MUX_CTL_PAD[padData->A_SS0.muxPadIndex] = padData->A_SS0.muxValue;
            IOMUXC->SW_PAD_CTL_PAD[padData->A_SS0.muxPadIndex] = csPadCtrlValue;
        }
        if (config->sflashA2Size)
        {
            if (padData->A_SS1.muxPadIndex)
            {
                IOMUXC->SW_MUX_CTL_PAD[padData->A_SS1.muxPadIndex] = padData->A_SS1.muxValue;
                IOMUXC->SW_PAD_CTL_PAD[padData->A_SS1.muxPadIndex] = csPadCtrlValue;
            }
        }

        // Configure Clock
        if (padData->A_SCLK.muxPadIndex)
        {
            IOMUXC->SW_MUX_CTL_PAD[padData->A_SCLK.muxPadIndex] =
                (uint32_t)padData->A_SCLK.muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
            IOMUXC->SW_PAD_CTL_PAD[padData->A_SCLK.muxPadIndex] = sclkPadCtrlValue;
        }

        // Configure DATA0-DATA3
        for (uint32_t i = 0; i < 4; i++)
        {
            const flexspi_pad_attribute_t *padAttribute = &padData->A_DATA[i];

            IOMUXC->SW_MUX_CTL_PAD[padAttribute->muxPadIndex] = padAttribute->muxValue;
            IOMUXC->SW_PAD_CTL_PAD[padAttribute->muxPadIndex] = dataPadCtrlValue;
            if (padAttribute->selectInputIndex)
            {
                IOMUXC->SELECT_INPUT[padAttribute->selectInputIndex] = padAttribute->selectInputValue;
            }
        }

        // Configure Data 4-Data7
        if (config->sflashPadType == kSerialFlash_8Pads)
        {
            if (padData->A_DATA[4].muxPadIndex > 0)
            {
                for (uint32_t i = 4; i < 8; i++)
                {
                    const flexspi_pad_attribute_t *padAttribute = &padData->A_DATA[i];
                    IOMUXC->SW_MUX_CTL_PAD[padAttribute->muxPadIndex] = padAttribute->muxValue;
                    IOMUXC->SW_PAD_CTL_PAD[padAttribute->muxPadIndex] = dataPadCtrlValue;
                    if (padAttribute->selectInputIndex)
                    {
                        IOMUXC->SELECT_INPUT[padAttribute->selectInputIndex] = padAttribute->selectInputValue;
                    }
                }
            }
        }

        // Configure DQS pad
        if ((config->readSampleClkSrc == kFlexSPIReadSampleClk_ExternalInputFromDqsPad) ||
            (config->readSampleClkSrc == kFlexSPIReadSampleClk_LoopbackFromDqsPad))
        {
            const flexspi_pad_attribute_t *dqsPadAttr = &padData->A_DQS;
            // For the internal loopback, always use the DUMMY pad, so that users can have one more pad for
            // other purpose
            bool isDqsOnAon = false;
            if ((kFlexSPIReadSampleClk_LoopbackFromDqsPad == config->readSampleClkSrc) &&
                (config->controllerMiscOption & FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondDqsPinMux)))
            {
                dqsPadAttr = &padData->A_DQS_DUMMY;
                isDqsOnAon = (padData->A_DQS_DUMMY_IOMUXC_IDX == kIomuxIdx_IOMUXC_AON);
            }

            // FLEXSPIA_DQS
            if ((dqsPadAttr != NULL) && dqsPadAttr->muxPadIndex)
            {
                if (isDqsOnAon)
                {
                    IOMUXC_AON->SW_MUX_CTL_PAD[dqsPadAttr->muxPadIndex] =
                        (uint32_t)dqsPadAttr->muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                    IOMUXC_AON->SW_PAD_CTL_PAD[dqsPadAttr->muxPadIndex] = dqsPadCtrlValue;
                }
                else
                {
                    IOMUXC->SW_MUX_CTL_PAD[dqsPadAttr->muxPadIndex] =
                        (uint32_t)dqsPadAttr->muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                    IOMUXC->SW_PAD_CTL_PAD[dqsPadAttr->muxPadIndex] = dqsPadCtrlValue;
                }
                if (dqsPadAttr->selectInputIndex)
                {
                    IOMUXC->SELECT_INPUT[dqsPadAttr->selectInputIndex] = dqsPadAttr->selectInputValue;
                }
            }
        }

        // Configure Differential Clock pin
        if (flexspi_is_differential_clock_enable(config))
        {
            if (padData->A_SCLKB.muxPadIndex)
            {
                IOMUXC->SW_MUX_CTL_PAD[padData->A_SCLKB.muxPadIndex] =
                    (uint32_t)padData->A_SCLKB.muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                IOMUXC->SW_PAD_CTL_PAD[padData->A_SCLKB.muxPadIndex] = sclkPadCtrlValue;
                if (padData->A_SCLKB.selectInputIndex)
                {
                    IOMUXC->SELECT_INPUT[padData->A_SCLKB.selectInputIndex] = padData->A_SCLKB.selectInputValue;
                }
            }
        }
    } // if (config->sflashA1Size || config->sflashA2Size)

    if (config->sflashB1Size || config->sflashB2Size)
    {
        // Configure Chip Select Pin
        if (config->sflashB1Size)
        {
            if (padData->B_SS0.muxPadIndex)
            {
                IOMUXC->SW_MUX_CTL_PAD[padData->B_SS0.muxPadIndex] = padData->B_SS0.muxValue;
                IOMUXC->SW_PAD_CTL_PAD[padData->B_SS0.muxPadIndex] = csPadCtrlValue;
            }
        }
        if (config->sflashB2Size)
        {
            if (padData->B_SS1.muxPadIndex)
            {
                IOMUXC->SW_MUX_CTL_PAD[padData->B_SS1.muxPadIndex] = padData->B_SS1.muxValue;
                IOMUXC->SW_PAD_CTL_PAD[padData->B_SS1.muxPadIndex] = csPadCtrlValue;
            }
        }

        // Only configure the remaining pads if the pad setting is available on the SoC
        if (padData->B_SS0.muxPadIndex || padData->B_SS1.muxPadIndex)
        {
            // Configure Clock
            if (padData->B_SCLK.muxPadIndex)
            {
                IOMUXC->SW_MUX_CTL_PAD[padData->B_SCLK.muxPadIndex] =
                    (uint32_t)padData->B_SCLK.muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                IOMUXC->SW_PAD_CTL_PAD[padData->B_SCLK.muxPadIndex] = sclkPadCtrlValue;
            }

            // Configure DATA0-DATA3
            for (uint32_t i = 0; i < 4; i++)
            {
                const flexspi_pad_attribute_t *padAttribute = &padData->B_DATA[i];
                IOMUXC->SW_MUX_CTL_PAD[padAttribute->muxPadIndex] = padAttribute->muxValue;
                IOMUXC->SW_PAD_CTL_PAD[padAttribute->muxPadIndex] = dataPadCtrlValue;
                if (padAttribute->selectInputIndex)
                {
                    IOMUXC->SELECT_INPUT[padAttribute->selectInputIndex] = padAttribute->selectInputValue;
                }
            }

            // Configure Data 4-Data7
            if (config->sflashPadType == kSerialFlash_8Pads)
            {
                if (padData->B_DATA[4].muxPadIndex > 0)
                {
                    for (uint32_t i = 4; i < 8; i++)
                    {
                        const flexspi_pad_attribute_t *padAttribute = &padData->B_DATA[i];
                        IOMUXC->SW_MUX_CTL_PAD[padAttribute->muxPadIndex] = padAttribute->muxValue;
                        IOMUXC->SW_PAD_CTL_PAD[padAttribute->muxPadIndex] = dataPadCtrlValue;
                        if (padAttribute->selectInputIndex)
                        {
                            IOMUXC->SELECT_INPUT[padAttribute->selectInputIndex] = padAttribute->selectInputValue;
                        }
                    }
                }
            }

            // Configure DQS pad
            if ((config->readSampleClkSrc == kFlexSPIReadSampleClk_ExternalInputFromDqsPad) ||
                (config->readSampleClkSrc == kFlexSPIReadSampleClk_LoopbackFromDqsPad))
            {
                const flexspi_pad_attribute_t *dqsPadAttr = &padData->B_DQS;
                bool isDqsOnAon = false;
                if ((config->readSampleClkSrc == kFlexSPIReadSampleClk_LoopbackFromDqsPad) &&
                    (config->controllerMiscOption & FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondDqsPinMux)))
                {
                    dqsPadAttr = &padData->B_DQS_DUMMY;
                    isDqsOnAon = (padData->B_DQS_DUMMY_IOMUXC_IDX == kIomuxIdx_IOMUXC_AON);
                }
                // FLEXSPIA_DQS
                if (dqsPadAttr->muxPadIndex)
                {
                    if (isDqsOnAon)
                    {
                        IOMUXC_AON->SW_MUX_CTL_PAD[dqsPadAttr->muxPadIndex] =
                            (uint32_t)dqsPadAttr->muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                        IOMUXC_AON->SW_PAD_CTL_PAD[dqsPadAttr->muxPadIndex] = dqsPadCtrlValue;
                    }
                    else
                    {
                        IOMUXC->SW_MUX_CTL_PAD[dqsPadAttr->muxPadIndex] =
                            (uint32_t)dqsPadAttr->muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                        IOMUXC->SW_PAD_CTL_PAD[dqsPadAttr->muxPadIndex] = dqsPadCtrlValue;
                    }
                    if (dqsPadAttr->selectInputIndex)
                    {
                        IOMUXC->SELECT_INPUT[dqsPadAttr->selectInputIndex] = dqsPadAttr->selectInputValue;
                    }
                }
            }

            // Configure Differential Clock pin
            if (flexspi_is_differential_clock_enable(config))
            {
                if (padData->B_SCLKB.muxPadIndex)
                {
                    IOMUXC->SW_MUX_CTL_PAD[padData->B_SCLKB.muxPadIndex] =
                        (uint32_t)padData->B_SCLKB.muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                    IOMUXC->SW_PAD_CTL_PAD[padData->B_SCLKB.muxPadIndex] = sclkPadCtrlValue;
                    if (padData->B_SCLKB.selectInputIndex)
                    {
                        IOMUXC->SELECT_INPUT[padData->B_SCLKB.selectInputIndex] = padData->B_SCLKB.selectInputValue;
                    }
                }
            }
        }
    } // if (config->sflashB1Size || config->sflashB2Size)
}

static void flexspi_iomux_config_on_iomuxc_aon(const flexspi_pad_data_t *padData, flexspi_mem_config_t *config)
{
    uint32_t csPadCtrlValue = config->csPadSettingOverride ? config->csPadSettingOverride : padData->csPadSetting;
    uint32_t dqsPadCtrlValue = config->dqsPadSettingOverride ? config->dqsPadSettingOverride : padData->dqsPadSetting;
    uint32_t sclkPadCtrlValue =
        config->sclkPadSettingOverride ? config->sclkPadSettingOverride : padData->sclkPadSetting;
    uint32_t dataPadCtrlValue =
        config->dataPadSettingOverride ? config->dataPadSettingOverride : padData->dataPadSetting;

    if (config->sflashA1Size || config->sflashA2Size)
    {
        // Configure Chip Select Pin
        if (config->sflashA1Size)
        {
            IOMUXC_AON->SW_MUX_CTL_PAD[padData->A_SS0.muxPadIndex] = padData->A_SS0.muxValue;
            IOMUXC_AON->SW_PAD_CTL_PAD[padData->A_SS0.muxPadIndex] = csPadCtrlValue;
        }
        if (config->sflashA2Size)
        {
            if (padData->A_SS1.muxPadIndex)
            {
                IOMUXC_AON->SW_MUX_CTL_PAD[padData->A_SS1.muxPadIndex] = padData->A_SS1.muxValue;
                IOMUXC_AON->SW_PAD_CTL_PAD[padData->A_SS1.muxPadIndex] = csPadCtrlValue;
            }
        }

        // Configure Clock
        if (padData->A_SCLK.muxPadIndex)
        {
            IOMUXC_AON->SW_MUX_CTL_PAD[padData->A_SCLK.muxPadIndex] =
                (uint32_t)padData->A_SCLK.muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
            IOMUXC_AON->SW_PAD_CTL_PAD[padData->A_SCLK.muxPadIndex] = sclkPadCtrlValue;
        }

        // Configure DATA0-DATA3
        for (uint32_t i = 0; i < 4; i++)
        {
            const flexspi_pad_attribute_t *padAttribute = &padData->A_DATA[i];

            IOMUXC_AON->SW_MUX_CTL_PAD[padAttribute->muxPadIndex] = padAttribute->muxValue;
            IOMUXC_AON->SW_PAD_CTL_PAD[padAttribute->muxPadIndex] = dataPadCtrlValue;
            if (padAttribute->selectInputIndex)
            {
                IOMUXC->SELECT_INPUT[padAttribute->selectInputIndex] = padAttribute->selectInputValue;
            }
        }

        // Configure Data 4-Data7
        if (config->sflashPadType == kSerialFlash_8Pads)
        {
            if (padData->A_DATA[4].muxPadIndex > 0)
            {
                for (uint32_t i = 4; i < 8; i++)
                {
                    const flexspi_pad_attribute_t *padAttribute = &padData->A_DATA[i];
                    IOMUXC_AON->SW_MUX_CTL_PAD[padAttribute->muxPadIndex] = padAttribute->muxValue;
                    IOMUXC_AON->SW_PAD_CTL_PAD[padAttribute->muxPadIndex] = dataPadCtrlValue;
                    if (padAttribute->selectInputIndex)
                    {
                        IOMUXC->SELECT_INPUT[padAttribute->selectInputIndex] = padAttribute->selectInputValue;
                    }
                }
            }
        }

        // Configure DQS pad
        if ((config->readSampleClkSrc == kFlexSPIReadSampleClk_ExternalInputFromDqsPad) ||
            (config->readSampleClkSrc == kFlexSPIReadSampleClk_LoopbackFromDqsPad))
        {
            const flexspi_pad_attribute_t *dqsPadAttr = &padData->A_DQS;
            // FOr the internal loopback, always use the DUMMY pad, so that users can have one more pad for
            // other purpose
            if ((config->readSampleClkSrc == kFlexSPIReadSampleClk_LoopbackFromDqsPad) &&
                (config->controllerMiscOption & FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondDqsPinMux)))
            {
                dqsPadAttr = &padData->A_DQS_DUMMY;
            }

            // FLEXSPIA_DQS
            if ((dqsPadAttr != NULL) && dqsPadAttr->muxPadIndex)
            {
                IOMUXC_AON->SW_MUX_CTL_PAD[dqsPadAttr->muxPadIndex] =
                    (uint32_t)dqsPadAttr->muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                IOMUXC_AON->SW_PAD_CTL_PAD[dqsPadAttr->muxPadIndex] = dqsPadCtrlValue;
                if (dqsPadAttr->selectInputIndex)
                {
                    IOMUXC->SELECT_INPUT[dqsPadAttr->selectInputIndex] = dqsPadAttr->selectInputValue;
                }
            }
        }

        // Configure Differential Clock pin
        if (flexspi_is_differential_clock_enable(config))
        {
            if (padData->A_SCLKB.muxPadIndex)
            {
                IOMUXC_AON->SW_MUX_CTL_PAD[padData->A_SCLKB.muxPadIndex] =
                    (uint32_t)padData->A_SCLKB.muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                IOMUXC_AON->SW_PAD_CTL_PAD[padData->A_SCLKB.muxPadIndex] = sclkPadCtrlValue;
                if (padData->A_SCLKB.selectInputIndex)
                {
                    IOMUXC->SELECT_INPUT[padData->A_SCLKB.selectInputIndex] = padData->A_SCLKB.selectInputValue;
                }
            }
        }
    } // if (config->sflashA1Size || config->sflashA2Size)

    if (config->sflashB1Size || config->sflashB2Size)
    {
        // Configure Chip Select Pin
        if (config->sflashB1Size)
        {
            if (padData->B_SS0.muxPadIndex)
            {
                IOMUXC_AON->SW_MUX_CTL_PAD[padData->B_SS0.muxPadIndex] = padData->B_SS0.muxValue;
                IOMUXC_AON->SW_PAD_CTL_PAD[padData->B_SS0.muxPadIndex] = csPadCtrlValue;
            }
        }
        if (config->sflashB2Size)
        {
            if (padData->B_SS1.muxPadIndex)
            {
                IOMUXC_AON->SW_MUX_CTL_PAD[padData->B_SS1.muxPadIndex] = padData->B_SS1.muxValue;
                IOMUXC_AON->SW_PAD_CTL_PAD[padData->B_SS1.muxPadIndex] = csPadCtrlValue;
            }
        }

        // Only configure the remaining pads if the pad setting is available on the SoC
        if (padData->B_SS0.muxPadIndex || padData->B_SS1.muxPadIndex)
        {
            // Configure Clock
            if (padData->B_SCLK.muxPadIndex)
            {
                IOMUXC_AON->SW_MUX_CTL_PAD[padData->B_SCLK.muxPadIndex] =
                    (uint32_t)padData->B_SCLK.muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                IOMUXC_AON->SW_PAD_CTL_PAD[padData->B_SCLK.muxPadIndex] = sclkPadCtrlValue;
            }

            // Configure DATA0-DATA3
            for (uint32_t i = 0; i < 4; i++)
            {
                const flexspi_pad_attribute_t *padAttribute = &padData->B_DATA[i];
                IOMUXC_AON->SW_MUX_CTL_PAD[padAttribute->muxPadIndex] = padAttribute->muxValue;
                IOMUXC_AON->SW_PAD_CTL_PAD[padAttribute->muxPadIndex] = dataPadCtrlValue;
                if (padAttribute->selectInputIndex)
                {
                    IOMUXC->SELECT_INPUT[padAttribute->selectInputIndex] = padAttribute->selectInputValue;
                }
            }

            // Configure Data 4-Data7
            if (config->sflashPadType == kSerialFlash_8Pads)
            {
                if (padData->B_DATA[4].muxPadIndex > 0)
                {
                    for (uint32_t i = 4; i < 8; i++)
                    {
                        const flexspi_pad_attribute_t *padAttribute = &padData->B_DATA[i];
                        IOMUXC_AON->SW_MUX_CTL_PAD[padAttribute->muxPadIndex] = padAttribute->muxValue;
                        IOMUXC_AON->SW_PAD_CTL_PAD[padAttribute->muxPadIndex] = dataPadCtrlValue;
                        if (padAttribute->selectInputIndex)
                        {
                            IOMUXC->SELECT_INPUT[padAttribute->selectInputIndex] = padAttribute->selectInputValue;
                        }
                    }
                }
            }

            // Configure DQS pad
            if ((config->readSampleClkSrc == kFlexSPIReadSampleClk_ExternalInputFromDqsPad) ||
                (config->readSampleClkSrc == kFlexSPIReadSampleClk_LoopbackFromDqsPad))
            {
                const flexspi_pad_attribute_t *dqsPadAttr = &padData->B_DQS;
                if ((kFlexSPIReadSampleClk_LoopbackFromDqsPad == config->readSampleClkSrc) &&
                    (config->controllerMiscOption & FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondDqsPinMux)))
                {
                    dqsPadAttr = &padData->B_DQS_DUMMY;
                }
                // FLEXSPIA_DQS
                if (dqsPadAttr->muxPadIndex)
                {
                    IOMUXC_AON->SW_MUX_CTL_PAD[dqsPadAttr->muxPadIndex] =
                        (uint32_t)dqsPadAttr->muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                    IOMUXC_AON->SW_PAD_CTL_PAD[dqsPadAttr->muxPadIndex] = dqsPadCtrlValue;
                    if (dqsPadAttr->selectInputIndex)
                    {
                        IOMUXC->SELECT_INPUT[dqsPadAttr->selectInputIndex] = dqsPadAttr->selectInputValue;
                    }
                }
            }

            // Configure Differential Clock pin
            if (flexspi_is_differential_clock_enable(config))
            {
                if (padData->B_SCLKB.muxPadIndex)
                {
                    IOMUXC_AON->SW_MUX_CTL_PAD[padData->B_SCLKB.muxPadIndex] =
                        (uint32_t)padData->B_SCLKB.muxValue | IOMUXC_SW_MUX_CTL_PAD_SION(1);
                    IOMUXC_AON->SW_PAD_CTL_PAD[padData->B_SCLKB.muxPadIndex] = sclkPadCtrlValue;
                    if (padData->B_SCLKB.selectInputIndex)
                    {
                        IOMUXC->SELECT_INPUT[padData->B_SCLKB.selectInputIndex] = padData->B_SCLKB.selectInputValue;
                    }
                }
            }
        }
    } // if (config->sflashB1Size || config->sflashB2Size)
}

enum
{
    kFlexSpi_PrimaryGroup = 0,
    kFlexSpi_SecondaryGroup = 1,
};

#define GET_FLEXSPI_PIN_TBL_IDX(inst, group) ((inst - 1) * 2 + group)

void flexspi_iomux_config(uint32_t instance, flexspi_mem_config_t *config)
{
    if ((instance != kFlexspiInstance_1) && (instance != kFlexspiInstance_2))
    {
        return;
    }

    uint32_t group = 0;

    if (config->controllerMiscOption & FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondPinMux))
    {
        debug_printf("%s, secondary pin group is selected", __func__);
        group = kFlexSpi_SecondaryGroup;
    }

    // Get the index of the pad setting entry
    uint32_t padSettingIndex = GET_FLEXSPI_PIN_TBL_IDX(instance, group);
    const flexspi_pad_data_t *padData = &k_flexspi_pads[padSettingIndex];
    if (padData->IOMUXC_IDX == kIomuxIdx_IOMUXC)
    {
        flexspi_iomux_config_on_iomuxc(padData, config);
    }
    else
    {
        flexspi_iomux_config_on_iomuxc_aon(padData, config);
    }
}
void flexspi_sw_delay_us(uint64_t us)
{
    uint32_t ticks_per_us = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000;
    while (us--)
    {
        register uint32_t ticks = 1 + ticks_per_us / 4;
        while (ticks--)
        {
            __NOP();
        }
    }
}

//!@brief Get maximum frequency supported by FlexSPI
status_t flexspi_get_max_supported_freq(uint32_t instance, uint32_t *freq, uint32_t clkMode)
{
    status_t status = kStatus_InvalidArgument;
    do
    {
        if (freq == NULL)
        {
            break;
        }

        *freq = (166UL * 1000 * 1000);
        status = kStatus_Success;

    } while (0);

    return status;
}

//!@brief Set FlexSPI failsafe setting
status_t flexspi_set_failsafe_setting(flexspi_mem_config_t *config)
{
    (void)config;

    return kStatus_Success;
}

// Update Clock Source
status_t flexspi_update_clock_source(uint32_t instance, uint32_t source)
{
    status_t status = kStatus_InvalidArgument;

    do
    {
        if ((instance < kFlexspiInstance_1) || (instance > kFlexspiInstance_2) || (source > kFlexSpiClkSrc_PLL_480))
        {
            break;
        }

        volatile uint32_t *clkRootRegister = NULL;
        if (instance == kFlexspiInstance_1)
        {
            clkRootRegister = &CCM->CLOCK_ROOT[kCLOCK_Root_Flexspi1].CONTROL;
        }
        else if (instance == kFlexspiInstance_2)
        {
            clkRootRegister = &CCM->CLOCK_ROOT[kCLOCK_Root_Flexspi2].CONTROL;
        }

        uint32_t expectedRegValue =
            ((*clkRootRegister) & ~CCM_CLOCK_ROOT_CONTROL_MUX_MASK) | CCM_CLOCK_ROOT_CONTROL_MUX(source);
        FLEXSPI_Type *g_flexspiInstances[] = FLEXSPI_BASE_PTRS;
        if (expectedRegValue != *clkRootRegister)
        {
            // Disable FlexSPI before clock switch
            bool needRestoreClock = false;
            if (is_flexspi_clock_enabled(instance))
            {
                g_flexspiInstances[instance]->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
                flexspi_clock_gate_disable(instance);

                needRestoreClock = true;
            }
            *clkRootRegister = expectedRegValue;

            if (needRestoreClock)
            {
                // Restore FlexSPI after clock switch
                flexspi_clock_gate_enable(instance);
                g_flexspiInstances[instance]->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
            }
        }

        status = kStatus_Success;

    } while (0);

    return status;
}

//!@brief Configure clock for FlexSPI peripheral
void flexspi_clock_config(uint32_t instance, uint32_t freq, uint32_t sampleClkMode)
{
#if !defined(BL_TARGET_FPGA)
    do
    {
        if ((sampleClkMode > kFlexSpiClk_DDR) || (freq < 1U))
        {
            break;
        }

        if ((instance != kFlexspiInstance_1) && (instance != kFlexspiInstance_2))
        {
            break;
        }

        volatile uint32_t *clkRootRegister = NULL;
        if (instance == kFlexspiInstance_1)
        {
            clkRootRegister = &CCM->CLOCK_ROOT[kCLOCK_Root_Flexspi1].CONTROL;
        }
        else
        {
            clkRootRegister = &CCM->CLOCK_ROOT[kCLOCK_Root_Flexspi2].CONTROL;
        }

        uint32_t freqSrc = ((*clkRootRegister) & CCM_CLOCK_ROOT_CONTROL_MUX_MASK) >> CCM_CLOCK_ROOT_CONTROL_MUX_SHIFT;

        if (freqSrc >= kFlexSpiClkSrc_MuxOpt_Num)
        {
            break;
        }

        uint32_t srcMux;    // Clock root mux
        uint32_t srcTyp;    // Clock source type
        clock_pll_t pllIdx; // PLL index (if available)
        clock_pfd_t pfdIdx; // PFD index (if available)
        uint32_t pfdFrc;    // PFD frac (if available)
        uint32_t clkDiv;    // Clock root divider

        const flexspi_clk_freq_list_t *freq_list = &k_flexspi_clk_list_sdr[instance - kFlexSpi_1st_Inst][freqSrc];
        if (sampleClkMode == kFlexSpiClk_DDR)
        {
            freq_list = &k_flexspi_clk_list_ddr[instance - kFlexSpi_1st_Inst][freqSrc];
        }

        if (freq > kFlexSpiSerialClk_200MHz)
        {
            freq = kFlexSpiSerialClk_SafeFreq;
        }

        srcMux = freq_list->src_mux;
        srcTyp = freq_list->src_typ;
        pllIdx = freq_list->pll_idx;
        pfdIdx = freq_list->pfd_idx;
        pfdFrc = freq_list->clk_cfg[freq - 1].pfd_frc; // PLL/PFD's frac: 13~35
        clkDiv = freq_list->clk_cfg[freq - 1].clk_div; // Clock root's div: 1~256

        uint32_t tmpClkRootRegValue = CCM_CLOCK_ROOT_CONTROL_DIV(clkDiv - 1) | CCM_CLOCK_ROOT_CONTROL_MUX(srcMux);

        // Disable FlexSPI before clock switch
        if (is_flexspi_clock_enabled(instance))
        {
            g_flexSpiInstances[instance]->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
            flexspi_clock_gate_disable(instance);
        }

        // Update FLEXSPI clock root setting as needed
        if (tmpClkRootRegValue != *clkRootRegister)
        {
            if (kFlexSpiClkSrc_Type_PllPfd == srcTyp)
            {
                CLOCK_InitPfd(pllIdx, pfdIdx, pfdFrc);
            }

            *clkRootRegister = tmpClkRootRegValue;
            (void)*clkRootRegister; // Intentionally readback for write-sync
        }

        // Restore FlexSPI after clock switch
        flexspi_clock_gate_enable(instance);
        g_flexSpiInstances[instance]->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;

        uint32_t serialRootClk = 0;
        flexspi_get_clock(instance, kFlexSpiClock_SerialRootClock, &serialRootClk);

        debug_printf("%s: expected freq=%d, configured clk root=%d", __func__, freq, serialRootClk);
    } while (0);
#endif
}

//!@brief Gate on the clock for the FlexSPI peripheral
void flexspi_clock_gate_enable(uint32_t instance)
{
    volatile uint32_t *reg = NULL;
    if (instance == kFlexspiInstance_1)
    {
        reg = &CCM->LPCG[kCLOCK_Flexspi1].DIRECT;
    }
    else if (instance == kFlexspiInstance_2)
    {
        reg = &CCM->LPCG[kCLOCK_Flexspi2].DIRECT;
    }

    if (reg != NULL)
    {
        *reg = 1;

        __DSB();
        __ISB();
    }
}

//!@brief Gate off the clock the FlexSPI peripheral
void flexspi_clock_gate_disable(uint32_t instance)
{
    volatile uint32_t *reg = NULL;
    if (instance == kFlexspiInstance_1)
    {
        reg = &CCM->LPCG[kCLOCK_Flexspi1].DIRECT;
    }
    else if (instance == kFlexspiInstance_2)
    {
        reg = &CCM->LPCG[kCLOCK_Flexspi2].DIRECT;
    }

    if (reg != NULL)
    {
        *reg = 0;

        __DSB();
        __ISB();
    }
}

bool is_flexspi_clock_enabled(uint32_t instance)
{
    volatile uint32_t *reg = NULL;
    bool isEnabled = false;
    if (instance == kFlexspiInstance_1)
    {
        reg = &CCM->LPCG[kCLOCK_Flexspi1].DIRECT;
    }
    else if (instance == kFlexspiInstance_2)
    {
        reg = &CCM->LPCG[kCLOCK_Flexspi2].DIRECT;
    }
    if (reg != NULL)
    {
        isEnabled = *reg ? true : false;
    }

    return isEnabled;
}

void flexspi_update_padsetting(uint32_t instance, flexspi_mem_config_t *config, uint32_t padSetting)
{
#define PAD_SLEW(padSetting) (((padSetting) >> 0x0U) & 0x1U)
#define PAD_DRV(padSetting) (((padSetting) >> 0x1U) & 0x1U)
#define PAD_PULL(padSetting) (((padSetting) >> 0x2U) & 0x1U)

    if ((instance != kFlexspiInstance_1) && (instance != kFlexspiInstance_2))
    {
        return;
    }

    if (NULL == config)
    {
        return;
    }

    uint32_t group = (config->controllerMiscOption & FLEXSPI_BITMASK(kFlexSpiMiscOffset_SecondPinMux)) ?
                         kFlexSpiPinGroup_Secondary :
                         kFlexSpiPinGroup_Primary;
    const flexspi_pad_data_t *padData = &k_flexspi_pads[GET_FLEXSPI_PIN_TBL_IDX(instance, group)];

    config->csPadSettingOverrideEn = 1;
    config->sclkPadSettingOverrideEn = 1;
    config->dataPadSettingOverrideEn = 1;

    if (kIomuxIdx_IOMUXC == padData->IOMUXC_IDX)
    {
        config->csPadSettingOverride =
            PAD_PULL(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_PULL(1) : IOMUXC_SW_PAD_CTL_PAD_PULL(3);
        config->csPadSettingOverride |=
            PAD_DRV(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_PDRV(0) : IOMUXC_SW_PAD_CTL_PAD_PDRV(1);
        config->sclkPadSettingOverride =
            PAD_PULL(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_PULL(1) : IOMUXC_SW_PAD_CTL_PAD_PULL(3);
        config->sclkPadSettingOverride |=
            PAD_DRV(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_PDRV(0) : IOMUXC_SW_PAD_CTL_PAD_PDRV(1);
        config->dataPadSettingOverride =
            PAD_PULL(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_PULL(1) : IOMUXC_SW_PAD_CTL_PAD_PULL(3);
        config->dataPadSettingOverride |=
            PAD_DRV(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_PDRV(0) : IOMUXC_SW_PAD_CTL_PAD_PDRV(1);
    }
    else // kIomuxIdx_IOMUXC_AON
    {
        config->csPadSettingOverride = PAD_PULL(padSetting) ?
                                           (IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1)) :
                                           IOMUXC_SW_PAD_CTL_PAD_PUE(0);
        config->csPadSettingOverride |=
            PAD_DRV(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_DSE(1) : IOMUXC_SW_PAD_CTL_PAD_DSE(0);
        config->csPadSettingOverride |=
            PAD_SLEW(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_SRE(0) : IOMUXC_SW_PAD_CTL_PAD_SRE(1);
        config->sclkPadSettingOverride = PAD_PULL(padSetting) ?
                                             (IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1)) :
                                             IOMUXC_SW_PAD_CTL_PAD_PUE(0);
        config->sclkPadSettingOverride |=
            PAD_DRV(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_DSE(1) : IOMUXC_SW_PAD_CTL_PAD_DSE(0);
        config->sclkPadSettingOverride |=
            PAD_SLEW(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_SRE(0) : IOMUXC_SW_PAD_CTL_PAD_SRE(1);
        config->dataPadSettingOverride = PAD_PULL(padSetting) ?
                                             (IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1)) :
                                             IOMUXC_SW_PAD_CTL_PAD_PUE(0);
        config->dataPadSettingOverride |=
            PAD_DRV(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_DSE(1) : IOMUXC_SW_PAD_CTL_PAD_DSE(0);
        config->dataPadSettingOverride |=
            PAD_SLEW(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_SRE(0) : IOMUXC_SW_PAD_CTL_PAD_SRE(1);
    }

    config->dqsPadSettingOverrideEn = 1;

    if ((kIomuxIdx_IOMUXC == padData->A_DQS_DUMMY_IOMUXC_IDX) || (kIomuxIdx_IOMUXC == padData->B_DQS_DUMMY_IOMUXC_IDX))
    {
        config->dqsPadSettingOverride =
            PAD_PULL(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_PULL(2) : IOMUXC_SW_PAD_CTL_PAD_PULL(3);
        config->dqsPadSettingOverride |=
            PAD_DRV(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_PDRV(0) : IOMUXC_SW_PAD_CTL_PAD_PDRV(1);
    }
    else // kIomuxIdx_IOMUXC_AON
    {
        config->dqsPadSettingOverride = PAD_PULL(padSetting) ?
                                            (IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(0)) :
                                            IOMUXC_SW_PAD_CTL_PAD_PUE(0);
        config->dqsPadSettingOverride |=
            PAD_DRV(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_DSE(1) : IOMUXC_SW_PAD_CTL_PAD_DSE(0);
        config->dqsPadSettingOverride |=
            PAD_SLEW(padSetting) ? IOMUXC_SW_PAD_CTL_PAD_SRE(0) : IOMUXC_SW_PAD_CTL_PAD_SRE(1);
    }
}
