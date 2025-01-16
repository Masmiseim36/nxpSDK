# ele_crypto_generic_hash

## Overview
The ELE Crypto Example project is a demonstration program that uses the MCUX SDK
software to perform crypto operations with EdgeLock Enclave (ELE) and usage of
its services with direct use of Messaging Unit driver.

## Running the demo
Example output on terminal:

EdgeLock Enclave Sub-System crypto example:

****************** Load EdgeLock FW ***********************
EdgeLock FW loaded and authenticated successfully.

****************** Open EdgeLock session ******************
Open session successfully. Session ID: 0x610a1d9

****************** Open Hash service **********************
Open HASH service successfully. Handle ID: 0x610a109

****************** Compute Hash (SHA256) of massage *******
*SUCCESS* Computed HASH matches the expected value.

****************** Open RNG service ***********************
Open RNG service successfully. Handle ID: 0x610a109

****************** Get RNG Random *************************
Get RNG random data successfully. First word: 0x98b5ef45

****************** Close RNG service *********************
Close RNG service successfully.

****************** Close Hash service *********************
Close HASH service successfully.

****************** Close EdgeLock session *****************
Close session successfully.

End of Example with SUCCESS!!

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ele_crypto/ele_crypto_generic_hash/example_board_readme.md)
