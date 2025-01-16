Hardware requirements
=====================
- Micro USB cable
- EVK-MIMXRT595 board
- Personal Computer

Board settings
============
- Input voltage signal(0~1.8V) to J30-1(LPADC0 CH0).

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J40) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, following information can be seen on the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC DMA Example

ADC Full Range: 4096

Full channel scale (Factor of 1).

Please press any key to trigger the conversion.

Adc conversion word : 0x81006148

ADC conversion value: 3113

Adc conversion word : 0x81006140

ADC conversion value: 3112

Adc conversion word : 0x81006140

ADC conversion value: 3112
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
