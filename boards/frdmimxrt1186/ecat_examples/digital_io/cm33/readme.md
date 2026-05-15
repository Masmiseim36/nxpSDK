# ecat_digital_io

## Overview
This example demonstrates how to develop a remote IO device based on Slave Stack Code(SSC).

This remote IO device only supports SM-Synchron mode for DC operation, and implements one Input channel and one Output channel:

	Output channel: connected with the LED and used to control this LED.
	
	Input channel: used to read the LED current status. 

Note that the demo CAN NOT be built directly. Please see the "Prepare the Demo" section for more details.

## Running the demo
When the example is running, the serial port will output:

       Start the SSC digital_io example...

After the PLC program started, the LED starts to blink with 0.5Hz frequency.

NOTE: refer to the User Guide for the details of how to setup the PLC project on TwinCAT3.

## TwinCAT Setup

To demonstrate this IO example, TwinCAT is used as the EtherCAT MainDevice.
- [Generate SSC Code for digital_io](../topics/generate_ssc_for_digital_io.md)
- [TwinCAT Installation](../topics/download_and_install_the_twincat_software.md)
- [TwinCAT EEPROM Update](../topics/twincat_eeprom_update_digital_io.md)
- [TwinCAT Project Setup](../topics/twincat_project_setup_digital_io.md)
- [Note](../topics/note_about_the_source_code_in_the_document.md)

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ecat_examples/digital_io/example_board_readme.md)
- [FRDM-MIMXRT1186](../../_boards/frdmimxrt1186/ecat_examples/digital_io/example_board_readme.md)
- [iMX943-EVK](../../_boards/imx943evk/ecat_examples/digital_io/example_board_readme.md)
