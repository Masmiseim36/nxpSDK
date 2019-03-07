Overview
========
The SAI Demo application demonstrates digital audio playback/record into sdacrd based on FatFs and freertos.
The demo can support three types of command:
Playback means playback a wav file located in sdcard.
Record means Record a 10s audio data to sdcard.
Duplex means record and playback at the same time.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- 5V power supply
- Personal Computer
- Headphone of iphone (Have mic)
- Sdcard

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 5V power supply to the board.
2.  Connect two USB cables from the host PC to the J1901 and the J1102 USB port on the target board.
3.  Connect Headphone to the J1401 port on the target board
4.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5.  Use MFGTool to download the program to the target board and run it. SAI module reused several pins 
    of JTAG, so it could not be downloaded and run by using IAR IDE. 

Running the demo
================
When the demo runs successfully, you can input some command to make the demo record, playback or duplex.
You can choose to format or not format sdcard before input command.

~~~~~~~~~~~~~~~~~~~
Command including like below:
playback <filepath>Playback the wav file, make sure the path existed.
record <filepath> Record a 10s audio data, save to the file path in sdcard as a wav file
duplex Playback the data recorded by sai now
 ~~~~~~~~~~~~~~~~~~~
Customization options
=====================

