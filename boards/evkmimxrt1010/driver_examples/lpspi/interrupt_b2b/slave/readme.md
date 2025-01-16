# lpspi_interrupt_b2b_slave

## Overview
The lpspi_interrupt_b2b example shows how to use LPSPI driver in interrupt way:

In this example, we need two boards, one board used as LPSPI master and another board used as LPSPI slave.
The file 'lpspi_interrupt_b2b_slave.c' includes the LPSPI slave code.
This example does not use the transactional API in LPSPI driver.
LPSPI master send/received data to/from LPSPI slave in interrupt. (LPSPI Slave using interrupt to receive/send the data)


## Running the demo
When the demo runs successfully, the log would be seen in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPSPI interrupt board to board (b2b) slave example.
  Slave start to receive data...

LPSPI transfer all data matched!

 Slave received:
  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F

End of slave example!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [EVK9-MIMX8ULP](../../../../_boards/evk9mimx8ulp/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [EVKB-IMXRT1050](../../../../_boards/evkbimxrt1050/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../../_boards/evkbmimxrt1060/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../../_boards/evkcmimxrt1060/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../../_boards/evkmcimx7ulp/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [EVK-MIMX8ULP](../../../../_boards/evkmimx8ulp/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [EVK-MIMXRT1010](../../../../_boards/evkmimxrt1010/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [EVK-MIMXRT1015](../../../../_boards/evkmimxrt1015/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [EVK-MIMXRT1020](../../../../_boards/evkmimxrt1020/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MIMXRT1024-EVK](../../../../_boards/evkmimxrt1024/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MIMXRT1040-EVK](../../../../_boards/evkmimxrt1040/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../_boards/evkmimxrt1064/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-K32L2A4S](../../../../_boards/frdmk32l2a4s/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-K32L3A6](../../../../_boards/frdmk32l3a6/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-KE15Z](../../../../_boards/frdmke15z/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-KE16Z](../../../../_boards/frdmke16z/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-KE17Z](../../../../_boards/frdmke17z/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-KE17Z512](../../../../_boards/frdmke17z512/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-MCXA153](../../../../_boards/frdmmcxa153/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-MCXA156](../../../../_boards/frdmmcxa156/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-MCXA166](../../../../_boards/frdmmcxa166/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-MCXA276](../../../../_boards/frdmmcxa276/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-MCXW71](../../../../_boards/frdmmcxw71/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [FRDM-MCXW72](../../../../_boards/frdmmcxw72/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [K32W148-EVK](../../../../_boards/k32w148evk/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [KW45B41Z-EVK](../../../../_boards/kw45b41zevk/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [KW47-EVK](../../../../_boards/kw47evk/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../../_boards/mcimx93autoevk/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MCIMX93-EVK](../../../../_boards/mcimx93evk/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MCIMX93-QSB](../../../../_boards/mcimx93qsb/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
- [MCX-W72-EVK](../../../../_boards/mcxw72evk/driver_examples/lpspi/interrupt_b2b/slave/example_board_readme.md)
