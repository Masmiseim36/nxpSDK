            ______________________________________________________

              NXPNFCRDLIB EXAMPLE : NFCRDLIB_ISO10373_6_PICC_APP
                                  (V06.01.00)
            ______________________________________________________


Table of Contents
_________________

1 Document Purpose
2 Description of the NFCRDLIB_ISO10373_6_PICC_APP
3 Restrictions on NFCRDLIB_ISO10373_6_PICC_APP
4 Configurations of NFCRDLIB_ISO10373_6_PICC_APP
5 Package Contents
6 Mandatory materials (not included)
7 Hardware Configuration
8 Software Configuration
9 Steps to build NFCRDLIB_ISO10373_6_PICC_APP for LPC1769 with PN5180 using MCUXpresso
10 Steps to build NFCRDLIB_ISO10373_6_PICC_APP for PN7462AU using MCUXpresso
11 Steps to build NFCRDLIB_ISO10373_6_PICC_APP for FRDM-K82F with PN5180 using MCUXpresso
12 Selection of Board / OSAL / Frontend (MCUXpresso)
13 Running NFCRDLIB_ISO10373_6_PICC_APP
14 List of supported NFC Reader Boards/ICs
15 Reference Documents





1 Document Purpose
==================

  This document describes the steps to be followed to execute
  NFCRDLIB_ISO10373_6_PICC_APP example as well as it's known problems and
  restrictions.


2 Description of the NFCRDLIB_ISO10373_6_PICC_APP
=================================================

  - NFCRDLIB_ISO10373_6_PICC_APP can be run with PN5180 and PN7462AU.  (See
    Section-14 for supported versions)
  This application is used to perform ISO 10373-6 PICC compliance validation..
  This application has to be executed using DUT which needs to be tested
  against ISO 10373-6 PICC test methods that verifies the compliance to ISO
  14443 protocols.

  NOTE : This application is developed based on Micropross user manual of ISO
  10373-6 PICC Test Suite v1.3.2 which is executed using MP500 TCL3 and is not
  executed with other tool ISO 10373-6 PICC test suite.


3 Restrictions on NFCRDLIB_ISO10373_6_PICC_APP
==============================================

  - NFCRDLIB_ISO10373_6_PICC_APP is restricted to be run on NFC Reader
    Boards/ICs mentioned in Section-14.
  - To execute the ISO PICC test bench using the configurations provided,
    Micropross MP500 TCL3 tool with ISO 10373-6 PICC test suite is required.
  - This Example is intended for NXP contact-less reader ICs only, as such the
    Application is not to be ported to any equivalent device from any other
    vendor.
  - NXP will not support porting to any other vendor platform.
  - This software project requires NxpNfcRdLib v06.01.00 or later.
  - **IMPORTANT** The default used MCU Type is LPC1769.  When executing example
    on FRDMK82 or PN7462AU, the MCU Type has to be changed in the MCUXpresso
    IDE.
  - For switching to MCU Type to PN7462AU, see Section-10
  - For switching to MCU Type to FRDMK82, see Section-11


4 Configurations of NFCRDLIB_ISO10373_6_PICC_APP
================================================

  - This application can be used for running ISO 10373-6 PICC test suite
    mentioned in Section-2.
  - This application has been verified using the MP500 TCL3 Test Tool with
    ISO10373-6 PICC test suite and load the Micropross test bench configuration
    file ISO10373_6_PICC_Test_Bench_Config.mpecc in Micropross PICC test suite.


5 Package Contents
==================

  - Readme.txt
    + This readme file
  - NFCRDLIB_ISO10373_6_PICC_APP.c
    + Main example file.
  - intfs/NFCRDLIB_ISO10373_6_PICC_APP.h
    + Interfaces/Defines specific to the example
  - intfs/ph_NxpBuild_App.h
    + Reader library build configuration file
  - src/phApp_Init.c and intfs/phApp_Init.h
    + Common utility functions (common across all examples)
  - mcux/.cproject and mcux/.project
    + MCUXpresso project configuration file
  - cr_startup_lpc175x_6x.c
    + Startup required to compile example for LPC1769 Micro Controller.
  - ISO10373_6_PICC_Test_Bench_Config.mpecc
    + Micropross ISO PICC test suite configuration file


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
    Section-14 (Note: PN7462AU (Section-14 Bullet-2) does not need any other
    other microcontroller).


7 Hardware Configuration
========================

  Before starting this application, HW Changes may be required for the used
  board.  Refer to the following User Manuals / Application notes before
  starting with this example.

  - AN11744: PN5180 Evaluation board quick start guide


8 Software Configuration
========================

  - The Software can be compiled for Cortex M3 LPC1769 micro-controller, Cortex
    M4 Freedom K82 and Cortex M0 based PN7462AU from NXP.
  - Since this example can be configured to run on various MCU ICs and various
    NFC ICs, appropriate changes are required as mentioned in Section-9,
    Section-10 and Section-11.
  - ISO 10373-6 PICC Test Bench: To run this project with ISO PICC test bench,
    load the configuration file ISO10373_6_PICC_Test_Bench_Config.mpecc in the
    Micropross PICC test suite.


9 Steps to build NFCRDLIB_ISO10373_6_PICC_APP for LPC1769 with PN5180 using MCUXpresso
======================================================================================

  See "Importing provided SW example projects" in AN11908 (See Section-15,
  Bullet-2 below)


10 Steps to build NFCRDLIB_ISO10373_6_PICC_APP for PN7462AU using MCUXpresso
============================================================================

  - For MCUXpressio versions before MCUXpressoIDE_10.0.2, See "Adding PN7462AU
    Plugin" in UM10883. (See Section-15, Bullet-4 below)
  - See "Importing provided SW example projects" in UM10883.


11 Steps to build NFCRDLIB_ISO10373_6_PICC_APP for FRDM-K82F with PN5180 using MCUXpresso
=========================================================================================

  See "Import projects to the MCUXpresso" in AN11908. (See Section-15, Bullet-2
  below)


12 Selection of Board / OSAL / Frontend (MCUXpresso)
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


13 Running NFCRDLIB_ISO10373_6_PICC_APP
=======================================

  The running application can now be used for ISO 10373-6 PICC Test case
  validation.


14 List of supported NFC Reader Boards/ICs
==========================================

  1) PNEV5180B v2.0 Customer Evaluation Board
  2) PNEV7462C Customer Evaluation Board


15 Reference Documents
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
  [http://www.nxp.com/pages/:NFC-READER-LIBRARY]
