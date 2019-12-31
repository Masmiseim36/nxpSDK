Overview
========
The application implements a Hybrid Bluetooth - Generic FSK demo. 
The example works on the FRDM-KW36 board. For more information, please consult the BLE Demo Applications User's Guide.

Toolchain supported
===================
- IAR Embedded Workbench 8.32.2
- MCUXpresso IDE 10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KW36 board

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a mini/micro USB cable between the PC host and the OpenSDA USB port on the board.
3.  Download the program to the target board.
4.  Press the reset button on your board to begin running the demo.
5.  Switches functionality:
      SW2 - start Bluetooth LE scanning;
      Long SW2  -  start Bluetooth advertising;
      SW3 - start/stop GFSK TX operation (advertising);
      Long SW3 - start/stop GFSK RX operation;
6.  Open a serial terminal application and use the following settings with the the detected serial device:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

Running the demo
================
Use a serial terminal to interact with the device.
