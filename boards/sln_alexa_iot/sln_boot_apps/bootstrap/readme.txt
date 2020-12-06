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
1. Connect a USB cable between the host PC and the USB-C port on the target board. 
2. Connect SEGGER J-Link to the 10-pin SWD connector on bottom of target board.
3. Connect USB/UART converter as described above.
4. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5. Download the program to the target board.
6. Either power cycle the board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of this demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BOOTSTRAP v1.0.6 > Booting...
BOOTSTRAP v1.0.6 > Context 0 KIB empty!
BOOTSTRAP v1.0.6 > Context 0 is empty.
BOOTSTRAP v1.0.6 > Context 1 KIB empty!
BOOTSTRAP v1.0.6 > Context 1 is empty.
BOOTSTRAP v1.0.6 > Not running encrypted XIP, no restore.
BOOTSTRAP v1.0.6 > Reading FICA table...
BOOTSTRAP v1.0.6 > Attempting to load bootloader...
BOOTSTRAP v1.0.6 > Attempting to jump to 0x60040481...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Version and jump address may vary.

NOTE: If debugger is attached, a security violation will be present in the log. This is expected when debugging.
Customization options
=====================

