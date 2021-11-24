/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "stdint.h"
#include "hal_vision_algo_oasis_lite.h"
#include "hal_smart_lock_config.h"
#include "hal_event_descriptor_face_rec.h"

#include "fwk_input_manager.h"
#include "fwk_log.h"
#include "fwk_lpm_manager.h"

#include "fsl_common.h"

#include "hal_input_dev.h"

typedef enum _switch_press_type
{
    kSwitchPressType_Long  = 0,
    kSwitchPressType_Short = 1,
} switch_press_type_t;

typedef enum _switch_id
{
    kSwitchID_1 = 0,
    kSwitchID_2,
    kSwitchID_3,
    kSwitchID_Count,
} switch_id_t;

event_face_rec_t s_FaceRecEvent;

int APP_InputDev_PushButtons_SetEvent(switch_id_t button,
                                      switch_press_type_t pressType,
                                      void **event,
                                      uint32_t *receiverList)
{
    int ret = kStatus_Success;

    if (event == NULL)
    {
        return kStatus_Fail;
    }

    *receiverList = 0;
    switch (button)
    {
        case kSwitchID_1:
            if (pressType == kSwitchPressType_Long)
            {
                unsigned int totalUsageCount;
                LOGD("Long PRESS Detected.");
                FWK_LpmManager_RequestStatus(&totalUsageCount);
                FWK_LpmManager_EnableSleepMode(kLPMManagerStatus_SleepEnable);
            }
            break;

        case kSwitchID_2:
            if ((pressType == kSwitchPressType_Short) || (pressType == kSwitchPressType_Long))
            {
                *receiverList                    = 1 << kFWKTaskID_VisionAlgo;
                s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_DelUser;
                s_FaceRecEvent.delFace.hasName   = false;
                s_FaceRecEvent.delFace.hasID     = false;
                *event                           = &s_FaceRecEvent;
            }
            break;

        case kSwitchID_3:
            if ((pressType == kSwitchPressType_Short) || (pressType == kSwitchPressType_Long))
            {
                *receiverList                    = 1 << kFWKTaskID_VisionAlgo;
                s_FaceRecEvent.eventBase.eventId = kEventFaceRecID_AddUser;
                s_FaceRecEvent.addFace.hasName   = false;
                s_FaceRecEvent.delFace.hasID     = false;
                *event                           = &s_FaceRecEvent;
            }
            break;

        default:
            ret = kStatus_Fail;
            break;
    }

    return ret;
}
