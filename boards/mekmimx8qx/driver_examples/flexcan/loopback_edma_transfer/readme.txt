Overview
========
The flexcan_loopback_edma example shows how to use the EDMA version transactional driver to receive
CAN Message from Rx FIFO:

To demonstrates this example, only one board is needed. The example will config one FlexCAN Message
Buffer to Tx Message Buffer and also setup Rx FIFO. After that, the example will send 4 CAN Messages
from Tx Message Buffer to Rx FIFO through internal loopback interconnect and read them out using
EDMA version FlexCAN transactional driver. The Sent and received message will be print out to terminal
at last.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- MIMX8QX MEK board
- J-Link Debug Probe
- 12V power supply
- Personal Computer

Board settings
==============
No special setting needed.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a micro USB cable between the host PC and the J11 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~

==FlexCAN loopback edma example -- Start.==


==FlexCAN loopback edma example -- Start.==

Send Msg1 to Rx FIFO: word0 = 0x11111111, word1 = 0x11111111.
Send Msg2 to Rx FIFO: word0 = 0x22222222, word1 = 0x22222222.
Send Msg3 to Rx FIFO: word0 = 0x33333333, word1 = 0x33333333.
Send Msg4 to Rx FIFO: word0 = 0x44444444, word1 = 0x44444444.

Receive Msg1 from FIFO: word0 = 0x11111111, word1 = 0x11111111, ID Filter Hit2.
Receive Msg2 from FIFO: word0 = 0x22222222, word1 = 0x22222222, ID Filter Hit2.
Receive Msg3 from FIFO: word0 = 0x33333333, word1 = 0x33333333, ID Filter Hit2.
Receive Msg4 from FIFO: word0 = 0x44444444, word1 = 0x44444444, ID Filter Hit2.

==FlexCAN loopback EDMA example -- Finish.==

~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

