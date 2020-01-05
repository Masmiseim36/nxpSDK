Overview
========
The sai_edma_record_playback example shows how to use sai driver with EDMA:

In this example, one sai instance record the audio data from input and playbacks the audio data.
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL28Z72M board
- Personal Computer
- TWR-SGTL5000
- Elevator Tower
- Headphone
- Line-in line

Board settings
==============
Insert all boards into Primary Elevator Tower. Connect J7-SGTL5000 to headphone

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
4. Insert the headphones into the headphone jack on the TWR-SGTL5000 board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~
SAI example started!
 ~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

