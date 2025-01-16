# lpuart_9bit_interrupt_transfer

## Overview
The lpuart_9bit_interrupt_transfer example shows how to use lpuart driver in 9-bit mode in multi-slave system.
Master can send data to slave with certain address specifically, and slave can only receive data when it is addressed.

In this example, one lpuart instance is used with address configured. Its TX and RX pins are connected together.
First it sends a piece of data out, then addresses itself, after that sends the other piece of data. Only data
sent after the address can be received by itself.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [EVK-MIMXRT1010](../../../_boards/evkmimxrt1010/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [EVK-MIMXRT1015](../../../_boards/evkmimxrt1015/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-K32L2B](../../../_boards/frdmk32l2b/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-KE16Z](../../../_boards/frdmke16z/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-MCXC242](../../../_boards/frdmmcxc242/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [TWR-KM34Z75M](../../../_boards/twrkm34z75m/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
- [TWR-KM35Z75M](../../../_boards/twrkm35z75m/driver_examples/lpuart/9bit_interrupt_transfer/example_board_readme.md)
