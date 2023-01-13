#ifndef TOUCH_H_
#define TOUCH_H_

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

typedef struct touch_poll_state_t_
{
    int x;
    int y;
    bool pressed;
    status_t state;
} touch_poll_state_t;


void BOARD_Touch_Init(void);
status_t BOARD_Touch_Poll(touch_poll_state_t *output);

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif /* TOUCH_H_ */
