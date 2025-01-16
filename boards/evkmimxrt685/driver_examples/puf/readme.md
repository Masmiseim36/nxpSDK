# puf

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
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/driver_examples/puf/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/driver_examples/puf/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/driver_examples/puf/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/driver_examples/puf/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/driver_examples/puf/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/driver_examples/puf/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/driver_examples/puf/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/driver_examples/puf/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/driver_examples/puf/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/driver_examples/puf/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/driver_examples/puf/example_board_readme.md)
