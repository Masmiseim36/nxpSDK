            ______________________________________________________

                NXPNFCRDLIB EXAMPLE : NFCRDLIBEX7_EMVCO_POLLING
                                  (V06.01.00)
            ______________________________________________________


Table of Contents
_________________

1 Document Purpose
2 Description of the NfcrdlibEx7_EMVCo_Polling
3 Restrictions on NfcrdlibEx7_EMVCo_Polling
4 Configurations of NfcrdlibEx7_EMVCo_Polling
5 Package Contents
6 Mandatory materials (not included)
7 Hardware Configuration
8 Software Configuration
9 Steps to build NfcrdlibEx7_EMVCo_Polling for LPC1769 with PN5180 using MCUXpresso
10 Steps to build NfcrdlibEx7_EMVCo_Polling for LPC1769 with RC663 using MCUXpresso
11 Steps to build NfcrdlibEx7_EMVCo_Polling for PN7462AU using MCUXpresso
12 Steps to build NfcrdlibEx7_EMVCo_Polling for FRDM-K82F using MCUXpresso
13 Selection of Board / OSAL / Frontend (MCUXpresso)
14 Steps to follow for PI / Linux / CMake
15 Running NfcrdlibEx7_EMVCo_Polling
16 List of supported NFC Reader Boards/ICs
17 Reference Documents





1 Document Purpose
==================

  This document describes the steps to be followed to execute
  NfcrdlibEx7_EMVCo_Polling example as well as it's known problems and
  restrictions.


2 Description of the NfcrdlibEx7_EMVCo_Polling
==============================================

  - NfcrdlibEx7_EMVCo_Polling can be run with CLEV663B, PN5180 and PN7462AU.
    (See Section-16 for supported versions)
  - This example performs the EMVCo Polling loop.
  - Upon activation of the detected card, the example shall send a Select PPSE
    command.
  - It displays/prints the response from the card for the select PPSE command.
  - This example is also a Loopback Application which is used to perform
    EMVCo2.5(L1) digital compliance validation which can be defining the
    #define RUN_TEST_SUIT.


3 Restrictions on NfcrdlibEx7_EMVCo_Polling
===========================================

  - NfcrdlibEx7_EMVCo_Polling is restricted to be run on NFC Reader Boards/ICs
    mentioned in Section-16.
  - The APIs are intended for NXP contact-less reader ICs only, as such the
    APIs are not to be ported to any technology from any other vendor.
  - NXP will not support porting to any other vendor platform.
  - This software project requires NxpNfcRdLib v06.01.00 or later.
  - **IMPORTANT** The default used MCU Type is LPC1769.  When executing example
    on FRDMK82 or PN7462AU, the MCU Type has to be changed in the MCUXpresso
    IDE.
  - For switching to MCU Type to PN7462AU, see Section-11
  - For switching to MCU Type to FRDMK82, see Section-12


4 Configurations of NfcrdlibEx7_EMVCo_Polling
=============================================

  - The example application is written to perform a polling loop and upon
    activation of the detected card, the application sends as PPSE select
    command.
  - The response data within a limit of 256 bytes shall be displayed as a part
    of this example.
  - This example supports different P2 parameter values [Which indicate
    returning of FCI, FCP, FMD as per section 5.3.3 of the ISO 7816-4
    standard]:

    - 000000xxb
    - 000001xxb
    - 000010xxb
    - 000011xxb

  - The example also perform EMVCo2.5(L1) digital compliance validation.
  - The following #defines can be used/configured to enable the different P2
    parameter values:

    - #define PPSE_FCI: P2:000000xxb (PPSE select response is a FCI)
    - #define PPSE_FMD: P2:000001xxb (PPSE select response is a FCP)
    - #define PPSE_FCP: P2:000010xxb (PPSE select response is a FMD)
    - #define PPSE_NO_LE: P2:000011xxb (No data recieved in the PPSE response
       )


5 Package Contents
==================

  - Readme.txt
    + This readme file
  - NfcrdlibEx7_EMVCo_Polling.c
    + Main example file.
  - intfs/NfcrdlibEx7_EMVCo_Polling.h
    + Interfaces/Defines specific to the example
  - intfs/ph_NxpBuild_App.h
    + Reader library build configuration file
  - src/phApp_Init.c and intfs/phApp_Init.h
    + Common utility functions (common across all examples)
  - src/phExMfDfCrypto.c and intfs/phExMfDfCrypto.h
    + MIFARE DESFire utility APIs
  - mcux/.cproject and mcux/.project
    + MCUXpresso project configuration file
  - cr_startup_lpc175x_6x.c
    + Startup required to compile example for LPC1769 Micro Controller.


