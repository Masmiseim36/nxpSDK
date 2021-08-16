Hardware requirements
===================
- Mini/micro USB cable
- FRDMK64F board
- FRDM-STBC-AGM01 board
- Personal Computer

Board settings
==============
On FRDM-STBC-AGM01 sensor shield board:
--------------------------------------
For I2C Mode: The examples use I2C1 and Pins 2-3 of Jumpers J6 and J7 on FRDM-STBC-AGM01 should be connected.

Demo Overview
============
The FreeMASTER Sensor demo using the ISSDK FXOS8700 sensor driver example and FreeMASTER drivers. This demo provides a embedded
project template that integrates sensor embedded functions, monitor sensor registers read/write and control variables. This demo
interfaces with FreeMASTER host using UART and demonstrates easy-to-use sensor evaluation & development tool.

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
===============
1. Follow FreeMASTER_Sensor_Tool Getting Started steps:
https://www.nxp.com/document/guide/get-started-with-the-freemaster-sensor-tool:GS-FREEMASTER-SENSOR-TOOL
2. Run the FXOS8700 FreeMASTER Demo.
