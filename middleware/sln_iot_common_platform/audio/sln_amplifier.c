/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"
#include "timers.h"

#include "app.h"
#include "audio_samples.h"
#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_sai.h"
#include "fsl_sai_edma.h"
#include "fsl_lpi2c.h"
#if USE_COMMON_CODEC
#include "fsl_codec_common.h"
#else
#include "fsl_tfa9894.h"
#endif
#include "sln_amplifier.h"

#if USE_AUDIO_SPEAKER
#include "composite.h"
#endif

#include "sln_flash_mgmt.h"
#include "sln_cfg_file.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define amplifier_send_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The DMA Handle for the TFA9894 audio amplifier SAI3 */
edma_handle_t sAmpDmaTxHandle       = {0};
edma_handle_t sAmpDmaRxHandle       = {0};
sai_edma_handle_t sAmpTxHandler     = {0};
sai_edma_handle_t sAmpRxHandler     = {0};
sai_transfer_format_t sAmpSaiFormat = {0};

#if USE_COMMON_CODEC
codec_handle_t codecHandle;
#else
tfa98xx_handle_t tfa98xx_handle[NUM_TFA_DEVICE] = {0};
extern tfa98xx_operation_t g_TFA9894_operation;
#endif

volatile bool isrxFinished     = false;
volatile uint32_t beginCount   = 0;
volatile uint32_t sendCount    = 0;
volatile uint32_t receiveCount = 0;
uint32_t u16RxBufferIndex      = 0;

TaskHandle_t xAmplifierSendTaskHandle;

static EventGroupHandle_t s_loopBackEventGroup;
static EventBits_t s_loopBackEvent;
EventGroupHandle_t g_dmaTxComplete;

__attribute__((section(".data.$SRAM_DTC"))) __attribute__((aligned(2)))
uint8_t s_ampRxDmaBuffer[PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES];
__attribute__((section(".data.$SRAM_DTC"))) __attribute__((aligned(2)))
uint8_t s_ampRxDataBuffer[PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES];

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t dummy_buffer[32], 32);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t dummy_rxbuffer[32], 32);

volatile uint32_t u32AudioLength = 0;
uint8_t *pu8AudioPlay            = NULL;

uint32_t startAudioLength = 0;
uint8_t *startAudioPlay   = NULL;
volatile uint8_t loop     = 0;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {{0, 0, kI2C_Write, 0, 0, NULL, 0}, 0, 0, NULL, NULL};
#endif

#if USE_AUDIO_SPEAKER
extern usb_device_composite_struct_t g_composite;
extern uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE];

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t audioPlayDMATempBuff[FS_ISO_OUT_ENDP_PACKET_SIZE];
#endif

static volatile uint8_t *pu8BufferPool = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Tx Callback for amplifier transmit  */
static void SLN_AMP_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);

/*! @brief Rx Callback for amplifier receive  */
static void SLN_AMP_RxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);

/*! @brief Configures the SAI mclk. Needs to be here instead of board.c due to #if */
void BOARD_EnableSaiMclkOutput(bool enable);

/*******************************************************************************
 * Code
 ******************************************************************************/

#if !(defined(USE_COMMON_CODEC) && USE_COMMON_CODEC)
static void wait_ms_func(uint32_t time_ms)
{
    vTaskDelay(portTICK_PERIOD_MS * time_ms);
}

// read bit in flash to know TFA has been calibrated
static void getCalibrationState(uint8_t *cur_state)
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
static void setCalibrationState(uint8_t new_state)
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
#endif

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI3_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI3_MCLK_DIR_MASK);
    }
}

void SAI_UserTxIRQHandler(void)
{
    SAI_TxClearStatusFlags(DEMO_SAI_OUT, kSAI_FIFOErrorFlag);
}

void SAI_UserRxIRQHandler(void)
{
    SAI_RxClearStatusFlags(DEMO_SAI_OUT, kSAI_FIFOErrorFlag);
}

void SAI_UserIRQHandler(void)
{
    if (DEMO_SAI_OUT->TCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserTxIRQHandler();
    }
}

