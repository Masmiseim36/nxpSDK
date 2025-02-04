Overview
========
The sai_interrupt_transfer example shows how to use sai driver with interrupt:

In this example, one sai instance playbacks the audio data stored in flash/SRAM using interrupt.

SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- IAR embedded Workbench  9.60.1
- Keil MDK  5.39.0
- MCUXpresso  11.10.0
- GCC ARM Embedded  13.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVK board
- Personal Computer
- Headphone(OMTP standard)

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
4. Insert the headphones into the headphone jack on MIMXRT1170-EVK board (J33).
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
Note: This demo outputs 215HZ sine wave audio signal.
When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~
SAI example started!
SAI example finished!
 ~~~~~~~~~~~~~~~~~~~
