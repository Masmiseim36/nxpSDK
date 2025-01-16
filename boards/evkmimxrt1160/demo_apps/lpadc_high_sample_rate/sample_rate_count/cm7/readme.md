# lpadc_sample_rate_count

## Overview
The lpadc sample rate count demo application can be used to measure ADC's sample rate roughly. The sample rate
for an ADC is defined as the number of output samples available per unit time, and is specified as samples per
second(SPS).
The sample rate is the reciprocal of one sample total conversion time. Total conversion time = sampling time + compare time
In this demo, users can select sample time, and the compare time is different in different devices, please see device datasheet
for details.
In this demo the ADC conversion clock is configured as the maximum frequency(Please refer to the datasheet for the
maximin value). So theoretically, sample rate = maximum frequency / total conversion time.

## Supported Boards
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/demo_apps/lpadc_high_sample_rate/sample_rate_count/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/demo_apps/lpadc_high_sample_rate/sample_rate_count/example_board_readme.md)
- [MIMXRT1180-EVK](../../../../_boards/evkmimxrt1180/demo_apps/lpadc_high_sample_rate/sample_rate_count/example_board_readme.md)
