/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_flexspi.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
* Definitions
******************************************************************************/
#define EXAMPLE_FLEXSPI FLEXSPI
#define FLASH_SIZE 0x2000
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define HYPERRAM_CMD_LUT_SEQ_IDX_READDATA 0
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERRAM_CMD_LUT_SEQ_IDX_READREG 2
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG 3

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk = 332000000, /* 332MHZ SPI serial clock */
    .isSck2Enabled = false,
    .flashSize = FLASH_SIZE,
    .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval = 2,
    .CSHoldTime = 0,
    .CSSetupTime = 4,
    .dataValidTime = 1,
    .columnspace = 3,
    .enableWordAddress = true,
    .AWRSeqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA,
    .AWRSeqNumber = 1,
    .ARDSeqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_READDATA,
    .ARDSeqNumber = 1,
    .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
    .enableWriteMask = true,
};

uint32_t customLUT[20] = {
        /* Read Data */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READDATA + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Write Data */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA + 2] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Read Register */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xE0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_READREG + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* Write Register */
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x60, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x06),
        [4 * HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG + 2] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),
};

static uint8_t s_hyper_ram_write_buffer[1024];
static uint8_t s_hyper_ram_read_buffer[1024];
#define DRAM_SIZE 0x80000U
/*******************************************************************************
 * Code
 ******************************************************************************/


status_t flexspi_hyper_ram_ipcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA;
    flashXfer.data = buffer;
    flashXfer.dataSize = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

void flexspi_hyper_ram_ahbcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_FLEXSPI_AMBA_BASE + address);
    memcpy(startAddr, buffer, length);

    /* Delay some time for tx buffer data finished. */
    uint32_t i = 1000;
    while (i--)
    {
        __NOP();
    }
}

status_t flexspi_hyper_ram_ipcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_READDATA;
    flashXfer.data = buffer;
    flashXfer.dataSize = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

void flexspi_hyper_ram_ahbcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_FLEXSPI_AMBA_BASE + address);
    memcpy(buffer, startAddr, length);
}

status_t flexspi_hyper_ram_get_id(FLEXSPI_Type *base, uint32_t *vendorId)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    uint32_t id;

    /* Write data */
    flashXfer.deviceAddress = 0x0U;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = HYPERRAM_CMD_LUT_SEQ_IDX_READREG;
    flashXfer.data = &id;
    flashXfer.dataSize = 4;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    *vendorId = id & 0xffffU;

    return status;
}

static volatile uint32_t g_msCount = 0;
void SysTick_Handler(void)
{
    g_msCount++;
}

