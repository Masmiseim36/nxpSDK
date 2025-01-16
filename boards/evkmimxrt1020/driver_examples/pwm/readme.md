# pwm

## Overview
The PWM project is a simple demonstration program of the SDK PWM driver.
The pulse width modulator (PWM) module contains PWM submodules, each of which is set up to control a single half-bridge power stage.
Fault channel support is provided. This PWM module can generate various switching patterns, including highly sophisticated waveforms.
It can be used to control all known Switched Mode Power Supplies (SMPS) topologies.

The project uses eFlexPWM to generate A-phase pwm, B-phase pwm and C-phase pwm through submodule 0, submodule 1, submodule 2.

Submodule 0 can generate one complementary PWM: PWM A and PWM B. For PWM A, PWM A(1kHz 50% duty cycle) is setup, PWM A works
based on SignedCenterAligned. PWM B operates in PWM A complementary mode, so the dutycycle field of PWM B does not matter.
To prevent short circuits, the dead time count is set, about 650ns.

Submodule 1 and submodule 2 are set to MasterSync. This means that submodule 1 and submodule 2 are initialized synchronously
from submodule 0. In MasterSync mode, submodule 0 can be used as the main module to generate control signals to control
other modules.

This program is set to center-aligned PWM mode, and the duty cycle and position of the PWM can be changed by modifying the
value of pwmVal in this mode.

In this project, the duty cycle of submodule 0 is pwmVal, the duty cycle of submodule 1 is pwmVal divided by 2 and the duty
cycle of submodule 2 is pwmVal divided by 4.

PWM_SetPwmLdok() can issue the load command to multiple submodules at the same time. The values are loaded immediately if
kPWM_ReloadImmediate option was choosen during config. Else the values are loaded at the next PWM reload point.

The log below shows example output of the PWM driver demo in the oscilloscope :

  ^
  | +--------+
  | |        |
  +-+--------+----->     submodule0
  |   +---+
  |   |   |
  +---+---+-------->     submodule1
  |    +-+
  |    | |
  +----+-+--------->     submodule2

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/driver_examples/pwm/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/driver_examples/pwm/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/pwm/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/driver_examples/pwm/example_board_readme.md)
- [EVK-MIMXRT1010](../../_boards/evkmimxrt1010/driver_examples/pwm/example_board_readme.md)
- [EVK-MIMXRT1015](../../_boards/evkmimxrt1015/driver_examples/pwm/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/driver_examples/pwm/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/driver_examples/pwm/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/driver_examples/pwm/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/driver_examples/pwm/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/pwm/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/driver_examples/pwm/example_board_readme.md)
- [FRDM-MCXA153](../../_boards/frdmmcxa153/driver_examples/pwm/example_board_readme.md)
- [FRDM-MCXA156](../../_boards/frdmmcxa156/driver_examples/pwm/example_board_readme.md)
- [FRDM-MCXA166](../../_boards/frdmmcxa166/driver_examples/pwm/example_board_readme.md)
- [FRDM-MCXA276](../../_boards/frdmmcxa276/driver_examples/pwm/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/driver_examples/pwm/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/driver_examples/pwm/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/driver_examples/pwm/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/driver_examples/pwm/example_board_readme.md)
