Hardware requirements
===================
- Mini/micro USB cable
- i.MX8QM MEK Board
- MCIMX8-8X-BB
- J-Link Debug Probe
- 12V power supply
- Personal Computer
Demo Overview
============
The ISL29023 Poll example application demonstrates the use of the ISL29023 in ALS continuous mode.
The example demonstrates configuration of all registers reguired to put the sensor into ISL29023 16-bits ALS continuous mode and read out ambient light samples.

The application consistently checks the IRQ flag bit in the COMMAND-I register and when set reads out the sample.

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
When the demo runs successfully, you can see the ambient light samples printed to the terminal.
