/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the license terms that accompany it. By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

/*
 * @brief SLN Oasis-Lite Vision Algorithm HAL driver implementation for the smart lock application.
 */

#if defined(SMART_LOCK_2D) || defined(SMART_ACCESS_2D)


#include <FreeRTOS.h>
#include <stdlib.h>
#include <stdio.h>
#include <task.h>
#include "fwk_log.h"
#include "fwk_sln_platform.h"
#include "fwk_vision_algo_manager.h"
#include "fwk_profiler.h"
#include "fwk_lpm_manager.h"
#include "hal_lpm_dev.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_sln_timer.h"
#include "hal_vision_algo_oasis_lite.h"
#include "hal_smart_lock_config.h"


/*******************************************************************************
 * Defines
 ******************************************************************************/
#define OASISLOG_ENABLE 1
#if OASISLOG_ENABLE
#define OASIS_LOGI LOGI
#define OASIS_LOGD LOGD
#define OASIS_LOGE LOGE
#define OASIS_LOGV LOGV
#else
#define OASIS_LOGI(...)
#define OASIS_LOGD(...)
#define OASIS_LOGE(...)
#define OASIS_LOGV(...)
#endif

#define RESULT_TIMER        2000
#define REGISTERING_TIMER   25000
#define DEREGISTERING_TIMER 25000
#define RECOGNITION_TIMER   25000
#define DETECT_FACE_TIMER   10000
#define ENTER_SLEEP_TIMER   30000
#define QUALITY_CHECK_TIMER 1500

typedef struct _oasis_lite_param
{
    OASISLTInitPara_t config;
    oasis_lite_result_t result;
    ImageFrame_t frames[OASISLT_INT_FRAME_IDX_LAST];
    ImageFrame_t *pframes[OASISLT_INT_FRAME_IDX_LAST];
    OASISRunFlag_t run_flag;

    /* 1. pDetFaceTimer + pRegTimer handle registration flow,
     * if no face detected during pDetFaceTimer, will exit registration in advance.
     * 2. pDetFaceTimer + pDeregTimer handle deregistration flow,
     * if no face detected during pDetFaceTimer, will exit deregistration in advance.
     * 3. pDetFaceTimer + pRecTimer handle recognition flow,
     * if no face detected during pDetFaceTimer, will turn off in advance.
     *
     * */
    sln_timer_t *pDetFaceTimer; /* timer for detect no face */
    sln_timer_t *pRegTimer;     /* timer for registering */
    sln_timer_t *pDeregTimer;   /* timer for deregistering */
    sln_timer_t *pRecTimer;     /* timer for recognition */
    sln_timer_t *pResultTimer;  /* timer for result */
    sln_timer_t *pQualityCheckTimer; /* timer for quality check advertise */

    uint8_t timer_timeout;
    uint8_t timerTimeoutQualityCheck;
    oasis_lite_quality_check_result_t qualityCheck;
    vision_algo_dev_t *dev;
    oasis_lite_mode_t mode; // 0:smart-lock   1:ffi
#if HEADLESS_ENABLE
    uint8_t headless_reg_status;
#endif
} oasis_lite_param_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static oasis_lite_param_t s_OasisLite;
static char s_UserName[64];
static char *s_UserNameReference = NULL;

/*dtc buffer for inference engine optimization*/
FWKDATA static uint8_t s_DTCOPBuf[DTC_OPTIMIZE_BUFFER_SIZE];

#if OASIS_STATIC_MEM_BUFFER
__attribute__((section(".bss.$SRAM_OCRAM_CACHED"), aligned(64))) uint8_t g_OasisMemPool[OASIS_STATIC_MEM_POOL];
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void _oasis_timer_quality_check(void *arg);

/*******************************************************************************
 * Code
 ******************************************************************************/
static inline void _oasis_lite_dev_response(event_base_t eventBase,
                                            void *response,
                                            event_status_t status,
                                            unsigned char isFinished)
{
    if (eventBase.respond != NULL)
    {
        eventBase.respond(eventBase.eventId, response, status, isFinished);
    }
}

static void _oasis_lite_dev_notify_result(const vision_algo_dev_t *dev, oasis_lite_result_t *result)
{
    if (dev != NULL && result != NULL && dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, kVAlgoEvent_VisionResultUpdate, result, sizeof(oasis_lite_result_t), fromISR);
    }
}

static void _oasis_lite_dev_led_pwm_control(const vision_algo_dev_t *dev, event_common_t *event)
{
    if (dev != NULL && event != NULL && dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, kVAlgoEvent_VisionLedPwmControl, event, sizeof(event_common_t), fromISR);
    }
}

static void _oasis_lite_dev_camera_exposure_control(const vision_algo_dev_t *dev, event_common_t *event)
{
    if (dev != NULL && event != NULL && dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev->id, kVAlgoEvent_VisionCamExpControl, event, sizeof(event_common_t), fromISR);
    }
}

