/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief output device manager framework declaration.
 */

#ifndef _FWK_OUTPUT_MANAGER_H_
#define _FWK_OUTPUT_MANAGER_H_

#include "hal_output_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*! @brief Capabilities of an output device */
typedef struct
{
    /* inference complete event handler */
    hal_output_status_t (*inferenceComplete)(const output_dev_t *dev, output_algo_source_t source, void *inferResult);
    /* input notify event handler */
    hal_output_status_t (*inputNotify)(const output_dev_t *dev, void *data);
    /* input kFWKMessageID_InputAudioReceived event handler */
    hal_output_status_t (*dump)(const output_dev_t *dev, void *data);

} output_dev_event_handler_t;

/**
 * @brief Register a display device. All display devices need to be registered before FWK_OutputManager_Start is called.
 * @param dev Pointer to an output device structure
 * @return int Return 0 if registration was successful
 */
int FWK_OutputManager_DeviceRegister(output_dev_t *dev);

/**
 * @brief Init internal structures for output manager.
 * @return int Return 0 if the init process was successful
 */
int FWK_OutputManager_Init();

/**
 * @brief Spawn output manager task which will call init/start for all registered output devices.
 * @return int Return 0 if starting was successful
 */
int FWK_OutputManager_Start();

/**
 * @brief DeInit internal structures for output manager.
 * @return int Return 0 if the deinit process was successful
 */
int FWK_OutputManager_Deinit();

/**
 * @brief A registered output device doesn't need to be also active. After the start procedure, the output device
 *          can register a handler of capabilities to recive events.
 * @param dev Device that register the handler
 * @param handler Pointer to a handler
 * @return int Return 0 if the registration of the event handler was successful
 */

int FWK_OutputManager_RegisterEventHandler(const output_dev_t *dev, const output_dev_event_handler_t *handler);

/**
 * @brief A registered output device doesn't need to be also active. A device can call this function to unsubscribe
 * from receiving events
 * @param dev Device that unregister the handler
 * @return int Return 0 if the unregistration of the event handler was successful
 */
int FWK_OutputManager_UnregisterEventHandler(const output_dev_t *dev);

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_OUTPUT_MANAGER_H_*/
