Overview
========
This GUI demo is developed with the Embedded Wizard GUI solution.

The sample application 'WaveformGenerator' is focused on the
calculation and presentation of dynamic data as graph diagramm
by using the Graphics::Path and Views::StrokePath classes.

Within this example, two data sources ("Channel1" and "Channel2") can
be configured to provide a sine, square, ramp or noise signal with a
certain amplitude and frequency. These two signals are then added and
used to calculate the path of the graph. Each time the signal has
changed, the entire path is calulated and the waveform is shown
automatically.

This examples runs bare metal, but Embedded Wizard is also possible
to use with FreeRTOS. Our official Build Environments, that can be downloaded
as described above, includes examples and demos that illustrate the integration
of FreeRTOS and can be used for your reference.



In order create your own GUI application powered by the latest version of
Embedded Wizard that is suitable for the i.MX RT1010 Evaluation Kit, please
visit https://doc.embedded-wizard.de/getting-started-mimxrt1010-evk

For this example a simulation of the user interface is also available.
It can be used under following link
http://www.embedded-wizard.de/NXP-SDK-Examples/IMXRT1010-EVK/WaveformGenerator/Simulation/
with any web browser. The simulation uses the JS/WebGL Embedded Wizard Platform Package.
For further information about Embedded Wizard, please visit our web site at
https://www.embedded-wizard.de/platforms/nxp/imx-rt


Toolchain supported
===================
- IAR embedded Workbench  9.40.1
- GCC ARM Embedded  12.2
- Keil MDK  5.38.1
- MCUXpresso  11.8.0

Hardware requirements
=====================
- Micro USB cable
- IMXRT1010-EVK board
- Personal Computer
- Adafruit 1947 display

Board settings
==============
There are different versions of Adafruit 2.8" TFT LCD shields. The shields marked
v2.0 works directly with this project. For the other shields, please solder
the center pin of IOREF pads to the 3.3V pad, and solder the pads 11, 12, and 13.
See the link for details:
https://community.nxp.com/t5/MCUXpresso-Community-Articles/Modifying-the-latest-Adafruit-2-8-quot-LCD-for-SDK-graphics/ba-p/1131104

- Attach the LCD shield to the board.
- Weld 0Ω resistor to R795.
- Connect jumper J21.
(Note: If we do not connect jumper J21, LCD may not display correctly when debugging
because it can't get reset signal.)

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the
    target board.
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
The log below shows the output of the sample application 'WaveformGenerator' in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initialize Display...                        [OK]
Initialize Touch Driver...                   [OK]
Initialize Memory Manager...                 [OK]
Initialize Graphics Engine...                [OK]
Create Embedded Wizard Root Object...        [OK]
Create Embedded Wizard Viewport...           [OK]
---------------------------------------------
Target system                                IMXRT1010-EVK
Color format                                 RGB565
MemoryPool address                           0x20200000
MemoryPool size                              49152 bytes
Scratch-pad buffer address                   0x20000714
Scratch-pad buffer size                      7680 pixel
Framebuffer size                             320 x 24
EwScreenSize                                 320 x 240
Graphics accelerator                         none
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
Runtime Environment (RTE) version            10.00
Graphics Engine (GFX) version                10.00
Max surface cache size                       0 bytes
Glyph cache size                             64 x 128
Max issue tasks                              40
Surface rotation                             0
---------------------------------------------
