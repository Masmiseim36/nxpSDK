/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <xtensa/config/core.h>
#include <xtensa/xos.h>

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

#include "fsl_dma.h"
#include "fsl_i2s_dma.h"
#include "fsl_dmic.h"
#include "fsl_dmic_dma.h"

#include "pin_mux.h"

#ifdef VOICE_SPOT
#include "public/rdsp_voicespot.h"
#include "public/rdsp_voicespot_utils.h"
#include "RdspMemoryUtilsPublic.h"
#include "HeyNXP_en-US_1_params.h"
#include "HeyNXP_en-US_1.h"
#endif

#include "dsp_config.h"
#include "board_fusionf1.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"
#include "fsl_i2s.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Line out configuration */
#define DEMO_AUDIO_PROTOCOL             kCODEC_BusI2S
#define DEMO_I2S_MASTER_CLOCK_FREQUENCY CLOCK_GetMclkClkFreq()
#define DEMO_I2S_TX                     (I2S3)
#define DEMO_AUDIO_BIT_WIDTH            (16)
#define DEMO_AUDIO_FRAME_LENGTH         (32)
#define DEMO_AUDIO_SAMPLE_RATE          (16000)
#define DEMO_I2S_TX_CHANNEL             (7)
#define DEMO_I2S_TX_MODE                kI2S_MasterSlaveNormalSlave
#define DEMO_I2S_CLOCK_DIVIDER                                   \
    (24576000U / DEMO_AUDIO_SAMPLE_RATE / DEMO_AUDIO_BIT_WIDTH / \
     1) /* I2S source clock 24.576MHZ, sample rate, bits width, channel */

/* DMIC configuration */
#define BYTE_DEPTH_DMIC          (4U)
#define DEMO_DMA                 (DMA1)
#define DMAREQ_DMIC0             16U
#define DEMO_DMIC_RX_CHANNEL     DMAREQ_DMIC0
#define DEMO_DMIC_CHANNEL        kDMIC_Channel0
#define DEMO_DMIC_CHANNEL_ENABLE DMIC_CHANEN_EN_CH0(1)
#define FIFO_DEPTH               (15U)
#define BOARD_XTAL_SYS_CLK_HZ 24000000U /*!< Board xtal_sys frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ  32768U    /*!< Board xtal32K frequency in Hz */

#define VOICESPOT_NUM_SAMPLES_PER_FRAME 200U
#define VOICESPOT_SAMPLE_RATE           16000
#define IN_BUFFER_NUM_FRAMES            (20U)
#define BUFFER_SIZE                     (VOICESPOT_NUM_SAMPLES_PER_FRAME * IN_BUFFER_NUM_FRAMES)

/*******************************************************************************
 * Variables
 ******************************************************************************/
static i2s_config_t tx_config;
static uint32_t volatile s_writeIndex = 0U;
static uint32_t volatile s_readIndex  = 0U;
static dma_handle_t s_i2sTxDmaHandle;
static i2s_dma_handle_t s_i2sTxHandle;
static dmic_dma_handle_t s_dmicDmaHandle;
static dma_handle_t s_dmicRxDmaHandle;
static dmic_transfer_t s_receiveXfer[IN_BUFFER_NUM_FRAMES];
static i2s_transfer_t i2sTxTransfer;

#if (XCHAL_DCACHE_SIZE > 0)
#if BYTE_DEPTH_DMIC == 2U
AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t s_buffer[BUFFER_SIZE], 4);
#else
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_buffer[BUFFER_SIZE], 16);
#endif
AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t i2s_buffer[BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmaDescriptorPingpong[IN_BUFFER_NUM_FRAMES], 16);
/* Frame buffer into VOICE_SPOT */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t frame_buffer[VOICESPOT_NUM_SAMPLES_PER_FRAME], 16);
#else
#if BYTE_DEPTH_DMIC == 2U
SDK_ALIGN(static uint16_t s_buffer[BUFFER_SIZE], 4);
#else
SDK_ALIGN(static uint32_t s_buffer[BUFFER_SIZE], 16);
#endif
SDK_ALIGN(static uint16_t i2s_buffer[BUFFER_SIZE], 4);
SDK_ALIGN(dma_descriptor_t s_dmaDescriptorPingpong[IN_BUFFER_NUM_FRAMES], 16);
/* Frame buffer into VOICE_SPOT */
SDK_ALIGN(uint32_t frame_buffer[VOICESPOT_NUM_SAMPLES_PER_FRAME], 16);
#endif

