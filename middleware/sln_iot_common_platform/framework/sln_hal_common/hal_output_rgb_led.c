/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief output led device implementation.
 */

#include "board_define.h"

#include "FreeRTOS.h"
#include "board.h"

#include "fwk_log.h"
#include "fwk_output_manager.h"
#include "hal_event_descriptor_common.h"
#include "hal_output_dev.h"
#include "hal_vision_algo_oasis_lite.h"

#if defined(__cplusplus)
extern "C" {
#endif
static hal_output_status_t HAL_OutputDev_RgbLed_InferComplete(const output_dev_t *dev,
                                                              output_algo_source_t source,
                                                              void *inferResult);

static hal_output_status_t HAL_OutputDev_RgbLed_Init(output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_RgbLed_Start(const output_dev_t *dev);
#if defined(__cplusplus)
}
#endif

#define LED_RED_PORT   BOARD_LED_RED_PORT
#define LED_RED_PIN    BOARD_LED_RED_PIN
#define LED_GREEN_PORT BOARD_LED_GREEN_PORT
#define LED_GREEN_PIN  BOARD_LED_GREEN_PIN
#define LED_BLUE_PORT  BOARD_LED_BLUE_PORT
#define LED_BLUE_PIN   BOARD_LED_BLUE_PIN

typedef enum _rgb_led_color
{
    kRGBLedColor_Red,    /*!< LED Red Color */
    kRGBLedColor_Orange, /*!< LED Orange Color */
    kRGBLedColor_Yellow, /*!< LED Yellow Color */
    kRGBLedColor_Green,  /*!< LED Green Color */
    kRGBLedColor_Blue,   /*!< LED Blue Color */
    kRGBLedColor_Purple, /*!< LED Purple Color */
    kRGBLedColor_Cyan,   /*!< LED Cyan Color */
    kRGBLedColor_White,  /*!< LED White Color */
    kRGBLedColor_Off,    /*!< LED Off */
} rgbLedColor_t;

static void RGB_LED_SET_COLOR(rgbLedColor_t color)
{
    switch (color)
    {
        case kRGBLedColor_Red:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 1);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 0);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 0);
            break;
        case kRGBLedColor_Orange:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 1);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 1);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 0);
            break;
        case kRGBLedColor_Yellow:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 1);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 1);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 0);
            break;
        case kRGBLedColor_Green:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 0);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 1);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 0);
            break;
        case kRGBLedColor_Blue:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 0);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 0);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 1);
            break;
        case kRGBLedColor_Purple:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 1);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 0);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 1);
            break;
        case kRGBLedColor_Cyan:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 0);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 1);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 1);
            break;
        case kRGBLedColor_White:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 1);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 1);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 1);
            break;
        case kRGBLedColor_Off:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 0);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 0);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 0);
            break;
        default:
            GPIO_PinWrite(LED_RED_PORT, LED_RED_PIN, 0);
            GPIO_PinWrite(LED_GREEN_PORT, LED_GREEN_PIN, 0);
            GPIO_PinWrite(LED_BLUE_PORT, LED_BLUE_PIN, 0);
            break;
    }
}

const static output_dev_event_handler_t s_OutputDev_RgbLedHandler = {
    .inferenceComplete = HAL_OutputDev_RgbLed_InferComplete,
    .inputNotify       = NULL,
};

static hal_output_status_t HAL_OutputDev_RgbLed_InferComplete(const output_dev_t *dev,
                                                              output_algo_source_t source,
                                                              void *inferResult)
{
    oasis_lite_result_t *result = (oasis_lite_result_t *)inferResult;
    hal_output_status_t error   = kStatus_HAL_OutputSuccess;
    if (source == kOutputAlgoSource_Vision)
    {
        if ((result->face_recognized) && (result->face_id >= 0))
        {
            RGB_LED_SET_COLOR(kRGBLedColor_Green);
        }
        else if (result->face_count)
        {
            RGB_LED_SET_COLOR(kRGBLedColor_Red);
        }
        else
        {
            RGB_LED_SET_COLOR(kRGBLedColor_Off);
        }
    }
    return error;
}

const static output_dev_operator_t s_OutputDev_RgbLedOps = {
    .init   = HAL_OutputDev_RgbLed_Init,
    .deinit = NULL,
    .start  = HAL_OutputDev_RgbLed_Start,
    .stop   = NULL,
};

static output_dev_t s_OutputDev_RgbLed = {
    .name         = "rgb_led",
    .attr.type    = kOutputDevType_Other,
    .attr.reserve = NULL,
    .ops          = &s_OutputDev_RgbLedOps,
};

static hal_output_status_t HAL_OutputDev_RgbLed_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    /* TODO: the "const" of both args is discarded by being passed here */
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_RgbLedHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }
    return error;
}

hal_output_status_t HAL_OutputDev_RgbLed_Init(output_dev_t *dev)
{
    BOARD_InitRGBLedResource();

    gpio_pin_config_t config = {
        .direction     = kGPIO_DigitalOutput, /*!< Specifies the pin direction. */
        .outputLogic   = 0,                   /*!< Set a default output logic, which has no use in input */
        .interruptMode = kGPIO_NoIntmode,     /*!< Specifies the pin interrupt mode, a value of
                                                 @ref gpio_interrupt_mode_t. */
    };
    int error = kStatus_HAL_OutputSuccess;

    LOGD("HAL_Output_rgb_led_Init");
    GPIO_PinInit(LED_RED_PORT, LED_RED_PIN, &config);
    GPIO_PinInit(LED_GREEN_PORT, LED_GREEN_PIN, &config);
    GPIO_PinInit(LED_BLUE_PORT, LED_BLUE_PIN, &config);
    return error;
}

int HAL_OutputDev_RgbLed_Register()
{
    int error = 0;
    LOGD("output_dev_rgb_led_register");
    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_RgbLed);
    return error;
}
