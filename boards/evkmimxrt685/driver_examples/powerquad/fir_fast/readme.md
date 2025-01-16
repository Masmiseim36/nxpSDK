# powerquad_fir_fast

## Overview
The PowerQuad fetches data from system RAM and private RAM through different
path, for the calculations which need data from two parts of memory, such as
FIR, convolve, correlate, and some matrix operations, place the input data B
in private RAM improves the performance.
PowerQuad private RAM starts from address 0xe0000000, the first 4KByte is used
by PowerQuad driver, the RAM starts from 0xe0001000 could be used by user
for optimization. In this example, the FIR taps, convolve input data B, and correlate
input data B are not changed, so they are converted to float format and saved
to private RAM at the beginning.
If the example runs successfully, the performance using optimized method is better
than the normal method.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/powerquad/fir_fast/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/powerquad/fir_fast/example_board_readme.md)
