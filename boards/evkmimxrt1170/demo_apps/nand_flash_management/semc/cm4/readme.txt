Overview
========

Nand flash management demo demonstrates the use of a open source nand flash middleware and nand flash componet in KSDK.
The demo combine a open source nand flash middleware which include:
---wear leveling,
---bad block management,
---power fail safe,
---garbage collection - user should be carefully with this function, there is a GC_RATIO in the middleware, the bigger value of the ratio, ---the more nand flash memory will be available to user, but it will cause the write amplification.
with fatfs and freertos.
The demo use shell to support multiple file operation command, such as
write -  write a new file
read -  read a file
list -  list the file in the directory
free - check the free cluster of current file system
..., user can get detail information by command "help".
There are two tasks in the demo, shell task is used to receive and process user command, another task is garbage collection task. The gc task priority is bigger than shell task to reduce the write amplification.



Toolchain supported
===================
- IAR embedded Workbench  9.20.2
- Keil MDK  5.36
- GCC ARM Embedded  10.3.1
- MCUXpresso  11.5.0

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1170 board
- Personal Computer

Board settings
==============
Make sure nand flash is available on board.

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
When the demo runs successfully, the log would be seen on the OpenSDA terminal like below, after shell is available, please start with command 
"help".
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
nand flash management demo.

Make file system..............

Create directory......

SHELL build: Oct  9 2020
Copyright  2020  NXP

SHELL>>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