#ifdef VOICE_SPOT

static rdsp_voicespot_control *voicespot_control;
static int32_t voicespot_handle;
static char **voicespot_class_string;
static int32_t num_samples_per_frame;
static int32_t num_outputs;

/* Frame counter */
static int64_t frame_count = 0;
/* Frame count where last trigger occurred */
int64_t frame_count_trigger = -1;
const char *model_file_name = "HeyNXP_en-US_1.bin";
const char *param_file_name = "HeyNXP_1_params.bin";
static int32_t *max_scores;
static int64_t *max_frame_count;
static int32_t *scores;
static int32_t *scores_trigger;
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void dmic_Callback(DMIC_Type *base, dmic_dma_handle_t *handle, status_t status, void *userData)
{
    /* Convert from 32 bit fixed-point to 16 bits */
    for (int32_t i = 0; i < VOICESPOT_NUM_SAMPLES_PER_FRAME; i++)
    {
        i2s_buffer[s_writeIndex + i] = (uint16_t)s_buffer[s_writeIndex + i];
    }

    i2sTxTransfer.data     = (uint8_t *)&i2s_buffer[s_writeIndex];
    i2sTxTransfer.dataSize = VOICESPOT_NUM_SAMPLES_PER_FRAME * sizeof(uint16_t);
    I2S_TxTransferSendDMA(DEMO_I2S_TX, &s_i2sTxHandle, i2sTxTransfer);

    // Update write index and check for wrap-around
    s_writeIndex += VOICESPOT_NUM_SAMPLES_PER_FRAME;
    if (s_writeIndex >= BUFFER_SIZE)
        s_writeIndex -= BUFFER_SIZE;
}

/*******************************************************************************
 * Code
 ******************************************************************************/
static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);

    xos_start_system_timer(-1, 0);
}

static void BOARD_InitClock(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* sets external XTAL OSC freq */

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* DSP_INT0_SEL18 = DMA1 */
    INPUTMUX_AttachSignal(INPUTMUX, 18U, kINPUTMUX_Dmac1ToDspInterrupt);

    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    /* attach AUDIO PLL clock to FLEXCOMM3 (I2S3) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM3);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;
    /* DMIC source from audio pll, divider 8, 24.576M/8=3.072MHZ */
    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC);
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 8);

    /* Set shared signal set 0: SCK, WS from Flexcomm1 */
    SYSCTL1->SHAREDCTRLSET[0] = SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL(1) | SYSCTL1_SHAREDCTRLSET_SHAREDWSSEL(1);
    /* Set flexcomm3 SCK, WS from shared signal set 0 */
    SYSCTL1->FCCTRLSEL[3] = SYSCTL1_FCCTRLSEL_SCKINSEL(1) | SYSCTL1_FCCTRLSEL_WSINSEL(1);
}

static void BOARD_Init_DMA(void)
{
    DMA_Init(DEMO_DMA);
    DMA_EnableChannel(DEMO_DMA, DEMO_I2S_TX_CHANNEL);
    DMA_EnableChannel(DEMO_DMA, DEMO_DMIC_RX_CHANNEL);
    DMA_SetChannelPriority(DEMO_DMA, DEMO_I2S_TX_CHANNEL, kDMA_ChannelPriority3);
    DMA_SetChannelPriority(DEMO_DMA, DEMO_DMIC_RX_CHANNEL, kDMA_ChannelPriority1);
}

