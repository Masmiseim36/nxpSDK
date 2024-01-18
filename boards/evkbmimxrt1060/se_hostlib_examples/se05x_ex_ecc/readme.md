
SDK version
===========
- Version: 2.15.000

Toolchain supported
===================
- MCUXpresso  11.8.0
- IAR embedded Workbench  9.40.1
- Keil MDK  5.38.1
- GCC ARM Embedded  12.2

Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-MIMXRT1060 board
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
Jumper settings for OM- SE050ARD & OM- SE051ARD:
    -J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
    -J15 3-4 (Default): I2C target SDA connection
    -J17 3-4 (Default): I2C target SCL connection


For SE050
- Connect OM- SE050ARD to the board.
- Please consult SE050 user manual and Plug & Trust middleware documentation which is present in the
  Plug & Trust MW package for further details.
- The Plug & Trust MW package can be downloaded from
https://www.nxp.com/products/security-and-authentication/authentication/:SE050?tab=Design_Tools_Tab
- Please refer to the RST file in the doc folder for additional details about the example.
