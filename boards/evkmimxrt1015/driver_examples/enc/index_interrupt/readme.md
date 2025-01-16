# enc_index_interrupt

## Overview

The enc_index_interrupt example shows how to use the interrupt of ENC module with ENC driver.

In this example, user needs to connect a real encoder to the board. Actually, only PHASE A and PHASE B are enough for the basic application. However, the additional INDEX/HOME/TRIGGER could be used as the event detector. 

INDEX: This pulse can optionally reset the position counter and the pulse accumulator of the quadrature decoder module. It also causes a change of state on the revolution counter. The direction of this change, increment or decrement, is calculated from the PHASEA and PHASEB inputs.

HOME: This input can be used to trigger the initialization of the position counters. Often this signal is connected to a sensor signalling the motor or machine, sending notification that it has reached a defined home position.

TRIGGER: This input can be used to clear the position counters or to take a snapshot of the POS, REV, and POSD registers. Often this signal is connected to a periodic pulse generator or timer to indicate an elapsed time period.

This example uses INDEX to response the external event. When running the project, user can turn the encoder so that ENC module can monitor the position change. Also, a variable counter would count the time of INDEX interrupt for rising edge on INDEX signal line.

## Running the demo
Turn the encoder feed the negative pulse into INDEX and type in any key into terminal.
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

ENC INDEX Interrupt Example.
Press any key to get the encoder values ...

Current position value: 0
Current position differential value: 0
Current position revolution value: 0
g_EncIndexCounter: 0

Current position value: 20
Current position differential value: 20
Current position revolution value: 1
g_EncIndexCounter: 1

Current position value: 25
Current position differential value: 5
Current position revolution value: 2
g_EncIndexCounter: 2

...

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/enc/index_interrupt/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/enc/index_interrupt/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/enc/index_interrupt/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/enc/index_interrupt/example_board_readme.md)
- [EVK-MIMXRT1015](../../../_boards/evkmimxrt1015/driver_examples/enc/index_interrupt/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/enc/index_interrupt/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/enc/index_interrupt/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/enc/index_interrupt/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/enc/index_interrupt/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/enc/index_interrupt/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/enc/index_interrupt/example_board_readme.md)
