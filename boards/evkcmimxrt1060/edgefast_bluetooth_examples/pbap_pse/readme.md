# pbap_pse

## Overview
This example demonstrates the PBAP PSE basic functionality, the PSE device support be connected to a PBAP PCE like a Hands-Free unit in the car or a 
board running a PBAP PCE application. And the PSE example support sending response to the PCE. 


## Prepare the Demo
1.  Open example's project and build it.
2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.
3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

## Running the demo
The log below shows the output of the example in the terminal window.  
The demo start waiting for the PCE to connect.
```
Bluetooth PBAP PSE demo start...
Bluetooth initialized
BR/EDR set connectable and discoverable done
```
Procedures to run
1. Prepare a PCE, connect the PSE device and then create PBAP connection.
2. After that, PCE can send commands to PSE and PSE will respond to the commands.

The bellow commands have been supported:
1. pull phonebook 
    - This example will send phonebook object but not parse/send all application parameters from/to PCE.
2. set phonebook path
    - This example will set phonebook path correctly.
3. pull vcard listing
    - This example will send vcard listing object but not parse/send all application parameters from/to PCE.
4. get vcard entry
    - This example will send vcard entry object but not parse/send all application parameters from/to PCE.


## Note:
1. This example only supports one PBAP connection.
2. This example doesn't support all application parameters and only supports to parse/send the part of application parameters from/to PCE.

## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/pbap_pse/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/pbap_pse/example_board_readme.md)
