Overview
========
Test application to be used with the auto-pts test framework. Contains GAP, GATT and L2CAP modules.

How to flush the board before testing with the bt_tester application
For the MIMXRT1016-EVK with AW-AM457-uSD module
    To be able to run multiple tests the bt_tester application is configured with CONTROLLER_INIT_ESCAPE 1, using this define will skip firmware download of the module image. (We must skip this step because after reset the MIMXRT1016-EVK is unable to download the firmware image in AW-AM457-uSD module)
    Steps to prepare the board to run multiple PTS tests:
        1) Download and run the wireless_uart application on the MIMXRT1016-EVK with AW-AM457-uSD (this step is done to download the AW-AM457-uSD module image)
        2) Without removing the power of the AW-AM457-uSD module write the bt_tester application. Press the reset button on MIMXRT1016-EVK board. Now the user can run multiple PTS tests.

Note: 
If the AW-AM457-uSD module loses power steps 1 and 2 must be redone.

For MIMXRT1170-EVK w and AW-CM358MA write the image on the board and press the reset button on MIMXRT1170-EVK board

Testing setup for Windows10
    - Download and install Python 3. Setup was tested with versions >=3.8.
    Let the installer add the Python installation directory to the PATH and disable the path length limitation
    - Download and install Git. During installation enable option: Enable experimental support for pseudo consoles.
    We will use Git Bash as Windows terminal.
    - Install latest PTS from https://www.bluetooth.org. Remember to install drivers from installation directory "C:/Program Files (x86)/Bluetooth SIG/Bluetooth PTS/PTS Driver/win64/CSRBlueCoreUSB.inf“
    - Download and extract socat.exe from: https://sourceforge.net/projects/unix-utils/files/socat/1.7.3.2/ (socat-1.7.3.2-1-x86_64.zip)
    Add path to directory of socat.exe to PATH
    - In Git Bash, clone project repo: git clone https://github.com/intel/auto-pts.git
        Go into the project folder: cd auto-pts
        Install required python modules:
        pip3 install --user wheel
        pip3 install --user -r autoptsserver_requirements.txt
        pip3 install --user -r autoptsclient_requirements.txt
    - Modification made to AutoPTS python setup to run with NXP boards: 
      In ptsprojects\boards add nxprtx.py (attached in this presentation) this is the file that implements board boards related functions.
      To reset the board between testes we added a reset command “0x00 0x06 0xFF 0x 00 0x00”.
      Modify Auto PTS to transmit this command whenever it wants to reset the board:
      In ptsprojects\zephyr\iutctl.py line 206 replace “self.board.reset()” with:
        reset_data = bytearray()
        self.btp_socket.send(0x00,0x06, 0xff, reset_data)
        time.sleep(1)
        

Toolchain supported
===================
- IAR embedded Workbench  9.30.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1170 board
- Personal Computer
- One of the following modules:
  - AzureWave AW-AM510-M.2
  - AzureWave AW-CM358-M.2
  - AzureWave AW-AM457-uSD
  - Murata uSD-M.2 Adapter (LBEE0ZZ1WE-uSD-M2) and Embedded Artists 1XK M.2 Module (EAR00385)
  - Embedded Artists 1XK M.2 Module (EAR00385) - direct M2 connection.
  - Murata uSD-M.2 Adapter (LBEE0ZZ1WE-uSD-M2) and Embedded Artists 1ZM M.2 Module (EAR00364)
  - Embedded Artists 1ZM M.2 Module (EAR00364) - direct M2 connection.

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want to use AzureWave WIFI_IW416_BOARD_AW_AM510MA, please change the macro to WIFI_IW416_BOARD_AW_AM510MA.
If you want to use AzureWave WIFI_88W8987_BOARD_AW_CM358MA, please change the macro to WIFI_88W8987_BOARD_AW_CM358MA.
If you want to use AzureWave WIFI_IW416_BOARD_AW_AM457_USD, please change the macro to WIFI_IW416_BOARD_AW_AM457_USD.
If you want to use Embedded Artists Type 1XK module with uSD-M2 adapter, please change the macro to WIFI_IW416_BOARD_MURATA_1XK_USD .
If you want to use Embedded Artists Type 1XK module(EAR00385 M2 only), please change the macro to WIFI_IW416_BOARD_MURATA_1XK_M2.
If you want to use Embedded Artists Type 1ZM module with uSD-M2 adapter, please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_USD.
If you want to use Embedded Artists Type 1ZM module(EAR00364 M2 only), please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_M2.