static void _oasis_lite_EvtCb(ImageFrame_t *frames[], OASISLTEvt_t evt, OASISLTCbPara_t *para, void *userData)
{
    OASIS_LOGI("  OASIS_EVT:[%d]", evt);
    oasis_lite_param_t *pOasisLite = (oasis_lite_param_t *)userData;
    oasis_lite_result_t *result    = &pOasisLite->result;
    oasis_lite_debug_t *debugInfo  = &pOasisLite->result.debug_info;

    switch (evt)
    {
        case OASISLT_EVT_DET_START:
        {
            FWK_Profiler_StartEvent(OASISLT_EVT_DET_START);
            memset(debugInfo, 0, sizeof(oasis_lite_debug_t));
        }
        break;

        case OASISLT_EVT_DET_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_DET_START);
            pOasisLite->result.face_id = -1;
            if (para->faceBoxRGB == NULL)
            {
                OASIS_LOGI("[OASIS]DET:No face detected.");
                result->face_count = 0;
            }
            else
            {
                OASIS_LOGD("[OASIS]DET:[Left: %d, Right: %d, Top: %d, Bottom: %d].", para->faceBoxRGB->rect[0],
                           para->faceBoxRGB->rect[1], para->faceBoxRGB->rect[2], para->faceBoxRGB->rect[3]);
                result->face_count = 1;
                result->face_box   = (*(para->faceBoxRGB));
            }
        }
        break;

        case OASISLT_EVT_QUALITY_CHK_START:
        {
            FWK_Profiler_StartEvent(OASISLT_EVT_QUALITY_CHK_START);
        }
        break;

        case OASISLT_EVT_QUALITY_CHK_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_QUALITY_CHK_START);
            oasis_lite_quality_check_result_t qualityCheck  =  kOasisLiteQualityCheck_Ok;
            debugInfo->irBrightness       = para->reserved[10];
            debugInfo->rgbBrightness      = para->reserved[12];
            OASIS_LOGI("[OASIS]irBrightness:%d", debugInfo->irBrightness);
            OASIS_LOGI("[OASIS]rgbBrightness:%d", debugInfo->rgbBrightness);
            if (para->qualityResult == OASIS_QUALITY_RESULT_FACE_OK)
            {
                qualityCheck    = kOasisLiteQualityCheck_Ok;
                debugInfo->isOk = 1;
                OASIS_LOGD("[OASIS]Quality:ok!.");
            }
            else if (para->qualityResult == OASIS_QUALITY_RESULT_FACE_SIDE_FACE)
            {
                qualityCheck            = kOasisLiteQualityCheck_NonFrontalFace;
                debugInfo->isSideFace   = 1;
                OASIS_LOGD("[OASIS]Quality:side face!.");

            }
            else if (para->qualityResult == OASIS_QUALITY_RESULT_FACE_TOO_SMALL)
            {
                OASIS_LOGD("[OASIS]Quality:Small Face!.");
            }
            else if (para->qualityResult == OASIS_QUALITY_RESULT_FACE_BLUR)
            {
                qualityCheck        = kOasisLiteQualityCheck_Blurry;
                debugInfo->isBlurry = 1;
                OASIS_LOGD("[OASIS]Quality:Blurry Face!.");
            }
            else if (para->qualityResult == OASIS_QUALITY_RESULT_IR_FAKE)
            {
                qualityCheck        = kOasisLiteQualityCheck_FakeFace;
                debugInfo->is2dFake = 1;
                OASIS_LOGD("[OASIS]Quality:2D liveness fail!");
            }
            else if (para->qualityResult == OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_DARK ||
                     para->qualityResult == OASIS_QUALITY_RESULT_FAIL_BRIGHTNESS_OVEREXPOSURE)
            {
                OASIS_LOGD("[OASIS]Quality:Face Brightness unfit!.");
            }
            else if (para->qualityResult == OASIS_QUALITY_RESULT_DEPTH_INVALID)
            {
                OASIS_LOGD("[OASIS]Quality:Depth Invalid!.");
            } /* OASIS_QUALITY_RESULT_INVALID for the case there is no face detected */

            if (qualityCheck != pOasisLite->qualityCheck)
            {
                pOasisLite->qualityCheck = qualityCheck;
                if (s_OasisLite.pQualityCheckTimer)
                {
                    sln_timer_stop(&s_OasisLite.pQualityCheckTimer);
                }

                if (pOasisLite->qualityCheck != kOasisLiteQualityCheck_Ok)
                {
                    sln_timer_start("QualityCheckTimer", QUALITY_CHECK_TIMER, 0, _oasis_timer_quality_check, &s_OasisLite, &s_OasisLite.pQualityCheckTimer);
                }
            }
        }
        break;

        case OASISLT_EVT_REC_START:
        {
            FWK_Profiler_StartEvent(OASISLT_EVT_REC_START);
        }
        break;

        case OASISLT_EVT_REC_COMPLETE:
        {
            FWK_Profiler_EndEvent(OASISLT_EVT_REC_START);
            // Recognition complete
            OASISLTRecognizeRes_t recResult = para->recResult;

            if (recResult == OASIS_REC_RESULT_KNOWN_FACE)
            {
                /* Recognized */
                result->rec_result       = kOASISLiteRecognitionResult_Success;
                result->face_recognized  = 1;
                result->face_id          = para->faceID;
                debugInfo->sim           = para->reserved[0];
                debugInfo->faceID        = para->faceID;
                char *faceName          = HAL_Facedb_GetName(para->faceID);

                if (faceName != NULL)
                {
                    OASIS_LOGD("[OASIS]KNOWN_FACE:[%s][%d].", faceName, para->reserved[0]);
                    strcpy(result->name, faceName);
                }
                else
                {
                    sprintf(result->name, "user_%03d", para->faceID);
                    OASIS_LOGE("ERROR:failed to get face name %d.", para->faceID);
                }
            }
            else if (recResult == OASIS_REC_RESULT_UNKNOWN_FACE)
            {
                result->rec_result        = kOASISLiteRecognitionResult_Unknown;
                result->face_recognized   = 1;
                result->face_id           = -1;
                debugInfo->sim            = para->reserved[0];
                debugInfo->faceID         = para->faceID;
                // unknown face
                OASIS_LOGD("[OASIS]UNKNOWN_FACE:Sim:[%d.%d%]:[%d].", (int)(para->reserved[0] / 100),
                           (int)(para->reserved[0] % 100), para->faceID);
            }
            else
            {
                result->rec_result      = kOASISLiteRecognitionResult_Invalid;
                result->face_recognized = 0;
                result->face_id         = -1;
                OASIS_LOGI("[OASIS]INVALID_FACE.");
            }
        }
        break;

        case OASISLT_EVT_REG_START:
        {
#if HEADLESS_ENABLE
            pOasisLite->headless_reg_status = OASIS_LITE_HEADLESS_REG_FRONT_FACE;
#endif
        }
        break;

        case OASISLT_EVT_REG_IN_PROGRESS:
        {
#if HEADLESS_ENABLE
            pOasisLite->headless_reg_status++;
#endif
        }
        break;

        case OASISLT_EVT_REG_COMPLETE:
        {
            uint16_t id              = para->faceID;
            OASISLTRegisterRes_t res = para->regResult;

            if (res == OASIS_REG_RESULT_OK)
            {
                result->reg_result = kOASISLiteRegistrationResult_Success;
                result->face_id    = id;
                if (id >= 0)
                {
                    char *faceName = HAL_Facedb_GetName(id);
                    strcpy(result->name, faceName);
                }
            }
            else if (res == OASIS_REG_RESULT_DUP)
            {
                result->reg_result = kOASISLiteRegistrationResult_Duplicated;
                result->face_id    = id;
                if (id >= 0)
                {
                    char *faceName = HAL_Facedb_GetName(id);
                    strcpy(result->name, faceName);
                }
            }
            else
            {
                result->reg_result = kOASISLiteRegistrationResult_Invalid;
                result->face_id    = -1;
            }
#if HEADLESS_ENABLE
            pOasisLite->headless_reg_status = OASIS_LITE_HEADLESS_REG_COMPLETE;
#endif
        }
        break;

        case OASISLT_EVT_DEREG_START:
        {
            result->face_id      = -1;
            result->dereg_result = kOASISLiteDeregistrationResult_Invalid;
        }
        break;
        case OASISLT_EVT_DEREG_COMPLETE:
        {
            if (para->deregResult == OASIS_DEREG_RESULT_OK)
            {
                OASIS_LOGD("[OASIS]FACE_REMOVED:[%d]", para->faceID);
                result->dereg_result = kOASISLiteDeregistrationResult_Success;
            }
        }
        break;
        default:
        {
        }
        break;
    }
}

