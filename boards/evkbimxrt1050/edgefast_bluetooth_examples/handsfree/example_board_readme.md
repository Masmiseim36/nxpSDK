Hardware requirements
=====================
- Micro USB cable
- EVKB-IMXRT1050 board
- Personal Computer
- One of the following modules:
  - AzureWave AW-AM510-uSD
  - AzureWave AW-AM457-uSD
  - AzureWave AW-CM358-uSD
  - Murata uSD-M.2 Adapter (LBEE0ZZ1WE-uSD-M2) and Embedded Artists 1ZM M.2 Module (EAR00364)
  - Murata uSD-M.2 Adapter (LBEE0ZZ1WE-uSD-M2) and Embedded Artists 1XK M.2 Module (EAR00385)
  - Murata uSD-M.2 Adapter (LBEE0ZZ1WE-uSD-M2) and Embedded Artists 2EL M.2 Module (EAR00409)

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_bluetooth_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want to use the AzureWave WIFI_IW416_BOARD_AW_AM510_USD, please change the macro to WIFI_IW416_BOARD_AW_AM510_USD.
If you want to use the AzureWave WIFI_IW416_BOARD_AW_AM457_USD, please change the macro to WIFI_IW416_BOARD_AW_AM457_USD.
If you want to use the AzureWave WIFI_88W8987_BOARD_AW_CM358_USD, please change the macro to WIFI_88W8987_BOARD_AW_CM358_USD.
If you want to use the Murata Type 1ZM module, please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_USD.
If you want to use the Murata Type 1XK module, please change the macro to WIFI_IW416_BOARD_MURATA_1XK_USD.
If you want to use the Murata Type 2EL module, please change the macro to WIFI_IW612_BOARD_MURATA_2EL_USD.

Jumper settings for Murata uSD-M.2 adapter:
  - J12 = 1-2: WLAN-SDIO = 1.8V
  - J13 = 1-2: BT-UART & WLAN/BT-CTRL = 3.3V
  - J1 = 2-3: 3.3V from uSD connector

The following pins between the evkbmimxrt1050 board and Murata uSD-M.2 Adapter with Embedded Artists 1ZM M.2 Module or 1XK M.2 Module are connected using male-to-female jumper cables:
------------------------------------------------------------------------------------------
PIN NAME         | uSD-M.2 Adapter | I.MXRT1050 | PIN NAME OF RT1050 | GPIO NAME OF RT1050
------------------------------------------------------------------------------------------
BT_UART_TXD_HOST | J9(pin 1)       | J22(pin 1) | LPUART3_RXD        | GPIO_AD_B1_07
BT_UART_RXD_HOST | J9(pin 2)       | J22(pin 2) | LPUART3_TXD        | GPIO_AD_B1_06
BT_UART_RTS_HOST | J8(pin 3)       | J23(pin 3) | LPUART3_CTS        | GPIO_AD_B1_04
BT_UART_CTS_HOST | J8(pin 4)       | J23(pin 4) | LPUART3_RTS        | GPIO_AD_B1_05
------------------------------------------------------------------------------------------
The pin connect for PCM interface as the following table,
-------------------------------------------------------------------------------------------------
PIN NAME | uSD-M.2 Adapter | I.MXRT1050      | PIN NAME OF RT1050-EVKB | GPIO NAME OF RT1050-EVKB
-------------------------------------------------------------------------------------------------
PCM_IN   | J5(pin 1)       | J22(pin 5) 	   | SAI2_TXD      	         | GPIO_AD_B0_09
PCM_OUT  | J5(pin 3)       | TP11            | SAI2_RXD                | GPIO_AD_B0_08
PCM_SYNC | J5(pin 5)       | J21(pin 9)  	   | SAI2_RX_SYNC            | GPIO_AD_B0_07
PCM_CLK  | J5(pin 7)       | J21(pin 7)      | SAI2_RX_BCLK            | GPIO_AD_B0_06
GND      | J5(pin 15)      | J21(pin 20)     | GND                     | GND
-------------------------------------------------------------------------------------------------
Jumper settings for AzureWave AW-AM510-uSD Module:
  - J2 1-2: 3.3V VIO_uSD (Power supply from uSD connector)
  - J4 2-3: 3.3V VIO

