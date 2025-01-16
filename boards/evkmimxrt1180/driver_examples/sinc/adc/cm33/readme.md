# sinc_adc

## Overview
The sinc adc example shows how to use SINC driver to configure SINC module as single conversion mode to convert
an external ADC sigma-delta modulator bitstream to a data stream. For this example, the over sample ratio is set as 128,
the order is 3, and the SINC result data is signed.
Theoretically,
When ADCin = 0V, ADC output 50% duty of MBIT, sinc result is 0x0(If unsigned mode, sinc result is 128^3/2 = 0x10_0000);
when ADCin = + maximum input voltage, ADC output always output 1(100% duty), SINC output is 128^3 = 0x20_0000.
when ADCin = - maximum input voltage, ADC output always output 0(0% duty), SINC output is -128^3 = 0xE0_0000(If unsigned
mode, sinc result is 0x0).
For ADCin between -maximum input voltage and +maximum input voltage, the SINC result is ADCin/maximum * 0x20_0000.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/sinc/adc/example_board_readme.md)
