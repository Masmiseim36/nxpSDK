Overview
========
After power on, the board will print the application logo over a serial interface and the beacon transmission is disabled. 

The beacon transmission can be enabled or disabled in two ways:
- by pressing the [SW2] button on the freedom board
- by entering the [Enter] key in a terminal window

The switch between 2 Mbps and 1 Mbps rates is done by pressing on [s] key in the terminal window.
By default application uses the 2 Mbps rate.

It is not mandatory to use the terminal as the transmission can be controlled by the [SW2] button on the board.

The board LED D2 will indicate the beacon transmission status:
- Off: beacon transmission disabled
- Blinking: beacon transmission enabled and the LED is toggled at each advertising event.

LowPower mode
=============
The application supports RAMOFF mode.
In this mode, the device will advertise and then go to RAMOFF, will wake up with low power timer expiration, and repeat.
In this configuration, the terminal cannot be used to change data rate or start/stop beaconing (RX is disabled).
The beaconing will start at startup at a data rate set with the flag gAppDefaultDataRate.
It is still possible to stop the beaconing and go to VLLS1 if you press SW2 while the device is active (can be tricky).
If the beaconing is stopped, the device goes to VLLS1 and can be woken up by pressing SW3.
To enable this mode, you need this configuration:
- gAppGenfskRamoff set to 1
- cPWR_UsePowerDownMode set to 1
- gTMR_EnableLowPowerTimers set to 1
WARNING: As the RAM if OFF between advertising events, all data in RAM are lost, it will act like a reset.

It is possible to go to PSWITCH mode after every advertising event by setting the flag gPWR_UsePswitchMode_d to 1.

To reduce active time, the device advertises only on 1 channel (only 1 TX) and the flag gGenfskPreserveXcvrDacTrimValue_d is set to 1 to save radio
calibration in flash.

It is possible to go only in STOP mode between advertising events if gAppGenfskRamoff is set to 0.
In this configuration, it's easier to control start/stop of beaconing with SW2.

Toolchain supported
===================
- IAR Embedded Workbench 8.30.2
- MCUXpresso IDE 10.3.0

Hardware requirements
=====================
- micro USB cable
- Val_kw38 board
- Personal Computer a serial port terminal application installed.

Board settings
==============
No special board setting.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for the detected serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press the reset button.
5.  OPTIONAL STEP: To check for the beacon transmission, you can use the connectivity_test application on a peer device. Use following configuration:
    -Press [1] Continuous tests
    -Use the [z] or [x] keys to select the appropriate rate: BLE 2Mbps or BLE 1Mbps
    -Press [7] Continuous Packet Mode Reception
6.  Enable the beacon transmission by pressing the [Enter] key in the terminal window or the [SW2] button on the board.
7.  Disable the beacon transmission by pressing the [Enter] key in the terminal window or the [SW2] button on the board.
