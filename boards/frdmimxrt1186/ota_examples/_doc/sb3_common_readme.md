# OTA update by using SB3 file

The [secure binary (SB)](https://spsdk-try.readthedocs.io/en/stable/images/secure_update.html) container in version 3.1 (SB3 for short) provides an alternative way to encapsulate an OTA image. This format offers a secure and easy way to utilize hardware security features of a device to safely transport an OTA update, regardless of the security of the transport channel.

In MCUXpresso OTA examples, there are two ways of using SB3:

   1. __The ROM bootloader utilizing Dual image feature__
      * The process is demonstrated in the `ota_rom_basic` example
      * Flash memory is divided into two equal regions for images
      * Based on image version, the ROM bootloader disables or enables the flash remapping feature and jumps to the application
   2. __The SB3 is used as an encapsulation of MCUboot image to secure its transport__
      * The process is demonstrated in the `ota_mcuboot_basic` example
      * The example provides an additional command 'xmodem_sb3' to receive and process the SB3 file
      * the flash remapping feature is driven by MCUboot

## OTA using SB3 and flash remapping functionality

Flash remapping feature, if supported, is enabled by default in the examples. However, there are a few things to keep in mind when using SB3 as an OTA update.

A user has to ensure that the SB3 processed by an application has the correct load address targeting the currently __inactive image region__ because the active application has no control over ROM IAP handling SB3. This creates a risk of overwriting the active image region, potentially leading to a bricked device if an incorrect SB3 is downloaded and no backup image is present in the inactive image region to recover. This applies to both MCUboot and ROM bootloader use cases.

__In other words, two separate SB3 files (for first and second image regions) have to be prepared if flash remap based on OVERLAY is used. The application has to distinguish between these two binaries during an OTA update.__ 

__In the case of flash remap based on the SWAP mechanism, the SB3 file must be always generated for the second image region. Due to the design of SWAP, the SB3 payload is always written to the inactive image region.__

For more information, please read ['Flash remapping functionality'](flash_remap_readme.md).

## Step-by-step walkthrough

- [RW61X](sb3_rw61x_readme.md)
- [MCXN](sb3_mcxn_readme.md)
- [RT7xx](sb3_rt7xx_readme.md)

## Supported Boards

- [RD-RW612-BGA](../../_boards/rdrw612bga/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)