# dsp_sema42

## Overview

The dsp_sema42 demo application demonstrates starting DSP core with DSP image.

In this example:
1. Firstly, CM33 core turn on LED  and lock a sema gate then boot up DSP core wake up.
2. DSP core must be wait until CM33 core unlocks this sema gate to lock it.
3. After user press any key in terminal window, the sema gate will be unlocked by CM33 core,
then DSP core will lock it and turn off the LED

If the board does not have LED to show the status, then DSP core will send a flag
to CM33 core when DSP core has locked the sema gate. CM33 core outputs the success log
after it received the flag.

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/dsp_examples/sema42/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/dsp_examples/sema42/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/dsp_examples/sema42/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/dsp_examples/sema42/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/dsp_examples/sema42/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/dsp_examples/sema42/example_board_readme.md)
