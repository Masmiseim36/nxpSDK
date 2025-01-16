# eqdc_index_interrupt

## Overview

The eqdc_index_interrupt example shows how to use the interrupt of EQDC module with EQDC driver.

In this example, user needs to connect a real encoder to the board. Actually, only PHASE A and PHASE B are enough for the basic application. However, the additional INDEX/HOME/TRIGGER could be used as the event detector. 

INDEX: This pulse can optionally reset the position counter and the pulse accumulator of the quadrature decoder module. It also causes a change of state on the revolution counter. The direction of this change, increment or decrement, is calculated from the PHASEA and PHASEB inputs.

HOME: This input can be used to trigger the initialization of the position counters. Often this signal is connected to a sensor signalling the motor or machine, sending notification that it has reached a defined home position.

TRIGGER: This input can be used to clear the position counters or to take a snapshot of the POS, REV, and POSD registers. Often this signal is connected to a periodic pulse generator or timer to indicate an elapsed time period.

This example uses INDEX to response the external event. When running the project, user can turn the encoder so that EQDC module can monitor the position change. Also, a variable counter would count the time of INDEX interrupt for rising edge on INDEX signal line.

## Running the demo
Turn the encoder feed the negative pulse into INDEX and type in any key into terminal.
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

EQDC INDEX Interrupt Example.
Press any key to get the encoder values ...

Current position value: 0
Current position differential value: 0
Current position revolution value: 0
g_EqdcIndexCounter: 0

Current position value: 20
Current position differential value: 20
Current position revolution value: 1
g_EqdcIndexCounter: 1

Current position value: 25
Current position differential value: 5
Current position revolution value: 2
g_EqdcIndexCounter: 2

...

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/eqdc/index_interrupt/example_board_readme.md)
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/eqdc/index_interrupt/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/eqdc/index_interrupt/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/eqdc/index_interrupt/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/eqdc/index_interrupt/example_board_readme.md)
