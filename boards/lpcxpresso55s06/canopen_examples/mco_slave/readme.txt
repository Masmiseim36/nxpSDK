Overview
========
The mco_slave/mcofd_slave example implements a minimal CANopen CiA 401 generic I/O device.
It produces a cyclic heartbeat message (703h) and waits to be detected by a 
CANopen Manager. It replies to (SDO) requests received by the manager.

NOTE: For more details, please see the User's Manual for the
EmSA CANopen (FD) Libraries for NXP SDKs.


Implementation Description
Node-ID: 0x03, Baudrate: 500kbps

Messages produced (by CAN ID):
0x083 - Emergency and emergency reset
0x583 - SDO responses to requests
0x703 - Heartbeat (default 1000ms)
In operatinal state:
0x183 - TPDO1 - Total of 4 input Bytes
    [6000h,1] Digital in byte 1: counter of digital data received
    [6000h,2] Digital in byte 2: counter of analog data received
    [6000h,3] Digital in byte 3: copy/echo of [6200h,3] Digital out byte 3
    [6000h,4] Digital in byte 4: copy/echo of [6200h,4] Digital out byte 4
0x283 - TPDO2 - Two 16-bit analog inputs.
    [6401h,1] Analog out word16 1: timer with hi byte seconds and lo byte quarter seconds
    [6401h,2] Analog out word16 2: copy/echo of [6411h,2] Analog out word16 2

Messages consumed (by CAN ID):
0x000 - NMT Master command message
0x701 - HBCon - Heartbeat consumer node 1 (after configiration through manager)
In operatinal state:
0x203 - RPDO1 - Four bytes of digital outputs. 
0x303 - RPDO2 - Two 16-bit analog outputs.

User can use a CAN monitor (USB CAN analysis) to probe the CAN frame transfered.
- CANL in CAN USB analysis connected with CANL in two board by shunt-wound method.
- CANH in CAN USB analysis connected with CANH in two board by shunt-wound method.
- (optinal) GND in CAN USB analysis connected with GND in two board by shunt-wound method.

Toolchain supported
===================
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1
- MCUXpresso  11.4.0

Hardware requirements
=====================
- Two Micro USB cables
- Two LPCXpresso55S06 boards
- Personal Computer

Board settings
==============
Between CAN transceivers:
- CANH, CANL, GND should be connected

Note: 
When using jlink to download program, please short JP9, Remove JP12. And do not use the serial port J1, J3-1 for UART RX, J3-2 for UART TX.

Prepare the Demo
================
1.  Connect a micro USB cable between the host PC and the LPC-Link USB port (J1) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
1. First press the reset button on the board with the mco_slave example
2. Second press the reset button on the board with the mco_manager example

~~~~~~~~~~~~~~~~~~~~~
The terminal connected to the mco_manager example displays similar like that:

Starting CANopen Library manager example
Provided by EmSA - www.em-sa.com/nxp

CANopen Library Event - Reset Communication, bitrate 500kbps, node id 1
CANopen Library Event - NMT Change: 0x 0 boot
CANopen Library Event - NMT Change: 0x 5 operational
CANopen Manager Event - Node Status Change: 3, 0x 0 booted
CANopen Manager Event - Node Status Change: 3, 0x81 emergency over / reset
CANopen Manager Event - Node Status Change: 3, 0xA0 scan complete
 [1000,00]:     191 [1018,01]:AF0002DC [1018,02]:C0DE0010
CANopen Manager Event - Node Status Change: 3, 0x 5 operational
CANopen Manager Event - Node Status Change: 3, 0x90 heartbeat monitoring active
CANopen Manager Client SDO Complete: node 3
 [1008,00]:CANopenLibNXPSDK CiA401 Example
 

The terminal connected to the mco_slave example displays similar like that:

Starting CANopen Library slave example
Provided by EmSA - www.em-sa.com/nxp

CANopen Library Event - Reset Communication, bitrate 500kbps, node id 3
CANopen Library Event - NMT Change: 0x 0 boot
CANopen Library Event - NMT Change: 0x7F pre-operational
CANopen Library Event - NMT Change: 0x7F pre-operational
CANopen Library Event - NMT Change: 0x 5 operational
~~~~~~~~~~~~~~~~~~~~~
