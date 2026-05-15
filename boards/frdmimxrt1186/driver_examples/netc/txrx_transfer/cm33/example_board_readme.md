Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT1186 board
- Personal Computer

Board settings
============
J18(1-2)/J17(2-3): ETH2 Function Via J56B.
J12(1-2)/J13(2-3): ETH0 Function Via J56A.

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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NETC Switch frame loopback example start.
Wait for PHY link up...
The 1 frame transmitted success on port 0!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 2 frame transmitted success on port 2!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
......
The 19 frame transmitted success on port 0!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00
The 20 frame transmitted success on port 2!
 A frame received. The length is 1000  Dest Address ff:ff:ff:ff:ff:ff Src Address 54:27:8d:00:00:00

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
