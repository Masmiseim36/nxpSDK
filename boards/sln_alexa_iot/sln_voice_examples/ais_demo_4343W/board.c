/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */

#include "fsl_iomuxc.h"

#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
#include "pdm_pcm_definitions.h"
#include "sln_cfg_file.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_tfa9xxx.h"
#if defined(USE_TFA9894_PUI) && USE_TFA9894_PUI
#include "tfa_config_TFA9894N2A1_48kHz_PUI.h"
#else
#include "tfa_config_TFA9894N2.h"
#endif
#endif /* SDK_SAI_BASED_COMPONENT_USED */

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern void __base_SRAM_DTC(void);
extern void __top_SRAM_DTC(void);
extern void __base_SRAM_ITC(void);
extern void __top_SRAM_ITC(void);
extern void __base_SRAM_OC_NON_CACHEABLE(void);
extern void __top_SRAM_OC_NON_CACHEABLE(void);
extern void __base_SRAM_OC_CACHEABLE(void);
extern void __top_SRAM_OC_CACHEABLE(void);

__attribute__((section(".vectorTableRam"), aligned(0x400))) uint32_t g_vectorTable[256] = {0};

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t __Vectors[];
extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit;
#define __VECTOR_TABLE __Vectors
#define __StackTop     Image$$ARM_LIB_STACK$$ZI$$Limit
#elif defined(__MCUXPRESSO)
extern uint32_t __Vectors[];
extern void _vStackTop(void);
#define __VECTOR_TABLE __Vectors
#define __StackTop     _vStackTop
#elif defined(__ICCARM__)
extern uint32_t __vector_table[];
extern uint32_t CSTACK$$Limit;
#define __VECTOR_TABLE __vector_table
#define __StackTop     CSTACK$$Limit
#elif defined(__GNUC__)
extern uint32_t __StackTop;
extern uint32_t __Vectors[];
#define __VECTOR_TABLE __Vectors
#endif

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t g_i2cHandle = {0};
#endif
#endif

#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t dummy_txbuffer[32], 32);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t dummy_rxbuffer[32], 32);

tfa9xxx_config_t tfa9xxxConfig = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = 10000000U},
    .slaveAddress = TFA9XXX_I2C_ADDR_0,
    .protocol     = kTFA9XXX_BusI2S,
    .format       = {.sampleRate = kTFA9XXX_AudioSampleRate48KHz, .bitWidth = kTFA9XXX_AudioBitWidth16bit},
    .tfaContainer = tfa_container_bin,
    .deviceIndex  = 0,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_TFA9XXX, .codecDevConfig = &tfa9xxxConfig};
#endif /* SDK_SAI_BASED_COMPONENT_USED */

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint8_t get_mpu_size(uint32_t ramSize)
{
    uint8_t mpuRegSize = 0U;

    if (ramSize == 0U)
    {
        mpuRegSize = 0U;
    }
    else if (ramSize <= (32 * 1024))
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_32KB;
    }
    else if (ramSize <= (64 * 1024))
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_64KB;
    }
    else if (ramSize <= (128 * 1024))
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_128KB;
    }
    else if (ramSize <= (256 * 1024))
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_256KB;
    }
    else
    {
        mpuRegSize = ARM_MPU_REGION_SIZE_512KB;
    }

    return mpuRegSize;
}

/* Get debug console frequency. */
uint32_t BOARD_DebugConsoleSrcFreq(void)
{
    uint32_t freq;

    /* To make it simple, we assume default PLL and divider settings, and the only variable
       from application is use PLL3 source or OSC source */
    if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
    {
        freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }
    else
    {
        freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
    }

    return freq;
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = BOARD_DebugConsoleSrcFreq();

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t lpi2cConfig = {0};

    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Hz = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&lpi2cConfig);
    LPI2C_MasterInit(base, &lpi2cConfig, BOARD_CODEC_I2C_CLOCK_FREQ);
    LPI2C_MasterTransferCreateHandle(base, &g_i2cHandle, NULL, NULL);
}

