# MCUboot opensource

- [Overview](#overview)
- [Flash memory layout](#flash-memory-layout)
- [Signing the application image](#signing-the-application-image)
- [Using MCUXpresso Secure Provisioning Tool for MCUBoot image signing](#using-mcuxpresso-secure-provisioning-tool-for-mcuboot-image-signing)
- [Prepare the Demo](#prepare-the-demo)
- [Running the demo](#running-the-demo)
- [Advanced topics](#advanced-topics)
- [Supported Boards](#supported-boards)

## Overview

The mcuboot_opensource is a second stage bootloader based on MCUBoot project. It is primarily meant to be used together with OTA (over-the-air) update examples
to demonstrate functionality of application self-upgrade.

## Flash memory layout

Flash memory is divided into multiple regions to allocate space for bootloader, main application
and application update:

 - MCUBoot partition (reserved for bootloader itself, starts at the beginning of the FLASH memory)
 - Primary application partition (active application image)
 - Secondary application partition (candidate application - place to download OTA image to be used for update)

The partitioning is defined by definitions in `flash_partitioning.h` header file.
The MCUBoot partition starts at the very beginning of the FLASH memory and spans up to `BOOT_FLASH_ACT_APP`.
The primary partition occupies range starting from `BOOT_FLASH_ACT_APP` up to `BOOT_FLASH_CAND_APP`.
The secondary partition starts at `BOOT_FLASH_CAND_APP` and it is automatically assigned the same size as the primary one.
The rest of the memory may be used by the application for arbitrary purposes.

Important notice: should you need to change the partitioning please make sure to update also the header file used by the OTA application!
If the partitioning information used by the bootloader and the application is not in sync, it may lead to malfunction of boot/OTA process or to upredictable behavior.

## Signing the application image

MCUBoot expects signed application image in specific format to be present in the primary partition.
The very same image format is also used for OTA updates.

A dedicated tool (imgtool) is used to generate application image in the desired format.
It is implemented as a Python script which can be found in the SDK package in *middleware/mcuboot_opensource/scripts* folder.

Alternatively the tool can be installed by the Python package manager:

    pip install imgtool

Please note that imgtool version installed by the Python package manager is not guaranteed to be compatible with MCUBoot present in you SDK package.

The mcuboot_opensource SDK project comes with its set of private-public keys.
The key pair is stored in the keys subdirectory (e.g. *boards/[board]/mcuboot_opensource/keys*).
The public key is already pre-configured in the source code of MCUBoot in a form of an array initializer.

To sign an application binary, imgtool must be provided with respective private key and a set of parameters as in the following example:

    imgtool sign --key sign-rsa2048-priv.pem
	             --align 4
	             --header-size 0x400
                 --pad-header
	             --slot-size 0x200000
	             --max-sectors 800
	             --version "1.0"
	             app_binary.bin
	             app_binary_SIGNED.bin 

NOTE that other boards may require different parameters - check their readme file.

The parameters used in the example above are tested with out-of-the-box configuration of MCUBoot and OTA examples in the SDK package.
However, some of them may depend on the application or board setup and thus may need to be modified.
See the MCUBoot documentation for the meaning of the parameters and align them with your project setup if necessary.
https://docs.mcuboot.com/imgtool.html

## Using MCUXpresso Secure Provisioning Tool for MCUBoot image signing

MCUXpresso Secure Provisioning Tool from verion 9 supports automation for MCUBoot image signing. Using this tool
it's possible to setup the device for entire boot chain in a few steps.

## Prepare the Demo

1.  Connect a USB cable between the PC host and the OpenSDA(or USB to Serial) USB port on the target board.
2.  Open a serial terminal on PC for OpenSDA serial(or USB to Serial) device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
    - line ending set for LF ('\n')
3.  Build project and program it to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo

When the demo runs successfully, the terminal will display the following:

    hello sbl.
    Bootloader Version 1.0.0

Further messages printed to the terminal depend on the content of the FLASH memory. In case there was no application programmed so far (i.e. the FLASH was blank), similar text as the following would be printed:

    Primary image: magic=unset, swap_type=0x1, copy_done=0x3, image_ok=0x3
    Secondary image: magic=unset, swap_type=0x1, copy_done=0x3, image_ok=0x3
    Boot source: none
    Swap type: none
    erasing trailer; fa_id=2
    Unable to find bootable image
        
At this point the bootloader is in place, resident in the FLASH memory. You may stop debuger, switch to an OTA example and follow the relevant readme located in example's directory.

## Advanced topics

[Flash remapping functionality](../_docs/flash_remap_readme.md)

[Encrypted XIP support](../_docs/encrypted_xip_readme.md)

[OTA update by using SB3 file](../_docs/sb3_readme.md)

## Supported Boards

- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/ota_examples/mcuboot_opensource/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-MCXA153](../../_boards/frdmmcxa153/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-MCXA156](../../_boards/frdmmcxa156/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-MCXA346](../../_boards/frdmmcxa346/ota_examples/mcuboot_opensource/example_board_readme.md)
