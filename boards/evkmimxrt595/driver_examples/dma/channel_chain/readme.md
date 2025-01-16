# dma_channel_chain

## Overview
The DMA channel chain transfer example is a simple demonstration program about how to use channel
chain feaute with the SDK software.
In this example three channel is used,
```
          ------->channel1------->channel0
          |
channel0-->
          |
          ------->channel2
```
Channel0 is configured with two descriptor, first descriptor is linked to the second, then trigger
the channel0 by software, after channel0 first descriptor finish, it will trigger channel1 and
channel2 start transfer, after channel1 descriptor exhaust it will trigger channel0, then channel0
second descritpor will be carried out, after second descriptor exhaust, example finish.

## Supported Boards
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/dma/channel_chain/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/dma/channel_chain/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso51U68](../../../_boards/lpcxpresso51u68/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso55S28](../../../_boards/lpcxpresso55s28/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso824MAX](../../../_boards/lpcxpresso824max/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/dma/channel_chain/example_board_readme.md)
- [LPCXpresso860MAX](../../../_boards/lpcxpresso860max/driver_examples/dma/channel_chain/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/dma/channel_chain/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/dma/channel_chain/example_board_readme.md)
