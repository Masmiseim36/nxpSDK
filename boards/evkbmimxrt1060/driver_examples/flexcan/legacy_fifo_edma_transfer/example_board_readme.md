Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVKB board
- Personal Computer

Board settings
==============
The example requires 2 sets of boards, each of them are mounted with the base board. Using a male to male CAN
cable to connect the CAN3 instance (J42) between the 2 base boards.

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


FlexCAN Legacy Rx FIFO edma example.
Board to board mode.
Node B Legacy Rx FIFO used for Rx.
Node A Message buffer 9 used for Tx.
Please select local node as A or B:
Note: Node B should start first.
Node:a
Press any key to trigger 6 transmission.

Send Msg0: word0 = 0x0, word1 = 0x55, id = 0x123.
Send Msg1: word0 = 0x1, word1 = 0x55, id = 0x124.
Send Msg2: word0 = 0x2, word1 = 0x55, id = 0x125.
Send Msg3: word0 = 0x3, word1 = 0x55, id = 0x126.
Send Msg4: word0 = 0x4, word1 = 0x55, id = 0x127.
Send Msg5: word0 = 0x5, word1 = 0x55, id = 0x128.

Press any key to trigger 6 transmission.
~~~~~~~~~~~~~~~~~~~~~

This message displays on the node B terminal:

FlexCAN Legacy Rx FIFO edma example.
Board to board mode.
Node B Legacy Rx FIFO used for Rx.
Node A Message buffer 9 used for Tx.
Please select local node as A or B:
Note: Node B should start first.
Node:b
Start to Wait data from Node A

Receive Msg0: word0 = 0x0, word1 = 0x55, ID Filter Hit: 0, Time stamp: 14162.
Receive Msg1: word0 = 0x1, word1 = 0x55, ID Filter Hit: 1, Time stamp: 16719.
Receive Msg2: word0 = 0x2, word1 = 0x55, ID Filter Hit: 2, Time stamp: 19277.
Receive Msg3: word0 = 0x3, word1 = 0x55, ID Filter Hit: 3, Time stamp: 21831.
Receive Msg4: word0 = 0x4, word1 = 0x55, ID Filter Hit: 4, Time stamp: 24385.
Receive Msg5: word0 = 0x5, word1 = 0x55, ID Filter Hit: 5, Time stamp: 26944.

Wait for the next 6 messages!
~~~~~~~~~~~~~~~~~~~~~

Note:
This case will be blocked when transferring data on armgcc debug/sdram_debug target. This issue has been fixed in later release by open ERR005829 feature.
