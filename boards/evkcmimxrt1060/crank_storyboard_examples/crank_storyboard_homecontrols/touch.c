#include "lcd.h"
#include "touch.h"
#include <fsl_lpi2c.h>
#include "fsl_video_common.h"
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
#include "fsl_gt911.h"
#else
#include "fsl_ft5406_rt.h"
#endif

#include "fsl_debug_console.h"
#include "peripherals.h"
#ifdef SDK_OS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
static void BOARD_PullTouchResetPin(bool pullUp);
static void BOARD_ConfigTouchIntPin(gt911_int_pin_mode_t mode);
#endif

// Touch driver handle.
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
static gt911_handle_t s_touchHandle;
static const gt911_config_t s_touchConfig = {
    .I2C_SendFunc     = BOARD_Touch_I2C_Send,
    .I2C_ReceiveFunc  = BOARD_Touch_I2C_Receive,
    .pullResetPinFunc = BOARD_PullTouchResetPin,
    .intPinFunc       = BOARD_ConfigTouchIntPin,
#ifdef SDK_OS_FREE_RTOS
    .timeDelayMsFunc = vTaskDelay,
#else
    .timeDelayMsFunc = VIDEO_DelayMs,
#endif
    .touchPointNum = 1,
    .i2cAddrMode   = kGT911_I2cAddrMode0,
    .intTrigMode   = kGT911_IntRisingEdge,
};

#else
static ft5406_rt_handle_t touchHandle;
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
static void BOARD_PullTouchResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, 0);
    }
}

static void BOARD_ConfigTouchIntPin(gt911_int_pin_mode_t mode)
{
    if (mode == kGT911_IntPinInput)
    {
        BOARD_TOUCH_INT_GPIO->GDIR &= ~(1UL << BOARD_TOUCH_INT_PIN);
    }
    else
    {
        if (mode == kGT911_IntPinPullDown)
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, 0);
        }
        else
        {
            GPIO_PinWrite(BOARD_TOUCH_INT_GPIO, BOARD_TOUCH_INT_PIN, 1);
        }

        BOARD_TOUCH_INT_GPIO->GDIR |= (1UL << BOARD_TOUCH_INT_PIN);
    }
}
#endif

int BOARD_Touch_Init(void)
{
    status_t status;
    gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        0,
        kGPIO_NoIntmode,
    };

    /*Enable clock setting for LPI2C*/
    CLOCK_EnableClock(kCLOCK_Lpi2c1);

    GPIO_PinInit(BOARD_TOUCH_RST_GPIO, BOARD_TOUCH_RST_PIN, &config);
    // Initialize the touch handle.
#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
#ifdef SDK_OS_FREE_RTOS
    // wait a while to finalize LCD inicialization to prevent toggle reset touch after initialisation
    vTaskDelay(190);
#endif
    status = GT911_Init(&s_touchHandle, &s_touchConfig);

#else
    status = FT5406_RT_Init(&touchHandle, BOARD_TOUCH_I2C);
#endif

    return status;
}

void BOARD_Touch_Deinit(void)
{
    LPI2C_MasterDeinit(BOARD_TOUCH_I2C);
}

int BOARD_Touch_Poll(touch_poll_state_t *output)
{
    status_t status;
#if (DEMO_PANEL == DEMO_PANEL_RK043FN02H)
    touch_event_t touch_event;
#endif
    int touch_x = -1;
    int touch_y = -1;

#if (DEMO_PANEL == DEMO_PANEL_RK043FN66HS)
    status = GT911_GetSingleTouch(&s_touchHandle, &touch_x, &touch_y);
    if ((status == kStatus_Success) || (status == kStatus_TOUCHPANEL_NotTouched))
    {
        output->x       = touch_y;
        output->y       = touch_x;
        output->pressed = (status == kStatus_Success);
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
#else
    status = FT5406_RT_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y);
    if (kStatus_Success != status)
        return status;

    if (touch_event == kTouch_Reserved)
        return 0;

    output->x       = touch_x;
    output->y       = touch_y;
    output->pressed = (touch_event == kTouch_Down || touch_event == kTouch_Contact);
    return kStatus_Success;
#endif
}
