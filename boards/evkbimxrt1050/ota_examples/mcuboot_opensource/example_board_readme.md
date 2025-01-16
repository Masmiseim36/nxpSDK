Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============

### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x60000000 | 0x6003FFFF | 256kB  |
| Primary slot   | 0x60040000 | 0x6023FFFF | 2048kB |
| Secondary slot | 0x60240000 | 0x6043FFFF | 2048kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is RSA-2048
- Write alignment is 4 bytes
- MCUBoot is configured to use `SWAP_MOVE` image handling strategy


### Image signing example

    imgtool sign   --key sign-rsa2048-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x200000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin
