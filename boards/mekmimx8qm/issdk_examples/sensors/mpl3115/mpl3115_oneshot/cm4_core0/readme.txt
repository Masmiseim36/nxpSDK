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
The MPL3115 One-Shot example application demonstrates the use of the MPL3115 sensor
in the One-Shot (on demand sampling) Mode.
The example demonstrates configuration of all registers reguired to put the sensor
into One-Shot Mode and read out a Pressure+Temperature sample.

The sensor reads samples when it is requested by the host then and sets a Flag.
The application then checks the Flag and when set reads out the sample.

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
