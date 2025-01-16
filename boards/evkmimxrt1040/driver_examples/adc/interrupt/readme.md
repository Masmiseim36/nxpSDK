# adc_12b1msps_sar_interrupt

## Overview

The adc_interrupt example shows how to use interrupt with adc driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the adc's
sample input. When running the project, typing any key into debug console would trigger the conversion. ADC interrupt 
would be asserted once the conversion is completed. In ADC ISR, the conversion completed flag would be cleared by 
reading the conversion result value. Also, the conversion result value is stored, and the ISR counter is increased. 
These information would be printed when the execution return to the main loop.

The point is that the adc interrupt configuration is set when configuring the adc's conversion channel. When in 
software trigger mode, the conversion would be launched by the operation of configuring channel, just like writing a
conversion command. So if user wants to generate the interrupt every time the conversion is completed, the channel's 
configuration with enabling interrupt setting would be used for each conversion.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/adc/interrupt/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/adc/interrupt/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/adc/interrupt/example_board_readme.md)
- [EVK-MIMXRT1010](../../../_boards/evkmimxrt1010/driver_examples/adc/interrupt/example_board_readme.md)
- [EVK-MIMXRT1015](../../../_boards/evkmimxrt1015/driver_examples/adc/interrupt/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/adc/interrupt/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/adc/interrupt/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/adc/interrupt/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/adc/interrupt/example_board_readme.md)
