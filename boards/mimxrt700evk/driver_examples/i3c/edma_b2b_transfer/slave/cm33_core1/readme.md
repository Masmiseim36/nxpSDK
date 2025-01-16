# i3c_edma_b2b_transfer_slave

## Overview
The example shows how to use i3c driver as slave to do board to board transfer with EDMA.

In this example, one i3c instance as slave and another i3c instance on the other board as master.
Master first enters dynamic address cycle to assign address to the connected slave, after success,
will use I3C SDR mode to transfer data, then receive data from the connected I3C slave and check
the data consistency.

## Supported Boards
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXA153](../../../../_boards/frdmmcxa153/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXA156](../../../../_boards/frdmmcxa156/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXA276](../../../../_boards/frdmmcxa276/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
- [MIMXRT700-EVK](../../../../_boards/mimxrt700evk/driver_examples/i3c/edma_b2b_transfer/slave/example_board_readme.md)
