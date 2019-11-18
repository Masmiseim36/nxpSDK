Hardware requirements
===================
- Mini/micro USB cable
- MEKMIMX8QM board
- Personal Computer

Board settings
==============

Demo Overview
============
The FXAS21002 Normal example application demonstrates the use of the on-board FXAS21002 sensor in the Poll (Non-Buffered) Mode.
The example demonstrates configuration of all registers reguired to put the sensor into Standard Mode and read out a sample.

The sensor reads samples as per the configured ODR and sets a Flag.
The application consistently checks the Flag and when set reads out the sample.s.

Prepare the Demo
===============
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a USB cable between the host PC and the Debug port on the board (Refer "Getting Started with MCUXpresso SDK for i.MX 8QuadMax.pdf" for debug port information).
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board (Please refer "Getting Started with MCUXpresso SDK for i.MX 8QuadMax.pdf" for how to run different targets).
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
===============
When the demo runs successfully, you can see the Samples printed to the terminal.

Note: you must rotate the board to get the samples to change.
