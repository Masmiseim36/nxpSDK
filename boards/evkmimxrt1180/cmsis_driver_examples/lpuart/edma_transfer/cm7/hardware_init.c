/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_edma.h"
#include "fsl_trdc.h"
#include "fsl_ele_base_api.h"
/*${header:end}*/

/*${macro:start}*/
/* When CM33 set TRDC, CM7 must NOT require TRDC ownership from ELE */
#define CM33_SET_TRDC 0U

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

/*${variable:start}*/
edma_config_t edmaConfig            = {0};
edma_channel_config_t channelConfig = {
    .enableMasterIDReplication = true,
    .securityLevel             = kEDMA_ChannelSecurityLevelSecure,
    .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged,
};

/*${variable:end}*/
/*${function:start}*/
void TRDC_EDMA3_ResetPermissions()
{
    uint8_t i, j;
    /* Set the master domain access configuration for eDMA3 */
    trdc_non_processor_domain_assignment_t edma3Assignment;
    (void)memset(&edma3Assignment, 0, sizeof(edma3Assignment));
    edma3Assignment.domainId = 0x7U;
    /* Use the bus master's privileged/user attribute directly */
    edma3Assignment.privilegeAttr = kTRDC_MasterPrivilege;
    /* Use the bus master's secure/nonsecure attribute directly */
    edma3Assignment.secureAttr = kTRDC_MasterSecure;
    /* Use the DID input as the domain indentifier */
    edma3Assignment.bypassDomainId = true;
    edma3Assignment.lock           = false;
    TRDC_SetNonProcessorDomainAssignment(TRDC1, kTRDC1_MasterDMA3, &edma3Assignment);

    /* Enable all access modes for MBC and MRC. */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(TRDC1, &hwConfig);

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
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

#if !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U))

    status_t sts;

    /* Get ELE FW status */
    do
    {
        uint32_t ele_fw_sts;
        sts = ELE_BaseAPI_GetFwStatus(MU_RT_S3MUA, &ele_fw_sts);
    } while (sts != kStatus_Success);

    /* Release TRDC A to CM7 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_AON_ID, ELE_CORE_CM7_ID);
    } while (ELE_IS_FAILED(sts));

    /* Release TRDC W to CM7 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_WAKEUP_ID, ELE_CORE_CM7_ID);
    } while (ELE_IS_FAILED(sts));

    TRDC_EDMA3_ResetPermissions();

#endif /* !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U)) */

    EDMA_GetDefaultConfig(&edmaConfig);
    edmaConfig.enableMasterIdReplication           = true;
    edmaConfig.channelConfig[RTE_USART1_DMA_TX_CH] = &channelConfig;
    edmaConfig.channelConfig[RTE_USART1_DMA_RX_CH] = &channelConfig;
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART1_DMA_TX_CH, RTE_USART1_DMA_TX_PERI_SEL);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART1_DMA_RX_CH, RTE_USART1_DMA_RX_PERI_SEL);
}

uint32_t LPUART1_GetFreq(void)
{
    return BOARD_DebugConsoleSrcFreq();
}
/*${function:end}*/
