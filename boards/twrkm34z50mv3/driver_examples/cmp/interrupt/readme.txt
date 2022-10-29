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
- IAR embedded Workbench  9.30.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.6.0

Hardware requirements
=====================
- Mini USB cable
- TWR-KM34Z50MV3 board
- Personal Computer

Board settings
==============
Turn on the third switch of S1(connect S1-3 to S1-18).

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Connect the analog signal source's output to the input of used-defined comparator's channel (defined as "DEMO_CMP_USER_CHANNEL" in source code.)
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
CMP polling Example
~~~~~~~~~~~~~~~~~~~~~

Then change CMP analog input, and watch the change of LED.
   - CMP0_IN1(J11_16) connected to VCC=3.3V: LED ORANGE ON
   - CMP0_IN1(J11_16) connected to GND: LED ORANGE OFF