status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subAddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize)
{
    status_t reVal;

    /* Send master blocking data to slave */
    reVal = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (kStatus_Success == reVal)
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, &subAddress, subAddressSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterSend(base, txBuff, txBuffSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }
    }

    return reVal;
}

status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subAddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize)
{
    status_t reVal;

    reVal = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (kStatus_Success == reVal)
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, &subAddress, subAddressSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterRepeatedStart(base, deviceAddress, kLPI2C_Read);

        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterReceive(base, rxBuff, rxBuffSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }
    }
    return reVal;
}

status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subAddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(base, deviceAddress, subAddress, subAddressSize, txBuff, txBuffSize);
}

status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subAddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize)
{
    status_t reVal;

    reVal = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (kStatus_Success == reVal)
    {
        while (LPI2C_MasterGetStatusFlags(base) & kLPI2C_MasterNackDetectFlag)
        {
        }

        reVal = LPI2C_MasterSend(base, &subAddress, subAddressSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        /* SCCB does not support LPI2C repeat start, must stop then start. */
        reVal = LPI2C_MasterStop(base);

        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Read);

        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterReceive(base, rxBuff, rxBuffSize);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }

        reVal = LPI2C_MasterStop(base);
        if (reVal != kStatus_Success)
        {
            return reVal;
        }
    }
    return reVal;
}

void BOARD_Accel_I2C_Init(void)
{
    BOARD_LPI2C_Init(BOARD_ACCEL_I2C_BASEADDR, BOARD_ACCEL_I2C_CLOCK_FREQ);
}

status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff)
{
    uint8_t data = (uint8_t)txBuff;

    return BOARD_LPI2C_Send(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, &data, 1);
}

status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}

void BOARD_Codec_I2C_Init(void)
{
    CLOCK_SetMux(kCLOCK_Lpi2cMux, BOARD_CODEC_I2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, BOARD_CODEC_I2C_CLOCK_SOURCE_DIVIDER);
    BOARD_LPI2C_Init(BOARD_CODEC_I2C_BASEADDR, BOARD_CODEC_I2C_CLOCK_FREQ);
#if defined(FSL_RTOS_FREE_RTOS)
    NVIC_SetPriority(BOARD_CODEC_I2C_IRQN, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
#endif
}

status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CODEC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

void BOARD_Camera_I2C_Init(void)
{
    CLOCK_SetMux(kCLOCK_Lpi2cMux, BOARD_CAMERA_I2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, BOARD_CAMERA_I2C_CLOCK_SOURCE_DIVIDER);
    BOARD_LPI2C_Init(BOARD_CAMERA_I2C_BASEADDR, BOARD_CAMERA_I2C_CLOCK_FREQ);
}

status_t BOARD_Camera_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_Send(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                            txBuffSize);
}

status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_Receive(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                               rxBuffSize);
}

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_LPI2C_SendSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                                txBuffSize);
}

status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_LPI2C_ReceiveSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                                   rxBuffSize);
}
#endif /* SDK_I2C_BASED_COMPONENT_USED */

#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
void BOARD_SAI_Enable_Mclk_Output(I2S_Type *base, bool enable)
{
    uint32_t mclk_dir_mask = 0;

    if (SAI1 == base)
        mclk_dir_mask = IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
    else if (SAI2 == base)
        mclk_dir_mask = IOMUXC_GPR_GPR1_SAI2_MCLK_DIR_MASK;
    else if (SAI3 == base)
        mclk_dir_mask = IOMUXC_GPR_GPR1_SAI3_MCLK_DIR_MASK;
    else
        return;

    if (enable)
    {
        IOMUXC_GPR->GPR1 |= mclk_dir_mask;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~mclk_dir_mask);
    }
}

