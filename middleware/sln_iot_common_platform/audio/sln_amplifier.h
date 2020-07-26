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

#ifndef __SLN_AMPLIFIER_H__
#define __SLN_AMPLIFIER_H__

#include "FreeRTOS.h"
#include "event_groups.h"
#include "fsl_common.h"
#include "fsl_edma.h"
#include "pdm_pcm_definitions.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef int32_t amplifier_status_t;

#if USE_TFA9894
/* Macro for SAI TX */
#define DEMO_SAI_OUT SAI3
/* I2C instance and clock */
#define DEMO_I2C LPI2C1
/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                       \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

#define DEMO_SAI_IRQ SAI3_IRQn
#define DEMO_SAI_TX_IRQ SAI3_TX_IRQn
#define DEMO_SAI_RX_IRQ SAI3_RX_IRQn
#define SAI_UserIRQHandler SAI3_TX_IRQHandler
#define SAI_RxUserIRQHandler SAI3_RX_IRQHandler
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMuxSai3Tx
#endif

/*******************************************************************************
 * Globals
 ******************************************************************************/
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

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif //__SLN_AMPLIFIER_H__
