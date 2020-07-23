/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app_periph_init.h"

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "freemaster.h"
#include "freemaster_serial_uart.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Calculate the UART0 periphery baud rate. */
#define CLOCK_UART_BAUD   (CLOCK_SYSTEM_FREQ / (BOARD_FMSTR_UART_BAUDRATE * 16UL))

/* Application version identification structure. */
typedef const struct _app_ver
{
    char    cBoardID[15U];     /* Board name. */
    char    cMotorType[4U];    /* Motor type. */
    char    cAppVer[5U];       /* Application version. */
} app_ver_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Application version identification structure. */
const app_ver_t c_sAppVersion = {APP_BOARD_NAME, APP_MOTOR_TYPE, APP_SW_VERSION};

/* Fast-control loop CPU usage measurement. */
cpu_tick_meas_t       g_sCpuTickMeas;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN(TSA_APPINIT)

    /* Board ID structure definition. */
    FMSTR_TSA_RO_MEM(c_sAppVersion.cBoardID,   FMSTR_TSA_UINT8, c_sAppVersion.cBoardID,   sizeof(c_sAppVersion.cBoardID))
    FMSTR_TSA_RO_MEM(c_sAppVersion.cMotorType, FMSTR_TSA_UINT8, c_sAppVersion.cMotorType, sizeof(c_sAppVersion.cMotorType))
    FMSTR_TSA_RO_MEM(c_sAppVersion.cAppVer,    FMSTR_TSA_UINT8, c_sAppVersion.cAppVer,    sizeof(c_sAppVersion.cAppVer))
FMSTR_TSA_TABLE_END()
MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*!
 * @brief   Initialize the serial port for FreeMASTER interface.
 *
 * @param   void
 *
 * @return  none
 */
static void InitUART(void);

/*!
 * @brief   CPU cycle measurement of the CPU ticks. The SysTick timer is
 *          configured as free-running timer.
 *
 * @param   ui32LoadVal - The timer load value.
 *
 * @return  none
 */
static void InitSysTick(uint32_t ui32LoadVal);


/*******************************************************************************
 * Code - functions with global scope
 ******************************************************************************/
/*!
 * Global MCU main initialization function.
 */
void APPPER_Init(void)
{
    /* Initialize the SysTick timer. */
    InitSysTick(CLOCK_SYSTEM_FREQ / CLOCK_SYSTICK_FREQ);

    /* Initialize all pins. */
    InitUART();

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(BOARD_FMSTR_UART_PORT);
}

/*!
 * CPU tick measurement using SysTick initialization.
 */
static void InitSysTick(uint32_t ui32LoadVal)
{
    /* Set period to maximum value.*/
    SysTick->LOAD = ui32LoadVal;

    /* Use system clock as source and start the timer. */
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
}

/*******************************************************************************
 * Code - functions with local scope
 ******************************************************************************/


/*!
 * Initialization of the UART periphery for FreeMASTER interface.
 */
static void InitUART(void)
{
    /* Enable clocks for the the UART periphery. */
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;

    /* Configure the UART for 8-bit mode and no parity. */
    BOARD_FMSTR_UART_PORT->C1 = 0U;

    /* Configure the baud rate. */
    BOARD_FMSTR_UART_PORT->BDH = (uint8_t)((CLOCK_UART_BAUD & 0x1F00UL) >> 8U);
    BOARD_FMSTR_UART_PORT->BDL = (uint8_t)(CLOCK_UART_BAUD & 0xFFUL);

    /* Enable receiver and transmitter. */
    BOARD_FMSTR_UART_PORT->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;
}

