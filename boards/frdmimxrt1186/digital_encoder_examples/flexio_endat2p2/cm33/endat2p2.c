/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_flexio_endat2p2.h"
#include "fsl_xbar.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

FLEXIO_ENDAT2_Type endat2Master = {
    .flexio = BOARD_FLEXIO_INSTANCE,
    .RxdPinIndex = BOARD_INITPINS_RXD_CHANNEL,
    .TxdPinIndex = BOARD_INITPINS_TXD_CHANNEL,
    .ClkPinIndex = BOARD_INITPINS_CLK_CHANNEL,
    .DirPinIndex = BOARD_INITPINS_DIR_CHANNEL,
    .txTrigger = kFlexioEndat2_txTriggerSw,
    .triggerInput = 0,
    .mtLen = DEMO_ENCODER_MT_LEN,
    .stLen = DEMO_ENCODER_ST_LEN,
    .recoveryTimeIII = DEMO_ENCODER_RECOVERY_TIME_III
};

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_FLEXIO_IRQHandler()
{
    if (FLEXIO_ENDAT2_DataProcess(&endat2Master) != kStatus_Success) {
        PRINTF("HW: CRC Error\r\n");
    }
    FLEXIO_ENDAT2_ClearRxStatusFlags(&endat2Master);
    PRINTF("HW: mt: 0x%x st:0x%x\r\n", endat2Master.mt,  endat2Master.st);
}

/*!
 * @brief Main function
 */
int main(void)
{
	BOARD_InitHardware();
	BOARD_InitDebugConsole();
	PRINTF("\r\nFlexIO EnDat2.2 Demo.\r\n");
	flexio_endat2_config_t config;
	FLEXIO_ENDAT2_GetDefaultConfig(&config);
    config.baudRate_Bps = DEMO_BIT_RATE;
    uint32_t srcClock_Hz = CLOCK_GetRootClockFreq(BOARD_FLEXIO_CLOCK_ROOT);
    uint32_t baudRate = DEMO_BIT_RATE;
    while (FLEXIO_ENDAT2_FlexIO_BuadrateVerify(srcClock_Hz, baudRate) != kStatus_Success) {
        baudRate++;
    }

    if (baudRate != config.baudRate_Bps) {
        PRINTF("\r\nThere is big deviation for Buadrate %d. The new Buadrate %d is used\r\n", config.baudRate_Bps, baudRate);
        config.baudRate_Bps = baudRate;
    }

    FLEXIO_ENDAT2_Init(&endat2Master, &config, srcClock_Hz);

    int number = 0x10;
    PRINTF("\r\nGet positon data in SW trigger mode.\r\n");
	while (number--) {
		if (FLEXIO_ENDAT2_ReadBlocking(&endat2Master) != kStatus_Success) {
            PRINTF("SW: CRC Error\r\n");
        } else {
            PRINTF("SW: mt: 0x%x st:0x%x\r\n", endat2Master.mt,  endat2Master.st);
        }
	}

    PRINTF("\r\nGet positon data in HW trigger mode.\r\n");
    FLEXIO_ENDAT2_Deinit(&endat2Master);
    endat2Master.txTrigger = kFlexioEndat2_txTriggerHw;
    FLEXIO_ENDAT2_Init(&endat2Master, &config, CLOCK_GetRootClockFreq(BOARD_FLEXIO_CLOCK_ROOT));
    FLEXIO_ENDAT2_WriteCmd(&endat2Master, kFlexIO_ENDAT2_CmdIdxEncSendPosVal);
    FLEXIO_ENDAT2_EnableInterrupts(&endat2Master);
    EnableIRQ(BOARD_FLEXIO_IRQn);
	PWM_Trigger_Init(BOARD_PWM_INSTANCE);
    while (1);
}
