# cdog

## Overview
The CWT Example project is a demonstration program that uses the KSDK software to set up secure counter and instruction timer.
Then tests several times the expected value with value in secure counter. After that miscompare fault is intentionally generated
by comparing secure counter with wrong value. At the end application let the instruction timer reach zero and generate another timeout fault.

## Running the demo
When the demo runs successfully, the terminal displays similar information like the following:
~~~~~~~~~~~~~~~~~~

CDOG Peripheral Driver Example

CDOG IRQ Reached
* Miscompare fault occured *

intruction timer:   fffc5
intruction timer:   c615e
intruction timer:   8c34e
intruction timer:   5251a
intruction timer:   18703
CDOG IRQ Reached
* Timeout fault occured *

End of example

Note:
To keep the program running correctly, it is recommended to perform a power on reset (POR) after loading the application.
SW reset does not clear pending fault flags.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/cdog/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/cdog/example_board_readme.md)
- [FRDM-MCXA153](../../_boards/frdmmcxa153/driver_examples/cdog/example_board_readme.md)
- [FRDM-MCXA156](../../_boards/frdmmcxa156/driver_examples/cdog/example_board_readme.md)
- [FRDM-MCXA166](../../_boards/frdmmcxa166/driver_examples/cdog/example_board_readme.md)
- [FRDM-MCXA276](../../_boards/frdmmcxa276/driver_examples/cdog/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/driver_examples/cdog/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/driver_examples/cdog/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/driver_examples/cdog/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/driver_examples/cdog/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/driver_examples/cdog/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/driver_examples/cdog/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/driver_examples/cdog/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/driver_examples/cdog/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/driver_examples/cdog/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/driver_examples/cdog/example_board_readme.md)