6 Mandatory materials (not included)
====================================

  - MCUXpresso IDE. It can be downloaded from
    [http://www.nxp.com/products/:MCUXpresso-IDE]
  - SDK for Freedom K82 Board ([http://www.nxp.com/products/:FRDM-K82F]).  The
    pre-build SDK can be downloaded from
    [https://mcuxpresso.nxp.com/en/license?hash=9897a8c19a6bc569c3fade7141f0f405&hash_download=true&to_vault=true]
    See MCUXpresso User Manual for steps needed to install an SDK.
  - Plugin to extend MCUXpresso to support PN7462AU. (Required for MCUXpress
    versions before MCUXpressoIDE_10.0.2)

  - LPCXpresso LPC1769 / Freedom K82 development boards For NFC ICs listed in
    Section-16 (Note: PN7462AU (Section-16 Bullet-3) does not need any other
    other microcontroller).


7 Hardware Configuration
========================

  Before starting this application, HW Changes may be required for the used
  board.  Refer to the following User Manuals / Application notes before
  starting with this example.

  - AN11211: Quick Start Up Guide RC663 Blueboard
  - AN11744: PN5180 Evaluation board quick start guide
  - AN11802: NFC Reader Library for Linux Installation Guidelines


8 Software Configuration
========================

  - The Software can be compiled for Cortex M3 LPC1769 micro-controller, Cortex
    M4 Freedom K82 and Cortex M0 based PN7462AU from NXP.
  - Since this example can be configured to run on various MCU ICs and various
    NFC ICs, appropriate changes are required as mentioned in Section-9,
    Section-10, Section-11 and Section-12.


9 Steps to build NfcrdlibEx7_EMVCo_Polling for LPC1769 with PN5180 using MCUXpresso
===================================================================================

  See "Importing provided SW example projects" in AN11908 (See Section-17,
  Bullet-2 below)


10 Steps to build NfcrdlibEx7_EMVCo_Polling for LPC1769 with RC663 using MCUXpresso
===================================================================================

  See "Importing provided SW example projects" in AN11022 (See Section-17,
  Bullet-3 below)


11 Steps to build NfcrdlibEx7_EMVCo_Polling for PN7462AU using MCUXpresso
=========================================================================

  - For MCUXpressio versions before MCUXpressoIDE_10.0.2, See "Adding PN7462AU
    Plugin" in UM10883. (See Section-17, Bullet-4 below)
  - See "Importing provided SW example projects" in UM10883.


12 Steps to build NfcrdlibEx7_EMVCo_Polling for FRDM-K82F using MCUXpresso
==========================================================================

  See "Import projects to the MCUXpresso" in AN11908. (See Section-17, Bullet-2
  below)


13 Selection of Board / OSAL / Frontend (MCUXpresso)
====================================================

  For MCUXpresso, the selection of Board / OSAL / Frontend has to be done via
  -D (Preprocessor defines).

  1) To select the board go to "Project Properties" --> "C/C++ Build" -->
     "Settings" --> "Preprocessor" --> "Defined symbols (-D)", and define the
     relevant PHDRIVER_<BoardNFCCombination>_BOARD macro.

     e.g. For using LPC1769 with Pn5180 use PHDRIVER_LPC1769PN5180_BOARD.  For
     list of supported boards refer to Platform\DAL\cfg\BoardSelection.h.

  2) To select the osal/os type, go to "Project Properties" --> "C/C++ Build"
     --> "Settings" --> "Preprocessor" --> "Defined symbols (-D)".

     e.g. For using FreeRTOS use PH_OSAL_FREERTOS. For other options refer to
     RTOS\phOsal\inc\phOsal_Config.h.


14 Steps to follow for PI / Linux / CMake
=========================================

  The steps are described in AN11802. (See Section-17, Bullet-5 below)


15 Running NfcrdlibEx7_EMVCo_Polling
====================================

  This application is for displaying the response recieved for PPSE select
  command.


16 List of supported NFC Reader Boards/ICs
==========================================

  1) CLEV6630B 2.0 Customer Evaluation Board
  2) PNEV5180B v2.0 Customer Evaluation Board
  3) PN7462AU v2.1 Customer Evaluation Board


17 Reference Documents
======================

  1) UM10954 : PN5180 SW Quick start guide
     [http://www.nxp.com/docs/en/user-guide/UM10954.pdf]

  2) AN11908 : NFC Reader Library for FRDM-K82F Board Installation guidelines
     [http://www.nxp.com/docs/en/application-note/AN11908.pdf]

  3) AN11022 : CLRC663 Evaluation board quick start guide
     [http://www.nxp.com/docs/en/application-note/AN11022.pdf]

  4) UM10883 : PN7462AU Quick Start Guide - Development Kit
     [http://www.nxp.com/docs/en/user-guide/UM10883.pdf]

  5) AN11802 : NFC Reader Library for Linux Installation Guidelines
     [http://www.nxp.com/docs/en/application-note/AN11802.pdf]


  -----------------------------------------------------------------------------

  For updates of this example, see
  [http://www.nxp.com/products/:NFC-READER-LIBRARY]
