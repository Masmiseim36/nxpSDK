Overview
========
The flash_swap example shows how to use flash swap feature:


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K65F180M board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Unlock target device to make sure the flash swap system is uninitialized.
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~
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
~~~~~~~~~~~~
Customization options
=====================

