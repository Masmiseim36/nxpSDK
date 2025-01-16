# bee

## Overview
This driver example should demonstrate how to setup BEE driver for on the fly decryption of data stored in QSPI memory region. The BEE is configuerd to 
decrypt all data starting from location 0x6000_0000 to 0x6001_0000 using AES ECB mode with user specified key. If the BEE_KEY_SEL fuse is programmed, then OTPMK key will be used instead.
The example itself does not perform any data decryption operation over QSPI. It is used only as a BEE setup guide.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/driver_examples/bee/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/driver_examples/bee/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/driver_examples/bee/example_board_readme.md)
- [EVK-MIMXRT1015](../../_boards/evkmimxrt1015/driver_examples/bee/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/driver_examples/bee/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/driver_examples/bee/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/driver_examples/bee/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/driver_examples/bee/example_board_readme.md)
