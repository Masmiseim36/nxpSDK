# flexio_spi_edma_lpspi_transfer_slave_dynamic

## Overview
The flexio_spi_edma_lpspi_transfer_slave_dynamic example shows how to use flexio spi slave driver in edma way:

In this example, a flexio simulated slave connect to a lpspi master. The CS signal remains low during transfer,
after master finishes the transfer and re-asserts the CS signal, interrupt is triggered to let user know.

## Supported Boards
- [EVKB-IMXRT1050](../../../../../_boards/evkbimxrt1050/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../../../_boards/evkbmimxrt1060/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../../../_boards/evkbmimxrt1170/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../../../_boards/evkcmimxrt1060/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [EVK-MIMXRT1010](../../../../../_boards/evkmimxrt1010/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [EVK-MIMXRT1015](../../../../../_boards/evkmimxrt1015/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [EVK-MIMXRT1020](../../../../../_boards/evkmimxrt1020/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [MIMXRT1024-EVK](../../../../../_boards/evkmimxrt1024/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [MIMXRT1040-EVK](../../../../../_boards/evkmimxrt1040/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../../_boards/evkmimxrt1064/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../../_boards/evkmimxrt1160/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
- [MIMXRT1180-EVK](../../../../../_boards/evkmimxrt1180/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/example_board_readme.md)
