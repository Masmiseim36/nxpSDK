Overview
========
This GUI demo is developed with the Embedded Wizard GUI solution.

The sample application 'PulseOximeter' shows the implementation of a medical 
device for monitoring a person's pulse frequency and peripheral oxygen 
saturation. The application demonstrates the usage of vector graphics 
within graphs and circular gauges.

The application provides the following screens / operation modes:
A main screen shows the current pulse frequency in bpm and the current 
peripheral oxygen saturation in %SpO2. The values are shown with a circular 
gauge, a numerical display and a small graph.
A graph screen presents the history of measured values as graphs with bézier 
lines and let the user slide through. A settings screen for oxygen and pulse 
let the user configure the device. A menu screen is prepared for additional 
settings.
This examples runs bare metal, but Embedded Wizard is also possible
to use with FreeRTOS. Our official Build Environments, that can be downloaded 
as described above, includes examples and demos that illustrate the integration 
of FreeRTOS and can be used for your reference.



In order create your own GUI application powered by the latest version of
Embedded Wizard that is suitable for the i.MX RT1170 Evaluation Kit, please
visit https://doc.embedded-wizard.de/getting-started-mimxrt1170-evk

For this example a simulation of the user interface is also available.
It can be used under following link
http://www.embedded-wizard.de/NXP-SDK-Examples/IMXRT1170-EVK/GraphicsAccelerator/Simulation/
with any web browser. The simulation uses the JS/WebGL Embedded Wizard Platform Package.
For further information about Embedded Wizard, please visit our web site at
https://www.embedded-wizard.de/platforms/nxp/imx-rt1170


Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- GCC ARM Embedded  10.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1170-EVK board
- Personal Computer
- RK055MHD091 panel or RK055AHD091 panel or RK055IQH091 panel

Board settings
==============
1. Connect the LCD panel to J48.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, the project uses RK055MHD091 by default, to use other panels, change #define DEMO_PANEL DEMO_PANEL_RK055MHD091 to #define DEMO_PANEL DEMO_PANEL_RK055IQH091 or #define DEMO_PANEL DEMO_PANEL_RK055AHD091 in display_support.h.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
If this example runs correctly, the sample application is displayed on the LCD screen.
The log below shows the output of the sample application 'PulseOximeter' in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initialize Realtime Clock...                 [OK]
Initialize Display...                        [OK]
Initialize Touch Driver...                   [OK]
Initialize Memory Manager...                 [OK]
Initialize Graphics Engine...                [OK]
Create Embedded Wizard Root Object...        [OK]
Create Embedded Wizard Viewport...           [OK]
---------------------------------------------
Target system                                IMXRT1170-EVK
Color format                                 RGB565
MemoryPool address                           0x80384000
MemoryPool size                              46645248 bytes
Framebuffer address                          0x80000000
Doublebuffer address                         0x801C2000
Framebuffer size                             1280 x 720
EwScreeenSize                                1280 x 720
Graphics accelerator                         VGLite
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
Glyph cache size                             512 x 512
Max issue tasks                              256
Surface rotation                             0
---------------------------------------------
