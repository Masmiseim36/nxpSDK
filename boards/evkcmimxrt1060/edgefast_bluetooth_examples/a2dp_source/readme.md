# a2dp_source

##  Overview
Application demonstrating how to use the a2dp source feature.

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
USB Host stack successfully initialized
Bluetooth initialized

SHELL build: Mar  2 2021
Copyright  2021  NXP

>> 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Procedures to run
1 input "help" to show command list
2 input "bt discover" to discover connctable bluetooth devices
3 input "bt connect [index]" to create basic bluetooth connection with the discovered device
4 the music start playing after connection success.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/edgefast_bluetooth_examples/a2dp_source/example_board_readme.md)
