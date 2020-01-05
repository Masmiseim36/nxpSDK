Overview
========
The flexnvm_eeprom example shows how to use flash driver to operate eeprom:



Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- HVP-KE18F board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
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
When the example runs successfully, the following message is displayed in the terminal

~~~~~~~~~~~~
 FlexNVM EEprom Example Start 

 Flash is UNSECURE!

 There is no available EEprom (FlexNVM) on this Device by default.
 Example is trying to configure FlexNVM block as EEprom.
 Perform a system reset 
 FlexNVM EEprom Example Start 

 Flash is UNSECURE!

 EEprom Information: 
 EEprom Base Address: (0x10008000) 
 EEprom Total Size:	1 KB, Hex: (0x400)
 Program a buffer to a sector of EEprom
 Successfully Programmed Location 0x14000000 -> 0x14000010 

 End of FlexNVM EEprom Example 
~~~~~~~~~~~~
Customization options
=====================

