Overview
========


Toolchain supported
===================
- MCUXpresso  11.2.0
- GCC ARM Embedded  9.2.1

Hardware requirements
=====================
- USB-C Cable
- SLN-ALEXA-IOT Development Kit
- Personal Computer
- SEGGER J-Link
- USB/UART converter

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Connect the board with PC by using USB/UART converter:
- board uart RX (pin 10 on J26) - connect it to TX pin on converter
- board uart TX (pin 12 on J26) - connect it to RX pin on converter
- board GND (pin 15 on J26)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap' is loaded onto target board.
2. Connect a USB cable between the host PC and the USB-C port on the target board. 
3. Connect SEGGER J-Link to the 10-pin SWD connector on bottom of target board.
4. Connect USB/UART converter as described above.
5. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
6. Download the program to the target board.
7. Either power cycle the board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of this demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*** BOOTLOADER v1.0.6 ***

0 1222 [BOOTLOADER_Task] Context 1 KIB empty!
1 1222 [BOOTLOADER_Task] Context check status -7
2 1222 [BOOTLOADER_Task] Ensuring context...
3 1222 [BOOTLOADER_Task] Failed to load crypto context...
4 1222 [BOOTLOADER_Task] Not running in encrypted XIP mode, ignore error.
5 1222 [BOOTLOADER_Task] [FICA] VERSION 2
6 1222 [BOOTLOADER_Task] [FICA] RT HYPERFLASH Check...
7 1222 [BOOTLOADER_Task] [FICA] Found Flash device!
8 1222 [BOOTLOADER_Task] [FICA] Checking Image Config Area (ICA) initialization
9 1222 [BOOTLOADER_Task] [FICA] Flash ICA already initialized
10 1222 [BOOTLOADER_Task] [FICA] Flash ICA initialization complete
11 1222 [BOOTLOADER_Task] Jumping to main application...
12 1222 [BOOTLOADER_Task] [FICA] VERSION 2
13 1222 [BOOTLOADER_Task] [FICA] RT HYPERFLASH Check...
14 1223 [BOOTLOADER_Task] [FICA] Found Flash device!
15 1223 [BOOTLOADER_Task] [FICA] Checking Image Config Area (ICA) initialization
16 1223 [BOOTLOADER_Task] [FICA] Flash ICA already initialized
17 1223 [BOOTLOADER_Task] [FICA] Flash ICA initialization complete
18 1223 [BOOTLOADER_Task] [FICA] VERSION 2
19 1223 [BOOTLOADER_Task] [FICA] RT HYPERFLASH Check...
20 1223 [BOOTLOADER_Task] [FICA] Found Flash device!
21 1223 [BOOTLOADER_Task] [FICA] Checking Image Config Area (ICA) initialization
22 1223 [BOOTLOADER_Task] [FICA] Flash ICA already initialized
23 1223 [BOOTLOADER_Task] [FICA] Flash ICA initialization complete
24 1321 [BOOTLOADER_Task] [FICA] VERSION 2
25 1321 [BOOTLOADER_Task] [FICA] RT HYPERFLASH Check...
26 1322 [BOOTLOADER_Task] [FICA] Found Flash device!
27 1322 [BOOTLOADER_Task] [FICA] Checking Image Config Area (ICA) initialization
28 1322 [BOOTLOADER_Task] [FICA] Flash ICA already initialized
29 1322 [BOOTLOADER_Task] [FICA] Flash ICA initialization complete
30 1322 [BOOTLOADER_Task] Launching into application at 0x300000...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Version and jump address may vary.

NOTE: Unless running with encrypted XIP, the Crypto context will fail as shown above.
Customization options
=====================

