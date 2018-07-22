Overview
========
The cmsis_dspi_edma_validation example tests cmsis dspi edma driver using CMSIS Driver Validation Suite 
which is provided by ARM to do some general validation, for more details of CMSIS Driver Validation pack 
you could refer to https://www.keil.com/pack/doc/CMSIS/Driver/html/_driver_validation.html

The cmsis_dspi_edma_validation example shows how to test CMSIS DSPI driver in edma way:
In this example , we just use one SPI as test instance. Against the driver specification, please reference 
the test issues as follow:
    -Main function cmsis_dv() where test cases are executed in main.c file.
    -Test configure options can be set in DV_Config.h with the Configuration Wizard.
    -Please set special param value as follow and keep the other SPI test params as default value:
      -Driver_SPI#         0
      -SPI data bits       16
      (In this validation suit,our transfer data size is 512.but the largest size of transfer buffer is 511 
      in 8 data bits mode.So, please make sure that the SPI data bits is set to 16.)
For driver loopback testing, please ensure that the MISO and MOSI are connected directly.


Toolchain supported
===================
- Keil MDK 5.20

Hardware requirements
=====================
- Mini/micro USB cable
- Two board TWR-K80F150M
- Personal Computer
- Primary Elevator Tower

Board settings
==============
To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI0)     CONNECTS TO         INSTANCE1(SPI0)
Pin Name   Board Location     Pin Name  Board Location
SIN            B44               SOUT         B45
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
CMSIS-Driver Test Suite   

TEST 01: SPI_GetCapabilities              PASSED
TEST 02: SPI_Initialization               PASSED
TEST 03: SPI_PowerControl                 
  DV_SPI.c (272) [WARNING] Low power is not supported
TEST 04: SPI_Config_PolarityPhase         PASSED
TEST 05: SPI_Config_DataBits              PASSED
TEST 06: SPI_Config_BitOrder              PASSED
TEST 07: SPI_Config_SSMode                PASSED
TEST 08: SPI_Config_BusSpeed              PASSED
TEST 09: SPI_Config_CommonParams          PASSED
TEST 10: SPI_Send                         PASSED
TEST 11: SPI_Receive                      PASSED
TEST 12: SPI_Loopback_CheckBusSpeed       
  DV_SPI.c (680) [WARNING] At 400kHz: measured time is 0.557440 x expected time
  DV_SPI.c (680) [WARNING] At 1000kHz: measured time is 0.618715 x expected time
  DV_SPI.c (680) [WARNING] At 4000kHz: measured time is 0.889319 x expected time
  DV_SPI.c (680) [WARNING] At 10000kHz: measured time is 1.510682 x expected time
  DV_SPI.c (680) [WARNING] At 25000kHz: measured time is 3.226224 x expected time
  DV_SPI.c (680) [WARNING] At 50000kHz: measured time is 6.051354 x expected time
TEST 13: SPI_Loopback_Transfer            PASSED
TEST 14: SPI_CheckInvalidInit             PASSED

Test Summary: 14 Tests, 14 Executed, 12 Passed, 0 Failed, 2 Warnings.
Test Result: WARNING
Customization options
=====================

