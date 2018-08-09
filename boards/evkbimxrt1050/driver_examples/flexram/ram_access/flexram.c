/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_flexram.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_FLEXRAM FLEXRAM
#define APP_FLEXRAM_IRQ FLEXRAM_IRQn
#define APP_FLEXRAM_OCRAM_START_ADDR 0x20200000
#define APP_FLEXRAM_OCRAM_MAGIC_ADDR 0x202000a8
#define APP_FLEXRAM_IRQ_HANDLER FLEXRAM_IRQHandler
#define APP_DSB() __DSB()
#define APP_ISB() __ISB()


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
* @brief ocram access function.
*
*/
static void OCRAM_Access(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool s_flexram_ocram_magic_addr_match = false;
static bool s_flexram_ocram_access_error_match = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_FLEXRAM_IRQ_HANDLER(void)
{
    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_OCRAMAccessError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_OCRAMAccessError);
        s_flexram_ocram_access_error_match = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_OCRAMMagicAddrMatch)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_OCRAMMagicAddrMatch);
        s_flexram_ocram_magic_addr_match = true;
    }

/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_DisableDCache();
#endif

    PRINTF("\r\nFLEXRAM ram access driver example.\r\n");

    /* enable IRQ */
    EnableIRQ(APP_FLEXRAM_IRQ);
    /* init flexram */
    FLEXRAM_Init(APP_FLEXRAM);
    /* access the default ocram */
    OCRAM_Access();

    PRINTF("\r\nFLEXRAM ram access driver example finish.\r\n");

    while (1)
    {
    }
}

static void OCRAM_Access(void)
{
    uint8_t *ocramAddr = (uint8_t *)APP_FLEXRAM_OCRAM_START_ADDR;

    /* enable FLEXRAM OCRAM access error interrupt and OCRAM magic address match interrupt */
    FLEXRAM_EnableInterruptSignal(APP_FLEXRAM, kFLEXRAM_OCRAMAccessError | kFLEXRAM_OCRAMMagicAddrMatch);
    /* config ocram magic address
    * read access hit magic address will generate interrupt
    */
    FLEXRAM_SetOCRAMMagicAddr(APP_FLEXRAM, (uint16_t)APP_FLEXRAM_OCRAM_MAGIC_ADDR, kFLEXRAM_Write);

    for (;;)
    {
        *ocramAddr = 0xCC;
        /* Synchronizes the execution stream with memory accesses */
        APP_DSB();
        APP_ISB();
        /* check ocram magic addr match event */
        if (s_flexram_ocram_magic_addr_match)
        {
            PRINTF("\r\nOCRAM Magic address 0x%x match\r\n", (uint32_t)(ocramAddr));
            s_flexram_ocram_magic_addr_match = false;
        }
        /* check ocram access error event */
        if (s_flexram_ocram_access_error_match)
        {
            PRINTF("\r\nOCRAM access 0x%x to boundary.\r\n", (uint32_t)(ocramAddr));
            s_flexram_ocram_access_error_match = false;
            break;
        }
        ocramAddr++;
    }
}
