Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer

Board settings
==============
The example requires 2 sets of boards, each of them are mounted with the base board. Using a male to male CAN
cable to connect the CAN3 instance (J35) between the 2 base boards.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the example.

Running the demo
================
After connecting the two boards, these instructions display on each terminal window.
One board must be chosen as node A and the other board as node B. (Note: Node B should start first)
Data is sent continuously between the node A and the node B.

~~~~~~~~~~~~~~~~~~~~~
This message displays on the node A terminal:

********* FLEXCAN Interrupt EXAMPLE *********
    Message format: Standard (11 bit id)
    Message buffer 9 used for Rx.
    Message buffer 8 used for Tx.
    Interrupt Mode: Enabled
    Operation Mode: TX and RX --> Normal
*********************************************

Please select local node as A or B:
Note: Node B should start first.
Node:A
Press any key to trigger one-shot transmission

Rx MB ID: 0x123, Rx MB data: 0x0, Time stamp: 21747
Press any key to trigger the next transmission!

Rx MB ID: 0x123, Rx MB data: 0x1, Time stamp: 20717
Press any key to trigger the next transmission!
~~~~~~~~~~~~~~~~~~~~~

This message displays on the node B terminal:

********* FLEXCAN Interrupt EXAMPLE *********
    Message format: Standard (11 bit id)
    Message buffer 9 used for Rx.
    Message buffer 8 used for Tx.
    Interrupt Mode: Enabled
    Operation Mode: TX and RX --> Normal
*********************************************

Please select local node as A or B:
Note: Node B should start first.
Node:B
Start to Wait data from Node A

Rx MB ID: 0x321, Rx MB data: 0x0, Time stamp: 46379
Wait Node A to trigger the next transmission!

Rx MB ID: 0x321, Rx MB data: 0x1, Time stamp: 45372
Wait Node A to trigger the next transmission!
~~~~~~~~~~~~~~~~~~~~~
