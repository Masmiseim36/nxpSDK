# acmp_interrupt_print

## Overview
The ACMP Interrupt project is a simple demonstration program that uses the SDK software. It
compares the selected analog input with ACMP internal DAC output continuously and print information
corresponding to different comparison result in terminal. The purpose of this demo is to show how to
use the ACMP driver in SDK software by interrupt way. The ACMP can be configured based on default
configuration returned by the API ACMP_GetDefaultConfig(). The default configuration is: high
speed is not enabled, invert output is not enabled, unfiltered output is not enabled, pin out
is not enabled, offset level is level 0, hysteresis level is level 0.

## Running the demo
Before example running, please connect the analog input voltage to stable external voltage generator
to avoid floating voltage and adjust external voltage generator to be higher than reference DAC output
(half of referene voltage) to make the initial comparison result to be 0.
Please note that the input voltage should range from 0 to 1.8V.
When the demo runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The example compares analog input to the reference DAC output(CMP positive port).

The terminal will print CMP's output value when press any key.

Please press any key to get CMP's output value.

The analog input is LOWER than DAC output

The analog input is HIGHER than DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/acmp/interrupt/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/acmp/interrupt/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/acmp/interrupt/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/acmp/interrupt/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/acmp/interrupt/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/acmp/interrupt/example_board_readme.md)
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/acmp/interrupt/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/acmp/interrupt/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/acmp/interrupt/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/acmp/interrupt/example_board_readme.md)
