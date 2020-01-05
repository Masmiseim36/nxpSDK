Overview
========
The purpose of this example project is to demonstrate how to control a stepper motor with enabled acceleration ramp using dual H-Bridge device (FRDM-34933EVB) and Low Voltage H-Bridge (LVHB) SW Driver. The project contains several cases to show how to use most of driver functions related to stepper motor control.

Doing a small change of main() function, following H-Bridge EVBs can be used instead of FRDM-34933EVB:
- FRDM-17529EVB
- FRDM-17531AEJEVB
- FRDM-17531AEPEVB
- FRDM-17C724EVB 

Hardware requirements
=====================
- FRDM-KL27Z
- FRDM-34933EVB (eventually FRDM-17529EVB, FRDM-17531AEJEVB, FRDM-17531AEPEVB or FRDM-17C724EVB)
- Stepper motor
- External Power Source (according to used stepper motor and H-bridge device maximum voltage)
- USB Mini B cable

Board settings
==============
Target platform for this example is FRDM-KL27Z as MCU board and FRDM-34933EVB as H-Bridge device board. Doing a small change in main() function following EVBs can be used instead of FRDM-34933EVB: FRDM-17529EVB, FRDM-17531AEJEVB, FRDM-17531AEPEVBor FRDM-17C724EVB.
Individual windings of stepper motor should be connected via FRDM LVHB J6 a J7 connectors, external power supply via J5 connector.
Pin configuration for FRDM LVHB: JP2 open, JP3 short.
Be sure that jumper J26 is open on FRDM-KL27Z board!

Prepare the Demo
================
1.  File main.c is implemented in order to support FRDM-34933EVB board. In case of any other supported LVHB board, do the following changes in main() function:
      - Change the 2nd parametr of LVHB_GetDefaultConfig function according to the utilized LVHB device.
      - Uncomment "LVHB_SetMode(&drvConfig, true);" command to enable the LVHB device with EN/OE/PSAVE pin.
      - Uncomment "LVHB_SetMode(&drvConfig, false);" command to disable the LVHB device with EN/OE/PSAVE pin in case of error in execution.
2.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The purpose of this example project is to show a simple demo intended for demonstration of full-stepping and micro-stepping mode with acceleration ramp. Full-stepping and micro-stepping signals are generated using timer periphery.

There are several test cases in the demo, which demonstrate full-step and micro-step mode with acceleration ramp. Function LVHB_AlignRotor is called at the start of the example to align rotor to a full-step position. Function WaitForCompletion uses polling to wait for completion of motor movement. When rotor is aligned, example contains never ending loop. Description of test cases follows:

1.   Firstly LVHB_MoveSteps function runs motor in forward direction. Motor executes 800 full-steps. It is not possible to start new movement while motor is running. Function LVHB_GetMotorStatus is used to poll motor status. You can also use LVHB_OnActionComplete event handler instead.

2.   Micro-step size is changed to 8 micro-steps per full-step by LVHB_SetMicroStepSize and stepper performs 3200 micro-steps in reversed direction (i.e. 3200 / 8 = 400 full-steps) by function LVHB_MoveMicroSteps.

3.   Now micro-step size is set to 2 micro-steps per one full-step and motor executes 800 micro-steps.

4.   Full-stepping speed is incremented by 100 in each loop. Note that size of micro-step is taken into account when micro-stepping speed is set (micro-step_speed = full-step_speed * size_of_micro-step). Full-step speed is changed by function LVHB_SetFullStepSpeed and micro-step speed is set by LVHB_SetMicroStepSpeed.

Any error is signalized by the red LED on MCU board.

See LVHB SW driver documentation for more information about the driver (https://www.nxp.com/support/developer-resources/run-time-software/analog-expert-software-and-tools/sdk-analog-expert-drivers/embedded-sw-low-voltage-h-bridge-software-driver-built-on-sdk:EMBEDDED-SW?). 