# qtmr_inputcapture_outputpwm_dma

## Overview

The QTMR project is a demonstration program of the SDK QTMR driver's input capture and output pwm feature by DMA.
The example sets up a QTMR channel for input capture. Once the input signal is received, this example will print the capture value.
The example also sets up one QTMR channel to output pwm. The example enables DMA and input edge flag setting will trigger DMA read
request for CAPT register. The user should probe a 50Khz PWM signal output with 50% dutycycle with a oscilloscope.
The user can enter a value to update the Duty cycle, when 5 is entered, the duty cycle will be set to 50%.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [EVK-MIMXRT1015](../../../_boards/evkmimxrt1015/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/qtmr/inputcapture_outputpwm_dma/example_board_readme.md)
