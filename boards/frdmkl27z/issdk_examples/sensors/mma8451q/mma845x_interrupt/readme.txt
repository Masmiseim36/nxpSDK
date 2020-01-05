Hardware requirements
===================
- Mini/micro USB cable
- FRDMKL27Z board
- Personal Computer

Demo Overview
============
The MMA8451Q FIFO example application demonstrates the use of the FRDMKL27Z on-board
MMA8451Q sensor in interrupt mode.
The example demonstrates configuration of all registers required to put the sensor in a interrupt mode and read out a sample.

The application waits for the interrupt signal and read the sample when interrupt is asserted.

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
