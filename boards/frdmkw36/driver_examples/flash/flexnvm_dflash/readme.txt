Overview
========
The flexnvm_dflash example shows how to use flash driver to operate data flash:



Toolchain supported
===================
- IAR embedded Workbench 8.11.1
- Keil MDK 5.23
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.3.1

Hardware requirements
=====================
- Micro USB cable
- FRDM-KW36 board
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
When the example runs successfully, you can see the similar information from the terminal as below.

1. For parts with FlexNVM:
~~~~~~~~~~~~
 FlexNVM DFlash example Start
 Flash is UNSECURE!
 DFlash Information: 
 Data Flash Base Address: (xx)
 Data Flash Total Size: xx KB, hex: (xx)
 Data Flash Sector Size: xx KB, hex: (xx)
 Erase a sector of Data Flash
 Successfully Erased Sector xx -> xx
 Program a buffer to a sector of Data Flash
 Successfully Programmed and Verified Location xx -> xx


 End of FlexNVM Dflash example
~~~~~~~~~~~~

2. For Some parts Without FlexNVM (such as K65FN):
~~~~~~~~~~~~
 FlexNVM DFlash example Start
 Current device doesn't support FlexNVM feature


 End of FlexNVM Dflash example
~~~~~~~~~~~~
Customization options
=====================

