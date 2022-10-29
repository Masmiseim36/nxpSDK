Hardware requirements
===================
- 2x Mini/micro USB cables
- FRDM-K22F board
- FRDM-STBA-A8967 board
- Personal Computer
- FreeMASTER 3.x installed on PC

Board settings
============
Since the examples use I2C1, Pins 2-3 of SW2 on FRDM-STBA-A8967 should be connected.
SW1 Pins 2-3 should be connected to select default operating mode i.e. "ACCEL NORMAL" mode.

Prepare the demo
===============
1.  Connect a USB cable between the host PC and the Debug USB port on the target board.
2.  Compile and download the program to the target microcontroller.
3.  Run and resume application execution when debugger stops in the main() function.

Connect with FreeMASTER
=======================
4.  Connect the 2nd USB cable between host PC and the microcontroller USB port on the target board.
5.  Wait for the operating system to configure the new USB serial device and assigns the COMx port.
6.  Run FreeMASTER, use the Connection Wizard or open Project Options.
7.  Select serial communication, choose the newly installed COMx port and connect at speed 115200 bps.

Running the demo
===============
1. Follow FreeMASTER_Sensor_Tool Getting Started steps:
https://www.nxp.com/document/guide/get-started-with-the-freemaster-sensor-tool:GS-FREEMASTER-SENSOR-TOOL
2. Run the FXLS896x FreeMASTER Demo.
