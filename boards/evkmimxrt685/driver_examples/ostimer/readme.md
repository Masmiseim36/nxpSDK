# ostimer_example

## Overview
The OSTIMER project is a simple demonstration program of the SDK OSTIMER driver. It sets the OSTIMER as
the wakeup source from deep-sleep mode. Board will enter power deep sleep mode, and then wakeup by OS timer after about 5 seconds.
After wakeup from deep-sleep mode, OS timer will set match value 
to trigger the interrupt while the timer count tick reach the match value about every 2 seconds.

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/driver_examples/ostimer/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/driver_examples/ostimer/example_board_readme.md)
- [FRDM-MCXA153](../../_boards/frdmmcxa153/driver_examples/ostimer/example_board_readme.md)
- [FRDM-MCXA156](../../_boards/frdmmcxa156/driver_examples/ostimer/example_board_readme.md)
- [FRDM-MCXA166](../../_boards/frdmmcxa166/driver_examples/ostimer/example_board_readme.md)
- [FRDM-MCXA276](../../_boards/frdmmcxa276/driver_examples/ostimer/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/driver_examples/ostimer/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/driver_examples/ostimer/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/driver_examples/ostimer/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/driver_examples/ostimer/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/driver_examples/ostimer/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/driver_examples/ostimer/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/driver_examples/ostimer/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/driver_examples/ostimer/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/driver_examples/ostimer/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/driver_examples/ostimer/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/driver_examples/ostimer/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/driver_examples/ostimer/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/driver_examples/ostimer/example_board_readme.md)
