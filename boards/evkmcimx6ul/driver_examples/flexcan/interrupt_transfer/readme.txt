Overview
========
The flexcan_interrupt example shows how to use FlexCAN driver in none-blocking interrupt way:

In this example, 2 boards are connected through CAN bus. Endpoint A(board A) send a CAN Message to
Endpoint B(board B) when user press space key in terminal. Endpoint B receive the message, print
the message content to terminal and echo back the message. Endpoint A will increase the received
message and waiting for the next transmission of the user initiated.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
FLEXCAN board to board:(only FlexCAN interrupt example):
- Connect the two board through the CAN CN (J1201):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        BOARD ONE           CONNECTS TO          BOARD TWO
Pin Name    Board Location               Pin Name  Board Location
CANL        J1201 - 3                       CANL      J1201 - 3
CANH        J1201 - 4                       CANH      J1201 - 4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
After connecting the two boards, these instructions display on each terminal window.
One board must be chosen as node A and the other board as node B. (Note: Node B should start first)
Data is sent continuously between the node A and the node B.

This message displays on the node A terminal:

~~~~~~~~~~~~~~~~~~~~~

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



Rx MB ID: 0x123, Rx MB data: 0x0

Press any key to trigger the next transmission!



Rx MB ID: 0x123, Rx MB data: 0x1

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



Rx MB ID: 0x321, Rx MB data: 0x0

Wait Node A to trigger the next transmission!



Rx MB ID: 0x321, Rx MB data: 0x1

Wait Node A to trigger the next transmission!
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================

