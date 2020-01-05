Overview
========
The tsi_v4_low_power example shows how to use TSI_V4 driver in low power modes:

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, we get the non-touch calibration results as baseline electrode counter;
2. Then, we start the Hardware-Trigger scan using interrupt method to wakeup from low power
   modes through pad touch;
3. Note: you can select which low power mode you want to enter into.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

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
1. Download the code to device flash, do not using debug mode;
2. Power-off then power-on the board;
3. Open serial terminal utility with specific COM number, then press "Reset Button" on board.
4. Do as the message shown in terminal guides;

When the example runs successfully, you can see the similar information from the terminal as below.

Note: The message shown below will different depends on your input character.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TSI_V4 Low_Power_Modes Example Start!
Please don't touch the touchpad
Press anykey to start TSI low power example
Start calibartion....
Calibrated counters for channel 0 is: 975
Calibrated counters for channel 1 is: 65535
Calibrated counters for channel 2 is: 65535
Calibrated counters for channel 3 is: 65535
Calibrated counters for channel 4 is: 65535
Calibrated counters for channel 5 is: 65535
Calibrated counters for channel 6 is: 1008
Calibrated counters for channel 7 is: 793
Calibrated counters for channel 8 is: 888
Calibrated counters for channel 9 is: 871
Calibrated counters for channel 10 is: 921
Calibrated counters for channel 11 is: 2337
Calibrated counters for channel 12 is: 2346
Calibrated counters for channel 13 is: 1233
Calibrated counters for channel 14 is: 992
Calibrated counters for channel 15 is: 1094

Let's move to the low power modes world!!!

Press specific key to enter stop mode.
After enter stop modes, touch PAD_11 to wakeup.

Please input the mode you want to enter!
0 ------>  STOP MODE
1 ------>  VLPS MODE
2 ------>  LLS  MODE
3 ------>  VLLS1 MODE
4 ------>  VLLS2 MODE
5 ------>  VLLS3 MODE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

