/*
 * Copyright 2016, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

#include "nfc_device.h"
#include "ntag_bridge.h"
#include "HAL_I2C_driver.h"
#include "fsl_device_registers.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NTAG_I2C_MASTER_BASEADDR I2C4
#define I2C_MASTER_CLK_SRC       4


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*
 * Setup the Chip
 */
void Setup(void);

/*!
 * @brief delay a while.
 */
void delay(void);

/*
 * @brief Let a LED blink (blocking function ~ 700ms)
 *
 */
void LED_blink(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static NFC_HANDLE_T ntag_handle;

/*
 * main Program
 * @return should never return
 */

int main(void)
{
    /* Init Board hardware. */
    Setup();

    /* Print a note to terminal. */
    PRINTF("\r\n\r\n NTAG I2C blink example\r\n");
    PRINTF("\r\n The LED is blinking when RF field is detected.\r\n");

    uint8_t reg = 0;
    uint8_t FD  = 1;
    BOOL status = FALSE;

    /* configure ntag to active the field detection signal on the presence of the RF field. */
    status = NFC_SetFDOnFunction(ntag_handle, RF_SWITCHED_ON_00b);

    // Main Loop
    while (1)
    {
        // Read NTAG I2C session registers. The last bit of NS_REG indicates whether RF field is detected
        status = NFC_ReadRegister(ntag_handle, NTAG_MEM_OFFSET_NS_REG, &reg);
        if (status)
        {
            PRINTF("\r\nCommunication Failed!");
        }

        // If RF field is detected, we blink a red led

        while ((reg & NTAG_NS_REG_MASK_RF_FIELD_PRESENT))
        {
            LED_blink();
            if (FD)
            {
                FD = 0;
                PRINTF("\r\nFIELD DETECTED!");
            }

            // we check again if RF field is available
            NFC_ReadRegister(ntag_handle, NTAG_MEM_OFFSET_NS_REG, &reg);
        }

        if (!FD)
        {
            PRINTF("\r\nFIELD LOST!");
            FD = 1;
        }
    }
}

//---------------------------------------------------------------------
/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1600000; ++i)
    {
        __asm("NOP");
        /* delay */
    }
}

void Setup(void)
{
    /* Board pin, clock, debug console init */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach 12 MHz clock to FLEXCOMM4 (I2C master) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);

    /* reset FLEXCOMM for I2C */
    RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);
    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();
    BOARD_InitLEDs();
    BOARD_InitDEBUG_UART();
    I2C4_InitPins();

    LED_RED_INIT(0);

    /* Initialize I2C */
    HAL_I2C_InitDevice(HAL_I2C_INIT_DEFAULT, I2C_MASTER_CLK_SRC, NTAG_I2C_MASTER_BASEADDR);

    /* Initialize the NTAG I2C components */
    ntag_handle = NFC_InitDevice((NTAG_ID_T)0, NTAG_I2C_MASTER_BASEADDR);
}

//---------------------------------------------------------------------

void LED_blink(void)
{
    delay();
    LED_RED_ON();
    delay();
    LED_RED_OFF();
}

//---------------------------------------------------------------------
//               Helping functions end
//---------------------------------------------------------------------