static void BOARD_Init_DMIC(void)
{
    dmic_channel_config_t dmic_channel_cfg;
    memset(&dmic_channel_cfg, 0U, sizeof(dmic_channel_config_t));
    dmic_channel_cfg.divhfclk            = kDMIC_PdmDiv2;
    dmic_channel_cfg.osr                 = 48U;
    dmic_channel_cfg.gainshft            = 0U;
    dmic_channel_cfg.preac2coef          = kDMIC_CompValueZero;
    dmic_channel_cfg.preac4coef          = kDMIC_CompValueZero;
    dmic_channel_cfg.dc_cut_level        = kDMIC_DcCut155;
    dmic_channel_cfg.post_dc_gain_reduce = 1U;
    dmic_channel_cfg.saturate16bit       = 0U;
    dmic_channel_cfg.enableSignExtend    = true;
    dmic_channel_cfg.sample_rate         = kDMIC_PhyFullSpeed;
    DMIC_Init(DMIC0);
#if !(defined(FSL_FEATURE_DMIC_HAS_NO_IOCFG) && FSL_FEATURE_DMIC_HAS_NO_IOCFG)
    DMIC_SetIOCFG(DMIC0, kDMIC_PdmDual);
#endif
    DMIC_Use2fs(DMIC0, true);
    DMIC_EnableChannelDma(DMIC0, DEMO_DMIC_CHANNEL, true);
    DMIC_ConfigChannel(DMIC0, DEMO_DMIC_CHANNEL, kDMIC_Left, &dmic_channel_cfg);
    DMIC_FifoChannel(DMIC0, DEMO_DMIC_CHANNEL, FIFO_DEPTH, true, true);
    DMIC_EnableChannnel(DMIC0, DEMO_DMIC_CHANNEL_ENABLE);
}

static void BOARD_Init_I2S(void)
{
    /*
     * masterSlave = kI2S_MasterSlaveNormalMaster;
     * mode = kI2S_ModeI2sClassic;
     * rightLow = false;
     * leftJust = false;
     * pdmData = false;
     * sckPol = false;
     * wsPol = false;
     * divider = 1;
     * oneChannel = false;
     * dataLength = 16;
     * frameLength = 32;
     * position = 0;
     * fifoLevel = 4;
     */
    I2S_TxGetDefaultConfig(&tx_config);
    tx_config.masterSlave = DEMO_I2S_TX_MODE;
    tx_config.divider     = DEMO_I2S_CLOCK_DIVIDER;
    tx_config.oneChannel  = true;
    tx_config.dataLength  = DEMO_AUDIO_BIT_WIDTH;
    tx_config.frameLength = DEMO_AUDIO_FRAME_LENGTH;
    I2S_TxInit(DEMO_I2S_TX, &tx_config);
}


#ifdef VOICE_SPOT

