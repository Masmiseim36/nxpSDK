/**************************************************************************
MODULE:    MAIN
CONTAINS:  Example application using MicroCANopen
           NXP LPC54xxx and LPC55xx derivatives with CAN interface.
COPYRIGHT: (c) Embedded Systems Academy (EmSA) 2002-2020
           All rights reserved. www.em-sa.com/nxp
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS MANAGER
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

// Include all MicroCANopen Plus Manager relevant files
#include "mcop_mgr_inc.h"
#include "pin_mux.h"
#include "board.h"

#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * DOES:    Working on the application
 * RETURNS: nothing
 ******************************************************************************/
void USER_ProcessApp(void)
{
    static uint16_t analog_in;
    static uint8_t digital;

    // simulate some input/output data

    // copy input byte 1 to output 3
    MCO_ReadProcessData(&(digital), 1, P620001_DigOutput8_1);
    MCO_WriteProcessData(P600003_DigInput8_3, 1, &(digital));

    // copy input byte 2 to output 4
    MCO_ReadProcessData(&(digital), 1, P620002_DigOutput8_2);
    MCO_WriteProcessData(P600004_DigInput8_4, 1, &(digital));

    // analog 1: hi byte seconds, lo byte quarter seconds
    analog_in = (((MCOHW_GetTime() / 1000) << 8) & 0x3F) + ((MCOHW_GetTime() / 250) & 3);
    MCO_WriteProcessData(P640101_AnalogInput16_1, 2, (uint8_t *)&(analog_in));

    // analog 2: copy of analog in 1
    MCO_ReadProcessData((uint8_t *)&(analog_in), 2, P641101_AnalogOutput16_1);
    MCO_WriteProcessData(P640102_AnalogInput16_2, 2, (uint8_t *)&(analog_in));
}

/*******************************************************************************
 * DOES:    The main function
 * RETURNS: nothing
 ******************************************************************************/
int main(void)
{
    uint8_t do_nmtreset = TRUE; // reset all slave nodes once
    uint16_t nmtreset_delay;    // delay before resetting slave nodes

    // Initialize hardware components needed for this example
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    BOARD_InitPins();

    BOARD_InitBootClocks();
#if USE_CANOPEN_FD
    /* Set MCAN clock 80Mhz/1=80Mhz for CAN FD. */
    CLOCK_SetClkDiv(kCLOCK_DivCanClk, 1U, true);
#else
    /* Set MCAN clock 80Mhz/4=20Mhz for legacy CAN. */
    CLOCK_SetClkDiv(kCLOCK_DivCanClk, 4U, true);
#endif
    CLOCK_AttachClk(kMCAN_DIV_to_MCAN);
    /* Enable FRO 1M clock for WWDT module. */
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;
    /* Set clock divider for WWDT clock source. */
    CLOCK_SetClkDiv(kCLOCK_DivWdtClk, 1U, true);

    BOARD_InitDebugConsole();

#if USE_LEDS
    // Initialize status LEDs
    LIBCB_InitLeds();
#endif

    // initialize timer interrupt - 1ms period
    SysTick_Config(SystemCoreClock / 1000U);

#if USE_CANOPEN_FD
    PRINTF("\r\nStarting CANopen FD Library manager example\r\n");
#else
    PRINTF("\r\nStarting CANopen Library manager example\r\n");
#endif
    PRINTF("Provided by EmSA - www.em-sa.com/nxp\r\n");

    // Reset/Initialize CANopen communication
    MCOUSER_ResetCommunication();

    // initialize timer interrupt - 1ms period
    SysTick_Config(SystemCoreClock / 1000U);

    // NMT reset delay
    nmtreset_delay = MCOHW_GetTime() + 500;

    // foreground loop
    while (1)
    {
        // Operate on CANopen protocol stack, slave
        MCO_ProcessStack();
        // Operate on application
        USER_ProcessApp();

        if (MY_NMT_STATE == NMTSTATE_OP)
        { // Only work on manager when we are operational
            // Operate on CANopen protocol stack, manager
            MGR_ProcessMgr();
            // Operate on custom CANopen manager application
            USER_ProcessMgr();
        }

        if (do_nmtreset && MCOHW_IsTimeExpired(nmtreset_delay))
        { // after delay, reset all slave nodes
            // restart all nodes
            MGR_TransmitNMT(NMTMSG_RESETAPP, 0);
            do_nmtreset = FALSE; // only do it once
        }

    } // end of while(1)
} // end of main

/*******************************************************************************
 * DOES:    System Tick handler, execute every 1 ms
 * RETURNS: nothing
 ******************************************************************************/
void SysTick_Handler(void)
{
    MCOHW_Tick(); // Call CANopen stack tick function
}

#if (defined(__CC_ARM)) || (defined(__ARMCC_VERSION)) || (defined(__ICCARM__)) || \
    (defined(__GNUC__) && !defined(__REDLIB__))
void __assertion_failed(char *failedExpr)
{
    PRINTF("ASSERT ERROR \" %s \n", failedExpr);
    for (;;)
    {
        __BKPT(0);
    }
}
#endif
