Hardware requirements
=====================
- FRDM-IMXRT1186 board
- FRDM-LVPMSM-FA boards
- FRDM-EXP-LVPMSM board
- Power supply 24V
- USB Type-C cable
- Low-voltage 3-phase permanent magnet motors with EnDat2.2 sensor
- Personal Computer

Overview
========
The 3-phase permanent magnet synchronous motor (PMSM) control reference application
is  intended to provide the example for 3-phase PMSM motor control solutions.

More information
================
Application web page with the board specific Application User's guide and other documentation: http://www.nxp.com/sdkmotorcontrol

Board settings
==============
Follow chapter "Hardware setup" in Application User's guide.

FRDM-RT1186 configuration:

	- Add/Set zero resistors: R2, R3, R34 (replace by 0 Ohm), R40, R83 1-3, R243, R319 1-3, R448
	- Remove resistors: R15, R17, R21, R24, R407
	- Set jumpers: J27 2-3, J29 2-3, J30 2-3, J33 2-3, J42 1-2, J49 2-3

FRDM-LVPMSM-FA boards configuration:

	- Set jumpers: J72 disconnect boths parts
	- Switch settings: 
		SW90:	
			SW[1-2] OFF:ON Half duplex
			SW[3]   ON     Echo Disabled
			SW[4]   ON     ENDAT is used.
		
		SW30:
			Select the appropriate voltage based on the properties of the sensor		
	- EnDat2.2 connection:
			INSTANCE(FRDM-LVPMSM-FA)  CONNECTS TO  INSTANCE(EnDat2.2 sensor)
			Pin Name        Board Location              Signal Name
			ENC_CLK_P          J70-2                     CLOCK_P
			ENC_CLK_N          J70-7                     CLOCK_N
			ENC_DATA_IO_P      J70-4                     DATA_P
			ENC_DATA_IO_N      J70-9                     DATA_N
			VENC               J70-1                     UP - Power supply voltage
			GND                J70-6                     GND
			
FRDM-EXP-LVPMSM boards configuration:
	- will be added soon
		
Running the demo
================
Follow chapter "User interface" in Application User's guide.

