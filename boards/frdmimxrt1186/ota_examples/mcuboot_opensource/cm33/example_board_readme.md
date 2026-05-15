Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
No special setup required.


### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x04000000 | 0x0403FFFF | 256kB  |
| Primary slot   | 0x04040000 | 0x0423FFFF | 2048kB |
| Secondary slot | 0x04240000 | 0x0443FFFF | 2048kB |

- MCUBoot header size is set to 1024 bytes
- Signing algorithm is ECDSA-P256
- Write alignment is 4 bytes
- MCUBoot is configured to use `SWAP_MOVE` image handling strategy


### Image signing example

    imgtool sign   --key sign-ecdsa-p256-priv.pem
                   --align 4
                   --version 1.1
                   --slot-size 0x200000
                   --header-size 0x400
                   --pad-header
                   ota_mcuboot_basic.bin
                   ota_mcuboot_basic.SIGNED.bin

