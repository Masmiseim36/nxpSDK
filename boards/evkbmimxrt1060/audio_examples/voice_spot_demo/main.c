/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"

#ifdef VOICE_SPOT
#include "arm_math.h"
#include "public/rdsp_voicespot.h"
#include "public/rdsp_voicespot_utils.h"
#include "HeyNXP_en-US_1_params.h"
#include "HeyNXP_en-US_1.h"
#endif
#if defined DEMO_CODEC_WM8960
#include "fsl_wm8960.h"
#else
#include "fsl_cs42448.h"
#endif
#include "fsl_codec_adapter.h"
#include "fsl_dmamux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI instance and clock */
#ifndef DEMO_CODEC_WM8960
#define DEMO_CODEC_WM8960 1
#endif
#ifndef DEMO_CODEC_CS42448
#define DEMO_CODEC_CS42448 0
#endif
#if DEMO_CODEC_WM8960 && DEMO_CODEC_CS42448
#error "Duplicate codec defined"
#endif

#define DEMO_SAI                       SAI1
#define DEMO_SAI_CHANNEL               (0)
#define DEMO_SAI_IRQ                   SAI1_IRQn
#define DEMO_SAITxIRQHandler           SAI1_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE          kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE          kSAI_ModeSync
#define DEMO_SAI_TX_BIT_CLOCK_POLARITY kSAI_PolarityActiveLow
#define DEMO_SAI_MCLK_OUTPUT           true

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth32bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* IRQ */
#define DEMO_SAI_TX_IRQ SAI1_IRQn
#define DEMO_SAI_RX_IRQ SAI1_IRQn

/* DMA */
#define DEMO_DMA             DMA0
#define DEMO_DMAMUX          DMAMUX
#define DEMO_TX_EDMA_CHANNEL (0U)
#define DEMO_RX_EDMA_CHANNEL (1U)
#define DEMO_SAI_TX_SOURCE   kDmaRequestMuxSai1Tx
#define DEMO_SAI_RX_SOURCE   kDmaRequestMuxSai1Rx

#if DEMO_CODEC_CS42448
#define DEMO_CS42448_I2C_INSTANCE      3
#define DEMO_CODEC_POWER_GPIO          GPIO1
#define DEMO_CODEC_POWER_GPIO_PIN      0
#define DEMO_CODEC_RESET_GPIO          GPIO1
#define DEMO_CODEC_RESET_GPIO_PIN      2
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (47U)
#define DEMO_SAI_MASTER_SLAVE          kSAI_Master
#else
#define DEMO_WM8960_I2C_INSTANCE       1
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)
#define DEMO_SAI_MASTER_SLAVE          kSAI_Slave
#endif

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* I2C instance and clock */
#define DEMO_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define BOARD_MASTER_CLOCK_CONFIG()

#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 100U
#endif
#define VOICESPOT_NUM_SAMPLES_PER_FRAME 200
#define VOICESPOT_SAMPLE_RATE           16000
#define BYTE_WIDTH                      4U
/* One DMA transfer has to be divisible by bytes_per_frame * count (= 64), 64 * 25 = BUFFER_SIZE) */
#define DMA_TRANSFER_MULT 2U
#define BUFFER_SIZE       (VOICESPOT_NUM_SAMPLES_PER_FRAME * BYTE_WIDTH * DMA_TRANSFER_MULT)
#define BUFFER_NUMBER     2U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined DEMO_CODEC_CS42448
void BORAD_CodecReset(bool state);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if DEMO_CODEC_WM8960
wm8960_config_t wm8960Config = {
    .i2cConfig = {.codecI2CInstance = DEMO_WM8960_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route     = kWM8960_RoutePlaybackandRecord,
    .leftInputSource  = kWM8960_InputDifferentialMicInput3,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusI2S,
    .format           = {.mclk_HZ    = 6144000U * 2,
               .sampleRate = kWM8960_AudioSampleRate16KHz,
               .bitWidth   = kWM8960_AudioBitWidth32bit},
    .master_slave     = true,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};
#elif DEMO_CODEC_CS42448
cs42448_config_t cs42448Config = {
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = BORAD_CodecReset,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = DEMO_CS42448_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .format       = {.mclk_HZ = 16384000U, .sampleRate = 16000U, .bitWidth = 16U},
    .bus          = kCS42448_BusI2S,
    .slaveAddress = CS42448_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &cs42448Config};
#else
#error "no codec enabled, please check."
#endif
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], BYTE_WIDTH);
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t txHandle);
AT_QUICKACCESS_SECTION_DATA(sai_edma_handle_t rxHandle);
static uint32_t tx_index = 0U, rx_index = 0U;
volatile uint32_t bufferRead    = 0;
volatile uint32_t bufferWritten = 0;
uint32_t firstRead              = 1;
uint32_t firstWrite             = 1;
edma_handle_t dmaTxHandle = {0}, dmaRxHandle = {0};
extern codec_config_t boardCodecConfig;
codec_handle_t codecHandle;

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
static int32_t *frame_buffer;
static int32_t *scores;
static int32_t *scores_trigger;
static int numrx_errors = 0;
static int numtx_errors = 0;
static int buf_num      = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
    }
}


#if DEMO_CODEC_CS42448
void BORAD_CodecReset(bool state)
{
    if (state)
    {
        GPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 0U);
    }
}
#endif
static void rx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
        numrx_errors++;
    }
    else
    {
        bufferRead++;
    }
}

static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        numtx_errors++;
    }
    else
    {
        bufferWritten++;
    }
}
#ifdef VOICE_SPOT

