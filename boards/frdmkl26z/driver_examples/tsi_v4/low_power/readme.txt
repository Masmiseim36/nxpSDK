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
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL26Z board
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
When running successfully, the log output in terminal shall be similar as below:

TSI_V4 Low_Power_Modes Example Start!
Please don't touch the touchpad.
Press anykey to start TSI low power example.
Start calibartion....
Calibrated counters for channel 0 is: 2772
Calibrated counters for channel 1 is: 65535
Calibrated counters for channel 2 is: 65535
Calibrated counters for channel 3 is: 65535
Calibrated counters for channel 4 is: 65535
Calibrated counters for channel 5 is: 65535
Calibrated counters for channel 6 is: 2965
Calibrated counters for channel 7 is: 2642
Calibrated counters for channel 8 is: 2682
Calibrated counters for channel 9 is: 2901
Calibrated counters for channel 10 is: 65535
Calibrated counters for channel 11 is: 2708
Calibrated counters for channel 12 is: 2958
Calibrated counters for channel 13 is: 5072
Calibrated counters for channel 14 is: 421
Calibrated counters for channel 15 is: 5073

Let's move to the low power modes world!!!

Press specific key to enter stop mode.
After enter stop modes, touch PAD XX to wakeup.

Please input the mode you want to enter!
0 ------>  STOP MODE
1 ------>  VLPS MODE
2 ------>  LLS  MODE
3 ------>  VLLS1 MODE
4 ------>  VLLS2 MODE
5 ------>  VLLS3 MODE
About to enter into STOP MODE!
Great!!! You touched it!!!

Please input the mode you want to enter!
0 ------>  STOP MODE
1 ------>  VLPS MODE
2 ------>  LLS  MODE
3 ------>  VLLS1 MODE
4 ------>  VLLS2 MODE
5 ------>  VLLS3 MODE
About to enter into VLPS MODE!
Great!!! You touched it!!!

Please input the mode you want to enter!
0 ------>  STOP MODE
1 ------>  VLPS MODE
2 ------>  LLS  MODE
3 ------>  VLLS1 MODE
4 ------>  VLLS2 MODE
5 ------>  VLLS3 MODE
About to enter into LLS MODE!
Wakeup will lead to a interrupt flow, pay attention to it!!!
Great!!! You touched it!!!

Please input the mode you want to enter!
0 ------>  STOP MODE
1 ------>  VLPS MODE
2 ------>  LLS  MODE
3 ------>  VLLS1 MODE
4 ------>  VLLS2 MODE
5 ------>  VLLS3 MODE
About to enter into VLLS1 MODE!
Wakeup will lead to a reset flow, pay attention to it!!!

Wake up from VLLSx modes...

TSI_V4 Low_Power_Modes Example Start!
Please don't touch the touchpad.
Press anykey to start TSI low power example.
......
......
Customization options
=====================

