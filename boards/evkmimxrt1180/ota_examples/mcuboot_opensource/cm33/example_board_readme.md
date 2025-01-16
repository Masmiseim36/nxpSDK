Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
============
The mcuboot binary must be processed by SPSDK tools to allow
ROM code to boot it from flash. If this is not done automatically
during build/postbuild it needs to be done manually.
When using NXP tools such as MCUXpresso IDE or MCUXpresso Secure Provisioning Tool
this should be done automatically by the tool.


### MCUBoot layout

| Region         | From       | To         | Size   |
|----------------|------------|------------|--------|
| MCUboot code   | 0x28000000 | 0x2803FFFF | 256kB  |
| Primary slot   | 0x28040000 | 0x2823FFFF | 2048kB |
| Secondary slot | 0x28240000 | 0x2843FFFF | 2048kB |

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