static int _oasis_lite_GetFaces(uint16_t *faceIds, void **pFaces, uint32_t *faceNum, void *userData)
{
    LOGI("++_oasis_lite_GetFaces");
    int ret      = 0;
    int dbCount = facedb_get_count();
    // query the face db count
    if (*faceNum == 0)
    {
        *faceNum = dbCount;
        return ret;
    }

    HAL_Facedb_GetAllFaceIds(faceIds, pFaces);

    LOGI("--_oasis_lite_GetFaces [%d]", dbCount);
    return ret;
}

static int _oasis_lite_DeleteFace(uint16_t faceId, void *userData)
{
    LOGI("++_oasis_lite_DeleteFace");
    facedb_status_t status;
    int ret = 0;
    if (faceId != INVALID_ID)
    {
        // TODO: Temporary workaround. Remove this once name is returned in oasisLite library as part of dereg event
        if (s_OasisLite.result.state == kOASISLiteState_DeRegistration)
        {
            strcpy(s_OasisLite.result.name, HAL_Facedb_GetName(faceId));
        }

        status = HAL_Facedb_DelFaceWithID(faceId);
        if (status == kFaceDBStatus_Success)
        {
            ret = 0;
        }
    }
    LOGI("--_oasis_lite_DeleteFace");
    return ret;
}

static int _oasis_lite_AddFace(uint16_t *faceId, void *faceData, void *snapshot, int snapshotLength, void *userData)
{
    LOGI("++_oasis_lite_AddFace");
    facedb_status_t status;
    int ret          = -1;
    int faceItemSize = OASISLT_getFaceItemSize();
    uint16_t newFaceId;
    status = HAL_Facedb_GenId(&newFaceId);

    if ((status == kFaceDBStatus_Success) && (newFaceId != INVALID_ID))
    {
        *faceId = newFaceId;
        status  = HAL_Facedb_AddFace(newFaceId, s_UserNameReference, faceData, faceItemSize);
        if (status == kFaceDBStatus_Success)
        {
            s_UserNameReference = NULL;
            ret                 = 0;
        }
    }
    else if (status == kFaceDBStatus_Full)
    {
        LOGE("OASIS: Database is full.");
    }

    LOGI("--_oasis_lite_AddFace");
    return ret;
}

static int _oasis_lite_UpdateFace(
    uint16_t faceId, void *faceData, void *snapshotData, int dataLength, int offset, void *userData)
{
    LOGI("++_oasis_lite_UpdateFace");

    int ret          = -1;
    int faceItemSize = OASISLT_getFaceItemSize();

    ret = HAL_Facedb_UpdateFace(faceId, HAL_Facedb_GetName(faceId), faceData, faceItemSize);

    LOGI("--_oasis_lite_UpdateFace");
    return ret;
}

/* Used to dynamically adjust face brightness, user can adjust brightness by modifing LED's light intensity or camera
 * exposure level. frame_idx: which frame needs to be adjusted on, OASISLT_INT_FRAME_IDX_RGB or OASISLT_INT_FRAME_IDX_IR
 * ? direction: 1: up, need to increase brightness;  0: down, need to reduce brightness. For GC0308, we can combine
 * camera exposure and led pwm to adjust rgb face brightness. For MT9M114, only use pwm to adjust rgb face brightness.
 */
static void _oasis_lite_AdjustBrightness(uint8_t frameIdx, uint8_t direction, void *userData)
{
    LOGI("++_oasis_lite_AdjustBrightness");

    if (frameIdx == OASISLT_INT_FRAME_IDX_IR)
    {
        event_common_t eventIRLed;
        eventIRLed.brightnessControl.enable    = true;
        eventIRLed.brightnessControl.direction = direction;
        eventIRLed.eventBase.eventId           = kEventID_ControlIRLedBrightness;
        _oasis_lite_dev_led_pwm_control(s_OasisLite.dev, &eventIRLed);
    }
    else
    {
        event_common_t eventWhiteLed;
        eventWhiteLed.brightnessControl.enable    = true;
        eventWhiteLed.brightnessControl.direction = direction;
        eventWhiteLed.eventBase.eventId           = kEventID_ControlWhiteLedBrightness;
        _oasis_lite_dev_led_pwm_control(s_OasisLite.dev, &eventWhiteLed);

        event_common_t eventRGBCam;
        eventRGBCam.brightnessControl.enable    = true;
        eventRGBCam.brightnessControl.direction = direction;
        eventRGBCam.eventBase.eventId           = kEventID_ControlRGBCamExposure;
        _oasis_lite_dev_camera_exposure_control(s_OasisLite.dev, &eventRGBCam);
    }

    LOGI("--_oasis_lite_AdjustBrightness");
}

static int _oasis_lite_Log(const char *formatString)
{
    // todo
    // LOGD("%s", formatString);
    return 0;
}

static void _oasis_timers_stop()
{
    if (s_OasisLite.pRegTimer)
    {
        sln_timer_stop(&s_OasisLite.pRegTimer);
    }
    if (s_OasisLite.pDetFaceTimer)
    {
        sln_timer_stop(&s_OasisLite.pDetFaceTimer);
    }
    if (s_OasisLite.pDeregTimer)
    {
        sln_timer_stop(&s_OasisLite.pDeregTimer);
    }
    if (s_OasisLite.pRecTimer)
    {
        sln_timer_stop(&s_OasisLite.pRecTimer);
    }
    if (s_OasisLite.pResultTimer)
    {
        sln_timer_stop(&s_OasisLite.pResultTimer);
    }
    if (s_OasisLite.pQualityCheckTimer)
    {
        s_OasisLite.qualityCheck = kOasisLiteQualityCheck_Ok;
        sln_timer_stop(&s_OasisLite.pQualityCheckTimer);
    }
}

static void _oasis_timer_quality_check(void *arg)
{
    if (arg == NULL)
        return;
    oasis_lite_param_t *pParam = (oasis_lite_param_t *)arg;

    pParam->timerTimeoutQualityCheck = 1;
}

static void _oasis_timer_rec(void *arg)
{
    if (arg == NULL)
        return;
    oasis_lite_param_t *pParam = (oasis_lite_param_t *)arg;

    if (pParam->pDetFaceTimer)
    {
        sln_timer_stop(&pParam->pDetFaceTimer);
    }

    pParam->timer_timeout = 1;
}

static void _oasis_timer_noface_detected(void *arg)
{
    if (arg == NULL)
        return;
    oasis_lite_param_t *pParam = (oasis_lite_param_t *)arg;

    _oasis_timers_stop();

    pParam->timer_timeout = 1;
}

