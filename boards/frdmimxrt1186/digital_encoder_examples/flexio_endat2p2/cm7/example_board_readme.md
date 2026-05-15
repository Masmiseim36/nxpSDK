Hardware requirements
=====================
- USB Type-C cable
- frdmimxrt1186 board
- Personal Computer
- Power supply 24V
- Motor control board (FRDM-LVPMSM-FA)
- EnDat2.2 sensor

Board settings
==============
Insert FRDM-LVPMSM-FA board into Arduino interface.
Connect EnDat2.2 sensor and power supply to FRDM-LVPMSM-FA

EnDat2.2 connection:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE(FRDM-LVPMSM-FA)  CONNECTS TO  INSTANCE(EnDat2.2 sensor)
Pin Name        Board Location              Signal Name
ENC_CLK_P          J70-2                     CLOCK_P
ENC_CLK_N          J70-7                     CLOCK_N
ENC_DATA_IO_P      J70-4                     DATA_P
ENC_DATA_IO_N      J70-9                     DATA_N
VENC               J70-1                     UP - Power supply voltage
GND                J70-6                     GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SW90 setting:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SW[1-2] OFF:ON Half duplex
SW[3]   ON     Echo Disabled
SW[4]   ON     ENDAT is used.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SW30 setting - Select the appropriate voltage based on the properties of the sensor

~~~~~~~~~~~~~~~~
SW[1] ON 3.3V
SW[2] ON 5.2V
SW[3] ON 9V
SW[4] ON 12V
~~~~~~~~~~~~~~~~

Jumpers:
~~~~~~~~~~~~~~~
J300: pin1 <--> pin2
~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a micro USB cable between the host PC and the J23(FTDI_DEBUG) USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows the output of the demo in the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  FlexIO EnDat2.2 Demo.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
