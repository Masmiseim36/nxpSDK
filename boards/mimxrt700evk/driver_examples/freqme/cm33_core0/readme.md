# freqme_interrupt

## Overview
The freqme_interrupt is a demonstration program of the SDK LPC_FREQME driver's features.
The example demostrates the usage of frequency-measurement operating mode and pulse-width measurement operating mode.
In frequency measurement mode, the reference clock source is fixed. Users can select target clock source and input
reference clock scaling factor, then after a while, the frequency of target clock source will be printed to the
terminal. To measure the frequency with a high degree of accuracy, the frequency of target clock is better less than
that of the reference clock.
In pulse width measurement mode, the target clock source is fixed. Users can select reference clock source and pulse
polarity, then after a while, the high or low period of reference clock will be printed to the terminal. To measure the
pulse period with a high degree of accuracy, the frequency of reference clock is better less than that of target clock.

## Supported Boards
- [FRDM-MCXA153](../../_boards/frdmmcxa153/driver_examples/freqme/example_board_readme.md)
- [FRDM-MCXA156](../../_boards/frdmmcxa156/driver_examples/freqme/example_board_readme.md)
- [FRDM-MCXA166](../../_boards/frdmmcxa166/driver_examples/freqme/example_board_readme.md)
- [FRDM-MCXA276](../../_boards/frdmmcxa276/driver_examples/freqme/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/driver_examples/freqme/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/driver_examples/freqme/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/driver_examples/freqme/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/driver_examples/freqme/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/driver_examples/freqme/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/driver_examples/freqme/example_board_readme.md)
