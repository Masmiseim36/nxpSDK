# lpit_adc

## Overview
The lpit_adc example shows how to use LPIT to generate ADC triggers. 
On top of the basic counter, to use the ADC trigger, simply enable 
the "milestone" of the ADC trigger and set it with a user-defined value. 
When the LPIT counter is on, when the count passes the "milestone", 
a pre-trigger of the ADC is generated and sent to the ADC module. 
In this example, the ADC is configured with hardware triggering and 
conversion complete interrupt enabled. Once it gets a trigger from the LPIT, 
the transition occurs and then the ISR is executed.

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/lpadc/lpit_adc/example_board_readme.md)
