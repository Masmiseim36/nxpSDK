# adc_12b1msps_sar_polling

## Overview

The adc_polling example shows the simplest way to use adc driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the adc's
sample input. When running the project, typing any key into debug console would trigger the conversion. The execution 
would check the conversion completed flag in loop until the flag is asserted, which means the conversion is completed. 
Then read the conversion result value and print it to debug console.

Note, the default setting of initialization for the ADC converter is just an available configuration. User can change
the configuration structure's setting in application to fit the special requirement.

The auto-calibration is not essential but strongly recommended. It can help to adjust the converter itself and improve
the adc's performance.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/adc/polling/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/adc/polling/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/adc/polling/example_board_readme.md)
- [EVK-MIMXRT1010](../../../_boards/evkmimxrt1010/driver_examples/adc/polling/example_board_readme.md)
- [EVK-MIMXRT1015](../../../_boards/evkmimxrt1015/driver_examples/adc/polling/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/adc/polling/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/adc/polling/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/adc/polling/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/adc/polling/example_board_readme.md)
