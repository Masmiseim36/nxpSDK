#include "lcd.h"
#include "touch.h"
#include <fsl_lpi2c.h>
#include <fsl_ft5406_rt.h>

#include "fsl_debug_console.h"
#include "peripherals.h"

// Touch driver handle.
static ft5406_rt_handle_t touchHandle;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
int BOARD_Touch_Init(void)
{
    status_t status;

    /*Enable clock setting for LPI2C*/
    CLOCK_EnableClock(kCLOCK_Lpi2c1);

    // Initialize the touch handle.
    status = FT5406_RT_Init(&touchHandle, LPI2C_1_PERIPHERAL);

    return status;
}

void BOARD_Touch_Deinit(void)
{
    LPI2C_MasterDeinit(LPI2C_1_PERIPHERAL);
}

int BOARD_Touch_Poll(touch_poll_state_t *output)
{
    status_t status;

    touch_event_t touch_event;
    int touch_x = -1;
    int touch_y = -1;

    status = FT5406_RT_GetSingleTouch(&touchHandle, &touch_event, &touch_x, &touch_y);
    if (kStatus_Success != status)
        return status;

    if (touch_event == kTouch_Reserved)
        return 0;

    output->x       = touch_x;
    output->y       = touch_y;
    output->pressed = (touch_event == kTouch_Down || touch_event == kTouch_Contact);
    return kStatus_Success;
}
