# dmic_i2s_codec

## Overview
Demonstrates the DMIC working with I2S. Audio is converted to samples in the DMIC module.
Then, the data is placed into the I2S buffer directly without DMA or CPU intervention. Last, it is read from the I2S buffer and sent
to the CODEC, then the audio data will be output to Lineout of CODEC.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/dmic/dmic_i2s_codec/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/dmic/dmic_i2s_codec/example_board_readme.md)
