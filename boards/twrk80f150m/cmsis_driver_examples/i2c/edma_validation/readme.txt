Overview
========
The cmsis_i2c_edma_validation example tests cmsis i2c edma driver using CMSIS Driver Validation Suite 
which is provided by ARM to do some general validation, for more details of CMSIS Driver Validation pack 
you could refer to https://www.keil.com/pack/doc/CMSIS/Driver/html/_driver_validation.html

In this example , we use one I2C as test instance.we test the following functions:
1: I2C_GetCapabilities            
2: I2C_Initialization              
3: I2C_PowerControl                 
4: I2C_SetBusSpeed                  
5: I2C_SetOwnAddress                
6: I2C_AbortTransfer              

Toolchain supported
===================
- Keil MDK 5.20

Hardware requirements
=====================
- Mini/Micro USB cable
- TWR-K80F150M board
- Personal Computer
- Elevator Tower

Board settings
==============
No special is needed

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

CMSIS-Driver Test Suite   Jul 28 2016   14:56:53 

TEST 01: I2C_GetCapabilities              PASSED
TEST 02: I2C_Initialization               PASSED
TEST 03: I2C_PowerControl                 
  DV_I2C.c (134) [WARNING] Low power is not supported
TEST 04: I2C_SetBusSpeed                  
  DV_I2C.c (180) [WARNING] High speed is not supported
TEST 05: I2C_SetOwnAddress                NOT EXECUTED
TEST 06: I2C_BusClear                     NOT EXECUTED
TEST 07: I2C_AbortTransfer                PASSED
TEST 08: I2C_CheckInvalidInit             NOT EXECUTED

Test Summary: 8 Tests, 5 Executed, 3 Passed, 0 Failed, 2 Warnings.
Test Result: WARNING
Customization options
=====================

