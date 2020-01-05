Overview
========
This document explains the freertos_mutex example. It shows how mutex manage access to common
resource (terminal output).

The example application creates two identical instances of write_task. Each task will lock the mutex
before printing and unlock it after printing to ensure that the outputs from tasks are not mixed
together.

The test_task accept output message during creation as function parameter. Output message have two
parts. If xMutex is unlocked, the write_task_1 acquire xMutex and print first part of message. Then
rescheduling is performed. In this moment scheduler check if some other task could run, but second
task write_task+_2 is blocked because xMutex is already locked by first write task. The first
write_task_1 continue from last point by printing of second message part. Finaly the xMutex is
unlocked and second instance of write_task_2 is executed.



Toolchain supported
===================
- IAR embedded Workbench 7.80.4
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5.4-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KL26Z board
- Personal Computer

Board settings
==============
This Example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.
Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set appropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none
5.  Confirm selected parameters by OK button.

Running the demo
================
After the board is flashed the Tera Term will start periodically printing strings synchronized by
mutex.

Example output:
"ABCD | EFGH"
"1234 | 5678"
"ABCD | EFGH"
"1234 | 5678"
Customization options
=====================

