Overview
========
This GUI demo is developed with the Embedded Wizard GUI solution.

The sample application 'BrickGame' implements a classic "paddle and ball" game.
In the game, a couple of brick rows are arranged in the upper part of the screen.
A ball travels across the screen, bouncing off the top and side walls of the screen.
When a brick is hit, the ball bounces away and the brick is destroyed. The player
loses a turn when the ball touches the bottom of the screen. To prevent this from
happening, the player has a movable paddle to bounce the ball upward, keeping it in play.
This demo game is implemented for a screen size of 480x272 pixel.

In this example a simulation of the user interface is integrated as well. It can
be executed with the MCUXpresso's built-in web browser. The simulation is
located in the folder 'ew_gui_examples\<example_name>\Simulation'. The
simulation uses the JS/WebGL Embedded Wizard Platform Package.


In order create your own GUI application powered by the latest version of
Embedded Wizard that is suitable for the i.MX RT1064 Evaluation Kit, please
visit https://doc.embedded-wizard.de/getting-started-mimxrt1064-evk

For further information about Embedded Wizard, please visit our web site at
https://www.embedded-wizard.de/platforms/nxp/imx-rt


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- GCC ARM Embedded  8.3.1
- Keil MDK  5.29
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer
- RK043FN02H-CT LCD board

Board settings
==============
1. Connect the RK043FN02H-CT board to J8.

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
If this example runs correctly, the sample application is displayed on the LCD screen.
The log below shows the output of the sample application 'BrickGame' in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initialize Realtime Clock...                 [OK]
Initialize Display...                        [OK]
Initialize Touch Driver...                   [OK]
Initialize Memory Manager...                 [OK]
Initialize Graphics Engine...                [OK]
Create Embedded Wizard Root Object...        [OK]
Create Embedded Wizard Viewport...           [OK]
---------------------------------------------
Target system                                IMXRT1064-EVK
Color format                                 RGB565
MemoryPool address                           0x8107F800
MemoryPool size                              16254976 bytes
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
External flash device                        none
Toolchain                                    <Toolchain name>
C-Compiler version                           <C-Compiler version>
Build date and time                          <MMM DD YYYY, HH:MM:SS>
Runtime Environment (RTE) version            9.20
Graphics Engine (GFX) version                9.20
Max surface cache size                       2097152 bytes
Glyph cache size                             256 x 256
Max issue tasks                              100
Surface rotation                             0
---------------------------------------------
Customization options
=====================

