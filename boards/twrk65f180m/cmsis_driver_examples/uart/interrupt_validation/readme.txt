Overview
========
The cmsis_uart_interrupt_validation example tests cmsis uart interrupt driver using CMSIS Driver Validation Suite 
which is provided by ARM to do some general validation, for more details of CMSIS Driver Validation pack you could 
refer to https://www.keil.com/pack/doc/CMSIS/Driver/html/_driver_validation.html.

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
- Mini USB cable
- TWR-K65F180M board
- Personal Computer

Board settings
==============
-A10 , B9 connected.

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
4.  Reset the SoC and run the project.

Running the demo
================
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

CMSIS-Driver Test Suite   Jul 29 2016   15:24:25 

TEST 01: USART_GetCapabilities            PASSED
TEST 02: USART_Initialization             PASSED
TEST 03: USART_PowerControl               
  DV_USART.c (282) [WARNING] Low power is not supported
TEST 04: USART_Config_PolarityPhase       NOT EXECUTED
TEST 05: USART_Config_DataBits            PASSED
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

Test Summary: 13 Tests, 11 Executed, 9 Passed, 0 Failed, 2 Warnings.
Test Result: WARNING

Customization options
=====================

