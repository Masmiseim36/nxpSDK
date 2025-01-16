# cmsis_lpi2c_int_b2b_transfer_master

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The lpi2c_int_b2b_transfer_master example shows how to use CMSIS i2c driver as master to do board to board transfer 
with interrupt:

In this example, one lpi2c instance as master and another lpi2c instance on the other board as slave. Master sends a 
piece of data to slave, and receive a piece of data from slave. This example checks if the data received from 
slave is correct.

## Supported Boards
- [EVK9-MIMX8ULP](../../../../_boards/evk9mimx8ulp/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [EVKB-IMXRT1050](../../../../_boards/evkbimxrt1050/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../../_boards/evkbmimxrt1060/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../../_boards/evkcmimxrt1060/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../../_boards/evkmcimx7ulp/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMX8ULP](../../../../_boards/evkmimx8ulp/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1010](../../../../_boards/evkmimxrt1010/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1015](../../../../_boards/evkmimxrt1015/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1020](../../../../_boards/evkmimxrt1020/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1024-EVK](../../../../_boards/evkmimxrt1024/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1040-EVK](../../../../_boards/evkmimxrt1040/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../_boards/evkmimxrt1064/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-K32L2A4S](../../../../_boards/frdmk32l2a4s/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-K32L3A6](../../../../_boards/frdmk32l3a6/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-KE15Z](../../../../_boards/frdmke15z/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-KE16Z](../../../../_boards/frdmke16z/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-KE17Z](../../../../_boards/frdmke17z/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-KE17Z512](../../../../_boards/frdmke17z512/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA153](../../../../_boards/frdmmcxa153/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA156](../../../../_boards/frdmmcxa156/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA166](../../../../_boards/frdmmcxa166/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXA276](../../../../_boards/frdmmcxa276/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXW71](../../../../_boards/frdmmcxw71/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../../_boards/imx95lp4xevk15/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../../_boards/imx95lpd5evk19/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [imx95verdinevk](../../../../_boards/imx95verdinevk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [K32W148-EVK](../../../../_boards/k32w148evk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [KW45B41Z-EVK](../../../../_boards/kw45b41zevk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [KW47-EVK](../../../../_boards/kw47evk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../../_boards/mcimx93autoevk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MCIMX93-EVK](../../../../_boards/mcimx93evk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MCIMX93-QSB](../../../../_boards/mcimx93qsb/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MCX-N5XX-EVK](../../../../_boards/mcxn5xxevk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MCX-N9XX-EVK](../../../../_boards/mcxn9xxevk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MCX-W72-EVK](../../../../_boards/mcxw72evk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
- [MIMXRT700-EVK](../../../../_boards/mimxrt700evk/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/example_board_readme.md)
