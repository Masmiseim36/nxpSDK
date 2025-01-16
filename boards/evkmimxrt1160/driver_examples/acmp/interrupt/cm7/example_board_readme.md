Hardware requirements
=====================
- Micro USB cable
- MIMXRT1160-EVK board
- Personal Computer

Board settings
==============
This example project uses ACMP instance 1 to compare the voltage signal input from External Input2(J25-13)
with the voltage signal(half of VDDA_1P8_IN) output by ACMP's internal DAC. Terminal will print information
corresponding to different comparison result.
Please note that the input voltage should in the range of 0 to 1.8V.
Connect ACMP External Input2(J25-13) to stable external voltage generator to avoid floating voltage. 
The example serial port output may be frequent change otherwise.

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
================
1. Connect 5V power supply and J-Link Debug Probe to the board, switch SW5 to power on the board.
2. Connect a micro USB cable between the host PC and the J11 USB port on the target board.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
If the input voltage input is in the range of 0.9V to 1.8V, that means the analog input is higher than DAC output.
If the input voltage input is in the range of 0V to 0.9V, that means the analog input is lower than DAC output. 

When the demo runs successfully, following information can be seen on the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The example compares analog input to the reference DAC output(CMP positive port).

The terminal will print CMP's output value when press any key.

Please press any key to get CMP's output value.

The analog input is LOWER than DAC output

The analog input is HIGHER than DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
