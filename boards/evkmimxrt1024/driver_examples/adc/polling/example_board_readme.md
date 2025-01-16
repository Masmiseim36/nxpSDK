Hardware requirements
=====================
- Mini/micro USB cable
- MIMXRT1024-EVK board
- Personal Computer

Board settings
============
ADC1_IN10 is ADC input. Please sample voltage by J18-2 pin.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC polling Example.
ADC_DoAutoCalibration() Done.
ADC Full Range: XXXX
Press any key to get user channel's ADC value.
ADC Value: 951
Press any key to get user channel's ADC value.
ADC Value: 1169
Press any key to get user channel's ADC value.
ADC Value: 1243
Press any key to get user channel's ADC value.
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J23.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.
