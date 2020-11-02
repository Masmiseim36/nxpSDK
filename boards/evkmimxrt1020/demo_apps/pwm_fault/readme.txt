Overview
========
This demo application demonstrates the EflexPWM fault demo.
This application demonstrates the pulse with modulation function of EflexPWM module. It outputs the
PWM to control the intensity of the LED. PWM shut down when a fault signal is detected on the CMP
output. One input of CMP, other input is from internal DAC.

Toolchain supported
===================
- IAR embedded Workbench  8.50.5
- Keil MDK  5.31
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1020 board
- Personal Computer

Board settings
==============

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
The log below shows the output of the pwm fault demo in the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Welcome to PWM Fault demo
Use oscilloscope to see PWM signal at probe pin: J17-8
Connect pin J17-7 to high level and ground to see change.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
The level state on pin J17-7 determines the output of the PWM signal
- When at low level, the PWM signal will output at pin J17-8.
- When at high level, the PWM signal output will be disabled.

Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