static int32_t VoiceSpotInit(void)
{
    int32_t data_type        = RDSP_DATA_TYPE__INT32;
    int32_t voicespot_status = -1;

    /* Create VOICE_SPOT control structure */
    voicespot_status = rdspVoiceSpot_CreateControl(&voicespot_control, data_type);
    PRINTF("rdspVoiceSpot_CreateControl: voicespot_status = %d\r\n", (int)voicespot_status);

    int32_t enable_highpass_filter = 1;
    int32_t generate_output        = 0;
    /* Create VOICE_SPOT instance */
    voicespot_status =
        rdspVoiceSpot_CreateInstance(voicespot_control, &voicespot_handle, enable_highpass_filter, generate_output);
    PRINTF("rdspVoiceSpot_CreateInstance: voicespot_status = %d\r\n", (int)voicespot_status);

    uint8_t *model_blob      = NULL;
    uint32_t model_blob_size = sizeof(model_blob_header);
    /* Allocate memory for an aligned copy of the model contained in header file. This allows dynamic modification for
     * e.g. weight permutation depending on target platform*/
    model_blob = rdsp_malloc_align(model_blob_size, 16);
    /* Copy model_blob from header file */
    rdsp_memcpy(model_blob, (uint8_t *)model_blob_header, model_blob_size);

    if (rdspVoiceSpot_CheckModelIntegrity(model_blob_size, model_blob) != RDSP_VOICESPOT_OK)
    {
        /* Check the integrity of the model */
        PRINTF("rdspVoiceSpot_CheckModelIntegrity: Model integrity check failed\r\n");
        return -1;
    }
    /* Open the VOICE_SPOT instance */
    voicespot_status =
        rdspVoiceSpot_OpenInstance(voicespot_control, voicespot_handle, model_blob_size, model_blob, 0, 0);
    PRINTF("rdspVoiceSpot_OpenInstance: voicespot_status = %d\r\n", (int)voicespot_status);

    /* Adaptive threshold modes
    0: fixed threshold (uses the array event_thresholds as thresholds)
    1: adaptive threshold
    2: adaptive sensitivity
    3: adaptive threshold + adaptive sensitivity */
    int32_t adapt_threshold_mode = 3;
    /* Enable use of the Adaptive Threshold mechanism */
    voicespot_status = rdspVoiceSpot_EnableAdaptiveThreshold(voicespot_control, voicespot_handle, adapt_threshold_mode);
    PRINTF("rdspVoiceSpot_EnableAdaptiveThreshold: voicespot_status = %d\r\n", (int)voicespot_status);

    uint8_t *param_blob = (uint8_t *)param_blob_header;

    voicespot_status = rdspVoiceSpot_SetParametersFromBlob(voicespot_control, voicespot_handle, param_blob);
    printf("rdspVoiceSpot_SetParametersFromBlob: voicespot_status = %d\r\n", (int)voicespot_status);

    rdsp_voicespot_version voicespot_version;
    char *voicespot_model_string;

    rdspVoiceSpot_GetLibVersion(voicespot_control, &voicespot_version);
    PRINTF("VoiceSpot library version: %d.%d.%d.%u\r\n", (int)voicespot_version.major, (int)voicespot_version.minor,
           (int)voicespot_version.patch, (unsigned int)voicespot_version.build);
    rdspVoiceSpot_GetModelInfo(voicespot_control, voicespot_handle, &voicespot_version, &voicespot_model_string,
                               &voicespot_class_string, &num_samples_per_frame, &num_outputs);
    PRINTF("VoiceSpot model version: %d.%d.%d\r\n", (int)voicespot_version.major, (int)voicespot_version.minor,
           (int)voicespot_version.patch);
    if (voicespot_model_string != NULL)
        PRINTF("VoiceSpot model string: %s\r\n", voicespot_model_string);
    if (VOICESPOT_NUM_SAMPLES_PER_FRAME != num_samples_per_frame)
    {
        PRINTF("Error: Mismatch in frame size\r\n");
        return -1;
    }

    return 0;
}

