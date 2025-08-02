# MCUboot and flash remapping feature

The default upgrade mechanism in MCUBoot is the SWAP-USING-MOVE algorithm. Several NXP processors support flash remapping functionality that can be used to speed up the OTA update process and minimize flash wear. This flash remapping feature enables zero-copy image swapping without requiring any data moving operations, resulting in the fastest possible update operation. Multiple platforms support this mechanism, primarily those based on their external memory FlexSPI controller (e.g., RT1060, RW612). Platforms like MCX N9 also implement this mechanism for their internal flash memory.

Development boards with these processors include example projects configured to use this feature. This is achieved by using MCUboot's [DIRECT-XIP](https://docs.mcuboot.com/design.html#direct-xip) mechanism and by activating flash remapping when an active flag is located in the secondary slot - the image is still built to run from the primary slot. Keep in mind that DIRECT-XIP mode loads the image with the highest version, as there is no rollback support.

**IMPORTANT NOTE:**
Signed application images directly programmed into flash memory by a programmer require an additional "--pad --confirm" parameter for [imgtool](https://docs.mcuboot.com/imgtool.html). This parameter adds an additional trailer to the initial signed image and is required by the bootloader's DIRECT-XIP process. Signed images used in the OTA process do not require the "--pad" parameter.

## Flash remap mechanisms in NXP devices

There are currently two flash remapping mechanisms:
1. Flash remap using __OVERLAY__
    * Active region overlays inactive region
    * __Inactive region is logically not accessible for read__
    * For flash reads from overlayed region, users must manually use a peripheral driver of the memory device
    * OTA process has to download the OTA image into primary or secondary slot depending on the active flag location
2. Flash remap using __SWAP__
    * Active region swaps address range with inactive region
    * __Both regions are logically accessible__
    * OTA process always downloads the OTA image into secondary slot due to SWAP mechanism design (even if flash remap is active)

## Supported Boards

__Flash remap using overlay__

- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/ota_examples/mcuboot_opensource/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/ota_examples/mcuboot_opensource/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/ota_examples/mcuboot_opensource/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MIMXRT700EVK](../../_boards/mimxrt700evk/ota_examples/mcuboot_opensource/example_board_readme.md)

__Flash remap using swap__

- [FRDM-MCXN947](../../_boards/frdmmcxn947/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/ota_examples/mcuboot_opensource/example_board_readme.md)