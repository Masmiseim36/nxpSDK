Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT1064 board
- Personal Computer
- Jlink

Board settings
============
1. Remove jumper of J47, J48, J49, J50

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. This is used only to power the board.
2.  Connect Jlink to J21 as debugger and SWO probe.
3.  Download the program to the target board. This step needs to change the project settings according to the IDE used, please refer to AN13234.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world swo demo in the JlinkerSWOView window and the demo will print output periodically or when SW8 is pressed:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SWO: timer_trigger
SWO: timer_trigger
SWO: timer_trigger
SWO: hello_world
SWO: timer_trigger
SWO: timer_trigger
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
   This demo targets to run SWO trace function in IDE debug mode and none-debug mode.
   In none-debug mode, may use below command(swoattach mode) with Jlink:
   c:/Program Files/SEGGER/JLink/JLinkSWOViewerCL.exe -device MIMXRT1064XXX6A -itmport 0 -swoattach 1 -swofreq 4000000

Note:
To download binary into qspiflash and boot from qspiflash directly, following steps are needed:
1. Compile flash target of the project, and get the binaray file "hello_world_swo.bin".
3. Set the SW7: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
4. Drop the binaray into disk "RT1064-EVK" on PC.
5. Wait for the disk disappear and appear again which will take couple of seconds.
7. Reset the board by pressing SW3 or power off and on the board. 
