Overview
========
The ACMP Round-Robin project is a simple demonstration program that uses the SDK software. User
must set the round-robin mode trigger in specific board properly according to the board resource
before running the example. When the example running, it sets positive port as fixed channel and
internal DAC output as comparison reference in positive port and sets the channels input by user
as round-robin checker channel. The example will enter stop mode and wait user to change the voltage
of round-robin checker channel. It will exit stop mode after the voltage of round-robin checker
channel changed by user.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- TWR-KE18F board
- Primary Elevator
- Personal Computer

Board settings
==============
The example use PTB4 to change the input voltage of ACMP1 channel 2. In order to observe the low
power wakeup phenomenon, channel 2(Elevator-B58) should be connected to GND on the board before
the example runs.Example will exit stop mode when PTB4 is disconnected from GND and the connection
time lasts for about 1s.
- Input signal to B58 Elevator

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, following information can be seen on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Example to demonstrate low power wakeup by round robin comparison!

In order to wakeup the MCU, please change the analog input voltage to be different from original pre-state setting.

The system entered into stop mode.

The system exited from stop mode!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

