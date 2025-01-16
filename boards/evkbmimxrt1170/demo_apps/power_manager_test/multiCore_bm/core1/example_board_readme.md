Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVKB board
- Personal Computer
- USB to Com Converter

Board settings
============
No special settings are required.

Prepare the Demo
===============
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
When running the demo, the debug console shows the menu to provide the desired operation.

Power Manager Multi-Core Bare-metal Demo.

Primary Core Boot.
Previous setpoint is 0.
Current setpoint is 0.
Starting Second Core...
Copy Secondary core image to address: 0x20200000, size: 11713
Second Core application has been started.

Please select the desired power mode:
        Press A to enter: Setpoint0, CM7 domain WAIT, CM4 domain WAIT
        Press B to enter: Setpoint1, CM7 domain STOP, CM4 domain WAIT
        Press C to enter: Setpoint10, CM7 domain SUSPEND, CM4 domain SUSPEND
        Press D to enter: Setpoint15, CM7 domain SUSPEND, CM4 domain SUSPEND

Waiting for power mode select...


Note:
To debug in external flash, following steps are needed:
1. Select the flash target and compile.
3. Set the SW1: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J11.
4. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to external flash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
   
Note:
Flexspi_nor_debug and Flexspi_nor_release targets on armgcc toolchain need to click SW4(reset button) to run cases after downloading the image.