Overview
========
The Power mode switch demo application demonstrates the use of power modes in the KSDK. The demo prints the power mode menu
through the debug console, where the user can set the MCU to a specific power mode. The user can also set the wakeup
source by following the debug console prompts. The purpose of this demo is to show how to switch between different power
 modes, and how to configure a wakeup source and wakeup the MCU from low power modes.

 Tips:
 This demo is to show how the various power mode can switch to each other. However, in actual low power use case, to save energy and reduce the consumption even more, many things can be done including:
 - Disable the clock for unnecessary module during low power mode. That means, programmer can disable the clocks before entering the low power mode and re-enable them after exiting the low power mode when necessary.
 - Disable the function for unnecessary part of a module when other part would keep working in low power mode. At the most time, more powerful function means more power consumption. For example, disable the digital function for the unnecessary pin mux, and so on.
 - Set the proper pin state (direction and logic level) according to the actual application hardware. Otherwise, the pin cirrent would be activied unexpectedly waste some energy.
 - Other low power consideration based on the actual application hardware.


Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- Micro USB cable
- MCIMX6UL-EVK -EVK board
- JLink Plus
- 5V power supply
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When running the demo, the debug console shows the menu to command the MCU to the target power mode.

NOTE: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow
and receive the wrong input value.
~~~~~~~~~~~~~~~~~~~~~
########## Power Mode Switch Demo (build Mar 23 2017) ###########

    Core Clock = 528000000Hz
    Power mode: RUN

Select the desired operation

Press  A for enter: RUN  - System Run mode
Press  B for enter: WAIT - System Wait mode
Press  C for enter: IDLE - Low Power Idle mode
Press  D for enter: DSM  - Deep Sleep mode
Press  E for enter: OFF  - Shutdown the system

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

