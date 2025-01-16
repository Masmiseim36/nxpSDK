Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x70000000 | 0x7003FFFF | 256kB  |
| Primary slot   | 0x70040000 | 0x7013FFFF | 1024kB |
| Secondary slot | 0x70140000 | 0x7023FFFF | 1024kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is RSA-2048
- Write alignment is 4 bytes
- MCUBoot is configured to use its DIRECT_XIP image handling strategy
- Flash remapping HW feature is used for zero-overhead image swapping


### Image signing example

    imgtool sign   --key sign-rsa2048-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x100000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

Note that for the first image flashed manually together with the bootloader
additional imgtool options "--pad" and "--confirm" must be used. Otherwise
the bootloader would reject the image for missing data in the trailer area.
