Overview
========
The sai_interrupt example shows how to use sai functional API to implement interrupt playback:

In this example, one sai instance playbacks the audio data stored in flash/SRAM using interrupt.

Toolchain supported
===================
- IAR embedded Workbench  8.50.1
- Keil MDK  5.30
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
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
4. Insert the headphones into the headphone jack on EVKB-IMXRT1050 board (J12).
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:
~~~~~~~~~~~~~~~~~~~
SAI functional interrupt example started!

SAI functional interrupt example finished!
 ~~~~~~~~~~~~~~~~~~~
Customization options
=====================

