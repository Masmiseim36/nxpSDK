Overview
========
This application demonstrates use of certified NXP Safety Library 3.0 which meets the IEC60730B standard. For more information see documentation.

Hardware requirements
=====================
- Micro USB cable
- MIMXRT1060-EVK board
- Personal Computer

Board settings
==============
- SW7 switch 1-OFF 2-OFF 3-ON 4-OFF

Prepare the Demo
================
1. Connect a micro USB cable between the host PC and the USB port (J23) on the target board.
2. You can enable/disable some safety tests in safety_config.h file (at the beginning of the file).
3. The project has two configuration: Debug and Release. If Debug is selected, you must disable Watchdog, Flash and Program counter test, because Debug configuration runs only from RAM.
If Release is selected, all tests can be enabled, because application runs from ROM. Turn optimizations off.
4. Build project.
5. Download the program to the target board.
6. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
7. Open a Freemaster 2.0 (file Safety_common_debug.pmp or Safety_common_release.pmp) with the following settings:
    - RS232: Set appropriate COM port, speed 19200 Bd
    - Set appropriate map file in 'MAP Files' tab: .out file for IAR, .axf file for MDK and MCUX.
8. Press CTRL-K to start communication in Freemaster.

Running the demo
================
- If you are using Freemaster, you should see test results in 'Variable Watch' window. 
- If some safety error occurs, the application stays in infinity while loop in SafetyErrorHandling function. 

Toolchain supported
===================
- IAR embedded Workbench 8.32.2
- Keil MDK 5.26.2.0
- MCUXpresso 11.0.0

Flash test
==========
If you want to enable flash test, the following steps are required:
1. Download srec_cat.exe from the following website: http://srecord.sourceforge.net/download.html. Then place the srec_cat.exe to folder "<repostiory-root-path>/tools/srec/". SRecord tool is used in a postbuild action to calculate checksum of builded image. 
2. Enable flash test in code (FLASH_TEST_ENABLE=1 in safety_config.h).
3a. Keil IDE: Set postbuild action: "crc_keil.bat". The final post-processed image is downloaded to ROM memory via Download button.
3b. MCUXpresso IDE: Set postbuild action: "${ProjDirPath}"/src/crc_mcux.bat "${BuildArtifactFileBaseName}". The final post-processed image is downloaded to ROM memory via GUI Flash Tool (choose file <board>_safety_iec60730b_CRC.bin).
4. Option 'Attach only' must be set if you want to debug application.