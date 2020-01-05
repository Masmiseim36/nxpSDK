Overview
========
This example demonstrates usage of eRPC between PC and board using UART transport layer.
Both board and the PC acts like server and client thanks to eRPC arbitrator.

When both servers starts, they waits for data being send from client over UART. 
A. Server on the board performs action (DAC/ADC conversion, turn on selected LEDs, read data from magnetometer and accelerometer sensor) 
and sends result data back to client (or lights LED).
B. Server on the PC displays which SW button was pressed on the board.

eRPC documentation
eRPC specific files are stored in: middleware\multicore\erpc
eRPC documentation is stored in: middleware\multicore\erpc\doc
eRPC is open-source project stored on github: https://github.com/EmbeddedRPC/erpc
eRPC documentation can be also found in: http://embeddedrpc.github.io

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- MCUXpresso  11.1.0
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1

Hardware requirements
=====================
- Mini USB cable
- FRDM-KL27Z board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Download the program to the target board.
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
4.  Run "run.py" on your PC with -p (--port) parameter to select COM port (baud rate can be also set through -b (--bd) parameter).

For detailed instructions, see the appropriate board User's Guide.

Running the demo
================

A. Server on the board, client on the PC:

Selected UART port: com15, 115200 bd
eRPC server has been run
eRPC client has been created
Board configuration:
    DAC: False
    LED: Red=True, Green=True, Blue=True
ADC configuration:
    Vref=3.29999995232 V
    Atomic steps=4096.0

---------------------------
eRPC Remote Control example
---------------------------
-> Press '1' for ADC conversion
-> Press '2' for GPIO LED
-> Press '3' for Accelerometer and Magnetometer values
2
Select which LED should be turned on:
--> Press '1' for red
--> Press '2' for green
--> Press '3' for blue
1

---------------------------
eRPC Remote Control example
---------------------------
-> Press '1' for ADC conversion
-> Press '2' for GPIO LED
-> Press '3' for Accelerometer and Magnetometer values
3
Read value from Accelerometer and Magnetometer:
    Accelerometer axis: x=7608, y=16736, z=-512
    Magnetometer axis: x=0, y=0, z=0

---------------------------
eRPC Remote Control example
---------------------------
-> Press '1' for ADC conversion
-> Press '2' for GPIO LED
-> Press '3' for Accelerometer and Magnetometer values
1
ADC value: 1.50095212675 V


A. Server on the PC, client on the board:

[Message from board] Button SW2 was pressed
[Message from board] Button SW3 was pressed
Customization options
=====================

