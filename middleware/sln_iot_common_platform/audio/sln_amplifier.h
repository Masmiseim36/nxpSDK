/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SLN_AMPLIFIER_H__
#define __SLN_AMPLIFIER_H__

#include "FreeRTOS.h"
#include "event_groups.h"
#include "fsl_common.h"
#include "fsl_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Globals
 ******************************************************************************/
typedef int32_t amplifier_status_t;
typedef void (*amp_get_cal_callback_t)(uint8_t *state);
typedef void (*amp_set_cal_callback_t)(uint8_t state);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/**
 * @brief Initialize SAI and DMA for amplifier streaming
 * This function also initialises the amplifier (TFA9894)
 * and calibrates it
 *
 * @param pu8BuffNum    The number of buffers for the transmission queue SAI->DMA->AMP
 * @return              0 if success
 */
amplifier_status_t SLN_AMP_Init(volatile uint8_t *pu8BuffNum);

/**
 * @brief Start the DMA read
 *
 * @return              0 if success
 */
amplifier_status_t SLN_AMP_Read(void);

/**
 * @brief Writes the data to the amplifier
 * The data is chop into 512KB chunks and sent to the SAI interface
 *
 * @param data                  Pointer to the data that will be sent over the DMA
 * @param length                The length of the data
 * @return amplifier_status_t   0 if success
 */
amplifier_status_t SLN_AMP_Write(uint8_t *data, uint32_t length);

/**
 * @brief Writes the data to the amplifier in a loop
 * The data will be written by the function and then by the audio_send_task in a loop
 * until we call SLN_AMP_AbortWrite
 *
 * @param data                  Pointer to the data that will be sent over the DMA
 * @param length                The length of the data
 * @return amplifier_status_t   0 if success
 */
amplifier_status_t SLN_AMP_WriteLoop(uint8_t *data, uint32_t length);

/**
 * @brief Writes data to the amplifier
 * This functions sends the data in one chunk to the SAI interfce without waiting
 * for the amplifier TX callback
 *
 * @param data                  Pointer to the data that will be sent over the DMA
 * @param length                The length of the data
 * @return amplifier_status_t   0 if success
 */
amplifier_status_t SLN_AMP_WriteNoWait(uint8_t *data, uint32_t length);

/**
 * @brief Writes data to the amplifier
 * This function is blocking meaning it waits, after each SAI send, for the
 * g_dmaTxComplete to be cleared by the amplifier TX callback. The data is sent
 * in 512KB chunks
 *
 * @param data                  Pointer to the data that will be sent over the DMA
 * @param length                The length of the data
 * @return amplifier_status_t   0 if success
 */
amplifier_status_t SLN_AMP_WriteBlocking(uint8_t *data, uint32_t length);

/**
 * @brief Writes the _med_ui_wakesound_wav to the amplifier
 *
 * @return amplifier_status_t   0 if success
 */
amplifier_status_t SLN_AMP_WriteDefault(void);

/**
 * @brief Terminates the SAI transfer to the amplifier
 * It cancels the write loop
 *
 * @return amplifier_status_t
 */
amplifier_status_t SLN_AMP_AbortWrite(void);

/**
 * @brief Gets the amplifier RX data buffer
 *
 * @return uint8_t*     Pointer to the RX data buffer
 */
uint8_t *SLN_AMP_GetLoopBackBuffer(void);

/**
 * @brief Sets the loopback event group
 *
 * @param eventGroup    Pointer to the event group handler
 */
void SLN_AMP_SetLoopBackEventGroup(EventGroupHandle_t *eventGroup);

/**
 * @brief Sets the loopback event bits
 *
 * @param event         Event bits
 */
void SLN_AMP_SetLoopBackEventBits(EventBits_t event);

/**
 * @brief Sets the amplifier volume
 *
 * @param volume        The amplifier volume from 0 to 100
 */
void SLN_AMP_SetVolume(uint8_t volume);

/**
 * @brief Gets the amplifier TX handler
 *
 * @return void*        Pointer to the amplifier TX handler
 */
void *SLN_AMP_GetAmpTxHandler();

/**
 * @brief Abort the amplifier
 *
 * @return void
 */
void SLN_AMP_Abort(void);

/**
 * @brief Enable DMA channel and SAI interrupts for AMP loopback
 *
 */
void SLN_AMP_LoopbackEnable(void);

/**
 * @brief Disable DMA channel and SAI interrupts for AMP loopback
 *
 */
void SLN_AMP_LoopbackDisable(void);

/**
 * @brief Set default audio data used by SLN_AMP_WriteDefault.
 *
 * @param data                  pointer to the default audio data.
 * @param length                The length of the data
 */
void SLN_AMP_SetDefaultAudioData(uint8_t* data, uint32_t len);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif //__SLN_AMPLIFIER_H__
