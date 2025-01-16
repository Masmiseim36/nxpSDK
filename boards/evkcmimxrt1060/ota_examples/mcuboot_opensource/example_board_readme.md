Hardware requirements
===================
- Mini/micro USB cable
- MIMXRT1060-EVKC board
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x60000000 | 0x6003FFFF | 256kB  |
| Primary slot   | 0x60040000 | 0x6023FFFF | 4096kB |
| Secondary slot | 0x60440000 | 0x6083FFFF | 4096kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is RSA-2048
- Write alignment is 4 bytes
- MCUBoot is configured to use its DIRECT_XIP image handling strategy
- Flash remapping HW feature is used for zero-overhead image swapping


### Image signing example

    imgtool sign   --key sign-rsa2048-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x400000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

Note that for the first image flashed manually together with the bootloader
additional imgtool options "--pad" and "--confirm" must be used. Otherwise
the bootloader would reject the image for missing data in the trailer area.
