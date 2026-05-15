Hardware requirements
===================
- Mini/micro USB cable
- Network cable RJ45 standard
- FRDM-IMXRT1186 board
- Personal Computer
- Servo-Motor

	DELTA_ASDA_B3  * 1
    
	INOVANCE_SV680 * 1

Board settings
============
- J12(1-2)/J13(2-3): ETH0 Function Via J56A.
- J18(1-2)/J17(2-3): ETH2 Function Via J56B.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Power up the servo-motor and connect it to the target board(J56-A) via an Ethernet Cable.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Write the program to the flash of the target board.
5.  Start the demo and test SOEM performance.

