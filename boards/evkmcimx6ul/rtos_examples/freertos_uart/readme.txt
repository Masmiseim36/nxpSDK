Overview
========
The UART example for FreeRTOS demonstrates the possibility to use the UART driver in the RTOS.
The example uses single instance of UART IP and writes string into, then reads back chars.
After every 4B received, these are sent back on UART.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special settings are required.

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
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
You will see the welcome string printed out on the console.
You can send characters to the console back and they will be printed out onto console in a group of 4 characters.

Customization options
=====================
