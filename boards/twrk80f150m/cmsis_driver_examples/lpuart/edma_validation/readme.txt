Overview
========
The cmsis_lpuart_edma_validation example tests cmsis lpuart edma driver using CMSIS Driver Validation Suite 
which is provided by ARM to do some general validation, for more details of CMSIS Driver Validation pack 
you could refer to https://www.keil.com/pack/doc/CMSIS/Driver/html/_driver_validation.html

In this example, we use one uart as test instance and test the following functions:
01: USART_GetCapabilities         
02: USART_Initialization             
03: USART_PowerControl               
04: USART_Config_DataBits           
05: USART_Config_StopBits            
06: USART_Config_Parity            
07: USART_Config_Baudrate          
08: USART_Config_CommonParams      
09: USART_Loopback_CheckBaudrate    
10: USART_Loopback_Transfer         
11: USART_CheckInvalidInit  


Toolchain supported
===================
- Keil MDK 5.20

Hardware requirements
=====================
- Micro USB cable
- TWR-K80F150M board
- Personal Computer

Board settings
==============
The example is a loopback test.
The connection should be set as following:
- PTA15 - A41(LPUART0_RX) , PTA14 - A42(LPUART0_TX) connected

Prepare the Demo
================
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.
CMSIS-Driver Test Suite   Jul 27 2016   10:57:30 

TEST 01: USART_GetCapabilities            PASSED
TEST 02: USART_Initialization             PASSED
TEST 03: USART_PowerControl               
  DV_USART.c (282) [WARNING] Low power is not supported
TEST 04: USART_Config_PolarityPhase       NOT EXECUTED
TEST 05: USART_Config_DataBits            
  DV_USART.c (352) [WARNING] Data Bits = 5 are not supported
  DV_USART.c (356) [WARNING] Data Bits = 6 are not supported
  DV_USART.c (360) [WARNING] Data Bits = 7 are not supported
  DV_USART.c (368) [WARNING] Data Bits = 9 are not supported
TEST 06: USART_Config_StopBits            
  DV_USART.c (406) [WARNING] Stop Bits = 1.5 are not supported
  DV_USART.c (410) [WARNING] Stop Bits = 0.5 are not supported
TEST 07: USART_Config_Parity              PASSED
TEST 08: USART_Config_Baudrate            PASSED
TEST 09: USART_Config_CommonParams        PASSED
TEST 10: USART_Send                       NOT EXECUTED
TEST 11: USART_Loopback_CheckBaudrate     PASSED
TEST 12: USART_Loopback_Transfer          PASSED
TEST 13: USART_CheckInvalidInit           PASSED

Test Summary: 13 Tests, 11 Executed, 8 Passed, 0 Failed, 3 Warnings.
Test Result: WARNING




Customization options
=====================

