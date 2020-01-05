Overview
========
The tsi_v5_self_cap example shows how to use TSI_V5 driver in self-cap mode:

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, we get the non-touch calibration results as baseline electrode counter;
2. Then, we start the Software-Trigger scan using polling method and interrupt method;
3. Then, we start the Hardware-Trigger scan using interrupt method.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
TSI_V5 Self-Cap mode Example Start!
Calibrated counters for channel 0 is: 65535 
Calibrated counters for channel 1 is: 65535 
Calibrated counters for channel 2 is: 65535 
Calibrated counters for channel 3 is: 65535 
Calibrated counters for channel 4 is: 65535 
Calibrated counters for channel 5 is: 65535 
Calibrated counters for channel 6 is: 65535 
Calibrated counters for channel 7 is: 65535 
Calibrated counters for channel 8 is: 523 
Calibrated counters for channel 9 is: 65535 
Calibrated counters for channel 10 is: 65535 
Calibrated counters for channel 11 is: 65535 
Calibrated counters for channel 12 is: 65535 
Calibrated counters for channel 13 is: 65535 
Calibrated counters for channel 14 is: 65535 
Calibrated counters for channel 15 is: 553 
Calibrated counters for channel 16 is: 65535 
Calibrated counters for channel 17 is: 1908 
Calibrated counters for channel 18 is: 1936 
Calibrated counters for channel 19 is: 65535 
Calibrated counters for channel 20 is: 2084 
Calibrated counters for channel 21 is: 45421 
Calibrated counters for channel 22 is: 65535 
Calibrated counters for channel 23 is: 65535 
Calibrated counters for channel 24 is: 65535 

NOW, comes to the software trigger scan using polling method!
Channel 0 Normal mode counter is: 65535 
Channel 1 Normal mode counter is: 65535 

NOW, comes to the software trigger scan using interrupt method!
Channel 0 Normal mode counter is: 65535 
Channel 1 Normal mode counter is: 65535 

NOW, comes to the hardware trigger scan method!
After running, touch pad E1 each time, you will see LED toggles.
~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Micro USB cable
- FRDM-KE16Z board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.


Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.2.1

