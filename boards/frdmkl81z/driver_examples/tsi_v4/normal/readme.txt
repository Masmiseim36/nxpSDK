Overview
========
The tsi_v4_normal example shows how to use TSI_V4 driver in normal modes:

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, we get the non-touch calibration results as baseline electrode counter;
2. Then, we start the Software-Trigger scan using polling method and interrupt method;
3. Then, we start the Hardware-Trigger scan using interrupt method.

Running the demo
================
1. Open serial terminal utility with specific COM number when board is powered on;
2. Download the code to device and run it;
3. When you touch the specific pad, the pad's LED will light on for some time to indicate that touch event happened.

When the example runs successfully, the following message is displayed in the terminal.
Note: The message shown below is only one part, as the hardware trigger scan run in a infinite way, the message will continue
      output until you power off the board.

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
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL81Z board
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
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

