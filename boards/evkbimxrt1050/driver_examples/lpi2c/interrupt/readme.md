# lpi2c_interrupt

## Overview
The lpi2c_functional_interrupt example shows how to use lpi2c functional driver to build a interrupt based application:

In this example , one lpi2c instance used as lpi2c master and another lpi2c instance used as lpi2c slave .
1. lpi2c master send data using interrupt to lpi2c slave in interrupt .
2. lpi2c master read data using interrupt from lpi2c slave in interrupt .
3. The example assumes that the connection is OK between master and slave, so there's NO error handling code.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [FRDM-MCXW72](../../../_boards/frdmmcxw72/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/lpi2c/interrupt/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/lpi2c/interrupt/example_board_readme.md)
