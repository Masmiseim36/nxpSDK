Hardware requirements
===================
- Mini/micro USB cable
- Network cable RJ45 standard
- MIMXRT1160-EVK board
- Personal Computer

Board settings
============
This example uses 1G port(J4) as default. If want to test 100M port(J3), please set the macro BOARD_NETWORK_USE_100M_ENET_PORT to 1.

Platform specific behavior
---------------------------
When an application programmatically performs software-induced reset while in a debugging session, the execution stalls in the ROM code. This is a feature.
Your attention is needed at that moment: please perform reset manually by pressing the on-board hw reset button once you spot "SystemReset" message in the serial console.
Manual reset is not needed while MCU is running freely without a debugger.

