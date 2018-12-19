Overview
========
The CRC Example project is a demonstration program that uses the KSDK software to generate checksums
for an ASCII string. Several CRC protocols are implemented using the CRC driver API.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~
CRC Peripheral Driver Example

Test string: 123456789
CRC-16 CCIT FALSE: 0x29b1
CRC-16 MAXIM: 0x44c2
CRC-16 KERMIT: 0x2189
CRC-32: 0xcbf43926
CRC-32 POSIX: 0x765e7680
~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Mini/micro USB cable
- Frdmkv31f board
- Personal Computer

Board settings
==============
The CRC demo does not call for any special hardware configurations.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB on frdmkv31f board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