static void _oasis_timer_registration(void *arg)
{
    if (arg == NULL)
        return;
    oasis_lite_param_t *pParam = (oasis_lite_param_t *)arg;

    if (pParam->pDetFaceTimer)
    {
        sln_timer_stop(&pParam->pDetFaceTimer);
    }

    pParam->timer_timeout = 1;
}

static void _oasis_timer_deregistration(void *arg)
{
    if (arg == NULL)
        return;
    oasis_lite_param_t *pParam = (oasis_lite_param_t *)arg;

    if (pParam->pDetFaceTimer)
    {
        sln_timer_stop(&pParam->pDetFaceTimer);
    }

    pParam->timer_timeout = 1;
}

static void _oasis_start_deregistration(oasis_lite_param_t *pParam)
{
    if (pParam == NULL)
        return;

    _oasis_timers_stop();
    memset(&pParam->result, 0, sizeof(pParam->result));
    pParam->result.state    = kOASISLiteState_DeRegistration;

    _oasis_lite_dev_notify_result(pParam->dev, &pParam->result);

    if (sln_timer_start("DetectFace", DETECT_FACE_TIMER, 0, _oasis_timer_noface_detected, pParam,
            &pParam->pDetFaceTimer))
    {
        LOGE("Failed to start \"DetectFace\" timer.");
    }

    /* start deregistration timer, 30s timeout regards as deregistration fail */
    if (sln_timer_start("Deregistering", DEREGISTERING_TIMER, 0, _oasis_timer_deregistration, pParam,
            &pParam->pDeregTimer))
    {
        LOGE("Failed to start \"Deregistering\" timer.");
    }

    pParam->qualityCheck    = kOasisLiteQualityCheck_Ok;
    pParam->run_flag        = OASIS_DET_REC_DEREG;
}

static void _oasis_start_registration(oasis_lite_param_t *pParam)
{
    if (pParam == NULL)
        return;

    _oasis_timers_stop();
    memset(&pParam->result, 0, sizeof(pParam->result));
    pParam->run_flag        = OASIS_DET_REC_REG;
    pParam->result.state    = kOASISLiteState_Registration;

    _oasis_lite_dev_notify_result(s_OasisLite.dev, &pParam->result);

    if (sln_timer_start("DetectFace", DETECT_FACE_TIMER, 0, _oasis_timer_noface_detected, pParam,
                        &pParam->pDetFaceTimer))
    {
        LOGE("Failed to start \"DetectFace\" timer.");
    }

    /* start registration timer, 30s timeout regards as registration fail */
    if (sln_timer_start("Registering", REGISTERING_TIMER, 0, _oasis_timer_registration, pParam,
            &pParam->pRegTimer))
    {
        LOGE("Failed to start \"Registering\" timer.");
    }

    pParam->qualityCheck    = kOasisLiteQualityCheck_Ok;
}

static void _oasis_start_recognition(oasis_lite_param_t *pParam)
{
    if (pParam == NULL)
        return;

    _oasis_timers_stop();
    pParam->run_flag = OASIS_DET_REC;
    memset(&pParam->result, 0, sizeof(pParam->result));
    _oasis_lite_dev_notify_result(pParam->dev, &pParam->result);

    if (HAL_OutputDev_SmartLockConfig_GetSleepMode() == kLPMManagerStatus_SleepEnable)
    {
        /* start rec timer, timeout will enable low power mode. */
        if (sln_timer_start("RecTimer", RECOGNITION_TIMER, 0, _oasis_timer_rec, &s_OasisLite, &s_OasisLite.pRecTimer))
        {
            LOGE("Failed to start \"RecTimer\" timer.");
        }

        if (sln_timer_start("DetectFace", DETECT_FACE_TIMER, 0, _oasis_timer_noface_detected, &s_OasisLite,
                            &s_OasisLite.pDetFaceTimer))
        {
            LOGE("Failed to start \"DetectFace\" timer.");
        }
    }
}

static void _oasis_timer_result(void *arg)
{
    if (arg == NULL)
        return;
    oasis_lite_param_t *pParam = (oasis_lite_param_t *)arg;

    if ((pParam->result.state == kOASISLiteState_Recognition) &&
        (HAL_OutputDev_SmartLockConfig_GetSleepMode() == kLPMManagerStatus_SleepEnable))
    {
        unsigned int totalUsageCount;
        FWK_LpmManager_RequestStatus(&totalUsageCount);
        if (totalUsageCount == 0)
        {
            /* Enter lpm after a result from a recognition operation */
            FWK_LpmManager_EnableSleepMode(kLPMManagerStatus_SleepEnable);
            pParam->run_flag = OASIS_RUN_FLAG_NUM;
        }
        else
        {
            _oasis_start_recognition(pParam);
        }
    }
    else
    {
        _oasis_start_recognition(pParam);
    }
}

static void _oasis_lite_check_timeout(oasis_lite_result_t *result, uint8_t *timerTimeout)
{
    if (result == NULL || timerTimeout == NULL)
    {
        return;
    }

    switch (result->state)
    {
        case kOASISLiteState_Recognition:
        {
            if ((result->rec_result == kOASISLiteRecognitionResult_Success))
            {
                *timerTimeout = 0;
            }
            else if (*timerTimeout == 1)
            {
                result->state      = kOASISLiteState_Recognition;
                result->rec_result = kOASISLiteRecognitionResult_Timeout;
                *timerTimeout      = 0;
            }
        }
        break;
        case kOASISLiteState_Registration:
        {
            if ((result->reg_result == kOASISLiteRegistrationResult_Success) ||
                (result->reg_result == kOASISLiteRegistrationResult_Duplicated))
            {
                *timerTimeout = 0;
            }
            else if (*timerTimeout == 1)
            {
                memset(result, 0, sizeof(oasis_lite_result_t));
                result->state      = kOASISLiteState_Registration;
                result->reg_result = kOASISLiteRegistrationResult_Timeout;
                *timerTimeout      = 0;
            }
        }
        break;
        case kOASISLiteState_DeRegistration:
        {
            if (result->dereg_result == kOASISLiteDeregistrationResult_Success)
            {
                *timerTimeout = 0;
            }
            else if (*timerTimeout == 1)
            {
                memset(result, 0, sizeof(oasis_lite_result_t));
                result->state        = kOASISLiteState_DeRegistration;
                result->dereg_result = kOASISLiteDeregistrationResult_Timeout;
                *timerTimeout        = 0;
            }
        }
        break;
        default:
        {
        }
    }
}

