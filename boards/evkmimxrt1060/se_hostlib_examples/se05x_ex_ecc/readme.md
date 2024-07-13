
SDK version
===========
- Version: 2.16.000

Toolchain supported
===================
- MCUXpresso  11.10.0
- IAR embedded Workbench  9.60.1
- Keil MDK  5.39.0
- GCC ARM Embedded  13.2.1

Hardware requirements
=====================
- Mini/micro USB cable
- EVK-MIMXRT1060 board
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
- For running ex_rsa example enable following options in the fsl_sss_ftr.h file:
  SSS_HAVE_APPLET_SE05X_C - 1
  SSS_HAVE_RSA - 1