int main(void)
{
    uint32_t i = 0;
    flexspi_config_t config;
    status_t status;
    uint32_t vendorID = 0;
    uint32_t t1;
    uint32_t t2;
    uint32_t coreclk = 0;
    uint32_t ahbclk = 0;
    uint32_t hyperbusclk = 0;
    uint32_t time;
    uint32_t tmp;
    uint32_t g_ms;
    uint32_t dsize = 32;
    uint8_t *ahb_addr = (uint8_t *)EXAMPLE_FLEXSPI_AMBA_BASE;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set flexspi root clock to 332MHZ. */
    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};

    CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332MHZ. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0);   /* Divide the clock by 1 to get flexspi root clock 332MHZ */

    /* Disable DCache */
    SCB_DisableDCache();

    coreclk = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    ahbclk = CLOCK_GetFreq(kCLOCK_IpgClk);
    hyperbusclk = CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk);
    PRINTF("coreclk: %d; ahbclk: %d; hyperbusclk: %d\r\n", coreclk, ahbclk, hyperbusclk);
   
    PRINTF("FlexSPI HyperRAM example started!\r\n");

    /* Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /* Init FLEXSPI. */
    config.rxSampleClock = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
    config.enableSckBDiffOpt = true;
    config.enableCombination = true;
    config.ahbConfig.enableAHBPrefetch = true;
    config.ahbConfig.enableAHBBufferable = true;
    config.ahbConfig.enableAHBCachable = true;

    FLEXSPI_Init(EXAMPLE_FLEXSPI, &config);

    /* Configure RAM settings according to serial RAM feature. */
    FLEXSPI_SetFlashConfig(EXAMPLE_FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(EXAMPLE_FLEXSPI, 0, customLUT, ARRAY_SIZE(customLUT));

    /* Do software reset. */
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);
#if 1
    /* Get vendor ID. */
    status = flexspi_hyper_ram_get_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", (uint16_t)vendorID);
    for (i = 0; i < sizeof(s_hyper_ram_write_buffer); i++)
    {
        s_hyper_ram_write_buffer[i] = i;
    }

    for (i = 0; i < DRAM_SIZE; i += 2048)
    {
        flexspi_hyper_ram_ipcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_write_buffer,
                                               sizeof(s_hyper_ram_write_buffer));
        flexspi_hyper_ram_ipcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_read_buffer,
                                              sizeof(s_hyper_ram_read_buffer));

        if (memcmp(s_hyper_ram_read_buffer, s_hyper_ram_write_buffer, sizeof(s_hyper_ram_write_buffer)) != 0)
        {
            PRINTF("IP Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 2047);
            return -1;
        }
    }

    PRINTF("IP Command Read/Write data succeed at all address range !\r\n");

    for (i = sizeof(s_hyper_ram_write_buffer); i > 0; i--)
    {
        s_hyper_ram_write_buffer[i] = i;
    }
    memset(s_hyper_ram_read_buffer, 0, sizeof(s_hyper_ram_read_buffer));

    for (i = 0; i < DRAM_SIZE; i += 2048)
    {
        flexspi_hyper_ram_ahbcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_write_buffer,
                                                sizeof(s_hyper_ram_write_buffer));
        flexspi_hyper_ram_ahbcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_read_buffer,
                                               sizeof(s_hyper_ram_read_buffer));

        if (memcmp(s_hyper_ram_read_buffer, s_hyper_ram_write_buffer, sizeof(s_hyper_ram_write_buffer)) != 0)
        {
            PRINTF("AHB Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 2047);
            return -1;
        }
    }

    PRINTF("AHB Command Read/Write data succeed at all address range !\r\n");
#endif
 
    g_msCount = 0;
    /* HyperRAM write test */
    SysTick_Config(coreclk / 1000); /* 1ms for one interrupt*/
  
    t1 = SysTick->VAL;
    for (i = 0; i < dsize; i += 4)
    {
        *(uint32_t *)(ahb_addr + i) = 0x5a5a5a5a;
    }
    __asm("DSB");
    t2 = SysTick->VAL;
    g_ms = g_msCount;
    time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
    PRINTF("##HyperRAM AHB write perf##t1: %d; t2: %d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
           t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
#if 1
    g_msCount = 0;
    /* HyperRAM read test */
    SysTick_Config(coreclk / 1000); /* 1ms for one interrupt*/
  
    t1 = SysTick->VAL;
    for (i = 0; i < dsize; i += 4)
    {
        tmp = *(uint32_t *)(ahb_addr + i);
    }
    __asm("DSB");
    t2 = SysTick->VAL;
    g_ms = g_msCount;
    time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
    PRINTF("##HyperRAM AHB read perf###t1: %d; t2: %d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
           t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
#endif
    for (i = 0; i < 16; i += 4)
    {
      PRINTF("0x%x: 0x%08x 0x%08x 0x%08x 0x%08x\r\n", ahb_addr + i, *(uint32_t *)(ahb_addr + i), *(uint32_t *)(ahb_addr + i + 4)
                                                          , *(uint32_t *)(ahb_addr + i + 8), *(uint32_t *)(ahb_addr + i + 12));
    }
    while (1)
    {
    }
}
