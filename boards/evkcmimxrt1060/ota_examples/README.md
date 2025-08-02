# MCUXpresso OTA examples

MCUXpresso SDK provides several examples and software to demonstrate OTA capabilities of NXP devices. The set of examples located in `ota_examples` targets to demonstrate usage of opensource MCUBoot or ROM bootloader in NXP devices.

## MCUboot examples

[MCUboot](https://docs.mcuboot.com/) is a second stage bootloader. A device typically needs a bootloader to be able to update firmware and verify its validity before executing it. NXP devices already implement a form of bootloader in their ROM code. The ROM bootloader often implements similar features such as code signature validation or mechanisms for firmware updates. If features offered by the ROM bootloader are not sufficient or a user has special requirements, the typical solution is to use a second-stage bootloader. The typical chain of execution is then: ROM bootloader → Second-stage bootloader → Application.

MCUboot is an open source community project that aims to implement necessary features for firmware update. It's used in the Zephyr project as a default bootloader. In MCUXpresso SDK it's used together with OTA applications to give an example of a solution for firmware update.

MCUboot examples structure looks like this:
*	`mcuboot_opensource` - bootloader application, __recommended starting point__
*	`ota_mcuboot_basic` - basic OTA application using a serial line to update
*	`ota_mcuboot_client` - client OTA application using a network connection to update
*	`ota_mcuboot_server` - server OTA application using a network connection to update

The `ota_mcuboot_basic` is always supported. Network examples and its variants (ethernet, wifi) depends on target device and current state of the support.

## ROM bootloader examples

A ROM bootloader is a first stage bootloader embedded in a NXP device which offers a lightweight OTA solution with minimum memory footprint. If supported, the ROM bootloader (without the second stage MCUboot) can be evaluated in `ota_rom_basic` example.

The example typically demostrate Dual image feature (ROM utilizing flash remap) and [SB3](https://spsdk-try.readthedocs.io/en/stable/images/secure_update.html) file as an OTA image. More information related to SB3 processing can be found in ['OTA update by using SB3 file'](_doc/sb3_common_readme.md).

## Advanced topics

['MCUboot and flash remapping'](_doc/flash_remap_readme.md)

['MCUboot and Encrypted XIP'](_doc/encrypted_xip_readme.md)

['OTA update by using SB3 file'](_doc/sb3_common_readme.md)