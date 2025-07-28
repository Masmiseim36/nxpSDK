# sctimer_multi_state_pwm

## Overview
The SCTImer multi-state project is a demonstration program of the SCTimer state machine. It shows how to set up events to be triggered in a certain state
and transitioning between states.
This project has two states: State 0 and State 1.

For State 0, PWM0 (24kHz 10% duty cycle) is setup, PWM0 works based on event eventFirstNumberOutput
and event (eventFirstNumberOutput + 1). Meanwhile, another event is scheduled to monitor the rising edge on input 1,
when rising edge detected on input 1, the SCTimer switches to State 1.

For State 1, PWM1 (24kHz 50% duty cycle) is setup, PWM1 works based on event eventSecondNumberOutput
and event (eventSecondNumberOutput + 1). Meanwhile, another event is scheduled to monitor the rising edge on input 1,
when rising edge detected on input 1, then SCTimer switches to State 0. To make PWM0 also work in State 1, the PWM0's
events eventFirstNumberOutput and (eventFirstNumberOutput + 1) are also enabled in State 1, using API
SCTIMER_ScheduleEvent. As a result, both PWM0 and PWM1 works in State 1.

The SCTimer input 1 is routed to a button GPIO, when press the button, the SCTimer switches between State 0 and State 1.
In State 0, there is only PWM0 output, in State 1, there are both PWM0 and PWM1 output.

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [FRDM-MCXW23](../../../_boards/frdmmcxw23/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso51U68](../../../_boards/lpcxpresso51u68/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso55S28](../../../_boards/lpcxpresso55s28/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso824MAX](../../../_boards/lpcxpresso824max/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
- [MCXW23-EVK](../../../_boards/mcxw23evk/driver_examples/sctimer/multi_state_pwm/example_board_readme.md)
