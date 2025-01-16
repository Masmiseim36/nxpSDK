# edma4_channel_link

## Overview
The EDMA channel link example is a simple demonstration program that uses the SDK software.
It excuates channel link transfer using the SDK EDMA drivers.
The purpose of this example is to show how to use the EDMA and to provide a simple example for
debugging and further development, it demostrates how to use the minor loop link and major loop link:
Since each transfer request can be divided into MAJOR_LOOPS_COUNTS * MINOR_LOOPS_BYTES,
such as you want to request EDMA transfer 8 bytes total, 4 bytes each request, then MAJOR_LOOPS_COUNTS = 2, MINOR_LOOPS_BYTES = 4.
The minor loop channel linking occurs at the completion of the minor loop 4 byte transferred.
The major loop channel linking is occurs at the major loop exhausted.
The example demostrate the channel link transfer by the feature of edma4:

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/edma4/channel_link/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/edma4/channel_link/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/edma4/channel_link/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/edma4/channel_link/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/edma4/channel_link/example_board_readme.md)
