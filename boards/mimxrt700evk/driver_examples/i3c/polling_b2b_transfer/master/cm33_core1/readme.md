# i3c_polling_b2b_transfer_master

## Overview
The example shows how to use i3c driver as master to do board to board transfer using polling method.

In this example, one i3c instance as master and another i3c instance on the other board as slave. Master
first sends a piece of data to slave in I2C mode, and receive a piece of data from slave and check if the
data received from slave is correct. Then master will enter dynamic address cycle to assign address to the
connected slave, after success, will use I3C SDR mode to transfer data, then receive data from the connected
I3C slave and check the data consistency.

## Supported Boards
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT595](../../../../_boards/evkmimxrt595/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT685](../../../../_boards/evkmimxrt685/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA153](../../../../_boards/frdmmcxa153/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA156](../../../../_boards/frdmmcxa156/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA276](../../../../_boards/frdmmcxa276/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXW71](../../../../_boards/frdmmcxw71/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [K32W148-EVK](../../../../_boards/k32w148evk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [KW45B41Z-EVK](../../../../_boards/kw45b41zevk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [KW47-EVK](../../../../_boards/kw47evk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [LPCXpresso55S36](../../../../_boards/lpcxpresso55s36/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [LPCXpresso860MAX](../../../../_boards/lpcxpresso860max/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../../_boards/mcimx93autoevk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [MCIMX93-EVK](../../../../_boards/mcimx93evk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [MCIMX93-QSB](../../../../_boards/mcimx93qsb/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [MCX-W72-EVK](../../../../_boards/mcxw72evk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
- [MIMXRT700-EVK](../../../../_boards/mimxrt700evk/driver_examples/i3c/polling_b2b_transfer/master/example_board_readme.md)
