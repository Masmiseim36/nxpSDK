Overview
========
The LED Blinky demo application provides a sanity check for the new SDK build environments and board bring up. The LED Blinky demo 
uses the systick interrupt to realize the function of timing delay. The example takes turns to shine the LED. The purpose of this 
demo is to provide a simple project for debugging and further development.
The code of this demo has been prepared and updated for use with the MCUXpresso Configuration Tools (Pins/Clocks/Peripherals).

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

Board settings
==============
Ensure J27 is set to position '1' (and is not set to serial downloader mode)

Prepare the Demo
================
1. Make sure that a valid 'bootstrap' and 'bootloader' are loaded onto target board.
2. Connect a USB cable between the host PC and the USB-C port on the target board. 
3. Connect SEGGER J-Link to the 10-pin SWD connector on bottom of target board.
4. Download the program to the target board.
5. Either power cycle the board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, you will find the LED is blinking.
Customization options
=====================

