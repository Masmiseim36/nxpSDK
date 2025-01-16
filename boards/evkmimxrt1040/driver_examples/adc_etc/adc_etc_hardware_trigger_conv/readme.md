# adc_etc_hardware_trigger_conv

## Overview

The adc_etc_hardware_trigger_conv example shows how to use the ADC_ETC to generate a ADC trigger by PIT channel0 trigger.

Every 1 second, PIT channel0 would send a trigger signal to ADC_ETC, which can arbitrate and manage multiple external triggers,
and ADC_ETC would generate ADC trigger.

In this example, the ADC is configured with hardware trigger. Once ADC gets the trigger from the ADC_ETC, the conversion goes,
then the ADC_ETC ISR would be executed.

## Supported Boards
- [EVKB-IMXRT1050](../../../../_boards/evkbimxrt1050/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../../_boards/evkbmimxrt1060/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../../_boards/evkcmimxrt1060/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [EVK-MIMXRT1010](../../../../_boards/evkmimxrt1010/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [EVK-MIMXRT1015](../../../../_boards/evkmimxrt1015/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [EVK-MIMXRT1020](../../../../_boards/evkmimxrt1020/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [MIMXRT1024-EVK](../../../../_boards/evkmimxrt1024/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [MIMXRT1040-EVK](../../../../_boards/evkmimxrt1040/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
- [EVK-MIMXRT1064](../../../../_boards/evkmimxrt1064/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/example_board_readme.md)
