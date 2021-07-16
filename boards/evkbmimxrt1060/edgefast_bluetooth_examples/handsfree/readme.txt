Overview
========
This example demonstrates the HFP HF basic functionality, the HFP device support be connected to a HFP AG like a mobile phone or a 
board running a HFP AG application. And the HF example support accept/reject/End the incoming call from HFP AG. 



Toolchain supported
===================
- MCUXpresso  11.4.0
- IAR embedded Workbench  9.10.2
- GCC ARM Embedded  10.2.1

Hardware requirements
=====================
- Micro USB cable
- evkbmimxrt1060 board
- Personal Computer
- One of the following WiFi modules:
  - AzureWave AW-AM457-uSD
  - AzureWave AW-CM358-uSD

Board settings
==============

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J42 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO

The hardware should be reworked according to the hardware rework guide for evkmimxrt1060 and AW-AM457-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J16(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_RXD |  J10(pin 2)  |   J16(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_RTS |  J10(pin 6)  |   J33(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_CTS |  J10(pin 8)  |   J33(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
GND      |  J6(pin 7)   |   J32(pin 7)   |    GND             | GND
------------------------------------------------------------------------------------

The pin connect for PCM interface as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
PCM_IN   |  J9(pin 1)   |   J16(pin 5)   |    SAI2_TXD        | GPIO_AD_B0_09
PCM_OUT  |  J9(pin 2)   |   TP11         |    SAI2_RXD        | GPIO_AD_B0_08   
PCM_SYNC |  J9(pin 3)   |   J21(pin 9)   |    SAI2_RX_SYNC    | GPIO_AD_B0_07
PCM_CLK  |  J9(pin 4)   |   J21(pin 7)   |    SAI2_RX_BCLK    | GPIO_AD_B0_06
GND      |  J9(pin 6)   |   J21(pin 20)  |    GND             | GND
------------------------------------------------------------------------------------

Jumper settings for AzureWave AW-CM358-uSD Module:
  - J2 1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4 2-3: VIO 3.3V (Voltage level of SDIO pins is 3.3V)

The hardware should be reworked according to the hardware rework guide for evkmimxrt1060 and AW-CM358-uSD in document Hardware Rework Guide for EdgeFast BT PAL.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-CM358-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J16(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_RXD |  J10(pin 2)  |   J16(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_RTS |  J10(pin 6)  |   J33(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_CTS |  J10(pin 8)  |   J33(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
GND      |  J6(pin 7)   |   J32(pin 7)   |    GND             | GND
------------------------------------------------------------------------------------

The pin connect for PCM interface as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-CM358-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
PCM_IN   |  J42(pin 1)  |   J16(pin 5)   |    SAI2_TXD        | GPIO_AD_B0_09
PCM_OUT  |  J42(pin 2)  |   TP11         |    SAI2_RXD        | GPIO_AD_B0_08   
PCM_SYNC |  J42(pin 3)  |   J21(pin 9)   |    SAI2_RX_SYNC    | GPIO_AD_B0_07
PCM_CLK  |  J42(pin 4)  |   J21(pin 7)   |    SAI2_RX_BCLK    | GPIO_AD_B0_06
GND      |  J42(pin 6)  |   J21(pin 20)  |    GND             | GND
------------------------------------------------------------------------------------

Note:
After downloaded binary into qspiflash and boot from qspiflash directly, 
please reset the board by pressing SW7 or power off and on the board to run the application.
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
USB Host stack successfully initialized
Bluetooth initialized
BR/EDR set connectable and discoverable done
Please use "Y" to accept the call,  use "N" to reject or end the call
Now Start SDP Service and the Service is now discoverable by remote device
- Can be connected by HFP AG device like a mobile phone or another device running HFP AG. 
- Can be a Outcoming a call or incoming a call in peer HFP AG device.
- In incoming call, use "Y" to accept the call or use "N" to reject the call
- In an active call, use "N" to end the call
- Just support adjust the volume on HFP AG side

Note:
There is a short noise can be heard at headset at the begin audio streaming when in running HFP Unit and HFP ring tone   
and at the end of each ring tone segment. The codec power on pop noise cannot eliminate.

