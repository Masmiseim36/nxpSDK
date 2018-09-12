Overview
========
The flexcan_interrupt example shows how to use FlexCAN driver in none-blocking interrupt way:

In this example, 2 boards are connected through CAN bus. Endpoint A(board A) send a CAN Message to
Endpoint B(board B) when user press space key in terminal. Endpoint B receive the message, print
the message content to terminal and echo back the message. Endpoint A will increase the received
message and waiting for the next transmission of the user initiated.

Toolchain supported
===================
- Keil MDK 5.25
- IAR embedded Workbench 8.30.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
- Personal Computer

Board settings
==============
The example requires 2 sets of boards, each of them are mounted with the base board. Using a male to male CAN
cable to connect the CAN2 instance (J11) between the 2 base boards.

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

Node:a

Press any key to trigger one-shot transmission

Rx MB ID: 0x123, Rx MB data: 0x0, Time stamp: 8877

Press any key to trigger the next transmission!

Rx MB ID: 0x123, Rx MB data: 0x1, Time stamp: 32459

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

Node:b

Start to Wait data from Node A

Rx MB ID: 0x321, Rx MB data: 0x0, Time stamp: 5759

Wait Node A to trigger the next transmission!

Rx MB ID: 0x321, Rx MB data: 0x1, Time stamp: 57276

Wait Node A to trigger the next transmission!
~~~~~~~~~~~~~~~~~~~~~

Note:
This case will be blocked when transferring data on armgcc debug/sdram_debug target. This issue will be fixed in later release.
Customization options
=====================