Jumper settings for RT1170 (enables external 5V supply):
remove  J38 5-6
connect J38 1-2
connect J43 with external power(controlled by SW5)

Murata Solution Board settings
Murata uSD-M.2 adapter resource page: https://www.murata.com/en-us/products/connectivitymodule/wi-fi-bluetooth/overview/lineup/usd-m2-adapter
Murata uSD-M.2 adapter datasheet: https://www.murata.com/-/media/webrenewal/products/connectivitymodule/asset/pub/rfm/data/usd-m2_revb1.ashx
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf

Jumper settings for Murata uSD-M.2 adapter:
  - Both J12 & J13 = 1-2: WLAN-SDIO = 1.8V; and BT-UART & WLAN/BT-CTRL = 3.3V
  - J1 = 2-3: 3.3V from uSD connector

The following pins between the evkmimxrt1170 board and Murata uSD-M.2 Adapter with Embedded Artists 1ZM M.2 Module or 1XK M.2 Module are connected using male-to-female jumper cables:

----------------------------------------------------------------------------------------------
PIN NAME         | uSD-M.2 Adapter |   I.MXRT1170    | PIN NAME OF RT1170 | GPIO NAME OF RT1170
----------------------------------------------------------------------------------------------
BT_UART_TXD_HOST |  J9(pin 1)  	   |   J25(pin 13)   |    LPUART7_RXD     | GPIO_AD_01
BT_UART_RXD_HOST |  J9(pin 2)  	   |   J25(pin 15)   |    LPUART7_TXD     | GPIO_AD_00
BT_UART_RTS_HOST |  J8(pin 3)  	   |   J25(pin 11)   |    LPUART7_CTS     | GPIO_AD_02
BT_UART_CTS_HOST |  J8(pin 4)  	   |   J25(pin 9)    |    LPUART7_RTS     | GPIO_AD_03
----------------------------------------------------------------------------------------------

NOTE: With direct M2 connection (EAR00364/EAR00385) no additional cabling is needed.

AzureWave Solution Board settings
Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVK and AW-AM457-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1170  | PIN NAME OF RT1170 | GPIO NAME OF RT1170
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J25(pin 13)   |    LPUART7_RXD     | GPIO_AD_01
UART_RXD |  J10(pin 2)  |   J25(pin 15)   |    LPUART7_TXD     | GPIO_AD_00
UART_CTS |  J10(pin 8)  |   J25(pin 9)    |    LPUART7_RTS     | GPIO_AD_03
UART_RTS |  J10(pin 6)  |   J25(pin 11)   |    LPUART7_CTS     | GPIO_AD_02
GND      |  J6(pin 7)   |   J26(pin 1)    |    GND             | GND
------------------------------------------------------------------------------------

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVK and AW-CM358MA in document Hardware Rework Guide for EdgeFast BT PAL.

The hardware should be reworked according to the Hardware Rework Guide for MIMXRT1170-EVK and AW-CM510MA in document Hardware Rework Guide for EdgeFast BT PAL.

Note:
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW4 or power off and on the board to run the application.
the SW7 is user button,please do role switch by pressing this button on the board.
Running the demo
================
- Running AutoPTS
    Server and client by default will run on localhost address.
    Run server: “python ./autoptsserver.py -S 65000”
    Run client: “python ./autoptsclient-zephyr.py zephyr-master ~/zephyrproject/build/zephyr/zephyr.elf -t COMx -b nxprtx -S 65000 -C 65001”




