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
	   	Image version: 2.4.0
		Image state: testing
		Set image as confirmed: success

## Supported Boards
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/ecat_examples/foe/example_board_readme.md)
