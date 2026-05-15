/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_romapi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_LEN FLASH_PAGE_SIZE

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void error_trap(void);
void app_finalize(void);
status_t FLEXSPI_NorFlash_GetVendorID(uint32_t instance, uint32_t *vendorID);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define FLASH_DUMMY_CYCLES 0x06
/*! @brief FLEXSPI NOR flash driver Structure */
static flexspi_nor_config_t norConfig;
/*! @brief Buffer for program */
static uint8_t s_buffer[BUFFER_LEN];
/*! @brief Buffer for readback */
static uint8_t s_buffer_rbc[BUFFER_LEN];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * @brief Gets called when an error occurs.
 *
 * @details Print error message and trap forever.
 */
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLEXSPI NOR ERROR! ----");
    while (1)
    {
    }
}

/*
 * @brief Gets called when the app is complete.
 *
 * @details Print finshed message and trap forever.
 */
void app_finalize(void)
{
    /* Print finished message. */
    PRINTF("\r\n End of FLEXSPI NOR Example! \r\n");
    while (1)
    {
    }
}

status_t FLEXSPI_NorFlash_GetVendorID(uint32_t instance, uint32_t *vendorID)
{
    uint32_t lut_seq[4];
    memset(lut_seq, 0, sizeof(lut_seq));
    // Read manufacturer ID
    lut_seq[0] = FSL_ROM_FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x9F, READ_SDR, FLEXSPI_1PAD, 4);
    ROM_FLEXSPI_NorFlash_UpdateLut(instance, NOR_CMD_LUT_SEQ_IDX_READID, (const uint32_t *)lut_seq, 1U);

    flexspi_xfer_t xfer;
    xfer.operation            = kFLEXSPIOperation_Read;
    xfer.seqId                = NOR_CMD_LUT_SEQ_IDX_READID;
    xfer.seqNum               = 1U;
    xfer.baseAddress          = 0U;
    xfer.isParallelModeEnable = false;
    xfer.rxBuffer             = vendorID;
    xfer.rxSize               = 1U;

    uint32_t status = ROM_FLEXSPI_NorFlash_CommandXfer(instance, &xfer);
    if (*vendorID != kSerialFlash_Winbond_ManufacturerID)
    {
        status = kStatus_ROM_FLEXSPINOR_Flash_NotFound;
        return status;
    }

    return status;
}


#define FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                              \
    (FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | FLEXSPI_LUT_OPERAND1(op1) | \
     FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))
/* These settings are related to board design for external QSPI flash memory, please adjust it accordingly based on your design */
status_t FLEXSPI_NorFlash_GetConfig(uint32_t instance,
                                           flexspi_nor_config_t *config)
{  
    config->memConfig.tag = 0x42464346UL;
    config->memConfig.version = 0x56010400UL;
    config->memConfig.readSampleClkSrc = kFLEXSPIReadSampleClk_LoopbackFromDqsPad;
    config->memConfig.csHoldTime = 3;
    config->memConfig.csSetupTime = 3;
    config->memConfig.controllerMiscOption = 0x10;
    config->memConfig.deviceType = kFLEXSPIDeviceType_SerialNOR;
    config->memConfig.sflashPadType = kSerialFlash_4Pads;
    config->memConfig.serialClkFreq = kFLEXSPISerialClk_100MHz;
    config->memConfig.sflashA1Size = 16u * 1024u * 1024u,
    config->memConfig.configModeType[0] = kDeviceConfigCmdType_Generic,
 
    config->memConfig.lookupTable[0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xEB, RADDR_SDR, FLEXSPI_4PAD, 0x18);
    config->memConfig.lookupTable[1] = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, FLASH_DUMMY_CYCLES, READ_SDR, FLEXSPI_4PAD, 0x04);
    config->memConfig.lookupTable[4 * 1 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05, READ_SDR, FLEXSPI_1PAD, 0x04);
    config->memConfig.lookupTable[4 * 3 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP, FLEXSPI_1PAD, 0x0);    
    config->memConfig.lookupTable[4 * 5 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x20, RADDR_SDR, FLEXSPI_1PAD, 0x18);    
    config->memConfig.lookupTable[4 * 8 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xD8, RADDR_SDR, FLEXSPI_1PAD, 0x18);
    config->memConfig.lookupTable[4 * 9 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x02, RADDR_SDR, FLEXSPI_1PAD, 0x18);
    config->memConfig.lookupTable[4 * 9 + 1] = FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_1PAD, 0x04, STOP, FLEXSPI_1PAD, 0x0);    
    config->memConfig.lookupTable[4 * 11 + 0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x60, STOP, FLEXSPI_1PAD, 0x0);       

    config->pageSize = 256u;
    config->sectorSize = 4u * 1024u;
    config->blockSize = 64u * 1024u;
    config->ipcmdSerialClkFreq = 0x1;
    config->isUniformBlockSize = false;
 
    return kStatus_Success;
}

