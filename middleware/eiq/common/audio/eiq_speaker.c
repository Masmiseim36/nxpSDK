/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>

#include "eiq_speaker.h"
#include "fsl_debug_console.h"

#include "fsl_sai.h"
#include "fsl_codec_common.h"

#include "fsl_wm8960.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_codec_adapter.h"


static EIQ_Speaker_t speaker;

static sai_handle_t handle = {0};
static EIQ_IUpdater_t readyCallback = NULL;

static sai_transfer_t xfer = {NULL, 0};

extern sai_transceiver_t g_transceiverConfig;

static void start(void)
{
  SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeSoftware);

  xfer.dataSize = BUFFER_SIZE;
}

static Dims_t getResolution()
{
  Dims_t dims;
  dims.width = AUDIO_NUM;
  dims.height = BUFFER_SIZE;
  return dims;
}

static void notify(void)
{
  if(xfer.data == NULL){
    printf("SAI_Tx buffer is undefined.\r\n");
  }

  if (SAI_TransferSendNonBlocking(DEMO_SAI, &handle, &xfer) != kStatus_Success)
  {
    printf("SAI_Tx failed!\r\n");
    return;
  }
}

static void setBuffer(uint32_t buffAddr)
{
  xfer.data = (uint8_t*)buffAddr;
}

static void setReadyCallback(EIQ_IUpdater_t updater){
  readyCallback = updater;
}

/*!
 * @brief TX callback
 *
 * @param pointer to I2S base address
 * @param pointer to sai edma handler
 * @param status
 * @param pointer to user data
 */
static void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
  if (kStatus_SAI_TxError == status)
  {
    printf("SAI_Tx failed!\r\n");
    return;
  }

  if(readyCallback != NULL){
    readyCallback();
  }
}


static void init(){
  /* Clear TCSR interrupt flags. */
  BOARD_ClearTxInterruptFlags();
  SAI_TransferTxCreateHandle(DEMO_SAI, &handle, callback, NULL);
  SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, DEMO_AUDIO_BIT_WIDTH,
                        DEMO_AUDIO_DATA_CHANNEL);
  SAI_TransferTxSetConfig(DEMO_SAI, &handle, &g_transceiverConfig);
}

EIQ_Speaker_t* EIQ_SpeakerInit(){
  speaker.base.getResolution = getResolution;
  speaker.base.notify = notify;
  speaker.base.start = start;
  speaker.setReadyCallback = setReadyCallback;
  speaker.setBuffer = setBuffer;

  init();

  return &speaker;
}
