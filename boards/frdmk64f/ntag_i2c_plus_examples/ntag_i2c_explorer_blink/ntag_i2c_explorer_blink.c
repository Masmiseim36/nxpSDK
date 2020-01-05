/*
 * Copyright 2016, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

#include "nfc_device.h"
#include "ntag_bridge.h"
#include "HAL_I2C_driver.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef I2C_CMSIS
#define NTAG_I2C_MASTER_BASEADDR &Driver_I2C0
#define I2C_MASTER_IRQ I2C0_IRQn
#endif

#ifdef I2C_FSL
#define NTAG_I2C_MASTER_BASEADDR I2C0
#define I2C_MASTER_CLK_SRC I2C0_CLK_SRC
#endif


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
uint32_t I2C0_GetFreq(void)
{
    return CLOCK_GetFreq(I2C0_CLK_SRC);
}

uint32_t I2C1_GetFreq(void)
{
    return CLOCK_GetFreq(I2C1_CLK_SRC);
}

uint32_t I2C2_GetFreq(void)
{
    return CLOCK_GetFreq(I2C2_CLK_SRC);
}

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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitButtons();
    BOARD_InitLEDs();
    BOARD_InitDEBUG_UART();
    I2C0_InitPins();

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
