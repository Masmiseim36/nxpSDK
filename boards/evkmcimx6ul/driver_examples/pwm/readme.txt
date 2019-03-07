Overview
========
The PWM project is a simple demonstration program of the SDK PWM driver. It sets up the PWM hardware
block to output PWM signals on one PWM channel. The example also shows PWM duty cycle is increase or decrease.
The FIFO empty interrupt is provided. A new value will be loaded into FIFO when FIFO empty status bit is set. 
The PWM will run at the last set duty-cycle setting if all the values of the FIFO has been utilized, 
until the FIFO is reloaded or the PWM is disabled. When a new value is written, the duty cycle changes after the current period is over.
The outputs can be observed by oscilloscope.

Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- GCC ARM Embedded 2016-5.4-q3

Hardware requirements
=====================
- USB cable
- MCIMX6UL-EVK  board
- JLink Plus
- 5V power supply
- Personal Computer
- Oscilloscope

Board settings
==============
No special is needed.

Prepare the Demo
================
1.  Connect 5V power supply and JLink Plus to the board, switch SW2001 to power on the board.
2.  Connect a USB cable between the host PC and the J1901 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - random baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~
PWM driver example.
~~~~~~~~~~~~~~~~~~~~~~~
Use Oscilloscope to measure and observe the J2101-47 output signal.

Customization options
=====================

