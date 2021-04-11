Overview
========
The user interface of a product is a key element that design engineers need to address
to provide a compelling user experience. Touchpads, slides and rotaries offer 
a more intuitive and effective way of user interaction than traditional buttons. 
And, designing a touch-based user interface is simplified with the NXP Touch solution.
This solution includes software that is designed to work simply and seamlessly with 
original capacitive touch sensing hardware IP.

Toolchain supported
===================
- IAR embedded Workbench 8.40
- Keil MDK 5.29
- MCUXpresso IDE 11.1.0

Hardware requirements
=====================
- Micro USB cable
- FRDM-K32L2A4S board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Download the program to the target board.
3.  Download a FreeMASTER tool (from: http://www.nxp.com/freemaster, version 2.5 or 3.0) and install the FreeMASTER
4.  Open the FreeMASTER project NXPTouchK32l2a4s.pmp file from: MCUXpresso SDK\middleware\touch\freemaster\NXPTouchK32l2a4s.pmp and choose Project -> Options... from FreeMASTER menu.  
5.  In Comm tab set the port with description: mbed Serial Port.
6.  Set the Speed to 115200 baud rate.
7.  Continue with help of the NXP Touch Library Reference Manual (Chapter 8 Examples).

Running the demo
================
The control page in FreeMASTER should show changing values from TSI measurement. The Led on FRDM-K32L2A4S board 
should be shining after the touch event or should be blinking when finger is moving on the slider control. 
The LED color demonstrates the slider position and is changing from red through yellow, green and purple to blue color.
More information can be found in the NXP Touch Library Reference Manual (Chapter 8 Examples).