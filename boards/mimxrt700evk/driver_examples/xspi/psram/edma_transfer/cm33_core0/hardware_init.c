/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_xspi.h"
#include "fsl_edma_soc.h"
/*${header:end}*/

/*${variable:start}*/
xspi_device_ddr_config_t deviceDdrConfig = {
    .ddrDataAlignedClk = kXSPI_DDRDataAlignedWith2xInternalRefClk,
    .enableByteSwapInOctalMode = false,
    .enableDdr = true,
};

xspi_device_config_t deviceConfig = {
#if defined(DEMO_USE_XSPI2) && DEMO_USE_XSPI2
   .xspiRootClk = 500000000, /*!< 500MHz for W958D6NBKX4I. */
#elif defined(DEMO_USE_XSPI1) && DEMO_USE_XSPI1
   .xspiRootClk = 400000000, /*!< 400MHz for W958D6NBKX5I. */
#endif
   .enableCknPad = true,
   .deviceInterface = kXSPI_HyperBus,
   .interfaceSettings.hyperBusSettings.x16Mode = kXSPI_x16ModeEnabledOnlyData, /*!< Only Data use x16 mode. */
#if defined(XSPI_ENABLE_VARIABLE_LATENCY) && XSPI_ENABLE_VARIABLE_LATENCY
   .interfaceSettings.hyperBusSettings.enableVariableLatency = true, /*!< Enable additional latency to increase performance. */
#else
   .interfaceSettings.hyperBusSettings.enableVariableLatency = false, /*!< Enable additional latency to increase performance. */
#endif
   .interfaceSettings.hyperBusSettings.forceBit10To1 = false,
	 .interfaceSettings.hyperBusSettings.pageSize = 1024U,
   .CSHoldTime = 2,
   .CSSetupTime = 2,
   .sampleClkConfig.sampleClkSource = kXSPI_SampleClkFromExternalDQS,
   .sampleClkConfig.enableDQSLatency = false,
   .sampleClkConfig.dllConfig.dllMode = kXSPI_AutoUpdateMode,
   .sampleClkConfig.dllConfig.useRefValue = true,
   .sampleClkConfig.dllConfig.enableCdl8 = false,
   .addrMode = kXSPI_Device4ByteAddressable,
   .columnAddrWidth = 3U,
   .enableCASInterleaving = false,
   .deviceSize[0] = DRAM_SIZE,
   .deviceSize[1] = DRAM_SIZE,
   .ptrDeviceRegInfo = NULL,
   .ptrDeviceDdrConfig = &deviceDdrConfig,
};

/* 
 * Errata ERR052528: Limitation on LUT-Data Size < 8byte in xspi.
 * Description: Read command including RDSR command can't work if LUT data size in read status is less than 8.
 * Workaround: Use LUT data size of minimum 8 byte for read commands including RDSR.
 */
const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
#if defined(XSPI_ENABLE_VARIABLE_LATENCY) && (XSPI_ENABLE_VARIABLE_LATENCY)
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xA0, 
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10, 
                                                                    kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 6), /* Dummy cycle: 2 * 6 + 2 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08, 
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* Memory Write */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 0] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x20,
                                                                    kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                    kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 6), /* Dummy cycle: 2 * 6 + 2 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x04,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0X0),

        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xE0,
                                                            kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x6), /* Dummy cycle: 2 * 6 + 2 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08,
                                                                kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* Register write */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x60,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x02),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0,
                                                                kXSPI_Command_STOP, kXSPI_1PAD, 0x0),
#else
        /* Memory Read */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xA0,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 13),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* Memory Write */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x20,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 13), /* Dummy cycle: 13 + 1 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x04,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0X0),

        /* Register Read */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xE0,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                    kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 13),  /* Dummy cycle: 13 + 1 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* Register write */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x60,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                    kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x02),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),
#endif
        
   /* reset */
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_RESET] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xFF, kXSPI_Command_DDR, kXSPI_8PAD, 0xFF),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_RESET + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_8PAD, 0x0, kXSPI_Command_STOP, kXSPI_8PAD, 0x0),
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();

#if defined(DEMO_USE_XSPI2) && DEMO_USE_XSPI2
    BOARD_InitPsRamPins_Xspi2();
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_XSPI2);
    CLOCK_SetClkDiv(kCLOCK_DivXspi2Clk, 1u);     /*500MHz*/
#elif defined(DEMO_USE_XSPI1) && DEMO_USE_XSPI1
    BOARD_InitPsRamPins_Xspi1();
    CLOCK_AttachClk(kAUDIO_PLL_PFD1_to_XSPI1);
    CLOCK_SetClkDiv(kCLOCK_DivXspi1Clk, 1u);     /*400MHz*/
#endif

    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);

    EDMA_EnableRequest(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_REQUEST_SOURCE);
    EDMA_EnableRequest(EXAMPLE_XSPI_DMA, XSPI_RX_DMA_REQUEST_SOURCE);
}
/*${function:end}*/
