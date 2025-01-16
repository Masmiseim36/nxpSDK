/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdarg.h>
#include "fsl_sema42.h"
#include "app.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

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
    BOARD_InitHardware();

    /* SEMA42 init */
    SEMA42_Init(DEMO_SEMA42_INSTANCE);

    uint8_t *src = (uint8_t *)(DEMO_DTCM_1);
    uint8_t *dst = (uint8_t *)(DEMO_DTCM_2);

    uint8_t(*addr)[DEMO_MEM_LEN];
    addr = (uint8_t(*)[DEMO_MEM_LEN])DEMO_DTCM_1;

    PRINTF("Memory to memory copy demo - message from ezhv core side.\r\n");
    PRINTF("Press any key to start the program.\r\n");
    GETCHAR();

    for (uint8_t index = 0; index < DEMO_MEM_LEN; ++index)
    {
        (*addr)[index] = index;
        PRINTF("DTCM_1 DST[%d] = %d\r\n", index, (*addr)[index]);
    }
    PRINTF("\r\n");

    /* Copy data from DTCM_1 to DTCM_2. */
    memcpy((void *)dst, (const char *)src, DEMO_MEM_LEN);
    for (uint32_t index = 0; index < DEMO_MEM_LEN; ++index)
    {
        PRINTF("DTCM_2 DST[%d] = %d\r\n", index, dst[index]);
    }
    PRINTF("\r\n");

    /* Copy data from DTCM_2 to SRAM. */
    src = (uint8_t *)(DEMO_DTCM_2);
    dst = (uint8_t *)(DEMO_SRAM);

    memcpy((void *)dst, (const char *)src, DEMO_MEM_LEN);
    for (uint32_t index = 0; index < DEMO_MEM_LEN; ++index)
    {
        PRINTF("SRAM DST[%d] = %d\r\n", index, dst[index]);
        dst[index]++;
    }
    PRINTF("\r\n");

    /* Copy data from SRAM to DTCM_1 */
    src = (uint8_t *)(DEMO_SRAM);
    dst = (uint8_t *)(DEMO_DTCM_1);

    memcpy((void *)dst, (const char *)src, DEMO_MEM_LEN);
    for (uint32_t index = 0; index < DEMO_MEM_LEN; ++index)
    {
        PRINTF("DTCM_1 DST[%d] = %d\r\n", index, dst[index]);
    }

    PRINTF("Memory to memory copy demo execute success.\r\n");

    while (1)
        ;
}
