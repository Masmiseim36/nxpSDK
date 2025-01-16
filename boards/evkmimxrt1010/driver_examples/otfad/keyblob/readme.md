# otfad_keyblob

## Overview

The OTFAD example shows how to use on the fly AES-128 decryption driver.

In this example, the external flash memory will be used to show the example.
The flash memory will store ciphertext and keyblob, and this example will use OTFAD to decrypt ciphertext with flexspi.
The FlexSPI fetches the context 0 key blob from the external flash memory, sends the data to the OTFAD which automatically
performs the required key blob unwrap function and loads the recovered plaintext data into the context 0 programming model registers.
OTFAD always processes key blob data for all four contexts when enabled, and the flash A1 zero address has been loaded valid key wraped data
for all four contexts and OTFAD will decryption automatically.

## Supported Boards
- [EVK-MIMXRT1010](../../../_boards/evkmimxrt1010/driver_examples/otfad/keyblob/example_board_readme.md)
