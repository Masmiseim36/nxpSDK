/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
   
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
*@brief      Retrieve UART source frequency
*
*@param      none
*            
*@return     UART source frequency
*/
uint32_t BOARD_UARTSrcFreq(void)
{
    uint32_t freq;
    
    CLOCK_SetMux(kCLOCK_UartMux, 1);

    /* To make it simple, we assume default PLL and divider settings, and the only variable
       from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
    {
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }
    else
    {
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return freq;
} 

