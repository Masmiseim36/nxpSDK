Overview
========
The purpose of this example project is to demonstrate how to control a DC brushed motor using single H-Bridge device (FRDM-17510EVB) and the Low Voltage H-Bridge (LVHB) SW Driver. The project contains several cases to show how to use most of the driver functions related to DC brushed motor control.

Doing a small change of main() function, FRDM-17511EVB can be used instead of FRDM-34933EVB.

Hardware requirements
=====================
- FRDM-K64F
- FRDM-17510EVB (eventually FRDM-17511EVB)
- DC Motor (2-15V, 2-6.8V in case of MPC17511)
- External Power Source (according to DC motor supply voltage)
- USB Micro B cable
- Personal Computer

Board settings
==============
Target platform for this example is FRDM-K64F and FRDM-17510EVB.
Motor should be connected via FRDM-17510EVB J6 connector, external power supply via J5 connector.
Pin configuration for FRDM-17510EVB: JP2 open, JP3 short.

Prepare the Demo
================
1.  File main.c is implemented in order to support FRDM-17510EVB board. In case of FRDM-17511EVB, change the second parametr of LVHB_GetDefaultConfig function in main() function to lvhbDeviceMPC17511.
2.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The demo consists of several test cases:

1.   Setting H-Bridge output to tri-state – the motor is running when function LVHB_SetTriState is called. You can see that motor slowly stops. Motor brake (H-Bridge outputs to LOW) follows so you can compare immediate and slow stopping.

2.   Disabling H-Bridge device – the motor is running when H-Bridge device is disabled using function LVHB_SetMode. Motor stops immediately. Then the device mode is set back to normal. The method uses EN pin.

3.   Control of GOUT H-Bridge output – value of GOUT output is initialized to LOW by default. In this test case function LVHB_SetGateDriver sets the output to HIGH and after a while back to LOW state. The other H-Bridge outputs are not influenced.

4.   Control of motor speed – PWM duty is increasing to reach value 100% (the highest speed). Then the duty is decreased to 0%. Motor direction is reversed and the procedure described above is repeated. Function LVHB_RotateProportional is used in this test case.

See LVHB SW driver documentation for more information about the driver (https://www.nxp.com/support/developer-resources/run-time-software/analog-expert-software-and-tools/sdk-analog-expert-drivers/embedded-sw-low-voltage-h-bridge-software-driver-built-on-sdk:EMBEDDED-SW?). 