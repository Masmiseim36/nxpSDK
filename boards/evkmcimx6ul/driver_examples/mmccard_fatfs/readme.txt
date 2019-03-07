Overview
========
The MMCCARD FATFS project is a demonstration program that uses the SDK software. Tt mounts a file 
system based on a MMC card then does "creat directory/read directory/create file/write file/read 
file" operation. The file sdhc_config.h has default SDHC configuration which can be adjusted to let
card driver has different performance. The purpose of this example is to show how to use MMCCARD 
driver based FATFS disk in SDK software.

Note:
User can use MMC plus card or emmc(on board IC, but not recommand use emmc socket,due to high speed timing restriction)

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK 
- Jlink
- Personal Computer

Board settings
==============
Please use the BB board SD socket with a mmc card insert for this example.
If user want to enable emmc with this example, user should provide an emmc on 
demo board and config the pin mux according to your board config.
Also the macro for mmc in board.h should be changed:
BOARD_MMC_HOST_BASEADDR /* host base address */
BOARD_MMC_HOST_CLK_FREQ /* host clock frequency */
BOARD_MMC_HOST_IRQ		/* host IRQ number */
BOARD_MMC_SUPPORT_8BIT_BUS	/* this is determined by user's board config */
BOARD_MMC_PIN_CONFIG		/* this is determined by user's board config */
BOARD_USDHC_MMCCARD_POWER_CONTROL_INIT/* this is determined by user's board config */


Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Makes file system......The time may be long if the card capacity is big

Creates directory......

Creates a file in that directory......

Lists the file in that directory......
Directory file : ..
Directory file : ...
General file : F_1.DAT.

Writes/reads file until encounters error......

Writes to above created file.
Reads from above created file.
Compares the read/write content.
The read/write content is consistent.

Input 'q' to quit read/write. 
Input other char to read/write file again.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

