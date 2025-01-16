Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1015 board
- Personal Computer
- Headphone(OMTP standard)

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
4. Insert the headphones into the headphone jack on EVK-MIMXRT1015 board (J11).
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
Note: This demo outputs 215HZ sine wave audio signal.
When the demo runs successfully, you can hear the tone and the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~
SAI EDMA Ping Pong Buffer Half Interrupt example started!
SAI EDMA Ping Pong Buffer Half Interrupt example finished!
 ~~~~~~~~~~~~~~~~~~~