static void _oasis_lite_reset_brightness(void)
{
    event_common_t eventIRLed;
    eventIRLed.brightnessControl.enable = false;
    eventIRLed.eventBase.eventId        = kEventID_ControlIRLedBrightness;
    _oasis_lite_dev_led_pwm_control(s_OasisLite.dev, &eventIRLed);

    event_common_t eventWhiteLed;
    eventWhiteLed.brightnessControl.enable = false;
    eventWhiteLed.eventBase.eventId        = kEventID_ControlWhiteLedBrightness;
    _oasis_lite_dev_led_pwm_control(s_OasisLite.dev, &eventWhiteLed);

    event_common_t eventRGBCam;
    eventRGBCam.brightnessControl.enable = false;
    eventRGBCam.eventBase.eventId        = kEventID_ControlRGBCamExposure;
    _oasis_lite_dev_camera_exposure_control(s_OasisLite.dev, &eventRGBCam);

    LOGD("Set to default pwm and/or exposure when processing is finished or timeout.");
}

static void _process_inference_result(oasis_lite_param_t *pParam)
{
    uint8_t lockOasis = false;
    if (pParam == NULL)
        return;

    oasis_lite_result_t *pResult = &pParam->result;
    vision_algo_dev_t *dev       = pParam->dev;
    if ((pResult == NULL) || (dev == NULL))
        return;

    switch (pResult->state)
    {
        case kOASISLiteState_Recognition:
        {
            if (pParam->mode == kOASISLiteMode_SmartLock)
            {
                if ((pParam->pDetFaceTimer) && (pResult->face_count == 1))
                {
                    sln_timer_reset(&pParam->pDetFaceTimer);
                }

                if ((pResult->rec_result == kOASISLiteRecognitionResult_Success) ||
                    (pResult->rec_result == kOASISLiteRecognitionResult_Timeout))
                {
                    lockOasis = true;
                }
                else
                {
                    lockOasis = false;
                }
            }
        }
        break;
        case kOASISLiteState_Registration:
        {
            if ((pParam->pDetFaceTimer) && (pResult->face_count == 1))
            {
                sln_timer_reset(&pParam->pDetFaceTimer);
            }

            if (pResult->reg_result > kOASISLiteRegistrationResult_Invalid)
            {
                lockOasis = true;
            }

            if (pParam->pRegTimer)
            {
                pResult->process = (float)sln_timer_getRemainingTime(&pParam->pRegTimer) / REGISTERING_TIMER;
            }
            else
            {
                pResult->process = 0;
            }
        }
        break;
        case kOASISLiteState_DeRegistration:
        {
            if ((pParam->pDetFaceTimer) && (pResult->face_count == 1))
            {
                sln_timer_reset(&pParam->pDetFaceTimer);
            }

            if (pResult->dereg_result > kOASISLiteDeregistrationResult_Invalid)
            {
                lockOasis = true;
            }

            if (pParam->pDeregTimer)
            {
                pResult->process = (float)sln_timer_getRemainingTime(&pParam->pDeregTimer) / DEREGISTERING_TIMER;
            }
            else
            {
                pResult->process = 0;
            }
        }
        break;
        default:
        {
            lockOasis = false;
        }
        break;
    }

    // notify the result
    _oasis_lite_dev_notify_result(dev, pResult);

    if (lockOasis)
    {
        _oasis_lite_reset_brightness();

        pParam->run_flag = OASIS_RUN_FLAG_NUM;

        _oasis_timers_stop();

        if (sln_timer_start("RegisterResult", RESULT_TIMER, 0, _oasis_timer_result, (void *)pParam,
                            &pParam->pResultTimer))
        {
            LOGE("Failed to start \"RegisterResult\" timer.");
        }
        return;
    }
}

static hal_valgo_status_t HAL_VisionAlgoDev_OasisLite_Init(vision_algo_dev_t *dev,
                                                           valgo_dev_callback_t callback,
                                                           void *param)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_OasisLite_Init");
    OASISLTResult_t oasisRet = OASISLT_OK;

    // init the database
    HAL_Facedb_Init();

    s_OasisLite.dev = dev;

    // init the device
    memset(&dev->cap, 0, sizeof(dev->cap));
    dev->cap.callback = callback;

    dev->data.frames[kVAlgoFrameID_RGB].height       = OASIS_RGB_FRAME_HEIGHT;
    dev->data.frames[kVAlgoFrameID_RGB].width        = OASIS_RGB_FRAME_WIDTH;
    dev->data.frames[kVAlgoFrameID_RGB].pitch        = OASIS_RGB_FRAME_WIDTH * OASIS_RGB_FRAME_BYTE_PER_PIXEL;
    dev->data.frames[kVAlgoFrameID_RGB].is_supported = 1;
    dev->data.frames[kVAlgoFrameID_RGB].rotate       = kCWRotateDegree_0;
    dev->data.frames[kVAlgoFrameID_RGB].flip         = kFlipMode_None;

    dev->data.frames[kVAlgoFrameID_RGB].format    = kPixelFormat_BGR;
    dev->data.frames[kVAlgoFrameID_RGB].srcFormat = kPixelFormat_UYVY1P422_RGB;
    int oasis_lite_rgb_frame_aligned_size =
        SDK_SIZEALIGN(OASIS_RGB_FRAME_HEIGHT * OASIS_RGB_FRAME_WIDTH * OASIS_RGB_FRAME_BYTE_PER_PIXEL,
                      FSL_FEATURE_L1DCACHE_LINESIZE_BYTE);
    dev->data.frames[kVAlgoFrameID_RGB].data = pvPortMalloc(oasis_lite_rgb_frame_aligned_size);

    if (dev->data.frames[kVAlgoFrameID_RGB].data == NULL)
    {
        LOGE("Unable to allocate memory for kVAlgoFrameID_RGB.");
        ret = kStatus_HAL_ValgoMallocError;
        return ret;
    }

    // init the RGB frame
    s_OasisLite.frames[OASISLT_INT_FRAME_IDX_RGB].height = OASIS_RGB_FRAME_HEIGHT;
    s_OasisLite.frames[OASISLT_INT_FRAME_IDX_RGB].width  = OASIS_RGB_FRAME_WIDTH;
    s_OasisLite.frames[OASISLT_INT_FRAME_IDX_RGB].fmt    = OASIS_IMG_FORMAT_BGR888;
    s_OasisLite.frames[OASISLT_INT_FRAME_IDX_RGB].data   = dev->data.frames[kVAlgoFrameID_RGB].data;
    s_OasisLite.pframes[OASISLT_INT_FRAME_IDX_RGB]       = &s_OasisLite.frames[OASISLT_INT_FRAME_IDX_RGB];

    dev->data.frames[kVAlgoFrameID_IR].height       = OASIS_IR_FRAME_HEIGHT;
    dev->data.frames[kVAlgoFrameID_IR].width        = OASIS_IR_FRAME_WIDTH;
    dev->data.frames[kVAlgoFrameID_IR].pitch        = OASIS_IR_FRAME_WIDTH * OASIS_IR_FRAME_BYTE_PER_PIXEL;
    dev->data.frames[kVAlgoFrameID_IR].is_supported = 1;
    dev->data.frames[kVAlgoFrameID_IR].rotate       = kCWRotateDegree_0;
    dev->data.frames[kVAlgoFrameID_IR].flip         = kFlipMode_None;

    dev->data.frames[kVAlgoFrameID_IR].format    = kPixelFormat_BGR;
    dev->data.frames[kVAlgoFrameID_IR].srcFormat = kPixelFormat_UYVY1P422_Gray;
    int oasis_lite_ir_frame_aligned_size =
        SDK_SIZEALIGN(OASIS_IR_FRAME_HEIGHT * OASIS_IR_FRAME_WIDTH * OASIS_IR_FRAME_BYTE_PER_PIXEL,
                      FSL_FEATURE_L1DCACHE_LINESIZE_BYTE);
    dev->data.frames[kVAlgoFrameID_IR].data = pvPortMalloc(oasis_lite_ir_frame_aligned_size);

    if (dev->data.frames[kVAlgoFrameID_IR].data == NULL)
    {
        LOGE("Unable to allocate memory for kVAlgoFrameID_IR.");
        /* here need release the RGB buffer before return. */
        vPortFree(dev->data.frames[kVAlgoFrameID_RGB].data);
        ret = kStatus_HAL_ValgoMallocError;
        return ret;
    }

    // init the IR frame
    s_OasisLite.frames[OASISLT_INT_FRAME_IDX_IR].height = OASIS_IR_FRAME_HEIGHT;
    s_OasisLite.frames[OASISLT_INT_FRAME_IDX_IR].width  = OASIS_IR_FRAME_WIDTH;
    s_OasisLite.frames[OASISLT_INT_FRAME_IDX_IR].fmt    = OASIS_IMG_FORMAT_BGR888;
    s_OasisLite.frames[OASISLT_INT_FRAME_IDX_IR].data   = dev->data.frames[kVAlgoFrameID_IR].data;
    s_OasisLite.pframes[OASISLT_INT_FRAME_IDX_IR]       = &s_OasisLite.frames[OASISLT_INT_FRAME_IDX_IR];

    // init the oasis lite config
