# otfad_decrypt_flexspi_nor

## Overview

The OTFAD example shows how to use on the fly AES-128 decryption driver.

In this example, the external flash memory will be used to show the example.
The flash memory will store ciphertext, and this example will use OTFAD to decrypt ciphertext with flexspi. 
It including configure AES key and counter into OTFAD and OTFAD will decryption automatically.

## Supported Boards
- [EVK-MIMXRT1010](../../../../_boards/evkmimxrt1010/driver_examples/otfad/decrypt/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/driver_examples/otfad/decrypt/example_board_readme.md)
