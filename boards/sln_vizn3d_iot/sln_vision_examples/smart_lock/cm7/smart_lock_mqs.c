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

#include "fsl_common.h"
#include "can_I_help.h" /* music extract, 48kHz, 16bit PCM, mono */
#include "tone_timeout.h"

#define PROMPT_EN

#ifdef PROMPT_EN
#include "ok.h"

#include "recognition_successful.h"
#include "recognition_failed.h"

#include "starting_registration.h"
#include "registration_successful.h"
#include "registration_failed.h"

#include "starting_deregistration.h"
#include "deregistration_failed.h"
#include "deregistration_successful.h"

#include "ble_connected.h"
#include "ble_disconnected.h"

#include "enter_sleep.h"
#include "fake_face.h"
#include "look_at_the_camera.h"

#if HEADLESS_ENABLE
#include "turn_face_to_left.h"
#include "turn_face_to_right.h"
#endif

#elif defined(PROMPT_CN)

#include "ok_cn.h" /* music extract, 48kHz, 16bit PCM, mono */

#include "recognition_successful_cn.h"
#include "recognition_failed_cn.h"

#include "starting_registration_cn.h"
#include "registration_successful_cn.h"
#include "registration_failed_cn.h"

#include "starting_deregistration_cn.h"
#include "deregistration_failed_cn.h"
#include "deregistration_successful_cn.h"

#include "ble_connected_cn.h"
#include "ble_disconnected_cn.h"

#include "enter_sleep_cn.h"
#include "fake_face_cn.h"
#include "look_at_the_camera_cn.h"

#if HEADLESS_ENABLE
#include "turn_face_to_left_cn.h"
#include "turn_face_to_right_cn.h"
#endif
#endif

#include "hal_output_dev.h"
#include "fwk_log.h"

static oasis_lite_state_t s_OasisLiteState = kOASISLiteState_Count;
#if HEADLESS_ENABLE
static oasis_lite_headless_reg_process_t s_HeadlessRegStatus = OASIS_LITE_HEADLESS_REG_START;
#endif

