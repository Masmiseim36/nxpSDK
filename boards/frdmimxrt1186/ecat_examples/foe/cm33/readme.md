# ecat_foe

## Overview
This example demonstrates how to develop EtherCAT FoE(File Access over EtherCAT) example based on Slave Stack Code(SSC).

Note that the demo CAN NOT be built directly. Please see the "Prepare the Demo" section for more details.


## Running the demo
When FoE upgrade is successful, the serial port will output:

		System reset...
		hello sbl.
		Bootloader Version 1.9.0
		Primary image: magic=good, swap_type=0x1, copy_done=0x3, image_ok=0x1
		Secondary image: magic=good, swap_type=0x1, copy_done=0x3, image_ok=0x3
		Boot source: none
		Swap type: test
		Starting swap using move algorithm.
		Bootloader chainload address offset: 0x40000
		Reset_Handler address offset: 0x40400
		Jumping to the image

       	Start the SSC FoE example...
	   	Image version: 2.3.0
		Image state: testing
		Set image as confirmed: success

## TwinCAT Setup

To demonstrate this FoE example, TwinCAT is used as the EtherCAT MainDevice.
- [Generate SSC Code for FoE](../topics/generate_ssc_for_foe.md)
- [TwinCAT Installation](../topics/download_and_install_the_twincat_software.md)
- [TwinCAT EEPROM Update](../topics/twincat_eeprom_update_foe.md)
- [TwinCAT Project Setup](../topics/twincat_project_setup_foe.md)
- [Note](../topics/note_about_the_source_code_in_the_document.md)

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ecat_examples/foe/example_board_readme.md)
- [FRDM-MIMXRT1186](../../_boards/frdmimxrt1186/ecat_examples/foe/example_board_readme.md)
