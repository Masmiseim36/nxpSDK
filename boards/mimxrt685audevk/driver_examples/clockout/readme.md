Overview
========
The syscon_clockout driver example shows how to output the internal clock signal. In this driver example, users can
choose the clock signal to be outputted, and the divider of the output clock signal. By probing the output pin, users
can observe the selected internal clock signal.

SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- IAR embedded Workbench  9.60.1
- Keil MDK  5.39.0
- GCC ARM Embedded  13.2.1
- MCUXpresso  11.10.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the clockout demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please choose one output clock.
        A -- SFRO Clock.
        B -- External Clock.
        C -- Low Power Oscillator Clock.
        D -- FFRO Clock.
        E -- Main Clock.
        F -- DSP Main Clock.
        G -- Main System PLL.
        H -- SYSPLL0 AUX0 PLL.
        I -- DSP PLL.
        J -- SYSPLL0 AUX1 PLL.
        K -- AUDIO PLL.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To measure the output clock signal, please use the oscilloscope to probe TP52(P2_29).
