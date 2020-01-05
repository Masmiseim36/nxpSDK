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
Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL82Z72M board
- Personal Computer

Board settings
==============
No special settings are required.

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
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
CMP interrupt Example.
~~~~~~~~~~~~~~~~~~~~~

Then change CMP analog input, and watch the change of LED.
    - CMP0_IN0 (Jump B45 on Elevator Tower) connected to VCC = 3.3V (Jump B3 on Elevator Tower): LED RED on
    - CMP0_IN0 (Jump B45 on Elevator Tower) connected to GND (Jump B2 on Elevator Tower): LED RED off

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

