Overview
========
The sai_edma_transfer example shows how to use sai driver with EDMA:

In this example, one sai instance playbacks the audio data stored in flash/SRAM using EDMA channel.

Toolchain supported
===================
- Keil MDK 5.25
- IAR embedded Workbench 8.30.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
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
4. Insert the headphones into the headphone jack on EVK-MIMXRT1060 board (J12).
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~
SAI example started!
SAI EDMA example finished!
 ~~~~~~~~~~~~~~~~~~~
Customization options
=====================

