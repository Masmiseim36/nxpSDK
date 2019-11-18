Overview
========
The WDOG32 Example project is to demonstrate usage of the KSDK wdog32 driver.
In this example, fast testing is first implemented to test the wdog32.
After this, refreshing the watchdog in None-window mode and window mode is executed.
Note wdog32 is disabled in SystemInit function which means wdog32 is disabled
after chip emerges from reset.



Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- GCC ARM Embedded  7.3.1

Hardware requirements
=====================
- Micro USB cable
- i.MX8QM MEK CPU Board
- J-Link Debug Probe
- 12V power supply
- Personal Computer

Board settings
==============
No special settings are required.

#### Note! ####
1. The M4 WDOG will reboot the partition that contains the M4 cores. The Partition need correctly configured for the M4 cores.
   When using imx-mkimage build the bootable image, add "-px" where "x" can be 1,3,4 to configure the partition for M4 cores.
   Such as the availabe targets in imx-mkimage: flash_b0_cm4_0, flash_b0_cm4_1, flash_linux_m4.
   (1) If only boot one of the M4 cores, use -p1 for the M4 image. Such as flash_b0_cm4_0, flash_b0_cm4_1.
   (2) If boot more than one M4 cores, use -p3 for M4 core0, -p4 for M4 core1 image. And add -flags 0x200000 for the first container. Such as flash_linux_m4.
   
   For more information about the reset, please refer to System Controller Firmware Porting Guide.
2. Please use imx-mkimage to load the application. Debugging or loading using debugger does not work.


Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board.
2.  Connect a USB cable between the host PC and the Debug port on the board (Refer "Getting Started with MCUXpresso SDK for i.MX 8QuadMax.pdf" for debug port information).
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board (Please refer "Getting Started with MCUXpresso SDK for i.MX 8QuadMax.pdf" for how to run different targets).
5.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
Low Byte test success
High Byte test success

----- Refresh test start -----
----- None-window mode -----
Refresh wdog32 0 time
Refresh wdog32 1 time
Refresh wdog32 2 time
Refresh wdog32 3 time
Refresh wdog32 4 time
Refresh wdog32 5 time
Refresh wdog32 6 time
Refresh wdog32 7 time
Refresh wdog32 8 time
Refresh wdog32 9 time
----- Window mode -----
Refresh wdog32 0 time
Refresh wdog32 1 time
Refresh wdog32 2 time
----- Refresh test success  -----

----- End of WDOG32 example  -----
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================