void BOARD_SAI_Init(sai_init_handle_t saiInitHandle)
{
    sai_config_t saiConfig = {0};
    sai_transfer_t txfer;
    sai_transfer_t rxfer;

    sai_transfer_format_t saiAmpFormat = {0};

#if USE_ALEXA_SOUND_PROMPT || USE_TFA9894 /* We can only use 48KHz right now due to limitations of TFA9894 */
    saiAmpFormat.bitWidth      = kSAI_WordWidth16bits;
    saiAmpFormat.sampleRate_Hz = kSAI_SampleRate48KHz;
#elif USE_16BIT_PCM
    saiAmpFormat.bitWidth      = kSAI_WordWidth16bits;
    saiAmpFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#elif USE_32BIT_PCM
    saiAmpFormat.bitWidth      = kSAI_WordWidth32bits;
    saiAmpFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#endif
    saiAmpFormat.channel            = 0U;
    saiAmpFormat.protocol           = kSAI_BusLeftJustified;
    saiAmpFormat.isFrameSyncCompact = true;
    saiAmpFormat.stereo             = kSAI_MonoLeft;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    saiAmpFormat.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif

    /*Clock setting for SAI1*/
    CLOCK_SetMux(kCLOCK_Sai3Mux, BOARD_AMP_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai3PreDiv, BOARD_AMP_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai3Div, BOARD_AMP_SAI_CLOCK_SOURCE_DIVIDER);

    BOARD_SAI_Enable_Mclk_Output(BOARD_AMP_SAI, true);

    EDMA_CreateHandle(saiInitHandle.amp_dma_tx_handle, DMA0, BOARD_AMP_SAI_EDMA_TX_CH);
    EDMA_CreateHandle(saiInitHandle.amp_dma_rx_handle, DMA0, BOARD_AMP_SAI_EDMA_RX_CH);
    DMAMUX_SetSource(DMAMUX, BOARD_AMP_SAI_EDMA_TX_CH, (uint8_t)BOARD_AMP_SAI_EDMA_TX_REQ);
    DMAMUX_SetSource(DMAMUX, BOARD_AMP_SAI_EDMA_RX_CH, (uint8_t)BOARD_AMP_SAI_EDMA_RX_REQ);
    DMAMUX_EnableChannel(DMAMUX, BOARD_AMP_SAI_EDMA_TX_CH);
    DMAMUX_EnableChannel(DMAMUX, BOARD_AMP_SAI_EDMA_RX_CH);

    /* Initialize SAI Tx */
    SAI_TxGetDefaultConfig(&saiConfig);
    saiConfig.protocol = kSAI_BusI2S;
    SAI_TxInit(BOARD_AMP_SAI, &saiConfig);

    //    /* Initialize SAI Rx */
    SAI_RxGetDefaultConfig(&saiConfig);
    saiConfig.protocol = kSAI_BusI2S;
    SAI_RxInit(BOARD_AMP_SAI, &saiConfig);

    SAI_TransferTxCreateHandleEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_tx_handle, saiInitHandle.sai_tx_callback, NULL,
                                   saiInitHandle.amp_dma_tx_handle);
    SAI_TransferRxCreateHandleEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_rx_handle, saiInitHandle.sai_rx_callback, NULL,
                                   saiInitHandle.amp_dma_rx_handle);

    SAI_TransferTxSetFormatEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_tx_handle, &saiAmpFormat, BOARD_AMP_SAI_CLK_FREQ,
                                BOARD_AMP_SAI_CLK_FREQ);
    SAI_TransferRxSetFormatEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_rx_handle, &saiAmpFormat, BOARD_AMP_SAI_CLK_FREQ,
                                BOARD_AMP_SAI_CLK_FREQ);

    /* Force bit clock to override standard enablement */
    SAI_TxSetBitClockRate(BOARD_AMP_SAI, BOARD_AMP_SAI_CLK_FREQ, saiAmpFormat.sampleRate_Hz, saiAmpFormat.bitWidth, 2U);
    SAI_RxSetBitClockRate(BOARD_AMP_SAI, BOARD_AMP_SAI_CLK_FREQ, saiAmpFormat.sampleRate_Hz, saiAmpFormat.bitWidth, 2U);

    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(BOARD_AMP_SAI, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(BOARD_AMP_SAI, kSAI_FIFOErrorInterruptEnable);

    NVIC_SetPriority(BOARD_AMP_SAI_EDMA_TX_IRQ, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    NVIC_SetPriority(BOARD_AMP_SAI_EDMA_RX_IRQ, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);

    EnableIRQ(BOARD_AMP_SAI_TX_IRQ);
    EnableIRQ(BOARD_AMP_SAI_RX_IRQ);

    memset(dummy_txbuffer, 0, 32);
    txfer.dataSize = 32;
    txfer.data     = dummy_txbuffer;
    SAI_TransferSendEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_tx_handle, &txfer);

    rxfer.dataSize = 32;
    rxfer.data     = dummy_rxbuffer;
    SAI_TransferReceiveEDMA(BOARD_AMP_SAI, saiInitHandle.amp_sai_rx_handle, &rxfer);
}

