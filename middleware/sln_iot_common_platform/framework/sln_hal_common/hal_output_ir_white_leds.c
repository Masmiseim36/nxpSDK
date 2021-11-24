/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief LED HAL output device implementation. Controls the FlexPWM settings for both the IR and white LEDs.
 */

#include "board_define.h"

#include "FreeRTOS.h"
#include "board.h"

#include "fwk_log.h"
#include "fwk_output_manager.h"
#include "hal_event_descriptor_common.h"
#include "hal_output_dev.h"
#include "hal_smart_lock_config.h"
#include "hal_vision_algo_oasis_lite.h"

#include "fsl_pwm.h"
#include "fsl_xbara.h"

#define IR_PWM_MIN         20
#define IR_PWM_MAX         100
#define IR_PWM_INTERVAL    10
#define WHITE_PWM_MIN      0
#define WHITE_PWM_MAX      30
#define WHITE_PWM_INTERVAL 5

#if defined(__cplusplus)
extern "C" {
#endif
static hal_output_status_t HAL_OutputDev_IrWhiteLeds_InputNotify(const output_dev_t *receiver, void *data);

static hal_output_status_t HAL_OutputDev_IrWhiteLeds_Init(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_IrWhiteLeds_Start(const output_dev_t *dev);

static status_t _getWhitePwm(char *valueToString);
static status_t _getIrPwm(char *valueToString);

#if defined(__cplusplus)
}
#endif

#define PWM_SRC_CLK_FREQ             CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#define MAX_WHITE_BRIGHTNESS_PERCENT 100
#define MAX_IR_BRIGHTNESS_PERCENT    100
typedef enum _led_type
{
    kLEDType_Ir    = 0, /*!< IR LED */
    kLEDType_White = 1, /*!< White LED */
} led_type_t;

typedef struct _led_pwm
{
    PWM_Type *baseAddr;
    pwm_submodule_t submodule;
    pwm_channels_t channel;
    pwm_module_control_t controlModule;
    pwm_fault_input_t faultInput;
    pwm_fault_channels_t faultChannel;
    pwm_fault_disable_t faultDisable;
} led_pwm_t;

static led_pwm_t s_LedPwms[2] = {[kLEDType_White] = {.baseAddr      = PWM1,
                                                     .submodule     = kPWM_Module_1,
                                                     .channel       = kPWM_PwmB,
                                                     .controlModule = kPWM_Control_Module_1,
                                                     .faultInput    = kPWM_Fault_1,
                                                     .faultChannel  = kPWM_faultchannel_1,
                                                     .faultDisable  = kPWM_FaultDisable_1},

                                 [kLEDType_Ir] = {.baseAddr      = PWM1,
                                                  .submodule     = kPWM_Module_2,
                                                  .channel       = kPWM_PwmA,
                                                  .controlModule = kPWM_Control_Module_2,
                                                  .faultInput    = kPWM_Fault_2,
                                                  .faultChannel  = kPWM_faultchannel_0,
                                                  .faultDisable  = kPWM_FaultDisable_2}};

static inline void _HAL_OutputDev_IrWhiteLeds_Response(event_base_t eventBase,
                                                       void *response,
                                                       event_status_t status,
                                                       unsigned char isFinished)
{
    if (eventBase.respond != NULL)
    {
        eventBase.respond(eventBase.eventId, response, status, isFinished);
    }
}

/**
 * @brief Configure the brightness of the specified LED.
 *
 * @param led - LED whose brightness should be set (IR|White)
 * @param percent - percent brightness to set specified LED to (0-100)
 * @return int - success (0), fail (1)
 */
static uint8_t set_led_brightness(led_type_t led, uint8_t percent)
{
    if ((led != kLEDType_Ir) && (led != kLEDType_White))
    {
        return 1;
    }
    else if ((percent < 0) || (100 < percent))
    {
        return 1;
    }

    if (led == kLEDType_White && percent > MAX_WHITE_BRIGHTNESS_PERCENT)
    {
        percent = MAX_WHITE_BRIGHTNESS_PERCENT;
    }

    if (led == kLEDType_Ir && percent > MAX_IR_BRIGHTNESS_PERCENT)
    {
        percent = MAX_IR_BRIGHTNESS_PERCENT;
    }

    PWM_UpdatePwmDutycycle(s_LedPwms[led].baseAddr, s_LedPwms[led].submodule, s_LedPwms[led].channel,
                           kPWM_SignedCenterAligned, percent);
    PWM_SetPwmLdok(s_LedPwms[led].baseAddr, s_LedPwms[led].controlModule, true);

    return 0;
}

static int _init_led_pwm(led_type_t led)
{
    pwm_config_t pwmConfig;
    pwm_fault_param_t faultConfig;
    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[1];
    uint32_t pwmSourceClockInHz = PWM_SRC_CLK_FREQ;
    /* PWM freq needs to remain low (<=500) due to the LED driver filtering high freq noise */
    uint32_t pwmFrequencyInHz = 500;

    /* Set the PWM Fault inputs to a low value */
    XBARA_Init(XBARA1);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault2);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1234Fault3);

    /*
     * pwmConfig.enableDebugMode = false;
     * pwmConfig.enableWait = false;
     * pwmConfig.reloadSelect = kPWM_LocalReload;
     * pwmConfig.clockSource = kPWM_BusClock;
     * pwmConfig.prescale = kPWM_Prescale_Divide_1;
     * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
     * pwmConfig.forceTrigger = kPWM_Force_Local;
     * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
     * pwmConfig.reloadLogic = kPWM_ReloadImmediate;
     * pwmConfig.pairOperation = kPWM_Independent;
     */
    PWM_GetDefaultConfig(&pwmConfig);

    pwmConfig.enableDebugMode = true;
    pwmConfig.enableWait      = true;
    pwmConfig.prescale        = kPWM_Prescale_Divide_8;
    /* Initialize specified submodule */
    if (PWM_Init(s_LedPwms[led].baseAddr, s_LedPwms[led].submodule, &pwmConfig) == kStatus_Fail)
    {
        return kStatus_Fail;
    }

    /*
     *   config->faultClearingMode = kPWM_Automatic;
     *   config->faultLevel = false;
     *   config->enableCombinationalPath = true;
     *   config->recoverMode = kPWM_NoRecovery;
     */
    PWM_FaultDefaultConfig(&faultConfig);
    /* Sets up the PWM fault protection */
    PWM_SetupFaults(s_LedPwms[led].baseAddr, s_LedPwms[led].faultInput, &faultConfig);

    /* Set PWM fault disable mapping for specified submodule */
    PWM_SetupFaultDisableMap(s_LedPwms[led].baseAddr, s_LedPwms[led].submodule, s_LedPwms[led].channel,
                             s_LedPwms[led].faultChannel, s_LedPwms[led].faultDisable);

    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

    pwmSignal[0].pwmChannel       = s_LedPwms[led].channel;
    pwmSignal[0].level            = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 0;
    pwmSignal[0].deadtimeValue    = deadTimeVal;
    pwmSignal[0].faultState       = kPWM_PwmFaultState1;

    PWM_SetupPwm(s_LedPwms[led].baseAddr, s_LedPwms[led].submodule, pwmSignal, 1, kPWM_SignedCenterAligned,
                 pwmFrequencyInHz, pwmSourceClockInHz); /* Set the load okay bit for specified submodule to
                                                           load register from its buffer */

    PWM_SetPwmLdok(s_LedPwms[led].baseAddr, s_LedPwms[led].controlModule, true);

    /* Start PWM generation from specified Submodule  */
    PWM_StartTimer(s_LedPwms[led].baseAddr, s_LedPwms[led].controlModule);

    return kStatus_Success;
}

