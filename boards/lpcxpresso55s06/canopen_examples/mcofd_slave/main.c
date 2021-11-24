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
LICENSE:   THIS IS THE NXP SDK VERSION OF MICROCANOPEN PLUS
           Licensed under a modified BSD License. See LICENSE.INFO
           file in the project root for full license information.
VERSION:   7.10, ESA 20-SEP-02
           $LastChangedDate: 2020-09-03 22:04:52 +0200 (Thu, 03 Sep 2020) $
           $LastChangedRevision: 5038 $
***************************************************************************/

#include "mcop_inc.h"
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
 * DOES:    USER Process, here simple I/O example
 * RETURNS: nothing
 ******************************************************************************/
void USER_Process(void)
{
    // buffers to hold CANopen in and out data
    static uint8_t digital_out;
    static uint8_t digital_in;
    static uint16_t analog_out;
    static uint16_t analog_in;
    static uint8_t cos_dig_cnt;                        // counter for digital change of state
    static uint8_t cos_ana_cnt;                        // counter for analog change of state
    static uint8_t digital_out_last[4] = {1, 1, 1, 1}; // copy of last received data
    static uint16_t analog_out_last[2] = {1, 1};       // copy of last received data
#if DEBUG_CONSOLE
#warning "Extended DEBUG_CONSOLE output enabled, delays execution"
    char linebuf[200];
    uint8_t print_info;
#endif

#if DEBUG_CONSOLE
    sprintf(linebuf, " ");
    print_info = FALSE;
#endif

    // Process all output data from CANopen, COS change of state detection

    MCO_ReadProcessData(&(digital_out), 1, P620001_DigOutput8_1);
    if (digital_out != digital_out_last[0])
    { // dig out 1 changed, replace with own code reacting to output
        cos_dig_cnt++;
        digital_out_last[0] = digital_out;
#if DEBUG_CONSOLE
        sprintf(linebuf + strlen(linebuf), "[6200h,01h]:0x%02X ", digital_out);
        print_info = TRUE;
#endif
    }

    MCO_ReadProcessData(&(digital_out), 1, P620002_DigOutput8_2);
    if (digital_out != digital_out_last[1])
    { // dig out 2 changed, replace with own code reacting to output
        cos_dig_cnt++;
        digital_out_last[1] = digital_out;
#if DEBUG_CONSOLE
        sprintf(linebuf + strlen(linebuf), "[6200h,02h]:0x%02X ", digital_out);
        print_info = TRUE;
#endif
    }

    MCO_ReadProcessData(&(digital_out), 1, P620003_DigOutput8_3);
    if (digital_out != digital_out_last[2])
    { // dig out 3 changed, replace with own code reacting to output
        cos_dig_cnt++;
        digital_out_last[2] = digital_out;
#if DEBUG_CONSOLE
        sprintf(linebuf + strlen(linebuf), "[6200h,03h]:0x%02X ", digital_out);
        print_info = TRUE;
#endif
    }

    MCO_ReadProcessData(&(digital_out), 1, P620004_DigOutput8_4);
    if (digital_out != digital_out_last[3])
    { // dig out 4 changed, replace with own code reacting to output
        cos_dig_cnt++;
        digital_out_last[3] = digital_out;
#if DEBUG_CONSOLE
        sprintf(linebuf + strlen(linebuf), "[6200h,04h]:0x%02X ", digital_out);
        print_info = TRUE;
#endif
    }

    MCO_ReadProcessData((uint8_t *)&(analog_out), 2, P641101_AnalogOutput16_1);
    if (analog_out != analog_out_last[0])
    { // ana out 1 changed, replace with own code reacting to output
        cos_ana_cnt++;
        analog_out_last[0] = analog_out;
#if DEBUG_CONSOLE
        sprintf(linebuf + strlen(linebuf), "[6411h,01h]:0x%04X ", analog_out);
        print_info = TRUE;
#endif
    }

    MCO_ReadProcessData((uint8_t *)&(analog_out), 2, P641102_AnalogOutput16_2);
    if (analog_out != analog_out_last[1])
    { // ana out 2 changed, replace with own code reacting to output
        cos_ana_cnt++;
        analog_out_last[1] = analog_out;
#if DEBUG_CONSOLE
        sprintf(linebuf + strlen(linebuf), "[6411h,02h]:0x%04X ", analog_out);
        print_info = TRUE;
#endif
    }

#if DEBUG_CONSOLE
    if (print_info)
        PRINTF("\r\n%s", linebuf);
#endif

    // Process all input data, to CANopen

    digital_in = cos_dig_cnt;
    MCO_WriteProcessData(P600001_DigInput8_1, 1, &(digital_in));

    digital_in = cos_ana_cnt;
    MCO_WriteProcessData(P600002_DigInput8_2, 1, &(digital_in));

    digital_in = digital_out_last[2];
    MCO_WriteProcessData(P600003_DigInput8_3, 1, &(digital_in));

    digital_in = digital_out_last[3];
    MCO_WriteProcessData(P600004_DigInput8_4, 1, &(digital_in));

    // hi byte seconds, lo byte quarter seconds
    analog_in = (((MCOHW_GetTime() / 1000) << 8) & 0x3F) + ((MCOHW_GetTime() / 250) & 3);
    MCO_WriteProcessData(P640101_AnalogInput16_1, 2, (uint8_t *)&(analog_in));

    analog_in = analog_out_last[1];
    MCO_WriteProcessData(P640102_AnalogInput16_2, 2, (uint8_t *)&(analog_in));
}

/*******************************************************************************
 * DOES:    The main function
 * RETURNS: nothing
 ******************************************************************************/
int main(void)
{
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
    PRINTF("\r\nStarting CANopen FD Library slave example\r\n");
#else
    PRINTF("\r\nStarting CANopen Library slave example\r\n");
#endif
    PRINTF("Provided by EmSA - www.em-sa.com/nxp\r\n");

    // Reset/Initialize CANopen communication
    MCOUSER_ResetCommunication();

    // foreground loop
    while (1)
    {
        // Operate on CANopen protocol stack
        MCO_ProcessStack();

        // Operate on application
        if (MY_NMT_STATE == NMTSTATE_OP)
        { // only when we are operational
            USER_Process();
        }

        // Check for CAN Err, auto-recover
        if (MCOHW_GetStatus() & HW_BOFF)
        {
            MCOUSER_FatalError(0xF6F6);
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
