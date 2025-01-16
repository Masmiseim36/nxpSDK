# sdcard_interrupt

## Overview

The SDCARD Interrupt project is a demonstration program that uses the SDK
software. It reads/writes/erases the SD card continusly. The purpose of this
example is to show how to use SDCARD driver andshow how to use interrupt based
transfer API in SDHC driver in SDK software to access SD card.Note: If DATA3 is
used as the card detect PIN, please make sure DATA3 is pull down, no matter
internal or external, at the same time, make sure the card can pull DATA3 up,
then host can detect card through DATA3.And SDHC do not support detect card
through CD by host, card can be detected through DATA3 or GPIO.No matter detect
card through host or gpio, make sure the pinmux configuration is correct.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/sdmmc_examples/sdcard_interrupt/example_board_readme.md)
