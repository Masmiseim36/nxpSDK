Overview
========
The SPI flash demo application demonstrates the use of the SPI peripheral and driver to erase, program, and read an
external flash device.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The spi_flash demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the USB port (CN14) on MAPS-DOCK board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the demo runs successfully, the following message will be seen on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
***SPI Flash Demo***

1 - Erase entire chip
2 - Erase sectors
3 - Erase block
4 - Program one page with pattern (0x5a) and verify
5 - Read byte
Please enter your choice (1-5):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Enter number 1-5, follow the tips  and "successfully"  wording will be on the terminal if the demo runs successfully.
Customization options
=====================

