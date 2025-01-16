# trng_random

## Overview
The True Random Number Generator (TRNG) is a hardware accelerator module that generates a 512-bit
entropy as needed by an entropy consuming module or by other post processing functions. The TRNG
Example project is a demonstration program that uses the KSDK software to generate random numbers
and prints them to the terminal.

NOTE:

On i.MXRT1020/1050/1060, the TRNG entropy register is initialized by the ROM boot process
with 128 entropy bits (read from registers ENT12-ENT15, sampleSize = 128).
 
The TRNG driver version <= 2.0.2 has issue that TRNG_Init() function doesn't flush
these entropy bits, thus, the first TRNG_GetRandomData(base, data, 64) after TRNG_Init()
only reads 384 non-random bits followed by 128 random bits. After the first call, next calls
to TRNG_GetRandomData() return entropy bits collected with new TRNG settings.

The issue is fixed in TRNG driver version 2.0.3, by regenerating entropy bits with new
TRNG settings already during TRNG_Init().

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/trng/random/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/trng/random/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/trng/random/example_board_readme.md)
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/trng/random/example_board_readme.md)
- [EVK-MIMXRT1010](../../../_boards/evkmimxrt1010/driver_examples/trng/random/example_board_readme.md)
- [EVK-MIMXRT1015](../../../_boards/evkmimxrt1015/driver_examples/trng/random/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/trng/random/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/trng/random/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/trng/random/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/trng/random/example_board_readme.md)
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/trng/random/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/trng/random/example_board_readme.md)
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/driver_examples/trng/random/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/trng/random/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/trng/random/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/trng/random/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/trng/random/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/trng/random/example_board_readme.md)
