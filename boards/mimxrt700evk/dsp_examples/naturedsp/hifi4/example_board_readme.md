Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT700-EVK board
- Personal Computer

Board settings
==============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the debug USB port (J54) on the board
2.  Open a serial terminal on PC for MCU-LINK serial device with these settings:
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
'Getting Started with Xplorer for MIMXRT700-EVK.pdf' for more information.

Running the demo CM33
===============
When the demo runs successfully, the terminal will display the following:

    NatureDSP demo start
    Going to start DSP core...

The terminal is then deinitialized so it can be used by DSP.

Running the demo DSP
===============
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

