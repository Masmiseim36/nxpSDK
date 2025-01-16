# lpadc_high_sample_rate_sample_signal

## Overview
This demo application demonstrates the use of the LPADC to sample the analog signal. In this demo, the ADC clock is
set as the maximum frequency, users should input analog signals to the ADC channel, press any keys to trigger the ADC
conversion. After sampling enough sample points, the ADC will be disabled and the sample data will be print via UART.
Users can use some tools such as EXCEL to recover the input signal based on those printed data.

## Supported Boards
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/demo_apps/lpadc_high_sample_rate/sample_signal/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/demo_apps/lpadc_high_sample_rate/sample_signal/example_board_readme.md)
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/demo_apps/lpadc_high_sample_rate/sample_signal/example_board_readme.md)