// read bit in flash to know TFA has been calibrated
void BOARD_Get_Calibration_State(uint8_t *cur_state)
{
    status_t ret;
    uint32_t len      = 0;
    sln_dev_cfg_t cfg = DEFAULT_CFG_VALUES;

    ret = SLN_FLASH_MGMT_Read(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, &len);
    if (kStatus_Success != ret)
    {
        configPRINTF(("Warning, unknown calibration state! %d\r\n", ret));
        *cur_state = 0; // FIXME: calibrate or do not calibrate? What's the risk?
    }
    else
    {
        *cur_state = cfg.audio_amp_calibration_state;
    }
}

// write bit in flash to set TFA calibration state
void BOARD_Set_Calibration_State(uint8_t new_state)
{
    status_t ret;
    uint32_t len      = 0;
    sln_dev_cfg_t cfg = DEFAULT_CFG_VALUES;

    ret = SLN_FLASH_MGMT_Read(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, &len);

    /* If this is a new file, then carry on and save the file */
    if (SLN_FLASH_MGMT_ENOENTRY2 == ret)
    {
        len = sizeof(sln_dev_cfg_t);
    }
    else if (SLN_FLASH_MGMT_OK != ret)
    {
        configPRINTF(("Warning, unknown calibration state! %d\r\n", ret));
        return;
    }

    /* If we got here, then we are ready to set the calibrated bit */
    if (cfg.audio_amp_calibration_state != new_state)
    {
        /* update bit in flash */
        cfg.audio_amp_calibration_state = new_state;
        ret                             = SLN_FLASH_MGMT_Save(DEVICE_CONFIG_FILE_NAME, (uint8_t *)&cfg, len);
        if (kStatus_Success != ret)
        {
            configPRINTF(("Warning, unknown new calibration state! %d\r\n", ret));
            return;
        }
    }
}

extern void SAI_UserTxIRQHandler(void);
extern void SAI_UserRxIRQHandler(void);
void BOARD_AMP_SAI_Tx_IRQ_Handler(void)
{
    if (BOARD_AMP_SAI->TCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserTxIRQHandler();
    }
}

void BOARD_AMP_SAI_Rx_IRQ_Handler(void)
{
    if (BOARD_AMP_SAI->RCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserRxIRQHandler();
    }
}
#endif

