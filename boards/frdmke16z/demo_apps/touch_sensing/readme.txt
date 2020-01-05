Overview
========
The user interface of a product is a key element that design engineers need to address
to provide a compelling user experience. Touchpads, slides and rotaries offer 
a more intuitive and effective way of user interaction than traditional buttons. 
And, designing a touch-based user interface is simplified with the NXP Touch solution.
This solution includes software that is designed to work simply and seamlessly with 
NXP’s capacitive touch sensing hardware IP that’s available on Kinetis KE15Z/KE16Z MCUs.
Key benefits of the NXP Touch solution include:
- Robust EMC performance, noise immunity with IEC61000-4-6 standard certification with the 3V and 10V tests
- Self-cap and mutual-cap modes supporting up to 6x6 matrix touchpads
- Effective performance even with liquid substances such as water, salt water, oil, cold steam, etc.
- High sensitivity with a boost feature to use when the touch overlay on the user interface is thick (acrylic, glass)

Toolchain supported
===================
- IAR embedded Workbench 8.30.1
- Keil MDK 5.25
- MCUXpresso IDE 10.2.1

Hardware requirements
=====================
- Micro USB cable
- FRDM-KE16Z board
- NXP FRDM-TOUCH board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Download the program to the target board.
3.  Download a FreeMASTER tool (from: http://www.nxp.com/freemaster) and install the FreeMASTER
4.  Open the FreeMASTER project NXPTouchKE16.pmp file from: MCUXpresso SDK\middleware\touch\freemaster\NXPTouchKE16.pmp and choose Project -> Options... from FreeMASTER menu.  
5.  In Comm tab set the port with description: USB Serial Device.
6.  Set the Speed to 115200 baud rate.
7.  Continue with help of the NXP Touch Library Reference Manual (Chapter 8 Examples).

Running the demo
================
The control page in FreeMASTER should show changing values from TSI measurement. The Led on FRDM-KE16Z board should change the colour after detected touch event. More information can be found in the NXP Touch Library Reference Manual (Chapter 8 Examples).