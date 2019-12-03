Overview
========

The enet_txrx_multiring_transfer example shows the way to use ENET driver to  
 receive and transmit avb frame in the avb feature supported multi-ring platforms.
 this example is only supported in multi-ring platform.

1. This example shows how to initialize the ENET MAC.
2. How to use ENET MAC to transmit frames in avb supported 
multiple-ring platforms.

The example transmits 30 number frames. For simple demo, we create frames with some special definition.
1. Build transmission frames with broadcast mac address.
2. Build different frames for each ring: 10 normal ethernet frame, 10 similar IEC 61883-6 type frames,
10 similar IEC 61883-8 type frames and the two IEC 1722 avb frames are set with different vlan priority .
 
Notice, To simply show how to use three different rings in rx/tx on one board. This example set PHY local loop and you will see there will be PHY init auto-negotiation fail and this is normal. because phy can not finish
auto-negotiation when phy is in loop back mode. However, the auto-negotiation fail has no impact on this loop back example, so just forget about the failure auto-negotiation.

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
- Network cable RJ45 standard
- Personal Computer

Board settings
==============
J7 connected with PC through RJ45 network cable.

#### Please note this application can't support running with Linux BSP! ####
This example aims to show the basic usage of the IP's function, some of the used Pads/Resources are also used by Cortex-A core.

Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a micro USB cable between the host PC and the J11 USB Debug Port on the CPU board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Insert Cable to Ethernet RJ45 port and connect it to your PC.
5.  Download the program to the target board.
6.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs, the log would be seen on the debug terminal like:

ENET multi-ring txrx example start.
The 0 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 1, 0, 0 now!
The 1 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 1, 0, 1 now!
The 1 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 1, 1, 1 now!
The 1 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 2, 1, 1 now!
The 2 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 2, 1, 2 now!
The 2 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 2, 2, 2 now!
The 2 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 3, 2, 2 now!
The 3 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 3, 2, 3 now!
The 3 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 3, 3, 3 now!
The 3 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 4, 3, 3 now!
The 4 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 4, 3, 4 now!
The 4 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 4, 4, 4 now!
The 4 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 5, 4, 4 now!
The 5 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 5, 4, 5 now!
The 5 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 5, 5, 5 now!
The 5 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 6, 5, 5 now!
The 6 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 6, 5, 6 now!
The 6 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 6, 6, 6 now!
The 6 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 7, 6, 6 now!
The 7 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 7, 6, 7 now!
The 7 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 7, 7, 7 now!
The 7 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 8, 7, 7 now!
The 8 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 8, 7, 8 now!
The 8 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 8, 8, 8 now!
The 8 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 9, 8, 8 now!
The 9 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 9, 8, 9 now!
The 9 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 9, 9, 9 now!
The 9 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 10, 9, 9 now!
The 10 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 10, 9, 10 now!
The 10 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 10, 10, 10 now!
30 frame transmitted succeed!
10 frame successfully received from the ring 0!
10 frame successfully received from the ring 1!
10 frame successfully received from the ring 2!


Customization options
=====================

