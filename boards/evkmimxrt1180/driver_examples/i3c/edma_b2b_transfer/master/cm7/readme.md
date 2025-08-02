# i3c_edma_b2b_transfer_master

## Overview
The example shows how to use i3c driver as slave to do board to board transfer with EDMA.

In this example, one i3c instance as slave and another i3c instance on the other board as master.
Master first enters dynamic address cycle to assign address to the connected slave, after success,
will use I3C SDR mode to transfer data, then receive data from the connected I3C slave and check
the data consistency.

## Running the demo
The following message shows in the terminal if the example runs successfully.

I3C board2board EDMA example -- Master transfer.
Master will send data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f

I3C master do dynamic address assignment to the I3C slaves on bus.
I3C master dynamic address assignment done.

Start to do I3C master transfer in I3C SDR mode.

I3C master wait for slave IBI event to notify the slave transmit size.
I3C master received slave IBI event, data size 1, value 0x20.

I3C master transfer successful in I3C SDR mode.

Start to do I3C master transfer in I3C HDR mode.
Receive sent data from slave :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f

I3C master transfer successful in I3C HDR mode.

## Supported Boards
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/driver_examples/i3c/edma_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA153](../../../../_boards/frdmmcxa153/driver_examples/i3c/edma_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA156](../../../../_boards/frdmmcxa156/driver_examples/i3c/edma_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/driver_examples/i3c/edma_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/i3c/edma_b2b_transfer/master/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/driver_examples/i3c/edma_b2b_transfer/master/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/driver_examples/i3c/edma_b2b_transfer/master/example_board_readme.md)
- [MIMXRT700-EVK](../../../../_boards/mimxrt700evk/driver_examples/i3c/edma_b2b_transfer/master/example_board_readme.md)
