Overview
========
The SNVS SW ZMK project shows how to provision the zeroizable master key (ZMK)
by software based on SDK SNVS driver. In this example, ZMK key value is set and
ECC is enabled. When change the ZMK key value, violation detected and ZMK key
is zeroized.
Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer

Board settings
==============
No special settings are required.

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
The log below shows the output of snvs example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SNVS software Zeroizable Master Key example:
Set the ZMK key value
Enable the ZMK
Enable the ZMK ECC
Set new ZMK key to generate ZMK violation
ZMK violation detected, ZMK key is zeroized

SNVS software ZMK example finished successfully
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