#ifdef SMART_LOCK_2D
    s_OasisLite.config.imgType = OASIS_IMG_TYPE_IR_RGB_DUAL;
#else
    s_OasisLite.config.imgType     = OASIS_IMG_TYPE_RGB_IR_DUAL;
#endif
    s_OasisLite.config.minFace              = OASIS_DETECT_MIN_FACE;
    s_OasisLite.config.cbs.EvtCb            = _oasis_lite_EvtCb;
    s_OasisLite.config.cbs.GetFaces         = _oasis_lite_GetFaces;
    s_OasisLite.config.cbs.AddFace          = _oasis_lite_AddFace;
    s_OasisLite.config.cbs.UpdateFace       = _oasis_lite_UpdateFace;
    s_OasisLite.config.cbs.AdjustBrightness = _oasis_lite_AdjustBrightness;
    s_OasisLite.config.cbs.DeleteFace       = _oasis_lite_DeleteFace;
    s_OasisLite.config.cbs.reserved         = _oasis_lite_Log;
#if HEADLESS_ENABLE
    s_OasisLite.config.enableFlags = OASIS_ENABLE_MULTI_VIEW | OASIS_ENABLE_LIVENESS;
#else
    s_OasisLite.config.enableFlags = OASIS_ENABLE_LIVENESS;
#endif
    s_OasisLite.config.falseAcceptRate = OASIS_FAR_1_1000000;
    s_OasisLite.config.height          = OASIS_RGB_FRAME_HEIGHT;
    s_OasisLite.config.width           = OASIS_RGB_FRAME_WIDTH;
    s_OasisLite.config.size            = 0;
    s_OasisLite.config.memPool         = NULL;
    s_OasisLite.config.fastMemSize     = DTC_OPTIMIZE_BUFFER_SIZE;
    s_OasisLite.config.fastMemBuf      = (char*) s_DTCOPBuf;

    s_OasisLite.run_flag = OASIS_DET_REC;

    oasisRet = OASISLT_init(&s_OasisLite.config);

    if (oasisRet == OASIS_INIT_INVALID_MEMORYPOOL)
    {
#if OASIS_STATIC_MEM_BUFFER
        if (OASIS_STATIC_MEM_POOL >= s_OasisLite.config.size)
        {
            s_OasisLite.config.memPool = (char *)g_OasisMemPool;
        }
        else
        {
            s_OasisLite.config.memPool = NULL;
        }
#else

        s_OasisLite.config.memPool = (char *)pvPortMalloc(s_OasisLite.config.size);

#endif
        LOGD("OASIS LITE MEM POOL %d.", s_OasisLite.config.size);

        if (s_OasisLite.config.memPool == NULL)
        {
            LOGE("Unable to allocate memory for OASIS mem pool of size %d.", s_OasisLite.config.size);

            while (1)
                ;
        }

        oasisRet = OASISLT_init(&s_OasisLite.config);
    }

    if (oasisRet != OASISLT_OK)
    {
        LOGE("OASISLT_init ret %d", ret);
        ret = kStatus_HAL_ValgoInitError;
        return ret;
    }

    _oasis_start_recognition(&s_OasisLite);

    LOGD("[OASIS]:Init ok");
    LOGI("--HAL_VisionAlgoDev_OasisLite_Init");
    return ret;
}

// deinitialize the dev
static hal_valgo_status_t HAL_VisionAlgoDev_OasisLite_Deinit(vision_algo_dev_t *dev)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_OasisLite_Deinit");
    facedb_status_t status = HAL_Facedb_SaveFace();

    if (status == kFaceDBStatus_Success)
    {
        LOGD("Successfully saved users to flash.");
        /* Try to do a cleanup 400 ms to erase sectors ~ 10 sectors erased */
        FWK_Flash_Cleanup(400);
    }
    else
    {
        LOGE("Failed to save users before entering low power mode.");
        ret = kStatus_HAL_ValgoError;
    }
    LOGI("--HAL_VisionAlgoDev_OasisLite_Deinit");
    return ret;
}

