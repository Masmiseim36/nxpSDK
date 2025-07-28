/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file hdi.c
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "hdi.h"
#include "fsl_common.h"
#include "fsl_lpuart.h"
#include "fsl_clock.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#ifndef HDI_TEST
#define HDI_TEST 0
#endif

/* Radio board mode selection */
#define HCI_CMD_BLE          0xF3
#define HCI_CMD_ZB           0xF4
#define HCI_CMD_GFSK         0xF5
#define HCI_CMD_CS_INITIATOR 0xF6
#define HCI_CMD_CS_REFLECTOR 0xF7

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static const lpuart_config_t uart_config = {
    .baudRate_Bps  = 115200,
    .dataBitsCount = kLPUART_EightDataBits,
    .parityMode    = kLPUART_ParityDisabled,
    .stopBitCount  = kLPUART_OneStopBit,
    .isMsb         = false,
    .enableTx      = true,
    .enableRx      = true,
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int HDI_Init(void)
{
    CLOCK_EnableClock(kCLOCK_Lpuart1);
    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_IpSrcFro192M);
    (void)LPUART_Init(LPUART1, &uart_config, 32000000);

#if defined(HDI_TEST) && (HDI_TEST > 0)
    const char *test = "HDI Init\n";
    HDI_WriteBlocking((uint8_t *)test, strlen(test));
#endif

    return 0;
}

int HDI_WriteBlocking(const uint8_t *data, size_t size)
{
    return (int)LPUART_WriteBlocking(LPUART1, data, size);
}

int HDI_ReadBlocking(uint8_t *out, size_t size)
{
    return (int)LPUART_ReadBlocking(LPUART1, out, size);
}

void HDI_Set_Mode_Ble(void)
{
    uint8_t cmd = HCI_CMD_BLE;
    (void)HDI_WriteBlocking(&cmd, 1U);
}

void HDI_Set_Mode_Zigbee(void)
{
    uint8_t cmd = HCI_CMD_ZB;
    (void)HDI_WriteBlocking(&cmd, 1U);
}

void HDI_Set_Mode_Gfsk(void)
{
    uint8_t cmd = HCI_CMD_GFSK;
    (void)HDI_WriteBlocking(&cmd, 1U);
}

void HDI_Set_Mode_Initiator(void)
{
    uint8_t cmd = HCI_CMD_CS_INITIATOR;
    (void)HDI_WriteBlocking(&cmd, 1U);
}

void HDI_Set_Mode_Reflector(void)
{
    uint8_t cmd = HCI_CMD_CS_REFLECTOR;
    (void)HDI_WriteBlocking(&cmd, 1U);
}
