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

IMPORTANT:
 - Do not try to update firmware on Silex shield !!!

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-IMXRT1050 board
- GT202 Adaptor V1.04
- Personal Computer

Board settings
==============
Plug GT202 Adaptor board to FRDM stackable headers (J1, J2, J3, J4).
Remove the resistor R334, dis-connect J15, and weld 0Ω resistor to R278,R279,R280,R281.

Please use external power supply (J2), USB may not be sufficient.


Toolchain supported
===================
- MCUXpresso 10.3.0
- IAR embedded Workbench  8.32.1
- Keil MDK  5.26
- GCC ARM Embedded  7.3.1

