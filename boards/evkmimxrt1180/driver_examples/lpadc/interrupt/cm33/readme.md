# lpadc_interrupt

## Overview

The lpadc_interrupt example shows how to use interrupt with LPADC driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the LPADC's
sample input. When running the project, typing any key into debug console would trigger the conversion. ADC watermark 
interrupt would be asserted once the number of datawords stored in the ADC Result FIFO is greater than watermark value.
In ADC ISR, the watermark flag would be cleared by reading the conversion result value. Also, result information would
be printed when the execution return to the main function.

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/lpadc/interrupt/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/lpadc/interrupt/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/lpadc/interrupt/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/lpadc/interrupt/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/lpadc/interrupt/example_board_readme.md)
- [EVK-MIMXRT595](../../../_boards/evkmimxrt595/driver_examples/lpadc/interrupt/example_board_readme.md)
- [EVK-MIMXRT685](../../../_boards/evkmimxrt685/driver_examples/lpadc/interrupt/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/lpadc/interrupt/example_board_readme.md)
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/lpadc/interrupt/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/lpadc/interrupt/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/lpadc/interrupt/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/lpadc/interrupt/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/lpadc/interrupt/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/lpadc/interrupt/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/lpadc/interrupt/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/lpadc/interrupt/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lpadc/interrupt/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lpadc/interrupt/example_board_readme.md)
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/lpadc/interrupt/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/lpadc/interrupt/example_board_readme.md)
- [LPCXpresso55S28](../../../_boards/lpcxpresso55s28/driver_examples/lpadc/interrupt/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/lpadc/interrupt/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/lpadc/interrupt/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/lpadc/interrupt/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/lpadc/interrupt/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/lpadc/interrupt/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../_boards/mimxrt685audevk/driver_examples/lpadc/interrupt/example_board_readme.md)
- [MIMXRT700-EVK](../../../_boards/mimxrt700evk/driver_examples/lpadc/interrupt/example_board_readme.md)
