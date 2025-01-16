Hardware requirements
===================
- Mini/micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer

Board settings
============
No special settings are required.

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

Note
- This demo will access GPIO DR_SET/DR_CLEAR registers which is only available on A1 silicon (MIMXRT106xxxxxA).

Running the demo
===============
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
CMP polling Example
~~~~~~~~~~~~~~~~~~~~~

Then change CMP analog input, and watch the change of LED.
    - CMP1_IN0 (J23-5) connected to VCC =3.3V (J24-8): USER LED on
    - CMP1_IN0 (J23-5) connected to GND (J24-7): USER LED off