// start the dev
static hal_valgo_status_t HAL_VisionAlgoDev_OasisLite_Run(const vision_algo_dev_t *dev, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_OasisLite_Run.");

#if HEADLESS_ENABLE
    if (s_OasisLite.headless_reg_status == OASIS_LITE_HEADLESS_REG_COMPLETE)
    {
        s_OasisLite.headless_reg_status = OASIS_LITE_HEADLESS_REG_START;
    }
#endif

    if (s_OasisLite.run_flag != OASIS_RUN_FLAG_NUM)
    {
        // clear the result
        memset(&s_OasisLite.result, 0, sizeof(s_OasisLite.result));

        if (s_OasisLite.run_flag == OASIS_DET_REC)
        {
            s_OasisLite.result.state = kOASISLiteState_Recognition;
        }
        else if (s_OasisLite.run_flag == OASIS_DET_REC_REG)
        {
            s_OasisLite.result.state = kOASISLiteState_Registration;
        }
        else if (s_OasisLite.run_flag == OASIS_DET_REC_DEREG)
        {
            s_OasisLite.result.state = kOASISLiteState_DeRegistration;
        }

        FWK_Profiler_ClearEvents();

#ifdef PROFILER_STATIC_FRAME
        s_OasisLite.frames[OASISLT_INT_FRAME_IDX_IR].data = clip_frame_hwc_rgb;
        s_OasisLite.frames[OASISLT_INT_FRAME_IDX_3D].data = s_RAW16_540_640_DEPTH_FRAME;
#endif

        int oasis_ret =
            OASISLT_run_extend(s_OasisLite.pframes, s_OasisLite.run_flag, s_OasisLite.config.minFace, &s_OasisLite);

        if (oasis_ret)
        {
            LOGE("OASISLT_run_extend failed with error: %d", oasis_ret);
        }

        if (s_OasisLite.timerTimeoutQualityCheck || (s_OasisLite.pQualityCheckTimer == NULL))
        {
            s_OasisLite.timerTimeoutQualityCheck = 0;
            s_OasisLite.result.qualityCheck      = s_OasisLite.qualityCheck;
            s_OasisLite.qualityCheck             = kOasisLiteQualityCheck_Ok;
        }

#if HEADLESS_ENABLE
        s_OasisLite.result.headless_reg_status = s_OasisLite.headless_reg_status;
#endif
        FWK_Profiler_Log();

        _oasis_lite_check_timeout(&s_OasisLite.result, &s_OasisLite.timer_timeout);

        /* Take decision regarding the inference results */
        _process_inference_result(&s_OasisLite);
    }

    LOGI("--HAL_VisionAlgoDev_OasisLite_Run");
    return ret;
}

