Overview
========
The Motor Control PMSM demo application provides field oriented speed control of permanent magnet synchronous motor using 
MIMXRT1050-EVK with FRDM-MC-LVPMSM. FreeMASTER pmsm_demo.pmp file is used for real-time debug monitoring and data visualization. 
Motor spinning is controlled by FreeMASTER file or by button SW8 placed on the demo board. 

Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1050-EVK board revision (A1 Silicon)
- FRDM-MC-LVPMSM low-voltage evaluation board 
- Linix 45ZWN24-40 motor (for sensorless three wires cable only) 
- Personal Computer

Boards settings
===============
Before you connect the MIMXRT1050-EVK board with FRDM-MC-LVPMSM ensure that the jumpers 
are configured according users guide PMSMDEMOUG. 

The low-voltage evaluation board does not require any hardware configuration or jumper settings as well.

Prepare the Demo
===============
1.  Connect the FRDM-MC-LVPMSM with MIMXRT1050-EVK board through cables according users guide PMSMDEMOUG. 
2.  Connect the Linix motor three-phase wires into the screw terminals on the board. 
3.  Plug the USB cable from the USB host to the micro USB connector on MIMXRT1050-EVK. 
4.  Plug the 24 V DC power supply to the DC Power connector on FRDM-MC-LVPMSM. 
5.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
6.  Download the program to the target board.
7.  To run demo follow Running the demo instructions.

Running the demo
================
Press button SW8 on MIMXRT1050-EVK board to switch on or off demonstration mode - the motor will rotate. 
Motor spinning can be controlled also via FreeMASTER pmp file placed in: {$ProjectDir$}\middleware\motor_control\freemaster\pmsm_demo.pmp. 
For control demo via Freemaster open the pmsm_demo.pmp, set the proper COM port in Projects\Options\Comm and start demo using RUN and Demo Mode buttons.  

Note:
To download application into hyper flash and boot from hyperflash directly from IAR, following steps are needed:

1. Select the target flexspi_nor_debug or flexspi_nor_release.
2. Compile the project.
3. Download the application to the board by pressing Ctrl+D (Download and debug)
4. Stop the debug session by pressing Ctrl+Shift+D (Stop Debugging)
5. Reset board with SW4.

Note:
To debug in hyper flash in MDK, following steps are needed:
1. Set the SW7: 1 off 2 on 3 on 4 off, then power on the board and connect USB cable to J28
2. Select the target flexspi_nor_debug or flexspi_nor_release.
3. Compile the project.
4. Press F8 or click the download button, to program the application into hyper flash.
5. Push SW4 to reset.
6. Start to debug.

Note:
To debug in hyper flash in MCUXpresso, following steps are needed:
1. Set the SW7: 1 off 2 on 3 on 4 off, then power on the board and connect USB cable to J28
2. Select the target Debug or Release.
3. Go to project properties (ALT+ENTER). Select "MCU Settings" under the "C/C++ Build" option. Add "MIMXRT1050-EVK_S26KS512S.cfx" driver to BOARD_FLASH row. 
4. Compile the project.
5. Download the project into hyper flash by press Debug button in Quickstart panel.
7. Start to debug.

More information
================
For more information visit the page: http://www.nxp.com/motorcontrol_pmsm 
More details about pmsm motor control application on MIMXRT1050-EVK board are described in document AN12169 and PMSMFOCRT1050UG.
Also look for DRM148. 