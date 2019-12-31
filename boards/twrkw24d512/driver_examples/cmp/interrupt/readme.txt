Overview
========

The CMP interrupt Example shows how to use interrupt with CMP driver.

In this example, user should indicate an input channel to capture a voltage signal (can be controlled by user) as the 
CMP's positive channel input. On the negative side, the internal 6-bit DAC is used to generate the fixed voltage about
half value of reference voltage.

When running the project, change the input voltage of user-defined channel, then the comparator's output would change
between logic one and zero when the user-defined channel's voltage crosses the internal DAC's value. The change of
comparator's output would generate the falling and rising edge events with their interrupts enabled. When any CMP 
interrupt happens, the CMP's ISR would turn on the LED light if detecting the output's rising edge, or turn off it when
detecting the output's falling edge.

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
- TWR-KW24D512 board
- Personal Computer
- Elevator Tower

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a mini USB cable between the host PC and the OpenSDA USB port on the board.
2.  Connect the analog signal source's output to the input of used-defined comparator's channel (defined as "DEMO_CMP_USER_CHANNEL" in source code.)
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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~
CMP interrupt Example
~~~~~~~~~~~~~~~~~~~~~

SW3 is used to control the input signal of CMP_IN0 (PTC6). To enable pull-up resister for SW3 to connect to VCC,
the pin mux is set to GPIO.
    - Don't press SW3: LED BLUE1 on
    - Press SW3 : LED BLUE1 off
Customization options
=====================

