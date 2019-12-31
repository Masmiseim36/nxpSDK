Overview
========
The flash_swap example shows how to use flash swap feature:


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW24 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

 PFLASH Swap example Start
 PFlash Information: 
 Total Program Flash Size: xx KB, Hex: (xx)
 Total Program Flash Block Count: xx
 Program Flash Sector Size: xx KB, hex: (xx)
 Flash is xx
 Original swap indicator address: xx
 Current swap system status: xx
 Validating the backup example image...
 Backup example image is invalid 
 Start to program backup example image
 Finish programming backup example image
 Checking the system status...
 It is first swap for the system
 Start to erase test data on lower pflash before swapping system
 Finish erasing test data
 Start to program backup test data on upper pflash
 Finish programming backup test data
 Start to swap the system

 Perform a system reset

 FLASH Swap example Start
 PFlash Information: 
 Total Program Flash Size: xx KB, Hex: (xx)
 Total Program Flash Block Count: xx
 Program Flash Sector Size: xx KB, hex: (xx)
 Flash is xx
 Original swap indicator address: xx
 Current swap system status: xx
 Validating the backup example image...
 Backup example image is valid
 Checking the system status...
 The system has been swapped successfully
 Start to erase test data on lower pflash before exiting this example
 Finish erasing test data

 End of PFLASH Swap example
Customization options
=====================

