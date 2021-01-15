/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "eiq_audio_worker.h"
#include "eiq_speaker.h"
#include "eiq_micro.h"
#include "stdbool.h"
#include "fsl_debug_console.h"

EIQ_AudioWorker_t worker;

EIQ_IWorkerUpdater_t handler;

uint8_t* pExtract = NULL;
bool ready = false;

static status_t start()
{
  worker.receiver->base.start();
  worker.sender->base.start();
  return kStatus_Success;
}

static bool isReady()
{
  return ready;
}

static void notify()
{
  worker.receiver->base.notify();
}

static Dims_t getResolution(){
  return worker.receiver->base.getResolution();
}

static uint8_t* getData()
{
  if(pExtract == NULL){
    printf("Internal buffer is not allocated.\r\n");
    return NULL;
  }
  ready = false;
  return (uint8_t*)pExtract;
}

static void refresh(uint32_t bufferAddr)
{
  ready = false;

  worker.sender->setBuffer(bufferAddr);
  worker.sender->base.notify();

  uint8_t* p = worker.receiver->getReadyBuff();
  if( p != NULL){
      pExtract = p;
      if(handler != NULL)
      {
        handler((EIQ_IWorker_t*)&worker);
      }
      ready = true;
  }

}

static void setReadyCallback(EIQ_IWorkerUpdater_t iworker)
{
  if(iworker != NULL)
  {
      handler = iworker;
  }
}

EIQ_AudioWorker_t* EIQ_AudioWorkerInit()
{
  worker.base.start = start;
  worker.base.isReady = isReady;
  worker.base.getResolution = getResolution;
  worker.base.notify = notify;
  worker.base.getData = getData;
  worker.base.refresh = refresh;
  worker.base.setReadyCallback = setReadyCallback;
  worker.receiver = EIQ_MicroInit();
  worker.sender = EIQ_SpeakerInit();

  worker.receiver->setReadyCallback(worker.base.refresh);

  return &worker;
}

