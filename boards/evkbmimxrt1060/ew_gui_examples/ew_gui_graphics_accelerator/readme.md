Overview
========
This GUI demo is developed with the Embedded Wizard GUI solution.

The intention of this example is to demonstrate the graphics performance.
For this purpose, a set of basic drawing operations are executed permanently and
continuously:
As a result, an endless stream of drawing instructions is produced...
In case that the graphics hardware provides a graphics accelerator, it can be
switched on/off to demonstrate the advantage of a graphics accelerator.
The user can choose the current graphics operation and switch on/off the usage
of the graphics acclerator (if available).

The resulting framerate (in fps) and the resulting CPU load (in percent) will be
shown.

This examples runs bare metal, but Embedded Wizard is also possible
to use with FreeRTOS. Our official Build Environments, that can be downloaded
as described above, includes examples and demos that illustrate the integration
of FreeRTOS and can be used for your reference.



In order create your own GUI application powered by the latest version of
Embedded Wizard that is suitable for the i.MX RT1060 Evaluation Kit, please
visit https://doc.embedded-wizard.de/getting-started-mimxrt1060-evk

For this example a simulation of the user interface is also available. 
It can be used under following link 
http://www.embedded-wizard.de/NXP-SDK-Examples/IMXRT1060-EVK/GraphicsAccelerator/Simulation/ 
with any web browser. The simulation uses the JS/WebGL Embedded Wizard Platform Package.
For further information about Embedded Wizard, please visit our web site at
https://www.embedded-wizard.de/platforms/nxp/imx-rt


SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- IAR embedded Workbench  9.40.1
- GCC ARM Embedded  12.2
- Keil MDK  5.38.1
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Micro USB cable
- IMXRT1060-EVK board
- RK043FN02H-CT or RK043FN66HS-CT6 LCD board
- Personal Computer

Board settings
==============
1. Connect the RK043FN02H-CT or RK043FN66HS-CT6 to board.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, the project uses RK043FN66HS-CT6 by default, to use other panels,
    change
    #define DEMO_PANEL DEMO_PANEL_RK043FN66HS
    to
    #define DEMO_PANEL DEMO_PANEL_RK043FN02H
    in ew_bsp_touch.h
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
If this example runs correctly, the sample application is displayed on the LCD screen.
The log below shows the output of the sample application 'GraphicsAccelerator' in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initialize Realtime Clock...                 [OK]
Initialize Display...                        [OK]
Initialize Touch Driver...                   [OK]
Initialize Memory Manager...                 [OK]
Initialize Graphics Engine...                [OK]
Create Embedded Wizard Root Object...        [OK]
Create Embedded Wizard Viewport...           [OK]
---------------------------------------------
Target system                                IMXRT1060-EVK
Color format                                 RGB565
MemoryPool address                           0x8107F800
MemoryPool size                              14157824 bytes
Framebuffer address                          0x81000000
Doublebuffer address                         0x8103FC00
Framebuffer size                             480 x 272
EwScreeenSize                                480 x 272
Graphics accelerator                         PXP
Vector graphics support                      enabled
Warp function support                        enabled
Index8 bitmap resource format                enabled
RGB565 bitmap resource format                enabled
Bidirectional text support                   enabled
Operating system                             none
External flash device                        QSPI Flash
Toolchain                                    <Toolchain name>
C-Compiler version                           <C-Compiler version>
Build date and time                          <MMM DD YYYY, HH:MM:SS>
Runtime Environment (RTE) version            10.00
Graphics Engine (GFX) version                10.00
Max surface cache size                       8388608 bytes
Glyph cache size                             256 x 256
Max issue tasks                              100
Surface rotation                             0
---------------------------------------------
