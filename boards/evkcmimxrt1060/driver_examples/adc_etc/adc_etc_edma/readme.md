# adc_etc_edma

## Overview

The adc_etc_edma example shows how to use the edma to transfer ADC convert result.

In this example, PIT is used as the hardware trigger source and is triggered once per second.
Once ADC gets the trigger from the ADC_ETC, the conversion goes.
When the conversion is done, the dma request will be sent, and the dma will transfers the conversion
result to a global variable. 
The user presses the keyboard according to the prompt in the serial terminal to see the ADC conversion result.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/adc_etc/adc_etc_edma/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/adc_etc/adc_etc_edma/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/adc_etc/adc_etc_edma/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/adc_etc/adc_etc_edma/example_board_readme.md)