static void VoiceSpotProcess(void)
{
    int32_t voicespot_status      = -1;
    int32_t **sfb_output          = NULL;
    int32_t num_frames_per_second = VOICESPOT_SAMPLE_RATE / num_samples_per_frame;

    /* event_thresholds is the array of manually set minimum thresholds for trigger event per class. NULL means
    automatic, i.e. no manually set minimum thresholds. */
    int32_t *event_thresholds     = NULL;
    int32_t event_duration_frames = 2 * num_frames_per_second;
    static int64_t last_event     = 0;
    int32_t processing_period     = 4;
    /* Used for saving score_index of latest trigger */
    int32_t score_index_trigger;
    /* Number of triggers events found in audio */
    static int32_t num_trigger_events = 0;
    /* Number of frames to delay estimation from trigger occurred. If an accurate stop estimate is needed, a
    delay may be needed as trigger may occur before keyword has been completely spoken. */
    int32_t estimation_delay_frames = 0;

    /* Process frame */
    int32_t num_scores = 0;

    /* Set processing level - Will be ignored if adaptive power state is enabled */
    int32_t processing_level;
    if ((frame_count % processing_period) < 4)
    {
        processing_level = RDSP_PROCESSING_LEVEL__FULL;
    }
    else
    {
        processing_level = RDSP_PROCESSING_LEVEL__SKIP_OUTPUT;
    }
    voicespot_status = rdspVoiceSpot_Process(voicespot_control, voicespot_handle, processing_level,
                                             (uint8_t *)frame_buffer, &num_scores, scores, (uint8_t **)sfb_output);

    if (voicespot_status != RDSP_VOICESPOT_OK)
    {
        PRINTF("[%d] rdspVoiceSpot_Process: voicespot_status = %d\r\n", (int)frame_count, (int)voicespot_status);
    }

    if (num_scores > 0)
    {
        /* Store maximum over the scores */
        for (int32_t score_index = 0; score_index < num_outputs; score_index++)
        {
            if (scores[score_index] > max_scores[score_index])
            {
                max_scores[score_index]      = scores[score_index];
                max_frame_count[score_index] = frame_count;
            }
        }
        int32_t condition   = (frame_count - last_event >= event_duration_frames);
        int32_t score_index = rdspVoiceSpot_CheckIfTriggered(voicespot_control, voicespot_handle, scores, condition,
                                                             event_thresholds, processing_period);
        if (score_index >= 0 && frame_count_trigger == -1)
        {
            /* Store trigger frame count for later estimation */
            frame_count_trigger = frame_count;
            /* Store trigger scores for later estimation */
            rdsp_memcpy(scores_trigger, scores, num_outputs * sizeof(int32_t));
            /* Store trigger score_index for later estimation */
            score_index_trigger = score_index;
            last_event          = frame_count;
        }
        if (frame_count_trigger >= 0 && (frame_count - frame_count_trigger >= estimation_delay_frames))
        {
            /* We found a trigger, so estimate the starting point now */
            int32_t start_offset_samples = 0;
            int32_t stop_offset_samples  = 0;
            /* Accuracy of the timing estimate, in frames */
            int32_t timing_accuracy = 4;

            /* Comment out this line if timing estimation is not needed */
            voicespot_status =
                rdspVoiceSpot_EstimateStartAndStop(voicespot_control, voicespot_handle, score_index_trigger, -1,
                                                   timing_accuracy, &start_offset_samples, &stop_offset_samples);
            if (voicespot_status != RDSP_VOICESPOT_OK)
                PRINTF("[%d] rdspVoiceSpot_EstimateStartAndStop: voicespot_status = %d\r\n", (int)frame_count,
                       (int)voicespot_status);
            int32_t trigger_sample    = (frame_count_trigger + 1) * num_samples_per_frame;
            int32_t estimation_sample = (frame_count + 1) * num_samples_per_frame;
            /* Find absolute start sample index by going back from current location */
            int32_t start_sample = estimation_sample - start_offset_samples;
            /* Find absolute stop sample index by going back from current location */
            int32_t stop_sample = estimation_sample - stop_offset_samples;

            /* Increment the number of found trigger events */
            PRINTF(
                "Trigger event found: Event = %d, Frame = %d, class_string = %s, Score = %d, "
                "trigger_sample = %d, estimation_sample = %d, start_offset_samples = %d, start_sample = "
                "%d, stop_offset_samples = %d, stop_sample = %d\r\n",
                (int)num_trigger_events, (int)frame_count_trigger, voicespot_class_string[score_index_trigger],
                (int)scores_trigger[score_index_trigger], (int)trigger_sample, (int)estimation_sample,
                (int)start_offset_samples, (int)start_sample, (int)stop_offset_samples, (int)stop_sample);
            num_trigger_events++;
        }
        /* Indicate that current trigger has been handled */
        frame_count_trigger = -1;
    }
    /* Increment the frame counter */
    frame_count++;
}

#endif

/*!
 * @brief Main function
 */
