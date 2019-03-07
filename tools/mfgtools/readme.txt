
   Manufacture tool patch User Guide
  
   ======================================
   
   This patch is based on IMX6_L4.1.15_2.0.0_MFG_TOOL.

   1. Download IMX6_L4.1.15_2.0.0_MFG_TOOL from nxp.com
   2. Extract IMX6_L4.1.15_2.0.0_MFG_TOOL and override the contents in mfgtools folder with this patch.
   3. Copy sdk20-app.img file made with imgutil to Profiles\Linux\OS Firmware\files folder
   4. Set the boot pin to Serial Downloader mode, connect a USB cable from a computer to the USB OTG port on the board.


   Boot from QSPI:
   ---------------
   5. Power on the board. Run mfgtool2-sdk20-mx6ul-evk-qspi-nor-n25q256a.vbs to write sdk20-app.img to QSPI flash.
   6. Switch boot mode to Internal Boot and set boot devices to QSPI flash, power on the board then the SDK2.0 application is running.
   Tips: To erase QSPI flash without writing a new sdk20-app.img, please run mfgtool2-erease-mx6ul-evk-qspi-nor-n25q256a.vbs under Serial Downloader boot mode.


   Boot from MicroSD:
   ------------------
   5. Insert MicroSD card and power on the board. Run mfgtool2-sdk20-mx6ul-evk-sdcard.vbs to write sdk20-app.img to MicroSD card.
   6. Switch boot mode to Internal Boot and set boot devices to MicroSD, power on the board then the SDK2.0 application is running.
   Tips: To erase MicroSD without writing a new sdk20-app.img, please run mfgtool2-erase-sdk20-mx6ul-evk-sdcard.vbs under Serial Downloader boot mode.

   For boot mode setting, please refer to i.MX 6UL/6ULL Evaluation Kit Quick Start Guide.

   ======================================
