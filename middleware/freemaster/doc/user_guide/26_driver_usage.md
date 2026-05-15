# Driver usage

Start using or evaluating FreeMASTER by opening some of the example applications available in the driver setup package.

Follow these steps to enable the basic FreeMASTER connectivity in the application:

- Make sure that all *\*.c* files of the FreeMASTER driver from the *src/common/platforms/[your_platform]* folder are a part of the project. See [Driver files](driver-files) for more details.

- Configure the FreeMASTER driver by creating or editing the *freemaster_cfg.h* file and by saving it into the application project directory. See [Driver configuration](driver-configuration) for more details.

- Include the *freemaster.h* file into any application source file that makes the FreeMASTER API calls.

- Initialize the Serial or CAN modules. Set the baud rate, parity, and other parameters of the communication. Do not enable the communication interrupts in the interrupt mask registers.

- For the FMSTR_LONG_INTR and FMSTR_SHORT_INTR modes, install the application-specific interrupt routine and call the FMSTR_SerialIsr or FMSTR_CanIsr functions from this handler.

- Call the FMSTR_Init function early on in the application initialization code.

- Call the FMSTR_RecorderCreate functions for each Recorder instance to enable the Recorder feature.

- In the main application loop, call the FMSTR_Poll API function periodically when the application is idle.

- For the FMSTR_SHORT_INTR and FMSTR_LONG_INTR modes, enable the interrupts globally so that the interrupts can be handled by the CPU.
