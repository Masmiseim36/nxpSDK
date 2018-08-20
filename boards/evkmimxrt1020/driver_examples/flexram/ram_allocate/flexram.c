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

#define APP_FLEXRAM_IRQ_HANDLER FLEXRAM_IRQHandler

#define APP_FLEXRAM_OCRAM_START_ADDR 0x20200000
#define APP_FLEXRAM_OCRAM_MAGIC_ADDR 0x202000A0

#define APP_FLEXRAM_DTCM_START_ADDR 0x20000000
#define APP_FLEXRAM_DTCM_MAGIC_ADDR 0x200000A0

#define APP_FLEXRAM_ITCM_START_ADDR 0x0
#define APP_FLEXRAM_ITCM_MAGIC_ADDR 0xA0

/* OCRAM relocate definition */
#define APP_OCRAM_SIZE (256 * 1024U)
#define APP_OCRAM_ALLOCATE_BANK_NUM 2
#define APP_ITCM_ALLOCATE_BANK_NUM 4
#define APP_DTCM_ALLOCATE_BANK_NUM 2

#define APP_DSB() __DSB()
#define APP_ISB() __ISB()


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief OCRAM reallocate function.
 *
 * @param base FLEXRAM base address.
 */
static status_t OCRAM_Reallocate(void);

/*!
 * @brief ocram access function.
 *
 * @param base FLEXRAM base address.
 */
static void OCRAM_Access(void);

/*!
 * @brief DTCM access function.
 *
 * @param base FLEXRAM base address.
 */
static void DTCM_Access(void);

/*!
 * @brief ITCM access function.
 *
 * @param base FLEXRAM base address.
 */
static void ITCM_Access(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool s_flexram_ocram_magic_addr_match = false;
static bool s_flexram_ocram_access_error_match = false;

static bool s_flexram_dtcm_magic_addr_match = false;
static bool s_flexram_itcm_magic_addr_match = false;

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

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_DTCMMagicAddrMatch)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_DTCMMagicAddrMatch);
        s_flexram_dtcm_magic_addr_match = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_ITCMMagicAddrMatch)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_ITCMMagicAddrMatch);
        s_flexram_itcm_magic_addr_match = true;
    }
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

    PRINTF("\r\nFLEXRAM ram allocate example.\r\n");

    /* enable IRQ */
    EnableIRQ(APP_FLEXRAM_IRQ);
    /* reallocate ram */
    OCRAM_Reallocate();
    /* init flexram */
    FLEXRAM_Init(APP_FLEXRAM);
    /*test OCRAM access*/
    OCRAM_Access();
    /*test dtcm access*/
    DTCM_Access();
    /*test itcm access*/
    ITCM_Access();

    PRINTF("\r\nFLEXRAM ram allocate example finish.\r\n");

    while (1)
    {
    }
}

static status_t OCRAM_Reallocate(void)
{
    flexram_allocate_ram_t ramAllocate = {
        .ocramBankNum = APP_OCRAM_ALLOCATE_BANK_NUM,
        .dtcmBankNum = APP_ITCM_ALLOCATE_BANK_NUM,
        .itcmBankNum = APP_DTCM_ALLOCATE_BANK_NUM,
    };

    PRINTF("\r\nAllocate on-chip ram:\r\n");
    PRINTF("\r\n   OCRAM bank numbers %d\r\n", ramAllocate.ocramBankNum);
    PRINTF("\r\n   DTCM  bank numbers %d\r\n", ramAllocate.dtcmBankNum);
    PRINTF("\r\n   ITCM  bank numbers %d\r\n", ramAllocate.itcmBankNum);

    if (FLEXRAM_AllocateRam(&ramAllocate) != kStatus_Success)
    {
        PRINTF("\r\nAllocate on-chip ram fail\r\n");
        return kStatus_Fail;
    }
    else
    {
        PRINTF("\r\nAllocate on-chip ram success\r\n");
    }

    return kStatus_Success;
}

