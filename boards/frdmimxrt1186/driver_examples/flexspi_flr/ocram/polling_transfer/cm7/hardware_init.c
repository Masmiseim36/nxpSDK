/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "app.h"
#include "fsl_flexspi.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk       = 0,
    .flashSize            = MEMORY_SIZE,
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 2,
    .columnspace          = 0,
    .enableWordAddress    = 0,
    .AWRSeqIndex          = FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(EXAMPLE_FLEXSPI_SLV_MODE),
    .AWRSeqNumber         = 1,
    .ARDSeqIndex          = FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(EXAMPLE_FLEXSPI_SLV_MODE),
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
    /* Read status register - SDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_SDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, FOLLOWER_CMD_READ_STATUS, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_SDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Read memory -SDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, FOLLOWER_CMD_READ_MEMORY, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx4) + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x14, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),

    /* Send mailbox - SDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_SDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, FOLLOWER_CMD_SEND_MAILBOX, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_SDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Write Memory - SDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, FOLLOWER_CMD_WRITE_MEMORY, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Read status register - SDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_SDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, FOLLOWER_CMD_READ_STATUS, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_SDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Read memory -SDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, FOLLOWER_CMD_READ_MEMORY, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Send mailbox - SDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_SDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, FOLLOWER_CMD_SEND_MAILBOX, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_SDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Write Memory - SDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, FOLLOWER_CMD_WRITE_MEMORY, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Read status register - DDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_4PAD, FOLLOWER_CMD_READ_STATUS, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx4) + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_4PAD, 0x14, kFLEXSPI_Command_READ_DDR, kFLEXSPI_4PAD, 0x04),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx4) + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0, 0, 0, 0),

    /* Read memory -DDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_4PAD, FOLLOWER_CMD_READ_MEMORY, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx4) + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_4PAD, 0x14, kFLEXSPI_Command_READ_DDR, kFLEXSPI_4PAD, 0x04),

    /* Send mailbox - DDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_DDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_4PAD, FOLLOWER_CMD_SEND_MAILBOX, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_DDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Write Memory - DDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_4PAD, FOLLOWER_CMD_WRITE_MEMORY, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Read status register - DDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, FOLLOWER_CMD_READ_STATUS, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Read memory -DDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, FOLLOWER_CMD_READ_MEMORY, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Send mailbox - DDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_DDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, FOLLOWER_CMD_SEND_MAILBOX, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_DDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Write Memory - DDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, FOLLOWER_CMD_WRITE_MEMORY, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
};

uint32_t FLEXSPI_GetRootClockFreqHz(clock_root_mux_source_t clkName)
{
    uint32_t clock = 0;

    switch (clkName)
    {
        case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc24M:
            clock = CLOCK_GetFreq(kCLOCK_OscRc24M) / (uint32_t)EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV;
            break;
        case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc400M:
            clock = CLOCK_GetFreq(kCLOCK_OscRc400M) / (uint32_t)EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV;
            break;
        case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0:
            clock = CLOCK_GetFreq(kCLOCK_SysPll3Pfd0) / (uint32_t)EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV;
            break;
        case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0:
            clock = CLOCK_GetFreq(kCLOCK_SysPll2Pfd0) / (uint32_t)EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV;
            break;
        default:
            clock = 0;
            break;
    }

    return clock;
}

void FLEXSPI_SLV_ClkRootFrq(flexspi_slv_clock_freq_t clock_freq)
{
    clock_root_config_t rootCfg = {0};

    switch (clock_freq)
    {
        case kFLEXSPI_SLV_RootClock_50M:
            /* Configure FLEXSPI_SLV using OSC_RC_400M */
            rootCfg.mux = (uint8_t)kCLOCK_FLEXSPI_SLV_ClockRoot_MuxOscRc400M;
            rootCfg.div = 8;
            break;

        case kFLEXSPI_SLV_RootClock_66M:
            /* Configure FLEXSPI_SLV using SYS_PLL2_CLK */
            rootCfg.mux = (uint8_t)kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll2Out;
            rootCfg.div = 8;
            break;

        case kFLEXSPI_SLV_RootClock_80M:
            /* Configure FLEXSPI_SLV using OSC_RC_400M */
            rootCfg.mux = (uint8_t)kCLOCK_FLEXSPI_SLV_ClockRoot_MuxOscRc400M;
            rootCfg.div = 5;
            break;

        case kFLEXSPI_SLV_RootClock_100M:
            /* Configure FLEXSPI_SLV using OSC_RC_400M */
            rootCfg.mux = (uint8_t)kCLOCK_FLEXSPI_SLV_ClockRoot_MuxOscRc400M;
            rootCfg.div = 4;
            break;

        case kFLEXSPI_SLV_RootClock_166M:
            /* Configure FLEXSPI_SLV using SYS_PLL1_CLK */
            rootCfg.mux = (uint8_t)kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll1Out;
            rootCfg.div = 6;
            break;

        case kFLEXSPI_SLV_RootClock_200M:
            /* Configure FLEXSPI_SLV using SYS_PLL1_CLK */
            rootCfg.mux = (uint8_t)kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll1Out;
            rootCfg.div = 5;
            break;

        case kFLEXSPI_SLV_RootClock_400M:
            /* Configure FLEXSPI_SLV using OSC_RC_400M */
            rootCfg.mux = (uint8_t)kCLOCK_FLEXSPI_SLV_ClockRoot_MuxOscRc400M;
            rootCfg.div = 1;
            break;

        default:
            /* RootClock_133M: Configure FLEXSPI_SLV using SYS_PLL2_CLK */
            rootCfg.mux = (uint8_t)kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll2Out;
            rootCfg.div = 4;
            break;
    }
    CLOCK_SetRootClock(kCLOCK_Root_Flexspi_Slv, &rootCfg);
}

void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
#ifdef EXAMPLE_RUN_FLEXSPI_LEADER
    BOARD_InitHYPERRAMPins();
#else
    BOARD_InitFLEXSPI_FLRPins();
#endif
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

#ifdef EXAMPLE_RUN_FLEXSPI_LEADER
    /* Clock setting for flexspi1. */
    CLOCK_SetRootClockDiv(kCLOCK_Root_Flexspi1, EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV);
    CLOCK_SetRootClockMux(kCLOCK_Root_Flexspi1, EXAMPLE_FLEXSPI_ROOT_CLOCK_SRC);
#else
    /* Clock setting for FlexSPI FLR. */
    FLEXSPI_SLV_ClkRootFrq(EXAMPLE_FLEXSPI_SLV_ROOT_CLOCK);
#endif
}

/*${function:end}*/
