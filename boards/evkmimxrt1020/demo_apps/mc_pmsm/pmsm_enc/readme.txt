Overview
========
The Motor Control PMSM demo application provides field oriented speed control of permanent magnet synchronous motor using 
MIMXRT1020-EVK with FRDM-MC-LVPMSM. FreeMASTER pmsm_float_enc.pmp file is used for real-time debug monitoring and data visualization. 
Motor spinning is controlled by FreeMASTER file or by button SW4 placed on the demo board. 

Toolchain supported
===================
- IAR embedded Workbench 8.32.3
- Keil MDK 5.27
- MCUXpresso IDE 11.0

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1020-EVK (schematic:SCH-29856 REV A2, board:700-29856 REV X2)
- FRDM-MC-LVPMSM low-voltage evaluation board 
- Teknic M-2310P motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
Before you connect the MIMXRT1020-EVK board with FRDM-MC-LVPMSM ensure that the jumpers 
are configured according the PMSMDEMOUG users guide (from KSDK package\docs\MC\PMSMDEMOUG.pdf). 

For correct signal connection and proper demo working, it is necessary to solder/remove resistors from the board. Please, see the PMSMDEMOUG user guide and MIMXRT1020-EVK board schematic.

The low-voltage evaluation board does not require any hardware configuration or jumper settings as well.

Prepare the Demo
===============
1.  Connect the FRDM-MC-LVPMSM with MIMXRT1020-EVK board through cables according users guide PMSMDEMOUG. 
2.  Connect the Teknic motor three-phase wires into the screw terminals on the board. 
3.  Plug the USB cable from the USB host to the micro USB connector on MIMXRT1020-EVK. 
4.  Plug the 24 V DC power supply to the DC Power connector on FRDM-MC-LVPMSM. 
5.  Connect a USB cable between the host PC and the OpenSDA USB port (Debug USB, J23) on the target board MIMXRT1020-EVK (see Figure 3 in PMSMDEMOUG.pdf). 
6.  Download the program to the target board.
7.  To run demo follow Running the demo instructions.

Running the demo
================
Press button SW4 on MIMXRT1020-EVK board to switch on or off demonstration mode - the motor will rotate. 
Motor spinning can be controlled also via FreeMASTER pmp file placed in: package\boards\evkmimxrt1020\demo_apps\mc_pmsm\pmsm_enc\pmsm_float_enc.pmp. 
For control demo via Freemaster open the pmsm_float_enc.pmp, set the proper COM port in Projects\Options\Comm and start demo using RUN and Demo Mode buttons.  

Note:
To download application into hyper flash and boot from hyperflash directly from IAR, following steps are needed:

1. Select the target flexspi_nor_debug or flexspi_nor_release.
2. Compile the project.
3. Download the application to the board by pressing Ctrl+D (Download and debug)
4. Stop the debug session by pressing Ctrl+Shift+D (Stop Debugging)
5. Reset board with SW3.

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_pmsm.
The user guide is in SDK install folder\docs\MC\PMSMDEMOUG.pdf.