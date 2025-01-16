# acmp_round_robin

## Overview
The ACMP Round-Robin project is a simple demonstration program that uses the SDK software. User
must set the round-robin mode trigger in specific board properly according to the board resource
before running the example. When the example running, it sets positive port as fixed channel and
internal DAC output as comparison reference in positive port and sets the channels input by user
as round-robin checker channel. The example will enter stop mode and wait user to change the voltage
of round-robin checker channel. It will exit stop mode after the voltage of round-robin checker
channel changed by user.

## Running the demo
When the demo runs successfully, following information can be seen on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Example to demonstrate low power wakeup by round robin comparison!
In order to wakeup the MCU, please change the analog input voltage to be different from original pre-state setting.

Press S for enter: Stop Mode

The system entered into stop mode.

The system exited from stop mode!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/acmp/round_robin/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/acmp/round_robin/example_board_readme.md)
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/acmp/round_robin/example_board_readme.md)
- [FRDM-KE16Z](../../../_boards/frdmke16z/driver_examples/acmp/round_robin/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/driver_examples/acmp/round_robin/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/acmp/round_robin/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/acmp/round_robin/example_board_readme.md)
