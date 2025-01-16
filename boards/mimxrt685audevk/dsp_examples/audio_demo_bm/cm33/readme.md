# dsp_audio_demo_bm

## Overview
The audio baremetal demo demonstrates audio processing using DSP core.
It uses the DMIC working with I2S. One channel Audio data is converted to samples in the DMIC module.
Then, the data is placed into the memory buffer. Last, it is sent to the I2S
buffer and output to the CODEC, where the audio data will be heard from lineout.

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/dsp_examples/audio_demo_bm/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/dsp_examples/audio_demo_bm/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/dsp_examples/audio_demo_bm/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/dsp_examples/audio_demo_bm/example_board_readme.md)
