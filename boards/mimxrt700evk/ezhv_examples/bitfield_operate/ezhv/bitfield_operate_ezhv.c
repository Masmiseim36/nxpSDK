/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "pin_mux.h"
#include "fsl_sema42.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "riscv_xzbf.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef PRINTF
#undef PRINTF
#define PRINTF EZHV_PRINTF
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void EZHV_PRINTF(const char *ptr, ...)
{
    va_list ap;
    SEMA42_Lock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE, DEMO_SEMA42_CORE_ID_EZHV);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE);
}

/*! @brief Main function */
int main(void)
{
    uint32_t srcData = 0U;
    uint32_t dstData = 0U;

    BOARD_InitHardware();

    SEMA42_Init(DEMO_SEMA42_INSTANCE);

    PRINTF("Bitfield operate demo - message from ezhv core side.\r\n");
    PRINTF("Press any key to start the project.\r\n");
    GETCHAR();


    PRINTF("\r\n========== Bit field extract instructions test ==========\r\n");
    srcData = 0x87654321U;
    srcData = XZBF_Bfextiu(srcData, 7U, 8U);
    PRINTF("The extracted data after execute the bfextiu instruction is 0x%x.\r\n", srcData);

    srcData = 0x87654321U;
    srcData = XZBF_Bfextu(srcData, ((7U << 5U) | 8U));
    PRINTF("The extracted data after execute the bfextu instruction is 0x%x.\r\n", srcData);


    PRINTF("\r\n========== Bit field insert instructions test ==========\r\n");
    srcData = 0x54U;
    dstData = 0x87600321U;
    dstData = XZBF_Bfinsi(dstData, srcData, 7U, 12U);
    PRINTF("The destination data after execute the bfinsi instruction is 0x%x.\r\n", dstData);

    srcData = 0x54U;
    dstData = 0x87600321U;
    dstData = XZBF_Bfins(dstData, srcData, ((7U << 5U) | 12U));
    PRINTF("The destination data after execute the bfins instruction is 0x%x.\r\n", dstData);


    PRINTF("\r\n========== Bit field set instructions test ==========\r\n");
    srcData = 0x87654321U;
    srcData = XZBF_Bfseti(srcData, 7U, 12U);
    PRINTF("The destination data after execute the bfseti instruction is 0x%x.\r\n", srcData);

    srcData = 0x87654321U;
    srcData = XZBF_Bfset(srcData, ((7U << 5U) | 12U));
    PRINTF("The destination data after execute the bfset instruction is 0x%x.\r\n", srcData);


    PRINTF("\r\n========== Bit field clear instructions test ==========\r\n");
    srcData = 0x87654321U;
    srcData = XZBF_Bfclri(srcData, 7U, 12U);
    PRINTF("The destination data after execute the bfclri instruction is 0x%x.\r\n", srcData);

    srcData = 0x87654321U;
    srcData = XZBF_Bfclr(srcData, ((7U << 5U) | 12U));
    PRINTF("The destination data after execute the bfclr instruction is 0x%x.\r\n", srcData);

    while(1)
    {
    }
}
