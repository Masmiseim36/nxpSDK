/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief output console device implementation.
 */

#include "FreeRTOS.h"

#include "fwk_log.h"
#include "fwk_output_manager.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_output_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif
int HAL_OutputDev_Console_Register();
#if defined(__cplusplus)
}
#endif

static hal_output_status_t HAL_OutputDev_Console_InferComplete(const output_dev_t *dev,
                                                               output_algo_source_t source,
                                                               void *inferResult)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    //    LOGD("face detected !!!!!!!!!!!!!! \n\r");
    return error;
}

static hal_output_status_t HAL_OutputDev_Console_InputNotify(const output_dev_t *receiver, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    event_base_t eventBase    = *(event_base_t *)data;

    if (kEventFaceRecID_AddUser == eventBase.eventId)
    {
        LOGD("Add Face !!!!!!!!!!!!!! \n\r");
    }

    return error;
}

const static output_dev_event_handler_t output_console_handler = {
    .inferenceComplete = HAL_OutputDev_Console_InferComplete,
    .inputNotify       = HAL_OutputDev_Console_InputNotify,
};

static hal_output_status_t HAL_OutputDev_Console_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    if (FWK_OutputManager_RegisterEventHandler(dev, &output_console_handler) != 0)
        error = kStatus_HAL_OutputError;
    return error;
}

const static output_dev_operator_t s_OutputDev_ConsoleOps = {
    .init   = NULL,
    .deinit = NULL,
    .start  = HAL_OutputDev_Console_Start,
    .stop   = NULL,
};

static output_dev_t s_OutputDev_Console = {
    .name         = "console",
    .attr.type    = kOutputDevType_Other,
    .attr.reserve = NULL,
    .ops          = &s_OutputDev_ConsoleOps,
};

int HAL_OutputDev_Console_Register()
{
    int error = 0;
    LOGD("HAL_OutputDev_Console_Register");
    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_Console);
    return error;
}
