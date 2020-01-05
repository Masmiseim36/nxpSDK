Overview
========


Toolchain supported
===================
- MCUXpresso  11.1.0
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-K64F board
- GT202 Adaptor V1.04
- Personal Computer

Board settings
==============
Plug GT202 Adaptor board to FRDM stackable headers (J1, J2, J3, J4).
GT202 shield is compatible with FRDM-K64 SCH-REV E and higher.
Previous revisions cannot provide GPIO mux on J2:2 (WLAN_PWRON).

The default compilation settings are predefined for GT202 shield.
Do not try to update a firmware on Silex shield !
Prepare the Demo
================
No special requirements

Running the demo
================
1. Download the application to the target board using external or on-board debugging probe.

2. While the application is running, locate the host.exe application and firmware folder at:
<SDK_PATH>\boards\<BOARD>\demo_apps\wifi_qca_examples\qca_fwupdate\fw\

3. Open the command line from windows and execute the host.exe application as follows:
   > host.exe COM59 flashotp_3_3_6.bin 845dd7490e4c -b115200
   where:
      COM59                - is board serial port name
      flashotp_3_3_6.bin   - is the path to firmware binary
      845dd7490e4c         - is the MAC address of the WiFi module
      -b115200             - is a communication baudrate

NOTES:
 - You can verify the board serial port number (COMx) with the Windows Device Manager
 - The MAC address of your WiFi IOE Module can be found in a label on top of the module
 - host.exe is taken from QUALCOMM MQX installer package ( https://developer.qualcomm.com/hardware/qca4002-4/tools )
 - host.exe cannot resolve COM port with number greater than 99
 - An issue has been discovered when using Jlink CMSIS-DAP Serial on FRDMK64 Rev.E.
   If the transmission stops after few kBytes, we recommend to use DAPLink.
 - After a successful update, the host.exe always shows a dummy firmware version 
   "3.3.1.256.116". To verify the current firmware version, run "qca_demo". 

IMPORTANT:
 - Do not try to update firmware on Silex shield !!!

Customization options
=====================

