Overview
========
Application demonstrating how to use the SPP feature.


Toolchain supported
===================
- MCUXpresso  11.4.0
- IAR embedded Workbench  9.10.2
- GCC ARM Embedded  10.2.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1170 board
- Personal Computer
- One of the following WiFi modules:
  - AzureWave AW-AM457-uSD
  - AW-CM358-M.2

Board settings
==============

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO

The hardware should be reworked according to the hardware rework guide for evkmimxrt1170 and AW-AM457-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1170  | PIN NAME OF RT1170 | GPIO NAME OF RT1170
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J25(pin 13)   |    LPUART7_RXD     | GPIO_AD_01
UART_RXD |  J10(pin 2)  |   J25(pin 15)   |    LPUART7_TXD     | GPIO_AD_00
UART_CTS |  J10(pin 8)  |   J25(pin 9)    |    LPUART7_RTS     | GPIO_AD_03
UART_RTS |  J10(pin 6)  |   J25(pin 11)   |    LPUART7_CTS     | GPIO_AD_02
GND      |  J6(pin 7)   |   J26(pin 1)    |    GND             | GND
------------------------------------------------------------------------------------
The hardware should be reworked according to the hardware rework guide for evkmimxrt1170 and AW-CM358-M.2 in document Hardware Rework Guide for EdgeFast BT PAL.

Note:
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW4 or power off and on the board to run the application.
Prepare the Demo
================

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Provide 5V voltage for the target board.

4.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

5.  Download the program to the target board.

6.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows the output of the example in the terminal window. 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USB Host stack successfully initialized
Bluetooth initialized
BR/EDR set connectable and discoverable done

SHELL build: Mar  2 2021
Copyright  2021  NXP

>> 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Notice: SPP project doesn't support multiple roles and multiple connections, which means 
        at one time, spp just servers as a server or a client and just supports 1 spp connection
        with peer spp device.

1.  Procedure to run SPP server
1.1 input "help" to show command list
1.2 input "spp register [5|3]" to register spp server channel 5/3, wait for spp connection
1.3 After spp connection is created, input "spp send [1|2|3|4]" to send data
1.4 After spp connection is created, input "spp disconnect" to disconnect with peer device

2.  Procedure to run SPP client
2.1 input "bt discover" to discover connctable bluetooth device
2.2 input "bt connect [index]" to create basic bluetooth connection with the discovered device
2.3 input "spp discover" to discover registered spp server channel in peer device
2.4 input "spp connect [channel]" to create spp connection with peer spp server channel
2.5 After spp connection is created, input "spp send [1|2|3|4]" to send data
2.6 After spp connection is created, input "spp disconnect" to disconnect with peer device

Here is the log of spp server:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USB Host stack successfully initialized
Bluetooth initialized
BR/EDR set connectable and discoverable done

SHELL build: Mar  2 2021
Copyright  2021  NXP

>> help

"help": List all the registered commands

"exit": Exit program

"bt": BT related function
  USAGE: bt [discover|connect|disconnect|delete]
    bt discover    start to find BT devices
    bt connect     connect to the device that is found, for example: bt connectdevice n (from 1)
    bt disconnect  disconnect current connection.
    bt delete      delete all devices. Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.

"spp": SPP related function
  USAGE: 
    spp register [5|3]    register a spp server channel
    spp discover          discover spp server channel on peer device
    spp connect [channel] create spp connection
    spp disconnect        disconnect current spp connection.
    spp send [1|2|3|4]    send data over spp connection.
>> spp register 5
Register spp server channel 5 successfully!
>> Connected
Security changed: 00:0C:61:20:00:10 (0x80) level 2
SPP connection is created successfully!

SPP received 11 data!

----------------CHAR DUMP-----------------------
A T + C I N D = ? \ r 
------------------------------------------------

----------------HEX DUMP------------------------
41 54 2B 43 49 4E 44 3D 3F 5C 72 
------------------------------------------------
spp send 2
>> 
SPP sent 10 data successfully
Sent data is:
----------------CHAR DUMP-----------------------
A T + C I N D ? \ r 
------------------------------------------------

SPP received 6 data!

----------------CHAR DUMP-----------------------
A T E P \ r 
------------------------------------------------

----------------HEX DUMP------------------------
41 54 45 50 5C 72 
------------------------------------------------
spp send 4
>> 
SPP sent 11 data successfully
Sent data is:
----------------CHAR DUMP-----------------------
A T + C K P D = E \ r 
------------------------------------------------
SPP connection is disconnected successfully!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here is the log of spp client:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
USB Host stack successfully initialized
Bluetooth initialized
BR/EDR set connectable and discoverable done

SHELL build: Mar  2 2021
Copyright  2021  NXP

>> bt discover
Discovery started. Please wait ...
>> BR/EDR discovery complete
[1]: 3C:6A:A7:ED:CD:BB, RSSI -79 
[2]: 20:4E:F6:25:F3:20, RSSI -42 edgefast spp
[3]: 58:A0:23:19:09:32, RSSI -84 
[4]: B0:E2:35:07:FA:76, RSSI -80 灏忕背鎵嬫満
[5]: 44:66:FC:1B:FC:70, RSSI -82 OPPO R15
[6]: D8:C0:A6:BD:9C:3C, RSSI -78 edgefast hfp
[7]: 7C:03:AB:43:0C:CB, RSSI -77 1162200453鐨凴edmi Note 7 Pro
[8]: 48:2C:A0:39:6E:9F, RSSI -97 
bt connect 2
Connection pending
>> Connected
spp discover
>> Discover 1 SPP server channel from device 20:F3:25:F6:4E:20!
0x0005
spp connect 5
Connect SPP Successful!
>> Security changed: 00:0C:61:20:00:10 (0x80) level 2
SPP connection is created successfully!
spp send 1
>> 
SPP sent 11 data successfully
Sent data is:
----------------CHAR DUMP-----------------------
A T + C I N D = ? \ r 
------------------------------------------------

SPP received 10 data!

----------------CHAR DUMP-----------------------
A T + C I N D ? \ r 
------------------------------------------------

----------------HEX DUMP------------------------
41 54 2B 43 49 4E 44 3F 5C 72 
------------------------------------------------
spp send 3
>> 
SPP sent 6 data successfully
Sent data is:
----------------CHAR DUMP-----------------------
A T E P \ r 
------------------------------------------------

SPP received 11 data!

----------------CHAR DUMP-----------------------
A T + C K P D = E \ r 
------------------------------------------------

----------------HEX DUMP------------------------
41 54 2B 43 4B 50 44 3D 45 5C 72 
------------------------------------------------
spp disconnect
>> SPP connection is disconnected successfully!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
