Overview
========

The demo sends the data received from the board's UART to the Bluetooth HCI interface of the Murata 1DX M.2 module.
Therefore it is possible to control the Bluetooth functionality from a personal computer using a tool capable of sending
HCI commands, such as CyBluetool, test the functionality, device throughput and so on.

Note that the HCI commands are sent over serial port, which is used for debugging messages and logs in most of the examples.
In this example, the logs are displayed on another port instead and a USB to serial device is needed to observe those messages.

CyBluetool software and its user guide is available for download on Cypress developer community web (registration required):
https://community.cypress.com/docs/DOC-15585 (Windows)
https://community.cypress.com/docs/DOC-15586 (Linux)


Toolchain supported
===================
- IAR embedded Workbench  8.50.5
- GCC ARM Embedded  9.2.1
- MCUXpresso  11.2.0

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1020 board
- Personal Computer
- Murata 1DX M.2 module
- Murata uSD M.2 Adapter
- 5 plug to receptable header cables
- USB/UART converter and cables

Board settings
==============
Make sure C28 is welded.

Murata 1DX M.2 module is connected with Murata uSD M.2 Adapter which is plugged into uSD connector on evkmimxrt1020 board.

Murata uSD M.2 Adapter's jumper J11 is open, J1 shorts position 2-3, J12 shorts position 1-2.

The following pins between the evkmimxrt1020 board and Murata uSD M.2 Adapter are connected using the plug to receptable cables:
- evkmimxrt1020 board's connector J17, pin 5 to Murata uSD M.2 Adapter's connector J9, pin 4
- evkmimxrt1020 board's connector J17, pin 7 to Murata uSD M.2 Adapter's connector J9, pin 2
- evkmimxrt1020 board's connector J19, pin 2 to Murata uSD M.2 Adapter's connector J9, pin 1
- evkmimxrt1020 board's connector J19, pins 4-5 to Murata uSD M.2 Adapter's connector J8, pins 3-4

Connect the board with PC by using USB/UART converter:
- board uart RX (pin 1 on J17) - connect it to TX pin on converter
- board uart TX (pin 2 on J17) - connect it to RX pin on converter
- board GND (pin 7 on J19)
Running the demo
================

1.  Connect a USB cable between the PC host and the OpenSDA port on the target board.
    
2.  Connect the WiFi/Bluetooth module, details are in section: Board settings.

3.  Download the program to the target board with the CMSIS-DAP or J-Link debugger.

4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

5.  Open the CyBluetool and download the Bluetooth firmware located in the firmware/4343W folder in the project via the "Tools > Firmware Download" menu.
    Use the UART available via target board's OpenSDA port for download.
    The picture firmware/4343W/FW_download.png shows the example settings, it may be needed to copy the firmware files into another folder so the paths are not too long.

6.  Click "Select Device" button in CyBluetool. Use the following settings in the dialog window:
    - Transport: UART
    - Device Name: the name of the UART available via target board's OpenSDA port
    - Baud Rate: 115200
    - Flow Control: None

7.  Using the CyBluetool, send the following HCI commands:
    - Reset
    - LE_Set_Advertise_Enable with parameter Advertising_Enable set to "Advertising is enabled"
    - Read_BD_ADDR
    
    Observe the log in CyBluetool, which will show messages like below:
    
    06/13/19 10:24:50.646   com15 -- Transport opened
        com15@115200
    06/13/19 10:24:50.646 com15 -- Protocol set to HCI
        com15@115200

    06/13/19 10:24:55.767  com15 c> Reset 
          HCI Command
        com15@115200
        [03 0C 00 ]
        opcode = 0x0C03 (3075, "Reset")

    06/13/19 10:24:55.792  com15 <c Reset
          HCI Command Complete Event
        com15@115200
        [0E 04 ]: 01 03 0C 00 
        event = 0x0E (14,"Command Complete")
        Num_HCI_Command_Packets = 0x1 (1)
        Command_Opcode = 0xC03 (3075, "Reset")
        Status = 0x0 (0, "Success", "Success")

    06/13/19 10:26:57.401  com15 c> LE_Set_Advertise_Enable 
          HCI Command
        com15@115200
        [0A 20 01 ]: 01
        opcode = 0x200A (8202, "LE_Set_Advertise_Enable")
        Advertising_Enable = 0x1 (1, "Advertising is enabled")

    06/13/19 10:26:57.406  com15 <c LE_Set_Advertise_Enable
          HCI Command Complete Event
        com15@115200
        [0E 04 ]: 01 0A 20 00 
        event = 0x0E (14,"Command Complete")
        Num_HCI_Command_Packets = 0x1 (1)
        Command_Opcode = 0x200A (8202, "LE_Set_Advertise_Enable")
        Status = 0x0 (0, "Success", "Success")

    06/13/19 10:27:03.632  com15 c> Read_BD_ADDR 
          HCI Command
        com15@115200
        [09 10 00 ]
        opcode = 0x1009 (4105, "Read_BD_ADDR")

    06/13/19 10:27:03.648  com15 <c Read_BD_ADDR
          HCI Command Complete Event
        com15@115200
        [0E 0A ]: 01 09 10 00 93 76 12 A1 43 43 
        event = 0x0E (14,"Command Complete")
        Num_HCI_Command_Packets = 0x1 (1)
        Command_Opcode = 0x1009 (4105, "Read_BD_ADDR")
        Status = 0x0 (0, "Success", "Success")
        BD_ADDR = "4343A1127693" 

8.  The board now advertises as a BLE device with the address indicated as BD_ADDR in the Read_BD_ADDR command response.

9.  Use CyBluetool to test various HCI commands and functionalities as described in its user guide.

    
Logs:

To see log from debug console make these steps:
 - Connect the board with PC by USB/UART converter, details for connection are in section: Board settings.
 - Open a serial terminal on PC for serial device (USB/UART converter) with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

When the application boots up the logs are as follows.

************************************************
 Bluetooth HCI Pass-Through example
************************************************
