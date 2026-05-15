/*
 * Copyright 2018 - 2019 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_semc.h"
#include "bootloader.h"
#include "fsl_iomuxc.h"
#include "fusemap.h"
#include "memory_config.h"
#include "peripherals_pinmux.h"

#if BL_FEATURE_EDGELOCK_MODULE
#include "fsl_edgelock.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void update_memory_map(void);

/*******************************************************************************
 * Codes
 ******************************************************************************/
//!@brief Configure IOMUX for SEMC Peripheral
void semc_iomux_config(semc_mem_config_t *config)
{
    uint8_t cePortOutputSelection;

    // Pinmux configuration for SEMC DA[15:0] Port (NOR)
    // Pinmux configuration for SEMC D[15:0] Port (NAND)
    IOMUXC_SetPinMux(SEMC_DATA00_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_DATA01_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_DATA02_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_DATA03_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_DATA04_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_DATA05_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_DATA06_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_DATA07_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinConfig(SEMC_DATA00_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_DATA01_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_DATA02_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_DATA03_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_DATA04_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_DATA05_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_DATA06_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_DATA07_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);

    if (
#if BL_FEATURE_SEMC_NOR_MODULE
        (config->deviceMemType == kSemcDeviceMemType_NOR) ||
#endif
#if BL_FEATURE_SEMC_NAND_MODULE
        ((config->deviceMemType == kSemcDeviceMemType_NAND) && (config->nandMemConfig.ioPortWidth == 16u)) ||
#endif
        0 /* dummy condition */
    )
    {
        IOMUXC_SetPinMux(SEMC_DATA08_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_DATA09_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_DATA10_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_DATA11_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_DATA12_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_DATA13_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_DATA14_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_DATA15_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinConfig(SEMC_DATA08_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_DATA09_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_DATA10_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_DATA11_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_DATA12_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_DATA13_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_DATA14_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_DATA15_IOMUXC_MUX_FUNC, SEMC_DATA_PAD_CTRL);
    }

    // Pinmux configuration for SEMC WE,OE,ADV Port (NOR)
    // Pinmux configuration for SEMC WE,RE,ALE Port (NAND)
    IOMUXC_SetPinMux(SEMC_ADDR11_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_ADDR12_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinMux(SEMC_BA1_IOMUXC_MUX_FUNC, 0U);
    IOMUXC_SetPinConfig(SEMC_ADDR11_IOMUXC_MUX_FUNC, SEMC_WE_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_ADDR12_IOMUXC_MUX_FUNC, SEMC_RE_PAD_CTRL);
    IOMUXC_SetPinConfig(SEMC_BA1_IOMUXC_MUX_FUNC, SEMC_ALE_PAD_CTRL);

#if BL_FEATURE_SEMC_NOR_MODULE
    if (config->deviceMemType == kSemcDeviceMemType_NOR)
    {
        IOMUXC_SetPinMux(SEMC_ADDR00_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_ADDR01_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_ADDR02_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_ADDR03_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_ADDR04_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_ADDR05_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_ADDR06_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_ADDR07_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinConfig(SEMC_ADDR00_IOMUXC_MUX_FUNC, SEMC_ADDR_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_ADDR01_IOMUXC_MUX_FUNC, SEMC_ADDR_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_ADDR02_IOMUXC_MUX_FUNC, SEMC_ADDR_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_ADDR03_IOMUXC_MUX_FUNC, SEMC_ADDR_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_ADDR04_IOMUXC_MUX_FUNC, SEMC_ADDR_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_ADDR05_IOMUXC_MUX_FUNC, SEMC_ADDR_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_ADDR06_IOMUXC_MUX_FUNC, SEMC_ADDR_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_ADDR07_IOMUXC_MUX_FUNC, SEMC_ADDR_PAD_CTRL);

        cePortOutputSelection = config->norMemConfig.cePortOutputSelection;
    }
    else
#endif // #if BL_FEATURE_SEMC_NOR_MODULE
#if BL_FEATURE_SEMC_NAND_MODULE
        if (config->deviceMemType == kSemcDeviceMemType_NAND)
    {
        // Pinmux configuration for SEMC CLE,R/B Port (NAND)
        IOMUXC_SetPinMux(SEMC_RDY_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinMux(SEMC_ADDR09_IOMUXC_MUX_FUNC, 0U);
        IOMUXC_SetPinConfig(SEMC_RDY_IOMUXC_MUX_FUNC, SEMC_RB_PAD_CTRL);
        IOMUXC_SetPinConfig(SEMC_ADDR09_IOMUXC_MUX_FUNC, SEMC_CLE_PAD_CTRL);

        cePortOutputSelection = config->nandMemConfig.cePortOutputSelection;
    }
    else
#endif
    {
        cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX0; // Use the default CS0
    }

    // Pinmux configuration for SEMC CE Port (NAND/NOR)
    switch (cePortOutputSelection)
    {
        case kSemcCeOutputSelection_MUX_A8:
            IOMUXC_SetPinMux(SEMC_CSXA8_IOMUXC_MUX_FUNC, 0U);
            IOMUXC_SetPinConfig(SEMC_CSXA8_IOMUXC_MUX_FUNC, SEMC_CSX_PAD_CTRL);
            break;
        case kSemcCeOutputSelection_MUX_CSX3:
            IOMUXC_SetPinMux(SEMC_CSX1_IOMUXC_MUX_FUNC, 0U);
            IOMUXC_SetPinConfig(SEMC_CSX1_IOMUXC_MUX_FUNC, SEMC_CSX_PAD_CTRL);
            break;
        case kSemcCeOutputSelection_MUX_CSX2:
            IOMUXC_SetPinMux(SEMC_CSX2_IOMUXC_MUX_FUNC, 0U);
            IOMUXC_SetPinConfig(SEMC_CSX2_IOMUXC_MUX_FUNC, SEMC_CSX_PAD_CTRL);
            break;
        case kSemcCeOutputSelection_MUX_CSX1:
            IOMUXC_SetPinMux(SEMC_CSX3_IOMUXC_MUX_FUNC, 0U);
            IOMUXC_SetPinConfig(SEMC_CSX3_IOMUXC_MUX_FUNC, SEMC_CSX_PAD_CTRL);
            break;
        default:
        case kSemcCeOutputSelection_MUX_CSX0:
            IOMUXC_SetPinMux(SEMC_CSX0_IOMUXC_MUX_FUNC, 0U);
            IOMUXC_SetPinConfig(SEMC_CSX0_IOMUXC_MUX_FUNC, SEMC_CSX_PAD_CTRL);
            break;
    }
}

bool is_boot_pin_asserted(void)
{
    // Boot pin for Flash only target
    return false;
}

//!@brief Get Primary boot device type
uint32_t get_primary_boot_device(void)
{
    uint8_t flash_device = kBootDevice_Invalid;

    if ((FUSE_FORCE_BOOT_FROM_FUSE_VALUE == 1) ||
        (/* (FUSE_FORCE_BOOT_FROM_FUSE_VALUE == 0) && */ (FUSE_BOOT_MODE_PIN_VALUE == kBootMode_Pin_Fuse)) ||
        (/* (FUSE_FORCE_BOOT_FROM_FUSE_VALUE == 0) && */ (FUSE_BOOT_MODE_PIN_VALUE == kBootMode_Pin_Test)) ||
        (/* (FUSE_FORCE_BOOT_FROM_FUSE_VALUE == 0) && */ (FUSE_BOOT_MODE_PIN_VALUE == kBootMode_Pin_InfiniteLoop)))
    {
        switch (FUSE_BOOT_MODE_FUSE_VALUE)
        {
            /* 0 - FlexSPI NOR */
            case kBootMode_Fuse_FlexSPINOR:
                flash_device = kBootDevice_FlexSpiNOR;
                break;
            /* 1 - FlexSPI NAND */
            case kBootMode_Fuse_FlexSPINAND:
                flash_device = kBootDevice_FlexSpiNAND;
                break;
            /* 2 - eMMC */
            case kBootMode_Fuse_eMMC:
                flash_device = kBootDevice_MMC;
                break;
            /* 3 - SD */
            case kBootMode_Fuse_SD:
                flash_device = kBootDevice_SD;
                break;
            /* 4 - SEMC SLC RAW NAND */
            case kBootMode_Fuse_SEMCNAND:
                flash_device = kBootDevice_SemcNAND;
                break;
            default:
                break;
        }
    }
    else
    {
        switch (FUSE_BOOT_MODE_PIN_VALUE)
        {
            /* 2 - eMMC */
            case kBootMode_Pin_eMMC:
                flash_device = kBootDevice_MMC;
                break;
            /* 3 - SD */
            case kBootMode_Pin_SD:
                flash_device = kBootDevice_SD;
                break;
            /* 4 - FlexSPI NOR */
            case kBootMode_Pin_FlexSPINOR:
                flash_device = kBootDevice_FlexSpiNOR;
                break;
            /* 5 - FlexSPI NAND */
            case kBootMode_Pin_FlexSPINAND:
                flash_device = kBootDevice_FlexSpiNAND;
                break;
            default:
                break;
        }
    }
    return flash_device;
}

#if __ICCARM__

size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // __ICCARM__

void update_available_peripherals()
{
    // Flashloader always uses NXP Kboot PID and VID.
}

void init_hardware(void)
{
    // Configure Clocks
    configure_clocks(kClockOption_EnterBootloader);

    // Update memory map according to actual Fuse definitions
    update_memory_map();

    EDGELOCK_Init(SxMU);

    CLOCK_EnableClock(kCLOCK_Usb);
}

void deinit_hardware(void)
{
    CLOCK_DisableClock(kCLOCK_Usb);
}

void update_memory_map(void)
{
    uint32_t m33TcmCfg = FUSE_CM33_TCM_CFG_VALUE;

    if (m33TcmCfg == 0)
    {
        m33TcmCfg = (BLK_CTRL_S_AONMIX->M33_CFG & BLK_CTRL_S_AONMIX_M33_CFG_TCM_SIZE_MASK) >>
                    BLK_CTRL_S_AONMIX_M33_CFG_TCM_SIZE_SHIFT;
    }
    /*
     * Note: Assign 1 to the start address to mark the region invalid.
     */
    uint32_t m33CtcmStartAddr = 1;
    uint32_t m33CtcmEndAddr = 0;
    uint32_t m33StcmStartAddr = 1;
    uint32_t m33StcmEndAddr = 0;
    switch (m33TcmCfg)
    {
        case kCM33_TCM_CFG_128CTCM_128STCM:
        case kCM33_TCM_CFG_Reseved0_128CTCM_128STCM:
            m33CtcmStartAddr = M33_CODE_TCM_START_ADDRESS_IN_M33 + SIZE_128KB;
            m33CtcmEndAddr = M33_CODE_TCM_END_ADDRESS_IN_M33;
            m33StcmStartAddr = M33_SYS_TCM_START_ADDRESS_IN_M33;
            m33StcmEndAddr = M33_SYS_TCM_END_ADDRESS_IN_M33 - SIZE_128KB;
            break;
        case kCM33_TCM_CFG_256CTCM:
            m33CtcmStartAddr = M33_CODE_TCM_START_ADDRESS_IN_M33;
            m33CtcmEndAddr = M33_CODE_TCM_END_ADDRESS_IN_M33;
            m33StcmStartAddr = M33_SYS_TCM_START_ADDRESS_IN_M33;
            m33StcmEndAddr = M33_SYS_TCM_END_ADDRESS_IN_M33 - SIZE_256KB;
            break;
        case kCM33_TCM_CFG_256STCM:
            m33CtcmStartAddr = M33_CODE_TCM_START_ADDRESS_IN_M33 + SIZE_256KB;
            m33CtcmEndAddr = M33_CODE_TCM_END_ADDRESS_IN_M33;
            m33StcmStartAddr = M33_SYS_TCM_START_ADDRESS_IN_M33;
            m33StcmEndAddr = M33_SYS_TCM_END_ADDRESS_IN_M33;
            break;
        default:
            break;
    }

    g_memoryMap[kIndexM33CodeTCM].startAddress = m33CtcmStartAddr;
    g_memoryMap[kIndexM33CodeTCM].endAddress = m33CtcmEndAddr;
    g_memoryMap[kIndexM33CoreTCM_NS].startAddress = address_to_nonsecure_address(m33CtcmStartAddr);
    g_memoryMap[kIndexM33CoreTCM_NS].endAddress = address_to_nonsecure_address(m33CtcmEndAddr);
    g_memoryMap[kIndexM33SysTCM].startAddress = m33StcmStartAddr;
    g_memoryMap[kIndexM33SysTCM].endAddress = m33StcmEndAddr;
    g_memoryMap[kIndexM33SysTCM_NS].startAddress = address_to_nonsecure_address(m33StcmStartAddr);
    g_memoryMap[kIndexM33SysTCM_NS].endAddress = address_to_nonsecure_address(m33StcmEndAddr);

    uint32_t m7TcmCfg = FUSE_CM7_TCM_CFG_VALUE;
    if (m7TcmCfg == 0)
    {
        m7TcmCfg = (BLK_CTRL_S_AONMIX->M7_CFG & BLK_CTRL_S_AONMIX_M7_CFG_TCM_SIZE_MASK) >>
                   BLK_CTRL_S_AONMIX_M7_CFG_TCM_SIZE_SHIFT;
    }

    /*
     * Note: Assign 1 to the start address to mark the region invalid.
     */
    uint32_t m7TcmStartAddr = 1;
    uint32_t m7TcmEndAddr = 0;
    switch (m7TcmCfg)
    {
        case kCM7_TCM_CFG_256ITCM_256DTCM:
        case kCM7_TCM_CFG_Reserved0_256ITCM_256DTCM:
        case kCM7_TCM_CFG_Reserved1_256ITCM_256DTCM:
        case kCM7_TCM_CFG_Reserved2_256ITCM_256DTCM:
            m7TcmStartAddr = M7_TCM_START_ADDRESS_IN_M33 + SIZE_256KB;
            m7TcmEndAddr = M7_TCM_END_ADDRESS_IN_M33 - SIZE_256KB;
            break;
        case kCM7_TCM_CFG_512ITCM:
            m7TcmStartAddr = M7_TCM_START_ADDRESS_IN_M33;
            m7TcmEndAddr = M7_TCM_END_ADDRESS_IN_M33 - SIZE_512KB;
            break;
        case kCM7_TCM_CFG_512DTCM:
            m7TcmStartAddr = M7_TCM_START_ADDRESS_IN_M33 + SIZE_512KB;
            m7TcmEndAddr = M7_TCM_END_ADDRESS_IN_M33;
            break;
        case kCM7_TCM_CFG_128ITCM_384DTCM:
            m7TcmStartAddr = M7_TCM_START_ADDRESS_IN_M33 + SIZE_384KB;
            m7TcmEndAddr = M7_TCM_END_ADDRESS_IN_M33 - SIZE_128KB;
            break;
        case kCM7_TCM_CFG_384ITCM_128DTCM:
            m7TcmStartAddr = M7_TCM_START_ADDRESS_IN_M33 + SIZE_128KB;
            m7TcmEndAddr = M7_TCM_END_ADDRESS_IN_M33 - SIZE_384KB;
            break;
        default:
            break;
    }

    /* M7 TCM is disabled at the same time disabling M7 Core. */
    if (FUSE_M7_DISABLE_VALUE)
    {
        m7TcmEndAddr = m7TcmStartAddr - 1;
    }

    g_memoryMap[kIndexM7TCM].startAddress = m7TcmStartAddr;
    g_memoryMap[kIndexM7TCM].endAddress = m7TcmEndAddr;
    g_memoryMap[kIndexM7TCM_NS].startAddress = address_to_nonsecure_address(m7TcmStartAddr);
    g_memoryMap[kIndexM7TCM_NS].endAddress = address_to_nonsecure_address(m7TcmEndAddr);

    uint32_t ocramStartAddr = OCRAM_START_ADDRESS;
    uint32_t ocramEndAddr = OCRAM_END_ADDRESS;
    if (FUSE_OCRAM1_DISABLE_VALUE)
    {
        ocramStartAddr = OCRAM_START_ADDRESS + SIZE_512KB;
    }
    if (FUSE_OCRAM2_DISABLE_VALUE)
    {
        ocramEndAddr = OCRAM_END_ADDRESS - SIZE_256KB;
    }

    g_memoryMap[kIndexOCRAM].startAddress = ocramStartAddr;
    g_memoryMap[kIndexOCRAM].endAddress = ocramEndAddr;
    g_memoryMap[kIndexOCRAM_NS].startAddress = address_to_nonsecure_address(ocramStartAddr);
    g_memoryMap[kIndexOCRAM_NS].endAddress = address_to_nonsecure_address(ocramEndAddr);
}

//!@brief Get the hab status.
habstatus_option_t get_hab_status()
{
    if (FUSE_XSPI_NOR_ENC_XIP_EN_VALUE)
    {
        return kHabStatus_Close;
    }
    else
    {
        return kHabStatus_Open;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
