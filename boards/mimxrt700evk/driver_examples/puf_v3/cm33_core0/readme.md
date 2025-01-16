# puf_v3

## Overview
The PUF Example project is a demonstration program that uses the KSDK software implement secure key storage using PUF software driver.

Usual use consists of these steps:
1. 	Enroll: The controller retrieves the Startup Data (SD) from the memory (SRAM), derives a digital fingerprint, 
	generates the corresponding Activation Code (AC) and sends it to the storage system. 
	Perform this step only once for each device. There is a control register that can block further enrollment. 
	This control register is write only and is reset on a power-on reset.

2. 	Start: The AC generated during the enroll operation and the SD are used to reconstruct the digital fingerprint. 
	It is done after every power-up and reset.

3. 	Generate Key: The controller generates an unique key and combines it with the digital fingerprint to output a key code. 
	Each time a Generate Key operation is executed a new unique key is generated.

4. 	Set Key: The digital fingerprint generated during the Enroll/Start operations and the key provided by the Client Design (CD) 
	are used to generate a Key Code. This KC can be stored externally. Perform this operation only once for each key.

5. 	Get Key: The digital fingerprint generated during the Start operation and the KC generated during a Set Key operation 
	are used to retrieve a stored key. Perform this operation every time a key is needed.

## Supported Boards
- [FRDM-MCXN236](../../_boards/frdmmcxn236/driver_examples/puf_v3/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/driver_examples/puf_v3/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/driver_examples/puf_v3/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/driver_examples/puf_v3/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/driver_examples/puf_v3/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/driver_examples/puf_v3/example_board_readme.md)
