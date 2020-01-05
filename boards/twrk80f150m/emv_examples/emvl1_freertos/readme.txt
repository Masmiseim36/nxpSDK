Overview
========
The EMVL1 demo application demonstrates the interaction between the KSDK peripheral drivers and EMVL1 stack with
FreeRTOS. The demo illustrates how to use the EMVL1 stack with the following methods:
- CPU transfer using T0 / T1 protocols

The example is intended to demonstrate a Point of Sale (POS) terminal, working with payment (credit/debit) cards.

The example completes the transfer using both T0 and T1 modes and reads basic smart card information, like the card holder name,
PAN, and expiration date.

Note that this demo is just a simple demonstration of the usage of the EMVL1 stack and smart card driver. Not all types
of payment cards are supported, and not all cards support the T1 protocol. This demo supports just one generic type
of MasterCard, Visa, Visa Electron, Amex and Europay, UnionPay Credit, and UnionPay Debit AIDs. If your credit card
does not work, try another card.


Toolchain supported
===================
- IAR embedded Workbench  8.40.2
- Keil MDK  5.29
- GCC ARM Embedded  8.3.1
- MCUXpresso  11.1.0

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

1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
Running the demo
================
These instructions are displayed on the terminal window (Be aware that the T1 part in terminal output is
displayed only when the T1 protocol is supported by smart card):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
........................... EMVL1 FreeRTOS demo ..............................
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

***** EMVL1 FreeRTOS Demo End *****
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For the cards which use T1 protocol, but doesn't support class and instruction used in this demo the output is following:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
........................... EMVL1 FreeRTOS demo ..............................
Card inserted.
Deactivating card...Done!
Resetting/Activating card...Done!
Protocol T=1 test started!
Unsupported card inserted.
Deactivating card...Done!
***** EMVL1 FreeRTOS Demo End *****
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