static void OCRAM_Access(void)
{
    uint32_t *ocramAddr = (uint32_t *)APP_FLEXRAM_OCRAM_START_ADDR;

    /* enable FLEXRAM OCRAM access error interrupt and OCRAM magic address match interrupt */
    FLEXRAM_EnableInterruptSignal(APP_FLEXRAM, kFLEXRAM_OCRAMAccessError | kFLEXRAM_OCRAMMagicAddrMatch);
    /* config ocram magic address
    * read access hit magic address will generate interrupt
    */
    FLEXRAM_SetOCRAMMagicAddr(APP_FLEXRAM, (uint16_t)APP_FLEXRAM_OCRAM_MAGIC_ADDR, kFLEXRAM_Write);

    for (;;)
    {
        *ocramAddr = 0xCCU;
        /* Synchronizes the execution stream with memory accesses */
        APP_DSB();
        APP_ISB();
        /* check ocram magic addr match event */
        if (s_flexram_ocram_magic_addr_match)
        {
            PRINTF("\r\nOCRAM Magic address 0x%x match.\r\n", ocramAddr);
            s_flexram_ocram_magic_addr_match = false;
        }
        /* check ocram access error event */
        if (s_flexram_ocram_access_error_match)
        {
            s_flexram_ocram_access_error_match = false;
            PRINTF("\r\nOCRAM access to 0x%x boundary.\r\n", ocramAddr);
            break;
        }

        ocramAddr++;
    }
}

static void DTCM_Access(void)
{
    uint32_t *dtcmAddr = (uint32_t *)APP_FLEXRAM_DTCM_START_ADDR;

    /* config ocram magic address
    * read access hit magic address will generate interrupt
    */
    FLEXRAM_SetDTCMMagicAddr(APP_FLEXRAM, (uint16_t)APP_FLEXRAM_DTCM_MAGIC_ADDR, kFLEXRAM_Write);
    /* enable FLEXRAM DTCM access error interrupt and DTCM magic address match interrupt */
    FLEXRAM_EnableInterruptSignal(APP_FLEXRAM, kFLEXRAM_DTCMMagicAddrMatch);
    for (;;)
    {
        *dtcmAddr = 0xDDU;
        /* Synchronizes the execution stream with memory accesses */
        APP_DSB();
        /* check ocram magic addr match event */
        if (s_flexram_dtcm_magic_addr_match)
        {
            PRINTF("\r\nDTCM Magic address 0x%x match.\r\n", dtcmAddr);
            s_flexram_dtcm_magic_addr_match = false;
            break;
        }

        dtcmAddr++;
    }
}

static void ITCM_Access(void)
{
    uint32_t *itcmAddr = (uint32_t *)APP_FLEXRAM_ITCM_START_ADDR;

    /* config ocram magic address
    * read access hit magic address will generate interrupt
    */
    FLEXRAM_SetITCMMagicAddr(APP_FLEXRAM, (uint16_t)APP_FLEXRAM_ITCM_MAGIC_ADDR, kFLEXRAM_Write);
    /* clear interrupt status first */
    FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_ITCMMagicAddrMatch);
    /* reset flag */
    s_flexram_itcm_magic_addr_match = false;
    /* enable FLEXRAM ITCM access error interrupt and ITCM magic address match interrupt */
    FLEXRAM_EnableInterruptSignal(APP_FLEXRAM, kFLEXRAM_ITCMMagicAddrMatch);

    for (;;)
    {
        *itcmAddr = 0xEEU;
        /* Synchronizes the execution stream with memory accesses */
        APP_DSB();
        /* check ocram magic addr match event */
        if (s_flexram_itcm_magic_addr_match)
        {
            s_flexram_itcm_magic_addr_match = false;
            PRINTF("\r\nITCM Magic address 0x%x match.\r\n", itcmAddr);
            break;
        }

        itcmAddr++;
    }
}
