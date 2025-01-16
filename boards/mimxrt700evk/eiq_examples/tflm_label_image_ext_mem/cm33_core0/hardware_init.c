/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "board_init.h"
#include "fsl_debug_console.h"
#include "fsl_edma.h"
#include "fsl_edma_soc.h"
#include "fsl_xspi.h"
#include "fsl_xspi_edma.h"
#include "pin_mux.h"
#include "pmic_support.h"

extern void xspi_callback(XSPI_Type *base, xspi_edma_handle_t *handle, status_t status, void *userData);
extern status_t xspi_nor_enable_octal_mode(XSPI_Type *base);
extern void xspi_nor_flash_init(XSPI_Type *base);

xspi_device_ddr_config_t flashDDrConfig =
{
    .ddrDataAlignedClk = kXSPI_DDRDataAlignedWith2xInternalRefClk,
    .enableDdr = true,
    .enableByteSwapInOctalMode = false,
};
/*!
 * @brief Configuration for MX25UW51345GXD100.
 */
xspi_device_config_t deviceConfig =
{
    .xspiRootClk = 400000000,  /*!< 400MHz */
    .enableCknPad = false,  /*!< Do not support differential clock */
    .deviceInterface = kXSPI_StrandardExtendedSPI,  /*!< Support Single IO and Octal IO */
    .interfaceSettings.strandardExtendedSPISettings.pageSize = FLASH_PAGE_SIZE,   /*!< 256 byte page buffer. */
    .CSHoldTime = 3,
    .CSSetupTime = 3,
    .sampleClkConfig.sampleClkSource = kXSPI_SampleClkFromExternalDQS,  /*!< Device support Data strobe signal.  */
    .sampleClkConfig.enableDQSLatency = false,
    .sampleClkConfig.dllConfig.dllMode = kXSPI_AutoUpdateMode,
    .sampleClkConfig.dllConfig.useRefValue = true,
    .sampleClkConfig.dllConfig.enableCdl8 = false,
    .ptrDeviceDdrConfig = &flashDDrConfig,
    .addrMode = kXSPI_DeviceByteAddressable,
    .columnAddrWidth = 0U,
    .enableCASInterleaving = false,
    .deviceSize[0] = FLASH_SIZE,
    .deviceSize[1] = FLASH_SIZE,   /*!< Single DIE flash, so deviceSize1 should equal to deviceSize0. */
    .ptrDeviceRegInfo = NULL,      /*!< Not used in this example. */
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
    /*Read*/
    [5 * NOR_CMD_LUT_SEQ_IDX_READ] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xEE, kXSPI_Command_DDR, kXSPI_8PAD, 0x11),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x12),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x2, kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x4),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ + 3] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_8PAD, 0x0, 0, 0, 0),

    /*Read status SPI*/
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x05, kXSPI_Command_READ_SDR, kXSPI_1PAD, 0x04),

    /* Read Status OPI */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x05, kXSPI_Command_DDR, kXSPI_8PAD, 0xFA),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x12),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x2, kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x4),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI + 3] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_8PAD, 0x0, 0, 0, 0),

    /*Write enable*/
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x06, kXSPI_Command_STOP, kXSPI_1PAD, 0x04),

    /* Write Enable - OPI */
    [5 * NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x06, kXSPI_Command_DDR, kXSPI_8PAD, 0xF9),

    /* Read ID */
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 0] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x9F, kXSPI_Command_DDR, kXSPI_8PAD, 0x60),
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 1] = XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x04), /*address is 0x00,0x00,0x00,0x00*/
    [5 * NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x04, kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

    /* Erase Sector */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x21, kXSPI_Command_DDR, kXSPI_8PAD, 0xDE),
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_STOP, kXSPI_8PAD, 0x0),

    /* Enable OPI DDR mode */
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x72, kXSPI_Command_SDR, kXSPI_1PAD, 0x00),
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x00, kXSPI_Command_SDR, kXSPI_1PAD, 0x00),
    [5 * NOR_CMD_LUT_SEQ_IDX_ENTER_OPI + 2] =
        XSPI_LUT_SEQ(kXSPI_Command_SDR, kXSPI_1PAD, 0x00, kXSPI_Command_WRITE_SDR,  kXSPI_1PAD, 0x01),

    /* Page program */
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x12, kXSPI_Command_DDR, kXSPI_8PAD, 0xED),
    [5 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20, kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x4),

    /* Erase Chip */
    [5 * NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x60, kXSPI_Command_DDR, kXSPI_8PAD, 0x9F),
};

edma_handle_t dmaTxHandle;
edma_handle_t dmaRxHandle;
xspi_edma_handle_t xspiHandle;

extern void XSPI_TX_DMA_ISR(void);
extern void XSPI_RX_DMA_ISR(void);
#if 0
void cleanCache(void)
{
   PRINTF("%s\r\n", __func__);
   XCACHE_CleanCache(XCACHE0);
}

void invalidateCache(void)
{
   XCACHE_CleanCache(XCACHE0);
   XCACHE_InvalidateCache(XCACHE0);
}
#endif
void BOARD_Init()
{
    edma_config_t userConfig;

    BOARD_InitPins();
    
    BOARD_InitPmicPins();
    BOARD_InitPmic();
    BOARD_SetPmicVdd2Voltage(1100000U); /* 1.1v for 325MHz clock. */

    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    BOARD_InitAHBSC();
    BOARD_ConfigMPU();

    CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
    CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 1u);     /*400MHz*/

    POWER_DisablePD(kPDRUNCFG_APD_NPU);
    POWER_DisablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();

    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);
    EDMA_EnableRequest(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_REQUEST_SOURCE);
    EDMA_EnableRequest(EXAMPLE_XSPI_DMA, XSPI_RX_DMA_REQUEST_SOURCE);

#if defined(ENABLE_RAM_VECTOR_TABLE)
    InstallIRQHandler(XSPI_TX_DMA_IRQn, (uint32_t)XSPI_TX_DMA_ISR);
    InstallIRQHandler(XSPI_RX_DMA_IRQn, (uint32_t)XSPI_RX_DMA_ISR);
#endif

    EDMA_GetDefaultConfig(&userConfig);
    /* To unlock TBDR, configure the master ID of EDMA to be the same as the cm33_core0 */
    userConfig.enableMasterIdReplication = true;
    EDMA_Init(EXAMPLE_XSPI_DMA, &userConfig);
    /* To unlock TBDR, configure the master ID of EDMA to be the same as the cm33_core0 */
    EDMA_EnableChannelMasterIDReplication(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL, true);
    /* Create the EDMA channel handles */
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_XSPI_DMA, XSPI_RX_DMA_CHANNEL);

    /* Create handle for xspi. */
    XSPI_TransferCreateHandleEDMA(EXAMPLE_XSPI, &xspiHandle, xspi_callback, NULL, &dmaTxHandle, &dmaRxHandle);
    /* XSPI init */
    xspi_nor_flash_init(EXAMPLE_XSPI);
    xspi_nor_enable_octal_mode(EXAMPLE_XSPI);
}
