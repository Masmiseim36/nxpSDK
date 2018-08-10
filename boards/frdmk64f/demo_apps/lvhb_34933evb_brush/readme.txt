Overview
========
The purpose of this example project is to demonstrate how to control two DC brushed motor using dual H-Bridge device (FRDM-34933EVB) and the Low Voltage H-Bridge (LVHB) SW Driver. The project contains several cases to show how to use most of the driver functions related to DC brushed motor control.

Hardware requirements
=====================
- FRDM-K64F (MCU freedom board)
- FRDM-34933EVB (H-Bridge freedom board)
- Two DC Motors (2-7V)
- External Power Source (according to DC motor supply voltage)
- USB Micro B cable
- Personal Computer

Board settings
==============
Target platform for this example is FRDM-K64F and FRDM-34933EVB. Motors should be connected via FRDM-34933EVB J6 a J7 connectors, external power supply via J5 connector. Pin configuration for FRDM-34933EVB: JP2 open, JP3 short.

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
Pins are configured as follows: IN1B pin is set as timer output. Other IN pins are controlled by GPIO. It enables speed adjustment of the first motor connected to H-Bridge interface 1 (OUT1A and OUT1B outputs). The second motor connected to interface 2 (OUT2A and OUT2B outputs) can be only turned on/off.

The demo consists of several test cases:

1.   Setting H-Bridge output to tri-state – the motor connected to H-bridge interface 1 is running when function LVHB_SetTriState is called. You can see that motor slowly stops. Motor brake (H-Bridge outputs to LOW) follows so you can compare immediate and slow stopping. The same procedure is done with the second motor on interface 2.

2.   Control of motor connected to interface 1 – PWM duty is increasing to reach value 100% (the highest speed). Then the duty is decreased to 0%. Motor direction is reversed and the procedure described above is repeated. Function LVHB_RotateProportional is used in this test case.

3.   Control of motor connected to interface 2 – motor is turned on and then back to off. This is repeated in reversed direction. This test case uses method LVHB_RotateFull.

See LVHB SW driver documentation for more information about the driver (https://www.nxp.com/support/developer-resources/run-time-software/analog-expert-software-and-tools/sdk-analog-expert-drivers/embedded-sw-low-voltage-h-bridge-software-driver-built-on-sdk:EMBEDDED-SW?). 