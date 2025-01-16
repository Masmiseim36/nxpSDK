/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_power.h"
#include "fsl_xspi.h"
#include "fsl_nor_flash.h"
#include "fsl_xspi_nor_flash.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define FLASH_PAGE_SIZE                 256
#define NOR_FLASH_START_ADDRESS (XSPI0_AMBA_BASE)

xspi_memory_config_t xspiMemConfig =
{
    .enableXspiDoze = false,
    .ptrCustomLut = NULL,
    .numPadUsed = kXSPI_8PAD,
    .enableClknPad = false,
    .sampleClkConfig.sampleClkSource = kXSPI_SampleClkFromExternalDQS,  /*!< Device support Data strobe signal.  */
    .sampleClkConfig.enableDQSLatency = false,
    .sampleClkConfig.dllConfig.dllMode = kXSPI_AutoUpdateMode,
    .sampleClkConfig.dllConfig.useRefValue = true,
    .sampleClkConfig.dllConfig.enableCdl8 = false,
    .addrMode = kXSPI_DeviceByteAddressable,
    .xspiRootClk = 400000000,  /*!< 400MHz */
    .tgId = kXSPI_TargetGroup0,
    
    .ptrXspiNorAhbAccessConfig = NULL,
    .ptrXspiNorIPAccessConfig = NULL,
};

nor_config_t norConfig = {
  .memControlConfig = &xspiMemConfig,
  .driverBaseAddr = (void *)XSPI0_BASE,
};
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
