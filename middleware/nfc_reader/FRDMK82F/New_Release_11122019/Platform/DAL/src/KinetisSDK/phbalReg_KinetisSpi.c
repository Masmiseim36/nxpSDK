/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  RS: Generated 24. Jan 2017
*
*/



#include "phDriver.h"
#include "BoardSelection.h"

#include "fsl_pit.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_dspi.h"

#ifndef PHDRIVER_KSDK_SPI_POLLING
#include <fsl_dspi_freertos.h>
#endif

#define PHBAL_REG_KINETIS_SPI_ID               0x0FU       /**< ID for Kinetis SPI BAL component */


#ifndef PHDRIVER_KSDK_SPI_POLLING
dspi_rtos_handle_t g_masterHandle;
#endif

#define RX_BUFFER_SIZE_MAX                     272U /* Receive Buffer size while exchange */

static void phbalReg_SpiInit(void);


phStatus_t phbalReg_Init(
                         void * pDataParams,
                         uint16_t wSizeOfDataParams
                         )
{
    dspi_master_config_t g_masterConfig;

    if ( (pDataParams == NULL) || (sizeof(phbalReg_Type_t) != wSizeOfDataParams))
    {
        return (PH_DRIVER_ERROR | PH_COMP_DRIVER);
    }

    ((phbalReg_Type_t *)pDataParams)->wId      = PH_COMP_DRIVER | PHBAL_REG_KINETIS_SPI_ID;
    ((phbalReg_Type_t *)pDataParams)->bBalType = PHBAL_REG_TYPE_SPI;

    memset(&g_masterConfig, 0, sizeof(dspi_master_config_t));

    g_masterConfig.whichCtar = kDSPI_Ctar0;
    g_masterConfig.ctarConfig.baudRate = PHDRIVER_KSDK_SPI_DATA_RATE;
    g_masterConfig.ctarConfig.bitsPerFrame = 8U;
    g_masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    g_masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    g_masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    g_masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / PHDRIVER_KSDK_SPI_DATA_RATE;
    g_masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / PHDRIVER_KSDK_SPI_DATA_RATE;
    g_masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / PHDRIVER_KSDK_SPI_DATA_RATE;
    g_masterConfig.whichPcs = kDSPI_Pcs0;
    g_masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
    g_masterConfig.enableContinuousSCK = false;
    g_masterConfig.enableRxFifoOverWrite = false;
    g_masterConfig.enableModifiedTimingFormat = false;
    g_masterConfig.samplePoint = kDSPI_SckToSin0Clock;

    phbalReg_SpiInit();

#ifdef PHDRIVER_KSDK_SPI_POLLING
    /* Initialize the DSPI peripheral */
    DSPI_MasterInit(PHDRIVER_KSDK_SPI_MASTER, &g_masterConfig, CLOCK_GetFreq(PHDRIVER_KSDK_SPI_CLK_SRC));
#else
    DSPI_RTOS_Init(&g_masterHandle, PHDRIVER_KSDK_SPI_MASTER, &g_masterConfig, CLOCK_GetFreq(PHDRIVER_KSDK_SPI_CLK_SRC));
#endif

    return PH_DRIVER_SUCCESS;
}

phStatus_t phbalReg_Exchange(
                             void * pDataParams,
                             uint16_t wOption,
                             uint8_t * pTxBuffer,
                             uint16_t wTxLength,
                             uint16_t wRxBufSize,
                             uint8_t * pRxBuffer,
                             uint16_t * pRxLength
                             )
{
    phStatus_t status = PH_DRIVER_SUCCESS;
    uint8_t * pRxBuf;
    status_t dspiStatus;
    dspi_transfer_t g_masterXfer;
    uint8_t g_dummyBuffer[RX_BUFFER_SIZE_MAX];

    memset(&g_masterXfer, 0, sizeof(dspi_transfer_t));

    if(pRxBuffer == NULL)
    {
        pRxBuf = g_dummyBuffer;
    }
    else
    {
        pRxBuf = pRxBuffer;
    }

    if(pTxBuffer == NULL)
    {
    	wTxLength = wRxBufSize;
    	g_dummyBuffer[0] = 0xFF;
    	pTxBuffer = g_dummyBuffer;
    }

    /* Set up the transfer */
    g_masterXfer.txData = pTxBuffer;
    g_masterXfer.rxData = pRxBuf;
    g_masterXfer.dataSize = wTxLength;
    g_masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    /* Start transfer */
#ifdef PHDRIVER_KSDK_SPI_POLLING
    dspiStatus = DSPI_MasterTransferBlocking(PHDRIVER_KSDK_SPI_MASTER, &g_masterXfer);
#else
    dspiStatus = DSPI_RTOS_Transfer(&g_masterHandle, &g_masterXfer);
#endif
    if (dspiStatus != kStatus_Success)
    {
        return (PH_DRIVER_FAILURE | PH_COMP_DRIVER);
    }

    if (pRxLength != NULL)
    {
        *pRxLength = wTxLength;
    }

    return status;
}

phStatus_t phbalReg_SetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t wValue
                              )
{
    return PH_DRIVER_SUCCESS;
}

phStatus_t phbalReg_GetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t * pValue
                              )
{
    return PH_DRIVER_SUCCESS;
}


static void phbalReg_SpiInit(void)
{
    port_pin_config_t pinConfig =
    {
        .pullSelect = kPORT_PullDisable,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable = kPORT_OpenDrainDisable,
        .driveStrength = kPORT_HighDriveStrength,
        .mux = kPORT_MuxAsGpio,
        .lockRegister = kPORT_UnlockRegister
    };

    /* SPI Configuration */
    NVIC_SetPriority(PHDRIVER_KSDK_SPI_IRQ, DSPI_IRQ_PRIORITY);

    /* Configure SSP pins (SCK, MOSI and MISO) */
    pinConfig.pullSelect = kPORT_PullDisable;
    pinConfig.mux = kPORT_MuxAlt2;

    CLOCK_EnableClock(ENABLE_PORT_SSP_1);
    PORT_SetPinConfig(PORT_SSP_1, FIRST_PINNUM_SSP, &pinConfig);
    CLOCK_EnableClock(ENABLE_PORT_SSP_2);
    PORT_SetPinConfig(PORT_SSP_2, SECOND_PINNUM_SSP, &pinConfig);
    CLOCK_EnableClock(ENABLE_PORT_SSP_3);
    PORT_SetPinConfig(PORT_SSP_3, THIRD_PINNUM_SSP, &pinConfig);
}
