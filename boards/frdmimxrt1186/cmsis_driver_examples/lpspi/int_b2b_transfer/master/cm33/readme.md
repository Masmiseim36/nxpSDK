# cmsis_lpspi_int_b2b_transfer_master

## Overview
The cmsis_lpspi_int_b2b_transfer example shows how to use LPSPI CMSIS driver in interrupt way.

In this example, we need two boards, one board used as LPSPI master and another board used as LPSPI slave.
The file 'cmsis_lpspi_int_b2b_transfer_master.c' includes the LPSPI master code.
This example uses the transactional API in LPSPI driver.

The LPSPI master send/receive data to/from LPSPI slave using interrupt.

## Running the demo
When the demo runs successfully, you will see output in the terminal like this:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPSPI CMSIS driver board to board interrupt example.
This example use one board as master and another as slave.
Master and slave uses interrupt way. Slave should start first.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
   CLK       --    CLK
   PCS       --    PCS
   SOUT      --    SIN
   SIN       --    SOUT
   GND       --    GND

Make sure the slave example is running, then press any key to continue.

Master transmit:
     1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
    11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
    21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
    31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

This is LPSPI_MasterSignalEvent_t
Master transmit data to slave has completed!
This is LPSPI_MasterSignalEvent_t
Master receive data from slave has completed!

LPSPI transfer all data matched!

Master received:
     1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10
    11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
    21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30
    31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 40

Press any key to run again.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## Supported Boards
- [EVK9-MIMX8ULP](../../../../_boards/evk9mimx8ulp/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [IMX952LPD5EVK-19](../../../../_boards/imx952evk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [IMX952LPD5EVK-15](../../../../_boards/imx952evk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [EVKB-IMXRT1050](../../../../_boards/evkbimxrt1050/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../../_boards/evkbmimxrt1060/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../../_boards/evkcmimxrt1060/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../../_boards/evkmcimx7ulp/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMX8ULP](../../../../_boards/evkmimx8ulp/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1010](../../../../_boards/evkmimxrt1010/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1015](../../../../_boards/evkmimxrt1015/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1020](../../../../_boards/evkmimxrt1020/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1024-EVK](../../../../_boards/evkmimxrt1024/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1040-EVK](../../../../_boards/evkmimxrt1040/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../_boards/evkmimxrt1064/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-K32L2A4S](../../../../_boards/frdmk32l2a4s/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-K32L3A6](../../../../_boards/frdmk32l3a6/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-KE15Z](../../../../_boards/frdmke15z/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-KE16Z](../../../../_boards/frdmke16z/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-KE17Z](../../../../_boards/frdmke17z/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-KE17Z512](../../../../_boards/frdmke17z512/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA153](../../../../_boards/frdmmcxa153/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA156](../../../../_boards/frdmmcxa156/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA174](../../../../_boards/frdmmcxa174/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA344](../../../../_boards/frdmmcxa344/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA346](../../../../_boards/frdmmcxa346/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXE247](../../../../_boards/frdmmcxe247/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXL255](../../../../_boards/frdmmcxl255/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXW71](../../../../_boards/frdmmcxw71/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCX-W71-EVK](../../../../_boards/mcxw71evk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [KW45B41Z-EVK](../../../../_boards/kw45b41zevk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [KW47-EVK](../../../../_boards/kw47evk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../../_boards/mcimx93autoevk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCIMX93-EVK](../../../../_boards/mcimx93evk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCIMX93W-EVK](../../../../_boards/mcimx93wevk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCIMX93-QSB](../../../../_boards/mcimx93qsb/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCX-W72-EVK](../../../../_boards/mcxw72evk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT700-EVK](../../../../_boards/mimxrt700evk/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [KW47-LOC](../../../../_boards/kw47loc/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [MCXW72-LOC](../../../../_boards/mcxw72loc/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-IMXRT1186](../../../../_boards/frdmimxrt1186/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA266](../../../../_boards/frdmmcxa266/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA366](../../../../_boards/frdmmcxa366/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXW72](../../../../_boards/frdmmcxw72/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA577](../../../../_boards/frdmmcxa577/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXC162](../../../../_boards/frdmmcxc162/cmsis_driver_examples/lpspi/int_b2b_transfer/master/example_board_readme.md)
