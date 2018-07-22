Overview
========
The Power manager demo application demonstrates the use of power modes in the KSDK. The demo prints the power mode menu
through the debug console, where the user can set the MCU to a specific power mode. The user can also set the wakeup
source by following the debug console prompts. The purpose of this demo is to demonstrate the implementation of a power
mode manager.

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
- FRDMK82F board
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When running the demo, the debug console shows the menu to command the MCU to the target power mode.
NOTE: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow
and receive the wrong input value.
~~~~~~~~~~~~~~~~~~~~~
####################  Power Manager Demo ####################
    Core Clock = 120000000Hz
    Power mode: RUN
Select the desired operation
Press  A for enter: RUN      - Normal RUN mode
Press  B for enter: WAIT     - Wait mode
Press  C for enter: STOP     - Stop mode
Press  D for enter: VLPR     - Very Low Power Run mode
Press  E for enter: VLPW     - Very Low Power Wait mode
Press  F for enter: VLPS     - Very Low Power Stop mode
Press  G for enter: LLS/LLS3 - Low Leakage Stop mode
Press  H for enter: VLLS0    - Very Low Leakage Stop 0 mode
Press  I for enter: VLLS1    - Very Low Leakage Stop 1 mode
Press  J for enter: VLLS2    - Very Low Leakage Stop 2 mode
Press  K for enter: VLLS3    - Very Low Leakage Stop 3 mode
Press  L for enter: HSRUN    - High Speed RUN mode
Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~

Customization options
=====================


