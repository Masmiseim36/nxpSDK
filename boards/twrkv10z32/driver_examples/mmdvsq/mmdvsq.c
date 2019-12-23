/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_mmdvsq.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
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
/*!
 * @brief Main function
 */
int main(void)
{
    uint16_t sqrtresult;  /* square root result */
    int32_t divideresult; /* divide result */
    uint32_t rcndval;     /* radicand value */
    int32_t dendval;      /* dividend value */
    int32_t dsorval;      /* divisor value */

    /* Initialize standard SDK demo application pins */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Print the initial banner */
    PRINTF("\r\nStart MMDVSQ Example\r\n");
    PRINTF("\r\nCalculation square root, please enter radicand\r\n");
    /* Import radicand value */
    SCANF("%d", &rcndval);
    /* Calculation square root */
    sqrtresult = MMDVSQ_Sqrt(MMDVSQ, rcndval);
    PRINTF("\r\nSquare root of %d is %d\r\n", rcndval, sqrtresult);

    PRINTF("\r\nCalculation division to get remainder and quotient");
    PRINTF("\r\nEnter dividend and divisor\r\n");
    /* Import dividend value and divisor value */
    SCANF("%d, %d", &dendval, &dsorval);
    /* Calculation Remainder */
    divideresult = MMDVSQ_GetDivideRemainder(MMDVSQ, dendval, dsorval, false);
    PRINTF("\r\nRemainder of %d and %d is %d\r\n", dendval, dsorval, divideresult);
    /* Calculation nQuotient */
    divideresult = MMDVSQ_GetDivideQuotient(MMDVSQ, dendval, dsorval, false);
    PRINTF("\r\nQuotient of %d and %d is %d\r\n", dendval, dsorval, divideresult);

    while (1)
    {
    }
}
