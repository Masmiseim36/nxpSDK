# fatfs_ramdisk_freertos

## Overview

The fatfs_ramdisk_freertos project is showing creation and usage of RAM allocated filesystem
in multitasking environment.


## Running the demo

After the board is flashed the Tera Term will print message on terminal.

Example output:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FatFs RAM disk FreeRTOS example.

READ_TASK created.
READ_TASK: Create an FAT volume on the RAM disk.
READ_TASK: Creating text file.
READ_TASK: File created.
READ_TASK: WRITE_TASK 0 created.
READ_TASK: WRITE_TASK 1 created.
READ_TASK: WRITE_TASK 2 created.
READ_TASK: Opening file for read.
READ_TASK: File opened.

READ_TASK: Closing file.

WRITE_TASK: 0: Opening for write.
WRITE_TASK: 0: File opened.
WRITE_TASK 0: Writing to text file.
AAAAA
WRITE_TASK 0: Writing finished.
WRITE_TASK 0: Closing file.

WRITE_TASK: 1: Opening for write.
WRITE_TASK: 1: File opened.
WRITE_TASK 1: Writing to text file.
BBBBB
WRITE_TASK 1: Writing finished.
WRITE_TASK 1: Closing file.

WRITE_TASK: 2: Opening for write.
WRITE_TASK: 2: File opened.
WRITE_TASK 2: Writing to text file.
CCCCC
WRITE_TASK 2: Writing finished.
WRITE_TASK 2: Closing file.

READ_TASK: Opening file for read.
READ_TASK: File opened.
AAAAABBBBBCCCCC
READ_TASK: Closing file.

WRITE_TASK: 0: Opening for write.
WRITE_TASK: 0: File opened.
WRITE_TASK 0: Writing to text file.
AAAAA
WRITE_TASK 0: Writing finished.
WRITE_TASK 0: Closing file.

WRITE_TASK: 1: Opening for write.
WRITE_TASK: 1: File opened.
WRITE_TASK 1: Writing to text file.
BBBBB
WRITE_TASK 1: Writing finished.
WRITE_TASK 1: Closing file.

WRITE_TASK: 2: Opening for write.
WRITE_TASK: 2: File opened.
WRITE_TASK 2: Writing to text file.
CCCCC
WRITE_TASK 2: Writing finished.
WRITE_TASK 2: Closing file.

READ_TASK: Opening file for read.
READ_TASK: File opened.
AAAAABBBBBCCCCCAAAAABBBBBCCCCC
READ_TASK: Closing file.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/fatfs_examples/fatfs_ramdisk_freertos/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/fatfs_examples/fatfs_ramdisk_freertos/example_board_readme.md)
