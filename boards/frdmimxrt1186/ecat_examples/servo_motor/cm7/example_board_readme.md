Hardware requirements
=====================
- FRDM-IMXRT1186
- FRDM-MC-LVPMSM
- Power supply 24V
- Teknic2311P Motor
- RJ45 Network cable
- Mini/micro USB cable
- Personal Computer on which the TwinCat3 has been installed

Board settings
==============
- Jumper setting:
  | Jumper | Setting |
	|--------|--------|
	|   J12  |   2-3  |
	|   J13  |   2-3  |
	|   J17  |   2-3  |
	|   J18  |   2-3  |
  |   J27  |   2-3  |
  |   J29  |   2-3  |
  |   J30  |   2-3  |
  |   J33  |   2-3  |

- For detailed motor connection and motor control user guide, please refer:
  -   http://www.nxp.com/sdkmotorcontrol 

Prepare the Demo
================
1. Connect Teknic2311P motor to the FRDM-LVPMSM-FA shield
2. Connect the FRDM-LVPMSM-FA shield on Motor Control interface of the FRDM-IMXRT1186 board.
3. Connect the EtherCAT Port0 on the FRDM-IMXRT1186 board with TwinCAT3
	The label of the EtherCAT Port0 is J57A.
4. Connect a USB cable between the host PC and the OpenSDA USB port on the target board

5. Open a serial terminal with the following settings
	- 115200 baud rate
	- 8 data bits
 	- No parity
	- One stop bit
	- No flow control

6. Download the program to the target board

7. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo
