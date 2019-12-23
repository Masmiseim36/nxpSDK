Overview
========
This is an CADC demo application which shows how to use hardware trigger sources to handle
the CADC hardware trigger function. The Low Power Timer (LPTMR) is a period timer source and the CADC
hardware trigger event. Because the LPTMR trigger event can be used to trigger one of the CADC ,
this demo uses the LPTMR as a trigger source for a CADCx channel. The LPTMR triggers the CADC in a
fixed frequency and the demo gets the CADC conversion result in the CADC Conversion Complete (COCO) interrupt.
This demo uses DAC fifo to generate sinewave on DACx_OUT.

Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- HVP-KV46F150M board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
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
The log below shows the output of the cadc_lptmr_trigger demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
adc_lptmr_trigger demo running...

                      *****                                             *****
                   ***     ****                                    * ***     ****
                 **            *                                    *            *
                *                *                                *                *
              **                * **                            **                * **
            **                      *                         **                      *
           *                          *                      *                          *
         **                          *  *                  **                          *  *
       **                              * *               **                              * *
      *                                    *            *                                    *
  ****                                    * **      ****                                    * ***
**                                            ******                                             ***

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

