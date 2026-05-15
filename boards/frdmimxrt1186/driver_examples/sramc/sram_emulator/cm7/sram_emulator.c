/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_rgpio.h"
#include "app.h"
#include "fsl_iomuxc.h"
#include "flexio_sram_emulator.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t dataBuffer[FLEXIO_SRAM_DATA_BUFFER_LEN];

static FLEXIO_SRAM_Type sramDev = {
    .flexio = BOARD_FLEXIO_INSTANCE,
    .daPinIndex = BOARD_FLEXIO_DA_PIN_INDEX,
    .wePinIndex = BOARD_FLEXIO_WE_PIN_INDEX,
    .oePinIndex = BOARD_FLEXIO_OE_PIN_INDEX,
    .advPinIndex = BOARD_FLEXIO_ADV_PIN_INDEX,
    .cePinIndex = BOARD_FLEXIO_CE_PIN_INDEX,
    .weShifterIndex = BOARD_FLEXIO_WE_SHIFTER_INDEX,
    .advShifterIndex = BOARD_FLEXIO_ADV_SHIFTER_INDEX,
    .weTimerIndex = BOARD_FLEXIO_WE_TIMER_INDEX,
    .advTimerIndex = BOARD_FLEXIO_ADV_TIMER_INDEX,
    .data = dataBuffer,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
AT_QUICKACCESS_SECTION_CODE(void BOARD_FLEXIO_IRQ_HANDLER(void))
{
    FLEXIO_SRAM_ISR(&sramDev);
}

int main(void)
{
    flexio_sram_config_t config;

    /* Hardware initialize. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    
    PRINTF("FELXIO SRAM EMULATOR EXAMPLE.\r\n");

    FLEXIO_SRAM_GetDefaultConfig(&config);
    FLEXIO_SRAM_Init(&sramDev, &config);

    NVIC_ClearPendingIRQ(BOARD_FLEXIO_IRQN);    /* Clear pending NVIC IRQ before enable NVIC IRQ. */
    (void)EnableIRQ(BOARD_FLEXIO_IRQN);         /* Enable interrupt in NVIC. */

    FLEXIO_SRAM_Start(&sramDev);

    while (true)
    {
    }
}