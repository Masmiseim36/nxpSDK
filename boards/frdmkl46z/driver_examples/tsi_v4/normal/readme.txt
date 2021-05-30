Overview
========
The tsi_v4_normal example shows how to use TSI_V4 driver in normal modes:

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, we get the non-touch calibration results as baseline electrode counter;
2. Then, we start the Software-Trigger scan using polling method and interrupt method;
3. Then, we start the Hardware-Trigger scan using interrupt method.


Toolchain supported
===================
- Keil MDK 5.24a
- IAR embedded Workbench 8.22.2
- GCC ARM Embedded 7-2017-q4-major
- MCUXpresso10.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL46Z board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
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
1. Open serial terminal utility with specific COM number when board is powered on;
2. Download the code to device and run it;
3. When you touch the specific pad, the pad's LED will light on for some time to indicate that touch event happened.

When the example runs successfully, you can see the similar information from the terminal as below.
Note: The message shown below is only one part, as the hardware trigger scan run in a infinite way, the message will
    continue output until you power off the board.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Calibrated counters for channel 0 is: 699
Calibrated counters for channel 1 is: 65535
Calibrated counters for channel 2 is: 65535
Calibrated counters for channel 3 is: 65535
Calibrated counters for channel 4 is: 65535
Calibrated counters for channel 5 is: 65535
Calibrated counters for channel 6 is: 939
Calibrated counters for channel 7 is: 661
Calibrated counters for channel 8 is: 648
Calibrated counters for channel 9 is: 1374
Calibrated counters for channel 10 is: 1422
Calibrated counters for channel 11 is: 661
Calibrated counters for channel 12 is: 623
Calibrated counters for channel 13 is: 614
Calibrated counters for channel 14 is: 65535
Calibrated counters for channel 15 is: 751

NOW, Test the software trigger scan using polling method!
Channel 9 Normal mode counter is: 1374
Channel 10 Normal mode counter is: 1423

NOW, Test the software trigger scan using interrupt method!
Enter into TSI0 IRQHandler...
Channel 9 Normal mode counter is: 65535
Enter into TSI0 IRQHandler...
Channel 10 Normal mode counter is: 1421

NOW, Test the hardware trigger scan method!
Enter into TSI0 IRQHandler...
Channel 9 Normal mode counter is: 1373
Enter into TSI0 IRQHandler...
Channel 9 Normal mode counter is: 1373
Enter into TSI0 IRQHandler...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