static hal_valgo_status_t HAL_VisionAlgoDev_OasisLite_InputNotify(const vision_algo_dev_t *receiver, void *data)
{
    hal_valgo_status_t ret = kStatus_HAL_ValgoSuccess;
    LOGI("++HAL_VisionAlgoDev_OasisLite_InputNotify");
    event_base_t eventBase = *(event_base_t *)data;

    switch (eventBase.eventId)
    {
        case kEventFaceRecID_AddUser:
        {
            event_face_rec_t event = *(event_face_rec_t *)data;
            if (event.addFace.hasName)
            {
                memcpy(s_UserName, event.addFace.name, FACE_NAME_MAX_LEN);
                s_UserNameReference = s_UserName;
            }
            else
            {
                s_UserNameReference = NULL;
                /* If registration currently in progress stop it */
                if (s_OasisLite.run_flag == OASIS_DET_REC_REG)
                {
                    _oasis_start_recognition(&s_OasisLite);
                    break;
                }
            }
            _oasis_start_registration(&s_OasisLite);
        }
        break;

        case kEventFaceRecID_DelUserAll:
        {
            event_face_rec_t event              = *(event_face_rec_t *)data;
            event_status_t event_respond_status = kEventStatus_Ok;
            if (HAL_Facedb_DelFaceWithID(INVALID_FACE_ID) != kFaceDBStatus_Success)
            {
                event_respond_status = kEventStatus_Error;
            }
            _oasis_lite_dev_response(eventBase, &event.delFace, event_respond_status, true);
        }
        break;

        case kEventFaceRecID_DelUser:
        {
            event_face_rec_t event              = *(event_face_rec_t *)data;
            event_status_t event_respond_status = kEventStatus_Ok;
            if (event.delFace.hasID)
            {
                if (HAL_Facedb_DelFaceWithID(event.delFace.id) != kFaceDBStatus_Success)
                {
                    event_respond_status = kEventStatus_Error;
                }
                _oasis_lite_dev_response(eventBase, &event.delFace, event_respond_status, true);
            }
            else if (event.delFace.hasName)
            {
                if (HAL_Facedb_DelFaceWithName(event.delFace.name) != kFaceDBStatus_Success)
                {
                    event_respond_status = kEventStatus_Error;
                }
                _oasis_lite_dev_response(eventBase, &event.delFace, event_respond_status, true);
            }
            /* No name and no id. Delete next recognized face */
            else
            {
                if (s_OasisLite.run_flag == OASIS_DET_REC_DEREG)
                {
                    /* If deregistration currently in progress stop it */
                    _oasis_start_recognition(&s_OasisLite);
                }
                else
                {
                    _oasis_start_deregistration(&s_OasisLite);
                }
            }
        }
        break;

        case kEventFaceRecID_AddUserRemote:
        {
            event_face_rec_t event       = *(event_face_rec_t *)data;
            remote_reg_event_t remoteEvt = event.remoteReg;
            remote_reg_result_t res      = {OASIS_REG_RESULT_INVALID, NULL};

            if (remoteEvt.dataLen == OASISLT_getFaceItemSize() + sizeof(remoteEvt.regData->name))
            {
                if (remoteEvt.isReRegister)
                {
                    /* find id by name */
                    int count         = facedb_get_count();
                    uint16_t *faceIds = (uint16_t *)pvPortMalloc(count * sizeof(uint16_t));

                    HAL_Facedb_GetIds(faceIds);
                    for (int i = 0; i < count; i++)
                    {
                        if (!strcmp(remoteEvt.regData->name, HAL_Facedb_GetName(faceIds[i])))
                        {
                            res.result = OASISLT_registration_by_feature(remoteEvt.regData->facedata, NULL, 0,
                                                                         &faceIds[i], NULL);
                            break;
                        }
                    }

                    vPortFree(faceIds);
                }
                else
                {
                    uint16_t id         = INVALID_FACE_ID;
                    s_UserNameReference = remoteEvt.regData->name;
                    res.result = OASISLT_registration_by_feature(remoteEvt.regData->facedata, NULL, 0, &id, NULL);

                    if (res.result == OASIS_REG_RESULT_DUP)
                    {
                        res.name = HAL_Facedb_GetName(id);
                        LOGD("Duplicate face registration:%d", id);
                    }
                }

                _oasis_lite_dev_response(eventBase, &res, kEventStatus_Ok, true);
            }
            else
            {
                _oasis_lite_dev_response(eventBase, &res, kEventStatus_Error, true);
            }
        }
        break;
        case kEventFaceRecID_SaveUserList:
        {
            event_face_rec_t event = *(event_face_rec_t *)data;
            uint8_t isSaved        = HAL_Facedb_GetSaveStatus(INVALID_FACE_ID);

            if (isSaved == true)
            {
                _oasis_lite_dev_response(eventBase, &event, kEventStatus_Ok, true);
                return ret;
            }

            facedb_status_t ret = HAL_Facedb_SaveFace();
            if (ret == kFaceDBStatus_Success)
            {
                _oasis_lite_dev_response(eventBase, &event, kEventStatus_Ok, true);
            }
            else
            {
                _oasis_lite_dev_response(eventBase, &event, kEventStatus_Error, true);
            }
        }
        break;
        case kEventFaceRecID_GetUserList:
        {
            user_info_event_t userEvent;
            int count         = facedb_get_count();
            uint16_t *faceIds = (uint16_t *)pvPortMalloc(count * sizeof(uint16_t));
            LOGI("[VALGO] Get User List Event");
            if (faceIds != NULL)
            {
                face_user_info_t *userInfos = (face_user_info_t *)pvPortMalloc(count * sizeof(face_user_info_t));
                if (userInfos != NULL)
                {
                    userEvent.userInfo = userInfos;
                    userEvent.count    = count;

                    HAL_Facedb_GetIds(faceIds);

                    for (int i = 0; i < count; i++)
                    {
                        userInfos[i].id      = faceIds[i];
                        userInfos[i].isSaved = HAL_Facedb_GetSaveStatus(faceIds[i]);
                        memcpy(userInfos[i].name, HAL_Facedb_GetName(faceIds[i]), sizeof(userInfos[i].name));
                    }

                    _oasis_lite_dev_response(eventBase, &userEvent, kEventStatus_Ok, true);
                    vPortFree(userInfos);
                }
                vPortFree(faceIds);
            }
            else
            {
                userEvent.userInfo = NULL;
                userEvent.count    = 0;

                _oasis_lite_dev_response(eventBase, &userEvent, kEventStatus_Ok, true);
            }
        }
        break;

        case kEventFaceRecID_GetUserCount:
        {
            int count = facedb_get_count();
            LOGI("[VALGO] User count is %d.", count);
            _oasis_lite_dev_response(eventBase, &count, kEventStatus_Ok, true);
        }
        break;

        case kEventFaceRecID_UpdateUserInfo:
        {
            event_face_rec_t event            = *(event_face_rec_t *)data;
            update_user_event_t userInfo      = event.updateFace;
            event_status_t eventRespondStatus = kEventStatus_Ok;
            if (HAL_Facedb_UpdateName(userInfo.id, userInfo.name) != kFaceDBStatus_Success)
            {
                eventRespondStatus = kEventStatus_Error;
                LOGE("[VALGO] Update user info failed.");
            }
            LOGI("[VALGO] Updating user info for id %d.", userInfo.id);
            _oasis_lite_dev_response(eventBase, &event.updateFace, eventRespondStatus, true);
        }
        break;

        case kEventID_GetLPMTrigger:
        {
            lpm_event_t lpmEvent;
            lpmEvent.status = HAL_OutputDev_SmartLockConfig_GetSleepMode();
            LOGI("[VALGO] get lpm trigger %d.", lpmEvent.status);
            _oasis_lite_dev_response(eventBase, &lpmEvent, kEventStatus_Ok, true);
        }
        break;
        case kEventID_SetLPMTrigger:
        {
            event_common_t event = *(event_common_t *)data;
            if (event.lpm.status == kLPMManagerStatus_SleepEnable)
            {
                if (s_OasisLite.run_flag == OASIS_DET_REC)
                {
                    /* start rec timer, timeout will enable low power mode. */
                    if (sln_timer_start("RecTimer", RECOGNITION_TIMER, 0, _oasis_timer_rec, &s_OasisLite,
                                        &s_OasisLite.pRecTimer))
                    {
                        LOGE("Failed to start \"Enter Sleep\" timer.");
                    }

                    if (sln_timer_start("DetectFace", DETECT_FACE_TIMER, 0, _oasis_timer_noface_detected, &s_OasisLite,
                                        &s_OasisLite.pDetFaceTimer))
                    {
                        LOGE("Failed to start \"DetectFace\" timer.");
                    }
                }

                HAL_OutputDev_SmartLockConfig_SetSleepMode(kLPMManagerStatus_SleepEnable);
                _oasis_lite_dev_response(eventBase, &event.lpm, kEventStatus_Ok, true);
            }
            else if (event.lpm.status == kLPMManagerStatus_SleepDisable)
            {
                if (s_OasisLite.pRecTimer)
                {
                    sln_timer_stop(&s_OasisLite.pRecTimer);
                }

                if (s_OasisLite.pDetFaceTimer)
                {
                    sln_timer_stop(&s_OasisLite.pDetFaceTimer);
                }

                FWK_LpmManager_EnableSleepMode(kLPMManagerStatus_SleepDisable);

                HAL_OutputDev_SmartLockConfig_SetSleepMode(kLPMManagerStatus_SleepDisable);
                _oasis_lite_dev_response(eventBase, &event.lpm, kEventStatus_Ok, true);
            }
            else
            {
                LOGD("Unsupported LPM mode %d.", event.lpm.status);
                _oasis_lite_dev_response(eventBase, &event.lpm, kEventStatus_WrongParam, true);
            }
        }
        break;

        default:
            break;
    }

    LOGI("--HAL_VisionAlgoDev_OasisLite_InputNotify");
    return ret;
}

const static vision_algo_dev_operator_t s_VisionAlgoDev_OasisLiteOps = {
    .init        = HAL_VisionAlgoDev_OasisLite_Init,
    .deinit      = HAL_VisionAlgoDev_OasisLite_Deinit,
    .run         = HAL_VisionAlgoDev_OasisLite_Run,
    .inputNotify = HAL_VisionAlgoDev_OasisLite_InputNotify,
};

static vision_algo_dev_t s_VisionAlgoDev_OasisLite = {
    .id   = 0,
    .name = "OASIS_2D",
    .ops  = (vision_algo_dev_operator_t *)&s_VisionAlgoDev_OasisLiteOps,
    .cap  = {.param = NULL},
};

// for vision_algo_oasis_lite2D device, please link oasis/liboasis_lite2D_DEFAULT_117f_ae.a
int HAL_VisionAlgo_OasisLite2D_Register(int mode) // mode=0 smartlock; mode=1 ffi
{
    int error = 0;
    LOGD("HAL_VisionAlgo_OasisLite2D_Register");
    error = FWK_VisionAlgoManager_DeviceRegister(&s_VisionAlgoDev_OasisLite);
    memset(&s_OasisLite, 0, sizeof(s_OasisLite));
    s_OasisLite.mode = mode;
    return error;
}

#endif /* defined(SMART_LOCK_2D) || defined(SMART_ACCESS_2D) */
