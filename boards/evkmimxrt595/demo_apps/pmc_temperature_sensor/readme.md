# pmc_temperature_sensor

## Overview

The demo shows how to measure the temperature within the PMC module.

In this demo, the ADC input channel is mapped to an internal temperature sensor. When running the project, typing
any key into debug console would trigger a series of ADC conversions. When the conversions are done, the temperature is
calculated per the specific formula.

## Supported Boards
- [EVK-MCIMX7ULP](../../_boards/evkmcimx7ulp/demo_apps/pmc_temperature_sensor/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/demo_apps/pmc_temperature_sensor/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/demo_apps/pmc_temperature_sensor/example_board_readme.md)
