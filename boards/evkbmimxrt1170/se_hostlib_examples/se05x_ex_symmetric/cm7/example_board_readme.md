Hardware requirements
=====================
- Mini/micro USB cable
- EVKB-MIMXRT1170 board with adapter
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

Board settings
==============
Jumper settings for OM- SE050ARD & OM- SE051ARD:
    -J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
    -J15 1-2: I2C target SDA connection
    -J17 1-2: I2C target SCL connection


For SE050
========
- Connect OM- SE050ARD to the board.
- Please consult SE050 user manual and Plug & Trust middleware documentation which is present in the
  Plug & Trust MW package for further details.
- The Plug & Trust MW package can be downloaded from
https://www.nxp.com/products/security-and-authentication/authentication/:SE050?tab=Design_Tools_Tab
- Please refer to the RST file in the doc folder for additional details about the example.
- For running ex_rsa example enable following options in the fsl_sss_ftr.h file:
  SSS_HAVE_APPLET_SE05X_C - 1
  SSS_HAVE_RSA - 1