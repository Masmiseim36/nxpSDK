Overview
========

Segger Sysview demo shows the basic capabilities of Segger System Viewer tool.
Required software:
    - J-Link Software and Documentation Pack
    - SystemView - Real-time analysis and visualization
    - J-Link OpenSDA - Board-Specific Firmwares
can be downloaded from https://www.segger.com/downloads/jlink

Demo requires JLink connected. The JLink can be external probe or on board SDA debugger.
In case of SDA debugger you have to change firmware to J-Link OpenSDA by holding board RESET 
button while power up board. The board is then detected as USB mass storage device and it's 
required to copy the .bin firmware to the device. When it's done you have to reset the board 
once more.

Running the demo
================

1) Connect board default console with PC. Set the JLink as current debugger in your IDE and build the demo.
When demo is running, you should see on the terminal the address of "RTT block".
The RTT block address is location of '_SEGGER_RTT' variable.

2) Launch System Viewer tool and click "Target - Start Recording". In modal window set the target 
interface. Automatic detection might not work so switch "RTT Control Block Detection" to "address"
and put the address from step (1) into the text field.

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K66F board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

