Overview
========
The dsp_naturedsp demo application a short self-test of selected NatureDSP functions.

This demo contains two applications:
- cm33/ is the ARM application for Cortex-M33 core
- dsp/ is the DSP application for HiFi4 core

The release configurations of the demo will combine both applications into one ARM
image.  With this, the ARM core will load and start the DSP application on
startup.  Pre-compiled DSP binary images are provided under dsp/binary/ directory.

The debug configurations will build two separate applications that need to be
loaded independently.  The ARM application will power and clock the DSP, so
it must be loaded prior to loading the DSP application.


Toolchain supported
===================
- Xtensa Xplorer  7.0.9
- Xtensa C Compiler  12.0.8

Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- EVK-MIMXRT685 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the debug USB port (J5) on the board
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program for CM33 core to the target board.
4.  Launch the debugger in your IDE to begin running the demo.
6.  If building debug configuration, download the program for DSP core to the target board.
7.  If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to
begin running the demo.

NOTE: DSP image can only be debugged using J-Link debugger.  See
'Getting Started with Xplorer for EVK-MIMXRT685.pdf' for more information.

Running the demo CM33
=====================
When the demo runs successfully, the terminal will display the following:

    NatureDSP demo start
    Going to start DSP core...

The terminal is then deinitialized so it can be used by DSP.

Running the demo DSP
====================
When the demo runs successfully, the following output will be appended to the terminal:

    Running NatureDSP library on DSP core
    NatureDSP library version: 4.0.0
    NatureDSP library API version: 4.10

    /*FFT TEST START*/
    FFT 256 takes 3452 cycles
    /*FFT TEST END with 0*/

    /*VECTOR DOT TEST START*/
    VECTOR DOT 16 takes 489 cycles
    /*VECTOR DOT TEST END with 0 */

    /*VECTOR ADD TEST START*/
    VECTOR ADD 32 takes 527 cycles
    /*VECTOR ADD TEST END with 0 */

    /*VECTOR MAX TEST START*/
    VECTOR MAX 1024f takes 15029 cycles
    /*VECTOR MAX TEST END with 0 */

    /*IIR TEST START*/
    IIR 32 1024 takes 270905 cycles
    /*IIR TEST END with 0 */

    /*FIR BLMS TEST START*/
    FIR BLMS 64 64 takes 45192 cycles
    /*FIR BLMS TEST END with 0 */

Customization options
=====================

