Overview
========
The EEPROM flash demo application demonstrates the use of the LPI2C IP and driver to read and write data from an
external EEPROM device.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- MAPS-KS22F256 board & MAPS-DOCK board
- Personal Computer

Board settings
==============
The eeprom demo is configured to use UART1 with PTE0 and PTE1 pins. To make JLink serial
device work, the jumpers on MAPS-DOCK board should be set as following:
- JP7 on MAPS-Dock: UART0 related jumpers connected

Connections on MAPS-KS22F256 board need to be setup as follows:
- JP13 Disonnected
- JP14 Disonnected

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EEPROM Demo
This demo will write data into on board EEPROM and then read it back.

Write data at 0x0 is 0xff, read back data is 0xff
Write data at 0x1 is 0xfe, read back data is 0xfe
Write data at 0x2 is 0xfd, read back data is 0xfd
Write data at 0x3 is 0xfc, read back data is 0xfc
Write data at 0x4 is 0xfb, read back data is 0xfb
Write data at 0x5 is 0xfa, read back data is 0xfa
Write data at 0x6 is 0xf9, read back data is 0xf9
Write data at 0x7 is 0xf8, read back data is 0xf8
.............

End of the demo.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

