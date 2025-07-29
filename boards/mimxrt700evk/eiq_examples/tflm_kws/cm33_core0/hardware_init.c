/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_init.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pmic_support.h"

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#elif defined(__ICCARM__)
#elif defined(__MCUXPRESSO)
#elif defined(__GNUC__)
extern uint32_t __modeldata_start__[];
extern uint32_t __modeldata_end__[];
extern uint32_t __MDATA_ROM[];

void SystemInitHook(){
    // Copy initialized model data from Flash to RAM
    uint32_t *pDataSrc, *pDataDest;
    pDataSrc  = (uint32_t *)__MDATA_ROM;
    pDataDest = (uint32_t *)__modeldata_start__;
    while (pDataDest < __modeldata_end__)
   {
        *pDataDest++ = *pDataSrc++;
    }
}
#endif

void BOARD_Init()
{
    BOARD_InitPins();

    BOARD_InitAHBSC();
    BOARD_ConfigMPU();
    
    BOARD_InitPmicPins();
    BOARD_InitPmic();
    BOARD_SetPmicVdd2Voltage(1100000U); /* 1.1v for 325MHz clock. */

    // Disable LDO
    POWER_SetVddnSupplySrc(kVddSrc_PMIC);
    POWER_SetVdd1SupplySrc(kVddSrc_PMIC);
    POWER_SetVdd2SupplySrc(kVddSrc_PMIC);
    POWER_ApplyPD();

    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_APD_NPU);
    POWER_DisablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();
}
