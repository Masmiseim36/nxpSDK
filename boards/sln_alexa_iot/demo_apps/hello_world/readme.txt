Overview
========
The Hello World demo application provides a sanity check for the new SDK build environments and board bring up. The Hello
World demo prints the "Hello World" string to the terminal using the SDK UART drivers. The purpose of this demo is to
show how to use the UART, and to provide a simple project for debugging and further development.
Note: Please input one character at a time. If you input too many characters each time, the receiver may overflow
because the low level UART uses simple polling way for receiving. If you want to try inputting many characters each time,
just define DEBUG_CONSOLE_TRANSFER_NON_BLOCKING in your project to use the advanced debug console utility.

Toolchain supported
===================
- MCUXpresso  11.2.0
- GCC ARM Embedded  9.2.1

Hardware requirements
=====================
- USB-C Cable
- SLN-ALEXA-IOT Development Kit
- Personal Computer
- USB/UART converter
- SEGGER J-Link

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Connect the board with PC by using USB/UART converter:
- board uart RX (pin 10 on J26) - connect it to TX pin on converter
- board uart TX (pin 12 on J26) - connect it to RX pin on converter
- board GND (pin 15 on J26)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap' and 'bootloader' are loaded onto target board.
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