void SAI_RxUserIRQHandler(void)
{
    if (DEMO_SAI_OUT->RCSR & kSAI_FIFOErrorFlag)
    {
        SAI_UserRxIRQHandler();
    }
}

void SLN_AMP_RxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    static uint8_t firstTime = 1;

    if (!firstTime)
    {
        if (u16RxBufferIndex)
        {
            u16RxBufferIndex--;
        }
        else
        {
            u16RxBufferIndex++;
        }

        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;

        GPIO_PinWrite(GPIO1, 2, 0U);
        sai_transfer_t sSaiRxBuffer = {0};

        memcpy(s_ampRxDataBuffer, s_ampRxDmaBuffer, PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES);

        if (NULL != s_loopBackEventGroup)
        {
            xEventGroupSetBitsFromISR(s_loopBackEventGroup, s_loopBackEvent, &xHigherPriorityTaskWoken);
        }

        // SLN_AMP_Read();
        /* Read the next 48KHz, Stereo, 16Bit sample */
        sSaiRxBuffer.data     = s_ampRxDmaBuffer;
        sSaiRxBuffer.dataSize = PCM_AMP_SAMPLE_COUNT * PCM_SAMPLE_SIZE_BYTES;
        SAI_TransferReceiveEDMA(base, &sAmpRxHandler, &sSaiRxBuffer);
        GPIO_PinWrite(GPIO1, 2, 1U);
    }
    firstTime = 0;
}

static void SLN_AMP_TxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xEventGroupSetBitsFromISR(g_dmaTxComplete, 1, &xHigherPriorityTaskWoken);

    if (pu8BufferPool != NULL)
    {
        (*pu8BufferPool)++;
    }

#if USE_AUDIO_SPEAKER
    sai_transfer_t xfer = {0};
    if ((g_composite.audioUnified.audioSendTimes >= g_composite.audioUnified.usbRecvTimes) &&
        (g_composite.audioUnified.startPlayHalfFull == 1))
    {
        g_composite.audioUnified.startPlayHalfFull      = 0;
        g_composite.audioUnified.speakerDetachOrNoInput = 1;
    }
    if (g_composite.audioUnified.startPlayHalfFull)
    {
        xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
        xfer.data     = audioPlayDataBuff + g_composite.audioUnified.tdWriteNumberPlay;
        g_composite.audioUnified.audioSendCount += FS_ISO_OUT_ENDP_PACKET_SIZE;
        g_composite.audioUnified.audioSendTimes++;
        g_composite.audioUnified.tdWriteNumberPlay += FS_ISO_OUT_ENDP_PACKET_SIZE;
        if (g_composite.audioUnified.tdWriteNumberPlay >=
            AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE)
        {
            g_composite.audioUnified.tdWriteNumberPlay = 0;
        }
    }
    else
    {
        xfer.dataSize = FS_ISO_OUT_ENDP_PACKET_SIZE;
        xfer.data     = audioPlayDMATempBuff;
    }

    SAI_TransferSendEDMA(base, handle, &xfer);
#endif
}

amplifier_status_t SLN_AMP_WriteDefault(void)
{
#if USE_SIGNAL_TEST_SIGNAL
    PlaybackSine();
#else
    sai_transfer_t xfer_codec = {0};

    // Take advantage of 32 being power of 2 to replace % to reduce # of instructions
    size_t overFlow = sizeof(_med_ui_wakesound_wav) & 31U;
    if (overFlow)
    {
        xfer_codec.dataSize = sizeof(_med_ui_wakesound_wav) - (overFlow);
    }
    xfer_codec.data = (uint8_t *)_med_ui_wakesound_wav;
    SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &xfer_codec);
#endif
    return 0;
}

