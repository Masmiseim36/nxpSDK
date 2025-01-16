# lpadc_temperature_measurement

## Overview

The lpadc_temperature_measurement example shows how to measure the temperature within the internal sensor.

In this example, the ADC input channel is mapped to an internal temperature sensor. When running the project, typing
any key into debug console would trigger the conversion. ADC watermark interrupt would be asserted once the number of
datawords stored in the ADC Result FIFO is greater than watermark value. In ADC ISR, the watermark flag would be
cleared by reading the conversion result value. When the conversion done, two valid result will be put in the FIFO,
then the temperature can be calculated within the two results and a specific formula.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/lpadc/temperature_measurement/example_board_readme.md)
