Supported Wi-Fi/BT/BLE modules
==============================
  - AzureWave AW-AM510MA
  - AzureWave AW-CM358MA
  - Embedded Artists 1ZM M.2 Module (EAR00364)
  - Embedded Artists 1XK M.2 Module (EAR00385)
  - Embedded Artists 2EL M.2 Module (EAR00409)
  - Embedded Artists 2LL M.2 Module (EAR00500)


Murata Solution Board settings
==============================
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf
Embedded Artists 2LL module datasheet: https://www.embeddedartists.com/wp-content/uploads/2024/12/2LL_M2_Datasheet.pdf


Board settings
==============
M.2 connector:
  - J76: 2-3
  - J109: 1-2

  - 2LL:
      - Connect J112

Wi-Fi Host Sleep Wakeup GPIO For 1XK/1ZM/2EL/2LL:
  - add 0Ohm resistor at position R252
  - 1XK:
      - Connect Fly-Wire between J33.1 and J108.5.
      - J108 is routed on M2.P44 which internally routed on GPIO[2] of Controller 1XK.
  - 1ZM:
      - Connect Fly-Wire between J33.1 and J108.2.
      - J108 is routed on M2.P40 which internally routed on GPIO[13] of Controller 1ZM.
  - 2EL:
      - No fly-wire connection required.
Wi-Fi Independent Reset OOB Trigger For 1XK/1ZM/2EL:
  - Connect Fly-Wire between J16.1 and J108.4.
  - J108 is routed on M2.P48 which internally routed on IR GPIO[15] of Controller 1XK/1ZM.
  - For 2EL-M2, No fly-wire connection required.

For 1XK/1ZM/2EL:
Connect external power supply to J45, set J40: 1-2 and turn on SW6.
