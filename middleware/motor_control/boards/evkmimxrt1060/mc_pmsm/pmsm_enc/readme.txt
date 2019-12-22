Overview
========
The Motor Control PMSM demo application provides field oriented position and speed control of permanent magnet synchronous motor using 
MIMXRT1060-EVK with FRDM-MC-LVPMSM. FreeMASTER pmsm_float_enc.pmp file is used for real-time debug monitoring and data visualization. 
Motor spinning is controlled by FreeMASTER file or by button SW8 placed on the demo board. 

Toolchain supported
===================
- IAR embedded Workbench 8.32.3
- Keil MDK 5.27
- MCUXpresso IDE 11.0

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1060-EVK board (schematic:SCH-31357 REV A2, board: 700-31357 REV X1)
- FRDM-MC-LVPMSM low-voltage evaluation board 
- Teknic M-2310P motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
Before you connect the MIMXRT1060-EVK board with FRDM-MC-LVPMSM ensures that the jumpers are configured according to the documentation available on http://www.nxp.com/motorcontrol_pmsm.

For correct signal connection and proper demo working, it is necessary to solder/remove resistors from the board. Please, see the documentation (http://www.nxp.com/motorcontrol_pmsm) and MIMXRT1060-EVK board schematic.

The low-voltage evaluation board does not require any hardware configuration or jumper settings as well.

Prepare the Demo
===============
1.  Connect the FRDM-MC-LVPMSM with MIMXRT1060-EVK board through cables according documentation available on (http://www.nxp.com/motorcontrol_pmsm). 
2.  Connect the Teknic motor three-phase wires into the screw terminals on the board. 
3.  Plug the USB cable from the USB host to the micro USB connector on MIMXRT1060-EVK. 
4.  Plug the 24 V DC power supply to the DC Power connector on FRDM-MC-LVPMSM. 
5.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board MIMXRT1060-EVK. 
6.  Download the program to the target board.
7.  To run demo follow Running the demo instructions.

Running the demo
================
Press button SW8 on MIMXRT1060-EVK board to switch on or off demonstration mode - the motor will rotate. 
Motor spinning can be controlled also via FreeMASTER pmp file placed in: KSDK package\boards\evkmimxrt1060\demo_apps\mc_pmsm\pmsm_enc\pmsm_float_enc.pmp. 
For control demo via Freemaster open the pmsm_float_enc.pmp, set the proper COM port in Projects\Options\Comm and start demo using RUN and Demo Mode buttons.  

Note:
For use MIMXRT1060-EVK onboard Hyper flash is needed reconfigure HW board according schematic. 
The default configuration of MIMXRT1060-EVK is set to use with onboard QSPI flash.

Note:
To download application into QSPI flash and boot from QSPI flash directly from IAR, following steps are needed:

1. Set the SW7: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41
1. Select the target flexspi_nor_debug or flexspi_nor_release.
2. Compile the project.
3. Download the application to the board by pressing Ctrl+D (Download and debug)
4. Stop the debug session by pressing Ctrl+Shift+D (Stop Debugging)
5. Reset board with SW3.

Note:
To debug in QSPI flash in MDK, following steps are needed:
1. Set the SW7: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41
2. Select the target flexspi_nor_debug or flexspi_nor_release.
3. Compile the project.
4. Press F8 or click the download button, to program the application into hyper flash.
5. Push SW3 to reset.
6. Start to debug.

Note:
To debug in QSPI flash in MCUXpresso, following steps are needed:
1. Set the SW7: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41
2. Select the target Debug or Release.
3. Go to project properties (ALT+ENTER). Select "MCU Settings" under the "C/C++ Build" option. Add "MIMXRT1060-EVK_IS25LP064.cfx" driver to BOARD_FLASH row.
4. Compile the project.
5. Download the project into QSPI flash by press Debug button in Quickstart panel.
7. Start to debug.

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_pmsm.
The documentation is available on the web page: http://www.nxp.com/motorcontrol_pmsm.
