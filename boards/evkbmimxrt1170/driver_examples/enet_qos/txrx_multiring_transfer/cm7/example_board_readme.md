Hardware requirements
===================
- Mini/micro USB cable
- Loopback network cable RJ45 standard
- EVK-MIMXRT1170 board
- Personal Computer

Board settings
============
If Rx frame CRC error occurs, try to populate a suitable C66. The REV C/C1 boards populate 22pF which is high and may result in CRC
errors on some of boards, if this is the case C66 may be removed or replaced by a smaller capacitor.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert loopback network cable to Ethernet RJ45 port(J4).
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Make loopback cable:
    568B standard 	Unknowed standard
J1	orange+white    green+white
J2	orange          green
J3	green+white     orange+white
J4	blue            brown+white
J5	blue+white      brown
J6	green           orange
J7	brown+white     blue
J8	brown           blue+white

Connect J1 => J3, J2 => J6, J4 => J7, J5 => J8. 10/100M transfer only requires J1, J2, J3, J6, and 1G transfer requires all 8 pins.
Check your net cable color order and refer to 568B standard or the other standard. If your cable's color order is not showed in the list,
please connect J1~J8 based on your situation.

Running the demo
===============
When the demo runs, the log would be seen on the terminal like:

 ENET multi-ring txrx example start.

30 frames will be sent in 3 queues, and frames will be received in  queues.
The frame transmitted from the ring 0, 1, 2 is 10, 10, 10!
30 frames transmitted succeed!
The frames successfully received from the ring 0, 1, 2 is 10, 10, 10!
