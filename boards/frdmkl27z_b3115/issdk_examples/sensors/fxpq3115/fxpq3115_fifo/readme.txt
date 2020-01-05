Hardware requirements
===================
- Mini/micro USB cable
- FRDMKL27Z board
- FRDM-STBC-B3115 board
- Personal Computer

Demo Overview
============
The FXPQ3115 FIFO example application demonstrates the use of the FXPQ3115 sensor
in Buffered (FIFO) Mode.
The example demonstrates configuration of all registers reguired to put the sensor
in FIFO Mode and read out Pressure+Temperature samples.

The sensor reads samples as per the configured Auto acquisition time step and are
buffered upto the configured Water Mark Level and then a Flag is set.
The application consistently checks the Flag and when set, reads out all
(count=Water Mark Level) samples.

Prepare the Demo
===============
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
===============
When the demo runs successfully, you can see the Samples printed to the terminal.
