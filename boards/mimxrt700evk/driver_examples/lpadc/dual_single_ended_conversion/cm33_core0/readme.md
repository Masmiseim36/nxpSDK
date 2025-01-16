# lpadc_dual_single_ended_conversion

## Overview

The lpadc_dual_single_ended_conversion example shows how to use two channel with LPADC driver.

In this example, user should indicate two channel to provide a voltage signal (can be controlled by user) as the LPADC's
sample input, Channel B does not need to be paired with Channel A, Channel B is user selectable.When running the project, 
typing any key into debug console would trigger the conversion. The execution would check the FIFO valid flag in loop until
the flag is asserted, which means the conversion is completed. Then read the conversion result value and print it to debug console.

Note, the default setting of initialization for the ADC converter is just an available configuration. User can change
the configuration structure's setting in application to fit the special requirement.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/lpadc/dual_single_ended_conversion/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/lpadc/dual_single_ended_conversion/example_board_readme.md)
