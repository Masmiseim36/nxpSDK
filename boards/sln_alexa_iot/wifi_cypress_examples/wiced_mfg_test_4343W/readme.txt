Overview
========
Manufacturing Test Application

The manufacturing test application is used to test the radio performance of the DUT (device under testing)
and to assist with regulatory certification. The application works in conjunction with a Cypress supplied
wireless LAN manufacturing test utility, known as 'wl', which can be found in this path:
    middleware\wiced\43xxx_Wi-Fi\libraries\test\wl_tool\

The test application reads packetized IOCTL commands from the PC via a serial UART and forwards the commands
to the Cypress Wi-Fi device on the WICED module. Responses to the IOCTLs from the Wi-Fi device are returned
over the UART to the PC.

More details about this Test Application can be found in WICED Studio from Cypress https://www.cypress.com/products/wiced-software



Toolchain supported
===================
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

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
Customization options
=====================