static int32_t VoiceSpotInit(void)
{
    int32_t data_type        = RDSP_DATA_TYPE__FLOAT32;
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

    uint32_t param_blob_size = sizeof(param_blob_header);
    uint8_t *param_blob      = (uint8_t *)param_blob_header;

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
    int32_t voicespot_status = -1;
    int32_t use_trigger_key  = 0;
    int32_t trigger_keys_num_entries;
    int32_t *trigger_key          = NULL;
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
    int32_t buffer_size_num_samples = 0;

    int32_t offset = rx_index * BUFFER_SIZE + buf_num * VOICESPOT_NUM_SAMPLES_PER_FRAME * sizeof(int32_t);
    int8_t *src    = Buffer + offset;

    arm_q31_to_float((int32_t *)src, (float32_t *)frame_buffer, VOICESPOT_NUM_SAMPLES_PER_FRAME);

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
    sai_transfer_t xfer;
    edma_config_t dmaConfig = {0};
    sai_transceiver_t saiConfig;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
#if DEMO_CODEC_WM8960
    BOARD_InitWM8960Pins();
#else
    BOARD_InitCS42448Pins();
#endif
    BOARD_BootClockRUN();
    CLOCK_InitAudioPll(&audioPllConfig);
    BOARD_InitDebugConsole();

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);

    /* Init DMAMUX */
    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_TX_EDMA_CHANNEL, (uint8_t)DEMO_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_TX_EDMA_CHANNEL);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_RX_EDMA_CHANNEL, (uint8_t)DEMO_SAI_RX_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_RX_EDMA_CHANNEL);

#if DEMO_CODEC_CS42448
    /* enable codec power */
    GPIO_PinWrite(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, 1U);
#endif

    PRINTF("VoiceSpot example started!\r\n");

    /* Init DMA and create handle for DMA */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, DEMO_DMA, DEMO_TX_EDMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, DEMO_DMA, DEMO_RX_EDMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA, DEMO_TX_EDMA_CHANNEL, DEMO_SAI_TX_EDMA_CHANNEL);
    EDMA_SetChannelMux(DEMO_DMA, DEMO_RX_EDMA_CHANNEL, DEMO_SAI_RX_EDMA_CHANNEL);
#endif

    /* SAI init */
    SAI_Init(DEMO_SAI);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, tx_callback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rx_callback, NULL, &dmaRxHandle);

    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&saiConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_MonoRight, 1U << DEMO_SAI_CHANNEL);
    saiConfig.syncMode              = DEMO_SAI_TX_SYNC_MODE;
    saiConfig.bitClock.bclkPolarity = DEMO_SAI_TX_BIT_CLOCK_POLARITY;
    saiConfig.masterSlave           = DEMO_SAI_MASTER_SLAVE;
    SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &saiConfig);
    saiConfig.syncMode = DEMO_SAI_RX_SYNC_MODE;
    SAI_TransferRxSetConfigEDMA(DEMO_SAI, &rxHandle, &saiConfig);

    /* set bit clock divider */
    SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);
    SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                          DEMO_AUDIO_DATA_CHANNEL);

    /* master clock configurations */
    BOARD_MASTER_CLOCK_CONFIG();

    /* Use default setting to init codec */
    if (CODEC_Init(&codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        assert(false);
    }
    if (CODEC_SetVolume(&codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        assert(false);
    }
#ifdef VOICE_SPOT

    if (VoiceSpotInit() != 0)
    {
        assert(false);
    }

    max_scores      = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    max_frame_count = (int64_t *)rdsp_malloc(num_outputs * sizeof(int64_t));
    /* Frame buffer into VOICE_SPOT */
    frame_buffer = (int32_t *)rdsp_malloc(num_samples_per_frame * sizeof(int32_t));
    scores       = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));
    /* Used for saving scores of latest trigger */
    scores_trigger = (int32_t *)rdsp_malloc(num_outputs * sizeof(int32_t));

    for (int i = 0; i < num_outputs; i++)
    {
        max_scores[i]      = 0;
        max_frame_count[i] = -1;
    }

#endif
    while (1)
    {
        xfer.data     = Buffer + rx_index * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        xfer.data     = Buffer + tx_index * BUFFER_SIZE;
        xfer.dataSize = BUFFER_SIZE;
        if (firstRead)
        {
            if (kStatus_Success == SAI_TransferReceiveEDMA(DEMO_SAI, &rxHandle, &xfer))
            {
                rx_index++;
            }
            if (rx_index == BUFFER_NUMBER)
            {
                rx_index = 0U;
            }
            firstRead = 0;
        }

        while (!bufferRead)
        {
        }

#ifdef VOICE_SPOT
        for (buf_num = 0; buf_num < DMA_TRANSFER_MULT; buf_num++)
        {
            VoiceSpotProcess();
        }
#endif
        if (kStatus_Success == SAI_TransferReceiveEDMA(DEMO_SAI, &rxHandle, &xfer))
        {
            rx_index++;
            bufferRead = 0;
        }
        if (rx_index == BUFFER_NUMBER)
        {
            rx_index = 0U;
        }

        if (firstWrite)
        {
            if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
            {
                tx_index++;
            }
            if (tx_index == BUFFER_NUMBER)
            {
                tx_index = 0U;
            }
            firstWrite = 0;
        }
        while (!bufferWritten)
        {
        }

        if (kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
        {
            tx_index++;
            bufferWritten = 0;
        }
        if (tx_index == BUFFER_NUMBER)
        {
            tx_index = 0U;
        }
    }
}