/* MPU configuration. */
void BOARD_ConfigMPU(void)
{
    unsigned int dtcSize = (unsigned int)__top_SRAM_DTC - (unsigned int)__base_SRAM_DTC;
    unsigned int itcSize = (unsigned int)__top_SRAM_ITC - (unsigned int)__base_SRAM_ITC;
    unsigned int ocSize_non_cacheable =
        (unsigned int)__top_SRAM_OC_NON_CACHEABLE - (unsigned int)__base_SRAM_OC_NON_CACHEABLE;
    unsigned int ocSize_cacheable = (unsigned int)__top_SRAM_OC_CACHEABLE - (unsigned int)__base_SRAM_OC_CACHEABLE;
#if defined(__MCUXPRESSO)
    /*! MCUX IDE changed how they check memory regions, we have to add special case for them. */
    if (4 == ocSize_cacheable)
    {
        ocSize_cacheable = 0;
    }
#endif
    unsigned int ocSize = ocSize_non_cacheable + ocSize_cacheable;

    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in core_cm7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in core_cm7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate 0             0           1           1              Normal             not shareable
     * Outer and inner write back no write allocate 0             1           1           0              Normal
     * shareable       Outer and inner write through no write allocate 0             1           1           1 Normal
     * shareable       Outer and inner write back no write allocate 1             0           0           0 Normal not
     * shareable   outer and inner noncache 1             1           0           0              Normal shareable outer
     * and inner noncache 1             0           1           1              Normal             not shareable   outer
     * and inner write back write/read acllocate 1             1           1           1              Normal shareable
     * outer and inner write back write/read acllocate 2             x           0           0              Device not
     * shareable Above are normal use settings, if your want to see more details or want to config different
     * inner/outter cache policy. please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide
     * <dui0646b_cortex_m7_dgug.pdf> param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled,
     * 1=sub-region is disabled. param Size              Region size of the region to be configured. use
     * ARM_MPU_REGION_SIZE_xxx MACRO in core_cm7.h.
     */

    /* Region 0 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);
#if 0
    /* Region 1 setting: Memory with Device type, not shareable,  non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);
#endif

    /* Region 2 setting */
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Setting Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512MB);
#else
    /* Setting Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);
#endif

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting: Memory with Normal type, not shareable, non-cacheable */
    uint8_t mpuRegSize = get_mpu_size(itcSize);
    MPU->RBAR          = ARM_MPU_RBAR(4, 0x00000000U);
    // MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, mpuRegSize);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, mpuRegSize);

    /* Region 5 setting: Memory with Normal type, not shareable, non-cacheable */
    mpuRegSize = get_mpu_size(dtcSize);
    MPU->RBAR  = ARM_MPU_RBAR(5, 0x20000000U);
    // MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, mpuRegSize);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, mpuRegSize);

    /* Region 6 setting: Memory with Normal type, not shareable, non-cacheable */
    if (ocSize_non_cacheable != 0)
    {
        mpuRegSize = get_mpu_size(ocSize_non_cacheable);
        MPU->RBAR  = ARM_MPU_RBAR(6, (unsigned int)__base_SRAM_OC_NON_CACHEABLE);
        // MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, mpuRegSize);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, mpuRegSize);
    }

    if (ocSize_cacheable != 0)
    {
        mpuRegSize = get_mpu_size(ocSize_cacheable);
        /* Region 7 setting: Memory with Normal type, not shareable, cacheable WB/WA */
        MPU->RBAR = ARM_MPU_RBAR(7, (unsigned int)__base_SRAM_OC_CACHEABLE);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 1, 1, 0, mpuRegSize);
    }

    /* Region 8 setting: Memory with Normal type, not shareable, non-cacheable */
    mpuRegSize = get_mpu_size(ocSize - (512 * 1024));
    MPU->RBAR  = ARM_MPU_RBAR(8, 0x20280000U);
    // MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, mpuRegSize);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, mpuRegSize);

    /* The define sets the cacheable memory to shareable,
     * this suggestion is referred from chapter 2.2.1 Memory regions,
     * types and attributes in Cortex-M7 Devices, Generic User Guide */
