Overview
========
This is the three-phase permanent magnet synchronous (PMSM) safety motor control
reference design for Kinetis® MCUs. The reference design features a sensorless 
closed-loop field-oriented control (FOC) algorithm with the IEC60730 class B 
safety.

Running the demo
================
Follow the "MCUXSDK3PPMSMCWSUG.pdf" User's guide chapters "Hardware setup" and 
"User interface".

Safety FLASH test
=================
The safety FLASH test is disabled by default. To enable it, please refer to the 
User's guide document "MCUXSDK3PPMSMCWSUG.pdf" in the "docs\MC" folder of SDK 
Documentation package.

Missing MCUXpresso Config Tools support
=======================================
This example cannot be imported into Config Tools because of safety design 
aspects coming from IEC60730 norm (need for RO, RW, and CODE memory separation, 
the necessity of software flow checking, etc.). This effectively prevents the 
use of SDK drivers for safety-related peripheral control and initialization as 
well.

More information
================
- Release note is in the SDK Documentation folder "docs\MC\Release_notes.pdf"
- Application User's guide is in the SDK Documentation package folder 
  "docs\MC\MCUXSDK3PPMSMCWSUG.pdf".
- Application web page is available at "http://www.nxp.com/motorcontrol_pmsm".