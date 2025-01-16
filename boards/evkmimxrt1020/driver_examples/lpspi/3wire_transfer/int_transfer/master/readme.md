# lpspi_3wire_int_transfer_master

## Overview
The lpspi_3wire_int_transfer example shows how to use LPSPI's 3-wire mode in interrupt way:

In this example , we need two boards, one board used as LPSPI master and another board used as LPSPI slave.
The file 'lpspi_3wire_int_b2b_transfer_master.c' includes the LPSPI master code.
This example uses the transactional API in LPSPI driver.

1. LPSPI master send/received data to/from LPSPI slave in interrupt . (LPSPI Slave using interrupt to receive/send the data)
2. LPSPI master will use SOUT pin as input/output pin, and slave will use SIN pin as input/output pin.

## Supported Boards
- [EVK-MIMXRT1015](../../../../../_boards/evkmimxrt1015/driver_examples/lpspi/3wire_transfer/int_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1020](../../../../../_boards/evkmimxrt1020/driver_examples/lpspi/3wire_transfer/int_transfer/master/example_board_readme.md)
- [MIMXRT1024-EVK](../../../../../_boards/evkmimxrt1024/driver_examples/lpspi/3wire_transfer/int_transfer/master/example_board_readme.md)
- [FRDM-KE15Z](../../../../../_boards/frdmke15z/driver_examples/lpspi/3wire_transfer/int_transfer/master/example_board_readme.md)
- [FRDM-KE17Z](../../../../../_boards/frdmke17z/driver_examples/lpspi/3wire_transfer/int_transfer/master/example_board_readme.md)
- [FRDM-KE17Z512](../../../../../_boards/frdmke17z512/driver_examples/lpspi/3wire_transfer/int_transfer/master/example_board_readme.md)
