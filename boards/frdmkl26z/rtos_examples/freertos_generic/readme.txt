Overview
========

This document explains the freertos_generic example. It is based on code FreeRTOS documentation from
http://www.freertos.org/Hardware-independent-RTOS-example.html. It shows combination of several
tasks with queue, software timer, tick hook and semaphore.

The example application creates three tasks. The prvQueueSendTask periodically sending data to
xQueue queue. The prvQueueReceiveTask is waiting for incoming message and counting number of
received messages. Task prvEventSemaphoreTask is waiting for xEventSemaphore semaphore given from
vApplicationTickHook. Tick hook give semaphore every 500 ms.

Other hook types used for RTOS and resource statistics are also demonstrated in example:
* vApplicationIdleHook
* vApplicationStackOverflowHook
* vApplicationMallocFailedHook



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
After the board is flashed the Tera Term will start periodically printing the state of generic example.

Example output:
Event task is running.
Receive message counter: 1.
Receive message counter: 2.
Event task is running.
Receive message counter: 3.
Receive message counter: 4.
Event task is running.
Receive message counter: 5.
Receive message counter: 6.
Receive message counter: 7.
Customization options
=====================

