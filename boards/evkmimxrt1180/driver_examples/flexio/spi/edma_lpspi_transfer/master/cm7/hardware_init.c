/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*${macro:start}*/
/* When CM33 set TRDC, CM7 must NOT require TRDC ownership from ELE */
#define CM33_SET_TRDC 0U
/*${macro:end}*/

/*${function:start}*/
#if !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U))
static void TRDC_EDMA4_ResetPermissions(void)
{
#define EDMA_DID 0x7U
    uint8_t i, j;

    /* Set the master domain access configuration for eDMA4 */
    trdc_non_processor_domain_assignment_t edma4Assignment;

    (void)memset(&edma4Assignment, 0, sizeof(edma4Assignment));
    edma4Assignment.domainId       = EDMA_DID;
    edma4Assignment.privilegeAttr  = kTRDC_MasterPrivilege;
    edma4Assignment.secureAttr     = kTRDC_ForceSecure;
    edma4Assignment.bypassDomainId = true;
    edma4Assignment.lock           = false;
    TRDC_SetNonProcessorDomainAssignment(TRDC2, kTRDC2_MasterDMA4, &edma4Assignment);

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
}
#endif /* !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U)) */

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

#if !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U))

    BOARD_RequestTRDC(true, true, false);

    TRDC_EDMA4_ResetPermissions();

#endif /* !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U)) */
}
/*${function:end}*/
