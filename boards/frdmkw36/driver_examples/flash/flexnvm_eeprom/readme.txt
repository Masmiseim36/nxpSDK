Overview
========
The flexnvm_eeprom example shows how to use flash driver to operate eeprom:



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
 FlexNVM EEprom Example Start
 Flash is UNSECURE!
 EEprom Information: 
 EEprom Base Address: (0xxx)
 EEprom Total Size: xx B
 Make FlexRAM available for EEPROM
 Now EEPROM data is read and written by accessing the FlexRAM address space
 FlexRAM Base Address: (0xxx)
 Read 16 bytes data from start of EEPROM space
 The first 16 bytes data in EEPROM are not all 0x00s
 Program the first 16 bytes memory space of EEprom as 0x00s
 Successfully Programmed Location xx -> xx
 Perform a system reset

 FlexNVM EEprom Example Start
 Flash is UNSECURE!
 EEprom Information: 
 EEprom Base Address: (0xxx)
 EEprom Total Size: xx B
 Make FlexRAM available for EEPROM
 Now EEPROM data is read and written by accessing the FlexRAM address space
 FlexRAM Base Address: (0xxx)
 Read 16 bytes data from start of EEPROM space
 The first 16 bytes data in EEPROM are all 0x00s
 Program a buffer(16 bytes) into the first 16 bytes memory space of EEprom
 Successfully Programmed Location xx -> xx
 Perform a system reset

 FlexNVM EEprom Example Start
 Flash is UNSECURE!
 EEprom Information: 
 EEprom Base Address: (0xxx)
 EEprom Total Size: xx B
 Make FlexRAM available for EEPROM
 Now EEPROM data is read and written by accessing the FlexRAM address space
 FlexRAM Base Address: (0xxx)
 Read 16 bytes data from start of EEPROM space
 The first 16 bytes data in EEPROM are what we have progrommed before
 Recover the first 16 bytes memory space of EEprom as 0xFFs

 End of FlexNVM EEprom example
~~~~~~~~~~~~

2. For Some parts Without FlexNVM (such as K65FN):
~~~~~~~~~~~~
 FlexNVM EEprom example Start
 Current device doesn't support FlexNVM feature


 End of FlexNVM EEprom example
~~~~~~~~~~~~
Customization options
=====================

