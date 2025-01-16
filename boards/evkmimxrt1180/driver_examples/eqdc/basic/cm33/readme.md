# eqdc_basic

## Overview

The eqdc_basic example shows how to quickly start using EQDC driver.

In this example, user needs to connect a real encoder to the board. Actually, only PHASE A and PHASE B are enough for the basic application. When running the project, user can turn the encoder so that EQDC module can monitor the position change. Then, the internal counter would also count for the position. User can also type keys into terminal, and the current position values recorded by EQDC would display. 

The EQDC hardware is created with a special synchronize mechanism. There are actually 4 counters (the 32-bit position counter is combined with the two 16-bit counter registers) for position with responding hold registers. When any of the counter registers is read, the contents of each counter register is written to the corresponding hold register. Taking a snapshot of the counters' values provides a consistent view of a system position and a velocity to be attained.

## Running the demo
Turn the encoder and type in any key into terminal.
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

EQDC Basic Example.
Press any key to get the encoder values ...

Current position value: 0
Position differential value: 0
Position revolution value: 0

Current position value: 10
Position differential value: 10
Position revolution value: 0

...

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/eqdc/basic/example_board_readme.md)
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/eqdc/basic/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/eqdc/basic/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/eqdc/basic/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/eqdc/basic/example_board_readme.md)