#if 0 // NOTE: Below is NOT valid for SLN-ALEXA-IOT
#if defined(SDRAM_IS_SHAREABLE)
    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#else
    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#endif

    /* Region 8 setting, set last 2MB of SDRAM can't be accessed by cache, glocal variables which are not expected to be
     * accessed by cache can be put here */
    /* Memory with Normal type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x81E00000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);
#endif

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

static const clock_arm_pll_config_t armPllConfig_BOARD_BoostClock = {
    .loopDivider = 100, /* PLL loop divider, Fout = Fin * 50 */
    .src         = 0,   /* Bypass clock source, 0 - OSC 24M, 1 - CLK1_P and CLK1_N */
};
static bool clockBoost = false;

void BOARD_BoostClock(void)
{
    if (!clockBoost)
    {
        clockBoost = true;
        /* Switch AHB_CLK_ROOT from pre_periph_clk to periph_clk2 */
        CLOCK_SetMux(kCLOCK_PeriphMux, 1);

        /* Setting the VDD_SOC to 1.275V. It is necessary to config AHB to 600Mhz. */
        DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0x13);
        /* Waiting for DCDC_STS_DC_OK bit is asserted */
        while (DCDC_REG0_STS_DC_OK_MASK != (DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
        {
        }
        CLOCK_InitArmPll(&armPllConfig_BOARD_BoostClock);
        CLOCK_SetMux(kCLOCK_PeriphMux, 0);

        SystemCoreClock = BOARD_BOOSTCLOCK_CORE_CLOCK;
    }
}

void BOARD_RevertClock(void)
{
    if (clockBoost)
    {
        clockBoost = false;

        /* Switch AHB_CLK_ROOT from pre_periph_clk to periph_clk2 */
        CLOCK_SetMux(kCLOCK_PeriphMux, 1);

        /* Setting the VDD_SOC to 1.15V. It is necessary to config AHB to 528Mhz. */
        DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0xE);
        /* Waiting for DCDC_STS_DC_OK bit is asserted */
        while (DCDC_REG0_STS_DC_OK_MASK != (DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
        {
        }
        CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);
        CLOCK_SetMux(kCLOCK_PeriphMux, 0);

        SystemCoreClock = DEFAULT_SYSTEM_CLOCK;
    }
}

void BOARD_RelocateVectorTableToRam(void)
{
    uint32_t n;
    uint32_t irqMaskValue;

    irqMaskValue = DisableGlobalIRQ();

    SCB_DisableDCache();
    SCB_DisableICache();

    /* Copy the vector table from ROM to RAM */
    for (n = 0; n < ((uint32_t)0x400) / sizeof(uint32_t); n++)
    {
        g_vectorTable[n] = __VECTOR_TABLE[n];
    }

    /* Set application defined stack pointer */
    volatile unsigned int vStackTop = (unsigned int)&__StackTop;
    g_vectorTable[0]                = vStackTop;

    /* Point the VTOR to the position of vector table */
    SCB->VTOR = (uint32_t)g_vectorTable;
    __DSB();

    SCB_EnableICache();
    SCB_EnableDCache();

    EnableGlobalIRQ(irqMaskValue);
}

uint8_t BUTTON_MSDPressed(void)
{
    /* Check if USB MSD Mode button (SW2) is pushed */
    /* SW2 is connected to GND and uses external pull-up resistor */
    if (0 == GPIO_PinRead(SW2_GPIO, SW2_GPIO_PIN))
        return 1;

    return 0;
}

uint8_t BUTTON_OTWPressed(void)
{
    /* Check if OTW Mode button (SW1) is pushed */
    /* SW1 is connected to GND and uses external pull-up resistor */
    if (0 == GPIO_PinRead(SW1_GPIO, SW1_GPIO_PIN))
        return 1;

    return 0;
}

#if defined(SDK_SAI_BASED_COMPONENT_USED) && SDK_SAI_BASED_COMPONENT_USED
void *BOARD_GetBoardCodecConfig(void)
{
    return (void *)&boardCodecConfig;
}
#endif
