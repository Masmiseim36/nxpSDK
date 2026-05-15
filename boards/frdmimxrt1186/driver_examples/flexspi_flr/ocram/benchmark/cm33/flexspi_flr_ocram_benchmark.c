/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_flexspi.h"
#include "fsl_flexspi_flr.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "fsl_gpt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef EXAMPLE_WR_BLOCK_SIZE_KB
#define EXAMPLE_WR_BLOCK_SIZE_KB 100U
#endif
#define TYPE int

#ifndef EXAMPLE_USE_CUSTOMIZED_RW
#define EXAMPLE_USE_CUSTOMIZED_RW 0
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void flexspi_ocram_init(FLEXSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
typedef uint32_t iter_t;
typedef struct _state
{
    TYPE *buf;
    TYPE *lastone;
} state_t;
typedef void (*test_func_t)(iter_t iterations, void *cookie);

static volatile uint64_t use_result_dummy;
#ifndef EXAMPLE_RUN_FLEXSPI_LEADER
static char *io_mode[4] = {"SDRx4", "SDRx8", "DDRx4", "DDRx8"};
#endif
extern uint32_t gptFreq;
extern flexspi_device_config_t deviceconfig;
/*******************************************************************************
 * Code
 ******************************************************************************/
void useParameter(int param)
{
    use_result_dummy += param;
}

void readTest(iter_t iterations, void *cookie)
{
    state_t *state = (state_t *)cookie;
    TYPE *bufStart = state->buf;
    TYPE *bufEnd   = state->lastone;
    int sum        = 0;

    while (iterations-- > 0U)
    {
        for (TYPE *p = bufStart; p < bufEnd; p += 128)
        {
            for (int i = 0; i < 128; i++)
            {
                sum += p[i];
            }
        }
    }
    useParameter(sum);
}

void writeTest(iter_t iterations, void *cookie)
{
    state_t *state = (state_t *)cookie;
    TYPE *lastone  = state->lastone;

    while (iterations-- > 0U)
    {
        TYPE *p = state->buf;

        while (p < lastone)
        {
            for (int i = 0; i < 128; i++)
            {
                p[i] = 1;
            }
            p += 128;
        }
    }
}

#if defined (EXAMPLE_USE_CUSTOMIZED_RW) && (EXAMPLE_USE_CUSTOMIZED_RW == 1U)
void cReadTest(iter_t iterations, void *cookie)
{
    state_t *state         = (state_t *)cookie;
    register int sum       = 0;
    register TYPE *lastone = state->lastone;

    while (iterations-- > 0)
    {
        register TYPE *p = state->buf;
        while (p < lastone)
        {
            sum +=
#define DOIT(i) p[i] +
                DOIT(0) DOIT(1) DOIT(2) DOIT(3) DOIT(4) DOIT(5) DOIT(6) DOIT(7) DOIT(8) DOIT(9) DOIT(10) DOIT(11)
                    DOIT(12) DOIT(13) DOIT(14) DOIT(15) DOIT(16) DOIT(17) DOIT(18) DOIT(19) DOIT(20) DOIT(21) DOIT(22)
                        DOIT(23) DOIT(24) DOIT(25) DOIT(26) DOIT(27) DOIT(28) DOIT(29) DOIT(30) DOIT(31) DOIT(32)
                            DOIT(33) DOIT(34) DOIT(35) DOIT(36) DOIT(37) DOIT(38) DOIT(39) DOIT(40) DOIT(41) DOIT(42)
                                DOIT(43) DOIT(44) DOIT(45) DOIT(46) DOIT(47) DOIT(48) DOIT(49) DOIT(50) DOIT(51)
                                    DOIT(52) DOIT(53) DOIT(54) DOIT(55) DOIT(56) DOIT(57) DOIT(58) DOIT(59) DOIT(60)
                                        DOIT(61) DOIT(62) DOIT(63) DOIT(64) DOIT(65) DOIT(66) DOIT(67) DOIT(68) DOIT(69)
                                            DOIT(70) DOIT(71) DOIT(72) DOIT(73) DOIT(74) DOIT(75) DOIT(76) DOIT(77)
                                                DOIT(78) DOIT(79) DOIT(80) DOIT(81) DOIT(82) DOIT(83) DOIT(84) DOIT(85)
                                                    DOIT(86) DOIT(87) DOIT(88) DOIT(89) DOIT(90) DOIT(91) DOIT(92)
                                                        DOIT(93) DOIT(94) DOIT(95) DOIT(96) DOIT(97) DOIT(98) DOIT(99)
                                                            DOIT(100) DOIT(101) DOIT(102) DOIT(103) DOIT(104) DOIT(105)
                                                                DOIT(106) DOIT(107) DOIT(108) DOIT(109) DOIT(110)
                                                                    DOIT(111) DOIT(112) DOIT(113) DOIT(114) DOIT(115)
                                                                        DOIT(116) DOIT(117) DOIT(118) DOIT(119)
                                                                            DOIT(120) DOIT(121) DOIT(122) DOIT(123)
                                                                                DOIT(124) DOIT(125) DOIT(126) p[127];
            p += 128;
        }
    }
    useParameter(sum);
}
#undef DOIT

void cWriteTest(iter_t iterations, void *cookie)
{
    state_t *state         = (state_t *)cookie;
    register TYPE *lastone = state->lastone;

    while (iterations-- > 0)
    {
        register TYPE *p = state->buf;
        while (p < lastone)
        {
#define DOIT(i) p[i] =
            DOIT(0)
            DOIT(1) DOIT(2) DOIT(3) DOIT(4) DOIT(5) DOIT(6) DOIT(7) DOIT(8) DOIT(9) DOIT(10) DOIT(11) DOIT(12) DOIT(13)
                DOIT(14) DOIT(15) DOIT(16) DOIT(17) DOIT(18) DOIT(19) DOIT(20) DOIT(21) DOIT(22) DOIT(23) DOIT(24)
                    DOIT(25) DOIT(26) DOIT(27) DOIT(28) DOIT(29) DOIT(30) DOIT(31) DOIT(32) DOIT(33) DOIT(34) DOIT(35)
                        DOIT(36) DOIT(37) DOIT(38) DOIT(39) DOIT(40) DOIT(41) DOIT(42) DOIT(43) DOIT(44) DOIT(45)
                            DOIT(46) DOIT(47) DOIT(48) DOIT(49) DOIT(50) DOIT(51) DOIT(52) DOIT(53) DOIT(54) DOIT(55)
                                DOIT(56) DOIT(57) DOIT(58) DOIT(59) DOIT(60) DOIT(61) DOIT(62) DOIT(63) DOIT(64)
                                    DOIT(65) DOIT(66) DOIT(67) DOIT(68) DOIT(69) DOIT(70) DOIT(71) DOIT(72) DOIT(73)
                                        DOIT(74) DOIT(75) DOIT(76) DOIT(77) DOIT(78) DOIT(79) DOIT(80) DOIT(81) DOIT(82)
                                            DOIT(83) DOIT(84) DOIT(85) DOIT(86) DOIT(87) DOIT(88) DOIT(89) DOIT(90)
                                                DOIT(91) DOIT(92) DOIT(93) DOIT(94) DOIT(95) DOIT(96) DOIT(97) DOIT(98)
                                                    DOIT(99) DOIT(100) DOIT(101) DOIT(102) DOIT(103) DOIT(104) DOIT(105)
                                                        DOIT(106) DOIT(107) DOIT(108) DOIT(109) DOIT(110) DOIT(111)
                                                            DOIT(112) DOIT(113) DOIT(114) DOIT(115) DOIT(116) DOIT(117)
                                                                DOIT(118) DOIT(119) DOIT(120) DOIT(121) DOIT(122)
                                                                    DOIT(123) DOIT(124) DOIT(125) DOIT(126) DOIT(127) 1;
            p += 128;
        }
    }
}
#undef DOIT
#endif

static void runFunc(char *func_name, test_func_t func, iter_t iter, void *cookie, uint32_t mb)
{
    assert(mb < ((UINT32_MAX) / 1000U));
    assert(gptFreq < ((UINT32_MAX) / 50U));

    TickType_t start_tick = 0, end_tick = 0;
    uint32_t msecs;
    int i = 3;

    PRINTF("Testing bandwidth of %s\r\n", func_name);

    while (i-- > 0)
    {
        start_tick = GPT_GetCurrentTimerCount(EXAMPLE_GPT);
        func(iter, cookie);
        end_tick = GPT_GetCurrentTimerCount(EXAMPLE_GPT);

        end_tick = (end_tick > start_tick) ? end_tick : (gptFreq * 50U - start_tick + end_tick);
        msecs    = (end_tick - start_tick) / (gptFreq / 1000U);

        if (msecs == 0U)
        {
            msecs = 1;
            PRINTF("<Warning> Cost time is shorter than 1 ms! \r\n");
        }

        PRINTF("R/W %d MB in %d ms, bandwidth=%d MB per second \r\n", mb, msecs, (mb * 1000U) / msecs);
    }
}

void benchmark(void)
{
#ifndef EXAMPLE_RUN_FLEXSPI_LEADER
    uintptr_t start_addr = OCRAM_BASEADDR;
#else
    uintptr_t start_addr = EXAMPLE_FLEXSPI_AMBA_BASE;
#endif
    TYPE *buf            = (TYPE *)start_addr;
    uint32_t buf_len_kb  = EXAMPLE_WR_BLOCK_SIZE_KB;
    uint32_t buf_len     = buf_len_kb * 1024U;
    iter_t iter          = 1024;
    state_t state;
    uint32_t mb;

    mb            = iter * buf_len_kb / 1024U;
    state.buf     = buf;
    state.lastone = (TYPE *)(start_addr + buf_len);

#if defined (EXAMPLE_USE_CUSTOMIZED_RW) && (EXAMPLE_USE_CUSTOMIZED_RW == 1U)
    runFunc("READ", cReadTest, iter, (void *)&state, mb);
    runFunc("WRITE", cWriteTest, iter, (void *)&state, mb);
#else
    runFunc("READ", readTest, iter, (void *)&state, mb);
    runFunc("WRITE", writeTest, iter, (void *)&state, mb);
#endif
    PRINTF("**************Testing End*************\r\n");
}

void flexspi_flr_callback(FLEXSPI_SLV_Type *base, flexspi_slv_handle_t *handle)
{
    uint32_t mailBox[FLEXSPI_SLV_SPIMAIL_COUNT] = {0};
    size_t rdCount, wrCount;

    if ((handle->intrMask & (uint32_t)kFLEXSPI_SLV_ErrorCommandFlag) != 0U)
    {
        /* Mask this log in case test reports error when there's glitch on bus. FLR ignores them. */
        //(void)PRINTF("[Follower](interrupt) Error command!\r\n");
    }
    if ((handle->intrMask & ((uint32_t)kFLEXSPI_SLV_WriteOverflowFlag | (uint32_t)kFLEXSPI_SLV_ReadUnderflowFlag)) != 0U)
    {
        FLEXSPI_SLV_GetOutOfRangeCounts(base, &rdCount, &wrCount);
        (void)PRINTF("[Follower](interrupt) Write error count = %u. Read error count = %u!\r\n", wrCount, rdCount);
    }
    if ((handle->intrMask & (uint32_t)kFLEXSPI_SLV_MailInterruptFlag) != 0U)
    {
        (void)PRINTF("[Follower](interrupt) Mailbox data: ");
        for (uint32_t i = 0; i < FLEXSPI_SLV_SPIMAIL_COUNT; i++)
        {
            mailBox[i] = FLEXSPI_SLV_GetMailboxData(base, i);
            (void)PRINTF("Box[%u] 0x%X. ", i, mailBox[i]);
        }
        (void)PRINTF("\r\n");
    }
}

int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

#ifndef EXAMPLE_RUN_FLEXSPI_LEADER
    flexspi_slv_config_t config;

    PRINTF("\r\nFLEXSPI Follower example started!\r\n");
    PRINTF("Root Clock: %uMHz, IO Mode: %s.\r\n", EXAMPLE_FLEXSPI_SLV_ROOT_CLOCK, io_mode[EXAMPLE_FLEXSPI_SLV_MODE]);

    FLEXSPI_SLV_GetDefaultConfig(&config);
    config.ioMode             = EXAMPLE_FLEXSPI_SLV_MODE;
    config.baseAddr1          = OCRAM_BASEADDR;
    config.baseAddr2          = OCRAM_BASEADDR;
    config.addrRange1         = MEMORY_SIZE;
    config.addrRange2         = MEMORY_SIZE;
    config.rxWatermark        = 32;
    config.txWatermark        = kFLEXSPI_SLV_Write_Watermark_256Bytes;
    config.readRegCmd         = FLEXSPI_SLV_CMD_DDR(FOLLOWER_CMD_READ_STATUS);
    config.readRegDummyCycle  = EXAMPLE_FLEXSPI_SLV_DUMMY_CYCLE;
    config.writeRegCmd        = FLEXSPI_SLV_CMD_DDR(FOLLOWER_CMD_SEND_MAILBOX);
    config.readMemCmd1        = FLEXSPI_SLV_CMD_DDR(FOLLOWER_CMD_READ_MEMORY);
    config.readMemDummyCycle1 = EXAMPLE_FLEXSPI_SLV_DUMMY_CYCLE;
    config.writeMemCmd1       = FLEXSPI_SLV_CMD_DDR(FOLLOWER_CMD_WRITE_MEMORY);

    FLEXSPI_SLV_Init(EXAMPLE_FLEXSPI_SLV, &config);
    PRINTF("FLEXSPI Follower is initialized!\r\n");
    flexspi_slv_handle_t handle;
    FLEXSPI_SLV_InterruptCreateHandle(EXAMPLE_FLEXSPI_SLV, &handle, flexspi_flr_callback,
                                      kFLEXSPI_SLV_AllInterruptFlags);
#else
    deviceconfig.flexspiRootClk = FLEXSPI_GetRootClockFreqHz(EXAMPLE_FLEXSPI_ROOT_CLOCK_SRC);
    PRINTF("\r\nFLEXSPI Leader example started!\r\n");
    PRINTF("Root Clock: %uHz.\r\n", deviceconfig.flexspiRootClk);

    flexspi_ocram_init(EXAMPLE_FLEXSPI);
    PRINTF("FLEXSPI Leader is initialized!\r\n");
#endif
    setupTimerInterrupt();

    benchmark();

    while (1)
    {
    }
}
