# ele_crypto_common

## Overview
The ELE Crypto Example project is a demonstration program that uses the MCUX SDK
software to perform crypto operations with EdgeLock Enclave (ELE) and usage of
its services with direct use of Messaging Unit driver.

## Running the demo
Example output on terminal:

EdgeLock Enclave Sub-System crypto example:

****************** Load EdgeLock FW ***********************
EdgeLock FW loaded and authenticated successfully.

****************** Compute Hash (SHA256) of massage *******
*SUCCESS* Computed HASH matches the expected value.

*SUCCESS* HASH Init done.

*SUCCESS* HASH Update done.

*SUCCESS* Computed HASH (Finish) matches the expected value.

****************** Start RNG ******************************
EdgeLock RNG Start success.

EdgeLock RNG ready to use.

****************** Get RNG Random **********************
Get RNG random data successfully. First word: 0xbcd7f0de

End of Example with SUCCESS!!

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ele_crypto/ele_crypto_common/example_board_readme.md)
