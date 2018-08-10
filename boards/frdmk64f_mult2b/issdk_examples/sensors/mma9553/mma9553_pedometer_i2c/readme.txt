Hardware requirements
===================
- Mini/micro USB cable
- FRDMK64F board
- FRDM-FXS-MULT2-B board
- Personal Computer

Demo Overview
============
The MMA9553L Pedometer I2C example application demonstrates the use of the MMA9553L as a Pedometer in I2C Mode.
The example demonstrates all commands reguired to put the sensor into Legacy Mode and read out Pedometer data (and samples).

The Pedometer reads samples as per the configured ODR and updates the output mailbox registers.
The application reads out Pedometer Data as well as samples when its ODR Timer expires.

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

Note: you must rotate and walk with the board to get the samples to change.
