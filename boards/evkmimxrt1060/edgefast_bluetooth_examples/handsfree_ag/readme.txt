Overview
========
This example demonstrates the HFP Ag basic functionality, currently support simulate an incoming call, and the call could be answered and ended.
The HFP Ag can connected a HFP HF device like headphone or device running HFP HF device.


Toolchain supported
===================
- MCUXpresso  11.3.0
- IAR embedded Workbench  8.50.9
- GCC ARM Embedded  9.3.1

Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1060 board
- Personal Computer
- One of the following WiFi modules:
  - AzureWave AW-AM457-uSD

Board settings
==============

Jumper settings for AzureWave AW-AM457-uSD Module:
  - J11 2-3: VIO_SD 3.3V (Voltage level of SDIO pins is 3.3V)
  - J2  1-2: 3.3V VIO_uSD (Power Supply from uSD connector)
  - J4  2-3: 3.3V VIO

The hardware should be reworked according to the hardware rework guide for evkmimxrt1060 and AW-AM457-uSD.
The pin connect for UART HCI as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
UART_TXD |  J10(pin 4)  |   J22(pin 1)   |    LPUART3_RXD     | GPIO_AD_B1_07
UART_RXD |  J10(pin 2)  |   J22(pin 2)   |    LPUART3_TXD     | GPIO_AD_B1_06
UART_RTS |  J10(pin 6)  |   J23(pin 3)   |    LPUART3_CTS     | GPIO_AD_B1_04
UART_CTS |  J10(pin 8)  |   J23(pin 4)   |    LPUART3_RTS     | GPIO_AD_B1_05
GND      |  J6(pin 7)   |   J25(pin 7)   |    GND             | GND
------------------------------------------------------------------------------------

The pin connect for PCM interface as the following table,
------------------------------------------------------------------------------------
PIN NAME | AW-AM457-USD |   I.MXRT1060   | PIN NAME OF RT1060 | GPIO NAME OF RT1060
------------------------------------------------------------------------------------
PCM_IN   |  J9(pin 1)   |   J22(pin 5)   |    SAI2_TXD        | GPIO_AD_B0_09
PCM_OUT  |  J9(pin 2)   |   TP11         |    SAI2_RXD        | GPIO_AD_B0_08   
PCM_SYNC |  J9(pin 3)   |   J21(pin 9)   |    SAI2_RX_SYNC    | GPIO_AD_B0_07
PCM_CLK  |  J9(pin 4)   |   J21(pin 7)   |    SAI2_RX_BCLK    | GPIO_AD_B0_06
GND      |  J9(pin 6)   |   J21(pin 20)  |    GND             | GND
------------------------------------------------------------------------------------
Note:
After downloaded binary into qspiflash and boot from qspiflash directly, 
please reset the board by pressing SW9 or power off and on the board to run the application.
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
The log below shows the output of the example in the terminal window. 
USB Host stack successfully initialized
Bluetooth initialized

SHELL build: Mar  1 2021
Copyright  2020  NXP

>>
the bellow commands have been supported:
"bt": BT related function
  USAGE: bt [discover|connect|disconnect|delete]
    discover    start to find BT devices
    connect     connect to the device that is found, for example: bt connect n (from 1)
    sincall     start an incoming call.
    aincall     accept the incoming call.
    eincall     end an in comingcall.
    disconnect  disconnect current connection.
    delete      delete all devices. Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.

Note：
There is a short noise can be heard at headset at the begin audio streaming when in running HFP Ag . 
The codec power on pop noise cannot eliminate.