void audio_send_task(void *pvParameters)
{
    sai_transfer_t write_xfer = {0};
    EventBits_t events;

    /* Chop into 512KB chunks so the DMA can handle it. */
    while (u32AudioLength)
    {
        events = xEventGroupWaitBits(g_dmaTxComplete, 3, pdTRUE, pdFALSE, portMAX_DELAY);

        // Abort
        if (events & 2)
        {
            SAI_TransferTerminateSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler);
            u32AudioLength = 0;
            pu8AudioPlay   = NULL;
            loop           = 0;
            break;
        }
        else
        {
            if (u32AudioLength > 0x80000)
            {
                write_xfer.dataSize = 0x80000;
                write_xfer.data     = pu8AudioPlay;
                SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

                pu8AudioPlay += 0x80000;
                u32AudioLength -= 0x80000;
            }
            else
            {
                write_xfer.dataSize = u32AudioLength;
                write_xfer.data     = pu8AudioPlay;
                SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

                if (loop)
                {
                    u32AudioLength = startAudioLength;
                    pu8AudioPlay   = startAudioPlay;
                }
                else
                {
                    u32AudioLength = 0;
                    pu8AudioPlay   = NULL;
                }
            }
        }
    }

    vTaskDelete(NULL);
}

amplifier_status_t SLN_AMP_Write(uint8_t *data, uint32_t length)
{
    sai_transfer_t write_xfer     = {0};
    amplifier_status_t eAmpStatus = 1;

    if (0 == u32AudioLength)
    {
        pu8AudioPlay = data;
        if (length)
        {
            u32AudioLength = length - (length % 32);
        }
        else
        {
            u32AudioLength = length;
        }

        /* Chop into 512KB chunks so the DMA can handle it. */
        if (u32AudioLength > 0x80000)
        {
            write_xfer.dataSize = 0x80000;
            write_xfer.data     = pu8AudioPlay;
            SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

            pu8AudioPlay += 0x80000;
            u32AudioLength -= 0x80000;

            if (xTaskCreate(audio_send_task, "amplifier_send_task", 1024U, NULL, amplifier_send_task_PRIORITY,
                            &xAmplifierSendTaskHandle) != pdPASS)
            {
                eAmpStatus = 1;
            }
            eAmpStatus = 0;
        }
        else
        {
            write_xfer.dataSize = u32AudioLength;
            write_xfer.data     = pu8AudioPlay;
            SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

            u32AudioLength = 0;
            pu8AudioPlay   = NULL;

            eAmpStatus = 0;
        }
    }
    return eAmpStatus;
}

amplifier_status_t SLN_AMP_WriteLoop(uint8_t *data, uint32_t length)
{
    sai_transfer_t write_xfer     = {0};
    amplifier_status_t eAmpStatus = 1;

    if (0 == u32AudioLength)
    {
        pu8AudioPlay = data;
        if (length)
        {
            u32AudioLength = length - (length % 32);
        }
        else
        {
            u32AudioLength = length;
        }

        startAudioLength = u32AudioLength;
        startAudioPlay   = data;
        loop             = 1;

        /* Chop into 512KB chunks so the DMA can handle it. */
        if (u32AudioLength > 0x80000)
        {
            write_xfer.dataSize = 0x80000;
            write_xfer.data     = pu8AudioPlay;
            SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

            pu8AudioPlay += 0x80000;
            u32AudioLength -= 0x80000;

            /* Clear all the bits before creating the task */
            xEventGroupClearBits(g_dmaTxComplete, 3);

            if (xTaskCreate(audio_send_task, "amplifier_send_task", 1024U, NULL, amplifier_send_task_PRIORITY,
                            &xAmplifierSendTaskHandle) != pdPASS)
            {
                eAmpStatus = 1;
                configPRINTF(("Fail to create amplifier_send_task! \r\n"));
            }
            eAmpStatus = 0;
        }
        else
        {
            write_xfer.dataSize = u32AudioLength;
            write_xfer.data     = pu8AudioPlay;
            SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

            u32AudioLength = 0;
            pu8AudioPlay   = NULL;

            eAmpStatus = 0;
        }
    }
    return eAmpStatus;
}