static void _Vision_InferCompleteDecode(oasis_lite_result_t inferResult, void const **audio, uint32_t *len)
{
    uint32_t result = inferResult.result;
    *audio          = NULL;
    *len            = 0;

    /* check the fake face alert */
    if (inferResult.qualityCheck == kOasisLiteQualityCheck_FakeFace)
    {
        *audio = fake_face_audio;
        *len   = FAKE_FACE_AUDIO_LEN;
    }
    /* check the non frontal alert */
    else if ((inferResult.qualityCheck == kOasisLiteQualityCheck_NonFrontalFace)
#if HEADLESS_ENABLE
             && (inferResult.headless_reg_status == OASIS_LITE_HEADLESS_REG_FRONT_FACE)
#endif
    )
    {
        *audio = look_at_the_camera_audio;
        *len   = LOOK_AT_THE_CAMERA_AUDIO_LEN;
    }

    switch (inferResult.state)
    {
        case kOASISLiteState_Recognition:
        {
            switch (result)
            {
                case kOASISLiteRecognitionResult_Success:
                {
                    *audio = recognition_successful_audio;
                    *len   = RECOGNITION_SUCCESSFUL_AUDIO_LEN;
                }
                break;
                case kOASISLiteRecognitionResult_Timeout:
                {
                    unsigned int totalUsageCount;
                    FWK_LpmManager_RequestStatus(&totalUsageCount);
                    if (totalUsageCount == 0)
                    {
                        *audio = recognition_failed_audio;
                        *len   = RECOGNITION_FAILED_AUDIO_LEN;
                    }
                }
                break;

                default:
                {
                }
                break;
            }
        }
        break;
        case kOASISLiteState_Registration:
            switch (result)
            {
                case kOASISLiteRegistrationResult_Invalid:
                    if (s_OasisLiteState != kOASISLiteState_Registration)
                    {
                        *audio = starting_registration_audio;
                        *len   = STARTING_REGISTRATION_AUDIO_LEN;
                    }
                    break;
                case kOASISLiteRegistrationResult_Success:
                    *audio = registration_successful_audio;
                    *len   = REGISTRATION_SUCCESSFUL_AUDIO_LEN;
                    break;

                case kOASISLiteRegistrationResult_Duplicated:
                    *audio = registration_failed_audio;
                    *len   = REGISTRATION_FAILED_AUDIO_LEN;
                    break;

                case kOASISLiteRegistrationResult_Timeout:
                    *audio = registration_failed_audio;
                    *len   = REGISTRATION_FAILED_AUDIO_LEN;
                    break;
                default:
                {
                }
            }

#if HEADLESS_ENABLE
            if (s_HeadlessRegStatus != inferResult.headless_reg_status)
            {
                switch (inferResult.headless_reg_status)
                {
                    case OASIS_LITE_HEADLESS_REG_FRONT_FACE:
                    {
                        *audio = look_at_the_camera_audio;
                        *len   = LOOK_AT_THE_CAMERA_AUDIO_LEN;
                    }
                    break;

                    case OASIS_LITE_HEADLESS_REG_LEFT_FACE:
                    {
                        *audio = turn_face_to_left_audio;
                        *len   = TURN_FACE_TO_LEFT_AUDIO_LEN;
                    }
                    break;

                    case OASIS_LITE_HEADLESS_REG_RIGHT_FACE:
                    {
                        *audio = turn_face_to_right_audio;
                        *len   = TURN_FACE_TO_RIGHT_AUDIO_LEN;
                    }
                    break;

                    default:
                        break;
                }
            }
#endif
            break;

        case kOASISLiteState_DeRegistration:
            switch (result)
            {
                case kOASISLiteDeregistrationResult_Invalid:
                    if (s_OasisLiteState != kOASISLiteState_DeRegistration)
                    {
                        *audio = starting_deregistration_audio;
                        *len   = STARTING_DEREGISTRATION_AUDIO_LEN;
                    }
                    break;
                case kOASISLiteDeregistrationResult_Success:
                    *audio = deregistration_successful_audio;
                    *len   = DEREGISTRATION_SUCCESSFUL_AUDIO_LEN;
                    break;
                case kOASISLiteDeregistrationResult_Timeout:
                    *audio = deregistration_failed_audio;
                    *len   = DEREGISTRATION_FAILED_AUDIO_LEN;
                    break;

                default:
                {
                }
            }
            break;

        default:
        {
        }
    }
    s_OasisLiteState = inferResult.state;
#if HEADLESS_ENABLE
    s_HeadlessRegStatus = inferResult.headless_reg_status;
#endif
}

int APP_OutputDev_MqsAudio_InferCompleteDecode(output_algo_source_t source,
                                               void *inferResult,
                                               void const **audio,
                                               uint32_t *len)
{
    if (source == kOutputAlgoSource_Vision)
    {
        _Vision_InferCompleteDecode(*(oasis_lite_result_t *)inferResult, audio, len);
    }
    else if (source == kOutputAlgoSource_LPM)
    {
        *audio = enter_sleep_audio;
        *len   = ENTER_SLEEP_AUDIO_LEN;
    }

    return 0;
}

int APP_OutputDev_MqsAudio_InputNotifyDecode(const output_dev_t *dev,
                                             void *inputData,
                                             void const **audio,
                                             uint32_t *len)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    event_base_t eventBase    = *(event_base_t *)inputData;
    switch (eventBase.eventId)
    {
        case (kEventFaceRecID_AddUser):
        {
            /* If a registration is not already in progress */
            if (s_OasisLiteState != kOASISLiteState_Registration)
            {
                *audio = starting_registration_audio;
                *len   = STARTING_REGISTRATION_AUDIO_LEN;
            }
        }
        break;
        case (kEventFaceRecID_DelUser):
        {
            /* If a deregistration is not already in progress */
            if (s_OasisLiteState != kOASISLiteState_DeRegistration)
            {
                *audio = starting_deregistration_audio;
                *len   = STARTING_DEREGISTRATION_AUDIO_LEN;
            }
        }
        break;
        case (kEventID_SetBLEConnection):
        {
            event_face_rec_t event = *(event_face_rec_t *)inputData;
            if (event.wuart.status == 1)
            {
                *audio = ble_connected_audio;
                *len   = BLE_CONNECTED_AUDIO_LEN;
            }
            else if (event.wuart.status == 0)
            {
                *audio = ble_disconnected_audio;
                *len   = BLE_DISCONNECTED_AUDIO_LEN;
            }
        }
        break;

        default:
            break;
    }
    return error;
}