The hardware should be reworked according to the Hardware Rework Guide for EVKB-IMXRT1050 and AW-AM510-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
-------------------------------------------------------------------------------
PIN NAME | AW-AM510-USD | I.MXRT1050 | PIN NAME OF RT1050 | GPIO NAME OF RT1050
-------------------------------------------------------------------------------
UART_TXD | J10(pin 4)   | J22(pin 1) | LPUART3_RXD        | GPIO_AD_B1_07
UART_RXD | J10(pin 2)   | J22(pin 2) | LPUART3_TXD        | GPIO_AD_B1_06
UART_RTS | J10(pin 6)   | J23(pin 3) | LPUART3_CTS        | GPIO_AD_B1_04
UART_CTS | J10(pin 8)   | J23(pin 4) | LPUART3_RTS        | GPIO_AD_B1_05
GND      | J6(pin 7)    | J25(pin 7) | GND                | GND
-------------------------------------------------------------------------------
The pin connect for PCM interface as the following table,
-------------------------------------------------------------------------------------------------
PIN NAME | AW-AM510-USD | I.MXRT1060-EVKB    | PIN NAME OF RT1050-EVKB | GPIO NAME OF RT1050-EVKB
-------------------------------------------------------------------------------------------------
PCM_IN   | J11(pin 1)       | J22(pin 5) 	   | SAI2_TXD      	         | GPIO_AD_B0_09
PCM_OUT  | J11(pin 2)       | TP11           | SAI2_RXD                | GPIO_AD_B0_08
PCM_SYNC | J11(pin 3)       | J21(pin 9)  	 | SAI2_RX_SYNC            | GPIO_AD_B0_07
PCM_CLK  | J11(pin 4)       | J21(pin 7)     | SAI2_RX_BCLK            | GPIO_AD_B0_06
GND      | J11(pin 6)       | J21(pin 20)    | GND                     | GND
-------------------------------------------------------------------------------------------------
Jumper settings for AzureWave AW-AM358-uSD Module:
  - J2 1-2: 3.3V VIO_uSD (Power supply from uSD connector)
  - J4 1-2: VIO 1.8V (Voltage level of SDIO pins is 1.8V)

The hardware should be reworked according to the Hardware Rework Guide for EVKB-IMXRT1050 and AW-CM358-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
-------------------------------------------------------------------------------
PIN NAME | AW-CM358-USD | I.MXRT1050 | PIN NAME OF RT1050 | GPIO NAME OF RT1050
-------------------------------------------------------------------------------
UART_TXD | J10(pin 4)   | J22(pin 1) | LPUART3_RXD        | GPIO_AD_B1_07
UART_RXD | J10(pin 2)   | J22(pin 2) | LPUART3_TXD        | GPIO_AD_B1_06
UART_RTS | J10(pin 6)   | J23(pin 3) | LPUART3_CTS        | GPIO_AD_B1_04
UART_CTS | J10(pin 8)   | J23(pin 4) | LPUART3_RTS        | GPIO_AD_B1_05
GND      | J6(pin 7)    | J25(pin 7) | GND                | GND
-------------------------------------------------------------------------------
The pin connect for PCM interface as the following table,
-------------------------------------------------------------------------------------------------
PIN NAME | AW-CM358-USD    | I.MXRT1060-EVKB | PIN NAME OF RT1050-EVKB | GPIO NAME OF RT1050-EVKB
-------------------------------------------------------------------------------------------------
PCM_IN   | J11(pin 1)       | J22(pin 5) 	   | SAI2_TXD      	         | GPIO_AD_B0_09
PCM_OUT  | J11(pin 2)       | TP11           | SAI2_RXD                | GPIO_AD_B0_08
PCM_SYNC | J11(pin 3)       | J21(pin 9)  	 | SAI2_RX_SYNC            | GPIO_AD_B0_07
PCM_CLK  | J11(pin 4)       | J21(pin 7)     | SAI2_RX_BCLK            | GPIO_AD_B0_06
GND      | J11(pin 5)       | J21(pin 20)    | GND                     | GND
-------------------------------------------------------------------------------------------------
Note:
To ensure that the LITTLEFS flash region has been cleaned,
all flash sectors need to be erased before downloading example code.
To support HFP feature, you must remove R316 and R323 on I.MXRT1050-EVKB.
To run HFP feature, you must remove J32 and J33 jumper.
There is a short noise can be heard at headset at the begin audio streaming when in running HFP Unit and HFP ring tone   
and at the end of each ring tone segment. The codec power on pop noise cannot eliminate.
