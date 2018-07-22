Overview
========
The SAI Demo application demonstrates digital audio playback. The demo performs audio playback from a .wav
file, stored in on-chip flash. The project also uses the CMSIS-DSP library to perform a Fast Fourier Transform, and to 
return the fundamental frequency of the audio.

Toolchain supported
===================
- IAR embedded Workbench 7.70.1
- Keil MDK 5.20
- GCC ARM Embedded 2015-4.9-q3
- Kinetis Development Studio IDE 3.2.0
- Atollic TrueSTUDIO 5.5.2

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K65F180M board
- Personal Computer
- TWR-SGTL5000
- Elevator Tower
- Headphones

Board settings
==============
Insert all boards into primary elevator tower.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal on PC for JLink serial device with these settings:
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
SAI example finished!
 ~~~~~~~~~~~~~~~~~~~
Customization options
=====================

