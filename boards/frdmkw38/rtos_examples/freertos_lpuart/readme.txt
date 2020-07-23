Overview
========
The LPUART example for FreeRTOS demonstrates the possibility to use the LPUART driver in the RTOS.
The example uses single instance of LPUART IP and writes string into, then reads back chars.
After every 4B received, these are sent back on LPUART.

Toolchain supported
===================
- IAR embedded Workbench  8.32.4
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini USB cable
- USB-to-Serial connector
- FRDM-KW38 board
- Personal Computer

Board settings
==============
The freertos_lpuart example is configured to use LPUART0 with PTC6 and PTC7 pins. 
To observe the execution log on serial port, the USB-to-Serial connector should be connected to PTC6, PTC7 on
FRDM-KW38 board and open the lpuart COM port in PC.

Prepare the Demo
================
1.  Connect a USB-to-Serial connector between the PC host and the LPUART1 pins on the board.
2.  Open a serial terminal on PC with these settings:
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

