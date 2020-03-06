Overview
========
The Secure GPIO demo application demonstrates using of secure GPIO peripheral and GPIO mask feature in
AHB secure controller. The secure GPIO is standard GPIO peripheral supporting 32 pins on port 0 only.
Having two GPIO peripherals allows user to configure one GPIO peripheral into secure world and another
one into normal world. Thus for all pins on port 0, user can select whether the pin is controlled
from secure or non-secure domain (using IOCON).
Since the every GPIO pin allows to read its state regardless of its configuration, the AHB secure
controller has build in feature GPIO mask for all GPIO pins. It is recommended to mask GPIO pins
for all peripherals assigned to secure domain in order to eliminate information leak from GPIO pins.
For example UART is assigned to secure world. But GPIO peripheral still allows to read state of
TxD and RxD pins. This information can be use to decode UART communication from normal world, which
can lead to unwanted information leak. 

This Secure GPIO demo uses secure GPIO to read S1 button from secure world and standard GPIO to read
S1 button. If SW1 button is pressed (logical zero is read) the GREEN LED is switched on in secure world
and blue led is switched on from normal world.
The second part of the demo is AHB secure controller GPIO mask feature. This feature is controlled by
button S2. If the S2 button is released, the GPIO mask feature is disabled so state of S1 can be read
also from normal world. Therefore S1 buttons controls both blue and green LEDs.
If the S2 button is pressed, the GPIO mask feature is enabled. So normal world cannot read state of 
S1 button and logical zero is read. Since logical zero corresponds to pressed button the blue LED is switched
on while S2 is pressed and cannot be further controlled by S1 switch. The green LED can be still controlled
by S1 button since state of S1 button is read by secure GPIO.
 


Toolchain supported
===================
- GCC ARM Embedded  8.3.1
- Keil MDK  5.29
- IAR embedded Workbench  8.40.2
- MCUXpresso  11.1.1

Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT685 board
- Personal Computer

Board settings
==============

Prepare the Demo
================
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
Use S1(SW2) button to control blue and green LED and S2(SW1) to control GPIO mask feature.
If S1 button is pressed (logical zero is read), the GREEN LED is switched on in secure world
and blue LED is switched on from normal world. So both blue and green LEDs are controlled by S1 button.
If also S2 button is pressed, the GPIO mask feature is activated. In this case the blue LED starts to light
regardless of S1 button state, since normal GPIO cannot read state of S1 button (only the logic 0 is read). 
The green LED can be still controlled by S1 button.

TrustZone Application Development in SDK
Every TrustZone based application consists of two independent parts - secure part/project and non-secure part/project.

The secure project is stored in <application_name>\<application_name>_s directory.
The non-secure project is stored in <application_name>\<application_name>_ns directory. 

The secure projects always contains TrustZone configuration and it is executed after device RESET. The secure project usually
ends by jump to non-secure application/project.
If IDE allows the two projects in single workspace, the user can also find the project with <application_name>.
This project contains both secure and non-secure projects in one workspace (Keil MDK, IAR) and it allows to user easy transition from
one to another project.

Project Structure
The all TrustZone files related to TrustZone are located in trustzone virtual directory. The files are:

- tzm_config.c
- tzm_config.h
- veneer_table.c
- veneer_table.h

File tzm_config.c, tzm_config.h
This file is used by secure project only. It contains one function BOARD_InitTrustZone(), which configures complete TrustZone
environment. It includes SAU, MPU's, AHB secure controller and some TrustZone related registers from System Control Block.
This function is called from SystemInitHook() function, it means during system initialization.

File veneer_table.c, veneer_table.h
This file defines all secure functions (secure entry functions) exported to normal world. This file is located in secure
project only. While header file is used by both secure and non-secure projects. The secure entry functions usually contain
validation of all input parameters in order to avoid any data leak from secure world.

The files veneer_table.h and <application_name>_s_import_lib.o or <application_name>_s_CMSE_lib.o create the connection
between secure and non-secure projects. The library file is generated by linker during compilation of secure project and
it is linked to the non-secure project as any other library.

TrustZone application compilation
As first compile secure project since CMSE library is needed for compilation of non-secure project. 
After successful compilation of secure project, compile non-secure project.

TrustZone application debugging
- Download both output file into device memory
- Start execution of secure project since secure project is going to be executed after device RESET.

If IDE (Keil MDK, IAR) allows to manage download both output files as single download, the secure project
is configured to download both secure and non-secure output files so debugging can be fully managed
from secure project.

For IAR, please don't choose Use flash loader option when download ram target output files.

If want to download secure and non-secure binary file into QSPI flash, should the following rules:
Flash target:
    1. secure binary download into 0x8000400 address.
    2. non-secure binary dowanload into 0x8100000 address.
RAM target:
    1. secure binary download into 0x8000400 address.
    2. non-secure binary dowanload into 0x8041000 address.

Device header file and secure/non-secure access to the peripherals
Both secure and non-secure project uses identical device header file. The access to secure and non-secure aliases for all peripherals
is managed using compiler macro __ARM_FEATURE_CMSE.

For secure project using <PERIPH_BASE> means access through secure alias (address bit A28=1), 
using <PERIPH_BASE>_NS means access through non-secure alias(address bit A28=0)
For non-secure project using <PERIPH_BASE> means access through non-secure alias (address bit A28=0). 
The non-secure project doesn't have access to secure memory or peripherals regions so the secure access is not defined.
Customization options
=====================

