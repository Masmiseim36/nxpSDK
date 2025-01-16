# iee_apc

## Overview
The IEE APC demo application demonstrates usage of the IEE and IEE APC driver. The Inline Encryption Engine (IEE) together with IEE APC provides a means to perform inline encryption and decryption of information transferred over memory interfaces. This demo application configures region 0 to perform AES XTS encryption and decryption while region 1 to perform AES-CTR decryption from external QSPI FLASH memory. Then data are written into RAM memory (region 0), after that encryption is disabled and encrypted data read. After the successful read, encryption is enabled and memory read again to see that data matches the original written value. Another encrypted data are programmed into FLASH memory (region 1) and after encryption is enabled data are read and compared with expected data.

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/demo_apps/iee_apc/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/demo_apps/iee_apc/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/demo_apps/iee_apc/example_board_readme.md)
