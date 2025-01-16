# ecat_digital_io

## Overview
This example demonstrates how to develop a remote IO device based on Slave Stack Code(SSC).

This remote IO device only supports SM-Synchron mode for DC operation, and implements one Input channel and one Output channel:
	Output channel: connected with the D6 LED and used to control this LED.
	Input channel: used to read the D6 LED current status. 

Note that the demo CAN NOT be built directly. Please see the "Prepare the Demo" section for more details.

## Running the demo
When the example is running, the serial port will output:

       Start the SSC digital_io example...

After the PLC program started, the D6 LED starts to blink with 0.5Hz frequency.

NOTE: refer to the User Guide for the details of how to setup the PLC project on TwinCAT3.

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ecat_examples/digital_io/example_board_readme.md)
