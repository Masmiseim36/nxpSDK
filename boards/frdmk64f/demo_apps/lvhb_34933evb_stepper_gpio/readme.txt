Overview
========
The purpose of this example project is to demonstrate how to control a stepper motor using dual H-Bridge device (FRDM-34933EVB) and Low Voltage H-Bridge (LVHB) SW Driver via four GPIO pins. The project contains several cases to show how to use driver functions related to stepper motor control.

Hardware requirements
=====================
- FRDM-K64F (MCU freedom board)
- FRDM-34933EVB (H-Bridge freedom board)
- Stepper motor
- External Power Source (according to used stepper motor)
- USB Micro B cable
- Personal Computer

Board settings
==============
Target platform for this example is FRDM-K64F and FRDM-34933EVB. Individual windings of stepper motor should be connected via FRDM-34933EVB J6 a J7 connectors, external power supply via J5 connector. Pin configuration for FRDM-34933EVB: JP2 open, JP3 short.

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
The purpose of this example project is to show a simple demo intended for demonstration of full-stepping mode without acceleration ramp. Full-stepping signals are generated using timer periphery and GPIO pins.

There are several test cases in the demo, which demonstrate full-step mode. Function LVHB_AlignRotor is called at the start of the example to align rotor to a full-step position. Function WaitForCompletion uses polling to wait for completion of motor movement. BasicDemo and AdvancedDemo functions are called in a loop.

1.   Basic demo (function BasicDemo) demonstrates control of motor in full-step mode. Firstly, full-stepping speed is set by function LVHB_SetFullStepSpeed. Then function LVHB_MoveSteps moves motor in forward direction by 100 full-steps. There is no way to start new movement while motor is running. Function WaitForCompletion is used to detect movement completion.

2.   Continual mode (function AdvancedDemo) shows usage of LVHB driver functions related to continual mode. Full-stepping speed is increased and motor runs in continual mode. Note that it is not possible to change speed while motor is running. The movement is started by function LVHB_MoveContinual and continues until function LVHB_StopContinualMovement is called. Note that stepper motor does not stop immediately (i.e. when the LVHB_StopContinualMovement is called), it can execute several steps. Therefore, motor does not have to stop at the same position where it started. Finally, stepper position counter is restarted by LVHB_ResetFullStepPosition and H-Bridge outputs are set to LOW using function LVHB_DisableMotor. Note that motor holds position (coils are powered) when movement is complete and function LVHB_DisableMotor is not called.

See LVHB SW driver documentation for more information about the driver (https://www.nxp.com/support/developer-resources/run-time-software/analog-expert-software-and-tools/sdk-analog-expert-drivers/embedded-sw-low-voltage-h-bridge-software-driver-built-on-sdk:EMBEDDED-SW?). 