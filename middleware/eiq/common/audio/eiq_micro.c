/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>

#include "eiq_micro.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"

static EIQ_Micro_t micro;
static EIQ_IBufferAddrUpdater_t readyCallback = NULL;

#if !defined(__ARMCC_VERSION)
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t buffer[AUDIO_NUM * BUFFER_SIZE * BUFFER_NUM], 4);
#else
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint8_t buffer[AUDIO_NUM * BUFFER_SIZE * BUFFER_NUM], 4);
#endif

static uint8_t bufIdx = 0;
static sai_handle_t handle = {0};
static codec_handle_t codecHandle;
static wm8960_config_t wm8960Config;
static codec_config_t boardCodecConfig;
sai_transceiver_t g_transceiverConfig;

static sai_transfer_t xfer = {NULL, 0};

/*!
 * @brief AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
static clock_audio_pll_config_t audioPllConfig;

static void start(void){
  memset(buffer, 0, AUDIO_NUM * BUFFER_SIZE * BUFFER_NUM);
  SAI_RxSoftwareReset(DEMO_SAI, kSAI_ResetTypeSoftware);
  xfer.dataSize = BUFFER_SIZE;
  xfer.data = buffer;
  bufIdx = 0;

  if (SAI_TransferReceiveNonBlocking(DEMO_SAI, &handle, &xfer) != kStatus_Success)
  {
    printf("SAI_TransferReceiveNonBlocking failed!\r\n");
  }
}

static Dims_t getResolution(){
  Dims_t dims;
  dims.width = AUDIO_NUM;
  dims.height = BUFFER_SIZE;
  return dims;
}

static void notify(void){

  if(++bufIdx == AUDIO_NUM * BUFFER_NUM){
      bufIdx = 0;
  }
  xfer.data = buffer + bufIdx * BUFFER_SIZE;
  if (SAI_TransferReceiveNonBlocking(DEMO_SAI, &handle, &xfer) != kStatus_Success)
  {
    printf("SAI_TransferReceiveNonBlocking failed!\r\n");
  }

}

static void setReadyCallback(EIQ_IBufferAddrUpdater_t updater){
  readyCallback = updater;
}

/*!
 * @brief RX callback
 *
 * @param pointer to I2S base address
 * @param pointer to sai edma handler
 * @param status
 * @param pointer to user data
 */
static void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
  if (kStatus_SAI_RxError == status)
  {
    printf("SAI_Rx failed!\r\n");
    return;
  }

  uint32_t addr = (uint32_t)xfer.data;
  notify();

  if (readyCallback != NULL){
    readyCallback((uint32_t) addr);
  }
}

static uint8_t* getReadyBuff()
{
  if ((bufIdx % AUDIO_NUM) == 0U)
  {
    return buffer + (bufIdx - ((bufIdx == AUDIO_NUM) ? AUDIO_NUM : 0))
        * BUFFER_SIZE;
  }
  return NULL;
}

/*!
 * @brief Initializes microphone.
 */
static void init(void)
{
  audioPllConfig.loopDivider = 32;  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
  audioPllConfig.postDivider = 1;   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
  audioPllConfig.numerator = 77;    /* 30 bit numerator of fractional loop divider. */
  audioPllConfig.denominator = 100; /* 30 bit denominator of fractional loop divider */

  wm8960Config.i2cConfig.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE;
  wm8960Config.i2cConfig.codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ;
  wm8960Config.route            = kWM8960_RoutePlaybackandRecord;
#if defined( CPU_MIMXRT1176DVMAA_cm7 ) || defined( CPU_MIMXRT1166DVM6A_cm7 )
  wm8960Config.leftInputSource  = kWM8960_InputDifferentialMicInput3,
  wm8960Config.rightInputSource = kWM8960_InputDifferentialMicInput2,
#else
  wm8960Config.rightInputSource = kWM8960_InputDifferentialMicInput2;
#endif
  wm8960Config.playSource       = kWM8960_PlaySourceDAC;
  wm8960Config.slaveAddress     = WM8960_I2C_ADDR;
  wm8960Config.bus              = kWM8960_BusI2S;
#if defined( CPU_MIMXRT1176DVMAA_cm7 ) || defined( CPU_MIMXRT1166DVM6A_cm7 )
  wm8960Config.format.mclk_HZ   = 24576000U;
#else
  wm8960Config.format.mclk_HZ = 6144000U;
#endif
  wm8960Config.format.sampleRate = kWM8960_AudioSampleRate16KHz;
  wm8960Config.format.bitWidth = kWM8960_AudioBitWidth16bit;
  wm8960Config.master_slave = false;

  boardCodecConfig.codecDevType = kCODEC_WM8960;
  boardCodecConfig.codecDevConfig = &wm8960Config;

  // Board inicialization.
  CLOCK_InitAudioPll(&audioPllConfig);

  /* Enable MCLK clock */
  BOARD_EnableSaiMclkOutput(true);

  /* Microphone clock init */
  BOARD_Microphone_Init();

  /* SAI init */
  SAI_Init(DEMO_SAI);

  /* Clear RCSR interrupt flags. */
  BOARD_ClearRxInterruptFlags();
  SAI_TransferRxCreateHandle(DEMO_SAI, &handle, callback, NULL);

  /* I2S mode configurations */
  SAI_GetClassicI2SConfig(&g_transceiverConfig, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
  g_transceiverConfig.syncMode = kSAI_ModeSync;
  SAI_TransferRxSetConfig(DEMO_SAI, &handle, &g_transceiverConfig);

  /* set bit clock divider */
  SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                        DEMO_AUDIO_DATA_CHANNEL);

  /* master clock configurations */
#if (defined(FSL_FEATURE_SAI_HAS_MCR) && (FSL_FEATURE_SAI_HAS_MCR)) || \
  (defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER))
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
  mclkConfig.mclkHz          = DEMO_AUDIO_MASTER_CLOCK;
  mclkConfig.mclkSourceClkHz = DEMO_SAI_CLK_FREQ;
#endif
  SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
#endif

  /* Use default setting to init codec */
  CODEC_Init(&codecHandle, &boardCodecConfig);
}

EIQ_Micro_t* EIQ_MicroInit(){
  micro.base.getResolution = getResolution;
  micro.base.notify = notify;
  micro.base.start = start;
  micro.setReadyCallback = setReadyCallback;
  micro.getReadyBuff = getReadyBuff;
  init();

  return &micro;
}
