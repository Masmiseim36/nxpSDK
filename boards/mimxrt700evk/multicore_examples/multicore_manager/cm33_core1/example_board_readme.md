Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect the PC host and the board
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
The log below shows the output of the hello world multicore demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hello World from the Primary Core!

Starting Secondary core.
The secondary core application has been started.

Press the SW5 button to Stop Secondary core.
Press the SW7 button to Start Secondary core again.
When no action is taken the secondary core application crashes intentionally after 100 LED toggles (simulated exception), generating the RemoteExceptionEvent to this core.
Use the Stop and then the Start button to get it running again.

.
.
.

Secondary core cannot be stopped!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

> **_NOTE: #1_**:
> The MIMXRT700-EVK board does not support stopping of secondary code.
> After pressing the button prints that secondary core cannot be stopped.
> Only way to start the secondary core again is POR.
