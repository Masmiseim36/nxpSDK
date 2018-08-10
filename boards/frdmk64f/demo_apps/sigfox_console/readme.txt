Overview
========
The purpose of this example project is to show how to control the SIGFOX device with use of the SIGFOX SW driver built on AML (Analog Middleware Layer). It uses the virtual serial console to execute SPI commands of the SIGFOX device.

Hardware requirements
=====================
- FRDM-K64F board
- OM2385/SF001 development kit (A1 firmware)
- Micro USB cable
- Antenna - for example QUINTUS EAD
- Personal Computer

Board settings
============
Target platform for this example is FRDM-K64F and OM2385/SF001. This evaluation board is designed to be pinout compatible with named FRDM-K64F and also with wide range of freedom boards based on Kinetis MCUs. Please refer to the SIGFOX software driver user guide for more details.

Make sure that you set jumpers JP1 and JP2 of the SIGFOX board to correct positions. This setting is crucial because it involves voltage reference selection. This application uses 3V option (1-2 position on JP1 and JP2).

The antenna should be attached to a connector on the small LID2186 board placed on the OM2385/SF001 board.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
In this example is shown how to control the SIGFOX device with use of the SIGFOX SW driver built on AML (Analog Middleware Layer). It enables the user to execute any SPI command supported by the device. The application prints a command list to the console and waits for a user action.

See SIGFOX SW driver documentation for more information (https://www.nxp.com/products/wireless-connectivity/sub-1-ghz-wireless-solutions/embedded-sw-for-sigfox-based-on-ol2385:EMBEDDED-SW-SIGFOX-OL2385-OL2361).

Note: The SIGFOX software driver user guide describes how to register a SIGFOX device on the SIGFOX website.