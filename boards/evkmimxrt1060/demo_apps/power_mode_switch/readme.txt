Overview
========
The Power mode switch demo application demonstrates the use of power modes in the KSDK. The demo prints the power mode menu
through the debug console, where the user can set the MCU to a specific power mode. User can wakeup the core by key interrupt.
The purpose of this demo is to show how to switch between different power  modes, and how to configure a wakeup source and
wakeup the MCU from low power modes.
Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
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
When running the demo, the debug console shows the menu to command the MCU to the target power mode.

~~~~~~~~~~~~~~~~~~~~~
CPU wakeup source 0x3...

***********************************************************
	Power Mode Switch Demo for iMXRT1060
***********************************************************

***********************************************************
CPU:             600000000 Hz
AHB:             600000000 Hz
SEMC:            100000000 Hz
IPG:             150000000 Hz
OSC:             24000000 Hz
RTC:             32768 Hz
ARMPLL:          1200000000 Hz
USB1PLL:         480000000 Hz
USB1PLLPFD0:     720000000 Hz
USB1PLLPFD1:     508235292 Hz
USB1PLLPFD2:     508235292 Hz
USB1PLLPFD3:     454736826 Hz
USB2PLL:         480000000 Hz
SYSPLL:          528000000 Hz
SYSPLLPFD0:      351999990 Hz
SYSPLLPFD1:      594000000 Hz
SYSPLLPFD2:      396000000 Hz
SYSPLLPFD3:      594000000 Hz
ENETPLL0:        25000000 Hz
ENETPLL1:        25000000 Hz
AUDIOPLL:        864000 Hz
VIDEOPLL:        2150000 Hz
***********************************************************

Task 2 is working now
Task 1 is working now

########## Power Mode Switch Demo (build Oct 12 2017) ###########

    Core Clock = 600000000Hz 
    Power mode: Over RUN

***********************************************************
CPU:             600000000 Hz
AHB:             600000000 Hz
SEMC:            100000000 Hz
IPG:             150000000 Hz
OSC:             24000000 Hz
RTC:             32768 Hz
ARMPLL:          1200000000 Hz
***********************************************************


Select the desired operation 

Press  A for enter: Over RUN       - System Over Run mode (600MHz)
Press  B for enter: Full RUN       - System Full Run mode (528MHz)
Press  C for enter: Low Speed RUN  - System Low Speed Run mode (132MHz)
Press  D for enter: Low Power RUN  - System Low Power Run mode (24MHz)
Press  E for enter: System Idle    - System Wait mode
Press  F for enter: Low Power Idle - Low Power Idle mode
Press  G for enter: Suspend        - Suspend mode
Press  H for enter: SNVS           - Shutdown the system

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~


Note: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow
and receive the wrong input value.

Note: When wake up from Suspend state, target will reset. Please run in flexspi_nor_debug and flexspi_nor_release targets to test Suspend states.

Note:
To download binary into qspiflash and boot from qspiflash directly, following steps are needed:
1. Compile flash target of the project, and get the binaray file "power_mode_switch_ca.bin".
3. Set the SW7: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Drop the binaray into disk "RT1060-EVK" on PC.
5. Wait for the disk disappear and appear again which will take couple of seconds.
7. Reset the board by pressing SW3 or power off and on the board. 

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