int main(void)
{
    XOS_Init();
    BOARD_InitPins();
    BOARD_InitClock();
    BOARD_InitDebugConsole();

    /* Map DMA IRQ handler to INPUTMUX selection DSP_INT0_SEL18
     * EXTINT19 = DSP INT 23 */
    xos_register_interrupt_handler(XCHAL_EXTINT19_NUM, (XosIntFunc *)DMA_IRQHandle, DMA1);
    xos_interrupt_enable(XCHAL_EXTINT19_NUM);

    /* Initialize DMA. */
    BOARD_Init_DMA();

    /* Initialize DMIC */
    BOARD_Init_DMIC();

    /* Initialize I2S */
    BOARD_Init_I2S();

    xos_start_main("main", 7, 0);

    PRINTF("VoiceSpot example started on core '%s'\r\n", XCHAL_CORE_ID);

#ifdef VOICE_SPOT

    const uint32_t plugin_heap_memory_size = 100000 & 0xFFFFFFF0; // Size is multiple of 16
    uint8_t __attribute__((aligned(16))) plugin_heap_memory[plugin_heap_memory_size];
    rdsp_plugin_malloc_init(plugin_heap_memory, plugin_heap_memory, plugin_heap_memory_size);

    if (VoiceSpotInit() != 0)
    {
        assert(false);
    }
    PRINTF("Voice spot init done.");
    max_scores      = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    max_frame_count = (int64_t *)rdsp_malloc(num_outputs * sizeof(int64_t));
    scores          = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    /* Used for saving scores of latest trigger */
    scores_trigger = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));

    for (int i = 0; i < num_outputs; i++)
    {
        max_scores[i]      = 0;
        max_frame_count[i] = -1;
    }

#endif

    for (int ibuf = 0; ibuf < IN_BUFFER_NUM_FRAMES; ibuf++)
    {
        s_receiveXfer[ibuf].data                   = &s_buffer[ibuf * VOICESPOT_NUM_SAMPLES_PER_FRAME];
        s_receiveXfer[ibuf].dataWidth              = BYTE_DEPTH_DMIC;
        s_receiveXfer[ibuf].dataSize               = VOICESPOT_NUM_SAMPLES_PER_FRAME * BYTE_DEPTH_DMIC;
        s_receiveXfer[ibuf].dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth;
        int ibuf_next                              = ibuf + 1;
        if (ibuf_next == IN_BUFFER_NUM_FRAMES)
            ibuf_next = 0;
        s_receiveXfer[ibuf].linkTransfer = &s_receiveXfer[ibuf_next];
    }

    DMA_CreateHandle(&s_i2sTxDmaHandle, DEMO_DMA, DEMO_I2S_TX_CHANNEL);
    DMA_CreateHandle(&s_dmicRxDmaHandle, DEMO_DMA, DEMO_DMIC_RX_CHANNEL);

    I2S_TxTransferCreateHandleDMA(DEMO_I2S_TX, &s_i2sTxHandle, &s_i2sTxDmaHandle, NULL, NULL);
    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmicDmaHandle, dmic_Callback, NULL, &s_dmicRxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmicDmaHandle, s_dmaDescriptorPingpong, IN_BUFFER_NUM_FRAMES);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmicDmaHandle, s_receiveXfer, DEMO_DMIC_CHANNEL);

    while (1)
    {
#ifdef VOICE_SPOT
        int32_t n_avail_Mic_buffer = s_writeIndex - s_readIndex;
        if (n_avail_Mic_buffer < 0)
            n_avail_Mic_buffer += BUFFER_SIZE;

        if (n_avail_Mic_buffer >= num_samples_per_frame)
        {
            /* Convert from 16 bit fixed-point to 32 bits */
            for (int32_t i = 0; i < num_samples_per_frame; i++)
            {
#if BYTE_DEPTH_DMIC == 2U

                /* Hifi implementation input is 32 bit fixed-point */
                frame_buffer[i] = s_buffer[s_readIndex + i] << 16;
#else
                /* On board DMIC provides data in 24-bit format */
                frame_buffer[i] = s_buffer[s_readIndex + i] << 8;
#endif
            }

            /* Update Mic read index and check for wrap-around */
            s_readIndex += num_samples_per_frame;
            if (s_readIndex >= BUFFER_SIZE)
                s_readIndex -= BUFFER_SIZE;

            VoiceSpotProcess();
        }
#endif
    }
}
