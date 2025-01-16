# lpadc_etc_hardware_trigger_conv

## Overview

The adc_etc_hardware_trigger_conv example shows how to use the ADC_ETC to generate a ADC trigger by PIT channel0 trigger.

Every 1 second, PIT channel0 would send a trigger signal to ADC_ETC, which can arbitrate and manage multiple external triggers,
and ADC_ETC would generate ADC trigger.

In this example, the ADC is configured with hardware trigger. Once ADC gets the trigger from the ADC_ETC, the conversion goes,
then the ADC_ETC ISR would be executed.

## Supported Boards
- [MIMXRT1170-EVKB](../../../../_boards/evkbmimxrt1170/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [MIMXRT1160-EVK](../../../../_boards/evkmimxrt1160/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
