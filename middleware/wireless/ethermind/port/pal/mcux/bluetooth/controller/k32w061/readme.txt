Overview
========
This readme explains how to flash a ble transceiver image on a k32W061 board.

Hardware requirements
=====================
Transceiver part:
- 1 Mini USB cable
- 1 OM15076-3 Carrier Board (DK6 board)
- 1 K32W061 Module to be plugged on the Carrier Board

Host part:
- 1 evkmimxrt1060 or 1 evkbmimxrt1060 or 1 evkmimxrt1170 or 1 evkmimxrt685 

Board settings
==============
Pin setting for evkbmimxrt1060 and evkmimxrt1060:
------------------------------------------------------------------------------------
PIN NAME | DK6 (K32W061) |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
UART_TXD |  PIO(pin 8)   |   J22(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_RXD |  PIO(pin 9)   |   J22(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_RTS |  PIO(pin 6)   |   J23(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_CTS |  PIO(pin 7)   |   J23(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
------------------------------------------------------------------------------------
Pin setting for evkmimxrt1170:
------------------------------------------------------------------------------------
PIN NAME | DK6 (K32W061) |   I.MXRT1170    | PIN NAME OF RT1170 | GPIO NAME OF RT1170
------------------------------------------------------------------------------------
UART_TXD |  PIO(pin 8)   |   J25(pin 13)   |    LPUART7_RXD     | GPIO_AD_01
UART_RXD |  PIO(pin 9)   |   J25(pin 15)   |    LPUART7_TXD     | GPIO_AD_00
UART_RTS |  PIO(pin 6)   |   J25(pin 11)   |    LPUART7_CTS     | GPIO_AD_02
UART_CTS |  PIO(pin 7)   |   J25(pin 9)    |    LPUART7_RTS     | GPIO_AD_03
------------------------------------------------------------------------------------
Pin setting for evkmimxrt685:
------------------------------------------------------------------------------------
PIN NAME | DK6 (K32W061) |   I.MXRT685    | PIN NAME OF RT685 | GPIO NAME OF RT685
------------------------------------------------------------------------------------
UART_TXD |  PIO(pin 8)   |   J27(pin 1)   |    USART4_RXD     | FC4_RXD_SDA_MOSI_DATA
UART_RXD |  PIO(pin 9)   |   J27(pin 2)   |    USART4_TXD     | FC4_TXD_SCL_MISO_WS
UART_RTS |  PIO(pin 6)   |   J47(pin 9)   |    USART4_CTS     | FC4_CTS_SDA_SSEL0
UART_CTS |  PIO(pin 7)   |   J27(pin 5)   |    USART4_RTS     | FC4_RTS_SCL_SSEL1
------------------------------------------------------------------------------------

Flashing the transceiver image
================
To flash an image, DK6Programmer should be used and has to be install in your computer.
To install it, you can run "JN-SW-4407+DK6+Production+Flash+Programmer+v2282.exe".
After having successfully installed the programmer, the tool DK6Programmer.exe could be used to flash a binary.
A single single ble only transceiver image could be flashed or a dual mode image supporting ble and openthread as an RCP.

1. Flashing the ble transceiver binary:
DK6Programmer.exe -V2 -s <COM_PORT> -P 1000000 -Y -p FLASH@0x00="<sdk_path>\middleware\wireless\ethermind\port\pal\mcux\bluetooth\controller\k32w061\hci_black_box_bm.bin"

For more information on the Programmer tool you can check "DK6-RN-0065-Production-Flash-Programmer.pdf".

