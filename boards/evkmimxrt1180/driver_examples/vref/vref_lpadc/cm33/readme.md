# vref_lpadc

## Overview
The vref example shows how to use the vref driver.

In this example, we first get the default trim value of the VREF and measure
the VREF output through the LPADC to get the default output voltage. Adjust
the trim value by 30 steps to create a 20mv (30*0.5*4/3=20mv) voltage difference,
The theoretical voltage value should be 20mv greater than or less than the
default output voltage, then use the LPADC to measure the VREF output again,
the measured voltage value should close to the theoretical voltage.