Overview
========
The tsi_v2_low_power example shows how to use TSI_V2 driver in low power modes:

In this example , we make use of the available electrodes on board to show driver usage.
1. Then, we start the Hardware-Trigger scan using interrupt method to wakeup from low power
   modes through pad touch;
2. Note: you can select which low power mode you want to enter into.


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
- TWR-K60D100M board
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

TSI_V2 Low_Power_Modes Example Start!

Let's move to the low power modes world!!!

Press specific key to enter stop mode.
After enter stop modes, touch pad D7 to wakeup.

 Please input the mode you want to enter! 
0 ------>  LLS  MODE 
1 ------>  VLLS1 MODE
2 ------>  VLLS2 MODE
3 ------>  VLLS3 MODE
About to enter into LLS MODE! 
Wakeup will lead to a interrupt flow, pay attention to it!!!
Great!!! You touched it!!!

 Please input the mode you want to enter! 
0 ------>  LLS  MODE 
1 ------>  VLLS1 MODE
2 ------>  VLLS2 MODE
3 ------>  VLLS3 MODE
About to enter into VLLS1 MODE! 
Wakeup will lead to a reset flow, pay attention to it!!!
Wake up from VLLSx modes...
Wakeup channel Counter is:18576

TSI_V2 Low_Power_Modes Example Start!

Let's move to the low power modes world!!!

Press specific key to enter stop mode.
After enter stop modes, touch pad D7 to wakeup.

 Please input the mode you want to enter! 
0 ------>  LLS  MODE 
1 ------>  VLLS1 MODE
2 ------>  VLLS2 MODE
3 ------>  VLLS3 MODE
About to enter into VLLS2 MODE! 
Wakeup will lead to a reset flow, pay attention to it!!!
Wake up from VLLSx modes...
Wakeup channel Counter is:17040

TSI_V2 Low_Power_Modes Example Start!

Let's move to the low power modes world!!!

Press specific key to enter stop mode.
After enter stop modes, touch pad D7 to wakeup.

 Please input the mode you want to enter! 
0 ------>  LLS  MODE 
1 ------>  VLLS1 MODE
2 ------>  VLLS2 MODE
3 ------>  VLLS3 MODE
About to enter into VLLS3 MODE! 
Wakeup will lead to a reset flow, pay attention to it!!!
Wake up from VLLSx modes...
Wakeup channel Counter is:20580

TSI_V2 Low_Power_Modes Example Start!

Let's move to the low power modes world!!!

Press specific key to enter stop mode.
After enter stop modes, touch pad D7 to wakeup.

 Please input the mode you want to enter! 
0 ------>  LLS  MODE 
1 ------>  VLLS1 MODE
2 ------>  VLLS2 MODE
3 ------>  VLLS3 MODE
Customization options
=====================

