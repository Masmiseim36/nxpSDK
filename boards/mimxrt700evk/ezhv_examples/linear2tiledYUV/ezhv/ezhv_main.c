/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "ezhv_api.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief shared space between primary core and ezhv, 
 *  default space size is 256Bytes.
 */
#define EZHV_SHARED_DATA_ADDR    0x2410FF00U

typedef void (*ezhv_func_ptr)(uint32_t paraAddr);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
ezhv_func_ptr EZHV_apiTable[] = {
#if (EZHV_LINEAR_TO_TILE_EN)
    &EZHV_Linear2Tile,
#endif
#if (EZHV_DVP_EN)
    &EZHV_FlexioCameraDvp,
#endif
#if (EZHV_MIP_EN)
    &EZHV_MiP,
#endif
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void MachineSoftInt_Handler(void)
{
    return;
}

int main(int argc, char *argv[])
{
    EZHV_Para_t *pstPara = (EZHV_Para_t *)EZHV_SHARED_DATA_ADDR;

	// PRINTF("[EZHV]: Running\r\n");

    /* Enable global interrupts. */
    EnableGlobalIRQ((uint32_t)kEZHV_Status_Mie);
    /* Enable machine external interrupt. */
    EnableMachineModeInt(kEZHV_Mie_Msie);
    while(1)
    {
        wfi();
        EZHV_apiTable[pstPara->apiIdx]((uint32_t)&pstPara->paraAddr);
    }

    return 0;
}
