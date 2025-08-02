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
  - remove R183
  - add 0Ohm resistor at position R404

Wi-Fi Host Sleep Wakeup GPIO For 2LL:
  - 2LL:
      - Connect J104

For 1XK/1ZM/2EL:
Connect external power supply to J43, set J38: 1-2 and turn on SW5.
