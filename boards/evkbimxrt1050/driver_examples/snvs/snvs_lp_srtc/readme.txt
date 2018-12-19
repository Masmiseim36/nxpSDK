Overview
========
The SNVS LP SRTC project is a simple demonstration program of the SDK SNVS LP driver. The test will set up secure RTC (SRTC) date and time to a predefined value and starts the counter, then the SRTC counter value is synchronized with non secure RTC (RTC). RTC then triggers an alarm after a user specified time period. The alarm will be set with reference to this predefined date
and time.
Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
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
The log below shows the output of snvs example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SNVS LP SRTC example:
RTC date and time has been synchronized with SRTC
Set up time to wake up an alarm.
Current datetime: 2014-12-25 19:00:00
Please input the number of second to wait for alarm and press enter
The second must be positive value
Alarm will occur at: 2014-12-25 19:00:16

 Alarm occurs !!!! Current datetime: 2014-12-25 19:00:16
Please input the number of second to wait for alarm and press enter
The second must be positive value

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