int main(void)
{
    status_t status;
    uint32_t i        = 0U;
    uint32_t vendorID = 0U;
    uint32_t serialNorAddress;        /* Address of the serial nor device location */
    uint32_t FlexSPISerialNorAddress; /* Address of the serial nor device in FLEXSPI memory */
    uint32_t serialNorTotalSize;
    uint32_t serialNorSectorSize;
    uint32_t serialNorPageSize;

    BOARD_InitHardware();

    PRINTF("\r\n FLEXSPI NOR example started!\r\n");

    ROM_API_Init();

    /* Clean up FLEXSPI NOR flash driver Structure */
    memset(&norConfig, 0U, sizeof(flexspi_nor_config_t));

#if (__CORTEX_M == 7U)
    /* Disable I cache */
    SCB_DisableICache();
#endif

    /* Setup FLEXSPI NOR Configuration Block */
    status = FLEXSPI_NorFlash_GetConfig(FlexSpiInstance, &norConfig);
    if (status == kStatus_Success)
    {
        PRINTF("\r\n Successfully get FLEXSPI NOR configuration block\r\n ");
    }
    else
    {
        PRINTF("\r\n Get FLEXSPI NOR configuration block failure!\r\n");
        error_trap();
    }

    /* Initializes the FLEXSPI module for the other FLEXSPI APIs */
    status = ROM_FLEXSPI_NorFlash_Init(FlexSpiInstance, &norConfig);
    if (status == kStatus_Success)
    {
        PRINTF("\r\n Successfully init FLEXSPI serial NOR flash\r\n ");
    }
    else
    {
        PRINTF("\r\n Erase sector failure !\r\n");
        error_trap();
    }

    /* Perform software reset after initializing flexspi module */
    ROM_FLEXSPI_NorFlash_ClearCache(FlexSpiInstance);

    status = FLEXSPI_NorFlash_GetVendorID(FlexSpiInstance, &vendorID);
    if (status == kStatus_Success)
    {
        PRINTF("\r\n Serial NOR flash has been found successfully\r\n ");
        PRINTF("Vendor ID: 0x%x\r\n", vendorID);
    }
    else
    {
        PRINTF("\r\n Serial NOR flash can not be found!\r\n");
        error_trap();
    }

    serialNorTotalSize  = norConfig.memConfig.sflashA1Size;
    serialNorSectorSize = norConfig.sectorSize;
    serialNorPageSize   = norConfig.pageSize;

#if (__CORTEX_M == 7U)
    /* Enable I cache */
    SCB_EnableICache();
#endif

    /* Print serial NOR flash information */
    PRINTF("\r\n Serial NOR flash Information: ");
    PRINTF("\r\n Total program flash size:\t%d KB, Hex: (0x%x)", (serialNorTotalSize / 1024U), serialNorTotalSize);
    PRINTF("\r\n Program flash sector size:\t%d KB, Hex: (0x%x) ", (serialNorSectorSize / 1024U), serialNorSectorSize);
    PRINTF("\r\n Program flash page size:\t%d B, Hex: (0x%x)\r\n", serialNorPageSize, serialNorPageSize);

/*
 * SECTOR_INDEX_FROM_END = 1 means the last sector,
 * SECTOR_INDEX_FROM_END = 2 means (the last sector - 1) ...
 */
#ifndef SECTOR_INDEX_FROM_END
#define SECTOR_INDEX_FROM_END 1U
#endif
    /* Erase a sector from target device dest address */
    serialNorAddress        = serialNorTotalSize - (SECTOR_INDEX_FROM_END * serialNorSectorSize);
    FlexSPISerialNorAddress = EXAMPLE_FLEXSPI_AMBA_BASE + serialNorAddress;

    /* Erase one sector. */
    PRINTF("\r\n Erasing serial NOR flash over FLEXSPI");
    status = ROM_FLEXSPI_NorFlash_Erase(FlexSpiInstance, &norConfig, serialNorAddress, serialNorSectorSize);
    if (status == kStatus_Success)
    {
        /* Print message for user. */
        PRINTF("\r\n Successfully erased one sector of NOR flash device 0x%x -> 0x%x\r\n", serialNorAddress,
               (serialNorAddress + serialNorSectorSize));
    }
    else
    {
        PRINTF("\r\n Erase sector failure!\r\n");
        error_trap();
    }

    PRINTF("\r\n Program a buffer to a page of NOR flash");
    /* Prepare user buffer. */
    for (i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer[i] = i;
    }

    /* Program user buffer into FLEXSPI NOR flash */
    status =
        ROM_FLEXSPI_NorFlash_ProgramPage(FlexSpiInstance, &norConfig, serialNorAddress, (const uint32_t *)s_buffer);
    if (status != kStatus_Success)
    {
        PRINTF("\r\n Page program failure!\r\n");
        error_trap();
    }

    DCACHE_InvalidateByRange(FlexSPISerialNorAddress, sizeof(s_buffer_rbc));
    /* Verify programming by reading back from FLEXSPI memory directly */
    memcpy(s_buffer_rbc, (void *)(FlexSPISerialNorAddress), sizeof(s_buffer_rbc));
    if (memcmp(s_buffer_rbc, s_buffer, sizeof(s_buffer)) == 0)
    {
        PRINTF("\r\n Successfully programmed and verified location FLEXSPI memory 0x%x -> 0x%x \r\n",
               (FlexSPISerialNorAddress), (FlexSPISerialNorAddress + sizeof(s_buffer)));
    }
    else
    {
        PRINTF("\r\n Program data -  read out data value incorrect!\r\n ");
        error_trap();
    }

    /* Erase the context we have progeammed before*/
    status = ROM_FLEXSPI_NorFlash_Erase(FlexSpiInstance, &norConfig, serialNorAddress, serialNorSectorSize);

    app_finalize();

    return 0;
}
