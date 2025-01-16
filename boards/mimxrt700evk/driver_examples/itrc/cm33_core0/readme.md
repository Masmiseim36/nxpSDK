# itrc

## Overview
The ITRC Example project is a demonstration program that uses the MCUX SDK software to set up Intrusion and Tamper Response Controller.
Then tests the expected behaviour.

## Running the demo
When the demo runs successfully, the terminal displays similar information like the following:
~~~~~~~~~~~~~~~~~~

ITRC Peripheral Driver Example

Pass: No Event/Action triggered after Init

Enable ITRC IRQ Action response to SW Event 0

Trigger SW Event 0

ITRC IRQ Reached!
ITRC STATUS:
SW Event0 occured!
Generated ITRC interrupt!

Clear ITRC IRQ and SW Event 0 STATUS

Disable ITRC IRQ Action response to SW Event 0

Trigger SW Event 0

Pass: No Action triggered

End of example

~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXN236](../../_boards/frdmmcxn236/driver_examples/itrc/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/driver_examples/itrc/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/driver_examples/itrc/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/driver_examples/itrc/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/driver_examples/itrc/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/driver_examples/itrc/example_board_readme.md)
