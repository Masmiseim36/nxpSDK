# pdm_sai_multi_channel_edma

## Overview
The pdm sai multi channel edma example shows how to use pdm driver with edma:

In this example, pdm gathers two channels' audio data though edma, then sai sends it to codec, the received DMIC data format,
 ----------------------------------------------------------------------------------------------------------------------
 |CHANNEL0 | CHANNEL1 | CHANNEL2 | .... | CHANNEL0 | CHANNEL 1 | CHANNEL2 |....| CHANNEL0 | CHANNEL 1 | CHANNEL2 |....|
 ----------------------------------------------------------------------------------------------------------------------

## Supported Boards
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/pdm/pdm_sai_multi_channel_edma/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/pdm/pdm_sai_multi_channel_edma/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/pdm/pdm_sai_multi_channel_edma/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/pdm/pdm_sai_multi_channel_edma/example_board_readme.md)
