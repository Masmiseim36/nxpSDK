# dmic_multi_channel

## Overview
The dmic multi channel example is a simple demonstration program about how to use the DMIC multi channel to gathering audio data with dma.

In this example, dmic channel 0, 1, 2, 3, 4, 5, 6, 7 will be used to record audio data, the raw audio data will be stored:
In buffer 0:
dmic0, dmic1, dmic2, dmic3, dmic0, dmic1.......
In buffer 1:
dmic4, dmic5, dmic6, dmic7, dmic4, dmic5.......

Then another two dma channels will convert the raw audio data to the required TDM format,
One DMA channel responsible for copy buffer 0 to the playback buffer
Another DMA channel responsible for copy buffer 1 to the playback buffer

Finally, the data in the playback buffer will be organized as below:
dmic0, dmic4, dmic1, dmic5, dmic2, dmic6, dmic3, dmic7......

At last the data in the playback buffer will be sent out through I2S with TDM format.

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/dmic/dmic_multi_channel/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/dmic/dmic_multi_channel/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/dmic/dmic_multi_channel/example_board_readme.md)
