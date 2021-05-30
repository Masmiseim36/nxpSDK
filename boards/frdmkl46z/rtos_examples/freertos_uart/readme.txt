Overview
========
The UART example for FreeRTOS demonstrates the possibility to use the UART driver in the RTOS.
The example uses single instance of UART IP and writes string into, then reads back chars.
After every 4B received, these are sent back on UART.

Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini USB cable
- FRDM-KL46Z board
- Personal Computer
- USB to Com Converter

Board settings
==============
USB2COM                UART1
Pin Name               Pin Name  Board Location
RX                     PTE0      J2 pin 18
TX                     PTE1      J2 pin 20
GND                    GND       J2 pin 14

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
You will see the welcome string printed out on the console.
You can send characters to the console back and they will be printed out onto console in a group of 4 characters.
Customization options
=====================

