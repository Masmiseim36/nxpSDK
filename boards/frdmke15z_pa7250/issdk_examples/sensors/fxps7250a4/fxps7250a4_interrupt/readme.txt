Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- FRDM-STBA-PA7250 shield board
- Personal Computer

Board settings
==============
Overview
========
The FXPS7250A4 Interrupt example application demonstrates the use of the FXPS7250A4 analog sensor with in ADC.
The example demonstrates configuration of ADC0 and LPTMR0 reguired to get the Pressure Sample and read out samples at periodic intervals.

The sensor constantly provides analog volatage output based on the perceived pressure differential.
The application configures LPTMR with the desired ODR and the ADC for voltage conversion and sends the CPU into wait mode.
The LPTMR periodically triggers ADC to read the voltage and convert it into a value.
Once the ADC completes conversion, it raises an interrupt and the application wakes up and displays the Pressure.

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
${ANCHOR}
