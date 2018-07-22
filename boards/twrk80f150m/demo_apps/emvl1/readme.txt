Overview
========
The EMVL1 demo application demonstrates the interaction between the KSDK peripheral drivers and EMVL1 stack. The demo
illustrates how to use the EMVL1 stack with the following methods:
- CPU transfer using T0 / T1 protocols

The example is intended to demonstrate a Point of Sale (POS) terminal, working with payment (credit/debit) cards.

The example completes the transfer using both T0 and T1 modes and reads basic smart card information, like the card holder name, PAN,
and expiration date.

Note that this demo is just simple demonstration of the usage of the EMVL1 stack and smart card driver. Not all types
of payment cards are supported, and not all cards support the T1 protocol. This demo supports just one generic type
of MasterCard, Visa, Visa Electron, Amex and Europay, UnionPay Credit, and UnionPay Debit AIDs. If your credit card
does not work, try another card.


Toolchain supported
===================
- IAR embedded Workbench 8.11.3
- Keil MDK 5.23
- GCC ARM Embedded 6-2017-q2
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso10.1.0

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-K80F150M board
- TWR-POSCARD board (TDA8035)
- Payment smartcard
- Personal Computer

Board settings
==============
TWR-K80F150M board requires a 0 ohm resistor R215 to be removed (resistor R215 is required to communicate with the
on board(K80F150M) SIM interface).
TWR-POSCARD board requires jumpers to be set to positions as follows: J6(4-6), J7(4-6), J8(4-6), J9(empty), J10(empty), 
J11(1-2), J12(1-3;4-6).

Toolchains supported
- iar
- mdk
- kds
- armgcc
- atollic

Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start TeraTerm (http://ttssh2.osdn.jp) and make
a connection to the virtual serial port.

1. Start TeraTerm.

2. New connection -> Serial.

3. Set the appropriate COMx port (where x is the port number) in port context menu. The number is provided by the host operating system, and
 may be different from computer to computer. Select the COM number related to virtual serial port. Confirm the selected port by
clicking the "OK" button.

4. Set following connection parameters in the setup menu: Setup -> Serial port.
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none

5.  Confirm selected parameters by clicking the "OK" button.

Running the demo
================
These instructions are displayed on the terminal window (Be aware that the T1 part in terminal output is
displayed only when T1 protocol is supported by smart card):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
........................... EMVL1 demo ..............................
Card inserted.
Deactivating card...Done!
Resetting/Activating card...Done!
Protocol T=0 test started!
Reading card records to get required data.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Card info:
          Primary account number: 00 00 00 00 00 00 00 00
          Application expiration date: 01/01/17
          Cardholder name: SURNAME/FIRST NAME
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Resetting/Activating card...Done!
Protocol T=1 test started!
Echo data matched!
TEST PASSED.
Returned Status Bytes = 0x90, 0x 0
Deactivating card...Done!

***** EMVL1 Demo End *****
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For the cards which use T1 protocol, but doesn't support class and instruction used in this demo the output is following:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
........................... EMVL1 demo ..............................
Card inserted.
Deactivating card...Done!
Resetting/Activating card...Done!
Protocol T=1 test started!
Unsupported card inserted.
Deactivating card...Done!
***** EMVL1 Demo End *****
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

