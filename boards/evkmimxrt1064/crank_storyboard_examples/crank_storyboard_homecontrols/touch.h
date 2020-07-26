#ifndef BOARD_TOUCH_H_
#define BOARD_TOUCH_H_

#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

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