amplifier_status_t SLN_AMP_WriteNoWait(uint8_t *data, uint32_t length)
{
    uint32_t total_len;
    uint8_t *ptr              = data;
    sai_transfer_t write_xfer = {0};
    status_t ret              = 0;

    if (length)
    {
        total_len = length - (length % 32);
    }
    else
    {
        total_len = length;
    }

    /* Chop into 512KB chunks so the DMA can handle it. */
    write_xfer.dataSize = total_len;
    write_xfer.data     = ptr;
    ret                 = SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

    return ret;
}

amplifier_status_t SLN_AMP_WriteBlocking(uint8_t *data, uint32_t length)
{
    uint32_t total_len;
    uint8_t *ptr              = data;
    sai_transfer_t write_xfer = {0};

    if (length)
    {
        total_len = length - (length % 32);
    }
    else
    {
        total_len = length;
    }

    /* Chop into 512KB chunks so the DMA can handle it. */
    while (total_len)
    {
        if (total_len > 0x80000)
        {
            write_xfer.dataSize = 0x80000;
            write_xfer.data     = ptr;
            SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

            ptr += 0x80000;
            total_len -= 0x80000;
        }
        else
        {
            write_xfer.dataSize = total_len;
            write_xfer.data     = ptr;
            SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &write_xfer);

            total_len = 0;
        }

        xEventGroupWaitBits(g_dmaTxComplete, 1, pdTRUE, pdTRUE, portMAX_DELAY);
    }

    return 0;
}

amplifier_status_t SLN_AMP_AbortWrite(void)
{
    xEventGroupSetBits(g_dmaTxComplete, 2);
    return 0;
}

amplifier_status_t SLN_AMP_Read(void)
{
    sai_transfer_t xfer = {0};
    xfer.data           = s_ampRxDmaBuffer;
    xfer.dataSize       = sizeof(s_ampRxDmaBuffer);

    SAI_TransferReceiveEDMA(DEMO_SAI_OUT, &sAmpRxHandler, &xfer);
    return 0;
}

amplifier_status_t SLN_AMP_Init(volatile uint8_t *pu8BuffNum)
{
    status_t ret               = kStatus_Fail;
    uint32_t mclkSourceClockHz = 0U;
    sai_config_t sCodecConfig;
    uint32_t i2cSourceClock;
    sai_transfer_t xfer;
    sai_transfer_t rxfer;
#if !(defined(USE_COMMON_CODEC) && USE_COMMON_CODEC)
    uint8_t do_calibration;
    uint8_t current_cal_state;
#endif

    pu8BufferPool   = pu8BuffNum;
    g_dmaTxComplete = xEventGroupCreate();

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
#endif

    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI3*/
    CLOCK_SetMux(kCLOCK_Sai3Mux, DEMO_SAI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai3PreDiv, DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai3Div, DEMO_SAI_CLOCK_SOURCE_DIVIDER);

    BOARD_EnableSaiMclkOutput(true);

#if USE_TFA9894
    EDMA_CreateHandle(&sAmpDmaTxHandle, DMA0, 2U);
    EDMA_CreateHandle(&sAmpDmaRxHandle, DMA0, 3U);
    DMAMUX_SetSource(DMAMUX, 2U, (uint8_t)kDmaRequestMuxSai3Tx);
    DMAMUX_SetSource(DMAMUX, 3U, (uint8_t)kDmaRequestMuxSai3Rx);
    DMAMUX_EnableChannel(DMAMUX, 2U);
    DMAMUX_EnableChannel(DMAMUX, 3U);
#endif

    SAI_TxGetDefaultConfig(&sCodecConfig);
    sCodecConfig.protocol = kSAI_BusI2S;
    SAI_TxInit(DEMO_SAI_OUT, &sCodecConfig);

    /* Initialize SAI Rx */
    SAI_RxGetDefaultConfig(&sCodecConfig);
    sCodecConfig.protocol = kSAI_BusI2S;
    SAI_RxInit(DEMO_SAI_OUT, &sCodecConfig);

    /* Configure the audio format */
#if USE_ALEXA_SOUND_PROMPT || USE_TFA9894 /* We can only use 48KHz right now due to limitations of TFA9894 */
    sAmpSaiFormat.bitWidth      = kSAI_WordWidth16bits;
    sAmpSaiFormat.sampleRate_Hz = kSAI_SampleRate48KHz;
#elif USE_16BIT_PCM
    sAmpSaiFormat.bitWidth      = kSAI_WordWidth16bits;
    sAmpSaiFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#elif USE_32BIT_PCM
    sAmpSaiFormat.bitWidth      = kSAI_WordWidth32bits;
    sAmpSaiFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#endif
    sAmpSaiFormat.channel = 0U;
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    sAmpSaiFormat.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
#else
    // sAmpSaiFormat.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    sAmpSaiFormat.protocol           = kSAI_BusLeftJustified;
    sAmpSaiFormat.isFrameSyncCompact = true;
#if USE_SNIPS
    sAmpSaiFormat.stereo = kSAI_Stereo;
#else
    sAmpSaiFormat.stereo = kSAI_MonoLeft;
#endif
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    sAmpSaiFormat.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif

    /* Configure Sgtl5000 I2C */
    i2cSourceClock = DEMO_I2C_CLK_FREQ;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
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
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
    NVIC_SetPriority(LPI2C1_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
#endif

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI_OUT, &sAmpTxHandler, SLN_AMP_TxCallback, NULL, &sAmpDmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI_OUT, &sAmpRxHandler, SLN_AMP_RxCallback, NULL, &sAmpDmaRxHandle);
    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;

    SAI_TransferTxSetFormatEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &sAmpSaiFormat, mclkSourceClockHz, DEMO_SAI_CLK_FREQ);
    SAI_TransferRxSetFormatEDMA(DEMO_SAI_OUT, &sAmpRxHandler, &sAmpSaiFormat, mclkSourceClockHz, DEMO_SAI_CLK_FREQ);

