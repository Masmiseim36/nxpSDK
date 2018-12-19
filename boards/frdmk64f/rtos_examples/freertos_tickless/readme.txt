Overview
========
This document explains the freertos_tickless example. It shows the CPU enter at sleep mode and then
it is waked up by expired time delay that using GPT module.

Running the demo
================
After the board is flashed the Tera Term will start periodically printing number tick count when the CPU
is running.

When the CPU is running and get tick count number, press button on board that suggest message display on
console screen ("eg:Press SW3 <or SW1,SW2> to wake up the CPU") to wake up CPU then Tera Term
will printing “CPU waked up by EXT interrupt” message.

Example output:

Press any key to start the example
Tickless Demo example
Press or turn on SWx to wake up the CPU

Tick count :
1
5001
10001
CPU waked up by EXT interrupt
15001
20001
25001

Explanation of the example
The example application will get tick count number after every period time delay vTaskDelay(time)
When this function called the CPU enter to sleep mode a period of time that called by vTaskDelay
function.

While the CPU is sleeping, if several time on board then CPU waked up
by external interrupt and continuous enter sleep mode after complete interrupt service.
The period of time delay is not change after occur external interrupt.
Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K64F board
- Personal Computer

Board settings
==============
This Example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Toolchain supported
===================
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1
- MCUXpresso 10.3.0

