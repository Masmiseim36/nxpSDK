Overview
========
This project shows how to use the MCUXpresso Config Tools TEE tool. This project
defines two domains, one memory region is not added to untrusted domain. User can
input option to access the memory region from untrusted domain, this results to
the fault.

Toolchain supported
===================
- GCC ARM Embedded  10.3.1
- Keil MDK  5.37
- IAR embedded Workbench  9.32.1
- MCUXpresso  11.7.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
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
Follow the instruction in debug console and input the option to trigger fault.
After each fault, reset and rerun.
