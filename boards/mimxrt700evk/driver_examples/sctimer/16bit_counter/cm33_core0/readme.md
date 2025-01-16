# sctimer_16bit_counter

## Overview
The SCTimer 16-bit counter project is a demonstration program of the SDK SCTimer driver operation when using the SCTimer counter
as two 16-bit counters.
The example toggles an output per counter when a match occurs.

To use any 16-bit counter, this project disables the Unify 32-bit Counter by hardware limit, no matter the Low 16-bit one or the
High 16-bit one. Both the Low 16-bit counters or the High 16-bit counters  enable bidirectional mode to extend the 16-bit counting
range. When the counter is in bidirectional mode, the effect of setting and clearing the output depends on whether the counter is
counting up or down.
The 16-bit low counter is scheduled for a match event every 0.1 seconds, and the 16-bit High counter is scheduled for a match event
every 0.2 seconds. When a 16-bit counter event occurs, the output is toggled and the counter is reset.

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso51U68](../../../_boards/lpcxpresso51u68/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso55S28](../../../_boards/lpcxpresso55s28/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso824MAX](../../../_boards/lpcxpresso824max/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/sctimer/16bit_counter/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/sctimer/16bit_counter/example_board_readme.md)
