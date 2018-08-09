Prerequisites
=============

First write the bootloader_otap binary to the OTA clients, and then write the
Thread ota_client application.


Standalone Mode
===============

In the standalone mode, the OTA image is transferred in its entirety to the OTA
server, which stores it and then forwards chunks to the clients.

Supported platforms for the Server:
    USB-KW41Z/KW24D - cannot be used, there is not enough space to store the
                      OTA image (no external memory)
    FRDM-KW41Z      - supported with project hcd_ota_server
    FRDM-KW24D      - supported with project hcd_ota_server
    FRDM-K64F       - supported with project hcd_ota_server

Dongle Mode
===========

In the dongle mode, the OTA server acts as a proxy between the clients and the
PC. Clients in the network ask for chunks and the PC delivers them one by one.

Both the USB and FRDM form-factors support the Dongle Mode.

Usage
=====

One needs to first obtain a ota_client .thread file from a .srec one. Steps to
perform:
1. Open Test Tool and got to tab OTA Updates -> OTAP Thread
2. Browse to the desired ota_client .srec
3. Press Save.. Choose whether the image embeds a bootloader or NVM sections
   should be added to it. Choose a name like end_device_ota_client_<board> or
   router_eligible_device_<board> to distinguish images easily.

Call the script
    $ cd hsdk-python/src/com/nxp/wireless_connectivity/test/ota/
    $ sudo python test_dongle_mode.py /dev/ttyACM0 end_device_ota_client.thread

When prompted with the message:
    "Press ENTER after joining all devices to the network...""

one should join the OTA clients to the network. Pressing ENTER will start the
transfer of the new firmware from PC to clients.

You may want to change the color of the blinking LED on FRDM-KW41Z/KW24D to check
whether the transfer has been successful. Do this by changing in app_led.c from:
    const appLedsConfig_t gLedCfgFactoryDefault = {LedCfg_BlueFlashing(LED_RGB, gLedSetup_Rgb_c, LED_MAX_RGB_VALUE_c), LedCfg_Flashing(LED1)};
to
    const appLedsConfig_t gLedCfgFactoryDefault = {LedCfg_RedFlashing(LED_RGB, gLedSetup_Rgb_c, LED_MAX_RGB_VALUE_c), LedCfg_Flashing(LED1)};

The recommendation is to flash the FRDMs with a .srec which has the blinking LED
red, and start the script with the blue LED .thread file as argument
(or vice-versa), in order to check if the new firmware has been successfully
transferred. A reset is needed on the OTA client in order to boot the
new firmware.

Notes:
- situations where the script appears to have blocked appear as a consequence of
  the OTA update scheduling algorithm. After utmost 20 seconds, the OTA clients
  should resume requesting image blocks.
- the OTA transfer usually takes ~7 minutes with two OTA clients in dongle mode
