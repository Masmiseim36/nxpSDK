Overview
========

Percepio_snapshot demo shows the basic capabilities of Percepio tool.
Required Segger software 
    - J-Link Software and Documentation Pack ( https://www.segger.com/downloads/jlink )
    - J-Link OpenSDA - Board-Specific Firmwares ( https://www.segger.com/downloads/jlink )
    - Tracealyzer for FreeRTOS ( http://percepio.com/download/ )

Demo requires JLink connected. The JLink can be external probe or on board SDA debugger.
In case of SDA debugger you have to change firmware to J-Link OpenSDA by holding board RESET 
button while power up board. The board is then detected as USB mass storage device and it's 
required to copy the .bin firmware to the device. When it's done you have to reset the board 
once more.


Toolchain supported
===================
- GCC ARM Embedded  8.2.1
- IAR embedded Workbench  8.32.3
- Keil MDK  5.27
- MCUXpresso  11.0.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.
Prepare the Demo
================

Running the demo
================

1) Connect board default console with PC. Set the JLink as current debugger in your IDE and build the demo.
When demo runs, you should see the address and size of "RecorderData" on the terminal.

2) Launch Percepio Tracealyzer, click on "JLink -> JLink Settings" menu and setup "Debugger Interface" and "Target Device".
In "Jlink -> Memory Region" please fill "Start Address" and "Bytes to read" with data printed in step (1).
Now you can click on "JLink -> Read Trace (Snapshot)" to get the visualization of tasks.

Customization options
=====================

