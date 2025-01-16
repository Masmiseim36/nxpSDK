Hardware requirements
===================
- Mini/micro USB cable
- MIMXRT1040-EVK board
- Personal Computer

Board settings
============
Remove jumper J80.

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
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
CMP interrupt Example
~~~~~~~~~~~~~~~~~~~~~

Then change CMP analog input, and watch the change of LED.
    - CMP1_IN1 (J17-9) connected to VCC =3.3V (J32-4): USER LED on
    - CMP1_IN1 (J17-9) connected to GND (J32-6): USER LED off
