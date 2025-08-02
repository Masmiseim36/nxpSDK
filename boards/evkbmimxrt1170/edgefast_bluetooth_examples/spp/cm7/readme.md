# spp

## Overview
Application demonstrating how to use the SPP feature.

## Prepare the Demo

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

## Running the demo
The log below shows the output of the example in the terminal window. 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bluetooth initialized
BR/EDR set connectable and discoverable done

Copyright  2020  NXP

>> 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1.  Procedure to run SPP server
    1. input "help" to show command list
    2. input "spp register 3" to register spp server channel 3, wait for spp connection
    3. After spp connection on channel 3 is created, input "spp send [1|2|3|4]" to send data
    4. After peer device send "spp set_pn client 3", input "spp get_pn server 3" to test pn command
    5. After peer device send "spp set_port client 5", input "spp register 5" to register spp server channel 5 and wait for spp connection
    6. After spp connection on channel 5 is created, input "spp get_port server 5" to test remote port negotiation command after spp connection is created
    7. input "spp handle" to show current active spp handle
    8. input "spp switch 0" to select the first spp handle
    9. input "spp disconnect" to disconnect with peer device

2.  Procedure to run SPP client
    1. input "bt discover" to discover connctable bluetooth device
    2. input "bt connect [index]" to create basic bluetooth connection with the discovered device
    3. input "spp discover" to discover registered spp server channel in peer device
    4. input "spp connect 3" to create spp connection on channel 3 with peer device
    5. After spp connection on channel 3 is created, input "spp send [1|2|3|4]" to send data
    6. After spp connection on channel 3 is created, input "spp send_rls" to test remote line status command
    7. After spp connection on channel 3 is created, input "spp send_msc" to test modem status command
    8. After spp connection on channel 3 is created, input "spp set_pn client 3" to test parameter command
    9. input "spp get_port client 5" to test remote port negotiation command before spp connection on channel 5 is created
    10. input "spp set_port client 5" to test remote port negotiation command before spp connection on channel 5 is created
    11. input "spp connect 5" to create spp connection on channel 5 with peer device
    12. input "spp get_port client 5" to test remote port negotiation command after spp connection on channel 5 is created

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/edgefast_bluetooth_examples/spp/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/spp/example_board_readme.md)
