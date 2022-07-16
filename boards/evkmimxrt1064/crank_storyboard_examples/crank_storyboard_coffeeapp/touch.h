#ifndef BOARD_TOUCH_H_
#define BOARD_TOUCH_H_

#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#define DEMO_PANEL_RK043FN02H  0 /* RK043FN02H-CT */
#define DEMO_PANEL_RK043FN66HS 1 /* RK043FN66HS-CTG */

#ifndef DEMO_PANEL
#define DEMO_PANEL DEMO_PANEL_RK043FN66HS
#endif

// Init/deinit
int BOARD_Touch_Init(void);
void BOARD_Touch_Deinit(void);

typedef struct touch_poll_state_t_
{
    int x;
    int y;
    bool pressed;
} touch_poll_state_t;

/**
 * Touch polling
 *
 * @retval 0 no update to touch coordinates.
 * @retval 1 updated touch coordinates.
 */
int BOARD_Touch_Poll(touch_poll_state_t *output);

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // BOARD_TOUCH_H_
