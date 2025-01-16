/*
* Copyright 2024 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include "flash_opts.h"
#include "fsl_debug_console.h"
#include "timer.h"
#include "fsl_xspi.h"

#if defined(__ARMCC_VERSION)
__attribute__((weak))
size_t __aeabi_read_tp(void)
{
  return 0;
}
#endif

#if 1
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
   .interfaceSettings.hyperBusSettings.pageSize = 1024,
   .CSHoldTime = 2,
   .CSSetupTime = 2,
   .sampleClkConfig.sampleClkSource = kXSPI_SampleClkFromExternalDQS,
   .sampleClkConfig.enableDQSLatency = false,
   .sampleClkConfig.dllConfig.dllMode = kXSPI_AutoUpdateMode,
   .sampleClkConfig.dllConfig.useRefValue = true,
   .sampleClkConfig.dllConfig.enableCdl8 = true,
   .addrMode = kXSPI_Device4ByteAddressable,
   .columnAddrWidth = 3U,
   .enableCASInterleaving = false,
   .deviceSize[0] = DRAM_SIZE,
   .deviceSize[1] = DRAM_SIZE,
   .ptrDeviceRegInfo = NULL,
   .ptrDeviceDdrConfig = &deviceDdrConfig,
};

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
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x08,
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
                                                                kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x08),
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
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x08,
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
                                                                    kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x08),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),
#endif
   /* reset */
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_RESET] =
        XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xFF, kXSPI_Command_DDR, kXSPI_8PAD, 0xFF),
    [5 * HYPERRAM_CMD_LUT_SEQ_IDX_RESET + 1] =
        XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_8PAD, 0x0, kXSPI_Command_STOP, kXSPI_8PAD, 0x0),
};
#endif
extern void xspi_hyper_ram_init(XSPI_Type *base);
extern void xspi_hyper_ram_ahbcommand_write_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern void xspi_hyper_ram_ahbcommand_read_data(XSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length);
extern status_t xspi_hyper_ram_ipcommand_write_data(XSPI_Type *base,
                                                    uint32_t address,
                                                    uint32_t *buffer,
                                                    uint32_t length);
extern status_t xspi_hyper_ram_ipcommand_read_data(XSPI_Type *base,
                                                   uint32_t address,
                                                   uint32_t *buffer,
                                                   uint32_t length);


status_t FlashInit(FlashConfig *config){
  /* XSPI init */
  	xspi_hyper_ram_init(EXAMPLE_XSPI);
  	PRINTF("XSPI Psram Enabled!\r\n");
  	return kStatus_Success;
}

status_t FlashErase(FlashConfig *config, uint32_t start, uint32_t length){
	 //PRINTF("PSram no need erase - successfully. \r\n");
 	 PRINTF("FLASH 0x%x Erased, ", EXAMPLE_XSPI_AMBA_BASE+start);
         return kStatus_Success;
}

status_t FlashProgram(FlashConfig *config, uint32_t start, uint32_t *src, uint32_t length){
	xspi_hyper_ram_ahbcommand_write_data(EXAMPLE_XSPI, start, src, length);
	PRINTF("Program at %x: %d bytes  ==\r\n", EXAMPLE_XSPI_AMBA_BASE+start, length);
        return kStatus_Success ;
}
