/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*${function:start}*/
static void TRDC_EDMA3_EDMA4_ResetPermissions(void)
{
    uint8_t i, j;

    /* Set the master domain access configuration for eDMA4 */
    trdc_non_processor_domain_assignment_t edmaAssignment;

    (void)memset(&edmaAssignment, 0, sizeof(edmaAssignment));
    edmaAssignment.domainId       = 0x7U;
    edmaAssignment.privilegeAttr  = kTRDC_MasterPrivilege;
    edmaAssignment.secureAttr     = kTRDC_ForceSecure;
    edmaAssignment.bypassDomainId = true;
    edmaAssignment.lock           = false;

    TRDC_SetNonProcessorDomainAssignment(TRDC1, kTRDC1_MasterDMA3, &edmaAssignment);
    TRDC_SetNonProcessorDomainAssignment(TRDC2, kTRDC2_MasterDMA4, &edmaAssignment);

    /* Enable all access modes for MBC and MRC of TRDCA and TRDCW */
    trdc_hardware_config_t hwConfig;
    trdc_memory_access_control_config_t memAccessConfig;

    (void)memset(&memAccessConfig, 0, sizeof(memAccessConfig));
    memAccessConfig.nonsecureUsrX  = 1U;
    memAccessConfig.nonsecureUsrW  = 1U;
    memAccessConfig.nonsecureUsrR  = 1U;
    memAccessConfig.nonsecurePrivX = 1U;
    memAccessConfig.nonsecurePrivW = 1U;
    memAccessConfig.nonsecurePrivR = 1U;
    memAccessConfig.secureUsrX     = 1U;
    memAccessConfig.secureUsrW     = 1U;
    memAccessConfig.secureUsrR     = 1U;
    memAccessConfig.securePrivX    = 1U;
    memAccessConfig.securePrivW    = 1U;
    memAccessConfig.securePrivR    = 1U;

    TRDC_GetHardwareConfig(TRDC1, &hwConfig);
    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MrcSetMemoryAccessConfig(TRDC1, &memAccessConfig, i, j);
        }
    }

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC1, &memAccessConfig, i, j);
        }
    }

    /* Set TRDC1(A) secure access for Domain 7(eDMA domain ID), MBC 0, MEM0 (AIPS1->SAI1) */
    TRDC_MbcNseClearAll(TRDC1, 0U, 1UL << 7U, 0x1 << 0);

    /* Set TRDC1(A) secure access for Domain 7(eDMA domain ID), MBC 1, MEM1 (CM33 System TCM) */
    TRDC_MbcNseClearAll(TRDC1, 1U, 1UL << 7U, 0x1 << 1);

    TRDC_GetHardwareConfig(TRDC2, &hwConfig);
    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MrcSetMemoryAccessConfig(TRDC2, &memAccessConfig, i, j);
        }
    }

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC2, &memAccessConfig, i, j);
        }
    }

    /* Set TRDC2(W) secure access for Domain 7(eDMA domain ID), MBC 1, MEM0(AIPS3->AIPS3_MEGA->PDM) */
    TRDC_MbcNseClearAll(TRDC2, 1U, 1UL << 7U, 0x1 << 0);

    /* Set TRDC2(W) secure access for Domain 7(eDMA domain ID), MRC 5, all region(SEMC->SDRAM) */
    TRDC_MrcEnableDomainNseUpdate(TRDC2, 5U, 1UL << 7U, true);
    TRDC_MrcRegionNseClear(TRDC2, 5U, 0xFFFFU);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    BOARD_RequestTRDC(true, true, false);
    TRDC_EDMA3_EDMA4_ResetPermissions();
}
/*${function:end}*/
