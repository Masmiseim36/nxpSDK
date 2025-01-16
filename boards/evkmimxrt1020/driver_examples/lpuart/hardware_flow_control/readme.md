# lpuart_hardware_flow_control

## Overview
The lpuart_hardware_flow_control Example project is to demonstrate usage of the hardware flow control function.
This example will send data to itself(loopback), and hardware flow control will be enabled in the example.
The CTS(clear-to-send) pin is for transmiter to check if receiver is ready, if the CTS pin is assert, transmiter start
to send data. The RTS(request-to-send) is a pin for receiver to inform the transmiter if receiver is ready to receive
data. So, please connect RTS to CTS pin directly.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [EVK-MIMXRT1015](../../../_boards/evkmimxrt1015/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/lpuart/hardware_flow_control/example_board_readme.md)
