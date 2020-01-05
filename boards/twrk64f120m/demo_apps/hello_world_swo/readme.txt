Overview
========
The Hello World SWO demo prints the "SWO: Hello World" string to the SWO viewer. The purpose of this demo is to
show how to use the swo, and to provide a simple project for debugging and further development.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K64F120M board
- Personal Computer
- Jlink plus or Jlink lite

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.	Connect a Jlink plus or Jlink lite probe to the JTAG connector J5.
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.	Open JlinkerSWOView(C:\Program Files (x86)\SEGGER\JLink_xxx), select the target device, such as MK64FN1M0XXX12 and make sure the core clock and swo clock frequency is equal to the demo setting, the demo use 100KHZ as SWO clock frequency and 120MHZ as core clock frequency.
5. 	After swo/core clock measure successfully, press ok to continue, make sure the bit 0 is selected in data from stimulus port item.
6	Press SW3.

Running the demo
================
The log below shows the output of the hello world swo demo in the JlinkerSWOView window and the demo will continuous print it with SW3 pressed:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SWO: hello_world
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

