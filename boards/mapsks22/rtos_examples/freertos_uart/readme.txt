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
- J-Link ARM
- Mini USB cable
- USB-to-Serial converter
- MAPS-KS22F256 board
- Personal Computer

Board settings
==============
The freertos_uart example is configured to use UART2 with PTD2 and PTD3 pins. 
To observe the execution log on serial port, the USB-to-Serial connector should be connected to CN2 port on
MAPS-KS22F256 board and open the RS232 COM port in PC.
PTD2(Pin-95)   <--> USB-to-Serial(TX end)
PTD3(Pin-96)   <--> USB-to-Serial(RX end)
MAPS-Board_GND <--> USB-to_Serial(GND end)

Prepare the Demo
================
1. Connect USB-to-Serial converter between host PC and CN2:5 (TX) and CN2:18 (RX)
2. Open a serial terminal on PC for the conencted USB-to-Serial converter device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo. For detailed instructions, see Getting Started with Software Development Kit for 
   KS22F25612 (document SDKKS22F256GSUG).

Running the demo
================
You will see the welcome string printed out on the console.
You can send characters to the console back and they will be printed out onto console in a group of 4 characters.

Customization options
=====================