const static output_dev_event_handler_t s_OutputDev_IrWhiteLedsHandler = {
    .inferenceComplete = NULL,
    .inputNotify       = HAL_OutputDev_IrWhiteLeds_InputNotify,
};

const static output_dev_operator_t s_OutputDev_IrWhiteLedsOps = {
    .init   = HAL_OutputDev_IrWhiteLeds_Init,
    .deinit = NULL,
    .start  = HAL_OutputDev_IrWhiteLeds_Start,
    .stop   = NULL,
};

static output_dev_t s_OutputDev_IrWhiteLeds = {
    .name         = "ir_white_leds",
    .attr.type    = kOutputDevType_Other,
    .attr.reserve = NULL,
    .ops          = &s_OutputDev_IrWhiteLedsOps,
    .configs      = {[kLEDType_White] = {.name          = "whitePwm",
                                    .expectedValue = "<0-100>",
                                    .description   = "% brightness of the white LED",
                                    .get           = _getWhitePwm},
                [kLEDType_Ir]    = {.name          = "irPwm",
                                 .expectedValue = "<0-100>",
                                 .description   = "% brightness of the IR LED",
                                 .get           = _getIrPwm}},
};

static status_t _getWhitePwm(char *valueToString)
{
    itoa(s_OutputDev_IrWhiteLeds.configs[kLEDType_White].value, valueToString, 10);
    strcat(valueToString, "%");
    return kStatus_Success;
}

