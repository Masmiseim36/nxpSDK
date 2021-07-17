#ifndef BOARD_TOUCH_H_
#define BOARD_TOUCH_H_

#include "board.h"
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

typedef struct touch_poll_state_t_
{
    int x;
    int y;
    bool pressed;
} touch_poll_state_t;

#if ((DEMO_PANEL == DEMO_PANEL_RM67162) || (DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))
status_t BOARD_MIPIPanelTouch_I2C_Receive(uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif

// Init/deinit
int BOARD_Touch_Init(void);
void BOARD_Touch_Deinit(void);


/**
 * Touch polling
 *
 * @retval 0 no update to touch coordinates.
 * @retval 1 updated touch coordinates.
 */
int BOARD_Touch_Poll(touch_poll_state_t *output);

#if (DEMO_PANEL == DEMO_PANEL_RM67162)
void BOARD_TouchIntHandler(void);
#endif

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // BOARD_TOUCH_H_
