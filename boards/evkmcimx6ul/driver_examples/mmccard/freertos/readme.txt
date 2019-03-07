Overview
========
The MMCCARD FreeRTOS project is a demonstration program that uses the SDK software. It reads/writes
/erases the MMC card continusly. The purpose of this example is to show how to use MMCCARD driver
with FreeRTOS in SDK software to access MMC card.

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

Card user partition size xxxx bytes

Working condition:

  Voltage : xxxxV

  Timing mode: xxxx

  Freq : xxxx HZ

Read/Write the card continuously until encounter error....

Write/read one data block......
Compare the read/write content......
The read/write content is consistent.
Write/read multiple data blocks......
Compare the read/write content......
The read/write content is consistent.
Erase data groups......

Input 'q' to quit read/write/erase process.
Input other char to read/write/erase data blocks again.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