static status_t _getIrPwm(char *valueToString)
{
    itoa(s_OutputDev_IrWhiteLeds.configs[kLEDType_Ir].value, valueToString, 10);
    strcat(valueToString, "%");
    return kStatus_Success;
}

static uint8_t _getTargetPwm(led_type_t led, uint8_t curPwm, uint8_t direction)
{
    uint8_t pwm;
    uint8_t pwmMin      = ((led == kLEDType_Ir) ? IR_PWM_MIN : WHITE_PWM_MIN);
    uint8_t pwmMax      = ((led == kLEDType_Ir) ? IR_PWM_MAX : WHITE_PWM_MAX);
    uint8_t pwmInterval = ((led == kLEDType_Ir) ? IR_PWM_INTERVAL : WHITE_PWM_INTERVAL);
    if (direction)
    {
        pwm = curPwm + pwmInterval;
        if (pwm >= pwmMax)
            pwm = pwmMax;
    }
    else
    {
        if (curPwm <= (pwmInterval + pwmMin))
        {
            pwm = pwmMin;
        }
        else
        {
            pwm = curPwm - pwmInterval;
        }
    }
    LOGI("[LED:%d][curPwm:%d][targetPwm:%d] \r\n", led, curPwm, pwm);
    return pwm;
}

static hal_output_status_t HAL_OutputDev_IrWhiteLeds_InputNotify(const output_dev_t *receiver, void *data)
{
    hal_output_status_t error    = kStatus_HAL_OutputSuccess;
    event_base_t eventBase       = *(event_base_t *)data;
    event_status_t eventResponse = kEventStatus_Ok;

    switch (eventBase.eventId)
    {
        case kEventID_GetIRLedBrightness:
        {
            ir_led_event_t irLedBrightness;
            irLedBrightness.brightness = HAL_OutputDev_SmartLockConfig_GetIrPwm();
            LOGD("IR LED Brightness is currently set to: %d", irLedBrightness.brightness);
            if (eventBase.respond != NULL)
            {
                _HAL_OutputDev_IrWhiteLeds_Response(eventBase, &irLedBrightness, eventResponse, true);
            }
        }
        break;
        case kEventID_SetIRLedBrightness:
        {
            event_common_t event = *(event_common_t *)data;
            error |= set_led_brightness(kLEDType_Ir, event.irLed.brightness);
            if (!error)
            {
                if (kSLNConfigStatus_Success != HAL_OutputDev_SmartLockConfig_SetIrPwm(event.irLed.brightness))
                {
                    error         = kStatus_HAL_OutputError;
                    eventResponse = kEventStatus_Error;
                }
                else
                {
                    s_OutputDev_IrWhiteLeds.configs[kLEDType_Ir].value = event.irLed.brightness;
                }
            }
            else
            {
                eventResponse = kEventStatus_WrongParam;
            }

            _HAL_OutputDev_IrWhiteLeds_Response(eventBase, &event.irLed.brightness, eventResponse, true);
        }
        break;

        case kEventID_GetWhiteLedBrightness:
        {
            white_led_event_t whiteLedBrightness;
            whiteLedBrightness.brightness = HAL_OutputDev_SmartLockConfig_GetWhitePwm();
            LOGD("White LED Brightness is currently set to: %d", whiteLedBrightness.brightness);
            _HAL_OutputDev_IrWhiteLeds_Response(eventBase, &whiteLedBrightness, eventResponse, true);
        }
        break;
        case kEventID_SetWhiteLedBrightness:
        {
            event_common_t event = *(event_common_t *)data;
            error |= set_led_brightness(kLEDType_White, event.whiteLed.brightness);
            if (!error)
            {
                if (kSLNConfigStatus_Success != HAL_OutputDev_SmartLockConfig_SetWhitePwm(event.whiteLed.brightness))
                {
                    error         = kStatus_HAL_OutputError;
                    eventResponse = kEventStatus_Error;
                }
                else
                {
                    s_OutputDev_IrWhiteLeds.configs[kLEDType_White].value = event.whiteLed.brightness;
                }
            }
            else
            {
                eventResponse = kEventStatus_WrongParam;
            }

            _HAL_OutputDev_IrWhiteLeds_Response(eventBase, &event.whiteLed.brightness, eventResponse, true);
        }
        break;
        case kEventID_ControlIRLedBrightness:
        {
            ir_led_event_t irLedBrightness;
            irLedBrightness.brightness = s_OutputDev_IrWhiteLeds.configs[kLEDType_Ir].value;
            event_common_t event       = *(event_common_t *)data;
            ir_led_event_t targetIrLedBrightness;
            if (event.brightnessControl.enable == true)
            {
                targetIrLedBrightness.brightness =
                    _getTargetPwm(kLEDType_Ir, irLedBrightness.brightness, event.brightnessControl.direction);
            }
            else
            {
                targetIrLedBrightness.brightness = HAL_OutputDev_SmartLockConfig_GetIrPwm();
            }
            LOGD("IR LED PWM will be adjusted to: %d", targetIrLedBrightness.brightness);
            error |= set_led_brightness(kLEDType_Ir, targetIrLedBrightness.brightness);
            if (!error)
            {
                s_OutputDev_IrWhiteLeds.configs[kLEDType_Ir].value = targetIrLedBrightness.brightness;
            }
        }
        break;
        case kEventID_ControlWhiteLedBrightness:
        {
            white_led_event_t whiteLedBrightness;
            whiteLedBrightness.brightness = s_OutputDev_IrWhiteLeds.configs[kLEDType_White].value;
            event_common_t event          = *(event_common_t *)data;
            white_led_event_t targetWhiteLedBrightness;
            if (event.brightnessControl.enable == true)
            {
                targetWhiteLedBrightness.brightness =
                    _getTargetPwm(kLEDType_White, whiteLedBrightness.brightness, event.brightnessControl.direction);
            }
            else
            {
                targetWhiteLedBrightness.brightness = HAL_OutputDev_SmartLockConfig_GetWhitePwm();
            }
            LOGD("WHITE LED PWM will be adjusted to: %d", targetWhiteLedBrightness.brightness);
            error |= set_led_brightness(kLEDType_White, targetWhiteLedBrightness.brightness);
            if (!error)
            {
                s_OutputDev_IrWhiteLeds.configs[kLEDType_White].value = targetWhiteLedBrightness.brightness;
            }
        }
        break;
        default:
            return error;
            break;
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_IrWhiteLeds_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    /* TODO: the "const" of both args is discarded by being passed here */
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_IrWhiteLedsHandler) != 0)
    {
        error = kStatus_HAL_OutputError;
    }

    /* TODO: Get configs by name using configs stored via json */
    s_OutputDev_IrWhiteLeds.configs[kLEDType_White].value = HAL_OutputDev_SmartLockConfig_GetWhitePwm();
    set_led_brightness(kLEDType_White, s_OutputDev_IrWhiteLeds.configs[kLEDType_White].value);

    s_OutputDev_IrWhiteLeds.configs[kLEDType_Ir].value = HAL_OutputDev_SmartLockConfig_GetIrPwm();
    set_led_brightness(kLEDType_Ir, s_OutputDev_IrWhiteLeds.configs[kLEDType_Ir].value);

    return error;
}

hal_output_status_t HAL_OutputDev_IrWhiteLeds_Init(const output_dev_t *dev)
{
    int error = kStatus_HAL_OutputSuccess;

    LOGD("HAL_OutputDev_IrWhiteLeds_Init");
    _init_led_pwm(kLEDType_Ir);
    _init_led_pwm(kLEDType_White);
    return error;
}

int HAL_OutputDev_IrWhiteLeds_Register()
{
    int error = 0;
    LOGD("output_dev_ir_white_led_register");
    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_IrWhiteLeds);
    return error;
}