#if USE_COMMON_CODEC
    /* Force bit clock to override standard enablement */
    SAI_TxSetBitClockRate(DEMO_SAI_OUT, mclkSourceClockHz, sAmpSaiFormat.sampleRate_Hz, sAmpSaiFormat.bitWidth, 2U);
    SAI_RxSetBitClockRate(DEMO_SAI_OUT, mclkSourceClockHz, sAmpSaiFormat.sampleRate_Hz, sAmpSaiFormat.bitWidth, 2U);
#endif

    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(DEMO_SAI_OUT, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(DEMO_SAI_OUT, kSAI_FIFOErrorInterruptEnable);

    NVIC_SetPriority(DMA2_DMA18_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    NVIC_SetPriority(DMA3_DMA19_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);

    EnableIRQ(DEMO_SAI_TX_IRQ);
    EnableIRQ(DEMO_SAI_RX_IRQ);

    memset(dummy_buffer, 0, 32);
    xfer.dataSize = 32;
    xfer.data     = dummy_buffer;
    SAI_TransferSendEDMA(DEMO_SAI_OUT, &sAmpTxHandler, &xfer);

    rxfer.dataSize = 32;
    rxfer.data     = dummy_rxbuffer;
    SAI_TransferReceiveEDMA(DEMO_SAI_OUT, &sAmpRxHandler, &rxfer);

    xEventGroupWaitBits(g_dmaTxComplete, 1, pdTRUE, pdTRUE, portMAX_DELAY);

#if USE_COMMON_CODEC

    ret = CODEC_Init(&codecHandle, BOARD_GetBoardCodecConfig());

#else
    getCalibrationState(&current_cal_state);
    do_calibration = !(current_cal_state); /* current_cal_state == 0 triggers calibration */

    /* Configure TFA9894 */
    if (NUM_TFA_DEVICE == TFA_STEREO)
    {
        tfa98xx_handle[TFA_RIGHT].slaveAddress = TFA_RIGHT_AMP;
        tfa98xx_handle[TFA_RIGHT].base         = BOARD_AMP_I2C;
        tfa98xx_handle[TFA_RIGHT].msWait       = wait_ms_func;
        memcpy(&tfa98xx_handle[TFA_RIGHT].op, &g_TFA9894_operation, sizeof(tfa98xx_operation_t));
        if (!do_calibration)
        {
            /* check the internal calibration state */
            do_calibration = !TFA9894_IsCalibrated(&tfa98xx_handle[TFA_RIGHT]);
        }
    }
    tfa98xx_handle[TFA_LEFT].slaveAddress = TFA_LEFT_AMP;
    tfa98xx_handle[TFA_LEFT].base         = BOARD_AMP_I2C;
    tfa98xx_handle[TFA_LEFT].msWait       = wait_ms_func;
    memcpy(&tfa98xx_handle[TFA_LEFT].op, &g_TFA9894_operation, sizeof(tfa98xx_operation_t));
    if (!do_calibration)
    {
        /* check the internal calibration state */
        do_calibration = !TFA9894_IsCalibrated(&tfa98xx_handle[TFA_LEFT]);
    }

    ret = TFA9894_Init(&tfa98xx_handle[TFA_LEFT], NULL, do_calibration);
    if (kStatus_Success == ret)
    {
        if (NUM_TFA_DEVICE == TFA_STEREO)
        {
            ret = TFA9894_Init(&tfa98xx_handle[TFA_RIGHT], NULL, do_calibration);
        }
        if ((kStatus_Success == ret) && do_calibration)
        {
            setCalibrationState(1); /* cal not needed anymore */
        }
    }

    /* If the calibration has just complete, re-initialize the TFA with the new calibration
     * If this is not done, the audio quality will be bad
     * */
    if (do_calibration)
    {
        /* Initialize again as calibration is complete and need to apply new settings without calibration */
        ret = TFA9894_Init(&tfa98xx_handle[TFA_LEFT], NULL, 0);
        if (kStatus_Success == ret)
        {
            if (NUM_TFA_DEVICE == TFA_STEREO)
            {
                ret = TFA9894_Init(&tfa98xx_handle[TFA_RIGHT], NULL, 0);
            }
        }
    }
#endif

    return ret;
}

uint8_t *SLN_AMP_GetLoopBackBuffer(void)
{
    return s_ampRxDataBuffer;
}

void SLN_AMP_SetLoopBackEventGroup(EventGroupHandle_t *eventGroup)
{
    if ((NULL != eventGroup) && (NULL != *eventGroup))
    {
        s_loopBackEventGroup = *eventGroup;
    }
}

void SLN_AMP_SetLoopBackEventBits(EventBits_t event)
{
    s_loopBackEvent = event;
}

void SLN_AMP_SetVolume(uint8_t volume)
{
#if USE_COMMON_CODEC
    /* Set Volume 0(min) to 100 (max) */
    CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelLeft0 | kCODEC_PlayChannelRight0, (uint32_t)volume);

#else
    /* Example volume table for the TFA
     * Volume level index and corresponding VOL value(dec) from TFA9894 register 0x51
     * Vol_lvl  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14 (Maximum attenuation)
     * Vol_val  0   1   2   4   6   10  16  24  30  40  50  60  80  100 255 (Soft mute) */

    switch (volume / 10)
    {
        case 0:
            // volume = 255;
            break;
        case 1:
            volume = 40;
            break;
        case 2:
            volume = 30;
            break;
        case 3:
            volume = 24;
            break;
        case 4:
            volume = 16;
            break;
        case 5:
            volume = 10;
            break;
        case 6:
            volume = 6;
            break;
        case 7:
            volume = 4;
            break;
        case 8:
            volume = 2;
            break;
        case 9:
            volume = 1;
            break;
        case 10:
            volume = 0;
            break;
    }

    /* Expects a value between 0(max volume) to 255 (max attenuation) */
    TFA9894_SetVolume(&tfa98xx_handle[TFA_LEFT], volume);
#endif
}

void *SLN_AMP_GetAmpTxHandler()
{
    return &sAmpTxHandler;
}
