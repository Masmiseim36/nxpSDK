/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_edma.h"
#include "fsl_trdc.h"
#include "fsl_ele_base_api.h"
#include "app.h"

/*${header:end}*/

/*${macro:start}*/
#define ELE_TRDC_AON_ID    0x74
#define ELE_TRDC_WAKEUP_ID 0x78
#define ELE_CORE_CM33_ID   0x1
#define ELE_CORE_CM7_ID    0x2

/*
 * Set ELE_STICK_FAILED_STS to 0 when ELE status check is not required,
 * which is useful when debug reset, where the core has already get the
 * TRDC ownership at first time and ELE is not able to release TRDC
 * ownership again for the following TRDC ownership request.
 */
#define ELE_STICK_FAILED_STS 1

#if ELE_STICK_FAILED_STS
#define ELE_IS_FAILED(x) (x != kStatus_Success)
#else
#define ELE_IS_FAILED(x) false
#endif
/*${macro:end}*/

/*${function:start}*/
static void TRDC_EDMA4_ResetPermissions(void)
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

void BOARD_InitHardware(void)
{
    status_t sts;
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_EnableClock(kCLOCK_Trdc);

    /* Get ELE FW status */
    do
    {
        uint32_t ele_fw_sts;
        sts = ELE_BaseAPI_GetFwStatus(MU_RT_S3MUA, &ele_fw_sts);
    } while (sts != kStatus_Success);

    /* Release TRDC A to CM33 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_AON_ID, ELE_CORE_CM33_ID);
    } while (ELE_IS_FAILED(sts));

    /* Release TRDC W to CM33 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_WAKEUP_ID, ELE_CORE_CM33_ID);
    } while (ELE_IS_FAILED(sts));

    TRDC_EDMA4_ResetPermissions();
}
/*${function:end}*/
