Overview
========
This document explains the freertos_tickless example. It shows how the CPU enters the sleep mode and then
it is woken up either by expired time delay using low power timer module or by external interrupt caused by a
user defined button.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- J-Link ARM
- Mini USB cable
- Twrk64f120m board
- Personal Computer

Board settings
==============
The hello world demo is configured to use UART1 with PTC4 and PTC3 pins. To make OpenSDA serial 
device work, the jumpers should be set as following:
- J10: 2-3 connected
- J15: 2-3 connected

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port (J2) on twrk64f120m board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo. For detailed instructions, see Getting Started with Software Development Kit for 
   K64F (document SDKK64FGSUG).

Running the demo
================
When the demo runs successfully, "Hello World!" can be seen on the OpenSDA terminal.

Customization options
=====================
1. If TWR-SER board was connected to the tower system with twrk64f120m, we can switch the serial
   output to TWR-SER as long as following settings are applied:
   - On twrk64f120m board
	   J10: 1-2 connected
	   J15: 1-2 connected
   - Connect the UART cable between PC and TWR-SER (J?)
   - Open a serial terminal on PC for TWR-SER serial device instead of OpenSDA with same settings

2. <FIXME>The USE_STDIO_FUNCTIONS definition determines whether the demo uses standard I/O
   functions, such as printf. If it is not defined, then the demo accesses the UART driver directly.
