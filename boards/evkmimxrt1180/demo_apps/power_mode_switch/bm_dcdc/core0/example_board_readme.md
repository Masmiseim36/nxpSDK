Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1180-EVK board
- Personal Computer
- USB to Com Converter

Board settings
============
No special settings are required.

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
================
1. When running the demo, the debug console shows the menu to provide the desired operation. Here we take multicore targets as an example.

This is core0.
Copy Secondary core image to address: 0x303c0000, size: XXXXX
Starting Secondary core.
The secondary core application has been started.

CPU wakeup source 0x10001...

***********************************************************
	Power Mode Switch Demo for iMXRT1189
***********************************************************

Core0 send message to core1.

M33 previous CPU mode is RUN
M33 current CPU mode is RUN
M33 CLK is 240 MHz
M7 previous CPU mode is RUN
M7 current CPU mode is RUN
M7 CLK is 798 MHz
EDGELOCK CLK is 200 MHz
BUS AON CLK is 132 MHz
BUS WAKEUP CLK is 264 MHz
WAKEUP AXI CLK is 240 MHz


Please select the desired operation:
Press  A to demonstrate run mode switch.
Press  B to demonstrate cpu mode switch.
Press  C to enter BBSM mode.

Waiting for select...

2. When 'A' is selected, the menu shows selections for different run mode.

RUN mode switch:
Press A to enter OverDrive RUN
Press B to enter Normal RUN
Press C to enter UnderDrive RUN
Press 'Q' to exit

Waiting for select...

3. When 'B' is selected, the menu shows selections for different CPU mode with/without system sleep request.

CPU mode switch:
Press A to enter CPU mode: RUN
Press B to enter CPU mode: WAIT
Press C to enter CPU mode: STOP
Press D to enter CPU mode: SUSPEND
Press E to enter CPU mode: STOP, system sleep
Press F to enter CPU mode: SUSPEND, system sleep
Press 'Q' to exit

Waiting for select...

3. When 'C' is selected, the menu shows wakeup source selection for BBSM mode.
System will wake up from reset when wakeup event happening.

Select the wake up source:
Press T for Timer
Press S for GPIO button SW8. 

Waiting for key press..

Note
===============
Reset event will happen when running this demo, so container header is needed. Please add container header by spsdk.