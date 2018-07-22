Overview
========


Toolchain supported
===================
- IAR embedded Workbench 7.50.1
- Keil MDK 5.17
- GCC ARM Embedded 2015-4.9-q3
- Kinetis Development Studio IDE 3.0.0
- Atollic TrueSTUDIO 5.4.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K82F board
- Personal Computer

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.

Running the demo
================
1. Open serial terminal utility with specific COM number when board is powered on;
2. Download the code to device and run it;
3. When you touch the specific pad, the pad's LED will light on for some time to indicate that touch event happened.
When the example runs successfully, you can see the similar information from the terminal as below.
Note: The message shown below is only one part, as the hardware trigger scan run in a infinite way, the message will 
      continue output until you power off the board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TSI_V4 Normal_mode Example Start!
Calibrated counters for channel 0 is: 976
Calibrated counters for channel 1 is: 9
Calibrated counters for channel 2 is: 65535
Calibrated counters for channel 3 is: 65535
Calibrated counters for channel 4 is: 110
Calibrated counters for channel 5 is: 65535
Calibrated counters for channel 6 is: 1006
Calibrated counters for channel 7 is: 796
Calibrated counters for channel 8 is: 886
Calibrated counters for channel 9 is: 869
Calibrated counters for channel 10 is: 918
Calibrated counters for channel 11 is: 2332
Calibrated counters for channel 12 is: 2349
Calibrated counters for channel 13 is: 1229
Calibrated counters for channel 14 is: 997
Calibrated counters for channel 15 is: 1089
NOW, Test the software trigger scan using polling method!
Channel 11 Normal mode counter is: 2326
Channel 12 Normal mode counter is: 2347
NOW, Test the software trigger scan using interrupt method!
Enter into TSI0 IRQHandler...
Channel 11 Normal mode counter is: 2331
Enter into TSI0 IRQHandler...
Channel 12 Normal mode counter is: 2346
NOW, Test the hardware trigger scan method!
Enter into TSI0 IRQHandler...
Channel 11 Normal mode counter is: 2335
Enter into TSI0 IRQHandler...
Channel 11 Normal mode counter is: 2332
Enter into TSI0 IRQHandler...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================


