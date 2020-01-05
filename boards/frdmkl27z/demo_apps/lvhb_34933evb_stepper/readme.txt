Overview
========
The purpose of this example project is to demonstrate how to control a stepper motor using dual H-Bridge device (FRDM-34933EVB) and Low Voltage H-Bridge (LVHB) SW Driver via four PWM pins. The project contains several cases to show how to use most of driver functions related to stepper motor control (including micro-stepping).

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
The purpose of this example project is to show a simple demo intended for demonstration of full-stepping and micro-stepping mode without acceleration ramp. Full-stepping and micro-stepping signals are generated using timer periphery.

There are several test cases in the demo, which demonstrate full-step and micro-step mode. Function LVHB_AlignRotor is called at the start of the example to align rotor to a full-step position. Function WaitForCompletion uses polling to wait for completion of motor movement. BasicDemo, AdvancedDemo and SetMicroStepSizeDemo functions are called in a loop.

1.   Basic demo (function BasicDemo) demonstrates control of motor in full-step and micro-step mode. Firstly, full-stepping and micro-stepping speed is set by functions LVHB_SetFullStepSpeed and LVHB_SetMicroStepSpeed. Then, micro-step size is set to 2 micro-steps per one full-step by function LVHB_SetMicroStepSize. Function LVHB_MoveSteps moves motor in forward direction by 100 full-steps. There is no way to start new movement while motor is running. Function WaitForCompletion is used to detect movement completion. Demonstration of micro-stepping follows, function LVHB_MoveMicroSteps starts movement and rotor should return back to its initial position.

2.   Continual mode (function AdvancedDemo) shows usage of LVHB driver functions related to continual mode. Full-stepping speed is increased and motor runs in continual mode. Note that it is not possible to change speed while motor is running. The movement is started by function LVHB_MoveContinual and continues until function LVHB_StopContinualMovement is called. Second part of the function AdvancedDemo is similar to the first one, motor speed is only decreased by micro-stepping movement. Note that stepper motor does not stop immediately (i.e. when the LVHB_StopContinualMovement is called), it can execute several steps. Therefore, motor does not have to stop at the same position where it started. Finally, stepper position counter is restarted by LVHB_ResetFullStepPosition and H-Bridge outputs are set to LOW using function LVHB_DisableMotor. Note that motor holds position (coils are powered) when movement is complete and function LVHB_DisableMotor is not called.

3.   Micro-step size demo (function SetMicroStepSizeDemo) shows how to change size of micro-step. Micro-step size is set to values: 2, 4, 8, 16 and 32. Motor runs in micro-step mode when the size is adjusted.

Any error is signalized by the red LED on MCU board.

See LVHB SW driver documentation for more information about the driver (https://www.nxp.com/support/developer-resources/run-time-software/analog-expert-software-and-tools/sdk-analog-expert-drivers/embedded-sw-low-voltage-h-bridge-software-driver-built-on-sdk:EMBEDDED-SW?). 