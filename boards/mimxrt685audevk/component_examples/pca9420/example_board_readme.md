Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
============


Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J5) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the pca9420 example in the terminal window, and user can interact with the application in terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-------------- PCA9420 on board PMIC driver example --------------

Please select the PMIC example you want to run:
[1]. Dumping Mode Settings
[2]. Switch Mode
[3]. Dumping Selected Register Content
[4]. Feed watch dog
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Note: Example enable watchdog power in mode 3, so it will timerout after 16 seconds unless keep call Feed watch dog example. 
