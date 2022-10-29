Overview
========
The AFE Example project is a demonstration program that uses the KSDK software.
In this example, the AFE module samples the voltage difference of the EXT_SD_ADP0 and EXT_SD_ADM0 pins.

Toolchain supported
===================
- IAR embedded Workbench  9.30.1
- Keil MDK  5.37
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.6.0

Hardware requirements
=====================
- J-Link ARM
- P&E Micro Multi-link universal
- Micro USB cable
- USB A to micro AB cable
- Hardware (tower/base board, ...) for specific device
- Personal Computer

Board settings
==============
This example requires one board and one DC Power Supply.
Connect the DC Power Supply output signals to AFE differential sample pins:

On TWR-KM35Z75M board:
Pin 45 of QFP144/AFE_SDADP0 (J31 Pin 2) -> (J21 Pin 2)
Pin 46 of QFP144/AFE_SDADM0 (J31 Pin 4) -> (J31 Pin 3)

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AFE interrupt example.

For example, roll the potentiometer to see value changes on terminal screen. If value of potentiometer output lager than 500mV,
result will return maximum of 24 bit (8388607).
The printed log is displayed in the terminal window as shown below.

AFE Interrupt example.
Press any key to trigger AFE conversion
AFE value  = 33456
Press any key to trigger AFE conversion
AFE value  = 31110
Press any key to trigger AFE conversion
AFE value  = 95880
Press any key to trigger AFE conversion
AFE value  = 17442
Press any key to trigger AFE conversion
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
