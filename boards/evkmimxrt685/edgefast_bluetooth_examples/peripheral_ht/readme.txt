Overview
========
Application demonstrating the BLE Peripheral role, except that this application specifically exposes the HT (Health Thermometer) GATT Service. Once a device connects it will generate dummy temperature values.


Toolchain supported
===================
- MCUXpresso  11.4.0
- IAR embedded Workbench  9.10.2
- Keil MDK  5.34
- GCC ARM Embedded  10.2.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt685 board
- Personal Computer
- One of the following modules:
  - AzureWave AW-AM457-uSD
  - AzureWave AW-CM358-uSD
  - K32W061

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want use the AzureWave WIFI_IW416_BOARD_AW_AM457_USD, please change the macro to WIFI_IW416_BOARD_AW_AM457_USD.
If you want use the AzureWave WIFI_88W8987_BOARD_AW_CM358_USD-M.2, please change the macro to WIFI_88W8987_BOARD_AW_CM358_USD.
If you want use the K32W061_TRANSCEIVER, please change the macro to K32W061_TRANSCEIVER.

Jumper settings for RT685:
connect J6 with external power

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO
Jumper settings for evkmimxrt685 Module:
  - JP12 2-3: 3.3V VDDIO

The hardware should be reworked according to the hardware rework guide for evkmimxrt685 and AW-AM457-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT685   | PIN NAME OF RT685 | GPIO NAME OF RT685
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J27(pin 1)   |    USART4_RXD     | FC4_RXD_SDA_MOSI_DATA
UART_RXD |  J10(pin 2)  |   J27(pin 2)   |    USART4_TXD     | FC4_TXD_SCL_MISO_WS
UART_RTS |  J10(pin 6)  |   J47(pin 9)   |    USART4_CTS     | FC4_CTS_SDA_SSEL0
UART_CTS |  J10(pin 8)  |   J27(pin 5)   |    USART4_RTS     | FC4_RTS_SCL_SSEL1
GND      |  J6(pin 7)   |   J29(pin 6)   |    GND            | GND
------------------------------------------------------------------------------------

Jumper settings for AzureWave AW-CM358-uSD Module:
  - J2 1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4 1-2: VIO 1.8V (Voltage level of SDIO pins is 1.8V)

The hardware should be reworked according to the hardware rework guide for evkmimxrt685 and AW-CM358-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-CM358-USD |   I.MXRT685   | PIN NAME OF RT685 | GPIO NAME OF RT685
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J27(pin 1)   |    USART4_RXD     | FC4_RXD_SDA_MOSI_DATA
UART_RXD |  J10(pin 2)  |   J27(pin 2)   |    USART4_TXD     | FC4_TXD_SCL_MISO_WS
UART_RTS |  J10(pin 6)  |   J47(pin 9)   |    USART4_CTS     | FC4_CTS_SDA_SSEL0
UART_CTS |  J10(pin 8)  |   J27(pin 5)   |    USART4_RTS     | FC4_RTS_SCL_SSEL1
GND      |  J6(pin 7)   |   J29(pin 6)   |    GND            | GND
------------------------------------------------------------------------------------

For K32W061, the readme located in <sdk>/middleware/wireless/ethermind/port/pal/mcux/bluetooth/controller/k32w061 explains how to flash the transceiver image and the hardware rework required.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | DK6 (K32W061) |   I.MXRT685   | PIN NAME OF RT685 | GPIO NAME OF RT685
------------------------------------------------------------------------------------
UART_TXD |  PIO(pin 8)   |   J27(pin 1)   |    USART4_RXD     | FC4_RXD_SDA_MOSI_DATA
UART_RXD |  PIO(pin 9)   |   J27(pin 2)   |    USART4_TXD     | FC4_TXD_SCL_MISO_WS
UART_RTS |  PIO(pin 6)   |   J47(pin 9)   |    USART4_CTS     | FC4_CTS_SDA_SSEL0
UART_CTS |  PIO(pin 7)   |   J27(pin 5)   |    USART4_RTS     | FC4_RTS_SCL_SSEL1
------------------------------------------------------------------------------------

Note:
After downloaded binary into qspiflash and boot from qspiflash directly, 
please reset the board by pressing SW3 or power off and on the board to run the application.
Prepare the Demo
================

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

4.  Download the program to the target board.

5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The demo does not require user interaction. The application will automatically start advertising the Health Thermometer Service and it will accept the first connection request it receives. If the peer subscribes to receive temperature indications, these will be sent every 1 second. The temperature readings are simulated with values between 20 and 25 degrees Celsius.
