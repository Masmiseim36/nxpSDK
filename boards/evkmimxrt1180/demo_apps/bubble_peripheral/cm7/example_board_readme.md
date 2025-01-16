Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
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
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
===============

When the example runs successfully, you can see the similar
information from the terminal as shown below.

~~~~~~~~~~~~~~~~~~~~~
Welcome to the BUBBLE example

You will see the change of LED brightness when change angles of board

x=  6 y = 22
x=  8 y = 26
x= 10 y = 28
x= 10 y = 28
x= 11 y = 29
x= 11 y = 29
~~~~~~~~~~~~~~~~~~~~~

Note:
- If the user LED flashes constantly and the following message is printed on the console:
    Sensor device initialize failed!

    Please check the sensor chip U115

  please check if the combo sensor (U115) is presented on the board.
