/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "boot.h"
#include "fsl_mmu.h"
#include "fsl_cache.h"
#include "flash_partitioning.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitNorFlashPins();

    CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
    CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 1u);     /*400MHz*/

    PRINTF("hello sbl.\r\n");

    (void)sbl_boot_main();

    return 0;
}

void MMU0_IRQHandler(void)
{
    uint32_t status = MMU_GetStatusFlags(MMU0);

    if ((status & kMMU_LUTInvalidFlag) != 0UL)
    {
        /* Access to an invalid item in LUT SRAM. */
        PRINTF("The word of selected pointer of LUT is marked as invalid!\r\n");
    }

    if ((status & kMMU_AddressInvalidFlag) != 0UL)
    {
        /* The AHB access is out of valid address range. */
        PRINTF("Input Address Out Of Range!!!\r\n");
        mmu_ahb_error_record_t ahbErrorRecord;

        MMU_GetAHBErrorRecord(MMU0, &ahbErrorRecord);
        PRINTF("AHB Address: %x\r\n", ahbErrorRecord.failAddr);
        PRINTF("Master Id: %d\r\n", ahbErrorRecord.masterId);
        if (ahbErrorRecord.accessType == kMMU_ReadAccess)
        {
            PRINTF("Access Type: AHB Read\r\n");
        }
        else
        {
            PRINTF("Access Type: AHB Write\r\n");
        }

        if (ahbErrorRecord.errorType == kMMU_LUTInvalidError)
        {
            PRINTF("LUT Invalid Error\r\n");
        }
        else if (ahbErrorRecord.errorType == kMMU_AddrInvalidError)
        {
            PRINTF("Invalid Address Error\r\n");
        }
        else
        {
            PRINTF("Parity Error\r\n");
        }
    }

    if ((status & kMMU_ParityErrorFlag) != 0UL)
    {
        PRINTF("A parity error detected in the access address!\r\n");
    }

    if ((status & kMMU_LockErrorFlag) != 0UL)
    {
        /* A lock error detected. */
        PRINTF("Attemp to update the locked LUT SRAM!");
    }

    if ((status & kMMU_IpsReadErrorFlag) != 0UL)
    {
        /* IPS read error detected. */
        PRINTF("Read Error detected in %d\r\n", MMU_GetIPSReadErrorAddress(MMU0));
    }

    MMU_ClearStatusFlags(MMU0, status);
}

void clean_cache(void)
{
    /* Invalidate CACHE64 */
    CACHE64_CTRL0->CCR |= CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK;
    while ((CACHE64_CTRL0->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
    {
    }
    CACHE64_CTRL0->CCR &= ~(CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);

    CACHE64_CTRL1->CCR |= CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK |
    CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK;
    while ((CACHE64_CTRL1->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
    {
    }
    CACHE64_CTRL1->CCR &= ~(CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK |
    CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);

    /* flush pipeline */
    __DSB();
    __ISB();
}

void SBL_EnableRemap(uint32_t start_addr, uint32_t end_addr, uint32_t off)
{
    /* Enable write access to MMU0 */
    GlikeyWriteEnable(GLIKEY0, 15U);

    /* Configure MMU0 */
    mmu_config_t mmuConfig;

    MMU_GetDefaultConfig(&mmuConfig);

    mmuConfig.unitSize            = kMMU_UnitSize8KB;
    mmuConfig.pageOpt             = kMMU_PageOpt_1Page;
    mmuConfig.invalidAddrMode     = kMMU_InvalidAddrAllowed;
    mmuConfig.baseAddr            = 0x2800UL;
    mmuConfig.maxAddr             = 0x2FFFUL;
    mmuConfig.ptrPhyAddrArray     = NULL;
    mmuConfig.ptrVirtualAddrArray = NULL;
    mmuConfig.enableMMP           = false;

    MMU_Deinit(MMU0);
    MMU_Init(MMU0, &mmuConfig);

    MMU_EnableInterrupts(MMU0, kMMU_AllInterruptsEnable);
    EnableIRQ(MMU0_IRQn);

    uint32_t sbl_addr = BOOT_FLASH_BASE;
    uint32_t sbl_size = BOOT_FLASH_ACT_APP - BOOT_FLASH_BASE;
    uint32_t act_addr = BOOT_FLASH_ACT_APP;
    uint32_t cand_addr = BOOT_FLASH_CAND_APP;
    uint32_t app_size = BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP;

    MMU_SetupRegionMap(MMU0, sbl_addr, sbl_addr, sbl_size);
    MMU_SetupRegionMap(MMU0, cand_addr, cand_addr, app_size);
    MMU_SetupRegionMap(MMU0, act_addr, cand_addr, app_size);
    MMU_EnableMemoryMap(MMU0, true);
    clean_cache();
}
void SBL_DisableRemap(void)
{
    /* Nothing to do here as MMU is disabled during reset */
}

void SBL_DisablePeripherals(void)
{
    DbgConsole_Deinit();
}

status_t CRYPTO_InitHardware(void)
{
    return kStatus_Success;
}
