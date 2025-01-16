Hardware requirements
=====================
- Micro USB cable
- evkcmimxrt1060 board
- Personal Computer
- One of the following WiFi modules:
- One of the following modules:
  - Embedded Artists 1XK M.2 Module (EAR00385) - direct M2 connection.
  - Embedded Artists 1ZM M.2 Module (EAR00364) - direct M2 connection.
  - Embedded Artists 2EL M.2 Module - direct M2 connection.
  - Embedded Artists 2DS M.2 Module (EAR00386) - direct M2 connection.

Board settings
==============

Jumper settings for RT1060-EVKC (enables external 5V supply):
========================================================

- remove  J40 5-6
- connect J40 1-2
- connect J45 with external power(controlled by SW6)

Murata Solution Board settings
==============================

- Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
- Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf
- Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf
- Embedded Artists 2DS module datasheet: https://www.embeddedartists.com/doc/ds/2DS_M2_Datasheet.pdf

RT1060-EVKC Board Rework For M2 Slot Enablement
===============================================

- Wi-Fi Rework:
	- Jumper Settings : Connect J109, connect J76 2-3
    - For 2DS M.2 Module: remove R2163
- Wi-Fi Host Sleep Wakeup GPIO For 1XK/1ZM/2EL:
    - add 0Ohm resistor at position R252
    - 1XK:
        - Connect Fly-Wire between J33.1 and J108.5.
        - J108 is routed on M2.P44 which internally routed on GPIO[2] of Controller 1XK.
    - 1ZM:
        - Connect Fly-Wire between J33.1 and J108.2.
        - J108 is routed on M2.P40 which internally routed on GPIO[13] of Controller 1ZM.
    - 2EL:
        - No fly-wire connection required.
- Wi-Fi Independent Reset OOB Trigger For 1XK/1ZM/2EL:
	- Connect Fly-Wire between J16.1 and J108.4.
	- J108 is routed on M2.P48 which internally routed on IR GPIO[15] of Controller 1XK/1ZM.
	- For 2EL-M2, No fly-wire connection required.
