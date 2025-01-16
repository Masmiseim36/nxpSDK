# mmccard_fatfs

## Overview

The MMCCARD FATFS project is a demonstration program that uses the SDK software.
Tt mounts a file system based on a MMC card then does "creat directory/read
directory/create file/write file/read file" operation. The file sdhc_config.h
has default SDHC configuration which can be adjusted to letcard driver has
different performance. The purpose of this example is to show how to use MMCCARD
driver based FATFS disk in SDK software.Note:User can use MMC plus card or
emmc(on board IC, but not recommand use emmc socket,due to high speed timing
restriction)

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/sdmmc_examples/mmccard_fatfs/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/sdmmc_examples/mmccard_fatfs/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/sdmmc_examples/mmccard_fatfs/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/sdmmc_examples/mmccard_fatfs/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/sdmmc_examples/mmccard_fatfs/example_board_readme.md)
